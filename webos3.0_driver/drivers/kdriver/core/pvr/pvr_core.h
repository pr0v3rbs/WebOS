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
 *  driver interface header for pvr device. ( used only within kdriver )
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  @author		kyungbin.pak
 *  @version	1.0
 *  @date		2009.11.15
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef _PVR_CORE_H_
#define _PVR_CORE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"
#include "pvr_cfg.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define USE_NEW_PIE_EXTRACTION	1


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PVR_DN_BUF_NUM_LIM			0x1F
#define PVR_DN_BUF_NUM_MAX			(PVR_DN_BUF_NUM_LIM + 1)
#define PVR_DN_UNIT_BUF_PKT_CNT		0x100	/* 48KByte(256 * 192). PKT_CNT_LIM in DN_BUF_LIM */

#define PVR_DN_BUF_WARNIG_PERCENT	20		/* Percentage */
#define PVR_DN_BUF_WARNIG_INTERVAL	3000	/* 3 seconds */

#define PVR_DN_MAX_MEESAGE_COUNT	10
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/


typedef struct
{
	UINT32		base;
	UINT32		end;
	UINT32		size;
	UINT32		rptr;
	UINT32		wptr;
} PVR_BUF_INFO_T;

typedef struct
{
	PVR_BUF_INFO_T		dn;
	PVR_BUF_INFO_T		up;

} PVR_BUF_T;



typedef union  tagPieIndexTable { // 0x2780
	UINT32	ui32Val;
	struct {
		UINT32 byte_info       : 8;   // 31:24   24
		UINT32 sdet            : 1;   // 23      23
		UINT32 idet            : 1;   // 22      22
		UINT32 pdet            : 1;   // 21      21
		UINT32 bdet            : 1;   // 20      20
		UINT32 maxp            : 1;   // 19      19
		UINT32 buf_num         : 5;   // 18:14   14
		UINT32 pack_cnt        : 14;  // 13:0    0
	}be;
	struct {
		UINT32 pack_cnt        : 14;  // 13:0    0
		UINT32 buf_num         : 5;   // 18:14   14
		UINT32 maxp            : 1;   // 19      19
		UINT32 bdet            : 1;   // 20      20
		UINT32 pdet            : 1;   // 21      21
		UINT32 idet            : 1;   // 22      22
		UINT32 sdet            : 1;   // 23      23
		UINT32 byte_info       : 8;   // 31:24   24
	}le;
} PIE_IND_T;


typedef struct tagPvrCurrentState
{
	LX_PVR_DN_STATE_T	eDnState;
	LX_PVR_UP_STATE_T	eUpState;
	LX_PVR_PIE_STATE_T	ePieState;

	LX_PVR_UP_MODE_T	up_mode;
	UINT8				use_timestamp;
}DVR_STATE_T;

typedef struct
{
	LX_PVR_BUF_STAT_T	dn;
	LX_PVR_BUF_STAT_T	up;
} DVR_BUFFER_STATE_T;



typedef struct
{
	UINT8	up_overlap;
} PVR_ERROR_STATUS_T;

typedef struct
{
    UINT32	dn_unitbuf;
    UINT32	dn_overflow;

    UINT32	up_overlap;
    UINT32	up_almost_empty;
	UINT32	up_sync_err;
} PVR_STATS_T;


#define PVR_PIE_IND_SCD_MASK	(0xF << 20)
typedef union
{
	UINT32	val;
	struct
	{
		UINT32 pack_cnt 	   : 14;  // 13:0	 0
		UINT32 buf_num		   : 5;   // 18:14	 14
		UINT32 maxp 		   : 1;   // 19 	 19
		UINT32 bdet 		   : 1;   // 20 	 20
		UINT32 pdet 		   : 1;   // 21 	 21
		UINT32 idet 		   : 1;   // 22 	 22
		UINT32 sdet 		   : 1;   // 23 	 23
		UINT32 byte_info	   : 8;   // 31:24	 24
	} mpeg2;
	struct
	{
		UINT32 pack_cnt 	   : 14;  // 13:0	 0
		UINT32 buf_num		   : 5;   // 18:14	 14
		UINT32 maxp 		   : 1;   // 19 	 19
		UINT32 scd0 		   : 1;   // 20 	 20
		UINT32 scd1 		   : 1;   // 21 	 21
		UINT32 scd2 		   : 1;   // 22 	 22
		UINT32 scd3 		   : 1;   // 23 	 23
		UINT32 byte_info	   : 8;   // 31:24	 24
	} gscd;
} PVR_PIE_IND_T;

typedef struct
{
	UINT16		idx;
	UINT16		count;
} PVR_PIE_ENTRY_T;

typedef struct
{
	UINT32			buf_num;
	PVR_PIE_IND_T	index;
} PVR_PIE_DATA_T;

typedef struct
{
	LX_PVR_PIE_TYPE_T	type;

	PVR_PIE_ENTRY_T		*entry;
	UINT32				entry_idx;
	UINT32				entry_num;

	PVR_PIE_DATA_T		*data;
	UINT32				wr_idx;
	UINT32				rd_idx;
	UINT32				max_data;
} PVR_PIE_TABLE_T;


#define PIE_MAX_ENTRIES_LOCAL	1024/* 256 */


typedef struct
{
	UINT32		size;			/* Unit buffer size */
	UINT32		pkt_cnt;		/* Number of 192bytes packets */

//	UINT32		base;			/* Base buffer number. Gets the unit buffer number after adding the H/W BUF_NUM in PIE */
	UINT32		curr;			/* Current unit buffer number */
	UINT32		total;			/* Total number of unit buffer */

	UINT32		buf_num;		/* Current downloading H/W unit buffer number. range from 0 to BUF_NUM_LIM */
} PVR_UNIT_BUF_INFO_T;


typedef struct
{


	UINT32	warn_bufsize;		/* Show warning message if availabe buffer is under this size */
	UINT32	warn_interval;		/* ms unit - do not display message again under this time after showing message*/
	UINT32	warn_time;			/* last displayed time */

	UINT32	errmsg_enable;
} PVR_DN_CTRL_T;


typedef struct
{
	LX_PVR_CH_T 			ch;

	PVR_DN_CTRL_T			dn;

	struct mutex			up_lock;

#ifndef USE_NEW_PIE_EXTRACTION
	/* Currently this entry is kept static inside the module structure,
	 * later to be made dynamic */
	PIE_IND_T	astPieTable[PIE_MAX_ENTRIES_LOCAL];
	UINT32		astBufTable[PIE_MAX_ENTRIES_LOCAL];
	UINT32		ui32PieIndex;
	UINT32		ui32PieRdIndex;
#endif

	UINT8		ui8UploadFirstTime;
	UINT8		ui8PacketLen;	// 188/192 bytes


	PVR_UNIT_BUF_INFO_T		unit_buf;

	PVR_BUF_T				buf;

	PVR_PIE_TABLE_T			pie_table;

	DVR_BUFFER_STATE_T		buf_state;
	DVR_STATE_T				state;
	PVR_ERROR_STATUS_T		err_stat;
	PVR_STATS_T				stats;
} PVR_CTRL_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

PVR_CTRL_T* PVR_GetCtrl(LX_PVR_CH_T ch);

int		PVR_InitDevice(void);
void	PVR_CleanupDevice(void);
int		PVR_InitModule(PVR_CTRL_T *ctrl);
int		PVR_Terminate( void );
int		PVR_GetHwInfo(LX_PVR_HW_INFO_T *info);
int		PVR_StartDownload(PVR_CTRL_T *ctrl);
int		PVR_StopDownload(PVR_CTRL_T *ctrl);
int		PVR_StopDownloadSDT(PVR_CTRL_T *ctrl);
int		PVR_SetDownloadBuffer(PVR_CTRL_T *ctrl, UINT32 uiBufAddr, UINT32 uiBufSize);
int		PVR_SetDownloadReadAddr(PVR_CTRL_T *ctrl, UINT32 ui32ReadPtr);
int		PVR_GetDownloadWritePtr(PVR_CTRL_T *ctrl, UINT32 *ptr);
int		PVR_SetUploadbuffer (PVR_CTRL_T *ctrl, UINT32 uiBufAddr, UINT32 uiBufSize);
int		PVR_StartUpload (PVR_CTRL_T *ctrl, UINT8 ui8PacketLen);
int		PVR_StopUpload ( PVR_CTRL_T *ctrl);
int		PVR_UpUploadbuffer (PVR_CTRL_T *ctrl, UINT32 ui32WritePtr);
int		PVR_SetUploadMode(PVR_CTRL_T *ctrl, LX_PVR_UP_MODE_T eMode);
int		PVR_SetUploadSpeed(PVR_CTRL_T *ctrl, UINT32 speed);
int		PVR_GetUploadReadAddr (PVR_CTRL_T *ctrl, UINT32 *pui32WritePtr, UINT32 *pui32ReadPtr);
int		PVR_PauseUpload (PVR_CTRL_T *ctrl);
int		PVR_ResumeUpload (PVR_CTRL_T *ctrl);

int		PVR_SetDownloadVPID(PVR_CTRL_T *ctrl, UINT32 pid);
int		PVR_SetDownloadUnitBufPktCnt(PVR_CTRL_T *ctrl, UINT32 pktCnt);
int		PVR_RestartUpload (PVR_CTRL_T *ctrl, UINT32 uiBufAddr, UINT32 uiBufSize);
int		PVR_ReSetUpload (PVR_CTRL_T *ctrl);
int		PVR_GetMemoryInfo ( LX_PVR_GPB_INFO_T	*stpLXPvrGPBInfo);
int		PVR_GetRegister(LX_PVR_GET_REG_T *reg);
int		PVR_SetRegister(LX_PVR_GET_REG_T *reg);
ULONG	PVR_GetRegBaseAddr(LX_PVR_BLOCK_T block);
UINT32	PVR_GetRegSize(LX_PVR_BLOCK_T block);

int		PVR_GetStatus(PVR_CTRL_T *ctrl, LX_PVR_STATUS_T *status);



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PVR_CORE_H */

/** @} */
