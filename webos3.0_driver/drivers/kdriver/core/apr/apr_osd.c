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
 *  rendering interface file for apr device
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.10
 *  note		Additional information.
 *
 *  @addtogroup apr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define USE_DISPLAY_TEXT_ON_SCREEN

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "apr_osd.h"
#include "apr_def.h"
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
#include "font_bmp_14x18.h"
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define APR_OSD_REG_BASE  0xC0035200
#define APR_OSD0_OFFSET   0x00000000
#define APR_OSD1_OFFSET   0x00000040

// register offset
#define OSD_EN            0x00
#define OSD_COLOR_FORMAT  0x08
#define OSD_SIZE          0x24
#define OSD_FRAME_ADDR    0x38

#define APR_OSD_MAX_WIDTH   1280
#define APR_OSD_MAX_HEIGHT   720

#define APR_OSD_RECT_X       32
#define APR_OSD_RECT_Y       32

#define APR_OSD_FONT_WIDTH   14
#define APR_OSD_FONT_HEIGHT  18

#define APR_OSD_FONT_X_OFS   32
#define APR_OSD_FONT_Y_OFS   70 

#define APR_OSD_FONT_MAX_CHAR ((APR_OSD_MAX_WIDTH-APR_OSD_FONT_X_OFS)/APR_OSD_FONT_WIDTH   - 1)
#define APR_OSD_FONT_MAX_LINE ((APR_OSD_MAX_HEIGHT-APR_OSD_FONT_Y_OFS)/APR_OSD_FONT_HEIGHT - 1)

#define APR_OSD_FONT_ALPHA 0x70
#define BMP_HEADER_SIZE      14
#define BMP_TOP_CROP_SIZE     2

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define OSD_PRINT if(1)APR_PRINT
#define APR_OSD_WR( addr, value )	    ( *( volatile UINT32 * )( h_osd.pOsdReg + (addr>>2) )) = ( UINT32 )( value )
#define APR_OSD_RD( addr )	            ( *( volatile UINT32 * )( h_osd.pOsdReg + (addr>>2) ))
#define CHECK_OSD() if(!h_osd.valid)    {OSD_PRINT("OSD1 is not avalible.\n");break;}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
/*
 * color information (32bpp)
 * (31:24) alpha
 * (23: 0) color
 * 0x00FF0000 red
 * 0x0000FF00 blue
 * 0x000000FF green
 */
RECT_INFO_T pattern_none[1] = {
	{ TRUE,  1,  1, 32, 32, 0x80000000},
};

RECT_INFO_T pattern_tp[4] = {
	{ TRUE, 13, 13, 20, 20, 0xFF000000},
	{ TRUE,  1,  1, 32, 32, 0xFFFFFFFF}, // white
	{FALSE,  0,  0,  0,  0,          0},
	{FALSE,  0,  0,  0,  0,          0}
};

RECT_INFO_T pattern_2d[4]  = {
	{ TRUE,  1,  1, 32, 32, 0xFF00FFFF}, // 
	{FALSE,  0,  0,  0,  0,          0},
	{FALSE,  0,  0,  0,  0,          0},
	{FALSE,  0,  0,  0,  0,          0}
};

RECT_INFO_T pattern_ss[4]  = {
	{ TRUE,  1,  1, 16, 32, 0xFFFF0000}, // 
	{ TRUE, 17,  1, 32, 32, 0xFF0000FF},
	{FALSE,  0,  0,  0,  0,          0},
	{FALSE,  0,  0,  0,  0,          0}
};

RECT_INFO_T pattern_tb[4]  = {
	{ TRUE,  1,  1, 32, 16, 0xFFFF0000}, // 
	{ TRUE,  1, 17, 32, 32, 0xFF0000FF},
	{FALSE,  0,  0,  0,  0,          0},
	{FALSE,  0,  0,  0,  0,          0}
};

RECT_INFO_T pattern_cb[4]  = {
	{ TRUE,  1,  1, 16, 16, 0xFFFF0000}, // 
	{ TRUE, 17, 17, 32, 32, 0xFFFF0000},
	{ TRUE, 17,  1, 32, 16, 0xFF0000FF},
	{ TRUE,  1, 17, 16, 32, 0xFF0000FF}
};

RECT_INFO_T pattern_cc[4]  = {
	{ TRUE,  1,  1,  8, 32, 0xFFFF0000}, // 
	{ TRUE, 17,  1, 24, 32, 0xFFFF0000},
	{ TRUE,  9,  1, 16, 32, 0xFF0000FF},
	{ TRUE, 25,  1, 32, 32, 0xFF0000FF}
};

RECT_INFO_T pattern_ll[4]  = {
	{ TRUE,  1,  1, 32,  8, 0xFFFF0000}, // 
	{ TRUE,  1, 17, 32, 24, 0xFFFF0000},
	{ TRUE,  1,  9, 32, 16, 0xFF0000FF},
	{ TRUE,  1, 25, 32, 32, 0xFF0000FF}
};

#ifdef USE_DISPLAY_TEXT_ON_SCREEN
CHAR_MAP_T char_map[] = {
	{'A','a',char_A},        {'0','0',char_0},        {'[','[',char_open_brk},   
	{'B','b',char_B},        {'1','1',char_1},        {']',']',char_close_brk},
	{'C','c',char_C},        {'2','2',char_2},        {':',':',char_semi},
	{'D','d',char_D},        {'3','3',char_3},        {';',';',char_colon},
	{'E','e',char_E},        {'4','4',char_4},        {'=','=',char_equal},
	{'F','f',char_F},        {'5','5',char_5},        {'_','_',char_under}, 
	{'G','g',char_G},        {'6','6',char_6},        {'/','/',char_slash},   
	{'H','h',char_H},        {'7','7',char_7},        {'|','|',char_vertical}, 
	{'I','i',char_I},        {'8','8',char_8},        {'^','^',char_exponent}, 
	{'J','j',char_J},        {'9','9',char_9},        {'<','<',char_less}, 
	{'K','k',char_K},        {'&','&',char_and},      {'>','>',char_greater}, 
	{'L','l',char_L},        {'"','"',char_quotation},{'!','!',char_exclamation},
	{'M','m',char_M},        {'@','@',char_at},       {'{','{',char_open_brc},  
	{'N','n',char_N},        {'~','~',char_tilde},    {'}','}',char_close_brc}, 
	{'O','o',char_O},        {'$','$',char_cache},    {0x5C,0x5C,char_backslash},    
	{'P','p',char_P},        {'%','%',char_percent},  {0x27,0x27,char_apostrophe},
	{'Q','q',char_Q},        {'*','*',char_asterik},
	{'R','r',char_R},        {'#','#',char_sharp},    
	{'S','s',char_S},        {' ',' ',char_space},    
	{'T','t',char_T},        {'(','(',char_open_par}, 
	{'U','u',char_U},        {')',')',char_close_par},
	{'V','v',char_V},        {'-','-',char_minus},    
	{'W','w',char_W},        {'+','+',char_plus},     
	{'X','x',char_X},        {'?','?',char_question}, 
	{'Y','y',char_Y},        {',',',',char_comma},
	{'Z','z',char_Z},        {'.','.',char_period},   {0xFF,0xFF,char_null}
};
#endif

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
static int APR_OSD_MapCharacter(char *ch, int **sel_array);
static int APR_OSD_DrawRect(BOOLEAN erase,       \
							UINT32 x_ofs,        \
							UINT32 y_ofs,        \
							RECT_INFO_T *pattern,\
							UINT32 pattern_size);
static int APR_OSD_DrawFont(BOOLEAN erase,       \
							UINT32 x_ofs,        \
							UINT32 y_ofs,        \
							UINT32 width,        \
							UINT32 height,       \
							UINT32 *pFont,       \
							UINT32 font_size);
static int APR_OSD_EraseFont(APR_OSD_PLANE_INFO plane,\
							UINT32 x_ofs,             \
							UINT32 y_ofs,             \
							UINT32 width,             \
							UINT32 height,            \
							UINT32 *pFont,            \
							UINT32 font_size);
static int APR_OSD_ClearPattern(void);
static int APR_OSD_ClearLine(UINT32 line);
static int APR_OSD_ClearAllLine(void);
#endif
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OSD_INFO_T h_osd;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
static ROW_USE_INFO_T used_row[APR_OSD_PLANE_MAX][APR_OSD_FONT_MAX_LINE];
static ROW_USE_INFO_T used_ptn[APR_OSD_PLANE_MAX];
static UINT32 vsync_count_for_osdflip = 0;
#endif
/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * osd initialize for APR device
 *
 * @param	none	
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_Init(void)
{
	int ret = RET_OK;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	UINT32 osd_frame_base = 0;
	UINT32 osd_width = 0, osd_height = 0, osd_stride = 0;
	UINT32 osd_layer = 0;
	static BOOLEAN init_flag = FALSE;

	do {
		if(init_flag){break;}
		// ioremap for osd layer 1 reg & frame
		h_osd.pOsdReg = (volatile UINT32 *)ioremap(APR_OSD_REG_BASE + APR_OSD1_OFFSET, sizeof(UINT32)*16);
		
		osd_frame_base = APR_OSD_RD(OSD_FRAME_ADDR);
		osd_width      = (APR_OSD_RD(OSD_SIZE) >> 16) & 0x0FFF;
		osd_height     = (APR_OSD_RD(OSD_SIZE) >> 0 ) & 0x0FFF;
		osd_stride     = osd_width;
		osd_layer      = 1;

		if(     (h_osd.pOsdReg == NULL )         || \
				(osd_frame_base < 0x60000000)    || \
				(osd_frame_base > 0x90000000)    || \
				(osd_width  < 1280)              || \
				(osd_height < 720)               || \
				(osd_width  > 1920)              || \
				(osd_height > 1080) )
		{
			OSD_PRINT("%s : ###### disabled OSD Layer 1 ######\n", __F__);

			h_osd.pOsdReg = (volatile UINT32 *)ioremap(APR_OSD_REG_BASE + APR_OSD0_OFFSET, sizeof(UINT32)*16);
			osd_frame_base = APR_OSD_RD(OSD_FRAME_ADDR);
			osd_width      = (APR_OSD_RD(OSD_SIZE) >> 16) & 0x0FFF;
			osd_height     = (APR_OSD_RD(OSD_SIZE) >> 0 ) & 0x0FFF;
			osd_stride     = osd_width;
			osd_layer      = 0;
			if(	(osd_frame_base < 0x60000000)    || \
				(osd_frame_base > 0x90000000)    || \
				(osd_width  < 1280)              || \
				(osd_height < 720)               || \
				(osd_width  > 1920)              || \
				(osd_height > 1080) )
			{
				OSD_PRINT("%s : ###### disabled OSD Layer 0 ######\n", __F__);
				h_osd.valid = FALSE;
				break;
			}
		}
		
		h_osd.osd_base_phys[0] = osd_frame_base;
		h_osd.osd_base_phys[1] = osd_frame_base + (4*osd_width*osd_height);
		h_osd.osd_base_virt[0] = (UINT32 *)ioremap(h_osd.osd_base_phys[0], 4*osd_stride*osd_height);
		h_osd.osd_base_virt[1] = (UINT32 *)ioremap(h_osd.osd_base_phys[1], 4*osd_stride*osd_height);

		if((h_osd.osd_base_virt[0]) && (h_osd.osd_base_virt[1]))
		{
			memset((char*)h_osd.osd_base_virt[0], 0, 4*osd_stride*osd_height);
			memset((char*)h_osd.osd_base_virt[1], 0, 4*osd_stride*osd_height);
		}
		
		h_osd.max_x  = osd_width;
		h_osd.max_y  = osd_height;
		h_osd.stride = osd_stride;

		memset(used_row, 0, sizeof(ROW_USE_INFO_T) * APR_OSD_PLANE_MAX * APR_OSD_FONT_MAX_LINE);
		memset(used_ptn, 0, sizeof(ROW_USE_INFO_T) * APR_OSD_PLANE_MAX);

		h_osd.valid = TRUE;
		init_flag = TRUE;
#if 0	
		APR_OSD_DrawRect(FALSE,  32, 32, pattern_tp, sizeof(pattern_tp));
		APR_OSD_DrawRect(FALSE,  96, 32, pattern_2d, sizeof(pattern_ss));
		APR_OSD_DrawRect(FALSE, 160, 32, pattern_ss, sizeof(pattern_ss));
		APR_OSD_DrawRect(FALSE, 224, 32, pattern_tb, sizeof(pattern_tb));
		APR_OSD_DrawRect(FALSE, 288, 32, pattern_cb, sizeof(pattern_cb));
		APR_OSD_DrawRect(FALSE, 352, 32, pattern_cc, sizeof(pattern_cc));
		APR_OSD_DrawRect(FALSE, 416, 32, pattern_ll, sizeof(pattern_ll));
		
		APR_OSD_DrawFont(FALSE, 32 + (APR_OSD_MAX_WIDTH>>1), 96 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_A, sizeof(char_A));
		APR_OSD_DrawFont(FALSE, 32 + (APR_OSD_MAX_WIDTH>>1),160 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_W, sizeof(char_W));
		APR_OSD_DrawFont(FALSE, 32 + (APR_OSD_MAX_WIDTH>>1),224 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_5, sizeof(char_5));
		APR_OSD_DrawFont(FALSE, 32 + (APR_OSD_MAX_WIDTH>>1),288 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_percent, sizeof(char_percent));

		APR_OSD_DrawFont(FALSE, 32, 96 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_C, sizeof(char_C));
		APR_OSD_DrawFont(FALSE, 96, 96 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_P, sizeof(char_P));
		APR_OSD_DrawFont(FALSE, 60, 96 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_U, sizeof(char_U));
		APR_OSD_DrawFont(FALSE,288, 96 + (APR_OSD_MAX_HEIGHT>>1), APR_OSD_FONT_WIDTH, APR_OSD_FONT_HEIGHT, char_i, sizeof(char_i));
#endif

		OSD_PRINT("%s : ###### Layer[%d] OsdBase[0x%08x], width[%d] height[%d] stride[%d] ######\n", __F__,\
																		osd_layer,     \
																		osd_frame_base,\
																		osd_width,     \
																		osd_height,    \
																		osd_stride);
	} while(0);
#else
	h_osd.valid = TRUE;
#endif
	return ret;
}

/**
 * osd uninitialize for APR device
 *
 * @param	none	
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_Uninit(void)
{
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	if(h_osd.pOsdReg)	       iounmap(h_osd.pOsdReg);
	if(h_osd.osd_base_virt[0]) iounmap(h_osd.osd_base_virt[0]);
	if(h_osd.osd_base_virt[1]) iounmap(h_osd.osd_base_virt[1]);
#endif
	return RET_OK;
}

/**
 * osd control
 *
 * @param	enable [IN] enable/disable osd
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_SetControl(BOOLEAN enable)
{
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	UINT32 reg = 0;
	do {
		CHECK_OSD();
		reg = APR_OSD_RD(OSD_EN);
		if(enable)
		{
			APR_OSD_WR(OSD_EN, reg | 0x11);
		}
		else
		{
			APR_OSD_WR(OSD_EN, reg & 0xFFFFFFFE);
		}
	} while(0);
#endif
	return RET_OK;
}

/**
 * osd flip control (for double buffer)
 *
 * @param	cmd [IN] SET_BASE/SET_FLIP
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_SetFlip(APR_OSD_FLIP_CMD cmd)
{
	int ret = RET_OK;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	do {
		CHECK_OSD();
		switch(cmd)
		{
			case APR_OSD_SET_BASE :
				if(	h_osd.plane == APR_OSD_PLANE_0 )
				{
					h_osd.pOsdBase = h_osd.osd_base_virt[1];
					h_osd.plane    = APR_OSD_PLANE_1;
				}
				else if( h_osd.plane == APR_OSD_PLANE_1 )
				{
					h_osd.pOsdBase = h_osd.osd_base_virt[0];
					h_osd.plane    = APR_OSD_PLANE_0;
				}
				break;
			case APR_OSD_SET_FLIP :
				vsync_count_for_osdflip = 1;
				break;
			default:
				break;
		}
	} while(0);
#endif
	return ret;
}

/**
 * vsync interrupt handler for osd flip
 *
 * @param	
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_VsyncHandler(void)
{
	int ret = RET_OK;
	if(h_osd.valid != TRUE) return ret;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	if(vsync_count_for_osdflip == 0) return ret;
	if(vsync_count_for_osdflip++ == 2)
	{
		if(	h_osd.plane == APR_OSD_PLANE_0 )
		{
			APR_OSD_WR(OSD_FRAME_ADDR, h_osd.osd_base_phys[0]);
		}
		else if( h_osd.plane == APR_OSD_PLANE_1 )
		{
			APR_OSD_WR(OSD_FRAME_ADDR, h_osd.osd_base_phys[1]);
		}
		ret = APR_OSD_ClearAllLine();
		vsync_count_for_osdflip = 0;
	}
#endif
	return ret;
}

/**
 * clear all line, all layer
 *
 * @param	none
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_ClearAllLayer(void)
{
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	APR_OSD_SetFlip(APR_OSD_SET_BASE);
	APR_OSD_SetFlip(APR_OSD_SET_FLIP);
#endif
	return RET_OK;	
}

/**
 * draw pre-defined pattern for APR devide (max 4 rect)
 *
 * @param	pattern [IN] APR_OSD_PATTERN
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_DrawPattern(APR_OSD_PATTERN pattern)
{
	int ret = RET_OK;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	do {
		CHECK_OSD();
		switch(pattern)
		{
			case APR_OSD_TP : 
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_tp, sizeof(pattern_tp));
				break;
			case APR_OSD_2D :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_2d, sizeof(pattern_2d));
				break;
			case APR_OSD_SS :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_ss, sizeof(pattern_ss));
				break;
			case APR_OSD_TB :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_tb, sizeof(pattern_tb));
				break;
			case APR_OSD_CB :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_cb, sizeof(pattern_cb));
				break;
			case APR_OSD_CC :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_cc, sizeof(pattern_cc));
				break;
			case APR_OSD_LL :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_ll, sizeof(pattern_ll));
				break;
			case APR_OSD_MAX:
			default :
				APR_OSD_DrawRect(FALSE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_none, sizeof(pattern_none));
				break;
		}
		used_ptn[h_osd.plane].use = 1;
	} while(0);
#endif
	return ret;
}

/**
 * draw string on specific position
 *
 * @param	*str [IN] string (up tp 64)
 * @param	x    [IN] x position
 * @param	y    [IN] y position
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_DrawString(char *str, UINT32 x, UINT32 y)
{
	int ret = RET_OK;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	UINT32 x_idx = 0;
	UINT32 y_idx = 0;
	int *char_ptr = NULL;
	
	do {
		CHECK_OSD();
		if(!str) break;
		if(y >= APR_OSD_FONT_MAX_LINE) 
		{
			OSD_PRINT("%s : out of screen (up tp %d lines)", __F__, APR_OSD_FONT_MAX_LINE);
			break;
		}
		while(*str)
		{
			ret = APR_OSD_MapCharacter(str, &char_ptr);
			if(!char_ptr)
			{
				OSD_PRINT("%s : undefined character [ %c ]\n", __F__, *str);
				x_idx++;
				str++;
				continue;
			}
			APR_OSD_DrawFont(	FALSE,\
								APR_OSD_FONT_X_OFS + (x * APR_OSD_FONT_WIDTH)  + (x_idx * APR_OSD_FONT_WIDTH), \
								APR_OSD_FONT_Y_OFS + (y * APR_OSD_FONT_HEIGHT) + (y_idx * APR_OSD_FONT_HEIGHT),\
								APR_OSD_FONT_WIDTH,                        \
								APR_OSD_FONT_HEIGHT,                       \
								char_ptr,                                  \
								sizeof(UINT32) * APR_OSD_FONT_WIDTH * APR_OSD_FONT_HEIGHT);
			x_idx++;
			y_idx = 0; // do not change line
			if(x_idx >= 64) break;
			if((x + x_idx) >= APR_OSD_FONT_MAX_CHAR)
			{
				OSD_PRINT("%s : out of screen (up tp %d characters)", __F__, APR_OSD_FONT_MAX_CHAR);
				break;
			}
			str++;
		};
		used_row[h_osd.plane][y].use   = 1;
		used_row[h_osd.plane][y].x_ofs = x;
		used_row[h_osd.plane][y].x_num = x_idx;
	} while(0);
#endif
	return ret;
}

/**
 * draw string on specific position with integer parameter
 *
 * @param	*buf0 [IN] string (up tp 64)
 * @param	data0 [IN] integer value 
 * @param	*buf1 [IN] string (up tp 64)
 * @param	data1 [IN] integer value 
 * @param	*buf2 [IN] string (up tp 64)
 * @param	data2 [IN] integer value 
 * @param	*buf3 [IN] string (up tp 64)
 * @param	data3 [IN] integer value 
 * @param	x     [IN] x position 
 * @param	y     [IN] y position 
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_PrintParam(char *buf0,    \
						UINT32 data0, \
						char *buf1,   \
						UINT32 data1, \
						char *buf2,   \
						UINT32 data2, \
						char *buf3,   \
						UINT32 data3, \
						UINT32 x,     \
						UINT32 y)
{
	int ret = RET_OK;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	char str[64];
	char data[16];
	int size = 0;

	do {
		CHECK_OSD();
		memset(str,  0, 64);
		memset(data, 0, 16);

		do {
			if(!buf0) break;
			strcat(str, buf0);
			sprintf(data, "%d", data0);
			strcat(str, data);
			if(!buf1) break;
			strcat(str, buf1);
			sprintf(data, "%d", data1);
			strcat(str, data);
			if(!buf2) break;
			strcat(str, buf2);
			sprintf(data, "%d", data2);
			strcat(str, data);
			if(!buf3) break;
			strcat(str, buf3);
			sprintf(data, "%d", data3);
			strcat(str, data);
		} while(0);
		
		size = strlen(str);
		if(size<64)	ret = APR_OSD_DrawString(str, x, y);
	} while(0);
#endif
	return ret;
}

/**
 * draw string on specific position with integer parameter
 *
 * @param	*buf0 [IN] string (up tp 64)
 * @param	data0 [IN] hex value 
 * @param	*buf1 [IN] string (up tp 64)
 * @param	data1 [IN] hex value 
 * @param	x     [IN] x position 
 * @param	y     [IN] y position 
 * @return	OK if success, ERROR otherwisw,
 * @see
 * @author
 */
int APR_OSD_PrintParamHex(char *buf0, \
						UINT32 data0, \
						char *buf1,   \
						UINT32 data1, \
						char *buf2,   \
						UINT32 data2, \
						char *buf3,   \
						UINT32 data3, \
						UINT32 x,     \
						UINT32 y)
{
	int ret = RET_OK;
#ifdef USE_DISPLAY_TEXT_ON_SCREEN
	char str[64];
	char data[16];
	int size = 0;

	do {
		CHECK_OSD();
		memset(str,  0, 64);
		memset(data, 0, 16);

		do {
			if(!buf0) break;
			strcat(str, buf0);
			sprintf(data, "0x%08X", data0);
			strcat(str, data);
			if(!buf1) break;
			strcat(str, buf1);
			sprintf(data, "0x%08X", data1);
			strcat(str, data);
			if(!buf2) break;
			strcat(str, buf2);
			sprintf(data, "0x%08X", data2);
			strcat(str, data);
			if(!buf3) break;
			strcat(str, buf3);
			sprintf(data, "0x%08X", data3);
			strcat(str, data);
		} while(0);
		
		size = strlen(str);
		if(size<64)	ret = APR_OSD_DrawString(str, x, y);
	} while(0);
#endif
	return ret;
}

#ifdef USE_DISPLAY_TEXT_ON_SCREEN
static int APR_OSD_DrawRect(BOOLEAN erase, UINT32 x_ofs, UINT32 y_ofs, RECT_INFO_T *pattern, UINT32 pattern_size)
{
	int ret = RET_OK;
	int i=0, j=0;
	volatile UINT32 *ptr;
	UINT32 rect_num= 0;
	UINT32 width, height;
	UINT32 *osd_base;

	do {
		if(erase)osd_base = (h_osd.plane==APR_OSD_PLANE_0)?h_osd.osd_base_virt[1]:h_osd.osd_base_virt[0];
		else     osd_base = (h_osd.plane==APR_OSD_PLANE_0)?h_osd.osd_base_virt[0]:h_osd.osd_base_virt[1];
		rect_num =  pattern_size / sizeof(RECT_INFO_T);
		while(rect_num--)
		{
			if(pattern[rect_num].active == FALSE) continue;
			width = pattern[rect_num].x1 - pattern[rect_num].x0 + 1;
			height = pattern[rect_num].y1 - pattern[rect_num].y0 + 1;
			for(i=0;i<height;i++)
			{
				for(j=0;j<width;j++)
				{
					ptr  = (volatile UINT32 *)(osd_base + (y_ofs * h_osd.stride + x_ofs + pattern[rect_num].x0 + j) + ((i * h_osd.stride) + (pattern[rect_num].y0 * h_osd.stride))); 
					if(erase) *ptr = 0;
					else *ptr = pattern[rect_num].color; 
				}
			}
		}

	} while(0);

	return ret;
}

static int APR_OSD_DrawFont(BOOLEAN erase, UINT32 x_ofs, UINT32 y_ofs, UINT32 width, UINT32 height, UINT32 *pFont, UINT32 font_size)
{
	int ret = RET_OK;
	int i=0, j=0;
	volatile UINT32 *ptr;
	UINT32 pxl_cnt = 0;
	UINT32 font;

	do {
		for(i=(height-1);i>=0;i--)
		{
			if(i<BMP_TOP_CROP_SIZE)	continue;  // crop top 2 lines
			for(j=0;j<width;j++)
			{
				ptr  = (volatile UINT32 *)(h_osd.pOsdBase + (y_ofs * h_osd.stride + x_ofs + j) + ((i * h_osd.stride) + (height* h_osd.stride))); 
				if(erase) font = 0;
				else font = pFont[BMP_HEADER_SIZE + pxl_cnt++];
				if((font & 0x00FFFFFF) < 0x00C00000)
				{
					font = 0;
				}
					
				font &= 0x00FFFFFF;
				font |= (APR_OSD_FONT_ALPHA << 24);
				*ptr = font;
			}
		}
		if(pxl_cnt > font_size) OSD_PRINT("mismatch  pixel count[%d] & font size[%d]\n", pxl_cnt, font_size);
	} while(0);

	return ret;
}

static int APR_OSD_EraseFont(APR_OSD_PLANE_INFO plane, UINT32 x_ofs, UINT32 y_ofs, UINT32 width, UINT32 height, UINT32 *pFont, UINT32 font_size)
{
	int ret = RET_OK;
	int i=0, j=0;
	UINT32 *osd_base;
	UINT32 *ptr;

	do {
		osd_base = (plane==APR_OSD_PLANE_0)?h_osd.osd_base_virt[0]:h_osd.osd_base_virt[1];
		for(i=(height-1);i>=0;i--)
		{
			if(i<BMP_TOP_CROP_SIZE) continue;  // crop top 2 lines
			for(j=0;j<width;j++)
			{
				ptr  = (UINT32*)(osd_base + (y_ofs * h_osd.stride + x_ofs + j) + ((i * h_osd.stride) + (height* h_osd.stride))); 
				*ptr = 0;
			}
		}
	} while(0);

	return ret;
}

static int APR_OSD_MapCharacter(char *ch, int **sel_array)
{
	int ret = RET_OK;
	int len = 0;
	int idx = 0;

	do {
		len = sizeof(char_map) / sizeof(CHAR_MAP_T);
		
		do {
			if(*ch == char_map[idx].upper) break;
			if(*ch == char_map[idx].lower) break;
		} while(idx++ < (len-1));

		if(idx < len)
			*sel_array = (int*)char_map[idx].ch_array;
		else
			*sel_array = NULL;

	} while(0);
	
	return ret;
}

static int APR_OSD_ClearPattern(void)
{
	int ret = RET_OK;
	APR_OSD_PLANE_INFO plane; 

	do {
		plane = (h_osd.plane==APR_OSD_PLANE_0)?APR_OSD_PLANE_1:APR_OSD_PLANE_0;
		if(used_ptn[plane].use == 1) {
			APR_OSD_DrawRect(TRUE, APR_OSD_RECT_X, APR_OSD_RECT_Y, pattern_none, sizeof(pattern_none));
			used_ptn[plane].use = 0;
		}
	} while(0);

	return ret;
}

static int APR_OSD_ClearLine(UINT32 line)
{
	int ret = RET_OK;
	UINT32 x = 0;
	UINT32 y = 0;
	UINT32 x_idx = 0;
	UINT32 y_idx = 0;
	APR_OSD_PLANE_INFO plane; 

	do {
		plane = (h_osd.plane==APR_OSD_PLANE_0)?APR_OSD_PLANE_1:APR_OSD_PLANE_0;
		if(!used_row[plane][line].use) break;
		x = used_row[plane][line].x_ofs;
		y = line;
		do {
			APR_OSD_EraseFont(	plane,\
								APR_OSD_FONT_X_OFS + (x * APR_OSD_FONT_WIDTH)  + (x_idx * APR_OSD_FONT_WIDTH), \
								APR_OSD_FONT_Y_OFS + (y * APR_OSD_FONT_HEIGHT) + (y_idx * APR_OSD_FONT_HEIGHT),\
								APR_OSD_FONT_WIDTH,                        \
								APR_OSD_FONT_HEIGHT,                       \
								NULL,                                      \
								sizeof(UINT32) * APR_OSD_FONT_WIDTH * APR_OSD_FONT_HEIGHT);
			y_idx = 0; // do not change line
			if(x_idx++ >= (used_row[plane][line].x_num-1)) break;
		} while(1);
		used_row[plane][line].use = 0;
	} while(0);

	return ret;
}

static int APR_OSD_ClearAllLine(void)
{
	UINT32 i;
	
	APR_OSD_ClearPattern();

	for(i=0;i<APR_OSD_FONT_MAX_LINE;i++) {
		APR_OSD_ClearLine(i);
	}
	return RET_OK;
}
#endif

