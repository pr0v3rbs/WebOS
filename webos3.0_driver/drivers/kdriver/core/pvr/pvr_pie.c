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
 *  main driver implementation for pvr device.
 *	pvr device will teach you how to make device driver with new platform.
 *
 *  @author		Kyungseok Hyun(ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2014.07.18
 *  note		Additional information.
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "pvr_impl.h"
#include "pvr_drv.h"
#include "pvr_core.h"
#include "pvr_reg.h"
#include "pvr_drv.h"
#include "pvr_reg_ctrl.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PVR_HEVC_INDEX_TYPE		1


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PVR_PIE_TRACE_PARAM_ENTER(ctrl,fmt,args...) \
			PVR_PRINT_TRACE("<== PVR_PIE(CH_%c) : %s("fmt")", PVR_CH_NAME(ctrl->ch), __FUNCTION__, ##args)
#define PVR_PIE_TRACE_PARAM_EXIT(ctrl,fmt,args...) \
			PVR_PRINT_TRACE("==> PVR_PIE(CH_%c) : %s("fmt")", PVR_CH_NAME(ctrl->ch), __FUNCTION__, ##args)


#define IS_VALID_DN_BUF_ADDR(ctrl, addr)	((addr) >= ctrl->buf.dn.base && (addr) < ctrl->buf.dn.end)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	start;
	UINT32	end;
	UINT32	size;
} PVR_PIE_BUF_INFO_T;

typedef void (*GET_INDEX_FUNC_T)(PVR_CTRL_T*, LX_PVR_INDEX_T*, PVR_PIE_IND_T*);

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

static void ResetTableIndex(PVR_PIE_TABLE_T *table)
{
	table->wr_idx = 0;
	table->rd_idx = 0;
	memset(table->entry, 0, table->entry_num * sizeof(PVR_PIE_ENTRY_T));
}

int PVR_PIE_InitTable(PVR_CTRL_T *ctrl)
{
	UINT32 max_data;
	PVR_PIE_TABLE_T *table = &ctrl->pie_table;


#define PIE_TABLE_UNIT_SIZE			(192*1024)
#define PIE_TABLE_DATA_PER_UNIT		30
#define PIE_TABLE_MIN_UNIT			10

	/* 1000 data used in 6.751384MBytes */
	if(ctrl->buf.dn.size < PIE_TABLE_UNIT_SIZE*PIE_TABLE_MIN_UNIT)	/* 1.875MB */
	{
		max_data = PIE_TABLE_MIN_UNIT * PIE_TABLE_DATA_PER_UNIT;	/* 300 */
	}
	else
	{
		max_data = (ctrl->buf.dn.size/PIE_TABLE_UNIT_SIZE) * PIE_TABLE_DATA_PER_UNIT;
	}
	PVR_PRINT_PIE("CH:%d - Max Data:%d", ctrl->ch, max_data);

	/* Initialize PIE Data Buffer */
	if(max_data != table->max_data)
	{
		if(table->data)
			OS_KFree(table->data);

		table->max_data = max_data;
		table->data = (PVR_PIE_DATA_T*)OS_KMalloc(max_data * sizeof(PVR_PIE_DATA_T));
	}

	/* Initialize PIE Table */
	if(table->entry_num != ctrl->unit_buf.total)
	{
		if(table->entry)
			OS_KFree(table->entry);

		table->entry_num = ctrl->unit_buf.total;
		table->entry = (PVR_PIE_ENTRY_T*)OS_KMalloc(table->entry_num * sizeof(PVR_PIE_ENTRY_T));
	}

	table->entry_idx = 0;
	ResetTableIndex(table);

	PVR_PIE_TRACE_PARAM_EXIT(ctrl, "max_data=%d", max_data);

	return 0;
}

int PVR_PIE_SetType(PVR_CTRL_T *ctrl, LX_PVR_PIE_TYPE_T type)
{
	int ret = RET_OK;

	PVR_PIE_TRACE_PARAM_ENTER(ctrl, "type=%d", type);

	DVR_PIE_ResetBlock(ctrl->ch);
	switch(type)
	{
		case LX_PVR_PIE_TYPE_MPEG2TS:
			/* Disable GSCD, Legacy mode MP2 PIE enabled */
			DVR_PIE_EnableSCDReg(ctrl->ch, FALSE);
			break;

		case LX_PVR_PIE_TYPE_H264TS:
		case LX_PVR_PIE_TYPE_OTHERS:
			DVR_PIE_EnableSCDReg(ctrl->ch, TRUE);
			/* MPEG-4/H.264 */
			DVR_PIE_ConfigureSCD(ctrl->ch, 0, 0xFFFFFF1F, 0x00000107, TRUE ); //SPS
			DVR_PIE_ConfigureSCD(ctrl->ch, 1, 0xFFFFFF1F, 0x00000105, FALSE ); //IDR picture
			DVR_PIE_ConfigureSCD(ctrl->ch, 2, 0xFFFFFF1F, 0x00000109, TRUE ); //AU Delimeter
			DVR_PIE_ConfigureSCD(ctrl->ch, 3, 0xFFFFFF1F, 0x00000106, FALSE ); //SEI
			break;

		case LX_PVR_PIE_TYPE_HEVCTS:
		#if (PVR_HEVC_INDEX_TYPE == 2)
			DVR_PIE_ConfigureSCD(ctrl->ch, 0, 0xFFFFFFFC, 0x00000140, TRUE );	//VPS/SPS
			DVR_PIE_ConfigureSCD(ctrl->ch, 1, 0xFFFFFFFE, 0x00000146, TRUE );	//AUD(I/P/B). 2nd byte(0x10:I, 0x30:P)
			DVR_PIE_ConfigureSCD(ctrl->ch, 2, 0xFFFFFFFF, 0x00000000, FALSE );
			DVR_PIE_ConfigureSCD(ctrl->ch, 3, 0xFFFFFFFF, 0x00000000, FALSE );
		#else
			DVR_PIE_ConfigureSCD(ctrl->ch, 0, 0xFFFFFF80, 0x00000100, TRUE );	// NAL Unit Type
			DVR_PIE_ConfigureSCD(ctrl->ch, 1, 0xFFFFFFFF, 0x00000000, FALSE );
			DVR_PIE_ConfigureSCD(ctrl->ch, 2, 0xFFFFFFFF, 0x00000000, FALSE );
			DVR_PIE_ConfigureSCD(ctrl->ch, 3, 0xFFFFFFFF, 0x00000000, FALSE );
		#endif
			DVR_PIE_EnableSCDReg(ctrl->ch, TRUE);
			break;

		default:
			break;
	}
	ctrl->pie_table.type = type;

	DVR_PIE_EnableINTReg (ctrl->ch, TRUE);

	return ret;
}

int PVR_PIE_Start(PVR_CTRL_T *ctrl)
{
	PVR_PIE_TRACE_PARAM_ENTER(ctrl, "");

	return RET_OK;
}

int PVR_PIE_Stop(PVR_CTRL_T *ctrl)
{
	PVR_PIE_TRACE_PARAM_ENTER(ctrl, "");

	DVR_PIE_EnableINTReg(ctrl->ch, FALSE);

	ctrl->pie_table.type = LX_PVR_PIE_TYPE_NONE;

	return RET_OK;
}

/* ISR Function  */
int PVR_PIE_PutIndex(PVR_CTRL_T *ctrl, PVR_PIE_IND_T *index, UINT32 cnt)
{
	int i;
	UINT32 wr_idx, wr_idx_next, rd_idx;
	UINT32 num, buf_num;
	PVR_PIE_TABLE_T *table = &ctrl->pie_table;

	/* get next unit buffer num to check the overflow */
	num = (ctrl->unit_buf.curr + ctrl->unit_buf.total + 1) % ctrl->unit_buf.total;
	if(num == table->entry_idx)
	{
		if(ctrl->dn.errmsg_enable)
			PVR_PRINT_WARNING("PIE-ERROR : Table Overflow...");
		ResetTableIndex(table);
	}

	rd_idx = table->rd_idx;
	wr_idx = table->wr_idx;

	for(i=0; i<cnt; i++)
	{
		if(!(index[i].val&PVR_PIE_IND_SCD_MASK))
			continue;

		if(ctrl->unit_buf.buf_num != index[i].gscd.buf_num)
		{
			UINT32 diff;
			diff = ((ctrl->unit_buf.buf_num + PVR_DN_BUF_NUM_MAX) - index[i].gscd.buf_num) % PVR_DN_BUF_NUM_MAX;
			if(diff != 1)
			{
				PVR_PRINT_PIE("PIE-WARNING : Ignore Index... HW:UNIT[%d],PIE[%d] Diff[%d]",
					ctrl->unit_buf.buf_num, index[i].gscd.buf_num, diff);
				continue;
			}
			buf_num = (ctrl->unit_buf.curr + ctrl->unit_buf.total - diff) % ctrl->unit_buf.total;
		}
		else
		{
			buf_num = ctrl->unit_buf.curr;
		}

		if(table->entry[buf_num].count == 0)
		{
			table->entry[buf_num].idx = wr_idx;
		}
		table->entry[buf_num].count++;


		table->data[wr_idx].buf_num = buf_num;
		table->data[wr_idx].index = index[i];

		wr_idx_next = (wr_idx+1)%table->max_data;
		if(wr_idx_next == rd_idx)
		{
			if(ctrl->dn.errmsg_enable)
				PVR_PRINT_WARNING("PIE-ERROR: Data Overflow...");

			ResetTableIndex(table);
			return RET_ERROR;
		}
		wr_idx = wr_idx_next;
	}
	table->wr_idx = wr_idx;

	return RET_OK;
}




/* PIE works base on uint buffer
 * and TS offset is also aligned to unit buffer
 *
 * |-----------------------| 0x8000 0000
 * |    UNIT BUFFER 0      |
 * |-----------------------| 0x8000 C000
 * |    UNIT BUFFER 1      |
 * |-----------------------| 0x8001 8000
 * |    UNIT BUFFER 2      |
 * |-----------------------| 0x8002 4000
 * |    UNIT BUFFER 3      |
 * |-----------------------| 0x8003 0000
 * |    UNIT BUFFER 4      |
 * |-----------------------| 0x8003 C000
 * |    UNIT BUFFER 5      |
 * |-----------------------| 0x8004 8000
 * |                       |
 *
 * This function get buffer number from ts offset
 */
static int GetTSUnitBufInfo(PVR_CTRL_T *ctrl, LX_PVR_BUFFER_T *ts, PVR_PIE_BUF_INFO_T *buf)
{
	UINT32 start, end, size;

	start	= ts->uiReadAddr - ctrl->buf.dn.base;
	end		= ts->uiWriteAddr - ctrl->buf.dn.base;
	size 	= (ctrl->buf.dn.size + start - end) % ctrl->buf.dn.size;

	if((start%ctrl->unit_buf.size) != 0)
	{
		PVR_PRINT_ERROR("RdAddr(0x%x) not aligned to unit buffer", ts->uiReadAddr);
		return RET_ERROR;
	}

	if((end%ctrl->unit_buf.size) != 0)
	{
		PVR_PRINT_ERROR("WrAddr(0x%x) not aligned to unit buffer", ts->uiWriteAddr);
		return RET_ERROR;
	}

	buf->start	= start/ctrl->unit_buf.size;
	buf->end	= end/ctrl->unit_buf.size;
	buf->size	= size/ctrl->unit_buf.size;

	return RET_OK;
}



static void GetMPEG2Index(PVR_CTRL_T *ctrl, LX_PVR_INDEX_T *pindex, PVR_PIE_IND_T	*pie_ind)
{
#if 0
	if(pie_ind->mpeg2.pack_cnt == ctrl->unit_buf.pkt_cnt)
	{
		PVR_PRINT_WARNING("PVR-CHECK: End of unit buf packet... Consider this case...");

		if(pie_ind->mpeg2.maxp == 0)
		{
			PVR_PRINT_ERROR("PVR-CHECK: Not maxp != 1.. Check This Case");
		}
	}
#endif

	if (pie_ind->mpeg2.sdet == 1)
	{
		pindex->type |= LX_PVR_IDX_SEQ;	//SEQ
		PVR_PRINT_PIE("{S}");
	}
	if (pie_ind->mpeg2.idet == 1)
	{
		pindex->type |= LX_PVR_IDX_I_PIC; //I Picture
		PVR_PRINT_PIE("{I}");
	}
	if (pie_ind->mpeg2.bdet == 1)
	{
		pindex->type |= LX_PVR_IDX_B_PIC;	//B Picture
		PVR_PRINT_PIE("{B}");
	}
	if (pie_ind->mpeg2.pdet == 1)
	{
		pindex->type |= LX_PVR_IDX_P_PIC;	//P Picture
		PVR_PRINT_PIE("{P}");
	}

}

static void GetH264Index(PVR_CTRL_T *ctrl, LX_PVR_INDEX_T *pindex, PVR_PIE_IND_T	*pie_ind)
{
	if (pie_ind->gscd.scd0 == 1)
	{
		pindex->type |= LX_PVR_IDX_SEQ;
		PVR_KDRV_LOG( PVR_PIE ,"{S}");
	}
	if (pie_ind->gscd.scd2 == 1)
	{
		switch(pie_ind->gscd.byte_info)
		{
			case 0x10:
				pindex->type |= LX_PVR_IDX_I_PIC;
				PVR_KDRV_LOG( PVR_PIE ,"{I}");
				break;
			case 0x30:
				pindex->type |= LX_PVR_IDX_P_PIC;
				PVR_KDRV_LOG( PVR_PIE ,"{P}");
				break;
			case 0x50:
			default:
				pindex->type |= LX_PVR_IDX_B_PIC;
				PVR_KDRV_LOG( PVR_PIE ,"{B}");
				break;
		}
	}
}

static void GetHEVCIndex(PVR_CTRL_T *ctrl, LX_PVR_INDEX_T *pindex, PVR_PIE_IND_T *pie_ind)
{
#if (PVR_HEVC_INDEX_TYPE == 2)
	if(pie_ind->gscd.scd0)
	{
		pindex->type |= LX_PVR_IDX_SEQ;
		PVR_PRINT_PIE("\n{S}");
	}
	if (pie_ind->gscd.scd1)
	{
		/* 2nd byte : I=0x10, P=0x30, B=Others */
		pindex->type |= LX_PVR_IDX_B_PIC;
		PVR_PRINT_PIE("{B}");
	}
#else
	if(pie_ind->gscd.scd0)
	{
		pindex->type |= LX_PVR_IDX_NONE;
		PVR_PRINT_PIE("\n{N}");
	}
#endif
}

static GET_INDEX_FUNC_T GetIndexFunc(LX_PVR_PIE_TYPE_T type)
{
	GET_INDEX_FUNC_T func;

	switch(type)
	{
		case LX_PVR_PIE_TYPE_MPEG2TS:	func = GetMPEG2Index; break;
		case LX_PVR_PIE_TYPE_H264TS:
		case LX_PVR_PIE_TYPE_OTHERS:	func = GetH264Index; break;
		case LX_PVR_PIE_TYPE_HEVCTS:	func = GetHEVCIndex; break;
		default: func = NULL; break;
	}

	return func;
}

static int GetPieTable(PVR_CTRL_T *ctrl, LX_PVR_BUFFER_T *ts, LX_PVR_INDEX_T *pindex, UINT32 max_size)
{
	int i;
	PVR_PIE_BUF_INFO_T buf_info;
	UINT32 buf_num, rbuf_num;

	UINT32 wr_idx, rd_idx;
	PVR_PIE_IND_T	pie_ind;

	LX_PVR_INDEX_T *pre_pindex;
	int new_pindex;
	int pindex_size;

	UINT32 byte_offset;
	GET_INDEX_FUNC_T GetIndex;
	PVR_PIE_TABLE_T *table = &ctrl->pie_table;


	PVR_CHECK_ERROR(ctrl == NULL || pindex == NULL, return RET_ERROR, "Null param");
	PVR_CHECK_ERROR(!IS_VALID_DN_BUF_ADDR(ctrl,ts->uiReadAddr), return RET_ERROR, "Invalid rd_addr(0x%x)", ts->uiReadAddr);
	PVR_CHECK_ERROR(!IS_VALID_DN_BUF_ADDR(ctrl,ts->uiWriteAddr), return RET_ERROR, "Invalid wr_addr(0x%x)", ts->uiWriteAddr);

	PVR_PRINT_PIE(PVR_COLOR_GREEN"Start Get PIE Table"PVR_COLOR_NONE);
	PVR_PRINT_PIE("TS:Rd[0x%x]Wr[0x%x]", ts->uiReadAddr, ts->uiWriteAddr);

	GetIndex = GetIndexFunc(table->type);
	PVR_CHECK_ERROR(GetIndex == NULL, return RET_ERROR, "Invalid index type[%d]", table->type);

	if(GetTSUnitBufInfo(ctrl, ts, &buf_info) < 0)
		return RET_ERROR;

	PVR_CHECK_ERROR(buf_info.size == 0, return RET_ERROR, "Too small size(under uint buffer)");

	rd_idx = table->rd_idx;
	wr_idx = table->wr_idx;

	PVR_PRINT_PIE("TS SIZE:%d, PIE - RdIdx:%d, WrIdx:%d", buf_info.size, rd_idx, wr_idx);


/*** FOR DEBUGGING ***/
	if(table->entry[buf_info.start].idx != rd_idx &&
		table->entry[buf_info.start].count != 0)
	{
		PVR_PRINT_WARNING("BUF[%d] Entry Index Diff. %d - %d",
			buf_info.start, table->entry[buf_info.start].idx, rd_idx);
	}
/*****************/

	pindex_size = 0;
	pre_pindex = NULL;

	buf_num = buf_info.start;
	while(1)
	{
		if(buf_num == buf_info.end)
			break;

		for(i=0; i<table->entry[buf_num].count; i++)
		{
			if(i == 0)	rd_idx = table->entry[buf_num].idx;

			pie_ind = table->data[rd_idx].index;
			PVR_PRINT_PIE("RD_IDX[%d], BUF_NUM[%d], PKT_CNT[%d]", rd_idx, buf_num, pie_ind.gscd.pack_cnt);

			if(buf_num != table->data[rd_idx].buf_num)
			{
				PVR_PRINT_WARNING("BUF NUM DIFF : %d %d", buf_num, table->data[rd_idx].buf_num);
				goto next;
			}

			/* Calculate the relative buffer num to get the byte offset */
			rbuf_num = (ctrl->unit_buf.total + buf_num - buf_info.start) % ctrl->unit_buf.total;
			byte_offset = rbuf_num*ctrl->unit_buf.size + (pie_ind.gscd.pack_cnt-1) * 192;

			/* Have to set only one index information per one TS packet */
			if(pre_pindex == NULL || pre_pindex->offset != byte_offset)
			{
				new_pindex		= 1;
				pindex->type	= 0;
				pindex->offset	= byte_offset;
			}
			else
			{
				new_pindex = 0;
				pindex = pre_pindex;
			}

			GetIndex(ctrl, pindex, &pie_ind);

			if(pindex->type != 0)
			{
				if(new_pindex)
				{
					pre_pindex = pindex;	/* save current pindex to check duplication */
					pindex_size++;
				}
				pindex++;
			}
next:
			rd_idx = (rd_idx+1)%table->max_data;
		}
		table->entry[buf_num].count = 0;	// clear entry info
		buf_num = (buf_num+1)%ctrl->unit_buf.total;
	}
	table->entry_idx = buf_num;	// update entry index
	table->rd_idx = rd_idx;		// update rd_idx

	PVR_PRINT_PIE(PVR_COLOR_YELLOW"End Get PIE Table"PVR_COLOR_NONE);

	return pindex_size;
}


int PVR_PIE_GetTable(PVR_CTRL_T *ctrl, LX_PVR_BUFFER_T *ts, LX_PVR_INDEX_T *index, UINT32 max_size)
{
#ifdef USE_NEW_PIE_EXTRACTION
	return GetPieTable(ctrl, ts, index, max_size);
#else
	int res = -1;
	UINT32	ui32IndexCnt, ui32Count;

	UINT32 ui32RawTsRdAddr = 0, ui32RawTsWrAddr = 0, ui32RawTsSize = 0;
	UINT32 ui32DnBufBase = 0, ui32DnBufEnd = 0;
	UINT32 ui32TsOffset = 0, ui32TotalBufCnt = 0;
	UINT32 ui32BaseBufNum = 0, ui32MaxBufCnt = 0, ui32WrapBufCnt = 0;

	UINT32 ui32Reset = 0; /* jinhwan.bae */
	/* History about Index Data Extraction Bug Fix
	 * 2013. 05. 28. - jinhwan.bae
	 * Problem reported from JP OLED Model DV Event
	 * Some macro block error happened at 2 times trick play mode (MPEG2)
	 * 1st Analysis result : P frame index data omitted from driver index report (From HE)
	 * Process : To test in following routine and print out I, P, B.
	 *               Sometimes index data is missed by stupid calculation with each circular buffer wrap around operation
	 * Fixed (4 calculation points) and Tested with ATSC KR channels.
	 * This problem may be existed from L9.
	 * Fixed points are marked with jinhwan.bae simply and added some log out for future.
	 */

	static	UINT32		preBaseBufNum = 1024; /*jinhan.bae ,  invalid buffer num */
	static 	UINT32		ui32PrevBaseBufNum = 1024, ui32PrevTsOffset = 0, ui32PrevTotalBufCnt = 0, ui32PrevRawTsSize = 0; /*jinhan.bae ,  used to 1st abnormal */

	if(ctrl->pie_table.type == LX_PVR_PIE_TYPE_HEVCTS)
		return GetPieTable(ctrl, ts, index, max_size);


	// get information about TS mermoy addr,size and make virtual memory offset, addr
	ui32RawTsRdAddr = ts->uiReadAddr;
	ui32RawTsWrAddr = ts->uiWriteAddr;
	ui32DnBufBase = ctrl->buf.dn.base;
	ui32DnBufEnd = ctrl->buf.dn.end;
	ui32TotalBufCnt = ctrl->unit_buf.total;

	if(ui32RawTsRdAddr > ui32RawTsWrAddr) //wrap arround
		ui32RawTsSize = (ui32DnBufEnd - ui32RawTsRdAddr) + (ui32RawTsWrAddr - ui32DnBufBase);
	else
		ui32RawTsSize = ui32RawTsWrAddr - ui32RawTsRdAddr;

	ui32TsOffset = ui32RawTsRdAddr - ui32DnBufBase;

	PVR_PRINT_PIE(PVR_COLOR_GREEN"Start Get PIE Table"PVR_COLOR_NONE);
	PVR_KDRV_LOG( PVR_PIE ,"--Start B[0x%x]E[0x%x]TSRd[0x%x]Off[0x%x]TSWr[0x%x]Size[0x%x]TotalBufCnt[%d]",
			ui32DnBufBase, ui32DnBufEnd, ui32RawTsRdAddr, ui32TsOffset, ui32RawTsWrAddr, ui32RawTsSize, ui32TotalBufCnt);
	PVR_KDRV_LOG( PVR_PIE, "preBaseBNum[%d]PrevBaseBNum[%d]PrevTsOffset[0x%x]PrevTotalBCnt[%d]PrevRawTsSize[0x%x]",
			preBaseBufNum, ui32PrevBaseBufNum, ui32PrevTsOffset, ui32PrevTotalBufCnt, ui32PrevRawTsSize);

	if (ctrl->ui32PieIndex == ctrl->ui32PieRdIndex)
	{
		//Index buffer empty
		ui32IndexCnt = 0;
	}
	else if (ctrl->ui32PieIndex > ctrl->ui32PieRdIndex)
	{
		/* Normal condition write ahead of read */
		ui32IndexCnt = ctrl->ui32PieIndex - ctrl->ui32PieRdIndex;
	}
	else
	{
		/* Wrap around condition */
		// jinhwan.bae ui32IndexCnt = ((PIE_MAX_ENTRIES_LOCAL-1) - ctrl->ui32PieRdIndex) + ctrl->ui32PieIndex;
		ui32IndexCnt = (PIE_MAX_ENTRIES_LOCAL - ctrl->ui32PieRdIndex) + ctrl->ui32PieIndex;
		PVR_KDRV_LOG( PVR_PIE ,"PIE_MAX_ENTRIES_LOCAL Wrap Around Case");
	}

	PVR_KDRV_LOG( PVR_PIE ,"PieCnt = [%d]-[%d] = [%d]", ctrl->ui32PieIndex, ctrl->ui32PieRdIndex, ui32IndexCnt);

	// 다운로드 버퍼사이즈를 기반으로 인덱스를 요청한 메모리 사이즈와 비교하여 매칭한다.
	ui32BaseBufNum = ui32TsOffset/ctrl->unit_buf.size;
	ui32MaxBufCnt = ui32BaseBufNum + (ui32RawTsSize/ctrl->unit_buf.size);

	PVR_KDRV_LOG( PVR_PIE ,"ui32DvrDnMinPktCount[0x%x] BaseBNum[%d] MaxBCnt[%d]",ctrl->unit_buf.pkt_cnt, ui32BaseBufNum, ui32MaxBufCnt);

	if(ui32RawTsSize%ctrl->unit_buf.size)  // Added by Soontae Kim
	{
		ui32MaxBufCnt++;
		PVR_KDRV_LOG( PVR_PIE ,"Increased ui32MaxBufCnt [%d]", ui32MaxBufCnt);
	}

	if(ui32MaxBufCnt > ui32TotalBufCnt)
	{
		ui32WrapBufCnt = ui32MaxBufCnt - ui32TotalBufCnt;
		PVR_KDRV_LOG( PVR_PIE ,"Calculated ui32WrapBufCnt [%d]", ui32WrapBufCnt);
	}

	if ( ui32IndexCnt && ui32RawTsSize )
	{
		PIE_IND_T			*pstPieSrcData;
		LX_PVR_INDEX_T		*pstIdx, *pstEnd;
		UINT32				*pstBufNum = NULL;
		UINT32				ui32ByteOffset = 0;
		SINT32 				si32AccCount = 0;
		static	UINT32		preByteOffset = -1;


		pstIdx = index;
		pstEnd = index + max_size;


		// Source data
		pstPieSrcData = &ctrl->astPieTable[ctrl->ui32PieRdIndex];
		pstBufNum = &ctrl->astBufTable[ctrl->ui32PieRdIndex];

		PVR_KDRV_LOG( PVR_PIE ,"pPie Addr[0x%x] Data[0x%x] BUF Addr[0x%x] BufNum[%d]", (UINT32)pstPieSrcData, (UINT32)(pstPieSrcData->ui32Val), (UINT32)pstBufNum, (UINT32)(*pstBufNum));

		if( ui32WrapBufCnt && ( *pstBufNum <= ui32WrapBufCnt))
		{
			*pstBufNum = *pstBufNum + ui32TotalBufCnt;
			PVR_KDRV_LOG( PVR_PIE ,"1st Updated pstBufNum value[0x%x]",(UINT32)(*pstBufNum));
		}

		PVR_KDRV_LOG( PVR_PIE ,"BUFNum: B[%d]Max[%d]Num[%d]", ui32BaseBufNum, ui32MaxBufCnt, *pstBufNum);

		// 인덱스가 가진 버퍼와 받은 버퍼가 다를경우 받은 버퍼와 매칭되는 인덱스를 찾도록 한다
		if((*pstBufNum < ui32BaseBufNum) || (*pstBufNum > ui32MaxBufCnt))
		{
			PVR_PRINT_PIE(PVR_COLOR_PURPLE"1st abnormal case"PVR_COLOR_NONE);
			for ( ui32Count = 0; ui32Count < ui32IndexCnt; ui32Count++)
			{
				if(ui32Count)
				{
					++pstPieSrcData;
					++pstBufNum;
					PVR_KDRV_LOG( PVR_PIE ,"INC: pPie[0x%x][0x%x] BUF[0x%x][%d]\n", (UINT32)pstPieSrcData, (UINT32)(pstPieSrcData->ui32Val), (UINT32)pstBufNum, (UINT32)(*pstBufNum));
				}

				if( ui32WrapBufCnt && ( *pstBufNum <= ui32WrapBufCnt))
				{
					*pstBufNum = *pstBufNum + ui32TotalBufCnt;
					PVR_KDRV_LOG( PVR_PIE ,"1st abnormal Updated pstBufNum value[0x%x]\n",(UINT32)(*pstBufNum));
				}

				PVR_KDRV_LOG( PVR_PIE ,"ui32Count[%d]pstPieSrcData[0x%x]value[0x%x] pstBufNum[0x%x]value[0x%x]\n", ui32Count, (UINT32)pstPieSrcData, (UINT32)(pstPieSrcData->ui32Val), (UINT32)pstBufNum, (UINT32)(*pstBufNum));

				if( (*pstBufNum >= ui32BaseBufNum) && (*pstBufNum < ui32MaxBufCnt))
				{
					PVR_KDRV_LOG( PVR_PIE ,"1st abnormal case break \n");
					break;
				}

				/* jinhwan.bae if index data is owned by previous data buffer */
				/* actually, this process make error in HAL(DDI) layer, so I adjusted data write pointer at UNIT_BUF interrupt time
				   , Down wptr is returned with saved write pointer , it may adjust PIE and UNIT BUF sync.
				   so, following routine should not be processed any time , only error check routine, you may see the error in HAL(DDI) warning message,
				   MPEG2 PICTURE START CODE FOUND BUT INVALID PICTURE TYPE */
				if( (*pstBufNum == (ui32BaseBufNum-1)) && (pstPieSrcData->ui32Val & 0x00F00000 ) )
				{
					/* save index data to index buffer */
					PVR_KDRV_LOG( PVR_PIE ,"find prev. index \n");
					si32AccCount = (*pstBufNum) - ui32PrevBaseBufNum;
					ui32ByteOffset = ((pstPieSrcData->le.pack_cnt - 1) + (ctrl->unit_buf.pkt_cnt * si32AccCount))*192 - ui32PrevTsOffset%ctrl->unit_buf.size;  // Modified by Soontae Kim

					if( ui32ByteOffset >= ui32PrevRawTsSize )
					{
						PVR_KDRV_LOG( PVR_PIE , "( ui32ByteOffset >= ui32PrevRawTsSize ) case \n");
						if( *pstBufNum > ui32PrevTotalBufCnt )
						{
							*pstBufNum = *pstBufNum - ui32PrevTotalBufCnt;
							PVR_KDRV_LOG( PVR_PIE ,"4th Updated pstBufNum value[0x%x]\n",(UINT32)(*pstBufNum));
						}
						PVR_KDRV_LOG( PVR_PIE ,"^R^overflow byteoffset 0x%x bufnum %d AccCount %d index %d \n", ui32ByteOffset, *pstBufNum, si32AccCount, ctrl->ui32PieRdIndex);
						// break;
						// jinhwan.bae original normal case results in break, but now abnormal case is process next
					}
					else
					{
						PVR_KDRV_LOG( PVR_PIE ," Change to NORM \n");

						// jinhwan.bae Rdindex++ is up to remain process of abnormal case

						//Total buffer count from beggining
						pstIdx->type  = LX_PVR_IDX_NONE;
						pstIdx->offset = ui32ByteOffset;

						// jinhwan.bae if(preByteOffset == ui32ByteOffset)
						// normal check with previous buffer number is not valid in this routine
						// because, base buf number is updated always. in that always process index

						if(ctrl->state.ePieState == LX_PVR_PIE_STATE_MP2)
						{
							//Assign the Picture index type
							if (pstPieSrcData->le.sdet == 1)
							{
								pstIdx->type = LX_PVR_IDX_SEQ;	//SEQ
								PVR_KDRV_LOG( PVR_PIE ,"\n{S}");
							}
							if (pstPieSrcData->le.idet == 1)
							{
								/*
								 * The bitwise OR is used when both Sequence header
								 * and I-Pic are present in same packet
								 */
								pstIdx->type |= LX_PVR_IDX_I_PIC; //I Picture
								PVR_KDRV_LOG( PVR_PIE ,"\n{I}");
							}
							if (pstPieSrcData->le.bdet == 1)
							{
								pstIdx->type = LX_PVR_IDX_B_PIC;	//B Picture
								PVR_KDRV_LOG( PVR_PIE ,"{B}");
							}

							if (pstPieSrcData->le.pdet == 1)
							{
								pstIdx->type = LX_PVR_IDX_P_PIC;	//P Picture
								PVR_KDRV_LOG( PVR_PIE ,"{P}");
							}
						}
						else	//LX_PVR_PIE_STATE_GSCD
						{
							if (pstPieSrcData->le.bdet == 1)
							{
								pstIdx->type = LX_PVR_IDX_SEQ;
								PVR_KDRV_LOG( PVR_PIE ,"\n{S}");
							}

							if (pstPieSrcData->le.idet == 1)
							{
								switch(pstPieSrcData->le.byte_info)
								{
									case 0x10:
										pstIdx->type = LX_PVR_IDX_I_PIC;
										PVR_KDRV_LOG( PVR_PIE ,"\n{I}");
										break;
									case 0x30:
										pstIdx->type = LX_PVR_IDX_P_PIC;
										PVR_KDRV_LOG( PVR_PIE ,"{P}");
										break;
									case 0x50:
									default:
										pstIdx->type = LX_PVR_IDX_B_PIC;
										PVR_KDRV_LOG( PVR_PIE ,"{B}");
										break;
								}
							}
						}

						++pstIdx;
						if ( pstIdx == pstEnd) break;	/* End of buffer */
					}
				}

				--ui32IndexCnt;
				++ctrl->ui32PieRdIndex;
				if (ctrl->ui32PieRdIndex == PIE_MAX_ENTRIES_LOCAL)
				{
					/* Wrap around */
					ctrl->ui32PieRdIndex = 0;
					pstPieSrcData = &ctrl->astPieTable[0];
					pstBufNum = &ctrl->astBufTable[0];
					PVR_KDRV_LOG( PVR_PIE ,"PVR_DRV %d> Pie Source buffer wrap around\n", __LINE__ );
				}
			}
		}

		ui32PrevBaseBufNum = ui32BaseBufNum;
		ui32PrevTsOffset = ui32TsOffset;
		ui32PrevTotalBufCnt = ui32TotalBufCnt;
		ui32PrevRawTsSize = ui32RawTsSize;

		PVR_KDRV_LOG( PVR_PIE ,"Total IDX Cnt[%d]",ui32IndexCnt);

		ui32Reset = 0; /* jinhwan.bae */
		for ( ui32Count = 0; ui32Count < ui32IndexCnt; ui32Count++)
		{
			PVR_KDRV_LOG( PVR_PIE ,"ui32Count [%d]", ui32Count);
#if 0 /* jinhwan.bae */
			if(ui32Count)
			{
				++pstPieSrcData;
				++pstBufNum;
				PVR_KDRV_LOG( PVR_PIE ,"increased pstPieSrcData[0x%x]value[0x%x] pstBufNum[0x%x]value[0x%x]\n", (UINT32)pstPieSrcData, (UINT32)(pstPieSrcData->ui32Val), (UINT32)pstBufNum, (UINT32)(*pstBufNum));
			}
#else /* jinhwan.bae */
			if((ui32Count != 0) && (ui32Reset == 0))
			{
				++pstPieSrcData;
				++pstBufNum;
				PVR_KDRV_LOG( PVR_PIE ,"INC: pPie[0x%x][0x%x] BUF[0x%x][%d]", (UINT32)pstPieSrcData, (UINT32)(pstPieSrcData->ui32Val), (UINT32)pstBufNum, (UINT32)(*pstBufNum));
			}
			else if(ui32Reset == 1)
			{
				/* delete original pointer moving and set pstPieSrcData and pstBufNum at this time */
				pstPieSrcData = &ctrl->astPieTable[0];
				pstBufNum = &ctrl->astBufTable[0];
			}
			ui32Reset = 0;
#endif /* jinhwan.bae */

			if( ui32WrapBufCnt && ( *pstBufNum <= ui32WrapBufCnt))
			{
				*pstBufNum = *pstBufNum + ui32TotalBufCnt;
				PVR_KDRV_LOG( PVR_PIE ,"2nd Updated pstBufNum value[0x%x]\n",(UINT32)(*pstBufNum));
			}

			if((*pstBufNum >= ui32MaxBufCnt) || (*pstBufNum < ui32BaseBufNum))
			{
				if( *pstBufNum > ui32TotalBufCnt )
				{
					*pstBufNum = *pstBufNum - ui32TotalBufCnt;
					PVR_KDRV_LOG( PVR_PIE ,"3rd Updated pstBufNum value[0x%x]\n",(UINT32)(*pstBufNum));
				}

				PVR_KDRV_LOG( PVR_PIE ,"^Y^overflow BufNum %d index %d \n", *pstBufNum, ctrl->ui32PieRdIndex);
				break;
			}

			PVR_KDRV_LOG( PVR_PIE ,"pstPieSrcData->ui32Val [0x%x]", pstPieSrcData->ui32Val);

			if ( pstPieSrcData->ui32Val & 0x00F00000 )
			{
				si32AccCount = *pstBufNum - ui32BaseBufNum;

				/* jinhwan.bae 20131227 Index Error Work Around H13/M14
				   Sometimes index error happened at user space.
				   Some index informations from driver are not valid especially S-I composition
				   If I picture start code existed in previous TP and picture type is in next TP,
				   and the previous TP is exist the last TP of previous buffer num and next is the start TP of next buffer num,
				   H/W indicated I picture index as maxp = 0, pack_cnt = 0x80(MAX).
				   Driver workaround is done by setting I picture index with previous TP (last TP of previous buf num)
				   because our M/W needs index information as picture start code so I picture index location should be the TP with picture start code
				   Exception : in this case, if the I picture type exist the first TS of requested buffer chunk, we can't give any information to M/W
				   because, there is no interface to give previous buffer chunk offset. so skip the index in this case. as a result, index missing will happen. */
#if 0
				if(pstPieSrcData->le.maxp == 1)
				{
					PVR_KDRV_LOG( PVR_ERROR ,"maxp == 1 pstPieSrcData->ui32Val[0x%x] BufNum[%d] BaseBufNum[%d] MaxBufNum[%d]",pstPieSrcData->ui32Val, *pstBufNum, ui32BaseBufNum, ui32MaxBufCnt);
				}

				if(pstPieSrcData->le.pack_cnt == ctrl->unit_buf.pkt_cnt)
				{
					PVR_KDRV_LOG( PVR_ERROR ,"pack_cnt[0x%x] pstPieSrcData->ui32Val[0x%x]",pstPieSrcData->le.pack_cnt, pstPieSrcData->ui32Val);
				}
#endif
				/* jinhwan.bae 20140211 in H264, normal index can be inserted the format as follows.
				   so, insert MP2 ctrl->state.ePieState == LX_PVR_PIE_STATE_MP2  restriction to work-around */
				if((ctrl->state.ePieState == LX_PVR_PIE_STATE_MP2) && (pstPieSrcData->le.maxp == 0) && (pstPieSrcData->le.pack_cnt == ctrl->unit_buf.pkt_cnt /* 0x80 */))
				{
					/* jinhwan.bae work around case */
					PVR_KDRV_LOG( PVR_PIE ,"maxp == 0, pack_cnt = 0x80 pstPieSrcData->ui32Val[0x%x] BufNum[%d] BaseBufNum[%d] MaxBufNum[%d]",pstPieSrcData->ui32Val, *pstBufNum, ui32BaseBufNum, ui32MaxBufCnt);
					if(si32AccCount == 0)
					{
						/* if the error happened at the start TP, skip the index information */
						PVR_KDRV_LOG( PVR_PIE ,"first offset of requested buffer chunk, skip index");
						continue;
					}

					/* work around can be done, set the index last TP of previous buffer number */
					/* 	if the byte offset is same as previous SEQ byte offset, this information will be discarded
						at (preByteOffset == ui32ByteOffset) && (preBaseBufNum == ui32BaseBufNum) #1410 , until now, always discarded 20131231 */
					si32AccCount -= 1;
					ui32ByteOffset = ((pstPieSrcData->le.pack_cnt - 1) + (ctrl->unit_buf.pkt_cnt * si32AccCount))*192 - ui32TsOffset%ctrl->unit_buf.size;  // Modified by Soontae Kim
				}
				else
				{
					/* jinhwan.bae normal operation case */
					//ui32ByteOffset = ((pstPieSrcData->le.pack_cnt - 1) + (ctrl->unit_buf.pkt_cnt * si32AccCount))*192;  // Removed by Soontae Kim
					ui32ByteOffset = ((pstPieSrcData->le.pack_cnt - 1) + (ctrl->unit_buf.pkt_cnt * si32AccCount))*192 - ui32TsOffset%ctrl->unit_buf.size;  // Modified by Soontae Kim
				}

				PVR_KDRV_LOG( PVR_PIE ,"ACC [%d] Byte Offset [0x%x]",si32AccCount, ui32ByteOffset);

				if( ui32ByteOffset >= ui32RawTsSize )
				{
					PVR_KDRV_LOG( PVR_PIE ,"( ui32ByteOffset >= ui32RawTsSize ) case \n");
					if( *pstBufNum > ui32TotalBufCnt )
					{
						*pstBufNum = *pstBufNum - ui32TotalBufCnt;
						PVR_KDRV_LOG( PVR_PIE ,"4th Updated pstBufNum value[0x%x]\n",(UINT32)(*pstBufNum));
					}
					PVR_KDRV_LOG( PVR_PIE ,"^R^overflow byteoffset 0x%x bufnum %d AccCount %d index %d \n", ui32ByteOffset, *pstBufNum, si32AccCount, ctrl->ui32PieRdIndex);
					break;
				}
				else
				{
					PVR_KDRV_LOG( PVR_PIE ,"Normal Case");
					// byte offset 을 먼저 체크해보고 Read index를 증가시킨다.
					++ctrl->ui32PieRdIndex;
					if (ctrl->ui32PieRdIndex == PIE_MAX_ENTRIES_LOCAL)
					{
						/* Wrap around */
						ctrl->ui32PieRdIndex = 0;
#if 0 /* jinhwan.bae move to start point of loop */
						pstPieSrcData = &ctrl->astPieTable[0];
						pstBufNum = &ctrl->astBufTable[0];
#else
						ui32Reset = 1;
#endif
						PVR_KDRV_LOG( PVR_PIE ,"PVR_DRV %d> Pie Source buffer wrap around\n", __LINE__ );
					}
				}

				//Total buffer count from beggining
				pstIdx->type  = LX_PVR_IDX_NONE;
				pstIdx->offset = ui32ByteOffset;

				// jinhwan.bae if(preByteOffset == ui32ByteOffset)
				if( (preByteOffset == ui32ByteOffset) && (preBaseBufNum == ui32BaseBufNum) ) /* jinhwan.bae */
				{
					PVR_KDRV_LOG( PVR_PIE ,"(preByteOffset[0x%x] == ui32ByteOffset[0x%x]) &&  (preBaseBufNum[%d] == ui32BaseBufNum[%d]) case skip index extraction! \n",
							preByteOffset, ui32ByteOffset, preBaseBufNum, ui32BaseBufNum);
					continue;
				}
				else
				{
					preByteOffset = ui32ByteOffset;
					preBaseBufNum = ui32BaseBufNum;	/* jinhwan.bae */
				}

				if(ctrl->state.ePieState == LX_PVR_PIE_STATE_MP2)
				{
					//Assign the Picture index type
					if (pstPieSrcData->le.sdet == 1)
					{
						pstIdx->type = LX_PVR_IDX_SEQ;	//SEQ
						PVR_KDRV_LOG( PVR_PIE ,"{S}");
					}
					if (pstPieSrcData->le.idet == 1)
					{
#if 0
						/*
						 * The bitwise OR is used when both Sequence header
						 * and I-Pic are present in same packet
						 */
						pstIdx->type |= PVR_IDX_I_PIC; //I Picture
						PVR_KDRV_LOG( PVR_PIE ,"\n{I}");
#else
						/* jinhwan.bae for test  i only return to as NONE, because 0xFF | I type */
						if(pstIdx->type == LX_PVR_IDX_SEQ)
						{
							pstIdx->type |= LX_PVR_IDX_I_PIC; //I Picture
						}
						else
						{
							pstIdx->type = LX_PVR_IDX_I_PIC; //I Picture
						}
						PVR_KDRV_LOG( PVR_PIE ,"{I}");
#endif
					}
					if (pstPieSrcData->le.bdet == 1)
					{
						pstIdx->type = LX_PVR_IDX_B_PIC;	//B Picture
						PVR_KDRV_LOG( PVR_PIE ,"{B}");
					}

					if (pstPieSrcData->le.pdet == 1)
					{
						pstIdx->type = LX_PVR_IDX_P_PIC;	//P Picture
						PVR_KDRV_LOG( PVR_PIE ,"{P}");
					}
				}
				else	//LX_PVR_PIE_STATE_GSCD
				{
					if (pstPieSrcData->le.bdet == 1)
					{
						pstIdx->type = LX_PVR_IDX_SEQ;
						PVR_KDRV_LOG( PVR_PIE ,"{S}");
					}

					if (pstPieSrcData->le.idet == 1)
					{
						switch(pstPieSrcData->le.byte_info)
						{
							case 0x10:
								pstIdx->type = LX_PVR_IDX_I_PIC;
								PVR_KDRV_LOG( PVR_PIE ,"{I}");
								break;
							case 0x30:
								pstIdx->type = LX_PVR_IDX_P_PIC;
								PVR_KDRV_LOG( PVR_PIE ,"{P}");
								break;
							case 0x50:
							default:
								pstIdx->type = LX_PVR_IDX_B_PIC;
								PVR_KDRV_LOG( PVR_PIE ,"{B}");
								break;
						}
					}
				}

				++pstIdx;
				if ( pstIdx == pstEnd) break;	/* End of buffer */
			}
			else
			{
				PVR_KDRV_LOG( PVR_PIE ,"pstPieSrcData does not contain IPB" );
				++ctrl->ui32PieRdIndex;
				if (ctrl->ui32PieRdIndex == PIE_MAX_ENTRIES_LOCAL)
				{
					/* Wrap around */
					ctrl->ui32PieRdIndex = 0;
#if 0 /* jinhwan.bae move to start point of loop */
					pstPieSrcData = &ctrl->astPieTable[0];
					pstBufNum = &ctrl->astBufTable[0];
#else
					ui32Reset = 1;
#endif
					PVR_KDRV_LOG( PVR_PIE ,"PVR_DRV %d> Pie Source buffer wrap around\n", __LINE__ );
				}
			}
		}

		res = pstIdx - index;	/* num of saved index */
	}


	/* Reset index write pointer to the base after copying to user for linear mode operation */
	//ctrl->buf.stPieUserVirtBuff.wptr = ctrl->buf.stPieUserVirtBuff.base;

	PVR_PRINT_PIE(PVR_COLOR_YELLOW"End Get PIE Table"PVR_COLOR_NONE);

	return res;
#endif
}

