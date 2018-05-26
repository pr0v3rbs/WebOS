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
 * field definition for ipc command.
 *
 * author
 * version    1.0
 * date
 * note
 *
 * @addtogroup
 * @{
 */

#ifndef _IPC_CMD_HEADER_H_
#define _IPC_CMD_HEADER_H_
/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*
 * field of ipc command for logical layer
 *
 *
 *  31        28       24       20        16        12       8       4       0
 *
 *   |   DST   |     FUNCTION    | MCU-ID  | RESERVE |          INDEX        |
 *
 *
 *
 * description
 *    DST      [31:28]
 *      : destination - DE / DENC / PQL / TOP / H3D / FRC / SRE / PE(DPPA)
 *                      DPPB / PWM / DEBUG
 *    FUNCTION [27:20]
 *      : function + chip rev.
 *
 *    MCU-ID   [19:16]
 *      : mcu_0, mcu_1, mcu_ext_0, mcu_ext_1
 *
 *    RESERVE  [15:12]
 *      : reserved field
 *
 *    INDEX    [11: 0]
 *      : command index
 *
*/


#define DST_OFFSET       28
#define FUNC_OFFSET      20
#define MCU_OFFSET       16
#define IDX_OFFSET        0

#define DST_MASK         0xF
#define FUNC_MASK        0xFF
#define MCU_MASK         0xF
#define IDX_MASK         0x3FF

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PACK_CMD(dst,func,mcu,idx)                    \
				(((dst & DST_MASK)   << DST_OFFSET)  |\
				 ((func & FUNC_MASK) << FUNC_OFFSET) |\
				 ((mcu & MCU_MASK)   << MCU_OFFSET)  |\
				 ((idx & IDX_MASK)   << IDX_OFFSET))

#define DE_PACK_CMD(cmd) PACK_CMD(IPC_DST_DE, 0, MCU_ID_0, cmd)
#define DENC_PACK_CMD(cmd) PACK_CMD(IPC_DST_DENC, 0, MCU_ID_0, cmd)
#define FMT3D_PACK_CMD(cmd) PACK_CMD(IPC_DST_FMT3D, 0, MCU_ID_0, cmd)
#define TOP_PACK_CMD(cmd) PACK_CMD(IPC_DST_TOP, 0, MCU_ID_1, cmd)
#define H3D_PACK_CMD(cmd) PACK_CMD(IPC_DST_H3D, 0, MCU_ID_1, cmd)
#define FRC_PACK_CMD(cmd) PACK_CMD(IPC_DST_FRC, 0, MCU_ID_1, cmd)
#define DPPB_PACK_CMD(cmd) PACK_CMD(IPC_DST_DPPB, 0, MCU_ID_1, cmd)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef enum {
	MCU_ID_0,
	MCU_ID_1,
	MCU_ID_2,
	MCU_ID_3,
	MCU_ID_EXT_0,
	MCU_ID_EXT_1,
	MCU_ID_MAX
} MCU_ID_TYPE;

typedef enum {
	IPC_DST_DE,
	IPC_DST_FMT3D   = 1, //  reserve for backward compatible
	IPC_DST_DENC    = 3,
	IPC_DST_PQL     = 5,
	IPC_DST_TOP,
	IPC_DST_H3D,
	IPC_DST_FRC,
	IPC_DST_SRE,
	IPC_DST_DPPA,
	IPC_DST_DPPB,
	IPC_DST_PWM,
	IPC_DST_DBG,

	IPC_DST_MAX
} IPC_DST_TYPE;



#endif //#ifndef _IPC_CMD_HEADER_H_
/**  @} */


