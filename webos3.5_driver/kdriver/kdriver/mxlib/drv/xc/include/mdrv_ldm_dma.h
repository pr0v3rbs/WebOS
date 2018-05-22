
#ifndef _MDRV_LDM_DMA_H_
#define _MDRV_LDM_DMA_H_


#ifdef _MDRV_LDM_DMA_C_
    #define _MDRV_LDM_DMA_DEC_
#else
    #define _MDRV_LDM_DMA_DEC_  extern
#endif

#include "mhal_ld.h"
#if 0 //SUPPORT_LD_SPI
#include "MsTypes.h"
#include "msReg.h"
#include "mdrv_mfc_spi.h"
#endif
#include "mdrv_ld.h"


#if 1//( LD_ENABLE)

#define REG_0900 				0x102D00

#define LDM_DMA_PORT  0 //(psDrvLdInfo->u8LdDmaSpiPort)
#define LDM_DMA_REG(x)  (REG_0900+0x100*LDM_DMA_PORT+(x << 1))
#if ENABLE_TWO_SPI
#define LDM_DMA_PORT2  MSPI_PORT_NUM2
#define LDM_DMA_REG2(x)  (REG_0900+0x100*LDM_DMA_PORT2+x)
#endif

#define LDM_MAX_SPI_CMD_NUM     10//ursa7 , 10

typedef enum
{
    E_LDM_SPI_TRIGER_STOP,
    E_LDM_SPI_TRIGER_ONE_SHOT,
    E_LDM_SPI_TRIGER_EVERY_V_SYNC,
}EnuLdmSpiTriggerMode;

void MDrv_LDM_DMA_Init(void* pInstance);

void MDrv_LDM_DMA_SetMenuloadNumber( MS_U16 u16Number );

void MDrv_LDM_DMA_SetSPICommandFormat( MS_U8 u8CmdLen, MS_U16* pu16CmdBuf, MS_BOOL bEnCheckSum );

void MDrv_LDM_DMA_SetSpiTriggerMode( EnuLdmSpiTriggerMode eLdmSpiTriggerMode );

void MDrv_LDM_DMA_SetSpiCmd(void* pInstance, MS_U8 u8Mask, MS_BOOL bEnable);

#endif 
#endif


