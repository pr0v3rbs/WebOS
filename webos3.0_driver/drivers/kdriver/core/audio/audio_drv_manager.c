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
#include "os_util.h"
#include "base_device.h"

#include "debug_util.h"
#include "audio_cfg.h"
#include "audio_drv.h"

#include "audio_buffer.h"

#include "imc/cmd/module_cmd_bypass.h"
#include "common/adec_config.h"
#include "buffer/adec_buf.h"

#include "audio_drv_master.h"
#include "audio_drv_decoder.h"
#include "audio_drv_manager.h"
#include "audio_imc_func.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define NOT_CLASSIFIED		0
#define LAST_MOD			1
#define MULTI_MOD			2
#define RM_MODULE_NUM		5

#define AUD_INPUT_MODULE_PATH_MAX 10

//#define BUFFER_SIZE	0x01E00000
#define BUFFER_SIZE	0x01000000
#define DSP_ADDR	0x68000000
#define REG_COUNT	76
#define MAX_REGION	19

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
extern LX_AUD_KDRV_T	g_AudKdrv;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

//need to sync with device num ,extra manager module
UINT8 *  strDevName [LX_AUD_DEV_MAX] = // device node(D) and module(M)
{
    "DEV_NONE",
    "DEV_DEC0",
    "DEV_DEC1",
    "DEV_ENC0",
    "DEV_ENC1",
    "DEV_CAP0",
    "DEV_CAP1",
    "DEV_CAP2",
    "DEV_CAP3",
    "DEV_CAP4",
    "DEV_CAP5",
    "DEV_REN0",
    "DEV_REN1",
    "DEV_REN2",
    "DEV_REN3",
    "DEV_REN4",
    "DEV_REN5",
    "DEV_REN6",
    "DEV_REN7",
    "DEV_REN8",
    "DEV_REN9",
    "DEV_RE10",
    "DEV_RE11",
    "DEV_MAS0",
};


//need to sync with device num ,extra manager module
UINT8 * strDevType[LX_AUD_DEV_TYPE_MAX] =  // device node(D) and module(M)
{
	"DEV_TYPE_NONE",
	"DEV_TYPE_DEC",
	"DEV_TYPE_ENC",
	"DEV_TYPE_CAP",
	"DEV_TYPE_REN",
	"DEV_TYPE_MAS",//4
};

//need to sync with device num ,extra manager module
UINT32 devAvailableNum[LX_AUD_DEV_TYPE_MAX]=
{
	DEV_NONE_NUM,
	DEV_DEC_NUM,
	DEV_ENC_NUM,
	DEV_CAP_NUM,
	DEV_REN_NUM,
	DEV_MAS_NUM,
};

#define NUM_SOURCE_MODULE_MAX	7
LX_AUD_INPUT_T gSourceModuleInput[NUM_SOURCE_MODULE_MAX] = {
	LX_AUD_INPUT_NONE,
	LX_AUD_INPUT_NONE,
	LX_AUD_INPUT_NONE,
	LX_AUD_INPUT_NONE,
	LX_AUD_INPUT_NONE,
	LX_AUD_INPUT_NONE,
	LX_AUD_INPUT_NONE,
};


UINT32	_gModTypeAllocNum[ADEC_MOD_TYPE_MAX];
UINT32	_gModTypeNum[ADEC_MOD_TYPE_MAX];
UINT32	_gModTypeBase[ADEC_MOD_TYPE_MAX];

AUD_DEV_MAN_INFO_T		_gDevManInfo;
AUD_MOD_INFO_T			_gModInfo[ADEC_MODULE_MAX];
AUD_PATH_INFO_T			_gInputModPath[AUD_INPUT_MODULE_PATH_MAX];

#if 0
OS_SEM_T				_gSemaDevMng;
OS_SEM_T				_gSemaModMng;

#define AUD_DEV_LOCK_INIT()		OS_InitMutex(&_gSemaDevMng, OS_SEM_ATTR_DEFAULT)
#define AUD_DEV_LOCK()			OS_LockMutex(&_gSemaDevMng)
#define AUD_DEV_UNLOCK()			OS_UnlockMutex(&_gSemaDevMng)

#define AUD_MOD_LOCK_INIT()		OS_InitMutex(&_gSemaModMng, OS_SEM_ATTR_DEFAULT)
#define AUD_MOD_LOCK()			OS_LockMutex(&_gSemaModMng)
#define AUD_MOD_UNLOCK()			OS_UnlockMutex(&_gSemaModMng)
#else
spinlock_t	_gSemaDevMng;		// device manager spin_lock
spinlock_t	_gSemaModMng;		// module manager spin_lock

#define AUD_DEV_LOCK_INIT()		spin_lock_init(&_gSemaDevMng)
#define AUD_DEV_LOCK()			spin_lock(&_gSemaDevMng)
#define AUD_DEV_UNLOCK()		spin_unlock(&_gSemaDevMng)

#define AUD_MOD_LOCK_INIT()		spin_lock_init(&_gSemaModMng)
#define AUD_MOD_LOCK()			spin_lock(&_gSemaModMng)
#define AUD_MOD_UNLOCK()		spin_unlock(&_gSemaModMng)
#endif
/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void  AUDIO_ModInfoInit(AUD_MOD_INFO_T *pModInfo);
SINT32 AUDIO_IMC_Connect(const void   *_self,
                               ADEC_MODULE_ID  _input_module,
                               unsigned int    _input_port,
                               ADEC_MODULE_ID  _output_module,
                               unsigned int    _output_port);
SINT32 AUDIO_IMC_Disconnect(const void   *_self,
	                          ADEC_MODULE_ID   _disconnect,
	                          unsigned int     _port);
SINT32 	AUDIO_IMC_AllocBuf(const void   *_self,
	                        ADEC_MODULE_ID _module,
	                        unsigned int   _port,
	                        unsigned int   _physical_addr,
	                        unsigned int   _register_addr_offset,
	                        unsigned int   _is_reader);
SINT32 AUDIO_IMC_DeallocBuf(const void   *_self,
	                           ADEC_MODULE_ID  _module,
	                           unsigned int    _port);


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/


/**
 * Alloc Device (Decorder,Render,Master)
 *
 * @param 	devType 	[in] LX_AUD_DEV_TYPE_T
 * @return 	LX_AUD_DEV_T
 * @see		.
 */
LX_AUD_DEV_T  AUDIO_AllocDev(LX_AUD_DEV_TYPE_T devType)
{
	AUD_DEV_INFO_T		*pDevInfo = NULL;
	LX_AUD_DEV_T		devToAlloc = LX_AUD_DEV_NONE;
	UINT32				searchStart, searchEnd;
	UINT32				availableNum, allocBase, allocCnt;

	AUD_DEV_LOCK();

	if(devType == LX_AUD_DEV_TYPE_NONE || devType >= LX_AUD_DEV_TYPE_MAX)
	{
		AUD_KDRV_ERROR("devType[%d] is invalid.\n", devType);
		AUD_DEV_UNLOCK();
		return LX_AUD_DEV_NONE;
	}

	allocCnt 		= _gDevManInfo.allocCnt[devType];
	availableNum 	= _gDevManInfo.availableNum[devType];
	allocBase 		= _gDevManInfo.allocBase[devType];

	if(allocCnt >= availableNum)
	{
		AUD_KDRV_ERROR("allocCnt[%d] is greater than availableNum[%d].\n", allocCnt, availableNum);
		AUD_DEV_UNLOCK();
		return LX_AUD_DEV_NONE;
	}

	searchStart = allocBase;
	searchEnd = searchStart + availableNum;

	for(devToAlloc = searchStart; devToAlloc < searchEnd; devToAlloc++)
	{
		pDevInfo = &_gDevManInfo.devInfo[devToAlloc];
		if(pDevInfo->used == FALSE)
		{
			pDevInfo->dev			= devToAlloc;
			pDevInfo->used			= 1;
			pDevInfo->devType 		= devType;
			_gDevManInfo.allocCnt[devType]++;
			break;
		}
	}

	if(devToAlloc == searchEnd)
	{
		AUD_KDRV_ERROR("devToAlloc(%d) failed.\n", devType);
		AUD_DEV_UNLOCK();
		return LX_AUD_DEV_NONE;
	}

	AUD_DEV_UNLOCK();

	AUD_KDRV_RM("%s is allocated.\n", strDevName[devToAlloc]);

	return devToAlloc;
}

/**
 * Free Device (Decorder,Render,Master)
 *
 * @param 	dev 			[in] LX_AUD_DEV_T
 * @return 	void
 * @see		.
 */
void  AUDIO_FreeDev(LX_AUD_DEV_T dev)
{
	LX_AUD_DEV_TYPE_T	devType;
	AUD_DEV_INFO_T		*pDevInfo = NULL;
	AUD_MOD_INFO_T		*pModInfo = NULL;
	SINT32	retVal = RET_OK;
	SINT32 	i;

	AUD_DEV_LOCK();

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		AUD_DEV_UNLOCK();
		return;
	}

	pDevInfo = AUDIO_GetDevInfo(dev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_RM("pDevInfo[%d] is NULL.\n", dev);
		AUD_DEV_UNLOCK();
		return;
	}

	devType = pDevInfo->devType;
	if(devType == LX_AUD_DEV_TYPE_NONE || devType >= LX_AUD_DEV_TYPE_MAX)
	{
		AUD_KDRV_RM("devType[%d] is invalid.\n", devType);
		AUD_DEV_UNLOCK();
		return;
	}

	if(_gDevManInfo.allocCnt[devType] == 0)
	{
		AUD_KDRV_RM("devType[%d] is already freed.\n", devType);
		AUD_DEV_UNLOCK();
		return;
	}

	pDevInfo->used		 		= 0;
	pDevInfo->devType  			= LX_AUD_DEV_TYPE_NONE;
	pDevInfo->dev				= LX_AUD_DEV_NONE;

	for(i = (pDevInfo->numMod - 1); i >= 0; i--)
	{
		pModInfo = pDevInfo->pModInfo[i];
		if(pModInfo == NULL)
		{
			continue;
		}

		retVal = AUDIO_RemoveModule(dev, pModInfo->mod);
		if(retVal < RET_OK)
		{
			AUD_KDRV_RM("AUDIO_RemoveModule(%d, %d) failed(%d)!!!\n", dev, pModInfo->mod, retVal);
			continue;
		}

		AUDIO_FreeModule(pModInfo->mod);
	}

	pDevInfo->index = DEFAULT_DEV_INDEX;
	pDevInfo->pCtx = NULL;

	pDevInfo->pHeadMod = NULL;
	pDevInfo->pTailMod = NULL;

	_gDevManInfo.allocCnt[devType]--;

	AUD_DEV_UNLOCK();

	AUD_KDRV_RM("%s is freed.\n", strDevName[dev]);

	return;
}

/**
 * Alloc Module ( Module in Firmware, refer to adec_module_def.h )
 *
 * @param 	modType 		[in] module type to allocate
 * @param 	coreType 		[in] core type to allocate
 * @param 	mod 		[in] module to allocate
 * @return 	ADEC_MODULE_ID
 * @see
 */
ADEC_MODULE_ID AUDIO_AllocModule(ADEC_MODULE_TYPE modType, ADEC_CORE_TYPE coreType, ADEC_MODULE_ID mod)
{
	UINT32				i;
	AUD_MOD_INFO_T		*pModInfo;

	AUD_MOD_LOCK();

	if(mod == ADEC_MODULE_NOT_DEF)
	{
		if(modType == ADEC_MOD_TYPE_NOT_DEF || modType >= ADEC_MOD_TYPE_MAX)
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("modType[%d] is invalid.\n", modType);
			return ADEC_MODULE_NOT_DEF;
		}

		if(coreType == ADEC_CORE_NOT_DEF || coreType >= ADEC_CORE_MAX)
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("coreType[%d] is invalid.\n", coreType);
			return ADEC_MODULE_NOT_DEF;
		}

		// alloc source modules & add path
		for(i = 1; i < ADEC_MODULE_MAX; i++)
		{
			pModInfo = AUDIO_GetModuleInfo(i);
			if(pModInfo == NULL)
			{
				continue;
			}

			if(pModInfo->modType == modType && pModInfo->coreType == coreType)
			{
				if(modType != ADEC_MOD_TYPE_SOURCE)
				{
					if(pModInfo->allocated == FALSE)
						break;
				}
				else
				{
					break;
				}
			}
		}

		if(i == ADEC_MODULE_MAX)
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("mt(%d),ct(%d) Can't find.\n", modType, coreType);
			return ADEC_MODULE_NOT_DEF;
		}

		mod = i;
	}

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_MOD_UNLOCK();
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return ADEC_MODULE_NOT_DEF;
	}

	pModInfo = AUDIO_GetModuleInfo(mod);

	if(pModInfo->allocated == TRUE && pModInfo->modType != ADEC_MOD_TYPE_SOURCE)
	{
		AUD_MOD_UNLOCK();
		AUD_KDRV_RM("mod[%d] is already allocated.\n", mod);
		return ADEC_MODULE_NOT_DEF;
	}

	pModInfo->allocated = TRUE;

	_gModTypeAllocNum[modType]++;

	AUD_MOD_UNLOCK();

	AUD_KDRV_RM("%s is allocated.\n", ModuleList_GetModuleName(mod));

	return mod;
}

/**
 * Free Module ( Module in Firmware, refer to adec_module_def.h )
 *
 * @param 	dev			[in] device of module to free
 * @param 	mod			[in] module to free
 * @return 	if succeeded -freed device num , else - ADEC_MODULE_NOT_DEF.
 * @see
 */
void AUDIO_FreeModule(ADEC_MODULE_ID mod)
{
	AUD_MOD_INFO_T 		*pModInfo;
	ADEC_MODULE_TYPE	modType;
	ModuleInfo			modInfo;

	SINT32				retVal = RET_OK;

	AUD_MOD_LOCK();

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_MOD_UNLOCK();
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return;
	}

	retVal = ModuleList_GetModuleInfo(mod, &modInfo);
	if(retVal != ADEC_ERR_NONE)
	{
		AUD_MOD_UNLOCK();
		AUD_KDRV_ERROR("ModuleList_GetModuleInfo(%d) failed.\n", mod);
		return;
	}

	modType = modInfo.type;
	pModInfo = &_gModInfo[mod];
	if(modType != ADEC_MOD_TYPE_SOURCE)
	{
		// Initialize module info.
		AUDIO_ModInfoInit(pModInfo);
	}
	else// if(modType == ADEC_MOD_TYPE_SOURCE)
	{
		if(mod >= ADEC_MODULE_TP_IN_0 && mod <= ADEC_MODULE_I2S_IN)
		{
			if(pModInfo->countOutConnection == 0)
			{
					AUDIO_ModInfoInit(pModInfo);
					gSourceModuleInput[mod - ADEC_MODULE_TP_IN_0] = LX_AUD_INPUT_NONE;
			}
		}
		else
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("mod(%d)'s type is not SOURCE.\n", mod);
			return;
		}
	}

	_gModTypeAllocNum[modType]--;

	AUD_MOD_UNLOCK();

	AUD_KDRV_RM("%s is freed.\n", ModuleList_GetModuleName(mod));

	return;
}

/**
 * Allocate Port
 *
 * @param 	pModInfo		[in] module info
 * @param 	bInOut			[in] is input or output
 * @param 	index		 	[in] index of output ports
 * @see
 */
SINT32 AUDIO_AllocPort(AUD_MOD_INFO_T *pModInfo, SINT32 port)
{
	UINT32	i = 0;

	AUD_MOD_LOCK();

	if((port & OUT_PORT) == OUT_PORT)
	{
		for(i = (port & 0x1F) ; i < pModInfo->numOutPort; i++)
		{
			if(pModInfo->outPort[i].mod == ADEC_MODULE_NOT_DEF)
			{
				break;
			}
		}

		if(i >= pModInfo->numOutPort)
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("OutPort[%x] is not allocated in mod[%d].\n", i, pModInfo->mod);
			return RET_ERROR;
		}

		port = OUT_PORT + i;

		AUD_KDRV_RM("OutPort[%x] is allocated in %s.\n", \
			port, ModuleList_GetModuleName(pModInfo->mod));
	}
	else// if ((port & OUT_PORT) == IN_PORT)
	{
		if(pModInfo->numInPort != 0)
		{
			for(i = port; i < pModInfo->numInPort; i++)
			{
				if(pModInfo->inPort[i].mod == ADEC_MODULE_NOT_DEF)
				{
					break;
				}
			}

			if(i >= pModInfo->numInPort)
			{
				AUD_MOD_UNLOCK();
				AUD_KDRV_ERROR("InPort[%x] is not allocated in mod[%d].\n", i, pModInfo->mod);
				return RET_ERROR;
			}
		}

		port = i;

		AUD_KDRV_RM("InPort[%x] is allocated in %s.\n", \
			port, ModuleList_GetModuleName(pModInfo->mod));
	}

	AUD_MOD_UNLOCK();

	return port;
}

/**
 * Free Port
 *
 * @param 	pModInfo		[in] module info
 * @param 	bInOut			[in] is input or output
 * @param 	index		 	[in] index of output ports
 * @see
 */
void AUDIO_FreePort(AUD_MOD_INFO_T *pModInfo, SINT32 port)
{
	SINT32		freePort;

	AUD_MOD_LOCK();

	if(pModInfo == NULL)
	{
		AUD_MOD_UNLOCK();
		AUD_KDRV_ERROR("pModInfo is invalid.\n");
		return;
	}

	freePort = port & 0x1F;

	if((port & OUT_PORT) == OUT_PORT)
	{
		if(freePort > pModInfo->numOutPort)
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("OutPort[%x] is not valid.\n", port);
			return;
		}

		pModInfo->outPort[freePort].mod = ADEC_MODULE_NOT_DEF;
		pModInfo->outPort[freePort].pBufMemCfg = NULL;
		pModInfo->countOutConnection--;

		AUD_KDRV_RM("OutPort[%x] is freed in %s.\n", \
			port, ModuleList_GetModuleName(pModInfo->mod));
	}
	else// if ((port & OUT_PORT) == IN_PORT)
	{
		if(freePort > pModInfo->numInPort)
		{
			AUD_MOD_UNLOCK();
			AUD_KDRV_ERROR("InPort[%x] is not valid.\n", port);
			return;
		}

		pModInfo->inPort[freePort].mod = ADEC_MODULE_NOT_DEF;
		if(pModInfo->inPort[freePort].pBufMemCfg != NULL)
		{
			AUDIO_FreeBufferCfg(pModInfo->inPort[freePort].pBufMemCfg);
		}
		pModInfo->inPort[freePort].pBufMemCfg = NULL;
		pModInfo->countInConnection--;

		AUD_KDRV_RM("InPort[%x] is freed in %s.\n", \
			port, ModuleList_GetModuleName(pModInfo->mod));
	}

	AUD_MOD_UNLOCK();

	return;
}

/**
 * Get Port
 *
 * @param 	pModInfo		[in] module info
 * @param 	bInOut			[in] is input or output
 * @param 	connectedMod 	[in] connectedMod
 * @see
 */
SINT32 AUDIO_GetPort(ADEC_MODULE_ID mod, UINT32 inOut, ADEC_MODULE_ID connectedMod)
{
	UINT32			i = 0;
	UINT32			getPort = 0;
	AUD_MOD_INFO_T	*pModInfo = NULL;

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return RET_ERROR;
	}

	pModInfo = &_gModInfo[mod];

	if(inOut == OUT_PORT)
	{
		for(i = 0; i < pModInfo->numOutPort; i++)
		{
			if(pModInfo->outPort[i].mod == connectedMod)
			{
				break;
			}
		}

		if(i == pModInfo->numOutPort)
		{
			AUD_KDRV_RM("connectedMod[%d] is not found in outPorts of %s.\n", \
				connectedMod, ModuleList_GetModuleName(mod));
			return RET_ERROR;
		}

		getPort = OUT_PORT + i;

		AUD_KDRV_RM("connectedMod[%s] is found in outPort[%x] of %s.\n", \
			ModuleList_GetModuleName(connectedMod), getPort, ModuleList_GetModuleName(mod));
	}
	else// if (inOut == IN_PORT)
	{
		if(pModInfo->numInPort != 0)
		{
			for(i = 0; i< pModInfo->numInPort; i++)
			{
				if(pModInfo->inPort[i].mod == connectedMod)
				{
					break;
				}
			}

			if(i == pModInfo->numInPort)
			{
				AUD_KDRV_RM("connectedMod[%d] is not found in inPorts of %s.\n", \
					connectedMod, ModuleList_GetModuleName(mod));
				return RET_ERROR;
			}
		}

		getPort = i;

		AUD_KDRV_RM("connectedMod[%s] is found in inPort[%x] of %s.\n", \
			ModuleList_GetModuleName(connectedMod), getPort, ModuleList_GetModuleName(mod));
	}

	return getPort;
}

/**
 * Add Module
 *
 * @param 	dev		 	[in] device
 * @param 	mod 		[in] module to insert
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_AddModule(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod)
{
	SINT32				retVal = RET_OK;
	UINT32				numMod = 0;
	AUD_DEV_INFO_T 		*pDevInfo = NULL;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return RET_INVALID_PARAMS;
	}

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return RET_INVALID_PARAMS;
	}

	pDevInfo = AUDIO_GetDevInfo(dev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_ERROR("pDevInfo(%d) is NULL!!!\n", dev);
		return RET_ERROR;
	}

	numMod = pDevInfo->numMod;
	pDevInfo->pModInfo[numMod] = &_gModInfo[mod];
	pDevInfo->numMod++;
	_gModInfo[mod].dev = pDevInfo->dev;

	AUD_KDRV_RMD("%s is added to %s.\n",ModuleList_GetModuleName(mod), strDevName[dev]);

	return retVal;
}

/**
 * Insert Module Last
 *
 * @param 	dev		 	[in] device
 * @param 	mod 		[in] module to insert
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_InsertModuleLast(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod, AUD_BUFFER_TYPE_T bufType)
{
	SINT32				retVal = RET_OK;
	AUD_DEV_INFO_T 		*pDevInfo = NULL;
	ADEC_MODULE_ID		outMod;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return RET_INVALID_PARAMS;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return RET_INVALID_PARAMS;
	}

	outMod = AUDIO_FindTailModule(dev);
	if(outMod != ADEC_MODULE_NOT_DEF)
	{
		retVal = AUDIO_ConnectModules(outMod, mod, bufType);
		if(retVal != 0)
		{
			AUD_KDRV_ERROR("AUDIO_ConnectModules(%d.%d) failed(%d)!!!\n", outMod, mod, retVal);
			return RET_ERROR;
		}
	}

	retVal = AUDIO_AddModule(dev, mod);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("AUDIO_AddModule(%d) failed(%d)!!!\n", mod, retVal);
		return RET_ERROR;
	}

	pDevInfo->pTailMod = AUDIO_GetModuleInfo(mod);
	if(pDevInfo->pHeadMod == NULL)
	{
		pDevInfo->pHeadMod = pDevInfo->pTailMod;
	}
	pDevInfo->pTailMod->dev = pDevInfo->dev;

	AUD_KDRV_RMD("%s is inserted to %s.\n", ModuleList_GetModuleName(mod), strDevName[dev]);

	return retVal;
}

/**
 * Insert Module First
 *
 * @param 	dev		 	[in] device
 * @param 	mod 		[in] module to insert
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_InsertModuleFirst(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod, AUD_BUFFER_TYPE_T bufType)
{
	SINT32				retVal = RET_OK;
	AUD_DEV_INFO_T 		*pDevInfo;
	ADEC_MODULE_ID		inMod;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return RET_INVALID_PARAMS;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return RET_INVALID_PARAMS;
	}

	inMod = AUDIO_FindHeadModule(dev);
	if(inMod != ADEC_MODULE_NOT_DEF)
	{
		retVal = AUDIO_ConnectModules(mod, inMod, bufType);
		if(retVal != 0)
		{
			AUD_KDRV_ERROR("AUDIO_ConnectModules(%d.%d) failed(%d)!!!\n", mod, inMod, retVal);
			return RET_ERROR;
		}
	}

	retVal = AUDIO_AddModule(dev, mod);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("AUDIO_AddModule(%d) failed(%d)!!!\n", mod, retVal);
		return RET_ERROR;
	}

	pDevInfo->pHeadMod = AUDIO_GetModuleInfo(mod);
	if(pDevInfo->pTailMod == NULL)
	{
		pDevInfo->pTailMod = pDevInfo->pHeadMod;
	}
	pDevInfo->pTailMod->dev = pDevInfo->dev;

	AUD_KDRV_RMD("%s is inserted to first of %s.\n", ModuleList_GetModuleName(mod), strDevName[dev]);

	return retVal;
}

/**
 * Insert Module First
 *
 * @param 	dev		 	[in] device
 * @param 	mod 		[in] module to insert
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_RemoveModule(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod)
{
	UINT32				numMod = 0;
	SINT32				i = 0, j = 0;
	SINT32				retVal = RET_OK;
	AUD_DEV_INFO_T 		*pDevInfo = NULL;
	AUD_MOD_INFO_T		*pModInfo = NULL;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return RET_INVALID_PARAMS;
	}

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return RET_INVALID_PARAMS;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	if(_gModInfo[mod].modType == ADEC_MOD_TYPE_EXT_BUF)
	{
		AUDIO_FreeBuffer(mod);
	}

	numMod = pDevInfo->numMod;

	if(numMod == 0)
	{
		AUD_KDRV_RMD("mod(%d) is not found!!!\n", mod);
		return RET_OK;
	}

	// find module
	for(i = 0; i < numMod; i++)
	{
		pModInfo = pDevInfo->pModInfo[i];
		if(pModInfo == NULL)
		{
			AUD_KDRV_RMD("pModInfo is NULL.\n");
			continue;
		}

		if(pModInfo->mod == mod)
		{
			break;
		}
	}

	if(i == numMod)
	{
		AUD_KDRV_RMD("mod(%d) is not found!!!\n", mod);
		return RET_OK;
	}

	// sort modules
	for(j = i; j < numMod - 1; j++)
	{
		pDevInfo->pModInfo[j] = pDevInfo->pModInfo[j+1];
	}

	pDevInfo->pModInfo[numMod-1] = NULL;
	pDevInfo->numMod--;

	AUD_KDRV_RMD("%s is removed in %s.\n", ModuleList_GetModuleName(mod), strDevName[dev]);

	return retVal;
}

/**
 * Connect devices
 *
 * @param 	outDev	 	[in] output device
 * @param 	inDev 		[in] input device
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_ConnectDevices(LX_AUD_DEV_T outDev, LX_AUD_DEV_T inDev, AUD_BUFFER_TYPE_T bufType)
{
	SINT32				retVal = RET_OK;
	ADEC_MODULE_ID		outMod, inMod;

	if(outDev == LX_AUD_DEV_NONE || outDev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("outDev[%d] is invalid.\n", outDev);
		return RET_INVALID_PARAMS;
	}

	if(inDev == LX_AUD_DEV_NONE || inDev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("inDev[%d] is invalid.\n", inDev);
		return RET_INVALID_PARAMS;
	}

	if(bufType  >= AUD_BUFFER_TYPE_MAX)
	{
		AUD_KDRV_ERROR("bufType[%d] is invalid.\n", bufType);
		return RET_INVALID_PARAMS;
	}

	outMod = AUDIO_FindTailModule(outDev);
	inMod = AUDIO_FindHeadModule(inDev);

	retVal = AUDIO_ConnectModules(outMod, inMod, bufType);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("AUDIO_ConnectModules(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	AUD_KDRV_RMD("outDev[%s] is connected to inDev[%s].\n", strDevName[outDev], strDevName[inDev]);

	return retVal;
}

/**
 * Disconnect devices
 *
 * @param 	outDev	 	[in] output device
 * @param 	inDev 		[in] input device
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_DisconnectDevices(LX_AUD_DEV_T outDev, LX_AUD_DEV_T inDev)
{
	SINT32				retVal = RET_OK;
	ADEC_MODULE_ID		outMod, inMod;

	if(outDev == LX_AUD_DEV_NONE || outDev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("outDev[%d] is invalid.\n", outDev);
		return RET_INVALID_PARAMS;
	}

	if(inDev == LX_AUD_DEV_NONE || inDev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("inDev[%d] is invalid.\n", inDev);
		return RET_INVALID_PARAMS;
	}

	outMod = AUDIO_FindTailModule(outDev);
	inMod = AUDIO_FindHeadModule(inDev);

	retVal = AUDIO_DisconnectModules(outMod, inMod);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("AUDIO_DisconnectModules(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	AUD_KDRV_RMD("outDev[%s] is disconnected to inDev[%s].\n", strDevName[outDev], strDevName[inDev]);

	return retVal;
}

/**
 * Connect Modules
 *
 * @param 	outMod	 	[in] output module
 * @param 	inMod 		[in] input module
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_ConnectModules(ADEC_MODULE_ID outMod, ADEC_MODULE_ID inMod, AUD_BUFFER_TYPE_T bufType)
{
	SINT32				retVal = RET_OK;
	UINT32				inPort = 0, outPort = 0;
	ADEC_CORE_TYPE		inCoreType = ADEC_CORE_NOT_DEF, outCoreType = ADEC_CORE_NOT_DEF;
	ModuleInfo			outModInfo, inModInfo;
	LX_AUD_BUF_MEM_CFG_S_T	*pBufCfg = NULL;

	if(outMod == ADEC_MODULE_NOT_DEF || outMod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("outMod[%d] is invalid.\n", outMod);
		return RET_INVALID_PARAMS;
	}

	if(inMod == ADEC_MODULE_NOT_DEF || inMod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("inMod[%d] is invalid.\n", inMod);
		return RET_INVALID_PARAMS;
	}

	// Get Connecting Port
	outCoreType = ModuleList_FindModule(outMod);
	inCoreType = ModuleList_FindModule(inMod);

	// allocate output port
	retVal = ModuleList_GetModuleInfo(outMod, &outModInfo);
	if(retVal == ADEC_ERR_RANGE)
	{
		AUD_KDRV_ERROR("ModuleList_GetModuleInfo(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	if(outModInfo.num_out == 0)
	{
		AUD_KDRV_ERROR("outMod(%d) has no port!!!\n", outMod);
		return RET_ERROR;
	}

	if(outModInfo.out_buffer_type[0] == ADEC_BUF_MULTI_SINK &&
		outModInfo.out_buffer_type[1] == ADEC_BUF_INTRA &&
		outCoreType != inCoreType &&
		bufType != AUD_BUFFER_TYPE_NONE)
	{
		outPort = MOD_OUT_PORT(1);
	}
	else
	{
		outPort = MOD_OUT_PORT(0);
	}

	outPort = AUDIO_AllocPort(&_gModInfo[outMod], outPort);
	if(outPort == RET_ERROR)
	{
		AUD_KDRV_ERROR("AUDIO_AllocPort(%x) failed!!!\n", outPort);
		return RET_ERROR;
	}

	// allocate input port
	retVal = ModuleList_GetModuleInfo(inMod, &inModInfo);
	if(retVal == ADEC_ERR_RANGE)
	{
		AUD_KDRV_ERROR("ModuleList_GetModuleInfo(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	if(inModInfo.num_in == 0)
	{
		AUD_KDRV_ERROR("inMod(%d) has no port!!!\n", inMod);
		return RET_ERROR;
	}

	inPort = AUDIO_AllocPort(&_gModInfo[inMod], IN_PORT);
	if(inPort == RET_ERROR)
	{
		AUD_KDRV_ERROR("AUDIO_AllocPort(%d) failed!!!\n", inMod);
		return RET_ERROR;
	}

	// connect modules
	if(outCoreType == inCoreType)
	{
		retVal = AUDIO_IMC_Connect(NULL, inMod, inPort, outMod, outPort);
		if(retVal != 0)
		{
			AUD_KDRV_ERROR("AUDIO_IMC_Connect(in:%d.0x%x out:%d.0x%x) failed(%d)!!!\n", \
				inMod, inPort, outMod, outPort, retVal);
			return RET_ERROR;
		}
	}
	else
	{
		ADEC_BUF_T				*pWriterStruct = NULL;

		pBufCfg = AUDIO_AllocBufferCfg(bufType);
		if(pBufCfg == NULL)
		{
			AUD_KDRV_ERROR("AUDIO_AllocBufferCfg(bufType:%d) failed!!!\n", bufType);
			return RET_ERROR;
		}

		pWriterStruct = (ADEC_BUF_T*)(pBufCfg->pWriterReaderStruct);
		if(pWriterStruct == NULL)
		{
			AUD_KDRV_ERROR("pWriterStruct(bufType:%d) is NULL!!!\n", bufType);
			return RET_ERROR;
		}

		// initialize buffer
		pWriterStruct->init(pWriterStruct);

		// connect input module
		AUDIO_IMC_AllocBuf(NULL, inMod, inPort, pBufCfg->MemPhyBase, (UINT32)(pBufCfg->pRegBase - (void *)g_AudKdrv.p_aud_mem_share_base), 1);
		// connect output module
		AUDIO_IMC_AllocBuf(NULL, outMod, outPort, pBufCfg->MemPhyBase, (UINT32)(pBufCfg->pRegBase - (void *)g_AudKdrv.p_aud_mem_share_base), 0);
	}

	// Update Module Info
	_gModInfo[inMod].inPort[inPort].mod = outMod;
	_gModInfo[inMod].inPort[inPort].pBufMemCfg = pBufCfg;
	_gModInfo[inMod].countInConnection++;
	_gModInfo[outMod].outPort[(outPort & 0x1F)].mod = inMod;
	_gModInfo[outMod].outPort[(outPort & 0x1F)].pBufMemCfg = pBufCfg;
	_gModInfo[outMod].countOutConnection++;

	AUD_KDRV_RMD("outMod[%s.%x] is connected to inMod[%s.%x].\n", \
		ModuleList_GetModuleName(outMod), outPort, ModuleList_GetModuleName(inMod), inPort);

	return retVal;
}

/**
 * Connect Module's port
 *
 * @param 	outMod	 	[in] output module
 * @param 	inMod 		[in] input module
 * @param 	inPort 		[in] input module;s port
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_ConnectModulePort(ADEC_MODULE_ID outMod, ADEC_MODULE_ID inMod, SINT32 inPort)
{
	SINT32				retVal = RET_OK;
	UINT32				outPort = MOD_OUT_PORT(0);
	ADEC_CORE_TYPE		inCoreType = ADEC_CORE_NOT_DEF, outCoreType = ADEC_CORE_NOT_DEF;
	ModuleInfo			outModInfo, inModInfo;
	LX_AUD_BUF_MEM_CFG_S_T	*pBufCfg = NULL;

	if(outMod == ADEC_MODULE_NOT_DEF || outMod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("outMod[%d] is invalid.\n", outMod);
		return RET_INVALID_PARAMS;
	}

	if(inMod == ADEC_MODULE_NOT_DEF || inMod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("inMod[%d] is invalid.\n", inMod);
		return RET_INVALID_PARAMS;
	}

	// Get Connecting Port
	outCoreType = ModuleList_FindModule(outMod);
	inCoreType = ModuleList_FindModule(inMod);

	// allocate output port
	retVal = ModuleList_GetModuleInfo(outMod, &outModInfo);
	if(retVal == ADEC_ERR_RANGE)
	{
		AUD_KDRV_ERROR("ModuleList_GetModuleInfo(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	if(outModInfo.num_out == 0)
	{
		AUD_KDRV_ERROR("outMod(%d) has no port!!!\n", outMod);
		return RET_ERROR;
	}

	if(outCoreType != inCoreType)
	{
		AUD_KDRV_ERROR("outCoreType !!!\n");
		return RET_ERROR;
	}

	outPort = AUDIO_AllocPort(&_gModInfo[outMod], outPort);
	if(outPort == RET_ERROR)
	{
		AUD_KDRV_ERROR("AUDIO_AllocPort(%x) failed!!!\n", outPort);
		return RET_ERROR;
	}

	// allocate input port
	retVal = ModuleList_GetModuleInfo(inMod, &inModInfo);
	if(retVal == ADEC_ERR_RANGE)
	{
		AUD_KDRV_ERROR("ModuleList_GetModuleInfo(%d) failed!!!\n", retVal);
		return RET_ERROR;
	}

	if(inModInfo.num_in == 0)
	{
		AUD_KDRV_ERROR("inMod(%d) have no port!!!\n", inMod);
		return RET_ERROR;
	}

	if(inModInfo.num_in < inPort)
	{
		AUD_KDRV_ERROR("inPort(%d) is greater than num_in(%d)!!!\n", inPort, inModInfo.num_in);
		return RET_ERROR;
	}

	// connect modules
	retVal = AUDIO_IMC_Connect(NULL, inMod, inPort, outMod, outPort);
	if(retVal != 0)
	{
		AUD_KDRV_ERROR("AUDIO_Connect(in:%d.0x%x out:%d.0x%x) is failed(%d)!!!\n", \
			inMod, inPort, outMod, outPort, retVal);
		return RET_ERROR;
	}

	// Update Module Info
	_gModInfo[inMod].inPort[inPort].mod = outMod;
	_gModInfo[inMod].inPort[inPort].pBufMemCfg = pBufCfg;
	_gModInfo[inMod].countInConnection++;
	_gModInfo[outMod].outPort[(outPort & 0x1F)].mod = inMod;
	_gModInfo[outMod].outPort[(outPort & 0x1F)].pBufMemCfg = pBufCfg;
	_gModInfo[outMod].countOutConnection++;

	AUD_KDRV_RMD("outMod[%s.%x] is connected to inMod[%s.%x].\n", \
		ModuleList_GetModuleName(outMod), outPort, ModuleList_GetModuleName(inMod), inPort);

	return retVal;
}

/**
 * Disconnect Modules
 *
 * @param 	outMod	 	[in] output module
 * @param 	inMod 		[in] input module
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_DisconnectModules(ADEC_MODULE_ID outMod, ADEC_MODULE_ID inMod)
{
	SINT32				retVal = RET_OK;
	SINT32				inPort = 0, outPort = 0;
	AUD_MOD_INFO_T 		*pModInInfo = NULL, *pModOutInfo = NULL;
	ADEC_CORE_TYPE		inCoreType = ADEC_CORE_NOT_DEF, outCoreType = ADEC_CORE_NOT_DEF;

	if(outMod == ADEC_MODULE_NOT_DEF || outMod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("outMod[%d] is invalid.\n", outMod);
		return RET_INVALID_PARAMS;
	}

	if(inMod == ADEC_MODULE_NOT_DEF || inMod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("inMod[%d] is invalid.\n", inMod);
		return RET_INVALID_PARAMS;
	}

	// Get Connecting Port
	pModInInfo = &_gModInfo[inMod];
	inPort = AUDIO_GetPort(inMod, IN_PORT, outMod);
	if(inPort >= 0)
	{
		// Get Discconnecting input Port
		outCoreType = ModuleList_FindModule(outMod);
		inCoreType = ModuleList_FindModule(inMod);

		if(outCoreType == inCoreType)
		{
			AUDIO_IMC_Disconnect(NULL, inMod, inPort);
		}
		else
		{
			AUDIO_IMC_DeallocBuf(NULL, inMod, inPort);
		}

		AUDIO_FreePort(pModInInfo, inPort);
	}

	pModOutInfo = &_gModInfo[outMod];
	outPort = AUDIO_GetPort(outMod, OUT_PORT, inMod);
	if(outPort >= 0)
	{
		// Get Discconnecting output Port
#if 0	// Error
		pOutModCtx = RscMan_GetModule(outMod);
		retVal = Connection_Disconnect(pOutModCtx, outPort);
		if(retVal < 0)
		{
			AUD_KDRV_ERROR("Connection_Disconnect(%d) is failed!!!\n", retVal);
			return RET_ERROR;
		}
#endif
		if(outCoreType != inCoreType)
		{
			AUDIO_IMC_DeallocBuf(NULL, outMod, outPort);
		}

		AUDIO_FreePort(pModOutInfo, outPort);
	}

	AUD_KDRV_RMD("outMod[%s.%x] is disconnected to inMod[%s.%x].\n", \
		ModuleList_GetModuleName(outMod), outPort, ModuleList_GetModuleName(inMod), inPort);

	return retVal;
}

/**
 * Disconnect All Modules
 *
 * @param 	dev	 		[in] device
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_DisconnectAllModules(LX_AUD_DEV_T dev)
{
	SINT32			i, j;
	SINT32			retVal = RET_OK;
	AUD_DEV_INFO_T	*pDevInfo = NULL;
	AUD_MOD_INFO_T	*pModInfo = NULL;
	AUD_MOD_INFO_T	*pOutModInfo = NULL;
	AUD_MOD_INFO_T	*pInModInfo = NULL;
	ADEC_MODULE_ID	outMod;
	ADEC_MODULE_ID	inMod;

	pDevInfo = AUDIO_GetDevInfo(dev);
	if(pDevInfo == NULL)
	{
		AUD_KDRV_ERROR("pDevInfo is invalid.\n");
		return RET_ERROR;
	}

	for(i = pDevInfo->numMod - 1; i >= 0; i--)
	{
		pModInfo = pDevInfo->pModInfo[i];
		if(pModInfo == NULL)
		{
			continue;
		}

		// SOURCE Module은 Device Driver에서 직접 Disconnection함.
		if(pModInfo->modType != ADEC_MOD_TYPE_SOURCE)
		{
			for(j = 0;j < MAX_IN_PORT; j++)
			{
				outMod = pModInfo->inPort[j].mod;
				if(outMod == ADEC_MODULE_NOT_DEF)
					continue;

				pOutModInfo = AUDIO_GetModuleInfo(outMod);
				if(pOutModInfo == NULL)
					continue;

					retVal = AUDIO_DisconnectModules(pOutModInfo->mod, pModInfo->mod);
					if(retVal != 0)
					{
						AUD_KDRV_ERROR("AUDIO_DisconnectModules(%d) failed(%d)!!!\n", pOutModInfo->mod, retVal);
						return RET_ERROR;
					}
			}

			// if inputctrl, disconnect in AUDIO_DisconnectIndexOutput.
			if(pModInfo->modType != ADEC_MOD_TYPE_INPUTCTRL)
			{
				for(j = 0;j < MAX_OUT_PORT; j++)
				{
					inMod = pModInfo->outPort[j].mod;
					if(inMod == ADEC_MODULE_NOT_DEF)
						continue;

					pInModInfo = AUDIO_GetModuleInfo(inMod);
					if(pInModInfo == NULL)
						continue;

					retVal = AUDIO_DisconnectModules(pModInfo->mod, pInModInfo->mod);
					if(retVal != 0)
					{
						AUD_KDRV_ERROR("AUDIO_DisconnectModules(%d) failed(%d)!!!\n", pInModInfo->mod, retVal);
						return RET_ERROR;
					}
				}
			}
		}
	}

	pDevInfo->pHeadMod = NULL;
	pDevInfo->pTailMod = NULL;

	AUD_KDRV_RMD("AllModules are disconnected in %s.\n", strDevName[dev]);

	return RET_OK;
}

/**
 * Disconnect Input Modules
 *
 * @param 	outMod	 	[in] output module
 * @param 	inMod 		[in] input module
 * @param 	opt		 	[in] buffer option to use
 * @return 	SINT32
 * @see
 */
SINT32 AUDIO_DisconnectInputModules(ADEC_MODULE_ID mod)
{
	SINT32				retVal = RET_OK;
	SINT32				inPort = 0, outPort = 0;
	AUD_MOD_INFO_T 		*pModInfo = NULL;
	ADEC_CORE_TYPE		inCoreType = ADEC_CORE_NOT_DEF, outCoreType = ADEC_CORE_NOT_DEF;
	ADEC_MODULE_ID		inputMod = ADEC_MODULE_NOT_DEF;

	if(mod == ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return RET_INVALID_PARAMS;
	}

	// Get Discconnecting input Port
	pModInfo = &_gModInfo[mod];
	for(inPort = 0; inPort < MAX_IN_PORT; inPort++)
	{
		inputMod = pModInfo->inPort[inPort].mod;
		if(inputMod != ADEC_MODULE_NOT_DEF)
		{
			outCoreType = ModuleList_FindModule(inputMod);
			inCoreType = ModuleList_FindModule(mod);

			if(outCoreType == inCoreType)
			{
				AUDIO_IMC_Disconnect(NULL, mod, inPort);
			}
			else
			{
				AUDIO_IMC_DeallocBuf(NULL, mod, inPort);
			}

			AUDIO_FreePort(pModInfo, inPort);

			AUD_KDRV_RMD("inputMod[%s.%x] is disconnected from Mod[%s.%x].\n", \
				ModuleList_GetModuleName(inputMod), outPort, ModuleList_GetModuleName(mod), inPort);
		}
	}

	return retVal;
}


/**
 * Find Module By Module Type
 *
 * @param 	dev				[in] device of module to find
 * @param 	modType			[in] module to find
 * @return 	ADEC_MODULE_ID.
 * @see
 */
ADEC_MODULE_ID AUDIO_FindModuleType(LX_AUD_DEV_T dev, ADEC_MODULE_TYPE modType)
{
	AUD_DEV_INFO_T 		*pDevInfo = NULL;
	AUD_MOD_INFO_T		*pModInfo = NULL;
	UINT32				i;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return ADEC_MODULE_NOT_DEF;
	}

	if(modType == ADEC_MOD_TYPE_NOT_DEF || modType >= ADEC_MOD_TYPE_MAX)
	{
		AUD_KDRV_ERROR("modType[%d] is invalid.\n", modType);
		return ADEC_MODULE_NOT_DEF;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	if(pDevInfo->numMod == 0)
	{
		return ADEC_MODULE_NOT_DEF;
	}

	for(i = 0; i < pDevInfo->numMod; i++)
	{
		pModInfo = pDevInfo->pModInfo[i];
		if(pModInfo != NULL)
		{
			if(pModInfo->modType == modType)
			{
				break;
			}
		}
	}

	if(i == pDevInfo->numMod)
	{
		AUD_KDRV_RM("%s of %s is not found\n", \
			ModuleList_GetModuleTypeName(modType), strDevName[dev]);
		return ADEC_MODULE_NOT_DEF;
	}

	if(pModInfo == NULL)
	{
		AUD_KDRV_RM("%s of %s is not found\n", \
			ModuleList_GetModuleTypeName(modType), strDevName[dev]);
		return ADEC_MODULE_NOT_DEF;
	}

	return pModInfo->mod;
}

/**
 * Find Head Module
 *
 * @param 	dev				[in] device
 * @return 	ADEC_MODULE_ID.
 * @see
 */
ADEC_MODULE_ID AUDIO_FindHeadModule(LX_AUD_DEV_T dev)
{
	AUD_DEV_INFO_T 	*pDevInfo = NULL;
	AUD_MOD_INFO_T	*pModInfo = NULL;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return ADEC_MODULE_NOT_DEF;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	pModInfo = pDevInfo->pHeadMod;
	if(pModInfo == NULL)
	{
		AUD_KDRV_ERROR("pHeadMod of dev(%d) is NULL!!!\n", dev);
		return ADEC_MODULE_NOT_DEF;
	}

	AUD_KDRV_RM("HeadModule[%s] is found in %s\n", ModuleList_GetModuleName(pModInfo->mod), strDevName[dev]);

	return pModInfo->mod;
}

/**
 * Find Tail Module
 *
 * @param 	dev				[in] device
 * @return 	ADEC_MODULE_ID.
 * @see
 */
ADEC_MODULE_ID AUDIO_FindTailModule(LX_AUD_DEV_T dev)
{
	AUD_DEV_INFO_T 	*pDevInfo = NULL;
	AUD_MOD_INFO_T	*pModInfo = NULL;

	if(dev == LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return ADEC_MODULE_NOT_DEF;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	pModInfo = pDevInfo->pTailMod;
	if(pModInfo == NULL)
	{
		AUD_KDRV_RM("pTailMod of dev(%d) is NULL!!!\n", dev);
		return ADEC_MODULE_NOT_DEF;
	}

	AUD_KDRV_RM("TailModule[%s] is found in %s\n", ModuleList_GetModuleName(pModInfo->mod), strDevName[dev]);

	return pModInfo->mod;
}


/**
 * Initailization of  Device  and Module info
 *
 * @param 	: none
 * @return 	if succeeded -RET_OK , else - RET_OK.
 * @see
 */
SINT32  AUDIO_DevInfoInit(void)
{
	UINT32			i, j, k;
	SINT32			retVal = 0;
	AUD_DEV_INFO_T	*pDevInfo = NULL;
	ModuleInfo		modInfo;

	memset(&_gDevManInfo, 0x0, sizeof(AUD_DEV_MAN_INFO_T));
	memset(&_gModTypeNum[0], 0, sizeof(UINT32)*ADEC_MOD_TYPE_MAX);

	// create sema
	AUD_DEV_LOCK_INIT();
	AUD_MOD_LOCK_INIT();

	// initailize dev info
	for( i = LX_AUD_DEV_NONE ; i < LX_AUD_DEV_MAX ; i++)
	{
		pDevInfo = &_gDevManInfo.devInfo[i];

		pDevInfo->devType	= LX_AUD_DEV_TYPE_NONE;
		pDevInfo->dev 		= LX_AUD_DEV_NONE;
		pDevInfo->used		= FALSE;

		pDevInfo->index		= DEFAULT_DEV_INDEX;
		pDevInfo->pCtx		= NULL;

		pDevInfo->pHeadMod	= NULL;
		pDevInfo->pTailMod	= NULL;

		pDevInfo->numMod	= 0;
		for(j = 0; j < MAX_OWN_MODULE; j++)
			pDevInfo->pModInfo[j] = NULL;

		pDevInfo->devOutType = LX_AUD_DEV_OUT_NONE;
	}

	_gDevManInfo.allocBase[0] = 0;

	for( i = LX_AUD_DEV_TYPE_NONE ; i < LX_AUD_DEV_TYPE_MAX ; i++)
	{
		_gDevManInfo.allocCnt[i] = 0;
		_gDevManInfo.availableNum[i] = devAvailableNum[i];

		if(i+1 < LX_AUD_DEV_TYPE_MAX)
			_gDevManInfo.allocBase[i+1] = _gDevManInfo.allocBase[i] + devAvailableNum[i];

		AUD_KDRV_RM("[%12s][%d]allocCnt[%d] availableNum[%d] allocBase[%d]\n",	\
			strDevType[i] , i, _gDevManInfo.allocCnt[i], _gDevManInfo.availableNum[i], _gDevManInfo.allocBase[i]);
	}

	// initailize module info
	for(i = 1; i < ADEC_MODULE_MAX ; i++)
	{
		_gModInfo[i].dev = LX_AUD_DEV_NONE;
		_gModInfo[i].allocated = FALSE;

		retVal = ModuleList_GetModuleInfo(i, &modInfo);
		if(retVal != ADEC_ERR_NONE)
		{
			AUD_KDRV_ERROR("ModuleList_GetModuleInfo(%d) failed!!!\n", i);
			continue;
		}

		// module id, module type, core type
		_gModInfo[i].mod = modInfo.module;
		_gModInfo[i].modType = modInfo.type;
		_gModInfo[i].coreType = modInfo.location;

		if(_gModInfo[i].modType != _gModInfo[i-1].modType)
		{
			_gModTypeBase[modInfo.type] = i;
		}

		_gModTypeNum[modInfo.type]++;

		// initialize input port
		_gModInfo[i].countInConnection = 0;
		_gModInfo[i].numInPort = modInfo.num_in;

		for(j = 0; j < modInfo.num_in; j++)
		{
			_gModInfo[i].inPort[j].mod = ADEC_MODULE_NOT_DEF;
			_gModInfo[i].inPort[j].pBufMemCfg = NULL;
		}

		// initialize output port
		_gModInfo[i].countOutConnection = 0;
		_gModInfo[i].numOutPort = 0;

		if(modInfo.num_out != 0)
		{
			for(k = 0; k < modInfo.num_out; k++)
			{
				if(modInfo.out_buffer_type[k] == ADEC_BUF_INTRA)
				{
					_gModInfo[i].numOutPort += MAX_OUT_INTRA_PORT;
				}
				else if(modInfo.out_buffer_type[k] == ADEC_BUF_MULTI_SINK)
				{
					_gModInfo[i].numOutPort += MAX_OUT_MULTI_SINK_PORT;
				}

				for(j = 0; j < MAX_OUT_PORT; j++)
				{
					_gModInfo[i].outPort[j].mod = ADEC_MODULE_NOT_DEF;
					_gModInfo[i].outPort[j].pBufMemCfg = NULL;
				}
			}
		}
	}

	AUD_KDRV_RM("%s.%d] initialized.\n", __F__, __L__);

	return RET_OK;
}

/**
 * Initialize Module info
 *
 * @param 	: none
 * @return 	if succeeded -RET_OK , else - RET_OK.
 * @see
 */
static void  AUDIO_ModInfoInit(AUD_MOD_INFO_T *pModInfo)
{
	UINT32			i;
	pModInfo->dev = LX_AUD_DEV_NONE;
	pModInfo->allocated = FALSE;

	// initialize input port
	pModInfo->countInConnection = 0;

	for(i = 0; i < pModInfo->numInPort; i++)
	{
		pModInfo->inPort[i].mod = ADEC_MODULE_NOT_DEF;
		pModInfo->inPort[i].pBufMemCfg = NULL;
	}
	// initialize output port
	pModInfo->countOutConnection = 0;

	for(i = 0; i < pModInfo->numOutPort; i++)
	{
		pModInfo->outPort[i].mod = ADEC_MODULE_NOT_DEF;
		pModInfo->outPort[i].pBufMemCfg = NULL;
	}

	AUD_KDRV_RM("%s info is initialized.\n", ModuleList_GetModuleName(pModInfo->mod));

	return;
}

/**
 * Get Pointer of Device information
 *
 * @param 	alloc 	[in] alloc device
 * @return 	if  succed-pointer of device information  sturct , else - .
 * @see
 */
AUD_DEV_INFO_T *AUDIO_GetDevInfo(LX_AUD_DEV_T dev)
{
	AUD_DEV_INFO_T *pDevInfo = NULL;

	if(dev <= LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return NULL;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	return pDevInfo;
}

/**
 * Get Pointer of Module information
 *
 * @param 	alloc 	[in] alloc device
 * @return 	if  succed-pointer of device information  sturct , else - .
 * @see
 */
AUD_MOD_INFO_T *AUDIO_GetModuleInfo(ADEC_MODULE_ID mod)
{
	AUD_MOD_INFO_T *pModInfo;

	if(mod <= ADEC_MODULE_NOT_DEF || mod >= ADEC_MODULE_MAX)
	{
		AUD_KDRV_ERROR("mod[%d] is invalid.\n", mod);
		return NULL;
	}

	pModInfo = &_gModInfo[mod];

	return pModInfo;
}

/**
 * Get Core of Given Module
 *
 * @param 	audioModule 	[in] alloc module
  * @return 	if  succed- Core(manager)   , else - .
 * @see
 */
ADEC_MODULE_ID AUDIO_GetManagerModule(ADEC_MODULE_ID mod)
{
	ADEC_MODULE_ID		managerModule;
	ADEC_CORE_TYPE		coreOfModule = ModuleList_FindModule(mod);

	switch (coreOfModule)
	{
		case ADEC_CORE_ARM:
			managerModule = ADEC_MODULE_MAN_ARM;
			break;
		case ADEC_CORE_DSP0:
			managerModule = ADEC_MODULE_MAN_DSP0;
			break;
		case ADEC_CORE_DSP1:
			managerModule = ADEC_MODULE_MAN_DSP1;
			break;
		default:
			managerModule = ADEC_MODULE_NOT_DEF;
			break;
	}

	return managerModule;
}

/**
 * Get Core of Given Device
 *
 * @param 	dev				[in] device
 * @return 	coreOfDevice	core type
 * @see
 */
ADEC_CORE_TYPE AUDIO_GetCoreDevice(LX_AUD_DEV_T dev)
{
	ADEC_CORE_TYPE		coreOfDevice;

	if(dev >= LX_AUD_DEV_DEC0 && dev <= LX_AUD_DEV_CAP2)
	{
		coreOfDevice = ADEC_CORE_DSP0;
	}
	else if(dev >= LX_AUD_DEV_REN0 && dev <= LX_AUD_DEV_MAS0)
	{
		coreOfDevice = ADEC_CORE_DSP1;
	}
	else
	{
		coreOfDevice = ADEC_CORE_NOT_DEF;
	}

	return coreOfDevice;
}

/**
 * Check if index is used.
 *
 * @param 	pIndexCtx				[in] index context
 * @return 	RET_OK or RET_ERROR
 * @see
 */
SINT32 AUDIO_CheckDuplicated(LX_AUD_DEV_INDEX_CTX_T *pIndexCtx)
{
	LX_AUD_DEV_TYPE_T		devType = LX_AUD_DEV_TYPE_NONE;
	LX_AUD_DEV_T			dev = LX_AUD_DEV_NONE;
	AUD_DEV_INFO_T			*pDevInfo = NULL;
	UINT32					searchStart, searchEnd;

	devType = pIndexCtx->devType;
	if(devType != LX_AUD_DEV_TYPE_DEC)
	{
		AUD_KDRV_RMD("devType[%d] is not decoder.\n", devType);
		return RET_OK;
	}

	searchStart = LX_AUD_DEV_DEC0;
	searchEnd = searchStart + DEV_DEC_NUM;

	for(dev = searchStart; dev < searchEnd; dev++)
	{
		pDevInfo = &_gDevManInfo.devInfo[dev];
		if(pDevInfo->used == FALSE)
		{
			break;
		}

		if(pDevInfo->index == pIndexCtx->ui32Index)
		{
			AUD_KDRV_ERROR("index[%d] cannot be used.\n", pIndexCtx->ui32Index);
			return RET_ERROR;
		}
	}

	return RET_OK;
}

/**
 * Set Index & Context of the Device
 *
 * @param 	dev				[in] device
 * @return 	coreOfDevice	core type
 * @see
 */
SINT32 AUDIO_SetDevIndexCtx(LX_AUD_DEV_T dev, LX_AUD_DEV_INDEX_CTX_T *pIndexCtx)
{
	AUD_DEV_INFO_T *pDevInfo = NULL;

	if(dev <= LX_AUD_DEV_NONE || dev >= LX_AUD_DEV_MAX)
	{
		AUD_KDRV_ERROR("dev[%d] is invalid.\n", dev);
		return RET_ERROR;
	}

	if(pIndexCtx == NULL)
	{
		AUD_KDRV_ERROR("pIndexCtx is NULL.\n");
		return RET_ERROR;
	}

	if(AUDIO_CheckDuplicated(pIndexCtx) == RET_ERROR)
	{
		AUD_KDRV_ERROR("index[%d] is already used.\n", pIndexCtx->ui32Index);
		return RET_ERROR;
	}

	pDevInfo = &_gDevManInfo.devInfo[dev];

	pDevInfo->index = pIndexCtx->ui32Index;
	pDevInfo->pCtx= pIndexCtx->pCtx;

	AUD_KDRV_RMD("dev[%d] idx[%d] pCtx[%p].\n", pDevInfo->dev, pDevInfo->index, pDevInfo->pCtx);

	return RET_OK;
}

/**
 * Get Index & Context of the Device
 *
 * @param 	dev				[in] device
 * @return 	coreOfDevice	core type
 * @see
 */
SINT32 AUDIO_GetDevIndexCtx(LX_AUD_DEV_INDEX_CTX_T *pIndexCtx)
{
	AUD_DEV_INFO_T *pDevInfo = NULL;
	SINT32			i = 0;
	UINT32			searchStart, searchEnd;
	UINT32			availableNum, allocBase;
	LX_AUD_DEV_TYPE_T	devType;

	if(pIndexCtx == NULL)
	{
		AUD_KDRV_ERROR("pIndexCtx is NULL.\n");
		return RET_ERROR;
	}

	devType = pIndexCtx->devType;

	if(devType == LX_AUD_DEV_TYPE_NONE || devType >= LX_AUD_DEV_TYPE_MAX)
	{
		AUD_KDRV_ERROR("devType[%d] is invalid.\n", devType);
		return RET_ERROR;
	}

	availableNum 	= _gDevManInfo.availableNum[devType];
	allocBase 		= _gDevManInfo.allocBase[devType];

	searchStart = allocBase;
	searchEnd = searchStart + availableNum;

	for(i = searchStart; i < searchEnd; i++)
	{
		pDevInfo = &_gDevManInfo.devInfo[i];
		if(pDevInfo->devType == LX_AUD_DEV_TYPE_REN)
		{
			if((pDevInfo->index == pIndexCtx->ui32Index) &&
				(pDevInfo->devOutType == pIndexCtx->devOutType))
			{
				break;
			}
		}
		else
		{
			if(pDevInfo->index == pIndexCtx->ui32Index)
			{
				break;
			}
		}
	}

	pIndexCtx->dev = pDevInfo->dev;
	pIndexCtx->pCtx = pDevInfo->pCtx;

	//AUD_KDRV_RMD("dev[%d] idx[%d] pCtx[%p].\n", pDevInfo->dev, pDevInfo->index, pDevInfo->pCtx);

	return RET_OK;
}

/**
 * Get module of Given input type
 *
 * @param 	mod 	[in] module
  * @return 	ADEC_MODULE_ID.
 * @see
 */
ADEC_MODULE_ID AUDIO_GetModuleInputType(LX_AUD_INPUT_T inputType)
{
	ADEC_MODULE_ID		inputModule;
	AUD_MOD_INFO_T		*pModInfo = NULL;

	switch (inputType)
	{
		case LX_AUD_INPUT_TP0:
			inputModule = ADEC_MODULE_TP_IN_0;
			break;
		case LX_AUD_INPUT_TP1:
			inputModule = ADEC_MODULE_TP_IN_1;
			break;
		case LX_AUD_INPUT_SIF:
			inputModule = ADEC_MODULE_SIF_IN;
			break;
		case LX_AUD_INPUT_ADC:
			inputModule = ADEC_MODULE_ADC_IN;
			break;
		case LX_AUD_INPUT_HDMI:
		case LX_AUD_INPUT_HDMI0:
		case LX_AUD_INPUT_HDMI1:
		case LX_AUD_INPUT_HDMI2:
		case LX_AUD_INPUT_HDMI3:
			inputModule = ADEC_MODULE_HDMI_IN_0;
			pModInfo = AUDIO_GetModuleInfo(inputModule);
			if((pModInfo->allocated == FALSE) ||
				(gSourceModuleInput[inputModule - ADEC_MODULE_TP_IN_0] == inputType))
			{
				break;
			}

			inputModule = ADEC_MODULE_HDMI_IN_1;
			pModInfo = AUDIO_GetModuleInfo(inputModule);
			if((pModInfo->allocated == FALSE) ||
				(gSourceModuleInput[inputModule - ADEC_MODULE_TP_IN_0] == inputType))
			{
				break;
			}
			else
			{
				return ADEC_MODULE_NOT_DEF;
			}
		case LX_AUD_INPUT_I2S:
			inputModule = ADEC_MODULE_I2S_IN;
			break;
		case LX_AUD_INPUT_SYSTEM:
			return ADEC_MODULE_NOT_DEF;
			break;
		default:
			AUD_KDRV_RM("InputType[%d] is not available \n", inputType);
			return ADEC_MODULE_NOT_DEF;
	}

	if((inputModule >= ADEC_MODULE_TP_IN_0) && (inputModule <= ADEC_MODULE_I2S_IN))
	{
		gSourceModuleInput[inputModule - ADEC_MODULE_TP_IN_0] = inputType;
	}

	AUD_KDRV_RM("source(%d), input(%d)\n", inputModule, inputType);

	return inputModule;
}

void *AUDIO_GetInputModulePath(LX_AUD_INPUT_T inputType)
{
	SINT32	i;
	ADEC_MODULE_ID	mod;

	mod = AUDIO_GetModuleInputType(inputType);
	if(mod == ADEC_MODULE_NOT_DEF)
	{
		AUD_KDRV_ERROR("AUDIO_GetModuleInputType(%d) is failed \n", inputType);
		return NULL;
	}

	for(i = 0; i < _gModTypeNum[ADEC_MOD_TYPE_SOURCE]; i++)
	{
		if(_gInputModPath[i].pModInfo[0]->mod == mod)
		{
			break;
		}
	}

	if(i == _gModTypeNum[ADEC_MOD_TYPE_SOURCE])
	{
		AUD_KDRV_ERROR("Failed to find module(%d).\n", mod);
		return NULL;
	}

	return _gInputModPath[i].pPath;
}

SINT32 AUDIO_IMC_Connect(const void   *_self,
                               ADEC_MODULE_ID  _input_module,
                               unsigned int    _input_port,
                               ADEC_MODULE_ID  _output_module,
                               unsigned int    _output_port)
{
	CmCmdConnect		connectModule;
	ADEC_MODULE_ID		managerModule;

	managerModule = AUDIO_GetManagerModule(_input_module);

	connectModule.input_module	= _input_module;
	connectModule.input_port	= _input_port;
	connectModule.output_module	= _output_module;
	connectModule.output_port	= _output_port;
	AUDIO_IMC_SendCmdParam(CM_CMD_CONNECT, managerModule, sizeof(CmCmdConnect), &connectModule);

	AUD_KDRV_RMD("im:%d, ip:0x%x, om:%d, op:0x%x\n", \
		_input_module, _input_port, _output_module, _output_port);

	return RET_OK;
}

SINT32 AUDIO_IMC_Disconnect(const void   *_self,
	                          ADEC_MODULE_ID   _disconnect,
	                          unsigned int     _port)
{
	CmCmdDisconnect		disconnectParam;
	ADEC_MODULE_ID		managerModule;

	managerModule = AUDIO_GetManagerModule(_disconnect);

	disconnectParam.module	= _disconnect;
	disconnectParam.port	= _port;

	AUDIO_IMC_SendCmdParam(CM_CMD_DISCONNECT, managerModule, sizeof(CmCmdDisconnect), &disconnectParam);

	AUD_KDRV_RMD("mod:%d, port:0x%x\n", _disconnect, _port);

	return RET_OK;
}

SINT32 AUDIO_IMC_AllocBuf(const void   *_self,
	                        ADEC_MODULE_ID _module,
	                        unsigned int   _port,
	                        unsigned int   _physical_addr,
	                        unsigned int   _register_addr_offset,
	                        unsigned int   _is_reader)
{
	CmCmdAllocBuf		allocBuf;
	ADEC_MODULE_ID		managerModule;

	managerModule = AUDIO_GetManagerModule(_module);

	allocBuf.module					= _module;
	allocBuf.port					= _port;
	allocBuf.physical_addr			= _physical_addr;
	allocBuf.register_addr_offset	= _register_addr_offset;
	allocBuf.is_reader				= _is_reader;
	AUDIO_IMC_SendCmdParam(CM_CMD_ALLOC_BUF, managerModule, sizeof(CmCmdAllocBuf), &allocBuf);

	AUD_KDRV_RMD("m:%d, m:%d, p:0x%x, pa:0x%x, rao:0x%x, ir:%d.\n",  \
		managerModule, _module, _port, _physical_addr, _register_addr_offset, _is_reader);

	return RET_OK;
}

SINT32 AUDIO_IMC_DeallocBuf(const void   *_self,
	                           ADEC_MODULE_ID  _module,
	                           unsigned int    _port)
{
	CmCmdDeallocBuf		deallocBuf;
	ADEC_MODULE_ID		managerModule;

	managerModule = AUDIO_GetManagerModule(_module);

	deallocBuf.module				= _module;
	deallocBuf.port					= _port;
	AUDIO_IMC_SendCmdParam(CM_CMD_DEALLOC_BUF, managerModule, sizeof(CmCmdDeallocBuf), &deallocBuf);

	AUD_KDRV_RMD("mod:%d, port:0x%x)\n", _module, _port);

	return RET_OK;
}

