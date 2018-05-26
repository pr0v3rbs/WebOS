/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  register interface implementation for png device of h13. ( used only within kdriver )
 *	png device will teach you how to make device driver with new platform.
 *
 *  @author		Youngwoo Jin (youngwoo.jin@lge.com)
 *  @version	1.0
 *  @date		2011.04.21
 *
 *  @addtogroup h13
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define LOG_NAME	vdec_hal_vdo
#define LOG_LEVEL	log_level_error

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/io.h>
#include "vdec_base.h"

#include "vdo_reg.h"
#include "vdogdi_reg.h"
#include "hal/de_vdo_hal_api.h"
#include "hal/top_hal_api.h"
#include "vcp_reg.h"

#include "log.h"
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PACKED_MODE		1	//10 bit packed mode -> 1   unpacked mode -> 0
#define SMART_BALANCING 	1
#define VDO_PORT_NUM		(3)


//D14 Soc MAP
#define BASE_ADDR_SROM0          0x00000000 //MCU0 binary
#define BASE_ADDR_SRAM0         (BASE_ADDR_SROM0+SROM0_SIZE) //MCU0 code
#define BASE_ADDR_SROM1         (BASE_ADDR_SRAM0+SRAM0_SIZE) //MCU1 binary  //0x00180000 -->0x50000000
#define BASE_ADDR_SRAM1         (BASE_ADDR_SROM1+SROM1_SIZE) //MCU1 code    //0x00200000 -->0x60000000

#define BASE_ADDR_CPB_BUFF      (BASE_ADDR_SRAM1+SRAM1_SIZE)                //0x01200000
#define BASE_ADDR_AUB_BUFF      (BASE_ADDR_CPB_BUFF+CPB_SIZE) //CPB NAL
#define BASE_ADDR_PIC_BUFF	    (BASE_ADDR_AUB_BUFF+AUB_SIZE)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	TILED_MAP_LINEAR  = 0,
	TILED_MAP_FRAME_V = 1,
	TILED_MAP_MIXED_V = 2,
	TILED_MAP_FRAME_V_HEVC = 3,
	TILED_MAP_GMAU_HEVC = 4,
	TILED_MAP_MAX
} TILED_MAP_T;

typedef struct {
	UINT32		ui32Data[0x41];
} TILED_MAP_CONFIG_T;

typedef struct
{
	BOOLEAN			bAutoVsync;
	UINT32			ui32CurDpbMapType;
	UINT32			ui32CurDpbTiledBase;
	UINT32			ui32CurDpbStride;
	UINT32			ui32CurFramerateRes;
	UINT32 			ui32CurFramerateDiv;
	UINT32 			ui32FrameSize;
	UINT32 			ui32V_Offset;
	UINT32			ui32H_Offset;
} VDO_CH_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void _SetTiledMapType(UINT8 ui8VdoCh, UINT32 u32FrameBufStartAddr, TILED_MAP_T mapType, UINT32 ui32Stride);
static void _SetGmauReg(UINT8 ui8VdoCh, UINT8 udDom, UINT8 dualChMode, UINT8 bitDepthYminus8, UINT8 bitDepthCminus8);
static void _SetPicSizeReg(UINT8 ui8VdoCh, UINT32 H_Offset, UINT32 V_Offset, UINT32 Size);
static void _SetPicAddrtoGmauReg(UINT8 ui8VdoCh, UINT32 addrY8, UINT32 addrC8, UINT32 addrY2, UINT32 addrC2);
static void initGmau(void);
void DE_VDO_HAL_SetSeamless(UINT8 ui8SeamlessMode);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static volatile UINT32 stpVdo1Reg = 0;//NULL;
static volatile UINT32 stpVdo2Reg = 0;//NULL;
static volatile UINT32 stpVdoGdiReg = 0;//NULL;
static volatile UINT32 stpTopRcReg = 0;//NULL;
static volatile UINT32 stpVdoSwitchReg = 0;
static UINT8 ui8SeamlessModeValue = 0;
static UINT8	ui8VdoNoRunCnt[2] = {0,0};

static VDO_CH_T	stVdoCh[DE_VDO_NUM_OF_CHANNEL];

static unsigned char update_channel[DE_VDO_NUM_OF_CHANNEL];
static unsigned int tiled[DE_IF_NUM_OF_CHANNEL];
#if defined(CHIP_NAME_h13) && (CHIP_REV<0xb0)
#define bTiedChannel	TRUE
#else
#define bTiedChannel	FALSE
#endif
const static TILED_MAP_CONFIG_T	_gastTiledMapConfig[TILED_MAP_MAX][4] =
{
	{	// TILED_MAP_LINEAR
		{{	// > 2048
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,	/*xy2axiLumMap*/
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,	/*xy2axiChrMap*/
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000000																									/*xy2axiConfig*/
		}},
		{{	// <= 2048
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000000
		}},
		{{	// <= 1024
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000000
		}},
		{{	// <= 512
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000000
		}}
	},
	{	// TILED_FRAME_V_MAP
		{{	// > 2048
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000009,
			0x0000000A, 0x0000000B, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000009,
			0x0000000A, 0x0000000B, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x0003F0F0
		}},
		{{	// <= 2048
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000009,
			0x0000000A, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000009,
			0x0000000A, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x0003F0F0
		}},
		{{	// <= 1024
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000009,
			0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000009,
			0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x0003F0F0
		}},
		{{	// <= 512
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000016,
			0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000016,
			0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x0003F0F0
		}}
	},
	{	// TILED_MIXED_V_MAP
		{{	// > 2048
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000009,
			0x0000000A, 0x0000000B, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000009,
			0x0000000A, 0x0000000B, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00077777
		}},
		{{	// <= 2048
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000009,
			0x0000000A, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000009,
			0x0000000A, 0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00077777
		}},
		{{	// <= 1024
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000009,
			0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000009,
			0x00000016, 0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00077777
		}},
		{{	// <= 512
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000014, 0x00000007, 0x00000015, 0x00000008, 0x00000016,
			0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000011, 0x00000012, 0x00000013, 0x00000010, 0x00000003,
			0x00000004, 0x00000005, 0x00000006, 0x00000015, 0x00000087, 0x00000094, 0x00000008, 0x00000016,
			0x00000017, 0x00000018, 0x00000019, 0x0000001A, 0x0000001B, 0x00000040, 0x00000040, 0x00000040,
			0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,
			0x00077777
		}}
	},
	{	// TILED_MAP_FRAME_V For HEVC
		{{	// > 2048
			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x0000000b, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x0000000b, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x0003f0f0
		}},
		{{	// <= 2048
			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x00000040,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x00000040,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x0003f0f0
		}},
		{{	// <= 2048
			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x00000040,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x00000040,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x0003f0f0
		}},
		{{	// <= 2048
			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x00000040,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x00000040, 0x00000040, 0x00000040, 0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014,
			 0x00000015, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007, 0x00000016, 0x00000008,
			 0x00000009, 0x0000000a, 0x00000017, 0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x00000040,
			 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040, 0x00000040,

			 0x0003f0f0
		}},
	}
};


/*========================================================================================
	Implementation Group
========================================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetPicSize(UINT8 ui8VdoCh, UINT32 ui32PicHeight, UINT32 ui32PicWidth)
{
	UINT32		ui32PicSize = 0;

	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	ui32PicSize = ((ui32PicWidth & 0xFFF) << 16) | (ui32PicHeight & 0xFFF);
	VDO_CH_PIC_SIZE(ui8VdoCh) = ui32PicSize;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetBufIdx(UINT8 ui8VdoCh, UINT32 ui32BufIdx)
{
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	VDO_CH_BUF_IDX(ui8VdoCh) = (ui32BufIdx & 0x1F);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetChromaFormat(UINT8 ui8VdoCh)
{
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	//VDO_CH_CHROMA_FORMAT(ui8VdoCh) = ;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetScanMode(UINT8 ui8VdoCh, UINT32 ui32ScanModeType)
{
	UINT32		ui32ScanMode = 0;

	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}
	if( (ui32ScanModeType > 3) || (ui32ScanModeType == 0) )
	{
		log_error("[DE_VDO%d][Err] Scan Mode: %d, %s\n", ui8VdoCh, ui32ScanModeType, __FUNCTION__ );
		return;
	}

	ui32ScanMode = VDO_CH_SCAN_MODE(ui8VdoCh);

/*
	scan mode (4th bit)
	0: interlaced scan mode
	1: progressive scan mode
*/

	if( ui32ScanModeType == 3 ) // progressive
	{
		ui32ScanMode |= (1 << 4);
	}
	else if( ui8VdoCh < 2 ) // interlaced
	{
		ui32ScanMode &= ~(1 << 4);
		if( ui32ScanModeType == 1 ) // Top Field First
			ui32ScanMode &= ~(1 << 0);
		else // Bottom Field First
			ui32ScanMode |= (1 << 0);
	}

	VDO_CH_SCAN_MODE(ui8VdoCh) = ui32ScanMode;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_EnableMifWaitReqb(UINT8 ui8VdoCh, UINT8 ui32ReqbWaitCycle)
{
	VDO_CH_WAIT_REQB(ui8VdoCh) = ((1 << 28) | (ui32ReqbWaitCycle & 0xFF));
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_DisableMifWaitReqb(UINT8 ui8VdoCh)
{
	VDO_CH_WAIT_REQB(ui8VdoCh) &= ~(1 << 28);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetLineWait(UINT8 ui8VdoCh, UINT32 ui32LineWaitCycle)
{
	UINT32 ui32VdoConf;
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	ui32VdoConf = VDO_CH_VDO_CONF(ui8VdoCh) & 0xFFFF;
	ui32VdoConf |= ((ui32LineWaitCycle & 0xFFFF) << 16 );

	VDO_CH_VDO_CONF(ui8VdoCh) = ui32VdoConf;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVSyncSel(UINT8 ui8VdoCh, UINT32 ui32VSyncSrc)
{
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVSyncMode(UINT8 ui8VdoCh, BOOLEAN bAutoGen)
{
	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

/*
	vsync mode (3~0 bits)
	0: MCU generation using VSYNC_GEN register
	1: Automatic generation using VBP_NUM and VSYNC_NUM registers
*/
	if( bAutoGen == TRUE )
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 8);
	else
		VDO_CH_VSYNC_CMD(ui8VdoCh) &= ~(1 << 8);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVActvMode(UINT8 ui8VdoCh, BOOLEAN bAutoGen)
{
	if( ui8VdoCh >= (DE_VDO_NUM_OF_CHANNEL * 2) )
	{
		log_error("[DE_VDO][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

/*
	vactive mode (7~4 bits)
	0: MCU generation using VACTV_GEN register
	1: Automatic generation using VBP_NUM and VSYNC_NUM and VFP registers

	vactive automatic generation when chx_vactv_mode set to '0' (11~8 bits)
	0: MCU should set VACTV_GEN register after ch_vsync_rdy interrupt occur
	1: Automatic generation of vactive after receiption of ch_vsync_rdy from DE
*/
	if( bAutoGen == TRUE )
	{
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 9);
		VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 10);
	}
	else
	{
		VDO_CH_VSYNC_CMD(ui8VdoCh) &= ~(1 << 9);
		VDO_CH_VSYNC_CMD(ui8VdoCh) &= ~(1 << 10);
	}
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVSyncGen(UINT8 ui8VdoCh)
{
/*
	vsync generation
	It is automatically cleared when set to '1'
*/
	VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 0);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_SetVActvGen(UINT8 ui8VdoCh)
{
/*
	vsync generation
	It is automatically cleared when set to '1'
*/
	VDO_CH_VSYNC_CMD(ui8VdoCh) |= (1 << 4);
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_Init(void)
{

	stpVdo1Reg			= (UINT32)ioremap(VDEC_VDO1_BASE, 0x100);
	stpVdo2Reg			= (UINT32)ioremap(VDEC_VDO2_BASE, 0x100);
	stpVdoGdiReg		= (UINT32)ioremap(VDEC_VDO_GDI_BASE, 0x200);
	stpTopRcReg	= (UINT32)ioremap(VDEC_TOP_RC_BASE, 0x100);
	stpVdoSwitchReg		= (UINT32)ioremap(VDEC_VDO_ADDR_SWITCH, 0x1000);

	log_noti("VDO Init\n");

	VCP_Init();

	VDO_CH_VDO_CONF(0) = 0x00408000;
	VDO_CH_VDO_CONF(1) = 0x00408000;

	//== for HEVC, gdi path setting once
	initGmau();

	stVdoCh[0].bAutoVsync = TRUE;
	stVdoCh[1].bAutoVsync = TRUE;

	stVdoCh[0].ui32CurDpbMapType = 0xFF;
	stVdoCh[1].ui32CurDpbMapType = 0xFF;
	stVdoCh[0].ui32CurDpbTiledBase = 0xFFFFFFFF;
	stVdoCh[1].ui32CurDpbTiledBase = 0xFFFFFFFF;
	stVdoCh[0].ui32CurDpbStride = 0xFFFFFFFF;
	stVdoCh[1].ui32CurDpbStride = 0xFFFFFFFF;
	stVdoCh[0].ui32CurFramerateDiv = 0xFFFFFFFF;
	stVdoCh[1].ui32CurFramerateDiv = 0xFFFFFFFF;
	stVdoCh[0].ui32CurFramerateRes = 0xFFFFFFFF;
	stVdoCh[1].ui32CurFramerateRes = 0xFFFFFFFF;
	stVdoCh[0].ui32FrameSize = 0xFFFFFFFF;
	stVdoCh[1].ui32FrameSize = 0xFFFFFFFF;
	stVdoCh[0].ui32V_Offset = 0xFFFFFFFF;
	stVdoCh[1].ui32V_Offset = 0xFFFFFFFF;

}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_Reset(void)
{

	log_error("VDO Reset\n");

	VDO_CH_VDO_CONF(0) = 0x00408000;
	VDO_CH_VDO_CONF(1) = 0x00408000;

	//== for HEVC, gdi path setting once
	initGmau();

	stVdoCh[0].bAutoVsync = TRUE;
	stVdoCh[1].bAutoVsync = TRUE;
	stVdoCh[0].ui32CurDpbMapType = 0xFF;
	stVdoCh[1].ui32CurDpbMapType = 0xFF;
	stVdoCh[0].ui32CurDpbTiledBase = 0xFFFFFFFF;
	stVdoCh[1].ui32CurDpbTiledBase = 0xFFFFFFFF;
	stVdoCh[0].ui32CurDpbStride = 0xFFFFFFFF;
	stVdoCh[1].ui32CurDpbStride = 0xFFFFFFFF;
	stVdoCh[0].ui32CurFramerateDiv = 0xFFFFFFFF;
	stVdoCh[1].ui32CurFramerateDiv = 0xFFFFFFFF;
	stVdoCh[0].ui32CurFramerateRes = 0xFFFFFFFF;
	stVdoCh[1].ui32CurFramerateRes = 0xFFFFFFFF;
	stVdoCh[0].ui32FrameSize = 0xFFFFFFFF;
	stVdoCh[1].ui32FrameSize = 0xFFFFFFFF;
	stVdoCh[0].ui32V_Offset = 0xFFFFFFFF;
	stVdoCh[1].ui32V_Offset = 0xFFFFFFFF;

}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	DE_VDO_HAL_InitFrameBuffer(UINT8 ui8VdoCh)
{
	if( ui8VdoCh >= DE_IF_NUM_OF_CHANNEL )
	{
		log_error("[VDEC][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

}
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void	DE_VDO_HAL_SetVsiInfo(UINT8 ui8VdoCh, UINT32 ui32PicWidth, UINT32 ui32PicHeight, UINT32 ui32H_Offset, UINT32 ui32V_Offset)
{
	UINT32 ui32CropW;
	UINT32 ui32CropH;
	UINT32 ui32HOffset;
	UINT32 ui32VOffset;

	ui32CropW = (ui32PicWidth -( (ui32H_Offset >>16)&0xFFFF) ) - (ui32H_Offset&0xFFFF);
	ui32CropH = (ui32PicHeight & 0xFFFF) - ((ui32V_Offset>>16) & 0xFFFF) - (ui32V_Offset & 0xFFFF);

	HDMI_FC_VSDPAYLOAD5(ui8VdoCh) = ((ui32CropW&0xFFFF)>>8);
	HDMI_FC_VSDPAYLOAD6(ui8VdoCh) = (ui32CropW&0xFF);
	HDMI_FC_VSDPAYLOAD7(ui8VdoCh) = ((ui32CropH&0xFFFF)>>8);
	HDMI_FC_VSDPAYLOAD8(ui8VdoCh) = (ui32CropH&0xFF);
/*
	HDMI_FC_AVIETB0(ui8VdoCh) = ((ui32CropW&0xFFFF)>>8); //upper
	HDMI_FC_AVIETB1(ui8VdoCh) = (ui32CropW&0xFF); //lower
	HDMI_FC_AVISBB0(ui8VdoCh) = ((ui32CropH&0xFFFF)>>8); //upper
	HDMI_FC_AVISBB1(ui8VdoCh) = (ui32CropH&0xFF); //lower
*/

	if (ui8SeamlessModeValue == 3)
	{
		if ( (ui32CropW * 9) < (ui32CropH<<4) ) 
		{
			ui32HOffset = ((ui32CropH<<4) - (ui32CropW*9)) / 9;   //((inHeight*16) - (inWidth*9)) / 9
			ui32CropW += ui32HOffset;
		}
		else 
		{
			ui32VOffset = ((ui32CropW*9) - (ui32CropH<<4)) >> 4 ;          //((inWidth*9) - (inHeight*16)) / 16
			ui32CropH += ui32VOffset;
		}
	}

	HDMI_FC_AVIETB0(ui8VdoCh) = (ui32CropW&0xFF); //lower
	HDMI_FC_AVIETB1(ui8VdoCh) = ((ui32CropW&0xFFFF)>>8); //upper
	HDMI_FC_AVISBB0(ui8VdoCh) = (ui32CropH&0xFF); //lower
	HDMI_FC_AVISBB1(ui8VdoCh) = ((ui32CropH&0xFFFF)>>8); //upper 

	HDMI_FC_VSDPAYLOAD1(ui8VdoCh) = (ui32CropW&0xFF);
	log_user1("Set VSI info for U14 : W %x, H %x\n", ui32CropW, ui32CropH);

}
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_Update(UINT8 ui8VdoCh, S_DE_IPC_T *pDisBuf, UINT32 ui32SyncField)
{
	UINT32 ui32Size; //H.264 pic size
	UINT32 u32Temp;
	UINT8   udDomain;
	UINT8   dualChMod;
	UINT8 DepthYminus8;
	UINT8 DepthCminus8;
	UINT32 u32FrameSize; //HEVC pic size
	UINT32 u32VdoStatus;
	BOOLEAN is_high_frq;

	//vcp register resolution size setting
	UINT32 ui32CropW;
	UINT32 ui32CropH;
	UINT32 ui32TestSize;
	UINT32 ui32DispOutWidth = 1920;
	UINT32 ui32DispOutHeight = 1080;
	UINT32 ui32PicSizeChroma;
	UINT32 ui32scalePicSizeChroma;
  UINT32 ui32ResMultiVal;
	UINT32 ui32HVOffset;
	
	log_user1("VdoCh %d: Map %d Y 0x%X  C 0x%X BY 0x%X  "
			"BC 0x%X Scan %d, Field %d fr %d/%d size %dx%d\n",
			ui8VdoCh, pDisBuf->ui32DpbMapType,
			pDisBuf->ui32Y_FrameBaseAddr, pDisBuf->ui32C_FrameBaseAddr,
			pDisBuf->ui32Y_FrameBaseAddrBot, pDisBuf->ui32C_FrameBaseAddrBot,
			pDisBuf->ui32DisplayMode, ui32SyncField, pDisBuf->ui16FrameRateRes,
			pDisBuf->ui16FrameRateDiv, pDisBuf->ui16PicWidth,pDisBuf->ui16PicHeight);

	if( ui8VdoCh >= DE_IF_NUM_OF_CHANNEL )
	{
		log_error("[VDEC][Err] Channel Number(%d), %s\n", ui8VdoCh, __FUNCTION__ );
		return;
	}

	u32FrameSize = ((pDisBuf->ui16PicWidth<<16)|(pDisBuf->ui16PicHeight & 0xFFFF));

	if(pDisBuf->ui16FrameRateDiv != 0 &&
			pDisBuf->ui16FrameRateRes/pDisBuf->ui16FrameRateDiv > 30)
		is_high_frq = TRUE;
	else
		is_high_frq = FALSE;

	//1. udDomain setting
	if(pDisBuf->ui16PicWidth > 2048 || ui8SeamlessModeValue == 1 
									|| ui8SeamlessModeValue == 2 || ui8SeamlessModeValue == 3)
		udDomain = 1;
	else
		udDomain = 0;

	// Init for first received frame
	if( (stVdoCh[ui8VdoCh].ui32CurDpbMapType != pDisBuf->ui32DpbMapType) ||
		(stVdoCh[ui8VdoCh].ui32CurDpbTiledBase != pDisBuf->ui32Tiled_FrameBaseAddr) ||
		(stVdoCh[ui8VdoCh].ui32CurDpbStride != pDisBuf->ui32Stride) ||
		(pDisBuf->ui16FrameRateRes != stVdoCh[ui8VdoCh].ui32CurFramerateRes) ||
		(pDisBuf->ui16FrameRateDiv != stVdoCh[ui8VdoCh].ui32CurFramerateDiv) ||
		(stVdoCh[ui8VdoCh].ui32FrameSize != u32FrameSize) ||
		(stVdoCh[ui8VdoCh].ui32V_Offset != pDisBuf->ui32V_Offset))
	{
		log_noti("VDO:%d: set dpb map type %d->%d stride %d V_offset %x -> %x\n", ui8VdoCh,
				stVdoCh[ui8VdoCh].ui32CurDpbMapType, pDisBuf->ui32DpbMapType,
				pDisBuf->ui32Stride,stVdoCh[ui8VdoCh].ui32V_Offset,pDisBuf->ui32V_Offset);

		stVdoCh[ui8VdoCh].ui32CurDpbMapType = pDisBuf->ui32DpbMapType;
		stVdoCh[ui8VdoCh].ui32CurDpbTiledBase = pDisBuf->ui32Tiled_FrameBaseAddr;
		stVdoCh[ui8VdoCh].ui32CurDpbStride = pDisBuf->ui32Stride;
		stVdoCh[ui8VdoCh].ui32CurFramerateRes = pDisBuf->ui16FrameRateRes;
		stVdoCh[ui8VdoCh].ui32CurFramerateDiv= pDisBuf->ui16FrameRateDiv;
		stVdoCh[ui8VdoCh].ui32FrameSize = u32FrameSize;
		stVdoCh[ui8VdoCh].ui32V_Offset = pDisBuf->ui32V_Offset;

		switch(pDisBuf->ui32DpbMapType)
		{
			case 1: // Frame Mmap
				_SetTiledMapType(ui8VdoCh, pDisBuf->ui32Tiled_FrameBaseAddr,
						TILED_MAP_FRAME_V, pDisBuf->ui32Stride);
				break;
			case 2: // Mixed Map
				_SetTiledMapType(ui8VdoCh, pDisBuf->ui32Tiled_FrameBaseAddr,
						TILED_MAP_MIXED_V, pDisBuf->ui32Stride);
				break;
			case 3: // 8bit HEVC
				//2. dualChMod setting
				if( is_high_frq && (udDomain == 1) )
				{
					dualChMod = 1;
					//In case of BBC seamlessmode FHD, single channel setting
					if( (pDisBuf->ui16PicWidth <= 2048) && (ui8SeamlessModeValue == 2) )
						dualChMod = 0;
				}
				else
					dualChMod = 0;

				//3. depth info -> DepthYminus8, DepthCminus8 set like this :
				//		8bit -> 0, 9bit -> 1, 10 bit -> 2
				DepthYminus8 = 0; //8 bit packed mode
				DepthCminus8 = 0;//8 bit packed mode

				_SetGmauReg(ui8VdoCh, udDomain, dualChMod, DepthYminus8, DepthCminus8);
				_SetPicSizeReg(ui8VdoCh, pDisBuf->ui32H_Offset, pDisBuf->ui32V_Offset, u32FrameSize);
				break;

			case 4:	//10 bit HEVC
				//2. dualChMod setting
				if( is_high_frq && (udDomain == 1) )
				{
					dualChMod = 1;
					//In case of BBC seamlessmode FHD, single channel setting
					if( (pDisBuf->ui16PicWidth <= 2048) && (ui8SeamlessModeValue == 2) )
						dualChMod = 0;
				}
				else
					dualChMod = 0;

				//3. depth info -> DepthYminus8, DepthCminus8 set like this :
				//		8bit -> 0, 9bit -> 1, 10 bit -> 2
				DepthYminus8 = 2; //10 bit
				DepthCminus8 = 2;//10 bit

				_SetGmauReg(ui8VdoCh, udDomain, dualChMod, DepthYminus8, DepthCminus8);
				_SetPicSizeReg(ui8VdoCh, pDisBuf->ui32H_Offset, pDisBuf->ui32V_Offset, u32FrameSize);
				break;

			case 0:	// Linear
				break;

			default :
				break;
		}

		// vcp register setting (resolution size,hdmi register)
		if(ui8SeamlessModeValue == 1) // 1. Netflix Mode
		{
			ui32CropW = (pDisBuf->ui16PicWidth -( (pDisBuf->ui32H_Offset >>16)&0xFFFF) ) - (pDisBuf->ui32H_Offset&0xFFFF);
			ui32CropH = (pDisBuf->ui16PicHeight & 0xFFFF) - (pDisBuf->ui32V_Offset & 0xFFFF);
			ui32TestSize = (ui32CropW << 16) | (ui32CropH & 0xFFFF);

			if(ui8VdoCh  == 0)
			{
				ui32PicSizeChroma = (((ui32CropW>>1)<<16)|(ui32CropH>>1));
				ui32scalePicSizeChroma = (((ui32CropW>>1)<<16)| ui32CropH);

				VCP_CH_C_SCALER_CTRL1(0) = ui32PicSizeChroma;
				VCP_CH_C_SCALER_CTRL2(0) = ui32scalePicSizeChroma;

				VCP_CH_CHAMP_CTRL0(0) = 0x00000010;
				VCP_CH_CHAMP_CTRL2(0) = ui32TestSize;

				VCP_CH_CCV_CTRL1(0) = ui32TestSize;
				VCP_CH_FVS_CTRL5(0) = ui32TestSize;

				VCP_CH_DESHP_CTRL2(0) = ui32TestSize;
				VCP_CH_FVS_CTRL1(0) = (ui32DispOutWidth << 16) | ui32DispOutHeight;

				VCP_CH_FVS_CTRL3(0) = (((((ui32CropW + ui32CropH)>>4)&0x0F)|0xF0) << 22)
					     | (((((ui32CropW + ui32CropH)>>4)&0xF0)|0xF00) << 8) | (((((ui32CropW + ui32CropH)>>4)&0x0F)|0xF0) << 2);
/*
				VCP_CH_FVS_CTRL3(0) = ((((((ui32CropW*5) + (ui32CropH<<1))/46)&0x0F)|0xF0) << 22)
							| ((((((ui32CropW*5) + (ui32CropH<<1))/46)&0xF0)|0xF00) << 8) | ((((((ui32CropW*5) + (ui32CropH<<1))/46)&0x0F)|0xF0) << 2);
*/
			}
		}
		else if(ui8SeamlessModeValue == 2) // 2. BBC Mode
		{
			// VCP size setting for BBC mode
				ui32CropW = (pDisBuf->ui16PicWidth -( (pDisBuf->ui32H_Offset >>16)&0xFFFF) ) - (pDisBuf->ui32H_Offset&0xFFFF);
				ui32CropH = (pDisBuf->ui16PicHeight & 0xFFFF) - (pDisBuf->ui32V_Offset & 0xFFFF);
				if (ui32CropW <= 1920)
				{
								ui32TestSize = (ui32CropW << 16) | (ui32CropH & 0xFFFF);
								ui32PicSizeChroma = (((ui32CropW>>1)<<16)|(ui32CropH>>1));
								ui32scalePicSizeChroma = (((ui32CropW>>1)<<16)| ui32CropH);
								VCP_CH_C_SCALER_CTRL1(0) = ui32PicSizeChroma;   //0xF0062054
								VCP_CH_C_SCALER_CTRL1(1) = ui32CropH>>1;    //0xF0062254 (<= 0xF0062054 + 0x200)
								VCP_CH_C_SCALER_CTRL2(0) = ui32scalePicSizeChroma;
								VCP_CH_C_SCALER_CTRL2(1) = ui32CropH;
								VCP_CH_CHAMP_CTRL2(0) = ui32TestSize;
								VCP_CH_CHAMP_CTRL2(1) = ui32CropH;
								VCP_CH_CCV_CTRL1(0) = ui32TestSize;
								VCP_CH_CCV_CTRL1(1) = ui32CropH;
								VCP_CH_FVS_CTRL5(0) = ( ui32CropW<<16 ) | ( ui32CropH<<1 ); //H_in  x V_in * 2;
								VCP_CH_FVS_CTRL5(1) = ui32CropH;
								VCP_CH_DESHP_CTRL2(0) = ( ui32CropW<<16 ) | ( ui32CropH<<1 ); //H_in  x V_in * 2;
								VCP_CH_DESHP_CTRL2(1) = ui32CropH;
								VCP_CH_FVS_CTRL1(0) = (ui32DispOutWidth << 16) | (ui32DispOutHeight<<1);
								VCP_CH_FVS_CTRL1(1) = (ui32DispOutWidth << 16) | (ui32DispOutHeight<<1);
								VCP_CH_FVS_CTRL0(0) = VCP_CH_FVS_CTRL0(0) | 0x00000110;
				}
				else
				{
								ui32TestSize = ((ui32CropW>>1) << 16) | (ui32CropH & 0xFFFF);
								ui32PicSizeChroma = (((ui32CropW>>2)<<16)|(ui32CropH>>1));
								ui32scalePicSizeChroma = (((ui32CropW>>2)<<16)| ui32CropH);
								VCP_CH_C_SCALER_CTRL1(0) = ui32PicSizeChroma;   //0xF0062054
								VCP_CH_C_SCALER_CTRL1(1) = ui32PicSizeChroma;   //0xF0062254
								VCP_CH_C_SCALER_CTRL2(0) = ui32scalePicSizeChroma;
								VCP_CH_C_SCALER_CTRL2(1) = ui32scalePicSizeChroma;
								VCP_CH_CHAMP_CTRL2(0) = ui32TestSize;
								VCP_CH_CHAMP_CTRL2(1) = ui32TestSize;
								VCP_CH_CCV_CTRL1(0) = ui32TestSize;
								VCP_CH_CCV_CTRL1(1) = ui32TestSize;
								VCP_CH_FVS_CTRL5(0) = ui32TestSize;
								VCP_CH_FVS_CTRL5(1) = ui32TestSize;
								VCP_CH_DESHP_CTRL2(0) = ui32TestSize;
								VCP_CH_DESHP_CTRL2(1) = ui32TestSize;
								VCP_CH_FVS_CTRL1(0) = (ui32DispOutWidth << 16) | (ui32DispOutHeight<<1);
								VCP_CH_FVS_CTRL1(1) = (ui32DispOutWidth << 16) | (ui32DispOutHeight<<1);
								VCP_CH_FVS_CTRL0(0) = VCP_CH_FVS_CTRL0(0) & 0xFFFFFEEF;
				}
		}
		else if(ui8SeamlessModeValue == 3) // 3. Amazon Mode
		{
						// VCP size setting for Amazon mode
						ui32CropW = (pDisBuf->ui16PicWidth -( (pDisBuf->ui32H_Offset >>16)&0xFFFF) ) - (pDisBuf->ui32H_Offset&0xFFFF);
						ui32CropH = (pDisBuf->ui16PicHeight & 0xFFFF) - (pDisBuf->ui32V_Offset & 0xFFFF);

						ui32TestSize = (ui32CropW << 16) | (ui32CropH & 0xFFFF);
						ui32PicSizeChroma = (((ui32CropW>>1)<<16)|(ui32CropH>>1));
						ui32scalePicSizeChroma = (((ui32CropW>>1)<<16)| ui32CropH);
						VCP_CH_C_SCALER_CTRL1(0) = ui32PicSizeChroma;   //0xF0062054
						VCP_CH_C_SCALER_CTRL1(1) = ui32PicSizeChroma;   //0xF0062254
						VCP_CH_C_SCALER_CTRL2(0) = ui32scalePicSizeChroma;
						VCP_CH_C_SCALER_CTRL2(1) = ui32scalePicSizeChroma;
						VCP_CH_CHAMP_CTRL2(0) = ui32TestSize;
						VCP_CH_CHAMP_CTRL2(1) = ui32TestSize;
						VCP_CH_CCV_CTRL1(0) = ui32TestSize;
						VCP_CH_CCV_CTRL1(1) = ui32TestSize;
						VCP_CH_FVS_CTRL5(0) = ui32TestSize;
						VCP_CH_FVS_CTRL5(1) = ui32TestSize;
						VCP_CH_DESHP_CTRL2(0) = ui32TestSize;
						VCP_CH_DESHP_CTRL2(1) = ui32TestSize;
						VCP_CH_FVS_CTRL1(0) = 0x0f000870;
						VCP_CH_FVS_CTRL1(1) = 0x0f000870;
						VCP_CH_FVS_CTRL0(0) = VCP_CH_FVS_CTRL0(0) & 0xFFFFFEEF;

						if ( (ui32CropW * 9) < (ui32CropH<<4) )     // AR 4:3, 1.6667:1, Insert Left/Right black bar
						{
										ui32HVOffset = ((ui32CropH<<4) - (ui32CropW*9)) / 9;   //((inHeight*16) - (inWidth*9)) / 9
										VCP_CH_FVS_CTRL4(0) = (ui32HVOffset>>1)<<16;         // horizontal offset
						}
						else                 // AR 16:9 (No Black bar) + AR 1.849:1, 2.2:1, 2.35:1, 2.4:2, Insert Top/Bottom black bar
						{
										ui32HVOffset = ((ui32CropW*9) - (ui32CropH<<4)) >> 4 ;          //((inWidth*9) - (inHeight*16)) / 16
										VCP_CH_FVS_CTRL4(0) = ui32HVOffset >> 1;                          // vertical offset
						}
						VCP_CH_FVS_CTRL4(1) = 0x0;        // 0xF0062294, // Image start position, [28:16] x_pos, [12:0]  y_pos
						//AVI info frame setting for Amazon mode
						DE_VDO_HAL_SetVsiInfo(ui8VdoCh, pDisBuf->ui16PicWidth, pDisBuf->ui16PicHeight, pDisBuf->ui32H_Offset, pDisBuf->ui32V_Offset);
/*
						ui32ResMultiVal = ui32CropW * ui32CropH;

						if ( ui32ResMultiVal <= (1280*720) ) // under 1280x720
						{
										VCP_CH_FVS_CTRL4(0) = ((720 - ui32CropH)>>1) | (((1280 - ui32CropW)>>1)<<16); // 0xF0062094
						}
						else if ( ui32ResMultiVal <= (1920*1080) )  // under 1920x1080
						{
										VCP_CH_FVS_CTRL4(0) = ((1080 - ui32CropH)>>1) | (((1920 - ui32CropW)>>1)<<16); // 0xF0062094
						}
						else if ( ui32ResMultiVal <= (2560*1440) )  // under 2560x1440
						{
										VCP_CH_FVS_CTRL4(0) = ((1440 - ui32CropH)>>1) | (((2560 - ui32CropW)>>1)<<16); // 0xF0062094
						}
						else if ( ui32ResMultiVal <= (3840*2160) )  // under 3840x2160
						{
										VCP_CH_FVS_CTRL4(0) = ((2160 - ui32CropH)>>1) | (((3840 - ui32CropW)>>1)<<16); // 0xF0062094
						}
						else  // Others
						{
										VCP_CH_FVS_CTRL4(0) = 0x0;
						}
						VCP_CH_FVS_CTRL4(1) = 0x0; // 0xF0062294, // Image start position, [28:16] x_pos, [12:0]  y_pos
*/
		}
	}

	// VDO Status check for guarantee Previous vdo working
	// u32VdoStatus 0 : previous done, 1 : previous runnig
	u32VdoStatus = VDO_CH_VDO_CMD(ui8VdoCh);
	u32VdoStatus = u32VdoStatus & 0xF;
	log_user1("VDO Core %d VDO Status: %s\n", ui8VdoCh,
			(!!u32VdoStatus)?"Previous VDO still RUN -> Current VDO Ignore"
			:"Previous VDO DONE -> Current VDO RUN");

	//VCP
	if (ui8SeamlessModeValue == 1)
		VCP_CH_PIC_INIT(ui8VdoCh) = 0x10000011;		// 1. Vcp init(Semaless)
	else
	{
		VCP_CH_PIC_INIT(0) = 0x10000011;			// 1. Vcp init(Not Seamless)
		if(is_high_frq && udDomain == 1) //60p
			VCP_CH_PIC_INIT(1) = 0x10000011;		// 1. Vcp init(Not Seamless)
	}
	//VDO
	if(!u32VdoStatus)
		VDO_CH_VDO_CMD(ui8VdoCh) = 0x00000010;  	// 1. Vdo_init


	//VCP
	if (ui8SeamlessModeValue == 1)
		VCP_CH_PIC_INIT(ui8VdoCh) = 0x10000000;		// 2. Vcp init off(Seamless)
	else
	{
		VCP_CH_PIC_INIT(0) = 0x10000000;			// 2. Vcp init off(Not Seamless)
		if(is_high_frq && udDomain == 1) //60p
			VCP_CH_PIC_INIT(1) = 0x10000000;		// 2. Vcp init off(Not Seamless)
	}
	//VDO
	if(!u32VdoStatus)
		VDO_CH_VDO_CMD(ui8VdoCh) = 0x00000000;      // 2. Vdo_init off

	// Crop (Requested by DE)- In case H.264 (right, bottom)
	ui32Size = ((pDisBuf->ui16PicWidth - (pDisBuf->ui32H_Offset & 0xFFFF)) << 16) |
				((pDisBuf->ui16PicHeight - (pDisBuf->ui32V_Offset & 0xFFFF)) & 0xFFFF);

	if (pDisBuf->ui32DpbMapType == 0 || pDisBuf->ui32DpbMapType == 1 ||
			pDisBuf->ui32DpbMapType == 2 )
		VDO_CH_PIC_SIZE(ui8VdoCh)       = ui32Size;

	VDO_CH_BUF_IDX(ui8VdoCh)        = ui8VdoCh;//pDisBuf->ui32FrameIdx + (ui8VdoCh*10); /* Fixed 11 total frame each channel */
	VDO_CH_CHROMA_FORMAT(ui8VdoCh)  = 0x00040004;

    if((pDisBuf->ui32DpbMapType == 3)||(pDisBuf->ui32DpbMapType == 4))//HEVC path GMAU YCbCr base address setting
	{
		// when packed mode && smart balance
		_SetPicAddrtoGmauReg(ui8VdoCh,pDisBuf->ui32Y_FrameBaseAddr,pDisBuf->ui32C_FrameBaseAddr,0,0);
	}
	else //H.264 path gdi setting
	{
		VDO_GDI_PIC_INIT_HOST(ui8VdoCh)  = 0x1;
		VDO_GDI_PIC_INIT_HOST(ui8VdoCh)  = 0x0;

		//	ui32PreAPBVal = TOP_HAL_SetApbVDO(1, 2);

		// [21:20]TILED_MIXED_MAP, [19:17]color_format(4:2:0),
		//	[16]CbCr_Interleaved, [15:0]Stride_Size
		u32Temp = 0x00010000 | pDisBuf->ui32Stride;

		switch( pDisBuf->ui32DpbMapType )
		{
			case TILED_MAP_LINEAR:
				u32Temp |= 0x00000000;
				break;
			case TILED_MAP_FRAME_V_HEVC:
				u32Temp |= 0x00110000;
				break;
			case TILED_MAP_FRAME_V:
				u32Temp |= 0x00100000;
				break;
			case TILED_MAP_MIXED_V:
				u32Temp |= 0x00400000;
				break;
			default:
				log_error("invalid map type %d\n", pDisBuf->ui32DpbMapType);
				break;
		}
		VDO_GDI_INFO_CONTROL(0,ui8VdoCh) = u32Temp;

		//_gpstFrameBuf[ui8VdoCh]->ui32Stride;
		VDO_GDI_INFO_PIC_SIZE(0,ui8VdoCh) = ui32Size;
		{
			VDO_GDI_INFO_BASE_Y(0,ui8VdoCh)   = pDisBuf->ui32Y_FrameBaseAddr;
			VDO_GDI_INFO_BASE_CB(0,ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddr;
			VDO_GDI_INFO_BASE_CR(0,ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddr;

			VDO_GDI_INFO_BASE_Y_BOT(0,ui8VdoCh)   = pDisBuf->ui32Y_FrameBaseAddrBot;
			VDO_GDI_INFO_BASE_CB_BOT(0,ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddrBot;
			VDO_GDI_INFO_BASE_CR_BOT(0,ui8VdoCh)  = pDisBuf->ui32C_FrameBaseAddrBot;
		}
	}

	// Vcp Run Command
	if (ui8SeamlessModeValue == 1)
		VCP_CH_PIC_START(ui8VdoCh) = 0x00000011;	// 3. Vcp Run(Seamless)
	else
	{
		VCP_CH_PIC_START(0) = 0x00000011;			// 3. Vcp Run(Not Seamless)
		if(is_high_frq && udDomain == 1) //60p
			VCP_CH_PIC_START(1) = 0x00000011;		// 3. Vcp Run(Not Seamless)
	}
	// Vdo Run Command
	if(!u32VdoStatus)
		VDO_CH_VDO_CMD(ui8VdoCh) = 0x01;			// 3. Vdo Run

	// VDO Reset condition checkcode
	// Incase of using core0 (Not include seamless mode)
	if ((!!u32VdoStatus) && (ui8SeamlessModeValue != 1))
	{
		ui8VdoNoRunCnt[ui8VdoCh]++;
		if (ui8VdoNoRunCnt[ui8VdoCh] == 5)
		{
			TOP_RC_SW_RESET = 0x06000000; //vdo core 0/1 reset
			DE_VDO_HAL_Reset(); //vdo reset initialization
			ui8VdoNoRunCnt[ui8VdoCh] = 0;
		}
	}
	// VDO no run count continuity check !!
	if ((!u32VdoStatus) && (ui8SeamlessModeValue != 1))
	{
		ui8VdoNoRunCnt[ui8VdoCh] = 0;
	}

	return;
}

/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_InitCmd(void)
{
}

void	DE_VDO_HAL_Suspend(void)
{

}
void	DE_VDO_HAL_Resume(void)
{

}
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
void DE_VDO_HAL_RunCmd(UINT8 ui8DeIfCh[], UINT32 aui32DisplayInfo[], UINT32 ui32SyncField)
{
}

void DE_VDO_HAL_Register(UINT8 u8VdoCh, UINT8 u8DeIfCh, BOOLEAN bTiled)
{
	if( u8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("wrong vdo channel num %d\n", u8VdoCh);
		return;
	}

	log_noti("VDO Register %d deif %d tiled %d\n", u8VdoCh, u8DeIfCh, bTiled);

	update_channel[u8VdoCh] = u8DeIfCh;

	if( !bTiedChannel )
		tiled[u8VdoCh] = bTiled;
}

void	DE_VDO_HAL_SetInterlaced(UINT8 ui8VdoCh, BOOLEAN bInterlaced)
{
	if( ui8VdoCh >= DE_VDO_NUM_OF_CHANNEL )
	{
		log_error("wrong vdo channel num %d\n", ui8VdoCh);
		return;
	}

	if( bInterlaced == TRUE )
	{
		/* Interlaced */
		VDO_CH_SCAN_MODE(ui8VdoCh)  &= ~0x00000010;
	}
	else
	{
		/* Progressive */
		VDO_CH_SCAN_MODE(ui8VdoCh)  |= 0x00000010;
	}
}

/*========================================================================================
	Static Implementation Group
========================================================================================*/
/**
********************************************************************************
* @brief
*
* @remarks
*  DETAIL INFORMATION
* @par requirements:
*
* @param
*
* @return
*
********************************************************************************
*/
static void _SetTiledMapType(UINT8 ui8VdoCh, UINT32 u32FrameBufStartAddr, TILED_MAP_T mapType, UINT32 ui32Stride)
{
	const TILED_MAP_CONFIG_T*	pstMapCfg;
	volatile UINT32 *pVdoGdiReg;
	UINT32	i;

	if( mapType >= TILED_MAP_MAX )
	{
		log_error("wrong tiled map type %d\n", ui8VdoCh);
		goto GOTO_END;
	}

	VDO_CH_VDO_CONFIG(ui8VdoCh) &= ~0x7;

	pstMapCfg = &_gastTiledMapConfig[(UINT32)mapType][(ui32Stride > 2048)? 0 :
													  (ui32Stride > 1024)? 1 :
													  (ui32Stride > 512)? 2 : 3];
	pVdoGdiReg = &(VDO_GDI_CH0_XY2_AXI_LUM_00(ui8VdoCh));

	for( i=0; i<0x40; i++, pVdoGdiReg++ )
	{
		*pVdoGdiReg = pstMapCfg->ui32Data[i];
	}

	VDO_GDI_CH0_XY2_AXI_CONFIG(ui8VdoCh) = pstMapCfg->ui32Data[i];

GOTO_END:
	return;
}

static void _SetGmauReg(UINT8 ui8VdoCh, UINT8 udDom, UINT8 dualChMode, UINT8 bitDepthYminus8, UINT8 bitDepthCminus8)
{
	//==================================================================================
	//Luma
	VDO_CH_VDO_GMAU0_0(ui8VdoCh) = 0x00000000;
	//[15:12] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
#if PACKED_MODE
	VDO_CH_VDO_GMAU0_2(ui8VdoCh) = udDom ? (SMART_BALANCING ? 0x00009706 : 0x0000B706) : (SMART_BALANCING ? 0x00008706 : 0x0000A706);
#else
	VDO_CH_VDO_GMAU0_2(ui8VdoCh) = udDom ? 0x0000F706 : 0x0000E706;
#endif
	VDO_CH_VDO_GMAU0_3(ui8VdoCh) = 0x00000000;

	//----------------------------------------------------------------------------------
	VDO_CH_VDO_GMAU1_0(ui8VdoCh) = 0x00000000;
	//[15:12] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
	VDO_CH_VDO_GMAU1_2(ui8VdoCh) = udDom ? 0x0000F706 : 0x0000E706;
	VDO_CH_VDO_GMAU1_3(ui8VdoCh) = 0x00000000;
	//==================================================================================


	//==================================================================================
	//Chroma
	VDO_CH_VDO_GMAU2_0(ui8VdoCh) = 0x00000000;

	//cmd_mode_c8 [15:12] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
	//cmd_mode_y2 [19:16] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
	//cmd_mode_c2 [23:20] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map

#if PACKED_MODE
	VDO_CH_VDO_GMAU2_2(ui8VdoCh) = udDom? (SMART_BALANCING ? 0x00009706 : 0x0000B706) : (SMART_BALANCING ? 0x00008706 : 0x0000A706);
#else
	if( (bitDepthYminus8 > 0) || (bitDepthCminus8 > 0) )
		VDO_CH_VDO_GMAU2_2(ui8VdoCh) = udDom ? 0x00DDF706 : 0x00CCE706;
	else
		VDO_CH_VDO_GMAU2_2(ui8VdoCh) = udDom ? 0x0000F706 : 0x0000E706;
#endif

	VDO_CH_VDO_GMAU2_3(ui8VdoCh) = 0x00000000;

	//----------------------------------------------------------------------------------
	VDO_CH_VDO_GMAU3_0(ui8VdoCh) = 0x00000000;

	//cmd_mode_c8 [15:12] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
	//cmd_mode_y2 [19:16] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
	//cmd_mode_c2 [23:20] 0xF : UD 8bit map 0xE : FHD 8bit map 0xD : UD 2bit map 0xC : FHD 2bit map
	if( (bitDepthYminus8 > 0) || (bitDepthCminus8 > 0) )
		VDO_CH_VDO_GMAU3_2(ui8VdoCh) = udDom ? 0x00DDF706 : 0x00CCE706;
	else
		VDO_CH_VDO_GMAU3_2(ui8VdoCh) = udDom ? 0x0000F706 : 0x0000E706;
	VDO_CH_VDO_GMAU3_3(ui8VdoCh) = 0x00000000;
	//==================================================================================
	// [2:0] sel_ch_path
	// b000: 1ch, H.264&D13 compatible
	// b100: 1ch, HEVC 30p (8bit/2bit map)
	// b101: 2ch, HEVC 60p (8bit/2bit map)
	// b110: 1ch, HEVC 30p (10bit packed map)
	// b111: 2ch, HEVC 60p (10bit packed map)

	// 4 ; 5; 6; 7

	// [3:3] sel_bits
	// 0: 8-bit 1: 10-bit

	//frameRateMode 0 - 30p frameRateMode 1 - 60p
	//fix me later : 4th bit default 1;

	if(dualChMode == 0 && !PACKED_MODE)
		VDO_CH_VDO_CONFIG(ui8VdoCh) = 0x00000004;
	else if( dualChMode == 1 && !PACKED_MODE)
		VDO_CH_VDO_CONFIG(ui8VdoCh) = 0x00000005;
	else if( dualChMode == 0 && PACKED_MODE)
		VDO_CH_VDO_CONFIG(ui8VdoCh) = 0x00000006;
	else //( frameRateMode == 1 && PACKED_MODE)
		VDO_CH_VDO_CONFIG(ui8VdoCh) = 0x00000007;

	//zero_position 2'b00 position @ 8bit 0:MSB, 1:LSB
	VDO_CH_VDO_CONFIG(ui8VdoCh) |= 0x00000010;

	//src_bit_width_y
	VDO_CH_VDO_CONFIG(ui8VdoCh) |= (bitDepthYminus8<<5);
	//src_bit_width_c
	VDO_CH_VDO_CONFIG(ui8VdoCh) |= (bitDepthCminus8<<7);

	return;
}

static void _SetPicSizeReg(UINT8 ui8VdoCh, UINT32 H_Offset, UINT32 V_Offset, UINT32 Size)
{

	UINT32 ui32CropWidth;
	UINT32 ui32CropHeight;
	UINT32 ui32RealWidthCh0;
	UINT32 ui32RealWidthCh1;
	UINT32 ui32XPosCh0;
	UINT32 ui32XPosCh1;

	//crop process
	ui32CropWidth = ( ( ( (Size>>16)&0xFFFF ) -( (H_Offset>>16)&0xFFFF) ) - (H_Offset&0xFFFF) );
//	ui32CropHeight = ( (Size & 0xFFFF) - ((V_Offset>>16)&0xFFFF) - (V_Offset & 0xFFFF) );
//	Only bottom crop set to VDO setting
	ui32CropHeight = ( (Size & 0xFFFF) - (V_Offset & 0xFFFF) );

	// when 8+2 map, crop size is registered
	//VDO_CH_PIC_SIZE(0) = ((ui32CropWidth << 16) | (ui32CropHeight & 0xFFFF));

	// when 10 bit packed map, original size is registered
	VDO_CH_PIC_SIZE(ui8VdoCh) = ((Size & 0xFFFF0000)|(ui32CropHeight & 0xFFFF));

	//[15:0]  start x position at ch0
	//[31:16] start x position at ch1
	//start position of right-hand on disp.
	if (ui32CropWidth&1) //when crop width size is odd
	{
		ui32RealWidthCh0 = ((ui32CropWidth/2+7)/8)*8;
		if (ui32CropWidth - ui32RealWidthCh0 > 0)
			ui32RealWidthCh1 = ui32CropWidth - ui32RealWidthCh0;
		else
			ui32RealWidthCh1 = 0;
	}
	else		//when crop width size is even
	{
		ui32RealWidthCh0 = ui32CropWidth/2;
		ui32RealWidthCh1 = ui32CropWidth/2;
	}
	VDO_CH_VDO_REAL_WIDTH(ui8VdoCh) = ( (ui32RealWidthCh1 << 16) | (ui32RealWidthCh0 & 0xFFFF) );

	ui32XPosCh0 = (H_Offset >> 16) & 0xFFFF;
	ui32XPosCh1 = ui32XPosCh0 + ui32RealWidthCh0;
	VDO_CH_VDO_START_XPOS(ui8VdoCh) = (ui32XPosCh1 << 16) | ui32XPosCh0;

	return;
}

static void _SetPicAddrtoGmauReg(UINT8 ui8VdoCh, UINT32 addrY8, UINT32 addrC8, UINT32 addrY2, UINT32 addrC2)
{
#if PACKED_MODE && !SMART_BALANCING
	VDO_CH_VDO_GMAU0_1_Y8(ui8VdoCh) = addrY8;
	VDO_CH_VDO_GMAU2_1_C8(ui8VdoCh) = addrY8;

	//#elif 0 //smart balancing //fix me later
	// VDO_GMAU0_1_Y8 = addrY8;
	// VDO_GMAU2_1_C8 = addrY8;
#elif PACKED_MODE && SMART_BALANCING
	VDO_CH_VDO_GMAU0_1_Y8(ui8VdoCh) = (addrY8>>1); //ddr0
	VDO_CH_VDO_GMAU2_1_C8(ui8VdoCh) = (0x80000000|(addrY8>>1)); //ddr1
#else
	VDO_CH_VDO_GMAU0_1_Y8(ui8VdoCh) = addrY8;
	VDO_CH_VDO_GMAU1_1_Y8(ui8VdoCh) = addrY8;

	VDO_CH_VDO_GMAU2_1_C8(ui8VdoCh) = addrC8;
	VDO_CH_VDO_GMAU3_1_C8(ui8VdoCh) = addrC8;

	VDO_CH_VDO_GMAU2_1_Y2(ui8VdoCh) = addrY2;
	VDO_CH_VDO_GMAU3_1_Y2(ui8VdoCh) = addrY2;

	VDO_CH_VDO_GMAU2_1_C2(ui8VdoCh) = addrC2;
	VDO_CH_VDO_GMAU3_1_C2(ui8VdoCh) = addrC2;
#endif

	return;
}

static void initGmau(void)
{
	UINT32 i;

	_SetTiledMapType(0, 0, TILED_MAP_FRAME_V_HEVC, 3840); //core 0
	_SetTiledMapType(1, 0, TILED_MAP_FRAME_V_HEVC, 3840); //core 1

	//FRAME_V/4:2:0/CbCrInterleaved/Stride=4096
	//core 0
	VDO_GDI_INFO_CONTROL(0,0) = 0x00111000;
	VDO_GDI_INFO_PIC_SIZE(0,0) = (3840<<16) | 2160;
	//core 1
	VDO_GDI_INFO_CONTROL(0,1) = 0x00111000;
	VDO_GDI_INFO_PIC_SIZE(0,1) = (3840<<16) | 2160;

	TOP_RC_TOPCONFIG = 0x000000A4;

	VDO_CH_VDO_ENDIAN(0)= 0x0FF00000;	//core 0
	VDO_CH_VDO_ENDIAN(1)= 0x0FF00000;   //core 1

	VDO_CH_VDO_10BPACKED_CONFIG(0) = SMART_BALANCING; //core 0
	VDO_CH_VDO_10BPACKED_CONFIG(1) = SMART_BALANCING; //core 1

	//vdo port change
	for(i = 0 ; i < 10; i++)
	{
		VDO_ADDR_SWITCH_CONTROL(i)        = 0x000000FF;
		VDO_ADDR_SWITCH_FUNC(i, 0) = 0x00007F00 + (VDO_PORT_NUM<<24);
		VDO_ADDR_SWITCH_FUNC(i, 1) = 0x0000FF80 + (VDO_PORT_NUM<<24);
	}

	return;
}

void DE_VDO_HAL_SetSeamless(UINT8 ui8SeamlessMode)
{
	// ui8SeamlessMode -> 0: No seamless mode, 1: Netflix, 2: BBC 
	ui8SeamlessModeValue = ui8SeamlessMode;
	
	if (ui8SeamlessModeValue == 1) //Netflix Seamless mode
		TOP_RC_TOPCONFIG = 0x000000C8;
	else //No seamless or BBC seamless mode or Amazon mode
		TOP_RC_TOPCONFIG = 0x000000A4;

	log_user1("Seamless Mode Status : %s\n",(TOP_RC_TOPCONFIG == 0x000000C8)?"Netflix Seamless Mode"
			:((ui8SeamlessModeValue == 2)?"BBC Seamless Mode":((ui8SeamlessModeValue == 3)?"Amazon Mode":"No Seamless Mode")));
}
