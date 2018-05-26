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
 *	GFX hw control
 *
 *  author      sinchul.jung (sinchul.jung@lge.com)
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg1150_gfx
 *  @{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define GFX_PALETTE_AUTO_INC
#define GFX_AUTO_VH_ORDER
#define GFX_AUTO_CMD_DELAY

#ifdef GFX_AUTO_CMD_DELAY
#include "de_kapi.h"
extern int DE_REG_H13_GetSourceStatus(LX_DE_SRC_STATUS_T *pstParams);
#endif

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "gfx_impl.h"
#include "gfx_reg_h13.h"
#include <linux/delay.h>
#include <asm/io.h>

#include "sys_regs.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define GFX_WRITE_REG(reg,val)	\
    __raw_writel(val , (volatile void*)((ULONG)(g_gfx_reg_h13) + (ULONG)OFFSET(GFX_H13_REG_T,reg)))

#define GFX_READ_REG(reg)	\
    __raw_readl( (volatile void*)((ULONG)(g_gfx_reg_h13) + (ULONG)OFFSET(GFX_H13_REG_T,reg)))

//#define SHOW_GFX_REGISTER //only debugging
#if defined(SHOW_GFX_REGISTER)

#define PRINT_READ_REG(reg)	\
   do { printk("GFX REG 0x%x -> 0x%0.8x\n",((UINT32)OFFSET(GFX_H13_REG_T,reg)), *((UINT32 *)(&g_gfx_reg_h13_cache->reg)) ); } while(0);

#endif

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32	addr;
	UINT32	data0;
	UINT32	data1;
	UINT32	data2;
}
GFX_H13_SCALE_HPHASE_DATA_T;

typedef struct
{
	UINT32	addr;
	UINT32	data0;
	UINT32	data1;
}
GFX_H13_SCALE_VPHASE_DATA_T;

typedef struct
{
#define	GFX_SCALE_PHASE_DATA_NUM	16
	GFX_H13_SCALE_HPHASE_DATA_T		hphase[GFX_SCALE_PHASE_DATA_NUM];		/* hhpase data */
	GFX_H13_SCALE_VPHASE_DATA_T		vphase[GFX_SCALE_PHASE_DATA_NUM];		/* vhpase data */
}
GFX_H13_SCALE_FILTER_DATA_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern volatile GFX_H13_REG_T	*g_gfx_reg_h13;
extern GFX_H13_REG_T 			*g_gfx_reg_h13_cache;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile UINT32 g_ui_ack_mode_h13 = 0;

#ifdef	KDRV_CONFIG_PM
unsigned int g_pm_cmd_delay_h13	= 0;
unsigned int g_pm_intr_ctrl_h13	= 0;
#endif

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static int	_g_gfx_h13_bw_val			= 0x7ff;
static int	_g_gfx_h13_bw_val_req		= 0x7ff;
module_param_named( gfx_h13_bw_val, _g_gfx_h13_bw_val_req, int, 0644 );

#define	GFX_H13_CUSTOM_FILTER_MAX	4

static int _g_gfx_h13_scaler_index = -1;
module_param_named( gfx_h13_scaler_index, _g_gfx_h13_scaler_index, int, 0644 );

static GFX_H13_SCALE_FILTER_DATA_T 	g_gfx_h13_custom_scale_filter_data[GFX_H13_CUSTOM_FILTER_MAX] = 
{
	/* normal upscale & downscale to 0.75 */
	[0] = {	.hphase	= {	/* addr,		data0,		data1,		data2 */
						{ 0x00000000, 0x3fd00400, 0x0ffc0340, 0x00000000 },
						{ 0x00000001, 0xff6013ff, 0x1ff00b3f, 0x00000000 },
						{ 0x00000002, 0x7f001bfe, 0x3fe4143f, 0x0000ffc0 },
						{ 0x00000003, 0xbea027fd, 0x4fd81d3e, 0x0000ffc0 },
						{ 0x00000004, 0x3e502ffc, 0x6fc8263e, 0x0000ff80 },
						{ 0x00000005, 0x3e0037fb, 0x7fbc303d, 0x0000ff80 },
						{ 0x00000006, 0x7dc03bfa, 0x9fac3a3c, 0x0000ff40 },
						{ 0x00000007, 0xbd9043fa, 0xafa0443a, 0x0000ff40 },
						{ 0x00000008, 0x7d6047f9, 0xbf944f39, 0x0000ff00 },
						{ 0x00000009, 0x3d404bf9, 0xdf885a37, 0x0000ff00 },
						{ 0x0000000a, 0x7d204ff9, 0xef7c6535, 0x0000fec0 },
						{ 0x0000000b, 0x7d1053f8, 0xff707033, 0x0000fec0 },
						{ 0x0000000c, 0x7d0053f8, 0x0f647c31, 0x0000fe81 },
						{ 0x0000000d, 0xfd0053f8, 0x1f5c872e, 0x0000fe81 },
						{ 0x0000000e, 0x3d1053f9, 0x2f54922c, 0x0000fe41 },
						{ 0x0000000f, 0xfd204ff9, 0x3f4c9c29, 0x0000fe41 } },
			.vphase	= {	/* addr,		data0,		data1 */
						{ 0x00000000, 0x002403fe, 0x00000000 },
						{ 0x00000001, 0xc09403f8, 0x000000ff },
						{ 0x00000002, 0x8103fbf4, 0x000000ff },
						{ 0x00000003, 0x4183eff0, 0x000000ff },
						{ 0x00000004, 0xc203e7ec, 0x000000fe },
						{ 0x00000005, 0x8293cfea, 0x000000fe },
						{ 0x00000006, 0x0323bfe7, 0x000000fe },
						{ 0x00000007, 0xc3c3a3e5, 0x000000fd },
						{ 0x00000008, 0x446387e4, 0x000000fd },
						{ 0x00000009, 0xc5036be3, 0x000000fc },
						{ 0x0000000a, 0x45b347e3, 0x000000fc },
						{ 0x0000000b, 0xc66323e3, 0x000000fb },
						{ 0x0000000c, 0x4712fbe4, 0x000000fb },
						{ 0x0000000d, 0xc7d2d3e4, 0x000000fa },
						{ 0x0000000e, 0x8882a7e5, 0x000000fa },
						{ 0x0000000f, 0x09327be7, 0x000000fa } },
			},
	/* downscale from 0.75 to 0.5 */
	[1] = {	.hphase	= {	/* addr,		data0,		data1,		data2 */
						{0x00000000, 0x032f9008, 0x8f8c3730, 0x00000000},
						{0x00000001, 0x02cf9009, 0x8f883d30, 0x00000000},
						{0x00000002, 0xc26f9809, 0x7f88432f, 0x00000000},
						{0x00000003, 0x821f9c09, 0x6f84492f, 0x00000040},
						{0x00000004, 0x01cfa009, 0x6f844f2f, 0x00000040},
						{0x00000005, 0xc16fa409, 0x5f84552e, 0x00000080},
						{0x00000006, 0xc12fac09, 0x4f885b2d, 0x00000080},
						{0x00000007, 0x80dfb009, 0x3f88612d, 0x00000080},
						{0x00000008, 0x808fb809, 0x2f8c672c, 0x000000c0},
						{0x00000009, 0xc04fbc09, 0x1f906d2b, 0x000000c0},
						{0x0000000a, 0xc00fc408, 0x0f94732a, 0x00000100},
						{0x0000000b, 0xffdfc808, 0xff987929, 0x0000013f},
						{0x0000000c, 0x3fafd008, 0xefa07f28, 0x0000017f},
						{0x0000000d, 0xbf6fd407, 0xdfa88427, 0x0000017f},
						{0x0000000e, 0x7f3fdc07, 0xbfb08926, 0x000001bf},
						{0x0000000f, 0xff1fe007, 0xafb88f24, 0x000001bf} },
			.vphase	= {	/* addr,		data0,		data1 */
						{0x00000000, 0x02a2e425, 0x000000fe},
						{0x00000001, 0x02f2e420, 0x000000fe},
						{0x00000002, 0xc352e01c, 0x000000fd},
						{0x00000003, 0x83b2e017, 0x000000fd},
						{0x00000004, 0x8412d813, 0x000000fd},
						{0x00000005, 0x4472d40f, 0x000000fd},
						{0x00000006, 0x44d2c80c, 0x000000fd},
						{0x00000007, 0x0532c408, 0x000000fd},
						{0x00000008, 0x05a2b405, 0x000000fd},
						{0x00000009, 0x0602a802, 0x000000fd},
						{0x0000000a, 0x06729400, 0x000000fd},
						{0x0000000b, 0x06d283ff, 0x000000fd},
						{0x0000000c, 0x47426ffc, 0x000000fd},
						{0x0000000d, 0x47a25bfb, 0x000000fd},
						{0x0000000e, 0x880247f9, 0x000000fd},
						{0x0000000f, 0xc8622ff8, 0x000000fd} },
			},
	/* downscale from 0.5 to 0.25 */
	[2] = {	.hphase	= {	/* addr,		data0,		data1,		data2 */
						{0x00000000, 0xc1bfc404, 0x11141f1a, 0x00000002},
						{0x00000001, 0x018fc405, 0x31181d1b, 0x00000002},
						{0x00000002, 0x415fc405, 0x511c1c1b, 0x00000002},
						{0x00000003, 0x412fc805, 0x711c1b1b, 0x00000042},
						{0x00000004, 0x810fc805, 0x9120191b, 0x00000042},
						{0x00000005, 0x80dfcc05, 0xc120181b, 0x00000042},
						{0x00000006, 0x80afd005, 0xe120171b, 0x00000082},
						{0x00000007, 0xc08fd005, 0x0120151b, 0x000000c3},
						{0x00000008, 0x805fd405, 0x3124141b, 0x000000c3},
						{0x00000009, 0x802fd805, 0x5124131b, 0x00000103},
						{0x0000000a, 0x000fdc05, 0x8124121b, 0x00000143},
						{0x0000000b, 0x3fefdc05, 0xb124101b, 0x00000183},
						{0x0000000c, 0xbfcfe005, 0xe1240f1a, 0x000001c3},
						{0x0000000d, 0xbfafe405, 0x01200e1a, 0x00000204},
						{0x0000000e, 0x3f8fe805, 0x31200d1a, 0x00000244},
						{0x0000000f, 0xbf6fec04, 0x61200c19, 0x000002c4} },
			.vphase	= {	/* addr,		data0,		data1 */
						{0x00000000, 0x03e21c3b, 0x00000000},
						{0x00000001, 0x04221838, 0x00000000},
						{0x00000002, 0x44521435, 0x00000000},
						{0x00000003, 0x44821432, 0x00000000},
						{0x00000004, 0x84b2102f, 0x00000000},
						{0x00000005, 0xc4e20c2c, 0x00000000},
						{0x00000006, 0x05220429, 0x00000001},
						{0x00000007, 0x45520026, 0x00000001},
						{0x00000008, 0x8581fc23, 0x00000001},
						{0x00000009, 0xc5b1f820, 0x00000001},
						{0x0000000a, 0x05e1f01e, 0x00000002},
						{0x0000000b, 0x8611e81b, 0x00000002},
						{0x0000000c, 0xc641e019, 0x00000002},
						{0x0000000d, 0x4671d417, 0x00000003},
						{0x0000000e, 0xc6a1c815, 0x00000003},
						{0x0000000f, 0x46c1c013, 0x00000004} },
			},
	/* downscale from 0.25 to 0.0 */
	[3] = {	.hphase	= {	/* addr,		data0,		data1,		data2 */
						{0x00000000, 0x83607808, 0x90783711, 0x00000000},
						{0x00000001, 0x43607408, 0x907c3811, 0x00000000},
						{0x00000002, 0x83507007, 0xa0803811, 0x00000000},
						{0x00000003, 0x83406c07, 0xa0843911, 0x00000000},
						{0x00000004, 0x43406807, 0xb0883911, 0x00000000},
						{0x00000005, 0x83306806, 0xb0883a11, 0x00000000},
						{0x00000006, 0x83206406, 0xc08c3a11, 0x00000000},
						{0x00000007, 0x43106005, 0xd0903b11, 0x00000040},
						{0x00000008, 0x43105c05, 0xd0943b11, 0x00000040},
						{0x00000009, 0xc3005c05, 0xe0983c10, 0x00000040},
						{0x0000000a, 0x82f05804, 0xe0983c11, 0x00000040},
						{0x0000000b, 0x42e05404, 0xf09c3d11, 0x00000040},
						{0x0000000c, 0xc2e05004, 0x00a03d10, 0x00000081},
						{0x0000000d, 0xc2d05003, 0x00a43e10, 0x00000081},
						{0x0000000e, 0xc2c04c03, 0x10a83e10, 0x00000081},
						{0x0000000f, 0x02b04803, 0x20a83e11, 0x00000081} },
			.vphase	= {	/* addr,		data0,		data1 */
						{0x00000000, 0x4451ac43, 0x00000003},
						{0x00000001, 0x8471a841, 0x00000003},
						{0x00000002, 0x0481a43f, 0x00000004},
						{0x00000003, 0x44a1a43c, 0x00000004},
						{0x00000004, 0x84b1a43a, 0x00000004},
						{0x00000005, 0x04d19c38, 0x00000005},
						{0x00000006, 0x44e19c36, 0x00000005},
						{0x00000007, 0x85019834, 0x00000005},
						{0x00000008, 0x05119432, 0x00000006},
						{0x00000009, 0x45319030, 0x00000006},
						{0x0000000a, 0xc5418c2e, 0x00000006},
						{0x0000000b, 0x05518c2c, 0x00000007},
						{0x0000000c, 0x8571842a, 0x00000007},
						{0x0000000d, 0x05818028, 0x00000008},
						{0x0000000e, 0x45917c27, 0x00000008},
						{0x0000000f, 0xc5b17425, 0x00000008} },
			},
};

/** get the best scaler set 
 *	set is chosen based on recommendation of H/W guys.
 *
 *
 */
static int		GFX_H13_GetBestHVScalerIndex ( GFX_SCALER_CONFIGURE_T* scaler )
{
	int	h_scale_ratio = 0;
	int	v_scale_ratio = 0;

	if ( _g_gfx_h13_scaler_index >= 0 ) return _g_gfx_h13_scaler_index;

	h_scale_ratio = scaler->out_dim.w * 100 / scaler->in_dim.w;
	v_scale_ratio = scaler->out_dim.h * 100 / scaler->in_dim.h;

#if 0
	if		( h_scale_ratio  <= 25 || v_scale_ratio <= 25 ) return 3;	
	else if	( h_scale_ratio  <= 50 || v_scale_ratio <= 50 ) return 2;
	if		( h_scale_ratio  <= 50 || v_scale_ratio <= 50 ) return 3;	/*  0% ~ 50% down scale */
	else if	( h_scale_ratio  <= 75 || v_scale_ratio <= 75 ) return 1;	/* 50% ~ 75% down scale */
#else
	if	( h_scale_ratio  <= 75 || v_scale_ratio <= 75 ) 	return 1;	/* 0% ~ 75% down scale */
#endif
	else 						   							return 0;	/* normal scale */
}

/*========================================================================================
	Implementation Group
========================================================================================*/
/** initialize scaler
 *
 *
 */
void	GFX_H13_InitScaler(void)
{
	int	i,j;
	GFX_H13_SCALE_FILTER_DATA_T*	filter_data;

	//GFX_WRITE_REG( scaler_vh_order, 0x0 );					/* initialize scaler as scale-up */
	GFX_WRITE_REG( scaler_frame_start, 0x1 );					/* scaler frame start (reset) */

	for ( j=0; j< GFX_H13_CUSTOM_FILTER_MAX; j++ )				/* download custome filter data */
	{
		filter_data = &(g_gfx_h13_custom_scale_filter_data[j]);	

		for ( i=0; i< GFX_SCALE_PHASE_DATA_NUM; i++ )
		{
			g_gfx_reg_h13->scaler_hphase_data0 = filter_data->hphase[i].data0;
			g_gfx_reg_h13->scaler_hphase_data1 = filter_data->hphase[i].data1;
			g_gfx_reg_h13->scaler_hphase_data2 = filter_data->hphase[i].data2;
			g_gfx_reg_h13->scaler_hphase_addr  = (j<<4) | filter_data->hphase[i].addr;
		}

		for ( i=0; i< GFX_SCALE_PHASE_DATA_NUM; i++ )
		{
			g_gfx_reg_h13->scaler_vphase_data0 = filter_data->vphase[i].data0;
			g_gfx_reg_h13->scaler_vphase_data1 = filter_data->vphase[i].data1;
			g_gfx_reg_h13->scaler_vphase_addr  = (j<<4) | filter_data->vphase[i].addr;
		}
	}
}

/** generate CQ BATCH RUN command after single or multiple CQ operation group.
 *	-- single CQ operation group is consist of single or multi normal CQs and CQ START
 *
 */
int GFX_H13_RunFlushCommand(void)
{
	GFX_TRACE_BEGIN();
	GFX_WRITE_REG( gfx_batch_run, 0x1 );
	GFX_TRACE_END();
	return RET_OK;
}

/** generate CQ START command after every GFX operation such write, blit etc.
 *
 *
 */
int GFX_H13_RunStartCommand(void)
{
	GFX_TRACE_BEGIN();

	/* write B/W limit if changed */
	if ( _g_gfx_h13_bw_val != _g_gfx_h13_bw_val_req	)
	{
		_g_gfx_h13_bw_val_req &= 0x7ff;

		GFX_WARN("+++ bw_limit changed 0x%x -> 0x%x\n", _g_gfx_h13_bw_val, _g_gfx_h13_bw_val_req );
		_g_gfx_h13_bw_val = _g_gfx_h13_bw_val_req;
		OS_WrReg( 0xc000e4f8, _g_gfx_h13_bw_val );
	}

	GFX_WRITE_REG( gfx_start, 0x1 );

	GFX_TRACE_END();
	return RET_OK;
}

/** pause GFX command
 *
 */
int GFX_H13_PauseCommand(void)
{
	GFX_TRACE_BEGIN();
	g_gfx_reg_h13->gfx_reset.gfx_reset				= 0x0;
	g_gfx_reg_h13->gfx_batch_run.gfx_batch_run		= 0x0;
	g_gfx_reg_h13->gfx_start.gfx_start				= 0x0;
	g_gfx_reg_h13->gfx_pause.gfx_pause				= 0x1;
	GFX_TRACE_END();

	return 0;
}

/** resume GFX command
 *
 */
int GFX_H13_ResumeCommand(void)
{
	GFX_TRACE_BEGIN();
	g_gfx_reg_h13->gfx_reset.gfx_reset				= 0x0;
	g_gfx_reg_h13->gfx_batch_run.gfx_batch_run		= 0x0;
	g_gfx_reg_h13->gfx_start.gfx_start				= 0x0;
	g_gfx_reg_h13->gfx_pause.gfx_pause				= 0x2;
	GFX_TRACE_END();

	return 0;
}

/** Do soft reset
 *
 * @return RET_OK when sucess, RET_ERROR otherwise
 */
int GFX_H13_SwResetCommand(void)
{
	GFX_WRITE_REG(gfx_reset, 0x1);
	GFX_DEBUG("gfx sw reset req !!\n");
	GFX_WRITE_REG(gfx_reset, 0x0);

	/* raxis.lim (2011/11/02) - GFX H/W guy said scaler_soft_reset are obsoleted in H13  */
#if 0
	GFX_WRITE_REG( scaler_soft_reset, 0x1 );
	GFX_WRITE_REG( scaler_soft_reset, 0x0 );
#endif
	return 0;
}

/** stop GFX command ( reset GFX & flush command queue )
 *
 */
int GFX_H13_StopCommand(void)
{
	return GFX_H13_SwResetCommand();
}

/** get command queue status 
 *
 */
void GFX_H13_GetComQueueStatus(GFX_CMD_QUEUE_CTRL_T *queue)
{
	GFX_TRACE_BEGIN();
	GFX_RdFL( gfx_status0 );
	GFX_RdFL( gfx_status1 );
	GFX_RdFL( gfx_status2 );

	GFX_Rd01( gfx_status0, exec_line, queue->usLine );
	GFX_Rd01( gfx_status0, exec_status, queue->bStatus );
	GFX_Rd01( gfx_status1, que_remain, queue->usRemainSpace );
	GFX_Rd01( gfx_status1, cmd_que_full, queue->bFull );
	GFX_Rd01( gfx_status1, cmd_que_status, queue->bCmdQStatus );
	GFX_Rd01( gfx_status2, batch_status, queue->bBatchStatus );
	GFX_Rd01( gfx_status2, batch_remain, queue->usRemainParam );

	queue->status[0] = GFX_Rd( gfx_status0 );
	queue->status[1] = GFX_Rd( gfx_status1 );
	queue->status[2] = GFX_Rd( gfx_status2 );

	GFX_TRACE_END();
}

/**	check if GFX engine is idle or busy state.
 *	this function is used for detecting graphic sync.
 *
 *	@param	none
 *	@return TRUE if GFX is idle, FALSE otherwise
 *	@note check code seems tbe be somewhat hureustric.
 *
 *	@see	GFX_IOW_WAIT_FOR_SYNC
 */
BOOLEAN GFX_H13_IsGfxIdle(void)
{
    BOOLEAN ret;

	GFX_RdFL( gfx_status0 );
	GFX_RdFL( gfx_status1 );
	GFX_RdFL( gfx_status2 );

	ret = ( g_gfx_reg_h13_cache->gfx_status0.exec_status	== 0 &&
			g_gfx_reg_h13_cache->gfx_status1.cmd_que_status== 0 )? TRUE : FALSE;
//   ret = ( g_gfx_reg_h13->status0.bwCmdStatus == 0 &&
//          g_gfx_reg_h13->status0.abwProcessingLine == 0 &&
//          g_gfx_reg_h13->status1.bwCmdQueueStatus == 0 )? TRUE : FALSE ;

	GFX_DEBUG("GFX IDLE = (%s) %d-%d-%d\n", (ret)? "IDLE":"BUSY",
			g_gfx_reg_h13_cache->gfx_status0.exec_status, g_gfx_reg_h13_cache->gfx_status0.exec_line, g_gfx_reg_h13_cache->gfx_status1.cmd_que_status );

    return ret;
}

/** get operation status & chroma filtering mode
 *
 *
 */
void GFX_H13_GetOperationStatus(GFX_CMD_OP_T *operate)
{
	GFX_TRACE_BEGIN();
	GFX_RdFL( gfx_op_mode );
	GFX_RdFL( cflt_coef0 );
	GFX_RdFL( cflt_coef1 );

	GFX_Rd04( gfx_op_mode,	op_mode,	operate->sPort,	
							burst_mode,	operate->bBurstModeEnable,							
							clut_up_en, operate->bUpdateCLUT,
							cflt_mode, 	operate->sFmode );

	operate->bChromaFilterEnable = 0 ; //g_gfx_reg_h13->op_mode.bwEnChromaFilter;

	GFX_Rd02( cflt_coef0, 	cflt_coef_0, operate->usCoef[0],
							cflt_coef_1, operate->usCoef[1] );
	GFX_Rd02( cflt_coef1, 	cflt_coef_2, operate->usCoef[2],
							cflt_coef_3, operate->usCoef[3] );
	GFX_TRACE_END();
}

/** Set operation status
 *
 *	@note operation status should be read at GFX_H13_GetOperationStatus
 */
void GFX_H13_SetOperationStatus(GFX_CMD_OP_T *operate)
{
	GFX_TRACE_BEGIN();

	GFX_Wr04( gfx_op_mode,	op_mode,	operate->sPort,	
							burst_mode,	operate->bBurstModeEnable,							
							clut_up_en, operate->bUpdateCLUT,
							cflt_mode, 	operate->sFmode );

	GFX_Wr02( cflt_coef0, 	cflt_coef_0, operate->usCoef[0],
							cflt_coef_1, operate->usCoef[1] );
	GFX_Wr02( cflt_coef1, 	cflt_coef_2, operate->usCoef[2],
							cflt_coef_3, operate->usCoef[3] );

	/* writer register ( memory cache -> register )
	 *
	 * - gfx_op_mode  
	 * - cflt_coef0
	 * - cflt_coef1
	 */
	GFX_WrFL(gfx_op_mode);
	GFX_WrFL(cflt_coef0);
	GFX_WrFL(cflt_coef1);

	GFX_TRACE_END();
}

/** get inpurt port configuration
 *
 *
 */
void GFX_H13_GetInputConfigure(int iPort, GFX_PORT_CONFIGURE_T *port)
{
	GFX_TRACE_BEGIN();

	switch(iPort)
	{
		case ePort0:
		{
			GFX_RdFL( r0_base_addr );
			GFX_RdFL( r0_stride );
			GFX_RdFL( r0_pformat );
			GFX_RdFL( r0_galpha );
			GFX_RdFL( r0_clut_ctrl );
			GFX_RdFL( r0_clut_data );
			GFX_RdFL( r0_ctrl );
			GFX_RdFL( r0_ckey_key0 );
			GFX_RdFL( r0_ckey_key1 );
			GFX_RdFL( r0_ckey_replace_color );
			GFX_RdFL( r0_coc_ctrl );

			GFX_Rd00( r0_base_addr, port->ulAddr );
			GFX_Rd01( r0_stride, 	r0_stride, 		port->usStride );
			GFX_Rd02( r0_pformat,	pixel_format,	port->sFmt,
									endian_mode,	port->sEndian );
			GFX_Rd00( r0_galpha,	port->uiGlobalAlpha );

			GFX_Rd03( r0_clut_ctrl,	clut0_addr,		port->usAddrCLUT,
									clut0_rw,		port->bRW_CLUT,	
									clut0_wai,		port->bAuto_Inc_CLUT );

			GFX_Rd00( r0_clut_data,	port->ulDataCLUT );

			GFX_Rd06( r0_ctrl,		color_key_en,	port->bColorKeyEnable,
									bitmask_en,		port->bBitMaskEnable,
									coc_en,			port->bCOCEnable,
									csc_en,			port->bCSCEnable,
									color_key_mode,	port->bColorKeyMode,
									csc_coef_sel,	port->sCSCSel );

			GFX_Rd00( r0_ckey_key0,			port->ulKeyLow );
			GFX_Rd00( r0_ckey_key1, 		port->ulKeyHigh );
			GFX_Rd00( r0_ckey_replace_color, port->ulReplaceColor );
			GFX_Rd00( r0_bitmask, 			 port->ulBitMask );
			GFX_Rd00( r0_coc_ctrl, 			port->ulCOCCtrl );
		}
		break;

		case ePort1:
		{
			GFX_RdFL( r1_base_addr );
			GFX_RdFL( r1_stride );
			GFX_RdFL( r1_pformat );
			GFX_RdFL( r1_galpha );
			GFX_RdFL( r1_clut_ctrl );
			GFX_RdFL( r1_clut_data );
			GFX_RdFL( r1_ctrl );
			GFX_RdFL( r1_ckey_key0 );
			GFX_RdFL( r1_ckey_key1 );
			GFX_RdFL( r1_ckey_replace_color );
			GFX_RdFL( r1_coc_ctrl );

			GFX_Rd00( r1_base_addr, port->ulAddr );
			GFX_Rd01( r1_stride, 	r1_stride, 		port->usStride );
			GFX_Rd02( r1_pformat,	pixel_format,	port->sFmt,
									endian_mode,	port->sEndian );
			GFX_Rd00( r1_galpha,	port->uiGlobalAlpha );

			GFX_Rd03( r1_clut_ctrl,	clut1_addr,		port->usAddrCLUT,
									clut1_rw,		port->bRW_CLUT,	
									clut1_wai,		port->bAuto_Inc_CLUT );

			GFX_Rd00( r1_clut_data,	port->ulDataCLUT );

			GFX_Rd06( r1_ctrl,		color_key_en,	port->bColorKeyEnable,
									bitmask_en,		port->bBitMaskEnable,
									coc_en,			port->bCOCEnable,
									csc_en,			port->bCSCEnable,
									color_key_mode,	port->bColorKeyMode,
									csc_coef_sel,	port->sCSCSel );

			GFX_Rd00( r1_ckey_key0,				port->ulKeyLow );
			GFX_Rd00( r1_ckey_key1, 			port->ulKeyHigh );
			GFX_Rd00( r1_ckey_replace_color, 	port->ulReplaceColor );
			GFX_Rd00( r1_bitmask, 			 	port->ulBitMask );
			GFX_Rd00( r1_coc_ctrl, 				port->ulCOCCtrl );
		}
		break;

		case ePort2:
		{
			GFX_RdFL( r2_base_addr );
			GFX_RdFL( r2_stride );
			GFX_RdFL( r2_pformat );
			GFX_RdFL( r2_galpha );

			GFX_Rd00( r2_base_addr, port->ulAddr );
			GFX_Rd01( r2_stride, 	r2_stride, 		port->usStride );
			GFX_Rd02( r2_pformat,	pixel_format,	port->sFmt,
									endian_mode,	port->sEndian );
			GFX_Rd00( r2_galpha,	port->uiGlobalAlpha );
		}
		break;

		default:
		{
			GFX_WARN(" Invalid port %d\n", port->sPort );
		}
		break;
	}

	GFX_TRACE_END();
}

void GFX_H13_SetInputConfigure(GFX_PORT_CONFIGURE_T *port)
{
	GFX_TRACE_BEGIN();

	switch(port->sPort)
	{
		case ePort0:
		{
			GFX_Wr00( r0_base_addr, port->ulAddr );
			GFX_Wr01( r0_stride, 	r0_stride, 		port->usStride );
			GFX_Wr02( r0_pformat,	pixel_format,	port->sFmt,
									endian_mode,	port->sEndian );
			GFX_Wr00( r0_galpha,	port->uiGlobalAlpha );
			GFX_Wr03( r0_clut_ctrl,	clut0_addr,		port->usAddrCLUT,
									clut0_rw,		port->bRW_CLUT,	
									clut0_wai,		port->bAuto_Inc_CLUT );
//			GFX_Wr00( r0_clut_data,	port->ulDataCLUT );
			GFX_Wr06( r0_ctrl,		color_key_en,	port->bColorKeyEnable,
									bitmask_en,		port->bBitMaskEnable,
									coc_en,			port->bCOCEnable,
									csc_en,			port->bCSCEnable,
									color_key_mode,	port->bColorKeyMode,
									csc_coef_sel,	port->sCSCSel );
			GFX_Wr00( r0_ckey_key0,	port->ulKeyLow );
			GFX_Wr00( r0_ckey_key1, port->ulKeyHigh );
			GFX_Wr00( r0_ckey_replace_color, port->ulReplaceColor );
			GFX_Wr00( r0_bitmask, 			 port->ulBitMask );
			GFX_Wr00( r0_coc_ctrl, 	port->ulCOCCtrl );

			/* write regiseter ( memory cache -> register ) */
			GFX_WrFL( r0_base_addr );
			GFX_WrFL( r0_stride );
			GFX_WrFL( r0_pformat );
			GFX_WrFL( r0_galpha );
			GFX_WrFL( r0_clut_ctrl );
			//GFX_WrFL( r0_clut_data );	<<-- useless for setting input configure !!
			GFX_WrFL( r0_ctrl );
			GFX_WrFL( r0_ckey_key0 );
			GFX_WrFL( r0_ckey_key1 );
			GFX_WrFL( r0_ckey_replace_color );
			GFX_WrFL( r0_coc_ctrl );

			GFX_WrFL( r0_bitmask ); // added by hwlee
		}
		break;

		case ePort1:
		{
			GFX_Wr00( r1_base_addr, port->ulAddr );
			GFX_Wr01( r1_stride, 	r1_stride, 		port->usStride );
			GFX_Wr02( r1_pformat,	pixel_format,	port->sFmt,
									endian_mode,	port->sEndian );
			GFX_Wr00( r1_galpha,	port->uiGlobalAlpha );
			GFX_Wr03( r1_clut_ctrl,	clut1_addr,		port->usAddrCLUT,
									clut1_rw,		port->bRW_CLUT,	
									clut1_wai,		port->bAuto_Inc_CLUT );
//			GFX_Wr00( r1_clut_data,	port->ulDataCLUT );
			GFX_Wr06( r1_ctrl,		color_key_en,	port->bColorKeyEnable,
									bitmask_en,		port->bBitMaskEnable,
									coc_en,			port->bCOCEnable,
									csc_en,			port->bCSCEnable,
									color_key_mode,	port->bColorKeyMode,
									csc_coef_sel,	port->sCSCSel );
			GFX_Wr00( r1_ckey_key0,	port->ulKeyLow );
			GFX_Wr00( r1_ckey_key1, port->ulKeyHigh );
			GFX_Wr00( r1_ckey_replace_color, port->ulReplaceColor );
			GFX_Wr00( r1_bitmask, 			 port->ulBitMask );
			GFX_Wr00( r1_coc_ctrl, 	port->ulCOCCtrl );

			/* write regiseter ( memory cache -> register ) */
			GFX_WrFL( r1_base_addr );
			GFX_WrFL( r1_stride );
			GFX_WrFL( r1_pformat );
			GFX_WrFL( r1_galpha );
			GFX_WrFL( r1_clut_ctrl );
			//GFX_WrFL( r1_clut_data );	<<-- useless for setting input configure !!
			GFX_WrFL( r1_ctrl );
			GFX_WrFL( r1_ckey_key0 );
			GFX_WrFL( r1_ckey_key1 );
			GFX_WrFL( r1_ckey_replace_color );
			GFX_WrFL( r1_coc_ctrl );
		}
		break;

		case ePort2:
		{
			GFX_Wr00( r2_base_addr, port->ulAddr );
			GFX_Wr01( r2_stride, 	r2_stride, 		port->usStride );
			GFX_Wr02( r2_pformat,	pixel_format,	port->sFmt,
									endian_mode,	port->sEndian );
			GFX_Wr00( r2_galpha,	port->uiGlobalAlpha );

			/* write regiseter ( memory cache -> register ) */
			GFX_WrFL( r2_base_addr );
			GFX_WrFL( r2_stride );
			GFX_WrFL( r2_pformat );
			GFX_WrFL( r2_galpha );
		}
		break;

		default:
		break;
	}
	GFX_TRACE_END();
}

/** get blending config
 *
 *	this function reads out_sel, blend_ctrl0, blend_ctrl1, blend_ctrl_const, rop_ctrl
 *
 */
void GFX_H13_GetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend)
{
	UINT32	color_a;
	UINT32	color_c;

	GFX_TRACE_BEGIN();

	GFX_RdFL( out_sel );
	GFX_RdFL( blend_ctrl0 );
	GFX_RdFL( blend_ctrl1 );
	GFX_RdFL( blend_ctrl_const );
	GFX_RdFL( rop_ctrl );

	GFX_Rd01( out_sel, 	out_sel,	blend->sOut );

	GFX_Rd11( blend_ctrl0,	pma0,			blend->bPMA0Enable,
							pma1,			blend->bPMA1Enable,
							xor0,			blend->bXOR0Enable,
							xor1,			blend->bXOR1Enable,		
							div_en,			blend->bDIVEnable,
							c_m0,			blend->usC_m0,
							alpha_m0,		blend->usAlpha_m0,
							comp_sel_b,		blend->sBBlend,
							comp_sel_g,		blend->sGBlend,
							comp_sel_r,		blend->sRBlend,
							comp_sel_a,		blend->sAlphaBlend );

	GFX_Rd08( blend_ctrl1,	b3_sel,		blend->usB3_B,
							a3_sel,		blend->usA3_B,
							b2_sel,		blend->usB2_G,
							a2_sel,		blend->usA2_G,
							b1_sel,		blend->usB1_R,
							a1_sel,		blend->usA1_R,
							b0_sel,		blend->usB0_Alpha,
							a0_sel,		blend->usA0_Alpha );

	GFX_Rd02( blend_ctrl_const, c_c,		color_c,
								alpha_c,	color_a );
	blend->ulBlendConstant	= ((color_a & 0xff)<<24)|(color_c & 0xffffff);
//	blend->ulBlendConstant	= g_gfx_reg_h13->out_const_ctrl.abwConstantCtrl;
	GFX_Rd01( rop_ctrl,		rop_ctrl,		blend->sRaster );
//	blend->sRaster			= g_gfx_reg_h13->out_rop_ctrl.abwRasterSel;

	GFX_TRACE_END();
}

/** set bleding output config 
 *
 *	@note out_sel should be read before this function at GFX_H13_GetBlendConfigure
 */
void GFX_H13_SetBlendingOut(GFX_ENUM_OUT_T type)
{
	GFX_TRACE_BEGIN();
	GFX_Wr01( out_sel, out_sel,	type );
	/* write regiseter ( memory cache -> register ) */
	GFX_WrFL( out_sel );
	GFX_TRACE_END();
}

/* set blend configure
 * this function write out_sel, blend_ctrl0, blend_ctrl1, blend_ctrl_const, rop_ctrl registers 
 *
 * 	@note all data should be read before this function at GFX_H13_GetBlendConfigure
 */
void GFX_H13_SetBlendConfigure( GFX_BLEND_CONFIGURE_T *blend)
{
	UINT32	color_a, color_c;
	GFX_TRACE_BEGIN();

	GFX_Wr01( out_sel, 	out_sel,	blend->sOut );
//	blend->sOut 		= g_gfx_reg_h13->out_sel.abwOutSel;

	GFX_Wr11( blend_ctrl0,	pma0,			blend->bPMA0Enable,
							pma1,			blend->bPMA1Enable,
							xor0,			blend->bXOR0Enable,
							xor1,			blend->bXOR1Enable,
							div_en,			blend->bDIVEnable,
							c_m0,			blend->usC_m0,
							alpha_m0,		blend->usAlpha_m0,
							comp_sel_b,		blend->sBBlend,
							comp_sel_g,		blend->sGBlend,
							comp_sel_r,		blend->sRBlend,
							comp_sel_a,		blend->sAlphaBlend );

	GFX_Wr08( blend_ctrl1,	b3_sel,		blend->usB3_B,
							a3_sel,		blend->usA3_B,
							b2_sel,		blend->usB2_G,
							a2_sel,		blend->usA2_G,
							b1_sel,		blend->usB1_R,
							a1_sel,		blend->usA1_R,
							b0_sel,		blend->usB0_Alpha,
							a0_sel,		blend->usA0_Alpha );

	color_a = (blend->ulBlendConstant>>24) & 0xff;
	color_c = blend->ulBlendConstant & 0xffffff;
	GFX_Wr02( blend_ctrl_const, c_c,		color_c,
								alpha_c,	color_a );

	GFX_Wr01( rop_ctrl,		rop_ctrl,		blend->sRaster );

	/* write regiseter ( memory cache -> register ) */
	GFX_WrFL( out_sel );
	GFX_WrFL( blend_ctrl0 );
	GFX_WrFL( blend_ctrl1 );
	GFX_WrFL( blend_ctrl_const );
	GFX_WrFL( rop_ctrl );

	GFX_TRACE_END();
}

void GFX_H13_GetOutputConfigure(GFX_OUT_CONFIGURE_T *port)
{
	GFX_TRACE_BEGIN();

	GFX_RdFL( wr_base_addr );
	GFX_RdFL( wr_stride );
	GFX_RdFL( wr_pformat );
	GFX_RdFL( wr_size );
	GFX_RdFL( wr_galpha );
//	GFX_RdFL( wr_clut_ctrl );
//	GFX_RdFL( wr_clut_data );
	GFX_RdFL( wr_ctrl );
	GFX_RdFL( wr_coc_ctrl );

	GFX_RdFL( wr_csc_coef_00 );
	GFX_RdFL( wr_csc_coef_01 );
	GFX_RdFL( wr_csc_coef_02 );
	GFX_RdFL( wr_csc_coef_03 );
	GFX_RdFL( wr_csc_coef_04 );
	GFX_RdFL( wr_csc_coef_05 );
	GFX_RdFL( wr_csc_coef_06 );
	GFX_RdFL( wr_csc_coef_07 );

	GFX_Rd00( wr_base_addr, port->ulAddr );
	GFX_Rd01( wr_stride, 	wr_stride, 		port->usStride );
	GFX_Rd02( wr_pformat,	pixel_format,	port->sFmt,
							endian_mode,	port->sEndian );

	GFX_Rd00( wr_galpha,	port->ulGAlpha );

	GFX_Rd02( wr_ctrl,		coc_en,			port->bCOCEnable,
							csc_en,			port->bCSCEnable );
	GFX_Rd00( wr_coc_ctrl, 	port->ulCOCCtrl );
	GFX_Rd02( wr_size,		hsize,			port->usHSize,
							vsize,			port->usVSize );

	GFX_Rd00( wr_csc_coef_00, port->uiCSCCoef[0] );
	GFX_Rd00( wr_csc_coef_01, port->uiCSCCoef[1] );
	GFX_Rd00( wr_csc_coef_02, port->uiCSCCoef[2] );
	GFX_Rd00( wr_csc_coef_03, port->uiCSCCoef[3] );
	GFX_Rd00( wr_csc_coef_04, port->uiCSCCoef[4] );
	GFX_Rd00( wr_csc_coef_05, port->uiCSCCoef[5] );
	GFX_Rd00( wr_csc_coef_06, port->uiCSCCoef[6] );
	GFX_Rd00( wr_csc_coef_07, port->uiCSCCoef[7] );

	GFX_TRACE_END();
}

void GFX_H13_SetOutputConfigure(GFX_OUT_CONFIGURE_T *port)
{
	GFX_TRACE_BEGIN();

	GFX_Wr00( wr_base_addr, port->ulAddr );
	GFX_Wr01( wr_stride, 	wr_stride, 		port->usStride );
	GFX_Wr02( wr_pformat,	pixel_format,	port->sFmt,
							endian_mode,	port->sEndian );
	GFX_Wr00( wr_galpha,	port->ulGAlpha );
	GFX_Wr02( wr_ctrl,		coc_en,			port->bCOCEnable,
							csc_en,			port->bCSCEnable );
	GFX_Wr00( wr_coc_ctrl, 	port->ulCOCCtrl );
	GFX_Wr02( wr_size,		hsize,			port->usHSize,
							vsize,			port->usVSize );
	GFX_Wr00( wr_csc_coef_00, port->uiCSCCoef[0] );
	GFX_Wr00( wr_csc_coef_01, port->uiCSCCoef[1] );
	GFX_Wr00( wr_csc_coef_02, port->uiCSCCoef[2] );
	GFX_Wr00( wr_csc_coef_03, port->uiCSCCoef[3] );
	GFX_Wr00( wr_csc_coef_04, port->uiCSCCoef[4] );
	GFX_Wr00( wr_csc_coef_05, port->uiCSCCoef[5] );
	GFX_Wr00( wr_csc_coef_06, port->uiCSCCoef[6] );
	GFX_Wr00( wr_csc_coef_07, port->uiCSCCoef[7] );

	/* write regiseter ( memory cache -> register ) */
	GFX_WrFL( wr_base_addr );
	GFX_WrFL( wr_stride );
	GFX_WrFL( wr_pformat );
	GFX_WrFL( wr_size );
	GFX_WrFL( wr_galpha );
	GFX_WrFL( wr_ctrl );
	GFX_WrFL( wr_coc_ctrl );

	GFX_WrFL( wr_csc_coef_00 );
	GFX_WrFL( wr_csc_coef_01 );
	GFX_WrFL( wr_csc_coef_02 );
	GFX_WrFL( wr_csc_coef_03 );
	GFX_WrFL( wr_csc_coef_04 );
	GFX_WrFL( wr_csc_coef_05 );
	GFX_WrFL( wr_csc_coef_06 );
	GFX_WrFL( wr_csc_coef_07 );

	GFX_TRACE_END();
}

static int GFX_H13_SelectScalerOrder( GFX_SCALER_CONFIGURE_T* s )
{
	int	vh_order;

	if ( ( s->in_dim.w < 2048 && s->in_dim.h < 2048 ) && ( s->out_dim.w >= 2048 || s->out_dim.h >= 2048 ) )
	{
		vh_order = 0; /* scale up : V->H */
	}
	else
	{
		vh_order = 1; /* scale dn : H->V */
	}
	GFX_DEBUG("[%d,%d] -> [%d,%d]. vh_order = %d\n", s->in_dim.w, s->in_dim.h, s->out_dim.w, s->out_dim.h, vh_order );
	//GFX_DEBUG("ret vh_order = %d. reg_off=0x%x\n", vh_order, OFFSET(GFX_H13_REG_T,scaler_vh_order));

	return vh_order;
}

void GFX_H13_SetScalerConfigure(GFX_SCALER_CONFIGURE_T* scaler)
{
	switch( scaler->cmd )
	{
		case GFX_SCALER_CMD_SOFT_RESET:
		{
			/* reset scaler block */
			GFX_WRITE_REG( scaler_frame_start, 0x1 );
			GFX_DEBUG("scaler reset req !!\n");  
		}
		break;

		case GFX_SCALER_CMD_START:
		{
			int scaler_set_idx = GFX_H13_GetBestHVScalerIndex( scaler );
			int vh_order= GFX_H13_SelectScalerOrder( scaler );

#if 1
			GFX_WRITE_REG( scaler_vh_order, vh_order );
#else
			GFX_RdFL( scaler_vh_order );
			GFX_Wr01( scaler_vh_order,	scaler_vh_order, vh_order );
			GFX_WrFL( scaler_vh_order );
#endif
#if 1
			GFX_WRITE_REG(scaler_line_init, 0x1);	/* init scaler line */
			GFX_WRITE_REG(scaler_line_init, 0x0);
#else
			g_gfx_reg_h13->scaler_line_init.scaler_line_init = 0x1;	/* init scaler line */
			g_gfx_reg_h13->scaler_line_init.scaler_line_init = 0x0;
#endif
			GFX_RdFL( scaler_bypass );
			GFX_RdFL( scaler_in_pic_width );
			GFX_RdFL( scaler_in_pic_height );
			GFX_RdFL( scaler_out_pic_width );
			GFX_RdFL( scaler_out_pic_height );

			GFX_RdFL( scaler_bilinear );

			GFX_RdFL( scaler_phase_offset );
			GFX_RdFL( scaler_boundary_mode );
			GFX_RdFL( scaler_sampling_mode );
			GFX_RdFL( scaler_numerator );
			GFX_RdFL( scaler_denominator );
			GFX_RdFL( scaler_hcoef_index );
			GFX_RdFL( scaler_vcoef_index );

			GFX_Wr01( scaler_bypass, 		scaler_bypass, 			0x0 );
			GFX_Wr01( scaler_in_pic_width,  scaler_in_pic_width, 	scaler->in_dim.w);
			GFX_Wr01( scaler_in_pic_height, scaler_in_pic_height,	scaler->in_dim.h);
			GFX_Wr01( scaler_out_pic_width, scaler_out_pic_width, 	scaler->out_dim.w);
			GFX_Wr01( scaler_out_pic_height,scaler_out_pic_height,	scaler->out_dim.h);

			GFX_Wr01( scaler_phase_offset, 	scaler_phase_offset, 	0x0 ); //scaler->filter_data.phase_offset );
			GFX_Wr01( scaler_boundary_mode, scaler_boundary_mode, 	0x1 ); //scaler->filter_data.boundary_mode );
			GFX_Wr01( scaler_sampling_mode, scaler_sampling_mode, 	0x0 ); //scaler->filter_data.sampling_mode );
			GFX_Wr01( scaler_numerator,   	scaler_numerator, 		0x0 ); //scaler->filter_data.numerator );
			GFX_Wr01( scaler_denominator, 	scaler_denominator, 	0x0 ); //scaler->filter_data.denomidator);

			GFX_Wr01( scaler_hcoef_index, 	scaler_hcoef_index, 	scaler_set_idx );
			GFX_Wr01( scaler_vcoef_index, 	scaler_vcoef_index, 	scaler_set_idx );

			if ( scaler->filter == GFX_SCALER_FILTER_CUSTOM )
			{
				GFX_Wr01( scaler_bilinear, 		scaler_bilinear, 	0x0 );
			}
			else	/* HW default (Bilinear) */
			{
				GFX_Wr01( scaler_bilinear, 		scaler_bilinear, 	0x1 );
			}

			GFX_WrFL( scaler_bypass ); 			/* disable scaler bypass 	*/

			GFX_WrFL( scaler_in_pic_width );	/* input picture width 		*/
			GFX_WrFL( scaler_out_pic_width );	/* output picture width 	*/
			GFX_WrFL( scaler_in_pic_height );	/* input picture height 	*/
			GFX_WrFL( scaler_out_pic_height );	/* output picture height 	*/

			GFX_WrFL( scaler_phase_offset );
			GFX_WrFL( scaler_boundary_mode );
			GFX_WrFL( scaler_sampling_mode );
			GFX_WrFL( scaler_numerator );
			GFX_WrFL( scaler_denominator );
			GFX_WrFL( scaler_bilinear );
			GFX_WrFL( scaler_hcoef_index );
			GFX_WrFL( scaler_vcoef_index );

			/* reset scaler block */
			GFX_WRITE_REG( scaler_frame_start, 0x1 );
		}
		break;

		case GFX_SCALER_CMD_BYPASS:
		default:
		{
		    g_gfx_reg_h13->scaler_bypass.scaler_bypass = 0x1;
		}
		break;
	}
}

void GFX_H13_SetInterruptMode(UINT32 uiMode)
{
	/* do nothing */
}

/* what is it ? */
void GFX_H13_SetAckMode(UINT32 *mode)
{
	GFX_TRACE_BEGIN();
	g_ui_ack_mode_h13 = *mode;
	g_ui_ack_mode_h13 = 1;
	GFX_TRACE_END();
}

/* what is it ? */
void GFX_H13_GetAckMode(UINT32 *mode)
{
	*mode = g_ui_ack_mode_h13;
}

int GFX_H13_SetCLUT(int port, int size , UINT32 *data)
{
	int i , ret = 0;

	GFX_TRACE_BEGIN();

    for ( i=0; i<100; i++ )
    {
        if( GFX_H13_IsGfxIdle() ) break;
        GFX_ERROR("GFX BUSY...\n"); OS_MsecSleep(1);
    }

	switch(port)
	{
        case 0:
        {
            g_gfx_reg_h13->r0_clut_ctrl.clut0_addr	= 0;
            g_gfx_reg_h13->r0_clut_ctrl.clut0_rw	= 0;
#ifdef GFX_PALETTE_AUTO_INC
            g_gfx_reg_h13->r0_clut_ctrl.clut0_wai	= 1;
#else
            g_gfx_reg_h13->r0_clut_ctrl.clut0_wai	= 0;
#endif
            for(i=0 ; i<size ; i++)
            {
#ifdef GFX_PALETTE_AUTO_INC

#else
                g_gfx_reg_h13->r0_clut_ctrl.clut0_addr = i;
#endif
                g_gfx_reg_h13->r0_clut_data			= *data++;
            }
        }
        break;

        case 1:
        {
            g_gfx_reg_h13->r1_clut_ctrl.clut1_addr	= 0;
            g_gfx_reg_h13->r1_clut_ctrl.clut1_rw	= 0;
#ifdef GFX_PALETTE_AUTO_INC
            g_gfx_reg_h13->r1_clut_ctrl.clut1_wai	= 1;
#else
            g_gfx_reg_h13->r1_clut_ctrl.clut1_wai	= 0;
#endif
            for(i=0 ; i<size ; i++)
            {
#ifdef GFX_PALETTE_AUTO_INC

#else
                g_gfx_reg_h13->r1_clut_ctrl.clut1_addr = i;
#endif
                g_gfx_reg_h13->r1_clut_data			= *data++;
			}
        }
        break;

		default:
		{
			GFX_WARN("invalid port: %d\n", port );
		}
		break;
	}

	udelay(10); /* wait for gfx sync */

	GFX_TRACE_END();
	return ret;
}

static UINT16   GFX_H13_CalcAutoCommandDelay(void)
{
#ifdef GFX_AUTO_CMD_DELAY
    int rc;
    LX_DE_SRC_STATUS_T  de_src_param;

    GFX_INFO("checking gfx auto bwlimit..\n");

    de_src_param.status_type = LX_DE_SRC_STATUS_OPER;
    rc = DE_REG_H13_GetSourceStatus(&de_src_param);

    if ( rc == RET_OK && de_src_param.operType == LX_DE_OPER_3D && de_src_param.operCtrlFlag == LX_DE_3D_IMG_IN_DUAL_HD )
    {
        GFX_INFO("++ 3D_DUAL decoding detected\n");
        return 0x50;    /* heuristic value */
    }
    else
    {
        return 0x0;
    }
#else
    return 0x0;
#endif
}

/**
 *
 * command delay register setting function
 *
 * @param	delay count
 * @return	void
 *
 */
void GFX_H13_SetCommandDelayReg(UINT16 delay)
{
    UINT32  curr_delay;

    /* if cmd_delay is -1, start auto bandwidth determination */
    if ( delay == (UINT16)-1 )
    {
        delay = GFX_H13_CalcAutoCommandDelay();
    }

    GFX_RdFL( gfx_cmd_dly );
    GFX_Rd01( gfx_cmd_dly, gfx_cmd_dly, curr_delay );

    if ( delay != curr_delay )
    {
        GFX_Wr01( gfx_cmd_dly, gfx_cmd_dly, delay );
        GFX_WrFL( gfx_cmd_dly );
    }

    GFX_INFO("++ gfx_cmddly = 0x%x\n", delay );
}

/**
 *
 * command delay register getting function
 *
 * @param	void
 * @return	delay count
 *
 * @ingroup osd_ddi_func
*/
UINT16 GFX_H13_GetCommandDelayReg(void)
{
	UINT16	val;
	GFX_TRACE_BEGIN();
	val = g_gfx_reg_h13->gfx_cmd_dly.gfx_cmd_dly;
	GFX_TRACE_END();
	return val;
}

/** dump GFX register value for debug.
 *  this function is used for debugging the crtical issues or proc 
 */
void GFX_H13_DumpRegister(void)
{
    int i;
    UINT32* reg_ptr = (UINT32*)g_gfx_reg_h13;

	printk("H13_GFX_REG\n");
    for ( i = 0; i < gpGfxRegCfg->reg_size; i+=4 )
    {
        printk("0x%08x [0x%04x] 0x%08x\n", (gpGfxRegCfg->reg_base_addr+i), i, *reg_ptr++ );
    }
}

int GFX_H13_RunSuspend(void)
{
	/* do nothing */
	return RET_OK;
}

int GFX_H13_RunResume(void)
{
	GFX_InitHW( );
	return RET_OK;
}

/** enable GFX clock 
 *
 */
int GFX_H13_ClkCheck(void)
{
	UINT32	gfx_clk;

    CTOP_CTRL_H13B0_RdFL( ctr07 );
    CTOP_CTRL_H13B0_Rd01( ctr07, gfx_aclk_sel, gfx_clk );

	/* if gfx clk value is 0x3, gfx clk is disabled */
	return (gfx_clk==0x3)? FALSE:TRUE;
}

/** disable GFX clock 
 *
 */
int GFX_H13_RunClkOffCommand(void)
{
	UINT64	start_ms;
	UINT64	curr_ms;

	int	ret = RET_ERROR;

	GFX_ERROR("<begin> GFX CLK OFF\n");

	/* step 0: wait until GFX is idle ( maximum 10 ms ) */
	start_ms = curr_ms = OS_GetMsecTicks();
	do 
	{
		if ( GFX_H13_IsGfxIdle() ) { GFX_ERROR("gfx idle ok\n"); break; }
		curr_ms = OS_GetMsecTicks();	
	}
	while( (int)(curr_ms - start_ms) <= 10 ); 

	GFX_CHECK_ERROR( (int)(curr_ms-start_ms) > 10, goto func_exit, "timeout during waiting for GFX idle\n");

	GFX_ShutdownHW( );

	/* step 1: flush enable */
    CPU_TOP_H13B0_RdFL( flush_req );
    CPU_TOP_H13B0_Wr01( flush_req, gfx_flush_en, 1 );
    CPU_TOP_H13B0_WrFL( flush_req );

	/* step 2: wait until gfx flush */
	start_ms = curr_ms = OS_GetMsecTicks();
	do 
	{
		UINT32	gfx_flush_done = 0;

		CPU_TOP_H13B0_RdFL( flush_done_status );
		CPU_TOP_H13B0_Rd01( flush_done_status, gfx_flush_done, gfx_flush_done );

		if ( gfx_flush_done ) { GFX_ERROR("gfx flush done\n"); break; }
		curr_ms = OS_GetMsecTicks();	
	}
	while( (int)(curr_ms - start_ms) <= 10 ); 

	GFX_CHECK_ERROR( (int)(curr_ms-start_ms) > 10, goto func_exit, "timeout during waiting for GFX flush\n");

	/* step 3: reset */
    CTOP_CTRL_H13B0_RdFL( ctr07 );
    CTOP_CTRL_H13B0_Wr01( ctr07, swrst_gfx, 1 );
    CTOP_CTRL_H13B0_WrFL( ctr07 );

	/* step 4: clock gating off */
    CTOP_CTRL_H13B0_RdFL( ctr07 );
    CTOP_CTRL_H13B0_Wr01( ctr07, gfx_aclk_sel, 0x3 );
    CTOP_CTRL_H13B0_WrFL( ctr07 );

	ret = RET_OK;
	GFX_ERROR("<end> GFX CLK OFF\n");
func_exit:
	return ret;
}

/** check GFX clock
 *
 */
int GFX_H13_RunClkOnCommand(void)
{
	GFX_ERROR("<begin> GFX CLK ON\n");

    // step 0: flush disable
    CPU_TOP_H13B0_RdFL( flush_req );
    CPU_TOP_H13B0_Wr01( flush_req, gfx_flush_en, 0 );
    CPU_TOP_H13B0_WrFL( flush_req );

    // step 1: Clock gating on
    CTOP_CTRL_H13B0_RdFL( ctr07 );
    CTOP_CTRL_H13B0_Wr01( ctr07, gfx_aclk_sel, 0x0 );
    CTOP_CTRL_H13B0_WrFL( ctr07 );

    // 3. software reset back to normal
    CTOP_CTRL_H13B0_RdFL( ctr07 );
    CTOP_CTRL_H13B0_Wr01( ctr07, swrst_gfx, 0 );
    CTOP_CTRL_H13B0_WrFL( ctr07 );

	GFX_InitHW( );

	GFX_ERROR("<end> GFX CLK ON\n");
	return RET_OK;
}

/** @} */

