#include <linux/device.h>
#include <linux/export.h>
#include <linux/delay.h>

#include "mst_str.h"
//#include "Str_audio.h"
#include "../../include/drvAUDIO_if.h"
#include "../../hal/mustang/audio/halAUDIO.h"
#include "../../hal/mustang/audio/halSOUND.h"
#include "../../hal/mustang/audsp/halAUDSP.h"
#include "../../hal/mustang/audio/halMAD.h"
#include "../../hal/mustang/audio/halMAD2.h"
#include "../../hal/mustang/audio/halADVAUDIO.h"
#include "../../hal/mustang/audio/regAUDIO.h"
#include "../../hal/mustang/audio/halAUR2.h"
#include "../../hal/mustang/audio/halSIF.h"
#include "../../include/drvAUDIO_if.h"
#include "../audio/internal/drvMAD.h"
#include "../audio/internal/drvADVSOUND.h"
#include "../audio/internal/drvAUDIO_internal.h"
//#include "mdrv_types.h"
//#include "mdrv_system.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
//#define AUDIO_STR_TEST
//-------------------------------------------------------------------------------------------------
//  Local struct define
//-------------------------------------------------------------------------------------------------
#define NumRegs 8
#define NumRows 15
#define LGSE_Restore_In_Kernel 0

typedef struct
{
    MS_U16 aud_reg[NumRegs];
} AU_RowRegs_t;

typedef struct
{
    AU_RowRegs_t aud_row[NumRows];
    MS_U32 u32Bank;
} AU_BankRegs_t;

//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U32 MAD_Base_Addr = 0x0;
MS_U32 DEC_Base_Addr = 0x0;
MS_U32 SE_Base_Addr = 0x0;

static MS_U8 eDecCmd = AU_DVB_DECCMD_STOP;
static MS_U8 eSECmd = AU_DVB_DECCMD_STOP;

static AUDIO_DSP_CODE_TYPE Dsp2CodeType = AU_DVB_STANDARD_INVALID;
static AUDIO_DSP_CODE_TYPE SifCodeType = AU_DVB_STANDARD_INVALID;
static AUDIO_DSP_CODE_TYPE ADEC0_R2CodeType = AU_DVB_STANDARD_INVALID;
static AUDIO_DSP_CODE_TYPE ADEC1_R2CodeType = AU_DVB_STANDARD_INVALID;

#if( LGSE_Restore_In_Kernel == 1)
static MS_BOOL bIsSTR_Suspend_LGSE_Fail = FALSE;
MS_U8 U8LGSEFN_MODE_REGISTER[4];
MS_U8 U8LGSEFNMAIN_INIT[40];
MS_U8 U8LGSEFNMAIN_VARIABLES[12];
MS_U8 U8LGSEFN000_INIT[1180];
MS_U8 U8LGSEFN000_VARIABLES[4];
MS_U8 U8LGSEFN001_INIT[84];
MS_U8 U8LGSEFN001_VARIABLES[520];
MS_U8 U8LGSEFN004_MODE1_VARIABLES[100];
MS_U8 U8LGSEFN004_MODE2_VARIABLES[60];
MS_U8 U8LGSEFN009_VARIABLES[8];
MS_U8 U8LGSEFN010_INIT[40];
MS_U8 U8LGSEFN017_VARIABLES[648];
MS_U8 U8LGSEFN028_VARIABLES[8];
#endif

AU_BankRegs_t AU_Bank112A;
AU_BankRegs_t AU_Bank112B;
AU_BankRegs_t AU_Bank112C;
AU_BankRegs_t AU_Bank112D;
AU_BankRegs_t AU_Bank112E;
AU_BankRegs_t AU_Bank1603;

//#define CPU_BUS_INTERVAL                       0x20000000 // ???
//-------------------------------------------------------------------------------------------------
//  Local functions
//-------------------------------------------------------------------------------------------------

#if 0
MS_U32 PhysicalAddr2BusAddr(MS_U32 addr)
{
    return addr + CPU_BUS_INTERVAL;
}
#endif

static void AU_Backup_BaseAddr(void)
{
    static MS_BOOL bIsBackup = FALSE;

    if (bIsBackup == FALSE)
    {
        // Backup MAD base addr
        MAD_Base_Addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
        HAL_AUDIO_SetDspBaseAddr(0, MAD_Base_Addr, MAD_Base_Addr);
        bIsBackup = TRUE;
    }
}

static void AU_BankRegs_Initialize(MS_U32 u32Bank, AU_BankRegs_t * pBank)
{
    int i = 0, j = 0;
    pBank->u32Bank = u32Bank;

    for (i = 0; i < NumRows; i++)
    {
        for (j = 0; j < NumRegs; j++)
        {
            pBank->aud_row[i].aud_reg[j] = 0x0000;
        }
    }
}

static void AU_Backup_Bank(AU_BankRegs_t * pBank)
{
    int i = 0, j = 0;
    if (pBank == NULL)
    {
        printk("AU_Backup_Bank(),  pBank is NULL!! \r\n");
        return;
    }

    STR_PRINT("pBank->u32Bank:%x\n", (int)pBank->u32Bank);

    for (i = 0; i < NumRows; i++)
    {
        for (j = 0; j < NumRegs; j++)
        {
            //printk("((i << 16) +(j << 1)) = %x       ",((i << 4) +(j << 1)));
            //printk("stBank.u32Bank = %x       ",pBank->u32Bank);
            MS_U32 ReadRegAddr = pBank->u32Bank + ((i << 4) + (j << 1));
            //printk("ReadRegAddr = %x       ",ReadRegAddr);
            pBank->aud_row[i].aud_reg[j] = HAL_AUDIO_AbsReadReg(ReadRegAddr);
            //printk("pBank->aud_row[%d].aud_reg[%d] = %x\n",i, j, pBank->aud_row[i].aud_reg[j]);
            //printk("%-4x ", pBank->aud_row[i].aud_reg[j]);
        }
        //printk("\n");
    }
}

static void AU_Restore_Bank(AU_BankRegs_t *stBank)
{
    int i = 0, j = 0;

    STR_PRINT("stBank.u32Bank:%x\n", (int)stBank->u32Bank);

    for (i = 0; i < NumRows; i++)
    {
        for (j = 0; j < NumRegs; j++)
        {
            MS_U32 ReadRegAddr = stBank->u32Bank + ((i << 4) + (j << 1));
            //printk("ReadRegAddr = %x     ",ReadRegAddr);
            HAL_AUDIO_AbsWriteReg(ReadRegAddr, stBank->aud_row[i].aud_reg[j]);
            //printk("HAL_AUDIO_AbsReadReg(ReadRegAddr); = %x \n", HAL_AUDIO_AbsReadReg(ReadRegAddr));
            //printk("%-4x ", HAL_AUDIO_AbsReadReg(ReadRegAddr));
        }
        //printk("\n");
    }
}

static void AU_Backup_PlayStatus(void)
{
    eDecCmd = HAL_MAD_GetDecCmd();
    eSECmd = HAL_MAD2_GetDecCmd();
}

static void AU_Restore_PlayStatus(void)
{
    HAL_MAD_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_playControl, eDecCmd);
    HAL_MAD_SetAudioParam2(AU_DEC_ID3, Audio_ParamType_playControl, eSECmd);
}

#if 0
void AU_COMMON_SHM_Init(void)	//lugo
{
    AU_Backup_BaseAddr();
    //AudioShmInfo = (AUDIO_SHM_INFO *)ioremap((PhysicalAddr2BusAddr(HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC) + AU_SHM_COMMON_BASE)), AU_SHM_COMMON_SIZE);

    if (AudioShmInfo == NULL)
    {
        printk("Audio Common SHM remap fail\r\n");
    }
}
#endif

#if( LGSE_Restore_In_Kernel == 1)
static void AU_LGSE_Initialize(void)
{
    memset(U8LGSEFN_MODE_REGISTER, 0, sizeof(U8LGSEFN_MODE_REGISTER));
    memset(U8LGSEFNMAIN_INIT     , 0, sizeof(U8LGSEFNMAIN_INIT));
    memset(U8LGSEFNMAIN_VARIABLES, 0, sizeof(U8LGSEFNMAIN_VARIABLES));
    memset(U8LGSEFN000_INIT      , 0, sizeof(U8LGSEFN000_INIT));
    memset(U8LGSEFN000_VARIABLES , 0, sizeof(U8LGSEFN000_VARIABLES));
    memset(U8LGSEFN001_INIT      , 0, sizeof(U8LGSEFN001_INIT));
    memset(U8LGSEFN001_VARIABLES , 0, sizeof(U8LGSEFN001_VARIABLES));
    memset(U8LGSEFN004_MODE1_VARIABLES, 0, sizeof(U8LGSEFN004_MODE1_VARIABLES));
    memset(U8LGSEFN004_MODE2_VARIABLES, 0, sizeof(U8LGSEFN004_MODE2_VARIABLES));
    memset(U8LGSEFN009_VARIABLES , 0, sizeof(U8LGSEFN009_VARIABLES));
    memset(U8LGSEFN010_INIT      ,     0, sizeof(U8LGSEFN010_INIT));
    memset(U8LGSEFN017_VARIABLES , 0, sizeof(U8LGSEFN017_VARIABLES));
    memset(U8LGSEFN028_VARIABLES , 0, sizeof(U8LGSEFN028_VARIABLES));
}

extern MS_BOOL MDrv_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);

static void AU_LGSE_Backup(void)
{
    MS_U32 U32Ret = 0;

    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN_MODE_REGISTER     , 0, (MS_U32 *)U8LGSEFN_MODE_REGISTER     , 4);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFNMAIN_INIT          , 0, (MS_U32 *)U8LGSEFNMAIN_INIT          , 40);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFNMAIN_VARIABLES     , 0, (MS_U32 *)U8LGSEFNMAIN_VARIABLES     , 12);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN000_INIT           , 0, (MS_U32 *)U8LGSEFN000_INIT           , 1180);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN000_VARIABLES      , 0, (MS_U32 *)U8LGSEFN000_VARIABLES      , 4);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN001_INIT           , 0, (MS_U32 *)U8LGSEFN001_INIT           , 84);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN001_VARIABLES      , 0, (MS_U32 *)U8LGSEFN001_VARIABLES      , 520);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN004_MODE1_VARIABLES, 0, (MS_U32 *)U8LGSEFN004_MODE1_VARIABLES, 100);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN004_MODE2_VARIABLES, 0, (MS_U32 *)U8LGSEFN004_MODE2_VARIABLES, 60);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN009_VARIABLES      , 0, (MS_U32 *)U8LGSEFN009_VARIABLES      , 8);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN010_INIT           , 0, (MS_U32 *)U8LGSEFN010_INIT           , 40);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN017_VARIABLES      , 0, (MS_U32 *)U8LGSEFN017_VARIABLES      , 648);
    U32Ret += (MS_U32)MDrv_ADVSOUND_GetParam2(LGSEFN028_VARIABLES      , 0, (MS_U32 *)U8LGSEFN028_VARIABLES      , 8);

    //take care of this number
    if(U32Ret < 13)
    {
        printk("U32Ret: %u, LGSE may have problem during suspend\r\n", U32Ret);
        bIsSTR_Suspend_LGSE_Fail = TRUE;
    }
    else
    {
        bIsSTR_Suspend_LGSE_Fail = FALSE;
    }

#if 0 //for temporary debug
    MS_U32 u32Cnt;
    MS_U8  u8CurrentDecWhile, u8PreDecWhile;
    MS_U8  u8CurrentDecTimer, u8PreDecTimer;

    u8CurrentDecWhile = 0;
    u8PreDecWhile = 0;
    u8CurrentDecTimer = 0;
    u8PreDecTimer = 0;

    for(u32Cnt = 0; u32Cnt < 50; u32Cnt++)
    {
        u8CurrentDecWhile =  HAL_AUDIO_ReadDspCounter(DSP_DEC_MAIN_COUNTER);
        u8CurrentDecTimer =  HAL_AUDIO_ReadDspCounter(DSP_DEC_TIMER_COUNTER);

        if((u32Cnt != 0) && (u8PreDecWhile != u8CurrentDecWhile) && (u8PreDecTimer != u8CurrentDecTimer))
        {
            break;
        }
        else if(u32Cnt == 49)
        {
            printk("\033[1;32m [AUDIO] [%s] [%d] [R2 is not alive]\033[0m\n",  __FUNCTION__, __LINE__);/*@audio*/
        }

        u8PreDecWhile = u8CurrentDecWhile;
        u8PreDecTimer = u8CurrentDecTimer;
        MsOS_DelayTaskUs(200);
    }
#endif

}

static void AU_LGSE_Restore(void)
{
    MS_U32 U32Ret = 0;

#if 0 //for temporary debug
    MS_U32 u32Cnt;
    MS_U8  u8CurrentDecWhile, u8PreDecWhile;
    MS_U8  u8CurrentDecTimer, u8PreDecTimer;

    u8CurrentDecWhile = 0;
    u8PreDecWhile = 0;
    u8CurrentDecTimer = 0;
    u8PreDecTimer = 0;

    for(u32Cnt = 0; u32Cnt < 50; u32Cnt++)
    {
        u8CurrentDecWhile =  HAL_AUDIO_ReadDspCounter(DSP_DEC_MAIN_COUNTER);
        u8CurrentDecTimer =  HAL_AUDIO_ReadDspCounter(DSP_DEC_TIMER_COUNTER);

        if((u32Cnt != 0) && (u8PreDecWhile != u8CurrentDecWhile) && (u8PreDecTimer != u8CurrentDecTimer))
        {
            break;
        }
        else if(u32Cnt == 49)
        {
            printk("\033[1;32m [AUDIO] [%s] [%d] [R2 is not alive]\033[0m\n",  __FUNCTION__, __LINE__);/*@audio*/
        }

        u8PreDecWhile = u8CurrentDecWhile;
        u8PreDecTimer = u8CurrentDecTimer;
        MsOS_DelayTaskUs(200);
    }
#endif

    MDrv_ADVSOUND_SubProcessEnable(LG_SOUNDENGINE_EN  , TRUE);
    U32Ret += (MS_U32)MDrv_ADVSOUND_SetParam2(LGSEFN_MODE_REGISTER     , 0, (MS_U32 *)U8LGSEFN_MODE_REGISTER     , 4);    AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFNMAIN_INIT                            , 0, (MS_U32 *)U8LGSEFNMAIN_INIT          , 40);   AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFNMAIN_VARIABLES                       , 0, (MS_U32 *)U8LGSEFNMAIN_VARIABLES     , 12);   AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN000_INIT                             , 0, (MS_U32 *)U8LGSEFN000_INIT           , 1180); AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN000_VARIABLES                        , 0, (MS_U32 *)U8LGSEFN000_VARIABLES      , 4);    AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN001_INIT                             , 0, (MS_U32 *)U8LGSEFN001_INIT           , 84);   AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN001_VARIABLES                        , 0, (MS_U32 *)U8LGSEFN001_VARIABLES      , 520);  AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN004_MODE1_VARIABLES                  , 0, (MS_U32 *)U8LGSEFN004_MODE1_VARIABLES, 100);  AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN004_MODE2_VARIABLES                  , 0, (MS_U32 *)U8LGSEFN004_MODE2_VARIABLES, 60);   AUDIO_DELAY1US(100);
    U32Ret += (MS_U32)MDrv_ADVSOUND_SetParam2(LGSEFN009_VARIABLES      , 0, (MS_U32 *)U8LGSEFN009_VARIABLES      , 8);    AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN010_INIT                             , 0, (MS_U32 *)U8LGSEFN010_INIT           , 40);   AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN017_VARIABLES                        , 0, (MS_U32 *)U8LGSEFN017_VARIABLES      , 648);  AUDIO_DELAY1US(100);
    MDrv_ADVSOUND_SetParam2(LGSEFN028_VARIABLES                        , 0, (MS_U32 *)U8LGSEFN028_VARIABLES      , 8);    AUDIO_DELAY1US(100);

    if( (U32Ret < 2) || (bIsSTR_Suspend_LGSE_Fail == TRUE) )
    {
        printk("LGSE may have problem during resume\r\n");
        HAL_MAD_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_STR_Resume_LGSE_Fail, TRUE);
        bIsSTR_Suspend_LGSE_Fail = FALSE;
    }
}
#endif

static void AU_SIF_Restore_Prescale(void)
{
    if ((HAL_SIF_GetDspCodeType()) == AU_SIF_BTSC)
    {
        HAL_SIF_SetPrescale(SET_PRESCALE_BTSC_MONO  , 0);
        HAL_SIF_SetPrescale(SET_PRESCALE_BTSC_STEREO, 0);
        HAL_SIF_SetPrescale(SET_PRESCALE_BTSC_SAP   , 0);
    }
    else if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
    {
        HAL_SIF_SetPrescale(SET_PRESCALE_A2_FM  , 12);
        HAL_SIF_SetPrescale(SET_PRESCALE_HIDEV  , -36);
        HAL_SIF_SetPrescale(SET_PRESCALE_NICAM  , 28);
        HAL_SIF_SetPrescale(SET_PRESCALE_FM_M   , 0);
        HAL_SIF_SetPrescale(SET_PRESCALE_HIDEV_M, 0);
        HAL_SIF_SetPrescale(SET_PRESCALE_AM     , 0);
    }
}

static void AU_SIF_LoadCode(void)
{
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, FALSE, 0);	// REset SIF
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_SIF_SYNTHESIZER, FALSE, 0x00);	// disable SIF Audio sythesizer & disable DVB fix-sync mode

    HAL_SIF_AbsWriteMaskByte(0x112CC9, 0x20, 0x20);   //0x112CC8[13] = 1 to IDMA port selection to SE DSP

    MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);
    if(HAL_AUDIO_Alg2ReloadCode(SifCodeType) == TRUE)
    {
        HAL_SIF_SetDspCodeType(SifCodeType);
    }
    MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, 0);
    // Enable SIF Audio synthesizer here to prevent reload unstable noise.
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_SIF_SYNTHESIZER, TRUE, 0x04);	// enable SIF Audio sythesizer

    if (SifCodeType == AU_SIF_PALSUM)
    {
        HAL_SIF_SendCmd(AU_SIF_CMD_SET_CARRIER_DEBOUNCE, 0x80, 0);
        HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, TRUE, 0);
        HAL_SIF_SendCmd(AU_SIF_CMD_DK123_MONITOR, TRUE, 0);
    }

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_MUTE, FALSE, 0);
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_SOUNDMODE, TRUE, 0);

    HAL_SIF_TriggerSifPLL();
}

#ifdef AUDIO_STR_TEST
extern void _MApi_AUDIO_Initialize(void);
extern void _MApi_AUDIO_WritePreInitTable(void);
extern MS_U8 _MApi_Audio_Monitor(void);
#endif

void _AUDIO_STR_SHOW_VERSION(void);

extern int suspend_audio(void)
{
    STR_PRINT("enter\n");
    _AUDIO_STR_SHOW_VERSION();

#ifdef AUDIO_STR_TEST
    MsOS_MPool_Mapping_Dynamic(0, 0x18900000, 0x00E00000, TRUE);
    _MApi_AUDIO_WritePreInitTable();

    HAL_AUDIO_SetDspBaseAddr(DSP_ADV, 0, 0x0018900000);

    _MApi_AUDIO_Initialize();

    STR_PRINT("Audio init Done!!!\n");
    MsOS_DelayTask(100);
#endif

    AU_Backup_BaseAddr();

    AU_Backup_PlayStatus();

    //Mute CH5/CH6 to prevent loud sound when resume due to LGSE reload is not finished.
    //The mute will be recovered to original status by _HAL_AUDIO_MuteDuringLimitedTime_Input in _HAL_AUDIO_POWER_ON_Monitor.
    HAL_SOUND_SetMixModeMute(E_AUDIO_INFO_GAME_IN, GAME1_VOL, TRUE);
    HAL_SOUND_SetMixModeMute(E_AUDIO_INFO_GAME_IN, GAME2_VOL, TRUE);

    //AU_BankRegs_Initialize(0x112A00, &AU_Bank112A);  // ???
    //AU_BankRegs_Initialize(0x112B00, &AU_Bank112B);  // ???
    AU_BankRegs_Initialize(0x112C00, &AU_Bank112C);
    AU_BankRegs_Initialize(0x112D00, &AU_Bank112D);
    AU_BankRegs_Initialize(0x112E00, &AU_Bank112E);
    AU_BankRegs_Initialize(0x160300, &AU_Bank1603);

    //AU_Backup_Bank(&AU_Bank112A);
    //AU_Backup_Bank(&AU_Bank112B);
    AU_Backup_Bank(&AU_Bank112C);
    AU_Backup_Bank(&AU_Bank112D);
    AU_Backup_Bank(&AU_Bank112E);
    AU_Backup_Bank(&AU_Bank1603);


#if(LGSE_Restore_In_Kernel == 1)
    //Back up LGSE parameters
    AU_LGSE_Initialize();
    AU_LGSE_Backup();
#endif

    Dsp2CodeType = HAL_AUDIO_GetDsp2CodeType();
    SifCodeType = HAL_SIF_GetDspCodeType();
    ADEC0_R2CodeType = HAL_AUDIO_GetDecR2_ADEC1_DecType();
    ADEC1_R2CodeType = HAL_AUDIO_GetDecR2_ADEC2_DecType();

    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
    HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_STOP);

    STR_PRINT("exit\n");
    return 0;
}

#define AU_STR_RESUME_TIME_MEASURE 0
#if AU_STR_RESUME_TIME_MEASURE
unsigned int u32Timer;
unsigned int u32Timer_us;
void AU_STR_Resume_Time_Measure_Timer_Reset(void)
{
    u32Timer = 0;
    u32Timer_us = 0;
    REG_WR(REG_PIU_TIMER0(0x00), (REG_RR(REG_PIU_TIMER0(0x00)) & (~(0x1))));
    REG_WR(REG_PIU_TIMER0(0x02), 0xFFFF);
    REG_WR(REG_PIU_TIMER0(0x03), 0xFFFF);
    REG_WR(REG_PIU_TIMER0(0x00), (REG_RR(REG_PIU_TIMER0(0x00)) | (0x1)));
}
void AU_STR_Resume_Time_Measure_Read_Timer(void)
{
    u32Timer = (((MS_U32)REG_RR(REG_PIU_TIMER0(0x04))) | ((MS_U32)REG_RR(REG_PIU_TIMER0(0x05)) << 16));
    u32Timer_us = ((u32Timer))/12;
    printk("resume time : %d.%06ds\n", u32Timer_us/1000000, u32Timer_us%1000000);
}
#endif

MS_BOOL HAL_AUDIO_SeSystemLoadCode_STR(void)
{
    MS_U16 time_out = 0;

    HAL_MAD2_SetDspIDMA();

    //!!! IMPORTANT NOTE: 0x61 is used as AU_SE_SYSTEM_STR, dont use 0x61 anymore!!!
    //original is HAL_AUDSP_DspLoadCode(AU_SE_SYSTEM);
    //in STR mode, use 0x61 to turn on STR flag.
    HAL_AUDSP_DspLoadCode(0x61);

    //Wait Dsp init finished Ack
    while(time_out++<100)
    {
        if(HAL_MAD2_GetLoadCodeAck() == 0xE3)
            break;
        AUDIO_DELAY1US(1000);
    }

    if(time_out >= 100)
    {
        printk("DSP2 Re-Active\r\n");
    }

    //inform DSP to start to run
    HAL_MAD2_SetMcuCmd(0xF3);

    return TRUE;
}

extern int resume_audio_part1(void)
{
    MS_U32 time_out = 0;

    STR_PRINT("enter\n");

#if AU_STR_RESUME_TIME_MEASURE
    AU_STR_Resume_Time_Measure_Timer_Reset();
    //­n¶q´úªºfunction()
    AU_STR_Resume_Time_Measure_Read_Timer();printk("\033[1;34m [AUDIO][STR][%s] [%d] [] \033[0m \n", __FUNCTION__, __LINE__);/*@audio*/
#endif

    HAL_AUDIO_WritePreInitTable();
    HAL_AUDIO_WriteInitTable();
    HAL_AUDIO_SetPowerOn(TRUE);
    HAL_AUDIO_AbsWriteMaskReg(0x112CEE, 0x1000, 0x1000); //ADC§Ö³t¥R¹q

    //Mute CH5/CH6 to prevent loud sound when resume due to LGSE reload is not finished.
    //The mute will be recovered to original status by _HAL_AUDIO_MuteDuringLimitedTime_Input in _HAL_AUDIO_POWER_ON_Monitor.
    HAL_SOUND_SetMixModeMute(E_AUDIO_INFO_GAME_IN, GAME1_VOL, TRUE);
    HAL_SOUND_SetMixModeMute(E_AUDIO_INFO_GAME_IN, GAME2_VOL, TRUE);

#if 0 //ADC§Ö³t¥R¹q (ÀÉ¦ì)
    0x112CE8[13:12]
    00: 150ms , default
    01: 100ms
    10: 200ms
    11: 300ms
#endif

    HAL_AUDIO_SetDspBaseAddr(0,MAD_Base_Addr,MAD_Base_Addr);
    HAL_MAD_SetMemInfo();
    HAL_MAD2_SetMemInfo();

    //Load DEC-DSP
    //consider ATV/Encode case

    HAL_SOUND_Init();
    HAL_AUDSP_DspLoadCode(AU_DVB2_NONE);
    HAL_AUDIO_SeSystemLoadCode_STR();
    HAL_MAD_Init();

    if ((SifCodeType & 0xF0) == AU_STANDARD_SIF_TYPE)
    {
        if (g_AudioVars2->eAudioSource != E_AUDIO_INFO_DTV_IN) //for ATV & APVR case, need to reload SIF
        {
            AU_SIF_LoadCode();
        }
        else //If DTV, don't need to load SIF to prevent resume time becomes longer. Reset g_AudioVars2->g_u8SifCodeType here, and it would reload when start play by AP
        {
            HAL_SIF_SetDspCodeType(AU_SIF_NONE);
        }
    }

    if ((Dsp2CodeType & 0xF0) == AU_DVB2_ENCODE_NONE)	// Consider DSP encoder case
    {
        /***********************
        If load encoder here, resume time would be longer.
        So reset clear g_AudioVars2->g_u8Dsp2CodeType here, and it would reload when start play by AP
        ***********************/
        HAL_AUDIO_SetDsp2CodeType(AU_DVB2_ENCODE_NONE, TRUE);
    }

    //AU_Restore_Bank(&AU_Bank112A); // need to confirm
    //AU_Restore_Bank(&AU_Bank112B); // need to confirm
    AU_Restore_Bank(&AU_Bank112C);
    AU_Restore_Bank(&AU_Bank112D);
    AU_Restore_Bank(&AU_Bank112E);
    AU_Restore_Bank(&AU_Bank1603);

    HAL_AUDIO_AbsWriteMaskReg(0x112CEE, 0x1000, 0x1000); //ADC§Ö³t¥R¹q

    STR_PRINT("exit\n");
    return 0;
}

extern int resume_audio_part2(void)
{
    STR_PRINT("enter\n");

    //Restore decoder type
    if(ADEC0_R2CodeType != AU_DVB_STANDARD_INVALID)
    {
        HAL_AUDIO_SetSystem2(AU_DEC_ID1, ADEC0_R2CodeType);
    }
    if(ADEC1_R2CodeType != AU_DVB_STANDARD_INVALID)
    {
        HAL_AUDIO_SetSystem2(AU_DEC_ID3, ADEC1_R2CodeType);
    }

    //init HW DMA for ALSA sound
    PCM_OUTPUT_FORMAT stPcmSettings;
    stPcmSettings.eFs = SAMPLE_RATE_48000;
    stPcmSettings.bLittleEndian = TRUE;
    stPcmSettings.u8Channels = 2;
    stPcmSettings.u8ByteWidth = 2;
    stPcmSettings.bMainChannel = FALSE;
    stPcmSettings.u8PathIdx = 0;
    stPcmSettings.OMX_ArenderPathID = 0;

    HAL_AUDIO_DmaReader_Init(&stPcmSettings);
    HAL_MAD_SetCommInfo(Audio_Comm_infoType_DMAReader_Command, TRUE, 0);
    HAL_MAD_SetCommInfo(Audio_Comm_infoType_ADEC1_DPFG_Flag, TRUE, 0);
    HAL_MAD_SetCommInfo(Audio_Comm_infoType_ADEC2_DPFG_Flag, TRUE, 0);
    HAL_MAD_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_HDMI_RX_Unstable_Threshold, 0x600);
    HAL_MAD_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_HDMI_Event_Mute_Enable, TRUE);

    //turn off STR flag!
    HAL_AUDSP_DspLoadCode(0x61);

#if(LGSE_Restore_In_Kernel == 1)
    // Reload LGSE parameters
    AU_LGSE_Restore();
#endif

    HAL_MAD_SetAudioParam2(AU_DEC_ID1, Audio_ParamType_STR_Resume, TRUE);

    STR_PRINT("exit\n");

    return 0;
}


#define AUDIO_STR_VERSION 0x002B
void _AUDIO_STR_SHOW_VERSION(void)
{
    STR_PRINT("\n");
    STR_PRINT("[--------------------------------------]\n");/*@audio*/
    STR_PRINT("[AU_STR] VERSION...¡¹[0x%-4X][0x%-4X]¡¹]\n", __LINE__, AUDIO_STR_VERSION);/*@audio*/
    STR_PRINT("[--------------------------------------]\n");/*@audio*/
}
