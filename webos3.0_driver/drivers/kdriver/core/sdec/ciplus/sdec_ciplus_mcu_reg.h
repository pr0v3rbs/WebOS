#ifndef _SDEC_CIPLUS_MCU_REG_H_
#define _SDEC_CIPLUS_MCU_REG_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif


#define MCU_REG_OFFSET_PROC_CTRL		0x000

#define MCU_REG_OFFSET_SROM_OFFSET		0x010
#define MCU_REG_OFFSET_SRAM_OFFSET_0	0x014
#define MCU_REG_OFFSET_SRAM_OFFSET_1	0x018
#define MCU_REG_OFFSET_SRAM_OFFSET_2	0x01C


#define MCU_REG_OFFSET_E_INTR_EN		0x030
#define MCU_REG_OFFSET_E_INTR_ST		0x034
#define MCU_REG_OFFSET_E_INTR_CL		0x038
#define MCU_REG_OFFSET_E_TO_I_EV		0x03C

#define MCU_REG_OFFSET_I_INTR_EN		0x040
#define MCU_REG_OFFSET_I_INTR_ST		0x044
#define MCU_REG_OFFSET_I_INTR_CL		0x048
#define MCU_REG_OFFSET_I_TO_E_EV		0x04C

#define 	MCU_INTR_IPC_MASK					(0x1 << 10)
#define 	MCU_INTR_DMA_MASK					(0x1 <<  9)


#define MCU_REG_OFFSET_IPC				0x100
#define MCU_REG_SIZE_IPC				0x100



#ifdef __cplusplus
}
#endif

#endif	/* _SDEC_CIPLUS_MCU_REG_H_ */

