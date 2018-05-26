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



/** @file	audio_kdrv_master.c
 *
 *  main driver implementation for  audio master device.
 *  audio master device will teach you how to make device driver with new platform.
 *
 *  author	wonchang.shin (wonchang.shin@lge.com)
 *  version	0.1
 *  date		2012.04.25
 *  note		Additional information.
 *
 *  @addtogroup lg1150_audio
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
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/interrupt.h>    /**< For isr */
#include <linux/irq.h>			/**< For isr */
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <linux/rmap.h>
#include <linux/kthread.h>
#include <asm/io.h>			/**< For ioremap_nocache */
#include <asm/memory.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/version.h>



#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"
#include "audio_cfg.h"
#include "audio_drv.h"
#include "audio_drv_manager.h"

#include "common/adec_config.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/


#define FINDMOD_STATE_MAX   4
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern UINT8 *strDevName [LX_AUD_DEV_MAX] ;
extern UINT8 *strDevType[LX_AUD_DEV_TYPE_MAX];
extern AUD_MOD_INFO_T _gModInfo[ADEC_MODULE_MAX];
extern AUD_DEV_MAN_INFO_T _gDevManInfo;
extern UINT32 _gModTypeNum[ADEC_MOD_TYPE_MAX];
extern UINT32 _gModTypeBase[ADEC_MOD_TYPE_MAX];


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

UINT8 *  modStatusStr[AUD_STATUS_MAX] = // device node(D) and module(M)
{
    "NONE",
    "INIT ",		//56
    "START",		//56
    "STOP",
    "FLUSH",
};

/**
 * Show alloc information of Device and Module
 *
 * @param 	: none
 * @return 	if succeeded -RET_OK , else - .
 * @see
 */

void  AUDIO_UpdateAllocInfo (void )
{
	SINT32 i, j, k;

	LX_AUD_DEV_TYPE_T		devType = LX_AUD_DEV_TYPE_NONE;
	AUD_DEV_INFO_T			*pDevInfo = NULL;
	AUD_MOD_INFO_T			*pModInfo = NULL;

	UINT32	searchStart;
	UINT32	availbleNum, allocBase;


	char		strsum[25];
	char		tstrsum[250];
	char		instr[3];
	char		i_instr[6];
	char		tinstr[60];
	char		outstr[3];
	char		i_outstr[6];
	char		toutstr[60];

	char		used[25];
	used[0]='-';
	used[1]='-';
	used[2]='\0';

	strsum[0]='\0';
	tstrsum[0]='\0';
	AUD_KDRV_PRINTF("\n");
	AUD_KDRV_PRINTF("========================================================= DEV INFO =======================================================\n");

	strsum[0]='\0';
	tstrsum[0]='\0';
	for(i = LX_AUD_DEV_TYPE_DEC; i < LX_AUD_DEV_TYPE_MAX; i++)
	{
		allocBase 		= _gDevManInfo.allocBase[i];
		availbleNum 	= _gDevManInfo.availableNum[i];

		for(searchStart = allocBase; searchStart < allocBase + availbleNum; searchStart++)
		{
			pDevInfo = &_gDevManInfo.devInfo[searchStart];
			if(pDevInfo->numMod > 0)
			{
				for(j = 0 ; j < MAX_OWN_MODULE; j++)
				{
					if(pDevInfo->pModInfo[j] != NULL)
					{
					 	if(pDevInfo->pModInfo[j]->mod != ADEC_MODULE_NOT_DEF)
						{
							sprintf(strsum, "[%9s] ", ModuleList_GetModuleName(pDevInfo->pModInfo[j]->mod));
							strcat(tstrsum, strsum);
					 	}
					}
				}

				AUD_KDRV_PRINTF("[%9s] : %s\n", strDevName[searchStart], tstrsum);
				strsum[0]='\0';
				tstrsum[0]='\0';
			}

		}

		devType++;
	}
	AUD_KDRV_PRINTF("==========================================================================================================================\n");

	devType = LX_AUD_DEV_TYPE_DEC;
	for(i = LX_AUD_DEV_TYPE_DEC; i < LX_AUD_DEV_TYPE_MAX; i++)
	{
		allocBase 		= _gDevManInfo.allocBase[i];
		availbleNum 	= _gDevManInfo.availableNum[i];

		for(searchStart = allocBase; searchStart < allocBase + availbleNum; searchStart++)
		{
			pDevInfo = &_gDevManInfo.devInfo[searchStart];

			instr[0]='\0';			i_instr[0]='\0';
			tinstr[0]='\0';
			strsum[0]='\0';

			for(j = 0 ; j < MAX_OWN_MODULE; j++)
			{
				if(pDevInfo->pModInfo[j] != NULL)
				{
					sprintf(instr, "%2d", pDevInfo->pModInfo[j]->mod);
					sprintf(i_instr, "%2s:",(pDevInfo->pModInfo[j]->mod != ADEC_MODULE_NOT_DEF)? instr : strsum);
					strcat(tinstr, i_instr);
				}
			}

			if(pDevInfo->dev != LX_AUD_DEV_NONE)
			{
				AUD_KDRV_PRINTF("[%9s:%2d] : modNum:%2d head[%9s] tail[%9s] own[%s]\n",
					strDevName[searchStart], pDevInfo->dev,
					pDevInfo->numMod,
					ModuleList_GetModuleName(AUDIO_FindHeadModule(pDevInfo->dev)),
					ModuleList_GetModuleName(AUDIO_FindTailModule(pDevInfo->dev)),
					tinstr
				);
			}
		}

		devType++;
	}

	AUD_KDRV_PRINTF("========================================================= MOD INFO =======================================================\n");
	for(i = ADEC_MOD_TYPE_SOURCE; i < ADEC_MOD_TYPE_MAX; i++)
	{
		for(k = 1; k < ADEC_MODULE_MAX; k++)
		{
			pModInfo = &_gModInfo[k];

			if(pModInfo->modType != i)
				continue;

			instr[0]='\0';			i_instr[0]='\0';
			tinstr[0]='\0';
			outstr[0]='\0';			i_outstr[0]='\0';
			toutstr[0]='\0';
			strsum[0]='\0';
			tstrsum[0]='\0';

			for(j = 0; j < MAX_IN_PORT; j++)
			{
				sprintf(instr, "%2d", pModInfo->inPort[j].mod);
				sprintf(i_instr, "%2s",(pModInfo->inPort[j].mod != ADEC_MODULE_NOT_DEF)? instr : strsum);
				strcat(tinstr, i_instr);
			}

			for(j = 0; j < MAX_OUT_PORT; j++)
			{
				sprintf(outstr,"%2d", pModInfo->outPort[j].mod);
				sprintf(i_outstr, "%2s", (pModInfo->outPort[j].mod != ADEC_MODULE_NOT_DEF) ? outstr : used);
				strcat(toutstr, i_outstr);
			}

			instr[0]='\0';
			i_instr[0]='\0';

			sprintf(instr,"%2d", pModInfo->mod);
			sprintf(i_instr,"%2s",(pModInfo->mod != ADEC_MODULE_NOT_DEF)? instr : strsum);

			if(pModInfo->allocated == TRUE)
			{
				AUD_KDRV_PRINTF("[%10s:%2d:%2x:%5s]Dev%2d I[%s]O[%s]\n",
								ModuleList_GetModuleName(k),
								k,
								k,
								modStatusStr[pModInfo->status],
								pModInfo->dev,
								tinstr,
								toutstr
								);
			}
		}
	}
	AUD_KDRV_PRINTF("==========================================================================================================================\n");
}

void  AUDIO_UpdateConnectionInfo(void)
{
	AUDIO_UpdateAllocInfo();
}


/**
 * show Audio Device Manager Information for debug
 * @see
*/
void AUDIO_ShowDevManInfo( void )
{
	SINT32	i;
	AUD_DEV_INFO_T *pDevInfo = NULL;
	AUD_MOD_INFO_T *pModInfo = NULL;


	for(i = LX_AUD_DEV_DEC0; i < LX_AUD_DEV_MAX;i++)
	{
		pDevInfo = AUDIO_GetDevInfo(i);
		if(pDevInfo == NULL)
			continue;
		AUD_KDRV_PRINTF("dev(%d) devType(%d) index(%d)\n", pDevInfo->dev, pDevInfo->devType, pDevInfo->index);
	}

	for(i = 1; i < ADEC_MODULE_MAX;i++)
	{
		pModInfo = AUDIO_GetModuleInfo(i);
		if(pModInfo == NULL)
			continue;
		AUD_KDRV_PRINTF("mod(%d) modType(%d) coreType(%d) allocated(%d) numInPort(%d) numOutPort(%d)\n",
			pModInfo->mod, pModInfo->modType, pModInfo->coreType, pModInfo->allocated,
			pModInfo->numInPort, pModInfo->numOutPort);
	}
}


