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



/** @file alsa_platform.c
 *
 *  This is alsa platform driver.
 *
 *  @author		Won Chang Shin(wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2013.03.20
 *  @note		The functions for alsa platform driver.
 *
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>

#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "audio_drv.h"
#include "audio_kapi_renderer.h"
#include "audio_drv_manager.h"
#include "audio_drv_renderer.h"
#include "audio_drv_master.h"

#include "alsa_platform.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
//define for audio ALSA Buffer size
#ifdef ENABLE_ALSA_BIG_BUFFER	//PIONEER platform
#define ALSA_BUFFER_BYTES_MAX	(PAGE_SIZE * 8 * 12)	//393216
#define ALSA_PERIOD_BYTES_MAX	(PAGE_SIZE * 8)			// 32768
#else
#define ALSA_BUFFER_BYTES_MAX	(PAGE_SIZE * 8)			//32768
#define ALSA_PERIOD_BYTES_MAX	(PAGE_SIZE)				// 4096
#endif	//#ifdef ENABLE_ALSA_BIG_BUFFER	//PIONEER platform

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#ifndef __devinit
# define __devinit
#endif
#ifndef __devexit
# define __devexit
#endif
#ifndef __devexit_p
# define __devexit_p
#endif

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern LX_AUD_KDRV_T	g_AudKdrv;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int lg115x_pcm_open(struct snd_pcm_substream *substream);
static int lg115x_pcm_close(struct snd_pcm_substream *substream);
static int lg115x_pcm_lib_ioctl(struct snd_pcm_substream *substream, unsigned int cmd, void *arg);
static int lg115x_pcm_hw_params(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params);
static int lg115x_pcm_hw_free(struct snd_pcm_substream *substream);
static int lg115x_pcm_prepare(struct snd_pcm_substream *substream);
static int lg115x_pcm_trigger(struct snd_pcm_substream *substream, int cmd);
static snd_pcm_uframes_t lg115x_pcm_pointer(struct snd_pcm_substream *substream);
static int lg115x_pcm_copy(struct snd_pcm_substream *substream, 	\
						   int channel, snd_pcm_uframes_t pos, void __user *buf, snd_pcm_uframes_t count);
static int lg115x_pcm_silence(struct snd_pcm_substream *substream,	\
							  int channel, snd_pcm_uframes_t pos, snd_pcm_uframes_t count);
//struct page *(*page)(struct snd_pcm_substream *substream, unsigned long offset);
static int lg115x_pcm_mmap(struct snd_pcm_substream *substream,	struct vm_area_struct *vma);
//int (*ack)(struct snd_pcm_substream *substream);

static int lg115x_pcm_new(struct snd_soc_pcm_runtime *rtd);
static void lg115x_pcm_free(struct snd_pcm *pcm);

static int __devinit lg115x_platform_probe(struct platform_device *pdev);
static int __devexit lg115x_platform_remove(struct platform_device *pdev);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static struct snd_pcm_hardware lg115x_pcm_hardware =
{
	.info				= SNDRV_PCM_INFO_PAUSE			\
						 |SNDRV_PCM_INFO_RESUME			\
						 |SNDRV_PCM_INFO_INTERLEAVED,
	.formats			= LG115x_SUPPORT_PLAYBACK_FORMAT,
	.rates 				= LG115x_SUPPORT_PLAYBACK_RATE,
	.rate_min			= SNDRV_PCM_RATE_8000,
	.rate_max			= SNDRV_PCM_RATE_192000,
	.channels_min		= 1,
	.channels_max		= 8,
	.buffer_bytes_max	= ALSA_BUFFER_BYTES_MAX, 	//32768
	.period_bytes_min	= 128,
	.period_bytes_max	= ALSA_PERIOD_BYTES_MAX, 	//4096
	.periods_min		= 2,
	.periods_max		= 8,
	.fifo_size			= 4,
};

static struct snd_pcm_ops lg115x_pcm_ops = {
	.open		= lg115x_pcm_open,
	.close		= lg115x_pcm_close,
	.ioctl		= lg115x_pcm_lib_ioctl,
	.hw_params	= lg115x_pcm_hw_params,
	.hw_free	= lg115x_pcm_hw_free,
	.prepare	= lg115x_pcm_prepare,
	.trigger	= lg115x_pcm_trigger,
	.pointer	= lg115x_pcm_pointer,
	.copy		= lg115x_pcm_copy,
	.silence	= lg115x_pcm_silence,
	/*.page */
	.mmap		= lg115x_pcm_mmap,
	/*.ack  */
};

static struct snd_soc_platform_driver lg115x_soc_platform_driver = {
	.ops		= &lg115x_pcm_ops,
	.pcm_new	= lg115x_pcm_new,
	.pcm_free	= lg115x_pcm_free,
};

static struct platform_driver lg115x_platform_driver = {
	.driver = {
		.name 	= PLATFORM_NAME,
		.owner 	= THIS_MODULE,
	},
	.probe 	= lg115x_platform_probe,
	.remove = __devexit_p(lg115x_platform_remove),
};

static struct platform_device *lg115x_platform_device;

static int lg115x_pcm_monitor_task(void *pParam);
static struct task_struct *_gpstAudioAlsaTask[ALSA_DEVICE_NUM] = {NULL, };
static struct runtime_data *lg115x_runtime_data[ALSA_DEVICE_NUM];


/*========================================================================================
	Implementation Group
========================================================================================*/
static int lg115x_pcm_open(struct snd_pcm_substream *substream)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd;

	unsigned int bUseSE = FALSE;

	LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;
	LX_AUD_MASTER_SOUNDBAR_STATUS_T	soundbarStatus;

	/* Check a Audio Driver Init Status */
	if(g_AudKdrv.bInitDone == FALSE)
	{
		AUD_KDRV_ALSA_PRINT("Audio Device is not initialized!!!(bInitDone = %d)\n", g_AudKdrv.bInitDone);

		/* Wait for feed data to init dsp. */
		OS_MsecSleep(1000);

		return -EIO;
	}

	prtd = kzalloc(sizeof(struct runtime_data), GFP_KERNEL);
	if(prtd == NULL)
	{
		AUD_KDRV_ERROR("[%d] %s : kzalloc is failed.\n", __L__, __F__);
		return -ENOMEM;
	}

	/* Copy a substream structure from app. */
	runtime->private_data = prtd;
	prtd->substream = substream;

	/* Set a HW params now that initialization is complete */
	snd_soc_set_runtime_hwparams(substream, &lg115x_pcm_hardware);

	/* Ensure that period size is multiple of 8 */
	ret = snd_pcm_hw_constraint_step(runtime, 0, SNDRV_PCM_HW_PARAM_PERIOD_BYTES, 0x8);

	/* Ensure that buffer size is a multiple of period size */
	ret = snd_pcm_hw_constraint_integer(runtime, SNDRV_PCM_HW_PARAM_PERIODS);

	/* Open a Audio Master Device and Initialize audio dsp firmware and hardware. */
	prtd->alloc_dev_master = AUDIO_OpenMaster();
	if(prtd->alloc_dev_master == LX_AUD_DEV_NONE)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_OpenMaster is failed.\n", __L__, __F__);
		return -EIO;
	}

	/* Open a Audio Render Device. */
	prtd->alloc_dev_render = AUDIO_OpenRenderer();
	if(prtd->alloc_dev_render == LX_AUD_DEV_NONE)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_OpenRenderer is failed.\n", __L__, __F__);

		/* Close a Opened Master Device. */
		(void)AUDIO_CloseMaster(prtd->alloc_dev_master);

		AUD_KDRV_ERROR("AUDIO_OpenRenderer is failed.\n");
		return -EIO;
	}

	/* Set a device type, device handler and index */
	devIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
	devIndexCtx.dev = prtd->alloc_dev_render;
	devIndexCtx.ui32Index = ALSA_GET_INDEX(substream->pcm->device);
	devIndexCtx.pCtx = NULL;

	/* Set a device indec for context. */
	ret = AUDIO_SetDevIndexCtx(prtd->alloc_dev_render, &devIndexCtx);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_SetDevIndexCtx is failed.\n", __L__, __F__);
	}

	/* Get a Sound Bar Status - Sound Canvas : On/Off */
	ret = AUDIO_GetSoundBarStatus(&soundbarStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetSoundBarStatus is failed.\n", __L__, __F__);
	}

	/* Sound Frame is controlled for SE Output. */
	if(soundbarStatus.ui32SoundFrameOnOff == TRUE)
	{
		/* Sets a SPDIF PCM output with SE ouput. */
		bUseSE = TRUE;
	}
	else
	{
		/* Sets a SPDIF PCM output with Mixer ouput. */
		bUseSE = FALSE;
	}

	/* Connects a Master and Master Render context for SPDIF PCM output with MIXER ouput. */
	ret = AUDIO_ConnectOutput(prtd->alloc_dev_master, LX_AUD_MASTER_OUTPUT_SPDIF, bUseSE);
	if(ret != RET_OK)
	{
		/* Set a connected status for spdif. */
		prtd->spdif_is_connected = 0;

		AUD_KDRV_ALSA_PRINT("[%d] %s : AUDIO_ConnectOutput is failed.\n", __L__, __F__);
	}
	else
	{
		/* Set a connected status for spdif. */
		prtd->spdif_is_connected = 1;
	}

	/* Initialize a running status. */
	prtd->running = ALSA_DEVICE_STOP;

	/* Initialize a buffer write pointer. */
	prtd->write_ptr = 0;

	/* Initialize a connected status for render. */
	prtd->render_is_connected = 0;
	prtd->master_is_connected = 0;
	prtd->wake_up_is_called = 0;
	prtd->old_free_size = 0;

	/* Copy a local variable to file descriptor. */
	runtime->private_data = prtd;

	/* Check a max mixer deice number. */
	if(substream->pcm->device < ALSA_DEVICE_NUM)
	{
		/* Set a local run time data to use task. */
		lg115x_runtime_data[substream->pcm->device] = runtime->private_data;
	}
	else
	{
		AUD_KDRV_ERROR("[%d] %s : Device number is error!!!(device = %d)\n", __L__, __F__, substream->pcm->device);
		return -EIO;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(master:%d, render:%d, spdif:%d)\n", 	\
						 __L__, __F__, prtd->alloc_dev_master, prtd->alloc_dev_render, prtd->spdif_is_connected);
	return RET_OK;
}

static int lg115x_pcm_close(struct snd_pcm_substream *substream)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd 	 = runtime->private_data;

	if(prtd == NULL)
	{
		AUD_KDRV_ERROR("[%d] %s : runtime_data is NULL.\n", __L__, __F__);
		return EINVAL;
	}

	/* Close a Opened Audio Render Device. */
	ret = AUDIO_CloseRenderer(prtd->alloc_dev_render);
	prtd->render_is_connected = 0;
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_CloseRenderer is failed.\n", __L__, __F__);
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s : SPDIF Connect Status(spdif:%d)\n",	\
						 __L__, __F__, prtd->spdif_is_connected);

	/* Check a connected status for spdif. */
	if(prtd->spdif_is_connected == 1)
	{
		/* Disconnects a Master and Master context for SPDIF PCM output */
		ret = AUDIO_DisconnectOutput(prtd->alloc_dev_master, LX_AUD_MASTER_OUTPUT_SPDIF);
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_DisconnectOutput is failed.\n", __L__, __F__);
		}
	}

	/* Close a Opened Master Device. */
	ret = AUDIO_CloseMaster(prtd->alloc_dev_master);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_CloseMaster is failed.\n", __L__, __F__);
	}

	/* Check a max mixer deice number. */
	if(substream->pcm->device < ALSA_DEVICE_NUM)
	{
		/* Set a local run time data to use task. */
		lg115x_runtime_data[substream->pcm->device] = NULL;
	}
	else
	{
		AUD_KDRV_ERROR("[%d] %s : Device number is error!!!(device = %d)\n", __L__, __F__, substream->pcm->device);
		return -EIO;
	}

	OS_MsecSleep(10);

	/* Free a local allocated memory. */
	kfree(prtd);

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(state:%d)\n", __L__, __F__, runtime->status->state);
	return ret;
}

int lg115x_pcm_lib_ioctl(struct snd_pcm_substream *substream, unsigned int cmd, void *arg)
{
	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed.(cmd:%d)\n", __L__, __F__, cmd);
	return RET_OK;
}

static int lg115x_pcm_hw_params(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	int ret = RET_OK;

	/* Create audio ALSA monitor task. */
	if(_gpstAudioAlsaTask[substream->pcm->device] == NULL)
	{
		_gpstAudioAlsaTask[substream->pcm->device] = kthread_run(lg115x_pcm_monitor_task, NULL, "AUD-ALSA-TASK");

		if (_gpstAudioAlsaTask[substream->pcm->device] != NULL)
			AUD_KDRV_ALSA_DEBUG("[%d] %s : ALSA Monitor task is created.\n", __L__, __F__);
		else
			AUD_KDRV_ERROR("[%d] %s : ALSA Monitor task creation error!!!.\n", __L__, __F__);
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(params:%d)\n", __L__, __F__, params_format(params));
	return ret;
}

static int lg115x_pcm_hw_free(struct snd_pcm_substream *substream)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd 	 = runtime->private_data;

	/* Release audio ALSA monitor task */
	if(_gpstAudioAlsaTask[substream->pcm->device] != NULL)
	{
		kthread_stop(_gpstAudioAlsaTask[substream->pcm->device]);
		_gpstAudioAlsaTask[substream->pcm->device] = NULL;

		AUD_KDRV_ALSA_PRINT("[%d] %s : ALSA Monitor task is stopped.\n", __L__, __F__);
	}

	/* Check a running status. */
	if (prtd->running != ALSA_DEVICE_STOP)
	{
		/* Flush a Audio Render Device. */
		ret = AUDIO_FlushRenderer(prtd->alloc_dev_render);
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_FlushRenderer is failed.\n", __L__, __F__);
		}
	}

	/* Check a connected status for render. */
	if(prtd->render_is_connected == 1)
	{
		/* Disconnects a Audio Render & Render Device. */
		ret = AUDIO_DisconnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_render);
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_DisconnectRenderer(render) is failed.\n", __L__, __F__);
			return ret;
		}
		//prtd->render_is_connected = 0;

		/* Disconnects a Audio Render & Master Device. */
		ret = AUDIO_DisconnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_master);
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_DisconnectRenderer(master) is failed.\n", __L__, __F__);
			return ret;
		}

		/* Reset a connected status for render. */
		prtd->master_is_connected = 0;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(connected:%d)\n", __L__, __F__, prtd->render_is_connected);
	return RET_OK;
}

static int lg115x_pcm_prepare(struct snd_pcm_substream *substream)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime 	= substream->runtime;
	struct runtime_data 	*prtd 		= runtime->private_data;
	snd_pcm_format_t 		pcm_format 	= runtime->format;		/* SNDRV_PCM_FORMAT_* */

	LX_AUD_RENDER_PARAM_T 	    paramCodec = {LX_AUD_INPUT_SYSTEM, LX_AUD_RENDER_LIPSYNC_PCM, 2, 32, LX_AUD_SAMPLING_FREQ_48_KHZ,	\
										   	  LX_AUD_RENDER_LITTLE_ENDIAN, LX_AUD_RENDER_SIGNED, FALSE, FALSE};

	LX_AUD_RENDERED_STATUS_T 	renderedStatus;
	LX_AUD_DEV_INDEX_CTX_T	devIndexCtx;

	if(prtd->render_is_connected == 1)
	{
		/* Disconnects a Audio Render & Render Device. */
		ret = AUDIO_DisconnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_render);
		//prtd->render_is_connected = 0;

		/* Disconnects a Audio Render & Master Device. */
		ret = AUDIO_DisconnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_master);
		prtd->master_is_connected = 0;

		/* Close a Opened Audio Render Device. */
		ret = AUDIO_CloseRenderer(prtd->alloc_dev_render);
		prtd->render_is_connected = 0;
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_CloseRenderer is failed.\n", __L__, __F__);
		}

		prtd->alloc_dev_render = AUDIO_OpenRenderer();
		if(prtd->alloc_dev_render == LX_AUD_DEV_NONE)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_OpenRenderer is failed.\n", __L__, __F__);

			/* Close a Opened Master Device. */
			(void)AUDIO_CloseMaster(prtd->alloc_dev_master);

			AUD_KDRV_ERROR("AUDIO_OpenRenderer is failed.\n");
			return -EIO;
		}

		/* Set a device type, device handler and index */
		devIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
		devIndexCtx.dev = prtd->alloc_dev_render;
		devIndexCtx.ui32Index = ALSA_GET_INDEX(substream->pcm->device);
		devIndexCtx.pCtx = NULL;

		/* Set a device indec for context. */
		ret = AUDIO_SetDevIndexCtx(prtd->alloc_dev_render, &devIndexCtx);
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_SetDevIndexCtx is failed.\n", __L__, __F__);
		}
	}

	/* Set a renderer parameters. */
	paramCodec.input           	= LX_AUD_INPUT_SYSTEM;
	paramCodec.lipsyncType      = LX_AUD_RENDER_LIPSYNC_PCM;
	paramCodec.ui32NumOfChannel	= runtime->channels;
	paramCodec.ui32BitPerSample	= runtime->sample_bits;
	paramCodec.samplingFreq    	= runtime->rate ;
	paramCodec.endianType      	= LX_AUD_RENDER_LITTLE_ENDIAN;	///< the endian of PCM, 0 is little, 1 is big
	paramCodec.signedType      	= LX_AUD_RENDER_SIGNED;			///< the signed of PCM, 0 is signed, 1 is unsigned
	paramCodec.bSetMainRen      = FALSE;						///< Main Render mode for Main SRC control.(Main : SIF, HDMI Input Only)
	paramCodec.bSetTrickMode	= FALSE;						///< Render mode for trick play control.(TRUE : Decoder, FALSE : Mixer)

	/* Set a endian and signed type for PCM data. */
	switch(pcm_format)
	{
		case SNDRV_PCM_FORMAT_S8:
		case SNDRV_PCM_FORMAT_S16_LE:
		case SNDRV_PCM_FORMAT_S24_LE:
		case SNDRV_PCM_FORMAT_S32_LE:
		case SNDRV_PCM_FORMAT_S24_3LE:
			paramCodec.endianType = LX_AUD_RENDER_LITTLE_ENDIAN;	///< the endian of PCM, 0 is little, 1 is big
			paramCodec.signedType = LX_AUD_RENDER_SIGNED;			///< the signed of PCM, 0 is signed, 1 is unsigned
			break;

		case SNDRV_PCM_FORMAT_U8:
		case SNDRV_PCM_FORMAT_U16_LE:
		case SNDRV_PCM_FORMAT_U24_LE:
		case SNDRV_PCM_FORMAT_U32_LE:
		case SNDRV_PCM_FORMAT_U24_3LE:
			paramCodec.endianType = LX_AUD_RENDER_LITTLE_ENDIAN;	///< the endian of PCM, 0 is little, 1 is big
			paramCodec.signedType = LX_AUD_RENDER_UNSIGNED;			///< the signed of PCM, 0 is signed, 1 is unsigned
			break;

		case SNDRV_PCM_FORMAT_S16_BE:
		case SNDRV_PCM_FORMAT_S24_BE:
		case SNDRV_PCM_FORMAT_S32_BE:
		case SNDRV_PCM_FORMAT_S24_3BE:
			paramCodec.endianType = LX_AUD_RENDER_BIG_ENDIAN;		///< the endian of PCM, 0 is little, 1 is big
			paramCodec.signedType = LX_AUD_RENDER_SIGNED;			///< the signed of PCM, 0 is signed, 1 is unsigned
			break;

		case SNDRV_PCM_FORMAT_U16_BE:
		case SNDRV_PCM_FORMAT_U24_BE:
		case SNDRV_PCM_FORMAT_U32_BE:
		case SNDRV_PCM_FORMAT_U24_3BE:
			paramCodec.endianType = LX_AUD_RENDER_BIG_ENDIAN;		///< the endian of PCM, 0 is little, 1 is big
			paramCodec.signedType = LX_AUD_RENDER_UNSIGNED;			///< the signed of PCM, 0 is signed, 1 is unsigned
			break;

		default:
			paramCodec.endianType = LX_AUD_RENDER_LITTLE_ENDIAN;	///< the endian of PCM, 0 is little, 1 is big
			paramCodec.signedType = LX_AUD_RENDER_SIGNED;			///< the signed of PCM, 0 is signed, 1 is unsigned
			break;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s : Params(ch:%d)(bs:%d)(sr:%d)(big-endian:%d)(un-signed:%d)\n",	\
						 __L__, __F__, paramCodec.ui32NumOfChannel, paramCodec.ui32BitPerSample, paramCodec.samplingFreq, paramCodec.endianType, paramCodec.signedType);

	/* Set a Render Parameter Setting. */
	ret = AUDIO_SetRendererParam(prtd->alloc_dev_render, &paramCodec);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_SetRendererParam is failed.\n", __L__, __F__);
		return ret;
	}

	/* Disables a audio Lip Sync Mode by default because of calling this function for audio only play. */
	ret = AUDIO_EnableLipsync(prtd->alloc_dev_render, FALSE);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_EnableLipsync is failed.\n", __L__, __F__);
		return ret;
	}

	/* Connect a Audio Render & Render Device. */
	if (prtd->render_is_connected != 1)
	{
		ret = AUDIO_ConnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_render);

		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_ConnectRenderer(render) is failed.\n", __L__, __F__);
			return ret;
		}

		/* Set a connected status for render. */
		prtd->render_is_connected = 1;
	}

	/* Connect a Audio Render & Master Device. */
	if (prtd->master_is_connected != 1)
	{
		ret = AUDIO_ConnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_master);
		if(ret != RET_OK)
		{
			AUD_KDRV_ERROR("[%d] %s : AUDIO_ConnectRenderer(master) is failed.\n", __L__, __F__);

			/* Disconnects a Audio Render & Render Device. */
			ret = AUDIO_DisconnectRenderer(prtd->alloc_dev_render, prtd->alloc_dev_render);
			if(ret != RET_OK)
			{
				AUD_KDRV_ERROR("[%d] %s : AUDIO_DisconnectRenderer(render) is failed.\n", __L__, __F__);
			}
			return ret;
		}

		prtd->master_is_connected = 1;
	}

	/* Check a current clip buffer size. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Set a default buffer location. */
	//prtd->buffer_size_diff = renderedStatus.ui32FreeMemSize - frames_to_bytes(runtime, runtime->buffer_size);
	prtd->buffer_size_diff = 0;

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(free:%d)(diff:%d)(format:%d)(connected:%d)\n",	\
						 __L__, __F__, renderedStatus.ui32FreeMemSize, prtd->buffer_size_diff, runtime->format, prtd->render_is_connected);
	return ret;
}

static int lg115x_pcm_trigger(struct snd_pcm_substream *substream, int cmd)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd 	 = runtime->private_data;

    unsigned int 	free_size = 0;

    LX_AUD_RENDERED_STATUS_T renderedStatus;

	switch(cmd)
	{
		case SNDRV_PCM_TRIGGER_START:
		case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		case SNDRV_PCM_TRIGGER_RESUME:
			/* Start a Audio Render. */
			ret = AUDIO_StartRenderer(prtd->alloc_dev_render);
			if(ret != RET_OK)
			{
				AUD_KDRV_ERROR("[%d] %s : AUDIO_StartRenderer is failed.\n", __L__, __F__);
			}
			else
			{
				/* Update a running status. */
				prtd->running = ALSA_DEVICE_RUNNING;

				/* Check a max mixer deice number. */
				if(substream->pcm->device < ALSA_DEVICE_NUM)
				{
					/* Copy a local run time data to use task. */
//					lg115x_runtime_data[substream->pcm->device].running = 1;
				}
				else
				{
					AUD_KDRV_ERROR("[%d] %s : Device number is error!!!(device = %d)\n", __L__, __F__, substream->pcm->device);
					return -EIO;
				}
			}
			break;

		case SNDRV_PCM_TRIGGER_STOP:
		case SNDRV_PCM_TRIGGER_SUSPEND:
			/* Stop a Audio Render. */
			ret = AUDIO_StopRenderer(prtd->alloc_dev_render);
			if(ret != RET_OK)
			{
				AUD_KDRV_ERROR("[%d] %s : AUDIO_StopRenderer is failed.\n", __L__, __F__);
			}
			else
			{
				/* Flush a Audio Render Device. */
				ret = AUDIO_FlushRenderer(prtd->alloc_dev_render);
				if(ret != RET_OK)
				{
					AUD_KDRV_ERROR("[%d] %s : AUDIO_FlushRenderer is failed.\n", __L__, __F__);
				}

				/* Update a running status. */
				prtd->running = ALSA_DEVICE_STOP;
			}
			break;

		case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
			/* Stop a Audio Render. */
			ret = AUDIO_StopRenderer(prtd->alloc_dev_render);
			if(ret != RET_OK)
			{
				AUD_KDRV_ERROR("[%d] %s : AUDIO_StopRenderer is failed.\n", __L__, __F__);
			}
			else
			{
				/* Update a running status. */
				prtd->running = ALSA_DEVICE_STOP;
			}
			break;

		default:
			ret = -EINVAL;
	}

	/* Check a Audio Render Buffer Status. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Compute a free render buffer size. */
	free_size = renderedStatus.ui32FreeMemSize - prtd->buffer_size_diff;

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(cmd:%d)(running:%d)(free_size:%d)(state:%d)\n",	\
						 __L__, __F__, cmd, prtd->running, free_size, runtime->status->state);
	return ret;
}

static snd_pcm_uframes_t lg115x_pcm_pointer(struct snd_pcm_substream *substream)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd 	 = runtime->private_data;

    unsigned int 	free_size = 0, read_ptr_byte = 0;

	LX_AUD_RENDERED_STATUS_T 	renderedStatus;

	/* Check a Audio Render Buffer Status. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Compute a free render buffer size. */
	free_size = renderedStatus.ui32FreeMemSize - prtd->buffer_size_diff;

	/* Compute a PCM buffer read pointer. */
	read_ptr_byte = (prtd->write_ptr + free_size)%frames_to_bytes(runtime, runtime->buffer_size);

	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed.(read_ptr:%d)(free_size:%d)(state:%d)\n",	\
						__L__, __F__, read_ptr_byte, free_size, runtime->status->state);
	return bytes_to_frames(runtime, read_ptr_byte);
}

static int lg115x_pcm_copy(struct snd_pcm_substream *substream,		\
						   int channel, snd_pcm_uframes_t pos, void __user *buf, snd_pcm_uframes_t count)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd 	 = runtime->private_data;

    unsigned int 	free_size = 0;
	unsigned long 	byte_size = 0;

	LX_AUD_RENDER_FEED_T 		renFeed;
	LX_AUD_RENDERED_STATUS_T 	renderedStatus;

	/* Check a Audio Render Buffer Status. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Compute a free render buffer size. */
	free_size = renderedStatus.ui32FreeMemSize - prtd->buffer_size_diff;

	/* Compute a PCM buffer write bytes. */
	byte_size = frames_to_bytes(runtime, count);

	/* Check a Audio Render Buffer Status. */
	if(free_size < (unsigned int)byte_size)
	{
		AUD_KDRV_ALSA_PRINT("[%d] %s : free_size is small.(free_size:%d)(byte_size:%d)\n", __L__, __F__, free_size, (unsigned int)byte_size);

		/* Wait for next feed data. */
		OS_MsecSleep(30);

		return ret;
	}

	/* Set a PCM buffer size and buffer address pointer .*/
	renFeed.pBuffer 		 = buf;
	renFeed.statusBuffer 	 = LX_AUD_BUFFER_START;
	renFeed.ui32BufferSize 	 = byte_size;
	renFeed.ui64TimeStamp 	 = 0;
	renFeed.ui32RepeatNumber = 1;
	renFeed.bUseGstcClock	 = FALSE;

	/* Feeds a audio PCM data. */
	ret = AUDIO_FeedRen(prtd->alloc_dev_render, &renFeed);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_FeedRen is failed.\n", __L__, __F__);
		return ret;
	}

	/* Updates a buffer write pointer. */
	prtd->write_ptr = (prtd->write_ptr + byte_size)%frames_to_bytes(runtime, runtime->buffer_size);

	/* Wait for feed data to play dsp. */
	//OS_MsecSleep(6);

	/* Check a Audio Render Buffer Status. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Compute a free render buffer size. */
	free_size = renderedStatus.ui32FreeMemSize - prtd->buffer_size_diff;

	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed.(cnt:%lu)(write:%u)(free:%d)(ch:%d)(pos:%lu)(state:%d)\n",	\
						__L__, __F__, count, prtd->write_ptr, free_size, channel, pos, runtime->status->state);
	return ret;
}

static int lg115x_pcm_silence(struct snd_pcm_substream *substream,	\
							  int channel, snd_pcm_uframes_t pos, snd_pcm_uframes_t count)
{
	int ret = RET_OK;

	struct snd_pcm_runtime 	*runtime = substream->runtime;
	struct runtime_data 	*prtd 	 = runtime->private_data;

	char *temp_buf;

    unsigned int 	free_size = 0;
	unsigned long 	byte_size  = 0;

	LX_AUD_RENDER_FEED_T 		renFeed;
	LX_AUD_RENDERED_STATUS_T 	renderedStatus;

	/* Check a Audio Render Buffer Status. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Compute a free render buffer size. */
	free_size = renderedStatus.ui32FreeMemSize - prtd->buffer_size_diff;

	/* Compute a PCM buffer write bytes. */
	byte_size = frames_to_bytes(runtime, count);

	/* Check a Audio Render Buffer Status. */
	if(free_size < (unsigned int)byte_size)
	{
		AUD_KDRV_ERROR("[%d] %s : free_size is small.(free_size:%d)(byte_size:%d)\n", __L__, __F__, free_size, (unsigned int)byte_size);
		return ret;
	}

	/* Set a PCM buffer write data to zero. */
	temp_buf = kzalloc(byte_size, GFP_KERNEL);
	if(temp_buf == NULL)
	{
		AUD_KDRV_ERROR("[%d] %s : kzalloc is failed.\n", __L__, __F__);
		return -ENOMEM;
	}

	/* Set a PCM buffer size and buffer address pointer .*/
	renFeed.pBuffer 		 = temp_buf;
	renFeed.statusBuffer 	 = LX_AUD_BUFFER_START;
	renFeed.ui32BufferSize 	 = byte_size;
	renFeed.ui64TimeStamp 	 = 0;
	renFeed.ui32RepeatNumber = 1;
	renFeed.bUseGstcClock	 = FALSE;

	/* Feeds a audio PCM data. */
	ret = AUDIO_FeedRen(prtd->alloc_dev_render, &renFeed);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_FeedRen is failed.\n", __L__, __F__);
		return ret;
	}

	/* Updates a buffer write pointer. */
	prtd->write_ptr = (prtd->write_ptr + byte_size)%frames_to_bytes(runtime, runtime->buffer_size);

	/* Wait for feed data to play dsp. */
	//OS_MsecSleep(6);

	/* Free a local allocated memory. */
	if(temp_buf != NULL)
	{
		kfree(temp_buf);
	}

	/* Check a Audio Render Buffer Status. */
	ret = AUDIO_GetRenderedStatus(prtd->alloc_dev_render, &renderedStatus);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
		return ret;
	}

	/* Compute a free render buffer size. */
	free_size = renderedStatus.ui32FreeMemSize - prtd->buffer_size_diff;

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.(cnt:%lu)(write:%u)(free:%d)(ch:%d)(pos:%lu)\n",	\
						__L__, __F__, count, prtd->write_ptr, free_size, channel, pos);
	return ret;
}

static int lg115x_pcm_mmap(struct snd_pcm_substream *substream,	struct vm_area_struct *vma)
{
	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;
}

static int lg115x_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;
}

static void lg115x_pcm_free(struct snd_pcm *pcm)
{
	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return;
}

static int __devinit lg115x_platform_probe(struct platform_device *pdev)
{
	int ret = RET_OK;

	ret = snd_soc_register_platform(&pdev->dev, &lg115x_soc_platform_driver);
	if(ret != RET_OK)
	{
		ret = -ENOMEM;
		AUD_KDRV_ERROR("[%d] %s : Could not register Platform: %d.\n", __L__, __F__, ret);
		goto err_register;
	}

	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;

err_register:
	AUD_KDRV_ERROR("[%d] %s : Return Value(%d).\n", __L__, __F__, ret);
	return ret;
}

static int __devexit lg115x_platform_remove(struct platform_device *pdev)
{
	snd_soc_unregister_platform(&pdev->dev);

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;
}

static int lg115x_pcm_monitor_task(void *pParam)
{
	int ret = RET_OK;
    unsigned int 	loop = 0, free_size = 0;
	LX_AUD_RENDERED_STATUS_T 	renderedStatus;

	AUD_KDRV_ALSA_PRINT("[%d] %s : Audio Monitor Task is started!!!\n", __L__, __F__);

	do
	{
		/* Check stop condition when device is closed. */
		if( kthread_should_stop())
		{
			AUD_KDRV_ALSA_DEBUG("[%d] %s : Audio Monitor Task - exit!!!\n", __L__, __F__);
			break;
		}

		/* Wait for feed data to play dsp. */
		OS_MsecSleep(50);

		/* Check a NULL status for runtime data. */
		for(loop = 0; loop < ALSA_DEVICE_NUM; loop++)
		{
			if(lg115x_runtime_data[loop] != NULL)
			{
				/* Check a render running status and device number. */
				if(lg115x_runtime_data[loop]->running == ALSA_DEVICE_RUNNING && lg115x_runtime_data[loop]->wake_up_is_called == 0)
				{
					/* Check a Audio Render Buffer Status. */
					ret = AUDIO_GetRenderedStatus(lg115x_runtime_data[loop]->alloc_dev_render, &renderedStatus);
					if(ret != RET_OK)
					{
						AUD_KDRV_ERROR("[%d] %s : AUDIO_GetRenderedStatus is failed.\n", __L__, __F__);
						break;
					}

					/* Compute a free render buffer size. */
					free_size = renderedStatus.ui32FreeMemSize - lg115x_runtime_data[loop]->buffer_size_diff;

					if (lg115x_runtime_data[loop]->wake_up_is_called == 0 && lg115x_runtime_data[loop]->substream != NULL)
					{
						/* Compute a free render buffer size. */
						//if(free_size == (renderedStatus.ui32MaxMemSize - 4))	/* ui32FreeMemSize = ui32MaxMemSize - 4 */
						if(free_size == lg115x_runtime_data[loop]->old_free_size && lg115x_runtime_data[loop]->substream->runtime->status->state == SNDRV_PCM_STATE_DRAINING)
						{
							{
								lg115x_runtime_data[loop]->wake_up_is_called = 1;
								lg115x_runtime_data[loop]->substream->runtime->status->state = SNDRV_PCM_STATE_SETUP;
								wake_up(&(lg115x_runtime_data[loop]->substream->runtime->sleep));
							}
							AUD_KDRV_ALSA_DEBUG("[%d] %s : Wake-Up %d(free_size:%d)(running:%d)\n", __L__, __F__, loop, free_size, lg115x_runtime_data[loop]->running);
						}
						else
						{
							if(lg115x_runtime_data[loop]->substream->runtime->status->state == SNDRV_PCM_STATE_RUNNING)
							{
								snd_pcm_period_elapsed(lg115x_runtime_data[loop]->substream);
								wake_up(&(lg115x_runtime_data[loop]->substream->runtime->sleep));
							}
							else
							{
								lg115x_runtime_data[loop]->old_free_size = free_size;
								AUD_KDRV_ALSA_DEBUG("[%d] %s : Running %d(free_size:%d)(running:%d)\n", __L__, __F__, loop, free_size, lg115x_runtime_data[loop]->running);
							}
						}
					}
				}
				else
				{
					AUD_KDRV_ALSA_DEBUG("[%d] %s : Stop %d(running:%d)\n",	__L__, __F__, loop, lg115x_runtime_data[loop]->running);
				}
			}
			else
			{
				//AUD_KDRV_ALSA_DEBUG("[%d] %s\n", __L__, __F__);
			}
		}
	} while (TRUE);

	AUD_KDRV_ALSA_PRINT("[%d] %s : Audio Monitor Task is ended!!!\n", __L__, __F__);
	return RET_OK;
}



int ALSA_PLATFORM_PCM_Init(void)
{
	int ret = RET_OK;

	/* Add and Register a Platform driver. */
	ret = platform_driver_register(&lg115x_platform_driver);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : Can not register platform driver.\n", __L__, __F__);
		goto out;
	}

	lg115x_platform_device = platform_device_alloc(PLATFORM_NAME, -1);
	if(!lg115x_platform_device)
	{
		AUD_KDRV_ERROR("[%d] %s : Can not allocate platform device.\n", __L__, __F__);
		ret = -ENOMEM;
		goto out;
	}

	ret = platform_device_add(lg115x_platform_device);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : Can not register platform device.\n", __L__, __F__);
		goto out;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return ret;

out:
	AUD_KDRV_ERROR("[%d] %s : Error Return Value(%d).\n", __L__, __F__, ret);
	return ret;
}

void ALSA_PLATFORM_PCM_Cleanup(void)
{
	/* Delete and unregister a Platform PCM driver. */
	platform_device_del(lg115x_platform_device);
	platform_driver_unregister(&lg115x_platform_driver);

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return;
}



