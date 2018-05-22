#ifndef _LED_PQ_SEC_INX65UNCH_DIRECT_C_
#define _LED_PQ_SEC_INX65UNCH_DIRECT_C_

#include "mhal_ld.h"

extern MS_U8 Tbl_LD_Edge2D_INX65inch_Direct48[];
extern MS_U8 Tbl_LD_Compensation_Gamma22[];

StuDrvLdPQInfo s_DrvLdPQInfo_SEC65 =
{
    HAL_LD_EDGE_TB_TYPE, // EnuLD_LED_TYPE eLEDType;

    // PANEL
    6, 2, // U8 u8LDFWidth, u8LDFHeight; // Statistic width & height
    6, 2, // U8 u8LEDWidth, u8LEDHeight; // LED width & height
    32, 18, // U8 u8LSFWidth, u8LSFHeight; // LSF width & height
    TRUE, // BOOL bCompEn; // Compensation enable
    Tbl_LD_Compensation_Gamma22, // const U8* pu8CompTable; // Compensation table  SIZE=256x2
    TRUE, // BOOL bEdge2DEn; // Edge2D enable
    Tbl_LD_Edge2D_INX65inch_Direct48, // const U8* pu8Edge2DTable; // Edge2D table  SIZE=(u8LEDWidth*u8LEDHeight)*(u8LDFWidth*u8LDFHeight)

    // HW
    0x80000, 0x80000, 0x80000, //U32 u32LsfInitialPhase_HL, u32LsfInitialPhase_HR, u32LsfInitialPhase_V;
    0x8, //U8 u8DCMaxAlpha; // statistics : higher -> prefer max, lower -> prefer average
    0x10, //U8 u8CompensationBlend; // pixel compensation tuner : higher -> prefer compensated pixel, lower -> prefer original pixel

    // SW
    0xFF, //U8 u8GDStrength; // global dimming strength
    0xFF, //U8 u8LDStrength; // local dimming strength
    0x80, //U8 u8TFStrengthUp; // temporal filter strength up
    0xF8, //U8 u8TFStrengthDn; // temporal filter strength down
    0x28, //U8 u8TFLowThreshold; // temporal filter low threshold
    0x38, //U8 u8TFHightThreshold; // temporal filter high threshold
    0xFF, //U8 u8SFStrength; // spatial filter strength
    0xFF, //U8 u8SFStrength2; // spatial filter strength 2
    0xFF, //U8 u8SFStrength3; // spatial filter strength 3
    0x0,  //U8 u8InGamma; // gamma at SW begin
    0x0,  //U8 u8OutGamma; // gamma at SW end
    0x04, //U8 u8BLMinValue; // min clamp value
    0x30, //U8 u8NRStrength; // de-noise strength
};

#endif
