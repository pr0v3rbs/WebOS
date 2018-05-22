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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSystem.h
/// @brief  System Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SYSTEM_LG_H_
#define _DRV_SYSTEM_LG_H_

#include <linux/version.h>
#include "chip_setup.h"
#include "mst_platform.h"


#ifndef _DRV_SYSTEM_H_
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define UART0_ENABLE   0
#define UART1_ENABLE   1
#define UART2_ENABLE   2

#define UART3_ENABLE   3
#define UART4_ENABLE   4




#define SYS_BOARD_NAME_MAX          32                                  ///< Maximum length of board name
#define SYS_PLATFORM_NAME_MAX       32                                  ///< Maximum length of playform name

#ifdef CONFIG_Titania2
#define REG_SW_RESET_CPU_AEON                   0x101086
//---------------------------------------------
// definition for REG_SW_RESET_CPU_AEON   //reg[0x1086]
#define AEON_SW_RESET                           BIT0
#endif

#if defined(CONFIG_Titania8)|| defined(CONFIG_Titania9)|| defined(CONFIG_Amber1)|| defined(CONFIG_Emerald) || defined(CONFIG_Nugget)
#define REG_PMMCU_BASE                          0xBF002000

#define REG_SW_RESET_CPU_AEON                   0x43
#define AEON_SW_RESET                           BIT0
#endif

#define REG_CKG_AEONTS0                         0x101E26
#define AEON_SPI_ADDR0                          0x100FFE
#define AEON_CLK_ENABLE                         0x00
#define AEON_CLK_DISABLE                        0x40


#define AEON_REG_CTRL                           0x100FF0

//---------------------------------------------
// definition for AEON_REG_CTRL   //reg[0x0FF0]
#define AEON_CTRL_EN                            BIT0
#define AEON_CTRL_RST                           BIT1
#define AEON_DWB_SWAP                           BIT3

#define KERNEL_PCMCIA_PERF						1
#if 0
//--------------------------------------------------------------------------------------
// 2012.11.08, Bruce utopia PCMCIA
#define REG_CHIP_NAND_MODE						0x1EA0
	#define REG_CHIP_NAND_MODE_MASK				0x0300
    #define REG_CHIP_NAND_MODE_PCMA				0x0200
	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PF_MODE			        0x1EDE
    #define REG_CHIP_PF_MODE_MASK				0x0010

#define REG_CHIP_PCMCFG                         0x1E9E
    #define REG_CHIP_PCMCFG_MASK                0xC000
    #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable

#define REG_EMMC_CONFIG                         0x1EBA
    #define REG_EMMC_CONFIG_MASK                0xC000

#define REG_CHIP_PCM_PE                         0x1E82
    #define REG_CHIP_PCM_PE_MASK                0xFFFF
#define REG_CHIP_PCM_PE1                        0x1E84
    #define REG_CHIP_PCM_PE1_MASK               0x03FF
#define REG_CHIP_PCM_D_PE                       0x1E18
    #define REG_CHIP_PCM_D_PE_MASK              0x00FF

#define REG_CHIP_PCM_NAND_BYPASS                0x1E86
    #define REG_CHIP_PCM_NAND_BYPASS_MASK       0x0002
    #define REG_CHIP_PCM_NAND_BYPASS_ENABLE     0x0002
    #define REG_CHIP_PCM_NAND_BYPASS_DISABLE    0x0000

#define REG_SD_CONFIG2                          0x1EAE
    #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2

#define REG_PCMCIA_PCM_MEM_IO_CMD           	0x00
#define REG_PCMCIA_ADDR0                    	0x02
#define REG_PCMCIA_ADDR1                    	0x03
#define REG_PCMCIA_WRITE_DATA               	0x04
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     	0x06
#define REG_PCMCIA_READ_DATA                	0x08
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  	0x09
#define REG_PCMCIA_INT_MASK_CLEAR           	0x0A
#define REG_PCMCIA_INT_MASK_CLEAR1          	0x0B
#define REG_PCMCIA_STAT_INT_RAW_INT         	0x0E
#define REG_PCMCIA_STAT_INT_RAW_INT1        	0x0F
#define REG_PCMCIA_MODULE_VCC_OOB           	0x10
#define PCMCIA_ATTRIBMEMORY_READ            	0x03
#define PCMCIA_ATTRIBMEMORY_WRITE           	0x04
#define PCMCIA_IO_READ                      	0x05
#define PCMCIA_IO_WRITE                     	0x06
#define REG_PCMCIA_BASE                     	0x3440
#define PCMCIA_FIRE_COMMAND         			BIT0
#define PCMCIA_CLEAN_STATE_RD_DONE  			BIT1
#define PCMCIA_STATE_RD_DONE        			BIT0
#define PCMCIA_STATE_BUS_IDLE       			BIT1
#define PCMCIA_MAX_DETECT_COUNT     			1
#define PCMCIA_MAX_POLLING_COUNT    			20000
#define MAX_PCMCIA_CONFIGS              		6       	//!< The maximum number of configurations supported by a PCMCIA card
//#define MAX_PCMCIA_STRLEN               		(20)    	//!< The maximum name of vendor/manufacturer/info strings
#define MAX_CIS_SIZE                    		0x100  		//!< The maximum size of a CIS, that is understood by this driver
#define PCMCIA_HW_TIMEOUT               		3000
#define PCMCIA_HW_MAX_RETRY_COUNT       		10      	// PCMCIA hardware register maximum access times
#define PCMCIA_IRQ_ENABLE               		1       	// FIXME_ALEC

#else

#if 1
/*
#if defined(CONFIG_MSTAR_TITANIA2)
#elif defined(CONFIG_MSTAR_EUCLID)
#elif defined(CONFIG_MSTAR_TITANIA3)
#elif defined(CONFIG_MSTAR_URANUS4)
#elif defined(CONFIG_MSTAR_TITANIA4)
#elif defined(CONFIG_MSTAR_TITANIA8) || \
	  defined(CONFIG_MSTAR_TITANIA12) || \
      defined(CONFIG_MSTAR_AMBER2)
#elif defined(CONFIG_MSTAR_AMBER5)

#elif defined(CONFIG_MSTAR_TITANIA9) || \
      defined(CONFIG_MSTAR_TITANIA13) || \
      defined(CONFIG_MSTAR_AMBER1) || \
      defined(CONFIG_MSTAR_AMBER6) || \
      defined(CONFIG_MSTAR_AMBER7) || \
      defined(CONFIG_MSTAR_AMETHYST) || \
      defined(CONFIG_MSTAR_EMERALD)
*/
#define _REG_ADDR(addr)                          (*((volatile U16*)(0xBF200000 + (addr << 1))))
/*
#define REG_CKG_AEONTS0                         0x0B24
#define AEON_CLK_ENABLE                         0x0084 //CLK 172.8MHz
#define AEON_CLK_DISABLE                        0x0001
#define MIU_PROTECT_EN                          0x12C0
#define MIU_PROTECT3_ID0                        0x12D6
#define MIU_PROTECT3_START_ADDR_H               0x12D8
#define MIU_PROTECT3_END_ADDR_H                 0x12DA
*/
#define MIU_CLI_AEON_RW                     	0x0005
#define MIU_PROTECT_4                       	0x0008
#define MIU_PROTECT_4                           0x0008
#define MBX_AEON_JUDGEMENT                      0x33DE
#define MHEG5_CPU_STOP                          0x0FE6
#define STOP_AEON                           	0x0001
#define MHEG5_REG_IOWINORDER                    0x0F80
#define REG_AEON_C_FIQ_MASK_L                   0x1948
#define REG_AEON_C_FIQ_MASK_H                   0x194A
#define REG_AEON_C_IRQ_MASK_L                   0x1968
#define REG_AEON_C_IRQ_MASK_H                   0x196A
#define REG_MAU0_MIU0_SIZE                      0x1842

//    #if (defined(CONFIG_MSTAR_TITANIA9) || defined(CONFIG_MSTAR_TITANIA13))
//	#elif defined(CONFIG_MSTAR_EMERALD)
    #define REG_CHIP_NAND_MODE						0x1EA0
	#define REG_CHIP_NAND_MODE_MASK				0x0380
	#define REG_CHIP_NAND_MODE_PCMA				0x0200
	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PCMCFG                         0x1E9E
        #define REG_CHIP_PCMCFG_MASK                0xC000
        #define REG_CHIP_PCMCFG_CTRL_EN             0xC000 //AD pads & Ctrl pads enable
    #define REG_SD_CONFIG2                          0x1EAE
        #define REG_SD_CONFIG2_MASK                 0x0F00 //reg_sd_config & reg_sd_config2

	#define REG_CHIP_PCM_GPIO_OUT_HI_PCM2_GPIO_OUT	0x1E7A
		#define REG_CHIP_PCM_MASK					0x1F00
/*    #else
#define REG_CHIP_NAND_MODE						0x1EA0
	#define REG_CHIP_NAND_MODE_MASK				0x0300
    #define REG_CHIP_NAND_MODE_PCMA				0x0200
	#define REG_CHIP_NAND_MODE_PCMD				0x0100
    #define REG_CHIP_PF_MODE			        0x1EDE
    #define REG_CHIP_PF_MODE_MASK				0x0010
    #endif
*/
#define REG_CHIP_PCM_PE                         0x1E16
#define REG_CHIP_PCM_PE_MASK                0x00FF // PCM_D
#define REG_CHIP_PCM_PE1                        0x1E18
#define REG_CHIP_PCM_PE1_MASK               0x00FF // PCM_A
#define MIPS_MIU0_BUS_BASE                      0x00000000
#define MIPS_MIU1_BUS_BASE                      0x60000000
/*
#elif defined(CONFIG_MSTAR_TITANIA11)
#elif defined(CONFIG_MSTAR_JANUS2)
#elif defined(CONFIG_MSTAR_KRONUS)
#elif defined(CONFIG_MSTAR_KAISERIN)
#elif defined(CONFIG_MSTAR_AMBER3)
#elif defined(CONFIG_MSTAR_EAGLE)
#elif defined(CONFIG_MSTAR_AGATE)
#elif defined(CONFIG_MSTAR_EDISON)
#elif defined(CONFIG_MSTAR_EINSTEIN)
#elif defined(CONFIG_MSTAR_EIFFEL)
#endif
*/

/*
#if defined(CONFIG_MSTAR_EUCLID) || \
    defined(CONFIG_MSTAR_URANUS4) || \
    defined(CONFIG_MSTAR_TITANIA2) || \
    defined(CONFIG_MSTAR_TITANIA4) || \
    defined(CONFIG_MSTAR_KRONUS) || \
    defined(CONFIG_MSTAR_KAISERIN) || \
    defined(CONFIG_MSTAR_TITANIA11) || \
    defined(CONFIG_MSTAR_AMBER2)
#elif defined(CONFIG_MSTAR_TITANIA3)
#elif defined(CONFIG_MSTAR_TITANIA8) || \
       defined(CONFIG_MSTAR_TITANIA12) || \
       defined(CONFIG_MSTAR_TITANIA9) || \
       defined(CONFIG_MSTAR_TITANIA13) || \
       defined(CONFIG_MSTAR_AMBER1) || \
       defined(CONFIG_MSTAR_AMBER5) || \
       defined(CONFIG_MSTAR_AMBER6) || \
       defined(CONFIG_MSTAR_AMBER7) || \
       defined(CONFIG_MSTAR_AMETHYST) || \
       defined(CONFIG_MSTAR_JANUS2) || \
       defined(CONFIG_MSTAR_EMERALD)
*/
    #define MS_MIU_INTERVAL 0x20000000

/*
#elif defined(CONFIG_MSTAR_AMBER3) || \
      defined(CONFIG_MSTAR_EAGLE) || \
      defined(CONFIG_MSTAR_AGATE) || \
      defined(CONFIG_MSTAR_EDISON) || \
      defined(CONFIG_MSTAR_EINSTEIN) || \
      defined(CONFIG_MSTAR_EIFFEL)
#else
     #error unknown chip!!
#endif
*/

#define REG_PCMCIA_PCM_MEM_IO_CMD           0x00
#define REG_PCMCIA_ADDR0                    0x02
#define REG_PCMCIA_ADDR1                    0x03
#define REG_PCMCIA_WRITE_DATA               0x04
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     0x06
#define REG_PCMCIA_READ_DATA                0x08
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  0x09
#define REG_PCMCIA_INT_MASK_CLEAR           0x0A
#define REG_PCMCIA_INT_MASK_CLEAR1          0x0B
#define REG_PCMCIA_STAT_INT_RAW_INT         0x0E
#define REG_PCMCIA_STAT_INT_RAW_INT1        0x0F
#define REG_PCMCIA_MODULE_VCC_OOB           0x10
#define PCMCIA_ATTRIBMEMORY_READ            0x03
#define PCMCIA_ATTRIBMEMORY_WRITE           0x04
#define PCMCIA_IO_READ                      0x05
#define PCMCIA_IO_WRITE                     0x06
#define REG_PCMCIA_BASE                     0x3440
#define PCMCIA_FIRE_COMMAND         BIT0
#define PCMCIA_CLEAN_STATE_RD_DONE  BIT1
#define PCMCIA_STATE_RD_DONE        BIT0
#define PCMCIA_STATE_BUS_IDLE       BIT1
#define PCMCIA_MAX_DETECT_COUNT     1
#define PCMCIA_MAX_POLLING_COUNT    20000
#define MAX_PCMCIA_CONFIGS              6       //!< The maximum number of configurations supported by a PCMCIA card
//#define MAX_PCMCIA_STRLEN               (20)    //!< The maximum name of vendor/manufacturer/info strings
#define MAX_CIS_SIZE                    0x100   //!< The maximum size of a CIS, that is understood by this driver
#define PCMCIA_HW_TIMEOUT               3000
#define PCMCIA_HW_MAX_RETRY_COUNT       5000      //  PCMCIA hardware register maximum access times
#define PCMCIA_IRQ_ENABLE               1       // FIXME_ALEC

#endif
#endif
//--------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// System output pad switch type
typedef enum
{
	E_SYS_PAD_MSD5010_SM2_IIC2,                                         ///< 5010 SM2, IIC2
	E_SYS_PAD_MSD5011_SM2_IIC2,                                         ///< 5011 SM2, IIC2
	E_SYS_PAD_MSD5015_GPIO,                                             ///< 5015 GPIO
	E_SYS_PAD_MSD5018_SM2,                                              ///< 5018 SM2
} SYS_PadType;

/// System information
typedef struct
{
	/// Software information
	struct
	{
		U8                          Board[SYS_BOARD_NAME_MAX];          ///< Board name
		U8                          Platform[SYS_PLATFORM_NAME_MAX];    ///< Platform name
	} SWLib;
} SYS_Info;

#if 0
/// Memory mapping type
typedef enum
{
	E_SYS_MMAP_LINUX_BASE,
	E_SYS_MMAP_VD_3DCOMB,
	E_SYS_MMAP_MAD_BASE,
	E_SYS_MMAP_MVD_FB,
	E_SYS_MMAP_MVD_BS,
	E_SYS_MMAP_EMAC,
	E_SYS_MMAP_VE,
	E_SYS_MMAP_SCALER_MENU_LOAD,
	E_SYS_MMAP_SCALER_DNR_BUF,
	E_SYS_MMAP_RLD_BUF,
	E_SYS_MMAP_TTX_BUF,
	E_SYS_MMAP_MPOOL,
	E_SYS_MMAP_LINUX_2ND_MEM,
	E_SYS_MMAP_SVD,
	E_SYS_MMAP_MVD_SW,
	E_SYS_MMAP_SVD_ALL,
	E_SYS_MMAP_POSD0_MEM,
	E_SYS_MMAP_POSD1_MEM,
	E_SYS_MMAP_TSP, // samuel, 20081107
	E_SYS_MMAP_AUDIO_CLIP_MEM, // samuel, 20081107
	E_SYS_MMAP_BIN_MEM,
	E_SYS_MMAP_JPD_MEM, // Samuel, 20090108
	E_SYS_MMAP_BT_POOL,	// dreamer@lge.com, 20090112
	E_SYS_MMAP_M4VE,
#if defined(CONFIG_Titania8)
	E_SYS_MMAP_JPG_OSD,
#endif
	//#if defined(CONFIG_MSTAR_TITANIA_BD_T3_FPGA)
	E_SYS_MMAP_MIU1_BASE,
	E_SYS_MMAP_FPGA_POOL_BASE,
	//#endif
    E_SYS_MMAP_PVR_DOWNLOAD_MEM, // StevenL, 20090311
	E_SYS_MMAP_PVR_UPLOAD_MEM, // StevenL, 20090311
	E_SYS_MMAP_PVR_THUMBNAIL_DECODE_MEM, // StevenL, 20090311
	E_SYS_MMAP_NUMBER,
} SYS_Memory_Mapping;
#endif
#endif // _DRV_SYSTEM_H_


//mail box crash protection 2009-11-06
typedef enum VDMCU_DSP_TYPE_e
{
    VDMCU_DSP_UNKNOWN = 0,
    VDMCU_DSP_DVBT,
    VDMCU_DSP_DVBC,
    VDMCU_DSP_ATSC,
    VDMCU_DSP_VIF,
    VDMCU_DSP_ISDBT,
    VDMCU_DSP_DTMB,
    VDMCU_DSP_DVBT2,
    VDMCU_DSP_DVBS
} VDMCU_DSP_TYPE_t;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#if 1 // Temporary Remove
U32 MDrv_SYS_LG_GetRev(U32 arg);
void MDrv_SYS_PCMCIA_Config(void);
//mail box crash protection 2009-11-06
void MDrv_SYS_VDmcuSetType(VDMCU_DSP_TYPE_t type);
VDMCU_DSP_TYPE_t MDrv_SYS_VDmcuGetType(void);
#else
BOOL MDrv_System_Init(void);
//BOOL MDrv_System_SwitchPad(SYS_PadType ePadType);
void MDrv_System_WDTEnable(BOOL bEnable);
void MDrv_System_WDTClear(void);
BOOL MDrv_System_WDTLastStatus(void);
void MDrv_System_WDTSetTime(U32 u32Ms);
void MDrv_System_ResetChip(void);
void MDrv_System_ResetCPU(void);


U32 MDrv_SYS_SetPanelInfo(U32 arg);
void MDrv_SYS_GetPanelRes(U32 arg);
PMST_PANEL_INFO_t MDrv_SYS_GetPanelInfo(void);

void MDrv_SYS_ReadGeneralRegister(U32 arg);
void MDrv_SYS_WriteGeneralRegister(U32 arg);
void MDrv_SYS_LoadAeon(U32 arg);
void MDrv_SYS_ResetAeon(U32 arg);
void MDrv_SYS_EnableAeon(void);
void MDrv_SYS_DumpAeonMessage(void);
void MDrv_SYS_DisableAeon(void);
void MDrv_SYS_SwitchUart(U32 arg);
U32 MDrv_SYS_IsAeonEnable(U32 arg);

U32 MDrv_SYS_GetRawUART(U32 arg);
void MDrv_SYS_ReloadAeon( U32 arg ) ;
U32 MDrv_SYS_Timer(U32 arg) ;
U32 MDrv_SYS_RegOP(U32 arg);
void MDrv_SYS_GetPanelRes(U32 arg);
extern void MDrv_SYS_MMAP_Dump( void ) ;
U32 MDrv_SYS_HotelMode(U32 arg) ;
U32 MDrv_SYS_HotelModePrintf(U32 arg) ;
U32 MDrv_SYS_SetUartMode(U32 arg);

U32 MDrv_SYS_SendRawUART(U32 arg);
U32 MDrv_SYS_GetRawUART(U32 arg);

void MDrv_SYS_UART_Enable(void);
void MDrv_SYS_UART_Disable(void);

void MDrv_SYS_EnableSVDCPU( int enable ) ; // samuel, 20081107
void MDrv_SYS_Enable3DCOM( int enable ) ; // samuel, 20081107

// return reserved memory address & size for specified type
extern int MDrv_SYS_GetMMAP(int type, unsigned int *addr, unsigned int *len);

void MDrv_SYS_ChangeUart( U32 arg );

void MDrv_SYS_CPU_Sync(void);

void MDrv_SYS_Flush_Memory(void);

void MDrv_SYS_Read_Memory(void);

void MDrv_SYS_Read_Memory_Range(U32 arg);

unsigned long MDrv_SYS_GetSyetemTime(void);

void* MDrv_SYS_PA2NonCacheSeg( void* pAddrPA );

U32 MDrv_SYS_SetSeqFile(U32 arg);

U32 MDrv_SYS_GetSPI(U32 arg); //20090724 Terry, URSA ISP Load Code

U32 MDrv_SYS_SPI_LOAD(U32 arg); //20100120 Terry, SPI Load Code

BOOL MDrv_SYS_MIU_Protect(U32 arg); //20091028 Terry, MIU protect

U32 MDrv_SYS_GetId(U32 arg);


void MDrv_SYS_LoadInitBWTable(void);                                                // 20091002 daniel.huang: add for bandwidth adjustment
void MDrv_SYS_LoadBWTable(U16 u16Input_HSize, U16 u16Input_VSize, BOOL bInterlace); // 20091002 daniel.huang: add for bandwidth adjustment

//mail box crash protection 2009-11-06
void MDrv_SYS_VDmcuSetType(VDMCU_DSP_TYPE_t type);
VDMCU_DSP_TYPE_t MDrv_SYS_VDmcuGetType(void);

U32 MDrv_SYS_SetMMIOMap( U32 arg );

void MDrv_SYS_SwitchPad(U32,U16, U16);

void MDrv_SYS_EFUSERead(U32 arg);
void MDrv_SYS_OTPRead(U32 arg); //20110415 Luke, OTP HDCP Read
void MDrv_SYS_Set_GPU_MIU_disable(U32 arg); //LGE chongheun.kim 20111126

void MDrv_SYS_SetServiceSlice(void);

U32 MDrv_SYS_PCMCIA_READ(U32 arg);
U32 MDrv_SYS_PCMCIA_READ_DATA(U32 arg);
U32 MDrv_SYS_PCMCIA_WRITE(U32 arg);
U32 MDrv_SYS_PCMCIA_WRITE_DATA(U32 arg);
void	MDrv_SYS_PCMCIA_SwitchPower_On(void);
void	MDrv_SYS_PCMCIA_SwitchPower_Off(void);
U32 MDrv_SYS_GetMemInfo(U32 arg);
U32 MDrv_SYS_GetMemBlock(U32 arg);
#endif

#endif // _DRV_SYSTEM_LG_H_
