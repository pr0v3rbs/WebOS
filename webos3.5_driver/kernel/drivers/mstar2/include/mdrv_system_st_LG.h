////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
//
// You can redistribute it and/or modify it under the terms of the GNU General Public
// License version 2 as published by the Free Foundation. This program is distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_SYSTEM_ST_LG_H__
#define __DRV_SYSTEM_ST_LG_H__

//------------------------------------------------------------------------------
// Data structure
//------------------------------------------------------------------------------
typedef struct IO_SYS_PANEL_INFO_s
{
    U32* pPanelInfo;
    U16  u16Len;
} IO_SYS_PANEL_INFO_t, *PIO_SYS_PANEL_INFO_t;

typedef struct IO_SYS_PANEL_Res_s
{
    U16  u16Width;
    U16  u16Height;
} IO_SYS_PANEL_GET_RES_t, *PIO_SYS_PANEL_GET_RES_t;

typedef struct IO_SYS_BOARD_INFO_s
{
    U32* pu32BoardInfo;
    U16  u16Len;
} IO_SYS_BOARD_INFO_t, *PIO_SYS_BOARD_INFO_t;

typedef struct IO_SYS_GENERAL_REG_s
{
    U16  u16Reg;
    U8   u8Value;
} IO_SYS_GENERAL_REG_t;

typedef struct IO_SYS_AEONBIN_INFO_s
{
	U8* pu8AeonStart;
	U32 u32Len;
	BOOL bRet;
} IO_SYS_AEONBIN_INFO_t;

typedef enum
{
    RELOAD_AEON_STOP,
    RELOAD_AEON_RESTART
} AEON_CONTROL ;

typedef struct IO_SYS_MIU_PROTECT_s
{
    U8   u8Blockx;
    U8   *pu8ProtectId;
    U32  u32Start;
    U32  u32End;
    BOOL bSetFlag;
} IO_SYS_MIU_PROTECT_t;


typedef struct IO_SYS_SPI_s
{
   U32 u32Start;
   U32 u32Len;
   U8 *u8data;
} IO_SYS_SPI_t;

typedef struct IO_SYS_MIOMAP_INFO_s
{
    unsigned int  u32Addr;
    unsigned int  u32Size;
} IO_SYS_MIOMAP_INFO_t;

typedef struct IO_SYS_MEM_INFO_s
{
    unsigned int  u321ddr;
    unsigned int  u32Miu1Adr;

    U32    u32TotalMemSize;
    U32    u32Miu0MemSize;
    U32    u32Miu1MemSize;
    U32    u32MiuBoundary;
} IO_SYS_MEM_INFO_t;

typedef struct
{
    U16                  u16Addr;
    U8                   u8Value;
    U8                   u8Type;     // 1: AttribMem, 2: IOMem
    U16                  u16DataLen;
    U8 *                 u8pReadBuffer;
    U8 *                 u8pWriteBuffer;
} PCMCIA_Map_Info_t;

typedef struct IO_SYS_MEM_BLOCK_s
{
    U8               u8BlockId;
    unsigned int  *u32Addr;
    unsigned int  *u32Size;
} IO_SYS_MEM_BLOCK_t;

#endif // __DRV_SYSTEM_ST_LG_H__
