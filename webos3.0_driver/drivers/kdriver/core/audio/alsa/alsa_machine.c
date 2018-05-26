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



/** @file alsa_machine.c
 *
 *  This is alsa machine driver.
 *
 *  @author		Won Chang Shin(wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2013.03.20
 *  @note		The functions for alsa machin driver.
 *
 *	@{
 */

#include <linux/module.h>
#include <sound/soc.h>
#include <sound/pcm_params.h>

#include "os_util.h"
#include "base_device.h"
#include "debug_util.h"

#include "audio_drv.h"

#include "alsa_codec.h"
#include "alsa_platform.h"
#include "alsa_machine.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
#define LG115x_DAI_LINK(id) \
		{ \
			.name 			= CARD_NAME "." #id, \
			.stream_name 	= CARD_NAME "." #id, \
			.cpu_dai_name 	= DAI_NAME "." #id, \
			.codec_dai_name = CODEC_DAI_NAME "." #id, \
			.platform_name 	= PLATFORM_NAME, \
			.codec_name 	= CODEC_NAME "." #id, \
		}

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
struct snd_soc_dai_link lg115x_dai_link[] = {
	LG115x_DAI_LINK(0),
	LG115x_DAI_LINK(1),
	LG115x_DAI_LINK(2),
	LG115x_DAI_LINK(3),
	LG115x_DAI_LINK(4),
	LG115x_DAI_LINK(5),
	LG115x_DAI_LINK(6),
	LG115x_DAI_LINK(7),
};

static struct snd_soc_card lg115x = {
	.name 		= CARD_NAME,
	.owner 		= THIS_MODULE,
	.dai_link 	= lg115x_dai_link,
	.num_links 	= ARRAY_SIZE(lg115x_dai_link),
};

static struct platform_device *lg115x_audio_device;


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

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
int ALSA_MACHINE_Init(void)
{
	int ret = RET_OK;

	/* Initializes ALSA Platform DAI driver. */
	ret = ALSA_PLATFORM_DAI_Init();
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : ALSA_CODEC_Init is failed.\n", __L__, __F__);
		return ret;
	}

	/* Initializes ALSA Platform PCM driver. */
	ret = ALSA_PLATFORM_PCM_Init();
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : ALSA_CODEC_Init is failed.\n", __L__, __F__);
		return ret;
	}

	/* Initializes ALSA Codec driver. */
	ret = ALSA_CODEC_Init();
	if(ret != RET_OK)
	{
		AUD_KDRV_ERROR("[%d] %s : ALSA_CODEC_Init is failed.\n", __L__, __F__);
		return ret;
	}

	/* Add and Register a platform device driver. */
	lg115x_audio_device = platform_device_alloc("soc-audio", -1);
	if(!lg115x_audio_device)
	{
		AUD_KDRV_ERROR("[%d] %s : platform_device_alloc is failed.\n", __L__, __F__);
		return -ENOMEM;
	}

	/* Do not use snd_soc_register_card(This function should be called kernel function.) */
	platform_set_drvdata(lg115x_audio_device, &lg115x);

	/* Add a platform device. */
	ret = platform_device_add(lg115x_audio_device);
	if(ret != RET_OK)
	{
		/* Delete a platform device if error is found. */
		platform_device_del(lg115x_audio_device);

		AUD_KDRV_ERROR("[%d] %s : platform_device_add is failed.\n", __L__, __F__);
		return -ENOMEM;
	}

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return ret;
}

void ALSA_MACHINE_Cleanup(void)
{
	/* Delete and unregister a Platform PCM driver. */
	platform_device_del(lg115x_audio_device);
	platform_set_drvdata(lg115x_audio_device, NULL);

	/* Cleanup a Codec Driver. */
	(void)ALSA_CODEC_Cleanup();

	/* Cleanup a Platform PCM Driver. */
	(void)ALSA_PLATFORM_PCM_Cleanup();

	/* Cleanup a Platform DAI Driver. */
	(void)ALSA_PLATFORM_DAI_Cleanup();

	AUD_KDRV_ALSA_PRINT("[%d] %s is completed.\n", __L__, __F__);
	return;
}



