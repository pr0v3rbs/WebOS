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
 *  @author	Kyungseok Hyun ( ks.hyun@lge.com)
 *  @version	1.0
 *  @date		2014-08-14
 *  @note		Additional information.
 */


/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "sdec_impl.h"

#include "sys_regs.h"

#include "sdec_io.h"
#include "sdec_mm.h"
#include "sdec_ciplus.h"

#include "sdec_ciplus_mcu_bin.h"
#include "sdec_ciplus_mcu_reg.h"
#include "sdec_ciplus_ipc_reg.h"



/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/
#define SDEC_CIPLUS_MODULE		"sdec_ciplus"
#define SDEC_CIPLUS_MAX_CH		4

#define PID_FLT_NUM				32
#define MCU_IPC_MAX_TIMEOUT		500		/* ms */
#define MCU_MEM_SIZE			(2*1024*1024)	/* 2MB */

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
enum
{
	LOG_LEVEL_TRACE_EX = LX_LOGM_LEVEL_TRACE + 1,
	LOG_LEVEL_MCU,
	LOG_LEVEL_IPC,
};

#define sdec_ciplus_log(level,fmt,args...)	DBG_PRINT(g_sdec_ciplus_debug_fd, level, fmt, ##args)

#define log_error(fmt,args...)		sdec_ciplus_log(LX_LOGM_LEVEL_ERROR, fmt, ##args)
#define log_warning(fmt,args...)	sdec_ciplus_log(LX_LOGM_LEVEL_WARNING, fmt, ##args)
#define log_noti(fmt,args...)		sdec_ciplus_log(LX_LOGM_LEVEL_NOTI, fmt, ##args)
#define log_info(fmt,args...)		sdec_ciplus_log(LX_LOGM_LEVEL_INFO, fmt, ##args)
#define log_debug(fmt,args...)		sdec_ciplus_log(LX_LOGM_LEVEL_DEBUG, fmt, ##args)
#define log_trace(fmt,args...)		sdec_ciplus_log(LX_LOGM_LEVEL_TRACE, fmt, ##args)

#define log_trace_ex(fmt,args...)	sdec_ciplus_log(LOG_LEVEL_TRACE_EX, fmt, ##args)
#define log_mcu(fmt,args...)		sdec_ciplus_log(LOG_LEVEL_MCU, fmt, ##args)
#define log_ipc(fmt,args...)		sdec_ciplus_log(LOG_LEVEL_IPC, fmt, ##args)



#define reg_write(addr, val)		__raw_writel(val , (void*)(addr))
#define reg_read(addr)				__raw_readl((void*)(addr))

#define mcu_reg_write(ofs,val)		reg_write(dev->mcu.reg_base+(ofs),val)
#define mcu_reg_read(ofs)			reg_read(dev->mcu.reg_base+(ofs))



#define CHECK_ERROR(__checker,__if_action,fmt,args...)   \
				 __CHECK_IF_ERROR(__checker, log_error, __if_action , fmt, ##args )



#define CHECK_CHANNEL(ch, action)				\
	do {										\
		if(ch == NULL) {				 		\
			log_error("invalid channel\n");		\
			action; 							\
		}										\
	} while(0)

#define CHECK_CHANNEL_OPENED(ch, action)					\
	do {													\
		CHECK_CHANNEL(ch, action);							\
		if(ch->status != CHANNEL_STATUS_OPENED) {			\
			log_error("not opened channel[%d]\n", ch->idx);	\
			action;											\
		}													\
	} while(0)


#define CHANNEL_CTRL(i)		(dev && i<SDEC_CIPLUS_MAX_CH) ?	&dev->ch[i] : NULL


#define IPC_CMD_CHANNEL_CONF(i)		(0x04 + (i)*4)
#define IPC_CMD_CHANNEL_OUT			0x14
#define IPC_CMD_PIDF				0x18
#define IPC_CMD_INPUT_BUFFER(i)		(0x20 + (i)*0x10)
#define IPC_CMD_INPUT_WPTR(i)		(0x28 + (i)*0x10)

#define IPC_CMD_OUTPUT_BUFFER(i)	(0x60 + (i)*0x10)
#define IPC_CMD_OUTPUT_RPTR(i)		(0x6C + (i)*0x10)

#define IPC_CMD_PRINTF_CTRL			0xE0
#define IPC_CMD_MCU_CMD				0xF0



#define SDEC_CIPLUS_ROM_OFFSET		0x0
#define SDEC_CIPLUS_ROM_SIZE		0x20000		/* 128KB */

#define SDEC_CIPLUS_RAM_OFFSET		(SDEC_CIPLUS_ROM_OFFSET + SDEC_CIPLUS_ROM_SIZE)
#define SDEC_CIPLUS_RAM_SIZE		0x00100000	/* 1MB */

#define MCU_PRINTF_BUFFER_OFFSET	(SDEC_CIPLUS_RAM_OFFSET + SDEC_CIPLUS_RAM_SIZE)
#define MCU_PRINTF_BUFFER_SIZE		0x1000		/* 4KB */


/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32		mcu_reg_addr;
	int 		irq_num;

} CONFIG_T;

enum
{
	PID_FLT_STATUS_FREE = 0,
	PID_FLT_STATUS_ALLOC,
	PID_FLT_STATUS_USED
};

enum
{
	CHANNEL_STATUS_CLOSED = 0,
	CHANNEL_STATUS_ALLOC,
	CHANNEL_STATUS_OPENED,
};

typedef struct
{
	UINT8		status;
	UINT16		pid;
} PID_FILTER_T;

typedef struct
{
	UINT32		base;
	UINT32		end;
	UINT32		size;
} BUFFER_INFO_T;

typedef struct
{
	int						idx;
	struct mutex			lock;
	int						status;

	CIPLUS_IPC_CHAN_CONF	conf;
	PID_FILTER_T			pidf[PID_FLT_NUM];

} CHANNEL_CTRL_T;


typedef struct
{
	UINT32		rom_addr;
	UINT32		ram_addr;
	UINT32		print_buf_addr;

	UINT8		*reg_base;
	char		*printf_buffer;

} MCU_T;

typedef struct
{
	struct completion	completion;
	struct mutex		lock;

} IPC_T;

typedef struct
{
	CHANNEL_CTRL_T			ch[SDEC_CIPLUS_MAX_CH];

	CONFIG_T				cfg;

	SDEC_MM_CTX_T			*mem_ctx;
	UINT8					*mem_base;

	MCU_T					mcu;
	IPC_T					ipc;

} SDEC_CIPLUS_DEV_T;


/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Functions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   global Variables
 *---------------------------------------------------------------------------------------*/
int 	g_sdec_ciplus_debug_fd = -1;

/*----------------------------------------------------------------------------------------
 *   Static Function Prototypes Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
static SDEC_CIPLUS_DEV_T *dev;

static volatile CIPLUS_IPC_TOP_REG_T *ipc_reg;


static int load_mcu_fw(void)
{
	void* fw_addr;
	int fw_size;

	/* 1. Copy MCU Firmware */
	fw_size = sizeof(ciplus_mcu_bin);
	if(fw_size > SDEC_CIPLUS_ROM_SIZE)
	{
		log_error("fw size(%d) over max(%d)\n", fw_size, SDEC_CIPLUS_ROM_SIZE);
		return -1;
	}

	fw_addr = (void*)ioremap(dev->mcu.rom_addr, SDEC_CIPLUS_ROM_SIZE);
	if(!fw_addr)
	{
		log_error("ioremap fail\n");
		return -1;
	}
	memcpy(fw_addr, ciplus_mcu_bin, fw_size);
	iounmap(fw_addr);

	/* 2. MCU Control */
	// set rom,ram base
	mcu_reg_write(MCU_REG_OFFSET_SROM_OFFSET, dev->mcu.rom_addr);
	mcu_reg_write(MCU_REG_OFFSET_SRAM_OFFSET_0, dev->mcu.ram_addr);
	mcu_reg_write(MCU_REG_OFFSET_SRAM_OFFSET_1, dev->mcu.ram_addr);
	mcu_reg_write(MCU_REG_OFFSET_SRAM_OFFSET_2, dev->mcu.print_buf_addr);

	// 4. Reset
	mcu_reg_write(MCU_REG_OFFSET_PROC_CTRL, 0x10000005);
	OS_MsecSleep(1);
	mcu_reg_write(MCU_REG_OFFSET_PROC_CTRL, 0x00000004);

	return RET_OK;
}


static int init_mcu(void)
{
	int rc;

	dev->mcu.reg_base = (UINT8*)ioremap(dev->cfg.mcu_reg_addr, 4096);
	if(!dev->mcu.reg_base)
	{
		log_error("ioremap fail\n");
		return -1;
	}

	ipc_reg = (CIPLUS_IPC_TOP_REG_T*)(dev->mcu.reg_base + 0x100);

	dev->mcu.rom_addr = (UINT32)dev->mem_base + SDEC_CIPLUS_ROM_OFFSET;
	dev->mcu.ram_addr = (UINT32)dev->mem_base + SDEC_CIPLUS_RAM_OFFSET;
	dev->mcu.print_buf_addr = (UINT32)dev->mem_base + MCU_PRINTF_BUFFER_OFFSET;

	rc = load_mcu_fw();
	if(rc < 0)
	{
		return rc;
	}
	dev->mcu.printf_buffer = (UINT8*)ioremap(dev->mcu.print_buf_addr, MCU_PRINTF_BUFFER_SIZE);

	return 0;
}

static int IPC_SendCmd(UINT32 cmd)
{
	int rc;
	int ret;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
	reinit_completion(&dev->ipc.completion);
#else
	INIT_COMPLETION(dev->ipc.completion);
#endif

	log_ipc("ipc cmd:0x%08x", cmd);

	ipc_reg->ipc_cmd_addr = cmd;
	mcu_reg_write(MCU_REG_OFFSET_E_TO_I_EV, 1);

	rc = wait_for_completion_timeout(&dev->ipc.completion, msecs_to_jiffies(MCU_IPC_MAX_TIMEOUT));
	if(rc <= 0)
	{
		log_warning("ipc cmd[0x%x] not completed. rc=%d\n", cmd, rc);
		ret = RET_ERROR;
	}
	else
	{
		log_ipc("ipc cmd[0x%x] done. rc=%d\n", cmd, rc);
		ret = RET_OK;
	}

	return ret;
}

static int IPC_CMD_SetPrintCtrl(UINT32 value)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->print_control = value;
	rc = IPC_SendCmd(IPC_CMD_PRINTF_CTRL);

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_SetChanConf(UINT8 idx, CIPLUS_IPC_CHAN_CONF *conf)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->chan_conf[idx] = *conf;
	rc = IPC_SendCmd(IPC_CMD_CHANNEL_CONF(idx));

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_SetInputBuffer(UINT8 idx, CIPLUS_IPC_DDR_CONF *conf)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->input_ddr_conf[idx] = *conf;
	rc = IPC_SendCmd(IPC_CMD_INPUT_BUFFER(idx));

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_SetOutputBuffer(UINT8 idx,CIPLUS_IPC_DDR_CONF *conf)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->input_ddr_conf[idx] = *conf;
	rc = IPC_SendCmd(IPC_CMD_OUTPUT_BUFFER(idx));

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_GetInputBufferPtr(UINT8 idx, UINT32 *wptr, UINT32 *rptr)
{
	mutex_lock (&dev->ipc.lock);

	*wptr = ipc_reg->input_ddr_conf[idx].wptr;
	*rptr = ipc_reg->input_ddr_conf[idx].rptr;

	mutex_unlock(&dev->ipc.lock);

	return 0;
}

static int IPC_CMD_UpdateInputBufferWptr(UINT8 idx, UINT32 wptr)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->input_ddr_conf[idx].wptr = wptr;
	rc = IPC_SendCmd(IPC_CMD_INPUT_WPTR(idx));

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_GetOutputBufferPtr(UINT8 idx, UINT32 *wptr, UINT32 *rptr)
{
	mutex_lock (&dev->ipc.lock);

	*wptr = ipc_reg->output_ddr_conf[idx].wptr;
	*rptr = ipc_reg->output_ddr_conf[idx].rptr;

	mutex_unlock(&dev->ipc.lock);

	return 0;
}

static int IPC_CMD_UpdateOutputBufferRptr(UINT8 idx, UINT32 rptr)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->output_ddr_conf[idx].rptr = rptr;
	rc = IPC_SendCmd(IPC_CMD_OUTPUT_RPTR(idx));

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_SetPidFilter(UINT8 idx, UINT32 fltid, UINT16 pid)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->pidf_index.chan_idx = idx;
	ipc_reg->pidf_index.filter_idx = fltid;
	ipc_reg->pidf_data.ref_pid = pid;
	ipc_reg->pidf_data.en = 1;
	rc = IPC_SendCmd(IPC_CMD_PIDF);

	mutex_unlock(&dev->ipc.lock);

	return rc;
}

static int IPC_CMD_FreePidFilter(UINT8 idx, UINT32 fltid)
{
	int rc;

	mutex_lock (&dev->ipc.lock);

	ipc_reg->pidf_index.chan_idx = idx;
	ipc_reg->pidf_index.filter_idx = fltid;
	ipc_reg->pidf_data.en = 0;
	rc = IPC_SendCmd(IPC_CMD_PIDF);

	mutex_unlock(&dev->ipc.lock);

	return rc;
}


static void init_debug(void)
{
	g_sdec_ciplus_debug_fd = DBG_OPEN( SDEC_CIPLUS_MODULE );
	OS_DEBUG_EnableModule ( g_sdec_ciplus_debug_fd );
	OS_DEBUG_EnableModuleByIndex ( g_sdec_ciplus_debug_fd, LX_LOGM_LEVEL_NOTI, DBG_COLOR_GREEN );	/* NOTI */

	LOGM_RegBitName(g_sdec_ciplus_debug_fd, LOG_LEVEL_TRACE_EX, "TRACE_EX");
	LOGM_RegBitName(g_sdec_ciplus_debug_fd, LOG_LEVEL_MCU, "MCU");
	LOGM_RegBitName(g_sdec_ciplus_debug_fd, LOG_LEVEL_IPC, "IPC");
}

static int init_memory(void)
{
	dev->mem_ctx = SDEC_MM_Open();
	if(!dev->mem_ctx)
	{
		log_error("can't open mm\n");
		return -1;
	}

	dev->mem_base = (UINT8*)SDEC_MM_Alloc(dev->mem_ctx, SDEC_MM_TYPE_CIPLUS, MCU_MEM_SIZE);
	if(!dev->mem_base)
	{
		log_error("can't alloc memory\n");
		return -1;
	}

	return 0;
}

static int init_config(void)
{
	UINT32 chip = lx_chip();

	if(0){}
	else if(chip == LX_CHIP_H15)
	{
		dev->cfg.mcu_reg_addr = H15_TE_BASE + 0x3800;
		dev->cfg.irq_num = H15_IRQ_TE3;
	}
	else if(chip == LX_CHIP_M14 && lx_chip_rev() >= LX_CHIP_REV(M14,C0))
	{
		// TODO:
		// cfg.irq_num = xxx;
	}
	else
	{
		log_error("not supported chip\n");
		return -1;
	}

	return 0;
}

static int init_ipc(void)
{
	mutex_init (&dev->ipc.lock);
	init_completion(&dev->ipc.completion);

	ipc_reg->mcu_cmd_ipc_done = 0;
	ipc_reg->mcu_cmd_printf = 0;

	return 0;
}

static int init_channel(void)
{
	int i;
	CHANNEL_CTRL_T *ch;

	for(i=0; i<SDEC_CIPLUS_MAX_CH; i++)
	{
		ch = &dev->ch[i];
		ch->idx = i;
		mutex_init (&ch->lock);
	}

	return 0;
}


static irqreturn_t mcu_interrupt(int irq, void *dev_id)
{
	UINT32 status;
	SDEC_CIPLUS_DEV_T *c = (SDEC_CIPLUS_DEV_T*)dev_id;

	status = mcu_reg_read(MCU_REG_OFFSET_E_INTR_ST);
	if(!status)
	{
		return IRQ_NONE;
	}
	mcu_reg_write(MCU_REG_OFFSET_E_INTR_CL, status);

	log_debug("irq status:0x%08x\n", status);
	if((status&MCU_INTR_IPC_MASK))
	{
		if(ipc_reg->mcu_cmd_ipc_done)
		{
			complete(&c->ipc.completion);
			ipc_reg->mcu_cmd_ipc_done = 0;
		}

		if(ipc_reg->mcu_cmd_printf)
		{
			log_mcu(dev->mcu.printf_buffer);
			ipc_reg->mcu_cmd_printf = 0;
		}
	}

	return IRQ_HANDLED;
}

#if 0
static void free_all_pid_filter(CHANNEL_CTRL_T *ch_ctrl)
{
	int i;

	for(i=0; i<PID_FLT_NUM; i++)
	{
		ch_ctrl->pidf[i].status = PID_FLT_STATUS_FREE;
		ch_ctrl->pidf[i].pid = 0x2000;	/* invalid pid */
	}
}
#endif

static int get_free_pid_filter(CHANNEL_CTRL_T *ch)
{
	int i;

	for(i=0; i<PID_FLT_NUM; i++)
	{
		if(ch->pidf[i].status == PID_FLT_STATUS_FREE)
			return i;
	}
	return -1;
}

static int get_used_pid_filter(CHANNEL_CTRL_T *ch, UINT16 pid)
{
	int i;

	for(i=0; i<PID_FLT_NUM; i++)
	{
		if(ch->pidf[i].status == PID_FLT_STATUS_USED &&
			ch->pidf[i].pid == pid)
			return i;
	}
	return -1;
}

int SDEC_CIPLUS_AllocChannel(void)
{
	int i;
	CHANNEL_CTRL_T *ch;

	for(i=0; i<SDEC_CIPLUS_MAX_CH; i++)
	{
		ch = &dev->ch[i];
		mutex_lock(&ch->lock);
		if(ch->status == CHANNEL_STATUS_CLOSED)
		{
			ch->status = CHANNEL_STATUS_ALLOC;
			mutex_unlock(&ch->lock);
			return i;
		}
		mutex_unlock(&ch->lock);
	}

	return -1;
}

int SDEC_CIPLUS_OpenChannel(UINT8 idx)
{
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	if(ch == NULL)
	{
		log_error("can't get channel[%d]\n", idx);
		return RET_ERROR;
	}

	mutex_lock(&ch->lock);
	if(ch->status == CHANNEL_STATUS_OPENED)
	{
		log_error("Already opened channel[%d]\n", idx);
		mutex_unlock(&ch->lock);
		return RET_ERROR;
	}

	log_trace("CH[%d]\n", idx);

	memset(&ch->conf, 0, sizeof(ch->conf));
	memset(&ch->pidf[0], 0, sizeof(ch->pidf));

	ch->conf.reset = 1;
	IPC_CMD_SetChanConf(idx, &ch->conf);
	ch->conf.reset = 0;

	ch->status = CHANNEL_STATUS_OPENED;

	mutex_unlock(&ch->lock);

	return RET_OK;
}


int SDEC_CIPLUS_CloseChannel(UINT8 idx)
{
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL_OPENED(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	log_trace("CH[%d]\n", idx);

	// TODO: Add Lock
	if(ch->conf.en)
	{
		ch->conf.en = 0;
		IPC_CMD_SetChanConf(idx, &ch->conf);
	}
	ch->status = CHANNEL_STATUS_CLOSED;
	mutex_unlock(&ch->lock);

	return RET_OK;
}

int SDEC_CIPLUS_ConfigureChannel(UINT8 idx, LX_SDEC_CIPLUS_CHANNEL_CONF_T *conf)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	log_trace("CH[%d]\n", idx);

	ch->conf.src_sel	= conf->src;
	ch->conf.synctype	= conf->sync_type;
	ch->conf.bypass		= conf->bypass;
	ch->conf.tei		= conf->transport_error_indicator;
	ch->conf.lts_en		= conf->local_ts_id_en;
	ch->conf.lts_id		= conf->local_ts_id;

	if(ch->conf.en)
	{
		IPC_CMD_SetChanConf(idx, &ch->conf);
	}
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}


int SDEC_CIPLUS_AddPidFilter(UINT8 idx, UINT16 *pids, UINT32 npid)
{
	int rc = RET_ERROR;
	int flt_id, i;
	UINT16 pid;
	int count;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);
	CHECK_ERROR(pids == NULL, return RET_INVALID_PARAMS, "null pid");
	CHECK_ERROR(npid == 0, return RET_INVALID_PARAMS, "npid is 0");

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	count = 0;
	for(i=0; i<npid; i++)
	{
		pid = pids[i];
		if(pid > 0x1fff)
		{
			log_warning("invalid pid[%d]:0x%x\n", i, pid);
			continue;
		}

		flt_id = get_used_pid_filter(ch, pid);
		if(flt_id < 0)
		{
			flt_id = get_free_pid_filter(ch);
			if(flt_id < 0)
			{
				log_warning("no available filter\n");
				break;
			}

			IPC_CMD_SetPidFilter(idx, flt_id, pid);

			ch->pidf[flt_id].status = PID_FLT_STATUS_USED;
			ch->pidf[flt_id].pid = pid;
		}
		count++;
	}
	rc = count;
exit:
	mutex_unlock(&ch->lock);
	return rc;
}


int SDEC_CIPLUS_RemovePidFilter(UINT8 idx, UINT16 *pids, UINT32 npid)
{
	int rc = RET_ERROR;
	int flt_id, i;
	UINT16 pid;
	int count;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);
	CHECK_ERROR(pids == NULL, return RET_INVALID_PARAMS, "null pid");
	CHECK_ERROR(npid == 0, return RET_INVALID_PARAMS, "npid is 0");

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	count = 0;
	for(i=0; i<npid; i++)
	{
		pid = pids[i];
		if(pid > 0x1fff)
		{
			log_warning("invalid pid[%d]:0x%x\n", i, pid);
			continue;
		}

		flt_id = get_used_pid_filter(ch, pid);
		if(flt_id < 0)
		{
			log_warning("not matched pid[%d]:0x%x\n", i, pid);
			continue;
		}

		IPC_CMD_FreePidFilter(idx, flt_id);
		ch->pidf[flt_id].status = PID_FLT_STATUS_FREE;
		count++;
	}
	rc = count;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}

int SDEC_CIPLUS_GetPidFilter(UINT8 idx, UINT16 *pids, UINT32 maxpid)
{
	int rc = RET_ERROR;
	int i;
	int count;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);
	CHECK_ERROR(pids == NULL, return RET_INVALID_PARAMS, "null pid");
	CHECK_ERROR(maxpid == 0, return RET_INVALID_PARAMS, "maxpid is 0");

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	count = 0;
	for(i=0; i<PID_FLT_NUM; i++)
	{
		if(ch->pidf[i].status == PID_FLT_STATUS_USED)
		{
			pids[count] = ch->pidf[i].pid;
			count++;
			if(count >= maxpid)	break;
		}
	}
	rc = count;
exit:
	mutex_unlock(&ch->lock);

	return rc;

}


int SDEC_CIPLUS_EnableChannel(UINT8 idx)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	log_trace("CH[%d]\n", idx);

	ch->conf.en = 1;

	IPC_CMD_SetChanConf(idx, &ch->conf);
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}

int SDEC_CIPLUS_DisableChannel(UINT8 idx)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	log_trace("CH[%d]\n", idx);

	ch->conf.en = 0;

	IPC_CMD_SetChanConf(idx, &ch->conf);
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;

}


int SDEC_CIPLUS_SetInputBuffer(UINT8 idx, UINT32 addr, UINT32 size)
{
	int rc = RET_ERROR;
	CIPLUS_IPC_DDR_CONF conf;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);


	log_trace("idx[%d], addr:0x%08x, size:%d\n", idx, addr, size);

	conf.sptr = addr;
	conf.eptr = addr + size;
	conf.wptr = addr;
	conf.rptr = addr;

	IPC_CMD_SetInputBuffer(idx, &conf);

	// TODO: when input buffer is set, have to init w/r ptr also in MCU
	IPC_CMD_UpdateInputBufferWptr(idx, conf.wptr);

	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;

}

int SDEC_CIPLUS_GetInputBufferPtr(UINT8 idx, UINT32 *wptr, UINT32 *rptr)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	IPC_CMD_GetInputBufferPtr(idx, wptr, rptr);

	log_trace_ex("idx[%d], wptr:0x%08x, rptr:0x%08x\n", idx, *wptr, *rptr);

	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}

int SDEC_CIPLUS_UpdateInputBufferWptr(UINT8 idx, UINT32 wptr)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	log_trace_ex("idx[%d], wptr:0x%08x\n", idx, wptr);

	IPC_CMD_UpdateInputBufferWptr(idx, wptr);
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}


int SDEC_CIPLUS_SetOutputBuffer(UINT8 idx, UINT32 addr, UINT32 size)
{
	int rc = RET_ERROR;
	CIPLUS_IPC_DDR_CONF conf;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	conf.sptr = addr;
	conf.eptr = addr + size;
	conf.wptr = addr;
	conf.rptr = addr;

	IPC_CMD_SetOutputBuffer(idx, &conf);
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}

int SDEC_CIPLUS_UpdateOutputBufferRptr(UINT8 idx, UINT32 rptr)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	log_trace("idx[%d], rptr:0x%08x\n", idx, rptr);

	IPC_CMD_UpdateOutputBufferRptr(idx, rptr);
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}


int SDEC_CIPLUS_GetOutputBufferPtr(UINT8 idx, UINT32 *wptr, UINT32 *rptr)
{
	int rc = RET_ERROR;
	CHANNEL_CTRL_T *ch = CHANNEL_CTRL(idx);

	CHECK_CHANNEL(ch, return RET_ERROR);

	mutex_lock(&ch->lock);
	CHECK_CHANNEL_OPENED(ch, goto exit);

	IPC_CMD_GetOutputBufferPtr(idx, wptr, rptr);
	rc = RET_OK;
exit:
	mutex_unlock(&ch->lock);

	return rc;
}

int SDEC_CIPLUS_SetOutClock(LX_SDEC_CIPLUS_OUT_CLOCK_T clock)
{
	UINT32 value;

	// TODO: Add Lock
	if(lx_chip() == LX_CHIP_H15)
	{
		switch(clock)
		{
			case LX_SDEC_CIPLUS_OUT_CLOCK_HIGH: value = 0; break;
			case LX_SDEC_CIPLUS_OUT_CLOCK_LOW: value = 2; break;
			case LX_SDEC_CIPLUS_OUT_CLOCK_DEFAULT:
			default:
				value = 1;
				break;
		}
		CTOP_CTRL_H15A0_RdFL(TI, ctr08);
		CTOP_CTRL_H15A0_Wr01(TI, ctr08, sel_ciout, value);
		CTOP_CTRL_H15A0_WrFL(TI, ctr08);
	}

	return RET_OK;
}

int SDEC_CIPLUS_Init(void)
{
	int ret;
	LX_SDEC_CFG_T* sdec_cfg = SDEC_CFG_GetConfig();

	if(dev != NULL)
	{
		// already inited
		return RET_OK;
	}

	init_debug();

	if(!sdec_cfg->ciPlus)
		return RET_OK;

	dev = kcalloc(1, sizeof(SDEC_CIPLUS_DEV_T), GFP_KERNEL);
	CHECK_ERROR(dev == NULL, return RET_ERROR, "can't alloc memory");

	if(init_config() < 0)
		return RET_ERROR;

	if(init_memory() < 0)
		goto error;

	if(init_mcu() < 0)
		goto error;

	init_ipc();
	init_channel();

	SDEC_CIPLUS_SetOutClock(LX_SDEC_CIPLUS_OUT_CLOCK_DEFAULT);

	/* initialize interrupt */
	ret = request_irq (dev->cfg.irq_num, mcu_interrupt, 0, SDEC_CIPLUS_MODULE, dev);
	if (ret < 0)
	{
		log_error ("dev:%s, request_irq failed. %d\n", SDEC_CIPLUS_MODULE, ret);
		return ret;
	}

	/* enable interrupt */
	mcu_reg_write(MCU_REG_OFFSET_E_INTR_EN, MCU_INTR_IPC_MASK);

	return RET_OK;

error:
	if(dev) kfree(dev);
	dev = NULL;

	return RET_ERROR;
}

void SDEC_CIPLUS_Cleanup(void)
{

}

int SDEC_CIPLUS_Suspend(void)
{
	return 0;
}

int SDEC_CIPLUS_Resume(void)
{
	return 0;
}

