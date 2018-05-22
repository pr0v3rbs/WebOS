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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif
#include "gpd_reg.h"
#include "drvgpd.h"
#include "MsCommon.h"
#include "MsOS.h"
#include "osalGPD.h"
#include "halCHIP.h"

U32 READBITBASE;
static U32 g_iwidth;
static U32 g_iheight;
static U32 g_gpd_cacheable = TRUE;
static U32 gRIU_REG_BASE;

#define DRVGPD_FLIP_GO


void _GPD_PrintMem(U32 u32Addr, U32 u32Size)
{
    U32 u32i;
    printf("===========================================================\n");
    printf("print memory addr=0x%lx, size=0x%lx\n", u32Addr, u32Size);
    printf("===========================================================\n");

    for(u32i=0; u32i<u32Size/16+((u32Size%16)? 1:0); u32i++)
    {
        printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((U8*)(u32Addr+u32i*16)), *((U8*)(u32Addr+u32i*16+1)), *((U8*)(u32Addr+u32i*16+2)), *((U8*)(u32Addr+u32i*16+3)),
        *((U8*)(u32Addr+u32i*16+4)), *((U8*)(u32Addr+u32i*16+5)), *((U8*)(u32Addr+u32i*16+6)), *((U8*)(u32Addr+u32i*16+7)),
        *((U8*)(u32Addr+u32i*16+8)), *((U8*)(u32Addr+u32i*16+9)), *((U8*)(u32Addr+u32i*16+10)), *((U8*)(u32Addr+u32i*16+11)),
        *((U8*)(u32Addr+u32i*16+12)), *((U8*)(u32Addr+u32i*16+13)), *((U8*)(u32Addr+u32i*16+14)), *((U8*)(u32Addr+u32i*16+15))
        );
    }
    printf("===========================================================\n");
}


void drv_gpd_set_scaling(unsigned int enable, unsigned int mode)
{

#if defined(GPD_SCALING_SUPPORT)
    GPD_SET_U32REG(reg_scale_en, enable);
    GPD_SET_U32REG(reg_scale_md, mode);
#endif

}

unsigned int drv_gpd_get_scaling_mode(void)
{
#if defined(GPD_SCALING_SUPPORT)
    return GPD_GET_U32REG(reg_scale_en) | GPD_GET_U32REG(reg_scale_md) << 1;
#endif
    return 0;
}

#if (defined(__arm__))
void drv_eng_always_active(unsigned int value)
{
    GPD_SET_U32REG(reg_eng_always_active, value);

}
#endif


unsigned int drv_reg_iofifo_state(void)
{

    return GPD_GET_U32REG(reg_iofifo_state);
}

void drv_gpd_eco_enable(unsigned int enable)
{

    GPD_SET_U32REG(reg_spare1, enable);

}

#if 0
static U32 u32ZbufVA;
static U32 u32CbufVA;
#endif

void drv_gpd_set_zbuf_bas(unsigned int base)
{
#if 0
    if (FALSE == g_gpd_cacheable)
    {
        u32ZbufVA = MsOS_PA2KSEG1(base<<3);
    }
    else
    {
        u32ZbufVA = MsOS_PA2KSEG0(base<<3);
    }
#endif    
    GPD_SET_U32REG(reg_zbuf_bas, base & (~DRAMBASE));
}

void drv_gpd_set_cbuf_bas(unsigned int base)
{
#if 0
    if (FALSE == g_gpd_cacheable)
    {
        u32CbufVA = MsOS_PA2KSEG1(base<<3);
    }
    else
    {
        u32CbufVA = MsOS_PA2KSEG0(base<<3);
    }
#endif    
    GPD_SET_U32REG(reg_cbuf_bas, base & (~DRAMBASE));

}


void set_nop_cmd(U32 num)
{
    U32 i;
    for(i = 0; i < num; i++)
        drv_gpd_set_iwidth_height(g_iwidth, g_iheight);
}

unsigned int drv_gpd_png_state(void)
{
    return GPD_GET_U32REG(reg_png_state);
}

unsigned int drv_gpd_bitpos(void)
{
    return GPD_GET_U32REG(reg_bitpos);
}

unsigned int drv_gpd_ififo_radr(void)
{
    return GPD_GET_U32REG(reg_ififo_radr);
}

unsigned int drv_gpd_ififo_empty(void)
{
    return GPD_GET_U32REG(reg_ififo_empty);
}

unsigned int drv_gpd_ififo_full(void)
{
    return GPD_GET_U32REG(reg_ififo_full);
}

unsigned int drv_gpd_ofifo_done(void)
{
    //return GPD_GET_U32REG(reg_ofifo_done);
    return 0;
}

unsigned int drv_gpd_bsadr_full(void)
{
    return GPD_GET_U32REG(reg_bsadr_full);
}

unsigned int drv_gpd_png_blk_done(void)
{
    return GPD_GET_U32REG(reg_png_blk_done);
}

unsigned int drv_gpd_png_eob(void)
{
    return GPD_GET_U32REG(reg_png_eob);
}

#if 0
void png_gif_en(void)
{
    unsigned int data;

    data=REDREG32(REG_GPD_8014);
    data|=(1<<7);
    WRTREG32(REG_GPD_8014,data);
}

unsigned short read_bits(int n)
{
    while((REDREG32(REG_GPD_8014)&IFIFO_EMPTY))
    {
       GifQprintf("RIFIFO_EMPTY\n");
    }
    return read_bits_reg(n);
}

unsigned short show_bits(int n)
{
    while((REDREG32(REG_GPD_8014)&IFIFO_EMPTY))
    {
       GifQprintf("SIFIFO_EMPTY\n");
    }
    return show_bits_reg(n);
}
#else

void drv_gpd_png_en(void)
{
    GPD_SET_U32REG(reg_png_en, 1);
    //drv_eng_always_active(0x0);
}

void drv_gpd_gif_en(void)
{
    GPD_SET_U32REG(reg_png_en, 1);
    #if (defined(__arm__))
    drv_eng_always_active(0xf);
    #endif
}

unsigned short read_bits(int n)
{
    while (GPD_GET_U32REG(reg_ififo_empty))
    {
       console_printf("RIFIFO_EMPTY\n");
    }
    return read_bits_reg(n);
}

unsigned short show_bits(int n)
{
    while (GPD_GET_U32REG(reg_ififo_empty))
    {
       console_printf("SIFIFO_EMPTY\n");
    }
    return show_bits_reg(n);
}
#endif

extern int num, blk_typ;
void drv_gpd_dbg_dump(void)
{
    U32 data;

    data = drv_gpd_png_state();
    UNUSED(data);
    console_printf("PNG state = %lx\n", data);
    console_printf("Block stop = %x\n", num);
    console_printf("Block Type = %x\n", blk_typ);
}

#if 0
void drv_gpd_Init(void)
{
    memset((void*)&GPDreg,0,sizeof(_GPDreg));
    MDrv_CQ_SWReset();

    set_single_cmd(0x831c,GPD_RST);
    MDrv_CQ_Fire();
}

void drv_gpd_set_bstart_end(unsigned int bstart, unsigned int bend)
{
    /*Set Bstart & Bend*/
    U32 dwPtr;
    MDrv_CQ_SWReset();
#ifdef DUMPQ
    GPDreg.h8340 = (((bstart&(~DRAMBASE))&(7))+8); //Boffset
    set_single_cmd(0x8340,GPDreg.h8340);

    GPDreg.h8334 = ((bstart&(~DRAMBASE))&(~7));
    GPDreg.h8338 = ((bend&(~DRAMBASE))&(~7));
#else
    GPDreg.h8334 = bstart;
    GPDreg.h8338 = bend;
#endif

    set_single_cmd(0x8334,GPDreg.h8334);
    set_single_cmd(0x8338,GPDreg.h8338);
    set_single_cmd(0x8340,0);
    set_nop_cmd(2);
    set_single_cmd(0x831c,GPD_BSADDR_GO);

    MDrv_CQ_Fire();
}

void drv_gpd_set_ocolor(int ocolor_typ)
{
    MDrv_CQ_SWReset();
    GPDreg.h8358 &= ~(0x03<<GPD_OCOLOR_SHF);
    GPDreg.h8358 |= (ocolor_typ<<GPD_OCOLOR_SHF);
    set_single_cmd(0x8358,GPDreg.h8358);
}

void drv_gpd_write_cmap(unsigned int num, unsigned int * ptr)
{
    unsigned int i,vdata;

    while(num > 0)
    {
        vdata = ((DEFAULT_ALPHA<<24)|(((unsigned char *)(*ptr))[0]<<16)
                                    |(((unsigned char *)(*ptr))[1]<<8)
                                    |((unsigned char *)(*ptr))[2]);
        set_single_cmd(0x8320,vdata);
        num--;
        (*ptr)+=3;
    }
}
#else

void drv_gpd_Init(void)
{
    g_iwidth = 0;
    g_iheight = 0;
    hal_gpd_SetMIUProtectMask(gRIU_REG_BASE, TRUE);
    console_printf("before reset.....bitpos=%d\n", drv_gpd_bitpos());
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 0);

#if ENABLE_GPD_PATCH
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 0);

    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 1);
    GPD_SET_U32REG(reg_gpd_rst, 0);
#endif

    console_printf("after reset.....bitpos=%d\n", drv_gpd_bitpos());
    hal_gpd_SetMIUProtectMask(gRIU_REG_BASE, FALSE);

#if ENABLE_GPD_PATCH
    // disable clock gating
    GPD_SET_U32REG(reg_mreq_always_active, 1);
    GPD_SET_U32REG(reg_eng_always_active, 0x3f);
    // disable zcache/ccache
    GPD_SET_U32REG(reg_cache_hit_cmp, 0x0);

    console_printf("set reg_mreq_always_active = 1\n");
    console_printf("set reg_eng_always_active = 0x3f\n");
    console_printf("set reg_cache_hit_cmp = 0\n");
#endif

}

void drv_gpd_set_bstart_end(unsigned int bstart, unsigned int bend)
{

    GPD_SET_U32REG(reg_gpd_bstart, bstart & (~DRAMBASE));
    GPD_SET_U32REG(reg_gpd_bend, bend & (~DRAMBASE));
    GPD_SET_U32REG(reg_gpd_boffset, 0);


#ifdef DRVGPD_FLIP_GO
	{
		U32 val = GPD_GET_U32REG(reg_gpd_bsaddr_go) ^ 1;
		GPD_SET_U32REG(reg_gpd_bsaddr_go, val);
	}
#else
    GPD_SET_U32REG(reg_gpd_bsaddr_go, 1);
#endif
}

void drv_gpd_set_ocolor(int ocolor_typ)
{
    GPD_SET_U32REG(reg_gpd_ocolor, ocolor_typ);
}


void drv_gpd_write_cmap(unsigned int num, unsigned char *ptr, int trans_idx)
{
    unsigned int i, vdata;
    unsigned char r, g, b, a;

    GPD_SET_U32REG(reg_gif_local_tbl, 0);
    for (i = 0; i < num; i++)
    {
        if (ptr)
        {
            r = *ptr++;
            g = *ptr++;
            b = *ptr++;
        }
        else
        {
            r = read_bits(8);
            g = read_bits(8);
            b = read_bits(8);
        }

        if (i == trans_idx)
            a = 0;
        else
            a = 0xFF;

        vdata = (a << 24) | (r << 16) | (g << 8) | b;
        GPD_SET_U32REG(reg_gpd_cmem_wd, vdata);
    }
}



#endif

#if 0
void drv_gpd_set_pitch(unsigned int pitch)
{
    GPDreg.h832c &=~(0xFFFF<<GPD_PITCH_SHF);
    GPDreg.h832c |=(pitch<<GPD_PITCH_SHF);
    set_single_cmd(0x832c,GPDreg.h832c);
}

void drv_gpd_set_gif_swidth_sheight(unsigned int width ,unsigned int height)
{
    GPDreg.h8348 &=~(0xFFFF<<GPD_ROI_WIDTH_SHF);
    GPDreg.h8348 |=(width<<GPD_ROI_WIDTH_SHF);
    GPDreg.h834c &=~0x0000FFFF;
    GPDreg.h834c |=height;
    set_single_cmd(0x8348,GPDreg.h8348);
    set_single_cmd(0x834c,GPDreg.h834c);
}

void drv_gpd_set_iwidth_height(unsigned int width, unsigned int height)
{
    GPDreg.h8330 = ((height<<GPD_IHEIGHT_SHF)|width);
    set_single_cmd(0x8330,GPDreg.h8330);
    pwidth=width;
    pheight=height;
}

void drv_gpd_set_istart(unsigned int vdata)
{
    GPDreg.h833c = vdata;
    set_single_cmd(0x833c,GPDreg.h833c);
    MDrv_CQ_Fire();
}

void drv_gpd_set_ROI(U32 enable,U32 hstart, U32 vstart, U32 width, U32 height)
{
    GPDreg.h8344 = hstart;
    GPDreg.h8348 = vstart|(width<<16);
    GPDreg.h834c &= ~(0xFFFF|(1<<16));

    if(enable)
        GPDreg.h834c |= height|(enable<<16);

    set_single_cmd(0x8344,GPDreg.h8344);
    set_single_cmd(0x8348,GPDreg.h8348);
    set_single_cmd(0x834c,GPDreg.h834c);
}

void drv_gpd_set_interlace(unsigned char state)
{
    MDrv_CQ_SWReset();

    if (state == TRUE)
        GPDreg.h83b8 |= GPD_INTERLACE;
    else
        GPDreg.h83b8 &= ~GPD_INTERLACE;
    set_single_cmd(0x83b8,GPDreg.h83b8);
    MDrv_CQ_Fire();
}

void drv_gpd_set_gif_local_map_size(int bitperpixel)
{
    MDrv_CQ_SWReset();

    if (!bitperpixel)
    {
        GPDreg.h8328 &= ~GIF_LOCAL_TBL;
    }
    else
    {
        GPDreg.h8328 |= GIF_LOCAL_TBL;
        GPDreg.h8328 &= ~(0x07<<GIF_LTAB_SIZ_SHF);
        GPDreg.h8328 |= (bitperpixel<<GIF_LTAB_SIZ_SHF);
    }
    set_single_cmd(0x8328,GPDreg.h8328);
    MDrv_CQ_Fire();
}
#else

void drv_gpd_set_pitch(unsigned int pitch)
{
    GPD_SET_U32REG(reg_gpd_pitch, pitch);
}

void drv_gpd_set_gif_swidth_sheight(unsigned int width ,unsigned int height)
{
    GPD_SET_U32REG(reg_gpd_roi_width, width);
    GPD_SET_U32REG(reg_gpd_roi_height, height);
}

void drv_gpd_set_iwidth_height(unsigned int iwidth, unsigned int iheight)
{
    g_iwidth = iwidth;
    g_iheight = iheight;
    GPD_SET_U32REG(reg_gpd_iwidth, iwidth);
    GPD_SET_U32REG(reg_gpd_iheight, iheight);
}

void drv_gpd_set_istart(unsigned int istart)
{

    //GPD_SET_U32REG(reg_gpd_istart, istart & (~DRAMBASE));
    GPD_SET_U32REG(reg_gpd_istart, istart & (~DRAMBASE));


}

void drv_gpd_set_ROI(U32 enable, U32 hstart, U32 vstart, U32 width, U32 height)
{
    GPD_SET_U32REG(reg_gpd_roi_hstart, hstart);
    GPD_SET_U32REG(reg_gpd_roi_vstart, vstart);
    GPD_SET_U32REG(reg_gpd_roi_width, width);
    GPD_SET_U32REG(reg_gpd_roi_height, height);

    if (enable)
        GPD_SET_U32REG(reg_gpd_roi_en, 1);
    else
        GPD_SET_U32REG(reg_gpd_roi_en, 0);
}

void drv_gpd_set_interlace(unsigned char interlace)
{
    if (interlace)
        GPD_SET_U32REG(reg_gpd_interlace, 1);
    else
        GPD_SET_U32REG(reg_gpd_interlace, 0);
}

void drv_gpd_set_gif_local_map_size(int bpp)
{
    if (!bpp)
    {
        GPD_SET_U32REG(reg_gif_local_tbl, 0);
    }
    else
    {
        GPD_SET_U32REG(reg_gif_ltbl_size, bpp);
        GPD_SET_U32REG(reg_gif_local_tbl, 1);
    }
}
#endif

#if 0
void drv_gpd_set_gif_go(void)
{
    MDrv_CQ_SWReset();
    set_single_cmd(0x831c,GIF_GO);
    MDrv_CQ_Fire();
}

unsigned int drv_gpd_gif_done(void)
{
    return (REDREG32(REG_GPD_8014)&GIF_DONE);
}

void drv_gpd_set_trans_rg(unsigned short r,unsigned short g)
{
    GPDreg.h8350=(r+(g<<PNG_TRANS_G_SHF));
    set_single_cmd(0x8350,GPDreg.h8350);
}

void drv_gpd_set_palette(unsigned char *palette,unsigned short palette_num,unsigned char *trans,unsigned short num_trans)
{
    int i;
    unsigned char palette_alpha;
    unsigned char R,G,B;
    unsigned int palette_data;

    for(i=0;i<palette_num;i++)
    {
        if(i<num_trans)
            palette_alpha=*(trans+i);
        else
            palette_alpha=DEFAULT_ALPHA;

        R=*(palette+i*3);
        G=*(palette+i*3+1);
        B=*(palette+i*3+2);
        GPDreg.h8320=palette_data=(R<<24)+(G<<16)+(B<<8)+palette_alpha;

        set_single_cmd(0x8320,GPDreg.h8320);
    }

    for(i=palette_num;i<256;i++)
        set_single_cmd(0x8320,0);
}

void drv_gpd_set_pgend(unsigned char pgend)
{
    GPDreg.h83c0 &=~(0x3<<GPD_PGEND_SHF);
    GPDreg.h83c0 |=(pgend<<GPD_PGEND_SHF);

    set_single_cmd(0x83c0,GPDreg.h83c0);
}
#else

void drv_gpd_set_gif_go(void)
{
#ifdef DRVGPD_FLIP_GO
	{
		U32 val = GPD_GET_U32REG(reg_gif_go) ^ 1;
		GPD_SET_U32REG(reg_gif_go, val);
	}
#else
    GPD_SET_U32REG(reg_gif_go, 1);
#endif
}

unsigned int drv_gpd_gif_done(void)
{
    return GPD_GET_U32REG(reg_gif_done);
}

void drv_gpd_set_trans(unsigned short r, unsigned short g, unsigned short b, unsigned char enable)
{
    GPD_SET_U32REG(reg_png_trans_r, r);
    GPD_SET_U32REG(reg_png_trans_g, g);
    GPD_SET_U32REG(reg_png_trans_b, b);
    GPD_SET_U32REG(reg_png_trans_en, enable);
}

void drv_gpd_set_palette(unsigned char *palette, unsigned short palette_num, unsigned char *trans, unsigned short num_trans)
{
    int i;
    unsigned char palette_alpha;
    unsigned char R,G,B;
    unsigned int palette_data;
    //unsigned int tmp;

    for (i = 0; i < palette_num; i++)
    {
        if (i < num_trans)
            palette_alpha = *(trans + i);
        else
            palette_alpha = DEFAULT_ALPHA;

        R = *(palette + i * 3);
        G = *(palette + i * 3 + 1);
        B = *(palette + i * 3 + 2);
        palette_data = (R << 24) + (G << 16) + (B << 8) + palette_alpha;

        GPD_SET_U32REG(reg_gpd_cmem_wd, palette_data);


    }

    for (i = palette_num; i < 256; i++)
        GPD_SET_U32REG(reg_gpd_cmem_wd, 0);
}

void drv_gpd_set_pgend(unsigned char pgend)
{
    GPD_SET_U32REG(reg_gpd_pgend, pgend);
}

void drv_gpd_set_hipri(unsigned short hipri)
{
    GPD_SET_U32REG(reg_hipri, hipri);
}

#endif

#if 0
void drv_gpd_set_frun_cnt(unsigned int frun_cnt)
{
    GPDreg.h83c4=frun_cnt;
    set_single_cmd(0x83c4,GPDreg.h83c4);
}

void drv_gpd_set_iccp_dtsize(unsigned char iccp,unsigned char dtsize)
{
     GPDreg.h8328 |=(iccp);

     GPDreg.h8328 &=~((0x3f<<PNG_DTBL_SIZ_SHF));
     GPDreg.h8328 |=(dtsize<<PNG_DTBL_SIZ_SHF);

     set_single_cmd(0x8328,GPDreg.h8328);
}

void drv_gpd_set_type(unsigned int data)
{
    GPDreg.h83b8=data;

    set_single_cmd(0x83b8,GPDreg.h83b8);
}

void drv_gpd_set_default_alpha(unsigned int vdata)
{
    GPDreg.h834c &= ~(0xFF<<GPD_DFALT_ALPHA_SHF);
    GPDreg.h834c |= (vdata<<GPD_DFALT_ALPHA_SHF);

    set_single_cmd(0x834c,GPDreg.h834c);
}

void drv_gpd_set_sca_dfa(unsigned char sca,unsigned char dfa)
{
    GPDreg.h834c &= ~(0x1f<<PNG_SCA_SHF);
    GPDreg.h834c |= (sca<<PNG_SCA_SHF);

    GPDreg.h834c &= ~(0xff<<GPD_DFALT_ALPHA_SHF);
    GPDreg.h834c |= (dfa<<GPD_DFALT_ALPHA_SHF);

    set_single_cmd(0x834c,GPDreg.h834c);
}

void drv_gpd_set_ltsize(unsigned short ltsize)
{
    GPDreg.h8358 &= ~(0x1ff<<PNG_LTBL_SIZ_SHF);
    GPDreg.h8358 |= (ltsize<<PNG_LTBL_SIZ_SHF);

    set_single_cmd(0x8358,GPDreg.h8358);
}
#else

void drv_gpd_set_frun_cnt(unsigned int frun_cnt)
{
    GPD_SET_U32REG(reg_frun_cnt, frun_cnt);
}

void drv_gpd_set_iccp_dtsize(unsigned char iccp, unsigned char dtsize)
{
    GPD_SET_U32REG(reg_gpd_only_decom_en, iccp);
    GPD_SET_U32REG(reg_png_dtbl_size, dtsize);
}

void drv_gpd_set_type(unsigned char color_type, unsigned char interlace, unsigned char color_depth)
{
    GPD_SET_U32REG(reg_png_color_type, color_type);
    GPD_SET_U32REG(reg_gpd_interlace, interlace);
    GPD_SET_U32REG(reg_png_color_depth, color_depth);
}

void drv_gpd_set_default_alpha(unsigned char alpha)
{
    GPD_SET_U32REG(reg_gpd_default_alpha, alpha);
}

void drv_gpd_set_sca_dfa(unsigned char sca, unsigned char alpha)
{
    GPD_SET_U32REG(reg_png_sca, sca);
    GPD_SET_U32REG(reg_gpd_default_alpha, alpha);
}

void drv_gpd_set_ltsize(unsigned short ltsize)
{
    GPD_SET_U32REG(reg_png_ltbl_size, ltsize);
}
#endif

#if 0
void drv_gpd_set_sc01_width(unsigned int data)
{
    GPDreg.h8398=data;

    set_single_cmd(0x8398,GPDreg.h8398);
}

void drv_gpd_set_sc23_width(unsigned int data)
{
    GPDreg.h839c=data;

    set_single_cmd(0x839c,GPDreg.h839c);
}

void drv_gpd_set_sc45_width(unsigned int data)
{
    GPDreg.h83a0=data;

    set_single_cmd(0x83a0,GPDreg.h83a0);
}

void drv_gpd_set_sc60_wh(unsigned int data)
{
    GPDreg.h83a4=data;

    set_single_cmd(0x83a4,GPDreg.h83a4);
}

void drv_gpd_set_sc12_height(unsigned int data)
{
    GPDreg.h83a8=data;

    set_single_cmd(0x83a8,GPDreg.h83a8);
}

void drv_gpd_set_sc34_height(unsigned int data)
{
    GPDreg.h83ac=data;

    set_single_cmd(0x83ac,GPDreg.h83ac);
}

void drv_gpd_set_sc56_height(unsigned int data)
{
    GPDreg.h83b0=data;

    set_single_cmd(0x83b0,GPDreg.h83b0);
}
#else

void drv_gpd_set_scline_width_progressive(unsigned int width, unsigned char bpp)
{
    GPD_SET_U32REG(reg_png_scline0_width, ((width * bpp + 7) >> 3) - 1);
}

void drv_gpd_set_scline_height_progressive(unsigned int height)
{
    GPD_SET_U32REG(reg_png_scline0_height, height - 1);
}

void drv_gpd_set_scline_width_interlace(unsigned int width, unsigned char bpp)
{
    GPD_SET_U32REG(reg_png_scline0_width, ((((width     + 7) >> 3) * bpp + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline1_width, ((((width - 4 + 7) >> 3) * bpp + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline2_width, ((((width     + 3) >> 2) * bpp + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline3_width, ((((width - 2 + 3) >> 2) * bpp + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline4_width, ((((width     + 1) >> 1) * bpp + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline5_width, ((((width - 1 + 1) >> 1) * bpp + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline6_width, ((( width              ) * bpp + 7) >> 3) - 1);
}

void drv_gpd_set_scline_height_interlace(unsigned int height)
{
    GPD_SET_U32REG(reg_png_scline0_height, ((height     + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline1_height, ((height     + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline2_height, ((height - 4 + 7) >> 3) - 1);
    GPD_SET_U32REG(reg_png_scline3_height, ((height     + 3) >> 2) - 1);
    GPD_SET_U32REG(reg_png_scline4_height, ((height - 2 + 3) >> 2) - 1);
    GPD_SET_U32REG(reg_png_scline5_height, ((height     + 1) >> 1) - 1);
    GPD_SET_U32REG(reg_png_scline6_height, ((height - 1 + 1) >> 1) - 1);
}
#endif

#if 0
void drv_gpd_set_png_go(void)
{
    MDrv_CQ_SWReset();
    set_nop_cmd(2);
    set_single_cmd(0x831c,PNG_GO);
    MDrv_CQ_Fire();
}

U32 val = 1;

void drv_gpd_set_blk_go(void)
{
    U32 dbg=0xff;

    MDrv_CQ_SWReset();

    set_nop_cmd(5);
    set_single_cmd(0x831c,PNG_BLK_GO);

    if(val)
        GPDreg.h8328 |=(GIF_LOCAL_TBL);
    else
        GPDreg.h8328 &=~(GIF_LOCAL_TBL);

    set_single_cmd(0x8328,GPDreg.h8328);

    MDrv_CQ_Fire();

    while(dbg!=val)
        dbg=((gpd_read_debug_port(0)&_BIT28)>>28);

    val=!val;
}

void drv_gpd_set_png_done(void)
{
    MDrv_CQ_SWReset();

    GPDreg.h83b8 |= (PNG_DONE);

    set_single_cmd(0x83b8,GPDreg.h83b8);
    MDrv_CQ_Fire();
}

void drv_gpd_set_cmp_type(unsigned char cmp)
{
    MDrv_CQ_SWReset();

    GPDreg.h83b8 &=~(0x3<<PNG_COMPRESS_TYPE_SHF);
    GPDreg.h83b8 |= (cmp<<PNG_COMPRESS_TYPE_SHF);

    set_single_cmd(0x83b8,GPDreg.h83b8);
    MDrv_CQ_Fire();
}
#else

void drv_gpd_set_png_go(void)
{
#ifdef DRVGPD_FLIP_GO
	{
		U32 val = GPD_GET_U32REG(reg_png_go) ^ 1;
		GPD_SET_U32REG(reg_png_go, val);
	}
#else
    GPD_SET_U32REG(reg_png_go, 1);
#endif
}

void drv_gpd_set_blk_go(void)
{
#ifdef DRVGPD_FLIP_GO
	{

		U32 val = GPD_GET_U32REG(reg_png_blk_go) ^ 1;
		GPD_SET_U32REG(reg_png_blk_go, val);
	}
#else
    GPD_SET_U32REG(reg_png_blk_go, 1);
#endif
    /*
    {
        static U32 val = 1;
        U32 dbg = 0xff;

        if(val)
            GPDreg.h8328 |=(GIF_LOCAL_TBL);
        else
            GPDreg.h8328 &=~(GIF_LOCAL_TBL);

        set_single_cmd(0x8328,GPDreg.h8328);

        MDrv_CQ_Fire();

        while(dbg!=val)
            dbg=((gpd_read_debug_port(0)&_BIT28)>>28);

        val=!val;
    }
    */
}

void drv_gpd_set_png_done(unsigned int value)
{
    GPD_SET_U32REG(reg_png_done, value);
}

void drv_gpd_set_cmp_type(unsigned char type)
{
    GPD_SET_U32REG(reg_png_compress_type, type);
}
#endif

#if 0
void drv_gpd_set_lbase_g1(unsigned int data)   //lbase 2~4
{
    MDrv_CQ_SWReset();

    GPDreg.h837c=data;

    set_single_cmd(0x837c,GPDreg.h837c);
}

void drv_gpd_set_lbase_g2(unsigned int data)   //5~7
{
    GPDreg.h8380=data;

    set_single_cmd(0x8380,GPDreg.h8380);
}

void drv_gpd_set_lbase_g3(unsigned int data)  //8~10
{
    GPDreg.h8384=data;

    set_single_cmd(0x8384,GPDreg.h8384);
}

void drv_gpd_set_lbase_g4(unsigned int data) //11~13
{
    GPDreg.h8388=data;

    set_single_cmd(0x8388,GPDreg.h8388);
}

void drv_gpd_set_lbase_g5(unsigned int data) //lbase 14~15
{
    GPDreg.h838c=data;

    set_single_cmd(0x838c,GPDreg.h838c);
}

void drv_gpd_set_dbase_g1(unsigned int data) //dbase 2~3
{
    GPDreg.h838c &= ~(0x3ff<<PNG_DBASE2_SHF);
    GPDreg.h838c |= (data);

    set_single_cmd(0x838c,GPDreg.h838c);
}

void drv_gpd_set_dbase_g2(unsigned int data) //dbase 4~9
{
    GPDreg.h8390=data;

    set_single_cmd(0x8390,GPDreg.h8390);
}

void drv_gpd_set_dbase_g3(unsigned int data) //dbase 10~15
{
    GPDreg.h8394=data;

    set_single_cmd(0x8394,GPDreg.h8394);
}

void drv_gpd_set_lmincode_valid(unsigned short lvalid)
{
    GPDreg.h83b4=lvalid;

    set_single_cmd(0x83b4,GPDreg.h83b4);
}

void drv_gpd_set_dmincode_valid(unsigned short dvalid)
{
    GPDreg.h83b4 &=~(0x7fff<<PNG2_MICODE_VALID_SHF);
    GPDreg.h83b4 |=((dvalid&0x7fff)<<PNG2_MICODE_VALID_SHF);

    set_single_cmd(0x83b4,GPDreg.h83b4);
}

void drv_gpd_set_lmincode_g1(unsigned int data) //1~7
{
    GPDreg.h8354=data;

    set_single_cmd(0x8354,GPDreg.h8354);
}

void drv_gpd_set_lmincode_g2(unsigned int data) //8~10
{
    GPDreg.h835c=data;

    set_single_cmd(0x835c,GPDreg.h835c);
}

void drv_gpd_set_lmincode_g3(unsigned int data) //11~12
{
    GPDreg.h8360=data;

    set_single_cmd(0x8360,GPDreg.h8360);
}

void drv_gpd_set_lmincode_g4(unsigned int data) //13~14
{
    GPDreg.h8364=data;

    set_single_cmd(0x8364,GPDreg.h8364);
}

void drv_gpd_set_lmincode_g5(unsigned int data) //lmin 15
{
    GPDreg.h8368=data;

    set_single_cmd(0x8368,GPDreg.h8368);
}

void drv_gpd_set_dmincode_g1(unsigned int data) //1~5
{
    GPDreg.h8368 &=~(0x7fff<<PNG2_MINCODE1_SHF);
    GPDreg.h8368|=(data);

    set_single_cmd(0x8368,GPDreg.h8368);
}

void drv_gpd_set_dmincode_g2(unsigned int data) //6~9
{
    GPDreg.h836c=data;

    set_single_cmd(0x836c,GPDreg.h836c);
}

void drv_gpd_set_dmincode_g3(unsigned int data) //10~11
{
    GPDreg.h8370=data;

    set_single_cmd(0x8370,GPDreg.h8370);
}

void drv_gpd_set_dmincode_g4(unsigned int data) //12~13
{
    GPDreg.h8374=data;

    set_single_cmd(0x8374,GPDreg.h8374);
}

void drv_gpd_set_dmincode_g5(unsigned int data) //14~15
{
    GPDreg.h8378=data;

    set_single_cmd(0x8378,GPDreg.h8378);
    MDrv_CQ_Fire();
}
#else

void drv_gpd_set_lbase(unsigned short *lbase)
{
    GPD_SET_U32REG(reg_png_lbase2,  lbase[2]);
    GPD_SET_U32REG(reg_png_lbase3,  lbase[3]);
    GPD_SET_U32REG(reg_png_lbase4,  lbase[4]);
    GPD_SET_U32REG(reg_png_lbase5,  lbase[5]);
    GPD_SET_U32REG(reg_png_lbase6,  lbase[6]);
    GPD_SET_U32REG(reg_png_lbase7,  lbase[7]);
    GPD_SET_U32REG(reg_png_lbase8,  lbase[8]);
    GPD_SET_U32REG(reg_png_lbase9,  lbase[9]);
    GPD_SET_U32REG(reg_png_lbase10, lbase[10]);
    GPD_SET_U32REG(reg_png_lbase11, lbase[11]);
    GPD_SET_U32REG(reg_png_lbase12, lbase[12]);
    GPD_SET_U32REG(reg_png_lbase13, lbase[13]);
    GPD_SET_U32REG(reg_png_lbase14, lbase[14]);
    GPD_SET_U32REG(reg_png_lbase15, lbase[15]);
}

void drv_gpd_set_dbase(unsigned short *dbase)
{
    GPD_SET_U32REG(reg_png_dbase2,  dbase[2]);
    GPD_SET_U32REG(reg_png_dbase3,  dbase[3]);
    GPD_SET_U32REG(reg_png_dbase4,  dbase[4]);
    GPD_SET_U32REG(reg_png_dbase5,  dbase[5]);
    GPD_SET_U32REG(reg_png_dbase6,  dbase[6]);
    GPD_SET_U32REG(reg_png_dbase7,  dbase[7]);
    GPD_SET_U32REG(reg_png_dbase8,  dbase[8]);
    GPD_SET_U32REG(reg_png_dbase9,  dbase[9]);
    GPD_SET_U32REG(reg_png_dbase10, dbase[10]);
    GPD_SET_U32REG(reg_png_dbase11, dbase[11]);
    GPD_SET_U32REG(reg_png_dbase12, dbase[12]);
    GPD_SET_U32REG(reg_png_dbase13, dbase[13]);
    GPD_SET_U32REG(reg_png_dbase14, dbase[14]);
    GPD_SET_U32REG(reg_png_dbase15, dbase[15]);
}

void drv_gpd_set_lmincode_valid(unsigned short lmin_valid)
{
    GPD_SET_U32REG(reg_png_mincode_valid, lmin_valid);
}

void drv_gpd_set_dmincode_valid(unsigned short dmin_valid)
{
    GPD_SET_U32REG(reg_png2_mincode_valid, dmin_valid);
}

void drv_gpd_set_lmincode(unsigned short *lmin)
{
    GPD_SET_U32REG(reg_png_mincode1,  lmin[1]);
    GPD_SET_U32REG(reg_png_mincode2,  lmin[2]);
    GPD_SET_U32REG(reg_png_mincode3,  lmin[3]);
    GPD_SET_U32REG(reg_png_mincode4,  lmin[4]);
    GPD_SET_U32REG(reg_png_mincode5,  lmin[5]);
    GPD_SET_U32REG(reg_png_mincode6,  lmin[6]);
    GPD_SET_U32REG(reg_png_mincode7,  lmin[7]);
    GPD_SET_U32REG(reg_png_mincode8,  lmin[8]);
    GPD_SET_U32REG(reg_png_mincode9,  lmin[9]);
    GPD_SET_U32REG(reg_png_mincode10, lmin[10]);
    GPD_SET_U32REG(reg_png_mincode11, lmin[11]);
    GPD_SET_U32REG(reg_png_mincode12, lmin[12]);
    GPD_SET_U32REG(reg_png_mincode13, lmin[13]);
    GPD_SET_U32REG(reg_png_mincode14, lmin[14]);
    GPD_SET_U32REG(reg_png_mincode15, lmin[15]);
}

void drv_gpd_set_dmincode(unsigned short *dmin)
{
    GPD_SET_U32REG(reg_png2_mincode1,  dmin[1]);
    GPD_SET_U32REG(reg_png2_mincode2,  dmin[2]);
    GPD_SET_U32REG(reg_png2_mincode3,  dmin[3]);
    GPD_SET_U32REG(reg_png2_mincode4,  dmin[4]);
    GPD_SET_U32REG(reg_png2_mincode5,  dmin[5]);
    GPD_SET_U32REG(reg_png2_mincode6,  dmin[6]);
    GPD_SET_U32REG(reg_png2_mincode7,  dmin[7]);
    GPD_SET_U32REG(reg_png2_mincode8,  dmin[8]);
    GPD_SET_U32REG(reg_png2_mincode9,  dmin[9]);
    GPD_SET_U32REG(reg_png2_mincode10, dmin[10]);
    GPD_SET_U32REG(reg_png2_mincode11, dmin[11]);
    GPD_SET_U32REG(reg_png2_mincode12, dmin[12]);
    GPD_SET_U32REG(reg_png2_mincode13, dmin[13]);
    GPD_SET_U32REG(reg_png2_mincode14, dmin[14]);
    GPD_SET_U32REG(reg_png2_mincode15, dmin[15]);
}
#endif

#if 0
void drv_gpd_set_fixed_ldata(void)
{
    int i;

    for (i=256;i<280;i+=2)
    {
        GPDreg.h8324=i+((i+1)<<9);
        set_single_cmd(0x8324,GPDreg.h8324);
    }

    for (i=0;i<144;i+=2)
    {
        GPDreg.h8324=i+((i+1)<<9);
        set_single_cmd(0x8324,GPDreg.h8324);
    }

    for (i=280;i<288;i+=2)
    {
        GPDreg.h8324=i+((i+1)<<9);
        set_single_cmd(0x8324,GPDreg.h8324);
    }

    for (i=144;i<256;i+=2)
    {
        GPDreg.h8324=i+((i+1)<<9);
        set_single_cmd(0x8324,GPDreg.h8324);
    }
}

void drv_gpd_set_dynamic_ldata(unsigned short num,unsigned short *ldata)
{
    int i;

    for(i=0;i<num;i+=2)
    {
        GPDreg.h8324=ldata[i]+(ldata[i+1]<<9);
        set_single_cmd(0x8324,GPDreg.h8324);
    }
}

void drv_gpd_set_dynamic_ddata(unsigned short num,unsigned short *ddata)
{
    int i;

    for (i = 0; i < num; i += 2)
    {
        GPDreg.h8318 = ddata[i] + (ddata[i+1] << 5);
        set_single_cmd(0x8318, GPDreg.h8318);
    }
}
#else

void drv_gpd_set_fixed_ldata(void)
{
    unsigned int i;

    for (i = 256; i < 280; i += 2)
        GPD_SET_U32REG(reg_png_ltbl_wd, i+((i+1)<<9));

    for (i = 0; i < 144; i += 2)
        GPD_SET_U32REG(reg_png_ltbl_wd, i+((i+1)<<9));

    for (i = 280; i < 288; i += 2)
        GPD_SET_U32REG(reg_png_ltbl_wd, i+((i+1)<<9));

    for (i = 144; i < 256; i += 2)
        GPD_SET_U32REG(reg_png_ltbl_wd, i+((i+1)<<9));
}

void drv_gpd_set_dynamic_ldata(unsigned short num, unsigned short *ldata)
{
    unsigned short i;

    for(i = 0; i < num; i += 2)
        GPD_SET_U32REG(reg_png_ltbl_wd, ldata[i] + (ldata[i+1] << 9));
}

void drv_gpd_set_dynamic_ddata(unsigned short num, unsigned short *ddata)
{
    unsigned short i;

    for(i = 0; i < num; i += 2)
        GPD_SET_U32REG(reg_png_dtbl_wd, ddata[i] + (ddata[i+1] << 5));
}
#endif

#if 0
int drv_gpd_check_engine(void)
{
    if(REDREG32(REG_GPD_8014)&GPD_TIME_OUT )
        return -1;

    if((!(REDREG32(REG_GPD_8014)&GPD_BLK_DONE)) && (REDREG32(REG_GPD_801c)&OFIFO_DONE))
        return -2;

    return 0;
}
#else

int drv_gpd_check_engine(void)
{
    if (GPD_GET_U32REG(reg_gpd_time_out))
    {
        console_printf("[Error] reg_gpd_time_out\n");
        return -1;
    }

    if(!GPD_GET_U32REG(reg_png_blk_done) && drv_gpd_ofifo_done())
    {
        console_printf("[Error] reg_png_blk_done != 1\n");
        return -2;
    }

    return 0;
}
#endif


void drv_gpd_set_access_region(U32 start, U32 end)
{

#ifdef GPD_PROTECT_ADDR_PATCH
    console_printf("MIU protect: [%lx, %lx]\n", start, end);
    GPD_SET_U32REG(reg_lb_addr, start);
    GPD_SET_U32REG(reg_ub_addr, end);
#elif ENABLE_GPD_PATCH
    console_printf("MIU protect: [%lx, %lx]\n", start, end);
    GPD_SET_U32REG(reg_lb_addr, start);
    GPD_SET_U32REG(reg_ub_addr, end);
#endif

}

unsigned int drv_gpd_get_clkbase()
{
    return hal_gpd_get_clkbase();

}

unsigned int drv_gpd_get_clkoffset()
{
    return hal_gpd_get_clkoffset();

}

void drv_gpd_init_outside_reg(U32 BankBaseAddr)
{   
    gRIU_REG_BASE = BankBaseAddr;
    hal_gpd_init_outside_reg(BankBaseAddr);
}


void drv_gpd_init_chip_specific_reg(void)
{
    hal_gpd_init_chip_specific_reg();
}

unsigned int is_stb()
{
#ifdef STB

    return 1;

#else

    return 0;

#endif

}

U8 MDrv_GPD_MIU_Select(U32 u32Addr)
{
    if(u32Addr >=HAL_MIU1_BASE)
    {
        console_printf("GPD on MIU1!!!!!!!!!!!!\n");
        // MIU1
        return TRUE;
    }
    else
    {
        console_printf("GPD on MIU0!!!!!!!!!!!!\n");
        // MIU0
        return FALSE;
    }
}


void MDrv_GPD_Reg_Base(U32* u32RIUBase, U32* u32XIUBase)
{
    hal_gpd_reg_base(u32RIUBase, u32XIUBase);
}

void MDrv_GPD_MIU_Client(U8* u8Offset, U16* u16BitMask)
{
    hal_gpd_miu_client(u8Offset, u16BitMask);
}

static GPD_IsrFuncCb _pGPDIsrCb = NULL;
static U8 bIsrEnable = FALSE;
static void _GPD_ISRHandler(U32 IsrNum)
{
    printf("_GPD_ISRHandler: IsrNum = %ld\n", IsrNum);
    if(_pGPDIsrCb)
    {
        _pGPDIsrCb();
    }
}

U8 MDrv_GPD_EnableISR(GPD_IsrFuncCb IsrCb)
{
    if(TRUE == bIsrEnable)
    {
        printf("GPD ISR has been enabled!!\n");
        return FALSE;
    }

    if(TRUE == OSAL_GPD_ISR_Attach((OSAL_GPD_IsrFuncCb)_GPD_ISRHandler))
    {
        printf("Attach GPD ISR Success!!\n");
        if(TRUE == OSAL_GPD_ISR_Enable())
        {
            printf("MDrv_GPD_EnableISR : Success!!\n");
            bIsrEnable = TRUE;
            if(IsrCb)
            {
                _pGPDIsrCb = IsrCb;
            }
            return TRUE;
        }
    }
    printf("MDrv_GPD_EnableISR Failed!!\n");
    return FALSE;
}

/********************************************************************/
U8 MDrv_GPD_DisableISR(void)
{
    if(FALSE == bIsrEnable)
    {
        printf("GPD ISR has been disabled!!\n");
        return FALSE;
    }

    if(TRUE == OSAL_GPD_ISR_Disable())
    {
        printf("Disable GPD ISR Success!!\n");
        if(TRUE == OSAL_GPD_ISR_Detach())
        {
            printf("MDrv_GPD_DisableISR : Success!!\n");
            bIsrEnable = FALSE;
            _pGPDIsrCb = NULL;
            return TRUE;
        }
    }
    return FALSE;
}

void MDrv_GPD_SetISRMask(void)
{
    printf("1......\n");
//    GPD_SET_U32REG(reg_int_mask,  0x1e);
    GPD_SET_U32REG(reg_int_mask,  0x1c);

//    GPD_SET_U32REG(reg_int_sw_force,  0x1f);
    printf("2......\n");
}


void MDrv_GPD_PrintGPDBuf(void)
{
#if 0
    printf("Dump Z buffer=============================start\n");
    _GPD_PrintMem(u32ZbufVA, 2*1024);
    printf("Dump Z buffer=============================end\n");

    printf("Dump C buffer=============================start\n");
    _GPD_PrintMem(u32CbufVA, 32*1024);
    printf("Dump C buffer=============================end\n");
#endif
}


void drv_gpd_set_cacheable(U32 u32Cacheable)
{
    g_gpd_cacheable = u32Cacheable;
}

U32 drv_gpd_get_cacheable(void)
{
    return g_gpd_cacheable;
}

U32 drv_gpd_get_RIU_BASE(void)
{
    return gRIU_REG_BASE;
}

