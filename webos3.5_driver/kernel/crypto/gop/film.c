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
/// @file   mdrv_gflip.c
/// @brief  MStar gflip Interface
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////


//=============================================================================
// Include Files
//=============================================================================
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#if defined(CONFIG_MIPS)
#elif defined(CONFIG_ARM)
#include <asm/io.h>
#endif
#include "mdrv_mstypes.h"

#include "film.h"

#include "reg_sc.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================

#define MAIN_WINDOW         0
#define SUB_WINDOW          1


//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void msWriteByte(DWORD u32Reg, BYTE u8Val )
{
    if(u32Reg%2)
    {
        REG_WH((u32Reg-1), u8Val);
    }
    else
    {
        REG_WL(u32Reg, u8Val);
    }
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
BYTE msReadByte(DWORD u32Reg )
{
    if(u32Reg%2)
    {
        u32Reg = u32Reg-1 ;
        return ((REG_RR(u32Reg) & 0xFF00)>>8);
    }
    else
    {
        return (REG_RR(u32Reg) & 0xFF);
    }
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void MApi_XC_W2BYTE(DWORD u32Reg, WORD u16Val )
{
    REG_W2B(u32Reg, u16Val);
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
WORD MApi_XC_R2BYTE(DWORD u32Reg )
{
    return REG_RR(u32Reg) ;
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
void MApi_XC_W2BYTEMSK(DWORD u32Reg, WORD u16Val, WORD u16Mask )
{
    WORD u16Data=0 ;
    u16Data = REG_RR(u32Reg);
    u16Data = (u16Data & (0xFFFF-u16Mask))|(u16Val &u16Mask);
    REG_W2B(u32Reg, u16Data);
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////
WORD MApi_XC_R2BYTEMSK(DWORD u32Reg, WORD u16Mask )
{
    return (REG_RR(u32Reg) & u16Mask);
}

//////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////

#if 0
void MDrv_GFLIP_FilmDriverHWVer1(void)
{
    MS_BOOL bIsFilmXOO = 0;
    MS_U32 u32Film22AdaptiveCounter1Th = 0;
    static MS_U16 _u16RdBank_Prv=0;
    MS_U16 u16RdBank;
    MS_U32 u32CurCounter1=0, u32CurCounter3=0, u32FixThofC3=0;
    static MS_U32 _u32Counter3_2=0, _u32Counter3_3=0;
    static MS_U32 _u32Counter1_2=0, _u32Counter1_3=0, _u32Counter1_4=0;
    static MS_U32 _u32CounterA_2=0, _u32CounterA_3=0;
    static MS_U32 _u32CounterB_2=0, _u32CounterB_3=0;
    static MS_U8 _u8Film22LikeCounter, _u8Film22LikeCounter2;
    static MS_U8 _u8Film22LikeCounter_ForSlowMotionDisableMCDi;
    static MS_U8 _u8SceneChangeFrame;
    static MS_U8 _u8IsFilmXOOCounter=0;
    static MS_BOOL _bIs2ndFilmXOOCounter;
    MS_BOOL bCounter1Have2xDiff, bFilm, bFilmAny;
    MS_U32 u32CurCounterA, u32CurCounterB;
    static MS_U8 _u8SmallCounter3Cnt;
    static MS_U8 _u8Film22LikeCounterA, _u8Film22LikeCounterB;
    static MS_U8 _u8Film22LikeCounter3_Big;
    static MS_U8 _u8Film32LikeCounter3_Big;
    static MS_U8 _u8Film22LikeCounter1_Big;
    static MS_U8 _u8VerticalCnt, _u8Unmatch3Cnt;
    static MS_U8 _u8FilmsmallCounter1 = 0 ;
    MS_BOOL bIsHDSource;
    MS_U16 u16Unmatch_3, u16Unmatch_1;
    static MS_U8 _u8NoCounter3_Cnt, _u8BigCounter3_Cnt, _u8BigCounter1_Cnt, _u8BigCounter_Cnt3;
    static MS_U8 _u8BigCounter1_Cnt2, _u8SmallCounter1_Cnt, _u8BigCounter3_Cnt2;
    MS_BOOL bVerticalMovingU ;
    MS_BOOL bIs60Hz = ( MApi_XC_R2BYTE(REG_SC_BK0A_10_L) & 0x8000 ) != 0; // checkfilm32 enable for 60Hz source
    static MS_U8 _u8StillCounter3;
    MS_U32 u32Counter3;
    static MS_U8 _u8MovingCharacterCounter;
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    MS_U32 u32Reg7C, u32Reg7D, u32Reg7E;
#endif
    MS_U16 u16InvStillFrameCnt;
    MS_U16 u16KuroPatchEn = 0; //for KURO patch

    //Check the IPM 3D input and OP 3D output or 2D to 3D enable.
    if((( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK12_30_L, 0x01) == 0x01 )
        && ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK12_33_L, 0x80) == 0x80 ))
        ||((( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK62_06_L, 0x7FFF) != 0x0 )
        || ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK62_0E_L, 0x7FFF) != 0x0 ))
        && ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK62_08_L, 0x10) == 0x10 )
        && ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK63_61_L, 0x01) == 0x01 )))
    {
        //For 3D mode or 2D to 3D mode need to close the film driver .
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_10_L, 0, 0xC000);
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_24_L, 0, 0x8000);
        return ;
    }

    u16RdBank = ( MApi_XC_R2BYTE(REG_SC_BK12_3A_L) & 0xF0 ) >> 4;
    bIsHDSource = ( MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF) > 0x700 );

    u32CurCounterA = 0  ; // MApi_XC_R2BYTE(REG_SC_BK0A_4A_L);
    u32CurCounterB = 0  ; // MApi_XC_R2BYTE(REG_SC_BK0A_4B_L);

    u32CurCounter1 = MApi_XC_R2BYTE(REG_SC_BK0A_0A_L);
    u32CurCounter3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

    if(u16RdBank == _u16RdBank_Prv)  // Not change read bank
    {
        return ;
    }

    bFilmAny = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040);
    bFilm = (( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008) ||  // film22
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010) ||  // film32
            ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040) );   // film any

    // check XOO
    if( ( (!bFilm) && (u32CurCounter3 > 0x200) && (_u32Counter3_2< 0x80) && (_u32Counter3_3< 0x80) ) || //not film
        ( (!bFilm) && (u32CurCounter3 > 0x80) && (_u32Counter3_2< 0x20) && (_u32Counter3_3< 0x20) ) ) //not film
    {
        if ( _u8IsFilmXOOCounter > 0 )
        {
            _bIs2ndFilmXOOCounter = true;
        }

        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if( bFilm && (!bFilmAny) && (u32CurCounter3 > 0x200) && (u32CurCounter3 > (_u32Counter3_2*2)) && (u32CurCounter3 > (_u32Counter3_3*2)) ) //32 or 22
    {
        if( _u8IsFilmXOOCounter > 0 )
        {
            _bIs2ndFilmXOOCounter = true;
        }
        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if(  bFilm && (u32CurCounter3 > 0x80)  && (u32CurCounter3 > (_u32Counter3_2*2)) && (u32CurCounter3 > (_u32Counter3_3*2)) ) // film-any
    {
        _bIs2ndFilmXOOCounter = true;
        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if( _u8IsFilmXOOCounter != 0 )
    {
        _u8IsFilmXOOCounter --;
    }
    else
    {
        _bIs2ndFilmXOOCounter = false;
    }

    //detect scene change into film...
    if( (u32CurCounter1 > (_u32Counter1_4*4)) && (u32CurCounter1 > (_u32Counter1_2*4)) && (u32CurCounter1 > (_u32Counter1_3*4)) )
        _u8SceneChangeFrame = 60;
    else if( _u8SceneChangeFrame != 0 )
        _u8SceneChangeFrame --;

    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2 < 0x10) && (_u32Counter3_3 > 0x80) )
        _u8Film22LikeCounter += 2;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2 > 0x80) && (_u32Counter3_3 < 0x10) )
        _u8Film22LikeCounter += 2;
    else if( (u32CurCounter1 > (_u32Counter1_2 + 0x200)) && (_u32Counter1_3 > (_u32Counter1_2 + 0x200)) )
        _u8Film22LikeCounter += 1;
    else if( (_u32Counter1_2 > (u32CurCounter1 + 0x200)) && (_u32Counter1_2 > (_u32Counter1_3 + 0x200)) )
        _u8Film22LikeCounter += 1;
    else if ( _u8Film22LikeCounter > 0 )
        _u8Film22LikeCounter --;

    if( u32CurCounter3 < 0x80 ) // for slow motion film22-like
    {
        if( (u32CurCounter1 > (_u32Counter1_2*2)) && (_u32Counter1_3 > (_u32Counter1_2*2)) )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi += 1;
        else if ( (_u32Counter1_2 > (u32CurCounter1*2)) && (_u32Counter1_2 > (_u32Counter1_3*2)) )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi += 1;
        else if ( _u8Film22LikeCounter_ForSlowMotionDisableMCDi > 0 )
            _u8Film22LikeCounter_ForSlowMotionDisableMCDi --;
    }
    else if( _u8Film22LikeCounter_ForSlowMotionDisableMCDi > 0 )
    {
        _u8Film22LikeCounter_ForSlowMotionDisableMCDi --;
    }

    if( _u8Film22LikeCounter_ForSlowMotionDisableMCDi > 10 )
        _u8Film22LikeCounter_ForSlowMotionDisableMCDi = 10;

    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2 < 0x10) && (_u32Counter3_3 > 0x80) )
        _u8Film22LikeCounter2 += 1;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2 > 0x80) && (_u32Counter3_3 < 0x10) )
        _u8Film22LikeCounter2 += 1;
    else
        _u8Film22LikeCounter2 = 0;

    if( _u8Film22LikeCounter2 > 10 )
        _u8Film22LikeCounter2 = 10;

    if( _u8Film22LikeCounter > 10 ) // clamp
        _u8Film22LikeCounter = 10;


    if(u32CurCounter1<0xF)
        _u8FilmsmallCounter1 ++;
    else
        _u8FilmsmallCounter1 = 0;

    if(_u8FilmsmallCounter1>10)
        _u8FilmsmallCounter1 = 10;

    if( _u32Counter3_2 < _u32Counter3_3 )
        u32FixThofC3 = _u32Counter3_2*8;    //u32FixThofC3 = _u32Counter3_2*4;
    else
        u32FixThofC3 = _u32Counter3_3*8;    //u32FixThofC3 = _u32Counter3_3*4;

    bCounter1Have2xDiff = (((u32CurCounter1 < 0xf000) && (_u32Counter1_2 < 0xf000)) &&
                        ((u32CurCounter1 > (_u32Counter1_2*2)) || ((u32CurCounter1*2) < _u32Counter1_2)));

    if( u32FixThofC3 > 0x4C0)
        u32FixThofC3 = 0x4C0;
    else if( u32FixThofC3 < 0x100)
        u32FixThofC3 = 0x100;

    if((!bFilm) && (_u8Film22LikeCounter2 > 5) && (!bCounter1Have2xDiff) ) //when moving sub-pixel, counter3 will like film22, check counter1 don't have big difference
        u32FixThofC3 = 0x100;   // moving shibasogu buildingg

    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, (MS_U16)u32FixThofC3);
    MApi_XC_W2BYTE(REG_SC_BK0A_41_L, 0xFFFF);
    MApi_XC_W2BYTE(REG_SC_BK0A_5C_L, 0);
    MApi_XC_W2BYTE(REG_SC_BK0A_5E_L, 0);

    MApi_XC_W2BYTE(REG_SC_BK0A_20_L, 0x10);


    if( u32CurCounter1 > _u32Counter1_2 ) //max
        u32Film22AdaptiveCounter1Th = u32CurCounter1;
    else
        u32Film22AdaptiveCounter1Th = _u32Counter1_2;

    if(u32CurCounter3<5)
    {
        _u8SmallCounter3Cnt += 1;

        if(_u8SmallCounter3Cnt>10)
            _u8SmallCounter3Cnt = 10;
    }
    else
    {
        _u8SmallCounter3Cnt --  ;
    }

    if( (u32CurCounterA > (_u32CounterA_2 + 0x200/0x10)) && (_u32CounterA_3 > (_u32CounterA_2 + 0x200/0x10)))
        _u8Film22LikeCounterA += 1;
    else if(( _u32CounterA_2 > (u32CurCounterA + 0x200/0x10)) && (_u32CounterA_2 >( _u32CounterA_3 + 0x200/0x10)))
        _u8Film22LikeCounterA += 1;
    else if( _u8Film22LikeCounterA > 0 )
        _u8Film22LikeCounterA --;

    if(_u8Film22LikeCounterA>10)
        _u8Film22LikeCounterA = 10;

    if((u32CurCounterB > (_u32CounterB_2 + 0x200/0x10)) &&( _u32CounterB_3 > (_u32CounterB_2 + 0x200/0x10)))
        _u8Film22LikeCounterB += 1;
    else if(( _u32CounterB_2 > (u32CurCounterB + 0x200/0x10)) && (_u32CounterB_2 > (_u32CounterB_3 + 0x200/0x10)))
        _u8Film22LikeCounterB += 1;
    else if( _u8Film22LikeCounterB > 0 )
        _u8Film22LikeCounterB --;

    if(_u8Film22LikeCounterB>10)
        _u8Film22LikeCounterB = 10;


    if ((u32CurCounter3 > ((_u32Counter3_2+1) *10)) && (_u32Counter3_3 > ((_u32Counter3_2+1)*10)))
        _u8Film22LikeCounter3_Big += 1;
    else if((_u32Counter3_2 > ((u32CurCounter3+1)*10)) && (_u32Counter3_2 > ((_u32Counter3_3+1)*10)))
        _u8Film22LikeCounter3_Big += 1;
    else if(_u8Film22LikeCounter3_Big > 0 )
        _u8Film22LikeCounter3_Big --;

    if(_u8Film22LikeCounter3_Big>10)
        _u8Film22LikeCounter3_Big = 10;

    if( ((u32CurCounter3 > (_u32Counter3_2*4))||(u32CurCounter3 > (_u32Counter3_3*4)) ) && (u32CurCounter3>0x40) )
        _u8Film32LikeCounter3_Big += 1;
    else if( ( (_u32Counter3_2 > (_u32Counter3_3*4))||(_u32Counter3_2 > (u32CurCounter3*4)) ) && (_u32Counter3_2>0x40))
        _u8Film32LikeCounter3_Big += 1;
    else if( ( (_u32Counter3_3 > (u32CurCounter3*4))||(_u32Counter3_3 > (_u32Counter3_2*4)) ) && (_u32Counter3_3>0x40))
        _u8Film32LikeCounter3_Big += 1;
    else
        _u8Film32LikeCounter3_Big --;

    if(_u8Film32LikeCounter3_Big>10)
        _u8Film32LikeCounter3_Big = 10;

    if ( (u32CurCounter1 > ((_u32Counter1_2+1)*20)) && (_u32Counter1_3 > ((_u32Counter1_2+1)*20)))
        _u8Film22LikeCounter1_Big += 1;
    else if (( _u32Counter1_2 > ((u32CurCounter1+1)*20)) && (_u32Counter1_2 > ((_u32Counter1_3+1)*20)))
        _u8Film22LikeCounter1_Big += 1;
    else if ( _u8Film22LikeCounter1_Big > 0 )
        _u8Film22LikeCounter1_Big --;

    if(_u8Film22LikeCounter1_Big>10)
        _u8Film22LikeCounter1_Big = 10;

    //for KURO patch
    if( u32CurCounter3 >_u32Counter3_2+0x20 && u32CurCounter1 < _u32Counter1_2+0x20 && u32CurCounter1==0 )
        u16KuroPatchEn = 1;
    else if( u32CurCounter3 <_u32Counter3_2+0x20 && u32CurCounter1 > _u32Counter1_2+0x20 && _u32Counter1_2==0)
        u16KuroPatchEn = 1;


    if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008 )
    {  //in film22
        if(((u32CurCounter3 > (_u32Counter3_2+1)*10)||(_u32Counter3_2 > (u32CurCounter3+1)*10)))
        {
            if(_u8Film22LikeCounter1_Big == 10)
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;
            }
            else
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*9/8;

                if( bIsHDSource )
                {
                    if( u32Film22AdaptiveCounter1Th < 0x400/0x2 *6)
                        u32Film22AdaptiveCounter1Th = 0x400/0x2 *6;
                }
                else
                {
                    if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                        u32Film22AdaptiveCounter1Th = 0x400/0x2;
                }
            }
        }
        else
        {
            if ( MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)) ) // 2R
            {
                u32Film22AdaptiveCounter1Th = 0xFFFF;
            }
            else
            {
                if(_u8Film22LikeCounter1_Big == 10)
                     u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;
                else
                    u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
            }

        }
    }
    else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010 )
    {  //in film32
        if((u32CurCounter3 > (_u32Counter3_2*10))||(_u32Counter3_2 > (u32CurCounter3*10))||(_u32Counter3_3 > (_u32Counter3_2*10)))
        {
            if( (u32CurCounter1>= _u32Counter1_2) && (u32CurCounter1 >= _u32Counter1_3)  )
                u32Film22AdaptiveCounter1Th = u32CurCounter1 * 2;
            else if((_u32Counter1_2>= u32CurCounter1) && (_u32Counter1_2 >= _u32Counter1_3))
                u32Film22AdaptiveCounter1Th = _u32Counter1_2 * 2;
            else if((_u32Counter1_3>= u32CurCounter1) &&( _u32Counter1_3 >= _u32Counter1_2))
                u32Film22AdaptiveCounter1Th = _u32Counter1_3 * 2;

            if( bIsHDSource )
            {
                if( u32Film22AdaptiveCounter1Th < (0x400/0x2*6) )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2 *6;
            }
            else
            {
                if( u32Film22AdaptiveCounter1Th < (0x400/0x2) )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }
        else
        {
            if ( MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)) ) // 2R
            {
                u32Film22AdaptiveCounter1Th = 0xFFFF;
            }
            else
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
           }
        }
    }
    else if( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 )
    {  //in filmany
        u32Film22AdaptiveCounter1Th = 0x800;
    }
    else
    {
        if(_u8SmallCounter3Cnt==10)
        {
            if((_u8Film22LikeCounterA>5) || (_u8Film22LikeCounterB>5))
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
                if( bIsHDSource )
                {
                    if( u32Film22AdaptiveCounter1Th < (0x400/0x2*6) )
                        u32Film22AdaptiveCounter1Th = 0x400/0x2*6;
                }
                else
                {
                    if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                        u32Film22AdaptiveCounter1Th = 0x400/0x2;
                }
            }
            else
            {
                if( bIsHDSource )
                {
                    u32Film22AdaptiveCounter1Th = 0x400/0x2*6;
                }
                else
                {
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
                }
            }
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

            if( bIsHDSource )
            {
                if( u32Film22AdaptiveCounter1Th < 0x400/0x2*6 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2*6;
            }
            else
            {
                if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }

        if(_u8Film22LikeCounter3_Big>8 || _u8Film32LikeCounter3_Big>8)
            u32Film22AdaptiveCounter1Th  = 0x7500;
    }


    if( bIsHDSource )
    {
        if( u32Film22AdaptiveCounter1Th < 0x400/0x4*6 )
            u32Film22AdaptiveCounter1Th = 0x400/0x4*6;
    }
    else
    {
        if( u32Film22AdaptiveCounter1Th < 0x800)
            u32Film22AdaptiveCounter1Th = 0x800;
    }

    _u32Counter3_3 = _u32Counter3_2;
    _u32Counter3_2  = u32CurCounter3;
    _u32Counter1_4 = _u32Counter1_3;
    _u32Counter1_3 = _u32Counter1_2;
    _u32Counter1_2  = u32CurCounter1;

    _u32CounterA_3 = _u32CounterA_2;
    _u32CounterA_2  = u32CurCounterA;
    _u32CounterB_3 = _u32CounterB_2;
    _u32CounterB_2  = u32CurCounterB;

    bIsFilmXOO = (_u8IsFilmXOOCounter != 0);

#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    u32Reg7C = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32Reg7D = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32Reg7E = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);

    // for all chip
    if( bIsHDSource )
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0200) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
    else
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0100) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
#else
    // for no MCNR
    bVerticalMovingU = false;
#endif

    u16Unmatch_3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);
    u16Unmatch_1 = MApi_XC_R2BYTE(REG_SC_BK0A_0A_L);

    // for sec , activate counter 1
    if( u16Unmatch_3 < 0x40 )
    {
        _u8NoCounter3_Cnt ++;
    }
    else
    {
        _u8NoCounter3_Cnt = 0;
    }

    // deactivate counter 1 jump out
    if( u16Unmatch_3 > 0x6000  )
    {
        _u8BigCounter3_Cnt = 10;
    }
    else if(_u8BigCounter3_Cnt > 0)
    {
        _u8BigCounter3_Cnt --;
    }

    if( u16Unmatch_3 > 0x1000  )
    {
        _u8BigCounter3_Cnt2 = 10;
    }
    else if(_u8BigCounter3_Cnt2 > 0)
    {
        _u8BigCounter3_Cnt2 --;
    }

    // deactivate counter 1 jump out
#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    if( u16Unmatch_1 > 0xF000  )
#else
    if( u16Unmatch_1 > 0xA000  )
#endif
    {
        _u8BigCounter1_Cnt = 10;
    }
    else if(_u8BigCounter1_Cnt > 0)
    {
        _u8BigCounter1_Cnt --;
    }

    if( u16Unmatch_1 > 0xc000)
    {
        if( _u8BigCounter_Cnt3 < 80)
            _u8BigCounter_Cnt3 ++;
    }
    else
        _u8BigCounter_Cnt3 = 0;

    if( u16Unmatch_1 > 0x1000 )
        _u8BigCounter1_Cnt2 = 0x8;
    else if( _u8BigCounter1_Cnt2 != 0)
        _u8BigCounter1_Cnt2 --;

    if( u16Unmatch_1 < 0x0800 )
    {
        _u8SmallCounter1_Cnt = 10;
    }
    else if(_u8SmallCounter1_Cnt > 0)
    {
        _u8SmallCounter1_Cnt --;
    }

    if((MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2500) || (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2700) )//(stXCStatus.bInterlace)
    {
        if(_u8NoCounter3_Cnt > 20)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x01)<<8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x04)<<8, 0x1F00);  // for SD cross hatch
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }

        // Film32 On,f2 32 film mode enable
        if( (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(14) ) ==  BIT(14) )
           || (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(15) ) ==  BIT(15) ) )
        {
            if(!MDrv_GFLIP_FrameLockCheck()) // Make not frame LOCKED then turn off FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
                //MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
            }
            else    // Make sure frame LOCKED then turn on FilmMode
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //hardware film mode ENABLE
                //MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2)); //software film mode ENABLE

                ///--> new add function for skip film mode in still pattern
                if((!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))&&(!MApi_XC_R2BYTEMSK(REG_SC_BK0A_02_L, BIT(14))))    // 2R mode can't enter this loop
                {
                    u16Unmatch_3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);

                    if( (u16Unmatch_3 > 0x80) )
                        _u8Unmatch3Cnt = 0;
                    else if(_u8Unmatch3Cnt == 0xFF)
                        _u8Unmatch3Cnt = 0xFF;
                    else
                        _u8Unmatch3Cnt = _u8Unmatch3Cnt + 1;

                    ///--> Jump out Film for LG tower
                    if( !bVerticalMovingU )
                        _u8VerticalCnt = 0;
                    else if(_u8VerticalCnt == 0xFF)
                        _u8VerticalCnt = 0xFF;
                    else
                        _u8VerticalCnt = _u8VerticalCnt + 1;
                }
            }
        }
        else
        {
            // DEFAULT value of hardware and software film mode suggested by SY.Shih
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1));
            //MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2));
        }

        // film-any continuous error line patch, not easy to enter film, but easy to exit film
        if ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040 ) // film-any status
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x08, 0xff);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x20, 0xff);
#if 0
        if ( bIsHDSource && (!bFilm) )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_15_L, 0x0008, 0x000F ); // for film32, also favor de-bouncing
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_15_L, 0x0008, 0x000F ); // for De-bouncing
#endif
        if(_u8VerticalCnt>0x03)  // only active at Film
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
        }
        else
        {
            if(u32CurCounter3<0x9 )
            {
                _u8StillCounter3++;
            }
            else
            {
                _u8StillCounter3 = 0;
            }

            if(_u8StillCounter3>=8)
            {
                _u8StillCounter3 = 8;
            }

            if(u32Film22AdaptiveCounter1Th>0xFFFF)
                u32Film22AdaptiveCounter1Th = 0xFFFF;

            MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, (MS_U16)u32Film22AdaptiveCounter1Th );


            if(_u8StillCounter3==8 || u16KuroPatchEn==1 || _u8FilmsmallCounter1==10) //default
            {
                if ( MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)) )  // 2R
                {

                }
                else //4R
                {
                    MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0 );
                    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, 0 );
                }
            }

            if( bIs60Hz ) //NTSC -> turn off fast film22
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
            else // PAL
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, BIT(15), BIT(15));
        }

        // for SEC pattern gen
        if(_u8NoCounter3_Cnt > 20)
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x00FF, 0xFFFF); // disable film-any
        else if ( !bIsFilmXOO )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0080, 0xFFFF); // disable film-any
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0810, 0xFFFF);

        // for auto 1
        // film32
        u32Counter3  = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);


        if( u32Counter3 < 0x100 )
            _u8MovingCharacterCounter = 0; //
        else if(_u8MovingCharacterCounter != 0xff )
            _u8MovingCharacterCounter ++;


        if ( bIsHDSource )
        {
            MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xCF40);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, BIT(13),BIT(13) );
            //MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0fff);
        }
        else
        {
            MApi_XC_W2BYTE(REG_SC_BK0A_06_L, 0xA810);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, 0x00 ,BIT(13) );
            MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0250);
        }

        //MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x00D8);
        if ( bIsHDSource )
        {
            MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x0078);// 1080i
        }
        else
        {
            MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x00D8);  // 480i
        }


        MApi_XC_W2BYTE(REG_SC_BK0A_1E_L, 0x0551);


        if ( MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, BIT(12)) ) // auto 1 , deal with video-over-film
        {
        }
        else // default
        {
            if ( bIsHDSource )
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F10);

            }
            else
            {
                MApi_XC_W2BYTE(REG_SC_BK0A_07_L, 0x7F08);
            }
        }
    } // end interlace loop
    _u16RdBank_Prv = u16RdBank;

    if(MApi_XC_R2BYTEMSK(REG_SC_BK2A_02_L, BIT(15)))
    {
        u16InvStillFrameCnt = 15;
    }
    else
    {
        u16InvStillFrameCnt = 0;
    }

    if((_u8Film22LikeCounter > 0x5) || (_u8Film22LikeCounter_ForSlowMotionDisableMCDi > 0x5))
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2F_L, 0x8000, 0xFFFF); //disable MCDi
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, 0xF000, 0xFF00); //Favor SST
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, 0x0000, 0xFF00); //Disable KFC-cplx
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2F_L, 0x0000, 0xFFFF);
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, 0x2100, 0xFF00);
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_2A_L, ((MS_U16)0xF000)|((MS_U16)(u16InvStillFrameCnt<<8)), 0xFF00);
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, (MS_U16)(u16InvStillFrameCnt<<4), 0x00F0);
    }
}
#endif
void MDrv_GFLIP_FilmDriverHWVer2(void)
{
    MS_BOOL bIsFilmXOO = 0;
    MS_U32 u32Film22AdaptiveCounter1Th = 0;
    static MS_U16 _u16RdBank_Prv=0;
    MS_U16 u16RdBank;
    MS_U32 u32CurCounter1=0, u32CurCounter3=0, u32FixThofC3=0;
    static MS_U32 _u32Counter3_2=0, _u32Counter3_3=0;
    static MS_U32 _u32Counter1_2=0, _u32Counter1_3=0, _u32Counter1_4=0;
    static MS_U32 _u32CounterA_2=0, _u32CounterA_3=0;
    static MS_U32 _u32CounterB_2=0, _u32CounterB_3=0;
    static MS_U8 _u8Film22LikeCounter2;


    static MS_U8 _u8IsFilmXOOCounter=0;

    MS_BOOL bCounter1Have2xDiff, bFilm, bFilm22, bFilm32, bFilmAny;
    MS_U32 u32CurCounterA, u32CurCounterB;
    static MS_U8 _u8SmallCounter3Cnt;
    static MS_U8 _u8Film22LikeCounterA, _u8Film22LikeCounterB;
    static MS_U8 _u8Film22LikeCounter3_Big;
    static MS_U8 _u8Film32LikeCounter3_Big;
    static MS_U8 _u8Film22LikeCounter1_Big;
    static MS_U8 _u8VerticalCnt;
    MS_BOOL bIsHDSource;
    //MS_U16 u16Unmatch_3, u16Unmatch_1;
    static MS_U8 _u8NoCounter3_Cnt;

    MS_BOOL bVerticalMovingU ;
    MS_BOOL bIs60Hz = ( MApi_XC_R2BYTE(REG_SC_BK0A_10_L) & 0x8000 ) != 0; // checkfilm32 enable for 60Hz source
    static MS_U8 _u8StillCounter3;

#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    MS_U32 u32Reg7C, u32Reg7D, u32Reg7E;
#endif
    MS_U16 u16KuroPatchEn = 0;  //for KURO patch

	MS_U16 u16hpfHdCntReport1 = 0;
	MS_U16 u16hpfHdCntReport2 = 0;
	MS_U32 u32hpfHdCntReport  = 0;
		MS_U32 Big_Unmatch = 0;
		MS_U32 Small_Unmatch = 0;
		MS_U32 Delta = 0;
	MS_U16 counter1_adap_HDrpt_LowTh = 0;
		MS_U16 HDrpt_TH = 0;
	MS_U16 u16X_R1,u16X_R2,u16X_L1,u16X_L2;
	MS_BOOL XXX_bool=0;

    //Check the IPM 3D input and OP 3D output or 2D to 3D enable.
    if((( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK12_30_L, 0x01) == 0x01 )
        && ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK12_33_L, 0x80) == 0x80 ))
        ||((( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK62_06_L, 0x7FFF) != 0x0 )
        || ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK62_0E_L, 0x7FFF) != 0x0 ))
        && ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK62_08_L, 0x10) == 0x10 )
        && ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK63_61_L, 0x01) == 0x01 )))
    {
        //For 3D mode or 2D to 3D mode need to close the film driver .
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_10_L, 0, 0xC000);
        MApi_XC_W2BYTEMSK(REG_SC_BK0A_24_L, 0, 0x8000);
        return ;
    }

    u16RdBank = ( MApi_XC_R2BYTE(REG_SC_BK12_3A_L) & 0xF0 ) >> 4;
    if(u16RdBank == _u16RdBank_Prv)  // Not change read bank
    {
        return ;
    }

    u32CurCounterA = MApi_XC_R2BYTE(REG_SC_BK0A_4A_L);
    u32CurCounterB = MApi_XC_R2BYTE(REG_SC_BK0A_4B_L);

    u32CurCounter1 = MApi_XC_R2BYTE(REG_SC_BK0A_0A_L);
    u32CurCounter3 = MApi_XC_R2BYTE(REG_SC_BK0A_0B_L);
    if(MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L , 0x0010))
    {
	//printf( "Counter1 : %ld, ", u32CurCounter1 ); // printf()
       // printf( "Counter3 : %ld, \n", u32CurCounter3 ); // printf()
    }

	u16hpfHdCntReport1 = MApi_XC_R2BYTE( REG_SC_BK30_0D_L);
	u16hpfHdCntReport2 = MApi_XC_R2BYTE( REG_SC_BK30_0E_L);
	u32hpfHdCntReport  = (u16hpfHdCntReport1<<16) + u16hpfHdCntReport2;
    if(MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L , 0x0010))
    {
	    //printf("HDrpt_222 = %lx\n", u32hpfHdCntReport);
    }
#if 0
	if (u32hpfHdCntReport >= 65536)
		counter1_adap_HDrpt_LowTh = (MS_U16)min(800 + 250*(u32hpfHdCntReport-65536)/65536, 0xFFFF);
	else if (u32hpfHdCntReport >= 4096)
		counter1_adap_HDrpt_LowTh = (MS_U16)(50*u32hpfHdCntReport/4096);
	else
		counter1_adap_HDrpt_LowTh = (MS_U16)(10*u32hpfHdCntReport/512);
#endif

	  u32hpfHdCntReport = u32hpfHdCntReport/4096;

		if ( u32hpfHdCntReport > 80)
			HDrpt_TH = 2048;
		else if ( u32hpfHdCntReport > 32)
			HDrpt_TH = (u32hpfHdCntReport-32)*512/16 + 128;
		else if ( u32hpfHdCntReport > 16)
			HDrpt_TH = (u32hpfHdCntReport-16)*256/16 + 64;
		else if ( u32hpfHdCntReport > 4)
			HDrpt_TH = (u32hpfHdCntReport-4)*128/12 + 32;
		else if ( u32hpfHdCntReport > 1)
			HDrpt_TH = (u32hpfHdCntReport-1)*48/3 + 16;
		else// (HD_report<=1)
			HDrpt_TH =  0;

    Big_Unmatch = max(_u32Counter1_2,u32CurCounter1);
    Small_Unmatch = min(_u32Counter1_2,u32CurCounter1);
    //Delta = Big_Unmatch - Big_Unmatch ;
    Delta = Big_Unmatch - Small_Unmatch;
#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
		if ((Big_Unmatch/4) > max(Small_Unmatch, (MS_U32)25))
#else
		if ((Big_Unmatch/4) > max(Small_Unmatch, 25))
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */
		{
			counter1_adap_HDrpt_LowTh = Big_Unmatch /4;
		}
		else if ((Big_Unmatch/2) > Small_Unmatch)
		{
		   if (Delta > HDrpt_TH)
		      counter1_adap_HDrpt_LowTh = Big_Unmatch/2;
		   else
		      counter1_adap_HDrpt_LowTh = HDrpt_TH; //counter1_adap_HDrpt_LowTh =0
		}
		else
		      counter1_adap_HDrpt_LowTh = 0;
		if(MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L , 0x0010))
        {
	        //printf("C1_HDrpt_LowTh = %lx\n", counter1_adap_HDrpt_LowTh);
        }
        //counter1_adap_HDrpt_LowTh
	//moving dir R
    u16X_R1 = MApi_XC_R2BYTE(REG_SC_BK17_61_L)>>8;
    u16X_R2 = MApi_XC_R2BYTE(REG_SC_BK17_67_L)>>8;

    //moving dir L
    u16X_L1 = MApi_XC_R2BYTE(REG_SC_BK17_62_L)&0x00FF;
    u16X_L2 = MApi_XC_R2BYTE(REG_SC_BK17_64_L)&0x00FF;

	XXX_bool = (((u16X_R1>0x40)&&(u16X_R2>0x40))|((u16X_L1>0x40)&&(u16X_L2>0x40)));//&&(max(u32CurCounter3,_u32Counter3_2) != 0);

#if 0
	if(XXX_bool)
	{
        MApi_XC_W2BYTEMSK(REG_SC_BK30_38_L, 0x0001, 0x0001);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_39_L, 0x0001, 0x0001);
        MApi_XC_W2BYTEMSK(REG_SC_BK30_3A_L, 0x000F, 0x000F);
//		MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0x00F0, 0x00F0);
	}
	else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK30_38_L, 0, 0x0001);
        MApi_XC_W2BYTEMSK(REG_SC_BK30_39_L, 0, 0x0001);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_3A_L, 0, 0x000F);
//		MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0, 0x00F0);
    }
#endif

    #if 1
    bIsHDSource = ( MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF) > 0x700 );
    bFilmAny = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040);
    bFilm22  = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008);
    bFilm32  = ( (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010);
    bFilm = ( bFilm22 || bFilm32 || bFilmAny );

    // check XOO
    if( ( (!bFilm) && (u32CurCounter3 > 0x200) && (_u32Counter3_2< 0x80) && (_u32Counter3_3< 0x80) ) || //not film
        ( (!bFilm) && (u32CurCounter3 > 0x80) && (_u32Counter3_2< 0x20) && (_u32Counter3_3< 0x20) ) ) //not film
    {

        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if( bFilm && (!bFilmAny) && (u32CurCounter3 > 0x200) && (u32CurCounter3 > (_u32Counter3_2*2)) && (u32CurCounter3 > (_u32Counter3_3*2)) ) //32 or 22
    {
        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if(  bFilm && (u32CurCounter3 > 0x80)  && (u32CurCounter3 > (_u32Counter3_2*2)) && (u32CurCounter3 > (_u32Counter3_3*2)) ) // film-any
    {
        _u8IsFilmXOOCounter = 10; // need to hit XOO on every 10 frames
    }
    else if( _u8IsFilmXOOCounter != 0 )
    {
        _u8IsFilmXOOCounter --;
    }
    else
    {

    }


    if( (u32CurCounter3 > 0x80) && (_u32Counter3_2 < 0x10) && (_u32Counter3_3 > 0x80) )
        _u8Film22LikeCounter2 += 1;
    else if( (u32CurCounter3 < 0x10) && (_u32Counter3_2 > 0x80) && (_u32Counter3_3 < 0x10) )
        _u8Film22LikeCounter2 += 1;
    else
        _u8Film22LikeCounter2 = 0;

    if( _u8Film22LikeCounter2 > 10 )
        _u8Film22LikeCounter2 = 10;

    if( _u32Counter3_2 < _u32Counter3_3 )
        u32FixThofC3 = _u32Counter3_2*4;
    else
        u32FixThofC3 = _u32Counter3_3*4;

    bCounter1Have2xDiff = (((u32CurCounter1 < 0xf000) && (_u32Counter1_2 < 0xf000)) &&
                        ((u32CurCounter1 > (_u32Counter1_2*2)) || ((u32CurCounter1*2) < _u32Counter1_2)));

    if( u32FixThofC3 > 0x4C0)
        u32FixThofC3 = 0x4C0;
    else if( u32FixThofC3 < 0x100)
        u32FixThofC3 = 0x100;

    if((!bFilm) && (_u8Film22LikeCounter2 > 5) && (!bCounter1Have2xDiff) ) //when moving sub-pixel, counter3 will like film22, check counter1 don't have big difference
        u32FixThofC3 = 0x100;   // moving shibasogu buildingg; pattern gen in 0.5 speed?




    if( u32CurCounter1 > _u32Counter1_2 ) //max
        u32Film22AdaptiveCounter1Th = u32CurCounter1;
    else
        u32Film22AdaptiveCounter1Th = _u32Counter1_2;

    if(u32CurCounter3<5)
    {
        _u8SmallCounter3Cnt += 1;

        if(_u8SmallCounter3Cnt>10)
            _u8SmallCounter3Cnt = 10;
    }
    else
    {
        _u8SmallCounter3Cnt --  ;
    }

    if( (u32CurCounterA > (_u32CounterA_2 + 0x200/0x10)) && (_u32CounterA_3 > (_u32CounterA_2 + 0x200/0x10)))
        _u8Film22LikeCounterA += 1;
    else if(( _u32CounterA_2 > (u32CurCounterA + 0x200/0x10)) && (_u32CounterA_2 >( _u32CounterA_3 + 0x200/0x10)))
        _u8Film22LikeCounterA += 1;
    else if( _u8Film22LikeCounterA > 0 )
        _u8Film22LikeCounterA --;

    if(_u8Film22LikeCounterA>10)
        _u8Film22LikeCounterA = 10;

    if((u32CurCounterB > (_u32CounterB_2 + 0x200/0x10)) &&( _u32CounterB_3 > (_u32CounterB_2 + 0x200/0x10)))
        _u8Film22LikeCounterB += 1;
    else if(( _u32CounterB_2 > (u32CurCounterB + 0x200/0x10)) && (_u32CounterB_2 > (_u32CounterB_3 + 0x200/0x10)))
        _u8Film22LikeCounterB += 1;
    else if( _u8Film22LikeCounterB > 0 )
        _u8Film22LikeCounterB --;

    if(_u8Film22LikeCounterB>10)
        _u8Film22LikeCounterB = 10;


    if ((u32CurCounter3 > ((_u32Counter3_2+1) *10)) && (_u32Counter3_3 > ((_u32Counter3_2+1)*10)))
        _u8Film22LikeCounter3_Big += 1;
    else if((_u32Counter3_2 > ((u32CurCounter3+1)*10)) && (_u32Counter3_2 > ((_u32Counter3_3+1)*10)))
        _u8Film22LikeCounter3_Big += 1;
    else if(_u8Film22LikeCounter3_Big > 0 )
        _u8Film22LikeCounter3_Big --;

    if(_u8Film22LikeCounter3_Big>10)
        _u8Film22LikeCounter3_Big = 10;

    if( ((u32CurCounter3 > (_u32Counter3_2*4))||(u32CurCounter3 > (_u32Counter3_3*4)) ) && (u32CurCounter3>0x40) )
        _u8Film32LikeCounter3_Big += 1;
    else if( ( (_u32Counter3_2 > (_u32Counter3_3*4))||(_u32Counter3_2 > (u32CurCounter3*4)) ) && (_u32Counter3_2>0x40))
        _u8Film32LikeCounter3_Big += 1;
    else if( ( (_u32Counter3_3 > (u32CurCounter3*4))||(_u32Counter3_3 > (_u32Counter3_2*4)) ) && (_u32Counter3_3>0x40))
        _u8Film32LikeCounter3_Big += 1;
    else
        _u8Film32LikeCounter3_Big --;

    if(_u8Film32LikeCounter3_Big>10)
        _u8Film32LikeCounter3_Big = 10;

    if ( (u32CurCounter1 > ((_u32Counter1_2+1)*20)) && (_u32Counter1_3 > ((_u32Counter1_2+1)*20)))
        _u8Film22LikeCounter1_Big += 1;
    else if (( _u32Counter1_2 > ((u32CurCounter1+1)*20)) && (_u32Counter1_2 > ((_u32Counter1_3+1)*20)))
        _u8Film22LikeCounter1_Big += 1;
    else if ( _u8Film22LikeCounter1_Big > 0 )
        _u8Film22LikeCounter1_Big --;

    if(_u8Film22LikeCounter1_Big>10)
        _u8Film22LikeCounter1_Big = 10;

    //for KURO patch
    if( u32CurCounter3 >_u32Counter3_2+0x20 && u32CurCounter1 < _u32Counter1_2+0x20 && u32CurCounter1==0 ) //u32CurCounter1 +0x20 < _u32Counter1_2 ?
        u16KuroPatchEn = 1;
    else if( u32CurCounter3 <_u32Counter3_2+0x20 && u32CurCounter1 > _u32Counter1_2+0x20 && _u32Counter1_2==0)
        u16KuroPatchEn = 1;

	if( XXX_bool)
	{
        u32Film22AdaptiveCounter1Th = /*(u32Film22AdaptiveCounter1Th)/4 + */counter1_adap_HDrpt_LowTh;
	}
    else if( bFilm22 )
    {  //in film22
        if(((u32CurCounter3 > (_u32Counter3_2+1)*10)||(_u32Counter3_2 > (u32CurCounter3+1)*10)))
        {
            if(_u8Film22LikeCounter1_Big == 10)
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;

            else
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*9/8;

                if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }
        else
        {
            if(_u8Film22LikeCounter1_Big == 10)
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*3/8;
            else
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

        }
    }
    else if( bFilm32 )
    {  //in film32
        if((u32CurCounter3 > (_u32Counter3_2*10))||(_u32Counter3_2 > (u32CurCounter3*10))||(_u32Counter3_3 > (_u32Counter3_2*10)))
        {
            if( (u32CurCounter1>= _u32Counter1_2) && (u32CurCounter1 >= _u32Counter1_3)  )
                u32Film22AdaptiveCounter1Th = u32CurCounter1 * 2;
            else if((_u32Counter1_2>= u32CurCounter1) && (_u32Counter1_2 >= _u32Counter1_3))
                u32Film22AdaptiveCounter1Th = _u32Counter1_2 * 2;
            else if((_u32Counter1_3>= u32CurCounter1) &&( _u32Counter1_3 >= _u32Counter1_2))
                u32Film22AdaptiveCounter1Th = _u32Counter1_3 * 2;

            if( u32Film22AdaptiveCounter1Th < (0x400/0x2) )
                u32Film22AdaptiveCounter1Th = 0x400/0x2;
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;
        }
    }
    else if( bFilmAny )
    {  //in filmany
        u32Film22AdaptiveCounter1Th = 0x5F00;
    }
    else
    {
        if(_u8SmallCounter3Cnt==10)
        {
            if((_u8Film22LikeCounterA>5) || (_u8Film22LikeCounterB>5))
            {
                u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

                if( u32Film22AdaptiveCounter1Th > 0x400/0x2 )
                    u32Film22AdaptiveCounter1Th = 0x400/0x2;

            }
            else
            {
                u32Film22AdaptiveCounter1Th = 0x400/0x2;
            }
        }
        else
        {
            u32Film22AdaptiveCounter1Th = (u32Film22AdaptiveCounter1Th)*6/8;

            if( u32Film22AdaptiveCounter1Th < 0x400/0x2 )
                u32Film22AdaptiveCounter1Th = 0x400/0x2;
        }

        if(_u8Film22LikeCounter3_Big>8 || _u8Film32LikeCounter3_Big>8)
            u32Film22AdaptiveCounter1Th  = 0xFFFF;
    }

	if( XXX_bool)
	{

	}
	else if( bFilm32 ) // in film32
		{
		    if( u32Film22AdaptiveCounter1Th < 0x400/0x2)
		        u32Film22AdaptiveCounter1Th = 0x400/0x2;
		}
		else //other film mode
		{
			  if( u32Film22AdaptiveCounter1Th < 0x400/0x4)
		        u32Film22AdaptiveCounter1Th = 0x400/0x4;
		}
    bIsFilmXOO = (_u8IsFilmXOOCounter != 0);

#ifdef PRJ_MCNR  // For A3, A5,A7P,T12
    u32Reg7C = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
    u32Reg7D = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
    u32Reg7E = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);

    // for all chip
    if( bIsHDSource )
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0200) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
    else
    {
        // for MCNR only
        bVerticalMovingU = ((u32Reg7E > 0x0100) && (u32Reg7E > (8*u32Reg7C)) && (u32Reg7E > (8*u32Reg7D)));
    }
#else
    // for no MCNR
    bVerticalMovingU = false;
#endif


    // for sec , activate counter 1
    if( u32CurCounter3 < 0x40 )
    {
        _u8NoCounter3_Cnt ++;
    }
    else
    {
        _u8NoCounter3_Cnt = 0;
    }


    if((MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2500)
		|| (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2700)
		|| (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2400)
		|| (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L, 0xFF00)==0x2600)
		)//(stXCStatus.bInterlace)
    {
        if(_u8NoCounter3_Cnt > 20)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x01)<<8, 0x1F00);
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, ((MS_U16)0x04)<<8, 0x1F00);  // for SD cross hatch
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_05_L, ((MS_U16)0x10)<<8, 0xFF00);
        }

        // Film32 On,f2 32 film mode enable
        if( (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(14) ) ==  BIT(14) )
           || (MApi_XC_R2BYTEMSK( REG_SC_BK0A_10_L, BIT(15) ) ==  BIT(15) ) )
        {
                ///--> new add function for skip film mode in still pattern
                if((!MApi_XC_R2BYTEMSK(REG_SC_BK12_03_L, BIT(1)))&&(!MApi_XC_R2BYTEMSK(REG_SC_BK0A_02_L, BIT(14))))    // 2R mode can't enter this loop
                {
                    ///--> Jump out Film for LG tower
                    if( !bVerticalMovingU )
                        _u8VerticalCnt = 0;
                    else if(_u8VerticalCnt == 0xFF)
                        _u8VerticalCnt = 0xFF;
                    else
                        _u8VerticalCnt = _u8VerticalCnt + 1;
                }
            }
        else
        {
            // DEFAULT value of hardware and software film mode suggested by SY.Shih
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //repeated by line 570
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2));
        }

        // film-any continuous error line patch, not easy to enter film, but easy to exit film
        if ( bFilmAny ) // film-any status
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x08, 0xff);
        else
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_57_L, 0x20, 0xff);


        if(_u8VerticalCnt>0x03)          // only active at Film
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
        }
        else
        {
            if((u32CurCounter3<0x9) && (u32CurCounter1<0x9))
            {
                _u8StillCounter3++;
            }
            else
            {
                _u8StillCounter3 = 0;
            }

            if(_u8StillCounter3>=8)
            {
                _u8StillCounter3 = 8;
            }

            if(u32Film22AdaptiveCounter1Th>0xFFFF)
                u32Film22AdaptiveCounter1Th = 0xFFFF;



            if(_u8StillCounter3==8 || u16KuroPatchEn==1) //default
            {
                //MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, 0 ); //repeated with line 522
                //MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, 0 ); //repeated with line 169
                u32Film22AdaptiveCounter1Th = 0;
                u32FixThofC3 = 0;
            }
			else if (XXX_bool)
			{
			    u32FixThofC3 = 0xFFFF;
            }

            MApi_XC_W2BYTE(REG_SC_BK0A_0C_L, (MS_U16)u32Film22AdaptiveCounter1Th );//repeated with line 526
            if( bIs60Hz ) //NTSC -> turn off fast film22
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, 0, BIT(15));
            else // PAL
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_23_L, BIT(15), BIT(15));
        }

        // for SEC pattern gen
        if(_u8NoCounter3_Cnt > 20)
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x00FF, 0xFFFF); // disable film-any
        else if ( !bIsFilmXOO )
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0080, 0xFFFF); // disable film-any
        else
        {
            if (bIsHDSource)
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0804, 0xFFFF);
            else
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_25_L, 0x0808, 0xFFFF);
        }

            if ( bIsHDSource )
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, BIT(13),BIT(13) );
            }
            else
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_17_L, 0x00 ,BIT(13) );
                MApi_XC_W2BYTE(REG_SC_BK0A_1F_L, 0x0250);
            }

            //MApi_XC_W2BYTE(REG_SC_BK0A_16_L, 0x00D8);
        MApi_XC_W2BYTE(REG_SC_BK0A_1E_L, 0x0551); //repeated by line 488

    } // end interlace loop
    MApi_XC_W2BYTE(REG_SC_BK0A_0D_L, (MS_U16)u32FixThofC3); //repeated with line 527
    MApi_XC_W2BYTE(REG_SC_BK0A_41_L, 0xFFFF);
    MApi_XC_W2BYTE(REG_SC_BK0A_5C_L, 0);
    MApi_XC_W2BYTE(REG_SC_BK0A_5E_L, 0);
    //MApi_XC_W2BYTEMSK(REG_SC_BK0A_02_L, 0, 0x0F );

    //MApi_XC_W2BYTE(REG_SC_BK0A_20_L, 0x10);

    _u16RdBank_Prv = u16RdBank;

		//*****can move to final********
    _u32Counter3_3 = _u32Counter3_2;
    _u32Counter3_2  = u32CurCounter3;
    _u32Counter1_4 = _u32Counter1_3;
    _u32Counter1_3 = _u32Counter1_2;
    _u32Counter1_2  = u32CurCounter1;

    _u32CounterA_3 = _u32CounterA_2;
    _u32CounterA_2  = u32CurCounterA;
    _u32CounterB_3 = _u32CounterB_2;
    _u32CounterB_2  = u32CurCounterB;
		//*****can move to final********

#endif
}
