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

/** @file audio_drv_renderer.h
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

#ifndef	_AUDIO_DRV_RENDERER_H_
#define	_AUDIO_DRV_RENDERER_H_

#include "audio_kapi_renderer.h"
#include "audio_drv_manager.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


#define AUD_RENDER_CONNECT_NUM	5

#undef USE_LIPSYNC_FREE
#undef AUD_RENDER_TIME_STAMP


/**
 * AUDIO Renderer Connect Information.
 */
typedef struct
{
	LX_AUD_DEV_TYPE_T	devType;
	LX_AUD_DEV_T		connectDev;
} AUD_RENDER_CONNECT_T;

/**
 * AUDIO Renderer Information.
 */
typedef struct {
	UINT32								ui32AllocDev;

	LX_AUD_RENDER_PARAM_T				renderParam;
	LX_AUD_RENDER_PARAM_LIPSYNC_T		renderLipsync;
	LX_AUD_RENDERED_STATUS_T			renderStatus;
	LX_AUD_RENDER_CLK_TYPE_T			renderClockType;
	LX_AUD_RENDER_PARAM_BASETIME_T		renderBasetime;			///< basetime for lipsync

	AUD_EVENT_T							renderEvent[AUD_EVENT_NUM];
	AUD_RENDER_CONNECT_T				renderConnect[AUD_RENDER_CONNECT_NUM];	///< For Reset Recovery

	UINT32				ui32MixerPort;			///< mixer port
	AUD_STATUS_T		status;					///< AUD_STATUS_T
	BOOLEAN				bStartCalled;
	UINT32				bClosed;
	UINT32				bResetting;
	UINT32				bLipsyncOnOff;
	UINT32				ui32Volume;
	UINT32				ui32LeftVolume;
	UINT32				ui32RightVolume;
	UINT32				ui32EventNum;
	UINT32				ui32ConnectNum;			///< For Reset Recovery

	BOOLEAN				bBufferRepeat;			///< The status of buffer repeat is set previously.
	LX_AUD_RENDER_PARAM_NODELAY_T		renderNodelay;
	UINT32				ui32Delay;				///< delay ms.
	LX_AUD_RENDER_RTS_PARAM_T			rtsParam;	///< the setting of RTS module
	UINT32				ui32CheckSum;			///< add checksum to fix audio decoder not working.
#ifdef USE_LIPSYNC_FREE
	BOOLEAN				bUnderflowCalled;		///< underflow called status
	UINT64 				ui64TickUnderflow;		///< start tick of undeflow
	UINT32				ui32CountUnderflow;		///< count of undeflow
#endif
	UINT32				ui32InputCtrlDelay;		///< input control delay
	LX_AUD_TRICK_MODE_T	trickMode;				///< trick mode

#ifdef AUD_RENDER_TIME_STAMP
	UINT64				ui64Timestamp[MPB_AUI_INDEX_COUNT];
#endif
} AUD_RENDER_INFO_T;

/**
 * AUDIO Renderer Input Control Information.
 */
typedef struct {
	UINT32				ui32InputCtrlVolume;
	UINT32				ui32InputCtrlMute;
	UINT32				ui32InputCtrlDelay;
} AUD_RENDER_INPUT_CTRL_T;

SINT32	KDRV_AUDIO_OpenRenderer(struct inode *inode, struct file *filp);
SINT32	KDRV_AUDIO_CloseRenderer(struct inode *inode, struct file *filp);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlRenderer(struct inode *, struct file *, unsigned int, unsigned long );
#else
long KDRV_AUDIO_IoctlRenderer(struct file *filp, unsigned int cmd, unsigned long arg );
#endif
void	KDRV_AUDIO_ResetRenderer(void);
void	KDRV_AUDIO_ResetRendererParam(void);
void	KDRV_AUDIO_RestartRenderer(void);
void	KDRV_AUDIO_RedemandDecodedNofiRenderer(void);

SINT32 AUDIO_StartRenderer(UINT32 allocDev);
SINT32 AUDIO_StopRenderer(UINT32 allocDev);
SINT32 AUDIO_FlushRenderer(UINT32 allocDev);
SINT32 AUDIO_FeedRen(UINT32 allocDev, LX_AUD_RENDER_FEED_T * pRenFeed);
SINT32 AUDIO_SetRendererParam(UINT32 allocDev, LX_AUD_RENDER_PARAM_T *pParamCodec);
SINT32 AUDIO_ConnectRenderer(UINT32 allocDev, UINT32 otherAllocDev);
SINT32 AUDIO_DisconnectRenderer(UINT32 allocDev, UINT32 otherAllocDev);
UINT32 AUDIO_OpenRenderer(void);
SINT32 AUDIO_CloseRenderer(UINT32 allocDev);
SINT32 AUDIO_GetRenderedStatus(UINT32 allocDev, LX_AUD_RENDERED_STATUS_T *pRenderedStatus);
SINT32 AUDIO_EnableLipsync(UINT32 allocDev, UINT32 bOnOff);
SINT32 AUDIO_GetRenderDelay(UINT32 allocDev, UINT32 *pDelay);
SINT32 AUDIO_SetInputCtrlVolume(UINT32 allocDev, UINT32 volume);
SINT32 AUDIO_SetInputCtrlMute(UINT32 allocDev, UINT32 mute);
SINT32 AUDIO_SetInputCtrlDelay(UINT32 allocDev, UINT32 delay);
SINT32 AUDIO_GetLipsyncOnOff(UINT32 allocDev, BOOLEAN *pbLipsyncOnOff);
SINT32 AUDIO_GetRenStartInfo(UINT32 allocDev, UINT32 *pStarted);

LX_AUD_RENDER_PARAM_T*	KDRV_AUDIO_GetRendererInfo(LX_AUD_DEV_T allocDev);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_RENDERER_H_ */

