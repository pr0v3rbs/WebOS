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

/** @file
 *
 *  AV LipSync HAL.
 *  (Register Access Hardware Abstraction Layer )
 *
 *  author	youngki.lyu@lge.com
 *  version	1.0
 *  date		2011-10-19
 * @addtogroup h13_vdec
 * @{
 */

#define LOG_NAME	vdec_hal_avsync
#include <linux/kernel.h>
#include <linux/io.h>

#ifndef __XTENSA__
#include "base_drv.h"
#endif

#include "hal/vdec_base.h"


#include "log.h"


#ifndef __XTENSA__
#define VDEC_PTS_BIT_SIZE		31
#define VDEC_PTS_MASK			((1U<<(VDEC_PTS_BIT_SIZE)) - 1)

#define	VDEC_AV_LIPSYNC_BASE(ch)	(VDEC_BASE + 0x08F0 + (ch * 0x8))	// write
#define	ADEC_LIPSYNC_BASE(ch)		(0xC8836000 + 0x06C0 + (ch * 0x8)) // read



typedef struct {
	unsigned int	stc;	// 0:27
	unsigned int	pts;	// 0:27
} AV_LIPSYNC_BASE_REG_T;


static volatile AV_LIPSYNC_BASE_REG_T		*vdec_base[VDEC_NUM_OF_CHANNEL];
static volatile AV_LIPSYNC_BASE_REG_T		*adec_base[VDEC_NUM_OF_CHANNEL];
#endif

void AV_LipSync_HAL_Init(void)
{
#ifndef __XTENSA__
	int a;

	log_noti ("init\n");

	for( a = 0; a < VDEC_NUM_OF_CHANNEL; a++ )
	{
		vdec_base[a] = (volatile AV_LIPSYNC_BASE_REG_T *)
			ioremap (VDEC_AV_LIPSYNC_BASE(a),
					sizeof (AV_LIPSYNC_BASE_REG_T));

		adec_base[a] = (volatile AV_LIPSYNC_BASE_REG_T *)
			ioremap (ADEC_LIPSYNC_BASE(a),
					sizeof (AV_LIPSYNC_BASE_REG_T));

		vdec_base[a]->stc = vdec_base[a]->pts = (unsigned int)-1;
	}
#endif
}

#ifndef __XTENSA__
static AV_LIPSYNC_BASE_REG_T snapshot_backup[VDEC_NUM_OF_CHANNEL];
#endif
void AV_LipSync_HAL_Suspend (void)
{
#ifndef __XTENSA__
	int a;

	for (a=0; a<VDEC_NUM_OF_CHANNEL; a++)
		snapshot_backup[a] = *vdec_base[a];
#endif
}

void AV_LipSync_HAL_Resume (void)
{
#ifndef __XTENSA__
	int a;

	for (a=0; a<VDEC_NUM_OF_CHANNEL; a++)
		*vdec_base[a] = snapshot_backup[a];
#endif
}

#ifndef __XTENSA__
static void AV_LipSync_HAL_Vdec_SetBase(int ch,
		unsigned int base_stc, unsigned int base_pts)
{
	log_debug ("src%d, set vdec base time %x, pts %x\n",
			ch, base_stc, base_pts );

	if( ch >= VDEC_NUM_OF_CHANNEL )
	{
		log_warning ("unknown channel number %d\n", ch);
		return;
	}

	vdec_base[ch]->stc = base_stc;
	vdec_base[ch]->pts = base_pts;
}

static void _AV_LipSync_HAL_Adec_GetBase(int ch,
		unsigned int *base_stc, unsigned int *base_pts)
{
	if( ch >= VDEC_NUM_OF_CHANNEL )
	{
		log_warning ("wrong channel number %d\n", ch);
		return;
	}

	*base_stc = adec_base[ch]->stc;
	*base_pts = adec_base[ch]->pts;

	log_debug ("src%d get adec base time %x, pts %x\n", ch,
			*base_stc, *base_pts);
}
#endif

void AV_LipSync_HAL_Adec_GetBase(int ch,
		unsigned int *base_stc, unsigned int *base_pts)
{
#ifndef __XTENSA__
	BASE_AVLIPSYNC_LOCK ();

	_AV_LipSync_HAL_Adec_GetBase (ch, base_stc, base_pts);

	BASE_AVLIPSYNC_UNLOCK ();
#endif
}

void AV_Set_AVLipSyncBase(int ch_vdec, int ch_adec,
		unsigned int *_base_stc, unsigned int *_base_pts)
{
#ifndef __XTENSA__
	unsigned int base_stc = *_base_stc;
	unsigned int base_pts = *_base_pts;

	if (
			ch_vdec < 0 ||
			ch_vdec >= VDEC_NUM_OF_CHANNEL ||
			ch_adec < 0 ||
			ch_adec >= VDEC_NUM_OF_CHANNEL
	   )
	{
		log_warning ("wrong channel %d, %d\n",
				ch_vdec, ch_adec);
		return;
	}

	log_debug ("vdec%d, adec%d got base stc %x, base pts %x\n",
			ch_vdec, ch_adec,
			base_stc, base_pts);

	BASE_AVLIPSYNC_LOCK();

	if( (base_stc != 0xFFFFFFFF) && (base_pts != 0xFFFFFFFF) )
	{
		unsigned int  adec_base_stc, adec_base_pts;

		_AV_LipSync_HAL_Adec_GetBase (ch_adec,
				&adec_base_stc, &adec_base_pts);
		log_debug ("adec%d, adec base stc %x, base pts %x\n", ch_adec,
				adec_base_stc, adec_base_pts);

		if( (adec_base_stc == 0xFFFFFFFF) || (adec_base_pts == 0xFFFFFFFF) )
		{
			log_debug ("adec%d adec have no base time. use mine.\n", ch_adec);
			base_stc &= VDEC_PTS_MASK;
			base_pts &= VDEC_PTS_MASK;
			*_base_stc = base_stc;
			*_base_pts = base_pts;
		}
		else
		{
			log_debug ("adec%d adec have base time. use adec.\n", ch_adec);

			*_base_stc = base_stc = adec_base_stc;
			*_base_pts = base_pts = adec_base_pts;
		}
	}

	log_debug ("vdec%d set base stc %x, base pts %x\n", ch_vdec,
			base_stc, base_pts);
	AV_LipSync_HAL_Vdec_SetBase(ch_vdec, base_stc, base_pts);

	BASE_AVLIPSYNC_UNLOCK();
#endif
}

/** @} */

