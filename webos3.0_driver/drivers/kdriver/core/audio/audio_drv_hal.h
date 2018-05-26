/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/



/** @file
 *
 *  Linux proc interface for audio device.
 *  audio device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.24
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_HAL_H_
#define	_AUDIO_DRV_HAL_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "os_util.h" //for alloc functions

#include "base_types.h"
#include "audio_kapi.h"
#include "audio_drv.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
#include <asm/hardware.h> // For Register base address
#endif

#include "sys_regs.h"	//for ACE TOP CTRL Reg. map

#include "audio_kapi.h"
#include "audio_drv.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


//#define ENABLE_ADEC_JTAG			//Enable ADEC DSP JTAG(2014.12.03)
//#define ENABLE_ADEC_JTAG_FOR_CPU	//Enable ADEC DSP JTAG for CPU PORT

//define DSP Memory Flush Iteration
#define AUD_DSP_FLUSH_ITERATION		10		//loop per 10 us

//define reset module
#define AUD_OPTION_DSP0				0x0		//DSP0 only
#define AUD_OPTION_DSP1				0x1		//DSP1 only
#define AUD_OPTION_AAD				0x2		//AAD Module only
#define AUD_OPTION_OTHERS			0x3		//Except DSP and Power-down
#define AUD_OPTION_ALL				0x4		//All reset

//define reset delay
#define AUD_RESET_DELAY				 5		//5us
#define AAD_RESET_DELAY				 5		//5us

//define for constant value
#define AUD_CODEC_NAME_SIZE			 30

#define AUDIO_IPC_BUF_SIZE			0x18400			// 97Kbytes


// Sound Engine Update Mask
#define MASK_SE_FN_MODE_SHIFT   		(8)
#define MASK_SE_FN_MODE_FN000   		(1 <<  0)
#define MASK_SE_FN_MODE_FN001   		(1 <<  1)
#define MASK_SE_FN_MODE_FN002   		(1 <<  2)
#define MASK_SE_FN_MODE_FN003   		(1 <<  3)
#define MASK_SE_FN_MODE_FN004_MODE1		(1 <<  4)
#define MASK_SE_FN_MODE_FN004_MODE2		(1 <<  5)
#define MASK_SE_FN_MODE_FN004_MODE3		(1 <<  6)
#define MASK_SE_FN_MODE_FN005   		(1 <<  7)
#define MASK_SE_FN_MODE_FN006   		(1 <<  8)
#define MASK_SE_FN_MODE_FN007   		(1 <<  9)
#define MASK_SE_FN_MODE_FN008   		(1 << 10)
#define MASK_SE_FN_MODE_FN009   		(1 << 11)
#define MASK_SE_FN_MODE_FN010   		(1 << 12)
#define MASK_SE_FN_MODE_FN011   		(1 << 13)
#define MASK_SE_FN_MODE_FN012   		(1 << 14)
#define MASK_SE_FN_MODE_FN013   		(1 << 15)
#define MASK_SE_FN_MODE_FN014   		(1 << 16)
#define MASK_SE_FN_MODE_FN015   		(1 << 17)
#define MASK_SE_FN_MODE_FN016   		(1 << 18)
#define MASK_SE_FN_MODE_FN017   		(1 << 19)
#define MASK_SE_FN_MODE_FN019   		(1 << 20)
#define MASK_SE_FN_MODE_FN020   		(1 << 21)
#define MASK_SE_FN_MODE_FN022   		(1 << 22)
#define MASK_SE_FN_MODE_MAIN	   		(1 << 23)

//define for constant value
#define ADEC_CODEC_NAME_SIZE			30

/**
 * AUD Register Address
 * This enumeration describes the ADEC Register Address.
 *
 */
typedef  enum
{
	LX_AUD_REG_DEBUG_PRINT_MASK		=   0,	///< aud_dbg_print_mask
	LX_AUD_REG_INT0_CLR				=   1,	///< aud_int0_clr
	LX_AUD_REG_INT1_CLR				=   2,	///< aud_int1_clr
	LX_AUD_REG_DSP0_GSTC			=   3,	///< aud_dsp0_gstc
	LX_AUD_REG_DSP1_GSTC			=   4,	///< aud_dsp1_gstc
	LX_AUD_REG_DEBUG_DSP0_RESET		=   5,	///< aud_dbg_dsp0_reset
	LX_AUD_REG_DEBUG_DSP1_RESET		=   6,	///< aud_dbg_dsp1_reset
	LX_AUD_REG_DEC0_CBT				=   7,	///< aud_dec0_cbt
	LX_AUD_REG_DEC0_SBT				=   8,	///< aud_dec0_sbt
	LX_AUD_REG_DEC1_CBT				=   9,	///< aud_dec1_cbt
	LX_AUD_REG_DEC1_SBT				=  10,	///< aud_dec1_sbt
	LX_AUD_REG_GSTC_LOW				=  11,	///< aud_gstcc0lreg
	LX_AUD_REG_LEVELMO_L			=  12,	///< aud_levelmo_l
	LX_AUD_REG_LEVELMO_R			=  13,	///< aud_levelmo_r
	LX_AUD_REG_AAD_BYPASS			=  14,	///< aad_bypass
	LX_AUD_REG_DSP0_IRQ				=  15,	///< aud_dirq0
	LX_AUD_REG_DSP1_IRQ				=  16,	///< aud_dirq1
	LX_AUD_REG_TPM_BUFSIZE 			=  17,	///< aud_tpm_bufsize
	LX_AUD_REG_TPA_BUFSIZE 			=  18,	///< aud_tpa_bufsize
} LX_AUD_REG_ADDRESS_T;

/**
 * AUD MonitorTask Message Status
 * This enumeration describes the ADEC monitor task status.
 *
 */
#ifdef DISABLE_SCART_HW_BYPASS
typedef  enum
{
	LX_AUD_TASK_NONE_MSG		= 0x000,	///< None
	LX_AUD_TASK_RESET_SCART		= 0x001,	///< SCART Reset after boot.
} LX_AUD_TASK_MSG_T;
#else
typedef  enum
{
	LX_AUD_TASK_NONE_MSG		= 0x000,	///< None
	LX_AUD_TASK_RESET_DSP0		= 0x001,	///< Reset DSP0 with exception handler interrupt
	LX_AUD_TASK_RESET_DSP1		= 0x002,	///< Reset DSP1 with exception handler interrupt
	LX_AUD_TASK_SET_SPDIF		= 0x004,	///< Set SPDIF clock for DEC0
	LX_AUD_TASK_USE_GSTC		= 0x008,	///< Use GSTC for MHEG TP stream lipsync
	LX_AUD_TASK_SET_DTO_RATE	= 0x010,	///< Set SPDIF DTO Clock Rate
	LX_AUD_TASK_SET_SPDIF_DEC1	= 0x020,	///< Set SPDIF clock for DEC1
	LX_AUD_TASK_RESET_CLOCK		= 0x040,	///< Reset a I2S and SPDIF clock registers
	LX_AUD_TASK_RESTART_DEC0	= 0x080,	///< Restart DEC0 DTS codec when undeflow occurrs dts codec in HDMI input
	LX_AUD_TASK_RESTART_DEC1	= 0x100		///< Restart DEC1 DTS codec when undeflow occurrs dts codec in HDMI input
} LX_AUD_TASK_MSG_T;
#endif	//#ifdef DISABLE_SCART_HW_BYPASS

typedef enum
{
	WRITER_RET_OPEN_OK = 0,
	WRITER_RET_CLOSE_OK,
	WRITER_RET_INIT_OK,
	WRITER_RET_WRITE_OK,
	WRITER_RET_FLUSH_OK,
	WRITER_RET_UNDERFLOW,
	WRITER_RET_OVERFLOW,
	WRITER_RET_ERROR,
	WRITER_RET_UNKNOWN
} WRITER_RET_TYPE;

typedef struct
{
	BOOLEAN			bInitDone;				/* Current Audio Init Done Status */

	//DSP load and copy flag after H/W reset
	BOOLEAN 		LoadCodec ;
	BOOLEAN 		CopyDSP0;
	BOOLEAN 		CopyDSP1;
	BOOLEAN 		EnableIMCDBG;

	/* Set for DSP 0/1 state for memory flush */
	UINT32			IsDSP0Reseted;
	UINT32			IsDSP1Reseted;
	int				IsDSP0Ready;
	int 			IsDSP1Ready;

	void			*p_aud_mem_share_base;	// 0x0300 : ''

	BOOLEAN			bSuspended;
}LX_AUD_KDRV_T;

/**
Lipsync basetime of VDEC
*/
typedef struct
{
	UINT32			vdec_dec0_cbt;	// 0x08F0
	UINT32			vdec_dec0_sbt;	// 0x08F4
	UINT32			vdec_dec1_cbt;	// 0x08F8
	UINT32			vdec_dec1_sbt;	// 0x08FC
} VDEC_LIPSYNC_REG_T;

/**
IPC R/W Offset Register Address
*/
typedef struct
{
	void	*p_aud_ipc_a2p_rptr;	// 0x0370 : ''
	void	*p_aud_ipc_a2p_wptr;	// 0x0374 : ''
	void	*p_aud_ipc_p2a_rptr;	// 0x0378 : ''
	void	*p_aud_ipc_p2a_wptr;	// 0x037c : ''
	void	*p_aud_ipc_a2d_rptr;	// 0x0380 : ''
	void	*p_aud_ipc_a2d_wptr;	// 0x0384 : ''
	void	*p_aud_ipc_d2a_rptr;	// 0x0388 : ''
	void	*p_aud_ipc_d2a_wptr;	// 0x038c : ''
	void	*p_aud_ipc_d2p_rptr;	// 0x0390 : ''
	void	*p_aud_ipc_d2p_wptr;	// 0x0394 : ''
	void	*p_aud_ipc_p2d_rptr;	// 0x0398 : ''
	void	*p_aud_ipc_p2d_wptr;	// 0x039c : ''
	void	*p_aud_ipc_dbgp_rptr;	// 0x03a0 : ''
	void	*p_aud_ipc_dbgp_wptr;	// 0x03a4 : ''
	void	*p_aud_ipc_dbgd_rptr;	// 0x03a8 : ''
	void	*p_aud_ipc_dbgd_wptr;	// 0x03ac : ''
} IPC_RW_OFFSET_ADDR_T;


/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
#ifdef DISABLE_SCART_HW_BYPASS
extern LX_AUD_TASK_MSG_T g_suiWaitQForMonitor;
extern wait_queue_head_t Monitor_WaitQ;
#endif	//#ifdef DISABLE_SCART_HW_BYPASS

SINT32 AUDIO_InitModule ( void );
SINT32 AUDIO_WriteReg ( LX_AUD_REG_ADDRESS_T addr, UINT32 data );
UINT32 AUDIO_ReadReg ( LX_AUD_REG_ADDRESS_T addr );

SINT32 AUDIO_LoadDSP1Codec ( void );
SINT32 AUDIO_LoadDSP0Codec ( void );

void AUDIO_ShowIoremap( void );
void AUDIO_ShowMemoryDump( LX_AUD_DEBUG_MEM_DUMP_T *pMemDump );

SINT32 AUDIO_MonitorTask(void *pParam);

UINT32 AUDIO_ReadDebugMask (void);
void AUDIO_WriteDebugMask (unsigned int mask);

void AUDIO_Restart(void);
SINT32 AUDIO_ResetAADModule(void);

SINT32 AUDIO_SetReset ( UINT8 ui8ResetFlag );
SINT32 AUDIO_ClearReset ( UINT8 ui8ResetFlag );
SINT32 AUDIO_InitReg ( void );
SINT32 AUDIO_InitRegForCheckbit ( void );

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_HAL_H_ */

/** @} */


