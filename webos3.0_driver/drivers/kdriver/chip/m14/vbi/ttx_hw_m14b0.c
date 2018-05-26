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



/** @file
 *
 *	TTX module control Drivers linux module
 *
 *  author      won.hur (won.hur@lge.com)
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_TTX
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */

#include "vbi_kapi.h"

#include "vbi_module.h"
#include "vbi_data.h"
#include "vbi_hw.h"
#include "vbi_drv.h"

#include "sys_regs.h"
#include "../afe/de_cvd_reg_m14bx.h"
#include "../../../core/afe/cvd_module.h"

#include "os_util.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	VBI_OFF	0
#define	VBI_ON	1
#define WSS_BUF_SIZE	5

#define ALLOW_54MHZ_TTX_MODE 0

#define	VPS_DATA_LINE	16
#define	WSS_DATA_LINE	23
#define	DATA_OFFSET		2

#define VBI_DATA_ODD_START_LINE		6
#define VBI_DATA_ODD_END_LINE		23
#define VBI_DATA_EVEN_START_LINE	318
#define VBI_DATA_EVEN_END_LINE		335
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
extern volatile DE_CVD_REG_M14Bx_T __iomem *gpRegCVD_M14Bx;
extern volatile DE_CVD_REG_M14Bx_T __iomem *gpRealRegCVD_M14Bx;

int g_M14Bx_enable_vps_slicer = 0;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
int gTTXCountOfLine318_M14B0;
int gTTXCountOfOtherLine_M14B0;



/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static volatile unsigned long gTTX_addr;
static int ttx_54Mhz_flag = 0;
static unsigned char *ucPacket;
static int gEnableTTXDataPrint = 0;
static UINT8 gWSSIncrementer = 0;


static unsigned int gPrevVline625 = 0xffffffff;
/*========================================================================================
	Implementation Group
========================================================================================*/

int TTX_HWInitialize_M14Bx(void)
{
	int ret = RET_ERROR;

	TTX_PRINT("VBI TTX Initialize for M14Bx entered!\n");

	gTTXCountOfLine318_M14B0 = 0;

	TTX_PRINT("[%s:%d] Attempting IOREMAPPING address of [0x%08x] with size of [0x%08x]\n", __F__, __L__, gstRegCfgVBI->vbi_register_base, gstRegCfgVBI->vbi_register_size);


	// TODO:dws
	gTTX_addr = (volatile unsigned long)ioremap(	gstRegCfgVBI->vbi_register_base , 	gstRegCfgVBI->vbi_register_size);

	if((void *)gTTX_addr == NULL){
		TTX_PRINT("[%s:%d] Critical Error! Failed IOREMAPPING VBI HW memory IO.\n",__F__, __L__); 
		return -1;
	}
	TTX_PRINT("[%s:%d] Success IOREMAPPING address of [0x%08x] with size of [0x%08x] to [0x%08x]\n", __F__, __L__, gstRegCfgVBI->vbi_register_base, gstRegCfgVBI->vbi_register_size, (unsigned int)gTTX_addr);


	ucPacket = (unsigned char *)kmalloc( (TTX_MAX_DATA + 7) * MAX_PACKET_NUM , GFP_KERNEL);

	// Need to modify kdriver to use little endian.
	CVD_M14Bx_RdFL(vbi_ctrl_001);
	CVD_M14Bx_Wr01(vbi_ctrl_001, reg_vbi_little_endian, 0);
	CVD_M14Bx_WrFL(vbi_ctrl_001);

	/* 2013.06.27 won.hur : Added code for reg_swrst_vbi set to 0 
	 * There will be no operation in vbi module if this value is not set to 0.
	 * Please be aware of this CTOP register */
	CTOP_CTRL_M14B0_RdFL(DEI, ctr03);
	CTOP_CTRL_M14B0_Wr01(DEI, ctr03, swrst_cvd_vbi, 0x0);
	CTOP_CTRL_M14B0_WrFL(DEI, ctr03);
	ret = RET_OK;

	TTX_PRINT("VBI TTX Initialize for M14Bx is succefully done!\n");
	return ret;
}

int TTX_HWUninitialize_M14Bx(void)
{
	int ret = 0;

	iounmap((void *)gTTX_addr);
	kfree((void*)ucPacket);

	return ret;
}

int TTX_SetInputFormat_M14Bx(eVBIDataType type)
{
	int ret = 0;

	TTX_PRINT("%s input : %d\n" , __func__ , type);

	switch(type)
	{
		case eDisable:
			CVD_M14Bx_RdFL(vbi_004);
			CVD_M14Bx_Wr01(vbi_004, reg_vbi_en, VBI_OFF);
			CVD_M14Bx_WrFL(vbi_004);
			break;

		case eTTX625B:
			CVD_M14Bx_RdFL(vbi_007);
			CVD_M14Bx_RdFL(vbi_008);
			CVD_M14Bx_RdFL(vbi_009);
			CVD_M14Bx_RdFL(vbi_010);
			CVD_M14Bx_RdFL(vbi_011);
			CVD_M14Bx_RdFL(vbi_006);
			CVD_M14Bx_RdFL(vbi_020);
			CVD_M14Bx_RdFL(vbi_014);
			CVD_M14Bx_RdFL(vbi_015);
			CVD_M14Bx_RdFL(vbi_016);
			CVD_M14Bx_RdFL(iris_040);
			CVD_M14Bx_RdFL(vbi_004);
			CVD_M14Bx_RdFL(vbi_013);
			CVD_M14Bx_RdFL(vbi_012);

			/* Added for M14Bx on improving performance & bugs... */
			/* 2012.10.22 won.hur */
			CVD_M14Bx_RdFL(vbi_021);
			CVD_M14Bx_RdFL(vbi_023);
			CVD_M14Bx_RdFL(vbi_024);
			CVD_M14Bx_RdFL(vbi_025);
			CVD_M14Bx_RdFL(vbi_026);
			CVD_M14Bx_RdFL(vbi_027);
			CVD_M14Bx_RdFL(vbi_028);
			CVD_M14Bx_RdFL(vbi_029);
			CVD_M14Bx_RdFL(vbi_030);
			CVD_M14Bx_RdFL(vbi_031);
			CVD_M14Bx_RdFL(vbi_032);
			CVD_M14Bx_RdFL(vbi_033);
			CVD_M14Bx_RdFL(vbi_034);
			CVD_M14Bx_RdFL(vbi_035);
			CVD_M14Bx_RdFL(vbi_036);
			CVD_M14Bx_RdFL(vbi_037);
			CVD_M14Bx_RdFL(vbi_038);

			CVD_M14Bx_Wr(vbi_007, 0x66666666);
			CVD_M14Bx_Wr(vbi_008, 0x66666666);
			// 110917 by dws : Enable 16, 329 line TTX Slicing
			CVD_M14Bx_Wr(vbi_009, 0x66666666);
			//CVD_M14Bx_Wr(vbi_009, 0x66663366);
			CVD_M14Bx_Wr(vbi_010, 0x66666666);
			// 110917 by dws : Disable 336 line WSS Slicing
			CVD_M14Bx_Wr(vbi_011, 0x660C0000);
			//CVD_M14Bx_Wr(vbi_011, 0x66cc0000);
			// vbi_data_hlvl & start_code
			CVD_M14Bx_Wr(vbi_006, 0x2736);
			// reg_vps_start & reg_teletext_start & reg_wss625_start & vbi_020
			CVD_M14Bx_Wr(vbi_020, 0x54845a5a);
			CVD_M14Bx_Wr(vbi_014, 0x35A00F91);

			//CVD_M14Bx_Wr(vbi_015, 0x2066159a);
			// 110712 : WSS malfunction on green/red pattern
			//CVD_M14Bx_Wr01(vbi_015, reg_wss625_dto, 0x205A);		//13.48MHz
			CVD_M14Bx_Wr01(vbi_015, reg_wss625_dto, 0x2066);		//13.5MHz
			CVD_M14Bx_Wr01(vbi_015, reg_vps_dto, 0x159a);

			CVD_M14Bx_Wr01(vbi_016, reg_wss625_frame_start, 0x2);
			CVD_M14Bx_Wr01(iris_040, reg_vsync_vbi_max, 0x25);

			// 110712 : WSS malfunction on green/red pattern
			CVD_M14Bx_Wr01(vbi_013, reg_vbi_wss_slicer_mode, 0x1);

			CVD_M14Bx_Wr01(vbi_004, reg_vbi_muxout, 0);
			CVD_M14Bx_Wr01(vbi_004, reg_vbi_hsyncout, 0);
			CVD_M14Bx_Wr01(vbi_004, reg_vbi_en, VBI_ON);

			CVD_M14Bx_Wr01(vbi_004, reg_vbi_st_err_ignored, 1);

			CVD_M14Bx_WrFL(vbi_007);
			CVD_M14Bx_WrFL(vbi_008);
			CVD_M14Bx_WrFL(vbi_009);
			CVD_M14Bx_WrFL(vbi_010);
			CVD_M14Bx_WrFL(vbi_011);
			CVD_M14Bx_WrFL(vbi_006);
			CVD_M14Bx_WrFL(vbi_020);
			CVD_M14Bx_WrFL(vbi_014);
			CVD_M14Bx_WrFL(vbi_015);
			CVD_M14Bx_WrFL(vbi_016);
			CVD_M14Bx_WrFL(iris_040);
			CVD_M14Bx_WrFL(vbi_004);
			CVD_M14Bx_WrFL(vbi_013);

			/* Added for M14Bx on improving performance & bugs... */
			/* 2012.10.22 won.hur */
			CVD_M14Bx_Wr(vbi_021, 0x00011044);
			CVD_M14Bx_Wr(vbi_023, 0x5F0A0110);
			CVD_M14Bx_Wr(vbi_024, 0x32E0222D);
			CVD_M14Bx_Wr(vbi_025, 0x00000000);
			CVD_M14Bx_Wr(vbi_026, 0x00000000);
			CVD_M14Bx_Wr(vbi_027, 0x400006A1);
			CVD_M14Bx_Wr(vbi_028, 0x40000501); // 20131210 won.hur : Changed TTX AGC gain from 6A->50 due to pyeongtek issue
			CVD_M14Bx_Wr(vbi_029, 0x00000000);
			CVD_M14Bx_Wr(vbi_030, 0x00000000);
			CVD_M14Bx_Wr(vbi_031, 0x00000000);
			CVD_M14Bx_Wr(vbi_032, 0x00000000);
			CVD_M14Bx_Wr(vbi_033, 0x00000000);
			CVD_M14Bx_Wr(vbi_034, 0x00000000);
			CVD_M14Bx_Wr(vbi_035, 0x11523001);
			CVD_M14Bx_Wr(vbi_036, 0x01181113);
			CVD_M14Bx_Wr(vbi_037, 0x00000000);
			CVD_M14Bx_Wr(vbi_038, 0x00000111);

			CVD_M14Bx_WrFL(vbi_021);
			CVD_M14Bx_WrFL(vbi_023);
			CVD_M14Bx_WrFL(vbi_024);
			CVD_M14Bx_WrFL(vbi_025);
			CVD_M14Bx_WrFL(vbi_026);
			CVD_M14Bx_WrFL(vbi_027);
			CVD_M14Bx_WrFL(vbi_028);
			CVD_M14Bx_WrFL(vbi_029);
			CVD_M14Bx_WrFL(vbi_030);
			CVD_M14Bx_WrFL(vbi_031);
			CVD_M14Bx_WrFL(vbi_032);
			CVD_M14Bx_WrFL(vbi_033);
			CVD_M14Bx_WrFL(vbi_034);
			CVD_M14Bx_WrFL(vbi_035);
			CVD_M14Bx_WrFL(vbi_036);
			CVD_M14Bx_WrFL(vbi_037);
			CVD_M14Bx_WrFL(vbi_038);
			// 110121 won.hur : for 318 line TTX data
			CVD_M14Bx_Wr(vbi_012, 0x00006600);
			CVD_M14Bx_WrFL(vbi_012);

			break;

		default:
			break;
	}

	TTX_PRINT("%s done\n", __func__);

	return ret;

}


int TTX_SetInterruptClear_M14Bx(void)
{
	int ret = 0;
	UINT32 intr_vp1_en;

	CVD_M14Bx_RdFL(cvd_intr1_clear0);
	intr_vp1_en = CVD_M14Bx_Rd(cvd_intr1_clear0);
	CVD_M14Bx_Wr(cvd_intr1_clear0, (intr_vp1_en |= (1<<15)));
	CVD_M14Bx_WrFL(cvd_intr1_clear0);

	CVD_M14Bx_RdFL(cvd_intr1_clear0);
	intr_vp1_en = CVD_M14Bx_Rd(cvd_intr1_clear0);
	CVD_M14Bx_Wr(cvd_intr1_clear0, (intr_vp1_en &= ~(1<<15)));
	CVD_M14Bx_WrFL(cvd_intr1_clear0);

	return ret;
}


int TTX_SetInterrupt_M14Bx(int on_off)
{
	int ret = 0;
	UINT32 intr_vp1_en;


	if(on_off)
	{
		/* 2013.06.27 won.hur : Added code for reg_swrst_vbi set to 0 
		 * There will be no operation in vbi module if this value is not set to 0.
		 * Please be aware of this CTOP register */
		CTOP_CTRL_M14B0_RdFL(DEI, ctr03);
		CTOP_CTRL_M14B0_Wr01(DEI, ctr03, swrst_cvd_vbi, 0x0);
		CTOP_CTRL_M14B0_WrFL(DEI, ctr03);

		CVD_M14Bx_RdFL(cvd_intr1_enable1);
		intr_vp1_en = CVD_M14Bx_Rd(cvd_intr1_enable1);
		CVD_M14Bx_Wr(cvd_intr1_enable1, (intr_vp1_en |= M14B0_VBI_INTERRUPT_MASK));
		CVD_M14Bx_WrFL(cvd_intr1_enable1);
	}
	else
	{
		CVD_M14Bx_RdFL(cvd_intr1_enable1);
		intr_vp1_en = CVD_M14Bx_Rd(cvd_intr1_enable1);
		CVD_M14Bx_Wr(cvd_intr1_enable1, (intr_vp1_en &= ~M14B0_VBI_INTERRUPT_MASK));
		CVD_M14Bx_WrFL(cvd_intr1_enable1);

		TTX_SetInterruptClear_M14Bx();
	}

	return ret;
}


int TTX_ParsingVBIData_M14Bx(sHeaderData *data )
{
	int ret = 0;
	unsigned long middle_addr;
	int iHeaderPos;
	int y,i;
	sCGMS cgms_data;

	int buffer_number = 0;

	CVD_M14Bx_RdFL(vbi_ctrl_002);
	CVD_M14Bx_Rd01(vbi_ctrl_002, reg_vbi_buffer_number, buffer_number);

	switch(buffer_number)
	{
		case 0:
			CVD_M14Bx_RdFL(vbi_ctrl_002);
			CVD_M14Bx_RdFL(vbi_ctrl_003);

			CVD_M14Bx_Rd01(vbi_ctrl_002, reg_vbi0_start_addr, data->start);
			CVD_M14Bx_Rd01(vbi_ctrl_003, reg_vbi0_end_addr, data->end);
			CVD_M14Bx_Rd01(vbi_ctrl_002, reg_vbi0_data_cnt, data->count);
			break;
		case 1:
			CVD_M14Bx_RdFL(vbi_ctrl_004);
			CVD_M14Bx_RdFL(vbi_ctrl_005);

			CVD_M14Bx_Rd01(vbi_ctrl_004, reg_vbi1_start_addr, data->start);
			CVD_M14Bx_Rd01(vbi_ctrl_005, reg_vbi1_end_addr, data->end);
			CVD_M14Bx_Rd01(vbi_ctrl_004, reg_vbi1_data_cnt, data->count);
			break;
		case 2:
			CVD_M14Bx_RdFL(vbi_ctrl_006);
			CVD_M14Bx_RdFL(vbi_ctrl_007);

			CVD_M14Bx_Rd01(vbi_ctrl_006, reg_vbi2_start_addr, data->start);
			CVD_M14Bx_Rd01(vbi_ctrl_007, reg_vbi2_end_addr, data->end);
			CVD_M14Bx_Rd01(vbi_ctrl_006, reg_vbi2_data_cnt, data->count);
			break;
		case 3:
			CVD_M14Bx_RdFL(vbi_ctrl_008);
			CVD_M14Bx_RdFL(vbi_ctrl_009);

			CVD_M14Bx_Rd01(vbi_ctrl_008, reg_vbi3_start_addr, data->start);
			CVD_M14Bx_Rd01(vbi_ctrl_009, reg_vbi3_end_addr, data->end);
			CVD_M14Bx_Rd01(vbi_ctrl_008, reg_vbi3_data_cnt, data->count);
		break;
	}

	if(data->count > 0x800)
	{
		if(gEnableTTXDataPrint>0) TTX_PRINT("VBI TTX : Sliced data has exceeded COUNT of 0x800. Value : [0x%x]\n", data->count);
		ret = -1;
	}
	else
	{
		unsigned int *uiData;
		data->data_num = 0;

		if( data->end > data->start )
		{
			for(i = 0; i < (data->end - data->start + 1) ; i++)
			{
				uiData = (unsigned int *)(gTTX_addr + (data->start + i)*4);

				*(ucPacket + i*4 + 0) = (unsigned char)( (*uiData>>24)&0xff );
				*(ucPacket + i*4 + 1) = (unsigned char)( (*uiData>>16)&0xff );
				*(ucPacket + i*4 + 2) = (unsigned char)( (*uiData>>8)&0xff );
				*(ucPacket + i*4 + 3) = (unsigned char)( (*uiData)&0xff );
			}

		}
		else
		{
			middle_addr = 0x200 - data->start;

			for(i = 0; i < (middle_addr) ; i++)
			{
				uiData = (unsigned int *)(gTTX_addr + (data->start + i)*4);

				*(ucPacket + i*4 + 0) = (unsigned char)( (*uiData>>24)&0xff );
				*(ucPacket + i*4 + 1) = (unsigned char)( (*uiData>>16)&0xff );
				*(ucPacket + i*4 + 2) = (unsigned char)( (*uiData>>8)&0xff );
				*(ucPacket + i*4 + 3) = (unsigned char)( (*uiData)&0xff );
			}

			for(i = 0; i < (data->end + 1) ; i++)
			{
				uiData = (unsigned int *)(gTTX_addr + i*4);

				*(ucPacket + (i + middle_addr)*4 + 0) = (unsigned char)( (*uiData>>24)&0xff );
				*(ucPacket + (i + middle_addr)*4 + 1) = (unsigned char)( (*uiData>>16)&0xff );
				*(ucPacket + (i + middle_addr)*4 + 2) = (unsigned char)( (*uiData>>8)&0xff );
				*(ucPacket + (i + middle_addr)*4 + 3) = (unsigned char)( (*uiData)&0xff );
			}
		}

		iHeaderPos = 0;

		for(i=0;i<MAX_PACKET_NUM;i++)
		{
			/* Get only valid data where it has the preamble of "00 ff ff" */
			if( ( *(ucPacket + iHeaderPos + 0) == 0x00) && ( *(ucPacket + iHeaderPos + 1) == 0xff) && (*(ucPacket+iHeaderPos+2) == 0xff))
			{

				/* Debug mode */
				if(gEnableTTXDataPrint>0)
				{
			    	printk("VBI DATA : %2x %2x %2x %2x ", *(ucPacket + iHeaderPos),*(ucPacket + iHeaderPos +1),*(ucPacket + iHeaderPos +2), *(ucPacket + iHeaderPos+3));
			    	printk("%2x %2x %2x ", *(ucPacket + iHeaderPos+4),*(ucPacket + iHeaderPos +5),*(ucPacket + iHeaderPos + 6) );
					for(y=0; y< *(ucPacket + iHeaderPos + 4); y++)
					{
						printk("%2x ", (*(ucPacket + iHeaderPos + 7 + y)));
					}
					if( *(ucPacket + iHeaderPos + 4) == 0x2a) printk(" TTX 625B\n");
					else printk(" NO TTX\n");
				}


				/* For software workaround purpose on L9Bx. From then this is used for performance upgrade feature */
				if( ( *(ucPacket + iHeaderPos + 6) == 0x01) && ( *(ucPacket + iHeaderPos + 5) == 0x3e)) gTTXCountOfLine318_M14B0++;
				else gTTXCountOfOtherLine_M14B0++;

				/* For Copy protection feature, CGMS & WSS detection */
				/* When iHeaderPos+3 is 0x08 or 0x09, this means that the VBI data is WSS */
				if( ( *(ucPacket + iHeaderPos + 3) == 0x08) || ( *(ucPacket + iHeaderPos + 3) == 0x09))
				{
					memcpy((void *)&cgms_data, (void *)(ucPacket + iHeaderPos), 9);
					VBI_PutCGMSData(&cgms_data);
				}
				/* When iHeaderPos+3 is 0x0A or 0x0B, this means that the VBI data is CGMS */
				else if( ( *(ucPacket + iHeaderPos + 3) == 0x0A) || ( *(ucPacket + iHeaderPos + 3) == 0x0B))
				{
					memcpy((void *)&cgms_data, (void *)(ucPacket + iHeaderPos), 10);
					VBI_PutCGMSData(&cgms_data);
				}

				ttx_54Mhz_flag = 0;
				data->data_start[data->data_num] = iHeaderPos + 7;
				data->data_end[data->data_num] = iHeaderPos + 7 + *(ucPacket + iHeaderPos + 4);
				data->data_num++;
				iHeaderPos = iHeaderPos + 7 + *(ucPacket + iHeaderPos + 4);

			}
			/* This is to detect whether the CVD is working on 54Mhz basis. In case of 54Mhz basis, the slicied preamble is "00 00 ff ff". */
			else if( ( *(ucPacket + iHeaderPos + 0) == 0x00) && ( *(ucPacket + iHeaderPos + 1) == 0x00) && ( *(ucPacket + iHeaderPos + 2) == 0xff) && (*(ucPacket+iHeaderPos+3) == 0xff))
			{
				if( ( *(ucPacket + iHeaderPos + 6) == 0x01) && ( *(ucPacket + iHeaderPos + 5) == 0x3e)) gTTXCountOfLine318_M14B0++;
				else gTTXCountOfOtherLine_M14B0++;
				ttx_54Mhz_flag = 1;
				data->data_start[data->data_num] = iHeaderPos + 8;
				data->data_end[data->data_num] = iHeaderPos + 7 + *(ucPacket + iHeaderPos + 5);
				data->data_num++;
				iHeaderPos = iHeaderPos + 7 + *(ucPacket + iHeaderPos + 5);

			}
			else
			{
				iHeaderPos = iHeaderPos + 7 + *(ucPacket + iHeaderPos + 4);
			}
			/* Loop escape when there is no more data to look for... . */
			if( data->count <= iHeaderPos + 1) break;

		}

	}
	return ret;
}


int TTX_PutVBIData_M14Bx(sHeaderData *data)
{
	int ret = 0;
	int i;
	int offset = 3;
	unsigned long ptr = 0;
	unsigned int data_size = 0;
	unsigned int temp_type = 0;
	unsigned int temp_line = 0;

	static sWSSBufInform	wss_buf[WSS_BUF_SIZE] = {{0,}, };
	UINT32	curr_wss_int_time;
	int wss_num;

	if (ttx_54Mhz_flag)
		offset++;

	for(i = 0 ; i < data->data_num ; i++)
	{

		// 111026 won.hur : Since VBIL26 register is initially writen to be 0x60(Related to a DQMS issue, where ttx data on line 318 couldn't be read),
		// 					We needed to add some filters as to block garbage data going up to the circular buffer on the upper stage.
		// 					So before sending the data to the circular buffer, data are filtered whether they are on the VBI range.
		temp_line = VBI_TTX_GetVBIDataLineCount((void *)(ucPacket + data->data_start[i] - DATA_OFFSET));
		if(((temp_line >= VBI_DATA_ODD_START_LINE)&&(temp_line <= VBI_DATA_ODD_END_LINE))||((temp_line >= VBI_DATA_EVEN_START_LINE)&&(temp_line <= VBI_DATA_EVEN_END_LINE)))
		{
			// by wonsik.do WSS and VPS are contained only in odd filed.
			data->type = *(ucPacket+offset) & 0x0F;

			VBI_TTX_GetDataAddr(data); // This would increase the circular buffer pointer!!
			data_size = data->data_end[i] - data->data_start[i];
			temp_type = data->type;

			switch(temp_type)
			{
				case 0x6: // teletext
				case 0x7:
					ptr = 0;
				break;

				//case 0x8:	// WSS
				case 0x9:	// WSS
					ptr = 0;

					curr_wss_int_time = jiffies_to_msecs(jiffies);	// get current time

					if(*(ucPacket +  data->data_start[i]-DATA_OFFSET) != WSS_DATA_LINE)
					{
						continue;
					}

					for(wss_num=1;wss_num<WSS_BUF_SIZE;wss_num++)
					{
						wss_buf[WSS_BUF_SIZE - wss_num].wss_data 		= wss_buf[WSS_BUF_SIZE -wss_num -1].wss_data;
						wss_buf[WSS_BUF_SIZE - wss_num].wss_int_time 	= wss_buf[WSS_BUF_SIZE -wss_num -1].wss_int_time;
					}

					wss_buf[0].wss_data =  ((*(ucPacket + data->data_start[i]+1)& 0xFF)<<8) | *(ucPacket + data->data_start[i]);
					wss_buf[0].wss_int_time = curr_wss_int_time;

					//DQMS 1109-04120 : Sometimes WSS data is missing on ATV 5CH.
					if( (wss_buf[WSS_BUF_SIZE - 1].wss_data	!= wss_buf[WSS_BUF_SIZE - 2].wss_data) || (wss_buf[WSS_BUF_SIZE - 2].wss_data	!= wss_buf[WSS_BUF_SIZE - 3].wss_data) || (wss_buf[WSS_BUF_SIZE - 3].wss_data	!= wss_buf[WSS_BUF_SIZE - 4].wss_data) || (wss_buf[WSS_BUF_SIZE - 4].wss_data	!= wss_buf[0].wss_data)  || ( (wss_buf[0].wss_int_time - wss_buf[WSS_BUF_SIZE -1].wss_int_time) > 600 ))
					{
						//TTX_PRINT("garbage wss data !!!\n");
						//do not copy wss data
						data->addr = 0;
						break;
					}

					if(data_size > 0xd) data_size = 0xd;

					/* Increment WSS counter */
					gWSSIncrementer++;

				break;
				case 0x0a:	// CGMS
				case 0x0b:
					ptr = 6;
				break;

				//case 0xc:	// VPS
				case 0xd:	// VPS
					ptr = 2;
					//TTX_PRINT("VPS: byte[0x%x] data9[0x%x] data10[0x%x] data11[0x%x] data12[0x%x] data13[0x%x] line[%d] status[0x%x] \n" , data->data_end[i]-data->data_start[i] , *(ucPacket + data->data_start[i]+8), *(ucPacket + data->data_start[i]+9), *(ucPacket + data->data_start[i]+10), *(ucPacket + data->data_start[i]+11), *(ucPacket + data->data_start[i]+12), *(ucPacket + data->data_start[i]-2), *(ucPacket + data->data_start[i]-4));
				//TTX_PRINT("VPS: [%x] [%x]  [%d] [%d]\n" , data->addr ,  data->data_end[i]-data->data_start[i] , *(ucPacket + data->data_start[i]) , *(ucPacket + data->data_start[i]+1));

					if(*(ucPacket +  data->data_start[i]-DATA_OFFSET) != VPS_DATA_LINE)
					{
						continue;
					}
					if(data_size > 0xd) data_size = 0xd;

				break;
				default:
					ptr = 0;
				break;

			}

			if(data->addr != 0)
			{
				if((temp_type == 0x06) || (temp_type == 0x07)) {
					memcpy( (void *)(data->addr + ptr) , (void *)(ucPacket + data->data_start[i] - 6 ) , data_size + 6);
				}
				else if(temp_type == 0x09) {
					memcpy( (void *)(data->addr + ptr) , (void *)(ucPacket + data->data_start[i]) , data_size);
					memcpy( (void *)(data->addr + ptr+4) , (void *)(&gWSSIncrementer) , sizeof(UINT8));
				}
				else {
					memcpy( (void *)(data->addr + ptr) , (void *)(ucPacket + data->data_start[i]) , data_size);
				}
			}

		}
		else if(gEnableTTXDataPrint>0) TTX_PRINT("VBI DATA NOT IN VBI RANGE!, LINE IS : [%d]\n", temp_line);

		offset = data->data_end[i] + 3;

		if (ttx_54Mhz_flag)
			offset++;
		//TTX_PRINT("offset [%d] end [%d]\n" , offset , data->data_end[i]);

	}
	return ret;

}


int TTX_Enable_VPS_Slicer_M14Bx(BOOLEAN Enable)
{
	CVD_M14Bx_RdFL(vbi_009);

	if(Enable){
		CVD_M14Bx_Wr01(vbi_009, reg_vbil16, 0x63);	// Enable VPS Slicing on Line #16
		g_M14Bx_enable_vps_slicer = 1;
	}
	else{
 		VBI_TTX_ClearVPSMemory();
		CVD_M14Bx_Wr01(vbi_009, reg_vbil16, 0x66);	// Enable TTX Slicing on Line #16
		g_M14Bx_enable_vps_slicer = 0;
	}
	CVD_M14Bx_WrFL(vbi_009);

	return 0;
}


int TTX_SetVBILPF_M14Bx(int onoff)
{
	int ret = 0;

	CVD_M14Bx_RdFL(vbi_005);

	if(onoff)
	{
		CVD_M14Bx_Wr(vbi_005, 0x00000001);
	}
	else
	{
		CVD_M14Bx_Wr(vbi_005, 0x00000000);
	}

	CVD_M14Bx_WrFL(vbi_005);

	return ret;
}

int TTX_CheckOnLine318_M14Bx(int mode)
{
	int ret = 0;

	if(mode)
	{
		//TTX_PRINT("Reset checking on line 318.\n");
		gTTXCountOfLine318_M14B0 = 0;
		gTTXCountOfOtherLine_M14B0 = 0;
		ret = 1;
	}
	else
	{
		//TTX_PRINT("Count of line 318 vs Other is [%d]/[%d]\n", gTTXCountOfLine318, gTTXCountOfOtherLine);
		if(gTTXCountOfLine318_M14B0 > 10)
		{
			if(gTTXCountOfOtherLine_M14B0 > 10) ret = 4;   // Case when data is also in line 318.
		    else ret = 3; 							 // Case when data is only in line 318.
		}
		else ret = 2;	// Case when data on line 318 does not exist...
	}
	return ret;
}


int TTX_DebugPrintMode_M14Bx(int mode)
{
	int ret = 0;

	if(mode) gEnableTTXDataPrint = 1;
	else gEnableTTXDataPrint = 0;

	return ret;
}


int TTX_SetTypeLine318_M14Bx(int onoff)
{

	int ret = 0;

	CVD_M14Bx_RdFL(vbi_012);

	if(onoff)
	{
		//TTX_PRINT("Detection for line 318 is ON.\n");
		CVD_M14Bx_Wr(vbi_012, 0x00000060);
	}
	else
	{
		//TTX_PRINT("Detection for line 318 is off.\n");
		CVD_M14Bx_Wr(vbi_012, 0x00000000);
	}

	CVD_M14Bx_WrFL(vbi_012);

	return ret;
}

int TTX_Set_TT_Slicer_Mode_M14Bx(unsigned int mode)
{

	int ret = 0;

	TTX_PRINT("%s enterd [%d]\n", __func__, mode);

	CVD_M14Bx_RdFL(vbi_013);

	if(mode < 4)
	{
		CVD_M14Bx_Wr01(vbi_013, reg_vbi_tt_slicer_mode, mode);
		CVD_M14Bx_WrFL(vbi_013);
		ret = 0;
	}
	else
	{
		ret = -1;
	}

	return ret;
}

int TTX_CheckVline625_M14Bx(void)
{
	int ret = RET_OK;
	unsigned int isVline625;

	CVD_M14Bx_RdFL(iris_013);
	CVD_M14Bx_Rd01(iris_013, reg_vline_625, isVline625);

	if(g_M14Bx_enable_vps_slicer == 1) {
			CVD_M14Bx_RdFL(vbi_007);
			CVD_M14Bx_RdFL(vbi_008);
			CVD_M14Bx_RdFL(vbi_009);
			CVD_M14Bx_RdFL(vbi_010);
			CVD_M14Bx_RdFL(vbi_011);
			CVD_M14Bx_RdFL(vbi_015);

			CVD_M14Bx_Wr(vbi_007, 0x66666666);
			CVD_M14Bx_Wr(vbi_008, 0x66666666);
			CVD_M14Bx_Wr(vbi_009, 0x66666666);
			CVD_M14Bx_Wr(vbi_010, 0x66666666);
			CVD_M14Bx_Wr(vbi_011, 0x660C0000);

			CVD_M14Bx_Wr01(vbi_009, reg_vbil16, 0x63);
			CVD_M14Bx_Wr01(vbi_015, reg_wss625_dto, 0x2066);		//13.5MHz

			CVD_M14Bx_WrFL(vbi_007);
			CVD_M14Bx_WrFL(vbi_008);
			CVD_M14Bx_WrFL(vbi_009);
			CVD_M14Bx_WrFL(vbi_010);
			CVD_M14Bx_WrFL(vbi_011);
			CVD_M14Bx_WrFL(vbi_015);
			return ret;
	}

	if(isVline625 == 0)
	{
		if(isVline625 != gPrevVline625)
		{
			gPrevVline625 = isVline625;

			CVD_M14Bx_RdFL(vbi_007);
			CVD_M14Bx_RdFL(vbi_008);
			CVD_M14Bx_RdFL(vbi_009);
			CVD_M14Bx_RdFL(vbi_010);
			CVD_M14Bx_RdFL(vbi_011);
			CVD_M14Bx_RdFL(vbi_015);

			CVD_M14Bx_Wr(vbi_007, 0x00000000);
			CVD_M14Bx_Wr(vbi_008, 0x00000000);
			CVD_M14Bx_Wr(vbi_009, 0x00000000);
			CVD_M14Bx_Wr(vbi_010, 0x1100DD11);	// Becareful of RTL bug
			CVD_M14Bx_Wr(vbi_011, 0x00000000);
			CVD_M14Bx_Wr01(vbi_015, reg_wss625_dto, 0x1E2B);		//13.5MHz

			CVD_M14Bx_WrFL(vbi_007);
			CVD_M14Bx_WrFL(vbi_008);
			CVD_M14Bx_WrFL(vbi_009);
			CVD_M14Bx_WrFL(vbi_010);
			CVD_M14Bx_WrFL(vbi_011);
			CVD_M14Bx_WrFL(vbi_015);
		}
	}
	else
	{
		if(isVline625 != gPrevVline625)
		{
			gPrevVline625 = isVline625;

			CVD_M14Bx_RdFL(vbi_007);
			CVD_M14Bx_RdFL(vbi_008);
			CVD_M14Bx_RdFL(vbi_009);
			CVD_M14Bx_RdFL(vbi_010);
			CVD_M14Bx_RdFL(vbi_011);
			CVD_M14Bx_RdFL(vbi_015);

			CVD_M14Bx_Wr(vbi_007, 0x66666666);
			CVD_M14Bx_Wr(vbi_008, 0x66666666);
			CVD_M14Bx_Wr(vbi_009, 0x66666666);
			CVD_M14Bx_Wr(vbi_010, 0x66666666);
			CVD_M14Bx_Wr(vbi_011, 0x660C0000);
			CVD_M14Bx_Wr01(vbi_015, reg_wss625_dto, 0x2066);		//13.5MHz

			CVD_M14Bx_WrFL(vbi_007);
			CVD_M14Bx_WrFL(vbi_008);
			CVD_M14Bx_WrFL(vbi_009);
			CVD_M14Bx_WrFL(vbi_010);
			CVD_M14Bx_WrFL(vbi_011);
			CVD_M14Bx_WrFL(vbi_015);
		}
	}

	return ret;
}

int TTX_CheckAnalogCP_M14Bx(void)
{
	LX_VBI_CP_TYPE_T ret = RET_ERROR;
	unsigned int analog_cp_psync;
	unsigned int analog_cp_cstripe;
	unsigned int cur_hlock, cur_vlock;
	
	/* Get Lock info */
	CVD_M14Bx_RdFL(iris_001);
	CVD_M14Bx_Rd01(iris_001, reg_hlock, cur_hlock);
	CVD_M14Bx_RdFL(iris_001);
	CVD_M14Bx_Rd01(iris_001, reg_vlock, cur_vlock);

	/* Get CP infro */
	CVD_M14Bx_RdFL(iris_001);
	CVD_M14Bx_Rd01(iris_001, reg_mv_vbi_detected, analog_cp_psync);
	CVD_M14Bx_Rd01(iris_001, reg_mv_colourstripes, analog_cp_cstripe);


	do{
		ret = 0; // Initiate return value
		if((analog_cp_psync == 0)||(cur_hlock == 0)||(cur_vlock == 0)) 
		{
			ret = 0; 
			break;
		}
		else {
			ret = (ret)|(LX_VBI_CP_TYPE_ANALOG_1);
		}

		/* DO NOT USE CSTRIPE INFO!
		 * CVD CP CSTRIPE HW DOES NOT WORK PROPERLY!! */
		//if(analog_cp_cstripe) ret = (ret)|(LX_VBI_CP_TYPE_ANALOG_2);
	}while(0);

	if(gEnableTTXDataPrint){
		printk("[%s] ret[0x%x] PSYNC[0x%x],CS[0x%x], Hlock[%d] Vlock[%d]\n", __F__, ret, analog_cp_psync, analog_cp_cstripe, cur_hlock, cur_vlock);
	}

	return (int)ret;
}



#ifdef	KDRV_CONFIG_PM

int TTX_RunSuspend_M14Bx(void)
{
	// param will be saved automatically....
	return 0;
}

int TTX_RunResume_M14Bx(void)
{
	TTX_PRINT("[%s:%d] Resume Phase #1\n", __F__, __L__);
	
	// Need to modify kdriver to use little endian.
	CVD_M14Bx_RdFL(vbi_ctrl_001);
	CVD_M14Bx_Wr01(vbi_ctrl_001, reg_vbi_little_endian, 0);
	CVD_M14Bx_WrFL(vbi_ctrl_001);

	/* 2013.06.27 won.hur : Added code for reg_swrst_vbi set to 0 
	 * There will be no operation in vbi module if this value is not set to 0.
	 * Please be aware of this CTOP register */
	CTOP_CTRL_M14B0_RdFL(DEI, ctr03);
	CTOP_CTRL_M14B0_Wr01(DEI, ctr03, swrst_cvd_vbi, 0x0);
	CTOP_CTRL_M14B0_WrFL(DEI, ctr03);

	TTX_SetInputFormat_M14Bx(CURRENT_TTX);

	TTX_PRINT("[%s:%d] Done\n", __F__, __L__);

	return 0;
}

#endif

/** @} */

