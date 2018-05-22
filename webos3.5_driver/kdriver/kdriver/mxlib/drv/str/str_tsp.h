#ifndef __STR_TSP_H
#define __STR_TSP_H

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <mst_str.h>
#include "mst_reg_bank.h"
#include "utopia.h"
#include "apiDMX.h"
#include "apiDMX_v2.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define TSP0_BANK_ADDR                      0x101500
#define TSP1_BANK_ADDR                      0x101600
#define TSO_BANK_ADDR                       0x113A00
#define TSO1_BANK_ADDR                      0x123D00
#define CLKGEN1_BANK_ADDR                   0x103300
#define TSO_SW_RSTZ_DISABLE                 0x0001

//--  TSP0 reg TSP_BASE  [0x44]  --//
#define TSIF_EN                             0x44


//--  TSP1 reg VQ0_BASE  [0x20]  --//
#define VQ0_BASE_L                          0x20
#define VQ0_BASE_H                          0x21
//--  TSP1 reg VQ0_CTRL  [0x22]  --//
#define VQ0_CTRL_L                          0x22
#define VQ0_CTRL_H                          0x23


//--  TSP1 reg VQ1_BASE  [0x56]  --//
#define VQ1_BASE_L                          0x56
#define VQ1_BASE_H                          0x57
//--  TSP1 reg VQ2_CTRL  [0x5c]  --//
#define VQ1_CTRL_L                          0x5c
#define VQ1_CTRL_H                          0x5d


//--  TSP1 reg VQ2_BASE  [0x5e]  --//
#define VQ2_BASE_L                          0x5e
#define VQ2_BASE_H                          0x5F
//--  TSP1 reg VQ2_CTRL  [0x64]  --//
#define VQ2_CTRL_L                          0x64
#define VQ2_CTRL_H                          0x65


//--  TSP1 reg VQ3_BASE  [0x74]  --//
#define VQ3_BASE_L                          0x74
#define VQ3_BASE_H                          0x75
//--  TSP1 reg VQ3_CTRL  [0x76]  --//
#define VQ3_CTRL_L                          0x76
#define VQ3_CTRL_H                          0x77
//--  TSO1 reg VQ related  --//
#define SVQ1_BASE_L                         0x18
#define SVQ1_BASE_H                         0x19
#define SVQ1_SIZE                           0x1A
#define SVQ1_TX_CFG                         0x1B
#define TSO_REG_RESET                       0x00

#define SVQ5_BASE_L                         0x28
#define SVQ5_BASE_H                         0x29
#define SVQ5_SIZE                           0x2A
#define SVQ5_TX_CFG                         0x2B


//-- CLKGEN1 reg PHASE TUNING [0x00 --//
#define PHASE_TUNING                        0x00

//--  TSP1 reg CA_CTRL  [0x40]  --//
#define CA_CTRL_L                           0x40


//boot/u-boot-2011.06/arch/arm/include/asm/mmap_hwopt_512_512.h
#define BIN_MEM_ADR  0x0017A00000
#define BIN_MEM_LEN  0x0000100000


typedef struct
{
    MS_U16 u16Reg_0;
    MS_U16 u16Reg_1;
    MS_U16 u16Reg_2;
    MS_U16 u16Reg_3;
}ST_VQ_INFO;

void TspSaveVqInfo(void);
void TspLoadVqInfo(void);

DMX_FILTER_STATUS tsp_utopia_suspend(void);
DMX_FILTER_STATUS tsp_utopia_resume(void);

MS_BOOL Utopia_MApi_Dmx_UtopiaOpen(void);
DMX_FILTER_STATUS Utopia_MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput);
DMX_FILTER_STATUS Utopia_MApi_DMX_Stop(MS_U8 u8DmxId);
DMX_FILTER_STATUS Utopia_MApi_DMX_SectReset(MS_U8 u8DmxId);
DMX_FILTER_STATUS Utopia_MApi_DMX_Start(MS_U8 u8DmxId);
DMX_FILTER_STATUS Utopia_MApi_DMX_SetPowerState(EN_POWER_MODE u16PowerState, MS_PHY phyFWAddr, MS_U32 u32FWSize);
DMX_FILTER_STATUS Utopia_MApi_DMX_SetFwDataAddr(MS_U32 u32DataAddr, MS_U32 u32size);
MS_BOOL Utopia_MApi_DMX_UtopiaClose(void);

void TspSaveVqInfo(void);
void TspLoadVqInfo(void);


void TspSaveCaCtrlInfo(void);
void TspLoadCaCtrlInfo(void);


void TspSavePhaseTuning(void);
void TspLoadPhaseTuning(void);


void TspSaveTsifInfo(void);
void TspLoadTsifInfo(void);

int application_tsp_init(void);
DMX_FILTER_STATUS Utopia_MApi_DMX_SetFW(MS_PHY pFwAddr, MS_U32 u32FwSize);
DMX_FILTER_STATUS Utopia_MApi_DMX_Init(void);
DMX_FILTER_STATUS Utopia_MApi_DMX_SetHK(MS_BOOL bIsHK);
DMX_FILTER_STATUS Utopia_MApi_DMX_ChkAlive(void);


#endif //End of  __STR_TSP_H

