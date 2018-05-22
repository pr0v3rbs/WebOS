#ifndef _LED_Device_INX_65INCH_DIRECT_48_H_
#define _LED_Device_INX_65INCH_DIRECT_48_H_


#ifdef _LED_Device_INX_65INCH_DIRECT_48_C_
    #define _LED_Device_INX_65INCH_DIRECT_DEC_
#else
    #define _LED_Device_INX_65INCH_DIRECT_DEC_  extern
#endif

#include "MsCommon.h"
//#include "datatype.h"
//#include "leddevice.h"

#include "drvPWM.h"

#define LED_Device_INX_65INCH_DIRECT_48 			0
#define LED_DEVICE_HIS_55INCH_DIRECT_144 			1
#define LED_DEVICE_SEL  LED_Device_INX_65INCH_DIRECT_48

// div 2
//duty 80.88
//shift 290.86

#if 1

    #define LD_SEC65_VSYNC_PWM_CH_SEL       PWM_CH5
    #define LD_SEC65_CS_PWM_CH_SEL            PWM_CH2

/*
#if(LD_ENABLE) //85inch 120Hz
    #define LD_VSYNC_PWM_CH_SEL       PWM_CH5
    #define LD_VSYNC_PWM_DUTYx100   (0x01 * 10)
    #define LD_VSYNC_PWM_SHIFTx100        0
    #define LD_VSYNC_PWM_DIV                    12
    #define LD_VSYNC_PWM_POLARITY           0

    #define LD_3D_VSYNC_DIV2_ENABLE     1

    #define LD_CS_PWM_CH_SEL       PWM_CH0
    #define LD_CS_PWM_DUTYx100   (0x7D * 10)
    #define LD_CS_PWM_SHIFTx100        6
    #define LD_CS_PWM_DIV                   12
    #define LD_CS_PWM_POLARITY          0
#endif
*/


//========  2D Timing  ========
    #define LD_SEC65_PERIOD_2D_SCLK_TO_VSYNC      (0x460)
//2D 50Hz
    #define LD_SEC65_VSYNC_2D50Hz_PWM_DUTYx100    (32)//(8)//(7)
    #define LD_SEC65_VSYNC_2D50Hz_PWM_SHIFTx100   (0)
    #define LD_SEC65_VSYNC_2D50Hz_PWM_DIV         (12)
    #define LD_SEC65_VSYNC_2D50Hz_PWM_POLARITY    (1)

    #define LD_SEC65_CS_2D50Hz_PWM_DUTYx100       (0x3F1)//(0x3F2)
    #define LD_SEC65_CS_2D50Hz_PWM_SHIFTx100      (0x2c)//(0x2b)
    #define LD_SEC65_CS_2D50Hz_PWM_DIV            (12)
    #define LD_SEC65_CS_2D50Hz_PWM_POLARITY       (0)

//2D 60Hz
    #define LD_SEC65_VSYNC_2D60Hz_PWM_DUTYx100    (36)//(9)//(8)
    #define LD_SEC65_VSYNC_2D60Hz_PWM_SHIFTx100   (0)
    #define LD_SEC65_VSYNC_2D60Hz_PWM_DIV         (12)
    #define LD_SEC65_VSYNC_2D60Hz_PWM_POLARITY    (1)

    #define LD_SEC65_CS_2D60Hz_PWM_DUTYx100       (0x4ff)//(0x4be)//(0x4bb)
    #define LD_SEC65_CS_2D60Hz_PWM_SHIFTx100      (0x34)
    #define LD_SEC65_CS_2D60Hz_PWM_DIV            (12)
    #define LD_SEC65_CS_2D60Hz_PWM_POLARITY       (0)

//========  3D Timing  ========
    #define LD_SEC65_PERIOD_3D_SCLK_TO_VSYNC      (0x460)

    #define LD_SEC65_VSYNC_3D_PWM_DUTYx100        (36)
    #define LD_SEC65_VSYNC_3D_PWM_SHIFTx100       (0)
    #define LD_SEC65_VSYNC_3D_PWM_DIV             (12)
    #define LD_SEC65_VSYNC_3D_PWM_POLARITY        (1)

    #define LD_SEC65_CS_3D_PWM_DUTYx100           (0x4BB)
    #define LD_SEC65_CS_3D_PWM_SHIFTx100          (0x32)
    #define LD_SEC65_CS_3D_PWM_DIV                (12)
    #define LD_SEC65_CS_3D_PWM_POLARITY           (0)

#endif

    
typedef struct
{
    MS_U8     PWM_CH;
    MS_U16    CurSysOutVFreqX10;
    MS_U16    SyncRstCnt;
}t_LD_Signal;

extern  t_sDrvLdCusPtrMap sCusPtrMap_LED_SEC65;

_LED_Device_INX_65INCH_DIRECT_DEC_ void _LED_SEC65_Set_OutputVFreq(MS_U16 u16OutVFreqX10, MS_BOOL Is3D);
_LED_Device_INX_65INCH_DIRECT_DEC_ void _LED_SEC65_Set_OutputCS(MS_U16 u16OutVFreqX10, MS_BOOL Is3D);
_LED_Device_INX_65INCH_DIRECT_DEC_ void _LED_SEC65_Set_SpiClk_Delay_FromVsync(MS_U16 u16OutVFreqX10, MS_BOOL Is3D);
_LED_Device_INX_65INCH_DIRECT_DEC_ void  _LED_SEC65_Set_OutputPWMData(MS_U8 ch, t_LD_Signal OutputSignal);

_LED_Device_INX_65INCH_DIRECT_DEC_ MS_U16  LED_SEC65_Get_OutputVFreq(MS_U8 ch);
_LED_Device_INX_65INCH_DIRECT_DEC_ MS_U16  LED_SEC65_Get_SyncRstCnt(MS_U8 ch);

//_LED_Device_INX_65INCH_DIRECT_DEC_ void LED_DEVICE_Config_OutputSignal_To_LED(MS_U16 u16OutVFreqX10, MS_BOOL Is3D);
_LED_Device_INX_65INCH_DIRECT_DEC_ void LED_SEC65_Config_OutputSignal_To_LED(MS_U16 u16OutVFreqX10, MS_BOOL Is3D);
_LED_Device_INX_65INCH_DIRECT_DEC_ void  LED_SEC65_Set_OutputSPI_Signal(void *pInstance, const MS_U16* SPI_blocks);


#endif // #ifndef _LED_Device_INX_65INCH_DIRECT_H_


