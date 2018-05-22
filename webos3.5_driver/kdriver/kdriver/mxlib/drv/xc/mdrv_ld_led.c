#ifndef _LED_Device_INX_65INCH_DIRECT_48_C_
#define _LED_Device_INX_65INCH_DIRECT_48_C_

#include "mhal_ld.h"

#include "MsCommon.h"
#include "mdrv_ld_led.h"
#include "mdrv_ld.h"

#if 1 //SUPPORT_LD_SPI	
#include "mdrv_ld_pwm.h"
#include "mhal_ld_pwm.h"
#include "mdrv_ldm_dma.h"
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif


extern t_sDrvLdCusPtrMap sCusPtrMap;
extern StuDrvLdPQInfo s_DrvLdPQInfo_SEC65;

StuDrvLdInfo s_DrvLdInfo_SEC65 =
{
    LD_PQ_STAGE, // EnuLD_DEVELOPE_STAGE eStage;

    // PANEL
    3840, 2160, // MS_U16 u16PanelWidth, u16PanelHeight; // Panel width & height
    6, 1, //20, 12, // MS_U8 u8LEDNumW, u8LEDNumH; // LED
    6, 1, // MS_U8 u8DMAWidth, u8DMAHeight; // DMA width & height
    (LOCAL_DIMMING_ADR|0x40000000), // MS_U32 u32BaseAddr; // DRAM base address for LD

    // SPI (LD)
    FALSE, //MS_BOOL bBacklightGlobeMode; // DMA output data non-8bit enable
    0x10, //MS_U8 u8BacklightGlobeGain;  // DMA output data gain
    0, //MS_U8 u8BacklightGlobeAlign; // DMA output data 0:16b  1:12b  2:4b  3:10b

    // SPI (LDM/MSPI)
    MSPI_CLK_SPD_43M,     // 43/32 = 1.34375
    MSPI_CLK_SPD__DIV_32, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase
    1, // u8Time_FromTriggerToFirstClock;  // The time from "reg_mspi_trigger" to first SPI clock.     0 => Delay 1 MSPI clock
    1, // u8Time_LastClockToDone;    // The time from last SPI clock to "reg_mspi_done_flag"     0 => Delay 1 MSPI clock
    0, // u8Time_BetweenByteToByte;  // The time between byte to byte transfer       0 => Delay 0 MSPI clock
    0, // u8Time_FromWriteToRead;    // The time between last write and first read   0 => Delay 0 MSPI clock
    0, // u8SpiTime_VSyncWidth
    0, // u8SpiTime_VSync_To_FirstClock
    MSPI_PORT_NUM,
    2, // SPI Command length
    {0x5055,0x5000}, // SPI Command
    1, // u8CheckSumMode
    10, //LDM DMA transfer bit per one pack
};

t_sDrvLdCusPtrMap sCusPtrMap_LED_SEC65=
{
    &s_DrvLdInfo_SEC65,
    &s_DrvLdPQInfo_SEC65,
    LED_SEC65_Config_OutputSignal_To_LED,
    LED_SEC65_Get_OutputVFreq,
    LED_SEC65_Get_SyncRstCnt,
    LED_SEC65_Set_OutputSPI_Signal,
};

#if (LED_MULTI_SUPPORT_ENABLE == 0)
t_sDrvLdCusPtrMap *psLEDCusPtrMap = &sCusPtrMap_LED_SEC65;
#endif


t_LD_Signal ld_pwm_SEC65[(MS_U8)PWM_NUMS]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
static MS_U16 PHY_blocks[LD_MAX_BLOCK];


void _LED_SEC65_Set_OutputVFreq(MS_U16 u16OutVFreqX10, MS_BOOL Is3D)
{
#if SUPPORT_LD_SPI	
    t_LD_Signal OutputSignal;

    if(Is3D)
    {
            OutputSignal.CurSysOutVFreqX10 =u16OutVFreqX10;
            OutputSignal.SyncRstCnt  =0;

            MDrv_MFC_PWM_waveform_maker(LD_SEC65_VSYNC_PWM_CH_SEL, OutputSignal.CurSysOutVFreqX10, LD_SEC65_VSYNC_3D_PWM_DUTYx100,
                                        LD_SEC65_VSYNC_3D_PWM_SHIFTx100, LD_SEC65_VSYNC_3D_PWM_DIV, OutputSignal.SyncRstCnt,
                                        Vsync_DoubleBF, PWM_RST_CTRL__VSYNC, LD_SEC65_VSYNC_3D_PWM_POLARITY);

    }
    else
    {
            OutputSignal.CurSysOutVFreqX10 =u16OutVFreqX10;
            OutputSignal.SyncRstCnt  =0;

            if(u16OutVFreqX10 < 1100)// 100Hz , 2D 50Hz input
            {
                MDrv_MFC_PWM_waveform_maker(LD_SEC65_VSYNC_PWM_CH_SEL, OutputSignal.CurSysOutVFreqX10, LD_SEC65_VSYNC_2D50Hz_PWM_DUTYx100,
                                            LD_SEC65_VSYNC_2D50Hz_PWM_SHIFTx100, LD_SEC65_VSYNC_2D50Hz_PWM_DIV, OutputSignal.SyncRstCnt,
                                            Vsync_DoubleBF, PWM_RST_CTRL__VSYNC, LD_SEC65_VSYNC_2D50Hz_PWM_POLARITY);

            }
            else
            {
                MDrv_MFC_PWM_waveform_maker(LD_SEC65_VSYNC_PWM_CH_SEL, OutputSignal.CurSysOutVFreqX10, LD_SEC65_VSYNC_2D60Hz_PWM_DUTYx100,
                                            LD_SEC65_VSYNC_2D60Hz_PWM_SHIFTx100, LD_SEC65_VSYNC_2D60Hz_PWM_DIV, OutputSignal.SyncRstCnt,
                                            Vsync_DoubleBF, PWM_RST_CTRL__VSYNC, LD_SEC65_VSYNC_2D60Hz_PWM_POLARITY);

        }
    }

    MHal_PWM_WaveformResetEnable(LD_SEC65_VSYNC_PWM_CH_SEL, ENABLE);

    MHal_PWM_OutputEn(LD_SEC65_VSYNC_PWM_CH_SEL, ENABLE);

    MDrv_PWM_Set_PadEn( LD_SEC65_VSYNC_PWM_CH_SEL, ENABLE );

    OutputSignal.PWM_CH = (MS_U8) LD_SEC65_VSYNC_PWM_CH_SEL;
    _LED_SEC65_Set_OutputPWMData(OutputSignal.PWM_CH, OutputSignal);
#else		
	u16OutVFreqX10 = u16OutVFreqX10;
	Is3D = Is3D;
#endif		
}

void _LED_SEC65_Set_OutputCS(MS_U16 u16OutVFreqX10, MS_BOOL Is3D)
{
#if SUPPORT_LD_SPI	
    t_LD_Signal OutputSignal ;

    if(Is3D)
    {
        MDrv_MFC_PWM_waveform_maker(LD_SEC65_CS_PWM_CH_SEL, u16OutVFreqX10, LD_SEC65_CS_3D_PWM_DUTYx100,
                                    LD_SEC65_CS_3D_PWM_SHIFTx100, LD_SEC65_CS_3D_PWM_DIV, 0,
                                    Vsync_DoubleBF, PWM_RST_CTRL__VSYNC, LD_SEC65_CS_3D_PWM_POLARITY);
    }
    else
    {
        if(u16OutVFreqX10 < 1100)// 100Hz , 2D 50Hz input
        {
            MDrv_MFC_PWM_waveform_maker(LD_SEC65_CS_PWM_CH_SEL, u16OutVFreqX10, LD_SEC65_CS_2D50Hz_PWM_DUTYx100,
                                        LD_SEC65_CS_2D50Hz_PWM_SHIFTx100, LD_SEC65_CS_2D50Hz_PWM_DIV, 0,
                                        Vsync_DoubleBF, PWM_RST_CTRL__VSYNC, LD_SEC65_CS_2D50Hz_PWM_POLARITY);

        }
        else
        {
            MDrv_MFC_PWM_waveform_maker(LD_SEC65_CS_PWM_CH_SEL, u16OutVFreqX10, LD_SEC65_CS_2D60Hz_PWM_DUTYx100,
                                        LD_SEC65_CS_2D60Hz_PWM_SHIFTx100, LD_SEC65_CS_2D60Hz_PWM_DIV, 0,
                                        Vsync_DoubleBF, PWM_RST_CTRL__VSYNC, LD_SEC65_CS_2D60Hz_PWM_POLARITY);
        }

    }

    MHal_PWM_WaveformResetEnable(LD_SEC65_CS_PWM_CH_SEL, ENABLE);

    MHal_PWM_OutputEn(LD_SEC65_CS_PWM_CH_SEL, ENABLE);

    MDrv_PWM_Set_PadEn( LD_SEC65_CS_PWM_CH_SEL, ENABLE );


    OutputSignal.PWM_CH = (MS_U8) LD_SEC65_CS_PWM_CH_SEL;
    OutputSignal.CurSysOutVFreqX10 = u16OutVFreqX10;
    OutputSignal.SyncRstCnt = 0 ;
    _LED_SEC65_Set_OutputPWMData(OutputSignal.PWM_CH, OutputSignal);
#else		
	u16OutVFreqX10 = u16OutVFreqX10;
	Is3D = Is3D;
#endif		

}

void _LED_SEC65_Set_SpiClk_Delay_FromVsync(MS_U16 u16OutVFreqX10, MS_BOOL Is3D)
{
#if SUPPORT_LD_SPI	
    u16OutVFreqX10 = u16OutVFreqX10;
    if(Is3D)
    {
        MDrv_Write2Byte(LDM_DMA_REG(0x58), LD_SEC65_PERIOD_3D_SCLK_TO_VSYNC);
    }
    else
    {
        MDrv_Write2Byte(LDM_DMA_REG(0x58), LD_SEC65_PERIOD_2D_SCLK_TO_VSYNC );
    }
#else		
	u16OutVFreqX10 = u16OutVFreqX10;
	Is3D = Is3D;
#endif		
}

void  _LED_SEC65_Set_OutputPWMData(MS_U8 ch, t_LD_Signal OutputSignal)
{
#if SUPPORT_LD_SPI	
    if(ch >= (MS_U8)PWM_NUMS)
    {
        printf("LED  error : %s() ; ln%d\n",__FUNCTION__,__LINE__);
        return ;
    }

    ld_pwm_SEC65[ch].PWM_CH = OutputSignal.PWM_CH;
    ld_pwm_SEC65[ch].CurSysOutVFreqX10 = OutputSignal.CurSysOutVFreqX10;
    ld_pwm_SEC65[ch].SyncRstCnt = OutputSignal.SyncRstCnt;
#else		
	ch = ch;
	OutputSignal = OutputSignal;
#endif		

}


MS_U16  LED_SEC65_Get_OutputVFreq(MS_U8 ch)
{
	//printf("test-- %s() ; ln%d\n",__FUNCTION__,__LINE__);
    if(ch >= (MS_U8)PWM_NUMS )
    {
        //printf("LED  error : %s() ; ln%d\n",__FUNCTION__,__LINE__);
        return 0xFFFF;
    }

    return ld_pwm_SEC65[ch].CurSysOutVFreqX10;
}

MS_U16  LED_SEC65_Get_SyncRstCnt(MS_U8 ch)
{
	//printf("test-- %s() ; ln%d\n",__FUNCTION__,__LINE__);
    if(ch >= (MS_U8)PWM_NUMS )
    {
        //printf("LED  error : %s() ; ln%d\n",__FUNCTION__,__LINE__);
        return 0xFFFF;
    }

    return ld_pwm_SEC65[ch].SyncRstCnt ;
}


void LED_SEC65_Config_OutputSignal_To_LED(MS_U16 u16OutVFreqX10, MS_BOOL Is3D)
{
     //printf("test-- %s() ; ln%d\n",__FUNCTION__,__LINE__);
    _LED_SEC65_Set_OutputVFreq(u16OutVFreqX10,Is3D);
    _LED_SEC65_Set_OutputCS(u16OutVFreqX10,Is3D);
    _LED_SEC65_Set_SpiClk_Delay_FromVsync(u16OutVFreqX10,Is3D);
}

void  LED_SEC65_Set_OutputSPI_Signal(void *pInstance, const MS_U16* SPI_blocks)
{
#if SUPPORT_LD_SPI	
    /* TRANSLATE SPI_blocks TO PHY_blocks on your own!!           */
    /* SPI_blocks contains 16-bit data, ranging from 0 to 0xFFFF. */

    // 1 virtual LED = 2x2 physical LED
    const int vir_to_phy_LED_ratio_H = 1; // <---- CHECK HERE !!!!
    const int vir_to_phy_LED_ratio_V = 1; // <---- CHECK HERE !!!!

    memset(PHY_blocks, 0x00, LD_MAX_BLOCK*sizeof(MS_U16));

    MS_U16 led_width = MHal_LD_Get_LED_BacklightWidth(pInstance);
    MS_U16 led_height = MHal_LD_Get_LED_BacklightHeight(pInstance);
    MS_U16 SPI_idx = 0, PHY_idx = 0, i = 0, j = 0, DMA_idx = 0;
    //MS_U32 addr_base = MHal_LD_Get_SPI_BufBaseAddr(pInstance);

    // Re-arrange virtual LED blocks into physical LED blocks
    MS_U16 phy_width = led_width * vir_to_phy_LED_ratio_H; // <---- CHECK HERE !!!!
    MS_U16 phy_height = led_height * vir_to_phy_LED_ratio_V; // <---- CHECK HERE !!!!
    MS_U16 n_phys = phy_width * phy_height;
    for (PHY_idx = 0; PHY_idx < n_phys; PHY_idx++)
    {
        j = PHY_idx / phy_width;
        i = PHY_idx % phy_width;
        SPI_idx = (i/vir_to_phy_LED_ratio_H) + (j/vir_to_phy_LED_ratio_V)*led_width; // <---- CHECK HERE !!!!
        PHY_blocks[PHY_idx] = SPI_blocks[SPI_idx];
    }

    /* DO NOT MODIFY THE CODE BELOW!! */

    // Debug mode for physical LED blocks
    MS_U8 led_mod = MHal_LD_Get_SW_PulseMode(pInstance);
    MS_U16 led_id = MHal_LD_Get_SW_PulseId(pInstance);
    MS_U16 led_str = MHal_LD_Get_SW_PulseLEDIntensity(pInstance);
    MS_U16 ldb_str = MHal_LD_Get_SW_PulseLDBIntensity(pInstance);
    if (led_mod == 0x7)     // 22[15:12]=4'b0111  real backlight (physical LED)
    {
        if (led_id == 0xfff)
        {
            for (PHY_idx = 0; PHY_idx < n_phys; PHY_idx++)
                PHY_blocks[PHY_idx] = 0xffff;
        }
        else if (led_id < n_phys)
        {
            for (PHY_idx = 0; PHY_idx < n_phys; PHY_idx++)
                PHY_blocks[PHY_idx] = 0;
            PHY_blocks[led_id] = 0xffff;
        }
    }
    else if (led_mod == 0x5)      // 22[15:12]=4'b0101 tunable real backlight (physical LED)
    {
        if (led_id == 0xfff)
        {
            for (PHY_idx = 0; PHY_idx < n_phys; PHY_idx++)
                PHY_blocks[PHY_idx] = (led_str << 8) | ldb_str;
        }
        else if (led_id < n_phys)
        {
            for (PHY_idx = 0; PHY_idx < n_phys; PHY_idx++)
                PHY_blocks[PHY_idx] = 0;
            PHY_blocks[led_id] = (led_str << 8) | ldb_str;
        }
    }

    MS_U16 dma_width = MHal_LD_Get_DMA_BacklightWidth(pInstance);
    MS_U16 dma_height = MHal_LD_Get_DMA_BacklightHeight(pInstance);
    MS_U32 addr_base = MHal_LD_Get_SPI_BufBaseAddr(pInstance);
	MS_U16 u16AvgData = 0;
	MS_U16 u16AvgDataCnt = 0;

	for (i= 0; i<dma_height ;i++)
	{
		for (j= 0; j< dma_width ;j++)
		{
			if(!MHal_LD_IsEnableLedOutput(pInstance))
			{
				PHY_blocks[j+(8*i)] = 0xFFFF;
			}
			
			if(MHal_LD_GetSPIMirror(pInstance))
			{
				MDrv_LD_MIUWriteByte(pInstance, addr_base+(32*i), dma_width - j - 1, (MS_U8)(PHY_blocks[j+(8*i)]>>8));
			}
			else
			{
				MDrv_LD_MIUWriteByte(pInstance, addr_base+(32*i), j, (MS_U8)(PHY_blocks[j+(8*i)]>>8));
			}
			//MDrv_LD_MIUWrite2Bytes(pInstance, addr_base+j+(32*i), j, (PHY_blocks[j+(8*i)]>>8)|(PHY_blocks[j+(8*i)]<<8));
			u16AvgData += (PHY_blocks[j+(8*i)]>>8);
			u16AvgDataCnt++;
		}
	}

	if(u16AvgDataCnt != 0)
    {
		u16AvgData /= u16AvgDataCnt;
		MHal_LD_Set_SPI_AvgData(pInstance, (MS_U8)u16AvgData);
    }
#if 0
printf("============ SPI ===========\n");
for(i = 0; i < 6; i++)
{
	if(i % 16 == 0) printf("%.2X     ", i);
	printf("%.4X(%.4X), ", MDrv_LD_MIURead2Bytes(pInstance, addr_base, i << 1), PHY_blocks[i]);
	if(i % 16 == 15) printf("=\n");
    }
printf("=\n");
#endif		
#else
	SPI_blocks = SPI_blocks;
#endif
    UNUSED(DMA_idx);
}

#endif

