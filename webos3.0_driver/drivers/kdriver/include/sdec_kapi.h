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
 *  sdec driver
 *
 *  @author	Jihoon Lee ( gaius.lee@lge.com)
 *  @author	Jinhwan Bae ( jinhwan.bae@lge.com) - modifier
 *  @version	1.0
 *  @date		2010-03-30
 *  @note		Additional information.
 */

#ifndef	_SDEC_KAPI_H_
#define	_SDEC_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
/*
 * 2011.08 gaius.lee
 *
 * H/W Bug가 있어서 S/W 로 Workaround 함.
 * PES Packet을 parsing 하는데, 마지막 PES Packet이 Adaptation Field Length가 182 일 때
 * ( 4 + 1 + 182 = 187 이므로 ) data byte는 1 byte인데,
 * H/W에서는 이것을 인식하지 못하여서 모든 PES packet을 넘겨버림.
 *
 * 이를 S/W workaround 하기 위해서
 * Pid filter에 TS_DN을 enable하여 모든 PES Packet을 TS Packet 형태로 저장하고,
 * PES_Thread에서 S/W Parsing 하여 Gathering 및 M/W에 알린다.
 * PES_Thread는 Polling 방식으로 GPB의 read/write pointer를 이용하고,
 * GPB는 X2를 하여 앞 부분은 H/W가 뒷부분은 S/W가 Parsing하여 저장하는 방식을 사용하고 있다.
 */
#define LX_SDEC_USE_KTHREAD_PES 	1
/* 1 - Overwrite enable.  S/W GPB Overflow handling disable
   0 - Overwrite disable. S/W GPB Overflow handling enable  */
#define LX_SDEC_USE_GPB_OW		1

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * SDEC IOCTL Magic number
 */
#define	SDEC_IOC_MAGIC		's'

/**
 * Number of SDEC channel.
 * Current LX has 2 channel number.
 */
#define	LX_SDEC_NUM_OF_CH		2

/**
 * Max number of SDEC packet between user layer and kernel layer.
 */
#define LX_SDEC_MAX_GPB_DATA	256

/**
 * GPB Number used by BDRC operation, regardless of channel, all channel use GPB63
 * Changed also HAL layer definitions
 */
#define LX_SDEC_BDRC_GPB_NO		63

/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
#define SDEC_IOR_INIT						_IOWR(SDEC_IOC_MAGIC, 0, LX_SDEC_CAP_T )
#define SDEC_IOR_GET_STC_PCR				_IOWR(SDEC_IOC_MAGIC, 1, LX_SDEC_GET_STC_PCR_T)
#define SDEC_IOW_SET_MCUMODE				_IOWR(SDEC_IOC_MAGIC, 2, LX_SDEC_SET_MCUMODE_T)
#define SDEC_IOW_SET_CIPHER_ENABLE			_IOWR(SDEC_IOC_MAGIC, 3, LX_SDEC_CIPHER_ENABLE_T)
#define SDEC_IOW_SET_CIPHER_MODE			_IOWR(SDEC_IOC_MAGIC, 4, LX_SDEC_CIPHER_MODE_T)
#define SDEC_IOW_SET_CIPHER_KEY				_IOWR(SDEC_IOC_MAGIC, 5, LX_SDEC_CIPHERKEY_T)
#define SDEC_IOW_SET_PCR_PID				_IOWR(SDEC_IOC_MAGIC, 6, LX_SDEC_PIDFLT_SET_PCRPID_T)
#define SDEC_IOW_SET_PCR_RECOVERY			_IOWR(SDEC_IOC_MAGIC, 7, LX_SDEC_SET_PCR_RECOVERY_T)
#define SDEC_IOR_GET_INPUT_PORT				_IOWR(SDEC_IOC_MAGIC, 8, LX_SDEC_GET_INPUT_T)
#define SDEC_IOW_SET_VDEC_PORT				_IOWR(SDEC_IOC_MAGIC, 9, LX_SDEC_SET_VDEC_PORT_T)

/// PID filter
#define SDEC_IOR_PIDFLT_ALLOC				_IOWR(SDEC_IOC_MAGIC, 10, LX_SDEC_PIDFLT_ALLOC_T)
#define SDEC_IOW_PIDFLT_FREE				_IOWR(SDEC_IOC_MAGIC, 11, LX_SDEC_PIDFLT_FREE_T)
#define SDEC_IOW_PIDFLT_SET_PID				_IOWR(SDEC_IOC_MAGIC, 12, LX_SDEC_PIDFLT_SET_PID_T)
#define SDEC_IOW_PIDFLT_MAP_SECFLT			_IOWR(SDEC_IOC_MAGIC, 13, LX_SDEC_PIDFLT_SELSECFLT_T)
#define SDEC_IOW_PIDFLT_ENABLE				_IOWR(SDEC_IOC_MAGIC, 14, LX_SDEC_PIDFLT_ENABLE_T)
#define SDEC_IOW_PIDFLT_CRC_ENABLE			_IOWR(SDEC_IOC_MAGIC, 15, LX_SDEC_PIDFLT_ENABLE_T)
#define SDEC_IOR_PIDFLT_GET_STATE			_IOWR(SDEC_IOC_MAGIC, 16, LX_SDEC_PIDFLT_STATE_T)
#define SDEC_IOR_PIDFLT_ENABLE_SCMBCHK		_IOWR(SDEC_IOC_MAGIC, 17, LX_SDEC_PIDFLT_ENABLE_T)
#define SDEC_IOW_PIDFLT_ENABLE_DOWNLOAD		_IOWR(SDEC_IOC_MAGIC, 18, LX_SDEC_PIDFLT_ENABLE_T)

/// Section filter
#define SDEC_IOR_SECFLT_ALLOC				_IOWR(SDEC_IOC_MAGIC, 20, LX_SDEC_SECFLT_ALLOC_T)
#define SDEC_IOW_SECFLT_FREE				_IOWR(SDEC_IOC_MAGIC, 21, LX_SDEC_SECFLT_FREE_T)
#define SDEC_IOW_PESFLT_FREE				_IOWR(SDEC_IOC_MAGIC, 22, LX_SDEC_SECFLT_FREE_T)
#define SDEC_IOW_SECFLT_PATTERN				_IOWR(SDEC_IOC_MAGIC, 23, LX_SDEC_SECFLT_PATTERN_T)
#define SDEC_IOW_SECFLT_BUFFER_RESET		_IOWR(SDEC_IOC_MAGIC, 24, LX_SDEC_SECFLT_BUFFER_RESET)
#define SDEC_IOW_SECFLT_BUFFER_SET			_IOWR(SDEC_IOC_MAGIC, 25, LX_SDEC_SECFLT_BUFFER_SET_T)
#define SDEC_IOW_SECFLT_BUFFER_GET_INFO		_IOWR(SDEC_IOC_MAGIC, 26, LX_SDEC_SECFLT_BUFFER_GET_INFO_T)
#define SDEC_IOW_SECFLT_SET_READPTR			_IOWR(SDEC_IOC_MAGIC, 27, LX_SDEC_SECFLT_READPTR_SET_T)
#define SDEC_IOR_SECFLT_GET_STATE			_IOWR(SDEC_IOC_MAGIC, 28, LX_SDEC_SECFLT_STATE_T)
#define SDEC_IOW_SECFLT_ENABLE_SWPARSER		_IOWR(SDEC_IOC_MAGIC, 29, LX_SDEC_SECFLT_ENABLE_SWPARSER_T)
#define SDEC_IOR_SECFLT_GET_AVAIL_NUMBER	_IOWR(SDEC_IOC_MAGIC, 30, LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T)

// Memory Pool
#define SDEC_IOW_MM_GETINFO					_IOWR(SDEC_IOC_MAGIC, 31, LX_SDEC_MEM_INFO_T)
#define SDEC_IOW_MM_FREE					_IOWR(SDEC_IOC_MAGIC, 33, UINT32)
#define SDEC_IOR_MM_ALLOC					_IOWR(SDEC_IOC_MAGIC, 34, LX_SDEC_MEM_INFO_T)
#define SDEC_IOR_MM_GET_STAT				_IOWR(SDEC_IOC_MAGIC, 35, LX_SDEC_MM_STAT_T)
#define SDEC_IOW_MM_CACHE_CTRL				_IOWR(SDEC_IOC_MAGIC, 36, LX_SDEC_CACHE_CTRL_T)

// STC
#define SDEC_IOR_GET_STCCASG				_IOWR(SDEC_IOC_MAGIC, 40, LX_SDEC_GET_STCCASG_T)
#define SDEC_IOR_SET_STCCASG				_IOWR(SDEC_IOC_MAGIC, 41, LX_SDEC_GET_STCCASG_T)
#define SDEC_IOR_GET_STCC_STATUS			_IOWR(SDEC_IOC_MAGIC, 42, LX_SDEC_GET_STCC_STATUS_T)
#define SDEC_IOR_GET_GSTC					_IOWR(SDEC_IOC_MAGIC, 43, LX_SDEC_GET_GSTC_T)
#define SDEC_IOR_SET_GSTC					_IOWR(SDEC_IOC_MAGIC, 44, LX_SDEC_SET_GSTC_T)
#define SDEC_IOR_GET_GSTC32					_IOWR(SDEC_IOC_MAGIC, 45, LX_SDEC_GET_GSTC32_T)
#define SDEC_IOR_GET_STC					_IOWR(SDEC_IOC_MAGIC, 46, LX_SDEC_GET_STC_T)


// Debug
#define SDEC_IOW_ENABLE_LOG					_IOWR(SDEC_IOC_MAGIC, 50, UINT32)
#define SDEC_IOW_SET_FCW					_IOWR(SDEC_IOC_MAGIC, 51, UINT32)
#define SDEC_IOR_GET_REG					_IOWR(SDEC_IOC_MAGIC, 52, LX_SDEC_GET_REG_T)
#define SDEC_IOR_SET_REG					_IOWR(SDEC_IOC_MAGIC, 53, LX_SDEC_SET_REG_T)

#define SDEC_IOW_GET_CIPHER_KEY				_IOWR(SDEC_IOC_MAGIC, 55, LX_SDEC_GET_CIPHERKEY_T)
#define SDEC_IOW_ENABLE_INPUT_PORT			_IOWR(SDEC_IOC_MAGIC, 59, LX_SDEC_ENABLE_INPUT_T)

#define SDEC_IOW_DL_SEL						_IOWR(SDEC_IOC_MAGIC, 60, LX_SDEC_DL_SEL_T)
#define SDEC_IOW_CFG_INPUT_PORT				_IOWR(SDEC_IOC_MAGIC, 61, LX_SDEC_CFG_INPUT_PARAM_T)
#define SDEC_IOW_SEL_INPUT_PORT				_IOWR(SDEC_IOC_MAGIC, 62, LX_SDEC_SEL_INPUT_T)
#define SDEC_IOW_SEL_PAR_INPUT				_IOWR(SDEC_IOC_MAGIC, 63, LX_SDEC_SEL_PAR_INPUT_T)
#define SDEC_IOW_SEL_CI_INPUT				_IOWR(SDEC_IOC_MAGIC, 64, LX_SDEC_SEL_CI_INPUT_T)
#define SDEC_IOW_SEL_INPUT_SRC				_IOWR(SDEC_IOC_MAGIC, 65, LX_SDEC_SEL_INPUT_SRC_T)
#define SDEC_IOW_GET_INPUT_SRC				_IOWR(SDEC_IOC_MAGIC, 66, LX_SDEC_GET_INPUT_SRC_T)


// TS Out Stub
#define SDEC_IOW_SECFLT_GET_WRITEPTR		_IOWR(SDEC_IOC_MAGIC, 69, LX_SDEC_SECFLT_GET_WRITEPTR_T)
#define SDEC_IOW_TSOUT_SET_SRC				_IOWR(SDEC_IOC_MAGIC, 70, LX_SDEC_SERIALTSOUT_SET_SOURCE_T)
#define SDEC_IOW_BDRC_SET_DATA				_IOWR(SDEC_IOC_MAGIC, 71, LX_SDEC_BDRC_T)
#define SDEC_IOW_TSOUT_SET_BUFELEV			_IOWR(SDEC_IOC_MAGIC, 72, LX_SDEC_SERIALTSOUT_SET_BUFELEV_T)
#define SDEC_IOW_CFG_OUTPUT_PORT			_IOWR(SDEC_IOC_MAGIC, 73, LX_SDEC_CFG_OUTPUT_T)
#define SDEC_IOW_SECFLT_SET_WRITEPTR		_IOWR(SDEC_IOC_MAGIC, 74, LX_SDEC_SECFLT_WRITEPTR_SET_T)
#define SDEC_IOW_BDRC_SET_WPTRUPD			_IOWR(SDEC_IOC_MAGIC, 75, LX_SDEC_BDRC_WPTRUPD_T)
#define SDEC_IOW_SECFLT_GET_READPTR			_IOWR(SDEC_IOC_MAGIC, 76, LX_SDEC_SECFLT_GET_READPTR_T)
#define SDEC_IOW_CFG_IO_PORT				_IOWR(SDEC_IOC_MAGIC, 77, LX_SDEC_CFG_IO_T)
#define SDEC_IOW_BDRC_ENABLE				_IOWR(SDEC_IOC_MAGIC, 78, LX_SDEC_BDRC_ENABLE_T)
// End of TS Out Stub

#define SDEC_IOW_GET_CHIPCFG				_IOWR(SDEC_IOC_MAGIC, 79, LX_SDEC_CHIP_CFG_T)

#define SDEC_IOW_DBG_CMD					_IOWR(SDEC_IOC_MAGIC, 80, LX_SDEC_DBG_CMD_T)

#define SDEC_IOW_GET_PAR_INPUT				_IOWR(SDEC_IOC_MAGIC, 81, LX_SDEC_GET_PAR_INPUT_T)
#define SDEC_IOW_GET_CI_INPUT				_IOWR(SDEC_IOC_MAGIC, 82, LX_SDEC_GET_CI_INPUT_T)

// LIVE_HEVC
#define SDEC_IOW_RFOUT_ENABLE				_IOWR(SDEC_IOC_MAGIC, 83, LX_SDEC_RFOUT_ENABLE_T)

#define SDEC_IOR_GPB_GET_BASE_ADDR_OFFSET	_IOWR(SDEC_IOC_MAGIC, 84, LX_SDEC_GPB_BASE_OFFSET_T)

// FOR M14Bx abnormal interrupt
#define SDEC_IOW_SECFLT_DUMMY_BUFFER_SET	_IOWR(SDEC_IOC_MAGIC, 85, LX_SDEC_SECFLT_BUFFER_SET_T)

// FOR Serial Input Clock mode in stpi, stpio
#define SDEC_IOW_SET_SERIAL_INPUT_CLK_MODE	_IOWR(SDEC_IOC_MAGIC, 86, LX_SDEC_SET_SERIALCLKMODE_T)

// FOR TVCT Gathering Mode
#define SDEC_IOW_SET_TVCTMODE				_IOWR(SDEC_IOC_MAGIC, 87, LX_SDEC_SET_TVCTMODE_T)

// SDEC CIPLUS
#define SDEC_IOW_CIPLUS_INIT				_IO(SDEC_IOC_MAGIC, 90)
#define SDEC_IOW_CIPLUS_CONTROL_CHANNEL		_IOWR(SDEC_IOC_MAGIC, 91, LX_SDEC_CIPLUS_CHANNEL_CONTROL_PARAM_T)
#define SDEC_IOW_CIPLUS_CONFIG_CHANNEL		_IOWR(SDEC_IOC_MAGIC, 92, LX_SDEC_CIPLUS_CHANNEL_CONF_PARAM_T)

#define SDEC_IOW_CIPLUS_SET_BUFFER			_IOWR(SDEC_IOC_MAGIC, 96, LX_SDEC_CIPLUS_SET_BUFFER_T)
#define SDEC_IOW_CIPLUS_GET_BUFFER_PTR		_IOWR(SDEC_IOC_MAGIC, 97, LX_SDEC_CIPLUS_BUFFER_PTR_PARAM_T)
#define SDEC_IOW_CIPLUS_SET_BUFFER_PTR		_IOWR(SDEC_IOC_MAGIC, 98, LX_SDEC_CIPLUS_BUFFER_PTR_PARAM_T)

#define SDEC_IOW_CIPLUS_ADD_PID_FILTER		_IOWR(SDEC_IOC_MAGIC, 99, LX_SDEC_CIPLUS_PID_FILTER_T)
#define SDEC_IOW_CIPLUS_REMOVE_PID_FILTER	_IOWR(SDEC_IOC_MAGIC, 100, LX_SDEC_CIPLUS_PID_FILTER_T)
#define SDEC_IOW_CIPLUS_GET_PID_FILTER		_IOWR(SDEC_IOC_MAGIC, 101, LX_SDEC_CIPLUS_PID_FILTER_T)

#define SDEC_IOW_CIPLUS_SET_OUTPUT_CLOCK	_IOWR(SDEC_IOC_MAGIC, 104, LX_SDEC_CIPLUS_OUT_CLOCK_T)

#define SDEC_IOW_CIPLUS_MM_GETINFO			_IOWR(SDEC_IOC_MAGIC, 105, LX_SDEC_MEM_INFO_T)
#define SDEC_IOW_CIPLUS_MM_FREE				_IOWR(SDEC_IOC_MAGIC, 106, UINT32)
#define SDEC_IOR_CIPLUS_MM_ALLOC			_IOWR(SDEC_IOC_MAGIC, 107, LX_SDEC_MEM_INFO_T)
#define SDEC_IOR_CIPLUS_MM_GET_STAT			_IOWR(SDEC_IOC_MAGIC, 108, LX_SDEC_MM_STAT_T)
#define SDEC_IOW_CIPLUS_MM_CACHE_CTRL		_IOWR(SDEC_IOC_MAGIC, 109, LX_SDEC_CACHE_CTRL_T)


#define SDEC_IOC_MAXNR						110
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#if 0 // jinhwan.bae for protecting alignment trap warning
/**
 * Macro to copy from cirular buffer.
 */
#define LX_MEMCPY(dst, src, n, bufStart, bufEnd)	\
{ \
	if( ( (UINT32)bufEnd - (UINT32)src ) >= n ) 				\
	{												\
		memcpy( (void*) dst, (void*) src, n );	\
	}												\
	else											\
	{												\
		memcpy( (void*) dst, (void*) src, ((UINT32)bufEnd - (UINT32)src) ); 								\
		memcpy( (void*) ( dst + ((UINT32)bufEnd - (UINT32)src) ), (void*) bufStart, n - ((UINT32)bufEnd - (UINT32)src) );	\
	}																						\
};


/**
 * Macro to copy to cirular buffer.
 */
#define LX_MEMCPY_TO_CIRCULAR(dst, src, n, bufStart, bufEnd)	\
{ \
	if( ( (UINT32)bufEnd - (UINT32)dst ) >= n ) 				\
	{												\
		memcpy( (void*) dst, (void*) src, n );	\
	}												\
	else											\
	{												\
		memcpy( (void*) dst, (void*) src, ((UINT32)bufEnd - (UINT32)dst) ); 								\
		memcpy( (void*) bufStart, (void*) ( src + ((UINT32)bufEnd - (UINT32)dst) ), n - ((UINT32)bufEnd - (UINT32)dst) );	\
	}																						\
};


/**
 * Macro to copy from circular to cirular buffer.
 */
#define LX_MEMCPY_CIR_TO_CIR(dst, src, n, dstBufStart, dstBufEnd, srcBufStart, srcBufEnd)	\
{ 																\
	if( ( (UINT32)dstBufEnd - (UINT32)dst ) >= n ) 				\
	{															\
		if( ( (UINT32)srcBufEnd - (UINT32)src) >=n )			\
		{														\
			memcpy( (void*) dst, (void*) src, n );				\
		}														\
		else													\
		{														\
			memcpy( (void*) dst, (void*) src, ((UINT32)srcBufEnd - (UINT32)src) ); 														\
			memcpy( (void*) ( dst + ((UINT32)srcBufEnd - (UINT32)src) ), (void*) srcBufStart, n - ((UINT32)srcBufEnd - (UINT32)src) );	\
		}											\
	}												\
	else											\
	{												\
		if( ( (UINT32)srcBufEnd - (UINT32)src) >=n )			\
		{														\
			memcpy( (void*) dst, (void*) src, ((UINT32)dstBufEnd - (UINT32)dst) ); 														\
			memcpy( (void*) dstBufStart, (void*) ( src + ((UINT32)dstBufEnd - (UINT32)dst) ), n - ((UINT32)dstBufEnd - (UINT32)dst) );	\
		}														\
		else													\
		{														\
			if( ( (UINT32)dstBufEnd - (UINT32)dst ) >= ( (UINT32)srcBufEnd - (UINT32)src) )\
			{																				\
				memcpy( (void*) dst, (void*) src, ((UINT32)srcBufEnd - (UINT32)src) ); 		\
				memcpy( (void*) ( dst + ((UINT32)srcBufEnd - (UINT32)src) ), (void*) srcBufStart, ((UINT32)dstBufEnd - (UINT32)dst) + ((UINT32)srcBufEnd - (UINT32)src) );				\
				memcpy( (void*) dstBufStart, (void*) ( srcBufStart + ((UINT32)dstBufEnd - (UINT32)dst) ), n - (((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst)) );	\
			}										\
			else									\
			{										\
				memcpy( (void*) dst, (void*) src, ((UINT32)dstBufEnd - (UINT32)dst) ); 		\
				memcpy( (void*) dstBufStart,  (void*) ( src + ((UINT32)dstBufEnd - (UINT32)dst) ), ((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst) );				\
				memcpy( (void*) ( dstBufStart + ((UINT32)srcBufEnd - (UINT32)src) ) , (void*) srcBufStart, n - (((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst)) );	\
			}													\
		}														\
	}															\
};
#else

/**
 * Macro to memcpy
 */
#define lx_sdec_memcpy(dst, src, n) \
{\
	unsigned char* p_dst = (unsigned char*)dst;\
	const unsigned char* p_src = (const unsigned char*)src;\
	int temp_i; \
	for(temp_i=0; temp_i<n; temp_i++)\
	{\
		*p_dst++ = *p_src++;\
	}\
};

/**
 * Macro to copy from cirular buffer.
 */
#define LX_MEMCPY(dst, src, n, bufStart, bufEnd)	\
{ \
	if( ( (UINT32)bufEnd - (UINT32)src ) >= n ) 				\
	{												\
		lx_sdec_memcpy( (void*) dst, (void*) src, n );	\
	}												\
	else											\
	{												\
		lx_sdec_memcpy( (void*) dst, (void*) src, ((UINT32)bufEnd - (UINT32)src) ); 								\
		lx_sdec_memcpy( (void*) ( dst + ((UINT32)bufEnd - (UINT32)src) ), (void*) bufStart, n - ((UINT32)bufEnd - (UINT32)src) );	\
	}																						\
};


/**
 * Macro to copy to cirular buffer.
 */
#define LX_MEMCPY_TO_CIRCULAR(dst, src, n, bufStart, bufEnd)	\
{ \
	if( ( (UINT32)bufEnd - (UINT32)dst ) >= n ) 				\
	{												\
		lx_sdec_memcpy( (void*) dst, (void*) src, n );	\
	}												\
	else											\
	{												\
		lx_sdec_memcpy( (void*) dst, (void*) src, ((UINT32)bufEnd - (UINT32)dst) ); 								\
		lx_sdec_memcpy( (void*) bufStart, (void*) ( src + ((UINT32)bufEnd - (UINT32)dst) ), n - ((UINT32)bufEnd - (UINT32)dst) );	\
	}																						\
};


/**
 * Macro to copy from circular to cirular buffer.
 */
#define LX_MEMCPY_CIR_TO_CIR(dst, src, n, dstBufStart, dstBufEnd, srcBufStart, srcBufEnd)	\
{ 																\
	if( ( (UINT32)dstBufEnd - (UINT32)dst ) >= n ) 				\
	{															\
		if( ( (UINT32)srcBufEnd - (UINT32)src) >=n )			\
		{														\
			lx_sdec_memcpy( (void*) dst, (void*) src, n );				\
		}														\
		else													\
		{														\
			lx_sdec_memcpy( (void*) dst, (void*) src, ((UINT32)srcBufEnd - (UINT32)src) ); 														\
			lx_sdec_memcpy( (void*) ( dst + ((UINT32)srcBufEnd - (UINT32)src) ), (void*) srcBufStart, n - ((UINT32)srcBufEnd - (UINT32)src) );	\
		}											\
	}												\
	else											\
	{												\
		if( ( (UINT32)srcBufEnd - (UINT32)src) >=n )			\
		{														\
			lx_sdec_memcpy( (void*) dst, (void*) src, ((UINT32)dstBufEnd - (UINT32)dst) ); 														\
			lx_sdec_memcpy( (void*) dstBufStart, (void*) ( src + ((UINT32)dstBufEnd - (UINT32)dst) ), n - ((UINT32)dstBufEnd - (UINT32)dst) );	\
		}														\
		else													\
		{														\
			if( ( (UINT32)dstBufEnd - (UINT32)dst ) >= ( (UINT32)srcBufEnd - (UINT32)src) )\
			{																				\
				lx_sdec_memcpy( (void*) dst, (void*) src, ((UINT32)srcBufEnd - (UINT32)src) ); 		\
				lx_sdec_memcpy( (void*) ( dst + ((UINT32)srcBufEnd - (UINT32)src) ), (void*) srcBufStart, ((UINT32)dstBufEnd - (UINT32)dst) + ((UINT32)srcBufEnd - (UINT32)src) );				\
				lx_sdec_memcpy( (void*) dstBufStart, (void*) ( srcBufStart + ((UINT32)dstBufEnd - (UINT32)dst) ), n - (((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst)) );	\
			}										\
			else									\
			{										\
				lx_sdec_memcpy( (void*) dst, (void*) src, ((UINT32)dstBufEnd - (UINT32)dst) ); 		\
				lx_sdec_memcpy( (void*) dstBufStart,  (void*) ( src + ((UINT32)dstBufEnd - (UINT32)dst) ), ((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst) );				\
				lx_sdec_memcpy( (void*) ( dstBufStart + ((UINT32)srcBufEnd - (UINT32)src) ) , (void*) srcBufStart, n - (((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst)) );	\
			}													\
		}														\
	}															\
};

#endif

/**
 * Macro to copy from circular to cirular buffer in case of RF OUT
 * lx_sdec_memcpy speed cause the problem at HEVC Live RF OUT to D13
 * use memcpy.
 */
#define LX_MEMCPY_CIR_TO_CIR_RFOUT(dst, src, n, dstBufStart, dstBufEnd, srcBufStart, srcBufEnd)	\
{																\
	if( ( (UINT32)dstBufEnd - (UINT32)dst ) >= n )				\
	{															\
		if( ( (UINT32)srcBufEnd - (UINT32)src) >=n )			\
		{														\
			memcpy( (void*) dst, (void*) src, n );				\
		}														\
		else													\
		{														\
			memcpy( (void*) dst, (void*) src, ((UINT32)srcBufEnd - (UINT32)src) );														\
			memcpy( (void*) ( dst + ((UINT32)srcBufEnd - (UINT32)src) ), (void*) srcBufStart, n - ((UINT32)srcBufEnd - (UINT32)src) );	\
		}											\
	}												\
	else											\
	{												\
		if( ( (UINT32)srcBufEnd - (UINT32)src) >=n )			\
		{														\
			memcpy( (void*) dst, (void*) src, ((UINT32)dstBufEnd - (UINT32)dst) );														\
			memcpy( (void*) dstBufStart, (void*) ( src + ((UINT32)dstBufEnd - (UINT32)dst) ), n - ((UINT32)dstBufEnd - (UINT32)dst) );	\
		}														\
		else													\
		{														\
			if( ( (UINT32)dstBufEnd - (UINT32)dst ) >= ( (UINT32)srcBufEnd - (UINT32)src) )\
			{																				\
				memcpy( (void*) dst, (void*) src, ((UINT32)srcBufEnd - (UINT32)src) );		\
				memcpy( (void*) ( dst + ((UINT32)srcBufEnd - (UINT32)src) ), (void*) srcBufStart, ((UINT32)dstBufEnd - (UINT32)dst) + ((UINT32)srcBufEnd - (UINT32)src) );				\
				memcpy( (void*) dstBufStart, (void*) ( srcBufStart + ((UINT32)dstBufEnd - (UINT32)dst) ), n - (((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst)) );	\
			}										\
			else									\
			{										\
				memcpy( (void*) dst, (void*) src, ((UINT32)dstBufEnd - (UINT32)dst) );		\
				memcpy( (void*) dstBufStart,  (void*) ( src + ((UINT32)dstBufEnd - (UINT32)dst) ), ((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst) ); 			\
				memcpy( (void*) ( dstBufStart + ((UINT32)srcBufEnd - (UINT32)src) ) , (void*) srcBufStart, n - (((UINT32)srcBufEnd - (UINT32)src) + ((UINT32)dstBufEnd - (UINT32)dst)) );	\
			}													\
		}														\
	}															\
};

/**
 * Macro to move advance in at cirular buffer.
 */
#define LX_ADVANCE_BUFF(ptr, step, bufStart, bufEnd)	\
{ \
	if( (ptr + step) < bufEnd)					\
		ptr += step;								\
	else if( (ptr + step) == bufEnd)				\
		ptr = bufStart;							\
	else											\
		ptr = bufStart + ( step - (bufEnd - ptr ) );	\
};

/**
 * Macro to cacluate size in cirular buffer.
 */
#define LX_MEMSIZE(sz, rp, wp, bs, be)	\
{ \
	if( ( (UINT32)rp <= (UINT32)wp) ) 				\
	{												\
		sz = (UINT32)wp - (UINT32)rp;	\
	}												\
	else											\
	{												\
		sz = ((UINT32)be - (UINT32)rp) + ((UINT32)wp - (UINT32)bs);	\
	}												\
};

#define PES_HEADER_SIZE	6
#define SEC_HEADER_SIZE	8
#define TS_HEADER_SIZE	(4+1)	/* TS Header + adaptation field length */

#define LX_GET_TS_HEADER(dst, src, bufStart, bufEnd)	LX_MEMCPY(dst, src, TS_HEADER_SIZE, 	bufStart, bufEnd)
#define LX_GET_PES_HEADER(dst, src, bufStart, bufEnd)	LX_MEMCPY(dst, src, PES_HEADER_SIZE, 	bufStart, bufEnd)
#define LX_GET_SEC_HEADER(dst, src, bufStart, bufEnd)	LX_MEMCPY(dst, src, SEC_HEADER_SIZE, 	bufStart, bufEnd)

#define PES_START_CODE_PREFIX	0x000001

/*----------------------------------------------------------------------------------------
	Enum Type Definitions
----------------------------------------------------------------------------------------*/


typedef enum{
	 LX_SDEC_GPB_SIZE_4K 		= 0x001,
	 LX_SDEC_GPB_SIZE_8K 		= 0x002,
	 LX_SDEC_GPB_SIZE_16K 		= 0x004,
	 LX_SDEC_GPB_SIZE_32K 		= 0x008,
	 LX_SDEC_GPB_SIZE_64K 		= 0x010,
	 LX_SDEC_GPB_SIZE_128K 		= 0x020,
	 LX_SDEC_GPB_SIZE_256K 		= 0x040,
	 LX_SDEC_GPB_SIZE_512K 		= 0x080,
	 LX_SDEC_GPB_SIZE_1024K 	= 0x100,
	 LX_SDEC_GPB_SIZE_2048K		= 0x200,
	 LX_SDEC_GPB_SIZE_3072K		= 0x300,
	 LX_SDEC_GPB_SIZE_4096K		= 0x400,
	 LX_SDEC_GPB_SIZE_5120K		= 0x500,
 	 LX_SDEC_GPB_SIZE_6144K 	= 0x600,		// 600000, 6M  LIVE_HEVC
	 LX_SDEC_GPB_SIZE_7520K 	= 0x758			// UD in H13, 7.34M
 }LX_SDEC_GPB_SIZE_T;

/**
 * Return Code
 * return code of kernel driver/adaptation driver.
 */
typedef enum
{
	LX_SDEC_E_OK		 				= 0,			/**< OK */
	LX_SDEC_E_Error					= 1,			/**< Something is wrong */
	LX_SDEC_E_InvalidParam			= 2,			/**< Parameter is not valid*/
	LX_SDEC_E_FullSlot				= 3,			/**< PID/Section Filter slot is full */
	LX_SDEC_E_NotAllocated			= 4				/**< PID/Section Filter slot is not allocated */
} LX_SDEC_Error_T;

typedef enum
{
	LX_SDEC_CH_A		 				= 0,
	LX_SDEC_CH_B,
	LX_SDEC_CH_C,
	LX_SDEC_CH_D,
	LX_SDEC_CH_E,
	LX_SDEC_CH_F,
	LX_SDEC_CH_G,
	LX_SDEC_CH_H,
	LX_SDEC_CH_NUM
}LX_SDEC_CH_T;

#define SDEC_IS_NORMAL_CHANNEL(ch) 	( ((ch == LX_SDEC_CH_A) || (ch == LX_SDEC_CH_B) || (ch == LX_SDEC_CH_E) || (ch == LX_SDEC_CH_F)) ? (1) : (0) )

/**
 * SDEC operation mode.
 * detailed description.
 * @see LX_SDEC_SET_MODE_T
 */
typedef enum
{
	LX_SDEC_MODE_NORMAL				= 0,			/**< NORMAL MODE */
	LX_SDEC_MODE_CHB					= 1,			/**< CHANNEL BROWSER MODE */
	LX_SDEC_MODE_NCHANNEL			= 2,			/**< N-CHANNEL DEMUX (IPTV) MODE */
	LX_SDEC_MODE_FILEPLAY			= 3,			/**< FILE PLAY MODE */
	LX_SDEC_MODE_BBTV					= 4,			/**< BBTV MODE */
	LX_SDEC_MODE_RECORD				= 5,			/**< DVR DOWN (RECORDING) MODE */
	LX_SDEC_MODE_PLAY					= 6,			/**< DVR UP (PLAY) MODE */
	LX_SDEC_MODE_TIMESHIFT			= 7				/**< DVR UP+DOWN (TIME SHIFT) MODE */
} LX_SDEC_MODE_T;

/**
 * SDEC CDIP setting.
 * detailed description.
 * @see LX_SDEC_CDIP_T
 */
typedef enum
{
	LX_SDEC_SET_CDIP_CI				= 0x1,			/**< Parallel from CI */
	LX_SDEC_SET_CDIP_DEMOD			= 0x2,			/**< Parallel from internal DEMOD */
	LX_SDEC_SET_CDIP_EXT_INPUT0		= 0x3,			/**< Serial from external DEMOD */
	LX_SDEC_SET_CDIP_EXT_INPUT1		= 0x4			/**< Serial from external DEMOD */
} LX_SDEC_SET_CDIP_T;

/**
 * SDEC core input port setting.
 * detailed description.
 * @see LX_SDEC_SEL_INPUT_T
 */
typedef enum
{
	LX_SDEC_INPUT_SERIAL0			= 0,			/**< Serial from Serial/Parallel Dual Port -> CDIN0 */
	LX_SDEC_INPUT_SERIAL1			= 1,			/**< Serial from Serial/Parallel Dual Port -> CDINA */
	LX_SDEC_INPUT_SERIAL2			= 2,			/**< Serial from Serial Input Only Port -> CDIN3 */
	LX_SDEC_INPUT_SERIAL3			= 3,			/**< Serial from Serial Input on IO Port -> CDIN4 */
	LX_SDEC_INPUT_PARALLEL0 		= 4,			/**< Parallel from CDIN0 */
	LX_SDEC_INPUT_PARALLEL1 		= 5,			/**< Parallel from CDINA */

	LX_SDEC_INPUT_DVR				= 6,			/**< Input From DVR UP A Channel */
	LX_SDEC_INPUT_DVR0				= LX_SDEC_INPUT_DVR,
	LX_SDEC_INPUT_DVR1				= 7,			/**< Input From DVR UP B Channel */
	LX_SDEC_INPUT_BDRC				= 8,			/**< Input From BDRC from GPB */

	LX_SDEC_INPUT_SERIAL4			= 10,			/**< Serial from Serial/Parallel Dual Port -> CDIN0 */
	LX_SDEC_INPUT_SERIAL5			= 11,			/**< Serial from Serial/Parallel Dual Port -> CDINA */
	LX_SDEC_INPUT_SERIAL6			= 12,			/**< Serial from Serial Input Only Port -> CDIN3 */
	LX_SDEC_INPUT_SERIAL7			= 13,			/**< Serial from Serial Input on IO Port -> CDIN4 */
	LX_SDEC_INPUT_PARALLEL4 		= 14,			/**< Parallel from CDIN0 */
	LX_SDEC_INPUT_PARALLEL5 		= 15,			/**< Parallel from CDINA */

	LX_SDEC_INPUT_INVALID			= 0xFF

} LX_SDEC_INPUT_T;

/**
 * SDEC core input port setting.
 * detailed description.
 * @see LX_SDEC_SEL_PAR_INPUT_T
 */
typedef enum
{
	LX_SDEC_INTERNAL_DEMOD			= 0,			/**< Internal DEMOD */
	LX_SDEC_CI_INPUT				= 1,			/**< External DEMOD */
	LX_SDEC_CI_OUTPUT				= 2,			/**< Input port of CI */
	LX_SDEC_EXTERNAL_DEMOD			= 3,			/**< Output port of CI */
	LX_SDEC_EXTERNAL_DEMOD0 = LX_SDEC_EXTERNAL_DEMOD,
	LX_SDEC_EXTERNAL_DEMOD1			= 4,
	LX_SDEC_INTERNAL_TE				= 5,

} LX_SDEC_PAR_INPUT_T;


/**
 * SDEC Input Source setting.
 * detailed description.
 * @see LX_SDEC_SEL_INPUT_SRC_T
 */
typedef enum
{
	LX_SDEC_SRC_IN_DEMOD0			= 0,
	LX_SDEC_SRC_EXT_INPUT0,
	LX_SDEC_SRC_EXT_INPUT1,
	LX_SDEC_SRC_EXT_INPUT2,
	LX_SDEC_SRC_CI_INPUT,
	LX_SDEC_SRC_CI_OUTPUT,
	LX_SDEC_SRC_TE,
	LX_SDEC_SRC_NONE,

} LX_SDEC_INPUT_SRC_T;



/**
 * SDEC Input Stream Mode setting.
 * Select ATSC/DVB/OpenCell mode depend on stream.
 * @see LX_SDEC_SEL_INPUT_T
 */
typedef enum
{
	LX_SDEC_STREAMMODE_ATSC			= 0x0,			/**< Stream mode is ATSC */
	LX_SDEC_STREAMMODE_DVB			= 0x1,			/**< Stream mode is DVB */
	LX_SDEC_STREAMMODE_OPENCELL		= 0x2,			/**< Stream mode is Opencell */
} LX_SDEC_STREAMMODE_T;

/**
 * Selection of video output .
 * Select SDEC/DVR0/DVR1/CDIC2(CHB).
 * @see LX_SDEC_SET_VDEC_PORT_T
 */
typedef enum
{
	/* Note - From L9 A0, VID_SEL is only effect on vid2. and value is changed */
	//LX_SDEC_VID_SEL_SDEC				= 0x0,			/**< Video output port from SDEC */
	LX_SDEC_VID_SEL_CDIC2			= 0x0,			/**< PES from CDIC2 ( Channel Browser ) */
	LX_SDEC_VID_SEL_DVR0				= 0x1,			/**< PES output port0 from DVR */
	LX_SDEC_VID_SEL_DVR1				= 0x2,			/**< PES output port1 from DVR */
	//LX_SDEC_VID_SEL_CDIC2				= 0x3,			/**< PES from CDIC2 ( Channel Browser ) */
} LX_SDEC_VID_SEL_T;

/**
 * SDEC core destination setting.
 * detailed description.
 * @see LX_SDEC_SEL_DESTINATION_T
 */
typedef enum
{
	LX_SDEC_DEST_INT_AVDEC			= 0,			/**< Internal Audio/Video Decoder */
	LX_SDEC_DEST_EXT_AVDEC			= 1,			/**< External Audio/Video Decoder */
	LX_SDEC_DEST_DVR					= 2,			/**< DVR Module	 */
	LX_SDEC_GPB						= 3,			/**< General Purpose Buffer */
} LX_SDEC_DESTINATION_T;

/**
 * Descrambler path setting.
 * detailed description.
 * @see LX_SDEC_SET_SCMB_PATH_T
 */
typedef enum
{
//	LX_SDEC_CSA_DATAPATH_INPUT_LIVE_IN			= 0x0,		/**< TS stream */
//	LX_SDEC_CSA_DATAPATH_INPUT_FILE_IN			= 0x1,		/**< Memory stream */
//	LX_SDEC_CSA_DATAPATH_INPUT_REC_LIVE			= 0x2,		/**< PVR stream */
//	LX_SDEC_CSA_DATAPATH_OUTPUT_PLAY_LIVE		= 0x8,		/**< to AV decoder */
//	LX_SDEC_CSA_DATAPATH_OUTPUT_PLAY_FILE		= 0x10,		/**< to Memory buffer */
//	LX_SDEC_CSA_DATAPATH_OUTPUT_REC_DESCRMB		= 0x20,		/**< to PVR */
	LX_SDEC_CSA_DATAPATH_BYPASS_ON				= 0x40,		/**< Bypass ON */
	LX_SDEC_CSA_DATAPATH_BYPASS_OFF				= 0x80		/**< Bypass OFF */
} LX_SDEC_CSA_DATAPATH_T;

/**
 * Descrambler mode setting.
 * detailed description.
 * @see LX_SDEC_CIPHER_MODE_T, LX_SDEC_CIPHERKEY_T
 */
typedef enum
{
	LX_SDEC_CIPHER_PROTOCOL_DES			= 0,			/**< DES */
	LX_SDEC_CIPHER_PROTOCOL_AES			= 1,			/**< AES */
	LX_SDEC_CIPHER_PROTOCOL_3DES		= 2,			/**< 3DES */
	LX_SDEC_CIPHER_PROTOCOL_DVB_CSA		= 3,			/**< DVB-CSA */
	LX_SDEC_CIPHER_PROTOCOL_MULTI2 		= 4 			/**< Multi-2 */
} LX_SDEC_CIPHER_PROTOCOL_T;

/**
 * Cipherkey setting.
 * detailed description.
 * @see LX_SDEC_CIPHERKEY_T
 */
typedef enum
{
	LX_SDEC_CIPHER_KEY_EVEN				= 0,			/**< Even key */
	LX_SDEC_CIPHER_KEY_ODD				= 1,			/**< Odd key */
	LX_SDEC_CIPHER_KEY_ODD_IV 			= 2,			/**< Odd Initial Vectore key */
	LX_SDEC_CIPHER_KEY_EVEN_IV			= 3, 			/**< Even Initial Vectore key */
	LX_SDEC_CIPHER_KEY_MULTI2			= 4, 			/**< Multi-2 Key */
	LX_SDEC_CIPHER_KEY_NUM
} LX_SDEC_CIPHER_KEY_T;

/**
 * Command of PCR recovery
 * enable/disable and reset the pcr recovery for some reason.
 * @see LX_SDEC_SET_PCR_RECOVERY_T
 */
typedef enum
{
	LX_SDEC_PCR_CMD_DISABLE		= 0,			/**< Disable PCR Recovery */
	LX_SDEC_PCR_CMD_ENABLE 		= 1,			/**< Enable PCR Recovery */
	LX_SDEC_PCR_CMD_RESET		= 2				/**< Reset */
} LX_SDEC_PCR_CMD_T;

/**
 * PID Filter allocation mode
 * If N Channel Demux Mode, allocated PID filter index should be continously.
 * @see LX_SDEC_PIDFLT_ALLOC_T
 */
typedef enum
{
	LX_SDEC_PIDFLT_MODE_NORMAL		= 0,			/**< Normal Mode */
	LX_SDEC_PIDFLT_MODE_NCHDEMUX	= 1				/**< N Channel Demux Mode */
} LX_SDEC_PIDFLT_MODE_T;

/**
 * PID Filter using PES filter for Setion filter
 * @see LX_SDEC_PIDFLT_SELSECFLT_T
 */
typedef enum
{
	LX_SDEC_PIDFLT_GPB_MODE_SECTION		= 0,			/**< SectionMode */
	LX_SDEC_PIDFLT_GPB_MODE_PES			= 1,			/**< PES Mode */
	LX_SDEC_PIDFLT_GPB_MODE_RAWTS 		= 2 			/**< Raw TS Mode */
} LX_SDEC_PIDFLT_GPB_MODE_T;

/**
 * destination of Packet through PID filter.
 * @see LX_SDEC_PIDFLT_SET_PID_T
 */
typedef enum
{
	LX_SDEC_PIDFLT_DEST_VDEC0		= (0x1 << 0),			/**< 0x01 : VDEC0 */
	LX_SDEC_PIDFLT_DEST_VDEC1		= (0x1 << 1),			/**< 0x02 : VDEC1*/
	LX_SDEC_PIDFLT_DEST_ADEC0		= (0x1 << 2),			/**< 0x04 : ADEC0*/
	LX_SDEC_PIDFLT_DEST_ADEC1		= (0x1 << 3),			/**< 0x08 : ADEC1*/
	LX_SDEC_PIDFLT_DEST_DVR 			= (0x1 << 4),			/**< 0x10 : DVR */
	LX_SDEC_PIDFLT_DEST_PCR 			= (0x1 << 5),			/**< 0x20 : PCR */
	LX_SDEC_PIDFLT_DEST_GPB 			= (0x1 << 6),			/**< 0x40 : Section/PES */
	LX_SDEC_PIDFLT_DEST_RAWTS		= (0x1 << 7),			/**< 0x80 : RAW TS */
	LX_SDEC_PIDFLT_DEST_OTHER		= (0x1 << 8),			/**< Reserved Types for Future */
	LX_SDEC_PIDFLT_DEST_TSO			= (0x1 << 9),			/**< 0x200 : TS OUT */
} LX_SDEC_PIDFLT_DEST_T;



/**
 * GPB Type of PID filter.
 */
typedef enum
{
	LX_SDEC_PIDFLT_TYPE_NONE 		= 0x00, 		/**< NO GPB */
	LX_SDEC_PIDFLT_TYPE_PES			= 0x01,			/**< PES */
	LX_SDEC_PIDFLT_TYPE_SECTION 		= 0x02, 		/**< Section */
	LX_SDEC_PIDFLT_TYPE_RAWTS 		= 0x03, 		/**< Raw TS */
} LX_SDEC_PIDFLT_GPBTYPE_T;


/**
 * PID/section filter status.
 * detailed description.
 * @see LX_SDEC_PIDFLT_STATE_T, LX_SDEC_SECFLT_STATE_T
 */
typedef enum
{
	LX_SDEC_FLTSTATE_FREE			= 0x00000001,	/**< Not allocated */
	LX_SDEC_FLTSTATE_ALLOC			= 0x00000002,	/**< Allocated but not enabled*/
	LX_SDEC_FLTSTATE_ENABLE			= 0x00000004,	/**< Enabled */
	LX_SDEC_FLTSTATE_SCRAMBLED		= 0x00000008,	/**< Scrambled */
	LX_SDEC_FLTSTATE_DATAREADY		= 0x00000010,	/**< data is ready - section filter use */
	LX_SDEC_FLTSTATE_SCRMREADY		= 0x00000020,	/**< scrambled check ready */
	LX_SDEC_FLTSTATE_OVERFLOW		= 0x00001000,	/**< Overflowed */
	LX_SDEC_FLTSTATE_DELETED		= 0x00002000	/**< deleted */
} LX_SDEC_FLTSTATE_T;

/**
 * Set section filter mode.
 * detailed description.
 * @see LX_SDEC_SECFLT_MODE_T
 */
typedef enum
{
	LX_SDEC_FLTMODE_CONTI			= 0x00000001,	/**< Continuous filtering */
	LX_SDEC_FLTMODE_ONESHOT			= 0x00000002,	/**< One packet mode */
	LX_SDEC_FLTMODE_NOCRCCHK		= 0x00000004	/**< no CRC checking */
} LX_SDEC_FLTMODE_T;


/**
 * blocks of sdec registers
 * used for reading registers for debugging
 * @see LX_SDEC_GET_REG_T
 */
typedef enum
{
	LX_SDEC_BLOCK_NONE					= 0,
	LX_SDEC_BLOCK_TOP					= 0x00000001,	/**< System Decoder Top */
	LX_SDEC_BLOCK_IO					= 0x00000002,	/**< System Decoder I/O */
	LX_SDEC_BLOCK_CORE_A				= 0x00000003,	/**< System Decoder Core Channel A */
	LX_SDEC_BLOCK_CORE_B				= 0x00000004,	/**< System Decoder Core Channel B */
	LX_SDEC_BLOCK_CORE1_IO				= 0x00000005,	/**< System Decoder I/O */
	LX_SDEC_BLOCK_CORE1_CORE_A			= 0x00000006,	/**< System Decoder Core Channel A */
	LX_SDEC_BLOCK_CORE1_CORE_B			= 0x00000007,	/**< System Decoder Core Channel B */
	LX_SDEC_BLOCK_MAX,
} LX_SDEC_BLOCK_T;


typedef enum
{
	LX_SDEC_FROM_SDEC	 				= 0x0,	/**< SDEC CH_A, B PIDF_DATA Set */
	LX_SDEC_FROM_SENC					= 0x1,	/**< From SENC */
	LX_SDEC_FROM_CDIC					= 0x2,	/**< From SDEC CH_D, CDIC3 , Full Download for Debug */
	/* ~ H13A0 Added */
	LX_SDEC_FROM_CDIC2					= 0x3,	/**< From CDIC2 (~H13A0), 4 PIDF for SDT  */
} LX_SDEC_DN_SRC_T;


typedef enum{
	LX_SDEC_CDIOP_SERIAL_0		= 0x0,		/* Serial input mode with CLK, SOP, ERR, DATA signals */
	LX_SDEC_CDIOP_SERIAL_1		= 0x1		/* Serial input mode with CLK, SOP, REQ, DATA signals */
 }LX_SDEC_CDIOP_PCONF_T;

typedef enum{
	LX_SDEC_CDIP_SERIAL_0		= 0x0,		/* Serial input mode with CLK, SOP, ERR, DATA signals */
	LX_SDEC_CDIP_SERIAL_1		= 0x1,		/* Serial input mode with CLK, SOP, REQ, DATA signals */
	LX_SDEC_CDIP_PARALLEL_0		= 0x2,		/* Parallel input mode with CLK, SOP, ERR, DATA signals */
	LX_SDEC_CDIP_PARALLEL_1		= 0x3		/* Parallel input mode with CLK, SOP, REQ, DATA signals */
 }LX_SDEC_CDIP_PCONF_T;

typedef enum{
	LX_SDEC_CDIP_47DETECTION		= 0x0,	/* Detects sync bytes(47h). Valid only when input data type is ISO/IEC 13818-1 TS */
	LX_SDEC_CDIP_NEG_SOP			= 0x1,	/* Uses the negative edge of the signal on SOP pin */
	LX_SDEC_CDIP_POS_SOP			= 0x2,	/* Uses the positive edge of the signal on SOP pin */
	LX_SDEC_CDIP_BOTH_SOP		= 0x3		/* Uses both edges of the signal on SOP pin */
 }LX_SDEC_CDIP_SYNC_TYPE_T;

/**
 * Debug Command
 * @see LX_SDEC_DBG_CMD_T
 */
typedef enum
{
	LX_SDEC_CMD_RESET_CDIC				= 0x1,			/**< CDIC RESET */
	LX_SDEC_CMD_RESET_SDMWC,								/**< SDMWC RESET */
	LX_SDEC_CMD_RESET_TE,									/**< TE   RESET */
	LX_SDEC_CMD_RESET_TEA,								/**< TEA  RESET */
	LX_SDEC_CMD_RESET_TEAH,								/**< TEAH  RESET */
	LX_SDEC_CMD_RESET_TEAH_FLUSH, 						/**< TEAH  RESET with Flush */

	LX_SDEC_CMD_EN_CIDC,									/**< Enable/Disable CDIC Reset */
	LX_SDEC_CMD_EN_SDMWC,									/**< Enable/Disable SDMWC Reset */

	LX_SDEC_CMD_SET_FCW									/**< Set FCW */
} LX_SDEC_COMMAND_T;


/**
 * Set io port  mode.
 * detailed description.
 * @see
 */
typedef enum
{
	LX_SDEC_PARALLEL_IO_0			= 0x0,	/**< Parallel IO Port for CI out */
	LX_SDEC_SERIAL_IO_0						/**< Serial IO port stpio */
} LX_SDEC_IO_PORT_T;


typedef enum
{
	LX_SDEC_INPUT_CLOCK_INT = 0x0,
	LX_SDEC_INPUT_CLOCK_EXT
} LX_SDEC_INPUT_CLOCK_MODE_T;


typedef enum
{
	LX_SDEC_SYNC_INT_SYNC_BYTE = 0x0,
	LX_SDEC_SYNC_EXT_SOP_LSB_BIT,
	LX_SDEC_SYNC_EXT_SOP_MSB_BIT,
	LX_SDEC_SYNC_EXT_SOP_ALL_BIT,
} LX_SDEC_SYNC_TYPE_T;


/*----------------------------------------------------------------------------------------
	Stucture Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * Initialize value.
 * initialize SDEC H/W and get H/W capability from kernel driver.
 * @see SDEC_IO_INIT
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT8						ucPidFltNr;			/**< out - number of PID Filter of SDEC */
	UINT8						ucSecFltNr;			/**< out - number of Section Filter of SDEC */
	UINT8						ucFltDepth;			/**< out - Depth of Section Filter of SDEC  */
} LX_SDEC_CAP_T;


/**
 * STC value.
 * @see SDEC_IOR_GET_STC_PCR
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiStc_hi;				/**< out - STC bit 32 -41*/
	UINT32						uiStc_lo;				/**< out - STC bit 0   -31 */
	UINT32						uiPcr_hi;				/**< out - PCR bit 32 -41*/
	UINT32						uiPcr_lo;				/**< out - PCR bit 0   -31 */
} LX_SDEC_GET_STC_PCR_T;


/**
 * Get live stc value
 * @see SDEC_IOR_GET_STC
 */
typedef struct
{
	LX_SDEC_CH_T				lxch;					/**< in  -  SDEC channel A/B */
	UINT32						stc_41_10;				/**< out - STC bit 10 - 41*/
	UINT32						stc_9_0;				/**< out - STC bit  0 -  9*/
} LX_SDEC_GET_STC_T;


/**
 * STCC ASG value.
 * @see SDEC_IOR_GET_STCCASG
 */
typedef struct
{
	UINT8						ui8Aud1;				/**< out - Audio 1 stc channel */
	UINT8						ui8Aud0;				/**< out - Audio 0 stc channel */
	UINT8						ui8Vid1;				/**< out - Video 1 stc channel */
	UINT8						ui8Vid0;				/**< out - Video 0 stc channel */
	UINT8						ui8Main;				/**< out - Main stc channel */
} LX_SDEC_GET_STCCASG_T;


/**
 * STCC REG value.
 * @see SDEC_IOR_GET_STCCREG
 */
typedef struct
{
	BOOLEAN						bEn;
	UINT8						ui8Chan;
	UINT16						ui16Pcr_Pid;
	BOOLEAN						bRd_Mode;
	BOOLEAN						bRate_Ctrl;
	BOOLEAN						bCopy_En;
	BOOLEAN						bLatch_En;
} LX_SDEC_STCC_STATUS_T;


/**
 * STCC REG value.
 * @see SDEC_IOR_GET_STCCREG
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	LX_SDEC_STCC_STATUS_T		stcc_status;			/**< out - STC Status */
} LX_SDEC_GET_STCC_STATUS_T;


/**
 * GSTC value.
 * @see SDEC_IOR_GET_STC_PCR
 */
typedef struct
{
	UINT32						gstc_32_1;			/**< out - GSTC bit 32 -1*/
	UINT32						gstc_0;				/**< out - GSTC bit 0 */
} LX_SDEC_GET_GSTC_T;

/**
 * Get GSTC value. - Extension
 * @see SDEC_IOR_GET_STC_PCR
 */
typedef struct
{
	UINT8						core;				/**< in  -  SDEC core number */
	UINT8						index;				/**< in  -  GSTC index */
	UINT32						gstc_32_1;			/**< out - GSTC bit 32 -1*/
	UINT32						gstc_0;				/**< out - GSTC bit 0 */
} LX_SDEC_GET_GSTC32_T;

/**
 * Set GSTC value.
 * @see SDEC_IOR_GET_STC_PCR
 */
typedef struct
{
	UINT8						core;				/**< in  -  SDEC core number */
	UINT8						index;				/**< in  -  GSTC index */
	UINT32						gstc_base;			/**< in  -  GSTC bit 32 -1, 45KBase, 32 bits */
} LX_SDEC_SET_GSTC_T;


/**
 * SDEC core input port selection.
 * @see SDEC_IOR_GET_INPUT_PORT
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSrc;					/**< Out -  Input source of Channel */
	UINT32						uiNo_wdata;			/**< Out -  No Write data */
	UINT32						uiSync_lost;			/**< Out -  Syncronization lost */
	UINT32						uiSb_dropped;			/**< Out -  Sync byte(s) dropped */
	UINT32						uiCdif_empty;			/**< Out -  Compressed data input FIFO empty*/
	UINT32						uiCdif_full;			/**< Out -  Compressed data input FIFO full */
	UINT32						uiPd_wait;				/**< Out -  pre-decpder wait */
	UINT32						uiCdif_ovflow;			/**< Out -  Compressed data input FIFO overflow */
	UINT32						uiCdif_wpage;			/**< Out -  Compressed data input FIFO write page error */
	UINT32						uiCdif_rpage;			/**< Out -  Compressed data input FIFO read page error */
} LX_SDEC_GET_INPUT_T;


/**
 * SDEC core input port configuration.
 * @see SDEC_IOW_CFG_INPUT_PORT
 */
typedef struct
{
	LX_SDEC_INPUT_T				eInputPort;			/**< in  -  input mode  - Serial/Parallel */

	UINT8						err_act_hi;			/**< in  -  Error pin active high */
	UINT8						clk_act_lo;			/**< in  -	clock pin active low */
	UINT8						val_act_lo; 		/**< in  -	valid pin active low */
	UINT8						req_act_lo; 		/**< in  -	request pin active low */
	UINT8						err_en; 			/**< in  -	Error pin enable  */
	UINT8						val_en;		 		/**< in  -	valid pin enable  */
	UINT8						req_en;				/**< in  -	Request pin enable  */
	UINT8						pconf;				/**< in  -	pin configuration
																CDIP -
																00 : Serial input with ERR 		/ 01 : Serial input with REQ
																10 : Parallel input with ERR	/ 01 : Parallel input with REQ
																CDIOP -
																00 : Serial input with ERR		/ 01 : Serial input with REQ
																10 : Serial output with REQ 	/ 01 : Serial input with VAL
								 					*/
	UINT8						clk_div;			/**< in  -	clock divisor */
	UINT8						sync_type;			/**< in  -	Synchronization type ( only for input ) */
	/* CDIOP Only */
	UINT8						val_sel;			/**< in  -	VAL pin select */
	UINT8						cdin_out;			/**< in  -	input/outmode selection ( 0 : in, 1 : out )*/

	/* CDIP 2nd for Serial Input */
	LX_SDEC_INPUT_CLOCK_MODE_T	clock_mode; 		/**< in  -	Internal/External */
	LX_SDEC_SYNC_TYPE_T			sync_type_2nd;

} LX_SDEC_CFG_INPUT_T;


/**
 * SDEC core input port configuration.
 * @see SDEC_IOW_CFG_INPUT_PORT
 */
typedef struct
{
	UINT8						ui8Ch;				/**< in - channel number */
	LX_SDEC_CFG_INPUT_T			input_param;		/**< in  - input param */
} LX_SDEC_CFG_INPUT_PARAM_T;

/**
 * SDEC core input port selection of parallel / serial.
 * @see SDEC_IOW_SEL_INPUT_PORT
 * @jinhwan.bae 2012. 12. 20 bPortReset is added.
 * @netcast4 EU CH_A,B share LX_PARRALEL_0 , config variation of A affect B
 * @example A is changed from LIVE to DVR or vice versa, LX_P0 do reset, so recording content from B is no signal during A reset
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	LX_SDEC_INPUT_T				eInputPath;			/**< in  -  input Path - Serial/Parallel/DVR */
	BOOLEAN						bPortReset;			/**< in  -  port reset or not */
} LX_SDEC_SEL_INPUT_T;

/**
 * SDEC core input port selection of parallel / serial.
 * @see SDEC_IOW_SEL_PAR_INPUT
 */
typedef struct
{
	LX_SDEC_INPUT_T				eInputPort;			/**< in  -  input port - Parallel0/Parallel1 */
	LX_SDEC_PAR_INPUT_T			eParInput;				/**< in  -  input of Parallel Port */
} LX_SDEC_SEL_PAR_INPUT_T;

/**
 * SDEC core input port selection of parallel / serial.
 * @see SDEC_IOW_SEL_PAR_INPUT
 */
typedef struct
{
	LX_SDEC_INPUT_T				eInputPort;			/**< in  -  input port - Parallel0/Parallel1 */
	LX_SDEC_PAR_INPUT_T			eParInput;				/**< out  -  input of Parallel Port */
} LX_SDEC_GET_PAR_INPUT_T;



typedef struct
{
	LX_SDEC_INPUT_T				input;				/**< in  -  input port - Serailx/Parallelx */
	LX_SDEC_INPUT_SRC_T			source;				/**< in  -  input source - CI/EXT_INPUTx/IN_DEMOD */
} LX_SDEC_SEL_INPUT_SRC_T, LX_SDEC_GET_INPUT_SRC_T;


/**
 * CI input port selection of internal/external
 * @see SDEC_IOW_SEL_CI_INPUT
 */
typedef struct
{
	LX_SDEC_PAR_INPUT_T			eParInput;				/**< in  -  input of Parallel Port */
} LX_SDEC_SEL_CI_INPUT_T;

/**
 * CI input port selection of internal/external
 * @see SDEC_IOW_SEL_CI_INPUT
 */
typedef struct
{
	LX_SDEC_PAR_INPUT_T			eParInput;				/**< out  -  input of Parallel Port */
} LX_SDEC_GET_CI_INPUT_T;

/**
 * SDEC core input port enable/disable
 * @see SDEC_IOW_ENABLE_INPUT_PORT
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B/C */
	LX_SDEC_INPUT_T 			eInputPath;			/**< in  -  input mode of the channel */
	BOOLEAN						bEnable;				/**< in  -  Enable/Disable */
} LX_SDEC_ENABLE_INPUT_T;

/**
 * Setting vid_sel to select video output
 * @see SDEC_IOW_SET_VDEC_PORT
 */
typedef struct
{
	UINT8						ucPort;					/**< In - video output port selection */
	LX_SDEC_VID_SEL_T			eFrom;					/**< In - data is coming from ( SDEC/DVR0/DVR1/CDIC2(CHB) ) */
} LX_SDEC_SET_VDEC_PORT_T;

/**
 * SDEC core input port enable/disable
 * @see SDEC_IOW_ENABLE_INPUT_PORT
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B/C */
	LX_SDEC_DN_SRC_T			eSrc;					/**< in  -  Download output prot from SDEC/SENC */
} LX_SDEC_DL_SEL_T;

/**
 * SDEC Chip Revision Configuration
 * @see SDEC_IOW_GET_CHIPCFG
 */
typedef struct
{
	UINT8				nCore;							/**< out -  number of cores */
	UINT8				nChannel;						/**< out -  number of channels */
	UINT8				nVdecOutPort;					/**< out -  number of vdec output port. */
	UINT8				noPesBug;						/**< out -  pes h/w bug fixed? */
	UINT8				staticGPB;						/**< out -  GPB should be static? */
	UINT8				inTsSelector;					/**< out -	has internal TS source selector ? */
	UINT8				ciPlus;							/**< out -  support ciplus 1.4 */
} LX_SDEC_CHIP_CFG_T;

/**
 * SDEC Chip Revision Configuration
 * @see LX_SDEC_SET_MCUMODE_T
 */
typedef struct
{
	UINT32				ui32param;						/**< in- reserved param */
} LX_SDEC_SET_MCUMODE_T;

/**
 * SDEC TVCT Gathering Mode - NO_DSC
 * @see LX_SDEC_SET_TVCTMODE_T
 */
typedef struct
{
	UINT32				ui32param;						/**< in- tvct mode */
} LX_SDEC_SET_TVCTMODE_T;

/**
 * SDEC Chip Revision Configuration
 * @see LX_SDEC_SET_SERIALCLKMODE_T
 */
typedef struct
{
	UINT32				ui32param;						/**< in- reserved param */
} LX_SDEC_SET_SERIALCLKMODE_T;

/**
 * SDEC Chip Dedug Commands
 * @see SDEC_IOW_DBG_CMD
 */
typedef struct
{
	LX_SDEC_COMMAND_T			eCmd;							/**< in  -  Debug Command */
	UINT32						inParam;							/**< in  -  in parameter */
	UINT32						outParam;							/**< out - out parameter */
} LX_SDEC_DBG_CMD_T;

/*______________________________________________________________________from here cas_____________________________________________________________*/
/**
 * Descrambler mode for the reset.
 * @see LX_SDEC_RES_MODE
 */
typedef enum
{
	LX_SDEC_CLEAR		 				= 0,
	LX_SDEC_CTS 						= 1,
	LX_SDEC_OFB 						= 2,
	LX_SDEC_SCTE52_2008 				= 3,
	LX_SDEC_CTSOC						= 4
}LX_SDEC_RES_MODE_T;


/**
 * Descrambler mode.
 * @see LX_SDEC_RES_MODE
 */
typedef enum
{
	LX_SDEC_ECB		 				= 0,
	LX_SDEC_CBC 						= 1
}LX_SDEC_BLK_MODE_T;

/**
 * Cipherkey setting.
 * @see LX_SDEC_CAS_TYPE
 */
typedef enum
{
	LX_SDEC_BYPASS		 				= 0,
	LX_SDEC_DES 							= 1,
	LX_SDEC_TDES 							= 2,
	LX_SDEC_AES 							= 3,
	LX_SDEC_CSA							= 4,
	LX_SDEC_MULTI2 						= 5,
}LX_SDEC_CAS_TYPE_T;

/**
 * VDEC Codec Type for PDEC initialize
 * @see LX_SDEC_PDEC_INIT
 */
typedef enum
{
	LX_SDEC_CODEC_MPEG2	 				= 0,
	LX_SDEC_CODEC_H264					= 1,
}LX_SDEC_VDEC_CODEC_T;


/**
 * Cipherkey setting.
 * @see LX_SDEC_CIPHER_ENABLE
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	BOOLEAN						bEnable;				/**< in  -  Enable/disable */
} LX_SDEC_CIPHER_ENABLE_T;


/**
 * Cipherkey setting.
 * @see LX_SDEC_PSC_EN
 */
typedef enum
{
	LX_SDEC_DISABLE			= 0,					/**< in  -  Ignorres PSC field */
	LX_SDEC_ENALBE			       = 1					/**< in  -  Checks PSC field */
} LX_SDEC_PSC_EN_T;

/**
 * Cipherkey setting.
 * @see LX_SDEC_KEY_SIZE
 */
typedef enum
{
	LX_SDEC_64				= 0,					/**< in  - 64bit */
	LX_SDEC_128			= 1,					/**< in  -  128bit */
	LX_SDEC_192			= 2,					/**< in  -  192 bit */
	LX_SDEC_256			= 3					/**< in  -  256 bit */
} LX_SDEC_KEY_SIZE_T;


/**
 * Decryption mode.
 * @see SDEC_IOW_SET_CIPHER_MODE
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	LX_SDEC_RES_MODE_T			eResMode;				/**< in  -  Descrambler mode for reset (Clear/CTS/OFB/SCTE52_2008/CTS-OC) */
	LX_SDEC_BLK_MODE_T			eBlkMode;				/**< in  -  Blk Mode (ECB/CBC) */
	LX_SDEC_CAS_TYPE_T			eCasType;				/**< in  -  Cas Type (Bypass/DES/TDES/AES/CSA) */
	LX_SDEC_PSC_EN_T			ePSCEn;					/**< in  -  PES_scrambling_control enable ( Ignore or check ) */
	LX_SDEC_KEY_SIZE_T			eKeySize;				/**< in  -  Key Size in bytes - AES: IVs 128bits + Keys 128bits, DES: Keys 64bits.*/
	//64bit, 01 128bit, 10 192bit, 11 256bit
} LX_SDEC_CIPHER_MODE_T;

/**
 * Cipherkey setting.
 * @see SDEC_IOW_SET_CIPHER_KEY
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	//LX_SDEC_CIPHER_PROTOCOL_T	eProtocol;				/**< in  -  DES/AES */
	LX_SDEC_CIPHER_KEY_T		eKeyMode;				/**< in  -  Even/Odd */
	UINT8						*puiCipherKey;			/**< in  -  Keys */
	//LX_SDEC_KEY_SIZE_T			eKeySize;				/**< in  -  Key Size in bytes - AES: IVs 128bits + Keys 128bits, DES: Keys 64bits.*/
	UINT8 						uiKeySize;	/**< in  -  Key Size in 4bytes aligen*/
	//
} LX_SDEC_CIPHERKEY_T;

/**
 * Get Cipherkey from KMEM
 * @see SDEC_IOW_GET_CIPHER_KEY
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	LX_SDEC_CIPHER_KEY_T		eKeyMode;				/**< in  -  Even/Odd */
	UINT8						uiCipherKey[32];		/**< out -  Keys */
	UINT8 						uiKeySize;				/**< in  -  Key Size in 4bytes aligen*/
} LX_SDEC_GET_CIPHERKEY_T;

/*______________________________________________________________________end of the cas_____________________________________________________________*/

/**
 * Set PCR pid value.
 * @see SDEC_IOW_SET_PCR_PID
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidValue;				/**< in  -  PID value */
	BOOLEAN						bMain;					/**< in  -  is this pcr is used for pcr recovery? */
} LX_SDEC_PIDFLT_SET_PCRPID_T;

/**
 * Enable/Disable PCR Recovery
 * @see SDEC_IOW_SET_PCR_RECOVERY
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	LX_SDEC_PCR_CMD_T			eCmd;					/**< in  -  Enable/disable/reset */
} LX_SDEC_SET_PCR_RECOVERY_T;


/**
 * Information of SDEC Memory
 * @see SDEC_IOW_MM_GETINFO
 */
typedef struct
{
	UINT32						addr;					/**< out -  Base address  */
	UINT32						size;					/**< out -  Size  */
} LX_SDEC_MEM_INFO_T;


/**
 * cache control for SDEC memory
 *
 */
typedef enum
{
	LX_SDEC_CACHE_OP_INVAL = 0x0,
	LX_SDEC_CACHE_OP_CLEAN = 0x1,
} LX_SDEC_CACHE_OP_T;

typedef struct
{
	LX_SDEC_CACHE_OP_T		op;

	void					*paddr;						/**< physical address */
	void					*vaddr;						/**< virtual address returned by mmap */
	UINT32					size;						/**< memory length */
} LX_SDEC_CACHE_CTRL_T;



/**
 * Information of SDEC gpb
 * @see SDEC_IOW_MM_Init
 */
typedef struct
{
	UINT32						uiGpbBase;				/**< out -  Base address of GPB */
	UINT32						uiGpbSize;				/**< out -  Size of GPB */
} LX_SDEC_GPB_INFO_T;


/**
 * brief information about sdec memory usage
 * Application can use this information for tuing memory size or someting like that...
 * @see SDEC_IOR_MM_GET_STAT
 */
typedef struct
{
	void*				mem_base;						/**< out -  base address of surface memory */
	UINT32				mem_length;					/**< out -  length of surface memory */
	UINT32				mem_alloc_size;				/**< out -  byte size of allocated surface */
	UINT32				mem_free_size;					/**< out -  byte size of free memory */

	/* add something...*/
} LX_SDEC_MM_STAT_T;


/**
 * Allocation information
 * @see SDEC_IOR_PIDFLT_ALLOC
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	LX_SDEC_PIDFLT_MODE_T		ePidFltMode;			/**< in  -  PID Filter allocation mode Normal/N Channel Demux */
	UINT32						uiPidValue;			/**< in  -  PID value */
	UINT32						uiPidFltId;			/**< out - PID filter ID */
} LX_SDEC_PIDFLT_ALLOC_T;

/**
 * Remove PID filter.
 * @see SDEC_IOW_PIDFLT_FREE
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
} LX_SDEC_PIDFLT_FREE_T;

/**
 * Set pid value.
 * @see SDEC_IOW_PIDFLT_SET_PID
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	UINT32						uiPidValue;			/**< in  -  PID value */
	UINT32 						uiDest;					/**< in  -  Destination of Packet  using LX_SDEC_PIDFLT_DEST_T*/
	BOOLEAN						bSection;				/**< in - Section or Not */
	BOOLEAN						bEnable;				/**< in - PID Filter Enable or Not */
} LX_SDEC_PIDFLT_SET_PID_T;

/**
 * Get pid value.
 * @see SDEC_IOR_PIDFLT_GET_PID
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	UINT32						uiPidValue;			/**< out - PID value */
} LX_SDEC_PIDFLT_GET_PID_T;

/**
 * Mapping section filter to PID filter.
 * @see SDEC_IOW_PIDFLT_MAP_SECFLT
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	LX_SDEC_PIDFLT_GPB_MODE_T	eGpbMode;				/**< in  -  Section/PES Mode */
} LX_SDEC_PIDFLT_SELSECFLT_T;

/**
 * Enable/disable PID filter.
 * @see SDEC_IOW_PIDFLT_ENABLE
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	BOOLEAN						bPidFltEnable;			/**< in  -  PID filter enable/disable */
} LX_SDEC_PIDFLT_ENABLE_T;


/**
 * Get PID filter status.
 * @see SDEC_IOR_PIDFLT_GET_STATE, LX_SDEC_FLTSTATE_T
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID filter ID */
	UINT32						uiPidValue;			/**< out - setted PID Value of PID Filter */
	BOOLEAN						bDec_en;				/**< out - PID filter enable/disable */
	BOOLEAN						bDl_en;					/**< out - Download enable/disable */
	BOOLEAN						bPload_pes;			/**< out - Payload of PES enable/disable */
	BOOLEAN						bSf_map_en;			/**< out - Section filter map is enabled/disabled */
	UINT32						uiSecf_map[2];			/**< out - connected section filter id if exist */
	UINT32						uiDest;					/**< out - Destination of packets */
	UINT32						uiFltState;			/**< out - Status of PID filter.  @see LX_SDEC_FLTSTATE_T */
	UINT32						uiRegValue;			/**< out - Value of the PID filter */
} LX_SDEC_PIDFLT_STATE_T;

/**
 * Add section filter.
 * @see SDEC_IOR_SECFLT_ALLOC
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< out - section filter ID */
} LX_SDEC_SECFLT_ALLOC_T;

/**
 * Remove section filter.
 * @see SDEC_IOW_SECFLT_FREE
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
} LX_SDEC_SECFLT_FREE_T;


/**
 * Set section filtering pattern.
 * @see SDEC_IOW_SECFLT_PATTERN, LX_SDEC_FLTMODE_T
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	UINT32						uiSecFltMode;			/**< in  -  section filter mode setting mask with enum LX_SDEC_FLTMODE_T */
	UINT8						*pucPattern;			/**< in  -  section filtering pattern */
	UINT8						*pucMask;				/**< in  -  section filtering mask */
	UINT8						*pucNotEqual;			/**< in  -  NotEqual mask. If setted as 1, SDEC exclude pattern value */
} LX_SDEC_SECFLT_PATTERN_T;

/**
 * reset the section buffer read/write pointer to start address.
 * It using for initializing and handling error like buffer overflow and wrong packet processing.
 * @see SDEC_IOW_SECFLT_BUFFER_RESET
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
} LX_SDEC_SECFLT_BUFFER_RESET;

/**
 * Set section filter buffer.
 * @see SDEC_IOW_SECFLT_BUFFER_SET
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	UINT32						uiBufAddress;			/**< in  -  Buffer address */
	UINT32						uiBufferSize;			/**< in  -  Buffer size */
	LX_SDEC_GPB_SIZE_T			eBufferSize;				/**< in  -  Buffer size */
	UINT16						uiFltState;				/**< out - Filter status using @LX_SDEC_FLTSTATE_T*/
} LX_SDEC_SECFLT_BUFFER_SET_T;

/**
 * Set read pointer to gpb.
 * @see SDEC_IOW_SECFLT_SET_READPTR
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	UINT32						uiReadPtr;				/**< in  -  Read pointer address */
} LX_SDEC_SECFLT_READPTR_SET_T;

/**
 * Available section filter number.
 * @see SDEC_IOR_SECFLT_GET_AVAIL_NUMBER
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiAvailFilter;			/**< out - section filter number */
} LX_SDEC_SECFLT_GET_AVAIL_NUMBER_T;

typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiGPBBaseOffset;		/**< out - GPB Base Offset Address */
} LX_SDEC_GPB_BASE_OFFSET_T;


/**
 * Set section filtering pattern.
 * @see SDEC_IOW_VIRT_SECFLT_CFG
 */
typedef struct
{
	UINT32						uiSecFltMode;			/**< in  -  section filter mode setting mask with enum LX_SDEC_FLTMODE_T */
	UINT32						uiPhysAddr;			/**< in  -  Physical Buffer address */
	UINT32						uiBufferSize;			/**< in  -  Buffer size */
	UINT8						*pucPattern;			/**< in  -  section filtering pattern */
	UINT8						*pucMask;				/**< in  -  section filtering mask */
	UINT8						*pucNotEqual;			/**< in  -  NotEqual mask. If setted as 1, SDEC exclude pattern value */
} LX_SDEC_VIRT_SECFLT_CFG_T;

/**
 * Set pdec information
 * @see SDEC_IOW_PDEC_INIT
 */
typedef struct
{
	UINT32						uiCPBAddr;				/**< in  -  Physical Buffer address */
	UINT32						uiCPBSize;				/**< in  -  Buffer size */
	LX_SDEC_VDEC_CODEC_T		eVideoCodec;			/**< in  -  Video Codec Type */
} LX_SDEC_PDEC_INIT;

/**
 * Get pdec status
 * @see SDEC_IOW_PDEC_STATUS
 */
typedef struct
{
	BOOLEAN						bRunning;				/**< out -  pdec is running? */
} LX_SDEC_PDEC_STATUS;

/**
 * Get information of section buffer
 * This structure is used for get serction data from kernel driver.
 * Kernel driver write to buffer and let upper layer know about buffer information.
 * Kernel driver give information about number of section and first packet address and size
 * First 4 byte contains size of packet. So we can get next address.
 * next packet is at bufferrPtr.uiReadAddress + (size of packet )
 * The next of next is same.
 * @see SDEC_IOW_SECFLT_BUFFER_GET_INFO
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	UINT32						uiSectionNr;			/**< out - Number of sectinos which is ready at kernel driver */
	UINT32						uiReadPtr;				/**< out - Read Pointer  */
	UINT32						uiWritePtr; 			/**< out - Write Pointer */
//	UINT32						uiReadPtr[SDEC_MAX_PACKETNUM];		/**< out - Read Pointer  */
//	UINT32						uiWritePtr[SDEC_MAX_PACKETNUM];	/**< out - Write Pointer */
} LX_SDEC_SECFLT_BUFFER_GET_INFO_T;

/**
 * Set read address of section buffer
 * This stucture is used for SDEC_IOW_SECFLT_SET_READPTR
 * @see SDEC_IOW_SECFLT_SET_READPTR
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	UINT32						uiReadAddr;			/**< in  -  Address pointer to read */
} LX_SDEC_SECFLT_SET_READPTR_T;

/**
 * Get status of section filter.
 * @see SDEC_IOR_SECFLT_GET_STATE, LX_SDEC_FLTSTATE_T
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;			/**< in  -  section filter ID */
	UINT16						uiFltState;			/**< out - Filter status using @LX_SDEC_FLTSTATE_T*/
	UINT8						ucSecf_mtype;			/**< out -  section map type */
	UINT32						uiPattern[2];			/**< out -  section filtering pattern */
	UINT32						uiMask[2];				/**< out -  section filtering mask */
	UINT32						uiNotEqual[2];			/**< out -  NotEqual mask. If setted as 1, SDEC exclude pattern value */
	UINT32						uiGpbIdx;				/**< out -  gpb index for output */
	UINT32						uiPidFltId;			/**< out -  Linked PID filter ID */
//	LX_SDEC_FLTSTATE_T			eFltState;
} LX_SDEC_SECFLT_STATE_T;


/**
 * Set SW Parser Enable / Disable
 * This stucture is used for SDEC_IOW_SECFLT_ENABLE_SWPARSER
 * @see SDEC_IOW_SECFLT_ENABLE_SWPARSER
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;				/**< in  -  SDEC channel A/B */
	UINT32						uiPidFltId;			/**< in  -  PID Filter ID */
	BOOLEAN						bEnable;			/**< in  -  Enable , Disable */
} LX_SDEC_SECFLT_ENABLE_SWPARSER_T;


/**
 * Contents of section filter.
 * @see SDEC_IOR_SECFLT_GET_STATE, LX_SDEC_FLTSTATE_T
 */
typedef struct
{
	UINT32						table_id : 8,						/**< Table ID value */
								section_syntax_indicator : 1,	/**< Section Syntax Indicator value */
								table_id_extension : 16,			/**< Table ID Extension value */
								version_number : 5, 				/**< Version Number value */
								current_next_indicator : 1, 		/**< Current Next Indicator value */
								section_number : 8, 				/**< section number value */
								last_section_number : 8,			/**< Last section number value */
								protocol_version : 8;				/**< Protocol Version value */
} LX_SDEC_SECFLT_FILTER_T;

/**
 * Get information of section filter.
 * @see SDEC_IOR_SECFLT_GET_STATE, LX_SDEC_FLTSTATE_T
 */
typedef struct
{
	/* Filtering Data */
	LX_SDEC_SECFLT_FILTER_T		pattern;				/**< out -  Section Filter pattern */
	LX_SDEC_SECFLT_FILTER_T		mask;					/**< out -  Section Filter mask */
	LX_SDEC_SECFLT_FILTER_T		not_equal;				/**< out -  Section Filter Not Equal pattern */
} LX_SDEC_SECFLT_INFO_T;

/**
 * read register with register direct address
 * @see SDEC_IOR_GET_REG
 */
typedef struct
{
	LX_SDEC_BLOCK_T				eSdecBlock;			/**< in  -  Sdec Block */
	UINT32						uiOffset;				/**< in  -  offset of register from block start address */
	UINT32						uiValue;				/**< out -  value of register written by kernel drvier */
} LX_SDEC_GET_REG_T;


/**
 * set register with register direct address
 * @see SDEC_IOR_SET_REG
 */
typedef struct
{
	LX_SDEC_BLOCK_T				eSdecBlock;			/**< in  -  Sdec Block */
	UINT32						uiOffset;				/**< in  -  offset of register from block start address */
	UINT32						uiValue;				/**< in  -  value of register */
} LX_SDEC_SET_REG_T;


typedef struct
{
	/**
	SDEC channel A/B
	*/
	UINT32	channel : 8,

	/**
	status of section filter buffer(Ready/Overflow)
	using @LX_SDEC_FLTSTATE_T
	*/
			status : 16,
//	LX_SDEC_FLTSTATE_T eFltState;

	/**
	notify data payload.
	which shall be filled from kernel driver only contains ready/overflow section filter index
	*/
			index : 8;
	UINT32	readPtr;
	UINT32	writePtr;
	UINT64	msec;
} LX_SDEC_NOTIFY_PARAM_T, *P_LX_SDEC_NOTIFY_PARAM_T;

/*----------------------------------------------------------------------------------------
	TS Out Stub
----------------------------------------------------------------------------------------*/
/**
 * CDOC Output Src
 * @see
 */
typedef enum
{
	LX_SDEC_SO_PES_FROM_SDEC				= 0,
	LX_SDEC_SO_TS_FROM_SDEC 				= 1,
	LX_SDEC_SO_NONE							= 2,
	LX_SDEC_SO_FROM_BDRC					= 3,
}LX_SDEC_SERIALTSOUT_SRC_T;


/**
 * @see LX_SDEC_TSOUT_SET_SOURCE_T
 */
typedef struct
{
	LX_SDEC_CH_T 							eCh;			/**< in  -  SDEC channel A/B */
	LX_SDEC_SERIALTSOUT_SRC_T				src;			/**< in- output src */
} LX_SDEC_SERIALTSOUT_SET_SOURCE_T;



/**
 * @see LX_SDEC_BDRC_T
 */
typedef struct
{
	UINT8				bdrc_idx;					/**< in- bdrc index*/
	UINT8				en;							/**< in- output src */
	UINT8				rst;						/**< in- output src */
	UINT8				wptr_auto;					/**< in- output src */
	UINT8				gpb_chan;					/**< in- output src */
	UINT8				gpb_idx;					/**< in- output src */
	UINT8				q_len;						/**< in- output src */
	UINT8				dtype;						/**< in- output src */
} LX_SDEC_BDRC_T;


/**
 * @see LX_SDEC_SERIALTSOUT_SET_BUFELEV_T
 */
typedef struct
{
	UINT32				buf_e_lev;						/**< in- output src */
} LX_SDEC_SERIALTSOUT_SET_BUFELEV_T;

/**
 * SDEC core input port configuration.
 * @see SDEC_IOW_CFG_INPUT_PORT
 */
typedef struct
{
	UINT32						/* CDIP & CDIOP */
								clk_act_lo	: 1, 		/**< in  -	clock pin active low */
								val_act_lo	: 1, 		/**< in  -	valid pin active low */
								req_act_lo	: 1, 		/**< in  -	request pin active low */
								val_en		: 1, 		/**< in  -	valid pin enable  */
								req_en		: 1,		/**< in  -	Request pin enable  */
								val_sel		: 2,		/**< in  -	VAL pin select */
								cdin_out	: 1,		/**< in  -	input/outmode selection ( 0 : in, 1 : out )*/
								pconf		: 1,
								clk_div		: 7,		/**< in  -	clock divisor */
								sync_type	: 2;
} LX_SDEC_CFG_OUTPUT_T;

/**
 * Set read pointer to gpb.
 * @see SDEC_IOW_SECFLT_SET_READPTR
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;				/**< in  -  section filter ID */
	UINT32						uiWritePtr;				/**< in  -  Read pointer address */
} LX_SDEC_SECFLT_WRITEPTR_SET_T;


/**
 * Set read pointer to gpb.
 * @see SDEC_IOW_SECFLT_SET_READPTR
 */
typedef struct
{
	UINT8						idx;					/**< in  -  SDEC channel A/B */
} LX_SDEC_BDRC_WPTRUPD_T;


/**
 * Set read address of section buffer
 * This stucture is used for SDEC_IOW_SECFLT_SET_READPTR
 * @see SDEC_IOW_SECFLT_SET_READPTR
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;					/**< in  -  SDEC channel A/B */
	UINT32						uiSecFltId;				/**< in  -  section filter ID */
	UINT32						uiValue;				/**< in  -  Address pointer to read */
} LX_SDEC_SECFLT_GET_READPTR_T, LX_SDEC_SECFLT_GET_WRITEPTR_T;


/**
 * Set stpio as in or out
 * @see
 */
typedef struct
{
	LX_SDEC_IO_PORT_T			ePort;					/**< in  - serial or parallel io port */
	UINT8						en_out;					/**< in  - output or input */
} LX_SDEC_CFG_IO_T;


/**
 * Set stpio as in or out
 * @see
 */
typedef struct
{
	UINT8						bdrc_idx;			/**< in  - bdrc index */
	UINT8						en;					/**< in  - enable or disable */
} LX_SDEC_BDRC_ENABLE_T;

/*----------------------------------------------------------------------------------------
	End of TS Out Stub
----------------------------------------------------------------------------------------*/

// LIVE_HEVC
/**
 * Set stpio as in or out
 * @see
 */
typedef struct
{
	LX_SDEC_CH_T				eCh;				/**< in  -  SDEC channel A/B */
	UINT8						bEnable;			/**< in  - enable or disable */
} LX_SDEC_RFOUT_ENABLE_T;




/*----------------------------------------------------------------------------------------
	SDEC CIPLUS
----------------------------------------------------------------------------------------*/

#define LX_SDEC_CIPLUS_NUM_PIDF		32

typedef enum
{
	LX_SDEC_CIPLUS_SRC_DEMOD0	= 0,
	LX_SDEC_CIPLUS_SRC_DEMOD1	= 1,
	LX_SDEC_CIPLUS_SRC_DEMOD2	= 2,
	LX_SDEC_CIPLUS_SRC_DEMOD3	= 3,
	LX_SDEC_CIPLUS_SRC_DDR		= 4,
} LX_SDEC_CIPLUS_TS_SOURCE_T;

typedef enum
{
	LX_SDEC_CIPLUS_SYNC_TYPE_SOP		= 0,
	LX_SDEC_CIPLUS_SYNC_TYPE_SYNC_BYTE	= 1,
} LX_SDEC_CIPLUS_SYNC_TYPE_T;

typedef enum
{
	LX_SDEC_CIPLUS_BUFFER_TYPE_INPUT	= 0,
	LX_SDEC_CIPLUS_BUFFER_TYPE_OUTPUT	= 1,
} LX_SDEC_CIPLUS_BUFFER_TYPE_T;

typedef enum
{
	LX_SDEC_CIPLUS_OUT_CLOCK_DEFAULT = 0,
	LX_SDEC_CIPLUS_OUT_CLOCK_LOW,
	LX_SDEC_CIPLUS_OUT_CLOCK_HIGH

} LX_SDEC_CIPLUS_OUT_CLOCK_T;

typedef enum
{
	LX_SDEC_CIPLUS_CHANNEL_ALLOC = 0,
	LX_SDEC_CIPLUS_CHANNEL_OPEN,
	LX_SDEC_CIPLUS_CHANNEL_CLOSE,
	LX_SDEC_CIPLUS_CHANNEL_ENABLE,
	LX_SDEC_CIPLUS_CHANNEL_DISABLE,

} LX_SDEC_CIPLUS_CHANNEL_CONTROL_T;


typedef struct
{
	LX_SDEC_CIPLUS_TS_SOURCE_T		src;						/**< in - channel source */
	LX_SDEC_CIPLUS_SYNC_TYPE_T		sync_type;					/**< in - sync type(SOP or SYNC_BYTE) */
	UINT8							bypass;						/**< in - enable bypass */
	UINT8							transport_error_indicator;	/**< in - check transport error indicator */
	UINT8							local_ts_id_en;				/**< in - enable local TS ID */
	UINT8							local_ts_id;				/**< in - local TS ID */
} LX_SDEC_CIPLUS_CHANNEL_CONF_T;

typedef struct
{
	UINT8							idx;						/**< in - channel index */
	LX_SDEC_CIPLUS_CHANNEL_CONF_T	conf;
} LX_SDEC_CIPLUS_CHANNEL_CONF_PARAM_T;

typedef struct
{
	UINT8								idx;						/**< in - channel index */
	LX_SDEC_CIPLUS_CHANNEL_CONTROL_T	control;
} LX_SDEC_CIPLUS_CHANNEL_CONTROL_PARAM_T;

typedef struct
{
	UINT8							idx;							/**< in - channel index */
	UINT16							pid[LX_SDEC_CIPLUS_NUM_PIDF];	/**< in/out - pid value */
	UINT32							npid;							/**< in/out - num of pid */
} LX_SDEC_CIPLUS_PID_FILTER_T;


typedef struct
{
	LX_SDEC_CIPLUS_BUFFER_TYPE_T	type;
	UINT8							idx;						/**< in - channel index */
	UINT32							addr;						/**< in - buffer address(physical) */
	UINT32							size;						/**< in - buffer size */
} LX_SDEC_CIPLUS_SET_BUFFER_T;

typedef struct
{
	LX_SDEC_CIPLUS_BUFFER_TYPE_T	type;
	UINT8							idx;						/**< in - channel index */
	UINT32							wptr;						/**< out - write pointer */
	UINT32							rptr;						/**< out - read pointer */
} LX_SDEC_CIPLUS_BUFFER_PTR_PARAM_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _SDEC_DRV_H_ */

/** @} */

