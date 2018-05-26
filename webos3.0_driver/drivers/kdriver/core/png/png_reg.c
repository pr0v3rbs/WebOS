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

    
/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "png_reg.h"
#include "icod_reg.h"

#include <asm/io.h>
#include <linux/slab.h>

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
    global Variables
----------------------------------------------------------------------------------------*/
static volatile PNG_REG_T *gpRealRegPNG = NULL; //real register of PNG
static volatile PNG_REG_T *gpRegPNG = NULL;     //shadow register

static volatile ICOD_REG_T *gpRealRegICOD = NULL;
static volatile ICOD_REG_T *gpRegICOD = NULL;


/*----------------------------------------------------------------------------------------
    Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
    Static Variables
----------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------
    Global Function Definitions
----------------------------------------------------------------------------------------*/
void PNG_Reg_Init(unsigned int png_reg_addr, unsigned int icod_reg_addr)
{
	printk("PNG_Reg_Init\n" );
	gpRealRegPNG = (PNG_REG_T*)ioremap(png_reg_addr, sizeof(PNG_REG_T));
	//*ppRemapRegPtr = (UINT32*)gpRealRegPNG;
	gpRegPNG = (PNG_REG_T*)kmalloc(sizeof(PNG_REG_T), GFP_KERNEL);
	//*ppShadowPtr = (UINT32*)gpRegPNG;
    
	gpRealRegICOD = (ICOD_REG_T*)ioremap(icod_reg_addr, sizeof(ICOD_REG_T));
	//*ppRemapRegPtr = (UINT32*)gpRealRegICOD;
	gpRegICOD = (ICOD_REG_T*)kmalloc(sizeof(ICOD_REG_T), GFP_KERNEL);
	//*ppShadowPtr = (UINT32*)gpRegICOD;
}

void PNG_Reg_Cleanup(void)
{
	printk("PNG_Reg_Cleanup\n" );
    kfree(gpRegICOD);
    kfree(gpRegPNG);
    iounmap(gpRealRegICOD);
    iounmap(gpRealRegPNG);
}


void PNG_Reg_StartDecode(void)
{
	PNG_RdFL(cmd);
	PNG_Wr01(cmd, start, 1);
	PNG_WrFL(cmd);
}

void PNG_Reg_StartDecodePartial(void)
{
	PNG_RdFL(cmd);
	PNG_Wr01(cmd, mid_start, 1);
	PNG_WrFL(cmd);
}

void PNG_Reg_SetDecodeImageSize(unsigned short width, unsigned short height)
{
	PNG_RdFL(image_info1);
	PNG_Wr01(image_info1, image_width, width);
	PNG_Wr01(image_info1, image_height, height);
	PNG_WrFL(image_info1);
}

unsigned int PNG_Reg_GetDecodeImageWidth(void)
{
    unsigned int width;
	PNG_RdFL(image_info1);
	PNG_Rd01(image_info1, image_width, width);
	return width;
}

unsigned int PNG_Reg_GetDecodeImageHeight(void)
{
    unsigned int height;
	PNG_RdFL(image_info1);
	PNG_Rd01(image_info1, image_height, height);
	return height;
}

void PNG_Reg_SetDecodeImageInformation(unsigned char bit_depth, unsigned char color_type, unsigned char output_format)
{
	PNG_RdFL(image_info2);
	PNG_Wr01(image_info2, bitdepth, bit_depth);
	PNG_Wr01(image_info2, color_type, color_type);
	PNG_Wr01(image_info2, out_format, output_format);
	PNG_WrFL(image_info2);
}

unsigned int PNG_Reg_GetDecodeImageBitDepth(void)
{
    unsigned int bit_depth;
	PNG_RdFL(image_info2);
	PNG_Rd01(image_info2, bitdepth, bit_depth);
	return bit_depth;
}

unsigned int PNG_Reg_GetDecodeImageColorType(void)
{
    unsigned int color_type;
	PNG_RdFL(image_info2);
	PNG_Rd01(image_info2, color_type, color_type);
	return color_type;
}

unsigned int PNG_Reg_GetDecodeImageOutputFormat(void)
{
    unsigned int output_format;
	PNG_RdFL(image_info2);
	PNG_Rd01(image_info2, out_format, output_format);
	return output_format;
}

void PNG_Reg_SetReadStartAddress(unsigned int addr)
{
	PNG_Wr(read_base_addr, addr);	
	PNG_WrFL(read_base_addr);
}

unsigned int PNG_Reg_GetReadStartAddress(void)
{
    unsigned int addr;
	PNG_RdFL(read_base_addr);
	addr = PNG_Rd(read_base_addr);
    return addr;
}

void PNG_Reg_SetReadLength(unsigned int size)
{
	PNG_Wr(read_byte_size, size);	
	PNG_WrFL(read_byte_size);
}

void PNG_Reg_SetWriteStartAddress(unsigned int addr)
{
	PNG_Wr(write_base_addr, addr);	
	PNG_WrFL(write_base_addr);
}

unsigned int PNG_Reg_GetWriteStartAddress(void)
{
    unsigned int addr;
	PNG_RdFL(write_base_addr);
	addr = PNG_Rd(write_base_addr);
    return addr;
}

void PNG_Reg_EnableInterrupt(unsigned int flag)
{
	PNG_Wr(intr_enable, flag);
	PNG_WrFL(intr_enable);
}

void PNG_Reg_EnableInterruptAll(void)
{
	PNG_Wr(intr_enable, 0xFFFFFFFF);
	PNG_WrFL(intr_enable);
}

unsigned int PNG_Reg_GetEnabledInterruptAll(void)
{
    unsigned int  flag;
	PNG_RdFL(intr_enable);
	flag = PNG_Rd(intr_enable);
    return flag;
}

void PNG_Reg_DisableInterrupt(unsigned int flag)
{
	PNG_Wr(intr_enable, ~flag);
	PNG_WrFL(intr_enable);
}

void PNG_Reg_DisableInterruptAll(void)
{
	PNG_Wr(intr_enable, 0x0);
	PNG_WrFL(intr_enable);
}

unsigned int PNG_Reg_GetInterruptStatusAll(void)
{
    unsigned int  flag;
	PNG_RdFL(intr_status);
	flag = PNG_Rd(intr_status);
    return flag;
}

void PNG_Reg_ClearInterrupt(unsigned short flag)
{
	PNG_Wr(intr_clear, flag);
	PNG_WrFL(intr_clear);
}

void PNG_Reg_ClearInterruptAll(void)
{
	PNG_Wr(intr_clear, 0xFFFFFFFF);
	PNG_WrFL(intr_clear);
}

unsigned int PNG_Reg_GetCurrentReadAddress(void)
{
    unsigned int addr;
	PNG_RdFL(cur_read_addr);
	addr = PNG_Rd(cur_read_addr);
    return addr;
}

unsigned int PNG_Reg_GetWriteOffset(void)
{
    unsigned int offset;
	PNG_RdFL(cur_ftr_wptr);
	offset = PNG_Rd(cur_ftr_wptr);
    return offset;
}

unsigned int PNG_Reg_GetCurrentWriteAddress(void)
{
    unsigned int addr;
	PNG_RdFL(cur_write_addr);
	addr = PNG_Rd(cur_write_addr);
    return addr;
}

void PNG_Reg_SetWriteAddressBoundary(unsigned int addr)
{
	PNG_Wr(ftr_wptr_bnd, addr);	
	PNG_WrFL(ftr_wptr_bnd);
}

unsigned int PNG_Reg_GetWriteAddressBoundary(void)

{
    unsigned int addr;
	PNG_RdFL(ftr_wptr_bnd);
	addr = PNG_Rd(ftr_wptr_bnd);
    return addr;
}

unsigned int PNG_Reg_GetInFIFOValidBits(void)
{
    unsigned int value;
	PNG_RdFL(fifo_status);
	PNG_Rd01(fifo_status, in_fifo_valid_bits, value);
    return value;
}

unsigned int PNG_Reg_GetFIFOSpace(void)
{
    unsigned int value;
	PNG_RdFL(fifo_status);
	PNG_Rd01(fifo_status, r_fifo_space, value);
    return value;
}

unsigned int PNG_Reg_GetInflateStatus(void)
{
    unsigned int status;
	PNG_RdFL(inflate_status);
	status = PNG_Rd(inflate_status);
    return status;
}

unsigned int PNG_Reg_GetMainState(void)
{
    unsigned int state;
	PNG_RdFL(inflate_status);
	PNG_Rd01(inflate_status, main_state, state);
    return state;
}

unsigned int PNG_Reg_GetTableState(void)
{
    unsigned int state;
	PNG_RdFL(inflate_status);
	PNG_Rd01(inflate_status, table_state, state);
    return state;
}

unsigned int PNG_Reg_GetInflateState(void)
{
    unsigned int state;
	PNG_RdFL(inflate_status);
	PNG_Rd01(inflate_status, inflate_state, state);
    return state;
}

unsigned int PNG_Reg_GetFilterType(void)
{
    unsigned int type;
	PNG_RdFL(ftr_type);
	PNG_Rd01(ftr_type, ftr_type, type);
    return type;
}

unsigned int PNG_Reg_GetFilterStatus(void)
{
    unsigned int status;
	PNG_RdFL(ftr_status);
	status = PNG_Rd(ftr_status);
    return status;
}

unsigned int PNG_Reg_GetFilterDone(void)
{
    unsigned int state;
	PNG_RdFL(ftr_status);
	PNG_Rd01(ftr_status, filter_done, state);
    return state;
}

unsigned int PNG_Reg_GetFilterLastLine(void)
{
    unsigned int state;
	PNG_RdFL(ftr_status);
	PNG_Rd01(ftr_status, filter_last_line, state);
    return state;
}

unsigned int PNG_Reg_GetInflateDone(void)
{
    unsigned int state;
	PNG_RdFL(ftr_status);
	PNG_Rd01(ftr_status, inflate_done, state);
    return state;
}

void PNG_Reg_SetTimeoutInterrupt(unsigned int value)
{
	PNG_Wr(time_out_mask, value);
	PNG_WrFL(time_out_mask);
}

unsigned int PNG_Reg_GetCurrentTimeCount(void)
{
    unsigned int status;
	PNG_RdFL(cur_time_cnt);
	status = PNG_Rd(cur_time_cnt);
    return status;
}



void ICOD_Reg_Reset(void)
{
	/* reset PNG H/W and wait for completion */
	ICOD_RdFL(sw_rst);
	ICOD_Wr01(sw_rst, sw_rst_png, 1);
	ICOD_WrFL(sw_rst);
}


/*----------------------------------------------------------------------------------------
    Static Function Definitions
----------------------------------------------------------------------------------------*/   




