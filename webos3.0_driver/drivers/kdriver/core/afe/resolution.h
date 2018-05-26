#ifndef L8_ADC_RESOLUTION_DEFINITION
#define L8_ADC_RESOLUTION_DEFINITION

#define THR_SD_EXT_VSIZE 50000
#define THR_SD_VD_VSIZE 20000
//#define THR_SD_VD_VSIZE_INT 20000
#define THR_SD_VD_HSIZE 50000

#define PHASE_METHOD1_WIDTH_MARGIN 400
#define PHASE_METHOD1_HEIGHT_MARGIN 300
/*
#define PHASE_METHOD2_OFFSET 20
#define PHASE_POS_X_STEP 3
#define PHASE_POS_Y_STEP 3

#define PHASE_POS_OFFSET 3

#define PHASE_STEP_DEFINE
#define PHASE_STEP 20
#define PHASE_OFFSET 5
*/
//#define PHASE_METHOD2_OFFSET 100 // by jjb
#define PHASE_METHOD2_OFFSET 20 // original value
#define PHASE_POS_X_STEP 1 //powerory 2010-10-05
#define PHASE_POS_Y_STEP 1 //powerory 2010-10-05

#define PHASE_POS_OFFSET 3

#define PHASE_STEP_DEFINE
//#define PHASE_STEP 30	//by jjb
//#define PHASE_STEP 20		// original value
#define PHASE_STEP 30		// 110314
#define PHASE_OFFSET 5
#define PHASE_STEP_SIMPLE 5		// original value
#define PHASE_OFFSET_SIMPLE 5
// 110721 wonsik.do : HActive Read value from ADC Boundary is sometimes bigger than it's real value
#define SIZE_FRONT_OFFSET 2
//#define SIZE_FRONT_OFFSET 4
//#define SIZE_BACK_OFFSET 3
#define SIZE_BACK_OFFSET 5
#define PHASE_STEP_COMPONENT 30	
#define PHASE_OFFSET_COMPONENT 5

#ifndef PHASE_STEP_DEFINE
#define PHASE_DUMP
#endif

#define	PARAM_NUM	344
#define	LX_RES_MAXIMUM	32

//extern LX_AFE_PCMODE_MODETABLE_T LX_Default_ModeTable[PARAM_NUM];
typedef struct
{
	UINT16	u16_Width;
	UINT16	u16_Height;
} KADP_AFE_RESOLUTION_T;
extern KADP_AFE_RESOLUTION_T LX_Default_Resolution[LX_RES_MAXIMUM];
extern int ConvertTable[PARAM_NUM];



// From forADC
extern int TB_PI[PARAM_NUM];
extern int TB_PC[PARAM_NUM];
extern int TB_WIDTH[PARAM_NUM];
extern int TB_HEIGHT[PARAM_NUM];
//120103 wonsik.do : removed floating point in kernel driver
extern unsigned int TB_FV[PARAM_NUM];
extern unsigned int TB_FH[PARAM_NUM];
extern unsigned int TB_FS[PARAM_NUM];
extern int TB_SD_EXT_VSIZE[PARAM_NUM];
extern int TB_SD_VD_VSIZE[PARAM_NUM];
extern int TB_SD_VD_HSIZE[PARAM_NUM];
extern int LLPLL_div_max[PARAM_NUM] ;
extern int LLPLL_counter_max[PARAM_NUM];
extern int LLPLL_dco_max[PARAM_NUM];
extern int LLPLL_dco_min[PARAM_NUM];
extern int LLPLL_coarse_scale[PARAM_NUM];
extern int LLPLL_g1_nom[PARAM_NUM];
extern int LLPLL_g2_nom[PARAM_NUM];
extern int LLPLL_g3_p_nom[PARAM_NUM];
extern int LLPLL_g3_n_nom[PARAM_NUM];
extern int LLPLL_g1_fine[PARAM_NUM];
extern int LLPLL_g2_fine[PARAM_NUM];
extern int LLPLL_g3_p_fine[PARAM_NUM];
extern int LLPLL_g3_n_fine[PARAM_NUM];
extern int HS_Out_Width[PARAM_NUM];

#endif
