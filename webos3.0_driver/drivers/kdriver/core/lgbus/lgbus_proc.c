/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file
 *
 *  main driver implementation for lgbus device.
 *	lgbus device will teach you how to make device driver with new platform.
 *
 *  author		bongrae.cho (bongrae.cho@lge.com)
 *  version		1.0
 *  date		2012.11.10
 *  note		Additional information.
 *
 *  @addtogroup lg115x_lgbus
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kthread.h>

#include "proc_util.h"
#include "os_util.h"
#include "sys_regs.h"

#include "lgbus_kapi.h"
#include "lgbus_drv.h"
#include "lgbus_proc.h"
#include "lgbus_cfg.h"
#include "lgbus_hal.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

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


/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int _LGBUS_PROC_Reader(UINT32 procId, char* buffer);
static int _LGBUS_PROC_Writer( UINT32 procId, char* command );



/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T	_g_lgbus_device_proc_table[] =
{
	{ "read_monitoring_result",		PROC_ID_READ_BW_MON, OS_PROC_FLAG_READ },
	{ "config_repeat_count",		PROC_ID_CFG_REPEAT_CNT, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
	{ "config_request_blocks",		PROC_ID_CFG_REQ_BLKS, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
	//{ "config_screan_clear",		PROC_ID_CFG_SCR_CLR, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
	{ "print_BW_monitoring",		PROC_ID_PRINT_BW_MON, OS_PROC_FLAG_READ | OS_PROC_FLAG_WRITE },
	{ "readme",		                PROC_ID_README, OS_PROC_FLAG_READ },
	{ NULL, 		                PROC_ID_MAX		, 0 }
};

static struct task_struct 	*g_pstLGBUSProcTask = NULL;

static int repeatCount = 100;
static int request_blks = 0x3FFFFFF;

/*========================================================================================
	Implementation Group
========================================================================================*/
#if 0
static int flag_screanClear = 0, flag_printing = 0;
static int LGBUS_Proc_Print_BW_Task(void *param)
{
    unsigned int i;
    LX_LGBUS_REQ_INFO_T proc_req_info;
    static LX_LGBUS_REPLY_T result;

    // set APB clk select to 50MHz(1), 25MHz(2), doesn't work at 100MHz(0) async arbitration H/W bug
	if (lx_chip_rev() == LX_CHIP_REV(H13, B0))
	{
        CTOP_CTRL_H13B0_RdFL(ctr01);
        CTOP_CTRL_H13B0_Wr01(ctr01, apb_clk_sel, 1);
        CTOP_CTRL_H13B0_WrFL(ctr01);
	}


    proc_req_info.bus_chk_id = LX_LGBUS_REQ_LGBUS;
    proc_req_info.chip_rev = LX_LGBUS_H13_B0;
    proc_req_info.mnt_chk_blks = 0x03FFFFFF;
    proc_req_info.mon_freq_per_second = 1;

    lgbus_wrap_control_init(proc_req_info);


    for(i = 0; i < repeatCount && flag_printing; i++)
    {
        lgbus_wrap_control_read(&result);

        showBWResult(&result);
    }


    // set APB clk select to 100MHz(0) for normal operation, It's workaround of async arbitration H/W bug
	if (lx_chip_rev() == LX_CHIP_REV(H13, B0))
	{
        CTOP_CTRL_H13B0_RdFL(ctr01);
        CTOP_CTRL_H13B0_Wr01(ctr01, apb_clk_sel, 0);
        CTOP_CTRL_H13B0_WrFL(ctr01);
	}

}

static int showBWResult(LX_LGBUS_REPLY_T* result)
{
    char lgbus_blk_name[LGBUS_MNT_TOTAL_HW_BLKS][20] =
    {
            /*0*/   "cpu_main",
            /*1*/   "cpu_sub",
            /*2*/   "gpu0",
            /*3*/   "gpu1",
            /*4*/   "gfx",
            /*5*/   "vdec",
            /*6*/   "vdo",
            /*7*/   "aud",
            /*8*/   "icod",
            /*9*/   "te",
            /*10*/  "venc",
            /*11*/  "tcon",
            /*12*/  "dpp_a",
            /*13*/  "de_a",
            /*14*/  "de_b",
            /*15*/  "de_c",
            /*16*/  "de_d",
            /*17*/  "de_e_cvd",
            /*18*/  "de_e_mcu",
            /*19*/  "dvi",
            /*20*/  "dvo",
            /*21*/  "sre",
            /*22*/  "mep_a",
            /*23*/  "mep_b",
            /*24*/  "bve_mcu",
            /*25*/  "mc",
    };

	int ret = 0, i, show_bus_idx = 0;
    unsigned long wr_total_bw = 0, rd_total_bw = 0, rw_total_bw = 0;

    const unsigned long total_bw = 64000000;  // 6.4G, but can't use UINT64 (unsigned long long) for divide


    for(show_bus_idx = 0; show_bus_idx < 2; show_bus_idx++)
    {/*
        ret += sprintf(buffer+ret, "\n");
        ret += sprintf(buffer+ret, "|    blk     |    wr_bw   |  wr_bw(%)  | wr_max_ltn | wr_avg_ltn |    rd_bw   |  rd_bw(%)  | rd_max_ltn | rd_avg_ltn |\n");
        ret += sprintf(buffer+ret, "\n");
        */
        printk("======================================================================================================================\n");
        printk("|    blk     |    wr_bw   |  wr_bw(%%)  | wr_max_ltn | wr_avg_ltn |    rd_bw   |  rd_bw(%%)  | rd_max_ltn | rd_avg_ltn |\n");
        printk("======================================================================================================================\n");

        for(i =0; i < LGBUS_MNT_TOTAL_HW_BLKS; i++)
    	{
            if(result->raw[i].requestd)
            {
        		if(result->raw[i].bus_idx == show_bus_idx)
        		{
        			//ret += sprintf(buffer+ret, "| %10s | %10u | %6d.%03d%%| %10u | %10u | %10u | %6d.%03d%%| %10u | %10u |\n",
        			printk("| %10s | %10u | %6d.%03d%%| %10u | %10u | %10u | %6d.%03d%%| %10u | %10u |\n",
        					lgbus_blk_name[i],
        					result->raw[i].wr_bw - result->raw[i].wr_port0_3_bw[0],
        					//mon_reply.result[show_bus_idx].raw[i].wr_bw_bud,
        					(result->raw[i].wr_bw - result->raw[i].wr_port0_3_bw[0]) / total_bw,
        					(result->raw[i].wr_bw - result->raw[i].wr_port0_3_bw[0]) / (total_bw / 10) % 1000,
        					result->raw[i].wr_max_ltn,
        					result->refined[i].wr_avg_ltn,
        					result->raw[i].rd_bw - result->raw[i].rd_port0_3_bw[0],
        					//mon_reply.result[show_bus_idx].raw[i].rd_bw_bud,
        					(result->raw[i].wr_bw - result->raw[i].rd_port0_3_bw[0]) / total_bw,
        					(result->raw[i].wr_bw - result->raw[i].rd_port0_3_bw[0]) / (total_bw / 10) % 1000,
        					result->raw[i].rd_max_ltn,
        					result->refined[i].rd_avg_ltn);

        			//printf("+--------------------------------------------------------------------------------------------------------------------+\n");
        		}
                else if(result->raw[i].bus_idx != show_bus_idx && (result->raw[i].wr_port0_3_bw[0] || result->raw[i].rd_port0_3_bw[0]))
                {
        			//ret += sprintf(buffer+ret, "| %10s | %10u |%6d.%03d%%| %10s | %10s | %10u | %6d.%03d%%| %10s | %10s |\n",
        			printk("| %10s | %10u |%6d.%03d%%| %10s | %10s | %10u | %6d.%03d%%| %10s | %10s |\n",
        					lgbus_blk_name[i],
        					result->raw[i].wr_port0_3_bw[0],
        					//mon_reply.result[show_bus_idx].raw[i].wr_bw_bud,
        					(result->raw[i].wr_bw - result->raw[i].wr_port0_3_bw[0]) / total_bw,
        					(result->raw[i].wr_bw - result->raw[i].wr_port0_3_bw[0]) / (total_bw / 10) % 1000,
        					"X",
        					"X",
        					result->raw[i].rd_port0_3_bw[0],
        					//mon_reply.result[show_bus_idx].raw[i].rd_bw_bud,
        					(result->raw[i].wr_bw - result->raw[i].rd_port0_3_bw[0]) / total_bw,
        					(result->raw[i].wr_bw - result->raw[i].rd_port0_3_bw[0]) / (total_bw / 10) % 1000,
        					"X",
        					"X");
                }
            }
    	}
        for(i = 0; i < 4; i++)
        {
            wr_total_bw += result->result[show_bus_idx].wr_ddrc_port0_3bw[i];
            rd_total_bw += result->result[show_bus_idx].rd_ddrc_port0_3bw[i];
            //rd_total_bw = mon_reply.result[show_bus_idx].rd_ddrc_port0_bw+mon_reply.result[show_bus_idx].rd_ddrc_port1_bw+mon_reply.result[show_bus_idx].rd_ddrc_port2_bw+mon_reply.result[show_bus_idx].rd_ddrc_port3_bw;
            rw_total_bw = wr_total_bw + rd_total_bw;
        }
        /*
    	ret += sprintf(buffer+ret, "\n");
    	ret += sprintf(buffer+ret, "| ddrc port  |  wr_port0  |  wr_port1  |  wr_port2  |  wr_port3  |  rd_port0  |  rd_port1  |  rd_port2  |  rd_port3  |\n");
    	ret += sprintf(buffer+ret, "\n");
        */
    	printk("======================================================================================================================\n");
    	printk("| ddrc port  |  wr_port0  |  wr_port1  |  wr_port2  |  wr_port3  |  rd_port0  |  rd_port1  |  rd_port2  |  rd_port3  |\n");
    	printk("======================================================================================================================\n");

    	//ret += sprintf(buffer+ret, "| %10s | %6d.%03d%| %6d.%03d%| %6d.%03d%| %6d.%03d%| %6d.%03d%| %6d.%03d%| %6d.%03d%| %6d.%03d%|\n",
    	printk("| %10s | %6d.%03d%%| %6d.%03d%%| %6d.%03d%%| %6d.%03d%%| %6d.%03d%%| %6d.%03d%%| %6d.%03d%%| %6d.%03d%%|\n",
    			"port# ",
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[0] / total_bw,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[0] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[1] / total_bw,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[1] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[2] / total_bw,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[2] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[3] / total_bw,
    			result->result[show_bus_idx].wr_ddrc_port0_3bw[3] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[0] / total_bw,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[0] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[1] / total_bw,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[1] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[2] / total_bw,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[2] / (total_bw / 10) % 1000,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[3] / total_bw,
    			result->result[show_bus_idx].rd_ddrc_port0_3bw[3] / (total_bw / 10) % 1000);
    	/*
    	ret += sprintf(buffer+ret, "\n");
    	ret += sprintf(buffer+ret, "| ddrc total | wr_bw_total| rd_bw_total|  r/w total |  total bw  |                                                   |\n");
    	ret += sprintf(buffer+ret, "\n");
        */
    	printk("======================================================================================================================\n");
    	printk("| ddrc total | wr_bw_total| rd_bw_total|  r/w total |  total bw  |                                                   |\n");
    	printk("======================================================================================================================\n");

    	//ret += sprintf(buffer+ret, "| %7s(%d) | %6d.%03d%| %6d.%03d%| %6d.%03d%|\n",
    	printk("| %7s(%d) | %6d.%03d%%| %6d.%03d%%| %6d.%03d%%|\n",
    			"total",
    			show_bus_idx,
    			wr_total_bw / total_bw,
    			wr_total_bw / (total_bw / 10) % 1000,
    			rd_total_bw / total_bw,
    			rd_total_bw / (total_bw / 10) % 1000,
    			rw_total_bw / total_bw,
    			rw_total_bw / (total_bw / 10) % 1000
    			);

    	//ret += sprintf(buffer+ret, "\n\n\n");
    	printk("======================================================================================================================\n\n\n");

    }

	return ret;
}
#endif

static int _LGBUS_PROC_Reader(UINT32 procId, char* buffer)
{
    unsigned int ret = 0;

	switch( procId )
	{
		case PROC_ID_READ_BW_MON:

            break;

		case PROC_ID_CFG_REPEAT_CNT:

            ret += sprintf(buffer+ret, "Current repeat count value : %d\n", repeatCount);
            ret += sprintf(buffer+ret, "input repeat count to print result\n");
            ret += sprintf(buffer+ret, "ex)echo 1000 >> config_repeat_count\n");

            break;

        case PROC_ID_CFG_REQ_BLKS:

            ret += sprintf(buffer+ret, "Current Request Blocks value : 0x%X\n", request_blks);
            ret += sprintf(buffer+ret, "[request monitoring blocks]\n");
            ret += sprintf(buffer+ret, "----------------------------------------------------------------------------------------------------------------------------------\n");
    		ret += sprintf(buffer+ret, "|  bit |  15  |  14  |  13  |   12  |  11  |   10   |  9 |  8   |  7  |    6   |    5   |  4  |   3  |  2   |    1    |     0    |\n");
    		ret += sprintf(buffer+ret, "| blks | de_c | de_b | de_a | dpp_a | tcon | ve_pri | te | icod | aud | vd_sec | vd_pri | gfx | gpu1 | gpu0 | cpu_sub | cpu_main |\n");
    		ret += sprintf(buffer+ret, "----------------------------------------------------------------------------------------------------------------------------------\n");
    		ret += sprintf(buffer+ret, "|  bit | 31 | 30 | 29 | 28 | 27 | 26 |  25 |    24   |   23  |   22  |  21 |  20 |  19 |    18    |    17    |  16  |\n");
    		ret += sprintf(buffer+ret, "| blks |    |    |    |    |    |    |  mc | bve_mcu | mep_b | mep_a | sre | dvo | dvi | de_e_mcu | de_e_cvd | de_d |\n");
    		ret += sprintf(buffer+ret, "---------------------------------------------------------------------------------------------------------------------\n");
            ret += sprintf(buffer+ret, "input request Blocks\n");
            ret += sprintf(buffer+ret, "ex)echo 3FFFFFF >> config_request_blks\n");

            break;

		case PROC_ID_PRINT_BW_MON:

            if(g_pstLGBUSProcTask != NULL)
            {
                ret += sprintf(buffer+ret, "now result printing!\n");
                ret += sprintf(buffer+ret, "if you want to stop printing result, then \"echo 0 >> print_BW_monitoring\"\n");
            }
            else
            {
                ret += sprintf(buffer+ret, "printing result is stopped\n");
                ret += sprintf(buffer+ret, "if you want to start printing result, then \"echo 1 >> print_BW_monitoring\"\n");
            }


            break;

		case PROC_ID_README:

            break;

        default:

            break;
	}



    return ret;
}

static int _LGBUS_PROC_Writer( UINT32 procId, char* command )
{
	UINT32  n=0, ret;
	char	*param[10];

	ret = strlen(command);

    while( command != NULL && n < 10 )
        param[n++] = strsep(&command, "     ");

	switch( procId )
	{
		case PROC_ID_CFG_REPEAT_CNT:

			repeatCount = simple_strtoul(param[0], NULL, 0);
            break;
        case PROC_ID_CFG_REQ_BLKS:

			request_blks = simple_strtoul(param[0], NULL, 16);
            break;

		case PROC_ID_PRINT_BW_MON:
#if 0
            flag_printing = simple_strtoul(param[0], NULL, 0);

            if(flag_printing)
            {
                if(g_pstLGBUSProcTask == NULL)
        			g_pstLGBUSProcTask = kthread_run(LGBUS_Proc_Print_BW_Task, NULL, "PRINT BW MON");
                else
                    ;
            }
            else
            {
                if(g_pstLGBUSProcTask != NULL)
                {
                    flag_printing = 0;
        			kthread_stop(g_pstLGBUSProcTask);
                    g_pstLGBUSProcTask = NULL;

                }
            }
#endif
            break;

        default:

            break;
	}

	return ret;
}

void LGBUS_PROC_Init(void)
{
	OS_PROC_CreateEntryEx(LGBUS_MODULE, _g_lgbus_device_proc_table,
										_LGBUS_PROC_Reader,
										_LGBUS_PROC_Writer);
}

void LGBUS_PROC_Cleanup(void)
{
	OS_PROC_RemoveEntry(LGBUS_MODULE);
}

/** @} */

