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
 */

#ifndef _MHAL_ALSA_DRIVER_HEADER
#define _MHAL_ALSA_DRIVER_HEADER

/*
 * ============================================================================
 * Include Headers
 * ============================================================================
 */
#include "mdrv_public.h"
#include "mhal_version.h"


/*
 * ============================================================================
 * Constant Definitions
 * ============================================================================
 */
/* Define a Ring Buffer data structure for MStar Audio DSP */
struct MStar_Ring_Buffer_Struct {
	unsigned char *addr;
	unsigned int size;
	unsigned char *w_ptr;
	unsigned char *r_ptr;
};

/* Define a STR (Suspend To Ram) data structure for MStar Audio DSP */
 struct MStar_STR_MODE_Struct {
	unsigned int status;
	unsigned int physical_addr;
	unsigned int bus_addr;
	unsigned int virtual_addr;
};

/* Define a DMA Reader data structure for MStar Audio DSP */
struct MStar_DMA_Reader_Struct {
	struct MStar_Ring_Buffer_Struct buffer;
	struct MStar_STR_MODE_Struct str_mode_info;
	unsigned int initialized_status;
	unsigned int channel_mode;
	unsigned int sample_rate;
	unsigned int period_size;
	unsigned int high_threshold;
	unsigned int remain_size;
};

/* Define a PCM Mixer Element Info data structure for MStar Audio DSP */
struct MStar_PCM_Mixer_Element_Info_Struct {
	unsigned char struct_version;
	unsigned int struct_size;
	unsigned char is_connected;
	unsigned char is_started;
	unsigned int group;
	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int read_ptr;
	unsigned int write_ptr;
	unsigned int channel;
	unsigned int sample_rate;
	unsigned int bit_width;
	unsigned int endien;
	unsigned int timestamp;
	unsigned int weighting;
	unsigned int volume;
	unsigned int readTime;
	unsigned int writeTime;
	unsigned int bufferLevel;
};

/* Define a PCM Mixer data structure for MStar Audio DSP */
struct MStar_PCM_Mixer_Struct {
	struct MStar_Ring_Buffer_Struct buffer;
	struct MStar_STR_MODE_Struct str_mode_info[2];
	struct MStar_PCM_Mixer_Element_Info_Struct *pcm_mixer_element_info;
	unsigned int pcm_mixer_element_index;
	unsigned int initialized_status;
	unsigned int channel_mode;
	unsigned int sample_rate;
	unsigned int period_size;
	unsigned int high_threshold;
    unsigned int written_size;
};

/* Define a DMA Reader data structure for MStar Audio DSP */
struct MStar_PCM_Capture_Struct {
	struct MStar_Ring_Buffer_Struct buffer;
	struct MStar_STR_MODE_Struct str_mode_info;
	unsigned int initialized_status;
	unsigned int channel_mode;
	unsigned int sample_rate;
};


/*
 * ============================================================================
 * Forward Declarations
 * ============================================================================
 */
extern void Chip_Flush_Memory(void);

#endif /* _MHAL_ALSA_DRIVER_HEADER */