//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
// png2rgb.cpp : Defines the entry point for the console application.
//
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/unistd.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif
#include "png.h"
#include "gpd_reg.h"
#include "drvgpd.h"
#include "mdrvgpd.h"
#include "gpd.h"
#include "MsOS.h"
//#include <semihost_io.h>

#define PRINT_GPD_DECODE_TIME    FALSE //TRUE

static my_png_buffer mybuff;
static my_png_buffer *my_buff=&mybuff;

//struct png_struct_def pngstruct;
//struct png_info_struct infostruct;

//unsigned char length_addr[4];
//unsigned char type_addr[4];
static unsigned int this_ichk_start;
//unsigned int next_ichk_start;
//unsigned int this_ichk_end;
static unsigned int this_ichk_len;
static unsigned int next_ichk_len;
//unsigned int this_read_addr;
//unsigned int next_read_addr;
//unsigned int addr_count;
static unsigned char no_ichk;
static unsigned char last_ichk;
//unsigned char no_fragment;
//unsigned char set;
static unsigned char read_num = 0;
extern unsigned int g_gpd_set_access_region, g_gpd_clock;


//unsigned int ichk_start_addr[350];
//unsigned int ichk_len[350];
//unsigned int ichk_num=0;
static int iccp_en=0;
//int ichk_count=0;

static char *IHDR="IHDR";
static char *IDAT="IDAT";
static char *PLTE="PLTE";
static char *tRNS="tRNS";

static gpd_addr_info * buf_addr;
static PngInfo info;
static PngInfop infoptr=&info;

static unsigned int bend;

extern int ZEXPORT gpdinflateReset(z_streamp strm);
unsigned short show_fifo(int n);


U32 PNG_GET_U32REG_Twice(int index)
{
    U32 u32Reg1, u32Reg2;
    while(1)
    {
        u32Reg1 = GPD_GET_U32REG(index);
        u32Reg2 = GPD_GET_U32REG(index);
        if(u32Reg1 == u32Reg2)
        {
            return u32Reg1;
        }
    }
}



void PNG_Get_reg_iofifo_state(void)
{
#if ENABLE_GPD_MESSAGE
    U32 _reg_iofifo_state = PNG_GET_U32REG_Twice(reg_iofifo_state);

    console_printf("ofifo_wr_cs[2:0] = 0x%lx \n", _reg_iofifo_state&0x7);
    console_printf("ofifo_rd_cs[2:0] = 0x%lx \n", (_reg_iofifo_state>>3)&0x7);
    console_printf("ififo_eco_rd[2:0] = 0x%lx \n", (_reg_iofifo_state>>6)&0x7);
    console_printf("ififo_eco_wr = 0x%lx \n", (_reg_iofifo_state>>9)&0x1);
#endif
}


void PNG_Get_reg_png_state(void)
{
#if ENABLE_GPD_MESSAGE
    U32 _reg_png_state = PNG_GET_U32REG_Twice(reg_png_state);

    console_printf("rw_st[2:0] = 0x%lx \n", _reg_png_state&0x7);
    console_printf("decom_st[2:0] = 0x%lx \n", (_reg_png_state>>3)&0x7);
    console_printf("fifo_empty = 0x%lx \n", (_reg_png_state>>6)&0x1);
    console_printf("decom_rdy = 0x%lx \n", (_reg_png_state>>7)&0x1);
    console_printf("deflt_ack_ufifo0 = 0x%lx \n", (_reg_png_state>>8)&0x1);
    console_printf("deflt_st[1:0] = 0x%lx \n", (_reg_png_state>>9)&0x3);
    console_printf("flg1 = 0x%lx \n", (_reg_png_state>>11)&0x1);
    console_printf("flg0 = 0x%lx \n", (_reg_png_state>>12)&0x1);
    console_printf("descan_ack_ufifo1 = 0x%lx \n", (_reg_png_state>>13)&0x1);
    console_printf("deflt_rdy = 0x%lx \n", (_reg_png_state>>14)&0x1);
    console_printf("deflt_ack = 0x%lx \n", (_reg_png_state>>15)&0x1);
    console_printf("decom_rdy_ufifo0 = 0x%lx \n", (_reg_png_state>>16)&0x1);
    console_printf("descan_st[3:0] = 0x%lx \n", (_reg_png_state>>17)&0xf);
    console_printf("ofifo_wrdy = 0x%lx \n", (_reg_png_state>>21)&0x1);
    console_printf("descan_rdy = 0x%lx \n", (_reg_png_state>>22)&0x1);
    console_printf("descan_ack_a1t = 0x%lx \n", (_reg_png_state>>23)&0x1);
    console_printf("deflt_rdy = 0x%lx \n", (_reg_png_state>>24)&0x1);
    console_printf("fifo1_empty = 0x%lx \n", (_reg_png_state>>25)&0x1);
    console_printf("fifo0_empty = 0x%lx \n", (_reg_png_state>>26)&0x1);
    console_printf("reg_gif_local_tbl = 0x%lx \n", (_reg_png_state>>27)&0x1);
    console_printf("flt_psram_full|lz_psram_full = 0x%lx \n", (_reg_png_state>>28)&0x1);
    console_printf("gpd_time_out = 0x%lx \n", (_reg_png_state>>29)&0x1);
    console_printf("ofifo_empty = 0x%lx \n", (_reg_png_state>>30)&0x1);
    console_printf("err_flg = 0x%lx \n", (_reg_png_state>>31)&0x1);
#endif
}

void PNG_Get_DebugMux1(void)
{
#if ENABLE_GPD_MESSAGE
    GPD_SET_U32REG(reg_debug_mux, 1);
    U32 _reg_gpd_debug = PNG_GET_U32REG_Twice(reg_gpd_debug);

    console_printf ("rw_st[2:0] = 0x%lx \n", _reg_gpd_debug&0x7);
    console_printf ("decom_st[2:0] = 0x%lx \n", (_reg_gpd_debug>>3)&0x7);
    console_printf ("fifo_empty = 0x%lx \n", (_reg_gpd_debug>>6)&0x1);
    console_printf ("decom_rdy = 0x%lx \n", (_reg_gpd_debug>>7)&0x1);
    console_printf ("deflt_ack_ufifo0 = 0x%lx \n", (_reg_gpd_debug>>8)&0x1);
    console_printf ("deflt_st[1:0] = 0x%lx \n", (_reg_gpd_debug>>9)&0x3);
    console_printf ("flg1 = 0x%lx \n", (_reg_gpd_debug>>11)&0x1);
    console_printf ("flg0 = 0x%lx \n", (_reg_gpd_debug>>12)&0x1);
    console_printf ("descan_ack_ufifo1 = 0x%lx \n", (_reg_gpd_debug>>13)&0x1);
    console_printf ("deflt_rdy = 0x%lx \n", (_reg_gpd_debug>>14)&0x1);
    console_printf ("deflt_ack = 0x%lx \n", (_reg_gpd_debug>>15)&0x1);
    console_printf ("decom_rdy_ufifo0 = 0x%lx \n", (_reg_gpd_debug>>16)&0x1);
    console_printf ("descan_st[3:0] = 0x%lx \n", (_reg_gpd_debug>>17)&0xf);
    console_printf ("ofifo_wrdy = 0x%lx \n", (_reg_gpd_debug>>21)&0x1);
    console_printf ("descan_rdy = 0x%lx \n", (_reg_gpd_debug>>22)&0x1);
    console_printf ("descan_ack_a1t = 0x%lx \n", (_reg_gpd_debug>>23)&0x1);
#endif
}

void PNG_Get_DebugMux2(void)
{
#if ENABLE_GPD_MESSAGE
    GPD_SET_U32REG(reg_debug_mux, 2);
    U32 _reg_gpd_debug = PNG_GET_U32REG_Twice(reg_gpd_debug);

    console_printf ("deflt_rdy = 0x%lx \n", _reg_gpd_debug&0x1);
    console_printf ("fifo1_empty = 0x%lx \n", (_reg_gpd_debug>>1)&0x1);
    console_printf ("fifo0_empty = 0x%lx \n", (_reg_gpd_debug>>2)&0x1);
    console_printf ("reg_gif_local_tbl = 0x%lx \n", (_reg_gpd_debug>>3)&0x1);
    console_printf ("flt_psram_full|lz_psram_full = 0x%lx \n", (_reg_gpd_debug>>4)&0x1);
    console_printf ("gpd_time_out = 0x%lx \n", (_reg_gpd_debug>>5)&0x1);
    console_printf ("ofifo_empty = 0x%lx \n", (_reg_gpd_debug>>6)&0x1);
    console_printf ("err_flg = 0x%lx \n", (_reg_gpd_debug>>7)&0x1);
    console_printf ("gif_cs[12:0] = 0x%lx \n", (_reg_gpd_debug>>8)&0x1FFF);
    console_printf ("gif_err = 0x%lx \n", (_reg_gpd_debug>>21)&0x1);
#endif
}

void PNG_Get_DebugMux3(void)
{
#if ENABLE_GPD_MESSAGE
    GPD_SET_U32REG(reg_debug_mux, 3);
    U32 _reg_gpd_debug = PNG_GET_U32REG_Twice(reg_gpd_debug);

    console_printf ("wr_st = 0x%lx \n", _reg_gpd_debug&0x1);
    console_printf ("gnt[5:0] = 0x%lx \n", (_reg_gpd_debug>>1)&0x3F);
    console_printf ("rw_gnt[5:0] = 0x%lx \n", (_reg_gpd_debug>>7)&0x3F);
    console_printf ("pg2ar_rdrq_rdy = 0x%lx \n", (_reg_gpd_debug>>13)&0x1);
    console_printf ("pg2ar_wtrq_rdy = 0x%lx \n", (_reg_gpd_debug>>14)&0x1);
    console_printf ("cc2ar_rdrq_rdy = 0x%lx \n", (_reg_gpd_debug>>15)&0x1);
    console_printf ("cc2ar_wtrq_rdy = 0x%lx \n", (_reg_gpd_debug>>16)&0x1);
    console_printf ("zc2ar_rdrq_rdy = 0x%lx \n", (_reg_gpd_debug>>17)&0x1);
    console_printf ("zc2ar_wtrq_rdy = 0x%lx \n", (_reg_gpd_debug>>18)&0x1);
    console_printf ("pg2ar_rdrq_pgend = 0x%lx \n", (_reg_gpd_debug>>19)&0x1);
    console_printf ("pg2ar_wtrq_pgend = 0x%lx \n", (_reg_gpd_debug>>20)&0x1);
    console_printf ("cc2ar_rdrq_pgend = 0x%lx \n", (_reg_gpd_debug>>21)&0x1);
    console_printf ("cc2ar_wtrq_pgend = 0x%lx \n", (_reg_gpd_debug>>22)&0x1);
    console_printf ("zc2ar_rdrq_pgend = 0x%lx \n", (_reg_gpd_debug>>23)&0x1);

#endif
}

void PNG_Get_DebugMux4(void)
{
#if ENABLE_GPD_MESSAGE
    GPD_SET_U32REG(reg_debug_mux, 4);
    U32 _reg_gpd_debug = PNG_GET_U32REG_Twice(reg_gpd_debug);

    console_printf ("rdq_sram_full = 0x%lx \n", _reg_gpd_debug&0x1);
    console_printf ("cmd_sram_full = 0x%lx \n", (_reg_gpd_debug>>1)&0x1);
    console_printf ("data_sram_full = 0x%lx \n", (_reg_gpd_debug>>2)&0x1);
    console_printf ("rdq_sram_empty = 0x%lx \n", (_reg_gpd_debug>>3)&0x1);
    console_printf ("cmd_sram_empty = 0x%lx \n", (_reg_gpd_debug>>4)&0x1);
    console_printf ("data_sram_empty = 0x%lx \n", (_reg_gpd_debug>>5)&0x1);

#endif
}

void PNG_Get_DebugMux5(void)
{
#if ENABLE_GPD_MESSAGE
    GPD_SET_U32REG(reg_debug_mux, 5);
    U32 _reg_gpd_debug = PNG_GET_U32REG_Twice(reg_gpd_debug);

    console_printf ("reg_ififo_radr[27:26] = 0x%lx \n", _reg_gpd_debug&0x3);

#endif
}



void PNG_Debug(void)
{
#if ENABLE_GPD_MESSAGE
    GPD_GET_U32REG_dbg(reg_int_status);
    GPD_GET_U32REG_dbg(reg_frun_cnt);
    GPD_GET_U32REG_dbg(reg_gpd_read_bits);
    console_printf("show bits:%x\n",show_fifo(16));

    PNG_Get_reg_iofifo_state();
    PNG_Get_reg_png_state();
    PNG_Get_DebugMux1();
    PNG_Get_DebugMux2();
    PNG_Get_DebugMux3();
    PNG_Get_DebugMux4();
    PNG_Get_DebugMux5();

#endif

#if 0
    console_printf("set reg_debug2_mux = 0\n");
    GPD_SET_U32REG(reg_debug2_mux, 0);
    GPD_GET_U32REG_dbg(reg_gpd_debug2);

    console_printf("set reg_debug2_mux = 1\n");
    GPD_SET_U32REG(reg_debug2_mux, 1);
    GPD_GET_U32REG_dbg(reg_gpd_debug2);
#endif

#if 0
    MDrv_GPD_PrintGPDBuf();
#endif

}


#if ENABLE_GPD_MESSAGE
void PNG_DumpRegTable(void)
{
        MS_U8 u8i;
        console_printf("=======================================================\n");
        console_printf("Dump register!!!!!\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<reg_gpd_debug2; u8i++)
        {
            GPD_GET_U32REG_dbg(u8i);
        }
        console_printf("=======================================================\n");
}
#endif
unsigned int get_length(unsigned char *addr)
{

    return ((*addr)<<24)+
        ((*(addr+1))<<16)+
        ((*(addr+2))<<8)+
        *(addr+3);
}

void get_next_IDAT(void)
{
    unsigned char *addr=my_buff->data+my_buff->data_offset;
    addr=addr+4;  //type
    this_ichk_len=next_ichk_len;

    this_ichk_start=(unsigned int) addr;
    addr=addr+this_ichk_len+4;
    next_ichk_len=get_length(addr);
    addr=addr+4;
    if(memcmp(addr,IDAT,2))
        last_ichk=1;
    my_buff->data_offset=addr-my_buff->data;

    console_printf("get_next_IDAT: [%x, %x, %x, %x %lx]\n", this_ichk_len, this_ichk_start,
        next_ichk_len, last_ichk, my_buff->data_offset);

}





int check_excess_bend(void)
{
    unsigned int addr;

    addr = drv_gpd_ififo_radr();
    addr = ((addr>>6)&0x1FFFFFF)<<3;
    if((addr>(bend&(~DRAMBASE))))
    {
        return 1;
    }
    return 0;
}


int check_engin_idle(void)
{
    unsigned int dcm_sta;
    unsigned int dfl_sta;
    unsigned int dsc_sta;
    unsigned int data;

    data = drv_gpd_png_state();
    dcm_sta = ((data>>2)&0x7);
    dfl_sta = ((data>>8)&0x3);
    dsc_sta = ((data>>15)&0xF);

    UNUSED(dfl_sta);
    UNUSED(dsc_sta);

    if(dcm_sta==0x7||dcm_sta==0)
    {
        return 1;
    }
    return 0;
}

int check_baddr_not_full(void)
{
    int i=1;
    while(i)
    {
        if (drv_gpd_bsadr_full())
           return 0;

        i--;
    }
    return 1;
}

void set_bsadr(void)
{
    if(!no_ichk)
    {
#ifdef PA
        this_ichk_start=this_ichk_start-(unsigned int)my_buff->data+buf_addr->u32PA_ReadBufferAddr;
#endif
        if(!last_ichk)
        {
            console_printf("@1 drv_gpd_set_bstart_end: (0x%x, 0x%x)\n", this_ichk_start,this_ichk_start+this_ichk_len-1);
            drv_gpd_set_bstart_end(this_ichk_start,this_ichk_start+this_ichk_len-1);

            bend =this_ichk_start+this_ichk_len-1;

            console_printf("bend=%d\n", bend);
            get_next_IDAT();

        }
        else
        {
            console_printf("@2 drv_gpd_set_bstart_end: (0x%x, 0x%x)\n", this_ichk_start,this_ichk_start+this_ichk_len-1+32);
            drv_gpd_set_bstart_end(this_ichk_start,this_ichk_start+this_ichk_len-1+32);

            bend = this_ichk_start+this_ichk_len-1+32;

            no_ichk=1;
        }
    }
}

unsigned short read_fifo(int n)
{
    read_num += n;

    while (drv_gpd_ififo_empty())
    {

        if(check_baddr_not_full())
            set_bsadr();

    }
    return read_bits_reg(n);
}

unsigned short show_fifo(int n)
{
    while(drv_gpd_ififo_empty())
    {

        if(check_baddr_not_full())
            set_bsadr();

    }
    return show_bits_reg(n);
}

void handle_IHDR(unsigned char *addr)
{

    infoptr->png_width = ((*addr)<<24)+
                                ((*(addr+1))<<16)+
                                ((*(addr+2))<<8)+
                                *(addr+3);
    infoptr->png_height = ((*(addr+4))<<24)+
                                    ((*(addr+5))<<16)+
                                    ((*(addr+6))<<8)+
                                    *(addr+7);
    infoptr->bps = *(addr+8);
    infoptr->color_type = *(addr+9);
    infoptr->compression = *(addr+10);
    infoptr->filter = *(addr+11);
    infoptr->interlace = *(addr+12);

    switch (infoptr->color_type)
    {
        case PNG_COLOR_TYPE_GRAY:

        case PNG_COLOR_TYPE_PALETTE:
            infoptr->bpp =infoptr->bps* 1;
            break;

        case PNG_COLOR_TYPE_RGB:
            infoptr->bpp=infoptr->bps* 3;
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            infoptr->bpp=infoptr->bps* 2;
            break;

        case PNG_COLOR_TYPE_RGB_ALPHA:
            infoptr->bpp=infoptr->bps* 4;
            break;
    }

}

void handle_PLTE(unsigned char *addr,unsigned int length)
{

    infoptr->palette=addr;
    infoptr->palette_num=length/3;

}

void handle_tRNS(unsigned char *addr,unsigned int length)
{

    infoptr->trans=addr;
    infoptr->num_trans=length;
    infoptr->trns_en=1;

}

void handle_first_IDAT(unsigned char **addr,unsigned int length)
{
    *addr=*addr+4;  //type
    this_ichk_len=length;
    this_ichk_start=(unsigned int)*addr;
    *addr=*addr+length+4;
    next_ichk_len=get_length(*addr);
    *addr=*addr+4;
    if(memcmp(*addr,IDAT,2))
        last_ichk=1;
}

void parse_header(void)
{
    unsigned int length;
    unsigned char *cur_addr;
    my_png_buffer *png_buff = my_buff;

    cur_addr = png_buff->data + png_buff->data_offset;
    cur_addr += 8; //signature

    while(1)
    {
        length = get_length(cur_addr);
        cur_addr += 4;     //length

        if (!memcmp(cur_addr,IHDR, 2))
        {
            cur_addr += 4; //type
            handle_IHDR(cur_addr);
            cur_addr += length; //data
        }
        else if(!memcmp(cur_addr,PLTE, 2))
        {
            cur_addr+=4; //type
            handle_PLTE(cur_addr,length);
            cur_addr+=length; //data
        }
        else if(!memcmp(cur_addr,tRNS, 2))
        {
            cur_addr+=4; //type
            handle_tRNS(cur_addr,length);
            cur_addr+=length; //data
        }
        else if(!memcmp(cur_addr,IDAT, 2))
        {

                handle_first_IDAT(&cur_addr,length);
            break;
        }
        else
        {
            cur_addr+=4; //type
            cur_addr+=length; //data
        }
        cur_addr+=4; //crc
    }
    png_buff->data_offset=cur_addr-png_buff->data;
}

int blk_cnt = 0;
int starttime;
int checkfulltime;
int bsaddrtime;
unsigned int totaltime = 0;
unsigned int donetime;
unsigned int gotime;
unsigned int timetable[1];

void verifyBitstream3(U32 u32PA, U32 u32Size)
{
    int i = 0;
    U32 u32VA;
    if (FALSE == drv_gpd_get_cacheable())
    {
        u32VA = MsOS_PA2KSEG1(u32PA);
    }
    else
    {
        u32VA = MsOS_PA2KSEG0(u32PA);
    }
    UNUSED(u32VA);
    console_printf("VA:0x%lx, PA:0x%lx, Size:0x%lx\n", u32PA, u32VA, u32Size);
    drv_gpd_set_bstart_end(u32PA, u32PA+u32Size - 1+8);

    for (i = 0; i < 4; i++)
        read_bits_reg(16);

    console_printf("printf read buffer start===================\n");
    for(i=0; i<u32Size/16; i++)
    {
        console_printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
            *(((unsigned char*)u32VA) + i*16+0),
            *(((unsigned char*)u32VA) + i*16+1),
            *(((unsigned char*)u32VA) + i*16+2),
            *(((unsigned char*)u32VA) + i*16+3),
            *(((unsigned char*)u32VA) + i*16+4),
            *(((unsigned char*)u32VA) + i*16+5),
            *(((unsigned char*)u32VA) + i*16+6),
            *(((unsigned char*)u32VA) + i*16+7),
            *(((unsigned char*)u32VA) + i*16+8),
            *(((unsigned char*)u32VA) + i*16+9),
            *(((unsigned char*)u32VA) + i*16+10),
            *(((unsigned char*)u32VA) + i*16+11),
            *(((unsigned char*)u32VA) + i*16+12),
            *(((unsigned char*)u32VA) + i*16+13),
            *(((unsigned char*)u32VA) + i*16+14),
            *(((unsigned char*)u32VA) + i*16+15)
            );
    }
    console_printf("printf read buffer end===================\n");

    console_printf("printf fifo start===================\n");
    for(i=0; i<u32Size/16; i++)
    {
        console_printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8)
            );
    }
    console_printf("printf fifo end===================\n");

}



void VerifyBitstream2(void)
{
    int i = 0;

    console_printf("Verify Bitstream start(PA):%lx  length:%lx\n", buf_addr->u32PA_ReadBufferAddr, buf_addr->u32ReadBufferSize);
    console_printf("Verify Bitstream start(VA):[%lx]\n", buf_addr->u32VA_ReadBufferAddr);
    console_printf("Verify Bitstream start(VA2PA):[%lx]\n", MsOS_VA2PA(buf_addr->u32VA_ReadBufferAddr));

    MsOS_FlushMemory();
    console_printf("Flush~~\n");

    console_printf("READBITBASE = 0x%lx\n", READBITBASE);

    drv_gpd_set_bstart_end(buf_addr->u32PA_ReadBufferAddr, buf_addr->u32PA_ReadBufferAddr+buf_addr->u32ReadBufferSize - 1 + 32);

    for (i = 0; i < 4; i++)
        read_bits_reg(16);

    console_printf("printf read buffer start===================\n");
    for(i=0; i<80; i++)
    {
        console_printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+0),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+1),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+2),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+3),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+4),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+5),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+6),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+7),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+8),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+9),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+10),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+11),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+12),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+13),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+14),
            *(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i*16+15)
            );
    }
    console_printf("printf read buffer end===================\n");

    console_printf("printf fifo start===================\n");
    for(i=0; i<80; i++)
    {
        console_printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8),
            read_bits_reg(8)
            );
    }
    console_printf("printf fifo end===================\n");
#if ENABLE_GPD_MESSAGE
extern U32 RIU_REG_BASE;

#define GPD_RIU_MAP RIU_REG_BASE  //obtain in init


#ifndef READ_BYTE
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }
#endif

#define GPD_READ_BYTE(addr)         READ_BYTE( (GPD_RIU_MAP + (addr)) )
#define GPD_READ_WORD(addr)         READ_WORD( (GPD_RIU_MAP + (addr)) )
#define GPD_WRITE_BYTE(addr, val)   WRITE_BYTE( (GPD_RIU_MAP + (addr)), (val) )
#define GPD_WRITE_WORD(addr, val)   WRITE_WORD( (GPD_RIU_MAP + (addr)), (val) )

#define GPD_MACRO_START     do {
#define GPD_MACRO_END       } while (0)

#define __HAL_GPD_ReadByte( u32Reg )   GPD_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define __HAL_GPD_Read2Byte( u32Reg )    (GPD_READ_WORD((u32Reg)<<1))

        MS_U8 u8i;
#define GPD_OFFSET(x)       ((x)<<1)
#define GPD_CHIP_REG_BASE           0x1E00
#define GPD_CLKGEN0_BASE                0x00B00
#define GPD_CLKGEN1_BASE                0x03300
#define GPD_CLKGEN2_BASE                0x00A00
#define GPD_MIU0_BASE               0x1200
#define GPD_MIU1_BASE               0x0600
#define BK_CHIPTOP_GENERAL(x)       (GPD_CHIP_REG_BASE+GPD_OFFSET(x))
#define BK_CLKGEN0_GENERAL(x)       (GPD_CLKGEN0_BASE+GPD_OFFSET(x))
#define BK_CLKGEN1_GENERAL(x)       (GPD_CLKGEN1_BASE+GPD_OFFSET(x))
#define BK_CLKGEN2_GENERAL(x)       (GPD_CLKGEN2_BASE+GPD_OFFSET(x))
#define BK_MIU0_GENERAL(x)          (GPD_MIU0_BASE+GPD_OFFSET(x))
#define BK_MIU1_GENERAL(x)          (GPD_MIU1_BASE+GPD_OFFSET(x))
#define GPD_BASE              0x13E00       //monaco
//#define GPD_BASE              0x10E00       //keres

#define BK_GPD_GENERAL(x)          (GPD_BASE+GPD_OFFSET(x))

        console_printf("=======================================================\n");
        console_printf("MIU0  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_MIU0_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");

        console_printf("=======================================================\n");
        console_printf("MIU1  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_MIU1_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");

        console_printf("=======================================================\n");
        console_printf("CHIP  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_CHIPTOP_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");

        console_printf("=======================================================\n");
        console_printf("GEN0  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_CLKGEN0_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");

        console_printf("=======================================================\n");
        console_printf("GEN1  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_CLKGEN1_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");

        console_printf("=======================================================\n");
        console_printf("GEN2  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_CLKGEN2_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");


        console_printf("=======================================================\n");
        console_printf("GPD   | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        console_printf("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            console_printf("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+1)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+2)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+3)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+4)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+5)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+6)),
            __HAL_GPD_Read2Byte(BK_GPD_GENERAL(u8i+7))
            );
        }
        console_printf("=======================================================\n");
        for(u8i=0; u8i<reg_gpd_version; u8i+=1)
        {
            GPD_GET_U32REG_dbg(u8i);
        }
#endif


}

int VerifyBitstream(void)
{

    int i = 0;

    console_printf("Verify Bitstream start:%lx  length:%lx\n", buf_addr->u32PA_ReadBufferAddr, buf_addr->u32ReadBufferSize);

    drv_gpd_set_bstart_end(buf_addr->u32PA_ReadBufferAddr, buf_addr->u32PA_ReadBufferAddr+buf_addr->u32ReadBufferSize - 1 + 32);

    for (i = 0; i < 4; i++)
        read_bits_reg(16);

    for (i = 0; i < buf_addr->u32ReadBufferSize; i++)
    {
        if (*(((unsigned char*)buf_addr->u32VA_ReadBufferAddr) + i) != read_bits_reg(8))
        {
            return 1;
        }
       console_printf(".");
    }

    return 0;

}



PngInfop PngProcess(void *ptr, U32 ocolor, U32 mode, void *roi)
{

    //K2 default setting
    unsigned char gpd_hipri = 0x15;
    int i, loop = 0;
    unsigned int free_count = (1 << 26) - 1;
    unsigned int pitch;
    unsigned int interesting_width;
    unsigned char timeout = 0;
    int status = 0;
    unsigned char default_alpha = 255;
    unsigned char png_sca = 0;
    unsigned char gpd_pgend = 0;
    unsigned short png_trns_r = 0;
    unsigned short png_trns_g = 0;
    unsigned short png_trns_b = 0;
    unsigned char *streambuf;
    unsigned int framebuf;
    unsigned int pixelbytes = 0;
    unsigned long frame_size = 0;
    unsigned int scaling_en, scaling_mode;
#if ENABLE_GPD_MESSAGE
    unsigned int nBlkNumber = 0;
#endif
    //unsigned int iofifo_state = 0;
    gpd_roi_info *roi_ptr;
    buf_addr=(gpd_addr_info*)ptr;
    roi_ptr=(gpd_roi_info*)roi;
    streambuf=(unsigned char*)buf_addr->u32VA_ReadBufferAddr;
    framebuf=(unsigned int)buf_addr->u32PA_WriteBufferAddr;


#ifdef GPD_CLOCK_PATCH

    hal_gpd_enable_clock();

#else

    if (TRUE == g_gpd_set_access_region)
    {
        *((volatile U32 *)g_gpd_clock) = 0;
    }

#endif


    drv_gpd_png_en();

    drv_gpd_Init();

    console_printf("gpd sw reset\n");



#if 0

    if (VerifyBitstream())
         console_printf("Verify fail\n");
    else
         console_printf("Verify success\n");

#elif 0
    VerifyBitstream2();
    PNG_Debug();
    while(1);
#endif

    no_ichk = 0;
    last_ichk = 0;
    blk_cnt = 0;
    totaltime = 0;

    my_buff->data = streambuf;
    my_buff->data_offset = 0;

    memset((void*)&info, 0, sizeof(info));
    gpdinflateInit(&infoptr->zstream);
    parse_header();

    #if 0
    if (infoptr->png_height > 2048 || infoptr->png_width > 2048)
    {
        console_printf("height or width exceeds 2048\n");
        return 0;
    }
    #endif

    switch (ocolor)
    {
    case ARGB8888:
        pixelbytes = 4; break;
    case RGB565:
    case YUV422:
    case YVYU422:
    case GRAY_ALPHA:
        pixelbytes = 2; break;
    case GRAY:
        pixelbytes = 1; break;
    default:
        console_printf("Unsupported output color %ld\n", ocolor);
        return NULL;
    }

    /* Check buffer size */
    if (mode == 2)
        frame_size = ((roi_ptr->width* pixelbytes + 7) & ~7) * (roi_ptr->height);
    else if (mode == 0)
        frame_size = ((infoptr->png_width * pixelbytes + 7) & ~7) * infoptr->png_height;

    if (frame_size > buf_addr->u32WriteBufferSize)
    {
        console_printf("Frame buffer is too small\n");
        return NULL;
    }

    switch(infoptr->bps)
    {
    case 1:
        infoptr->color_depth = 0;
        break;
    case 2:
        infoptr->color_depth = 1;
        break;
    case 4:
        infoptr->color_depth = 2;
        break;
    case 8:
        infoptr->color_depth = 3;
        break;
    case 16:
        infoptr->color_depth = 4;
        break;
    default:
        infoptr->color_depth = 0;
    }

    if (mode == 1)
        return infoptr;


    drv_gpd_eco_enable(1);

    drv_gpd_set_ocolor(ocolor);




    switch(infoptr->color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
        if(infoptr->trns_en)
        {
            png_trns_r=((*infoptr->trans) << 8) + *(infoptr->trans + 1);
            drv_gpd_set_trans(png_trns_r, 0, 0, 1);
        }
        else
        {
            drv_gpd_set_trans(0, 0, 0, 0);
        }
        break;

    case PNG_COLOR_TYPE_GRAY_ALPHA:
        drv_gpd_set_trans(0, 0, 0, 0);
        break;

    case PNG_COLOR_TYPE_RGB:
        if(infoptr->trns_en)
        {
            png_trns_r=((*infoptr->trans)<<8)+*(infoptr->trans+1);
            png_trns_g=((*(infoptr->trans+2))<<8)+*(infoptr->trans+3);
            png_trns_b=((*(infoptr->trans+4))<<8)+*(infoptr->trans+5);
            drv_gpd_set_trans(png_trns_r, png_trns_g, png_trns_b, 1);
        }
        else
        {
            drv_gpd_set_trans(0, 0, 0, 0);
        }
        break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
        drv_gpd_set_trans(0, 0, 0, 0);
        break;

    case PNG_COLOR_TYPE_PALETTE:
        drv_gpd_set_trans(0, 0, 0, 0);
        drv_gpd_set_palette(infoptr->palette,infoptr->palette_num,infoptr->trans,infoptr->num_trans);
        break;

    default:
        console_printf("color_type(%d) not implemented.\n",infoptr->color_type);
    }

    //set free count
    drv_gpd_set_frun_cnt(free_count);

    if (mode == 2)
    {
        if (roi_ptr->hstart + roi_ptr->width > infoptr->png_width || roi_ptr->vstart + roi_ptr->height > infoptr->png_height)
        {
            console_printf("Error: ROI exceeds image region\n");
            return 0;
        }

        if (infoptr->interlace)
        {
            if (roi_ptr->width < 8)
            {
                console_printf("Error: roi width < 8 for interlace PNG\n");
                return 0;
            }
            roi_ptr->hstart &= ~7UL;
            roi_ptr->vstart &= ~7UL;
        }
        interesting_width = roi_ptr->width;
        drv_gpd_set_ROI(1, roi_ptr->hstart, roi_ptr->vstart, roi_ptr->width, roi_ptr->height);
    }
    else
    {
        interesting_width = infoptr->png_width;
        drv_gpd_set_ROI(0, 64, 64, 64, 64);
    }

    if ((scaling_en = drv_gpd_get_scaling_mode() & 1))
    {
        scaling_mode = drv_gpd_get_scaling_mode() >> 1;

        if (scaling_mode == 0)
            interesting_width = (interesting_width + 1) >> 1;
        else if (scaling_mode == 1)
            interesting_width = (interesting_width + 3) >> 2;
        else if (scaling_mode == 2)
            interesting_width = (interesting_width + 7) >> 3;
        else if (scaling_mode == 3)
            interesting_width = (interesting_width + 15) >> 4;
    }

    //set pitch
    #if 1

    pitch = (interesting_width * pixelbytes + 7) >> 3;

    #else

    if (ocolor == ARGB8888)
        pitch = (infoptr->png_width + 1) >> 1;
    else if (ocolor == GRAY)
        pitch = (infoptr->png_width + 7) >> 3;
    else
        pitch = (infoptr->png_width + 3) >> 2;



    #endif


    drv_gpd_set_pitch(pitch);

    //set height and width
    drv_gpd_set_iwidth_height(infoptr->png_width,infoptr->png_height);


    console_printf("width = %d, height = %d\n", infoptr->png_width, infoptr->png_height);



    if (iccp_en)
    {
        drv_gpd_set_iccp_dtsize(1, 32);

        /*
        data = (PNG_COLOR_TYPE_RGB_ALPHA << PNG_COLOR_TYPE_SHF) +
               (infoptr->interlace << GPD_INTERLACE_SHF) +
               (infoptr->color_depth << PNG_COLOR_DEPTH_SHF) +
               (infoptr->trns_en << PNG_TRANS_EN_SHF) +
               (png_trns_b << PNG_TRANS_B_SHF);
        */



        drv_gpd_set_type(PNG_COLOR_TYPE_RGB_ALPHA, infoptr->interlace, infoptr->color_depth);

    }
    else
    {
        //set only_decom and dtbl_size;
        drv_gpd_set_iccp_dtsize(0,32);

        /*
        data = (infoptr->color_type << PNG_COLOR_TYPE_SHF) +
               (infoptr->interlace << GPD_INTERLACE_SHF) +
               (infoptr->color_depth << PNG_COLOR_DEPTH_SHF) +
               (infoptr->trns_en << PNG_TRANS_EN_SHF) +
               (png_trns_b << PNG_TRANS_B_SHF);
        */

        drv_gpd_set_type(infoptr->color_type, infoptr->interlace, infoptr->color_depth);
    }

    //set interlace and scanline width and scanline height
    if(infoptr->interlace)
    {
        if (infoptr->png_width >= 2)
            png_sca |= (1 << 4);
        if (infoptr->png_width >= 3)
            png_sca |= (1 << 2);
        if (infoptr->png_width >= 5)
            png_sca |= (1);
        if (infoptr->png_height >= 3)
            png_sca |= (1 << 3);
        if (infoptr->png_height >= 5)
            png_sca |= (1 << 1);

        drv_gpd_set_scline_width_interlace(infoptr->png_width, infoptr->bpp);
        drv_gpd_set_scline_height_interlace(infoptr->png_height);
    }
    else
    {
        drv_gpd_set_scline_width_progressive(infoptr->png_width, infoptr->bpp);
        drv_gpd_set_scline_height_progressive(infoptr->png_height);
    }

    drv_gpd_set_sca_dfa(png_sca, default_alpha);
    drv_gpd_set_ltsize(288);

    drv_gpd_set_pgend(gpd_pgend);

    if (is_stb())
    {
        //for U01 ECO item
        drv_gpd_set_hipri(gpd_hipri);
    }

    //set istart
    drv_gpd_set_istart(framebuf);

    for (i = 0; i < 4; i++)
        read_fifo(16);



    console_printf("Flush done\n");



    drv_gpd_set_png_done(0);

#if PRINT_GPD_DECODE_TIME
    printf("before set png go @ [%ld]ms\n", MsOS_GetSystemTime());
#endif

    drv_gpd_set_png_go();



    console_printf("show bits:%x\n",show_fifo(16));


    //parse zlib and block header
    status = gpdinflate(&infoptr->zstream, 1);


    console_printf("inflate ok\n");


    if(status == Z_STREAM_ERROR)
    {
        console_printf("block header error\n");
        return 0;
    }


    console_printf("=== block %d ===\n", ++nBlkNumber);
#if 0
    PNG_Get_reg_iofifo_state();
    PNG_Get_reg_png_state();
    //PNG_Debug();
    //PNG_DumpRegTable();
#endif

#if PRINT_GPD_DECODE_TIME
    U32 u32StartTime = MsOS_GetSystemTime();
    U32 u32TotalTime = 0;
#endif

    drv_gpd_set_blk_go();


    console_printf("blk go\n");


    while(1)
    {


        if(check_baddr_not_full())
        {

            set_bsadr();

        }

        //end of block
        if (drv_gpd_png_eob())
        {

            console_printf("end of block\n");

            //last block

            if (status == Z_STREAM_END)
            {

                console_printf("last block\n");

                break;
            }

            if (status == Z_STREAM_ERROR)
            {
                console_printf("block header error\n");
                return 0;
            }

            while (!drv_gpd_png_blk_done())
            {

            switch (drv_gpd_check_engine())
                {
                case -1:
                        loop++;
                    if (loop > 15)
                    {
                            timeout = 1;
#if 0
                            PNG_Debug();
                            PNG_DumpRegTable();
#endif
                            console_printf("PNG EOB, error -1\n");
                            goto END_DECODE;
                    }
                    break;

                case -2:
                    loop++;
                        if (loop > 15)
                    {
                            timeout=1;
#if 0
                            PNG_Debug();
                            PNG_DumpRegTable();
#endif
                            console_printf("PNG EOB, error -2\n");
                            goto END_DECODE;
                    }
                        break;

                case 0:
                    break;
                }
                MsOS_DelayTaskUs(10);
            }

#if PRINT_GPD_DECODE_TIME
            u32TotalTime+=MsOS_GetSystemTime()-u32StartTime;
#endif

            console_printf("block done\n");

#if 0
            PNG_Debug();
#endif

            loop = 0;

            gpdinflateReset(&infoptr->zstream);
            status = gpdinflate(&infoptr->zstream, 0);


            console_printf("inflate ok\n");


            //set block_go
            console_printf("=== block %d ===\n", ++nBlkNumber);
#if 0
            PNG_Get_reg_iofifo_state();
            PNG_Get_reg_png_state();
            //PNG_Debug();
            //PNG_DumpRegTable();
#endif
            drv_gpd_set_blk_go();

#if PRINT_GPD_DECODE_TIME
            u32StartTime = MsOS_GetSystemTime();
#endif
            console_printf("blk go\n");

        }
        else
        {

            switch (drv_gpd_check_engine())
            {
            case -1:
                loop++;
                if (loop > 15)
                {
                    timeout = 1;
#if 0
                    PNG_Debug();
                    PNG_DumpRegTable();
#endif
                    console_printf("not PNG EOB, error -1\n");
                    goto END_DECODE;
                }
                break;

            case -2:
                loop++;
                if (loop > 15)
                {
                    timeout = 1;
#if 0
                    PNG_Debug();
                    PNG_DumpRegTable();
#endif
                    console_printf("not PNG EOB, error -2\n");
                    goto END_DECODE;
                }
                break;

            case 0:
                break;
            }
            MsOS_DelayTaskUs(10);
        }
    }

END_DECODE:

    if (!timeout)
    {
        while (!drv_gpd_png_blk_done()) MsOS_DelayTaskUs(10);

        drv_gpd_set_png_done(1);

#if PRINT_GPD_DECODE_TIME
        u32TotalTime+=MsOS_GetSystemTime()-u32StartTime;
        printf("end decode @ [%ld]ms\n", MsOS_GetSystemTime());
        printf("u32TotalTime=%ld\n", u32TotalTime);
#endif

        console_printf("png done\n");


    }
    else
    {

        console_printf("Unusual Decode End\n");
        drv_gpd_dbg_dump();
        return 0;
    }

    return  infoptr;
}

