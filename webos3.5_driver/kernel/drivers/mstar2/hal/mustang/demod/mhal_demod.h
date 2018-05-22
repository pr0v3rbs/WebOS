
#ifndef _MHAL_DEMOD_H_
#define _MHAL_DEMOD_H_

#include "mdrv_types.h"

#include "mdrv_demod_st.h"

#ifdef _MHAL_DEMOD_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

//----------------------------------------------------------------------
INTERFACE void MHal_Demod_WriteReg(U32 u32Addr, U8 u8Value);
INTERFACE void MHal_Demod_ReadReg(U32 u32Addr, U8 *pu8Value);
INTERFACE B16 MHal_Demod_MB_WriteReg(U32 u32Addr, U8 u8Value);
INTERFACE B16 MHal_Demod_MB_ReadReg(U32 u32Addr, U8 *pu8Value);
//Driver update 2009/11/20
INTERFACE B16 MHal_Demod_MB_WriteDspReg(U32 u32Addr, U8 u8Value);
INTERFACE B16 MHal_Demod_MB_ReadDspReg(U32 u32Addr, U8 *pu8Value);
INTERFACE B16 MHal_Demod_MB_WriteReg_ATSC(U32 u32Addr, U8 u8Value);
INTERFACE B16 MHal_Demod_MB_ReadReg_ATSC(U32 u32Addr, U8 *pu8Value);
INTERFACE B16 MHal_Demod_LoadDSPCode(DEMOD_DSP_PARAM_t stParam);// 0402 change for tuner option
INTERFACE B16 MHal_Demod_Stop(DEMOD_STANDARD_TYPE_t eStandard);//mstar 0901 update
INTERFACE B16 MHal_Demod_MB_WriteReg_ISDBT(U32 u32Addr, U8 u8Value);
INTERFACE B16 MHal_Demod_MB_ReadReg_ISDBT(U32 u32Addr, U8 *pu8Value);
INTERFACE B16 MHal_Demod_MB_WriteReg_DTMB(U32 u32Addr, U8 u8Value);
INTERFACE B16 MHal_Demod_MB_ReadReg_DTMB(U32 u32Addr, U8 *pu8Value);


//----------------------------------------------------------------------

#undef INTERFACE

#endif
