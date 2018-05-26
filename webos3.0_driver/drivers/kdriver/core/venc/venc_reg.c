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
 *  register api implementation for venc device.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2013.12.18
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
#ifdef INCLUDE_H15_CHIP_KDRV
#undef INCLUDE_H15_CHIP_KDRV
#endif

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "os_util.h"
#include "venc_drv.h"
#include "venc_cfg.h"
#include "venc_reg.h"

#include "venc_h13_reg.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#define VENC_REG_RdFL(_c,_id,_r)		((gShadRegVENC[id]->_c._r)=(gRealRegVENC[id]->_c._r))
#define VENC_REG_WrFL(_c,_id,_r)		((gRealRegVENC[id]->_c._r)=(gShadRegVENC[id]->_c._r))

#define VENC_REG_Rd(_c,_id,_r)			*((UINT32*)(&(gShadRegVENC[id]->_c._r)))
#define VENC_REG_Wr(_c,_id,_r,_v)		((VENC_REG_Rd(_c,_id,_r))=((UINT32)(_v)))

#define VENC_REG_Rd01(_c,_id,_r,_f01,_v01)			\
	do { 											\
		(_v01) = (gShadRegVENC[id]->_c._r._f01);	\
	} while(0)

#define VENC_REG_Wr01(_c,_id,_r,_f01,_v01)			\
	do { 											\
		(gShadRegVENC[id]->_c._r._f01) = (_v01);	\
	} while(0)

#if 0
#define VENC_REG_LOCK(id)				spin_lock(&venc_reg_lock[id])
#define VENC_REG_UNLOCK(id)				spin_unlock(&venc_reg_lock[id])
#else
#define VENC_REG_LOCK(id)				spin_lock_irqsave(&venc_reg_lock[id], flags)
#define VENC_REG_UNLOCK(id)				spin_unlock_irqrestore(&venc_reg_lock[id], flags)
#endif

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef union {
	VENC_H13_REG_T	H13;
} VENC_REG_T;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
//static DEFINE_SPINLOCK(venc_reg_lock);
static spinlock_t			venc_reg_lock[VENC_MAX_DEVICE];
static volatile VENC_REG_T 	*gShadRegVENC[VENC_MAX_DEVICE];
static volatile VENC_REG_T 	*gRealRegVENC[VENC_MAX_DEVICE];
static volatile void 		*gIPCExternal[VENC_MAX_DEVICE];	// for DE IPC Register.

/*========================================================================================
	Implementation Group
========================================================================================*/

int VENC_REG_Init( int id, LX_VENC_CFG_CORE_T cfg_core )
{
	u32 base = cfg_core.reg_base_internal;
	u32 size = cfg_core.reg_size_internal;

	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}

	gRealRegVENC[id] = (VENC_REG_T *)ioremap( base, size);
	gShadRegVENC[id] = (VENC_REG_T *)kzalloc( size, GFP_KERNEL );

	if ( cfg_core.reg_base_external > 0 )
	{
		gIPCExternal[id] = ioremap( cfg_core.reg_base_external, 0x100 );

		VENC_INFO("CORE#%d gIPCExternal = 0x%08X\n", id, gIPCExternal[id] );
	}
	else
	{
		gIPCExternal[id] = NULL;
	}

	spin_lock_init(&venc_reg_lock[id]);
	
	if ( gRealRegVENC[id] == NULL || gShadRegVENC[id] == NULL )
	{
		VENC_ERROR("ioremap failed\n");

		VENC_REG_Cleanup( id );

		return RET_ERROR;
	}

	VENC_INFO("CORE#%d gRealRegVENC=0x%08X, gShadRegVENC=0x%08X\n", id, gRealRegVENC[id], gShadRegVENC[id]);

	return RET_OK;
}

int VENC_REG_Cleanup( int id )
{
	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}

	if ( gRealRegVENC[id] != NULL )
	{
		iounmap( (void *)gRealRegVENC[id] );
		gRealRegVENC[id] = NULL;
	}

	if ( gShadRegVENC[id] != NULL )
	{
		kfree( (void *)gShadRegVENC[id] );
		gShadRegVENC[id] = NULL;
	}

	// INCLUDE_M14_CHIP_KDRV
	//if ( lx_chip_rev() >= LX_CHIP_REV( M14, B0 ) && lx_chip_rev() < LX_CHIP_REV( H14, A0 ) )
	if ( gIPCExternal[id] != NULL )
	{
		if ( gIPCExternal[id] != NULL )
		{
			iounmap( (void *)gIPCExternal[id] );
			gIPCExternal[id] = NULL;
		}
	}

	return RET_OK;
}

int VENC_REG_CopyExternalRegs( int id )
{
	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return RET_ERROR;
	}

	//if ( lx_chip_rev() >= LX_CHIP_REV(M14, B0) && lx_chip_rev() < LX_CHIP_REV(H14, A0) )
	if ( gIPCExternal[id] != NULL )
	{
		UINT32 IPCInternalBase = (UINT32)gRealRegVENC[id] + 0xA0;
		UINT32 IPCExternalBase = gIPCExternal[id];

		//printk("IPCInternalBase=0x%08X, IPCExternalBase=0x%08X\n", IPCInternalBase, IPCExternalBase);

		// Copy ipc register values.
		memcpy( (void *)IPCInternalBase, (void *)IPCExternalBase, 0x40 );
	}

	return RET_OK;
}

int VENC_REG_DebugPrintRegs( int id )
{
	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}

	if ( gIPCExternal[id] != NULL )
	{
		int i = 0;
		UINT32 *regBase = (UINT32 *)(gIPCExternal[id]);

		printk("External IPC:\n");

		for ( i = 0; i < 6; i++ )
		{
			UINT32 *ptr = (UINT32 *)&regBase[i*4];
			UINT32 offset = ((UINT32)ptr - (UINT32)regBase);

			printk("0x%04X %08X %08X %08X %08X\n", offset, ptr[0], ptr[1], ptr[2], ptr[3]);
		}

		printk("\n");
	}

	if ( gRealRegVENC[id] != NULL )
	{
		int i = 0;
		UINT32 *regBase = (UINT32 *)(gRealRegVENC[id]);

		printk("Internal IPC:\n");

		for ( i = 0; i < 16; i++ )
		{
			UINT32 *ptr = (UINT32 *)&regBase[i*4];
			UINT32 offset = ((UINT32)ptr - (UINT32)regBase);

			printk("0x%04X %08X %08X %08X %08X\n", offset, ptr[0], ptr[1], ptr[2], ptr[3]);
		}

		printk("\n");
	}

	return RET_OK;
}

void VENC_REG_SetTime( int id, UINT32 clock )
{
	unsigned long		flags;

	if ( id >= VENC_MAX_DEVICE )
	{
		return;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return;
	}

	VENC_REG_LOCK(id);

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		VENC_REG_RdFL(H15,id,timer);
		VENC_REG_Wr(H15,timer,clock);
		VENC_REG_WrFL(H15,timer);
	}
	else
#endif //INCLUDE_H15_CHIP_KDRV
	{
		VENC_REG_RdFL(H13,id,timer);
		VENC_REG_Wr(H13,id,timer,clock);
		VENC_REG_WrFL(H13,id,timer);
	}

	VENC_REG_UNLOCK(id);
}

UINT32 VENC_REG_GetTime( int id )
{
	UINT32 clock;

	if ( id >= VENC_MAX_DEVICE )
	{
		return 0;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return 0;
	}

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		VENC_REG_RdFL(H15,id,timer);
		clock = VENC_REG_Rd(H15,timer);
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		VENC_REG_RdFL(H13,id,timer);
		clock = VENC_REG_Rd(H13,id,timer);
	}

	return clock;
}

UINT32 VENC_REG_GetSTC( int id, LX_VENC_TIMESTAMP_TYPE_T eTimestamp )
{
	UINT32 timestamp;

	if ( id >= VENC_MAX_DEVICE )
	{
		return 0;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return 0;
	}

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		switch( eTimestamp )
		{
			case LX_VENC_TIMESTAMP_PTS:
				VENC_REG_RdFL(H15,id,info);
				timestamp = VENC_REG_Rd(H15,info);
			break;

#ifdef H1ENCODE_USE_PREP_STC
			case LX_VENC_TIMESTAMP_STC:
				VENC_REG_RdFL(H15,id,stcc);
				timestamp = VENC_REG_Rd(H15,stcc);
			break;
#endif

			case LX_VENC_TIMESTAMP_GSTC:
				VENC_REG_RdFL(H15,id,vdec_gstcc0);
				timestamp = VENC_REG_Rd(H15,vdec_gstcc0);
			default:
			break;
		}
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		switch( eTimestamp )
		{
			case LX_VENC_TIMESTAMP_PTS:
				VENC_REG_RdFL(H13,id,info);
				timestamp = VENC_REG_Rd(H13,id,info);
			break;

#ifdef H1ENCODE_USE_PREP_STC
			case LX_VENC_TIMESTAMP_STC:
				VENC_REG_RdFL(H13,id,stcc);
				timestamp = VENC_REG_Rd(H13,id,stcc);
			break;
#endif

			case LX_VENC_TIMESTAMP_GSTC:
				VENC_REG_RdFL(H13,id,vdec_gstcc0);
				timestamp = VENC_REG_Rd(H13,id,vdec_gstcc0);
			default:
			break;
		}
	}

	return timestamp;
}

UINT32 VENC_REG_PrintIPCStatus( int id )
{
	u32 yBase, cBase;
	int inHeight, inWidth;
	int yStride, cStride;
	int frm_rate;
	LX_VENC_FRAME_TYPE_T frameType;
	BOOLEAN	topFieldFirst = FALSE;
	int decHeight, decWidth;

	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return RET_ERROR;
	}

	// Frame base Address (Y)
	VENC_REG_RdFL(H13,id,y_addr);
	VENC_REG_Rd01(H13,id,y_addr, addr, yBase);

	// Frame base Address (CbCr)	
	VENC_REG_RdFL(H13,id,c_addr);
	VENC_REG_Rd01(H13,id,c_addr, addr, cBase);

	// Size
	VENC_REG_RdFL(H13,id,de_dsize);
	VENC_REG_Rd01(H13,id,de_dsize, vsize, inHeight);
	VENC_REG_Rd01(H13,id,de_dsize, hsize, inWidth);

	// SAR Infomation
	VENC_REG_RdFL(H13,id,decsize);
	VENC_REG_Rd01(H13,id,decsize, dec_vsize, decHeight);
	VENC_REG_Rd01(H13,id,decsize, dec_hsize, decWidth);

	// Frame Type (Field/Frame)
	VENC_REG_RdFL(H13,id,pic_info);
	VENC_REG_Rd01(H13,id,pic_info, frm_struct, frameType);

	// Top Field First (True/False)
	VENC_REG_RdFL(H13,id,pic_info);
	VENC_REG_Rd01(H13,id,pic_info, topfield_first, topFieldFirst);

	// Stride (Y/C)
	VENC_REG_RdFL(H13,id,y_stride);
	VENC_REG_Rd01(H13,id,y_stride, stride, yStride);
	VENC_REG_RdFL(H13,id,c_stride);
	VENC_REG_Rd01(H13,id,c_stride, stride, cStride);

	// Sequence Info
	VENC_REG_RdFL(H13,id,seqinfo);
	VENC_REG_Rd01(H13,id,seqinfo, frm_rate, frm_rate);

	VENC_INFO("[%d] %d %d %d %d %X %X", 
			id, frameType, frm_rate, inWidth, inHeight, yBase, cBase);

	return RET_OK;
}

UINT32 VENC_REG_ReadIPCStatus( int id, LX_VENC_REG_IPC_STATUS_T *pstIPCStatus )
{
	BOOLEAN _enableFlipTB = FALSE;

	u32 yBase, cBase;
	int inHeight, inWidth;
	int yStride, cStride;
	int frm_rate;
	LX_VENC_FRAME_TYPE_T frameType;
	BOOLEAN	topFieldFirst = FALSE;
	int decHeight, decWidth;

	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}

	if ( pstIPCStatus == NULL )
	{
		return RET_ERROR;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return RET_ERROR;
	}

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		// Frame base Address (Y)
		VENC_REG_RdFL(H15,id,y_addr);
		VENC_REG_Rd01(H15,y_addr, addr, yBase);

		// Frame base Address (CbCr)	
		VENC_REG_RdFL(H15,id,c_addr);
		VENC_REG_Rd01(H15,c_addr, addr, cBase);

		// Size
		VENC_REG_RdFL(H15,id,de_dsize);
		VENC_REG_Rd01(H15,de_dsize, vsize, inHeight);
		VENC_REG_Rd01(H15,de_dsize, hsize, inWidth);

		// SAR Infomation
		VENC_REG_RdFL(H15,id,decsize);
		VENC_REG_Rd01(H15,decsize, dec_vsize, decHeight);
		VENC_REG_Rd01(H15,decsize, dec_hsize, decWidth);

		// Frame Type (Field/Frame)
		VENC_REG_RdFL(H15,id,pic_info);
		VENC_REG_Rd01(H15,pic_info, frm_struct, frameType);

		// Top Field First (True/False)
		VENC_REG_RdFL(H15,id,pic_info);
		VENC_REG_Rd01(H15,pic_info, topfield_first, topFieldFirst);

		// Stride (Y/C)
		VENC_REG_RdFL(H15,id,y_stride);
		VENC_REG_Rd01(H15,y_stride, stride, yStride);
		VENC_REG_RdFL(H15,id,c_stride);
		VENC_REG_Rd01(H15,c_stride, stride, cStride);

		// Sequence Info
		VENC_REG_RdFL(H15,id,seqinfo);
		VENC_REG_Rd01(H15,seqinfo, frm_rate, frm_rate);
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		// Frame base Address (Y)
		VENC_REG_RdFL(H13,id,y_addr);
		VENC_REG_Rd01(H13,id,y_addr, addr, yBase);

		// Frame base Address (CbCr)	
		VENC_REG_RdFL(H13,id,c_addr);
		VENC_REG_Rd01(H13,id,c_addr, addr, cBase);

		// Size
		VENC_REG_RdFL(H13,id,de_dsize);
		VENC_REG_Rd01(H13,id,de_dsize, vsize, inHeight);
		VENC_REG_Rd01(H13,id,de_dsize, hsize, inWidth);

		// SAR Infomation
		VENC_REG_RdFL(H13,id,decsize);
		VENC_REG_Rd01(H13,id,decsize, dec_vsize, decHeight);
		VENC_REG_Rd01(H13,id,decsize, dec_hsize, decWidth);

		// Frame Type (Field/Frame)
		VENC_REG_RdFL(H13,id,pic_info);
		VENC_REG_Rd01(H13,id,pic_info, frm_struct, frameType);

		// Top Field First (True/False)
		VENC_REG_RdFL(H13,id,pic_info);
		VENC_REG_Rd01(H13,id,pic_info, topfield_first, topFieldFirst);

		// Stride (Y/C)
		VENC_REG_RdFL(H13,id,y_stride);
		VENC_REG_Rd01(H13,id,y_stride, stride, yStride);
		VENC_REG_RdFL(H13,id,c_stride);
		VENC_REG_Rd01(H13,id,c_stride, stride, cStride);

		// Sequence Info
		VENC_REG_RdFL(H13,id,seqinfo);
		VENC_REG_Rd01(H13,id,seqinfo, frm_rate, frm_rate);
	}

	pstIPCStatus->yBase = yBase;
	pstIPCStatus->cBase = cBase;

	pstIPCStatus->inWidth = inWidth;
	pstIPCStatus->inHeight = inHeight;
	pstIPCStatus->yStride = yStride;
	pstIPCStatus->cStride = cStride;
	pstIPCStatus->frameType = frameType;
	pstIPCStatus->frameRateCode = frm_rate;
	pstIPCStatus->topFieldFirst = topFieldFirst;

	pstIPCStatus->decWidth = decWidth;
	pstIPCStatus->decHeight = decHeight;

	return RET_OK;
}

void VENC_REG_InterruptEnable( int id, BOOLEAN enable )
{
	unsigned long		flags;

	if ( id >= VENC_MAX_DEVICE )
	{
		return;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return;
	}

	VENC_REG_LOCK(id);

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		VENC_REG_RdFL(H15,id,vdecintr_e_en);

		if ( enable )
		{
#if !defined(H1ENCODE_USE_POLLING)
			VENC_REG_Wr01(H15,vdecintr_e_en, h1_intr_e_en, 1);
#else
			VENC_REG_Wr01(H15,vdecintr_e_en, h1_intr_e_en, 0);
#endif
		}
		else
		{
			VENC_REG_Wr01(H15,vdecintr_e_en, h1_intr_e_en, 0);
		}

		VENC_REG_WrFL(H15,vdecintr_e_en);
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		VENC_REG_RdFL(H13,id,vdecintr_e_en);

		if ( enable )
		{
#if !defined(H1ENCODE_USE_POLLING)
			VENC_REG_Wr01(H13,id,vdecintr_e_en, h1_intr_e_en, 1);
#else
			VENC_REG_Wr01(H13,id,vdecintr_e_en, h1_intr_e_en, 0);
#endif
		}
		else
		{
			VENC_REG_Wr01(H13,id,vdecintr_e_en, h1_intr_e_en, 0);
		}

		VENC_REG_WrFL(H13,id,vdecintr_e_en);
	}

	VENC_REG_UNLOCK(id);
}

UINT32 VENC_REG_InterruptRead( int id )
{
	UINT32 status;

	if ( id >= VENC_MAX_DEVICE )
	{
		return RET_ERROR;
	}


	if ( gRealRegVENC[id] == NULL )
	{
		return 0;
	}

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		VENC_REG_RdFL(H15,id,vdecintr_e_st);
		status = VENC_REG_Rd(H15,vdecintr_e_st);
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		VENC_REG_RdFL(H13,id,vdecintr_e_st);
		status = VENC_REG_Rd(H13,id,vdecintr_e_st);
	}

	return status;
}

void VENC_REG_InterruptClear( int id )
{
	UINT32 enableIntr;
	unsigned long		flags;

	if ( id >= VENC_MAX_DEVICE )
	{
		return;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return;
	}

	VENC_REG_LOCK(id);

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		VENC_REG_RdFL(H15,id,vdecintr_e_en);
		enableIntr = VENC_REG_Rd(H15,vdecintr_e_en);

		if ( enableIntr & 0x1 ) // clear vdec
			VENC_REG_Wr01(H15,vdecintr_e_cl, vdec_intr_e_cl, 1);

		if ( enableIntr & 0x2 ) // clear de vsync
			VENC_REG_Wr01(H15,vdecintr_e_cl, de_intr_e_cl, 1);

		VENC_REG_WrFL(H15,vdecintr_e_cl);
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		VENC_REG_RdFL(H13,id,vdecintr_e_en);
		enableIntr = VENC_REG_Rd(H13,id,vdecintr_e_en);

		if ( enableIntr & 0x1 ) // clear vdec
			VENC_REG_Wr01(H13,id,vdecintr_e_cl, vdec_intr_e_cl, 1);

		if ( enableIntr & 0x2 ) // clear de vsync
			VENC_REG_Wr01(H13,id,vdecintr_e_cl, de_intr_e_cl, 1);

		VENC_REG_WrFL(H13,id,vdecintr_e_cl);
	}

	VENC_REG_UNLOCK(id);
}

void VENC_REG_InterruptEnableVsync( int id, BOOLEAN enable )
{
	unsigned long		flags;

	if ( id >= VENC_MAX_DEVICE )
	{
		return;
	}

	if ( gRealRegVENC[id] == NULL )
	{
		return;
	}

	VENC_REG_LOCK(id);

#ifdef INCLUDE_H15_CHIP_KDRV
	if ( lx_chip_rev() >= LX_CHIP_REV( H15, A0 ) )
	{
		VENC_REG_RdFL(H15,id,vdecintr_e_en);

		if ( enable )
		{
			VENC_REG_Wr01(H15,vdecintr_e_en, vdec_intr_e_en, 1);
			VENC_REG_Wr01(H15,vdecintr_e_en, de_intr_e_en, 1);
		}
		else
		{
			VENC_REG_Wr01(H15,vdecintr_e_en, vdec_intr_e_en, 0);
			VENC_REG_Wr01(H15,vdecintr_e_en, de_intr_e_en, 0);
		}

		VENC_REG_WrFL(H15,id,vdecintr_e_en);
	}
	else
#endif // INCLUDE_H15_CHIP_KDRV
	{
		VENC_REG_RdFL(H13,id,vdecintr_e_en);

		if ( enable )
		{
			VENC_REG_Wr01(H13,id,vdecintr_e_en, vdec_intr_e_en, 1);
			VENC_REG_Wr01(H13,id,vdecintr_e_en, de_intr_e_en, 1);
		}
		else
		{
			VENC_REG_Wr01(H13,id,vdecintr_e_en, vdec_intr_e_en, 0);
			VENC_REG_Wr01(H13,id,vdecintr_e_en, de_intr_e_en, 0);
		}

		VENC_REG_WrFL(H13,id,vdecintr_e_en);
	}

	VENC_REG_UNLOCK(id);
}


/*-----------------------------------------------------------------------------
	Static Function Definition
-----------------------------------------------------------------------------*/


/** @} */

