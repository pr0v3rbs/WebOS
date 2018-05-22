/*
 * Copyright (c) 2010-2011 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * Unless otherwise stipulated in writing, any and all information contained
 * herein regardless in any format shall remain the sole proprietary of
 * MStar Semiconductor Inc. and be kept in strict confidence
 * ("MStar Confidential Information") by the recipient.
 * Any unauthorized act including without limitation unauthorized disclosure,
 * copying, use, reproduction, sale, distribution, modification, disassembling,
 * reverse engineering and compiling of the contents of MStar Confidential
 * Information is unlawful and strictly prohibited. MStar hereby reserves the
 * rights to any and all damages, losses, costs and expenses resulting therefrom.
 *
 * MStar's Audio Driver for ALSA.
 * Author: Darren Chen <darren.chen@mstarsemi.com>
 *
 * Munich
 *
 */


/*
 * ============================================================================
 * Include Headers
 * ============================================================================
 */
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,1))
#include <linux/module.h>
#endif
#include <asm/io.h>

#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/uaccess.h>

#include "mstar/mstar_chip.h"
#include "mhal_audio.h"


/*
 * ============================================================================
 * Constant Definitions
 * ============================================================================
 */
#define _MAD_CHIP_MODEL    "MAD MUSTANG"

#define _MAD_PM_MODE_REG_BASE    0xfd000000
#define _MAD_NON_PM_MODE_REG_BASE    0xfd200000
#define _MAD_PHYSICAL_MEM_BASE    _MAD_PM_MODE_REG_BASE
#define _MAD_KERNEL_SPACE_MEMORY_OFFSET    0xA0000000
#define _MAD_PHYSICAL_ADDR_TO_BUS_ADDR    MSTAR_MIU0_BUS_BASE

#define _MAD_BYTES_IN_LINE    16
#define _MAD_ADDR_CONVERTOR    0x1000
#define _MAD_MAILBOX_OFFSET    0x110000

#define _MAD_MAX_CHANNEL    2
#define _MAD_MAX_PCM_MIXER_ELEMENT    8
#define _MAD_MAX_PCM_MIXER_GROUP    2

#define _MAD_PCM_MIXER_VERSION    2
#define _MAD_PCM_MIXER_GROUP_NONE    0x00000000
#define _MAD_PCM_MIXER_GROUP_1    0x00000001
#define _MAD_PCM_MIXER_GROUP_2    0x00000002
#define _MAD_PCM_MIXER_GROUP_3    0x00000004
#define _MAD_PCM_MIXER_GROUP_4    0x00000008
#define _MAD_PCM_MIXER_GROUP_5    0x00000010
#define _MAD_PCM_MIXER_GROUP_6    0x00000020
#define _MAD_PCM_MIXER_GROUP_7    0x00000040
#define _MAD_PCM_MIXER_GROUP_8    0x00000080
#define _MAD_PCM_MIXER_GROUP_ALL    0xFFFFFFFF

#define _MAD_DSP2_DRAM_SIZE    0x300000
#define _MAD_DSP2_TO_COMMON_DRAM_OFFSET    (_MAD_DSP2_DRAM_SIZE)

#define _MAD_DMA_READER_BASE_OFFSET    (0x00000000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_DMA_READER_BUF_UNIT    4096
#define _MAD_DMA_READER_TOTAL_BUF_SIZE    0x20000 /* 128 KByte */
#define _MAD_DMA_READER_MIN_AVAIL    (_MAD_DMA_READER_TOTAL_BUF_SIZE >> 3) /* The minimal available size should be reserved */
#define _MAD_DMA_READER_HIGH_THRESHOLD    (_MAD_DMA_READER_TOTAL_BUF_SIZE - _MAD_DMA_READER_MIN_AVAIL)
#define _MAD_DMA_READER_BUF_SIZE    (_MAD_DMA_READER_TOTAL_BUF_SIZE >> 1) /* Only half size can be used */
#define _MAD_DMA_READER_PERIOD_SIZE    (_MAD_DMA_READER_BUF_SIZE >> 2)
#define _MAD_DMA_READER_QUEUE_SIZE    100 /* ms */

#define _MAD_PCM_PLAYBACK2_BASE_OFFSET    (0x20000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_PCM_PLAYBACK2_BUF_UNIT    4096
#define _MAD_PCM_PLAYBACK2_TOTAL_BUF_SIZE    0x10000 /* 64 KByte */
#define _MAD_PCM_PLAYBACK2_MIN_AVAIL    (_MAD_PCM_PLAYBACK2_TOTAL_BUF_SIZE >> 3) /* The minimal available size should be reserved */
#define _MAD_PCM_PLAYBACK2_HIGH_THRESHOLD    (_MAD_PCM_PLAYBACK2_TOTAL_BUF_SIZE - _MAD_PCM_PLAYBACK2_MIN_AVAIL)
#define _MAD_PCM_PLAYBACK2_BUF_SIZE    (_MAD_PCM_PLAYBACK2_TOTAL_BUF_SIZE >> 1) /* Only half size can be used */
#define _MAD_PCM_PLAYBACK2_PERIOD_SIZE    (_MAD_PCM_PLAYBACK2_BUF_SIZE >> 2)
#define _MAD_PCM_PLAYBACK2_QUEUE_SIZE    100 /* ms */

#define _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET    (0x3D0000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE    0x1000
#define _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET    (0x3D1000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE    0x78000
#define _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE    (_MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE / _MAD_MAX_PCM_MIXER_ELEMENT)
#define _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE    (_MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE >> 3)
#define _MAD_PCM_MIXER_ELEMENT_MIN_AVAIL    _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE
#define _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD    (_MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE - _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE)

#define _MAD_PCM_CAPTURE1_BASE_OFFSET   (0x30000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_PCM_CAPTURE2_BASE_OFFSET   (0x3C000 + _MAD_DSP2_TO_COMMON_DRAM_OFFSET)
#define _MAD_PCM_CAPTURE_BUF_UNIT    128
#define _MAD_PCM_CAPTURE_BUF_SIZE    0xC000 /* 48 KByte */

#define _MAD_READ_BYTE(_reg)    (*(volatile unsigned char*)(_reg))
#define _MAD_READ_WORD(_reg)    (*(volatile unsigned short*)(_reg))
#define _MAD_WRITE_BYTE(_reg, _val)    { (*((volatile unsigned char*)(_reg))) = (unsigned char)(_val); }
#define _MAD_WRITE_WORD(_reg, _val)    { (*((volatile unsigned short*)(_reg))) = (unsigned short)(_val); }
#define _MAD_R1BYTE(u32Addr, u8mask)    (_MAD_READ_BYTE (_MAD_PHYSICAL_MEM_BASE + ((u32Addr) << 1) - ((u32Addr) & 1)) & (u8mask))
#define _MAD_AU_AbsReadByte(u32Reg)    (_MAD_READ_BYTE (_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1) - ((u32Reg) & 1)))
#define _MAD_AU_AbsRead2Byte(u32Reg)    (_MAD_READ_WORD (_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1)) )
#define _MAD_AU_AbsWriteByte(u32Reg, u8Val) \
	do { \
		(_MAD_WRITE_BYTE(_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1) - ((u32Reg) & 1), u8Val)); \
	} while(0)
#define _MAD_AU_AbsWriteMaskByte(u32Reg, u8Mask, u8Val) \
	do { \
		(_MAD_WRITE_BYTE(_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1) - ((u32Reg) & 1), (_MAD_R1BYTE((u32Reg), 0xFF) & ~(u8Mask)) | ((u8Val) & (u8Mask)))); \
	} while(0)
#define _MAD_AU_AbsWrite2Byte(u32Reg, u16Val) \
	do { \
		(_MAD_WRITE_WORD(_MAD_PHYSICAL_MEM_BASE + ((u32Reg) << 1), u16Val)); \
	} while(0)

#define ALSA_AUDIO_HAL_DEBUG 0

/*
 * ============================================================================
 * Forward Declarations
 * ============================================================================
 */
/* Read/Write Register */
#if 0 /* It's not in used for the moment, might be TODO */
static unsigned char _mhal_alsa_read_byte(unsigned int u32RegAddr);
#endif
static unsigned short _mhal_alsa_read_reg(unsigned int u32RegAddr);
#if 0 /* It's not in used for the moment, might be TODO */
static void _mhal_alsa_write_byte(unsigned int u32RegAddr, unsigned char u8Val);
#endif
static void _mhal_alsa_write_mask_byte(unsigned int u32RegAddr, unsigned char u8Mask, unsigned char u8Val);
static void _mhal_alsa_write_reg(unsigned int u32RegAddr, unsigned short u16Val);
static void _mhal_alsa_write_mask_reg(unsigned int u32RegAddr, unsigned short u16Mask, unsigned short u16Val);
static unsigned int _mhal_alsa_get_device_status(void);

/* DMA Reader */
static int _mhal_alsa_dma_reader_init(void);
static int _mhal_alsa_dma_reader_exit(void);
static int _mhal_alsa_dma_reader_start(void);
static int _mhal_alsa_dma_reader_stop(void);
static int _mhal_alsa_dma_reader_resume(void);
static int _mhal_alsa_dma_reader_suspend(void);
static unsigned int _mhal_alsa_dma_reader_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_dma_reader_get(int cmd, unsigned int *param);
static int _mhal_alsa_dma_reader_set(int cmd, unsigned int *param);
static int _mhal_alsa_dma_reader_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_dma_reader_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_dma_reader_get_inused_lines(void);
static int _mhal_alsa_dma_reader_get_avail_lines(void);

/* PCM Playback2 */
static int _mhal_alsa_pcm_playback2_init(void);
static int _mhal_alsa_pcm_playback2_exit(void);
static int _mhal_alsa_pcm_playback2_start(void);
static int _mhal_alsa_pcm_playback2_stop(void);
static int _mhal_alsa_pcm_playback2_resume(void);
static int _mhal_alsa_pcm_playback2_suspend(void);
static unsigned int _mhal_alsa_pcm_playback2_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_playback2_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_playback2_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_playback2_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_playback2_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_playback2_get_inused_lines(void);
static int _mhal_alsa_pcm_playback2_get_avail_lines(void);

/* PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_init(void);
static int _mhal_alsa_pcm_mixer3_exit(void);
static int _mhal_alsa_pcm_mixer3_start(void);
static int _mhal_alsa_pcm_mixer3_stop(void);
static int _mhal_alsa_pcm_mixer3_resume(void);
static int _mhal_alsa_pcm_mixer3_suspend(void);
static unsigned int _mhal_alsa_pcm_mixer3_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_mixer3_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer3_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer3_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_mixer3_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_mixer3_get_inused_bytes(void);
static int _mhal_alsa_pcm_mixer3_get_avail_bytes(void);

/* PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_init(void);
static int _mhal_alsa_pcm_mixer4_exit(void);
static int _mhal_alsa_pcm_mixer4_start(void);
static int _mhal_alsa_pcm_mixer4_stop(void);
static int _mhal_alsa_pcm_mixer4_resume(void);
static int _mhal_alsa_pcm_mixer4_suspend(void);
static unsigned int _mhal_alsa_pcm_mixer4_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_mixer4_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer4_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer4_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_mixer4_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_mixer4_get_inused_bytes(void);
static int _mhal_alsa_pcm_mixer4_get_avail_bytes(void);

/* PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_init(void);
static int _mhal_alsa_pcm_mixer5_exit(void);
static int _mhal_alsa_pcm_mixer5_start(void);
static int _mhal_alsa_pcm_mixer5_stop(void);
static int _mhal_alsa_pcm_mixer5_resume(void);
static int _mhal_alsa_pcm_mixer5_suspend(void);
static unsigned int _mhal_alsa_pcm_mixer5_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_mixer5_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer5_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer5_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_mixer5_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_mixer5_get_inused_bytes(void);
static int _mhal_alsa_pcm_mixer5_get_avail_bytes(void);

/* PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_init(void);
static int _mhal_alsa_pcm_mixer6_exit(void);
static int _mhal_alsa_pcm_mixer6_start(void);
static int _mhal_alsa_pcm_mixer6_stop(void);
static int _mhal_alsa_pcm_mixer6_resume(void);
static int _mhal_alsa_pcm_mixer6_suspend(void);
static unsigned int _mhal_alsa_pcm_mixer6_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_mixer6_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer6_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer6_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_mixer6_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_mixer6_get_inused_bytes(void);
static int _mhal_alsa_pcm_mixer6_get_avail_bytes(void);

/* PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_init(void);
static int _mhal_alsa_pcm_mixer7_exit(void);
static int _mhal_alsa_pcm_mixer7_start(void);
static int _mhal_alsa_pcm_mixer7_stop(void);
static int _mhal_alsa_pcm_mixer7_resume(void);
static int _mhal_alsa_pcm_mixer7_suspend(void);
static unsigned int _mhal_alsa_pcm_mixer7_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_mixer7_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer7_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer7_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_mixer7_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_mixer7_get_inused_bytes(void);
static int _mhal_alsa_pcm_mixer7_get_avail_bytes(void);

/* PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_init(void);
static int _mhal_alsa_pcm_mixer8_exit(void);
static int _mhal_alsa_pcm_mixer8_start(void);
static int _mhal_alsa_pcm_mixer8_stop(void);
static int _mhal_alsa_pcm_mixer8_resume(void);
static int _mhal_alsa_pcm_mixer8_suspend(void);
static unsigned int _mhal_alsa_pcm_mixer8_write(void* buffer, unsigned int bytes);
static int _mhal_alsa_pcm_mixer8_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer8_set(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_mixer8_set_sample_rate(unsigned int sample_rate);
static int _mhal_alsa_pcm_mixer8_set_channel_mode(unsigned int channel_mode);
static int _mhal_alsa_pcm_mixer8_get_inused_bytes(void);
static int _mhal_alsa_pcm_mixer8_get_avail_bytes(void);

/* PCM Capture1 */
static int _mhal_alsa_pcm_capture1_init(void);
static int _mhal_alsa_pcm_capture1_exit(void);
static int _mhal_alsa_pcm_capture1_start(void);
static int _mhal_alsa_pcm_capture1_stop(void);
static int _mhal_alsa_pcm_capture1_resume(void);
static int _mhal_alsa_pcm_capture1_suspend(void);
static unsigned int _mhal_alsa_pcm_capture1_read(void *buffer, unsigned int bytes);
static int _mhal_alsa_pcm_capture1_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_capture1_set(int cmd, unsigned int *param);
static unsigned int _mhal_alsa_pcm_capture1_get_new_avail_bytes(void);
static unsigned int _mhal_alsa_pcm_capture1_get_total_avail_bytes(void);
static int _mhal_alsa_pcm_capture1_set_buffer_size(unsigned int buffer_size);

/* PCM Capture2 */
static int _mhal_alsa_pcm_capture2_init(void);
static int _mhal_alsa_pcm_capture2_exit(void);
static int _mhal_alsa_pcm_capture2_start(void);
static int _mhal_alsa_pcm_capture2_stop(void);
static int _mhal_alsa_pcm_capture2_resume(void);
static int _mhal_alsa_pcm_capture2_suspend(void);
static unsigned int _mhal_alsa_pcm_capture2_read(void *buffer, unsigned int bytes);
static int _mhal_alsa_pcm_capture2_get(int cmd, unsigned int *param);
static int _mhal_alsa_pcm_capture2_set(int cmd, unsigned int *param);
static unsigned int _mhal_alsa_pcm_capture2_get_new_avail_bytes(void);
static unsigned int _mhal_alsa_pcm_capture2_get_total_avail_bytes(void);
static int _mhal_alsa_pcm_capture2_set_buffer_size(unsigned int buffer_size);


/*
 * ============================================================================
 * Local Variables
 * ============================================================================
 */
/* MStar Audio DSP */
static struct MStar_MAD_Info MStar_MAD;

/* Supported Audio Rates by MStar Audio DSP */
static unsigned int mad_rates[] = {
	8000,
	11025,
	12000,
	16000,
	22050,
	24000,
	32000,
	44100,
	48000,
};

static unsigned int pcm_mixer_rates[] = {
	8000,
	11025,
	12000,
	16000,
	22050,
	24000,
	32000,
	44100,
	48000,
	96000,
};

/* MStar Audio DSP - DMA Reader */
static struct MStar_DMA_Reader_Struct g_dma_reader = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_DMA_READER_TOTAL_BUF_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.initialized_status = MAD_FALSE,
	.channel_mode = 0,
	.sample_rate = 0,
	.period_size = _MAD_DMA_READER_PERIOD_SIZE,
	.high_threshold = _MAD_DMA_READER_HIGH_THRESHOLD,
	.remain_size = 0,
};

static struct MStar_MAD_Ops MStar_DMA_Reader_Ops = {
	.open = _mhal_alsa_dma_reader_init,
	.close = _mhal_alsa_dma_reader_exit,
	.start = _mhal_alsa_dma_reader_start,
	.stop = _mhal_alsa_dma_reader_stop,
	.resume = _mhal_alsa_dma_reader_resume,
	.suspend = _mhal_alsa_dma_reader_suspend,
	.read = NULL,
	.write = _mhal_alsa_dma_reader_write,
	.get = _mhal_alsa_dma_reader_get,
	.set = _mhal_alsa_dma_reader_set,
};

/* MStar Audio DSP - PCM Playback2 */
static struct MStar_DMA_Reader_Struct g_pcm_playback2 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_PLAYBACK2_TOTAL_BUF_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.initialized_status = MAD_FALSE,
	.channel_mode = 0,
	.sample_rate = 0,
	.period_size = _MAD_PCM_PLAYBACK2_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_PLAYBACK2_HIGH_THRESHOLD,
	.remain_size = 0,
};

static struct MStar_MAD_Ops MStar_PCM_Playback2_Ops = {
	.open = _mhal_alsa_pcm_playback2_init,
	.close = _mhal_alsa_pcm_playback2_exit,
	.start = _mhal_alsa_pcm_playback2_start,
	.stop = _mhal_alsa_pcm_playback2_stop,
	.resume = _mhal_alsa_pcm_playback2_resume,
	.suspend = _mhal_alsa_pcm_playback2_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_playback2_write,
	.get = _mhal_alsa_pcm_playback2_get,
	.set = _mhal_alsa_pcm_playback2_set,
};

/* MStar Audio DSP - PCM Mixer3 */
static struct MStar_PCM_Mixer_Struct g_pcm_mixer3 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info[0] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.str_mode_info[1] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.pcm_mixer_element_info = NULL,
	.pcm_mixer_element_index = 0,
	.initialized_status = MAD_FALSE,
	.period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD,
};

static struct MStar_MAD_Ops MStar_PCM_Mixer3_Ops = {
	.open = _mhal_alsa_pcm_mixer3_init,
	.close = _mhal_alsa_pcm_mixer3_exit,
	.start = _mhal_alsa_pcm_mixer3_start,
	.stop = _mhal_alsa_pcm_mixer3_stop,
	.resume = _mhal_alsa_pcm_mixer3_resume,
	.suspend = _mhal_alsa_pcm_mixer3_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_mixer3_write,
	.get = _mhal_alsa_pcm_mixer3_get,
	.set = _mhal_alsa_pcm_mixer3_set,
};

/* MStar Audio DSP - PCM Mixer4 */
static struct MStar_PCM_Mixer_Struct g_pcm_mixer4 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info[0] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.str_mode_info[1] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.pcm_mixer_element_info = NULL,
	.pcm_mixer_element_index = 0,
	.initialized_status = MAD_FALSE,
	.period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD,
};

static struct MStar_MAD_Ops MStar_PCM_Mixer4_Ops = {
	.open = _mhal_alsa_pcm_mixer4_init,
	.close = _mhal_alsa_pcm_mixer4_exit,
	.start = _mhal_alsa_pcm_mixer4_start,
	.stop = _mhal_alsa_pcm_mixer4_stop,
	.resume = _mhal_alsa_pcm_mixer4_resume,
	.suspend = _mhal_alsa_pcm_mixer4_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_mixer4_write,
	.get = _mhal_alsa_pcm_mixer4_get,
	.set = _mhal_alsa_pcm_mixer4_set,
};

/* MStar Audio DSP - PCM Mixer5 */
static struct MStar_PCM_Mixer_Struct g_pcm_mixer5 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info[0] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.str_mode_info[1] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.pcm_mixer_element_info = NULL,
	.pcm_mixer_element_index = 0,
	.initialized_status = MAD_FALSE,
	.period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD,
};

static struct MStar_MAD_Ops MStar_PCM_Mixer5_Ops = {
	.open = _mhal_alsa_pcm_mixer5_init,
	.close = _mhal_alsa_pcm_mixer5_exit,
	.start = _mhal_alsa_pcm_mixer5_start,
	.stop = _mhal_alsa_pcm_mixer5_stop,
	.resume = _mhal_alsa_pcm_mixer5_resume,
	.suspend = _mhal_alsa_pcm_mixer5_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_mixer5_write,
	.get = _mhal_alsa_pcm_mixer5_get,
	.set = _mhal_alsa_pcm_mixer5_set,
};

/* MStar Audio DSP - PCM Mixer6 */
static struct MStar_PCM_Mixer_Struct g_pcm_mixer6 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info[0] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.str_mode_info[1] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.pcm_mixer_element_info = NULL,
	.pcm_mixer_element_index = 0,
	.initialized_status = MAD_FALSE,
	.period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD,
};

static struct MStar_MAD_Ops MStar_PCM_Mixer6_Ops = {
	.open = _mhal_alsa_pcm_mixer6_init,
	.close = _mhal_alsa_pcm_mixer6_exit,
	.start = _mhal_alsa_pcm_mixer6_start,
	.stop = _mhal_alsa_pcm_mixer6_stop,
	.resume = _mhal_alsa_pcm_mixer6_resume,
	.suspend = _mhal_alsa_pcm_mixer6_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_mixer6_write,
	.get = _mhal_alsa_pcm_mixer6_get,
	.set = _mhal_alsa_pcm_mixer6_set,
};

/* MStar Audio DSP - PCM Mixer7 */
static struct MStar_PCM_Mixer_Struct g_pcm_mixer7 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info[0] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.str_mode_info[1] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.pcm_mixer_element_info = NULL,
	.pcm_mixer_element_index = 0,
	.initialized_status = MAD_FALSE,
	.period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD,
	.written_size = 0,
};

static struct MStar_MAD_Ops MStar_PCM_Mixer7_Ops = {
	.open = _mhal_alsa_pcm_mixer7_init,
	.close = _mhal_alsa_pcm_mixer7_exit,
	.start = _mhal_alsa_pcm_mixer7_start,
	.stop = _mhal_alsa_pcm_mixer7_stop,
	.resume = _mhal_alsa_pcm_mixer7_resume,
	.suspend = _mhal_alsa_pcm_mixer7_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_mixer7_write,
	.get = _mhal_alsa_pcm_mixer7_get,
	.set = _mhal_alsa_pcm_mixer7_set,
};

/* MStar Audio DSP - PCM Mixer8 */
static struct MStar_PCM_Mixer_Struct g_pcm_mixer8 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info[0] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.str_mode_info[1] = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.pcm_mixer_element_info = NULL,
	.pcm_mixer_element_index = 0,
	.initialized_status = MAD_FALSE,
	.period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE,
	.high_threshold = _MAD_PCM_MIXER_ELEMENT_HIGH_THRESHOLD,
};

static struct MStar_MAD_Ops MStar_PCM_Mixer8_Ops = {
	.open = _mhal_alsa_pcm_mixer8_init,
	.close = _mhal_alsa_pcm_mixer8_exit,
	.start = _mhal_alsa_pcm_mixer8_start,
	.stop = _mhal_alsa_pcm_mixer8_stop,
	.resume = _mhal_alsa_pcm_mixer8_resume,
	.suspend = _mhal_alsa_pcm_mixer8_suspend,
	.read = NULL,
	.write = _mhal_alsa_pcm_mixer8_write,
	.get = _mhal_alsa_pcm_mixer8_get,
	.set = _mhal_alsa_pcm_mixer8_set,
};

/* MStar Audio DSP - PCM Capture1 */
static struct MStar_PCM_Capture_Struct g_pcm_capture1 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_CAPTURE_BUF_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.initialized_status = MAD_FALSE,
	.channel_mode = 2,
	.sample_rate = 48000,
};

static struct MStar_MAD_Ops MStar_PCM_Capture1_Ops = {
	.open = _mhal_alsa_pcm_capture1_init,
	.close = _mhal_alsa_pcm_capture1_exit,
	.start = _mhal_alsa_pcm_capture1_start,
	.stop = _mhal_alsa_pcm_capture1_stop,
	.resume = _mhal_alsa_pcm_capture1_resume,
	.suspend = _mhal_alsa_pcm_capture1_suspend,
	.read = _mhal_alsa_pcm_capture1_read,
	.write = NULL,
	.get = _mhal_alsa_pcm_capture1_get,
	.set = _mhal_alsa_pcm_capture1_set,
};

/* MStar Audio DSP - PCM Capture2 */
static struct MStar_PCM_Capture_Struct g_pcm_capture2 = {
	.buffer = {
		.addr = NULL,
		.size = _MAD_PCM_CAPTURE_BUF_SIZE,
		.w_ptr = NULL,
		.r_ptr = NULL,
	},
	.str_mode_info = {
		.status = E_STOP,
		.physical_addr = 0,
		.bus_addr = 0,
		.virtual_addr = 0,
	},
	.initialized_status = MAD_FALSE,
	.channel_mode = 2,
	.sample_rate = 48000,
};

static struct MStar_MAD_Ops MStar_PCM_Capture2_Ops = {
	.open = _mhal_alsa_pcm_capture2_init,
	.close = _mhal_alsa_pcm_capture2_exit,
	.start = _mhal_alsa_pcm_capture2_start,
	.stop = _mhal_alsa_pcm_capture2_stop,
	.resume = _mhal_alsa_pcm_capture2_resume,
	.suspend = _mhal_alsa_pcm_capture2_suspend,
	.read = _mhal_alsa_pcm_capture2_read,
	.write = NULL,
	.get = _mhal_alsa_pcm_capture2_get,
	.set = _mhal_alsa_pcm_capture2_set,
};

static unsigned int g_pcm_dmaRdr1_base_va = 0;
static unsigned int g_pcm_dmaRdr2_base_va = 0;
static unsigned int g_pcm_capture1_base_va = 0;
static unsigned int g_pcm_capture2_base_va = 0;
static unsigned int g_pcm_mixer_info_base_va = 0;
static unsigned int g_pcm_mixer_buf_base_va = 0;

/*
 * ============================================================================
 * Function Implementations
 * ============================================================================
 */
#if 0 /* It's not in used for the moment, might be TODO */
static unsigned char _mhal_alsa_read_byte(unsigned int u32RegAddr)
{
	return (_MAD_AU_AbsReadByte(u32RegAddr+_MAD_MAILBOX_OFFSET));
}
#endif

static unsigned short _mhal_alsa_read_reg(unsigned int u32RegAddr)
{
	return (_MAD_AU_AbsRead2Byte(u32RegAddr+_MAD_MAILBOX_OFFSET));
}

#if 0 /* It's not in used for the moment, might be TODO */
static void _mhal_alsa_write_byte(unsigned int u32RegAddr, unsigned char u8Val)
{
	_MAD_AU_AbsWriteByte((u32RegAddr+_MAD_MAILBOX_OFFSET), u8Val);
}
#endif

static void _mhal_alsa_write_mask_byte(unsigned int u32RegAddr, unsigned char u8Mask, unsigned char u8Val)
{
	_MAD_AU_AbsWriteMaskByte((u32RegAddr+_MAD_MAILBOX_OFFSET), u8Mask, u8Val);
}

static void _mhal_alsa_write_reg(unsigned int u32RegAddr, unsigned short u16Val)
{
	_MAD_AU_AbsWrite2Byte((u32RegAddr+_MAD_MAILBOX_OFFSET), u16Val);
}

static void _mhal_alsa_write_mask_reg(unsigned int u32RegAddr, unsigned short u16Mask, unsigned short u16Val)
{
	unsigned short u16RegVal;

	u16RegVal = _mhal_alsa_read_reg(u32RegAddr);
	u16RegVal = ((u16RegVal & (~(u16Mask))) | (u16Val & u16Mask));
	_mhal_alsa_write_reg(u32RegAddr, u16RegVal);
}

static unsigned int _mhal_alsa_get_device_status(void)
{
	if (((_mhal_alsa_read_reg(0x2D30) & 0x0200) == 0x0200) || (_mhal_alsa_read_reg(0x2A90) == 0x0000)) {
		return MAD_FALSE;
	}
	else {
		return MAD_TRUE;
	}
}

/* Initiate DMA Reader */
static int _mhal_alsa_dma_reader_init(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	unsigned int audio_pcm_dmaRdr_bufSz = 0;  /* must be multiple of DMA_RDR_PCM_BUF_UNIT*2 (= 0x2000) */
	unsigned int audio_pcm_dmaRdr_base_pa = 0; /* DMA Reader Input buffer (DM_Prefetch) */
	unsigned int audio_pcm_dmaRdr_base_ba = 0;
	unsigned int audio_pcm_dmaRdr_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Playback1 engine\n");

	if ((dma_reader->initialized_status != MAD_TRUE) || (dma_reader->str_mode_info.status != E_RESUME)) {
		audio_pcm_dmaRdr_bufSz = dma_reader->buffer.size;
		audio_pcm_dmaRdr_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_DMA_READER_BASE_OFFSET);
		audio_pcm_dmaRdr_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_dmaRdr_base_ba = audio_pcm_dmaRdr_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if (audio_pcm_dmaRdr_base_pa >= 0xA0000000) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM reader address!\n");
			return -EFAULT;
		}
		else if ((audio_pcm_dmaRdr_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM reader bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_dmaRdr1_base_va == 0) {
			g_pcm_dmaRdr1_base_va = (unsigned int)ioremap_nocache(audio_pcm_dmaRdr_base_ba, audio_pcm_dmaRdr_bufSz);
			if (g_pcm_dmaRdr1_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Playback1 Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_dmaRdr_base_va = g_pcm_dmaRdr1_base_va;

		dma_reader->str_mode_info.physical_addr = audio_pcm_dmaRdr_base_pa;
		dma_reader->str_mode_info.bus_addr = audio_pcm_dmaRdr_base_ba;
		dma_reader->str_mode_info.virtual_addr = audio_pcm_dmaRdr_base_va;

		dma_reader->initialized_status = MAD_TRUE;
	}
	else {
		audio_pcm_dmaRdr_bufSz = dma_reader->buffer.size;
		audio_pcm_dmaRdr_base_pa = dma_reader->str_mode_info.physical_addr;
		audio_pcm_dmaRdr_base_ba = dma_reader->str_mode_info.bus_addr;
		audio_pcm_dmaRdr_base_va = dma_reader->str_mode_info.virtual_addr;
	}

	/* init DMA writer address */
	dma_reader->buffer.addr = (unsigned char *)audio_pcm_dmaRdr_base_va;
	dma_reader->buffer.w_ptr = dma_reader->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Playback1 buffer start address = 0x%08X\n", dma_reader->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Playback1 buffer end address = 0x%08X\n", (dma_reader->buffer.addr + dma_reader->buffer.size));

	/* initial DMA Reader path & clk select (DMA reader -> CH8) */
	_mhal_alsa_write_mask_byte(0x2CB0, 0x07, 0x04); /* SEL_CLK_DMA_READER */
	_mhal_alsa_write_mask_byte(0x2C6B, 0xFF, 0x8F); /* CH8 sel to DMA Rdr */

	_mhal_alsa_write_mask_reg(0x2B82, 0xFFFF, 0x0000); /* reset dma reader */
	_mhal_alsa_write_mask_byte(0x2B8E, 0xFF, ((audio_pcm_dmaRdr_base_pa / _MAD_BYTES_IN_LINE) & 0xFF)); /* RD_BASE_ADDR[7:0] */
	_mhal_alsa_write_mask_reg(0x2B84, 0xFFFF, (((audio_pcm_dmaRdr_base_pa / _MAD_BYTES_IN_LINE) >> 8) & 0xFFFF)); /* RD_BASE_ADDR[23:8] */
	_mhal_alsa_write_mask_byte(0x2B8F, 0x0F, (((audio_pcm_dmaRdr_base_pa / _MAD_BYTES_IN_LINE) >> 24) & 0x0F)); /* RD_BASE_ADDR[27:24] */
	_mhal_alsa_write_mask_reg(0x2B86, 0xFFFF, audio_pcm_dmaRdr_bufSz / _MAD_BYTES_IN_LINE); /* setting : DMA Reader Size */
	_mhal_alsa_write_mask_reg(0x2B88, 0xFFFF, audio_pcm_dmaRdr_bufSz - 12); /* setting : DMA Reader Overrun Thr */
	_mhal_alsa_write_mask_reg(0x2B8A, 0xFFFF, 0x0012); /* setting : DMA Reader Underrun Thr */

	/* reset and start DMA Reader */
	_mhal_alsa_write_mask_reg(0x2B80, 0xFFFF, 0x6402);
	_mhal_alsa_write_mask_reg(0x2B80, 0xFFFF, 0x2402);

	/* reset remain size */
	dma_reader->remain_size = 0;

	/* clear DMA reader buffer */
	memset((void *)dma_reader->buffer.addr, 0x00, dma_reader->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit DMA Reader */
static int _mhal_alsa_dma_reader_exit(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Playback1 engine\n");

	dma_reader->str_mode_info.status = E_STOP;
	dma_reader->initialized_status = MAD_FALSE;

	return 0;
}

/* Start DMA Reader */
static int _mhal_alsa_dma_reader_start(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Playback1 engine\n");

	/* Initial DMA Reader path & clk select (DMA reader -> CH8) */
	_mhal_alsa_write_mask_byte(0x2CB0, 0x07, 0x04); /* SEL_CLK_DMA_READER */
	_mhal_alsa_write_mask_byte(0x2C6B, 0xFF, 0x8F); /* CH8 sel to DMA Rdr */

	_mhal_alsa_write_mask_reg(0x2B80, 0x0001, 0x0001);
	_mhal_alsa_write_mask_reg(0x2B80, 0x0100, 0x0100);

	dma_reader->str_mode_info.status = E_START;

	return 0;
}

/* Stop DMA Reader */
static int _mhal_alsa_dma_reader_stop(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Playback1 engine\n");

	/* reset dma reader */
	_mhal_alsa_write_mask_reg(0x2B82, 0xFFFF, 0x0000); /* clear wr cntrs */

	/* reset and Start DMA Reader */
	_mhal_alsa_write_mask_reg(0x2B80, 0xFFFE, 0x6402);
	_mhal_alsa_write_mask_reg(0x2B80, 0xFFFE, 0x2402);

	/* reset Write Pointer */
	dma_reader->buffer.w_ptr = dma_reader->buffer.addr;

	_mhal_alsa_write_mask_reg(0x2B80, 0x0001, 0x0000);
	_mhal_alsa_write_mask_reg(0x2B80, 0x0100, 0x0000);

	return 0;
}

/* Resume DMA Reader */
static int _mhal_alsa_dma_reader_resume(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Playback1 engine\n");

	dma_reader->str_mode_info.status = E_RESUME;

	return 0;
}

/* Suspend DMA Reader */
static int _mhal_alsa_dma_reader_suspend(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Playback1 engine\n");

	dma_reader->str_mode_info.status = E_SUSPEND;

	return 0;
}

/* Write PCM to DMA Reader */
static unsigned int _mhal_alsa_dma_reader_write(void *buffer, unsigned int bytes)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	bool replay_flag = MAD_FALSE;
	unsigned char *bufptr = (unsigned char *)buffer;
	unsigned char tmp_bufptr1 = 0;
	unsigned char tmp_bufptr2 = 0;
	int loop = 0;
	int inused_lines = 0;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;

	inused_lines = _mhal_alsa_dma_reader_get_inused_lines();
	if (inused_lines <= 6) {
		if ((_mhal_alsa_read_reg(0x2B80) & 0x0001) == 0x0001)
			replay_flag = MAD_TRUE;

		/* reset dma reader */
		_mhal_alsa_write_mask_reg(0x2B82, 0xFFFF, 0x0000); /* clear wr cntrs */

		/* reset and Start DMA Reader */
		_mhal_alsa_write_mask_reg(0x2B80, 0xFFFE, 0x6402);
		_mhal_alsa_write_mask_reg(0x2B80, 0xFFFE, 0x2402);

		/* reset Write Pointer */
		dma_reader->buffer.w_ptr = dma_reader->buffer.addr;

		/* reset remain size */
		dma_reader->remain_size = 0;

		//MAD_PRINT(KERN_INFO "***** PCM Playback1 Buffer empty !! ***** \n");
	}

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (dma_reader->channel_mode == E_MONO) ? (bytes * 4) : (bytes * 2);

	/* copy data to DMA Reader buffer */
	if ( ((inused_lines * _MAD_BYTES_IN_LINE) + copy_size) < dma_reader->high_threshold) {
		if (dma_reader->channel_mode == E_MONO) {
			for (loop = 0; loop < copy_lr_sample; loop++) {
				tmp_bufptr1 = *bufptr++;
				tmp_bufptr2 = *bufptr++;

				*(dma_reader->buffer.w_ptr++) = 0;
				*(dma_reader->buffer.w_ptr++) = tmp_bufptr1;
				*(dma_reader->buffer.w_ptr++) = tmp_bufptr2;
				*(dma_reader->buffer.w_ptr++) = 0;
				*(dma_reader->buffer.w_ptr++) = 0;
				*(dma_reader->buffer.w_ptr++) = tmp_bufptr1;
				*(dma_reader->buffer.w_ptr++) = tmp_bufptr2;
				*(dma_reader->buffer.w_ptr++) = 0;

				if (dma_reader->buffer.w_ptr >= (dma_reader->buffer.addr + dma_reader->buffer.size))
					dma_reader->buffer.w_ptr = dma_reader->buffer.addr;
			}
		}
		else {
			for (loop = 0; loop < copy_lr_sample; loop++) {
				*(dma_reader->buffer.w_ptr++) = 0;
				*(dma_reader->buffer.w_ptr++) = *bufptr++;
				*(dma_reader->buffer.w_ptr++) = *bufptr++;
				*(dma_reader->buffer.w_ptr++) = 0;

				if (dma_reader->buffer.w_ptr >= (dma_reader->buffer.addr + dma_reader->buffer.size))
					dma_reader->buffer.w_ptr = dma_reader->buffer.addr;
			}
		}

		/* flush MIU */
		Chip_Flush_Memory();

		/* update copied size to DMA Reader */
		copy_size += dma_reader->remain_size;
		_mhal_alsa_write_mask_reg(0x2B82, 0xFFFF, (copy_size/_MAD_BYTES_IN_LINE));
		dma_reader->remain_size = copy_size % _MAD_BYTES_IN_LINE;

		if (replay_flag == MAD_TRUE) {
			_mhal_alsa_write_mask_reg(0x2B80, 0x0001, 0x0001);
			_mhal_alsa_write_mask_reg(0x2B80, 0x0100, 0x0100);
		}

		return bytes;
	}

	//MAD_PRINT(KERN_INFO "***** PCM Playback1 Buffer busy !! ***** \n");

	return 0;
}

/* Get information from DMA Reader */
static int _mhal_alsa_dma_reader_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Playback1\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;

			*param = dma_reader->buffer.size >> 1; /* Only return valid bytes */
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;

			*param = dma_reader->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;

			*param = dma_reader->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;

			*param = dma_reader->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(mad_rates) / sizeof(mad_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&mad_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = (_mhal_alsa_dma_reader_get_inused_lines() * _MAD_BYTES_IN_LINE) >> 1; /* Only return valid bytes */
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = (_mhal_alsa_dma_reader_get_avail_lines() * _MAD_BYTES_IN_LINE) >> 1; /* Only return valid bytes */
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;

			*param = dma_reader->str_mode_info.status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to DMA Reader */
static int _mhal_alsa_dma_reader_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Playback1\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_dma_reader_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_dma_reader_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get DMA Reader PCM buffer level */
static int _mhal_alsa_dma_reader_get_inused_lines(void)
{
	int inused_lines = 0;

	/* Mask LEVEL_CNT_MASK before read */
	_mhal_alsa_write_mask_reg(0x2B80, 0x0010, 0x0010);
	inused_lines = _mhal_alsa_read_reg(0x2B8C);
	_mhal_alsa_write_mask_reg(0x2B80, 0x0010, 0x0000);

	return inused_lines;
}

/* Get DMA Reader PCM avail bytes */
static int _mhal_alsa_dma_reader_get_avail_lines(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	int inused_lines = 0;
	int avail_lines = 0;

	inused_lines = _mhal_alsa_dma_reader_get_inused_lines();
	avail_lines = (dma_reader->high_threshold / _MAD_BYTES_IN_LINE) - inused_lines;
	if (avail_lines < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused lines %d!\n", inused_lines);
		avail_lines = 0;
	}

	return avail_lines;
}

/* Set smaple rate to DMA Reader */
static int _mhal_alsa_dma_reader_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	unsigned short synthrate, divisor;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	dma_reader->sample_rate = sample_rate;

	/* New DMA Reader setting
	 * Formula is :
	 * Synthesizer Rate = 216MHz / Divisor(1, 2, 4 or 8) * 1024 / 256 / Sampling Rate(32Khz, 44.1KHz or 48KHz)
	 */
	switch(sample_rate) {
		case 8000:
		{
			divisor = 2;
			synthrate = 0x6978;
			break;
		}

		case 11025:
		{
			divisor = 2;
			synthrate = 0x4C87;
			break;
		}

		case 12000:
		{
			divisor = 2;
			synthrate = 0x4650;
			break;
		}

		case 16000:
		{
			divisor = 1;
			synthrate = 0x6978;
			break;
		}

		case 22050:
		{
			divisor = 1;
			synthrate = 0x4C87;
			break;
		}

		case 24000:
		{
			divisor = 1;
			synthrate = 0x4650;
			break;
		}

		case 32000:
		{
			divisor = 0;
			synthrate = 0x6978;
			break;
		}

		case 44100:
		{
			divisor = 0;
			synthrate = 0x4C87;
			break;
		}

		case 48000:
		{
			divisor = 0;
			synthrate = 0x4650;
			break;
		}

		default:
		{
			MAD_PRINT(KERN_ERR "Error! un-supported sample rate %u !!!\n", sample_rate);
			divisor = 0;
			synthrate = 0x4650;
			dma_reader->sample_rate = 48000;
			break;
		}
	}

	/* synthersizer setting update (DMA reader (DVB4 clock) -> CH8) */
	_mhal_alsa_write_mask_byte(0x2CA4, 0x20, 0x20); /* enable DMA synthesizer */
	_mhal_alsa_write_mask_reg(0x2CB0, 0x6000, (divisor << 13)); /* Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH */
	_mhal_alsa_write_mask_reg(0x2C20, 0xFFFF, synthrate); /* DMA synthesizer N.F. */

	return 0;
}

/* Set channel mode to DMA Reader */
static int _mhal_alsa_dma_reader_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_dma_reader;
	unsigned int buffer_size = 0;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	dma_reader->channel_mode = channel_mode;
	buffer_size = ((dma_reader->sample_rate << dma_reader->channel_mode) * _MAD_DMA_READER_QUEUE_SIZE) / 1000;
	if ((buffer_size % _MAD_BYTES_IN_LINE))
		buffer_size += (_MAD_BYTES_IN_LINE - (buffer_size % _MAD_BYTES_IN_LINE));

	dma_reader->period_size = buffer_size >> 2;
	dma_reader->buffer.size = (dma_reader->channel_mode == E_MONO) ? (buffer_size * 4) : (buffer_size * 2);
	dma_reader->high_threshold = dma_reader->buffer.size - (dma_reader->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Playback2 */
static int _mhal_alsa_pcm_playback2_init(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	unsigned int audio_pcm_dmaRdr_base_pa = 0;
	unsigned int audio_pcm_dmaRdr_base_ba = 0;
	unsigned int audio_pcm_dmaRdr_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Playback2 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info.status != E_RESUME)) {
		audio_pcm_dmaRdr_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_PLAYBACK2_BASE_OFFSET);
		audio_pcm_dmaRdr_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_dmaRdr_base_ba = audio_pcm_dmaRdr_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if (audio_pcm_dmaRdr_base_pa >= 0xA0000000) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback address!\n");
			return -EFAULT;
		}
		else if ((audio_pcm_dmaRdr_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_dmaRdr2_base_va == 0) {
			g_pcm_dmaRdr2_base_va = (unsigned int)ioremap_nocache(audio_pcm_dmaRdr_base_ba, pcm_playback->buffer.size);
			if (g_pcm_dmaRdr2_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Playback2 Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_dmaRdr_base_va = g_pcm_dmaRdr2_base_va;

		pcm_playback->str_mode_info.physical_addr = audio_pcm_dmaRdr_base_pa;
		pcm_playback->str_mode_info.bus_addr = audio_pcm_dmaRdr_base_ba;
		pcm_playback->str_mode_info.virtual_addr = audio_pcm_dmaRdr_base_va;

		pcm_playback->initialized_status = MAD_TRUE;
	}
	else {
		audio_pcm_dmaRdr_base_pa = pcm_playback->str_mode_info.physical_addr;
		audio_pcm_dmaRdr_base_ba = pcm_playback->str_mode_info.bus_addr;
		audio_pcm_dmaRdr_base_va = pcm_playback->str_mode_info.virtual_addr;
	}

	/* init PCM playback2 buffer address */
	pcm_playback->buffer.addr = (unsigned char *)audio_pcm_dmaRdr_base_va;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Playback2 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Playback2 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* reset PCM playback2 engine */
	_mhal_alsa_write_mask_reg(0x2D36, 0xFFFF, 0x0001);

	/* clear PCM playback2 write pointer */
	_mhal_alsa_write_mask_reg(0x2D34, 0xFFFF, 0x0000);

	/* clear PCM playback2 pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Playback2 */
static int _mhal_alsa_pcm_playback2_exit(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Playback2 engine\n");

	/* reset PCM playback2 engine */
	_mhal_alsa_write_mask_reg(0x2D36, 0xFFFF, 0x0001);

	/* clear PCM playback2 write pointer */
	_mhal_alsa_write_mask_reg(0x2D34, 0xFFFF, 0x0000);

	pcm_playback->str_mode_info.status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Playback2 */
static int _mhal_alsa_pcm_playback2_start(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Playback2 engine\n");

	/* start PCM playback2 engine */
	_mhal_alsa_write_mask_reg(0x2D36, 0xFFFF, 0x0002);

	pcm_playback->str_mode_info.status = E_START;

	return 0;
}

/* Stop PCM Playback2 */
static int _mhal_alsa_pcm_playback2_stop(void)
{
	struct MStar_DMA_Reader_Struct *dma_reader = &g_pcm_playback2;
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Playback2 engine\n");

	/* reset PCM playback2 engine */
	_mhal_alsa_write_mask_reg(0x2D36, 0xFFFF, 0x0001);

	/* clear PCM playback2 write pointer */
	_mhal_alsa_write_mask_reg(0x2D34, 0xFFFF, 0x0000);

	/* reset Write Pointer */
	dma_reader->buffer.w_ptr = dma_reader->buffer.addr;

	return 0;
}

/* Resume PCM Playback2 */
static int _mhal_alsa_pcm_playback2_resume(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Playback2 engine\n");

	pcm_playback->str_mode_info.status = E_RESUME;

	return 0;
}

/* Suspend PCM Playback2 */
static int _mhal_alsa_pcm_playback2_suspend(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Playback2 engine\n");

	pcm_playback->str_mode_info.status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Playback2 */
static unsigned int _mhal_alsa_pcm_playback2_write(void *buffer, unsigned int bytes)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int w_ptr_offset = 0;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_playback2_get_inused_lines() * _MAD_BYTES_IN_LINE;

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Playback2 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Playback2 Buffer full !! ***** \n");
		return 0;
	}

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* flush MIU */
	Chip_Flush_Memory();

	/* update PCM playback2 write pointer */
	w_ptr_offset = pcm_playback->buffer.w_ptr - pcm_playback->buffer.addr;
	_mhal_alsa_write_mask_reg(0x2D34, 0xFFFF, (w_ptr_offset / _MAD_BYTES_IN_LINE));

	return bytes;
}

/* Get information from PCM Playback2 */
static int _mhal_alsa_pcm_playback2_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Playback2\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(mad_rates) / sizeof(mad_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&mad_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_playback2_get_inused_lines() * _MAD_BYTES_IN_LINE;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_playback2_get_avail_lines() * _MAD_BYTES_IN_LINE;
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;

			*param = pcm_playback->str_mode_info.status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Playback2 */
static int _mhal_alsa_pcm_playback2_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Playback2\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_playback2_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_playback2_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Playback2 buffer level */
static int _mhal_alsa_pcm_playback2_get_inused_lines(void)
{
	int inused_lines = 0;

	inused_lines = _mhal_alsa_read_reg(0x2DE0);

	return inused_lines;
}

/* Get PCM Playback2 avail bytes */
static int _mhal_alsa_pcm_playback2_get_avail_lines(void)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	int inused_lines = 0;
	int avail_lines = 0;

	inused_lines = _mhal_alsa_pcm_playback2_get_inused_lines();
	avail_lines = (pcm_playback->buffer.size / _MAD_BYTES_IN_LINE) - inused_lines;
	if (avail_lines < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused lines %d!\n", inused_lines);
		avail_lines = 0;
	}

	return avail_lines;
}

/* Set smaple rate to PCM Playback2 */
static int _mhal_alsa_pcm_playback2_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	unsigned short synthrate;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	switch(sample_rate) {
		case 8000:
		{
			synthrate = 0x6978;
			break;
		}

		case 11025:
		{
			synthrate = 0x4C87;
			break;
		}

		case 12000:
		{
			synthrate = 0x4650;
			break;
		}

		case 16000:
		{
			synthrate = 0x34BC;
			break;
		}

		case 22050:
		{
			synthrate = 0x2643;
			break;
		}

		case 24000:
		{
			synthrate = 0x2328;
			break;
		}

		case 32000:
		{
			synthrate = 0x1A5E;
			break;
		}

		case 44100:
		{
			synthrate = 0x1321;
			break;
		}

		case 48000:
		{
			synthrate = 0x1194;
			break;
		}

		default:
		{
			MAD_PRINT(KERN_ERR "Error! un-supported sample rate %u !!!\n", sample_rate);
			synthrate = 0x1194;
			pcm_playback->sample_rate = 48000;
			break;
		}
	}

	_mhal_alsa_write_mask_reg(0x2D56, 0xFFFF, synthrate);

	return 0;
}

/* Set channel mode to PCM Playback2 */
static int _mhal_alsa_pcm_playback2_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_DMA_Reader_Struct *pcm_playback = &g_pcm_playback2;
	unsigned int buffer_size = 0;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	buffer_size = _MAD_PCM_PLAYBACK2_TOTAL_BUF_SIZE >> 1;
	pcm_playback->period_size = buffer_size >> 2;
	pcm_playback->buffer.size = buffer_size;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_init(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info = NULL;
	unsigned int pcm_mixer_element_index = 2; /* defualt is 3rd PCM Mixer Element */
	unsigned int audio_pcm_mixer_info_base_pa = 0;
	unsigned int audio_pcm_mixer_info_base_ba = 0;
	unsigned int audio_pcm_mixer_info_base_va = 0;
	unsigned int audio_pcm_mixer_buf_base_pa = 0;
	unsigned int audio_pcm_mixer_buf_base_ba = 0;
	unsigned int audio_pcm_mixer_buf_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Mixer3 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info[0].status != E_RESUME)) {
		audio_pcm_mixer_info_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET);
		audio_pcm_mixer_info_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_info_base_ba = audio_pcm_mixer_info_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		audio_pcm_mixer_buf_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET);
		audio_pcm_mixer_buf_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_buf_base_ba = audio_pcm_mixer_buf_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if ((audio_pcm_mixer_info_base_ba % 0x1000) || (audio_pcm_mixer_buf_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_mixer_info_base_va == 0) {
			g_pcm_mixer_info_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_info_base_ba, _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE);
			if (g_pcm_mixer_info_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Info Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_info_base_va = g_pcm_mixer_info_base_va;

		if (g_pcm_mixer_buf_base_va == 0) {
			g_pcm_mixer_buf_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_buf_base_ba, _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE);
			if (g_pcm_mixer_buf_base_va == 0) {
				iounmap((void *)g_pcm_mixer_info_base_va);
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Buffer Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_buf_base_va = g_pcm_mixer_buf_base_va;

		pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)(audio_pcm_mixer_info_base_va + (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct)));
		if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
			pcm_mixer_element_info->is_connected = MAD_TRUE;
		}
		else {
			pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;

			for (pcm_mixer_element_index = 0; pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT; pcm_mixer_element_index++) {
				if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
					pcm_mixer_element_info->is_connected = MAD_TRUE;
					break;
				}

				pcm_mixer_element_info++;
			}
		}

		if (pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT) {
			MAD_PRINT(KERN_INFO "Get MStar PCM Mixer3 Element on PCM Mixer Element %u\n", pcm_mixer_element_index);

			audio_pcm_mixer_info_base_pa += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_ba += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_va += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			pcm_playback->str_mode_info[0].physical_addr = audio_pcm_mixer_info_base_pa;
			pcm_playback->str_mode_info[0].bus_addr = audio_pcm_mixer_info_base_ba;
			pcm_playback->str_mode_info[0].virtual_addr = audio_pcm_mixer_info_base_va;

			audio_pcm_mixer_buf_base_pa += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_ba += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_va += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			pcm_playback->str_mode_info[1].physical_addr = audio_pcm_mixer_buf_base_pa;
			pcm_playback->str_mode_info[1].bus_addr = audio_pcm_mixer_buf_base_ba;
			pcm_playback->str_mode_info[1].virtual_addr = audio_pcm_mixer_buf_base_va;

			pcm_playback->initialized_status = MAD_TRUE;
			pcm_playback->pcm_mixer_element_index = pcm_mixer_element_index;
		}
		else {
			MAD_PRINT(KERN_ERR "Error! No available PCM Mixer Element for PCM Mixer3!\n");
			iounmap((void *)g_pcm_mixer_info_base_va);
			iounmap((void *)g_pcm_mixer_buf_base_va);
			return -EBUSY;
		}
	}
	else {
		audio_pcm_mixer_info_base_pa = pcm_playback->str_mode_info[0].physical_addr;
		audio_pcm_mixer_info_base_ba = pcm_playback->str_mode_info[0].bus_addr;
		audio_pcm_mixer_info_base_va = pcm_playback->str_mode_info[0].virtual_addr;

		audio_pcm_mixer_buf_base_pa = pcm_playback->str_mode_info[1].physical_addr;
		audio_pcm_mixer_buf_base_ba = pcm_playback->str_mode_info[1].bus_addr;
		audio_pcm_mixer_buf_base_va = pcm_playback->str_mode_info[1].virtual_addr;
	}

	/* init PCM Mixer element info */
	pcm_playback->pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;
	pcm_playback->pcm_mixer_element_info->struct_version = _MAD_PCM_MIXER_VERSION;
	pcm_playback->pcm_mixer_element_info->struct_size = sizeof(struct MStar_PCM_Mixer_Element_Info_Struct);
	pcm_playback->pcm_mixer_element_info->is_connected = MAD_TRUE;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_FALSE;
	pcm_playback->pcm_mixer_element_info->group = _MAD_PCM_MIXER_GROUP_ALL;
	pcm_playback->pcm_mixer_element_info->buffer = audio_pcm_mixer_buf_base_va;
	pcm_playback->pcm_mixer_element_info->buffer_size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->pcm_mixer_element_info->read_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->write_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->channel = pcm_playback->channel_mode;
	pcm_playback->pcm_mixer_element_info->sample_rate = pcm_playback->sample_rate;
	pcm_playback->pcm_mixer_element_info->bit_width = 16;
	pcm_playback->pcm_mixer_element_info->endien = 0;
	pcm_playback->pcm_mixer_element_info->timestamp = 0;
	pcm_playback->pcm_mixer_element_info->weighting = 100;
	pcm_playback->pcm_mixer_element_info->volume = 96;
	pcm_playback->pcm_mixer_element_info->readTime = 0;
	pcm_playback->pcm_mixer_element_info->writeTime = 0;
	pcm_playback->pcm_mixer_element_info->bufferLevel = 0;

	/* init PCM Mixer buffer address */
	pcm_playback->buffer.addr = (unsigned char *)pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	pcm_playback->buffer.r_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Mixer3 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Mixer3 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* clear PCM Mixer pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_exit(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Mixer3 engine\n");

	/* clear pcm buffer */
	if (pcm_playback->buffer.addr != NULL) {
		memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	}
	pcm_playback->buffer.addr = NULL;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
	pcm_playback->buffer.w_ptr = NULL;
	pcm_playback->buffer.r_ptr = NULL;

	/* clear element info */
	if (pcm_playback->pcm_mixer_element_info != NULL) {
		memset((void *)pcm_playback->pcm_mixer_element_info, 0x00, sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
	}
	pcm_playback->pcm_mixer_element_info = NULL;

	pcm_playback->str_mode_info[0].status = E_STOP;
	pcm_playback->str_mode_info[1].status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_start(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Mixer3 engine\n");

	pcm_playback->str_mode_info[0].status = E_START;
	pcm_playback->str_mode_info[1].status = E_START;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_TRUE;

	return 0;
}

/* Stop PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Mixer3 engine\n");

	return 0;
}

/* Resume PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_resume(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Mixer3 engine\n");

	pcm_playback->str_mode_info[0].status = E_RESUME;
	pcm_playback->str_mode_info[1].status = E_RESUME;

	return 0;
}

/* Suspend PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_suspend(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Mixer3 engine\n");

	pcm_playback->str_mode_info[0].status = E_SUSPEND;
	pcm_playback->str_mode_info[1].status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Mixer3 */
static unsigned int _mhal_alsa_pcm_mixer3_write(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_mixer3_get_inused_bytes();

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Mixer3 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer3 Buffer full !! ***** \n");
		return 0;
	}

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* update PCM Mixer write pointer */
	pcm_playback->pcm_mixer_element_info->write_ptr = (unsigned int)pcm_playback->buffer.w_ptr;

	/* flush MIU */
	Chip_Flush_Memory();

	return bytes;
}

/* Get information from PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Mixer3\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(pcm_mixer_rates) / sizeof(pcm_mixer_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&pcm_mixer_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer3_get_inused_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer3_get_avail_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;

			*param = pcm_playback->str_mode_info[0].status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Mixer3\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_mixer3_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_mixer3_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Mixer3 buffer level */
static int _mhal_alsa_pcm_mixer3_get_inused_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	int inused_bytes = 0;

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;
	pcm_playback->buffer.r_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->read_ptr;

	inused_bytes = pcm_playback->buffer.w_ptr - pcm_playback->buffer.r_ptr;
	if (inused_bytes < 0) {
		inused_bytes += pcm_playback->buffer.size;
	}

	return inused_bytes;
}

/* Get PCM Mixer3 avail bytes */
static int _mhal_alsa_pcm_mixer3_get_avail_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	int inused_bytes = 0;
	int avail_bytes = 0;

	inused_bytes = _mhal_alsa_pcm_mixer3_get_inused_bytes();
	avail_bytes = pcm_playback->buffer.size - inused_bytes;
	if (avail_bytes < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused bytes %d!\n", inused_bytes);
		avail_bytes = 0;
	}

	return avail_bytes;
}

/* Set smaple rate to PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	return 0;
}

/* Set channel mode to PCM Mixer3 */
static int _mhal_alsa_pcm_mixer3_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer3;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	pcm_playback->period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_init(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info = NULL;
	unsigned int pcm_mixer_element_index = 3; /* defualt is 4th PCM Mixer Element */
	unsigned int audio_pcm_mixer_info_base_pa = 0;
	unsigned int audio_pcm_mixer_info_base_ba = 0;
	unsigned int audio_pcm_mixer_info_base_va = 0;
	unsigned int audio_pcm_mixer_buf_base_pa = 0;
	unsigned int audio_pcm_mixer_buf_base_ba = 0;
	unsigned int audio_pcm_mixer_buf_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Mixer4 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info[0].status != E_RESUME)) {
		audio_pcm_mixer_info_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET);
		audio_pcm_mixer_info_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_info_base_ba = audio_pcm_mixer_info_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		audio_pcm_mixer_buf_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET);
		audio_pcm_mixer_buf_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_buf_base_ba = audio_pcm_mixer_buf_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if ((audio_pcm_mixer_info_base_ba % 0x1000) || (audio_pcm_mixer_buf_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_mixer_info_base_va == 0) {
			g_pcm_mixer_info_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_info_base_ba, _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE);
			if (g_pcm_mixer_info_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Info Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_info_base_va = g_pcm_mixer_info_base_va;

		if (g_pcm_mixer_buf_base_va == 0) {
			g_pcm_mixer_buf_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_buf_base_ba, _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE);
			if (g_pcm_mixer_buf_base_va == 0) {
				iounmap((void *)g_pcm_mixer_info_base_va);
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Buffer Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_buf_base_va = g_pcm_mixer_buf_base_va;

		pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)(audio_pcm_mixer_info_base_va + (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct)));
		if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
			pcm_mixer_element_info->is_connected = MAD_TRUE;
		}
		else {
			pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;

			for (pcm_mixer_element_index = 0; pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT; pcm_mixer_element_index++) {
				if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
					pcm_mixer_element_info->is_connected = MAD_TRUE;
					break;
				}

				pcm_mixer_element_info++;
			}
		}

		if (pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT) {
			MAD_PRINT(KERN_INFO "Get MStar PCM Mixer4 Element on PCM Mixer Element %u\n", pcm_mixer_element_index);

			audio_pcm_mixer_info_base_pa += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_ba += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_va += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			pcm_playback->str_mode_info[0].physical_addr = audio_pcm_mixer_info_base_pa;
			pcm_playback->str_mode_info[0].bus_addr = audio_pcm_mixer_info_base_ba;
			pcm_playback->str_mode_info[0].virtual_addr = audio_pcm_mixer_info_base_va;

			audio_pcm_mixer_buf_base_pa += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_ba += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_va += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			pcm_playback->str_mode_info[1].physical_addr = audio_pcm_mixer_buf_base_pa;
			pcm_playback->str_mode_info[1].bus_addr = audio_pcm_mixer_buf_base_ba;
			pcm_playback->str_mode_info[1].virtual_addr = audio_pcm_mixer_buf_base_va;

			pcm_playback->initialized_status = MAD_TRUE;
			pcm_playback->pcm_mixer_element_index = pcm_mixer_element_index;
		}
		else {
			MAD_PRINT(KERN_ERR "Error! No available PCM Mixer Element for PCM Mixer4!\n");
			iounmap((void *)g_pcm_mixer_info_base_va);
			iounmap((void *)g_pcm_mixer_buf_base_va);
			return -EBUSY;
		}
	}
	else {
		audio_pcm_mixer_info_base_pa = pcm_playback->str_mode_info[0].physical_addr;
		audio_pcm_mixer_info_base_ba = pcm_playback->str_mode_info[0].bus_addr;
		audio_pcm_mixer_info_base_va = pcm_playback->str_mode_info[0].virtual_addr;

		audio_pcm_mixer_buf_base_pa = pcm_playback->str_mode_info[1].physical_addr;
		audio_pcm_mixer_buf_base_ba = pcm_playback->str_mode_info[1].bus_addr;
		audio_pcm_mixer_buf_base_va = pcm_playback->str_mode_info[1].virtual_addr;
	}

	/* init PCM Mixer element info */
	pcm_playback->pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;
	pcm_playback->pcm_mixer_element_info->struct_version = _MAD_PCM_MIXER_VERSION;
	pcm_playback->pcm_mixer_element_info->struct_size = sizeof(struct MStar_PCM_Mixer_Element_Info_Struct);
	pcm_playback->pcm_mixer_element_info->is_connected = MAD_TRUE;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_FALSE;
	pcm_playback->pcm_mixer_element_info->group = _MAD_PCM_MIXER_GROUP_ALL;
	pcm_playback->pcm_mixer_element_info->buffer = audio_pcm_mixer_buf_base_va;
	pcm_playback->pcm_mixer_element_info->buffer_size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->pcm_mixer_element_info->read_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->write_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->channel = pcm_playback->channel_mode;
	pcm_playback->pcm_mixer_element_info->sample_rate = pcm_playback->sample_rate;
	pcm_playback->pcm_mixer_element_info->bit_width = 16;
	pcm_playback->pcm_mixer_element_info->endien = 0;
	pcm_playback->pcm_mixer_element_info->timestamp = 0;
	pcm_playback->pcm_mixer_element_info->weighting = 100;
	pcm_playback->pcm_mixer_element_info->volume = 96;
	pcm_playback->pcm_mixer_element_info->readTime = 0;
	pcm_playback->pcm_mixer_element_info->writeTime = 0;
	pcm_playback->pcm_mixer_element_info->bufferLevel = 0;

	/* init PCM Mixer buffer address */
	pcm_playback->buffer.addr = (unsigned char *)pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	pcm_playback->buffer.r_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Mixer4 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Mixer4 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* clear PCM Mixer pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_exit(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Mixer4 engine\n");

	/* clear pcm buffer */
	if (pcm_playback->buffer.addr != NULL) {
		memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	}
	pcm_playback->buffer.addr = NULL;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
	pcm_playback->buffer.w_ptr = NULL;
	pcm_playback->buffer.r_ptr = NULL;

	/* clear element info */
	if (pcm_playback->pcm_mixer_element_info != NULL) {
		memset((void *)pcm_playback->pcm_mixer_element_info, 0x00, sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
	}
	pcm_playback->pcm_mixer_element_info = NULL;

	pcm_playback->str_mode_info[0].status = E_STOP;
	pcm_playback->str_mode_info[1].status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_start(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Mixer4 engine\n");

	pcm_playback->str_mode_info[0].status = E_START;
	pcm_playback->str_mode_info[1].status = E_START;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_TRUE;

	return 0;
}

/* Stop PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Mixer4 engine\n");

	return 0;
}

/* Resume PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_resume(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Mixer4 engine\n");

	pcm_playback->str_mode_info[0].status = E_RESUME;
	pcm_playback->str_mode_info[1].status = E_RESUME;

	return 0;
}

/* Suspend PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_suspend(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Mixer4 engine\n");

	pcm_playback->str_mode_info[0].status = E_SUSPEND;
	pcm_playback->str_mode_info[1].status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Mixer4 */
static unsigned int _mhal_alsa_pcm_mixer4_write(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_mixer4_get_inused_bytes();

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Mixer4 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer4 Buffer full !! ***** \n");
		return 0;
	}

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* update PCM Mixer write pointer */
	pcm_playback->pcm_mixer_element_info->write_ptr = (unsigned int)pcm_playback->buffer.w_ptr;

	/* flush MIU */
	Chip_Flush_Memory();

	return bytes;
}

/* Get information from PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Mixer4\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(pcm_mixer_rates) / sizeof(pcm_mixer_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&pcm_mixer_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer4_get_inused_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer4_get_avail_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;

			*param = pcm_playback->str_mode_info[0].status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Mixer4\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_mixer4_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_mixer4_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Mixer4 buffer level */
static int _mhal_alsa_pcm_mixer4_get_inused_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	int inused_bytes = 0;

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;
	pcm_playback->buffer.r_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->read_ptr;

	inused_bytes = pcm_playback->buffer.w_ptr - pcm_playback->buffer.r_ptr;
	if (inused_bytes < 0) {
		inused_bytes += pcm_playback->buffer.size;
	}

	return inused_bytes;
}

/* Get PCM Mixer4 avail bytes */
static int _mhal_alsa_pcm_mixer4_get_avail_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	int inused_bytes = 0;
	int avail_bytes = 0;

	inused_bytes = _mhal_alsa_pcm_mixer4_get_inused_bytes();
	avail_bytes = pcm_playback->buffer.size - inused_bytes;
	if (avail_bytes < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused bytes %d!\n", inused_bytes);
		avail_bytes = 0;
	}

	return avail_bytes;
}

/* Set smaple rate to PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	return 0;
}

/* Set channel mode to PCM Mixer4 */
static int _mhal_alsa_pcm_mixer4_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer4;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	pcm_playback->period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_init(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info = NULL;
	unsigned int pcm_mixer_element_index = 4; /* defualt is 5th PCM Mixer Element */
	unsigned int audio_pcm_mixer_info_base_pa = 0;
	unsigned int audio_pcm_mixer_info_base_ba = 0;
	unsigned int audio_pcm_mixer_info_base_va = 0;
	unsigned int audio_pcm_mixer_buf_base_pa = 0;
	unsigned int audio_pcm_mixer_buf_base_ba = 0;
	unsigned int audio_pcm_mixer_buf_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Mixer5 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info[0].status != E_RESUME)) {
		audio_pcm_mixer_info_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET);
		audio_pcm_mixer_info_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_info_base_ba = audio_pcm_mixer_info_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		audio_pcm_mixer_buf_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET);
		audio_pcm_mixer_buf_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_buf_base_ba = audio_pcm_mixer_buf_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if ((audio_pcm_mixer_info_base_ba % 0x1000) || (audio_pcm_mixer_buf_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_mixer_info_base_va == 0) {
			g_pcm_mixer_info_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_info_base_ba, _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE);
			if (g_pcm_mixer_info_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Info Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_info_base_va = g_pcm_mixer_info_base_va;

		if (g_pcm_mixer_buf_base_va == 0) {
			g_pcm_mixer_buf_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_buf_base_ba, _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE);
			if (g_pcm_mixer_buf_base_va == 0) {
				iounmap((void *)g_pcm_mixer_info_base_va);
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Buffer Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_buf_base_va = g_pcm_mixer_buf_base_va;

		pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)(audio_pcm_mixer_info_base_va + (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct)));
		if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
			pcm_mixer_element_info->is_connected = MAD_TRUE;
		}
		else {
			pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;

			for (pcm_mixer_element_index = 0; pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT; pcm_mixer_element_index++) {
				if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
					pcm_mixer_element_info->is_connected = MAD_TRUE;
					break;
				}

				pcm_mixer_element_info++;
			}
		}

		if (pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT) {
			MAD_PRINT(KERN_INFO "Get MStar PCM Mixer5 Element on PCM Mixer Element %u\n", pcm_mixer_element_index);

			audio_pcm_mixer_info_base_pa += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_ba += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_va += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			pcm_playback->str_mode_info[0].physical_addr = audio_pcm_mixer_info_base_pa;
			pcm_playback->str_mode_info[0].bus_addr = audio_pcm_mixer_info_base_ba;
			pcm_playback->str_mode_info[0].virtual_addr = audio_pcm_mixer_info_base_va;

			audio_pcm_mixer_buf_base_pa += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_ba += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_va += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			pcm_playback->str_mode_info[1].physical_addr = audio_pcm_mixer_buf_base_pa;
			pcm_playback->str_mode_info[1].bus_addr = audio_pcm_mixer_buf_base_ba;
			pcm_playback->str_mode_info[1].virtual_addr = audio_pcm_mixer_buf_base_va;

			pcm_playback->initialized_status = MAD_TRUE;
			pcm_playback->pcm_mixer_element_index = pcm_mixer_element_index;
		}
		else {
			MAD_PRINT(KERN_ERR "Error! No available PCM Mixer Element for PCM Mixer5!\n");
			iounmap((void *)g_pcm_mixer_info_base_va);
			iounmap((void *)g_pcm_mixer_buf_base_va);
			return -EBUSY;
		}
	}
	else {
		audio_pcm_mixer_info_base_pa = pcm_playback->str_mode_info[0].physical_addr;
		audio_pcm_mixer_info_base_ba = pcm_playback->str_mode_info[0].bus_addr;
		audio_pcm_mixer_info_base_va = pcm_playback->str_mode_info[0].virtual_addr;

		audio_pcm_mixer_buf_base_pa = pcm_playback->str_mode_info[1].physical_addr;
		audio_pcm_mixer_buf_base_ba = pcm_playback->str_mode_info[1].bus_addr;
		audio_pcm_mixer_buf_base_va = pcm_playback->str_mode_info[1].virtual_addr;
	}

	/* init PCM Mixer element info */
	pcm_playback->pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;
	pcm_playback->pcm_mixer_element_info->struct_version = _MAD_PCM_MIXER_VERSION;
	pcm_playback->pcm_mixer_element_info->struct_size = sizeof(struct MStar_PCM_Mixer_Element_Info_Struct);
	pcm_playback->pcm_mixer_element_info->is_connected = MAD_TRUE;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_FALSE;
	pcm_playback->pcm_mixer_element_info->group = _MAD_PCM_MIXER_GROUP_ALL;
	pcm_playback->pcm_mixer_element_info->buffer = audio_pcm_mixer_buf_base_va;
	pcm_playback->pcm_mixer_element_info->buffer_size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->pcm_mixer_element_info->read_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->write_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->channel = pcm_playback->channel_mode;
	pcm_playback->pcm_mixer_element_info->sample_rate = pcm_playback->sample_rate;
	pcm_playback->pcm_mixer_element_info->bit_width = 16;
	pcm_playback->pcm_mixer_element_info->endien = 0;
	pcm_playback->pcm_mixer_element_info->timestamp = 0;
	pcm_playback->pcm_mixer_element_info->weighting = 100;
	pcm_playback->pcm_mixer_element_info->volume = 96;
	pcm_playback->pcm_mixer_element_info->readTime = 0;
	pcm_playback->pcm_mixer_element_info->writeTime = 0;
	pcm_playback->pcm_mixer_element_info->bufferLevel = 0;

	/* init PCM Mixer buffer address */
	pcm_playback->buffer.addr = (unsigned char *)pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	pcm_playback->buffer.r_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Mixer5 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Mixer5 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* clear PCM Mixer pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_exit(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Mixer5 engine\n");

	/* clear pcm buffer */
	if (pcm_playback->buffer.addr != NULL) {
		memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	}
	pcm_playback->buffer.addr = NULL;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
	pcm_playback->buffer.w_ptr = NULL;
	pcm_playback->buffer.r_ptr = NULL;

	/* clear element info */
	if (pcm_playback->pcm_mixer_element_info != NULL) {
		memset((void *)pcm_playback->pcm_mixer_element_info, 0x00, sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
	}
	pcm_playback->pcm_mixer_element_info = NULL;

	pcm_playback->str_mode_info[0].status = E_STOP;
	pcm_playback->str_mode_info[1].status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_start(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Mixer5 engine\n");

	pcm_playback->str_mode_info[0].status = E_START;
	pcm_playback->str_mode_info[1].status = E_START;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_TRUE;

	return 0;
}

/* Stop PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Mixer5 engine\n");

	return 0;
}

/* Resume PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_resume(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Mixer5 engine\n");

	pcm_playback->str_mode_info[0].status = E_RESUME;
	pcm_playback->str_mode_info[1].status = E_RESUME;

	return 0;
}

/* Suspend PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_suspend(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Mixer5 engine\n");

	pcm_playback->str_mode_info[0].status = E_SUSPEND;
	pcm_playback->str_mode_info[1].status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Mixer5 */
static unsigned int _mhal_alsa_pcm_mixer5_write(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_mixer5_get_inused_bytes();

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Mixer5 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer5 Buffer full !! ***** \n");
		return 0;
	}

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* update PCM Mixer write pointer */
	pcm_playback->pcm_mixer_element_info->write_ptr = (unsigned int)pcm_playback->buffer.w_ptr;

	/* flush MIU */
	Chip_Flush_Memory();

	return bytes;
}

/* Get information from PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Mixer5\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(pcm_mixer_rates) / sizeof(pcm_mixer_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&pcm_mixer_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer5_get_inused_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer5_get_avail_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;

			*param = pcm_playback->str_mode_info[0].status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Mixer5\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_mixer5_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_mixer5_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Mixer5 buffer level */
static int _mhal_alsa_pcm_mixer5_get_inused_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	int inused_bytes = 0;

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;
	pcm_playback->buffer.r_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->read_ptr;

	inused_bytes = pcm_playback->buffer.w_ptr - pcm_playback->buffer.r_ptr;
	if (inused_bytes < 0) {
		inused_bytes += pcm_playback->buffer.size;
	}

	return inused_bytes;
}

/* Get PCM Mixer5 avail bytes */
static int _mhal_alsa_pcm_mixer5_get_avail_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	int inused_bytes = 0;
	int avail_bytes = 0;

	inused_bytes = _mhal_alsa_pcm_mixer5_get_inused_bytes();
	avail_bytes = pcm_playback->buffer.size - inused_bytes;
	if (avail_bytes < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused bytes %d!\n", inused_bytes);
		avail_bytes = 0;
	}

	return avail_bytes;
}

/* Set smaple rate to PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	return 0;
}

/* Set channel mode to PCM Mixer5 */
static int _mhal_alsa_pcm_mixer5_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer5;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	pcm_playback->period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_init(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info = NULL;
	unsigned int pcm_mixer_element_index = 5; /* defualt is 6th PCM Mixer Element */
	unsigned int audio_pcm_mixer_info_base_pa = 0;
	unsigned int audio_pcm_mixer_info_base_ba = 0;
	unsigned int audio_pcm_mixer_info_base_va = 0;
	unsigned int audio_pcm_mixer_buf_base_pa = 0;
	unsigned int audio_pcm_mixer_buf_base_ba = 0;
	unsigned int audio_pcm_mixer_buf_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Mixer6 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info[0].status != E_RESUME)) {
		audio_pcm_mixer_info_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET);
		audio_pcm_mixer_info_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_info_base_ba = audio_pcm_mixer_info_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		audio_pcm_mixer_buf_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET);
		audio_pcm_mixer_buf_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_buf_base_ba = audio_pcm_mixer_buf_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if ((audio_pcm_mixer_info_base_ba % 0x1000) || (audio_pcm_mixer_buf_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_mixer_info_base_va == 0) {
			g_pcm_mixer_info_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_info_base_ba, _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE);
			if (g_pcm_mixer_info_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Info Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_info_base_va = g_pcm_mixer_info_base_va;

		if (g_pcm_mixer_buf_base_va == 0) {
			g_pcm_mixer_buf_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_buf_base_ba, _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE);
			if (g_pcm_mixer_buf_base_va == 0) {
				iounmap((void *)g_pcm_mixer_info_base_va);
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Buffer Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_buf_base_va = g_pcm_mixer_buf_base_va;

		pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)(audio_pcm_mixer_info_base_va + (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct)));
		if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
			pcm_mixer_element_info->is_connected = MAD_TRUE;
		}
		else {
			pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;

			for (pcm_mixer_element_index = 0; pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT; pcm_mixer_element_index++) {
				if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
					pcm_mixer_element_info->is_connected = MAD_TRUE;
					break;
				}

				pcm_mixer_element_info++;
			}
		}

		if (pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT) {
			MAD_PRINT(KERN_INFO "Get MStar PCM Mixer6 Element on PCM Mixer Element %u\n", pcm_mixer_element_index);

			audio_pcm_mixer_info_base_pa += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_ba += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_va += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			pcm_playback->str_mode_info[0].physical_addr = audio_pcm_mixer_info_base_pa;
			pcm_playback->str_mode_info[0].bus_addr = audio_pcm_mixer_info_base_ba;
			pcm_playback->str_mode_info[0].virtual_addr = audio_pcm_mixer_info_base_va;

			audio_pcm_mixer_buf_base_pa += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_ba += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_va += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			pcm_playback->str_mode_info[1].physical_addr = audio_pcm_mixer_buf_base_pa;
			pcm_playback->str_mode_info[1].bus_addr = audio_pcm_mixer_buf_base_ba;
			pcm_playback->str_mode_info[1].virtual_addr = audio_pcm_mixer_buf_base_va;

			pcm_playback->initialized_status = MAD_TRUE;
			pcm_playback->pcm_mixer_element_index = pcm_mixer_element_index;
		}
		else {
			MAD_PRINT(KERN_ERR "Error! No available PCM Mixer Element for PCM Mixer6!\n");
			iounmap((void *)g_pcm_mixer_info_base_va);
			iounmap((void *)g_pcm_mixer_buf_base_va);
			return -EBUSY;
		}
	}
	else {
		audio_pcm_mixer_info_base_pa = pcm_playback->str_mode_info[0].physical_addr;
		audio_pcm_mixer_info_base_ba = pcm_playback->str_mode_info[0].bus_addr;
		audio_pcm_mixer_info_base_va = pcm_playback->str_mode_info[0].virtual_addr;

		audio_pcm_mixer_buf_base_pa = pcm_playback->str_mode_info[1].physical_addr;
		audio_pcm_mixer_buf_base_ba = pcm_playback->str_mode_info[1].bus_addr;
		audio_pcm_mixer_buf_base_va = pcm_playback->str_mode_info[1].virtual_addr;
	}

	/* init PCM Mixer element info */
	pcm_playback->pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;
	pcm_playback->pcm_mixer_element_info->struct_version = _MAD_PCM_MIXER_VERSION;
	pcm_playback->pcm_mixer_element_info->struct_size = sizeof(struct MStar_PCM_Mixer_Element_Info_Struct);
	pcm_playback->pcm_mixer_element_info->is_connected = MAD_TRUE;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_FALSE;
	pcm_playback->pcm_mixer_element_info->group = _MAD_PCM_MIXER_GROUP_ALL;
	pcm_playback->pcm_mixer_element_info->buffer = audio_pcm_mixer_buf_base_va;
	pcm_playback->pcm_mixer_element_info->buffer_size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->pcm_mixer_element_info->read_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->write_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->channel = pcm_playback->channel_mode;
	pcm_playback->pcm_mixer_element_info->sample_rate = pcm_playback->sample_rate;
	pcm_playback->pcm_mixer_element_info->bit_width = 16;
	pcm_playback->pcm_mixer_element_info->endien = 0;
	pcm_playback->pcm_mixer_element_info->timestamp = 0;
	pcm_playback->pcm_mixer_element_info->weighting = 100;
	pcm_playback->pcm_mixer_element_info->volume = 96;
	pcm_playback->pcm_mixer_element_info->readTime = 0;
	pcm_playback->pcm_mixer_element_info->writeTime = 0;
	pcm_playback->pcm_mixer_element_info->bufferLevel = 0;

	/* init PCM Mixer buffer address */
	pcm_playback->buffer.addr = (unsigned char *)pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	pcm_playback->buffer.r_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Mixer6 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Mixer6 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* clear PCM Mixer pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_exit(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Mixer6 engine\n");

	/* clear pcm buffer */
	if (pcm_playback->buffer.addr != NULL) {
		memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	}
	pcm_playback->buffer.addr = NULL;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
	pcm_playback->buffer.w_ptr = NULL;
	pcm_playback->buffer.r_ptr = NULL;

	/* clear element info */
	if (pcm_playback->pcm_mixer_element_info != NULL) {
		memset((void *)pcm_playback->pcm_mixer_element_info, 0x00, sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
	}
	pcm_playback->pcm_mixer_element_info = NULL;

	pcm_playback->str_mode_info[0].status = E_STOP;
	pcm_playback->str_mode_info[1].status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_start(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Mixer6 engine\n");

	pcm_playback->str_mode_info[0].status = E_START;
	pcm_playback->str_mode_info[1].status = E_START;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_TRUE;

	return 0;
}

/* Stop PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Mixer6 engine\n");

	return 0;
}

/* Resume PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_resume(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Mixer6 engine\n");

	pcm_playback->str_mode_info[0].status = E_RESUME;
	pcm_playback->str_mode_info[1].status = E_RESUME;

	return 0;
}

/* Suspend PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_suspend(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Mixer6 engine\n");

	pcm_playback->str_mode_info[0].status = E_SUSPEND;
	pcm_playback->str_mode_info[1].status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Mixer6 */
static unsigned int _mhal_alsa_pcm_mixer6_write(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_mixer6_get_inused_bytes();

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Mixer6 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer6 Buffer full !! ***** \n");
		return 0;
	}

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* update PCM Mixer write pointer */
	pcm_playback->pcm_mixer_element_info->write_ptr = (unsigned int)pcm_playback->buffer.w_ptr;

	/* flush MIU */
	Chip_Flush_Memory();

	return bytes;
}

/* Get information from PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Mixer6\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(pcm_mixer_rates) / sizeof(pcm_mixer_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&pcm_mixer_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer6_get_inused_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer6_get_avail_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;

			*param = pcm_playback->str_mode_info[0].status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Mixer6\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_mixer6_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_mixer6_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Mixer6 buffer level */
static int _mhal_alsa_pcm_mixer6_get_inused_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	int inused_bytes = 0;

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;
	pcm_playback->buffer.r_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->read_ptr;

	inused_bytes = pcm_playback->buffer.w_ptr - pcm_playback->buffer.r_ptr;
	if (inused_bytes < 0) {
		inused_bytes += pcm_playback->buffer.size;
	}

	return inused_bytes;
}

/* Get PCM Mixer6 avail bytes */
static int _mhal_alsa_pcm_mixer6_get_avail_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	int inused_bytes = 0;
	int avail_bytes = 0;

	inused_bytes = _mhal_alsa_pcm_mixer6_get_inused_bytes();
	avail_bytes = pcm_playback->buffer.size - inused_bytes;
	if (avail_bytes < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused bytes %d!\n", inused_bytes);
		avail_bytes = 0;
	}

	return avail_bytes;
}

/* Set smaple rate to PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	return 0;
}

/* Set channel mode to PCM Mixer6 */
static int _mhal_alsa_pcm_mixer6_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer6;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	pcm_playback->period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_init(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info = NULL;
	unsigned int pcm_mixer_element_index = 6; /* defualt is 7th PCM Mixer Element */
	unsigned int audio_pcm_mixer_info_base_pa = 0;
	unsigned int audio_pcm_mixer_info_base_ba = 0;
	unsigned int audio_pcm_mixer_info_base_va = 0;
	unsigned int audio_pcm_mixer_buf_base_pa = 0;
	unsigned int audio_pcm_mixer_buf_base_ba = 0;
	unsigned int audio_pcm_mixer_buf_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Mixer7 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info[0].status != E_RESUME)) {
		audio_pcm_mixer_info_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET);
		audio_pcm_mixer_info_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_info_base_ba = audio_pcm_mixer_info_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		audio_pcm_mixer_buf_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET);
		audio_pcm_mixer_buf_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_buf_base_ba = audio_pcm_mixer_buf_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if ((audio_pcm_mixer_info_base_ba % 0x1000) || (audio_pcm_mixer_buf_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_mixer_info_base_va == 0) {
			g_pcm_mixer_info_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_info_base_ba, _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE);
			if (g_pcm_mixer_info_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Info Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_info_base_va = g_pcm_mixer_info_base_va;

		if (g_pcm_mixer_buf_base_va == 0) {
			g_pcm_mixer_buf_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_buf_base_ba, _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE);
			if (g_pcm_mixer_buf_base_va == 0) {
				iounmap((void *)g_pcm_mixer_info_base_va);
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Buffer Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_buf_base_va = g_pcm_mixer_buf_base_va;

		pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)(audio_pcm_mixer_info_base_va + (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct)));
		if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
			pcm_mixer_element_info->is_connected = MAD_TRUE;
		}
		else {
			pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;

			for (pcm_mixer_element_index = 0; pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT; pcm_mixer_element_index++) {
				if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
					pcm_mixer_element_info->is_connected = MAD_TRUE;
					break;
				}

				pcm_mixer_element_info++;
			}
		}

		if (pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT) {
			//MAD_PRINT(KERN_INFO "Get MStar PCM Mixer7 Element on PCM Mixer Element %u\n", pcm_mixer_element_index);

			audio_pcm_mixer_info_base_pa += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_ba += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_va += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			pcm_playback->str_mode_info[0].physical_addr = audio_pcm_mixer_info_base_pa;
			pcm_playback->str_mode_info[0].bus_addr = audio_pcm_mixer_info_base_ba;
			pcm_playback->str_mode_info[0].virtual_addr = audio_pcm_mixer_info_base_va;

			audio_pcm_mixer_buf_base_pa += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_ba += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_va += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			pcm_playback->str_mode_info[1].physical_addr = audio_pcm_mixer_buf_base_pa;
			pcm_playback->str_mode_info[1].bus_addr = audio_pcm_mixer_buf_base_ba;
			pcm_playback->str_mode_info[1].virtual_addr = audio_pcm_mixer_buf_base_va;

			pcm_playback->initialized_status = MAD_TRUE;
			pcm_playback->pcm_mixer_element_index = pcm_mixer_element_index;
		}
		else {
			MAD_PRINT(KERN_ERR "Error! No available PCM Mixer Element for PCM Mixer7!\n");
			iounmap((void *)g_pcm_mixer_info_base_va);
			iounmap((void *)g_pcm_mixer_buf_base_va);
			return -EBUSY;
		}
	}
	else {
		audio_pcm_mixer_info_base_pa = pcm_playback->str_mode_info[0].physical_addr;
		audio_pcm_mixer_info_base_ba = pcm_playback->str_mode_info[0].bus_addr;
		audio_pcm_mixer_info_base_va = pcm_playback->str_mode_info[0].virtual_addr;

		audio_pcm_mixer_buf_base_pa = pcm_playback->str_mode_info[1].physical_addr;
		audio_pcm_mixer_buf_base_ba = pcm_playback->str_mode_info[1].bus_addr;
		audio_pcm_mixer_buf_base_va = pcm_playback->str_mode_info[1].virtual_addr;
	}

	/* init PCM Mixer element info */
	pcm_playback->pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;
	pcm_playback->pcm_mixer_element_info->struct_version = _MAD_PCM_MIXER_VERSION;
	pcm_playback->pcm_mixer_element_info->struct_size = sizeof(struct MStar_PCM_Mixer_Element_Info_Struct);
	pcm_playback->pcm_mixer_element_info->is_connected = MAD_TRUE;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_FALSE;
	pcm_playback->pcm_mixer_element_info->group = _MAD_PCM_MIXER_GROUP_ALL;
	pcm_playback->pcm_mixer_element_info->buffer = audio_pcm_mixer_buf_base_va;
	pcm_playback->pcm_mixer_element_info->buffer_size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->pcm_mixer_element_info->read_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->write_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->channel = pcm_playback->channel_mode;
	pcm_playback->pcm_mixer_element_info->sample_rate = pcm_playback->sample_rate;
	pcm_playback->pcm_mixer_element_info->bit_width = 16;
	pcm_playback->pcm_mixer_element_info->endien = 0;
	pcm_playback->pcm_mixer_element_info->timestamp = 0;
	pcm_playback->pcm_mixer_element_info->weighting = 100;
	pcm_playback->pcm_mixer_element_info->volume = 96;
	pcm_playback->pcm_mixer_element_info->readTime = 0;
	pcm_playback->pcm_mixer_element_info->writeTime = 0;
	pcm_playback->pcm_mixer_element_info->bufferLevel = 0;

	/* init PCM Mixer buffer address */
	pcm_playback->buffer.addr = (unsigned char *)pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	pcm_playback->buffer.r_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Mixer7 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Mixer7 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* clear PCM Mixer pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

       pcm_playback->written_size = 0;

	return 0;
}

/* Exit PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_exit(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Mixer7 engine\n");

	/* clear pcm buffer */
	if (pcm_playback->buffer.addr != NULL) {
		memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	}
	pcm_playback->buffer.addr = NULL;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
	pcm_playback->buffer.w_ptr = NULL;
	pcm_playback->buffer.r_ptr = NULL;

	/* clear element info */
	if (pcm_playback->pcm_mixer_element_info != NULL) {
		memset((void *)pcm_playback->pcm_mixer_element_info, 0x00, sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
	}
	pcm_playback->pcm_mixer_element_info = NULL;

	pcm_playback->str_mode_info[0].status = E_STOP;
	pcm_playback->str_mode_info[1].status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_start(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Mixer7 engine\n");

	pcm_playback->str_mode_info[0].status = E_START;
	pcm_playback->str_mode_info[1].status = E_START;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_TRUE;

	return 0;
}

/* Stop PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Mixer7 engine\n");

	return 0;
}

/* Resume PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_resume(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Mixer7 engine\n");

	pcm_playback->str_mode_info[0].status = E_RESUME;
	pcm_playback->str_mode_info[1].status = E_RESUME;

	return 0;
}

/* Suspend PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_suspend(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Mixer7 engine\n");

	pcm_playback->str_mode_info[0].status = E_SUSPEND;
	pcm_playback->str_mode_info[1].status = E_SUSPEND;

	return 0;
}

#if ALSA_AUDIO_HAL_DEBUG
struct file *filp1 = NULL;
char str2[128];
char str3[128];

#define PLAYBACK_HAL_FILE_PATH      "/tmp/alsa_hal.log"
int audio_write_file1(char *filename, void *data, int count)
{
    mm_segment_t fs;
    int ret = 0;
    loff_t pos = 0;
    /* get current->addr_limit. It should be 0-3G */
    fs = get_fs();
    /* set current->addr_limit to 4G */
    set_fs(get_ds());

    if (filp1 == NULL)
        filp1 = (struct file *)filp_open(filename, O_CREAT | O_APPEND | O_RDWR, S_IRUSR|S_IWUSR);

    if (IS_ERR(filp1)) {
        ret = PTR_ERR(filp1);
        set_fs(fs);
        printk("%s fails to open file %s(errno = %d)\n", __func__, filename, ret);
        ret = -1;
        goto exit;
    }

    ret = vfs_write(filp1, data, count, &pos);

    //if (filp->f_op && filp->f_op->write)
    //	filp->f_op->write(filp, data, count, &filp->f_pos);

exit:
    /* restore the addr_limit */
    set_fs(fs);

    return ret;
}
#endif

/* Write PCM to PCM Mixer7 */
static unsigned int _mhal_alsa_pcm_mixer7_write(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;
#if ALSA_AUDIO_HAL_DEBUG
	struct timeval time1, time2;
	long diff = 0;

    do_gettimeofday(&time1);
#endif
	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_mixer7_get_inused_bytes();

	if (inused_bytes == 0) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer7 Buffer empty !! ***** \n");
		pcm_playback->written_size = 0;
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer7 Buffer full !! ***** \n");
#if ALSA_AUDIO_HAL_DEBUG
	    sprintf(str2, "[%010lu] full, w: %x, r: %x, used_byte:%d, copy_byte:%d, thre:%d\n", jiffies_to_msecs(jiffies), pcm_playback->pcm_mixer_element_info->write_ptr, pcm_playback->pcm_mixer_element_info->read_ptr, inused_bytes, copy_size, pcm_playback->high_threshold);
	    audio_write_file1(PLAYBACK_HAL_FILE_PATH, str2, strlen(str2));
#endif
		return 0;
	}

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* update PCM Mixer write pointer */
	pcm_playback->pcm_mixer_element_info->write_ptr = (unsigned int)pcm_playback->buffer.w_ptr;

    pcm_playback->written_size += copy_size;

	/* flush MIU */
	Chip_Flush_Memory();
#if ALSA_AUDIO_HAL_DEBUG
    do_gettimeofday(&time2);
    diff = (time2.tv_sec * 1000000 + time2.tv_usec) - (time1.tv_sec * 1000000 + time1.tv_usec);
	sprintf(str3, "[%010lu] write time [%d] copy_byte [%ld] usec\n", jiffies_to_msecs(jiffies), diff, copy_size);

	audio_write_file1(PLAYBACK_HAL_FILE_PATH, str3, strlen(str3));
#endif
	return bytes;
}

/* Get information from PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Mixer7\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(pcm_mixer_rates) / sizeof(pcm_mixer_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&pcm_mixer_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer7_get_inused_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer7_get_avail_bytes();
			break;
		}

        case E_PCM_PLAYBACK_GET_BUFFER_CONSUMED_BYTES:
        {
            struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
            int inused_bytes = 0;
            int consumed_bytes = 0;

            inused_bytes = _mhal_alsa_pcm_mixer7_get_inused_bytes();
            consumed_bytes = pcm_playback->written_size - inused_bytes;
            *param = consumed_bytes;
            pcm_playback->written_size = inused_bytes;
            break;
        }

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;

			*param = pcm_playback->str_mode_info[0].status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Mixer7\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_mixer7_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_mixer7_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Mixer7 buffer level */
static int _mhal_alsa_pcm_mixer7_get_inused_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	int inused_bytes = 0;

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;
	pcm_playback->buffer.r_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->read_ptr;

	inused_bytes = pcm_playback->buffer.w_ptr - pcm_playback->buffer.r_ptr;
	if (inused_bytes < 0) {
		inused_bytes += pcm_playback->buffer.size;
	}

	return inused_bytes;
}

/* Get PCM Mixer7 avail bytes */
static int _mhal_alsa_pcm_mixer7_get_avail_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	int inused_bytes = 0;
	int avail_bytes = 0;

	inused_bytes = _mhal_alsa_pcm_mixer7_get_inused_bytes();
	avail_bytes = pcm_playback->buffer.size - inused_bytes;
	if (avail_bytes < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused bytes %d!\n", inused_bytes);
		avail_bytes = 0;
	}

	return avail_bytes;
}

/* Set smaple rate to PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	//MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	return 0;
}

/* Set channel mode to PCM Mixer7 */
static int _mhal_alsa_pcm_mixer7_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer7;
	//MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	pcm_playback->period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_init(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info = NULL;
	unsigned int pcm_mixer_element_index = 7; /* defualt is 8th PCM Mixer Element */
	unsigned int audio_pcm_mixer_info_base_pa = 0;
	unsigned int audio_pcm_mixer_info_base_ba = 0;
	unsigned int audio_pcm_mixer_info_base_va = 0;
	unsigned int audio_pcm_mixer_buf_base_pa = 0;
	unsigned int audio_pcm_mixer_buf_base_ba = 0;
	unsigned int audio_pcm_mixer_buf_base_va = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Mixer8 engine\n");

	if ((pcm_playback->initialized_status != MAD_TRUE) || (pcm_playback->str_mode_info[0].status != E_RESUME)) {
		audio_pcm_mixer_info_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_INFO_BASE_OFFSET);
		audio_pcm_mixer_info_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_info_base_ba = audio_pcm_mixer_info_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		audio_pcm_mixer_buf_base_pa = (((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_MIXER_ELEMENT_BUFFER_BASE_OFFSET);
		audio_pcm_mixer_buf_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_mixer_buf_base_ba = audio_pcm_mixer_buf_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if ((audio_pcm_mixer_info_base_ba % 0x1000) || (audio_pcm_mixer_buf_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM playback bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		if (g_pcm_mixer_info_base_va == 0) {
			g_pcm_mixer_info_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_info_base_ba, _MAD_PCM_MIXER_ELEMENT_INFO_TOTAL_SIZE);
			if (g_pcm_mixer_info_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Info Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_info_base_va = g_pcm_mixer_info_base_va;

		if (g_pcm_mixer_buf_base_va == 0) {
			g_pcm_mixer_buf_base_va = (unsigned int)ioremap_nocache(audio_pcm_mixer_buf_base_ba, _MAD_PCM_MIXER_ELEMENT_BUFFER_TOTAL_SIZE);
			if (g_pcm_mixer_buf_base_va == 0) {
				iounmap((void *)g_pcm_mixer_info_base_va);
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Mixer Buffer Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_mixer_buf_base_va = g_pcm_mixer_buf_base_va;

		pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)(audio_pcm_mixer_info_base_va + (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct)));
		if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
			pcm_mixer_element_info->is_connected = MAD_TRUE;
		}
		else {
			pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;

			for (pcm_mixer_element_index = 0; pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT; pcm_mixer_element_index++) {
				if (pcm_mixer_element_info->is_connected == MAD_FALSE) {
					pcm_mixer_element_info->is_connected = MAD_TRUE;
					break;
				}

				pcm_mixer_element_info++;
			}
		}

		if (pcm_mixer_element_index < _MAD_MAX_PCM_MIXER_ELEMENT) {
			MAD_PRINT(KERN_INFO "Get MStar PCM Mixer8 Element on PCM Mixer Element %u\n", pcm_mixer_element_index);

			audio_pcm_mixer_info_base_pa += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_ba += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			audio_pcm_mixer_info_base_va += (pcm_mixer_element_index * sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
			pcm_playback->str_mode_info[0].physical_addr = audio_pcm_mixer_info_base_pa;
			pcm_playback->str_mode_info[0].bus_addr = audio_pcm_mixer_info_base_ba;
			pcm_playback->str_mode_info[0].virtual_addr = audio_pcm_mixer_info_base_va;

			audio_pcm_mixer_buf_base_pa += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_ba += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			audio_pcm_mixer_buf_base_va += (pcm_mixer_element_index * _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE);
			pcm_playback->str_mode_info[1].physical_addr = audio_pcm_mixer_buf_base_pa;
			pcm_playback->str_mode_info[1].bus_addr = audio_pcm_mixer_buf_base_ba;
			pcm_playback->str_mode_info[1].virtual_addr = audio_pcm_mixer_buf_base_va;

			pcm_playback->initialized_status = MAD_TRUE;
			pcm_playback->pcm_mixer_element_index = pcm_mixer_element_index;
		}
		else {
			MAD_PRINT(KERN_ERR "Error! No available PCM Mixer Element for PCM Mixer8!\n");
			iounmap((void *)g_pcm_mixer_info_base_va);
			iounmap((void *)g_pcm_mixer_buf_base_va);
			return -EBUSY;
		}
	}
	else {
		audio_pcm_mixer_info_base_pa = pcm_playback->str_mode_info[0].physical_addr;
		audio_pcm_mixer_info_base_ba = pcm_playback->str_mode_info[0].bus_addr;
		audio_pcm_mixer_info_base_va = pcm_playback->str_mode_info[0].virtual_addr;

		audio_pcm_mixer_buf_base_pa = pcm_playback->str_mode_info[1].physical_addr;
		audio_pcm_mixer_buf_base_ba = pcm_playback->str_mode_info[1].bus_addr;
		audio_pcm_mixer_buf_base_va = pcm_playback->str_mode_info[1].virtual_addr;
	}

	/* init PCM Mixer element info */
	pcm_playback->pcm_mixer_element_info = (struct MStar_PCM_Mixer_Element_Info_Struct *)audio_pcm_mixer_info_base_va;
	pcm_playback->pcm_mixer_element_info->struct_version = _MAD_PCM_MIXER_VERSION;
	pcm_playback->pcm_mixer_element_info->struct_size = sizeof(struct MStar_PCM_Mixer_Element_Info_Struct);
	pcm_playback->pcm_mixer_element_info->is_connected = MAD_TRUE;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_FALSE;
	pcm_playback->pcm_mixer_element_info->group = _MAD_PCM_MIXER_GROUP_ALL;
	pcm_playback->pcm_mixer_element_info->buffer = audio_pcm_mixer_buf_base_va;
	pcm_playback->pcm_mixer_element_info->buffer_size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->pcm_mixer_element_info->read_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->write_ptr = pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->pcm_mixer_element_info->channel = pcm_playback->channel_mode;
	pcm_playback->pcm_mixer_element_info->sample_rate = pcm_playback->sample_rate;
	pcm_playback->pcm_mixer_element_info->bit_width = 16;
	pcm_playback->pcm_mixer_element_info->endien = 0;
	pcm_playback->pcm_mixer_element_info->timestamp = 0;
	pcm_playback->pcm_mixer_element_info->weighting = 100;
	pcm_playback->pcm_mixer_element_info->volume = 96;
	pcm_playback->pcm_mixer_element_info->readTime = 0;
	pcm_playback->pcm_mixer_element_info->writeTime = 0;
	pcm_playback->pcm_mixer_element_info->bufferLevel = 0;

	/* init PCM Mixer buffer address */
	pcm_playback->buffer.addr = (unsigned char *)pcm_playback->pcm_mixer_element_info->buffer;
	pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
	pcm_playback->buffer.r_ptr = pcm_playback->buffer.addr;
	//MAD_PRINT(KERN_INFO "PCM Mixer8 buffer start address = 0x%08X\n", pcm_playback->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Mixer8 buffer end address = 0x%08X\n", (pcm_playback->buffer.addr + pcm_playback->buffer.size));

	/* clear PCM Mixer pcm buffer */
	memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	Chip_Flush_Memory();

	return 0;
}

/* Exit PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_exit(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Mixer8 engine\n");

	/* clear pcm buffer */
	if (pcm_playback->buffer.addr != NULL) {
		memset((void *)pcm_playback->buffer.addr, 0x00, pcm_playback->buffer.size);
	}
	pcm_playback->buffer.addr = NULL;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE,
	pcm_playback->buffer.w_ptr = NULL;
	pcm_playback->buffer.r_ptr = NULL;

	/* clear element info */
	if (pcm_playback->pcm_mixer_element_info != NULL) {
		memset((void *)pcm_playback->pcm_mixer_element_info, 0x00, sizeof(struct MStar_PCM_Mixer_Element_Info_Struct));
	}
	pcm_playback->pcm_mixer_element_info = NULL;

	pcm_playback->str_mode_info[0].status = E_STOP;
	pcm_playback->str_mode_info[1].status = E_STOP;
	pcm_playback->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_start(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Mixer8 engine\n");

	pcm_playback->str_mode_info[0].status = E_START;
	pcm_playback->str_mode_info[1].status = E_START;
	pcm_playback->pcm_mixer_element_info->is_started = MAD_TRUE;

	return 0;
}

/* Stop PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Mixer8 engine\n");

	return 0;
}

/* Resume PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_resume(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Mixer8 engine\n");

	pcm_playback->str_mode_info[0].status = E_RESUME;
	pcm_playback->str_mode_info[1].status = E_RESUME;

	return 0;
}

/* Suspend PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_suspend(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Mixer8 engine\n");

	pcm_playback->str_mode_info[0].status = E_SUSPEND;
	pcm_playback->str_mode_info[1].status = E_SUSPEND;

	return 0;
}

/* Write PCM to PCM Mixer8 */
static unsigned int _mhal_alsa_pcm_mixer8_write(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	unsigned char *buffer_tmp = (unsigned char *)buffer;
	unsigned int copy_lr_sample = 0;
	unsigned int copy_size = 0;
	int inused_bytes = 0;
	int loop = 0;

	copy_lr_sample = bytes / 2; /* L + R samples */
	copy_size = (pcm_playback->channel_mode == E_MONO) ? (bytes * 2) : bytes;
	inused_bytes = _mhal_alsa_pcm_mixer8_get_inused_bytes();

	if (inused_bytes == 0) {
		MAD_PRINT(KERN_INFO "***** PCM Mixer8 Buffer empty !! ***** \n");
	}
	else if ((inused_bytes + copy_size) > pcm_playback->high_threshold) {
		//MAD_PRINT(KERN_INFO "***** PCM Mixer8 Buffer full !! ***** \n");
		return 0;
	}

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;

	if (pcm_playback->channel_mode == E_MONO) {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			unsigned char sample_lo = *(buffer_tmp++);
			unsigned char sample_hi = *(buffer_tmp++);

			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;
			*(pcm_playback->buffer.w_ptr++) = sample_lo;
			*(pcm_playback->buffer.w_ptr++) = sample_hi;

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}
	else {
		for (loop = 0; loop < copy_lr_sample; loop++) {
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);
			*(pcm_playback->buffer.w_ptr++) = *(buffer_tmp++);

			if (pcm_playback->buffer.w_ptr >= (pcm_playback->buffer.addr + pcm_playback->buffer.size))
				pcm_playback->buffer.w_ptr = pcm_playback->buffer.addr;
		}
	}

	/* update PCM Mixer write pointer */
	pcm_playback->pcm_mixer_element_info->write_ptr = (unsigned int)pcm_playback->buffer.w_ptr;

	/* flush MIU */
	Chip_Flush_Memory();

	return bytes;
}

/* Get information from PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Mixer8\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_GET_BUFFER_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;

			*param = pcm_playback->buffer.size;
			break;
		}

		case E_PCM_PLAYBACK_GET_PERIOD_SIZE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;

			*param = pcm_playback->period_size;
			break;
		}

		case E_PCM_PLAYBACK_GET_SAMPLE_RATE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;

			*param = pcm_playback->sample_rate;
			break;
		}

		case E_PCM_PLAYBACK_GET_CHANNEL_MODE:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;

			*param = pcm_playback->channel_mode;
			break;
		}

		case E_PCM_PLAYBACK_GET_MAX_CHANNEL:
		{
			*param = _MAD_MAX_CHANNEL;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_COUNT:
		{
			*param = sizeof(pcm_mixer_rates) / sizeof(pcm_mixer_rates[0]);
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_LIST:
		{
			*param = (unsigned int)&pcm_mixer_rates;
			break;
		}

		case E_PCM_PLAYBACK_GET_CONSTRAINTS_MASK:
		{
			*param = 0;
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_INUSED_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer8_get_inused_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_BUFFER_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_mixer8_get_avail_bytes();
			break;
		}

		case E_PCM_PLAYBACK_GET_STR_STATUS:
		{
			struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;

			*param = pcm_playback->str_mode_info[0].status;
			break;
		}

		case E_PCM_PLAYBACK_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Mixer8\n");

	switch(cmd) {
		case E_PCM_PLAYBACK_SET_SAMPLE_RATE:
		{
			_mhal_alsa_pcm_mixer8_set_sample_rate(*param);
			break;
		}

		case E_PCM_PLAYBACK_SET_CHANNEL_MODE:
		{
			_mhal_alsa_pcm_mixer8_set_channel_mode(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Mixer8 buffer level */
static int _mhal_alsa_pcm_mixer8_get_inused_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	int inused_bytes = 0;

	pcm_playback->buffer.w_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->write_ptr;
	pcm_playback->buffer.r_ptr = (unsigned char *)pcm_playback->pcm_mixer_element_info->read_ptr;

	inused_bytes = pcm_playback->buffer.w_ptr - pcm_playback->buffer.r_ptr;
	if (inused_bytes < 0) {
		inused_bytes += pcm_playback->buffer.size;
	}

	return inused_bytes;
}

/* Get PCM Mixer8 avail bytes */
static int _mhal_alsa_pcm_mixer8_get_avail_bytes(void)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	int inused_bytes = 0;
	int avail_bytes = 0;

	inused_bytes = _mhal_alsa_pcm_mixer8_get_inused_bytes();
	avail_bytes = pcm_playback->buffer.size - inused_bytes;
	if (avail_bytes < 0) {
		MAD_PRINT(KERN_ERR "Error! Incorrect inused bytes %d!\n", inused_bytes);
		avail_bytes = 0;
	}

	return avail_bytes;
}

/* Set smaple rate to PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_set_sample_rate(unsigned int sample_rate)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	MAD_PRINT(KERN_INFO "Target sample rate is %u\n", sample_rate);

	pcm_playback->sample_rate = sample_rate;

	return 0;
}

/* Set channel mode to PCM Mixer8 */
static int _mhal_alsa_pcm_mixer8_set_channel_mode(unsigned int channel_mode)
{
	struct MStar_PCM_Mixer_Struct *pcm_playback = &g_pcm_mixer8;
	MAD_PRINT(KERN_INFO "Target channel mode is %u\n", channel_mode);

	pcm_playback->channel_mode = channel_mode;
	pcm_playback->period_size = _MAD_PCM_MIXER_ELEMENT_PERIOD_SIZE;
	pcm_playback->buffer.size = _MAD_PCM_MIXER_ELEMENT_BUFFER_SIZE;
	pcm_playback->high_threshold = pcm_playback->buffer.size - (pcm_playback->buffer.size >> 3);

	return 0;
}

/* Initiate PCM Capture1 */
static int _mhal_alsa_pcm_capture1_init(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	unsigned int audio_pcm_capture_base_pa = 0;
	unsigned int audio_pcm_capture_base_ba = 0;
	unsigned int audio_pcm_capture_base_va = 0;
	unsigned int w_ptr_offset = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Capture1 engine\n");

	if ((pcm_capture->initialized_status != MAD_TRUE) || (pcm_capture->str_mode_info.status != E_RESUME)) {
		audio_pcm_capture_base_pa = ((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_CAPTURE1_BASE_OFFSET;
		audio_pcm_capture_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_capture_base_ba = audio_pcm_capture_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if (audio_pcm_capture_base_pa >= 0xA0000000) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM capture address!\n");
			return -EFAULT;
		}
		else if ((audio_pcm_capture_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM capture bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		pcm_capture->buffer.size = _MAD_PCM_CAPTURE_BUF_SIZE;
		if (g_pcm_capture1_base_va == 0) {
			g_pcm_capture1_base_va = (unsigned int)ioremap_nocache(audio_pcm_capture_base_ba, pcm_capture->buffer.size);
			if (g_pcm_capture1_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Capture1 Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_capture_base_va = g_pcm_capture1_base_va;

		pcm_capture->str_mode_info.physical_addr = audio_pcm_capture_base_pa;
		pcm_capture->str_mode_info.bus_addr = audio_pcm_capture_base_ba;
		pcm_capture->str_mode_info.virtual_addr = audio_pcm_capture_base_va;

		pcm_capture->initialized_status = MAD_TRUE;
	}
	else {
		audio_pcm_capture_base_pa = pcm_capture->str_mode_info.physical_addr;
		audio_pcm_capture_base_ba = pcm_capture->str_mode_info.bus_addr;
		audio_pcm_capture_base_va = pcm_capture->str_mode_info.virtual_addr;
	}

	/* init PCM capture1 buffer address */
	pcm_capture->buffer.addr = (unsigned char *)audio_pcm_capture_base_va;
	//MAD_PRINT(KERN_INFO "PCM Capture1 buffer start address = 0x%08X\n", pcm_capture->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Capture1 buffer end address = 0x%08X\n", (pcm_capture->buffer.addr + pcm_capture->buffer.size));

	/* clear all PCM capture1 buffer */
	memset((void *)pcm_capture->buffer.addr, 0x00, _MAD_PCM_CAPTURE_BUF_SIZE);
	Chip_Flush_Memory();

	/* reset PCM capture1 write pointer */
	w_ptr_offset = _mhal_alsa_read_reg(0x2DF0) * _MAD_BYTES_IN_LINE;
	pcm_capture->buffer.w_ptr = pcm_capture->buffer.addr + w_ptr_offset;

	/* reset PCM capture1 read pointer */
	pcm_capture->buffer.r_ptr = pcm_capture->buffer.w_ptr;
	_mhal_alsa_write_reg(0x2DD4, (unsigned short)(w_ptr_offset / _MAD_BYTES_IN_LINE));

	/* reset PCM capture1 buffer size */
	_mhal_alsa_write_reg(0x2DD6, (unsigned short)(pcm_capture->buffer.size / _MAD_BYTES_IN_LINE));

	return 0;
}

/* Exit PCM Capture1 */
static int _mhal_alsa_pcm_capture1_exit(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Capture1 engine\n");

	/* clear PCM capture1 buffer size */
	_mhal_alsa_write_reg(0x2DD6, 0x0000);

	/* clear PCM capture1 read pointer */
	_mhal_alsa_write_reg(0x2DD4, 0x0000);

	pcm_capture->str_mode_info.status = E_STOP;
	pcm_capture->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Capture1 */
static int _mhal_alsa_pcm_capture1_start(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Capture1 engine\n");

	pcm_capture->str_mode_info.status = E_START;

	return 0;
}

/* Stop PCM Capture1 */
static int _mhal_alsa_pcm_capture1_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Capture1 engine\n");

	return 0;
}

/* Resume PCM Capture1 */
static int _mhal_alsa_pcm_capture1_resume(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Capture1 engine\n");

	pcm_capture->str_mode_info.status = E_RESUME;

	return 0;
}

/* Suspend PCM Capture1 */
static int _mhal_alsa_pcm_capture1_suspend(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Capture1 engine\n");

	pcm_capture->str_mode_info.status = E_SUSPEND;

	return 0;
}

/* Read PCM from PCM Capture1 */
static unsigned int _mhal_alsa_pcm_capture1_read(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	unsigned int rest_size_to_buffer_end = (pcm_capture->buffer.addr + _MAD_PCM_CAPTURE_BUF_SIZE) - pcm_capture->buffer.r_ptr;
	unsigned int r_ptr_offset = 0;
	unsigned int read_size = 0;
	//MAD_PRINT(KERN_INFO "Read PCM from PCM Capture1 engine\n");

	read_size = (rest_size_to_buffer_end > bytes) ? bytes : rest_size_to_buffer_end;

	memcpy(buffer, pcm_capture->buffer.r_ptr, read_size);
	Chip_Flush_Memory();

	pcm_capture->buffer.r_ptr += read_size;
	if (pcm_capture->buffer.r_ptr == (pcm_capture->buffer.addr + _MAD_PCM_CAPTURE_BUF_SIZE))
		pcm_capture->buffer.r_ptr = pcm_capture->buffer.addr;

	r_ptr_offset = pcm_capture->buffer.r_ptr - pcm_capture->buffer.addr;
	_mhal_alsa_write_reg(0x2DD4, (unsigned short)(r_ptr_offset / _MAD_BYTES_IN_LINE));

	return read_size;
}

/* Get infromation from PCM Capture1 */
static int _mhal_alsa_pcm_capture1_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Capture1 engine\n");

	switch(cmd) {
		case E_PCM_CAPTURE_GET_NEW_PCM_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_capture1_get_new_avail_bytes();
			break;
		}

		case E_PCM_CAPTURE_GET_TOTAL_PCM_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_capture1_get_total_avail_bytes();
			break;
		}

		case E_PCM_CAPTURE_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Capture1 */
static int _mhal_alsa_pcm_capture1_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Capture1 engine\n");

	switch(cmd) {
		case E_PCM_CAPTURE_SET_BUFFER_SIZE:
		{
			if (*param > _MAD_PCM_CAPTURE_BUF_SIZE) {
				*param = _MAD_PCM_CAPTURE_BUF_SIZE;
				MAD_PRINT(KERN_INFO "Target buffer is too large, reset to %u\n", *param);
			}

			if ((*param % _MAD_BYTES_IN_LINE)) {
				*param = (*param / _MAD_BYTES_IN_LINE) * _MAD_BYTES_IN_LINE;
				MAD_PRINT(KERN_INFO "Target buffer is not aligned, reset to %u\n", *param);
			}

			_mhal_alsa_pcm_capture1_set_buffer_size(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Capture1's new PCM available bytes */
static unsigned int _mhal_alsa_pcm_capture1_get_new_avail_bytes(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	unsigned char *new_w_ptr = NULL;
	unsigned int new_w_ptr_offset = 0;
	int new_avail_bytes = 0;

	new_w_ptr_offset = _mhal_alsa_read_reg(0x2DF0) * _MAD_BYTES_IN_LINE;
	new_w_ptr = pcm_capture->buffer.addr + new_w_ptr_offset;

	new_avail_bytes = new_w_ptr - pcm_capture->buffer.w_ptr;
	if (new_avail_bytes < 0)
		new_avail_bytes += _MAD_PCM_CAPTURE_BUF_SIZE;

	pcm_capture->buffer.w_ptr = new_w_ptr;

	return new_avail_bytes;
}

/* Get PCM Capture1's total PCM available bytes */
static unsigned int _mhal_alsa_pcm_capture1_get_total_avail_bytes(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	unsigned int r_ptr_offset = 0;
	unsigned int w_ptr_offset = 0;
	int avail_bytes = 0;
	int loop = 0;
	int loop_timeout = 100;

	r_ptr_offset = pcm_capture->buffer.r_ptr - pcm_capture->buffer.addr;
	w_ptr_offset = _mhal_alsa_read_reg(0x2DF0) * _MAD_BYTES_IN_LINE;

	avail_bytes = w_ptr_offset - r_ptr_offset;
	if (avail_bytes < 0)
		avail_bytes += _MAD_PCM_CAPTURE_BUF_SIZE;

	if (avail_bytes >= (pcm_capture->buffer.size - _MAD_PCM_CAPTURE_BUF_UNIT)) {
		MAD_PRINT(KERN_INFO "***** Audio PCM Capture1 Buffer is overrun !! ***** \n");

		/* clear PCM capture buffer size */
		_mhal_alsa_write_reg(0x2DD6, 0x0000);

		/* clear PCM capture read pointer */
		_mhal_alsa_write_reg(0x2DD4, 0x0000);

		/* check if PCM capture receives reset command */
		while(_mhal_alsa_read_reg(0x2DF0) != 0) {
			mdelay(1);

			if ((++loop) >= loop_timeout)
				break;
		}

		/* reset PCM capture write pointer */
		w_ptr_offset = _mhal_alsa_read_reg(0x2DF0) * _MAD_BYTES_IN_LINE;
		pcm_capture->buffer.w_ptr = pcm_capture->buffer.addr + w_ptr_offset;

		/* reset PCM capture read pointer */
		pcm_capture->buffer.r_ptr = pcm_capture->buffer.w_ptr;
		_mhal_alsa_write_reg(0x2DD4, (unsigned short)(w_ptr_offset / _MAD_BYTES_IN_LINE));

		/* reset PCM capture buffer size */
		_mhal_alsa_write_reg(0x2DD6, (unsigned short)(pcm_capture->buffer.size / _MAD_BYTES_IN_LINE));

		return 0;
	}

	return avail_bytes;
}

/* Set PCM Capture1's PCM buffer size */
static int _mhal_alsa_pcm_capture1_set_buffer_size(unsigned int buffer_size)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture1;
	MAD_PRINT(KERN_INFO "Target buffer size is %u\n", buffer_size);

	pcm_capture->buffer.size = buffer_size;
	_mhal_alsa_write_reg(0x2DD6, (unsigned short)(buffer_size / _MAD_BYTES_IN_LINE));

	return 0;
}

/* Initiate PCM Capture2 */
static int _mhal_alsa_pcm_capture2_init(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	unsigned int audio_pcm_capture_base_pa = 0;
	unsigned int audio_pcm_capture_base_ba = 0;
	unsigned int audio_pcm_capture_base_va = 0;
	unsigned int w_ptr_offset = 0;
	MAD_PRINT(KERN_INFO "Initiate MStar PCM Capture2 engine\n");

	if ((pcm_capture->initialized_status != MAD_TRUE) || (pcm_capture->str_mode_info.status != E_RESUME)) {
		audio_pcm_capture_base_pa = ((unsigned int)_mhal_alsa_read_reg(0x2A90) * _MAD_ADDR_CONVERTOR) + _MAD_PCM_CAPTURE2_BASE_OFFSET;
		audio_pcm_capture_base_pa += (((_mhal_alsa_read_reg(0x2AC0) & 0x0F) << 16) * _MAD_ADDR_CONVERTOR);
		audio_pcm_capture_base_ba = audio_pcm_capture_base_pa + _MAD_PHYSICAL_ADDR_TO_BUS_ADDR;

		if (audio_pcm_capture_base_pa >= 0xA0000000) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM capture address!\n");
			return -EFAULT;
		}
		else if ((audio_pcm_capture_base_ba % 0x1000)) {
			MAD_PRINT(KERN_ERR "Error! Invalid MStar PCM capture bus address, it should be aligned by 4 KB!\n");
			return -EFAULT;
		}

		/* convert Bus Address to Virtual Address */
		pcm_capture->buffer.size = _MAD_PCM_CAPTURE_BUF_SIZE;
		if (g_pcm_capture2_base_va == 0) {
			g_pcm_capture2_base_va = (unsigned int)ioremap_nocache(audio_pcm_capture_base_ba, pcm_capture->buffer.size);
			if (g_pcm_capture2_base_va == 0) {
				MAD_PRINT(KERN_ERR "Error! fail to convert PCM Capture2 Bus Address to Virtual Address\n");
				return -ENOMEM;
			}
		}
		audio_pcm_capture_base_va = g_pcm_capture2_base_va;

		pcm_capture->str_mode_info.physical_addr = audio_pcm_capture_base_pa;
		pcm_capture->str_mode_info.bus_addr = audio_pcm_capture_base_ba;
		pcm_capture->str_mode_info.virtual_addr = audio_pcm_capture_base_va;

		pcm_capture->initialized_status = MAD_TRUE;
	}
	else {
		audio_pcm_capture_base_pa = pcm_capture->str_mode_info.physical_addr;
		audio_pcm_capture_base_ba = pcm_capture->str_mode_info.bus_addr;
		audio_pcm_capture_base_va = pcm_capture->str_mode_info.virtual_addr;
	}

	/* init PCM capture2 buffer address */
	pcm_capture->buffer.addr = (unsigned char *)audio_pcm_capture_base_va;
	//MAD_PRINT(KERN_INFO "PCM Capture2 buffer start address = 0x%08X\n", pcm_capture->buffer.addr);
	//MAD_PRINT(KERN_INFO "PCM Capture2 buffer end address = 0x%08X\n", (pcm_capture->buffer.addr + pcm_capture->buffer.size));

	/* clear all PCM capture2 buffer */
	memset((void *)pcm_capture->buffer.addr, 0x00, _MAD_PCM_CAPTURE_BUF_SIZE);
	Chip_Flush_Memory();

	/* reset PCM capture2 write pointer */
	w_ptr_offset = _mhal_alsa_read_reg(0x2DF4) * _MAD_BYTES_IN_LINE;
	pcm_capture->buffer.w_ptr = pcm_capture->buffer.addr + w_ptr_offset;

	/* reset PCM capture2 read pointer */
	pcm_capture->buffer.r_ptr = pcm_capture->buffer.w_ptr;
	_mhal_alsa_write_reg(0x2D38, (unsigned short)(w_ptr_offset / _MAD_BYTES_IN_LINE));

	/* reset PCM capture2 buffer size */
	_mhal_alsa_write_reg(0x2D3A, (unsigned short)(pcm_capture->buffer.size / _MAD_BYTES_IN_LINE));

	return 0;
}

/* Exit PCM Capture2 */
static int _mhal_alsa_pcm_capture2_exit(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	MAD_PRINT(KERN_INFO "Exit MStar PCM Capture2 engine\n");

	/* clear PCM capture2 buffer size */
	_mhal_alsa_write_reg(0x2D3A, 0x0000);

	/* clear PCM capture2 read pointer */
	_mhal_alsa_write_reg(0x2D38, 0x0000);

	pcm_capture->str_mode_info.status = E_STOP;
	pcm_capture->initialized_status = MAD_FALSE;

	return 0;
}

/* Start PCM Capture2 */
static int _mhal_alsa_pcm_capture2_start(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	//MAD_PRINT(KERN_INFO "Start MStar PCM Capture2 engine\n");

	pcm_capture->str_mode_info.status = E_START;

	return 0;
}

/* Stop PCM Capture2 */
static int _mhal_alsa_pcm_capture2_stop(void)
{
	//MAD_PRINT(KERN_INFO "Stop MStar PCM Capture2 engine\n");

	return 0;
}

/* Resume PCM Capture2 */
static int _mhal_alsa_pcm_capture2_resume(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	//MAD_PRINT(KERN_INFO "Resume MStar PCM Capture2 engine\n");

	pcm_capture->str_mode_info.status = E_RESUME;

	return 0;
}

/* Suspend PCM Capture2 */
static int _mhal_alsa_pcm_capture2_suspend(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	//MAD_PRINT(KERN_INFO "Suspend MStar PCM Capture2 engine\n");

	pcm_capture->str_mode_info.status = E_SUSPEND;

	return 0;
}

/* Read PCM from PCM Capture2 */
static unsigned int _mhal_alsa_pcm_capture2_read(void *buffer, unsigned int bytes)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	unsigned int rest_size_to_buffer_end = (pcm_capture->buffer.addr + _MAD_PCM_CAPTURE_BUF_SIZE) - pcm_capture->buffer.r_ptr;
	unsigned int r_ptr_offset = 0;
	unsigned int read_size = 0;
	//MAD_PRINT(KERN_INFO "Read PCM from PCM Capture2 engine\n");

	read_size = (rest_size_to_buffer_end > bytes) ? bytes : rest_size_to_buffer_end;

	memcpy(buffer, pcm_capture->buffer.r_ptr, read_size);
	Chip_Flush_Memory();

	pcm_capture->buffer.r_ptr += read_size;
	if (pcm_capture->buffer.r_ptr == (pcm_capture->buffer.addr + _MAD_PCM_CAPTURE_BUF_SIZE))
		pcm_capture->buffer.r_ptr = pcm_capture->buffer.addr;

	r_ptr_offset = pcm_capture->buffer.r_ptr - pcm_capture->buffer.addr;
	_mhal_alsa_write_reg(0x2D38, (unsigned short)(r_ptr_offset / _MAD_BYTES_IN_LINE));

	return read_size;
}

/* Get infromation from PCM Capture2 */
static int _mhal_alsa_pcm_capture2_get(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Get parameter from PCM Capture2 engine\n");

	switch(cmd) {
		case E_PCM_CAPTURE_GET_NEW_PCM_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_capture2_get_new_avail_bytes();
			break;
		}

		case E_PCM_CAPTURE_GET_TOTAL_PCM_AVAIL_BYTES:
		{
			*param = _mhal_alsa_pcm_capture2_get_total_avail_bytes();
			break;
		}

		case E_PCM_CAPTURE_GET_DEVICE_STATUS:
		{
			*param = _mhal_alsa_get_device_status();
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid GET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Set information to PCM Capture2 */
static int _mhal_alsa_pcm_capture2_set(int cmd, unsigned int *param)
{
	int err = 0;
	//MAD_PRINT(KERN_INFO "Set parameter to PCM Capture2 engine\n");

	switch(cmd) {
		case E_PCM_CAPTURE_SET_BUFFER_SIZE:
		{
			if (*param > _MAD_PCM_CAPTURE_BUF_SIZE) {
				*param = _MAD_PCM_CAPTURE_BUF_SIZE;
				MAD_PRINT(KERN_INFO "Target buffer is too large, reset to %u\n", *param);
			}

			if ((*param % _MAD_BYTES_IN_LINE)) {
				*param = (*param / _MAD_BYTES_IN_LINE) * _MAD_BYTES_IN_LINE;
				MAD_PRINT(KERN_INFO "Target buffer is not aligned, reset to %u\n", *param);
			}

			_mhal_alsa_pcm_capture2_set_buffer_size(*param);
			break;
		}

		default:
		{
			MAD_PRINT(KERN_INFO "Invalid SET command %d\n", cmd);
			err = -EINVAL;
			break;
		}
	}

	return err;
}

/* Get PCM Capture2's new PCM available bytes */
static unsigned int _mhal_alsa_pcm_capture2_get_new_avail_bytes(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	unsigned char *new_w_ptr = NULL;
	unsigned int new_w_ptr_offset = 0;
	int new_avail_bytes = 0;

	new_w_ptr_offset = _mhal_alsa_read_reg(0x2DF4) * _MAD_BYTES_IN_LINE;
	new_w_ptr = pcm_capture->buffer.addr + new_w_ptr_offset;

	new_avail_bytes = new_w_ptr - pcm_capture->buffer.w_ptr;
	if (new_avail_bytes < 0)
		new_avail_bytes += _MAD_PCM_CAPTURE_BUF_SIZE;

	pcm_capture->buffer.w_ptr = new_w_ptr;

	return new_avail_bytes;
}

/* Get PCM Capture2's total PCM available bytes */
static unsigned int _mhal_alsa_pcm_capture2_get_total_avail_bytes(void)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	unsigned int r_ptr_offset = 0;
	unsigned int w_ptr_offset = 0;
	int avail_bytes = 0;
	int loop = 0;
	int loop_timeout = 100;

	r_ptr_offset = pcm_capture->buffer.r_ptr - pcm_capture->buffer.addr;
	w_ptr_offset = _mhal_alsa_read_reg(0x2DF4) * _MAD_BYTES_IN_LINE;

	avail_bytes = w_ptr_offset - r_ptr_offset;
	if (avail_bytes < 0)
		avail_bytes += _MAD_PCM_CAPTURE_BUF_SIZE;

	if (avail_bytes >= (pcm_capture->buffer.size - _MAD_PCM_CAPTURE_BUF_UNIT)) {
		MAD_PRINT(KERN_INFO "***** Audio PCM Capture2 Buffer is overrun !! ***** \n");

		/* clear PCM capture buffer size */
		_mhal_alsa_write_reg(0x2D3A, 0x0000);

		/* clear PCM capture read pointer */
		_mhal_alsa_write_reg(0x2D38, 0x0000);

		/* check if PCM capture receives reset command */
		while(_mhal_alsa_read_reg(0x2DF4) != 0) {
			mdelay(1);

			if ((++loop) >= loop_timeout)
				break;
		}

		/* reset PCM capture write pointer */
		w_ptr_offset = _mhal_alsa_read_reg(0x2DF4) * _MAD_BYTES_IN_LINE;
		pcm_capture->buffer.w_ptr = pcm_capture->buffer.addr + w_ptr_offset;

		/* reset PCM capture read pointer */
		pcm_capture->buffer.r_ptr = pcm_capture->buffer.w_ptr;
		_mhal_alsa_write_reg(0x2D38, (unsigned short)(w_ptr_offset / _MAD_BYTES_IN_LINE));

		/* reset PCM capture buffer size */
		_mhal_alsa_write_reg(0x2D3A, (unsigned short)(pcm_capture->buffer.size / _MAD_BYTES_IN_LINE));

		return 0;
	}

	return avail_bytes;
}

/* Set PCM Capture2's PCM buffer size */
static int _mhal_alsa_pcm_capture2_set_buffer_size(unsigned int buffer_size)
{
	struct MStar_PCM_Capture_Struct *pcm_capture = &g_pcm_capture2;
	MAD_PRINT(KERN_INFO "Target buffer size is %u\n", buffer_size);

	pcm_capture->buffer.size = buffer_size;
	_mhal_alsa_write_reg(0x2D3A, (unsigned short)(buffer_size / _MAD_BYTES_IN_LINE));

	return 0;
}

static int __init _mhal_alsa_init(void)
{
	int err = 0;

	MAD_PRINT(KERN_INFO "Initiate MStar ALSA core driver\n");

	memset(&MStar_MAD, 0x00, sizeof(struct MStar_MAD_Info));

	memcpy(MStar_MAD.name, _MAD_CHIP_MODEL, sizeof(MStar_MAD.name));
	sprintf(MStar_MAD.version, "%d.%d.%d", _MAD_ALSA_HAL_VERSION_MAJOR, _MAD_ALSA_HAL_VERSION_MINOR, _MAD_ALSA_HAL_VERSION_REVISION);

	/* Hook Playback Operators */
	MStar_MAD.playback_pcm_ops[0] = &MStar_DMA_Reader_Ops;
	MStar_MAD.playback_pcm_ops[1] = &MStar_PCM_Playback2_Ops;
	MStar_MAD.playback_pcm_ops[2] = &MStar_PCM_Mixer3_Ops;
	MStar_MAD.playback_pcm_ops[3] = &MStar_PCM_Mixer4_Ops;
	MStar_MAD.playback_pcm_ops[4] = &MStar_PCM_Mixer5_Ops;
	MStar_MAD.playback_pcm_ops[5] = &MStar_PCM_Mixer6_Ops;
	MStar_MAD.playback_pcm_ops[6] = &MStar_PCM_Mixer7_Ops;
	MStar_MAD.playback_pcm_ops[7] = &MStar_PCM_Mixer8_Ops;

	/* Hook Capture Operators */
	MStar_MAD.capture_pcm_ops[0] = &MStar_PCM_Capture1_Ops;
	MStar_MAD.capture_pcm_ops[1] = &MStar_PCM_Capture2_Ops;

	err = _mdrv_alsa_hook_device(&MStar_MAD);
	if (err < 0) {
		MAD_PRINT(KERN_ERR "Error(%d)! fail to hook PCM operators\n", err);
		return err;
	}

	return 0;
}

static void __exit _mhal_alsa_exit(void)
{
	int err = 0;

	MAD_PRINT(KERN_INFO "Exit MStar ALSA core driver\n");

	err = _mdrv_alsa_unhook_device();
	if (err < 0) {
		MAD_PRINT(KERN_ERR "Error(%d)! fail to unhook PCM operators\n", err);
		return;
	}

	return;
}

/*
 * ============================================================================
 * Module Information
 * ============================================================================
 */
module_init(_mhal_alsa_init);
module_exit(_mhal_alsa_exit);

MODULE_AUTHOR("MStar Semiconductor, Inc.");
MODULE_DESCRIPTION("MStar ALSA Driver - HAL Layer");
MODULE_SUPPORTED_DEVICE("MAD DEVICE");
MODULE_LICENSE("Proprietary");
