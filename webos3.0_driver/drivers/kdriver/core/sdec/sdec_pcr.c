/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"
#include "sdec_drv.h"
#include "sdec_io.h"
#include "sdec_hal.h"

#include "sys_regs.h"
#include "sdec_reg_ctrl.h"

#include "sdec_pcr.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#ifdef __NEW_PWM_RESET_COND__ /* jinhwan.bae for new PWM reset condition */

#define 	MAX_ERROR_FOR_PWM_RESET		 	10

static UINT8	pcr_error_for_reset = 0;

#endif


/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/
static	void pwm_context_reset(S_SDEC_PARAM_T	*stpSdecParam, UINT8 cur_ch);


/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/




/**
********************************************************************************
* @brief
*   Set PCR PID
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   stpLXSdecPIDFltSetPID : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SetPCRPID
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_PIDFLT_SET_PCRPID_T *stpLXSdecPIDFltSetPID)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT32 ui32PidValue = 0x0;
	BOOLEAN bMain = 0x0;
	UINT8 core = 0, org_ch = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecPIDFltSetPID == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SetPCRPID");

	ui8Ch = stpLXSdecPIDFltSetPID->eCh;
	ui32PidValue = stpLXSdecPIDFltSetPID->uiPidValue;
	bMain = stpLXSdecPIDFltSetPID->bMain;

	org_ch = ui8Ch;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);
	LX_SDEC_CHECK_CODE( ui8Ch > 2, goto exit, "channel is invalid org_ch[%d]ui8Ch[%d]", org_ch, ui8Ch );

	SDEC_PRINT_PIDSEC( "Ch[%d] PCR  PidValue[0x%08x]", org_ch,	ui32PidValue);

	if(ui32PidValue == 0x1fff)
	{
		// chan | pcr_pid
		SDEC_HAL_STCCSetReg(core, ui8Ch, 0x00000000 | (ui8Ch << 29) | (ui32PidValue << 16));
		if(bMain) pwm_context_reset(stpSdecParam, ui8Ch);
	}
	else
	{
		// en | chan | pcr_pid | copy_en | latch_en
		SDEC_HAL_STCCSetReg(core, ui8Ch, 0x80000000 | (ui8Ch << 29) );
		SDEC_HAL_STCCSetSTC(core, ui8Ch, 0);
		SDEC_HAL_STCCSetReg(core, ui8Ch, 0x80000003 | (ui8Ch << 29) | (ui32PidValue << 16));
	}

	/* if current channel is main channel, set main
	 * warning *
	 * viewing channel is not main channel, main channel is pcr recovery channel */
	if(bMain)
	{
		SDEC_HAL_STCCSetMain(core, ui8Ch);
		pwm_context_reset(stpSdecParam, ui8Ch);	// added by jinhwan.bae for support pip operation
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_SetPCRPID");

	eRet = RET_OK;

exit:

	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);

}


/**
********************************************************************************
* @brief
*   set pcr recovery.
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*   stpSdecParam :SDEC parameter
*   stpLXSdecSetPCRRecovery : ioctrl arguments from userspace
* @return
*  int
********************************************************************************
*/
int SDEC_IO_SetPcrRecovery
	(S_SDEC_PARAM_T *stpSdecParam,
	LX_SDEC_SET_PCR_RECOVERY_T *stpLXSdecSetPCRRecovery)
{
	int eRet = RET_ERROR;
	UINT8 ui8Ch = 0x0;
	UINT8 ui8PcrCmd = 0x0;
	UINT8 core = 0, org_ch = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( stpLXSdecSetPCRRecovery == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	OS_LockMutex(&stpSdecParam->stSdecMutex);

	SDEC_PRINT_TRACE( "<--SDEC_IO_SetPcrRecovery");

	ui8Ch = stpLXSdecSetPCRRecovery->eCh;
	ui8PcrCmd = stpLXSdecSetPCRRecovery->eCmd;

	org_ch = ui8Ch;
	SDEC_CONVERT_CORE_CH(core, ui8Ch);
	LX_SDEC_CHECK_CODE( ui8Ch > 2, goto exit, "channel is invalid org_ch[%d]ui8Ch[%d]", org_ch, ui8Ch );

	switch (ui8PcrCmd)
	{
		case LX_SDEC_PCR_CMD_DISABLE:
			stpSdecParam->bPcrRecoveryFlag[org_ch] = FALSE;
//			SDEC_HAL_STCCReset(core, ui8Ch);
			break;
		case LX_SDEC_PCR_CMD_ENABLE:
			stpSdecParam->bPcrRecoveryFlag[org_ch] = TRUE;
#if 0	/* M14_TBD jinhwan.bae to add for stc counter reset to 0 , which is A/V decoder STC counter value reset , pause -> play sequence */
			SDEC_HAL_AVSTCReset(core, ui8Ch);
			usleep(50);	/* actually 12us needed to send the reset value to A/V Decoder */
#endif
			SDEC_HAL_STCCReset(core, ui8Ch);

#ifdef __NEW_PWM_RESET_COND__
			pcr_error_for_reset = 0;
#endif
			break;
		case LX_SDEC_PCR_CMD_RESET:
			SDEC_HAL_STCCReset(core, ui8Ch);
			SDEC_HAL_STCCEnableCopy(core, ui8Ch, SDEC_HAL_ENABLE);
		break;
		default:
			SDEC_PRINT_ERROR("Invalid PCR cmd:[%d]", ui8PcrCmd);
			goto exit;
		break;
	}

	SDEC_PRINT_TRACE( "-->SDEC_IO_SetPcrRecovery");

	eRet = RET_OK;

exit:
	OS_UnlockMutex(&stpSdecParam->stSdecMutex);
	return (eRet);
}




/**
 * Below code is from LG D2A STB
 *
 * Modified and adjusted by Jihoon Lee ( gaius.lee@lge.com )
 *
 * 2010.06.14 @ DTV Lab.
 */


/**
 *
 * Filter context - initialised by calling pwm_init
 * @ingroup sdec_struct_type
 */
typedef struct
{
  unsigned long service				: 1, 	///< 0x00(bit 1) : pcr service doing
                recovery_Enable     : 1, 	///< 0x00(bit 1) : enable pcr recovery
                first_pcr_arrived	: 1,	///< 0x00(bit 1) : first_pcr_arrived
                reserved 			: 29;	///< 0x00(bit 28): reserved
  unsigned long fcw_base;					///< 0x04        : fcw base for selected Memory Clock
  signed   long fcw_offset;					///< 0x08        : fcw offset for first reset after each ch. change.
  unsigned long fcw_value;					///< 0x0c        : temperary fcw value
  unsigned long lastUpdateTime;				///< 0x10        : last updated Time Tick
  unsigned long new_reset_discard;			///< 0x14        : after reset recovery discarding count
  unsigned long new_adjust_skip;			///< 0x18        : skip adjusting right after adjust
  unsigned long skip_chk_overshoot;		 	///< 0x1c        : check divergence
  unsigned long fcw_trickSpeed;				///< 0x20        : multiply value for trick mode
  unsigned int	max_ignore_count;			///			 : max ignore count for protecting first jitter
  unsigned int  stable_overshoot;
} pwm_context_t;

static UINT32	max_fcw_value;
static UINT32	min_fcw_value;

static UINT32	pwm_fcw_default		= 0;
static UINT32	pwm_fcw_threshold		= 0;
static UINT32	pwm_fcw_threshold_with_max_ignore = 0;
static UINT32	pwm_fcw_window_sz		= 0;
static SINT32	pwm_fcw_sz_step		= 0;


/*---------------------------------------------------------
 * Constant definition
---------------------------------------------------------*/
#define DCO_INPUT_CLOCK 	198
#define DCO_COEF			8388608		/* 2^23 */

#define FCW_DEFAULT_VALUE_27025 0x0011789A	/* 27 Mhz */
#define FCW_DEFAULT_VALUE 		0x0011745D	/* 27 Mhz */
//#define FCW_DEFAULT_VALUE (UINT32)(KHZ_TO_FCW(24000))
#define FCW_PREFIX			0

#define FREQ_THRESHOLD_KHZ 						100
#define FREQ_THRESHOLD_KHZ_WITH_MAX_IGNORE	    5

#define PWM_DEBUG_LEVEL		2

#define FCW_WINDOW_SZ		8000

#define FCW_SZ_STEP 		0x9			/* 정확하게 계산하면 8.67 정도 됨 */

#define	PWM_MAX_DIFF_IGNORE		 36		/* 36 unit in 45 Khz PCR Frequency	  */
#define	PWM_MIN_DIFF_IGNORE		  4		/*  4 unit in 45 Khz PCR Frequency	  */
#define	PWM_DELTA_WINDOW		 24		/* Window size of direction filtering */
#define PWM_SUM_WINDOW			  4		/* Number of sum windows              */

#define MTS_HUGE_PCR_DIFFERENCE  (45*5000) /* <-- For stream wrap arround , 45*5000ms 5 sec --- 45MHz 020524 */

//#define MAX_DELTA_CLK 				(2 * 45)
#define MAX_DELTA_CLK 				(2* 45 * 10)

/*---------------------------------------------------------
 * Macro definition
---------------------------------------------------------*/
#define FCW_TO_KHZ(fcw)	( ((fcw) & 0x1FFFFF) * DCO_INPUT_CLOCK  / ( DCO_COEF / 1000 ))
#define KHZ_TO_FCW(khz)	( (khz)  * (DCO_COEF / DCO_INPUT_CLOCK) / 1000)

/* SINT64 divide macro with do_div */
#define DO_DIV_SINT64(n, div) { \
	if(n < 0 ) \
	{ \
		UINT64	u = (UINT64) (-n); \
		do_div(u, div); \
		n = (SINT64) (-u); \
	} \
	else \
		do_div(n, div); \
};
/*---------------------------------------------------------
    External 변수와 함수 prototype 선언
    (External Variables & Function Prototypes Declarations)
---------------------------------------------------------*/

/*---------------------------------------------------------
    Static 변수와 함수 prototype 선언
    (Static Variables & Function Prototypes Declarations)
---------------------------------------------------------*/
/*prototypes*/
static UINT32		nDeltaChkCount	= 0;	/* No. of data in nPwmWindow  */

static UINT32		_TimeStamps[PWM_SUM_WINDOW+1];		/* 시간  */
static SINT64		_MeanOfDelta[PWM_SUM_WINDOW*2];	/* pcr - stc의 차이를 저장한 nSumOfDelta를 시간으로 나눈 값 */
static UINT32		_AbsOfDelta[PWM_SUM_WINDOW*2];		/* pcr - stc의 차이를 저장한 nSumOfDelta를 시간으로 나눈 값의 절대값 */

static UINT32		*nTimeStamps = &_TimeStamps[1];	/* [1]을 가리키는 이유는 nTimeStamps[-1]로 이전 시간을 얻기 위해서다 */
static UINT32		*nAbsOfDelta = &_AbsOfDelta[PWM_SUM_WINDOW];
static SINT64		*nMeanOfDelta = &_MeanOfDelta[PWM_SUM_WINDOW];

static SINT32		nSumOfDelta[PWM_SUM_WINDOW];		/* pcr - stc의 차이를 저장한 값 */
static SINT32		nMstElapsed[PWM_SUM_WINDOW];		/* _timeStamp 값들의 차이값.. 즉, nSumOfDelta를 모을 때 걸린  시간. 이 값으로 _MeanofDelta나누어서 freq를 측정한다. */


static pwm_context_t pwm_context = {
	FALSE,				  // service
	FALSE,				  // recovery_Enable
	FALSE,				  // first_pcr_arrived
	    0,				  // reserved
		0,				  // fcw_base
	    0,				  // fcw_offset
	    0,				  // fcw_value
	    0,				  // lastUpdateTime
	    5,                // new_reset_discard
	    3,				  // new_adjust_skip
	    0,				  // skip_chk_overshoot
	    0,				  // fcw_trickSpeed
	    0				  // max_ignore_count
	};



UINT32 SDEC_IO_SettingFCW(S_SDEC_PARAM_T *stpSdecParam, UINT32 new_fcw_value)
{
	UINT32	ui32stcReal_32 = 0, ui32stcReal_31_0 = 0;

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );

	/* 2013. 09. 25. jinhwan.bae
	    CPU TOP Register Should Be SET to run with NEW DCO Value
	    CPU_TOP register base and offset is same at H14, M14B0, H13B0
	    DCO Set CTOP Register are different, H14, M14
	    H14 : 0x154 (CTR85)
	    M14-B0 : 0x5C (CTOP_TOP CTR23) */

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if(lx_chip() == LX_CHIP_H15)
	{
		SDTOP_Wr(dco_fcw_ctrl, new_fcw_value);
		SDTOP_WrFL(dco_fcw_ctrl);

		SDTOP_RdFL(stcc0_real1);
		SDTOP_RdFL(stcc0_real32);
		SDTOP_Rd(stcc0_real1, ui32stcReal_32);
		SDTOP_Rd(stcc0_real32, ui32stcReal_31_0);

		SDTOP_Wr(stcc0_ref1, ui32stcReal_32);
		SDTOP_Wr(stcc0_ref32, ui32stcReal_31_0 + 0x10);
		SDTOP_WrFL(stcc0_ref1);
		SDTOP_WrFL(stcc0_ref32);

		OS_MsecSleep(1);
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H14, A0))
	{
		CTOP_CTRL_H14_WRITE(0x154, new_fcw_value);

		////////////////////////////////////////////////////////////////////////////
		/* Setting DCO Value */
		/* STC Referrence - will be set time which will apply DCO to system */
		/* Actually, During Read/Write, All DCO Related Function Control Needed.ex. Stop, Set and Restart */
		/* But even if not, Read/Write Action is needed to adapt to the DCO Value really */
		/* Before Read/Write, DCO Value is not adapted really */
		/* read real STC value */
		ui32stcReal_32		= CPU_TOP_H14_READ(0x150);
		ui32stcReal_31_0	= CPU_TOP_H14_READ(0x154);

		/* setting referrence STC value with real stc + 0x10. */
		CPU_TOP_H14_WRITE(0x148, ui32stcReal_32);
		CPU_TOP_H14_WRITE(0x14C, ui32stcReal_31_0 + 0x10);
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		CTOP_CTRL_M14B0_WRITE(TOP, 0x5C, new_fcw_value);

		////////////////////////////////////////////////////////////////////////////
		/* Setting DCO Value */
		/* STC Referrence - will be set time which will apply DCO to system */
		/* Actually, During Read/Write, All DCO Related Function Control Needed.ex. Stop, Set and Restart */
		/* But even if not, Read/Write Action is needed to adapt to the DCO Value really */
		/* Before Read/Write, DCO Value is not adapted really */
		/* read real STC value */
		ui32stcReal_32		= CPU_TOP_M14_READ(0x150);
		ui32stcReal_31_0	= CPU_TOP_M14_READ(0x154);

		/* setting referrence STC value with real stc + 0x10. */
		CPU_TOP_M14_WRITE(0x148, ui32stcReal_32);
		CPU_TOP_M14_WRITE(0x14C, ui32stcReal_31_0 + 0x10);

	}
	else if (lx_chip_rev() >= LX_CHIP_REV(M14, A0))
	{
		CTOP_CTRL_M14_WRITE(0x154, new_fcw_value);

		////////////////////////////////////////////////////////////////////////////
		/* Setting DCO Value */
		/* STC Referrence - will be set time which will apply DCO to system */
		/* Actually, During Read/Write, All DCO Related Function Control Needed.ex. Stop, Set and Restart */
		/* But even if not, Read/Write Action is needed to adapt to the DCO Value really */
		/* Before Read/Write, DCO Value is not adapted really */
		/* read real STC value */
		ui32stcReal_32		= CPU_TOP_M14_READ(0x150);
		ui32stcReal_31_0	= CPU_TOP_M14_READ(0x154);

		/* setting referrence STC value with real stc + 0x10. */
		CPU_TOP_M14_WRITE(0x148, ui32stcReal_32);
		CPU_TOP_M14_WRITE(0x14C, ui32stcReal_31_0 + 0x10);

	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if (lx_chip_rev() >= LX_CHIP_REV(H13, A0))
	{
		CTOP_CTRL_H13_WRITE(0x154, new_fcw_value);

		////////////////////////////////////////////////////////////////////////////
		/* Setting DCO Value */
		/* STC Referrence - will be set time which will apply DCO to system */
		/* Actually, During Read/Write, All DCO Related Function Control Needed.ex. Stop, Set and Restart */
		/* But even if not, Read/Write Action is needed to adapt to the DCO Value really */
		/* Before Read/Write, DCO Value is not adapted really */
		/* read real STC value */
		ui32stcReal_32		= CPU_TOP_H13_READ(0x150);
		ui32stcReal_31_0	= CPU_TOP_H13_READ(0x154);

		/* setting referrence STC value with real stc + 0x10. */
		CPU_TOP_H13_WRITE(0x148, ui32stcReal_32);
		CPU_TOP_H13_WRITE(0x14C, ui32stcReal_31_0 + 0x10);
	}
#endif
	else
	{
		SDEC_PRINT_ERROR("Invalid Chip : 0x%x\n", lx_chip_rev());
	}

	return RET_OK;
}


#define NORMAL_STC 0x0011743D

int SDEC_IO_SetSTCMultiply(S_SDEC_PARAM_T *stpSdecParam, UINT32 *pui32Arg)
{
	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return RET_INVALID_PARAMS, "Invalid parameters" );
	LX_SDEC_CHECK_CODE( pui32Arg == NULL, return RET_INVALID_PARAMS, "Invalid argument" );

	pwm_context.fcw_trickSpeed = ((*pui32Arg) * NORMAL_STC) / 10;
	pwm_context.fcw_trickSpeed |= FCW_PREFIX;

	if((*pui32Arg) != 10)
		SDEC_IO_SettingFCW(stpSdecParam, pwm_context.fcw_trickSpeed);
	else
		SDEC_IO_SettingFCW(stpSdecParam, pwm_context.fcw_value);

	return RET_OK;
}


/**
 * pwm status clear and data structure init.
 *
 * @param 		fResetCount   pwm reset value
 * @return		void
 * @ingroup sdec_pwm_func
 */
static void pwm_context_reset(S_SDEC_PARAM_T *stpSdecParam, UINT8 cur_ch)
{
	int		i;
	UINT8 	core = 0;

	SDEC_PRINT_PCR( "pwm_context_reset CH[%d]",cur_ch);

	SDEC_CONVERT_CORE_CH(core, cur_ch);

	for (i = 0; i < PWM_SUM_WINDOW; i++)
		nSumOfDelta[i] = 0;

	pwm_context.new_reset_discard = 5;
	pwm_context.new_adjust_skip   = PWM_SUM_WINDOW-1;
	pwm_context.fcw_value = pwm_context.fcw_base = pwm_fcw_default;
	pwm_context.stable_overshoot = 0;

	max_fcw_value = pwm_context.fcw_base + (pwm_fcw_window_sz * pwm_fcw_sz_step);
	min_fcw_value = pwm_context.fcw_base - (pwm_fcw_window_sz * pwm_fcw_sz_step);

	SDEC_IO_SettingFCW(stpSdecParam, pwm_context.fcw_value);

	nDeltaChkCount		= 0;

	SDEC_HAL_STCCEnableCopy(core, cur_ch, SDEC_HAL_ENABLE);

#ifdef __NEW_PWM_RESET_COND__
	pcr_error_for_reset = 0;
#endif
}

/**
 *
 *  Initialisation function, resets hardware on first call
 *
 *  @return			void
 *
 * 	@ingroup sdec_pwm_func
 */
void SDEC_PWM_Init(S_SDEC_PARAM_T	*stpSdecParam )
{
	SDEC_PRINT_PCR( "SDEC_PWM_Init\n");

	if(0) // --> fix to original 27M configuration (lx_chip_rev() >= LX_CHIP_REV(M14, B0))
	{
		pwm_fcw_default		= ( FCW_DEFAULT_VALUE_27025 | FCW_PREFIX );
	}
	else
	{
		pwm_fcw_default		= ( FCW_DEFAULT_VALUE | FCW_PREFIX );
	}
	pwm_fcw_threshold		= KHZ_TO_FCW(FREQ_THRESHOLD_KHZ);
	pwm_fcw_threshold_with_max_ignore = KHZ_TO_FCW(FREQ_THRESHOLD_KHZ_WITH_MAX_IGNORE);
	pwm_fcw_sz_step		= FCW_SZ_STEP;
	pwm_fcw_window_sz		= FCW_WINDOW_SZ;

	pwm_context_reset(stpSdecParam, 0);
	pwm_context.first_pcr_arrived = FALSE;

}


#ifdef __NEW_PWM_RESET_COND__	/* jinhwan.bae for NEW PWM Reset Condition */
/**
********************************************************************************
* @brief
*   Set PWM Reset Condition
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
* @return
*  int
********************************************************************************
*/
void SDEC_IO_SetPWMResetCondition(S_SDEC_PARAM_T *stpSdecParam, UINT8 ch, BOOLEAN reset)
{

	if(pcr_error_for_reset > 0)
	{
		SDEC_PRINT_PCR( "Set PWM Reset Condition CH[%d] Reset[%d] pcr_error_for_reset[%d] ", ch, reset, pcr_error_for_reset );
	}

	/* Reset Condition */
	if(reset)
	{
		if(pcr_error_for_reset < MAX_ERROR_FOR_PWM_RESET * 2)
		{
			pcr_error_for_reset++;
		}

		if( pcr_error_for_reset > MAX_ERROR_FOR_PWM_RESET )
		{
			SDEC_PRINT_PCR( "PWM Reset by Condition");
			pwm_context_reset(stpSdecParam, ch);
		}
	}
	else /* Normal Case */
	{
		if( pcr_error_for_reset > 0 )
		{
			pcr_error_for_reset--;
		}
	}
}
#endif

/**
********************************************************************************
* @brief
*   Get Download Status in SDEC Driver
* @remarks
*   It's a temporal function for H13, Netcast 4.0 .
*   It's only for detecting Delayed Play mode in Time Shift
*   Return Value is used for work around of our stupid upload speed
* @par requirements:
*
* @param
* @return
*  int
********************************************************************************
*/

/* SHOULD BE MODIFIED in M14_TBD, H14_TBD */
static BOOLEAN SDEC_IsDownloading(UINT8 ch, S_SDEC_PARAM_T *stpSdecParam)
{
	int i, check_ch = 0;
	BOOLEAN retval = FALSE;

	if(ch == 0) check_ch = 1;
	else if(ch == 1) check_ch = 0;
	else return FALSE;

	for(i=0;i<4;i++)
	{
		if(stpSdecParam->stPIDMap[check_ch][i].used == 0x1)
		{
			retval = TRUE;
			break;
		}
	}

	return retval;
}


/**
********************************************************************************
* @brief
*   PCR Recovery Algorithm Routine
* @remarks
*   HISTORY
*	jinhwan.bae
*	This Routine had been composed at the time of beginning of DTV development in LGE.
*	Now, nobody remember exactly about history of algorithm.
*	It's one of the reason why we can't modify with instantaneous temptation.
*	It's LG's own algorithm accumulated many years.
*	- 2012. 11. 16
*	Inserted one thing to send the command for freerunning with invalid PCR stream
*	Main Target Stream is 330MHz_SR6956_256QAM_STV_Estonia.ts
*	Recovery Characteristics
*		- Almost time : after 500ms from previous PCR interrupt, PCR-STC > 20ms
*		- Sometimes  : corrected pwm value is more than 100 KHz
*	Condition for Free Run
*		- insert delta_error : raising at > 20ms, falling at first pwm correction
*
* @par requirements:
* @param
* @return
*  int
********************************************************************************
*/
static void action_pcr(S_SDEC_PARAM_T	*stpSdecParam, UINT8 cur_ch, UINT32 cur_pcr, UINT32 cur_stc)
{
	static SINT32	prev_pcr, prev_stc, prev_jit;
	UINT32 		cur_fcw_value = pwm_context.fcw_value;
	UINT32 		new_fcw_value = pwm_context.fcw_value;
	UINT32		capture_ms = OS_GetMsecTicks();
	SINT32		sDelta_clk;
	UINT32		uDelta_clk;
	SINT32		pcr_diff = 0;
	SINT32		stc_diff = 0;
	SINT32		pcr_jitt = 0;
	BOOLEAN		is_delayedplay = FALSE;
	UINT8		core = 0, org_ch = cur_ch;
	LX_SDEC_CFG_T	*cfg = SDEC_CFG_GetConfig();

	SDEC_CONVERT_CORE_CH(core, cur_ch);

	/* Check if the PCR Recovery @ channel is enabled or not */
	if ( stpSdecParam->bPcrRecoveryFlag[org_ch] == FALSE )
	{
		SDEC_PRINT_PCR( "No need to check , PCR Recovery is not Started CH[%d]", org_ch);
  		return;
	}

	/* It's not needed if we use 27Mhz fixed clock as pvr clock source */
	if(cfg->noFixedClock)
	{
		UINT32		cur_ch_src = 0;

		/* jinhwan.bae there is only copy and reset by jitter in Delayed Play Mode */
		/* How about remove PCR Recovery at the time of Uploading ? M14_TBD, H14_TBD  */
		/* read uploading status */
		SDEC_HAL_CIDCRead(core, cur_ch, &cur_ch_src);
		cur_ch_src = ((cur_ch_src >> 20) & 0xF);
		if((cur_ch_src == 0x6) || (cur_ch_src == 0x7))
		{
			if( TRUE == SDEC_IsDownloading(cur_ch, stpSdecParam) )
			{
				SDEC_PRINT_PCR( "\x1b[32m" "uploading mode" "\x1b[0m");
				is_delayedplay = TRUE;
			}
		}
	}

	sDelta_clk = (SINT32)cur_pcr - (SINT32)cur_stc;
	uDelta_clk = abs(sDelta_clk);

	/* After pwm_context_reset, we would 5 pcr interrupt to start recovery  */
	if(pwm_context.new_reset_discard != 0)
	{
		pwm_context.new_reset_discard--;
		if(pwm_context.new_reset_discard == 0)
		{
			prev_pcr = cur_pcr;
			prev_stc = cur_stc;
			pwm_context.lastUpdateTime = capture_ms;
			pwm_context.max_ignore_count = 0;
			SDEC_PRINT_PCR( "5th pcr interrupt after reset arrived. start PCR recovery at next time");
		}
		return;
	}

	/* Calulate PCR/STC difference, 45KHz Based */
    pcr_diff = cur_pcr - prev_pcr;
	stc_diff = cur_stc - prev_stc;
	pcr_jitt = cur_pcr - cur_stc;

	SDEC_PRINT_PCR( "CH[%d],(%5d KHz), FCW[0x%08x]FCW_OFFSET[%6ld], PCR[0x%08x]-STC[0x%08x]=[%d]",
			org_ch,	FCW_TO_KHZ(cur_fcw_value), cur_fcw_value, cur_fcw_value-pwm_context.fcw_base, cur_pcr, cur_stc , pcr_jitt);

	/* Reduce the frequency of PCR Recovery Based on PCR difference. 2000 was from SAA7219 Reference driver , now use 2250 */
	if (abs(pcr_diff) < 2250)	/* 50 ms */
	{
		return;
	}

	/* No Recovery @ TimeShift Delayed Play Mode, Only Check Reset Condition */
	if (is_delayedplay == TRUE)
	{
		prev_pcr = cur_pcr;
		prev_stc = cur_stc;
		prev_jit = pcr_jitt;

		if( abs(pcr_diff) >= MTS_HUGE_PCR_DIFFERENCE)
		{
			SDEC_PRINT_PCR( "DPMode: CurPCR[0x%08x]-PrevPCR[0x%08x]=Diff(%d) TOO BIG > MTS_HUGE_PCR_DIFFERENCE -> reset", cur_pcr, prev_pcr, pcr_diff);
			pwm_context_reset(stpSdecParam, org_ch);
		}
		return;	/* No Recovery @ TimeShift Delayed Play Mode */
	}

	/* --- Calculate the error, and a correction value, the correction value is based on the VCXO specs,
		   and should cause the error value to fall to zero in approximately 1 second,
		   ie an adjustment of 1 digit should result in a modification to the clock of approximately 64 cycles per second.
		   NOTE if we are closely synchronised then the correction value may be zero.                         --- */
	if( abs(pcr_diff) >= MTS_HUGE_PCR_DIFFERENCE)
	{
		/* Current PCR - Previous PCR Check */
		SDEC_PRINT_PCR( "CurPCR[0x%08x]-PrevPCR[0x%08x]=Diff(%d) TOO BIG > MTS_HUGE_PCR_DIFFERENCE -> reset", cur_pcr, prev_pcr, pcr_diff);
		pwm_context_reset(stpSdecParam, org_ch);
		return;
	}
	else if(uDelta_clk >= MTS_HUGE_PCR_DIFFERENCE)
	{
		/* Current PCR - Current STC Check */
		SDEC_PRINT_PCR( "PCR-STC=(%d) TOO BIG > MTS_HUGE_PCR_DIFFERENCE -> reset", uDelta_clk);
		pwm_context_reset(stpSdecParam, org_ch);
		return;
	}

	/*----- Main Branch Routine -----*/
	if (pwm_context.lastUpdateTime && (uDelta_clk < PWM_MAX_DIFF_IGNORE))
	{
		/* pwm_context.lastUpdateTime : not 0 @ first trial of PCR Recovery after reset. If at least 1 FCW adjust(PCR Recovery) is done after reset, lastUpdateTime is 0 */
		/* If first trial and PCR-STC is less than PWM_MAX_DIFF_IGNORE(36), no recovery is needed */
		/* BYPASS Condition */
		/* jinhwan.bae . BUT if the max_ignore_value is big, the next FCW adjust may be burden of total recovery, because so big */
		SDEC_PRINT_PCR( "small PCR-STC. save & return. max_ignore_count [%d]", pwm_context.max_ignore_count);

#ifdef __NEW_PWM_RESET_COND__
		/* jinhwan.bae, Normal and Recovery Sequence, set --  */
		SDEC_IO_SetPWMResetCondition( stpSdecParam, org_ch, FALSE);
#endif
		if(pwm_context.max_ignore_count < 100) /* jinhwan.bae temporally limit to 100, FIXME in M14~) */
		{
			pwm_context.max_ignore_count++;
		}

		/* Goto Save Current PCR, STC Value and Return , Wait Next PCR */
	}
	else if (pwm_context.lastUpdateTime && ((capture_ms - pwm_context.lastUpdateTime) <= 500))
	{
		/* If first trial and rapid arrival of PCR, wait Next PCR to make less the calculation error */
		/* It means, bypass current PCR data if the pcr sample time is small than 500 */
		/* This routine also prevent from "divide by zero" */
		SDEC_PRINT_PCR( "PCR arrival is so rapid after reset waiting(%ld) < 500. Bypass and wait Next PCR",(capture_ms - pwm_context.lastUpdateTime));
	}
	else if(uDelta_clk >= MAX_DELTA_CLK)
	{
		SDEC_PRINT_PCR( "|PCR-STC| > MAX_DELTA_CLK, update reset condition");
		/* jinhwan.bae, Error Case but need to waiting */
		SDEC_IO_SetPWMResetCondition( stpSdecParam, org_ch, TRUE);
	}
	else if (pwm_context.lastUpdateTime)
	{
		UINT32 elapsed_ms;
		SINT32 adjust;
		SINT32 coef = (27000000/45000);

		/*---------- condition reached to first recovery after reset -----------*/
		/* uDelta_clk is based on 45Khz clock	*/
		if(capture_ms > pwm_context.lastUpdateTime)
			elapsed_ms = capture_ms - pwm_context.lastUpdateTime;
		else
			elapsed_ms = capture_ms + (0xffffffff - pwm_context.lastUpdateTime);

		SDEC_PRINT_PCR( "First Recovery after pwm_context_reset, elapsed ms(%5d) ",elapsed_ms);

		adjust = ( ((coef * uDelta_clk * 1000 ) / elapsed_ms) * pwm_fcw_sz_step ) / 250;

		if (sDelta_clk < 0)
			adjust = -adjust;

		SDEC_PRINT_PCR( "\x1b[31m" "estimated instantaneous frequency = %lu.%03lu Mhz" "\x1b[0m",
			FCW_TO_KHZ(pwm_context.fcw_value + adjust) / 1000, FCW_TO_KHZ(pwm_context.fcw_value + adjust) % 1000);

		/*---------- IF adjustment is error case, wait next pcr ------------*/
		if(pwm_context.max_ignore_count > 10 )
		{
			/* it means, lots of stable pcr were arrived before first recovery after pwm_context_reset. */
			/* so if the current adjust is so big, skip the adjustment (it may jitter), and wait next pcr */
			/* The state(condition) leaves as lastupdatetime, it means, wait next pcr at first recovery state */
			/* jinhwan.bae this routine is made 2013. 03. for protecting timeshift clock fluctuation */
			/* it means, if we block pcr recovery at timeshift, this case never happen */
			if( abs(adjust) > pwm_fcw_threshold_with_max_ignore )
			{
				SDEC_PRINT_PCR( "\x1b[31m" "adjust[%d]>threshold[%d] It May Jitter! first recovery after reset and stable max ignore pcr!" "\x1b[0m",
					adjust,	pwm_fcw_threshold_with_max_ignore);

				prev_pcr = cur_pcr;
				prev_stc = cur_stc;
				prev_jit = pcr_jitt;
				return;
			}
		}
		else
		{
			/* we dicide, recovery is needed. but the adjustment value is bigger than threshold, reset pwm and wait next recovery */
			if( abs(adjust) > pwm_fcw_threshold)
			{
				SDEC_PRINT_PCR( "\x1b[31m" "adjust[%d]>threshold[%d] reset!" "\x1b[0m",
					adjust,	pwm_fcw_threshold);

				pwm_context_reset(stpSdecParam, org_ch);
				return;
			}
		}

 		/*--------  First Adjustment !! --------*/
		pwm_context.fcw_value			+= (adjust * 103 ) / 100;
		pwm_context.lastUpdateTime		 = 0;
		pwm_context.new_adjust_skip		 = PWM_SUM_WINDOW-1;
		pwm_context.skip_chk_overshoot	 = PWM_SUM_WINDOW;
		nTimeStamps[-1] = capture_ms;

		new_fcw_value = pwm_context.fcw_value;

		if      (new_fcw_value < min_fcw_value) new_fcw_value = min_fcw_value;
		else if (new_fcw_value > max_fcw_value) new_fcw_value = max_fcw_value;

		SDEC_PRINT_PCR( "\x1b[33m" "FIRST Adjustment! CH[%d]ADJ[%6d]FCWDiff[%6ld]NewFCW[0x%08x]CurFCW[%08x]Elapsedms[%5d]PCR[%08x]-STC[%08x]=[%d]" "\x1b[0m",
					cur_ch,	adjust,	new_fcw_value-pwm_context.fcw_base,	new_fcw_value, cur_fcw_value, elapsed_ms, cur_pcr, cur_stc , pcr_jitt);

		SDEC_IO_SettingFCW(stpSdecParam, new_fcw_value);

		pwm_context.fcw_value = new_fcw_value;
#ifdef __NEW_PWM_RESET_COND__
		/* jinhwan.bae, Normal and Recovery Sequence, set --  */
		SDEC_IO_SetPWMResetCondition( stpSdecParam, org_ch, FALSE);
#endif
	}
	else
	{
		UINT32	changeMask;
		SINT32	adjust = 0;
		SINT32	nSignOfDelta;
		SINT32	nSignOfSlope, nDiffSlope;
		UINT32	index, o_idx;
		int		i, nOvershoot = 0;

		/*---------- condition reached to recovery after first recovery, normal case -----------*/

		o_idx	= nDeltaChkCount / PWM_DELTA_WINDOW;
		index	= (o_idx % PWM_SUM_WINDOW);

		nSumOfDelta[index] += sDelta_clk;
		nDeltaChkCount++;

#ifdef __NEW_PWM_RESET_COND__
		/* jinhwan.bae, Normal and Recovery Sequence, set --  */
		SDEC_IO_SetPWMResetCondition( stpSdecParam, org_ch, FALSE);
#endif
		if ((nDeltaChkCount % PWM_DELTA_WINDOW) == 0)
		{
			nTimeStamps[index]		= capture_ms;
			nMstElapsed[index]		= nTimeStamps[index] - nTimeStamps[index-1];
			nMeanOfDelta[index] 	= 600 * 1000 * (SINT64) nSumOfDelta[index];
			DO_DIV_SINT64(nMeanOfDelta[index],PWM_DELTA_WINDOW);
			DO_DIV_SINT64(nMeanOfDelta[index], nMstElapsed[index]);
			nAbsOfDelta[index]		= abs(nMeanOfDelta[index]);

			if (pwm_context.new_adjust_skip > 0)
				pwm_context.new_adjust_skip--;

			if (o_idx >= PWM_SUM_WINDOW)	/* after normal pcr recovery and has 4 valid sum window */
			{
				nSignOfDelta  = ((nMeanOfDelta[index] >= 0) ? 1 : -1);

				nSignOfSlope  = ((nAbsOfDelta[index-3] > nAbsOfDelta[index-4]) ? 0x1000 : 0);
				nSignOfSlope |= ((nAbsOfDelta[index-2] > nAbsOfDelta[index-3]) ? 0x0100 : 0);
				nSignOfSlope |= ((nAbsOfDelta[index-1] > nAbsOfDelta[index-2]) ? 0x0010 : 0);
				nSignOfSlope |= ((nAbsOfDelta[index-0] > nAbsOfDelta[index-1]) ? 0x0001 : 0);

				changeMask	  = ((nMeanOfDelta[index-3] > 0) ? 0x1000 : 0);
				changeMask	 |= ((nMeanOfDelta[index-2] > 0) ? 0x0100 : 0);
				changeMask	 |= ((nMeanOfDelta[index-1] > 0) ? 0x0010 : 0);
				changeMask	 |= ((nMeanOfDelta[index-0] > 0) ? 0x0001 : 0);

				nDiffSlope = ((int)nAbsOfDelta[index] - (int)nAbsOfDelta[index-4])/PWM_SUM_WINDOW;
#if 0
				SDEC_PRINT_PCR( "nMeanOfDelta[%lld, %lld, %lld, %lld]",
					nMeanOfDelta[index-3],	nMeanOfDelta[index-2],	nMeanOfDelta[index-1],	nMeanOfDelta[index-0]);
				SDEC_PRINT_PCR( "new_adjust_skip[%lu] changeMask[0x%04x] nAbsOfDelta[%d], nSignOfSlope[0x%04x] nOvershoot[%d]",
					pwm_context.new_adjust_skip, changeMask, nAbsOfDelta[index], nSignOfSlope, nOvershoot);
#endif
				if (pwm_context.skip_chk_overshoot == 0)
				{
					for (i = 0; i < PWM_SUM_WINDOW; i++)
					{
					  if (nAbsOfDelta[index-i] > 2700) nOvershoot++;
					}
				}

				if (pwm_context.new_adjust_skip > 0)
				{
					/* *	Skip 3 times just after last adjustment	 */
				}
				else if ((changeMask == 0x1110) || (changeMask == 0x1))
				{
					/* * If difference sign is changed (+ -> - or - -> +), adjust Center Frequency */
					int diff = (nMeanOfDelta[index] - nMeanOfDelta[index-4])/PWM_SUM_WINDOW;

					pwm_context.new_adjust_skip = PWM_SUM_WINDOW-1;
					pwm_context.skip_chk_overshoot = 0;
					adjust = (diff * 103 * pwm_fcw_sz_step) / (250 * 100);

					SDEC_PRINT_PCR( "\x1b[32m" "CH[%d][ChangeMASK] adjust(%3d) diff[%4d]" "\x1b[0m",
									cur_ch,  adjust, diff);
					SDEC_PRINT_PCR( "[ch%d] nMeanOfDelta[%d]%lld] nMeanOfDelta[%d][%lld]",
									cur_ch,  index, nMeanOfDelta[index], index-4, nMeanOfDelta[index-4]);
				}
				else if (nAbsOfDelta[index] < 250)
				{
					/* *	System Clock has been almost converged, 	Nothing to do.	 */
					pwm_context.skip_chk_overshoot = 0;
				}
				else if (nSignOfSlope == 0x1111)
				{
					/* *	System Clock is going to diverge. MUST adjust FCW value to slope	 */
					pwm_context.new_adjust_skip = PWM_SUM_WINDOW-1;
					adjust = nSignOfDelta * (nDiffSlope * 120)  * pwm_fcw_sz_step / (250 * 100);

					SDEC_PRINT_PCR( "\x1b[32m" "CH[%d][Divergence] adjust(%3d) nSignOfDelta[0x%04x] nDiffSlope[%4d]" "\x1b[0m",
									cur_ch,  adjust, nSignOfDelta, nDiffSlope);
					SDEC_PRINT_PCR( "[ch%d] nMeanOfDelta[%d]%lld] nMeanOfDelta[%d][%lld]",
									cur_ch,  index, nMeanOfDelta[index], index-4, nMeanOfDelta[index-4]);
				}
				else if (nOvershoot >= 2)
				{
					/* *	Jitter is too big, speed up more. MUST adjust FCW value to slope 	 */
					int diff1 = (nMeanOfDelta[index] - nMeanOfDelta[index-4])/PWM_SUM_WINDOW;
					int diff2 = (nMeanOfDelta[index] / 8);

					if      ((diff1 ^ diff2) &    0x8000000) diff1 = 0;
					else if ((diff1 < 0) && (diff2 < diff1)) diff1 = diff2;
					else if ((diff1 > 0) && (diff2 > diff1)) diff1 = diff2;

					pwm_context.new_adjust_skip = PWM_SUM_WINDOW-1;
					pwm_context.skip_chk_overshoot = PWM_SUM_WINDOW;
					adjust = (diff1 * 120  * pwm_fcw_sz_step) / (250*100);

					SDEC_PRINT_PCR( "\x1b[32m" "CH[%d][JITTER][nOvershoot >= 2] adjust(%3d) diff1[%4d] diff2[%4d]" "\x1b[0m",
									cur_ch,  adjust, diff1, diff2);
					SDEC_PRINT_PCR( "[ch%d] nMeanOfDelta[%d]%lld] nMeanOfDelta[%d][%lld]",
									cur_ch,  index, nMeanOfDelta[index], index-4, nMeanOfDelta[index-4]);
				}
				else if ((changeMask != 0x1111) && (changeMask != 0x0000))
				{
					if (pwm_context.skip_chk_overshoot > 0)
						pwm_context.skip_chk_overshoot--;
				}
				else
				{
					/*
					 * 20141014 - ks.hyun
					 * chageMask가 1011 => 0110 => 1101 => 1011 => 0110 과 같은 패턴으로 서서히 증가하는 경우
					 * 제대로 된 처리가 되지 못함(이렇게 증가하다  if(uDelta_clk >= MAX_DELTA_CLK) 조건에 의해 리셋걸림)
					 *
					 * 기본적으로 nOvershoot 루틴으로 보정이 되어야 할것으로 보이나
					 * pwm_context.skip_chk_overshoot 가 0으로 설정이 되지 못하여 해당 조건을 충족하지 못함
					 *  skip_chk_overshoot을 만족하는 조건이 nAbsOfDelta[] < 250 인데 이 값이 너무 작음
					 *  nAbsOfDelta 가 27M 기반이라 시간으로 환산하면 10us
					 *
					 * 하지만 기존 구현 코드들을 변경할 경우 risk가 존재하여
					 * 특정 delta 값 이상에서 머무를 경우 pwm을 조정하도록 함
					 *
					 */
					#define MAX_STABLE_OVERSHOOT	(27*1000*5)		/* 5ms */
				 	if((changeMask == 0x1111 || changeMask == 0x0000) &&
						nAbsOfDelta[index] > MAX_STABLE_OVERSHOOT)
				 	{
			 			pwm_context.stable_overshoot++;
						if(pwm_context.stable_overshoot > 10)
						{
							SDEC_PRINT_PCR(GREEN_COLOR "Stable Overshoot" NORMAL_COLOR);
							pwm_context.new_adjust_skip = PWM_SUM_WINDOW-1;
							adjust = nSignOfDelta * (2000 * 103)  * pwm_fcw_sz_step / (250 * 100);	/* 2Khz shift */
						}
				 	}
					else
					{
						pwm_context.stable_overshoot = 0;
					}
				}


#if	0
				{
				int	idx_3 = ((index + PWM_SUM_WINDOW - 3) % PWM_SUM_WINDOW);
				int	idx_2 = ((index + PWM_SUM_WINDOW - 2) % PWM_SUM_WINDOW);
				int	idx_1 = ((index + PWM_SUM_WINDOW - 1) % PWM_SUM_WINDOW);

				SDEC_PRINT_PCR("INDEX: %d %d %d [%d]", idx_3, idx_2, idx_1, index);
				SDEC_PRINT_PCR("ELAPSED : [%4d, %4d, %4d, %4d]",
							nMstElapsed [idx_3], nMstElapsed [idx_2], nMstElapsed [idx_1], nMstElapsed [index]);
				SDEC_PRINT_PCR("MEAN    : [%lld, %lld, %lld,  %lld]",
							nMeanOfDelta[idx_3], nMeanOfDelta[idx_2], nMeanOfDelta[idx_1], nMeanOfDelta[index]);
				SDEC_PRINT_PCR("singOfDelta=%2d, ss=%04x, cm=%04x, diffSlop=%d, adjust=%d, overshoot=%lu",
							nSignOfDelta, nSignOfSlope,
							changeMask, nDiffSlope,	adjust, pwm_context.skip_chk_overshoot);
				}
#endif
				if (adjust == 0)
				{
					/* No action needed if adjust == 0 */
				}
				else
				{
					new_fcw_value += adjust;

					if		(new_fcw_value < min_fcw_value) new_fcw_value = min_fcw_value;
					else if (new_fcw_value > max_fcw_value) new_fcw_value = max_fcw_value;

					/* PWM Register Setting */
					SDEC_IO_SettingFCW(stpSdecParam, new_fcw_value);
					pwm_context.fcw_value = new_fcw_value;
					pwm_context.stable_overshoot = 0;

					SDEC_PRINT_PCR( "\x1b[34m" "CH[%d] NEW FCW, 0x%06x==>0x%06x, adjust(%3d) PCR[%08x]-STC[%08x]=%d" "\x1b[0m",
								cur_ch,	cur_fcw_value, new_fcw_value, adjust, cur_pcr,cur_stc , pcr_jitt );
				}
			}
			else
			{
				SDEC_PRINT_PCR("Waiting for the 2nd Recovery(%d/%d)", o_idx, PWM_SUM_WINDOW);
			}

			if (index == (PWM_SUM_WINDOW-1))
			{
				nTimeStamps[-1] = nTimeStamps[index];
			}

			nAbsOfDelta[index-4]  = nAbsOfDelta[index];
			nMeanOfDelta[index-4] = nMeanOfDelta[index];
			nSumOfDelta[(index+1)%PWM_SUM_WINDOW] = 0;
		}
	}

	prev_pcr = cur_pcr;
	prev_stc = cur_stc;
	prev_jit = pcr_jitt;

}

void SDEC_PCRRecovery(struct work_struct *work)
{
	S_SDEC_PARAM_T 	*stpSdecParam;
	UINT32 			ui32Curr_stc 				= 0x0;
	UINT32 			ui32Curr_pcr 				= 0x0;
	UINT8 			cur_ch 						= 0x0;

	stpSdecParam =	container_of (work, S_SDEC_PARAM_T, PcrRecovery);

	LX_SDEC_CHECK_CODE( stpSdecParam == NULL, return, "Invalid parameters" );

	cur_ch			= stpSdecParam->stPCR_STC.ui8Channel;
	ui32Curr_stc 	= stpSdecParam->stPCR_STC.STC_hi_value;
	ui32Curr_pcr 	= stpSdecParam->stPCR_STC.PCR_hi_value;

	action_pcr(stpSdecParam, cur_ch, ui32Curr_pcr,ui32Curr_stc);
}


