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



/** @file audio_drv_manager.h
 *
 *  driver interface header for audio device. ( used only within kdriver )
 *	audio device will teach you how to make device driver with new platform.
 *
 *  @author	wonchang.shin (wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2012.04.25
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_MANAGER_MGR_H_
#define	_AUDIO_DRV_MANAGER_MGR_H_

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "module/adec_module_list.h"

#include "audio_kapi.h"
#include "audio_buffer.h"

#define DEV_NONE_NUM	1
#define DEV_DEC_NUM		2
#define DEV_ENC_NUM		2
#define DEV_CAP_NUM		6
#define DEV_REN_NUM		12		//8
#define DEV_MAS_NUM		1


#define MAX_OWN_MODULE	16

#define MAX_PORT		32

#define MAX_IN_PORT				10
#define MAX_OUT_MULTI_SINK_PORT	8
#define MAX_OUT_INTRA_PORT		1
#define MAX_OUT_PORT			(MAX_OUT_MULTI_SINK_PORT + MAX_OUT_INTRA_PORT)

#define IN_PORT 	0x000
#define OUT_PORT 	0x100

//related bypass module
#define BYTE_BASED_MODE		0
#define AU_BASED_MODE		1

#define NO_OVERFLOW_PROTECTION		0
#define ENABLE_OVERFLOW_PROTECTION		1

#define GET_DEC_INDEX(ALLOCDEC_DEV_ID)	((ALLOCDEC_DEV_ID) - LX_AUD_DEV_DEC0)
#define GET_REN_INDEX(ALLOCDEC_DEV_ID)	((ALLOCDEC_DEV_ID) - LX_AUD_DEV_REN0)
#define GET_ENC_INDEX(ALLOCDEC_DEV_ID)	((ALLOCDEC_DEV_ID) - LX_AUD_DEV_ENC0)
#define GET_CAP_INDEX(ALLOCDEC_DEV_ID)	((ALLOCDEC_DEV_ID) - LX_AUD_DEV_CAP0)

#define DEV_TYPE_BASE	1

#define	PATH_NAME_SIZE	20

// InputCtrl Information
#define DEFAULT_DEV_INDEX	0xFF
#define MAX_DEV_INDEX		10			///< NUM_OF_DEC_IN(2) + NUM_OF_MPB_IN(8)
#define NUM_OF_ES_IN		2
#define NUM_OF_INPUTCTRL	(MAX_DEV_INDEX + NUM_OF_ES_IN)

/**
 * AUDIO Drive/Module Status.
 */
typedef enum {
	AUD_STATUS_NONE,		//	0,
	AUD_STATUS_INIT,		//	1,
	AUD_STATUS_START,		//	2,
	AUD_STATUS_STOP,		//	3,
	AUD_STATUS_FLUSH,		//	4,
	AUD_STATUS_MAX
} AUD_STATUS_T;

/**
 * Audio Port information
*/
typedef struct
{
	ADEC_MODULE_ID	mod;										// Connected Module ID
	void			*pBufMemCfg;								// Connected Buffer Configuration Pointer
} AUD_PORT_INFO_T;

/**
 * Audio module information
*/
typedef struct
{
	LX_AUD_DEV_T		dev;									// device id

	ADEC_MODULE_ID		mod;									// module id
	ADEC_MODULE_TYPE	modType;								// module type
	ADEC_CORE_TYPE		coreType;								// core type

	BOOLEAN				allocated;								// allocated or not
	AUD_STATUS_T		status;									// status
	UINT32				countInConnection;						// count of input connection
	UINT32				numInPort;								// number of input port
	AUD_PORT_INFO_T		inPort[MAX_IN_PORT];					// connected module of input port

	UINT32				countOutConnection;						// count of output port connection
	UINT32				numOutPort;								// number of output port
	AUD_PORT_INFO_T		outPort[MAX_OUT_PORT];					// connected module of output port
} AUD_MOD_INFO_T;

/**
 * Audio Path information
*/
typedef struct
{
	UINT8				strPathName[PATH_NAME_SIZE];			// path name
	void				*pPath;									// connection path
	UINT32				numMod;									// number of allocated modules
	AUD_MOD_INFO_T		*pModInfo[MAX_OWN_MODULE];				// info of allocated modules
} AUD_PATH_INFO_T;

/**
 * Audio device information
*/
typedef struct
{
	LX_AUD_DEV_TYPE_T		devType;								// device type
	LX_AUD_DEV_T			dev;									// device id
	BOOLEAN					used;									// used or not

	UINT32					index;									// index
	void					*pCtx;									// KADP context

	AUD_MOD_INFO_T			*pHeadMod;								// info of Head modules
	AUD_MOD_INFO_T			*pTailMod;								// info of Tail modules

	UINT32					numMod;									// number of allocated modules
	AUD_MOD_INFO_T			*pModInfo[MAX_OWN_MODULE];				// info of allocated modules

	LX_AUD_DEV_OUT_TYPE_T	devOutType;								// device output type
} AUD_DEV_INFO_T;

/**
 * Audio alloc free information for Main dev(having a device node) , Sub dev
 * adec ioctl parameter.
 * @see ADEC_IOR_GET_ENCODING_INFO
*/
typedef struct
{
	UINT32	conNum;
	UINT32 	allocCnt[LX_AUD_DEV_TYPE_MAX];
	UINT32 	availableNum[LX_AUD_DEV_TYPE_MAX];
	UINT32 	allocBase[LX_AUD_DEV_TYPE_MAX];

	AUD_DEV_INFO_T devInfo[LX_AUD_DEV_MAX];
} AUD_DEV_MAN_INFO_T;



SINT32  AUDIO_DevInfoInit (void );

LX_AUD_DEV_T  AUDIO_AllocDev(LX_AUD_DEV_TYPE_T devType);
void  AUDIO_FreeDev(LX_AUD_DEV_T dev);
ADEC_MODULE_ID AUDIO_AllocModule(ADEC_MODULE_TYPE modType, ADEC_CORE_TYPE coreType, ADEC_MODULE_ID mod);
void AUDIO_FreeModule(ADEC_MODULE_ID mod);

SINT32 AUDIO_ConnectDevices(LX_AUD_DEV_T outDev, LX_AUD_DEV_T inDev, AUD_BUFFER_TYPE_T bufType);
SINT32 AUDIO_DisconnectDevices(LX_AUD_DEV_T outDev, LX_AUD_DEV_T inDev);
SINT32 AUDIO_ConnectModules(ADEC_MODULE_ID outMod, ADEC_MODULE_ID inMod, AUD_BUFFER_TYPE_T bufType);
SINT32 AUDIO_DisconnectModules(ADEC_MODULE_ID outMod, ADEC_MODULE_ID inMod);
SINT32 AUDIO_DisconnectAllModules(LX_AUD_DEV_T dev);
SINT32 AUDIO_ConnectModulePort(ADEC_MODULE_ID outMod, ADEC_MODULE_ID inMod, SINT32 inPort);
SINT32 AUDIO_DisconnectInputModules(ADEC_MODULE_ID mod);

SINT32 AUDIO_AddModule(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod);
SINT32 AUDIO_InsertModuleLast(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod, AUD_BUFFER_TYPE_T bufType);
SINT32 AUDIO_InsertModuleFirst(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod, AUD_BUFFER_TYPE_T bufType);
SINT32 AUDIO_RemoveModule(LX_AUD_DEV_T dev, ADEC_MODULE_ID mod);

ADEC_MODULE_ID AUDIO_FindHeadModule(LX_AUD_DEV_T dev);
ADEC_MODULE_ID AUDIO_FindTailModule(LX_AUD_DEV_T dev);

ADEC_MODULE_ID AUDIO_FindModuleType(LX_AUD_DEV_T dev, ADEC_MODULE_TYPE modType);
ADEC_MODULE_ID AUDIO_GetManagerModule(ADEC_MODULE_ID mod);
ADEC_MODULE_ID AUDIO_GetModuleInputType(LX_AUD_INPUT_T inputType);
ADEC_CORE_TYPE AUDIO_GetCoreDevice(LX_AUD_DEV_T dev);
AUD_DEV_INFO_T *AUDIO_GetDevInfo(LX_AUD_DEV_T dev);
AUD_MOD_INFO_T *AUDIO_GetModuleInfo(ADEC_MODULE_ID mod);
SINT32 AUDIO_GetPort(ADEC_MODULE_ID mod, UINT32 inOut, ADEC_MODULE_ID connectedMod);


void  AUDIO_UpdateConnectionInfo(void );
void  AUDIO_UpdateAllocInfo(void );
void  AUDIO_ShowDevManInfo(void );

SINT32 AUDIO_SetDevIndexCtx(LX_AUD_DEV_T dev, LX_AUD_DEV_INDEX_CTX_T *pIndexCtx);
SINT32 AUDIO_GetDevIndexCtx(LX_AUD_DEV_INDEX_CTX_T*pIndexCtx);

extern AUD_DEV_MAN_INFO_T _gDevManInfo;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_MANAGER_MGR_H_ */

/** @} */

