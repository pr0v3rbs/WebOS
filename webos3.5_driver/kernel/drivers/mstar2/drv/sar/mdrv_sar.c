////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¨MStar Confidential Information〃) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_sar.c
/// @brief  Real Time Clock (RTC) Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
#include <linux/autoconf.h>
//#include <linux/undefconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include "mst_devid.h"

#include "mdrv_sar.h"
#include "mhal_sar_reg.h"
#include "mhal_sar.h"
#include "../iic/mdrv_iic.h"

extern SAR_ModHandle_t SARDev;
//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define SAR_PRINT(fmt, args...)        //printk("[SAR][%05d] " fmt, __LINE__, ## args)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MOD_SAR_DEVICE_COUNT    1
#define SAR_POLL_PERIOD			20
#define SAR_TRANSIT_TIME		3
#define SAR_STABLE_CNT			10
#define SAR_STABLE_TIMEOUT		120		/* at least, greater than micom poll period */
#define SAR_REPEAT_TIME			120
#define SAR_REPEAT_TIMEOUT		420 //(SAR_REPEAT_TIME*2)

#define SAR_KEY_SINGLE			1
#define SAR_KEY_REPEAT			2

#define SAR_ADC_NOISE_THRD		6
#define SAR_ADC_CMP(x,y)		( ((int)(x) - (int)(y)) > SAR_ADC_NOISE_THRD || ((int)(x) - (int)(y)) < -SAR_ADC_NOISE_THRD ) ? FALSE:TRUE
										/* Noised Filtered ADC Comparation */

#define SAR_DEBUG
#ifdef SAR_DEBUG
#define DEBUG_SAR(x) (x)
#else
#define DEBUG_SAR(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef enum {
	SAR_FSM_IDLE,
	SAR_FSM_TRANSITION,
	SAR_FSM_STABLE,
	SAR_FSM_PRE_REPEAT,
	SAR_FSM_REPEAT
} SAR_FSM_STATE_T;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static struct task_struct *_gsar_task = NULL;
static wait_queue_head_t _wq_fsm;
static int	_bmicom_polled = 0;
static int	_bwait_fsm_repeat = 0;

static int  _bsar_pressed  = 0;

static SAR_FSM_STATE_T	_gsar_status_successed_exp = SAR_FSM_IDLE;
static SAR_FSM_STATE_T	_gsar_status_exp = SAR_FSM_IDLE;	/* fsm status exported to micom */
static U8 u8StorageChInfo[4] = {0xFF,0xFF,0xFF,0xFF};
static U8 u8StorageLevelInfo[4][4];
static U8 u8StorageKeyInfo[4][4];

static U8 PreviousCMD;

static U8 u8RepeatCount;
static U8 u8LastTouchScanData;
static U8 u8LastTouchScanMode;
static U8 u8WaferKeyType;

static U8 _gsar_cur_key	= 0xff;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static int _MDrv_SAR_Poll_Thread(void *data);
static BOOL _MDrv_SAR_MonitorChannel( U8 *ch, U8 *val);
static U8 _MDrv_SAR_GetChannelADC_Filtered(U8 ch);
static U8 _MDrv_SAR_ConvADCValToKey( U8 ch, U8 val );
#if 0
static irqreturn_t _MDrv_SAR_ISR(int irq, void *dev_id)
{
    U8	u8Key, u8RepeatFlag;

    if(MDrv_SAR_GetKeyCode(&u8Key, &u8RepeatFlag))
    {
        SAR_PRINT("_MDrv_SAR_ISR() -> %x, %x\n", u8Key, u8RepeatFlag);

       	if (SARDev.async_queue)
			wake_up_interruptible(&key_wait_q);
    		kill_fasync(&SARDev.async_queue, SIGRTMIN, POLL_IN);
    }

    return IRQ_HANDLED;
}
#endif

static BOOL _MDrv_SAR_MonitorChannel( U8 *ch, U8 *val)
{
	U8 ch_idx, sto_idx;
	U8 value;
	U8 cur_ch;

	/* For Each Valid ADC Channel */
	for (ch_idx=0; ch_idx<4; ch_idx++)
	{
		if (u8StorageChInfo[ch_idx] < MHal_SAR_GetChannelMaxId())
		{
			cur_ch = u8StorageChInfo[ch_idx];
			value = _MDrv_SAR_GetChannelADC_Filtered(cur_ch);

			if ( value >= u8StorageLevelInfo[ch_idx][3] )
				continue;

			/* For Each Storage Level */
			for (sto_idx=0; sto_idx<4; sto_idx++) {
				if ( value < u8StorageLevelInfo[ch_idx][sto_idx]) {
					*ch = cur_ch;
					*val = value;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

static U8 _MDrv_SAR_GetChannelADC_Filtered( U8 ch )
{
	U8 i, ret;
	U8 med_filter[3];	/* 3 tab median filter */

	for (i=0; i<3; i++)
		med_filter[i] = MHal_SAR_GetChannelADC(ch);

	if (med_filter[0] <= med_filter[1]) {
		if ( med_filter[1] <= med_filter[2])
			ret =  med_filter[1];
		else if ( med_filter[0] <= med_filter[2])
			ret =  med_filter[2];
		else
			ret =  med_filter[0];
	} else {
		if (med_filter[0] <= med_filter[2])
			ret =  med_filter[0];
		else if (med_filter[1] <= med_filter[2])
			ret =  med_filter[2];
		else
			ret =  med_filter[1];
	}

	for (i=0; i<4; i++) {
		if ( ch == u8StorageChInfo[i])
			break;
	}

	if ( i == 4 || ret > u8StorageLevelInfo[i][3] )
		ret = 0xff;

	return ret;
}

static U8 _MDrv_SAR_ConvADCValToKey( U8 ch, U8 val )
{
	U8 ch_idx, sto_idx;
	/* Get Ch to ch_idx */
	for (ch_idx=0; ch_idx<4; ch_idx++) {
		if (ch == u8StorageChInfo[ch_idx])
		break;
	}

	/* Unknown channel */
	if (ch_idx == 4) {
		printk ("unknown channel\n");
		return 0xFF;
	}

	for (sto_idx=0; sto_idx<4; sto_idx++) {
		if (val < u8StorageLevelInfo[ch_idx][sto_idx])
			return u8StorageKeyInfo[ch_idx][sto_idx];
	}

	/* Unknown ADC Value */
	printk ("unknown adc value %d, ch: %d\n", val, ch);
	return 0xFF;
}

static int _MDrv_SAR_Poll_Thread(void *data)
{
	U8 		value, key;
	U32		timeout;
	static	SAR_FSM_STATE_T _sar_status = SAR_FSM_IDLE;
	static	U8 _event_ch;
	static	U8 _event_val;
	static  U8 _stable_cnt = 0;

	while (1)
	{

		switch (_sar_status)
		{
			case SAR_FSM_IDLE:
				_gsar_status_exp = _sar_status;
				_stable_cnt = 0;
				msleep(SAR_POLL_PERIOD);

				if (_MDrv_SAR_MonitorChannel(&_event_ch, &_event_val)) {
					if ( !SAR_ADC_CMP(_event_val,0xff) )
						_sar_status = SAR_FSM_TRANSITION;
				}
				break;

			case SAR_FSM_TRANSITION:
				_gsar_status_exp = _sar_status;
				msleep(SAR_TRANSIT_TIME); //13 y tact key failing time 2ms

				value = _MDrv_SAR_GetChannelADC_Filtered(_event_ch);

				if (SAR_ADC_CMP(value,0xff)) {
					_sar_status = SAR_FSM_IDLE;
				}
				else
				{
					if (!SAR_ADC_CMP(value,_event_val)) {
						_sar_status = SAR_FSM_IDLE;
						break;
					}
					else
					{
						_event_val = value;
						_stable_cnt++;

						if( _stable_cnt >= SAR_STABLE_CNT)
							_sar_status = SAR_FSM_STABLE;
					}
				}

				break;

			case SAR_FSM_STABLE:
				value = _MDrv_SAR_GetChannelADC_Filtered(_event_ch);
				//final check
				if (!SAR_ADC_CMP(value,_event_val)) {
					_sar_status = SAR_FSM_IDLE;
					break;
				}


				key = _MDrv_SAR_ConvADCValToKey(_event_ch, value);
				if ( key == 0xff ) {
					_sar_status = SAR_FSM_IDLE;
					break;
				}

				_gsar_cur_key = key;

				if(u8LastTouchScanMode)
					MDrv_SAR_ReadTouchKeyScanData();


				_gsar_status_exp = _sar_status;;
				_gsar_status_successed_exp = _sar_status;
				_bsar_pressed = TRUE ;

				_bmicom_polled = 0;
				if (!wait_event_timeout(_wq_fsm, _bmicom_polled, SAR_STABLE_TIMEOUT)) {
					_sar_status = SAR_FSM_IDLE;
					break;
				}

				_sar_status = SAR_FSM_PRE_REPEAT;
				_gsar_status_exp = _sar_status;;

				break;

			case SAR_FSM_PRE_REPEAT:
				value = _MDrv_SAR_GetChannelADC_Filtered(_event_ch);
				if (!SAR_ADC_CMP(value,_event_val)) {
					_sar_status = SAR_FSM_IDLE;
					_bwait_fsm_repeat = 1;
					wake_up(&_wq_fsm);
					msleep (SAR_POLL_PERIOD);
					break;
				}

				timeout = 0;
				do
				{
					msleep (SAR_POLL_PERIOD);
					timeout += SAR_POLL_PERIOD;

					value = _MDrv_SAR_GetChannelADC_Filtered(_event_ch);
					if (!SAR_ADC_CMP(value,_event_val)) {
						_sar_status = SAR_FSM_IDLE;
						_bwait_fsm_repeat = 1;
						wake_up(&_wq_fsm);
						break;
					}

				} while ( timeout < SAR_REPEAT_TIMEOUT);

				if (_sar_status != SAR_FSM_PRE_REPEAT)
					break;

				_sar_status = SAR_FSM_REPEAT;
				_gsar_status_exp = _sar_status;

				_bwait_fsm_repeat = 1;
				wake_up(&_wq_fsm);
				break;


			case SAR_FSM_REPEAT:

				if(u8LastTouchScanMode){
					MDrv_SAR_ReadTouchKeyScanData();
					msleep(SAR_POLL_PERIOD*2);
				}
				else
					msleep(SAR_POLL_PERIOD);
				value = _MDrv_SAR_GetChannelADC_Filtered(_event_ch);
				if (!SAR_ADC_CMP(value,_event_val)) {
					_sar_status = SAR_FSM_IDLE;
				}

				break;

			default :
				_gsar_status_exp = SAR_FSM_IDLE;
				break;
		}
	}

	return 0;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void MDrv_SAR_Init(void)
{
    PreviousCMD = 0xFF;
    u8RepeatCount = 0;
	u8LastTouchScanMode = 0;
    //S32 s32Result;

    MHal_SAR_Config_SingleChannel(0x04);
    MHal_SAR_Config_TriggerMode(HAL_SAR_EDGE); //0:edge trigger, 1: level trigger
    MHal_SAR_Config_SingleChannelEn(DISABLE);
    MHal_SAR_Config_ShotMode(HAL_SAR_FREERUN);
    MHal_SAR_Config_Powerdown(HAL_SAR_ADC_POWERUP);
    MHal_SAR_Config_Start(DISABLE);
    MHal_SAR_Config_ADCPowerdown(DISABLE);
    MHal_SAR_Config_FreeRun(ENABLE);
    MHal_SAR_Config_Selection(DISABLE);
    MHal_SAR_Config_8Channel(ENABLE);
    MHal_SAR_Config_ClockSamplePeriod(0x05);

	MHal_SAR_SwPackEnable(ENABLE);//Luke
    MHal_SAR_Config_IntMask(DISABLE);
    MHal_SAR_Config_IntClear(DISABLE);
    MHal_SAR_Config_IntForce(DISABLE);
    MHal_SAR_Config_IntClear(ENABLE);
    MHal_SAR_Config_IntClear(DISABLE);

	MHal_SAR_SetInterrupt();//Luke
#if 0
    s32Result = request_irq(E_IRQ_KEYPAD, _MDrv_SAR_ISR, SA_INTERRUPT, "SAR", NULL);
    if (s32Result)
    {
        SAR_PRINT("SAR IRQ registartion ERROR\n");
	}
	else
    {
        SAR_PRINT("SAR IRQ registartion OK\n");
    }
#endif

	init_waitqueue_head(&_wq_fsm);
	if (!_gsar_task) {
		_gsar_task = kthread_run(_MDrv_SAR_Poll_Thread, NULL, "%s", "sar-polling");
		if (IS_ERR(_gsar_task))
			BUG();
	}

}

static U8 MDrv_SAR_GetChID(U8 u8ChIdx)
{
    if(u8ChIdx>=CFG_SAR_ADC_AVAILABLES)
        return 0xFF;
    return u8StorageChInfo[u8ChIdx];
}


static void MDrv_SAR_CfgAnalogInputs(void)
{
    U8 i,j;

    // configure analog input & output enable according to used channels
    for(j=0;j<CFG_SAR_ADC_CHANNELS;j++)
    {
        for(i=0;i<CFG_SAR_ADC_AVAILABLES;i++)
        {
            if(MDrv_SAR_GetChID(i) == j)
                break;
        }

        if(i<CFG_SAR_ADC_AVAILABLES)//matches
        {
            HAL_SAR_Config_AanlogInputSelect(j,ENABLE);
            HAL_SAR_Config_OptputEnable(j,DISABLE);
        }
        else
        {
            HAL_SAR_Config_AanlogInputSelect(j,DISABLE);
            HAL_SAR_Config_OptputEnable(j,ENABLE);
        }
    }
}


U8 MDrv_SAR_SetChInfo(SAR_CFG_t *sarChInfo)
{
    U8 i,j;
    U8 ret_val = FALSE;


    if(sarChInfo->u8SARChID >= MHal_SAR_GetChannelMaxId())
        return ret_val;


    for(i=0;i<4;i++)
    {
        if((u8StorageChInfo[i] == 0xFF) || (u8StorageChInfo[i] == sarChInfo->u8SARChID))
        {
            u8StorageChInfo[i] = sarChInfo->u8SARChID;
            for(j=0;j<sarChInfo->u8KeyLevelNum;j++)
            {
                u8StorageLevelInfo[i][j] = sarChInfo->u8KeyThreshold[j];
                u8StorageKeyInfo[i][j] = sarChInfo->u8KeyCode[j];
            }
            MHal_SAR_Config_ChannelBound(sarChInfo->u8SARChID,&sarChInfo->SARChBnd);
            ret_val = TRUE;
            break;
        }
    }
    MDrv_SAR_CfgAnalogInputs();

    return ret_val;
}
#define MHal_GPIO_REG(addr)             (*(volatile U8*)(REG_MIPS_BASE + (((addr) & ~1)<<1) + (addr & 1)))

void MDrv_SAR_SetTouchScanMode(U8 bonoff){


	u8LastTouchScanMode = bonoff;

	MHal_GPIO_REG(0x0e6a) = (MHal_GPIO_REG(0x0e6a)) & (~BIT3); //gpio set
	MHal_GPIO_REG(0x0e1f) = (MHal_GPIO_REG(0x0e1f)) | (BIT2);  //gpio output -> Read
	if( (MHal_GPIO_REG(0x0e23) & (BIT2) ) == (BIT2) )
	{
		u8WaferKeyType = WAFER_TYPE_11Y;
	}
	else
	{
		u8WaferKeyType = WAFER_TYPE_12Y;
	}

}

U8 MDrv_SAR_GetTouchKeyScanData(){
	return u8LastTouchScanData;
}

void MDrv_SAR_ReadTouchKeyScanData(){
	U8 data[2],cmpdata[2]={0},value[8],cmd,i;
	U32 tValue=0,max=1;
	S32 ret;

//	printk(" %d \n\n====\n",u8WaferKeyType);

	if(u8WaferKeyType == WAFER_TYPE_11Y)
	{

		data[0] = 0x23;
		data[1] = 1;
		MDrv_SW_IIC_Read(4,0x52,1,&(data[0]),1,&(data[1]));
			u8LastTouchScanData = data[1];
	}
	else if(u8WaferKeyType == WAFER_TYPE_12Y)
	{
		data[0] = 0x62;

		switch(_gsar_cur_key)
		{
			case 0x08 : //power
				data[1] 	= 0x90;
				cmpdata[0]  = 0x04;
				cmpdata[1]  = 0x00;

				break;
			case 0x0b ://input
				data[1] 	= 0x98;
				cmpdata[0]  = 0x08;
				cmpdata[1]  = 0x00;

				break;
			case 0x43: //menu
				data[1] 	= 0xA0;
				cmpdata[0]  = 0x10;
				cmpdata[1]  = 0x00;

				break;
			case 0x44: //enter
				data[1] 	= 0xA8;
				cmpdata[0]  = 0x20;
				cmpdata[1]  = 0x00;

				break;
			case 0x02: // v+
				data[1] 	= 0xB0;
				cmpdata[0]  = 0x40;
				cmpdata[1]  = 0x00;

				break;
			case 0x03: // v-
				data[1] 	= 0xB8;
				cmpdata[0]  = 0x80;
				cmpdata[1]  = 0x00;
				break;
			case 0x00: //c+
				data[1] 	= 0xC0;
				cmpdata[0]  = 0x00;
				cmpdata[1]  = 0x01;
				break;
			case 0x01: // c-
				data[1] 	= 0xC8;
				cmpdata[0]  = 0x01;
				cmpdata[1]  = 0x00;
				break;
		}


		MDrv_SW_IIC_Write(7,0x48,0,0,2,data);

		msleep(10);

		cmd = 0x50;

		for( i = 0 ; i < 100 ; i ++)
		{

			ret=  MDrv_SW_IIC_Read( 7 , 0x48, 1, &cmd, 8, value) ;

			if(value[0] == value[6] && cmpdata[0] == value[6] && value[1] == value[7]&& cmpdata[1] == value[1]
							&& value[2] <= 0x10 && value[3] <= 10)
			{
				tValue = ((tValue | value[3])<<8) ;
				tValue |=  (value[4]);
				if( max < tValue);
					max = tValue;
				tValue = 0;
			}
		}

		max *= 100;
		max /= 25;
		max /= 13;

		u8LastTouchScanData = (U8)max;
	}

}

U8 MDrv_SAR_CHGetKey(U8 u8Index, U8 u8Channel, U8 *u8Key , U8 *u8Repstatus)
{

	SAR_PRINT("MDrv_SAR_CHGetKey CALLED!!!\t u8Index=%d u8Channel=%d are obsolete\n", u8Index, u8Channel);

	return MDrv_SAR_GetKeyCode(u8Key, u8Repstatus);
}

void MDrv_SAR_SetPDPErrorPort(void)
{
	HAL_SAR_Config_AanlogInputSelect(3,ENABLE);
    HAL_SAR_Config_OptputEnable(3,DISABLE);
	HAL_SAR_Config_AanlogInputSelect(4,ENABLE);
    HAL_SAR_Config_OptputEnable(4,DISABLE);
}

// This function should be called after MDrv_SAR_Init() for PDP 17V & VS Error detection
// GP2R_20101026_jy.song : Touch Key HW Revison Read위한 ADC Port Control
U8 MDrv_SAR_GetADC_Value(U8 u8Channel)
{
	U8 i;
	U8 ADC_Value;
	U16 total = 0;
	U8 average = 0;

//	Channel = u8Channel;

	SAR_PRINT("MDrv_SAR_GetADC_Value CALLED!!!\t  u8Channel=%d\n",u8Channel);

	for ( i = 0; i < 10; i++ ) //10회 체크후 평균값을 올려준다.
	{
		ADC_Value = MHal_SAR_GetChannelADC(u8Channel);

		SAR_PRINT("i=%d ADC_Value=%d\n", i, ADC_Value);

		total = total + ADC_Value;

	}

	average = (total/10);

	return average;

}


U8 MDrv_SAR_GetKeyCode(U8 *u8Key, U8 *u8Repstatus)
{
	SAR_PRINT("MDrv_SAR_GetKeyCode CALLED!!!\n");

	if ( _gsar_status_exp == SAR_FSM_STABLE ) {
		*u8Key = _gsar_cur_key;
		*u8Repstatus = SAR_KEY_SINGLE;
		_bmicom_polled = 1;
		wake_up(&_wq_fsm);
		return TRUE;
	}

	if ( _gsar_status_exp == SAR_FSM_PRE_REPEAT ) {

		_bwait_fsm_repeat = 0;

		if (!wait_event_timeout(_wq_fsm, _bwait_fsm_repeat, SAR_REPEAT_TIMEOUT))
		{
			return FALSE;
		}
	}

	if ( _gsar_status_exp == SAR_FSM_REPEAT ) {
		*u8Key = _gsar_cur_key;
		*u8Repstatus = SAR_KEY_REPEAT;
		return TRUE;
	}
    
    return FALSE;
}


