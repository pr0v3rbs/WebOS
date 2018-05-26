/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


#include <linux/kernel.h>

#include "vds/pts_drv.h"

#include "audioinf.h"

#define LOG_NAME	vdec_audioinf
#include "log.h"

#if !defined(CONFIG_VDEC_ON_FPGA) && !defined(__XTENSA__)
#define HAVE_AUDIO_API
#endif

#if defined(__XTENSA__) && defined(CHIP_NAME_h15)
#define HAVE_AUDIO_IPC
#endif


#if defined(HAVE_AUDIO_API)
#include "audio_kapi_renderer.h"
#endif

#if defined(HAVE_AUDIO_IPC)
#include "ipc_vdec.h"
extern struct ipc *vdec_ipc;
#endif


int audioinf_set_basetime (int port,
		uint64_t base_stc,
		uint64_t base_pts)
{
	if (port < 0)
	{
		log_warning ("wrong audio port %d\n", port);
		return -1;
	}

	{
#if defined(HAVE_AUDIO_API)
		extern SINT32 KDRV_AUDIO_SetBasetime(UINT32 index, LX_AUD_RENDER_PARAM_BASETIME_T *pParamBasetime);
		LX_AUD_RENDER_PARAM_BASETIME_T param;

		memset (&param, 0, sizeof (param));
		param.ui64ClockBaseTime = base_stc & VDEC_PTS_MASK;
		param.ui64StreamBaseTime = base_pts & VDEC_PTS_MASK;

		KDRV_AUDIO_SetBasetime (port, &param);
#elif defined(HAVE_AUDIO_IPC)
		ipc_send_noirq (vdec_ipc, ipc_vdec_type_audioinf_set_basetime_stc,
				0, port,
				base_stc & 0xffffffff,
				NULL);
		ipc_send (vdec_ipc, ipc_vdec_type_audioinf_set_basetime_pts,
				0, port,
				base_pts & 0xffffffff,
				NULL);
#endif
	}

	log_noti ("audio%d, set basetime. "
			"basestc: 0x%08llx, basepts: 0x%08llx\n",
			port, base_stc, base_pts);

	return 0;
}


int audioinf_playto (int port, uint64_t pts)
{
	if (port < 0)
	{
		log_warning ("wrong audio port %d\n", port);
		return -1;
	}

	{
#if defined(HAVE_AUDIO_API)
		extern SINT32 KDRV_AUDIO_SetPlayToPts(UINT32 index, UINT32 pts);

		KDRV_AUDIO_SetPlayToPts (port, pts);
#elif defined(HAVE_AUDIO_IPC)
		ipc_send (vdec_ipc, ipc_vdec_type_audioinf_playto,
				0, port, pts & 0xffffffff,
				NULL);
#endif
	}

	log_noti ("audio%d play to 0x%08llx\n",
			port, pts);

	return 0;
}

