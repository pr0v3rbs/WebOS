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

#ifndef __ADEC_MEDIA_TYPE_H__
#define __ADEC_MEDIA_TYPE_H__


typedef enum _ADEC_MEDIA_TYPE
{
	ADEC_MEDIA_UNKNOWN = 0,
	ADEC_MEDIA_PCM,
	ADEC_MEDIA_MS10_DDC,
	ADEC_MEDIA_MS10_DDT,
	ADEC_MEDIA_DRA,
	ADEC_MEDIA_MP3,
	ADEC_MEDIA_WMA_PRO,
	ADEC_MEDIA_DTS_M6,
	ADEC_MEDIA_OGG,
	ADEC_MEDIA_AMR_WB,
	ADEC_MEDIA_AMR_NB,
	ADEC_MEDIA_ADPCM,
	ADEC_MEDIA_RA8,
	ADEC_MEDIA_FLAC,
	ADEC_MEDIA_AAC_ENC,
	ADEC_MEDIA_AC3_ENC,
#ifdef IC_CERTIFICATION
	ADEC_MEDIA_DTS_ENC,
#endif // #ifdef IC_CERTIFICATION
	ADEC_MEDIA_MAX,
}ADEC_MEDIA_TYPE;

typedef enum _ADEC_SPDIF_TYPE
{
	ADEC_SPDIF_TYPE_PCM = 0,
	ADEC_SPDIF_TYPE_ES,
	ADEC_SPDIF_TYPE_DTSCD,
} ADEC_SPDIF_TYPE;

typedef struct _ADEC_MEDIA_INFO
{
	ADEC_MEDIA_TYPE		media_type;
	unsigned int		avg_bit_rate;
	unsigned int		sample_rate;
	unsigned int		num_of_channel;
	unsigned int		bit_per_sample;
}ADEC_MEDIA_INFO;

#endif
