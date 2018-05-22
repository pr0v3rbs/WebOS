#define _MDRV_LDM_DMA_C_


#include "mdrv_ld.h"
#include "mdrv_ldm_dma.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "xc_hwreg_utility2.h"

#if 0 //SUPPORT_LD_SPI
#include "Board.h"
//#include "type_def.h"
#include "mdrv_mfc_spi.h"
//#include "mdrv_debug.h"
//#include "msos_nos.h"
//#include "common.h"
//#include "mhal_msio.h"
#include "drvGlobal.h"
#endif



#if( LD_ENABLE)
extern t_sDrvLdCusPtrMap sCusPtrMap;

void MDrv_LDM_DMA_SetMenuloadNumber( MS_U16 u16Number )
{
#if SUPPORT_LD_SPI
    MDrv_Write2ByteMask(LDM_DMA_REG(0x02), u16Number, 0x3FF);
    #if ENABLE_TWO_SPI
    MDrv_Write2ByteMask(LDM_DMA_REG2(0x02), u16Number, 0x3FF);
    #endif
#else
	u16Number = u16Number;
#endif
}


void MDrv_LDM_DMA_SetSPICommandFormat( MS_U8 u8CmdLen, MS_U16* pu16CmdBuf, MS_BOOL bEnCheckSum )
{
#if SUPPORT_LD_SPI
    MS_U8 i;
    MS_U16 u16Reg_Cmd_Len_Val = 0;
    MS_U8 u8Reg_chksum_md_Val = 0;


    if( u8CmdLen > LDM_MAX_SPI_CMD_NUM )
    {
        printf("[LDM-DMA]Spi command len is too long=%u!\n", u8CmdLen);
        u8CmdLen = LDM_MAX_SPI_CMD_NUM;
    }


    if( u8CmdLen == 0 )
        u8Reg_chksum_md_Val = 2; // data only
    else if( u8CmdLen == 1 )
        u8Reg_chksum_md_Val = 1; // command + data
    else //if( u8CmdLen >= 2 )
        u8Reg_chksum_md_Val = 0; // start + command + data

    //u8Reg_chksum_md_Val = 1; // command + data
    MDrv_WriteByteMask(LDM_DMA_REG(0x1A), u8Reg_chksum_md_Val, 0x7);//checksum mode
    
    #if ENABLE_TWO_SPI
    MDrv_WriteByteMask(LDM_DMA_REG2(0x1A), u8Reg_chksum_md_Val, 0x7);//checksum mode
    #endif

    for( i = 0; i < u8CmdLen; ++ i )
    {
        u16Reg_Cmd_Len_Val |= (1<<i);

         if( i == 0 )
        {
            MDrv_Write2Byte(LDM_DMA_REG(0x16), pu16CmdBuf[0]);
            #if ENABLE_TWO_SPI
            MDrv_Write2Byte(LDM_DMA_REG2(0x16), pu16CmdBuf[0]);
            #endif
        }    
        else if( i == 1 )
        {
            MDrv_Write2Byte(LDM_DMA_REG(0x18), pu16CmdBuf[1]);
            #if ENABLE_TWO_SPI
            MDrv_Write2Byte(LDM_DMA_REG2(0x18), pu16CmdBuf[1]);
            #endif
        }
        else
        {
            MDrv_Write2Byte(LDM_DMA_REG(0x42)+(i-2)*2, pu16CmdBuf[i]);
            #if ENABLE_TWO_SPI
            MDrv_Write2Byte(LDM_DMA_REG2(0x42)+(i-2)*2, pu16CmdBuf[i]);
            #endif
        }
    }

    if( bEnCheckSum )
    {
        u16Reg_Cmd_Len_Val |= BIT(15);
    }
    
    MDrv_Write2Byte(LDM_DMA_REG(0x40), u16Reg_Cmd_Len_Val);
    #if ENABLE_TWO_SPI
    MDrv_Write2Byte(LDM_DMA_REG2(0x40), u16Reg_Cmd_Len_Val);
    #endif
#else
	u8CmdLen = u8CmdLen;
	pu16CmdBuf = pu16CmdBuf;
	bEnCheckSum = bEnCheckSum;
#endif
}


/*
typedef enum
{
    E_LDM_SPI_TRIGER_STOP,
    E_LDM_SPI_TRIGER_ONE_SHOT,
    E_LDM_SPI_TRIGER_EVERY_V_SYNC,
}EnuSpiTriggerMode;
*/
void MDrv_LDM_DMA_SetSpiTriggerMode( EnuLdmSpiTriggerMode eSpiTriggerMode )
{
#if SUPPORT_LD_SPI
    switch(eSpiTriggerMode)
    {
        case E_LDM_SPI_TRIGER_STOP:
            // Menuload trigger src = one shot
            
            MDrv_WriteByteMask(LDM_DMA_REG(0x00), 0, 0x03);
            // menuload abort one shot
            MDrv_WriteRegBit(LDM_DMA_REG(0x0A), 1, 1);
            // menuload disable one shot
            MDrv_WriteRegBit(LDM_DMA_REG(0x0C), 0, 1);
            
            #if ENABLE_TWO_SPI
            // Menuload trigger src = one shot
            MDrv_WriteByteMask(LDM_DMA_REG2(0x00), 0, 0x03);
            // menuload abort one shot
            MDrv_WriteRegBit(LDM_DMA_REG2(0x0A), 1, 1);
            // menuload disable one shot
            MDrv_WriteRegBit(LDM_DMA_REG2(0x0C), 0, 1);
            #endif
            
            break;
            

        case E_LDM_SPI_TRIGER_ONE_SHOT:
            // Menuload trigger src = one shot
            MDrv_WriteByteMask(LDM_DMA_REG(0x00), 0, 0x03);
            // menuload enable one shot
            MDrv_WriteRegBit(LDM_DMA_REG(0x0C), 1, 1);
            // menuload realtime trigger one shot
            MDrv_WriteRegBit(LDM_DMA_REG(0x08), 1, 1);
            
            #if ENABLE_TWO_SPI
            // Menuload trigger src = one shot
            MDrv_WriteByteMask(LDM_DMA_REG2(0x00), 0, 0x03);
            // menuload enable one shot
            MDrv_WriteRegBit(LDM_DMA_REG2(0x0C), 1, 1);
            // menuload realtime trigger one shot
            MDrv_WriteRegBit(LDM_DMA_REG2(0x08), 1, 1);
            #endif
            
            break;

        case E_LDM_SPI_TRIGER_EVERY_V_SYNC:
            // Menuload trigger src = V sync
            MDrv_WriteByteMask(LDM_DMA_REG(0x00), 0x81, 0x83); //bit7 , hw mode enable
            
            #if ENABLE_TWO_SPI
            // Menuload trigger src = V sync
            MDrv_WriteByteMask(LDM_DMA_REG2(0x00), 0x81, 0x83); //bit7 , hw mode enable
            #endif
            
            break;
    }
#else
	eSpiTriggerMode = eSpiTriggerMode;
#endif
}

void MDrv_LDM_DMA_Init(void* pInstance)
{
#if SUPPORT_LD_SPI
#if 0
    MDrv_LDM_DMA_SetSpiTriggerMode(E_LDM_SPI_TRIGER_EVERY_V_SYNC);
    // Setup LDM-DMA ...
    MDrv_LDM_DMA_SetSPICommandFormat( psDrvLdInfo->u8SpiCmdLen, psDrvLdInfo->au16SpiCmd, psDrvLdInfo->u8CheckSumMode );
	#if(LED_DEVICE_SEL == LED_DEVICE_HIS_55INCH_DIRECT_144)
    MDrv_LDM_DMA_SetMenuloadNumber( psDrvLdInfo->u8DMAWidth*psDrvLdInfo->u8DMAHeight - 1);
	#else
    MDrv_LDM_DMA_SetMenuloadNumber( psDrvLdInfo->u8DMAWidth*psDrvLdInfo->u8DMAHeight);
	#endif
#else
	MDrv_Write2Byte(LDM_DMA_REG(0x2C), 0x200);
	MDrv_Write2Byte(LDM_DMA_REG(0x2E), 0x1);
	MDrv_Write2Byte(LDM_DMA_REG(0x40), 0x81);
	MDrv_Write2Byte(LDM_DMA_REG(0x41), 0x6);
	MDrv_Write2Byte(LDM_DMA_REG(0x44), 0x1);
	MDrv_Write2Byte(LDM_DMA_REG(0x4C), 0x81);
	MDrv_Write2Byte(LDM_DMA_REG(0x60), 0x8003);
	MDrv_Write2Byte(LDM_DMA_REG(0x6A), 0x100);
	MDrv_Write2Byte(LDM_DMA_REG(0x6C), 0x200);
	MDrv_Write2Byte(LDM_DMA_REG(0x6E), 0x1);
#endif
#endif    
}

void MDrv_LDM_DMA_SetSpiCmd(void* pInstance, MS_U8 u8Mask, MS_BOOL bEnable)
{
	MDrv_WriteByteMask(LDM_DMA_REG(0x4C), bEnable? u8Mask: 0, u8Mask);
}

#if  0
static BOOL s_bMDrv_LDM_DMA_InitFinished = FALSE;

BOOL MDrv_LDM_DMA_Init(void)
{
    if( s_bMDrv_LDM_DMA_InitFinished )
        return TRUE;

/*
    if( MDrv_SPI_Init(TRUE) == FALSE )
    {
        ADD_SYS_ERR_MSG(SYS_ERR_INIT_MSPI_FAILED, "Init drvMSPI failed!\n");
        return FALSE;
    }
*/
#if ( DRV_LD_USE_SWSPI )
    MDrv_LDM_DMA_SetSpiTriggerMode(E_LDM_SPI_TRIGER_STOP);
#else
    MDrv_LDM_DMA_SetSpiTriggerMode(E_LDM_SPI_TRIGER_EVERY_V_SYNC);
#endif

#if ( DRV_LD_USE_SWSPI )
{
    U16 i;
    U16 u16TotalSPIDataLen = 0;

    // Setup SPI cmd data...
    for( i = 0; i < s_DrvLdInfo.u8SpiCmdLen; ++ i )
    {
        s_au8DrvLD_SPIBuf[i] = s_DrvLdInfo.au16SpiCmd[i];
    }

    // Setup MSPI timing ...
    u16TotalSPIDataLen = s_DrvLdInfo.u8SpiCmdLen + LD_LED_NUMBER;
    if( s_DrvLdInfo.u8CheckSumMode )
        u16TotalSPIDataLen += 1;

    // Setup the delay time before every data
    //s_au8DrvLD_SPI_delay[0] = (s_DrvLdInfo.u8SpiTime_VSyncWidth + s_DrvLdInfo.u8SpiTime_VSync_To_FirstClock);
    s_au8DrvLD_SPI_delay[0] = s_DrvLdInfo.u8SpiTime_VSync_To_FirstClock;

    for( i = 1; i < u16TotalSPIDataLen; ++ i )
    {
        s_au8DrvLD_SPI_delay[i] = s_DrvLdInfo.u8SpiTime_Byte_to_Byte;
    }
    MDrv_SPI_SetWriteBuf_Delay( s_au8DrvLD_SPI_delay, u16TotalSPIDataLen );
}
#else
    // Setup MSPI timing
//    MHal_MSPI_SetTime_TriggerToFirstClock(s_DrvLdInfo.u8SpiTime_FromTriggerToFirstClock);
//    MHal_MSPI_SetTime_LastClockToDone(s_DrvLdInfo.u8SpiTime_LastClockToDone);

    // Setup LDM-DMA ...
    MDrv_LDM_DMA_SetSPICommandFormat( s_DrvLdInfo.u8SpiCmdLen, s_DrvLdInfo.au16SpiCmd, s_DrvLdInfo.u8CheckSumMode );
    MDrv_LDM_DMA_SetMenuloadNumber( s_DrvLdInfo.u8DMAWidth*s_DrvLdInfo.u8DMAHeight);        
#endif

    s_bMDrv_LDM_DMA_InitFinished = TRUE;

printf("\n -- ox %s()\n",__FUNCTION__);

    return s_bMDrv_LDM_DMA_InitFinished;
}
#endif


#endif

