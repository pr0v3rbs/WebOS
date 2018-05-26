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


/*****************************************************************************
**
**  Name: ademod_m14_agc.c
**
**  Description:   ABB M14 AGC block.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Line_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Start
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Line_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Start
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
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

#include <asm/div64.h> //do_div

#include "ademod_m14_config.h"


#include "ademod_m14_demod.h"
#include "ademod_m14_regaddress.h"
#include "ademod_m14_agc_dig.h"


/*
**  Backoff LUTs
*/
#define LX_ADEMOD_DIGAGC_BACKOFF_LUT_SIZE		(201)
#define LX_ADEMOD_DIGAGC_HYBRID_TARGET_SIZE	(201)
static UINT32 LX_ADEMOD_DIGAGC_BackoffLUT[LX_ADEMOD_DIGAGC_BACKOFF_LUT_SIZE] = {

/* Backoff(dB)   +0      +0.1      +0.2      +0.3      +0.4      +0.5      +0.6      +0.7      +0.8      +0.9   */
/* --------  --------  --------  --------  --------  --------  --------  --------  --------  --------  -------- */
/* 0 dB  */  16777216, 16585169, 16395320, 16207644, 16022117, 15838713, 15657409, 15478180, 15301003, 15125854,
/* 1 dB  */  14952709, 14781547, 14612344, 14445078, 14279727, 14116268, 13954680, 13794943, 13637033, 13480931,
/* 2 dB  */  13326616, 13174068, 13023266, 12874189, 12726820, 12581137, 12437122, 12294755, 12154019, 12014893,
/* 3 dB  */  11877359, 11741400, 11606998, 11474133, 11342790, 11212950, 11084597, 10957712, 10832280, 10708284,
/* 4 dB  */  10585708, 10464534, 10344748, 10226332, 10109272,  9993552,  9879157,  9766071,  9654280,  9543768,
/* 5 dB  */   9434522,  9326526,  9219766,  9114228,  9009898,  8906763,  8804808,  8704020,  8604386,  8505893,
/* 6 dB  */   8408526,  8312275,  8217125,  8123064,  8030080,  7938161,  7847293,  7757466,  7668667,  7580885,
/* 7 dB  */   7494107,  7408323,  7323520,  7239689,  7156817,  7074893,  6993908,  6913849,  6834707,  6756471,
/* 8 dB  */   6679130,  6602675,  6527094,  6452379,  6378520,  6305505,  6233327,  6161974,  6091439,  6021711,
/* 9 dB  */   5952781,  5884640,  5817279,  5750689,  5684862,  5619788,  5555458,  5491866,  5429001,  5366855,
/* 10 dB */   5305422,  5244691,  5184655,  5125307,  5066638,  5008641,  4951307,  4894630,  4838602,  4783215,
/* 11 dB */   4728462,  4674336,  4620829,  4567935,  4515646,  4463956,  4412857,  4362344,  4312409,  4263045,
/* 12 dB */   4214246,  4166006,  4118318,  4071176,  4024574,  3978505,  3932963,  3887943,  3843438,  3799443,
/* 13 dB */   3755951,  3712957,  3670455,  3628440,  3586905,  3545846,  3505257,  3465133,  3425468,  3386257,
/* 14 dB */   3347495,  3309176,  3271296,  3233850,  3196833,  3160239,  3124064,  3088303,  3052951,  3018005,
/* 15 dB */   2983458,  2949306,  2915546,  2882172,  2849180,  2816566,  2784325,  2752453,  2720946,  2689799,
/* 16 dB */   2659010,  2628572,  2598483,  2568739,  2539334,  2510267,  2481532,  2453126,  2425046,  2397286,
/* 17 dB */   2369845,  2342717,  2315900,  2289391,  2263184,  2237278,  2211668,  2186351,  2161324,  2136584,
/* 18 dB */   2112126,  2087949,  2064048,  2040422,  2017065,  1993976,  1971151,  1948587,  1926282,  1904232,
/* 19 dB */   1882435,  1860887,  1839585,  1818528,  1797711,  1777133,  1756790,  1736680,  1716801,  1697149,
/* 20 dB */   1677722 };

static UINT32 LX_ADEMOD_DIGAGC_BackoffLUT_A2[LX_ADEMOD_DIGAGC_BACKOFF_LUT_SIZE] = { // TBD

/* Backoff(dB)   +0      +0.1      +0.2      +0.3      +0.4      +0.5      +0.6      +0.7      +0.8      +0.9   */
/* --------  --------  --------  --------  --------  --------  --------  --------  --------  --------  -------- */
/* 0 dB  */  16777200, 16585162, 16395302, 16207642, 16022116, 15838702, 15657400, 15478166, 15301000, 15125836,
/* 1 dB  */  14952696, 14781536, 14612334, 14445068, 14279716, 14116256, 13954666, 13794924, 13637030, 13480918,
/* 2 dB  */  13326610, 13174062, 13023252, 12874180, 12726802, 12581118, 12437106, 12294744, 12154010, 12014882,
/* 3 dB  */  11877338, 11741400, 11606980, 11474122, 11342782, 11212938, 11084590, 10957694, 10832272, 10708280,
/* 4 dB  */  10585696, 10464520, 10344730, 10226326, 10109264,  9993544,  9879144,  9766064,  9654260,  9543754,
/* 5 dB  */  9434502,   9326504,  9219760,  9114226,  9009880,  8906744,  8804796,  8704014,  8604376,  8505882,
/* 6 dB  */  8408510,   8312260,  8217110,  8123060,  8030066,  7938150,  7847290,  7757464,  7668650,  7580870,
/* 7 dB  */  7494102,   7408302,  7323514,  7239672,  7156798,  7074892,  6993888,  6913830,  6834696,  6756464,
/* 8 dB  */  6679112,   6602662,  6527092,  6452358,  6378504,  6305486,  6233326,  6161958,  6091426,  6021708,
/* 9 dB  */  5952760,   5884626,  5817262,  5750668,  5684844,  5619768,  5555440,  5491860,  5428984,  5366834,
/* 10 dB */  5305410,   5244690,  5184652,  5125296,  5066622,  5008630,  4951298,  4894626,  4838592,  4783196,
/* 11 dB */  4728460,   4674318,  4620814,  4567926,  4515632,  4463954,  4412848,  4362336,  4312396,  4263028,
/* 12 dB */  4214232,   4165986,  4118312,  4071166,  4024570,  3978502,  3932962,  3887928,  3843422,  3799422,
/* 13 dB */  3755950,   3712940,  3670436,  3628438,  3586902,  3545828,  3505238,  3465132,  3425466,  3386240,
/* 14 dB */  3347476,   3309174,  3271290,  3233846,  3196820,  3160234,  3124044,  3088294,  3052940,  3018004,
/* 15 dB */  2983442,   2949298,  2915528,  2882154,  2849176,  2816550,  2784320,  2752442,  2720938,  2689786,
/* 16 dB */  2659008,   2628560,  2598464,  2568720,  2539328,  2510266,  2481512,  2453110,  2425038,  2397274,
/* 17 dB */  2369840,   2342714,  2315896,  2289386,  2263184,  2237268,  2211660,  2186338,  2161324,  2136574,
/* 18 dB */  2112110,   2087932,  2064040,  2040412,  2017048,  1993970,  1971134,  1948584,  1926276,  1904232,
/* 19 dB */  1882430,   1860870,  1839574,  1818520,  1797708,  1777116,  1756788,  1736680,  1716792,  1697146,
/* 20 dB */  1677720 };

static UINT32 LX_ADEMOD_DIGAGC_HybridTargetLUT[LX_ADEMOD_DIGAGC_HYBRID_TARGET_SIZE] = { // TBD

/* Backoff(dB)   +0        +0.1        +0.2        +0.3        +0.4        +0.5        +0.6        +0.7        +0.8        +0.9   */
/* --------  ----------  ----------  ----------  ----------  ----------  ----------  ----------  ----------  ----------  ---------- */
/* 0 dB  */  0x001F0000, 0x001E001F, 0x001E001F, 0x001E001E, 0x001E001E, 0x001E001E, 0x001E001D, 0x001E001D, 0x001E001C, 0x001E001C,
/* 1 dB  */  0x001E001C, 0x001E001B, 0x001E001B, 0x001E001A, 0x001E001A, 0x001E0019, 0x001E0019, 0x001E0019, 0x001E0018, 0x001E0018,
/* 2 dB  */  0x001E0017, 0x001E0017, 0x001E0016, 0x001E0016, 0x001E0015, 0x001E0015, 0x001E0014, 0x001E0014, 0x001E0013, 0x001E0013,
/* 3 dB  */  0x001E0012, 0x001E0012, 0x001E0011, 0x001E0011, 0x001E0010, 0x001E0010, 0x001E000F, 0x001E000F, 0x001E000E, 0x001E000D,
/* 4 dB  */  0x001E000D, 0x001E000C, 0x001E000C, 0x001E000B, 0x001E000A, 0x001E000A, 0x001E0009, 0x001E0009, 0x001E0008, 0x001E0007,
/* 5 dB  */  0x001E0007, 0x001E0006, 0x001E0005, 0x001E0005, 0x001E0004, 0x001E0003, 0x001E0003, 0x001E0002, 0x001E0001, 0x001E0000,
/* 6 dB  */  0x001E0000, 0x001D001F, 0x001D001E, 0x001D001D, 0x001D001D, 0x001D001C, 0x001D001B, 0x001D001A, 0x001D0019, 0x001D0019,
/* 7 dB  */  0x001D0018, 0x001D0017, 0x001D0016, 0x001D0015, 0x001D0014, 0x001D0014, 0x001D0013, 0x001D0012, 0x001D0011, 0x001D0010,
/* 8 dB  */  0x001D000F, 0x001D000E, 0x001D000D, 0x001D000C, 0x001D000B, 0x001D000A, 0x001D0009, 0x001D0008, 0x001D0007, 0x001D0006,
/* 9 dB  */  0x001D0005, 0x001D0004, 0x001D0003, 0x001D0002, 0x001D0001, 0x001D0000, 0x001C001F, 0x001C001E, 0x001C001D, 0x001C001B,
/* 10 dB */  0x001C001A, 0x001C0019, 0x001C0018, 0x001C0017, 0x001C0016, 0x001C0014, 0x001C0013, 0x001C0012, 0x001C0011, 0x001C000F,
/* 11 dB */  0x001C000E, 0x001C000D, 0x001C000B, 0x001C000A, 0x001C0009, 0x001C0007, 0x001C0006, 0x001C0004, 0x001C0003, 0x001C0002,
/* 12 dB */  0x001C0000, 0x001B001F, 0x001B001D, 0x001B001C, 0x001B001A, 0x001B0019, 0x001B0017, 0x001B0015, 0x001B0014, 0x001B0012,
/* 13 dB */  0x001B0011, 0x001B000F, 0x001B000D, 0x001B000C, 0x001B000A, 0x001B0008, 0x001B0006, 0x001B0005, 0x001B0003, 0x001B0001,
/* 14 dB */  0x001A001F, 0x001A001D, 0x001A001B, 0x001A0019, 0x001A0018, 0x001A0016, 0x001A0014, 0x001A0012, 0x001A0010, 0x001A000E,
/* 15 dB */  0x001A000C, 0x001A0009, 0x001A0007, 0x001A0005, 0x001A0003, 0x001A0001, 0x0019001F, 0x0019001C, 0x0019001A, 0x00190018,
/* 16 dB */  0x00190016, 0x00190013, 0x00190011, 0x0019000E, 0x0019000C, 0x0019000A, 0x00190007, 0x00190005, 0x00190002, 0x00190000,
/* 17 dB */  0x0018001D, 0x0018001A, 0x00180018, 0x00180015, 0x00180012, 0x00180010, 0x0018000D, 0x0018000A, 0x00180007, 0x00180004,
/* 18 dB */  0x00180001, 0x0017001E, 0x0017001B, 0x00170018, 0x00170015, 0x00170012, 0x0017000F, 0x0017000C, 0x00170009, 0x00170006,
/* 19 dB */  0x00170002, 0x0016001F, 0x0016001C, 0x00160018, 0x00160015, 0x00160011, 0x0016000E, 0x0016000A, 0x00160007, 0x00160003,
/* 20 dB */  0x00160000 };


#define LX_ADEMOD_DIGAGC_GAIN_LUT_SIZE	(121)
static UINT32 LX_ADEMOD_DIGAGC_GainLUT[LX_ADEMOD_DIGAGC_GAIN_LUT_SIZE] = {

/* Dig_gain(dB)  +0.0      +0.5      +1.0      +1.5      +2.0      +2.5      +3.0      +3.5      +4.0      +4.5   */
/* --------  --------  --------  --------  --------  --------  --------  --------  --------  --------  -------- */
/* 0 dB   */	1024,	  1085,		1149,	  1217,		1289,	  1366,		1446,	  1532,		1623,	  1719,
/* 5 dB   */	1821,	  1929,		2043,	  2164,		2292,	  2428,		2572,	  2725,		2886,	  3057,
/* 10 dB  */	3238,	  3430,		3633,	  3849,		4077,	  4318,		4574,	  4845,		5132,	  5436,
/* 15 dB  */	5758,	  6100,		6461,	  6844,		7249,	  7679,		8134,	  8616,		9126,	  9667,
/* 20 dB  */   10240,	 10847,	   11489,	 12170,	   12891,	 13655,	   14464,	 15321,	   16229,	 17191,
/* 25 dB  */   18210,	 19289,	   20431,	 21642,	   22925,	 24283,	   25722,	 27246,	   28860,	 30570,
/* 30 dB  */   32382,	 34300,	   36333,	 38486,	   40766,	 43182,	   45740,	 48451,	   51322,	 54363,
/* 35 dB  */   57584,	 60996,	   64610,	 68438,	   72494,	 76789,	   81339,	 86159,	   91264,	 96672,
/* 40 dB  */  102400,	108468,	  114895,	121703,	  128914,	136553,   144644,   153215,   162293,   171910,
/* 45 dB  */  182096,   192886,   204315,   216421,   229245,   242829,   257217,   272458,   288602,   305703,
/* 50 dB  */  323817,   343005,   363329,   384858,   407662,   431817,   457404,   484507,   513216,   543626,
/* 55 dB  */  575838,   609958,   646100,   684384,   724936,   767892,   813392,   861589,   912641,   966718,
/* 60 dB  */ 1024000 };

/*
**  Local Function Prototypes - not available for external access.
*/


static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPF(pLX_ADEMOD_DigAgc pContext, UINT32 LPF_BW, UINT32* pLPF_Shift1, UINT32* pLPF_Shift2);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pLX_ADEMOD_DigAgc pContext, UINT32* pLPFGain, UINT32* pLPFGain_Shift);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LoopGain(pLX_ADEMOD_DigAgc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF,
									   UINT32 resp_time_poserr_Dig,
									   UINT32 resp_time_negerr_Dig);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LoopGainCoherent(pLX_ADEMOD_DigAgc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF,
									   UINT32 resp_time_poserr_Dig,
									   UINT32 resp_time_negerr_Dig);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LoopGainHybrid(pLX_ADEMOD_DigAgc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF,
									   UINT32 resp_time_poserr_Dig,
									   UINT32 resp_time_negerr_Dig);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BackOff(pLX_ADEMOD_DigAgc pContext, int BackOff_dB, UINT32* pAGC_ref);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Target(pLX_ADEMOD_DigAgc pContext, int BackOff_dB, UINT32* pAGC_target);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Attack(pLX_ADEMOD_DigAgc pContext, UINT32 Attack);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Decay(pLX_ADEMOD_DigAgc pContext, UINT32 Decay_us);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_FastDecay(pLX_ADEMOD_DigAgc pContext, UINT32 DecayTimeOut_us);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_RFGainThrsh_Set(pLX_ADEMOD_DigAgc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_IFGainThrsh_Set(pLX_ADEMOD_DigAgc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_DigGainThrsh_Set(pLX_ADEMOD_DigAgc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_RFGainPreset_Set(pLX_ADEMOD_DigAgc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_IFGainPreset_Set(pLX_ADEMOD_DigAgc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_DigGainPreset_Set(pLX_ADEMOD_DigAgc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Reset(pLX_ADEMOD_DigAgc pContext, BOOL on_off, Handle_t hUser);
/*
**  API implementation.
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Factory_Set
**
**  Description:    Initializes AGC context with factory settings.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Factory_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// Digital AGC
	pContext->rf_mode = LX_ADEMOD_AGC_TOP_MODE_FCT;
	pContext->if_mode = LX_ADEMOD_AGC_TOP_MODE_FCT;
	pContext->dig_mode = LX_ADEMOD_AGC_TOP_MODE_FCT;
	pContext->dig_mode_nbnd = LX_ADEMOD_AGC_TOP_MODE_FCT;
	pContext->cvbs_mode = LX_ADEMOD_DIGAGC_CVBS_MODE_NONCOHERENT;
	pContext->decay_thr = LX_ADEMOD_DIGAGC_DECAYTHRSH_NOMINAL_FCT;
	pContext->leaky_pd_attack = LX_ADEMOD_DIGAGC_ATTACK_FAST_FCT;
	pContext->leaky_pd_decay = LX_ADEMOD_DIGAGC_DECAY_SLOW_FCT;
	pContext->leaky_pd_decaywin = LX_ADEMOD_DIGAGC_DECAYWIN_SHORT_FCT;
	pContext->fast_decay = LX_ADEMOD_DIGAGC_FASTDECAY_SLOW_FCT;
	pContext->lpf_bw = LX_ADEMOD_DIGAGC_RFBNDW_NARROW_FCT;
	pContext->lpf_bw_coh = LX_ADEMOD_DIGAGC_RFBNDW_NARROW_FCT;
	pContext->backoff = LX_ADEMOD_DIGAGC_BACKOFF_HIGH_FCT;
	pContext->backoff_coh = LX_ADEMOD_DIGAGC_BACKOFF_COH_HIGH_FCT;
	pContext->dig_gain = LX_ADEMOD_PRESET_OPTION_MIN;
	pContext->if_gain = LX_ADEMOD_PRESET_OPTION_MIN;
	pContext->rf_gain = LX_ADEMOD_PRESET_OPTION_MIN;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Flags_Set
**
**  Description:    Sets AGC flags.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Flags        - The flags' bitmap,
**                  mask         - Indicates, which flags where changed.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Set(pLX_ADEMOD_DigAgc pContext, UINT32 Flags, UINT32 mask)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_MODTYPE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		if (mask & LX_ADEMOD_UFLAG_DIGAGC_MODTYPE_USR) {
			pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_MODTYPE_USR;
		}
	}

	pContext->flags = Flags;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Flags_Get
**
**  Description:    Returns AGC flags.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Get(pLX_ADEMOD_DigAgc pContext, UINT32* pFlags)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pFlags = pContext->flags;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_MixedMode_Set
**
**  Description:    Sets AGC mixed mode.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Mode         - 0- wideband mode,
**								   1- mixed mode,
**								   2- narrowband mode,
**								   3- hybrid mode.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_MixedMode_Set(pLX_ADEMOD_DigAgc pContext, UINT32 Mode, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_MIXMODE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_MIXMODE_USR;
	}

	if (Mode == LX_ADEMOD_AGC_LPF_MODE_WIDEBAND)
	{
		pContext->flags &= ~LX_ADEMOD_FFLAG_AGC_POSTSAW_EN;
		if (Update) {
			ADEMOD_WriteLong( hUser, DIGAGC_MAXGAIN, pContext->max_gain_dig, 5);
			ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN, pContext->min_gain_dig, 5);
			ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN, pContext->pres_gain_dig, 5);
		}
	}
	else
	{ // mix/narrowband/hybrid
		pContext->flags |= LX_ADEMOD_FFLAG_AGC_POSTSAW_EN;

		if (Update) {
			ADEMOD_WriteLong( hUser, DIGAGC_MAXGAIN, pContext->max_gain_dig_nbnd, 5);
			ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN, pContext->min_gain_dig_nbnd, 5);
			ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN, pContext->pres_gain_dig_nbnd, 5);
		}
	}

	if (Mode == LX_ADEMOD_AGC_LPF_MODE_HYBRID)
	{
		pContext->flags |= LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN;
	}
	else
	{
		pContext->flags &= ~LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN;
	}
	return status;
}



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_Mode_Set
**
**  Description:    Sets input mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  InputMode    - The input mode.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**  Note:           The positive/negative demodulation mode used in this function
**                  can be affected by DEMOD_M14_Analog_AGC_Standard_Set. Therefore it is a good idea
**                  to call DEMOD_M14_Analog_AGC_Standard_Set prior to this function.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_Mode_Set(pLX_ADEMOD_DigAgc pContext, UINT32 InputMode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (InputMode)
	{
		case LX_ADEMOD_INPUTMODE_LEGACY:
			{
			UINT32 flags = LX_ADEMOD_DIGAGC_LEG_CTRL_FLAGS_FCT; // factory flags for Legacy mode
			// set default flags if they were not affected by the user.
			// Don't touch negative/positive modulation since it has
			// already been set by DEMOD_M14_Analog_AGC_Standard_Set.
			UINT32 mask = LX_ADEMOD_FFLAG_DIGAGCMOD_TYPE | LX_ADEMOD_FFLAG_AGC_POSTSAW_EN | LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN | LX_ADEMOD_FFLAG_DIGAGC_VSYNCTILT_EN | LX_ADEMOD_FFLAG_DIGAGC_LVLDEP_EN;
			flags &= ~mask;
			pContext->flags &= mask;
			pContext->flags |= flags;
			}

		break;

		case LX_ADEMOD_INPUTMODE_DVBT:
			// Low IF
			{
			UINT32 flags = LX_ADEMOD_DIGAGC_LOWIF_CTRL_FLAGS_FCT; // factory flags for Low IF mode
			// set default flags if they were not affected by the user.
			UINT32 mask = 0;
			flags &= ~mask;
			pContext->flags &= mask;
			pContext->flags |= flags;
			pContext->REF = -15 * 100;
			}
		break;

		case LX_ADEMOD_INPUTMODE_ATSC:
			// not supported yet
		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  VideoStandard  - The video standard.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
**  Note:           Parameter values set by the user preside over the settings
**                  derived from video standard. Therefore if FmLg_Parameter_Set
**                  function was called to set appropriate parameters prior to
**                  Standard_Set then the values given to the registers will be
**                  those defined by FmLg_Parameter_Set
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Standard_Set(pLX_ADEMOD_DigAgc pContext, UINT32 VideoStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (VideoStandard)
	{
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_433:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_50:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_G:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_H:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_K:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_M:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_N:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_60:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_NC:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_B:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_G:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_H:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_D:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K1:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
			// set negative modulation
			if (!(pContext->update & LX_ADEMOD_UFLAG_DIGAGC_MODTYPE_USR)) {
				pContext->flags &= ~LX_ADEMOD_FFLAG_DIGAGCMOD_TYPE;
			}
			if (!(pContext->update & LX_ADEMOD_UFLAG_DIGAGC_COHERENT_MODE_USR)) {
				pContext->cvbs_mode = LX_ADEMOD_DIGAGC_CVBS_MODE_NONCOHERENT;
			}
			pContext->REF = -1 * 100;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
			// set positive modulation
			if (!(pContext->update & LX_ADEMOD_UFLAG_DIGAGC_MODTYPE_USR)) {
				pContext->flags |= LX_ADEMOD_FFLAG_DIGAGCMOD_TYPE;
			}
			if (!(pContext->update & LX_ADEMOD_UFLAG_DIGAGC_COHERENT_MODE_USR)) {
				pContext->cvbs_mode = LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT;
			}
			pContext->REF = -15 * 100;
		break;

		default:
			status = retErrBadParam;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Init
**
**  Description:    Initializes AGC hardware.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Init(pLX_ADEMOD_DigAgc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Start
**
**  Description:    Starts AGC block.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Start(pLX_ADEMOD_DigAgc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	status  = DEMOD_M14_Analog_DigAGC_Reset(pContext, TRUE, hUser);

	if (pContext->flags & LX_ADEMOD_FFLAG_AGC_POSTSAW_EN)
	{
		ADEMOD_WriteLong( hUser, DIGAGC_MAXGAIN, pContext->max_gain_dig_nbnd, 5);
		ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN, pContext->min_gain_dig_nbnd, 5);
		ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN, pContext->pres_gain_dig_nbnd, 5);
	}
	else
	{
		ADEMOD_WriteLong( hUser, DIGAGC_MAXGAIN, pContext->max_gain_dig, 5);
		ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN, pContext->min_gain_dig, 5);
		ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN, pContext->pres_gain_dig, 5);
	}
	ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN_RF, pContext->min_gain_rf, 5);
	ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN_IF, pContext->min_gain_if, 5);

	ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN_RF, pContext->pres_gain_rf, 5);
	ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN_IF, pContext->pres_gain_if, 5);

	//// CVBS Mode: Non-coherent or Coherent Mode
	ADEMOD_WriteSub( hUser, DIGAGC_COHMODEN, (pContext->cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT ||
										  pContext->cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT_FORCED) ? 1 : 0, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_COHMODFORCE, (pContext->cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT_FORCED) ? 1 : 0, 1);

	//// Leaky Peak Detector speed
	ADEMOD_WriteSub( hUser, DIGAGC_ATTACK_1, -pContext->attack_1, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_ATTACK_2, -pContext->attack_2, 1);

	//// Leaky Peak Detector Decay
	ADEMOD_WriteSub( hUser, DIGAGC_DECAY,            pContext->Leaky_Decay, 4);
	ADEMOD_WriteSub( hUser, DIGAGC_PRESETDECAYCONST, pContext->Leaky_Decay, 4);

	//// Leaky Peak Detector Fast Decay
	ADEMOD_WriteSub( hUser, DIGAGC_FASTDECAY, pContext->fast_decay, 1);

	//// Leaky Peak Detector Fast Decay window
	ADEMOD_WriteSub( hUser, DIGAGC_DECAYWIN, pContext->win, 3);

	//// Leaky Peak Detector Decay Threshold
	ADEMOD_WriteSub( hUser, DIGAGC_FASTDECAYTHRESH, pContext->decay_thr, 4);

	//// DigAGC bandwidth (non-coherent)
	ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_NONCOH_1, pContext->LPF_Shift1, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_NONCOH_2, pContext->LPF_Shift2, 1);

	////DigAGC speed (non-coherent)
	if (!(pContext->flags & LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN)) {
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_NONCOH_1, pContext->LPFGain_poserr_RF, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_NONCOH_2, pContext->LPFGain_poserr_RF_Shift, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_NONCOH_1, pContext->LPFGain_negerr_RF, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_NONCOH_2, pContext->LPFGain_negerr_RF_Shift, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_1, pContext->LPFGain_poserr_IF, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_2, pContext->LPFGain_poserr_IF_Shift, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_NONCOH_1, pContext->LPFGain_negerr_IF, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_NONCOH_2, pContext->LPFGain_negerr_IF_Shift, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_1,    pContext->LPFGain_poserr_Dig, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_2,    pContext->LPFGain_poserr_Dig_Shift, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_1,    pContext->LPFGain_negerr_Dig, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_2,    pContext->LPFGain_negerr_Dig_Shift, 1);
	}

	//// DigAGC bandwidth (coherent) in mHz
	ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_COH_1, pContext->LPF_Shift1_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_COH_2, pContext->LPF_Shift2_coh, 1);

	//// DigAGC speed (coherent)
	ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_COH_1,    pContext->LPFGain_poserr_RF_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_COH_2,    pContext->LPFGain_poserr_RF_Shift_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_COH_1,    pContext->LPFGain_negerr_RF_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_COH_2,    pContext->LPFGain_negerr_RF_Shift_coh, 1);
	if (!(pContext->flags & LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN)) {
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_COH_1,    pContext->LPFGain_poserr_IF_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_COH_2,    pContext->LPFGain_poserr_IF_Shift_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_COH_1,    pContext->LPFGain_negerr_IF_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_COH_2,    pContext->LPFGain_negerr_IF_Shift_coh, 1);
	}
	ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_COH_1,       pContext->LPFGain_poserr_Dig_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_COH_2,       pContext->LPFGain_poserr_Dig_Shift_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_COH_1,       pContext->LPFGain_negerr_Dig_coh, 1);
	ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_COH_2,       pContext->LPFGain_negerr_Dig_Shift_coh, 1);

	//// DigAGC backoff (non-coherent)
	if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
		UINT32 ref = pContext->ref & 0xFFFFF800; // mask 11 bits
		UINT32 DigADC_Ref_NoCoh = (UINT32)(
			((pContext->narrowband_thr2 & 0x1F)	<< 0) +
			((pContext->narrowband_thr1 & 0x3F)	<< 5) +
			(ref)
		);
		ADEMOD_WriteSub( hUser, DIGAGC_REF_NONCOH, DigADC_Ref_NoCoh, 4);
	}
	else {
		ADEMOD_WriteSub( hUser, DIGAGC_REF_NONCOH, pContext->ref, 4);
	}

	//// DigAGC backoff (coherent)
	ADEMOD_WriteSub( hUser, DIGAGC_REF_COH, pContext->ref_coh, 4);

	// hybrid mode
	if (((pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) && (pContext->flags & LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN)) {
		UINT32 AGC_target = 0;
		// program TOP
		status = DEMOD_M14_Analog_DigAGC_Target(pContext, pContext->target, &AGC_target);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_COH_1,    0x1, 1); // program sign bit
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_1, (AGC_target >> 16) & 0xFFFF, 1); // integer bits
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_2, AGC_target & 0xFFFF, 1); // fractional bits
		// program decimation ratio
		ADEMOD_WriteSub( hUser, SLOW_MODE_AGC_CNT_TH,    pContext->decimation_ratio, 2);
		// digital RF loop speed
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_GAIN_AGC_ECO,    pContext->LPFGain_poserr_RF_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_SHIFT_AGC_ECO,   pContext->LPFGain_poserr_RF_Shift_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_1,    pContext->LPFGain_poserr_Dig_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_2,    pContext->LPFGain_poserr_Dig_Shift_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_1,    pContext->LPFGain_negerr_Dig_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_2,    pContext->LPFGain_negerr_Dig_Shift_hybr, 1);

	}

	status  = DEMOD_M14_Analog_DigAGC_Reset(pContext, FALSE, hUser);

	// clear update flag
	pContext->update &= ~LX_ADEMOD_UFLAG_UPDATE;

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_GainThrsh_Set
**
**  Description:    Sets AGC min/max gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF/Digital,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit min threshold (LSB), when mode is "none",
**						      pValue[3] - explicit min threshold (MSB), when mode is "none",
**						      pValue[4] - explicit max threshold (LSB), when mode is "none".
**						      pValue[5] - explicit max threshold (MSB), when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_GainThrsh_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_RFGAIN_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_RFGAIN_THR_USR;
			}
			pContext->rf_mode = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->min_gain_rf = pValue[2];
				pContext->min_gain_rf += (UINT64)pValue[3] << 32;
				pContext->max_gain_rf = pValue[4];
				pContext->max_gain_rf += (UINT64)pValue[5] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_RFGainThrsh_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN_RF, pContext->min_gain_rf * (INT64)(1 << 22), 5);
			}
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_IFGAIN_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_IFGAIN_THR_USR;
			}
			pContext->if_mode = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->min_gain_if = pValue[2];
				pContext->min_gain_if += (UINT64)pValue[3] << 32;
				pContext->max_gain_if = pValue[4] & 0xffffffff;
				pContext->max_gain_if += (UINT64)pValue[5] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_IFGainThrsh_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN_IF, pContext->min_gain_if * (INT64)(1 << 22), 5);
			}
			break;

		case LX_ADEMOD_AGC_MODE_DIG:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_THR_USR;
			}
			pContext->dig_mode = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->min_gain_dig = pValue[2];
				pContext->min_gain_dig += (UINT64)pValue[3] << 32;
				pContext->max_gain_dig = pValue[4]& 0xffffffff;;
				pContext->max_gain_dig += (UINT64)pValue[5] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_DigGainThrsh_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_MAXGAIN, pContext->max_gain_dig, 5);
				ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN, pContext->min_gain_dig, 5);
			}
			break;

		case LX_ADEMOD_AGC_MODE_DIG_NBND:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_THR_USR;
			}
			pContext->dig_mode_nbnd = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->min_gain_dig_nbnd = pValue[2];
				pContext->min_gain_dig_nbnd += (UINT64)pValue[3] << 32;
				pContext->max_gain_dig_nbnd = pValue[4];
				pContext->max_gain_dig_nbnd += (UINT64)pValue[5] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_DigGainThrsh_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_MAXGAIN, pContext->max_gain_dig_nbnd, 5);
				ADEMOD_WriteLong( hUser, DIGAGC_MINGAIN, pContext->min_gain_dig_nbnd, 5);
			}
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_GainThrsh_Get
**
**  Description:    Returns AGC min/max gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF/Digital,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit min threshold (LSB), when mode is "none",
**						      pValue[3] - explicit min threshold (MSB), when mode is "none",
**						      pValue[4] - explicit max threshold (LSB), when mode is "none".
**						      pValue[5] - explicit max threshold (MSB), when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_GainThrsh_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			pValue[1] =  pContext->rf_mode;
			pValue[2] =	(UINT32)(pContext->min_gain_rf & 0xffffffff);
			pValue[3] =	(UINT32)((pContext->min_gain_rf >> 32) & 0xffffffff);
			pValue[4] =	(UINT32)(pContext->max_gain_rf & 0xffffffff);
			pValue[5] =	(UINT32)((pContext->max_gain_rf >> 32) & 0xffffffff);
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			pValue[1] =  pContext->if_mode;
			pValue[2] =	(UINT32)(pContext->min_gain_if & 0xffffffff);
			pValue[3] =	(UINT32)((pContext->min_gain_if >> 32) & 0xffffffff);
			pValue[4] =	(UINT32)(pContext->max_gain_if & 0xffffffff);
			pValue[5] =	(UINT32)((pContext->max_gain_if >> 32) & 0xffffffff);
			break;

		case LX_ADEMOD_AGC_MODE_DIG:
			pValue[1] =  pContext->dig_mode;
			pValue[2] =	(UINT32)(pContext->min_gain_dig & 0xffffffff);
			pValue[3] =	(UINT32)((pContext->min_gain_dig >> 32) & 0xffffffff);
			pValue[4] =	(UINT32)(pContext->max_gain_dig & 0xffffffff);
			pValue[5] =	(UINT32)((pContext->max_gain_dig >> 32) & 0xffffffff);
			break;

		case LX_ADEMOD_AGC_MODE_DIG_NBND:
			pValue[1] =  pContext->dig_mode_nbnd;
			pValue[2] =	(UINT32)(pContext->min_gain_dig_nbnd & 0xffffffff);
			pValue[3] =	(UINT32)((pContext->min_gain_dig_nbnd >> 32) & 0xffffffff);
			pValue[4] =	(UINT32)(pContext->max_gain_dig_nbnd & 0xffffffff);
			pValue[5] =	(UINT32)((pContext->max_gain_dig_nbnd >> 32) & 0xffffffff);
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_GainPreset_Set
**
**  Description:    Sets Digital AGC preset gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF/Digital,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - LSB explicit preset gain, when mode is "none",
**						      pValue[3] - MSB explicit preset gain, when mode is "none",
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_GainPreset_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_RFGAIN_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_RFGAIN_USR;
			}
			pContext->rf_gain = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->pres_gain_rf =  pValue[2];
				pContext->pres_gain_rf += (UINT64)pValue[3] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_RFGainPreset_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN_RF, pContext->pres_gain_rf, 5);
			}
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_IFGAIN_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_IFGAIN_USR;
			}
			pContext->if_gain = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->pres_gain_if =  pValue[2];
				pContext->pres_gain_if += (UINT64)pValue[3] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_IFGainPreset_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN_IF, pContext->pres_gain_if, 5);
			}
			break;

		case LX_ADEMOD_AGC_MODE_DIG:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_USR;
			}
			pContext->dig_gain = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->pres_gain_dig =  pValue[2];
				pContext->pres_gain_dig += (UINT64)pValue[3] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_DigGainPreset_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN,    pContext->pres_gain_dig, 5);
			}
			break;

		case LX_ADEMOD_AGC_MODE_DIG_NBND:
			if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_USR;
			}
			pContext->dig_gain_nbnd = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->pres_gain_dig_nbnd =  pValue[2];
				pContext->pres_gain_dig_nbnd += (UINT64)pValue[3] << 32;
			}
			else {
				status = DEMOD_M14_Analog_DigAGC_DigGainPreset_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteLong( hUser, DIGAGC_PRESETGAIN,    pContext->pres_gain_dig_nbnd, 5);
			}
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_CVBS_Mode_Set
**
**  Description:    Sets Digital AGC CVBS.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - CVBS mode: 1- non-coherent/2 -coherent (not forced)/3 - coherent (forced),
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_CVBS_Mode_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_COHERENT_MODE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_COHERENT_MODE_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_DIGAGC_CVBS_MODE_NONCOHERENT:
			break;

		case LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT:
			break;

		case LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT_FORCED:
			break;

		default:
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		pContext->cvbs_mode = pValue[0];
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_COHMODEN, (pContext->cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT ||
											  pContext->cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT_FORCED) ? 1 : 0, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_COHMODFORCE, (pContext->cvbs_mode == LX_ADEMOD_DIGAGC_CVBS_MODE_COHERENT_FORCED) ? 1 : 0, 1);
	}


	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_CVBS_Mode_Get
**
**  Description:    Returns Digital AGC CVBS mode (coherent/no coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - CVBS mode: 1- non-coherent/2 -coherent (not forced)/3 - coherent (forced),
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_CVBS_Mode_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] =  (UINT32)(pContext->cvbs_mode);

	return status;
}



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_Speed_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector speed.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual speed, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_Speed_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPD_SPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPD_SPEED_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->leaky_pd_attack = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->leaky_pd_attack = LX_ADEMOD_DIGAGC_ATTACK_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->leaky_pd_attack = LX_ADEMOD_DIGAGC_ATTACK_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->leaky_pd_attack = LX_ADEMOD_DIGAGC_ATTACK_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->leaky_pd_attack = LX_ADEMOD_DIGAGC_ATTACK_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_Attack(pContext, pContext->leaky_pd_attack);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_ATTACK_1, -pContext->attack_1, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_ATTACK_2, -pContext->attack_2, 1);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_Decay_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector's decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_Decay_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAY_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAY_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->leaky_pd_decay = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->leaky_pd_decay = LX_ADEMOD_DIGAGC_DECAY_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->leaky_pd_decay = LX_ADEMOD_DIGAGC_DECAY_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->leaky_pd_decay = LX_ADEMOD_DIGAGC_DECAY_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->leaky_pd_decay = LX_ADEMOD_DIGAGC_DECAY_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_Decay(pContext, pContext->leaky_pd_decay);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_DECAY,            pContext->Leaky_Decay * (UINT32)(1 << 27), 4);
		ADEMOD_WriteSub( hUser, DIGAGC_PRESETDECAYCONST, pContext->Leaky_Decay * (UINT32)(1 << 27), 4);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_FastDecay_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector's fast decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_FastDecay_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPD_FASTDECAY_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPD_FASTDECAY_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->fast_decay = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->fast_decay = LX_ADEMOD_DIGAGC_FASTDECAY_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->fast_decay = LX_ADEMOD_DIGAGC_FASTDECAY_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->fast_decay = LX_ADEMOD_DIGAGC_FASTDECAY_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->fast_decay = LX_ADEMOD_DIGAGC_FASTDECAY_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_FASTDECAY, pContext->fast_decay, 1);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_DecayWindow_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector decay window.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/short/long/max/none,
**						      pValue[1] - actual window, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_DecayWindow_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAYWIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAYWIN_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->leaky_pd_decaywin = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->leaky_pd_decaywin = LX_ADEMOD_DIGAGC_DECAYWIN_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->leaky_pd_decaywin = LX_ADEMOD_DIGAGC_DECAYWIN_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SHORT:
			pContext->leaky_pd_decaywin = LX_ADEMOD_DIGAGC_DECAYWIN_SHORT_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LONG:
			pContext->leaky_pd_decaywin = LX_ADEMOD_DIGAGC_DECAYWIN_LONG_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_FastDecay(pContext, pContext->leaky_pd_decaywin);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_DECAYWIN, pContext->win, 3);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_DecayThresh_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector's decay threshold.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_DecayThresh_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAYTHRSH_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAYTHRSH_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->decay_thr = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->decay_thr = LX_ADEMOD_DIGAGC_DECAYTHRSH_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->decay_thr = LX_ADEMOD_DIGAGC_DECAYTHRSH_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->decay_thr = LX_ADEMOD_DIGAGC_DECAYTHRSH_NOMINAL_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_FASTDECAYTHRESH, pContext->decay_thr * (UINT32)(1 << 27), 4);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Bndw_Set
**
**  Description:    Sets Digital AGC non-coherent bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/narrow/wide/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Bndw_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int LPF_Shift1 = 0;
	int LPF_Shift2 = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPFBNDW_USR) {
		// when user sticky bit is raised, don't reset to factory
		return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPFBNDW_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->lpf_bw = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->lpf_bw = LX_ADEMOD_DIGAGC_RFBNDW_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->lpf_bw = LX_ADEMOD_DIGAGC_RFBNDW_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NARROW:
			pContext->lpf_bw = LX_ADEMOD_DIGAGC_RFBNDW_NARROW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_WIDE:
			pContext->lpf_bw = LX_ADEMOD_DIGAGC_RFBNDW_WIDE_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_LPF(pContext, pContext->lpf_bw, (UINT32 *)&LPF_Shift1, (UINT32 *)&LPF_Shift2);

		pContext->LPF_Shift1 = (INT16)(-LPF_Shift1);
		pContext->LPF_Shift2 = (INT16)(-LPF_Shift2);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_NONCOH_1, pContext->LPF_Shift1, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_NONCOH_2, pContext->LPF_Shift2, 1);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Speed_Set
**
**  Description:    Sets Digital AGC speed (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Speed_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	BOOL ResetRequired = FALSE;
	int ChangeSpeedBy_RF = 0;
	int ChangeSpeedBy_IF = 0;
	int ChangeSpeedBy_Dig = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_USR;
	}


	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->resp_time_poserr_RF =  pValue[1];
			pContext->resp_time_negerr_RF =  pValue[2];
			pContext->resp_time_poserr_IF =  pValue[3];
			pContext->resp_time_negerr_IF =  pValue[4];
			pContext->resp_time_poserr_Dig = pValue[5];
			pContext->resp_time_negerr_Dig = pValue[6];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->resp_time_poserr_RF =  (UINT16)5000000;
			pContext->resp_time_negerr_RF =  (UINT16)5000000;
			pContext->resp_time_poserr_IF =  (UINT16)5000000;
			pContext->resp_time_negerr_IF =  (UINT16)5000000;
			pContext->resp_time_poserr_Dig = (UINT16)5000000;
			pContext->resp_time_negerr_Dig = (UINT16)5000000;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->resp_time_poserr_RF =  100;
			pContext->resp_time_negerr_RF =  100;
			pContext->resp_time_poserr_IF =  100;
			pContext->resp_time_negerr_IF =  100;
			pContext->resp_time_poserr_Dig = 100;
			pContext->resp_time_negerr_Dig = 100;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->resp_time_poserr_RF =  (UINT16)300000;
			pContext->resp_time_negerr_RF =  (UINT16)300000;
			pContext->resp_time_poserr_IF =  (UINT16)300000;
			pContext->resp_time_negerr_IF =  (UINT16)300000;
			pContext->resp_time_poserr_Dig = (UINT16)300000;
			pContext->resp_time_negerr_Dig = (UINT16)300000;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->resp_time_poserr_RF =  523;
			pContext->resp_time_negerr_RF =  1046;
			pContext->resp_time_poserr_IF =  500;
			pContext->resp_time_negerr_IF =  1000;
			pContext->resp_time_poserr_Dig = 600;
			pContext->resp_time_negerr_Dig = 1400;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOWER:
			ChangeSpeedBy_RF += (int)pValue[1];
			ChangeSpeedBy_IF += (int)pValue[2];
			ChangeSpeedBy_Dig += (int)pValue[3];
			if (pValue[4] == 1) {
				ResetRequired = TRUE;
			}
		break;

		case LX_ADEMOD_PRESET_OPTION_FASTER:
			ChangeSpeedBy_RF -= (int)pValue[1];
			ChangeSpeedBy_IF -= (int)pValue[2];
			ChangeSpeedBy_Dig -= (int)pValue[3];
			if (pValue[4] == 1) {
				ResetRequired = TRUE;
			}
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if ((pValue[0] == LX_ADEMOD_PRESET_OPTION_SLOWER) ||
		(pValue[0] == LX_ADEMOD_PRESET_OPTION_FASTER)) {
		if (Update) {
			ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_NONCOH_2, pContext->LPFGain_poserr_RF_Shift + ChangeSpeedBy_RF, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_NONCOH_2, pContext->LPFGain_negerr_RF_Shift + ChangeSpeedBy_RF, 1);
			//ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_2, pContext->LPFGain_poserr_IF_Shift + ChangeSpeedBy_IF, 1);
			//ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_NONCOH_2, pContext->LPFGain_negerr_IF_Shift + ChangeSpeedBy_IF, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_2,    pContext->LPFGain_poserr_Dig_Shift + ChangeSpeedBy_Dig, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_2,    pContext->LPFGain_negerr_Dig_Shift + ChangeSpeedBy_Dig, 1);
			if (ResetRequired) {
				ADEMOD_WriteSub( hUser, SOFTRESETN0,    0xd0, 1);
				ADEMOD_WriteSub( hUser, SOFTRESETN0,    0xff, 1);
			}
		}
		else {
			// These options should be always used as a run-time one, i.e. with Update = TRUE;
			// For changing digital AGC speed in a sequence when Demodulate is called last, use
			// other options, such as "SLOW", "FAST" or "NONE" (for direct values).
			// When called with Update = FALSE options "SLOWER" and "FASTER" will not have any effect on the system.
			status = retErrBadParam;
		}
	}
	else {

		if (status == retOK) {
			pContext->speed = pValue[0];
			status = DEMOD_M14_Analog_DigAGC_LoopGain(pContext, pContext->resp_time_poserr_RF,
													  pContext->resp_time_negerr_RF,
													  pContext->resp_time_poserr_IF,
													  pContext->resp_time_negerr_IF,
													  pContext->resp_time_poserr_Dig,
													  pContext->resp_time_negerr_Dig);
		}


		if (Update) {
			ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_NONCOH_1, pContext->LPFGain_poserr_RF, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_NONCOH_2, pContext->LPFGain_poserr_RF_Shift, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_NONCOH_1, pContext->LPFGain_negerr_RF, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_NONCOH_2, pContext->LPFGain_negerr_RF_Shift, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_1, pContext->LPFGain_poserr_IF, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_2, pContext->LPFGain_poserr_IF_Shift, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_NONCOH_1, pContext->LPFGain_negerr_IF, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_NONCOH_2, pContext->LPFGain_negerr_IF_Shift, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_1,    pContext->LPFGain_poserr_Dig, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_2,    pContext->LPFGain_poserr_Dig_Shift, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_1,    pContext->LPFGain_negerr_Dig, 1);
			ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_2,    pContext->LPFGain_negerr_Dig_Shift, 1);
		}
	}
	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Speed_Get
**
**  Description:    Returns Digital AGC speed (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Speed_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] =  (UINT32)(pContext->speed);
	pValue[1] =  (UINT32)(pContext->resp_time_poserr_RF);
	pValue[2] =  (UINT32)(pContext->resp_time_negerr_RF);
	pValue[3] =  (UINT32)(pContext->resp_time_poserr_IF);
	pValue[4] =  (UINT32)(pContext->resp_time_negerr_IF);
	pValue[5] =  (UINT32)(pContext->resp_time_poserr_Dig);
	pValue[6] =  (UINT32)(pContext->resp_time_negerr_Dig);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedCoherent_Get
**
**  Description:    Returns Digital AGC speed (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedCoherent_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] =  (UINT32)(pContext->speed_coherent);
	pValue[1] =  (UINT32)(pContext->resp_time_poserr_RF_coh);
	pValue[2] =  (UINT32)(pContext->resp_time_negerr_RF_coh);
	pValue[3] =  (UINT32)(pContext->resp_time_poserr_IF_coh);
	pValue[4] =  (UINT32)(pContext->resp_time_negerr_IF_coh);
	pValue[5] =  (UINT32)(pContext->resp_time_poserr_Dig_coh);
	pValue[6] =  (UINT32)(pContext->resp_time_negerr_Dig_coh);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedHybrid_Get
**
**  Description:    Returns Digital AGC speed (hybrid).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedHybrid_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] =  (UINT32)(pContext->speed_hybrid);
	pValue[1] =  (UINT32)(pContext->resp_time_poserr_RF_hybr);
	pValue[2] =  (UINT32)(pContext->resp_time_negerr_RF_hybr);
	pValue[3] =  (UINT32)(pContext->resp_time_poserr_IF_hybr);
	pValue[4] =  (UINT32)(pContext->resp_time_negerr_IF_hybr);
	pValue[5] =  (UINT32)(pContext->resp_time_poserr_Dig_hybr);
	pValue[6] =  (UINT32)(pContext->resp_time_negerr_Dig_hybr);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_Target_Set
**
**  Description:    Sets Digital AGC Hybrid mode target value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - when mode = none sets digital backoff in dB,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_Target_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_HYBMODE_TARG_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_HYBMODE_TARG_USR;
	}

	if (!(pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) {
		return retErrBadDevice; // applicable to FM1151 only
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->target = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			status = retErrNoSupport; // TBD
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			status = retErrNoSupport; // TBD
		break;

		case LX_ADEMOD_PRESET_OPTION_LOW:
			status = retErrNoSupport; // TBD
		break;

		case LX_ADEMOD_PRESET_OPTION_HIGH:
			status = retErrNoSupport; // TBD
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		UINT32 AGC_target = 0;
		status = DEMOD_M14_Analog_DigAGC_Target(pContext, pContext->target, &AGC_target);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_COH_1,    0x1, 1); // program sign bit
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_1, (AGC_target >> 16) & 0xFFFF, 1); // integer bits
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_NONCOH_2, AGC_target & 0xFFFF, 1); // fractional bits
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_Target_Get
**
**  Description:    Returns Digital AGC Hybrid mode target value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - target,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_Target_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (!(pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) {
		return retErrBadDevice; // applicable to FM1151 only
	}

	pValue[0] =  (UINT32)(pContext->target);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_DecimationRatio_Set
**
**  Description:    Sets Digital AGC Decimation Ratio value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - when mode = none sets decimation ratio,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_DecimationRatio_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_DECRATIO_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_DECRATIO_USR;
	}

	if (!(pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) {
		return retErrBadDevice; // applicable to FM1151 only
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->decimation_ratio = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			status = retErrNoSupport; // TBD
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			status = retErrNoSupport; // TBD
		break;

		case LX_ADEMOD_PRESET_OPTION_LOW:
			status = retErrNoSupport; // TBD
		break;

		case LX_ADEMOD_PRESET_OPTION_HIGH:
			status = retErrNoSupport; // TBD
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, SLOW_MODE_AGC_CNT_TH,    pContext->decimation_ratio, 2);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_DecimationRatio_Get
**
**  Description:    Returns Digital AGC Decimation Ratio value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - decimation ratio,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_DecimationRatio_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (!(pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) {
		return retErrBadDevice; // applicable to FM1151 only
	}

	pValue[0] =  (UINT32)(pContext->decimation_ratio);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_RFLevel_Get
**
**  Description:    Returns Digital AGC RF level.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: 0- RF, 1- IF, 2- Digital,
**							  pValue[1] - RF gain,
**							  pValue[2] - Digital RF gain,
**							  pValue[3] - Digital gain.
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_RFLevel_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 temp = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	status = ADEMOD_ReadSub(hUser, DIGAGC_TOP_MODE_RD,	&pValue[0], 1);
	status = ADEMOD_ReadSub(hUser, RFGAIN,				&temp, 4); // 31 bit
	if (temp & 0x40000000) {
		temp |= 0x80000000;
	}
	pValue[1] = temp;

	status = ADEMOD_ReadSub(hUser, DIGAGC_RFGAIN,		&temp, 4); // 31 bit
	if (temp & 0x40000000) {
		temp |= 0x80000000;
	}
	pValue[2] = temp;

	status = ADEMOD_ReadSub(hUser, DIGAGC_GAIN,			&temp, 3); // 21 bit
	if (temp & 0x100000) {
		temp |= 0xFFF00000;
	}
	pValue[3] = temp;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Gain_Get
**
**  Description:    Returns Digital AGC gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - DigAGC_Gain in dB*100,
**						      pValue[1] - DigAGC_Gain register reading.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Gain_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 Dig_gain = 0;
	int Dig_level = 0;
	int Dig_gain_dB = 0;
	int IF_level = 0;
	int i;

	status = ADEMOD_ReadSub(hUser, DIGAGC_GAIN, &Dig_gain, 3);

	if (status == retOK) {
		for ( i = 0; i < LX_ADEMOD_DIGAGC_GAIN_LUT_SIZE; i++) {
			if (Dig_gain < LX_ADEMOD_DIGAGC_GainLUT[i]) {
				Dig_gain_dB = i * 50; // dB*100
				break;
			}
		}
		// determine desired signal level after Main Filter
		Dig_level = pContext->REF - Dig_gain_dB;
		// determine desired IF signal level after VGA (6.3dB attenuation from filters
		IF_level = Dig_level + 6 * 100;
		// convert IF level to dBuV (ADC full scale = 119 dBuV)
		IF_level += 119 * 100;
		// determine desired IF signal level before VGA (for SECAM-L)
		if ((pContext->flags & LX_ADEMOD_FFLAG_DIGAGCMOD_TYPE) && (pValue[0] & LX_ADEMOD_CONFIG_FLAG_VGA_STATUS)) {
			// positive modulation and VGA is on */
			IF_level -= 856;
		}

		pValue[0] = IF_level;
		pValue[1] = Dig_gain;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BndwCoherent_Set
**
**  Description:    Sets Digital AGC coherent bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/narrow/wide/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BndwCoherent_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int LPF_Shift1 = 0;
	int LPF_Shift2 = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPFBNDW_COH_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPFBNDW_COH_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->lpf_bw_coh = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->lpf_bw_coh = LX_ADEMOD_DIGAGC_RFBNDW_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->lpf_bw_coh = LX_ADEMOD_DIGAGC_RFBNDW_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NARROW:
			pContext->lpf_bw_coh = LX_ADEMOD_DIGAGC_RFBNDW_NARROW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_WIDE:
			pContext->lpf_bw_coh = LX_ADEMOD_DIGAGC_RFBNDW_WIDE_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_LPF(pContext, pContext->lpf_bw_coh, (UINT32 *)&LPF_Shift1, (UINT32 *)&LPF_Shift2 );

		pContext->LPF_Shift1_coh = (INT16)(-LPF_Shift1);
		pContext->LPF_Shift2_coh = (INT16)(-LPF_Shift2);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_COH_1, pContext->LPF_Shift1_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_COEFFB_COH_2, pContext->LPF_Shift2_coh, 1);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedCoherent_Set
**
**  Description:    Sets Digital AGC speed (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedCoherent_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_COH_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_COH_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->resp_time_poserr_RF_coh =  pValue[1];
			pContext->resp_time_negerr_RF_coh =  pValue[2];
			pContext->resp_time_poserr_IF_coh =  pValue[3];
			pContext->resp_time_negerr_IF_coh =  pValue[4];
			pContext->resp_time_poserr_Dig_coh = pValue[5];
			pContext->resp_time_negerr_Dig_coh = pValue[6];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->resp_time_poserr_RF_coh =  5000000;
			pContext->resp_time_negerr_RF_coh =  5000000;
			pContext->resp_time_poserr_IF_coh =  5000000;
			pContext->resp_time_negerr_IF_coh =  5000000;
			pContext->resp_time_poserr_Dig_coh = 5000000;
			pContext->resp_time_negerr_Dig_coh = 5000000;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->resp_time_poserr_RF_coh =  100;
			pContext->resp_time_negerr_RF_coh =  100;
			pContext->resp_time_poserr_IF_coh =  100;
			pContext->resp_time_negerr_IF_coh =  100;
			pContext->resp_time_poserr_Dig_coh = 100;
			pContext->resp_time_negerr_Dig_coh = 100;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->resp_time_poserr_RF_coh =  300000;
			pContext->resp_time_negerr_RF_coh =  300000;
			pContext->resp_time_poserr_IF_coh =  300000;
			pContext->resp_time_negerr_IF_coh =  300000;
			pContext->resp_time_poserr_Dig_coh = 300000;
			pContext->resp_time_negerr_Dig_coh = 300000;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->resp_time_poserr_RF_coh =  523;
			pContext->resp_time_negerr_RF_coh =  1046;
			pContext->resp_time_poserr_IF_coh =  500;
			pContext->resp_time_negerr_IF_coh =  1000;
			pContext->resp_time_poserr_Dig_coh = 600;
			pContext->resp_time_negerr_Dig_coh = 1400;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		pContext->speed_coherent = pValue[0];
		status = DEMOD_M14_Analog_DigAGC_LoopGainCoherent(pContext, pContext->resp_time_poserr_RF_coh,
														  pContext->resp_time_negerr_RF_coh,
														  pContext->resp_time_poserr_IF_coh,
														  pContext->resp_time_negerr_IF_coh,
														  pContext->resp_time_poserr_Dig_coh,
														  pContext->resp_time_negerr_Dig_coh);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_COH_1,    pContext->LPFGain_poserr_RF_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_POSERR_COH_2,    pContext->LPFGain_poserr_RF_Shift_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_COH_1,    pContext->LPFGain_negerr_RF_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_NEGERR_COH_2,    pContext->LPFGain_negerr_RF_Shift_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_COH_1,    pContext->LPFGain_poserr_IF_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_POSERR_COH_2,    pContext->LPFGain_poserr_IF_Shift_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_COH_1,    pContext->LPFGain_negerr_IF_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_IF_LPFGAIN_NEGERR_COH_2,    pContext->LPFGain_negerr_IF_Shift_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_COH_1,       pContext->LPFGain_poserr_Dig_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_COH_2,       pContext->LPFGain_poserr_Dig_Shift_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_COH_1,       pContext->LPFGain_negerr_Dig_coh, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_COH_2,       pContext->LPFGain_negerr_Dig_Shift_coh, 1);
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedHybrid_Set
**
**  Description:    Sets Digital AGC speed in Hybrid mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - not used,
**						      pValue[3] - not used,
**						      pValue[4] - not used.
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedHybrid_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_HYBR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_HYBR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->resp_time_poserr_RF_hybr =  pValue[1];
			pContext->resp_time_negerr_RF_hybr =  pValue[2];
			pContext->resp_time_poserr_IF_hybr =  pValue[3];
			pContext->resp_time_negerr_IF_hybr =  pValue[4];
			pContext->resp_time_poserr_Dig_hybr = pValue[5];
			pContext->resp_time_negerr_Dig_hybr = pValue[6];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->resp_time_poserr_RF_hybr =  5000000;
			pContext->resp_time_negerr_RF_hybr =  5000000;
			pContext->resp_time_poserr_IF_hybr =  5000000;
			pContext->resp_time_negerr_IF_hybr =  5000000;
			pContext->resp_time_poserr_Dig_hybr = 5000000;
			pContext->resp_time_negerr_Dig_hybr = 5000000;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->resp_time_poserr_RF_hybr =  100;
			pContext->resp_time_negerr_RF_hybr =  100;
			pContext->resp_time_poserr_IF_hybr =  100;
			pContext->resp_time_negerr_IF_hybr =  100;
			pContext->resp_time_poserr_Dig_hybr = 100;
			pContext->resp_time_negerr_Dig_hybr = 100;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->resp_time_poserr_RF_hybr =  300000;
			pContext->resp_time_negerr_RF_hybr =  300000;
			pContext->resp_time_poserr_IF_hybr =  300000;
			pContext->resp_time_negerr_IF_hybr =  300000;
			pContext->resp_time_poserr_Dig_hybr = 300000;
			pContext->resp_time_negerr_Dig_hybr = 300000;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->resp_time_poserr_RF_hybr =  523;
			pContext->resp_time_negerr_RF_hybr =  1046;
			pContext->resp_time_poserr_IF_hybr =  500;
			pContext->resp_time_negerr_IF_hybr =  1000;
			pContext->resp_time_poserr_Dig_hybr = 600;
			pContext->resp_time_negerr_Dig_hybr = 1400;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		pContext->speed_hybrid = pValue[0];
		status = DEMOD_M14_Analog_DigAGC_LoopGainHybrid(pContext, pContext->resp_time_poserr_RF_hybr,
														  pContext->resp_time_negerr_RF_hybr,
														  pContext->resp_time_poserr_IF_hybr,
														  pContext->resp_time_negerr_IF_hybr,
														  pContext->resp_time_poserr_Dig_hybr,
														  pContext->resp_time_negerr_Dig_hybr);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_GAIN_AGC_ECO,    pContext->LPFGain_poserr_RF_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_RF_LPFGAIN_SHIFT_AGC_ECO,   pContext->LPFGain_poserr_RF_Shift_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_1,    pContext->LPFGain_poserr_Dig_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_POSERR_NONCOH_2,    pContext->LPFGain_poserr_Dig_Shift_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_1,    pContext->LPFGain_negerr_Dig_hybr, 1);
		ADEMOD_WriteSub( hUser, DIGAGC_LPFGAIN_NEGERR_NONCOH_2,    pContext->LPFGain_negerr_Dig_Shift_hybr, 1);
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Backoff_Set
**
**  Description:    Sets Digital AGC backoff (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/low/high/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Backoff_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 AGC_ref = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_BACKOFF_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_BACKOFF_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->backoff = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->backoff = LX_ADEMOD_DIGAGC_BACKOFF_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->backoff = LX_ADEMOD_DIGAGC_BACKOFF_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LOW:
			pContext->backoff = LX_ADEMOD_DIGAGC_BACKOFF_LOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_HIGH:
			pContext->backoff = LX_ADEMOD_DIGAGC_BACKOFF_HIGH_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_BackOff(pContext, pContext->backoff, &AGC_ref);
		pContext->ref = AGC_ref;
		pContext->backoff_mode = pValue[0];
	}

	if (Update) {
		if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
			UINT32 ref = pContext->ref & 0xFFFFF800; // mask 11 bits
			UINT32 DigADC_Ref_NoCoh = (UINT32)(
				((pContext->narrowband_thr2 & 0x1F)	<< 0) +
				((pContext->narrowband_thr1 & 0x3F)	<< 5) +
				(ref)
			);
			ADEMOD_WriteSub( hUser, DIGAGC_REF_NONCOH, DigADC_Ref_NoCoh, 4);
		}
		else {
			ADEMOD_WriteSub( hUser, DIGAGC_REF_NONCOH, pContext->ref, 4);
		}
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Backoff_Get
**
**  Description:    Returns Digital AGC backoff (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/low/high/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Backoff_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->backoff_mode;
	pValue[1] = pContext->backoff;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Set
**
**  Description:    Sets Narrowband AGC Piece-wise LPFGain thresholds.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - thresh1 register: DigAGC_Ref_noncoh[10:5] -> signed.
**						      pValue[2] - thresh2 register: DigAGC_Ref_noncoh[4:0]  -> unsigned.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_LPDNBND_THR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_LPDNBND_THR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->narrowband_thr1 = pValue[1];
			pContext->narrowband_thr2 = pValue[2];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->narrowband_thr1 = LX_ADEMOD_DIGAGC_NBNDTHR1_MIN_FCT;
			pContext->narrowband_thr2 = LX_ADEMOD_DIGAGC_NBNDTHR2_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->narrowband_thr1 = LX_ADEMOD_DIGAGC_NBNDTHR1_MAX_FCT;
			pContext->narrowband_thr2 = LX_ADEMOD_DIGAGC_NBNDTHR2_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->narrowband_thr1 = LX_ADEMOD_DIGAGC_NBNDTHR1_NOM_FCT;
			pContext->narrowband_thr2 = LX_ADEMOD_DIGAGC_NBNDTHR2_NOM_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}


	if (status == retOK) {
		pContext->narrowband_mode_thr = pValue[0];
	}

	if (Update) {
		if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
			UINT32 ref = pContext->ref & 0xFFFFF800; // mask 11 bits
			UINT32 DigADC_Ref_NoCoh = (UINT32)(
				((pContext->narrowband_thr2 & 0x1F)	<< 0) +
				((pContext->narrowband_thr1 & 0x3F)	<< 5) +
				(ref)
			);
			ADEMOD_WriteSub( hUser, DIGAGC_REF_NONCOH, DigADC_Ref_NoCoh, 4);
		}
		else {
			// N/A
		}
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Get
**
**  Description:    Returns Narrowband AGC Piece-wise LPFGain thresholds.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - thresh1 register: DigAGC_Ref_noncoh[10:5] -> signed.
**						      pValue[2] - thresh2 register: DigAGC_Ref_noncoh[4:0]  -> unsigned.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->narrowband_mode_thr;
	pValue[1] = pContext->narrowband_thr1;
	pValue[2] = pContext->narrowband_thr2;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BackoffCoherent_Set
**
**  Description:    Sets Digital AGC backoff (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/low/high/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BackoffCoherent_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 AGC_ref = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DIGAGC_BACKOFF_COH_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DIGAGC_BACKOFF_COH_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->backoff_coh = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->backoff_coh = LX_ADEMOD_DIGAGC_BACKOFF_COH_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->backoff_coh = LX_ADEMOD_DIGAGC_BACKOFF_COH_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LOW:
			pContext->backoff_coh = LX_ADEMOD_DIGAGC_BACKOFF_COH_LOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_HIGH:
			pContext->backoff_coh = LX_ADEMOD_DIGAGC_BACKOFF_COH_HIGH_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_DigAGC_BackOff(pContext, pContext->backoff_coh, &AGC_ref);
		pContext->ref_coh = AGC_ref;
		pContext->backoff_mode_coh = pValue[0];
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, DIGAGC_REF_COH, pContext->ref_coh, 4);
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BackoffCoherent_Get
**
**  Description:    Returns Digital AGC backoff (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/low/high/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BackoffCoherent_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->backoff_mode_coh;
	pValue[1] = pContext->backoff_coh;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_MeasurementMode_Set
**
**  Description:    Sets DigAGC settings according to MeasurementMode.
**
**  Parameters:     pContext     - Pointer to DigAGC context.
**                  pValue       - Pointer to the value,
**						where pValue[0] - Measurement mode 1- enabled, 0- disabled
**						when measurement_mode = LX_ADEMOD_MEASUREMENTMODE_POWERMETER:
**						      pValue[1] - Audio Lower frequency offset,
**						      pValue[2] - Audio Higher frequency offset,
**						      pValue[3] - AGC manual gain,
**						      pValue[4] - Power meter low threshold,
**						      pValue[5] - Power meter high threshold,
**						      pValue[6] - pres_gain_rf,
**						      pValue[7] - iir,
**						      pValue[8] - caltone offset.
**						when measurement_mode = LX_ADEMOD_MEASUREMENTMODE_DIGAGCGAIN:
**						      pValue[1] - backoff,
**						      pValue[2] - LPF bandwidth,
**						      pValue[3] - DigAGC_MinGain,
**						      pValue[4] - DigAGC_MaxGain,
**						      pValue[5] - DigAGC_PresetGain,
**						      pValue[6] - pres_gain_rf,
**						      pValue[7] - Reserved,
**						      pValue[8] - video bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/

/*
** Static functions implementation
*/


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPF
**
**  Description:    Returns Digital LPF shifts according to the bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPF_BW       - LPF Bandwidth,
**                  pLPF_Shift1  - Pointer to shift 1,
**                  pLPF_Shift2  - Pointer to shift 2.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPF(pLX_ADEMOD_DigAgc pContext, UINT32 LPF_BW, UINT32* pLPF_Shift1, UINT32* pLPF_Shift2)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 LPF_Coeff;
	UINT64 LPF_Shift1;
	UINT64 LPF_Shift2;

	LPF_Coeff = ((UINT64)LPF_BW * 128);
	do_div(LPF_Coeff, 124);	// in mHz

	LPF_Shift1 = 31;
	while ((LPF_Coeff <  (UINT64)1 << LPF_Shift1) && (LPF_Shift1 > 7))
	{
		LPF_Shift1--;
	}

	LPF_Coeff = LPF_Coeff - (INT64)(1 << LPF_Shift1);

	LPF_Shift2 = LPF_Shift1;
	while ((LPF_Coeff < (UINT64)1 << LPF_Shift2) && (LPF_Shift2 > 7))
	{
		LPF_Shift2--;
	}

	*pLPF_Shift1 = (UINT32)LPF_Shift1 - 31;
	*pLPF_Shift2 = (UINT32)LPF_Shift2 - 31;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPFGain_Shift
**
**  Description:    Sets Digital LPF gain shift according to the LPF gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPFGain      - LPF Gain,
**                  pLPFGain_Shift      - Pointer to LPF Gain shift.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pLX_ADEMOD_DigAgc pContext, UINT32* pLPFGain, UINT32* pLPFGain_Shift)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 LPFGain_Shift;

	LPFGain_Shift = 0;
	while ( *pLPFGain / (UINT32)(1 << LPFGain_Shift) >= 1 )
	{
		LPFGain_Shift++;
	}

	*pLPFGain = *pLPFGain * 16 / (UINT32) (1 << LPFGain_Shift);

	*pLPFGain_Shift = 31 - LPFGain_Shift;


	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LoopGain
**
**  Description:    Sets Digital AGC loop gain (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPFGain      - LPF Bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LoopGain(pLX_ADEMOD_DigAgc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF,
									   UINT32 resp_time_poserr_Dig,
									   UINT32 resp_time_negerr_Dig)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT32 LPFGain_Shift = 0;
	UINT64 LPFGain = 0;

	if ((resp_time_poserr_RF  == 0) ||
		(resp_time_negerr_RF  == 0) ||
		(resp_time_poserr_IF  == 0) ||
		(resp_time_negerr_IF  == 0) ||
		(resp_time_poserr_Dig == 0) ||
		(resp_time_negerr_Dig == 0)) {
		return retErrBadParam;
	}

	LPFGain = (50000 * 32768);
	do_div(LPFGain,(resp_time_poserr_RF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_RF = (UINT16)LPFGain;
	pContext->LPFGain_poserr_RF_Shift = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,(resp_time_poserr_RF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_RF = (UINT16)LPFGain;
	pContext->LPFGain_negerr_RF_Shift = (UINT16)LPFGain_Shift;

	LPFGain = ((UINT64)50000 * 32768 * 3);
	do_div(LPFGain,(resp_time_poserr_RF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_IF = (UINT16)LPFGain;
	pContext->LPFGain_poserr_IF_Shift = (UINT16)LPFGain_Shift;

	LPFGain = ((UINT64)50000 * 32768 * 3);
	do_div(LPFGain,(resp_time_negerr_IF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_IF = (UINT16)LPFGain;
	pContext->LPFGain_negerr_IF_Shift = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,resp_time_poserr_Dig  );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_Dig = (UINT16)LPFGain;
	pContext->LPFGain_poserr_Dig_Shift = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,resp_time_negerr_Dig  );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_Dig = (UINT16)LPFGain;
	pContext->LPFGain_negerr_Dig_Shift = (UINT16)LPFGain_Shift;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LoopGainCoherent
**
**  Description:    Sets Digital AGC loop gain (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LoopGainCoherent(pLX_ADEMOD_DigAgc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF,
									   UINT32 resp_time_poserr_Dig,
									   UINT32 resp_time_negerr_Dig)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT32 LPFGain_Shift = 0;
	UINT64 LPFGain = 0;

	if ((resp_time_poserr_RF  == 0) ||
		(resp_time_negerr_RF  == 0) ||
		(resp_time_poserr_IF  == 0) ||
		(resp_time_negerr_IF  == 0) ||
		(resp_time_poserr_Dig == 0) ||
		(resp_time_negerr_Dig == 0)) {
		return retErrBadParam;
	}

	LPFGain = (50000 * 32768);
	do_div(LPFGain,(resp_time_poserr_RF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_RF_coh = (UINT16)LPFGain;
	pContext->LPFGain_poserr_RF_Shift_coh = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,(resp_time_negerr_RF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_RF_coh = (UINT16)LPFGain;
	pContext->LPFGain_negerr_RF_Shift_coh = (UINT16)LPFGain_Shift;

	LPFGain = ((UINT64)50000 * 32768 * 3);
	do_div(LPFGain,(resp_time_poserr_IF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_IF_coh = (UINT16)LPFGain;
	pContext->LPFGain_poserr_IF_Shift_coh = (UINT16)LPFGain_Shift;

	LPFGain = ((UINT64)50000 * 32768 * 3);
	do_div(LPFGain,(resp_time_negerr_IF * 128) );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_IF_coh = (UINT16)LPFGain;
	pContext->LPFGain_negerr_IF_Shift_coh = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,resp_time_poserr_Dig );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_Dig_coh = (UINT16)LPFGain;
	pContext->LPFGain_poserr_Dig_Shift_coh = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,resp_time_negerr_Dig );	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_Dig_coh = (UINT16)LPFGain;
	pContext->LPFGain_negerr_Dig_Shift_coh = (UINT16)LPFGain_Shift;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LoopGainHybrid
**
**  Description:    Sets Digital AGC loop gain (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPFGain      - LPF Bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LoopGainHybrid(pLX_ADEMOD_DigAgc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF,
									   UINT32 resp_time_poserr_Dig,
									   UINT32 resp_time_negerr_Dig)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT32 LPFGain_Shift = 0;
	UINT64 LPFGain = 0;

	if ((resp_time_poserr_RF  == 0) ||
		(resp_time_poserr_Dig == 0) ||
		(resp_time_negerr_Dig == 0)) {
		return retErrBadParam;
	}

	LPFGain = (50000 * 32768);
	do_div(LPFGain,(resp_time_poserr_RF * 128));	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_RF_hybr = (UINT16)LPFGain;
	pContext->LPFGain_poserr_RF_Shift_hybr = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,resp_time_poserr_Dig);	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_poserr_Dig_hybr = (UINT16)LPFGain;
	pContext->LPFGain_poserr_Dig_Shift_hybr = (UINT16)LPFGain_Shift;

	LPFGain = (50000 * 32768);
	do_div(LPFGain,resp_time_negerr_Dig);	//in us
	DEMOD_M14_Analog_DigAGC_LPFGain_Shift(pContext, (UINT32 *)&LPFGain, &LPFGain_Shift);
	pContext->LPFGain_negerr_Dig_hybr = (UINT16)LPFGain;
	pContext->LPFGain_negerr_Dig_Shift_hybr = (UINT16)LPFGain_Shift;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BackOff
**
**  Description:    Sets Digital AGC backoff.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  BackOff_dB   - Backoff in dB,
**                  pAGC_ref     - Pointer to AGC reference.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BackOff(pLX_ADEMOD_DigAgc pContext, int BackOff_dB, UINT32* pAGC_ref)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (BackOff_dB < LX_ADEMOD_DIGAGC_BACKOFF_LUT_SIZE) {
		if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
			*pAGC_ref = LX_ADEMOD_DIGAGC_BackoffLUT_A2[BackOff_dB];
		}
		else {
			*pAGC_ref = LX_ADEMOD_DIGAGC_BackoffLUT[BackOff_dB];
		}
	}
	else {
		status = retErrBadParam;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Target
**
**  Description:    Sets Digital AGC target in Hybrid mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  BackOff_dB   - Backoff in dB,
**                  pAGC_target  - Pointer to AGC target.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Target(pLX_ADEMOD_DigAgc pContext, int BackOff_dB, UINT32* pAGC_target)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (BackOff_dB < LX_ADEMOD_DIGAGC_HYBRID_TARGET_SIZE) {
		*pAGC_target = LX_ADEMOD_DIGAGC_HybridTargetLUT[BackOff_dB];
	}
	else {
		status = retErrBadParam;
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Attack
**
**  Description:    Sets Digital AGC attack.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  Attack       - Attack.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Attack(pLX_ADEMOD_DigAgc pContext, UINT32 Attack)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 Attack_Shift1;
	UINT64 Attack_Shift2;

	Attack_Shift1 = 27;
	while ((Attack < (UINT64)1 << Attack_Shift1) && (Attack > 4))
	{
		Attack_Shift1--;
	}

	Attack -= (UINT32)(1 << Attack_Shift1);

	Attack_Shift2 = Attack_Shift1;
	while ((Attack < (UINT64)1 << Attack_Shift2) && (Attack_Shift2 > 4))
	{
		Attack_Shift2--;
	}

	pContext->attack_1 = (UINT16)Attack_Shift1 - 27;
	pContext->attack_2 = (UINT16)Attack_Shift2 - 27;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_FastDecay
**
**  Description:    Sets RF AGC fast decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  DecayTimeOut_us - Decay timeout.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_FastDecay(pLX_ADEMOD_DigAgc pContext, UINT32 DecayTimeOut_us)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 FastDecayWin;

	FastDecayWin = DecayTimeOut_us * 1009 / 25600;

	pContext->win = (UINT32)FastDecayWin;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Decay
**
**  Description:    Sets RF AGC decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  DecayTimeOut_us - Decay timeout.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Decay(pLX_ADEMOD_DigAgc pContext, UINT32 Decay_us)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 temp;
	
	temp = (8053 * 575);
	do_div(temp, Decay_us);

	pContext->Leaky_Decay = (UINT32)temp;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_RFGainThrsh_Set
**
**  Description:    Sets Digital AGC min/max RF gain according to the top mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_RFGainThrsh_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->rf_mode)
	{
		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->max_gain_rf = LX_ADEMOD_DIGAGC_MAX_GAIN_RFNOM_FCT;
			pContext->min_gain_rf = LX_ADEMOD_DIGAGC_MIN_GAIN_RFNOM_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->max_gain_rf = LX_ADEMOD_DIGAGC_MAX_GAIN_RFMIN_FCT;
			pContext->min_gain_rf = LX_ADEMOD_DIGAGC_MIN_GAIN_RFMIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->max_gain_rf = LX_ADEMOD_DIGAGC_MAX_GAIN_RFMAX_FCT;
			pContext->min_gain_rf = LX_ADEMOD_DIGAGC_MIN_GAIN_RFMAX_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_IFGainThrsh_Set
**
**  Description:    Sets Digital AGC min/max IF gain according to the top mode.
**
**  Parameters:     pContext     - Pointer to AGC context
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_IFGainThrsh_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->if_mode)
	{
		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->max_gain_if = LX_ADEMOD_DIGAGC_MAX_GAIN_IFNOM_FCT;
			pContext->min_gain_if = LX_ADEMOD_DIGAGC_MIN_GAIN_IFNOM_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->max_gain_if = LX_ADEMOD_DIGAGC_MAX_GAIN_IFMIN_FCT;
			pContext->min_gain_if = LX_ADEMOD_DIGAGC_MIN_GAIN_IFMIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->max_gain_if = LX_ADEMOD_DIGAGC_MAX_GAIN_IFMAX_FCT;
			pContext->min_gain_if = LX_ADEMOD_DIGAGC_MIN_GAIN_IFMAX_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_DigGainThrsh_Set
**
**  Description:    Sets Digital AGC min/max digital gain according to the top mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_DigGainThrsh_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->dig_mode)
	{
		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->max_gain_dig = LX_ADEMOD_DIGAGC_MAX_GAIN_DIGNOM_FCT;
			pContext->min_gain_dig = LX_ADEMOD_DIGAGC_MIN_GAIN_DIGNOM_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->max_gain_dig = LX_ADEMOD_DIGAGC_MAX_GAIN_DIGMIN_FCT;
			pContext->min_gain_dig = LX_ADEMOD_DIGAGC_MIN_GAIN_DIGMIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->max_gain_dig = LX_ADEMOD_DIGAGC_MAX_GAIN_DIGMAX_FCT;
			pContext->min_gain_dig = LX_ADEMOD_DIGAGC_MIN_GAIN_DIGMAX_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	switch (pContext->dig_mode_nbnd)
	{
		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->max_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MAX_NBND_GAIN_DIGNOM_FCT;
			pContext->min_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MIN_NBND_GAIN_DIGNOM_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->max_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MAX_NBND_GAIN_DIGMIN_FCT;
			pContext->min_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MIN_NBND_GAIN_DIGMIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->max_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MAX_NBND_GAIN_DIGMAX_FCT;
			pContext->min_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MIN_NBND_GAIN_DIGMAX_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_RFGainPreset_Set
**
**  Description:    Sets Digital AGC RF gain preset according to manual gain mode.
**
**  Parameters:     pContext     - Pointer to AGC context.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_RFGainPreset_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->rf_gain)
	{
		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->pres_gain_rf = pContext->min_gain_rf;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->pres_gain_rf = pContext->max_gain_rf;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->pres_gain_rf = 0;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_IFGainPreset_Set
**
**  Description:    Sets Digital AGC IF gain preset according to manual gain mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_IFGainPreset_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->if_gain)
	{
		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->pres_gain_if = pContext->min_gain_if;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->pres_gain_if = pContext->max_gain_if;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->pres_gain_if = 0;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_DigGainPreset_Set
**
**  Description:    Sets Digital AGC Digital gain preset according to manual gain mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_DigGainPreset_Set(pLX_ADEMOD_DigAgc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->dig_gain)
	{
		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->pres_gain_dig = pContext->min_gain_dig;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->pres_gain_dig = pContext->max_gain_dig;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->pres_gain_dig = LX_ADEMOD_DIGAGC_MAX_GAIN_DIGNOM_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	switch (pContext->dig_gain_nbnd)
	{
		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->pres_gain_dig_nbnd = pContext->min_gain_dig_nbnd;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->pres_gain_dig_nbnd = pContext->max_gain_dig_nbnd;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->pres_gain_dig_nbnd = LX_ADEMOD_DIGAGC_MAX_NBND_GAIN_DIGNOM_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Reset
**
**  Description:    Executes soft reset on AGC block.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**					on_off       - When TRUE the reset state is entered,
**                                 when FALSE the reset state is terminated.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Reset(pLX_ADEMOD_DigAgc pContext, BOOL on_off, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// FIFO reset AGC,
	// register FIFO_CLR0:
	// bit 4 - dac_if_rfgain_fifo_clr
	// bit 5 - dac_if_ifgain_fifo_clr

	if (on_off) {
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xc0);
	}
	else {
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xdf);
	}

	return status;
}



