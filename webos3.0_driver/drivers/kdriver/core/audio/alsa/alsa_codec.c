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



/** @file alsa_codec.c
 *
 *  This is alsa codec driver.
 *
 *  @author		Won Chang Shin(wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2013.03.20
 *  @note		The functions for alsa codec driver.
 *
 *	@{
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/initval.h>

#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "audio_drv.h"
#include "audio_drv_manager.h"
#include "audio_drv_renderer.h"
#include "audio_drv_master.h"
#include "audio_kapi_master.h"


#include "alsa_codec.h"
#include "alsa_platform.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

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
#define LG115x_SOC_CODEC_DAI(id) \
	{ \
		.name		= CODEC_DAI_NAME "." #id, \
		.playback 	= { \
			.stream_name	= "Playback." #id, \
			.channels_min	= 1, \
			.channels_max	= 8, \
			.rates			= LG115x_SUPPORT_PLAYBACK_RATE, \
			.formats		= LG115x_SUPPORT_PLAYBACK_FORMAT, \
		}, \
	}
/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern int snd_soc_register_codec(struct device *dev, const struct snd_soc_codec_driver *codec_drv,	\
								  struct snd_soc_dai_driver *dai_drv, int num_dai);


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
static int lg115x_codec_probe(struct platform_device *pdev);
static int lg115x_codec_remove(struct platform_device *pdev);
static int lg115x_volume_get(struct snd_kcontrol *kcontrol,	struct snd_ctl_elem_value *ucontrol);
static int lg115x_volume_put(struct snd_kcontrol *kcontrol,	struct snd_ctl_elem_value *ucontrol);

static int lg115x_control_probe(struct snd_soc_codec *codec);
static int lg115x_control_remove(struct snd_soc_codec *codec);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static struct snd_soc_codec_driver lg115x_soc_codec_driver = {
	.probe 	= lg115x_control_probe,
	.remove = lg115x_control_remove,
};

static struct platform_driver lg115x_codec_driver = {
		.driver		= {
			.name	= CODEC_NAME,
			.owner	= THIS_MODULE,
		},
		.probe		= lg115x_codec_probe,
		.remove		= __devexit_p(lg115x_codec_remove),
};

struct snd_soc_dai_driver lg115x_soc_codec_dai[] = {
	LG115x_SOC_CODEC_DAI(0),
	LG115x_SOC_CODEC_DAI(1),
	LG115x_SOC_CODEC_DAI(2),
	LG115x_SOC_CODEC_DAI(3),
	LG115x_SOC_CODEC_DAI(4),
	LG115x_SOC_CODEC_DAI(5),
	LG115x_SOC_CODEC_DAI(6),
	LG115x_SOC_CODEC_DAI(7),
};

static const struct snd_kcontrol_new lg115x_controls[ALSA_DEVICE_NUM] = {
	SOC_SINGLE_EXT("Volume0", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume1", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume2", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume3", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume4", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume5", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume6", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
	SOC_SINGLE_EXT("Volume7", 0, 0, ADEC_DSP_MIX_VOLUME_GAIN_0DB, 0, lg115x_volume_get, lg115x_volume_put),
};

static struct platform_device *lg115x_codec_device[ALSA_DEVICE_NUM];


/*========================================================================================
	Implementation Group
========================================================================================*/
static int lg115x_volume_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0)
		struct snd_soc_codec *codec 	  = snd_kcontrol_chip(kcontrol);
#else
		struct snd_soc_codec *codec 	  = snd_soc_kcontrol_codec(kcontrol);
#endif

	struct runtime_data  *lg115x_data = snd_soc_codec_get_drvdata(codec);

	if(lg115x_data == NULL)
	{
		AUD_KDRV_ERROR("[%d] %s : lg115x_data is NULL.\n", __L__, __F__);
		return RET_ERROR;
	}

	/* Get a mix volume from local memory. */
	ucontrol->value.integer.value[0] = (long)lg115x_data->volume;

	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed(id:%d)(val0:0x%lx).\n", __L__, __F__, 	\
						 codec->id, ucontrol->value.integer.value[0]);
	return RET_OK;
}

static int lg115x_volume_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	int ret = RET_OK;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,16,0)
	struct snd_soc_codec *codec 	  = snd_kcontrol_chip(kcontrol);
#else
	struct snd_soc_codec *codec 	  = snd_soc_kcontrol_codec(kcontrol);
#endif
	struct runtime_data  *lg115x_data = snd_soc_codec_get_drvdata(codec);

	LX_AUD_DEV_INDEX_CTX_T devIndexCtx;
	LX_AUD_MASTER_CONFIG_MIXER_VOLUME_T	mixerVolumeConfig;

	if(lg115x_data == NULL)
	{
		AUD_KDRV_ERROR("[%d] %s : lg115x_data is NULL.\n", __L__, __F__);
		return RET_ERROR;
	}

	/* Set a mix volume to local memory. */
	lg115x_data->volume = (unsigned int)ucontrol->value.integer.value[0];

	devIndexCtx.devType = LX_AUD_DEV_TYPE_REN;
	devIndexCtx.ui32Index = ALSA_GET_INDEX(codec->id);

	ret = AUDIO_GetDevIndexCtx(&devIndexCtx);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : AUDIO_GetDevIndexCtx is failed.\n", __L__, __F__);
		return ret;
	}

	/* Set a mixer volume. */
	mixerVolumeConfig.mixer 	  = LX_AUD_MASTER_MIXER_0;
	mixerVolumeConfig.ui32Port	  = ALSA_GET_INDEX(codec->id);
	mixerVolumeConfig.ui32Volume  = lg115x_data->volume;

	/* Sets audio mix volume that is before mixing main audio. */
	ret = AUDIO_SetMixerVolume(&mixerVolumeConfig);

	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : KADP_AUDIO_SetMixerVolume is failed.\n", __L__, __F__);
		return ret;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed(id:%d)(render_volume:0x%X).\n", __L__, __F__, 	\
						 codec->id, lg115x_data->volume);
	return RET_OK;
}

static int lg115x_control_probe(struct snd_soc_codec *codec)
{
	int ret = RET_OK;

	struct runtime_data *lg115x_data;

	/* Sanity Check. */
	if(codec->id >= ALSA_DEVICE_NUM)
	{
		AUD_KDRV_ERROR("[%d] %s : id(%d) is not supported.\n", __L__, __F__, codec->id);
		return -ENOMEM;
	}

	/* Add and Register a Codec Control driver. */
	lg115x_data = kzalloc(sizeof(struct runtime_data), GFP_KERNEL);
	if(lg115x_data == NULL)
	{
		AUD_KDRV_ERROR("[%d] %s : Cannot allocate memory.\n", __L__, __F__);
		return -ENOMEM;
	}

	snd_soc_codec_set_drvdata(codec, lg115x_data);
	ret = snd_soc_add_codec_controls(codec, &lg115x_controls[codec->id], 1);
	if(ret != RET_OK)
	{
		kfree(lg115x_data);
	}

	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed(id:%d).\n", __L__, __F__, codec->id);
	return RET_OK;
}

static int lg115x_control_remove(struct snd_soc_codec *codec)
{
	struct runtime_data *lg115x_data = snd_soc_codec_get_drvdata(codec);

	if(lg115x_data != NULL)
	{
		kfree(lg115x_data);
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed(id:%d).\n", __L__, __F__, codec->id);
	return 0;
}

static int lg115x_codec_probe(struct platform_device *pdev)
{
	int ret = RET_OK;

	ret = snd_soc_register_codec(&pdev->dev, &lg115x_soc_codec_driver, lg115x_soc_codec_dai,	\
								 ARRAY_SIZE(lg115x_soc_codec_dai));

	if(ret != RET_OK)
	{
		ret = -ENOMEM;
		AUD_KDRV_ERROR("[%d] %s : Could not register codec.(ret:%d).\n", __L__, __F__, ret);
		return ret;
	}

	AUD_KDRV_ALSA_DEBUG("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;
}

static int lg115x_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	platform_set_drvdata(pdev, NULL);

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;
}


int ALSA_CODEC_Init(void)
{
	int ret = RET_OK;

	unsigned int i, j;

	/* Add and Register a Codec driver. */
	ret = platform_driver_register(&lg115x_codec_driver);
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : Can not codec driver.\n", __L__, __F__);
		goto out;
	}

	for(i = 0; i < ALSA_DEVICE_NUM; i++)
	{
		lg115x_codec_device[i] = platform_device_alloc(CODEC_NAME, i);
		if(!lg115x_codec_device[i])
		{
			AUD_KDRV_ERROR("[%d] %s : Can not allocate codec device.\n", __L__, __F__);
			ret = -ENOMEM;
			goto out;
		}

		ret = platform_device_add(lg115x_codec_device[i]);
		if(ret != RET_OK)
		{
			for(j = 0; j <= i; j++)
			{
				platform_device_del(lg115x_codec_device[j]);
			}

			platform_driver_unregister(&lg115x_codec_driver);
			AUD_KDRV_ERROR("[%d] %s : Can not register codec device.\n", __L__, __F__);
			goto out;
		}
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return RET_OK;

out :
	AUD_KDRV_ERROR("[%d] %s : Error Return Value(%d).\n", __L__, __F__, ret);
	return ret;
}

void ALSA_CODEC_Cleanup(void)
{
	unsigned int i;

	/* Delete and unregister a Codec driver. */
	for(i = 0; i < ALSA_DEVICE_NUM; i++)
	{
		platform_device_del(lg115x_codec_device[i]);
		platform_driver_unregister(&lg115x_codec_driver);
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return;
}



