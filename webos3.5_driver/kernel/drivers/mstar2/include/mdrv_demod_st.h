
#ifndef _MDRV_DEMOD_ST_H_
#define _MDRV_DEMOD_ST_H_

#include "mdrv_types.h"

typedef struct DEMOD_REG_s
{
    U32 u32Address;
    U8  u8Value;
} DEMOD_REG_t, *pDEMOD_REG_t;
//mstar 0901 update
typedef enum DEMOD_STANDARD_TYPE_e
{
    DEMOD_STANDARD_UNKNOWN = 0,
    DEMOD_STANDARD_DVBT,
    DEMOD_STANDARD_DVBC,
    DEMOD_STANDARD_ATSC,
    DEMOD_STANDARD_VIF,
    DEMOD_STANDARD_ISDBT,
    DEMOD_STANDARD_DTMB,
    DEMOD_STANDARD_DVBT2,
    DEMOD_STANDARD_DVBS
} DEMOD_STANDARD_TYPE_t;

typedef enum TUNER_TYPE_e
{
    TUNER_TYPE_UNKNOWN = 0,
    TUNER_TYPE_INNOTEK,
    TUNER_TYPE_SANYO,
} TUNER_TYPE_t;

typedef struct _s_ISDBT_INITIAL_PARAMETERS
{
	  U16 u16DMD_ISDBT_TUNER_IF_KHZ;
	  U8  u8DMD_ISDBT_TUNER_IS_IQ_SWAP;
	  U16 u16DMD_ISDBT_AGC_REF_VALUE;
	  U32 u32DMD_ISDBT_TDI_START_ADDR;
}s_ISDBT_INITIAL_PARAMETERS;

typedef struct s_DTMB_INITIAL_PARAMETERS
{
	  U16 u16DMD_DTMB_TUNER_IF_KHZ;
	  U8  u8DMD_DTMB_TUNER_IS_IQ_SWAP;
	  U16 u16DMD_DTMB_AGC_REF_VALUE;
	  U32 u32DMD_DTMB_TDI_START_ADDR;
}s_DTMB_INITIAL_PARAMETERS;

typedef enum
{
    _T2_QPSK    = 0x0,
    _T2_16QAM   = 0x1,
    _T2_64QAM   = 0x2,
    _T2_256QAM    = 0x3,
    _T2_QAM_UNKNOWN     = 0xFF,
} DMD_T2_CONSTEL;

typedef enum
{
    _T2_CR1Y2   = 0x0,
    _T2_CR3Y5   = 0x1,
    _T2_CR2Y3   = 0x2,
    _T2_CR3Y4   = 0x3,
    _T2_CR4Y5   = 0x4,
    _T2_CR5Y6   = 0x5,
    _T2_CR_UNKNOWN     = 0xFF,
} DMD_T2_CODERATE;

typedef struct
{
    DMD_T2_CONSTEL   constel;
    DMD_T2_CODERATE   code_rate;
    float   cn_ref;
} DMD_T2_SQI_CN_NORDIGP1;

typedef struct
{
    DMD_T2_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    U16 u16SqiCnNordigP1_Size;

    U32  u32EqStartAddr;
    U32  u32TdiStartAddr;
    U32  u32DjbStartAddr;
    U32  u32FwStartAddr;
} DMD_DVBT2_InitData_t;

// 0402 change for tuner option
 typedef struct DEMOD_DSP_PARAM_s
{
       DEMOD_STANDARD_TYPE_t eDSP_Type;
       TUNER_TYPE_t  eTuner_Type;
       s_ISDBT_INITIAL_PARAMETERS sIsdbtInitialParamTable;
       s_DTMB_INITIAL_PARAMETERS sDtmbInitialParamTable;
       DMD_DVBT2_InitData_t sDVBT2InitData;
} DEMOD_DSP_PARAM_t, *pDEMOD_DSP_PARAM_t;

typedef struct
{
    U8 sdram_state;
    U8 sram_state;
} DEMOD_DVBEXT_STATE_t, *pDEMOD_DVBEXT_STATE_t;

#endif
