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

/** @file pe_fwi_h15.c
 *
 *  driver for firmware interface functions. ( used only within kdriver )
 *
 *	@author		Seung-Jun, Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.04.12
 *	@see
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_fwi_h15.h"

#include "../../be/fwi/ipc_def_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#undef PE_FWI_H15_USE_PACK_CMD

#define PE_FWI_H15A_FW_SRC_OFF		(0)
#define PE_FWI_H15A_FW_SRC_ON		(1)
#define PE_FWI_H15A_FW_RESET_STATUS_OFF		(0)
#define PE_FWI_H15A_FW_RESET_STATUS_ON		(1)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_FWI_H15_ERROR	PE_PRINT_ERROR

#define PE_FWI_H15_DBG_PRINT(fmt, args...)	\
	if (_g_fwi_h15_trace)\
	{\
		printk("[%x,%x][%s,%d] "fmt, PE_CHIP_VER, \
			g_pe_kdrv_ver_mask, __F__, __L__, ##args);\
	}
#define PE_FWI_H15_CHECK_CODE(_checker, _action, fmt, args...)	\
	{if (_checker){PE_FWI_H15_ERROR(fmt, ##args);_action;}}

#define PE_FWI_H15_IPC0_CHK_NULL()		\
	PE_FWI_H15_CHECK_CODE((pe_fwi_h15_mcu0_ipc==NULL || \
	pe_fwi_h15_mcu0_ipc->m_ipc_client->write==NULL || \
	pe_fwi_h15_mcu0_ipc->m_ipc_client->read==NULL), \
	ret = RET_ERROR;break, "[%s,%d] mcu0_ipc is null.\n", __F__, __L__)

#define PE_FWI_H15_IPC0_CHK_READY()		\
	PE_FWI_H15_CHECK_CODE(pe_fwi_h15_mcu0_ipc->p_ready==NULL, \
	ret = RET_ERROR;break, "[%s,%d] mcu0_ipc is not ready.\n", __F__, __L__)

#define PE_FWI_H15_IPC0_SET_DATA(cmd, msg, size)		\
	pe_fwi_h15_mcu0_ipc->m_ipc_client->write(cmd, msg, size)

#define PE_FWI_H15_IPC1_CHK_NULL()		\
	PE_FWI_H15_CHECK_CODE((pe_fwi_h15_mcu1_ipc==NULL || \
	pe_fwi_h15_mcu1_ipc->m_ipc_client->write==NULL || \
	pe_fwi_h15_mcu1_ipc->m_ipc_client->read==NULL), \
	ret = RET_ERROR;break, "[%s,%d] mcu1_ipc is null.\n", __F__, __L__)

#define PE_FWI_H15_IPC1_CHK_READY()		\
	PE_FWI_H15_CHECK_CODE(pe_fwi_h15_mcu1_ipc->p_ready==NULL, \
	ret = RET_ERROR;break, "[%s,%d] mcu1_ipc is not ready.\n", __F__, __L__)

#define PE_FWI_H15_IPC1_SET_DATA(cmd, msg, size)		\
	pe_fwi_h15_mcu1_ipc->m_ipc_client->write(cmd, msg, size)

#define PE_PACK_CMD(_id, _cmd) 	PACK_CMD(0, 0, (_id), (_cmd))

#define PE_FWI_H15_CMD_TO_STR(_id, _cmd)		(\
	(((_id)==MCU_ID_0)&&((_cmd)==PQL_IPC_SET_TABLE))?		"PQL_IPC_SET_TABLE":\
	(((_id)==MCU_ID_0)&&((_cmd)==DE_IPC_SET_CVI_CSC))?		"DE_IPC_SET_CVI_CSC":\
	(((_id)==MCU_ID_0)&&((_cmd)==PQL_IPC_SET_TNR_ENABLE))?	"PQL_IPC_SET_TNR_ENABLE":\
	(((_id)==MCU_ID_0)&&((_cmd)==DE_IPC_SET_PUSH_MODE))?	"DE_IPC_SET_PUSH_MODE":\
	(((_id)==MCU_ID_0)&&((_cmd)==PQL_IPC_LOAD_DB_DATA))?	"PQL_IPC_LOAD_DB_DATA":\
	"INVALIDE_CMD")

/* define for refresh pq fw table when fw reset */
#define PE_FWI_H15A_CHECK_REFRESH_TBL( )	\
	do{\
		if (PE_KDRV_VER_H15)\
		{\
			PE_FWI_H15_SETTINGS_T *p_temp_info = &_g_pe_fwi_h15_info;\
			if ((p_temp_info->fw_status.reset) == PE_FWI_H15A_FW_RESET_STATUS_ON)\
			{\
				PE_FWI_H15_DBG_PRINT("RESET ON, fw src_info:%d\n", \
					p_temp_info->fw_status.src_info);\
				ret = PE_FWI_H15A_RefreshFwTable();\
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15A_RefreshFwTable() error.\n", __F__, __L__);\
				(p_temp_info->fw_status.reset) = PE_FWI_H15A_FW_RESET_STATUS_OFF;\
			}\
		}\
	}while (0)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

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
struct ipc_handle *pe_fwi_h15_mcu0_ipc = NULL;
struct ipc_handle *pe_fwi_h15_mcu1_ipc = NULL;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int PE_FWI_H15_IPC_SetData(UINT32 mcuId, UINT32 ipcCmd, void *pMsg, UINT32 msgCnt);
static int PE_FWI_H15_SetFwSettings(PE_FWI_PQ_TABLE_HEAD_T header, UINT8 *pTable, UINT32 tblSize);
static int PE_FWI_H15A_RefreshFwTable(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_fwi_h15_trace=0x0;	//default should be off.
static PE_FWI_H15_SETTINGS_T _g_pe_fwi_h15_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * set f/w ipc set data
 *	- never use this on sync handler
 *
 * @param   cmd [in] UINT32
 * @param   *msg [in] void
 * @param   size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_FWI_H15_IPC_SetData(UINT32 mcuId, UINT32 ipcCmd, void *pMsg, UINT32 msgCnt)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pMsg);
		PE_FWI_H15_DBG_PRINT("mcuId:%d, ipcCmd:0x%08X(%s), msgCnt:%d\n", \
			mcuId, ipcCmd, PE_FWI_H15_CMD_TO_STR(mcuId, ipcCmd), msgCnt);
		if (PE_KDRV_VER_H15)
		{
			if (mcuId == MCU_ID_0 || mcuId == MCU_ID_1)
			{
				PE_FWI_H15_IPC0_CHK_NULL();
				PE_FWI_H15_IPC0_CHK_READY();
				#ifdef PE_FWI_H15_USE_PACK_CMD
				PE_FWI_H15_IPC0_SET_DATA(PE_PACK_CMD(mcuId, ipcCmd), pMsg, msgCnt);
				#else
				PE_FWI_H15_IPC0_SET_DATA(ipcCmd, pMsg, msgCnt);
				#endif
			}
			else
			{
				PE_FWI_H15_ERROR("[%s,%d] mcuId(%d) is invalid.\n", __F__, __L__, mcuId);
				ret = RET_ERROR;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set pq f/w settings
 *	- never use this on sync handler
 *
 * @param   header [in] PE_FWI_PQ_TABLE_HEAD_T
 * @param   *table [in] UINT8
 * @param   tbl_size [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_FWI_H15_SetFwSettings(PE_FWI_PQ_TABLE_HEAD_T header, UINT8 *pTable, UINT32 tblSize)
{
	int ret = RET_OK;
	UINT32 offset;
	UINT32 length;
	UINT32 dataSize;
	UINT8 *pData = NULL;
	DE_IPC_CMD_T ipcCmd = PQL_IPC_SET_TABLE;
	do{
		offset = header.offset;
		length = header.length;
		PE_FWI_H15_CHECK_CODE(!length, break, "[%s,%d] length is zero.\n", __F__, __L__);
		PE_FWI_H15_CHECK_CODE(!pTable, break, "[%s,%d] pTable is null.\n", __F__, __L__);
		PE_FWI_H15_CHECK_CODE(!tblSize, break, "[%s,%d] tblSize is zero.\n", __F__, __L__);
		PE_FWI_H15_CHECK_CODE(((offset+length)>tblSize), break, \
			"[%s,%d] offset(%d) and length(%d) is greater than tblSize(%d).\n", \
			__F__, __L__, offset, length, tblSize);
		PE_FWI_H15_DBG_PRINT(" wid:%d, ofs:%d, len:%d, tbl_size:%d\n", \
			header.win_id, header.offset, header.length, tblSize);
		dataSize = sizeof(header)+length;
		pData = OS_KMalloc(dataSize);
		PE_FWI_H15_CHECK_CODE(!pData, break, "[%s,%d] pData is null.\n", __F__, __L__);
		memcpy(pData, &header, sizeof(header));
		memcpy(&pData[sizeof(header)], &pTable[offset], length);
		ret = PE_FWI_H15_IPC_SetData(MCU_ID_0, ipcCmd, pData, dataSize);
		PE_FWI_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_FWI_H15_IPC_SetData() error.\n", __F__, __L__);
	}while (0);
	if (pData)	OS_Free(pData);
	return ret;
}
/**
 * refresh f/w table
 *	- never use this on sync handler
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_FWI_H15A_RefreshFwTable(void)
{
	int ret = RET_OK;
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		if (PE_KDRV_VER_H15)
		{
			pTable = (UINT8 *)&table;
			tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
			header.win_id = LX_PE_WIN_0;
			header.offset = PE_FWI_H15_REFRESH_TBL_OFST;
			header.length = PE_FWI_H15_REFRESH_TBL_SIZE;
			memcpy(&(table.tnr_ctrl), &(pInfo->tnr_ctrl[LX_PE_WIN_0]), sizeof(PE_FWI_H15_TNR_CTRL));
			memcpy(&(table.fw_ctrl), &(pInfo->fw_ctrl), sizeof(PE_FWI_H15_FW_CTRL));
			memcpy(&(table.src_info), &(pInfo->src_info), sizeof(PE_FWI_H15_SRC_INFO));
			#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
			memcpy(&(table.dnr_common), &(pInfo->dnr_common), sizeof(PE_FWI_H15_DNR_COMMON));
			memcpy(&(table.dnr_detail), &(pInfo->dnr_detail), sizeof(PE_FWI_H15_DNR_DETAIL));
			#endif
			#if 0	//not_used
			memcpy(&(table.shp_h_ctrl), &(pInfo->shp_h_ctrl), sizeof(PE_FWI_H15_HOR_CTRL));
			memcpy(&(table.shp_m_ctrl), &(pInfo->shp_m_ctrl), sizeof(PE_FWI_H15_MISC_CTRL));
			memcpy(&(table.shp_e_ctrl), &(pInfo->shp_e_ctrl), sizeof(PE_FWI_H15_EDGE_GAIN));
			memcpy(&(table.shp_t_ctrl), &(pInfo->shp_t_ctrl), sizeof(PE_FWI_H15_TEXTURE_GAIN));
			memcpy(&(table.src_info), &(pInfo->src_info), sizeof(PE_FWI_H15_SRC_INFO));
			memcpy(&(table.cen_ctrl), &(pInfo->cen_ctrl), sizeof(PE_FWI_H15_CEN_CTRL));
			#endif
			PE_FWI_H15_DBG_PRINT("wid:%d, ofs:%d, len:%d, tbl_size:%d\n", \
				header.win_id, header.offset, header.length, tbl_size);
			ret = PE_FWI_H15_SetFwSettings(header,  pTable,  tbl_size);
			PE_FWI_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set debug print enable
 *
 * @param   on_off [in] UINT32
 * @return  void
 * @see
 * @author
 */
void PE_FWI_H15_SetDbgPrint(UINT32 on_off)
{
	_g_fwi_h15_trace = on_off? 1:0;
	return;
}
/**
 * get debug print enable
 *
 * @param   void
 * @return  on, off
 * @see
 * @author
 */
UINT32 PE_FWI_H15_GetDbgPrint(void)
{
	return _g_fwi_h15_trace;
}
/**
 * open fwi ipc
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_IPC_Open(void)
{
	int ret = RET_OK;
	do{
		if (PE_KDRV_VER_H15)
		{
			PE_PRINT_NOTI("open ipc.\n");
			/* check pe_fwi_h15_mcu0_ipc is already opened */
			if(pe_fwi_h15_mcu0_ipc == NULL)
			{
				pe_fwi_h15_mcu0_ipc = ipc_hal_open("pe", MCU_ID_0);
				PE_FWI_H15_CHECK_CODE(!pe_fwi_h15_mcu0_ipc, ret = RET_ERROR;break, \
					"[%s,%d] pe_fwi_h15_mcu0_ipc open fail.\n", __F__, __L__);
			}
			/* check pe_fwi_h15_mcu1_ipc is already opened */
			if(pe_fwi_h15_mcu1_ipc == NULL)
			{
				pe_fwi_h15_mcu1_ipc = ipc_hal_open("pe", MCU_ID_1);
				PE_FWI_H15_CHECK_CODE(!pe_fwi_h15_mcu1_ipc, ret = RET_ERROR;break, \
					"[%s,%d] pe_fwi_h15_mcu1_ipc open fail.\n", __F__, __L__);
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while (0);
	return ret;
}

/**
 * close fwi ipc
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_IPC_Close(void)
{
	int ret = RET_OK;
	do{
		if (PE_KDRV_VER_H15)
		{
			PE_PRINT_NOTI("close ipc.\n");
			if (pe_fwi_h15_mcu0_ipc)
			{
				ipc_hal_close(pe_fwi_h15_mcu0_ipc);
				pe_fwi_h15_mcu0_ipc = NULL;
			}
			if (pe_fwi_h15_mcu1_ipc)
			{
				ipc_hal_close(pe_fwi_h15_mcu1_ipc);
				pe_fwi_h15_mcu1_ipc = NULL;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * init fwi module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_H15_FW_CTRL fw_ctrl;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15_DBG_PRINT("init global values.\n");
			memset(&_g_pe_fwi_h15_info, 0x0, sizeof(PE_FWI_H15_SETTINGS_T));
			_g_pe_fwi_h15_info.src_info.dtv_mode = 0xff;	//default
			_g_pe_fwi_h15_info.src_info.hdd_mode = 0xff;	//default
			#if 0	//not_used
			memset(&(_g_pe_fwi_h15_info.cen_ctrl[0]), 0x0, \
				sizeof(PE_FWI_H15_CEN_CTRL)*LX_PE_CMG_REGION_NUM);
			#endif
			/* fw_ctrl.enable(=0:by register, =1:by fw) */
			fw_ctrl.enable = 0x0;
			ret = PE_FWI_H15_SetFwCtrl(&fw_ctrl, FALSE);
			PE_FWI_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_SetFwCtrl() error.\n", __F__, __L__);
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set f/w control
 *	- it's for shp fw only
 *	- never use fw ipc on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_FW_CTRL
 * @param   called_on_sync [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetFwCtrl(PE_FWI_H15_FW_CTRL *pstParams, UINT32 called_on_sync)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_FOR_BRINGUP
	#else
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			if (pInfo->fw_ctrl.enable!=pstParams->enable)
			{
				if (!called_on_sync)
				{
					pTable = (UINT8 *)&table;
					tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
					header.win_id = LX_PE_WIN_0;
					header.offset = PE_FWI_OFFSET(PE_FWI_H15_FW_CTRL);
					header.length = sizeof(PE_FWI_H15_FW_CTRL);
					table.fw_ctrl.enable = (pstParams->enable)? 1:0;
					PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d, "\
						"fw_ctrl.enable:%d->%d\n", \
						header.win_id, header.offset, header.length, tbl_size, \
						pInfo->fw_ctrl.enable, table.fw_ctrl.enable);
					ret = PE_FWI_H15_SetFwSettings(header,  pTable,  tbl_size);
					PE_FWI_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				}
				PE_FWI_H15_DBG_PRINT("set reg_shp_fw_en:%d->%d\n", \
					pInfo->fw_ctrl.enable, table.fw_ctrl.enable);
				PE_PE1_H15A0_QWr01(pe1_fsw_ctrl_01, reg_shp_fw_en, (pstParams->enable)? 1:0);
				pInfo->fw_ctrl.enable = (pstParams->enable)? 1:0;
			}
			else
			{
				/*too many print*/
				/*PE_FWI_H15_DBG_PRINT("fw_ctrl.enable:%d->%d, same..\n", \
					pInfo->fw_ctrl.enable, pstParams->enable);*/
				ret = RET_OK;
				break;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * set fw ctrl enable(periodic)
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetFwCtrlEnable(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_FWI_H15_FW_CTRL pre_fw_ctrl, cur_fw_ctrl;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if (PE_KDRV_VER_H15)
		{
			pre_fw_ctrl.enable = pInfo->fw_ctrl.enable;
			/** check pip same h, v case(=split window),
			* because of avoiding peaking transition artifact */
			if (disp0_info->out_type==LX_PE_OUT_PIP && \
				(ctrl0_info->hv_same || ctrl1_info->hv_same))
			{
				cur_fw_ctrl.enable = 0;
			}
			else
			{
				cur_fw_ctrl.enable = 1;
			}
			/* don't use kipc, if pic_sync_handler registered*/
			ret = PE_FWI_H15_SetFwCtrl(&cur_fw_ctrl, pCfg->hdlr_opr.is_registered);
			PE_FWI_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_SetFwCtrl() error.\n", __F__, __L__);
			/* set shp again when fw.en on <-> off */
			if (pre_fw_ctrl.enable!=cur_fw_ctrl.enable && !cur_fw_ctrl.enable)
			{
				PE_FWI_H15_HOR_CTRL shp_h_ctrl;
				PE_FWI_H15_MISC_CTRL shp_m_ctrl;
				PE_FWI_H15_EDGE_GAIN shp_e_ctrl;
				PE_FWI_H15_TEXTURE_GAIN shp_t_ctrl;
				shp_h_ctrl.e_gain_th1 = pInfo->shp_h_ctrl.e_gain_th1;
				shp_h_ctrl.e_gain_th2 = pInfo->shp_h_ctrl.e_gain_th2;
				shp_m_ctrl.gt_th0 = pInfo->shp_m_ctrl.gt_th0;
				shp_m_ctrl.gt_th1 = pInfo->shp_m_ctrl.gt_th1;
				shp_m_ctrl.gt_th0a = pInfo->shp_m_ctrl.gt_th0a;
				shp_m_ctrl.gt_th0b = pInfo->shp_m_ctrl.gt_th0b;
				shp_e_ctrl.co09_gain_b = pInfo->shp_e_ctrl.co09_gain_b;
				shp_e_ctrl.co09_gain_w = pInfo->shp_e_ctrl.co09_gain_w;
				shp_e_ctrl.dp01_gain_b = pInfo->shp_e_ctrl.dp01_gain_b;
				shp_e_ctrl.dp01_gain_w = pInfo->shp_e_ctrl.dp01_gain_w;
				shp_t_ctrl.co09_gain_b = pInfo->shp_t_ctrl.co09_gain_b;
				shp_t_ctrl.co09_gain_w = pInfo->shp_t_ctrl.co09_gain_w;
				shp_t_ctrl.dp01_gain_b = pInfo->shp_t_ctrl.dp01_gain_b;
				shp_t_ctrl.dp01_gain_w = pInfo->shp_t_ctrl.dp01_gain_w;
				PE_FWI_H15_DBG_PRINT("fw_en:%d->%d, set prev shp settings again.\n", \
					pre_fw_ctrl.enable, cur_fw_ctrl.enable);
				ret = PE_FWI_H15_SetShpHorCtrl(&shp_h_ctrl);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpHorCtrl() error.\n", __F__, __L__);
				ret = PE_FWI_H15_SetShpMiscCtrl(&shp_m_ctrl);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetShpMiscCtrl() error.\n", __F__, __L__);
				ret = PE_FWI_H15_SetEdgeGainCtrl(&shp_e_ctrl);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetEdgeGainCtrl() error.\n", __F__, __L__);
				ret = PE_FWI_H15_SetTextureGainCtrl(&shp_t_ctrl);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetTextureGainCtrl() error.\n", __F__, __L__);
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * check f/w status(periodic)
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15A_CheckFwStatus(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if (PE_KDRV_VER_H15)
		{
			if (pInfo->fw_status.src_info != ctrl0_info->fw_src)
			{
				PE_FWI_H15_DBG_PRINT("fw_src changed : %d -> %d.\n", \
					pInfo->fw_status.src_info, ctrl0_info->fw_src);
				if ((pInfo->fw_status.src_info != PE_FWI_H15A_FW_SRC_OFF) && \
					(ctrl0_info->fw_src == PE_FWI_H15A_FW_SRC_OFF))
				{
					PE_FWI_H15_DBG_PRINT("fw_reset status on\n");
					pInfo->fw_status.reset = PE_FWI_H15A_FW_RESET_STATUS_ON;
				}
				pInfo->fw_status.src_info = ctrl0_info->fw_src;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set fw tnr enable
 *	- never use this on sync handler
 *
 * @param   on_off [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetFwTnrEnable(UINT32 on_off)
{
	int ret = RET_OK;
	DE_IPC_CMD_T ipc_cmd = PQL_IPC_SET_TNR_ENABLE;
	do{
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15_DBG_PRINT("set on_off : %d\n", on_off);
			ret = PE_FWI_H15_IPC_SetData(MCU_ID_0, ipc_cmd, &on_off, sizeof(on_off));
			PE_FWI_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_IPC_SetData() error.\n", __F__, __L__);
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * load db data
 *	- never use this on sync handler
 *
 * @param   base_addr [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_LoadDbData(UINT32 base_addr)
{
	int ret = RET_OK;
	DE_IPC_CMD_T ipc_cmd = PQL_IPC_LOAD_DB_DATA;
	do{
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15_DBG_PRINT("base_addr : 0x%08x\n", base_addr);
			ret = PE_FWI_H15_IPC_SetData(MCU_ID_0, ipc_cmd, &base_addr, sizeof(base_addr));
			PE_FWI_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_IPC_SetData() error.\n", __F__, __L__);
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set fw cvi csc
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] LX_PE_CSC_INPUT_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetCviCsc(LX_PE_CSC_INPUT_T *pstParams)
{
	int ret = RET_OK;
	DE_IPC_CMD_T ipc_cmd = DE_IPC_SET_CVI_CSC;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15_DBG_PRINT("set pstParams[%d] : en:%d\n"\
				"coef:%x,%x,%x,%x,%x,%x,%x,%x,%x,"\
				"ofs:%x,%x,%x,%x,%x,%x\n", \
				pstParams->win_id, pstParams->enable, \
				pstParams->matrix[0], pstParams->matrix[1], pstParams->matrix[2], \
				pstParams->matrix[3], pstParams->matrix[4], pstParams->matrix[5], \
				pstParams->matrix[6], pstParams->matrix[7], pstParams->matrix[8], \
				pstParams->offset[0], pstParams->offset[1], pstParams->offset[2], \
				pstParams->offset[3], pstParams->offset[4], pstParams->offset[5]);
			ret = PE_FWI_H15_IPC_SetData(MCU_ID_0, \
					ipc_cmd, (void *)pstParams, sizeof(LX_PE_CSC_INPUT_T));
			PE_FWI_H15_CHECK_CODE(ret, break, \
				"[%s,%d] PE_FWI_H15_IPC_SetData() error.\n", __F__, __L__);
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set sharpness horizontal control to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_HOR_CTRL
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetShpHorCtrl(PE_FWI_H15_HOR_CTRL *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_FOR_BRINGUP
	#else
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			if (!pInfo->fw_ctrl.enable)
			{
				PE_FWI_H15_DBG_PRINT("fw_en:off, e_g12:%d, %d\n", \
					pstParams->e_gain_th1, pstParams->e_gain_th2);
				PE_SHPLR_H15A0_QWr02(pe1_derh_ctrl_0b, e_gain_th1, pstParams->e_gain_th1, \
                                                       e_gain_th2, pstParams->e_gain_th2);
			}
			if ((pInfo->shp_h_ctrl.e_gain_th1!=pstParams->e_gain_th1) || \
				(pInfo->shp_h_ctrl.e_gain_th2!=pstParams->e_gain_th2))
			{
				pTable = (UINT8 *)&table;
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_HOR_CTRL);
				header.length = sizeof(PE_FWI_H15_HOR_CTRL);
				table.shp_h_ctrl.e_gain_th1 = pstParams->e_gain_th1;
				table.shp_h_ctrl.e_gain_th2 = pstParams->e_gain_th2;
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d, e_g12:%d, %d\n", \
					header.win_id, header.offset, header.length, tbl_size, \
					table.shp_h_ctrl.e_gain_th1, table.shp_h_ctrl.e_gain_th2);
				ret = PE_FWI_H15_SetFwSettings(header, pTable, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				pInfo->shp_h_ctrl.e_gain_th1 = table.shp_h_ctrl.e_gain_th1;
				pInfo->shp_h_ctrl.e_gain_th2 = table.shp_h_ctrl.e_gain_th2;
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("e_g12:pre[%d, %d]->cur[%d, %d], same..\n", \
					pInfo->shp_h_ctrl.e_gain_th1, pInfo->shp_h_ctrl.e_gain_th2, \
					pstParams->e_gain_th1, pstParams->e_gain_th2);
				ret = RET_OK;
				break;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * set sharpness misc control to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_MISC_CTRL
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetShpMiscCtrl(PE_FWI_H15_MISC_CTRL *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_FOR_BRINGUP
	#else
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			if (!pInfo->fw_ctrl.enable)
			{
				PE_FWI_H15_DBG_PRINT("fw_en:off, gt01ab:%d, %d, %d, %d\n", \
					pstParams->gt_th0, pstParams->gt_th1, \
					pstParams->gt_th0a, pstParams->gt_th0b);
				PE_SHPLR_H15A0_QWr02(pe1_coring_ctrl_05,	gt_th1,		GET_BITS(pp->gt_th1,0,8), \
															gt_th0,		GET_BITS(pp->gt_th0,0,8));
				PE_SHPLR_H15A0_QWr02(pe1_coring_ctrl_06,	gt_th0a,	GET_BITS(pp->gt_th0a,0,8), \
															gt_th0b,	GET_BITS(pp->gt_th0b,0,8));
			}
			if ((pInfo->shp_m_ctrl.gt_th0!=pstParams->gt_th0) || \
				(pInfo->shp_m_ctrl.gt_th1!=pstParams->gt_th1) || \
				(pInfo->shp_m_ctrl.gt_th0a!=pstParams->gt_th0a) || \
				(pInfo->shp_m_ctrl.gt_th0b!=pstParams->gt_th0b))
			{
				pTable = (UINT8 *)&table;
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_MISC_CTRL);
				header.length = sizeof(PE_FWI_H15_MISC_CTRL);
				table.shp_m_ctrl.gt_th0 = pstParams->gt_th0;
				table.shp_m_ctrl.gt_th1 = pstParams->gt_th1;
				table.shp_m_ctrl.gt_th0a = pstParams->gt_th0a;
				table.shp_m_ctrl.gt_th0b = pstParams->gt_th0b;
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d,"\
					"gt01ab:%d, %d, %d, %d\n", \
					header.win_id, header.offset, header.length, tbl_size, \
					table.shp_m_ctrl.gt_th0, table.shp_m_ctrl.gt_th1, \
					table.shp_m_ctrl.gt_th0a, table.shp_m_ctrl.gt_th0b);
				ret = PE_FWI_H15_SetFwSettings(header, pTable, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				pInfo->shp_m_ctrl.gt_th0 = table.shp_m_ctrl.gt_th0;
				pInfo->shp_m_ctrl.gt_th1 = table.shp_m_ctrl.gt_th1;
				pInfo->shp_m_ctrl.gt_th0a = table.shp_m_ctrl.gt_th0a;
				pInfo->shp_m_ctrl.gt_th0b = table.shp_m_ctrl.gt_th0b;
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("gt01ab:pre[%d, %d, %d, %d]->"\
					"cur[%d, %d, %d, %d], same..\n", \
					pInfo->shp_m_ctrl.gt_th0, pInfo->shp_m_ctrl.gt_th0, \
					pInfo->shp_m_ctrl.gt_th0a, pInfo->shp_m_ctrl.gt_th0b, \
					pstParams->gt_th0, pstParams->gt_th1, \
					pstParams->gt_th0a, pstParams->gt_th0b);
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * set edge gain control to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_EDGE_GAIN
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetEdgeGainCtrl(PE_FWI_H15_EDGE_GAIN *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_FOR_BRINGUP
	#else
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			if (!pInfo->fw_ctrl.enable)
			{
				#if 0	//temp
				UINT8 cur_gain[4];
				PE_INF_H15_SETTINGS_T inf_set;
				ret = PE_INF_H15_GetCurInfSettings(&inf_set);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n", __F__, __L__);
				cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
				CHECK_KNULL(cur0_inf);
				PE_FWI_H15_DBG_PRINT("fw_en:off, src:%d, eg_b, w:%d,%d,%d,%d\n", \
					cur0_inf->src_type, pstParams->co09_gain_b, pstParams->co09_gain_w, \
					pstParams->dp01_gain_b, pstParams->dp01_gain_w);
				/** set 50% edge, texture gain to avoid strong peaking in sub dtv
				* with main atv on split window mode  */
				if (cur0_inf->src_type==LX_PE_SRC_ATV)
				{
					cur_gain[0] = pstParams->co09_gain_b>>1;
					cur_gain[1] = pstParams->co09_gain_w>>1;
					cur_gain[2] = pstParams->dp01_gain_b>>1;
					cur_gain[3] = pstParams->dp01_gain_w>>1;
				}
				else
				{
					cur_gain[0] = pstParams->co09_gain_b;
					cur_gain[1] = pstParams->co09_gain_w;
					cur_gain[2] = pstParams->dp01_gain_b;
					cur_gain[3] = pstParams->dp01_gain_w;
				}
				#endif
				PE_FWI_H15_DBG_PRINT("cur eg_b, w:%d, %d, %d, %d\n", \
					cur_gain[0], cur_gain[1], cur_gain[2], cur_gain[3]);
				PE_SHPLR_H15A0_QWr02(pe1_coring_ctrl_09,edge_gain_b,cur_gain[0], \
														edge_gain_w,cur_gain[1]);
				PE_SHPLR_H15A0_QWr02(pe1_dp_ctrl_01,edge_gain_b,    cur_gain[2], \
													edge_gain_w,    cur_gain[3]);
			}
			if ((pInfo->shp_e_ctrl.co09_gain_b!=pstParams->co09_gain_b) || \
				(pInfo->shp_e_ctrl.co09_gain_w!=pstParams->co09_gain_w) || \
				(pInfo->shp_e_ctrl.dp01_gain_b!=pstParams->dp01_gain_b) || \
				(pInfo->shp_e_ctrl.dp01_gain_w!=pstParams->dp01_gain_w))
			{
				pTable = (UINT8 *)&table;
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_EDGE_GAIN);
				header.length = sizeof(PE_FWI_H15_EDGE_GAIN);
				table.shp_e_ctrl.co09_gain_b = pstParams->co09_gain_b;
				table.shp_e_ctrl.co09_gain_w = pstParams->co09_gain_w;
				table.shp_e_ctrl.dp01_gain_b = pstParams->dp01_gain_b;
				table.shp_e_ctrl.dp01_gain_w = pstParams->dp01_gain_w;
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d,"\
					"eg_b, w:%d, %d, %d, %d\n", \
					header.win_id, header.offset, header.length, tbl_size, \
					table.shp_e_ctrl.co09_gain_b, table.shp_e_ctrl.co09_gain_w, \
					table.shp_e_ctrl.dp01_gain_b, table.shp_e_ctrl.dp01_gain_w);
				ret = PE_FWI_H15_SetFwSettings(header, pTable, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				pInfo->shp_e_ctrl.co09_gain_b = table.shp_e_ctrl.co09_gain_b;
				pInfo->shp_e_ctrl.co09_gain_w = table.shp_e_ctrl.co09_gain_w;
				pInfo->shp_e_ctrl.dp01_gain_b = table.shp_e_ctrl.dp01_gain_b;
				pInfo->shp_e_ctrl.dp01_gain_w = table.shp_e_ctrl.dp01_gain_w;
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("eg_b, w:pre[%d, %d, %d, %d]->"\
					"cur[%d, %d, %d, %d], same..\n", \
					pInfo->shp_e_ctrl.co09_gain_b, pInfo->shp_e_ctrl.co09_gain_w, \
					pInfo->shp_e_ctrl.dp01_gain_b, pInfo->shp_e_ctrl.dp01_gain_w, \
					pstParams->co09_gain_b, pstParams->co09_gain_w, \
					pstParams->dp01_gain_b, pstParams->dp01_gain_w);
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * set texture gain control to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_TEXTURE_GAIN
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetTextureGainCtrl(PE_FWI_H15_TEXTURE_GAIN *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_HW_H15_FOR_BRINGUP
	#else
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			if (!pInfo->fw_ctrl.enable)
			{
				#if 0	//temp
				UINT8 cur_gain[4];
				PE_INF_H15_SETTINGS_T inf_set;
				ret = PE_INF_H15_GetCurInfSettings(&inf_set);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_INF_H15_GetCurInfSettings() error.\n", __F__, __L__);
				cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
				CHECK_KNULL(cur0_inf);
				PE_FWI_H15_DBG_PRINT("fw_en:off, src:%d, tg_b, w:%d, %d, %d, %d\n", \
					cur0_inf->src_type, pstParams->co09_gain_b, pstParams->co09_gain_w, \
					pstParams->dp01_gain_b, pstParams->dp01_gain_w);
				/** set 50% edge, texture gain to avoid strong peaking in sub dtv
				* with main atv on split window mode  */
				if (cur0_inf->src_type==LX_PE_SRC_ATV)
				{
					cur_gain[0] = pstParams->co09_gain_b>>1;
					cur_gain[1] = pstParams->co09_gain_w>>1;
					cur_gain[2] = pstParams->dp01_gain_b>>1;
					cur_gain[3] = pstParams->dp01_gain_w>>1;
				}
				else
				{
					cur_gain[0] = pstParams->co09_gain_b;
					cur_gain[1] = pstParams->co09_gain_w;
					cur_gain[2] = pstParams->dp01_gain_b;
					cur_gain[3] = pstParams->dp01_gain_w;
				}
				#endif
				PE_FWI_H15_DBG_PRINT("cur tg_b, w:%d, %d, %d, %d\n", \
					cur_gain[0], cur_gain[1], cur_gain[2], cur_gain[3]);
				PE_SHPLR_H15A0_QWr02(pe1_coring_ctrl_09,texture_gain_b,cur_gain[0], \
														texture_gain_w,cur_gain[1]);
				PE_SHPLR_H15A0_QWr02(pe1_dp_ctrl_01,texture_gain_b,    cur_gain[2], \
													texture_gain_w,    cur_gain[3]);
			}
			if ((pInfo->shp_t_ctrl.co09_gain_b!=pstParams->co09_gain_b) || \
				(pInfo->shp_t_ctrl.co09_gain_w!=pstParams->co09_gain_w) || \
				(pInfo->shp_t_ctrl.dp01_gain_b!=pstParams->dp01_gain_b) || \
				(pInfo->shp_t_ctrl.dp01_gain_w!=pstParams->dp01_gain_w))
			{
				pTable = (UINT8 *)&table;
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_TEXTURE_GAIN);
				header.length = sizeof(PE_FWI_H15_TEXTURE_GAIN);
				table.shp_t_ctrl.co09_gain_b = pstParams->co09_gain_b;
				table.shp_t_ctrl.co09_gain_w = pstParams->co09_gain_w;
				table.shp_t_ctrl.dp01_gain_b = pstParams->dp01_gain_b;
				table.shp_t_ctrl.dp01_gain_w = pstParams->dp01_gain_w;
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d,"\
					"tg_b, w:%d, %d, %d, %d\n", \
					header.win_id, header.offset, header.length, tbl_size, \
					table.shp_t_ctrl.co09_gain_b, table.shp_t_ctrl.co09_gain_w, \
					table.shp_t_ctrl.dp01_gain_b, table.shp_t_ctrl.dp01_gain_w);
				ret = PE_FWI_H15_SetFwSettings(header, pTable, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				pInfo->shp_t_ctrl.co09_gain_b = table.shp_t_ctrl.co09_gain_b;
				pInfo->shp_t_ctrl.co09_gain_w = table.shp_t_ctrl.co09_gain_w;
				pInfo->shp_t_ctrl.dp01_gain_b = table.shp_t_ctrl.dp01_gain_b;
				pInfo->shp_t_ctrl.dp01_gain_w = table.shp_t_ctrl.dp01_gain_w;
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("tg_b, w:pre[%d, %d, %d, %d]->"\
					"cur[%d, %d, %d, %d], same..\n", \
					pInfo->shp_t_ctrl.co09_gain_b, pInfo->shp_t_ctrl.co09_gain_w, \
					pInfo->shp_t_ctrl.dp01_gain_b, pInfo->shp_t_ctrl.dp01_gain_w, \
					pstParams->co09_gain_b, pstParams->co09_gain_w, \
					pstParams->dp01_gain_b, pstParams->dp01_gain_w);
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * set tnr ctrl to f/w
 *	- never use this on sync handler
 *
 * @param   win_id [in] UINT32
 * @param   *pstParams [in] PE_FWI_H15_TNR_CTRL
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetTnrCtrl(UINT32 win_id, PE_FWI_H15_TNR_CTRL *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID cur_win_id;
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			/* don't care ctrl enable on/off */
			if (win_id>LX_PE_WIN_ALL)
			{
				PE_FWI_H15_DBG_PRINT("win_id:%d, just return\n", win_id);
				ret = RET_OK;
				break;
			}
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			cur_win_id = PE_GET_CHECKED_WINID(win_id);
			if (((win_id==LX_PE_WIN_ALL)&& \
				((pInfo->tnr_ctrl[LX_PE_WIN_0].tnr_en!=pstParams->tnr_en) || \
				(pInfo->tnr_ctrl[LX_PE_WIN_0].m_fir_en!=pstParams->m_fir_en) || \
				(pInfo->tnr_ctrl[LX_PE_WIN_1].tnr_en!=pstParams->tnr_en) || \
				(pInfo->tnr_ctrl[LX_PE_WIN_1].m_fir_en!=pstParams->m_fir_en))) || \
				((pInfo->tnr_ctrl[cur_win_id].tnr_en!=pstParams->tnr_en) || \
				(pInfo->tnr_ctrl[cur_win_id].m_fir_en!=pstParams->m_fir_en)))
			{
				pTable = (UINT8 *)&table;
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = win_id;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_TNR_CTRL);
				header.length = sizeof(PE_FWI_H15_TNR_CTRL);
				table.tnr_ctrl.tnr_en = (pstParams->tnr_en)? 1:0;
				table.tnr_ctrl.m_fir_en = (pstParams->m_fir_en)? 1:0;
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d,"\
					"core, m_en[%d]:%d, %d->%d, %d\n", \
					header.win_id, header.offset, header.length, tbl_size, cur_win_id, \
					pInfo->tnr_ctrl[cur_win_id].tnr_en, pInfo->tnr_ctrl[cur_win_id].m_fir_en, \
					table.tnr_ctrl.tnr_en, table.tnr_ctrl.m_fir_en);
				ret = PE_FWI_H15_SetFwSettings(header, pTable, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				if (PE_CHECK_WIN0(win_id))
				{
					pInfo->tnr_ctrl[LX_PE_WIN_0].tnr_en = table.tnr_ctrl.tnr_en;
					pInfo->tnr_ctrl[LX_PE_WIN_0].m_fir_en = table.tnr_ctrl.m_fir_en;
				}
				if (PE_CHECK_WIN1(win_id))
				{
					pInfo->tnr_ctrl[LX_PE_WIN_1].tnr_en = table.tnr_ctrl.tnr_en;
					pInfo->tnr_ctrl[LX_PE_WIN_1].m_fir_en = table.tnr_ctrl.m_fir_en;
				}
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("wid:%d, core, m_en[%d]:"\
					"pre[%d, %d][%d, %d]->cur[%d, %d], same..\n", \
					win_id, cur_win_id, \
					pInfo->tnr_ctrl[LX_PE_WIN_0].tnr_en, \
					pInfo->tnr_ctrl[LX_PE_WIN_0].m_fir_en, \
					pInfo->tnr_ctrl[LX_PE_WIN_1].tnr_en, \
					pInfo->tnr_ctrl[LX_PE_WIN_1].m_fir_en, \
					pstParams->tnr_en, pstParams->m_fir_en);
				ret = RET_OK;
				break;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set src info to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_SRC_INFO
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetSrcInfo(PE_FWI_H15_SRC_INFO *pstParams)
{
	int ret = RET_OK;
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 tbl_size;
	UINT8 *pTable = NULL;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pstParams);
		if (PE_KDRV_VER_H15)
		{
			#if 0	// independ on fw enable
			if (!pInfo->fw_ctrl.enable)
			{
				PE_FWI_H15_DBG_PRINT("fw_ctrl.enable:%d, just return\n", pInfo->fw_ctrl.enable);
				ret = RET_OK;
				break;
			}
			#endif
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			if ((pInfo->src_info.dtv_mode!=pstParams->dtv_mode) || \
				(pInfo->src_info.hdd_mode!=pstParams->hdd_mode))
			{
				pTable = (UINT8 *)&table;
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_SRC_INFO);
				header.length = sizeof(PE_FWI_H15_SRC_INFO);
				table.src_info.dtv_mode = pstParams->dtv_mode;
				table.src_info.hdd_mode = pstParams->hdd_mode;
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, "\
					"tbl_size:%d, src_info.dtv:%d->%d, hdd:%d->%d\n", \
					header.win_id, header.offset, header.length, \
					tbl_size, pInfo->src_info.dtv_mode, table.src_info.dtv_mode, \
					pInfo->src_info.hdd_mode, table.src_info.hdd_mode);
				ret = PE_FWI_H15_SetFwSettings(header, pTable, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				pInfo->src_info.dtv_mode=pstParams->dtv_mode;
				pInfo->src_info.hdd_mode=pstParams->hdd_mode;
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("src_info.dtv:%d->%d, hdd:%d->%d, same..\n", \
					pInfo->src_info.dtv_mode, pstParams->dtv_mode, \
					pInfo->src_info.hdd_mode, pstParams->hdd_mode);
				ret = RET_OK;
				break;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * set dnr common data to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_DNR_COMMON
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetDnrCommon(PE_FWI_H15_DNR_COMMON *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 i, tbl_size, diff_cnt;
	UINT8 *cd;//curr data
	UINT8 *pd = _g_pe_fwi_h15_info.dnr_common.data;//prev data
	do{
		CHECK_KNULL(pstParams);
		cd = pstParams->data;
		if (PE_KDRV_VER_H15)
		{
			#if 0	// independ on fw enable
			if (!pInfo->fw_ctrl.enable)
			{
				PE_FWI_H15_DBG_PRINT("fw_ctrl.enable:%d, just return\n", pInfo->fw_ctrl.enable);
				ret = RET_OK;
				break;
			}
			#endif
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			diff_cnt = 0;
			for (i=0; i<PE_FWI_H15_DNR_CMN_SIZE; i++)
			{
				diff_cnt = (pd[i]!=cd[i])? 1:0;
				if (diff_cnt)	break;
			}
			if (diff_cnt)
			{
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_DNR_COMMON);
				header.length = sizeof(PE_FWI_H15_DNR_COMMON);
				memcpy(table.dnr_common.data, cd, sizeof(UINT8)*PE_FWI_H15_DNR_CMN_SIZE);
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d\n", \
					header.win_id, header.offset, header.length, tbl_size);
				PE_FWI_H15_DBG_PRINT("cmn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d->\n", \
					pd[0],pd[1],pd[2],pd[3],pd[4],pd[5],pd[6],pd[7],pd[8],pd[9],\
					pd[10],pd[11],pd[12],pd[13],pd[14]);
				PE_FWI_H15_DBG_PRINT("cmn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d.\n", \
					cd[0],cd[1],cd[2],cd[3],cd[4],cd[5],cd[6],cd[7],cd[8],cd[9],\
					cd[10],cd[11],cd[12],cd[13],cd[14]);
				ret = PE_FWI_H15_SetFwSettings(header, (UINT8 *)&table, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				memcpy(pd, cd, sizeof(PE_FWI_H15_DNR_COMMON));
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("cmn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d. same\n", \
					cd[0],cd[1],cd[2],cd[3],cd[4],cd[5],cd[6],cd[7],cd[8],cd[9],\
					cd[10],cd[11],cd[12],cd[13],cd[14]);
				ret = RET_OK;
				break;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * set dnr detail data to f/w
 *	- never use this on sync handler
 *
 * @param   *pstParams [in] PE_FWI_H15_DNR_DETAIL
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_SetDnrDetail(PE_FWI_H15_DNR_DETAIL *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
	PE_FWI_PQ_TABLE_HEAD_T header;
	PE_FWI_H15_PQ_IPC_CTRL table;
	UINT32 i, tbl_size, diff_cnt[2];
	UINT8 *cdd,*cad;//curr data
	UINT8 *pdd = _g_pe_fwi_h15_info.dnr_detail.dc_bnr;//prev data
	UINT8 *pad = _g_pe_fwi_h15_info.dnr_detail.ac_bnr;//prev data
	do{
		CHECK_KNULL(pstParams);
		cdd = pstParams->dc_bnr;
		cad = pstParams->ac_bnr;
		if (PE_KDRV_VER_H15)
		{
			#if 0	// independ on fw enable
			if (!pInfo->fw_ctrl.enable)
			{
				PE_FWI_H15_DBG_PRINT("fw_ctrl.enable:%d, just return\n", pInfo->fw_ctrl.enable);
				ret = RET_OK;
				break;
			}
			#endif
			PE_FWI_H15A_CHECK_REFRESH_TBL();
			diff_cnt[0] = diff_cnt[1] = 0;
			for (i=0; i<PE_FWI_H15_DNR_DCB_SIZE; i++)
			{
				diff_cnt[0] = (pdd[i]!=cdd[i])? 1:0;
				if (diff_cnt[0])	break;
			}
			for (i=0; i<PE_FWI_H15_DNR_ACB_SIZE; i++)
			{
				diff_cnt[1] = (pad[i]!=cad[i])? 1:0;
				if (diff_cnt[1])	break;
			}
			if (diff_cnt[0]||diff_cnt[1])
			{
				tbl_size = sizeof(PE_FWI_H15_PQ_IPC_CTRL);
				header.win_id = LX_PE_WIN_0;
				header.offset = PE_FWI_OFFSET(PE_FWI_H15_DNR_DETAIL);
				header.length = sizeof(PE_FWI_H15_DNR_DETAIL);
				memcpy(table.dnr_detail.dc_bnr, cdd, sizeof(UINT8)*PE_FWI_H15_DNR_DCB_SIZE);
				memcpy(table.dnr_detail.ac_bnr, cad, sizeof(UINT8)*PE_FWI_H15_DNR_ACB_SIZE);
				PE_FWI_H15_DBG_PRINT("\n wid:%d, ofs:%d, len:%d, tbl_size:%d\n", \
					header.win_id, header.offset, header.length, tbl_size);
				PE_FWI_H15_DBG_PRINT("dbn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d\n", \
					pdd[0],pdd[1],pdd[2],pdd[3],pdd[4],pdd[5],pdd[6],pdd[7],pdd[8],pdd[9],\
					pdd[10],pdd[11],pdd[12],pdd[13],pdd[14]);
				PE_FWI_H15_DBG_PRINT("dbn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d->\n", \
					pdd[15],pdd[16],pdd[17],pdd[18],pdd[19],pdd[20],pdd[21],pdd[22],pdd[23],pdd[24],\
					pdd[25],pdd[26],pdd[27],pdd[28],pdd[29]);
				PE_FWI_H15_DBG_PRINT("dbn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d\n", \
					cdd[0],cdd[1],cdd[2],cdd[3],cdd[4],cdd[5],cdd[6],cdd[7],cdd[8],cdd[9],\
					cdd[10],cdd[11],cdd[12],cdd[13],cdd[14]);
				PE_FWI_H15_DBG_PRINT("dbn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d.\n", \
					cdd[15],cdd[16],cdd[17],cdd[18],cdd[19],cdd[20],cdd[21],cdd[22],cdd[23],cdd[24],\
					cdd[25],cdd[26],cdd[27],cdd[28],cdd[29]);
				PE_FWI_H15_DBG_PRINT("abn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d\n", \
					pad[0],pad[1],pad[2],pad[3],pad[4],pad[5],pad[6],pad[7],pad[8],pad[9],\
					pad[10],pad[11],pad[12],pad[13],pad[14]);
				PE_FWI_H15_DBG_PRINT("abn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d->\n", \
					pad[15],pad[16],pad[17],pad[18],pad[19],pad[20],pad[21],pad[22],pad[23],pad[24],\
					pad[25],pad[26],pad[27]);
				PE_FWI_H15_DBG_PRINT("abn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d\n", \
					cad[0],cad[1],cad[2],cad[3],cad[4],cad[5],cad[6],cad[7],cad[8],cad[9],\
					cad[10],cad[11],cad[12],cad[13],cad[14]);
				PE_FWI_H15_DBG_PRINT("abn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d.\n", \
					cad[15],cad[16],cad[17],cad[18],cad[19],cad[20],cad[21],cad[22],cad[23],cad[24],\
					cad[25],cad[26],cad[27]);
				ret = PE_FWI_H15_SetFwSettings(header, (UINT8 *)&table, tbl_size);
				PE_FWI_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_SetFwSettings() error.\n", __F__, __L__);
				memcpy(pdd, cdd, sizeof(UINT8)*PE_FWI_H15_DNR_DCB_SIZE);
				memcpy(pad, cad, sizeof(UINT8)*PE_FWI_H15_DNR_ACB_SIZE);
			}
			else
			{
				PE_FWI_H15_DBG_PRINT("dbn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d\n", \
					cdd[0],cdd[1],cdd[2],cdd[3],cdd[4],cdd[5],cdd[6],cdd[7],cdd[8],cdd[9],\
					cdd[10],cdd[11],cdd[12],cdd[13],cdd[14]);
				PE_FWI_H15_DBG_PRINT("dbn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d.same.\n", \
					cdd[15],cdd[16],cdd[17],cdd[18],cdd[19],cdd[20],cdd[21],cdd[22],cdd[23],cdd[24],\
					cdd[25],cdd[26],cdd[27],cdd[28],cdd[29]);
				PE_FWI_H15_DBG_PRINT("abn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d\n", \
					cad[0],cad[1],cad[2],cad[3],cad[4],cad[5],cad[6],cad[7],cad[8],cad[9],\
					cad[10],cad[11],cad[12],cad[13],cad[14]);
				PE_FWI_H15_DBG_PRINT("abn:%d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %d,%d,%d.same.\n", \
					cad[15],cad[16],cad[17],cad[18],cad[19],cad[20],cad[21],cad[22],cad[23],cad[24],\
					cad[25],cad[26],cad[27]);
				ret = RET_OK;
				break;
			}
		}
		else
		{
			PE_FWI_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	#endif
	return ret;
}
/**
 * get f/w settings
 *
 * @param   *pstParams [in] PE_FWI_H15_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_FWI_H15_GetCurFwiSettings(PE_FWI_H15_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_FWI_H15_SETTINGS_T *pInfo=&_g_pe_fwi_h15_info;
	do{
		CHECK_KNULL(pstParams);
		memcpy(pstParams, pInfo, sizeof(PE_FWI_H15_SETTINGS_T));
	}while (0);
	return ret;
}

