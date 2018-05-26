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



/** @file alsa_platform.h
 *
 *  alsa platform driver.
 *
 *  @author		Won Chang Shin(wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2013.03.20
 *
 *	@{
 */

#ifndef	_LG115x_ALSA_PLATFORM_H_
#define	_lg115x_ALSA_PLATFORM_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */
/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define PLATFORM_NAME 	"lg115x-platform"
#define DAI_NAME 		"lg115x-dai"

#define ALSA_DEVICE_NUM 		8
#define ALSA_DEV_TO_INDEX		2
#define ALSA_GET_INDEX(ALSA_DEV_ID)	((ALSA_DEV_ID) + ALSA_DEV_TO_INDEX)

//Define for ALSA Device Running Status
#define ALSA_DEVICE_STOP 		0
#define ALSA_DEVICE_RUNNING		1

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define ADEC_DSP_MIX_VOLUME_GAIN_0DB	1024	//0x08000000
#define ADEC_DSP_MIX_VOLUME_GAIN_MUTE	0

#define ADEC_MIX_VOLUME_MAIN_30DB		0x9D	//157
#define ADEC_MIX_VOLUME_MAIN_0DB		0x00	//0
#define ADEC_MIX_VOLUME_FINE_15_16DB	0x0F	//15
#define ADEC_MIX_VOLUME_FINE_0DB		0x00	//0

#define LG115x_SUPPORT_PLAYBACK_RATE	(SNDRV_PCM_RATE_8000  | SNDRV_PCM_RATE_11025	\
										|SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050	\
										|SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100	\
										|SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_64000	\
										|SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000	\
										|SNDRV_PCM_RATE_176400| SNDRV_PCM_RATE_192000)

#define LG115x_SUPPORT_PLAYBACK_FORMAT	(SNDRV_PCM_FMTBIT_S8  	  | SNDRV_PCM_FMTBIT_U8		\
										|SNDRV_PCM_FMTBIT_S16_LE  | SNDRV_PCM_FMTBIT_S16_BE	\
										|SNDRV_PCM_FMTBIT_U16_LE  | SNDRV_PCM_FMTBIT_U16_BE	\
										|SNDRV_PCM_FMTBIT_S24_LE  | SNDRV_PCM_FMTBIT_S24_BE	\
										|SNDRV_PCM_FMTBIT_U24_LE  | SNDRV_PCM_FMTBIT_U24_BE	\
										|SNDRV_PCM_FMTBIT_S32_LE  | SNDRV_PCM_FMTBIT_S32_BE	\
										|SNDRV_PCM_FMTBIT_U32_LE  | SNDRV_PCM_FMTBIT_U32_BE	\
										|SNDRV_PCM_FMTBIT_S24_3LE | SNDRV_PCM_FMTBIT_U24_3LE\
										|SNDRV_PCM_FMTBIT_S24_3BE | SNDRV_PCM_FMTBIT_U24_3BE)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
struct runtime_data{
	struct snd_pcm_substream *substream;
	spinlock_t lock;
	unsigned int running;
	unsigned int alloc_dev_master;
	unsigned int alloc_dev_render;
	unsigned int write_ptr;
	unsigned int render_is_connected;		// 0 : not connected, 1 : connected
	unsigned int master_is_connected;
	unsigned int spdif_is_connected;		// 0 : not connected, 1 : connected
	unsigned int wake_up_is_called;
	unsigned int volume;				// 1024 : 0dB.
	unsigned int buffer_size_diff;
	unsigned int old_free_size;
};

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Generic Usage Functions
----------------------------------------------------------------------------------------*/
int ALSA_PLATFORM_DAI_Init(void);
void ALSA_PLATFORM_DAI_Cleanup(void);

int ALSA_PLATFORM_PCM_Init(void);
void ALSA_PLATFORM_PCM_Cleanup(void);


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _lg115x_ALSA_PLATFORM_H_ */


