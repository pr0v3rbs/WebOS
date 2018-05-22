////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_SYSTEM_LG_IO_H__
#define __DRV_SYSTEM_LG_IO_H__

//#define IO_SYS_GET_RAW_UART	// not used for LGE (dreamer@lge.com)
//#define IO_SYS_REG_OP      	// not used for LGE (dreamer@lge.com)

//-------------------------------------------------------------------------------------------------
//  ioctl method
//-------------------------------------------------------------------------------------------------

// Use 'S' as magic number
#define SYS_LG_IOCTL_MAGIC             'L'

#if 1 // Temporary Remove
#define IOCTL_SYS_LG_GETREV               _IOR (SYS_LG_IOCTL_MAGIC, 0x01, int)
#define IOCTL_SYS_LG_PCMCIA_CONFIG	      _IOR (SYS_LG_IOCTL_MAGIC, 0x02, int)

#define IOCTL_SYS_LG_GFLIP_INIT	      _IOR (SYS_LG_IOCTL_MAGIC, 0x80, int)

#define IOCTL_SYS_LG_MAXNR                0xFF
#else
#define IOCTL_SYS_INIT                 _IOWR(SYS_IOCTL_MAGIC, 0x00, int)
#define IOCTL_SYS_SET_PANEL_INFO       _IOW (SYS_IOCTL_MAGIC, 0x01, int)
#define IOCTL_SYS_SET_BOARD_INFO       _IOW (SYS_IOCTL_MAGIC, 0x02, int)
#define IOCTL_SYS_GET_PANEL_RES        _IOR (SYS_IOCTL_MAGIC, 0x03, int)
#define IOCTL_SYS_READ_GEN_REGISTER    _IOR (SYS_IOCTL_MAGIC, 0x04, int)
#define IOCTL_SYS_WRITE_GEN_REGISTER   _IOWR(SYS_IOCTL_MAGIC, 0x05, int)
#define IOCTL_SYS_LOAD_AEON            _IOWR(SYS_IOCTL_MAGIC, 0x06, int)
#define IOCTL_SYS_RESET_AEON           _IOWR(SYS_IOCTL_MAGIC, 0x07, int)
#define IOCTL_SYS_ENABLE_AEON          _IO(SYS_IOCTL_MAGIC, 0x08)
#define IOCTL_SYS_DISABLE_AEON         _IO(SYS_IOCTL_MAGIC, 0x09)
#define IOCTL_SYS_SWITCH_UART          _IOR (SYS_IOCTL_MAGIC, 0x0A, int)
#define IOCTL_SYS_DUMP_AEON_MSG        _IOR (SYS_IOCTL_MAGIC, 0x0B, int)
#define IOCTL_SYS_IS_AEON_ENABLE	   _IOR (SYS_IOCTL_MAGIC, 0x0C, int)
#define IOCTL_SYS_CHANGE_UART          _IOWR(SYS_IOCTL_MAGIC, 0x0D, int)

#define IOCTL_SYS_FLUSH_MEMORY         _IO(SYS_IOCTL_MAGIC, 0x50)
#define IOCTL_SYS_READ_MEMORY          _IO(SYS_IOCTL_MAGIC, 0x51)

#ifdef IO_SYS_REG_OP
#   define IOCTL_SYS_REG_OP            _IOWR(SYS_IOCTL_MAGIC, 0x54, int)
#endif

#ifdef IO_SYS_GET_RAW_UART
#define IOCTL_SYS_GET_RAW_UART         _IOWR(SYS_IOCTL_MAGIC, 0x55, int) //terry, change ID 56 -> 55
#define IOCTL_SYS_SEND_RAW_UART        _IOWR(SYS_IOCTL_MAGIC, 0x56, int)
#endif

#define IOCTL_SYS_TIMER                _IOWR(SYS_IOCTL_MAGIC, 0x57, int)
#define IOCTL_SYS_RELOAD_AEON          _IOWR(SYS_IOCTL_MAGIC, 0x58, int)

#if 0
//#define IOCTL_SYS_SWITCH_PAD        _IOWR(SYS_IOCTL_MAGIC, 0x01, DevSys_Switch_Pad)
#define IOCTL_SYS_WDT_ENABLE        _IOW (SYS_IOCTL_MAGIC, 0x02, MS_U32)
#define IOCTL_SYS_WDT_CLEAR         _IO  (SYS_IOCTL_MAGIC, 0x03)
#define IOCTL_SYS_WDT_LASTSTATUS    _IOWR(SYS_IOCTL_MAGIC, 0x04, MS_U32)
#define IOCTL_SYS_WDT_SETTIME       _IOW (SYS_IOCTL_MAGIC, 0x05, MS_U32)
#define IOCTL_SYS_RESET_CHIP        _IO  (SYS_IOCTL_MAGIC, 0x06)
#define IOCTL_SYS_RESET_CPU         _IO  (SYS_IOCTL_MAGIC, 0x07)
#endif

#define IOCTL_SYS_HOTEL_MODE           _IOWR(SYS_IOCTL_MAGIC, 0x59, int)
#define IOCTL_SYS_HOTEL_MODE_PRINTF    _IOWR(SYS_IOCTL_MAGIC, 0x5A, int)

#define IOCTL_SYS_SETSEQFILE           _IOW (SYS_IOCTL_MAGIC, 0x60, int)

// Samuel, bad HDD timeout, 090115
#define IOCTL_SYS_SETVBUS              _IOW (SYS_IOCTL_MAGIC, 0x61, int)

//20090724 Terry, URSA ISP Load Code
#define IOCTL_SYS_GETSPI               _IOWR (SYS_IOCTL_MAGIC, 0x62, int)

#define IOCTL_SYS_GETREV               _IOR (SYS_IOCTL_MAGIC, 0x63, int)

//20091028, Terry, MIU protect
#define IOCTL_SYS_MIU_PROTECT          _IOWR (SYS_IOCTL_MAGIC, 0x64, int)

#define IOCTL_SYS_SETUART_MODE         _IOWR (SYS_IOCTL_MAGIC, 0x65, int)

#define IOCTL_SYS_SPI_LOAD             _IOWR (SYS_IOCTL_MAGIC, 0x66, int)

#define IOCTL_SYS_MMIO_SET_MAP          _IOWR (SYS_IOCTL_MAGIC, 0x67, int)

#define IOCTL_SYS_READ_MEMORY_RANGE     _IOWR (SYS_IOCTL_MAGIC, 0x68, int)
#define IOCTL_SYS_UART_ENABLE			        _IOWR (SYS_IOCTL_MAGIC, 0x69, int)

#define IOCTL_SYS_UART_DISABLE		        _IOWR (SYS_IOCTL_MAGIC, 0x6A, int)

//20110415, Luke, OTP Read HDCP
#define IOCTL_SYS_OTP_READ 		_IOWR(SYS_IOCTL_MAGIC, 0x69, IO_SYS_SPI_t)

#define IOCTL_SYS_CPU_SYNC          _IO(SYS_IOCTL_MAGIC, 0x6B)
#define IOCTL_SYS_SET_GPU_MIU_OFF	_IOW(SYS_IOCTL_MAGIC, 0x6C, int)

#define IOCTL_SYS_GETID               _IOR (SYS_IOCTL_MAGIC, 0x6D, int)
#define IOCTL_SYS_EFUSE_READ 		_IOWR(SYS_IOCTL_MAGIC, 0x6E, IO_SYS_SPI_t)

#define IOCTL_SYS_SET_SERVICE_SLICE	_IO(SYS_IOCTL_MAGIC, 0x6F)
#define IOCTL_SYS_GET_MEM_INFO 		_IOWR(SYS_IOCTL_MAGIC, 0x70, IO_SYS_MEM_INFO_t)
#define IOCTL_SYS_GET_MEM_BLOCK 		_IOWR(SYS_IOCTL_MAGIC, 0x71, IO_SYS_MEM_BLOCK_t)

//20121107, Bruce, utopia PCMCIA use
#define IOCTL_SYS_PCMCIA_WRITE      	_IOW(SYS_IOCTL_MAGIC, 0x40, int)
#define IOCTL_SYS_PCMCIA_READ       	_IOR(SYS_IOCTL_MAGIC, 0x41, int)
#define IOCTL_SYS_PCMCIA_WRITE_DATA     _IOW(SYS_IOCTL_MAGIC, 0x42, int)
#define IOCTL_SYS_PCMCIA_READ_DATA      _IOR(SYS_IOCTL_MAGIC, 0x43, int)
#define IOCTL_SYS_PCMCIA_POWER_ON		_IOR(SYS_IOCTL_MAGIC, 0x44, int)
#define IOCTL_SYS_PCMCIA_POWER_OFF		_IOR(SYS_IOCTL_MAGIC, 0x45, int)

#define IOCTL_SYS_MAXNR    0xFF
#endif

#endif // __DRV_SYSTEM_LG_IO_H__
