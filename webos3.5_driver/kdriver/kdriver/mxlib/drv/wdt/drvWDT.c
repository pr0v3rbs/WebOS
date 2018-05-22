//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvWDT.c
/// @brief  Piu Watch Dog Timer Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Include Files
////////////////////////////////////////////////////////////////////////////////
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsDevice.h"
#include "MsVersion.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "drvMMIO.h"
#include "drvWDT.h"
#include "halWDT.h"
#include "regWDT.h"
#include "MsOS.h"
#if (WDT_UTOPIA20)
#include "utopia.h"
#endif
#define WDT_DBG_FUNC()            if (_geDbfLevel >= E_WDT_DBGLV_ALL) \
                                           {MS_DEBUG_MSG(printf("\t====   %s   ====\n", __FUNCTION__));}
#define WDT_DBG_INFO(x, args...)  if (_geDbfLevel >= E_WDT_DBGLV_INFO ) \
                                           {MS_DEBUG_MSG(printf(x, ##args));}
#define WDT_DBG_ERR(x, args...)   if (_geDbfLevel >= E_WDT_DBGLV_ERR_ONLY) \
										   {MS_DEBUG_MSG(printf(x, ##args));}
#if (WDT_UTOPIA20)
MS_BOOL bU20WDTOpened = FALSE;
void* pInstantWdt;
void* pAttributeWdt;
#endif
////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static tmr_interrupt tTmrTbl[MAX_TIMER_NUM];	/* Table of timers managed by this module		*/
static MS_BOOL  _gbInitWDT = FALSE;
static WDT_DbgLv _geDbfLevel = E_WDT_DBGLV_ERR_ONLY;
static MSIF_Version _drv_wdt_version = {
    .DDI = { WDT_DRV_VERSION },
};

inline static void _MDrv_TIMER_Trigger(E_PIU_Timer eTimer, MS_U32 u32count);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_WDT_GetLibVer
/// @brief \b Function \b Description: Show the WDT driver version
/// @param <IN>        \b None:
/// @param <OUT>       \b **pVerString : output WDT driver version
/// @param <RET>       \b eRETCODE :
/// @param <GLOBAL>    \b VER: Library version string
////////////////////////////////////////////////////////////////////////////////
WDT_Result MDrv_WDT_GetLibVer(const MSIF_Version **ppVersion)
{
    WDT_DBG_FUNC();

    if (!ppVersion)
        return E_WDT_FAIL;

    *ppVersion = &_drv_wdt_version;
    return E_WDT_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsEnable
/// @brief \b Function  \b Description: check if WDT is Enable
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_IsEnable(void)
{
    WDT_DBG_FUNC();

    _gbInitWDT = HAL_WDT_IsEnable();

    if (!_gbInitWDT)
        WDT_DBG_ERR("Call MDrv_WDT_Start first!\n");
    return (WDT_Result)_gbInitWDT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Init
/// @brief \b Function  \b Description: Init and Start WDT
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_Init(WDT_DbgLv eLevel)
{
    MS_U32 u32BaseAddr = 0;
    MS_U32 u32BaseSize = 0; /* No use */

    _geDbfLevel = eLevel;
    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);
    /*  This is not really necessary, WDT is enabled after MCU reset
        *   but it is important to get the IOMAP Base to access RIU.
        */
    if (!_gbInitWDT)
    {
        if(!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            WDT_DBG_ERR("Get IOMAP Base faill!\n");
            return E_WDT_FAIL;
        }

        HAL_WDT_SetIOMapBase(u32BaseAddr);
        HAL_WDT_Start();

        _gbInitWDT = TRUE;
        return E_WDT_OK;
    }
    else
    {
        WDT_DBG_ERR("WDT had initial!\n");
        return E_WDT_OK;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Stop
/// @brief \b Function  \b Description: Init WDT Stop
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_Stop(WDT_DbgLv eLevel)
{
    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_Stop();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_Clear
/// @brief \b Function  \b Description: clear WDT.
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_Clear(void)
{
    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_Clear();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_ClearRstFlag
/// @brief \b Function  \b Description: clear WDT reset flag.
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_ClearRstFlag(void)
{
    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_ClearRstFlag();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_IsReset
/// @brief \b Function  \b Description: check if WDT is reset
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_IsReset(void)
{
    WDT_Result eWDTRe = E_WDT_FAIL;

    eWDTRe = (WDT_Result)HAL_WDT_IsReset();

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_SetTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer(sec);

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: msec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_SetTimer_ms(WDT_DbgLv eLevel, MS_U16 msec)
{
    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer_ms(msec);

    return eWDTRe;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetTimer
/// @brief \b Function  \b Description: Set Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: usec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_SetTimer_us(WDT_DbgLv eLevel, MS_U16 usec)
{
    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetTimer_us(usec);

    return eWDTRe;
}

//LG@LG
MS_U32 MDrv_WDT_GetTimer(WDT_DbgLv eLevel)
{
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    return HAL_WDT_GetTimer();
}

//LG@LG
MS_U32 MDrv_WDT_GetTimer_ms(WDT_DbgLv eLevel)
{
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    return HAL_WDT_GetTimer_ms();
}

//LG@LG
MS_U32 MDrv_WDT_GetTimer_us(WDT_DbgLv eLevel)
{
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    return HAL_WDT_GetTimer_us();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_WDT_SetIntTimer
/// @brief \b Function  \b Description: Set INT Timer
/// @param <IN>         \b eLevel: E_WDT_DBGLV_NONE/E_WDT_DBGLV_ERR_ONLY/E_WDT_DBGLV_INFO/E_WDT_DBGLV_ALL
/// @param <IN>         \b MS_U16: sec
/// @param <OUT>        \b None :
/// @param <RET>        \b WDT_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
WDT_Result _MDrv_WDT_SetIntTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
    WDT_Result eWDTRe = E_WDT_FAIL;
    _geDbfLevel = eLevel;

    WDT_DBG_INFO("%s dbg level: %u\n", __FUNCTION__, eLevel);

    eWDTRe = (WDT_Result)HAL_WDT_SetIntTimer(sec);

    return eWDTRe;
}

//------- ------------------------------------------------------------------------------------------
/// Description : Store and resume WDT initial status for fast booting.
/// @param  EN_POWER_MODE    \b IN: The mode of power on/off
/// @return TRUE : succeed
/// @return FALSE : failed
/// @return 1: Resume OK; 2: Suspend OK; 3: Suspend failed
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_WDT_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;
	
	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInitWDT = FALSE;
		MDrv_TIMER_Exit();
		u16Return = 2;				// suspend OK
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
			MDrv_WDT_Init(E_WDT_DBGLV_ERR_ONLY);
			MDrv_TIMER_Init();
			_prev_u16PowerState = u16PowerState;
            u16Return = 1;		// resume OK
		}
		else
		{
            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u16Return = 3;		// resume failed		
		}
	}
	else
	{
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u16Return = FALSE;
	}
	
	return u16Return;
}
// PIU TIMER
inline static void _MDrv_TIMER_Trigger(E_PIU_Timer eTimer, MS_U32 u32count)
{
	if ( eTimer == E_TIMER_0 )
	{
		HAL_WDT_Write4Byte(TIMER_0_MAX_REG,u32count);
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,TIMER_DISABLE);
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_TRIGGER);
	}
	else
	{
		HAL_WDT_Write4Byte(TIMER_1_MAX_REG,u32count);
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,TIMER_DISABLE);
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_TRIGGER);
	}
	while(!MDrv_TIMER_HitMaxMatch(eTimer));
}

static void MDrv_TIMER_Stop(E_PIU_Timer eTimer)
{
    if ( eTimer == E_TIMER_0 )
    {   
        HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)&(~TIMER_ENABLE));
        HAL_WDT_WriteByte(TIMER_0_MATCH_REG,HAL_WDT_ReadByte(TIMER_0_MATCH_REG)|(TIMER_MATCH));
    }
    else
    {   
        HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)&(~TIMER_ENABLE));
        HAL_WDT_WriteByte(TIMER_1_MATCH_REG,HAL_WDT_ReadByte(TIMER_1_MATCH_REG)|(TIMER_MATCH));
    }
}

void _MDrv_TIMER_Count(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
    	tmr_interrupt *ptmr;

    	ptmr = &tTmrTbl[eTimer];
	ptmr->bTmrEn = bEnable;

	if ( eTimer == E_TIMER_0 )
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_ENABLE);
		else
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)&(~TIMER_ENABLE));
	}
	else
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_ENABLE);
		else
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)&(~TIMER_ENABLE));
	}
}

void _MDrv_TIMER_INT(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
	if ( eTimer == E_TIMER_0 )
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_0_CTRL_REG+1)|TIMER_INTEN);
		else
			HAL_WDT_WriteByte(TIMER_0_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_0_CTRL_REG+1)&(~TIMER_INTEN));
	}
	else
	{
		if(bEnable)
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_1_CTRL_REG+1)|TIMER_INTEN);
		else
			HAL_WDT_WriteByte(TIMER_1_CTRL_REG+1,HAL_WDT_ReadByte(TIMER_1_CTRL_REG+1)&(~TIMER_INTEN));
	}
}

MS_BOOL _MDrv_TIMER_HitMaxMatch(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
	{
		return (HAL_WDT_ReadByte(TIMER_0_MATCH_REG)&TIMER_MATCH);
	}
	else
	{
		return (HAL_WDT_ReadByte(TIMER_1_MATCH_REG)&TIMER_MATCH);
	}
}

void _MDrv_TIMER_Rst(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
		HAL_WDT_WriteByte(TIMER_0_CTRL_REG,HAL_WDT_ReadByte(TIMER_0_CTRL_REG)|TIMER_ENABLE);
	else
		HAL_WDT_WriteByte(TIMER_1_CTRL_REG,HAL_WDT_ReadByte(TIMER_1_CTRL_REG)|TIMER_ENABLE);
}

void _MDrv_TIMER_SetMaxMatch(E_PIU_Timer eTimer, MS_U32 u32MaxTimer)
{
    	tmr_interrupt *ptmr;

    	ptmr = &tTmrTbl[eTimer];
	ptmr->u32TmrMax = u32MaxTimer;

	if ( eTimer == E_TIMER_0 )
		HAL_WDT_Write4Byte(TIMER_0_MAX_REG, u32MaxTimer);
	else
		HAL_WDT_Write4Byte(TIMER_1_MAX_REG, u32MaxTimer);
}

MS_U32 _MDrv_TIMER_GetMaxMatch(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_MAX_REG));
	else
		return (HAL_WDT_Read4Byte(TIMER_1_MAX_REG));
}

MS_U32 _MDrv_TIMER_GetCounter(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG));
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG));
}

MS_U32 _MDrv_TIMER_GetSecond(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_HZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_HZ);
}

MS_U32 _MDrv_TIMER_GetMs(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_KHZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_KHZ);
}

MS_U32 MDrv_TIMER_GetUs(E_PIU_Timer eTimer)
{
	if ( eTimer == E_TIMER_0 )
		return (HAL_WDT_Read4Byte(TIMER_0_COUNT_REG)/MST_XTAL_CLOCK_MHZ);
	else
		return (HAL_WDT_Read4Byte(TIMER_1_COUNT_REG)/MST_XTAL_CLOCK_MHZ);
}

void _MDrv_TIMER_Delay(E_PIU_Timer eTimer, MS_U32 u32Second)//@@@need restore status
{
    MS_U32 u32Salt = HAL_WDTCycles(u32Second);
	MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

	_MDrv_TIMER_Trigger(eTimer,u32Salt);
	MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
	MDrv_TIMER_Count(eTimer, ENABLE);
}

void _MDrv_TIMER_DelayMs(E_PIU_Timer eTimer, MS_U32 u32MS)//@@@need restore status
{
	MS_U32 u32Salt = HAL_WDTCyclesM(u32MS);
	MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

	_MDrv_TIMER_Trigger(eTimer,u32Salt);
	MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
	MDrv_TIMER_Count(eTimer, ENABLE);
}

void MDrv_TIMER_DelayUs(E_PIU_Timer eTimer, MS_U32 u32US)//@@@need restore status
{
   printf("TIMER_DelayUs unSupported!! \n");

   if (0)
    {
      MS_U32 u32Salt = HAL_WDTCyclesU(u32US);
      MS_U32 u32Bkup = MDrv_TIMER_GetMaxMatch(eTimer);

      _MDrv_TIMER_Trigger(eTimer,u32Salt);
      MDrv_TIMER_SetMaxMatch(eTimer,u32Bkup);
      MDrv_TIMER_Count(eTimer, ENABLE);
    }

}

void MDrv_TIMER_Init(void)
{
    MS_U32 u32BaseAddr = 0, u32BaseSize = 0, idx;
    tmr_interrupt *ptmr;

    // Clear & disable all timers
    ptmr = &tTmrTbl[0];
    for( idx = 0; idx < MAX_TIMER_NUM; idx++ )
    {
		ptmr->bTmrEn = FALSE;
		ptmr->u32TmrInit = 0x00000000;
		ptmr->u32TmrMax = 0xFFFFFFFF;
		ptmr->TmrFnct = NULL;
		ptmr++;
    }

    // Get Memory Base
    if (!_gbInitWDT)
    {
        if(!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            WDT_DBG_ERR("Get IOMAP Base faill!\n");
            return;
        }

        HAL_WDT_SetIOMapBase(u32BaseAddr);

        _gbInitWDT = TRUE;
        return;
    }
    else
    {
        WDT_DBG_ERR("PIU timer had initial!\n");
        return;
    }
}

static void DRV_TIMER0_PIU_Isr(void)
{
	tmr_interrupt *ptmr;

	ptmr = &tTmrTbl[E_TIMER_0];

	if(MDrv_TIMER_HitMaxMatch(E_TIMER_0))
	{
            WDT_DBG_INFO("%s(%d) =======> comes\n", __FUNCTION__ ,__LINE__);
            MDrv_TIMER_Stop(E_TIMER_0);
            ptmr->TmrFnct(ptmr->TmrFnctArg0,ptmr->TmrFnctArg1,ptmr->TmrFnctArg2);
            // restore tmr default
            //MDrv_TIMER_Rst(E_TIMER_0);
	}
}
static void DRV_TIMER1_PIU_Isr(void)
{
 	tmr_interrupt *ptmr;
    
	ptmr = &tTmrTbl[E_TIMER_1];

	if(MDrv_TIMER_HitMaxMatch(E_TIMER_1))
	{
            WDT_DBG_INFO("%s(%d) =======> comes\n", __FUNCTION__ ,__LINE__);
            MDrv_TIMER_Stop(E_TIMER_1);
            ptmr->TmrFnct(ptmr->TmrFnctArg0,ptmr->TmrFnctArg1,ptmr->TmrFnctArg2);
            // restore tmr default
            //MDrv_TIMER_Rst(E_TIMER_1);
	}
}

void MDrv_TIMER_CfgFnct(E_PIU_Timer eTimer, void (*fnct)(void *, void *, void *), void *arg0, void *arg1, void *arg2 )
{
	tmr_interrupt *ptmr;
    
	if( fnct )
	{
		if( eTimer < MAX_TIMER_NUM )
		{
			ptmr = &tTmrTbl[eTimer];
			//OS_ENTER_CRITICAL();
			ptmr->TmrFnct		= fnct;
			ptmr->TmrFnctArg0 	= arg0;
			ptmr->TmrFnctArg1 	= arg1;
			ptmr->TmrFnctArg2	= arg2;
			//OS_EXIT_CRITICAL();
			MDrv_TIMER_INT(eTimer, 1);
			if( eTimer )
			{
				 MsOS_AttachInterrupt(E_INT_FIQ_EXTIMER1, (InterruptCb)DRV_TIMER1_PIU_Isr);
        			 MsOS_EnableInterrupt(E_INT_FIQ_EXTIMER1);
			}
			else
			{
				 MsOS_AttachInterrupt(E_INT_FIQ_EXTIMER0, (InterruptCb)DRV_TIMER0_PIU_Isr);
        			 MsOS_EnableInterrupt(E_INT_FIQ_EXTIMER0);
			}
			
		}
	}
}

void MDrv_TIMER_Exit(void)
{
    MS_U32 idx;
    tmr_interrupt *ptmr;

    // Clear & disable all timers
    ptmr = &tTmrTbl[0];
    for( idx = 0; idx < MAX_TIMER_NUM; idx++ )
    {
		ptmr->bTmrEn = FALSE;
		ptmr->u32TmrInit = 0x00000000;
		ptmr->u32TmrMax = 0xFFFFFFFF;
		ptmr->TmrFnct = NULL;
		
		MDrv_TIMER_Count((E_PIU_Timer) idx, 0);
		MDrv_TIMER_SetMaxMatch((E_PIU_Timer) idx, ptmr->u32TmrMax);
		MDrv_TIMER_INT((E_PIU_Timer) idx, 0);

        ptmr++;
    }
}

#if (WDT_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_WDT_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    if (FALSE == bU20WDTOpened)
    {
        //if(UtopiaOpen(MODULE_WDT|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        if(UtopiaOpen(MODULE_WDT|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open WDT fail\n");
    		return FALSE;
        }
        bU20WDTOpened = TRUE;
    }
    return TRUE;
}
#endif


WDT_Result MDrv_WDT_Init(WDT_DbgLv eLevel)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
   
    PWDT_PRIVATE_PARAM_Init pWdtParam_Init = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;	
    pWdtParam_Init = (PWDT_PRIVATE_PARAM_Init)malloc(sizeof(WDT_PRIVATE_PARAM_Init)); 
    pWdtParam_Init->eLevel=eLevel;    
    
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_INIT,(void*)pWdtParam_Init); 
 
    free(pWdtParam_Init);  
    return (WDT_Result)u32Ret; 
#else
     return _MDrv_WDT_Init(eLevel);
#endif	
	
}

WDT_Result MDrv_WDT_Stop(WDT_DbgLv eLevel)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    
    PWDT_PRIVATE_PARAM_Stop pWdtParam_Stop = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;	
    pWdtParam_Stop = (PWDT_PRIVATE_PARAM_Stop)malloc(sizeof(WDT_PRIVATE_PARAM_Stop)); 
    pWdtParam_Stop->eLevel=eLevel;    
    
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_STOP,(void*)pWdtParam_Stop); 
 
    free(pWdtParam_Stop);  
    return (WDT_Result)u32Ret; 
#else
     return _MDrv_WDT_Stop(eLevel);
#endif	
}

WDT_Result MDrv_WDT_Clear(void)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_CLEAR,(void*)NULL); 
    return (WDT_Result)u32Ret;
#else
    return _MDrv_WDT_Clear();
#endif
}

WDT_Result MDrv_WDT_ClearRstFlag(void)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_CLEAR_RST_FLAG,(void*)NULL); 
    return (WDT_Result)u32Ret;
#else
    return _MDrv_WDT_ClearRstFlag();
#endif
}

WDT_Result MDrv_WDT_IsReset(void)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_IS_RESET,(void*)NULL); 
    return (WDT_Result)u32Ret;
#else
    return _MDrv_WDT_IsReset();
#endif	
}

WDT_Result MDrv_WDT_IsEnable(void)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_IS_ENABLE,(void*)NULL); 
    return (WDT_Result)u32Ret;
#else
    return _MDrv_WDT_IsEnable();
#endif	
}

WDT_Result MDrv_WDT_SetTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PWDT_PRIVATE_PARAM_SetTimer pWdtParam_SetTimer = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pWdtParam_SetTimer = (PWDT_PRIVATE_PARAM_SetTimer)malloc(sizeof(WDT_PRIVATE_PARAM_SetTimer)); 
    pWdtParam_SetTimer->eLevel=eLevel;
    pWdtParam_SetTimer->sec=sec;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_SETTIMER,(void*)pWdtParam_SetTimer); 
 
    free(pWdtParam_SetTimer);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_WDT_SetTimer(eLevel,sec);
#endif	
}

WDT_Result MDrv_WDT_SetTimer_ms(WDT_DbgLv eLevel, MS_U16 msec)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PWDT_PRIVATE_PARAM_SetTimer_ms pWdtParam_SetTimer_ms = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pWdtParam_SetTimer_ms = (PWDT_PRIVATE_PARAM_SetTimer_ms)malloc(sizeof(WDT_PRIVATE_PARAM_SetTimer_ms)); 
    pWdtParam_SetTimer_ms->eLevel=eLevel;
    pWdtParam_SetTimer_ms->msec=msec;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_SETTIMER_MS,(void*)pWdtParam_SetTimer_ms); 
 
    free(pWdtParam_SetTimer_ms);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_WDT_SetTimer_ms(eLevel,msec);
#endif	
}

WDT_Result MDrv_WDT_SetTimer_us(WDT_DbgLv eLevel, MS_U16 usec)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PWDT_PRIVATE_PARAM_SetTimer_us pWdtParam_SetTimer_us = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pWdtParam_SetTimer_us = (PWDT_PRIVATE_PARAM_SetTimer_us)malloc(sizeof(WDT_PRIVATE_PARAM_SetTimer_us)); 
    pWdtParam_SetTimer_us->eLevel=eLevel;
    pWdtParam_SetTimer_us->usec=usec;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_SETTIMER_US,(void*)pWdtParam_SetTimer_us); 
 
    free(pWdtParam_SetTimer_us);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_WDT_SetTimer_us(eLevel,usec);
#endif	
}

WDT_Result MDrv_WDT_SetIntTimer(WDT_DbgLv eLevel, MS_U16 sec)
{
#if (WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PWDT_PRIVATE_PARAM_SetIntTimer pWdtParam_SetIntTimer = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pWdtParam_SetIntTimer = (PWDT_PRIVATE_PARAM_SetIntTimer)malloc(sizeof(WDT_PRIVATE_PARAM_SetIntTimer)); 
    pWdtParam_SetIntTimer->eLevel=eLevel;
    pWdtParam_SetIntTimer->sec=sec;
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_WDT_SETINTTIMER,(void*)pWdtParam_SetIntTimer); 
 
    free(pWdtParam_SetIntTimer);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_WDT_SetIntTimer(eLevel,sec);
#endif
}


void MDrv_TIMER_Count(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
#if 0//(WDT_UTOPIA20)
    //MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_Count pTimerParam_Count = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return;
    pTimerParam_Count = (PTIMER_PRIVATE_PARAM_Count)malloc(sizeof(TIMER_PRIVATE_PARAM_Count)); 
    pTimerParam_Count->eTimer=eTimer;
    pTimerParam_Count->bEnable=bEnable;
    UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_COUNT,(void*)pTimerParam_Count); 
 
    free(pTimerParam_Count);  
#else
    _MDrv_TIMER_Count(eTimer,bEnable);
#endif
}

void MDrv_TIMER_INT(E_PIU_Timer eTimer, MS_BOOL bEnable)
{
#if 0//(WDT_UTOPIA20)
    //MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_INT pTimerParam_Int = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return;
    pTimerParam_Int = (PTIMER_PRIVATE_PARAM_INT)malloc(sizeof(TIMER_PRIVATE_PARAM_INT)); 
    pTimerParam_Int->eTimer=eTimer;
    pTimerParam_Int->bEnable=bEnable;
    UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_INT,(void*)pTimerParam_Int); 
 
    free(pTimerParam_Int);  
#else
    _MDrv_TIMER_INT(eTimer,bEnable);
#endif	
}

void MDrv_TIMER_Rst(E_PIU_Timer eTimer)
{
#if 0//(WDT_UTOPIA20)
    //MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_Rst pTimerParam_Rst = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return;
    pTimerParam_Rst = (PTIMER_PRIVATE_PARAM_Rst)malloc(sizeof(TIMER_PRIVATE_PARAM_Rst)); 
    pTimerParam_Rst->eTimer=eTimer;
    UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_RST,(void*)pTimerParam_Rst); 
 
    free(pTimerParam_Rst);  
#else
    _MDrv_TIMER_Rst(eTimer);
#endif	
}

void MDrv_TIMER_SetMaxMatch(E_PIU_Timer eTimer, MS_U32 u32MaxTimer)
{
#if 0//(WDT_UTOPIA20)
    //MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_SetMaxMatch pTimerParam_SetMaxMatch = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return;
    pTimerParam_SetMaxMatch = (PTIMER_PRIVATE_PARAM_SetMaxMatch)malloc(sizeof(TIMER_PRIVATE_PARAM_SetMaxMatch)); 
    pTimerParam_SetMaxMatch->eTimer=eTimer;
    pTimerParam_SetMaxMatch->u32MaxTimer=u32MaxTimer;
    UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_SETMAXMATCH,(void*)pTimerParam_SetMaxMatch); 
 
    free(pTimerParam_SetMaxMatch);  
#else
    _MDrv_TIMER_SetMaxMatch(eTimer,u32MaxTimer);
#endif	
}

MS_BOOL MDrv_TIMER_HitMaxMatch(E_PIU_Timer eTimer)
{
#if 0//(WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_HitMaxMatch pTimerParam_HitMaxMatch = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pTimerParam_HitMaxMatch = (PTIMER_PRIVATE_PARAM_HitMaxMatch)malloc(sizeof(TIMER_PRIVATE_PARAM_HitMaxMatch)); 
    pTimerParam_HitMaxMatch->eTimer=eTimer;
 
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_HITMAXMATCH,(void*)pTimerParam_HitMaxMatch); 
 
    free(pTimerParam_HitMaxMatch);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_TIMER_HitMaxMatch(eTimer);
#endif
}

MS_U32 MDrv_TIMER_GetMaxMatch(E_PIU_Timer eTimer)
{
#if 0//(WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_GetMaxMatch pTimerParam_GetMaxMatch = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pTimerParam_GetMaxMatch = (PTIMER_PRIVATE_PARAM_GetMaxMatch)malloc(sizeof(TIMER_PRIVATE_PARAM_GetMaxMatch)); 
    pTimerParam_GetMaxMatch->eTimer=eTimer;
 
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_GETMAXMATCH,(void*)pTimerParam_GetMaxMatch); 
 
    free(pTimerParam_GetMaxMatch);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_TIMER_GetMaxMatch(eTimer);
#endif
}


MS_U32 MDrv_TIMER_GetCounter(E_PIU_Timer eTimer)
{
#if 0//(WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_GetCounter pTimerParam_GetCounter = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pTimerParam_GetCounter = (PTIMER_PRIVATE_PARAM_GetCounter)malloc(sizeof(TIMER_PRIVATE_PARAM_GetCounter)); 
    pTimerParam_GetCounter->eTimer=eTimer;
 
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_GETCOUNTER,(void*)pTimerParam_GetCounter); 
 
    free(pTimerParam_GetCounter);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_TIMER_GetCounter(eTimer);
#endif	
}	


MS_U32 MDrv_TIMER_GetSecond(E_PIU_Timer eTimer)
{
#if 0//(WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_GetSecond pTimerParam_GetSecond = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pTimerParam_GetSecond = (PTIMER_PRIVATE_PARAM_GetSecond)malloc(sizeof(TIMER_PRIVATE_PARAM_GetSecond)); 
    pTimerParam_GetSecond->eTimer=eTimer;
 
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_GETSECOND,(void*)pTimerParam_GetSecond); 
 
    free(pTimerParam_GetSecond);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_TIMER_GetSecond(eTimer);
#endif	
}

MS_U32 MDrv_TIMER_GetMs(E_PIU_Timer eTimer)
{
#if 0//(WDT_UTOPIA20)
    MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_GetMs pTimerParam_GetMs = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return 0;
    pTimerParam_GetMs = (PTIMER_PRIVATE_PARAM_GetMs)malloc(sizeof(TIMER_PRIVATE_PARAM_GetMs)); 
    pTimerParam_GetMs->eTimer=eTimer;
 
    u32Ret=UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_GETMS,(void*)pTimerParam_GetMs); 
 
    free(pTimerParam_GetMs);  
    return (WDT_Result)u32Ret; 
#else
    return _MDrv_TIMER_GetMs(eTimer);
#endif	
}

void MDrv_TIMER_Delay(E_PIU_Timer eTimer, MS_U32 u32Second)
{
#if 0//(WDT_UTOPIA20)
    //MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_Delay pTimerParam_Delay = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return;
    pTimerParam_Delay = (PTIMER_PRIVATE_PARAM_Delay)malloc(sizeof(TIMER_PRIVATE_PARAM_Delay)); 
    pTimerParam_Delay->eTimer=eTimer;
    pTimerParam_Delay->u32Second=u32Second;
    UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_DELAY,(void*)pTimerParam_Delay); 

    free(pTimerParam_Delay);  
#else
    _MDrv_TIMER_Delay(eTimer,u32Second);
#endif
}	

void MDrv_TIMER_DelayMs(E_PIU_Timer eTimer, MS_U32 u32MS)
{
#if 0//(WDT_UTOPIA20)
    //MS_U32 u32Ret=0;
    PTIMER_PRIVATE_PARAM_DelayMs pTimerParam_DelayMs = NULL;
    if (FALSE == _MDrv_WDT_CheckUtopia20Open(&pInstantWdt, 0, pAttributeWdt))
        return;
    pTimerParam_DelayMs = (PTIMER_PRIVATE_PARAM_DelayMs)malloc(sizeof(TIMER_PRIVATE_PARAM_DelayMs)); 
    pTimerParam_DelayMs->eTimer=eTimer;
    pTimerParam_DelayMs->u32MS=u32MS;
    UtopiaIoctl(pInstantWdt,MDrv_CMD_TIMER_DELAYMS,(void*)pTimerParam_DelayMs); 
 
    free(pTimerParam_DelayMs);  
#else
    _MDrv_TIMER_Delay(eTimer,u32MS);
#endif
}
