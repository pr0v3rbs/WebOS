
#define _MHAL_DEMOD_C_
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/io.h>

#include "mdrv_types.h"
    
#include "mhal_demod.h"
#include "mhal_demod_reg.h"

#include "mdrv_system.h"
#include "mdrv_system_LG.h"

//----------------------------------------------------------------------
#define OPT_DEMOD_HAL_DEBUG
#undef DEMOD_HAL_DBG
#ifdef OPT_DEMOD_HAL_DEBUG
    #define DEMOD_HAL_DBG(fmt, args...)      printk(KERN_WARNING "[DEMOD_HAL][%05d]" fmt, __LINE__, ## args)
#else
    #define DEMOD_HAL_DBG(fmt, args...)
#endif

#undef DEMOD_HAL_DBGX
#define DEMOD_HAL_DBGX(fmt, args...)
//----------------------------------------------------------------------
//======================== DEMOD TOP =================================//
//DMD_TOP
#define DMDTOP_DEMOD_ENA_L_ATSC                 0x01 //ATSC, BIT0
#define DMDTOP_DEMOD_ENA_L_ISDBT                0x08 //isdbt, BIT3

#define DEMOD_HW_ATSC_ENABLE              0x08
#define DEMOD_HW_J83B_ENABLE              0x04


#define FW_MAX_LENGTH 28672 //27648 //1024*27 (Mustang could extend to 32KB)

static unsigned char fw_DEMOD[FW_MAX_LENGTH];
//** Debug **//
#define DEBUG_MSG       0 // 1

static struct save_demod_mode_setting_t {
    //FLAG
    U16 u16DEMOD_ATSC_ELAG;
    U16 u16DEMOD_ISDBT_ELAG;
    //MOD_MODE
    U16 u16DEMOD_ATSC_MODE;
    U16 u16DEMOD_J83B_MODE;
}save_demod_mode_setting;
////////////////////////////////////////////////////////////////

#define DEMOD_VERIFY_DSP 1
#define DVBT2_PSRAM_LENGTH 0x8000

static U8 _dsp_dvbt[] = {
    #include "M2R_DVBT_R22.04.dat"
};

static U8 _dsp_dvbt2[] = {
    #include "M2R_DVBT2_R3E.11.dat"
};

static U8 _dsp_dvbc[] = {
    #include "M2R_DVBC_R18.05.dat"
};

static U8 _dsp_dvbs[] = {
    #include "M2R_DVBS2_R00_11.dat"
};

static U8 _dsp_atsc[] = {
    #include "M2R_E_2_1_ATSC_demod_atsc.dat"
};
static U8 _dsp_atsc_sanyo[] = {
    #include "M2R_E_2_1_ATSC_demod_atsc_sanyo.dat"
};
static U8 _dsp_isdbt[] = {
    #include "M2R_1_1_11_ISDBT_demod.dat"
};

static U8 _dsp_dtmb[] = {
    #include "LM14A_1_0_0_internal_demod_dtmb.dat"
};

void MHal_Demod_WriteReg(U32 u32Addr, U8 u8Value)
{
    _MHal_W1B(BYTE2REAL(u32Addr & 0x000FFFFF), u8Value);
    //DEMOD_HAL_DBG("set reg(0x%06x) to 0x%02x\n", u32Addr, u8Value);
}

void MHal_Demod_ReadReg(U32 u32Addr, U8 *pu8Value)
{
    *pu8Value = _MHal_R1B(BYTE2REAL(u32Addr & 0x000FFFFF));
    //DEMOD_HAL_DBG("reg(0x%06x) = 0x%02x\n", u32Addr, *pu8Value);
}

B16 MHal_Demod_MB_WriteReg(U32 u32Addr, U8 u8Value)
{
    U8        RegVal = 0;
    U16        u8WaitCnt = 0;
    VDMCU_DSP_TYPE_t mcu_type = MDrv_SYS_VDmcuGetType();

    //mail box crash protection 2009-11-06
    switch(mcu_type)
    {
        case VDMCU_DSP_DVBT:
        case VDMCU_DSP_DVBC:
        case VDMCU_DSP_DVBT2:
        case VDMCU_DSP_DVBS:
            break;

        default:
            printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, mcu_type);
            return FALSE;
    }

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

    if (RegVal)
    {
        //Driver update 2009/11/20
        U8  tmp_h, tmp_l;
        U16 tmp_addr;
        MHal_Demod_ReadReg(MBRegBase + 0x02, &tmp_h);
        MHal_Demod_ReadReg(MBRegBase + 0x01, &tmp_l);
        tmp_addr = ( (tmp_h << 8) | tmp_l );

        DEMOD_HAL_DBG("Mailbox crash (write reg 0x%04x) (last reg 0x%04x)\n", u32Addr, tmp_addr);
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x02, (U8)(u32Addr >> 8));        // ADDR_H
    MHal_Demod_WriteReg(MBRegBase + 0x01, (U8)u32Addr);                // ADDR_L
    MHal_Demod_WriteReg(MBRegBase + 0x03, u8Value);                    // REG_DATA
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x02);                    // MB_CNTL set write mode
//andy 20090720 update start
    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x02);        // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x00);        // de-assert interrupt to VD MCU51
//andy 20090720 update end
    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0xFF)            // wait done flag
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

        if (u8WaitCnt++ > 0x7FFF)
        {
            DEMOD_HAL_DBG("Demod WriteReg Fail!\n");
            return FALSE;
        }
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                    // MB_CNTL clear

    return TRUE;
}

B16 MHal_Demod_MB_ReadReg(U32 u32Addr, U8 *pu8Value)
{
    U16        u8WaitCnt = 0;
    U8        RegVal;
    VDMCU_DSP_TYPE_t mcu_type = MDrv_SYS_VDmcuGetType();

    //mail box crash protection 2009-11-06
    switch(mcu_type)
    {
        case VDMCU_DSP_DVBT:
        case VDMCU_DSP_DVBC:
        case VDMCU_DSP_DVBT2:
        case VDMCU_DSP_DVBS:
            break;

        default:
            printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, mcu_type);
            return FALSE;
    }

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

    if (RegVal)
    {
        //Driver update 2009/11/20
        U8  tmp_h, tmp_l;
        U16 tmp_addr;
        MHal_Demod_ReadReg(MBRegBase + 0x02, &tmp_h);
        MHal_Demod_ReadReg(MBRegBase + 0x01, &tmp_l);
        tmp_addr = ( (tmp_h << 8) | tmp_l );

        DEMOD_HAL_DBG("Mailbox crash (read reg 0x%04x) (last reg 0x%04x)\n", u32Addr, tmp_addr);
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x02, (U8)(u32Addr >> 8));        // ADDR_H
    MHal_Demod_WriteReg(MBRegBase + 0x01, (U8)u32Addr);                // ADDR_L
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x01);                    // MB_CNTL set read mode
//andy new driver update 090720 start
    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x02);        // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x00);        // de-assert interrupt to VD MCU51
//andy new driver update 090720 end
    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0xFF)            // wait MB_CNTL set done
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

        if (u8WaitCnt++ > 0x7FFF)
        {
            DEMOD_HAL_DBG("Demod ReadReg Fail!\n");
            return FALSE;
        }
    }

    MHal_Demod_ReadReg(MBRegBase + 0x03, pu8Value);                    // REG_DATA get
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                    // MB_CNTL clear

    return TRUE;
}

B16 MHal_Demod_MB_WriteDspReg(U32 u32Addr, U8 u8Value)
{
    U8        RegVal = 0;
    U16        u8WaitCnt = 0;
    VDMCU_DSP_TYPE_t mcu_type = MDrv_SYS_VDmcuGetType();

    switch(mcu_type)
    {
        case VDMCU_DSP_DVBT:
        case VDMCU_DSP_DVBC:
        case VDMCU_DSP_DVBT2:
        case VDMCU_DSP_DVBS:
            break;

        default:
            printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, mcu_type);
            return FALSE;
    }

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

    if (RegVal)
    {
        U8  tmp_h, tmp_l;
        U16 tmp_addr;
        MHal_Demod_ReadReg(MBRegBase + 0x02, &tmp_h);
        MHal_Demod_ReadReg(MBRegBase + 0x01, &tmp_l);
        tmp_addr = ( (tmp_h << 8) | tmp_l );

        DEMOD_HAL_DBG("Mailbox crash (write reg 0x%04x) (last reg 0x%04x)\n", u32Addr, tmp_addr);
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x02, (U8)(u32Addr >> 8));        // ADDR_H
    MHal_Demod_WriteReg(MBRegBase + 0x01, (U8)u32Addr);                // ADDR_L
    MHal_Demod_WriteReg(MBRegBase + 0x03, u8Value);                    // REG_DATA
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x04);                    // MB_CNTL set write dsp mode

    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x02);                    // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x00);                    // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0xFF)                                            // wait done flag
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

        if (u8WaitCnt++ > 0x7FFF)
        {
            DEMOD_HAL_DBG(">> DVBT WriteDspReg Fail!");
            return FALSE;
        }
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                    // MB_CNTL clear

    return TRUE;

}

B16 MHal_Demod_MB_ReadDspReg(U32 u32Addr, U8 *pu8Value)
{
    U16        u8WaitCnt = 0;
    U8        RegVal;
    VDMCU_DSP_TYPE_t mcu_type = MDrv_SYS_VDmcuGetType();

    switch(mcu_type)
    {
        case VDMCU_DSP_DVBT:
        case VDMCU_DSP_DVBC:
        case VDMCU_DSP_DVBT2:
        case VDMCU_DSP_DVBS:
            break;

        default:
            printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, mcu_type);
            return FALSE;
    }

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

    if (RegVal)
    {
        U8  tmp_h, tmp_l;
        U16 tmp_addr;
        MHal_Demod_ReadReg(MBRegBase + 0x02, &tmp_h);
        MHal_Demod_ReadReg(MBRegBase + 0x01, &tmp_l);
        tmp_addr = ( (tmp_h << 8) | tmp_l );

        DEMOD_HAL_DBG("Mailbox crash (read reg 0x%04x) (last reg 0x%04x)\n", u32Addr, tmp_addr);
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x02, (U8)(u32Addr >> 8));        // ADDR_H
    MHal_Demod_WriteReg(MBRegBase + 0x01, (U8)u32Addr);                // ADDR_L
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x03);                    // MB_CNTL set read dsp mode

    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x02);                    // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(VDMcuBase + 0x83, 0x00);                    // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0xFF)                                            // wait MB_CNTL set done
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);

        if (u8WaitCnt++ > 0x7FFF)
        {
            DEMOD_HAL_DBG(">> DVB(%d) ReadReg Fail!", mcu_type);
            return FALSE;
        }
    }

    MHal_Demod_ReadReg(MBRegBase + 0x03, pu8Value);                    // REG_DATA get
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                    // MB_CNTL clear

    return TRUE;

}

B16 MHal_Demod_MB_WriteReg_ATSC(U32 u32Addr, U8 u8Value)
{
    U16 u16CheckCount;
    U8 u8CheckFlag;

    //mail box crash protection 2009-11-06
    if (VDMCU_DSP_ATSC != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }


    MHal_Demod_WriteReg(0x112600, (u32Addr&0xff));
    MHal_Demod_WriteReg(0x112601, (u32Addr>>8));
    MHal_Demod_WriteReg(0x112610, u8Value);
    MHal_Demod_WriteReg(0x11261E, 0x01);

      MHal_Demod_WriteReg(0x103483, 0x02);        // assert interrupt to VD MCU51
      MHal_Demod_WriteReg(0x103483, 0x00);        // de-assert interrupt to VD MCU51

    // andy 2009-7-27 ¢¯AAu 11:35:48 for ( u16CheckCount=0; u16CheckCount < 1000 ; u16CheckCount++ )//
    for ( u16CheckCount=0; u16CheckCount < 2000 ; u16CheckCount++ )//total 200ms
    {
      MHal_Demod_ReadReg(0x11261E, &u8CheckFlag);
      if ((u8CheckFlag&0x01)==0)
        return TRUE;
        udelay(10);
    }

    printk("\n###MHal_Demod_MB_WriteReg_ATSC time out### Addr=[0x%X], Value=[0x%X]\n\n", u32Addr, u8Value);

    return FALSE;
}

B16 MHal_Demod_MB_ReadReg_ATSC(U32 u32Addr, U8 *pu8Value)
{
    U16 u16CheckCount;
    U8 u8CheckFlag;

    //mail box crash protection 2009-11-06
    if (VDMCU_DSP_ATSC != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }

    MHal_Demod_WriteReg(0x112600, (u32Addr&0xff));
    MHal_Demod_WriteReg(0x112601, (u32Addr>>8));
    MHal_Demod_WriteReg(0x11261E, 0x02);

      MHal_Demod_WriteReg(0x103483, 0x02);        // assert interrupt to VD MCU51
      MHal_Demod_WriteReg(0x103483, 0x00);        // de-assert interrupt to VD MCU51

    for ( u16CheckCount=0; u16CheckCount < 2000 ; u16CheckCount++ )//andy new driver update 090720
    {
      MHal_Demod_ReadReg(0x11261E, &u8CheckFlag);
      if ((u8CheckFlag&0x02)==0)
      {
        MHal_Demod_ReadReg(0x112610, pu8Value);
        return TRUE;
      }
        udelay(10);
    }
    return FALSE;
}

B16 MHal_Demod_MB_WriteReg_ISDBT(U32 u32Addr, U8 u8Value)
{
    U16 u16CheckCount;
    U8 u8CheckFlag;

    //mail box crash protection 2009-11-06
    if (VDMCU_DSP_ISDBT != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }

    MHal_Demod_WriteReg(0x112600, (u32Addr&0xff));
    MHal_Demod_WriteReg(0x112601, (u32Addr>>8));
    MHal_Demod_WriteReg(0x112610, u8Value);
    MHal_Demod_WriteReg(0x11261E, 0x01);

      MHal_Demod_WriteReg(0x103483, 0x02);        // assert interrupt to VD MCU51
      MHal_Demod_WriteReg(0x103483, 0x00);        // de-assert interrupt to VD MCU51

    // andy 2009-7-27 ¢¯AAu 11:35:48 for ( u16CheckCount=0; u16CheckCount < 1000 ; u16CheckCount++ )//
    for ( u16CheckCount=0; u16CheckCount < 2000 ; u16CheckCount++ )//total 200ms
    {
      MHal_Demod_ReadReg(0x11261E, &u8CheckFlag);
      if ((u8CheckFlag&0x01)==0)
        return TRUE;
            udelay(10);
    }

    printk("\n###MHal_Demod_MB_WriteReg_ISDBT time out### Addr=[0x%X], Value=[0x%X]\n\n", u32Addr, u8Value);

    return FALSE;
}

B16 MHal_Demod_MB_ReadReg_ISDBT(U32 u32Addr, U8 *pu8Value)
{
    U16 u16CheckCount;
    U8 u8CheckFlag;

    //mail box crash protection 2009-11-06
    if (VDMCU_DSP_ISDBT != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }

    MHal_Demod_WriteReg(0x112600, (u32Addr&0xff));
    MHal_Demod_WriteReg(0x112601, (u32Addr>>8));
    MHal_Demod_WriteReg(0x11261E, 0x02);

       MHal_Demod_WriteReg(0x103483, 0x02);        // assert interrupt to VD MCU51
       MHal_Demod_WriteReg(0x103483, 0x00);        // de-assert interrupt to VD MCU51

       for ( u16CheckCount=0; u16CheckCount < 2000 ; u16CheckCount++ )//andy new driver update 090720
    {
      MHal_Demod_ReadReg(0x11261E, &u8CheckFlag);
      if ((u8CheckFlag&0x02)==0)
      {
        MHal_Demod_ReadReg(0x112610, pu8Value);
        return TRUE;
      }
            udelay(10);
    }
    return FALSE;
}

B16 MHal_Demod_MB_WriteReg_DTMB(U32 u32Addr, U8 u8Value)
{
    U16 u16CheckCount;
    U8 u8CheckFlag;

    //mail box crash protection 2009-11-06
    if (VDMCU_DSP_DTMB != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }

    MHal_Demod_WriteReg(0x112600, (u32Addr&0xff));
    MHal_Demod_WriteReg(0x112601, (u32Addr>>8));
    MHal_Demod_WriteReg(0x112610, u8Value);
    MHal_Demod_WriteReg(0x11261E, 0x01);

    MHal_Demod_WriteReg(0x103483, 0x02);        // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);        // de-assert interrupt to VD MCU51

    // andy 2009-7-27 ¢¯AAu 11:35:48 for ( u16CheckCount=0; u16CheckCount < 1000 ; u16CheckCount++ )//
    for ( u16CheckCount=0; u16CheckCount < 2000 ; u16CheckCount++ )//total 200ms
    {
        MHal_Demod_ReadReg(0x11261E, &u8CheckFlag);
        if ((u8CheckFlag&0x01) == 0)
           return TRUE;
        udelay(10);
    }

    printk("\n###MHal_Demod_MB_WriteReg_DTMB time out### Addr=[0x%X], Value=[0x%X]\n\n", u32Addr, u8Value);

    return FALSE;
}

B16 MHal_Demod_MB_ReadReg_DTMB(U32 u32Addr, U8 *pu8Value)
{
    U16 u16CheckCount;
    U8 u8CheckFlag;

    //mail box crash protection 2009-11-06
    if (VDMCU_DSP_DTMB != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }

    MHal_Demod_WriteReg(0x112600, (u32Addr&0xff));
    MHal_Demod_WriteReg(0x112601, (u32Addr>>8));
    MHal_Demod_WriteReg(0x11261E, 0x02);

    MHal_Demod_WriteReg(0x103483, 0x02);        // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);        // de-assert interrupt to VD MCU51

    for ( u16CheckCount=0; u16CheckCount < 2000 ; u16CheckCount++ )//andy new driver update 090720
    {
        MHal_Demod_ReadReg(0x11261E, &u8CheckFlag);
        if ((u8CheckFlag&0x02) == 0)
        {
           MHal_Demod_ReadReg(0x112610, pu8Value);
           return TRUE;
        }
     udelay(10);
    }
    return FALSE;
}

// 0402 change for tuner option
// update 0402 for ATSC
U8 Demod_Flow_register_Tuner_Innotek[17] =   {0x52, 0x72, 0x52, 0x72, 0x4C, 0x52, 0xA3, 0xEC, 0xEA, 0x05,
    0x74, 0x1E, 0x42, 0x42, 0x1C, 0x62, 0x00};//#03.0E

U8 Demod_Flow_register_Tuner_Sanyo[17] =   {0x52, 0x72, 0x52, 0x72, 0x60, 0x60, 0xA4, 0xEC, 0xEA, 0x05,
        0x74, 0x1E, 0x5C, 0x5C, 0x08, 0x64, 0x0C};//#03.10

// 0402 change for tuner option
B16 MHal_Demod_LoadDSPCode(DEMOD_DSP_PARAM_t stParam)
{
    DEMOD_STANDARD_TYPE_t eStandard;
    TUNER_TYPE_t eTunerType;

    U8* pu8Demod_DSP;
    U32 u32Demod_DSP_len;
    U32 BUS_BASE = 0;
    static U32 u32T2FWStartPA = 0;
    static U8 *pu8T2FWStartVA = NULL;

    U32 i;
    B16 bRet = TRUE;
    U16 u16AddressOffset;
    U8  u8Data, u8TmpData;
// 0402 change for tuner option
    eStandard = stParam.eDSP_Type;
    eTunerType = stParam.eTuner_Type;

    //DSP branch
    if(eStandard == DEMOD_STANDARD_DVBT)
    {
        pu8Demod_DSP = (U8*)_dsp_dvbt;
        u32Demod_DSP_len = sizeof(_dsp_dvbt);
    }
    else if (eStandard == DEMOD_STANDARD_DVBC)
    {
        pu8Demod_DSP = (U8*)_dsp_dvbc;
        u32Demod_DSP_len = sizeof(_dsp_dvbc);
    }
    else if (eStandard == DEMOD_STANDARD_ATSC)
    {
        if(eTunerType == TUNER_TYPE_INNOTEK)
        {
            pu8Demod_DSP = (U8*)_dsp_atsc;
            u32Demod_DSP_len = sizeof(_dsp_atsc);
        }
    else
    {
        pu8Demod_DSP = (U8*)_dsp_atsc_sanyo;
            u32Demod_DSP_len = sizeof(_dsp_atsc_sanyo);
    }
    }
    else if (eStandard == DEMOD_STANDARD_ISDBT)
    {
        pu8Demod_DSP = (U8*)_dsp_isdbt;
        u32Demod_DSP_len = sizeof(_dsp_isdbt);
    }
    else if (eStandard == DEMOD_STANDARD_DTMB)
    {
        pu8Demod_DSP = (U8*)_dsp_dtmb;
        u32Demod_DSP_len = sizeof(_dsp_dtmb);
    }
    else if (eStandard == DEMOD_STANDARD_DVBS)
    {
        pu8Demod_DSP = (U8*)_dsp_dvbs;
        u32Demod_DSP_len = sizeof(_dsp_dvbs);
    }    
    else if (eStandard == DEMOD_STANDARD_DVBT2)
    {
        if (sizeof(_dsp_dvbt2) < 0x8000)
        {
            printk("dvbt2 image size incorrect\n");
            MDrv_SYS_VDmcuSetType(VDMCU_DSP_UNKNOWN);
            return FALSE;
        }
        pu8Demod_DSP = (U8*)_dsp_dvbt2;
        u32Demod_DSP_len = DVBT2_PSRAM_LENGTH;
    }
    else
    {
        DEMOD_HAL_DBG("no such standard %d\n", eStandard);
        //mail box crash protection 2009-11-06
        MDrv_SYS_VDmcuSetType(VDMCU_DSP_UNKNOWN);
        return FALSE;
    }

    DEMOD_HAL_DBG("DSP type = %d, code size = %u\n", eStandard, u32Demod_DSP_len);

    // Prevent over the max size of VD DSP code.
    if(u32Demod_DSP_len > 0x9000)
    {
        printk("DSP code size (%u) is larger than 35k bytes\n", u32Demod_DSP_len);
        //mail box crash protection 2009-11-06
        MDrv_SYS_VDmcuSetType(VDMCU_DSP_UNKNOWN);
        return FALSE;
    }

#if 0
    if (1 <= Chip_Query_Rev())
    {
        *((volatile unsigned int*)0xBF201684) &= ~0x1F ;
        *((volatile unsigned int*)0xBF201684) |= 0x0D ;
    }
#endif
    /*remapping reset*/
    _MHal_W1Bb(VD_MCU_RST, 0x01, _BIT1); //by RIU R/W
    /* reset VD MCU */
    _MHal_W1Bb(VD_MCU_RST, 0x01, _BIT0);

    /* disable sram */
    _MHal_W1Bb(VD_MCU_SRAM_EN, 0x00, _BIT0);

    /* release VD_MCU    , fixed unlock issue for different standards switch */
    _MHal_W1Bb(VD_MCU_RST, 0x00, _BIT0);

    /* enable down load code */
    _MHal_W1BM(VD_MCU_KEY, 0x50, 0xF0);

    /* enable address auto increment */
    _MHal_W1Bb(VD_MCU_ADDR_AUTO_INC, 0x01, _BIT0);

    /* reset sram address to 0 */
    _MHal_W1B(VD_MCU_ADDR_L, 0x00);
    _MHal_W1B(VD_MCU_ADDR_H, 0x00);

    /* download code */
    for (i=0; i<u32Demod_DSP_len; i++)
    {
        //access VD_MCU_WDATA and VD_MCU_WDATA_CTRL must use BYTE access
        _MHal_W1B(VD_MCU_SRAM_WD, pu8Demod_DSP[i]);
    }

    // update 0929 for ATSC
    if (eStandard == DEMOD_STANDARD_ATSC)
    {
        /* reset sram address to 0 */
        //_MHal_W1B(VD_MCU_ADDR_L, 0x80);
        //_MHal_W1B(VD_MCU_ADDR_H, 0x6B); // VT modify according toFW
        _MHal_W1B(VD_MCU_ADDR_L, 0x00); //Musting need extend code size.(28672 KB)
        _MHal_W1B(VD_MCU_ADDR_H, 0x70);//0x6C); // VT modify according toFW 

        // 0402 change for tuner option
        /* download code */

        switch (eTunerType)  //PLEASE FIX IT
        {
            case TUNER_TYPE_INNOTEK:
                for (i=0; i<sizeof(Demod_Flow_register_Tuner_Innotek); i++)
                {
                    //access VD_MCU_WDATA and VD_MCU_WDATA_CTRL must use BYTE access
                    _MHal_W1B(VD_MCU_SRAM_WD, Demod_Flow_register_Tuner_Innotek[i]);
                }
                break;

            case TUNER_TYPE_SANYO:
                for (i=0; i<sizeof(Demod_Flow_register_Tuner_Sanyo); i++)
                {
                    //access VD_MCU_WDATA and VD_MCU_WDATA_CTRL must use BYTE access
                    _MHal_W1B(VD_MCU_SRAM_WD, Demod_Flow_register_Tuner_Sanyo[i]);
                }

                break;

            default:
                MDrv_SYS_VDmcuSetType(VDMCU_DSP_UNKNOWN);
                printk("ATSC DSP code loaded fail !!! not defined tuner type \n");// update 0402 for ATSC
                return FALSE;
                break;
        }
    }

#if DEMOD_VERIFY_DSP
    /* reset sram address to 0 */
    _MHal_W1B(VD_MCU_ADDR_L, 0x00);
    _MHal_W1B(VD_MCU_ADDR_H, 0x00);

    for (i=0; i<u32Demod_DSP_len; i++)
    {
        if (pu8Demod_DSP[i] != _MHal_R1B(VD_MCU_SRAM_RD))
        {
            printk("DSP code loaded fail (error at byte %u)\n", i);
            bRet = FALSE;
            break;
        }
    }

    if (i == u32Demod_DSP_len)
        DEMOD_HAL_DBG("DSP code loaded successfully\n");
#endif

    if (eStandard == DEMOD_STANDARD_ISDBT)
    {
        /* reset sram address to 0 */
        _MHal_W1B(VD_MCU_ADDR_L, 0x80);
        _MHal_W1B(VD_MCU_ADDR_H, 0x6B); // VT modify according toFW

        _MHal_W1B(VD_MCU_ADDR_L,  0x00);        // sram address low byte
        _MHal_W1B(VD_MCU_ADDR_H,  0x04);        // sram address high byte
        u8Data = _MHal_R1B(VD_MCU_SRAM_RD);
        u16AddressOffset = u8Data << 8;
        u8Data = _MHal_R1B(VD_MCU_SRAM_RD);
        u16AddressOffset |= u8Data;

        _MHal_W1B(VD_MCU_ADDR_L,  (u16AddressOffset & 0xFF));        // sram address low byte
        _MHal_W1B(VD_MCU_ADDR_H,  (u16AddressOffset >>8));        // sram address high byte
        u8TmpData = (U8)stParam.sIsdbtInitialParamTable.u16DMD_ISDBT_TUNER_IF_KHZ;
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u16DMD_ISDBT_TUNER_IF_KHZ >> 8);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u8DMD_ISDBT_TUNER_IS_IQ_SWAP);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u16DMD_ISDBT_AGC_REF_VALUE);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u16DMD_ISDBT_AGC_REF_VALUE >> 8);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u32DMD_ISDBT_TDI_START_ADDR);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u32DMD_ISDBT_TDI_START_ADDR >> 8);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u32DMD_ISDBT_TDI_START_ADDR >> 16);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sIsdbtInitialParamTable.u32DMD_ISDBT_TDI_START_ADDR >> 24);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
    }

    if (eStandard == DEMOD_STANDARD_DTMB)
    {
        /* reset sram address to 0 */
        _MHal_W1B(VD_MCU_ADDR_L, 0x80);
        _MHal_W1B(VD_MCU_ADDR_H, 0x6B); // VT modify according toFW

        _MHal_W1B(VD_MCU_ADDR_L,  0x00);        // sram address low byte
        _MHal_W1B(VD_MCU_ADDR_H,  0x04);        // sram address high byte
        u8Data = _MHal_R1B(VD_MCU_SRAM_RD);
        u16AddressOffset = u8Data << 8;
        u8Data = _MHal_R1B(VD_MCU_SRAM_RD);
        u16AddressOffset |= u8Data;

        _MHal_W1B(VD_MCU_ADDR_L,  (u16AddressOffset & 0xFF));        // sram address low byte
        _MHal_W1B(VD_MCU_ADDR_H,  (u16AddressOffset >>8));        // sram address high byte
        u8TmpData = (U8)stParam.sDtmbInitialParamTable.u16DMD_DTMB_TUNER_IF_KHZ;
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u16DMD_DTMB_TUNER_IF_KHZ >> 8);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u8DMD_DTMB_TUNER_IS_IQ_SWAP);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u16DMD_DTMB_AGC_REF_VALUE);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u16DMD_DTMB_AGC_REF_VALUE >> 8);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u32DMD_DTMB_TDI_START_ADDR);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u32DMD_DTMB_TDI_START_ADDR >> 8);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u32DMD_DTMB_TDI_START_ADDR >> 16);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
        u8TmpData = (U8)(stParam.sDtmbInitialParamTable.u32DMD_DTMB_TDI_START_ADDR >> 24);
        _MHal_W1B(VD_MCU_SRAM_WD, u8TmpData); // write data to VD MCU 51 code sram
    }

    if (eStandard == DEMOD_STANDARD_DVBT2)
    {
        u32Demod_DSP_len = sizeof(_dsp_dvbt2) - DVBT2_PSRAM_LENGTH;
        if ((stParam.sDVBT2InitData.u32FwStartAddr != u32T2FWStartPA) || (pu8T2FWStartVA == NULL))
        {
            u32T2FWStartPA = stParam.sDVBT2InitData.u32FwStartAddr;
#if defined(CONFIG_MIPS)
            if (stParam.sDVBT2InitData.u32FwStartAddr & MS_MIU_INTERVAL)
                BUS_BASE = MIPS_MIU1_BUS_BASE;
            else
                BUS_BASE = MIPS_MIU0_BUS_BASE;
#elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
            if (stParam.sDVBT2InitData.u32FwStartAddr & MS_MIU_INTERVAL)
                BUS_BASE = ARM_MIU1_BUS_BASE;
            else
                BUS_BASE = ARM_MIU0_BUS_BASE;
#endif
            pu8T2FWStartVA = ioremap((u32T2FWStartPA + BUS_BASE), u32Demod_DSP_len);
        }
        pu8Demod_DSP = pu8T2FWStartVA;
//        printk("u32FwStartAddr=%x,pu8Demod_DSP=%x\n",stParam.sDVBT2InitData.u32FwStartAddr,(U32)pu8Demod_DSP);
        memcpy(pu8Demod_DSP, &_dsp_dvbt2[DVBT2_PSRAM_LENGTH], u32Demod_DSP_len);

//        printk("u32EqStartAddr=%x,u32TdiStartAddr=%x,u32DjbStartAddr=%x,u32FwStartAddr=%x\n",
//            stParam.sDVBT2InitData.u32EqStartAddr,
//            stParam.sDVBT2InitData.u32TdiStartAddr,
//            stParam.sDVBT2InitData.u32DjbStartAddr,
//            stParam.sDVBT2InitData.u32FwStartAddr);

        // add T2 DRAM bufer start address into fixed location
        _MHal_W1B(VD_MCU_ADDR_L, 0x30);
        _MHal_W1B(VD_MCU_ADDR_H, 0x00);

        //0x30~0x33
        stParam.sDVBT2InitData.u32EqStartAddr &= ~MS_MIU_INTERVAL;
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32EqStartAddr));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32EqStartAddr >> 8));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32EqStartAddr >> 16));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32EqStartAddr >> 24));

        //0x34~0x37
        stParam.sDVBT2InitData.u32TdiStartAddr &= ~MS_MIU_INTERVAL;
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32TdiStartAddr));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32TdiStartAddr >> 8));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32TdiStartAddr >> 16));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32TdiStartAddr >> 24));

        //0x38~0x3b
        stParam.sDVBT2InitData.u32DjbStartAddr &= ~MS_MIU_INTERVAL;
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32DjbStartAddr));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32DjbStartAddr >> 8));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32DjbStartAddr >> 16));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32DjbStartAddr >> 24));

        //0x3c~0x3f
        stParam.sDVBT2InitData.u32FwStartAddr &= ~MS_MIU_INTERVAL;
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32FwStartAddr));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32FwStartAddr >> 8));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32FwStartAddr >> 16));
        _MHal_W1B(VD_MCU_SRAM_WD, (U8)(stParam.sDVBT2InitData.u32FwStartAddr >> 24));
    }

    /* disable address auto increment */
    _MHal_W1Bb(VD_MCU_ADDR_AUTO_INC, 0x00, _BIT0);

    /* disable down load code */
    _MHal_W1BM(VD_MCU_KEY, 0x00, 0xF0);

    /* reset VD_MCU , fixed unlock issue for different standards switch */
    _MHal_W1Bb(VD_MCU_RST, 0x01, _BIT0);

    /* enable sram */
    _MHal_W1Bb(VD_MCU_SRAM_EN, 0x01, _BIT0);

    /* release remmapping reset */
    _MHal_W1Bb(VD_MCU_RST, 0x00, _BIT1);
    /* release VD mcu */
    _MHal_W1Bb(VD_MCU_RST, 0x00, _BIT0);

    //mail box crash protection 2009-11-06
    if (bRet)
    {
        MDrv_SYS_VDmcuSetType((VDMCU_DSP_TYPE_t)eStandard);
    }
    else
    {
     MDrv_SYS_VDmcuSetType(VDMCU_DSP_UNKNOWN);
    }

    return bRet;
}

B16 _MHal_Demod_DVBT2_Stop (void) // m2r checked
{
    U16 u8WaitCnt=0;
    U8 RegVal = 0;

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    if (RegVal)
    {
        printk(">> MB Busy!\n");
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set pause mcu

    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0x5A)                                        // wait MB_CNTL set done
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
        if (u8WaitCnt++ >= 0x7FFF)
        {
            DEMOD_HAL_DBG(">> DVBT2 Exit Fail!\n");
            return FALSE;
        }
        udelay(10);
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
    return TRUE;
}

//mstar 0901 update
B16 _MHal_Demod_DVBT_Stop (void)
{
    U16     u8WaitCnt=0;
    U8        RegVal = 0;

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    if (RegVal)
    {
        DEMOD_HAL_DBG(">> MB Busy!\n");
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set pause mcu

    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0x5A)                                        // wait MB_CNTL set done
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
        if (u8WaitCnt++ >= 0xFF)
        {
            DEMOD_HAL_DBG(">> DVBT Exit Fail!\n");
            return FALSE;
        }
     udelay(10);
    }

    // MHal_Demod_WriteReg(0x103480, 0x01);                         // reset DMD_MCU
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

B16 _MHal_Demod_DVBC_Stop (void)
{
    U16     u8WaitCnt=0;
    U8      RegVal=0;

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    if (RegVal)
    {
        DEMOD_HAL_DBG(">> MB Busy!\n");
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set read mode

    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0x5A)                                        // wait MB_CNTL set done
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
        if (u8WaitCnt++ >= 0xFF)
        {
            DEMOD_HAL_DBG(">> DVBC Exit Fail!\n");
            return FALSE;
        }
     udelay(10);
    }

    // MHal_Demod_WriteReg(0x103480, 0x01);                         // reset DMD_MCU
    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);             // MB_CNTL clear

    return TRUE;
}

B16 _MHal_Demod_ATSC_Stop (void)
{
    U16      u8CheckCount = 0;
    U8      RegVal = 0;

    //Notify firmware before reset by mailbox
    MHal_Demod_WriteReg(0x11261C, 0x01);
    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(0x11261C, &RegVal);

    //printk("\n###Enter ATSC_Stop####\n");

    while ((RegVal&0x02) != 0x02)
    {
        MHal_Demod_ReadReg(0x11261C, &RegVal);
        if (u8CheckCount++ >= 0xFF)
        {
            DEMOD_HAL_DBG(">> ATSC Exit Fail!\n");
            //while(1)
            {
                printk("\n###Enter ATSC_Stop Fail = [%d]####\n", u8CheckCount);
            }
            return FALSE;
        }
        udelay(100);
    }
    printk("\n###Enter ATSC_Stop Succ = [%d]####\n", u8CheckCount);
    return TRUE;
}

B16 _MHal_Demod_ISDBT_Stop (void)
{
    U16      u8CheckCount = 0;
    U8      RegVal = 0;

    //Notify firmware before reset by mailbox
    MHal_Demod_WriteReg(0x11261C, 0x01);
    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(0x11261C, &RegVal);

    //printk("\n###Enter ATSC_Stop####\n");

    while ((RegVal&0x02) != 0x02)
    {
        MHal_Demod_ReadReg(0x11261C, &RegVal);
        if (u8CheckCount++ >= 0xFF)
        {
            DEMOD_HAL_DBG(">> ISDBT Exit Fail!\n");
            //while(1)
            {
                printk("\n###Enter ISDBT_Stop Fail = [%d]####\n", u8CheckCount);
            }
            return FALSE;
        }
        udelay(100);
    }
    printk("\n###Enter ISDBT_Stop Succ = [%d]####\n", u8CheckCount);
    return TRUE;
}

B16 _MHal_Demod_DTMB_Stop (void)
{
    U16    u8CheckCount = 0;
    U8      RegVal = 0;

    //Notify firmware before reset by mailbox
    MHal_Demod_WriteReg(0x11261C, 0x01);
    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(0x11261C, &RegVal);

    //printk("\n###Enter ATSC_Stop####\n");

    while ((RegVal&0x02) != 0x02)
    {
        MHal_Demod_ReadReg(0x11261C, &RegVal);
        if (u8CheckCount++ >= 0xFF)
        {
            DEMOD_HAL_DBG(">> DTMB Exit Fail!\n");
            //while(1)
            {
                printk("\n###Enter DTMB_Stop Fail = [%d]####\n", u8CheckCount);
            }
            return FALSE;
        }
        udelay(100);
    }
    printk("\n###Enter DTMB_Stop Succ = [%d]####\n", u8CheckCount);
    return TRUE;
}

B16 _MHal_Demod_DVBS_Stop (void)
{
    U16     u8WaitCnt=0;
    U8        RegVal = 0;

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    if (RegVal)
    {
        DEMOD_HAL_DBG(">> MB Busy!\n");
        return FALSE;
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set pause mcu

    MHal_Demod_WriteReg(0x103483, 0x02);                         // assert interrupt to VD MCU51
    MHal_Demod_WriteReg(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
    while(RegVal != 0x5A)                                        // wait MB_CNTL set done
    {
        MHal_Demod_ReadReg(MBRegBase + 0x00, &RegVal);
        if (u8WaitCnt++ >= 0xFFF)
        {
            DEMOD_HAL_DBG(">> DVBS Exit Fail!\n");
            return FALSE;
        }
     udelay(1);
    }

    MHal_Demod_WriteReg(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

B16 MHal_Demod_Stop(DEMOD_STANDARD_TYPE_t eStandard)
{
    B16 bRet = TRUE;

    //mail box crash protection 2009-11-06
    if ((VDMCU_DSP_TYPE_t)eStandard != MDrv_SYS_VDmcuGetType())
    {
        printk("\n\033[1;31m %s: !!! WARNING VDMCU DSP TYPE IS NOT MATCHED !!! (current type is %d)\033[0m\n\n", __FUNCTION__, MDrv_SYS_VDmcuGetType());
        return FALSE;
    }

    DEMOD_HAL_DBG("MHal_Demod_Stop !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    switch (eStandard)
    {
        case DEMOD_STANDARD_DVBT:
            bRet = _MHal_Demod_DVBT_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_DVBT\n");
            break;

        case DEMOD_STANDARD_DVBT2:
            bRet = _MHal_Demod_DVBT2_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_DVBT2\n");
            break;

        case DEMOD_STANDARD_DVBC:
            bRet = _MHal_Demod_DVBC_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_DVBC\n");
            break;

        case DEMOD_STANDARD_ATSC:
            bRet = _MHal_Demod_ATSC_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_ATSC\n");
            break;

        case DEMOD_STANDARD_ISDBT:
            bRet = _MHal_Demod_ISDBT_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_ISDBT\n");
            break;

        case DEMOD_STANDARD_DTMB:
            bRet = _MHal_Demod_DTMB_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_DTMB\n");
            break;

        case DEMOD_STANDARD_DVBS:
            bRet = _MHal_Demod_DVBS_Stop();
            DEMOD_HAL_DBG("mode !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DEMOD_STANDARD_DVBS\n");
            break;

        default:
            DEMOD_HAL_DBG("no such standard %d\n", eStandard);
            bRet = FALSE;
            break;
    }


    return bRet;
}

//---------------------STR-------------------------------------------------

#ifdef CONFIG_PM
static void demod_save_mode_setting_reg(void)
{

    U8 u8Data = 0;
    U16 i=0;

    U8 u8Data_HW_MODE_ENA_BIT = 0; //ATSC(BIT0), ISDBT(BIT3)
    U8 u8Data_ATSC_ENA_BIT = 0; //ATSC(0x08), J83B(0x04)

    MHal_Demod_WriteReg(0x101e39, 0x00);
    //hw bit
    MHal_Demod_ReadReg(0x112002,&u8Data);
    u8Data_HW_MODE_ENA_BIT = u8Data & 0x0f;
    //ATSC, J83B hw bit
    MHal_Demod_ReadReg(0x1120c0,&u8Data);
    u8Data_ATSC_ENA_BIT = u8Data & 0x0f;

    if((u8Data_HW_MODE_ENA_BIT & DMDTOP_DEMOD_ENA_L_ISDBT) == DMDTOP_DEMOD_ENA_L_ISDBT) //ISDBT: enable bit(BIT3)=1
        save_demod_mode_setting.u16DEMOD_ISDBT_ELAG = 1;
    else if((u8Data_HW_MODE_ENA_BIT & DMDTOP_DEMOD_ENA_L_ATSC) == DMDTOP_DEMOD_ENA_L_ATSC) //ATSC: enable bit(BIT0)=1
    {
        save_demod_mode_setting.u16DEMOD_ATSC_ELAG = 1;

        //ATSC,J83B
        if((u8Data_ATSC_ENA_BIT & DEMOD_HW_ATSC_ENABLE) == DEMOD_HW_ATSC_ENABLE) //ATSC_AIR
        {
            save_demod_mode_setting.u16DEMOD_ATSC_MODE = 1;
        }
        else if((u8Data_ATSC_ENA_BIT & DEMOD_HW_J83B_ENABLE) == DEMOD_HW_J83B_ENABLE) //ATSC_CABLE (J83B)
        {
            save_demod_mode_setting.u16DEMOD_J83B_MODE = 1;
        }
        else
        {
            printk("Not ATSC, Not J83B\n");
        }
    }
    else
        printk("////save_mode////: No ISDBT & No ATSC !!!!!\n");

    if ( save_demod_mode_setting.u16DEMOD_ATSC_ELAG)
    {   
        if(_MHal_Demod_ATSC_Stop())
            printk("DEMOD_STANDARD_ATSC@@\n");
    }
    else if(save_demod_mode_setting.u16DEMOD_ISDBT_ELAG)
    {
         if(_MHal_Demod_ISDBT_Stop())
            printk("_MHal_Demod_ISDBT_Stop@@\n");
    }
    if ((save_demod_mode_setting.u16DEMOD_ISDBT_ELAG) || (save_demod_mode_setting.u16DEMOD_ATSC_ELAG))
    {

        //read the fw.dat
        MHal_Demod_WriteReg(0x103480 + 0x00,  0x01);        // reset VD_MCU
        MHal_Demod_WriteReg(0x103480 + 0x01,  0x00);        // disable SRAM
        MHal_Demod_WriteReg(0x103480 + 0x03,  0x50);        // enable "vdmcu51_if"
        MHal_Demod_WriteReg(0x103480 + 0x03,  0x51);        // enable auto-increase
        MHal_Demod_WriteReg(0x103480 + 0x04,  0x00);        // sram address low byte
        MHal_Demod_WriteReg(0x103480 + 0x05,  0x00);        // sram address high byte

        for ( i = 0; i < sizeof(fw_DEMOD); i++)
        {
           MHal_Demod_ReadReg(0x103480 + 0x10,&fw_DEMOD[i]);    // read sram data
        }

    }
    else
    {
        printk("Not ATSC, Not J83B ,Not ISDBT\n");
    }


    #if (DEBUG_MSG == 1)
        printk("u8Data_HW_MODE_ENA_BIT 0x2002 = 0x%x, u8Data_ATSC_ENA_BIT 0x20c1(LB) = 0x%x\n",
            (int)u8Data_HW_MODE_ENA_BIT,
            (int)u8Data_ATSC_ENA_BIT);
    #endif
    MHal_Demod_WriteReg(0x101e39, 0x03);
    

}
static void demod_clk_init(void)
{

    if (save_demod_mode_setting.u16DEMOD_ISDBT_ELAG)
    {
        U8 u8Val = 0;

        printk("STR ISDBT M2R Mustang InitialDemodSetClock start ================@.@\n");
        MHal_Demod_ReadReg(0x101e39, &u8Val);
        MHal_Demod_WriteReg(0x101e39, u8Val&~0x03);
        MHal_Demod_WriteReg(0x1128d0, 0x01);
        MHal_Demod_WriteReg(0x10331f, 0x00);
        MHal_Demod_WriteReg(0x10331e, 0x10);
        MHal_Demod_WriteReg(0x103301, 0x06); //ts clock = 7.2M 
        MHal_Demod_WriteReg(0x103300, 0x0b);
        MHal_Demod_WriteReg(0x103309, 0x00);
        MHal_Demod_WriteReg(0x103308, 0x00);
        MHal_Demod_WriteReg(0x103302, 0x01);
        MHal_Demod_WriteReg(0x103302, 0x00);
        MHal_Demod_WriteReg(0x111f0b, 0x00);
        MHal_Demod_WriteReg(0x111f0a, 0x00);    
        MHal_Demod_WriteReg(0x103315, 0x00);
        MHal_Demod_WriteReg(0x103314, 0x00);
        MHal_Demod_WriteReg(0x1128d0, 0x00);
        MHal_Demod_WriteReg(0x111f13, 0x00);
        MHal_Demod_WriteReg(0x111f12, 0x00); 
        MHal_Demod_WriteReg(0x111f25, 0x04); 
        MHal_Demod_WriteReg(0x111f31, 0x00); 
        MHal_Demod_WriteReg(0x111f30, 0x01);   
        MHal_Demod_WriteReg(0x111f3b, 0x00); 
        MHal_Demod_WriteReg(0x111f3a, 0x00);      
        MHal_Demod_WriteReg(0x111f43, 0x80);
        MHal_Demod_WriteReg(0x111f42, 0x08);
        MHal_Demod_WriteReg(0x111f45, 0x04);
        MHal_Demod_WriteReg(0x111f44, 0x44);
        MHal_Demod_WriteReg(0x111f63, 0x00); 
        MHal_Demod_WriteReg(0x111f62, 0x00);  
        MHal_Demod_WriteReg(0x111f65, 0x44);
        MHal_Demod_WriteReg(0x111f64, 0x44);
        MHal_Demod_WriteReg(0x111f66, 0x01);
        MHal_Demod_WriteReg(0x111f69, 0x00);
        MHal_Demod_WriteReg(0x111f68, 0x00);
        MHal_Demod_WriteReg(0x111f6b, 0x00);
        MHal_Demod_WriteReg(0x111f6a, 0x00);
        MHal_Demod_WriteReg(0x111f6d, 0x00);
        MHal_Demod_WriteReg(0x111f6c, 0x40);
        MHal_Demod_WriteReg(0x111f6f, 0x01);
        MHal_Demod_WriteReg(0x111f6e, 0x00);
        MHal_Demod_WriteReg(0x111f71, 0x44);  
        MHal_Demod_WriteReg(0x111f70, 0x44);
        MHal_Demod_WriteReg(0x111f73, 0x00);  
        MHal_Demod_WriteReg(0x111f72, 0x04);
        MHal_Demod_WriteReg(0x111f75, 0x44);  
        MHal_Demod_WriteReg(0x111f74, 0x44);      
        MHal_Demod_WriteReg(0x111f77, 0x44);
        MHal_Demod_WriteReg(0x111f76, 0x44);   
        MHal_Demod_WriteReg(0x111f79, 0x44);
        MHal_Demod_WriteReg(0x111f78, 0x44);
        MHal_Demod_WriteReg(0x111f7b, 0x44);
        MHal_Demod_WriteReg(0x111f7a, 0x44);  
        MHal_Demod_WriteReg(0x111f7d, 0x44);
        MHal_Demod_WriteReg(0x111f7c, 0x44);
        MHal_Demod_WriteReg(0x111f7f, 0x44);
        MHal_Demod_WriteReg(0x111f7e, 0x44);
        MHal_Demod_WriteReg(0x111fe1, 0x01);
        MHal_Demod_WriteReg(0x111fe0, 0x08);
        MHal_Demod_WriteReg(0x111ff0, 0x08);
        MHal_Demod_WriteReg(0x111fe3, 0x08);
        MHal_Demod_WriteReg(0x111fe2, 0x10);
        MHal_Demod_WriteReg(0x111feb, 0x11);
        MHal_Demod_WriteReg(0x111fea, 0x00);
        MHal_Demod_WriteReg(0x111fef, 0x00);
        MHal_Demod_WriteReg(0x111fee, 0x88);
        MHal_Demod_WriteReg(0x15298f, 0x44);
        MHal_Demod_WriteReg(0x15298e, 0x44);
        MHal_Demod_WriteReg(0x152991, 0x44);
        MHal_Demod_WriteReg(0x152990, 0x44);
        MHal_Demod_WriteReg(0x152992, 0x04);
        MHal_Demod_WriteReg(0x1529e5, 0x00);
        MHal_Demod_WriteReg(0x1529e4, 0x04);
        MHal_Demod_WriteReg(0x152971, 0x10);
        MHal_Demod_WriteReg(0x152970, 0x01);
        MHal_Demod_ReadReg(0x101e39, &u8Val);
        MHal_Demod_WriteReg(0x101e39,(u8Val| 0x03));
        printk("enter u16DEMOD_ISDBT_ELAG!!\n");

    }
    else if (save_demod_mode_setting.u16DEMOD_ATSC_ELAG)
    {
        U8 u8Val = 0;
        printk("STR ATSC M2 MUSTANG InitialDemodSetClock start ================@.@\n");
        MHal_Demod_ReadReg(0x101e39, &u8Val);
        MHal_Demod_WriteReg(0x101e39, u8Val&~0x03);
        MHal_Demod_WriteReg(0x10331e, 0x10);
        MHal_Demod_WriteReg(0x103301, 0x05);
        MHal_Demod_WriteReg(0x103300, 0x11);
        MHal_Demod_WriteReg(0x103309, 0x00);
        MHal_Demod_WriteReg(0x103308, 0x00);
        MHal_Demod_WriteReg(0x103315, 0x00);
        MHal_Demod_WriteReg(0x103314,0x08);
        MHal_Demod_WriteReg(0x103302,0x01);
        MHal_Demod_WriteReg(0x103302,0x00);
        MHal_Demod_WriteReg(0x152928,0x00);
        MHal_Demod_WriteReg(0x152903,0x00);
        MHal_Demod_WriteReg(0x152902,0x00);
        MHal_Demod_WriteReg(0x152905,0x00);
        MHal_Demod_WriteReg(0x152904,0x00);
        MHal_Demod_WriteReg(0x152907,0x00);
        MHal_Demod_WriteReg(0x152906,0x00);
        MHal_Demod_WriteReg(0x111f0b,0x00);
        MHal_Demod_WriteReg(0x111f0a,0x08);
        MHal_Demod_WriteReg(0x111f21,0x44);
        MHal_Demod_WriteReg(0x111f20,0x40);
        MHal_Demod_WriteReg(0x111f23,0x10);
        MHal_Demod_WriteReg(0x111f22,0x44);
        MHal_Demod_WriteReg(0x111f3b,0x08);
        MHal_Demod_WriteReg(0x111f3a,0x08);
        MHal_Demod_WriteReg(0x111f71,0x00);
        MHal_Demod_WriteReg(0x111f70,0x00);
        MHal_Demod_WriteReg(0x111f73,0x00);
        MHal_Demod_WriteReg(0x111f72,0x00);
        MHal_Demod_WriteReg(0x111f79,0x11);
        MHal_Demod_WriteReg(0x111f78,0x18);
        MHal_Demod_WriteReg(0x152991,0x88);
        MHal_Demod_WriteReg(0x152990,0x88);
        MHal_Demod_WriteReg(0x111f69,0x44);
        MHal_Demod_WriteReg(0x111f68,0x00);
        MHal_Demod_WriteReg(0x111f75,0x81);
        MHal_Demod_WriteReg(0x111f74,0x11);
        MHal_Demod_WriteReg(0x111f77,0x81);
        MHal_Demod_WriteReg(0x111f76,0x88);
        MHal_Demod_WriteReg(0x15298f,0x11);
        MHal_Demod_WriteReg(0x15298e,0x88);
        MHal_Demod_WriteReg(0x152923,0x00);
        MHal_Demod_WriteReg(0x152922,0x00);
        MHal_Demod_WriteReg(0x111f25,0x10);
        MHal_Demod_WriteReg(0x111f24,0x11);
        MHal_Demod_WriteReg(0x152971,0x1c);
        MHal_Demod_WriteReg(0x152970,0xc1);
        MHal_Demod_WriteReg(0x152977,0x04);
        MHal_Demod_WriteReg(0x152976,0x04);
        MHal_Demod_WriteReg(0x111f6f,0x11);
        MHal_Demod_WriteReg(0x111f6e,0x00);
        MHal_Demod_WriteReg(0x111feb,0x18);
        MHal_Demod_WriteReg(0x111f31,0x14);
        MHal_Demod_WriteReg(0x152981,0x00);
        MHal_Demod_WriteReg(0x152980,0x00);
        MHal_Demod_WriteReg(0x152983,0x00);
        MHal_Demod_WriteReg(0x152982,0x00);
        MHal_Demod_WriteReg(0x152985,0x00);
        MHal_Demod_WriteReg(0x152984,0x00);
        MHal_Demod_WriteReg(0x152987,0x00);
        MHal_Demod_WriteReg(0x152986,0x00);
        MHal_Demod_WriteReg(0x152979,0x11);
        MHal_Demod_WriteReg(0x152978,0x14);
        MHal_Demod_WriteReg(0x15298d,0x81);
        MHal_Demod_WriteReg(0x15298c,0x44);
        MHal_Demod_ReadReg(0x101e39, &u8Val);
        MHal_Demod_WriteReg(0x101e39, u8Val|0x03);
    }
    else
    {
        printk("ERROR demod_clk_init!!!!\n");
    }


}
static BOOL demod_load_fw(void)
{

    if ((save_demod_mode_setting.u16DEMOD_ISDBT_ELAG) || (save_demod_mode_setting.u16DEMOD_ATSC_ELAG))
    {

        U16 i=0;
        printk("enter demod_load_fw!!\n");
        MHal_Demod_WriteReg(0x103480 + 0x00,  0x01);        // reset VD_MCU
        MHal_Demod_WriteReg(0x103480 + 0x01,  0x00);        // disable SRAM
        MHal_Demod_WriteReg(0x103480 + 0x03,  0x50);        // enable "vdmcu51_if"
        MHal_Demod_WriteReg(0x103480 + 0x03,  0x51);        // enable auto-increase
        MHal_Demod_WriteReg(0x103480 + 0x04,  0x00);        // sram address low byte
        MHal_Demod_WriteReg(0x103480 + 0x05,  0x00);        // sram address high byte
        #if (DEBUG_MSG == 1)
            printk(">Load Code...\n");
        #endif
        for ( i = 0; i < sizeof(fw_DEMOD); i++)
        {
            MHal_Demod_WriteReg(0x103480 + 0x0C, fw_DEMOD[i]); // write data to VD MCU 51 code sram
        }


        MHal_Demod_WriteReg(0x103480 + 0x03, 0x50);     // diable auto-increase
        MHal_Demod_WriteReg(0x103480 + 0x03, 0x00);     // disable "vdmcu51_if"
        MHal_Demod_WriteReg(0x103480 + 0x01, 0x01);     // enable SRAM
        MHal_Demod_WriteReg(0x103480 + 0x00, 0x00);     // release VD_MCU

        printk(">DSP Loadcode done.\n");
        mdelay(30); //mdelay(20);

        return TRUE;

    }
    else
    {

        printk("ERROR Standard case!!!\n");
        return FALSE;

    }
}
static void demod_load_mode_setting_reg(void)
{
    U8 u8Val = 0;
    MHal_Demod_ReadReg(0x101e39, &u8Val);
    MHal_Demod_WriteReg(0x101e39, u8Val&~0x03);
    if (save_demod_mode_setting.u16DEMOD_ISDBT_ELAG)
    {
        MHal_Demod_WriteReg(0x112000 + 0xC0, 0x04);
        //MHal_Demod_ReadReg(0x20C0,0x04); //yd,dbg
    }
    else if (save_demod_mode_setting.u16DEMOD_ATSC_ELAG)
    {

        if (save_demod_mode_setting.u16DEMOD_ATSC_MODE)
        {
            MHal_Demod_WriteReg(0x112000 + 0xC0, 0x08);
            printk("ATSC ACTIVE!!!\n");
            //MAdp_Demod_MB_WriteReg_ATSC(0x20C0,0x08); //yd,dbg
        }
        else if(save_demod_mode_setting.u16DEMOD_J83B_MODE)
        {
            MHal_Demod_WriteReg(0x112000 + 0xC0, 0x04);
            //MAdp_Demod_MB_WriteReg_ATSC(0x20C0,0x04); //yd,dbg
        }
        else
        {
            printk("////load mode////: Not ATSC, Not J83B\n");
        }

    }
    else
    {
        printk("ERROR Standard in demod_load_mode_setting_reg!!!!\n");
    }
    MHal_Demod_ReadReg(0x101e39, &u8Val);
    MHal_Demod_WriteReg(0x101e39, u8Val|0x03);

}

extern int suspend_demod(void)
{ 
    U8 RegVal = 0;
    MHal_Demod_ReadReg(0x101e39, &RegVal);
    printk("kavana DTV suspend_demod!!!!\n");
    if(RegVal&0x03)//DTV
       demod_save_mode_setting_reg();
    else
       printk("kavana Not DTV mode!!!!\n"); 
    return 0;
}
extern int resume_demod(void)
{
    U8 RegVal = 0;
    MHal_Demod_ReadReg(0x101e39, &RegVal);
    printk("kavana DTV resume_demod!!!!\n");
    if(RegVal&0x03)//DTV
    {
       demod_clk_init();
       if(demod_load_fw())
        demod_load_mode_setting_reg();
       else
        printk("kavana demod_load_fw fail!!!!\n");
    }
    else
       printk("kavana Not DTV mode!!!!\n"); 
   return 0;
   
}
EXPORT_SYMBOL(suspend_demod);
EXPORT_SYMBOL(resume_demod);
#endif
//----------------------------------------------------------------------

