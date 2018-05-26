/*
SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
Copyright(c) 1998-2013 by LG Electronics Inc.
 
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
 *  mjpeg encoder/decoder interface implementation.
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.04.11
 *  note		Additional information.
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
#define FRAME_ENDIAN				BITS64_LITTLE
#define STREAM_ENDIAN				BITS64_LITTLE
//#define MJPEG_ENDOFSTREAM

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/mm.h>

#include "os_util.h"
#include "mjpeg_core.h"
#include "mjpeg_drv.h"
#include "mjpeg_const.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define MIN_VSIZE			LX_MJPEG_MIN_VSIZE
#define MIN_HSIZE			LX_MJPEG_MIN_HSIZE
#define MAX_VSIZE			LX_MJPEG_MAX_VSIZE
#define MAX_HSIZE			LX_MJPEG_MAX_HSIZE

#define DISABLE 			0
#define ENABLE				1

enum {
	BITS64_LITTLE			= 0,		// [1] 32(1)/64(0), [0] BIG(1)/LITTLE(0)
	BITS64_BIG				= 1,
	BITS32_LITTLE			= 2,
	BITS32_BIG				= 3,
};

enum {
	HUFF_MIN_TABLE			= 0,
	HUFF_MAX_TABLE			= 1,
	HUFF_POINTER_TABLE		= 2,
	HUFF_VALUE_TABLE 		= 3,
};

enum {
	HUFF_DC_INDEX0			= 0,
	HUFF_AC_INDEX0			= 1,
	HUFF_DC_INDEX1			= 2,
	HUFF_AC_INDEX1			= 3,
};

enum {
	COMPONENT_Y 			= 0,
	COMPONENT_CB			= 1,
	COMPONENT_CR			= 2,
};

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#ifndef MIN
#define MIN( a, b )     ( (a) <= (b) ? (a) : (b) )
#endif

#ifndef MAX
#define MAX( a, b )     ( (a) >= (b) ? (a) : (b) )
#endif

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/
typedef enum {
	MJPEG_SAMPLE_420	= 0xA,
	MJPEG_SAMPLE_H422	= 0x9,
	MJPEG_SAMPLE_V422	= 0x6,
	MJPEG_SAMPLE_444	= 0x5,
	MJPEG_SAMPLE_400	= 0x1,
} SAMPLE_FACTOR_T;

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/
extern volatile MJPEG_REG_T*	gpRealRegMJPEG; 	/** mjpeg register */
extern MJPEG_REG_T*				gpRegMJPEG; 		/** mjpeg shadow register */

#if defined(MJPEG_DEBUG_REG_DUMP) || defined(MJPEG_DEBUG_TRACE_DUMP)
extern char 	*_logBuffer;
extern int		_logIndex;
extern int		_logNumber;
#endif

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
// For encoding and decoding
static void		_Initialize_Reg(void);
static void		_SetRST_Reg(UINT16 i_ui16RstInterval);
static void		_SetDPB_Reg(LX_MJPEG_IMAGE_BUF_T *i_pstImageBuffer);
static void		_Start_Reg(void);

// For encoding
static void		_Encode_SetBBC_Reg(LX_MEM_BUF_T *i_pstOutputBuffer);
static void		_Encode_SetGBU_Reg(void);
static void		_Encode_SetMain_Reg(LX_MJPEG_HEADER_T *i_pstHeader, LX_MJPEG_IMAGE_INFO_T *pstImageInfo, LX_MJPEG_OPTION_T *i_pstOption);
static BOOLEAN	_Encode_SetHuffmanTable_Reg(LX_MJPEG_HUFFMAN_TABLE_T *i_pstHuffman);
static void		_Encode_SetQTable_Reg(LX_MJPEG_FRAME_HEADER_T *i_pstFrameInfo, LX_MJPEG_QUANTIZATION_TABLE_T *i_pstQTable);
static int		_Encode_CreateQuantTable( LX_MJPEG_QUANTIZATION_TABLE_T *pstQTable, int q );

// For decoding
static void		_Decode_SetBBC_Reg(LX_MEM_STREAM_T *i_pstInputStream, UINT32 i_i32Size, UINT32 i_ui32PagePointer, BOOLEAN bEndofStream);
static void		_Decode_SetGBU_Reg(UINT32 i_ui32PagePointer, UINT32 i_ui32WordPointer, UINT32 i_ui32ReadPointer);
static void		_Decode_SetMain_Reg(LX_MJPEG_HEADER_T *i_pstHeader, LX_MJPEG_IMAGE_INFO_T *i_pstImageInfo,
											BOOLEAN i_bUseUserHuff, LX_MJPEG_OPTION_T *i_pstOption);
static void		_Decode_SetDPCM_Reg(void);
static BOOLEAN	_Decode_SetHuffmanTable_Reg(LX_MJPEG_HUFFMAN_TABLE_T *i_pstHuffman);
static void		_Decode_SetQTable_Reg(LX_MJPEG_FRAME_HEADER_T *i_strFrameInfo, LX_MJPEG_QUANTIZATION_TABLE_T *i_strQTable);

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
static const UINT8 _g_ui8ConfigPreset[5][6] = {
	{ 2, 6, 3, 10, 5, 5 },
	{ 3, 4, 3,  9, 5, 5 },
	{ 3, 4, 3,  6, 5, 5 },
	{ 4, 3, 3,  5, 5, 5 },
	{ 4, 1, 1,  5, 0, 0 }
}; // # of request, total block #, #of components, sampling factor 0, sampling factor 1, sampling factor 2

static LX_MJPEG_HEADER_T *_g_pstHeader;
static LX_MJPEG_IMAGE_INFO_T *_g_pstImageInfo;
static LX_MJPEG_IMAGE_BUF_T *g_pstImageBuffer;
LX_MJPEG_OPTION_T *_g_pstOption;

static UINT16 _g_ui16HuffSizes[256];
static UINT16 _g_ui16HuffCodes[256];
static UINT16 _g_ui16EncHuffSizes[256];
static UINT16 _g_ui16EncHuffCodes[256];

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * Initialize encoder
 *
 */
UINT32	MJPEG_ENC_Init(LX_MJPEG_IMAGE_INFO_T *i_pstImageInfo, LX_MJPEG_IMAGE_BUF_T *i_pstImageBuf,
									LX_MJPEG_OPTION_T *i_pstOption, LX_MJPEG_HEADER_T *o_pstHeader )
{
	BOOLEAN bSwap;
	UINT32 ui32ImageSize;
	const UINT8 ui8ConfigPreset[5][6] = {	{ 3, 1, 2, 2, 0 },
											{ 3, 1, 2, 1, 0 },
											{ 3, 1, 1, 2, 0 },
											{ 3, 1, 1, 1, 0 },
											{ 1, 1, 1, 1, 0 }	};	// number of component and preset of component ID 0.

	_g_pstImageInfo = i_pstImageInfo;
	_g_pstHeader = o_pstHeader;
	g_pstImageBuffer = i_pstImageBuf;
	_g_pstOption = i_pstOption;

	if ( ( ( _g_pstImageInfo->eFormat != LX_MJPEG_FORMAT_420 ) &&
		   ( _g_pstImageInfo->eFormat != LX_MJPEG_FORMAT_422 ) &&
		   ( _g_pstImageInfo->eFormat != LX_MJPEG_FORMAT_224 ) &&
		   ( _g_pstImageInfo->eFormat != LX_MJPEG_FORMAT_444 ) &&
		   ( _g_pstImageInfo->eFormat != LX_MJPEG_FORMAT_400 ) )	||
		   ( _g_pstImageInfo->ui16Height < MIN_VSIZE ) 			||
		   ( _g_pstImageInfo->ui16Height > MAX_VSIZE )			||
		   ( _g_pstImageInfo->ui16Width < MIN_HSIZE )			||
		   ( _g_pstImageInfo->ui16Width > MAX_HSIZE ) )
	{
		MJPEG_WARN("Can't support image infomation.\n");
		return 0;
	}

	if ( i_pstImageInfo->eFormat < 0 || i_pstImageInfo->eFormat > 4 )
	{
		MJPEG_WARN("Invalid format type: %d\n", i_pstImageInfo->eFormat);
		return 0;
	}
	
	// Picture size alignment
	if ( i_pstImageInfo->eFormat == LX_MJPEG_FORMAT_420 || i_pstImageInfo->eFormat == LX_MJPEG_FORMAT_422 )
	{	i_pstImageInfo->ui16Width = ((i_pstImageInfo->ui16Width+15)/16)*16; }
	else
	{	i_pstImageInfo->ui16Width = ((i_pstImageInfo->ui16Width+7)/8)*8; }

	if ( i_pstImageInfo->eFormat == LX_MJPEG_FORMAT_420 || i_pstImageInfo->eFormat == LX_MJPEG_FORMAT_224 )
	{	i_pstImageInfo->ui16Height = ((i_pstImageInfo->ui16Height+15)/16)*16; }
	else
	{	i_pstImageInfo->ui16Height = ((i_pstImageInfo->ui16Height+7)/8)*8; }

	memcpy( (void *)&o_pstHeader->stFrameHeader, (void *)&_g_stFrameHeaderPreset, sizeof(LX_MJPEG_FRAME_HEADER_T) );
	memcpy( (void *)&o_pstHeader->stScanHeader, (void *)&_g_stScanHeaderPreset, sizeof(LX_MJPEG_SCAN_HEADER_T) );
	memcpy( (void *)&o_pstHeader->stHuffmanTables, (void *)_g_ui8PresetHuffTables[0], sizeof(LX_MJPEG_HUFFMAN_TABLE_T)*4 );

	if ( _g_pstOption->bQuality )
	{
		if ( _Encode_CreateQuantTable( o_pstHeader->stQuantiztionTables, (int)_g_pstOption->nQuality ) != RET_OK )
		{
			MJPEG_WARN("Invalid quality value: %d\n", _g_pstOption->nQuality );
			return 0;
		}
	}
	else
	{
		memcpy( (void *)&o_pstHeader->stQuantiztionTables, (void *)_g_ui8PresetQuantTables[MJPEG_ENCODE_QUALITY], sizeof(LX_MJPEG_QUANTIZATION_TABLE_T)*2 );
	}

	o_pstHeader->ui16ResetInterval = PRESET_RESTART_INTERVAL;
	o_pstHeader->stFrameHeader.ui8NumberOfComponents = ui8ConfigPreset[i_pstImageInfo->eFormat][0];
	o_pstHeader->stScanHeader.ui8NumberOfComponents = ui8ConfigPreset[i_pstImageInfo->eFormat][0];
	memcpy( (void *)o_pstHeader->stFrameHeader.stComponents, (void *)&ui8ConfigPreset[i_pstImageInfo->eFormat][1], 4 ); // copy preset values for component 1

	// Recalculate output image
	bSwap = ((_g_pstOption->eRotation == LX_MJPEG_ROTATE_90)||(_g_pstOption->eRotation == LX_MJPEG_ROTATE_270))? TRUE : FALSE;
	o_pstHeader->stFrameHeader.ui16Width	= (bSwap)? i_pstImageInfo->ui16Height : i_pstImageInfo->ui16Width;
	o_pstHeader->stFrameHeader.ui16Height	= (bSwap)? i_pstImageInfo->ui16Width  : i_pstImageInfo->ui16Height;

	if( bSwap == TRUE )
	{
		LX_MJPEG_FORMAT_T eFormat = (i_pstImageInfo->eFormat==LX_MJPEG_FORMAT_422)? LX_MJPEG_FORMAT_224 :
									(i_pstImageInfo->eFormat==LX_MJPEG_FORMAT_224)? LX_MJPEG_FORMAT_422 : i_pstImageInfo->eFormat;
		memcpy( (void *)o_pstHeader->stFrameHeader.stComponents, (void *)&ui8ConfigPreset[eFormat][1], 4 ); // copy preset values for component 1
	}

	// Recalculate image size
	ui32ImageSize = i_pstImageInfo->ui16Width*i_pstImageInfo->ui16Height;

	switch ( i_pstImageInfo->eFormat ) {
		case LX_MJPEG_FORMAT_420:	{ ui32ImageSize *= 3/2;	break; }
		case LX_MJPEG_FORMAT_422:	{ ui32ImageSize *= 2;	break; }
		case LX_MJPEG_FORMAT_224:	{ ui32ImageSize *= 2;	break; }
		case LX_MJPEG_FORMAT_444:	{ ui32ImageSize *= 3;	break; }
		case LX_MJPEG_FORMAT_400:	{ ui32ImageSize *= 1;	break; }
		default:				{ ui32ImageSize = 0; }
	}

	MJPEG_INFO("Initialized encoder.\n");

	return ui32ImageSize;
}

/**
 * Start encoding
 *
 */
void	MJPEG_ENC_Start(LX_MEM_BUF_T *i_pstDataBuf)
{
	LX_MEM_BUF_T stOutputBuffer;
	stOutputBuffer.addr = LX_CALC_ALIGNED_VALUE( i_pstDataBuf->addr - 3, 2 );
	stOutputBuffer.length = i_pstDataBuf->length;//LX_CALC_ALIGNED_VALUE( i_ui32DataLength, 2 );

	_Encode_SetBBC_Reg( &stOutputBuffer );
	_Encode_SetGBU_Reg();
	_Encode_SetMain_Reg( _g_pstHeader, _g_pstImageInfo, _g_pstOption );
	_SetRST_Reg( _g_pstHeader->ui16ResetInterval );
	_Encode_SetHuffmanTable_Reg( _g_pstHeader->stHuffmanTables );
	_Encode_SetQTable_Reg( &_g_pstHeader->stFrameHeader, _g_pstHeader->stQuantiztionTables );
	_SetDPB_Reg( g_pstImageBuffer );

#if 1
	_Start_Reg();
#else
	MJPEG_RdFL( pic_start );
	MJPEG_Wr01( pic_start, partial_start, 1 );
	MJPEG_WrFL( pic_start );
#endif

}

/**
 * Continue encoding
 *
 */
UINT32	MJPEG_ENC_Continue(LX_MEM_BUF_T *i_pstDataBuf)
{
	LX_MEM_BUF_T stOutputBuffer;
	UINT32 ui32BaseAddress, ui32EndAddress, ui32RdPtr, ui32WrPtr;
	UINT32 ui32Size = 0;

	stOutputBuffer.addr = LX_CALC_ALIGNED_VALUE( i_pstDataBuf->addr - 3, 2 );
	stOutputBuffer.length = i_pstDataBuf->length;//LX_CALC_ALIGNED_VALUE( i_ui32DataLength, 2 );

	MJPEG_RdFL( bbc_bas_addr );
	MJPEG_RdFL( bbc_endaddr );
	MJPEG_RdFL( bbc_rd_ptr );
	MJPEG_RdFL( bbc_wr_ptr );
	MJPEG_RdFL( bbc_ext_addr );
	MJPEG_RdFL( bbc_cur_pos );

	ui32BaseAddress = MJPEG_Rd( bbc_bas_addr );
	ui32EndAddress = MJPEG_Rd( bbc_endaddr );
	ui32RdPtr = MJPEG_Rd( bbc_rd_ptr );
	ui32WrPtr = MJPEG_Rd( bbc_wr_ptr );

	if (ui32RdPtr == ui32WrPtr)
		ui32Size = ui32EndAddress - ui32BaseAddress;
	else if (ui32RdPtr < ui32WrPtr)
		ui32Size = ui32WrPtr - ui32RdPtr;
	else if (ui32RdPtr > ui32WrPtr)
		ui32Size = ui32EndAddress - (ui32RdPtr - ui32WrPtr);

	MJPEG_Wr  ( bbc_bas_addr, stOutputBuffer.addr );
	MJPEG_Wr  ( bbc_endaddr, stOutputBuffer.addr + stOutputBuffer.length );
	MJPEG_Wr01( bbc_cur_pos, bbc_cur_pos, 0 );
	MJPEG_Wr  ( bbc_ext_addr, stOutputBuffer.addr );

	// Set read pointer
	MJPEG_Wr  ( bbc_rd_ptr, stOutputBuffer.addr );
	MJPEG_Wr  ( bbc_wr_ptr, stOutputBuffer.addr );

	MJPEG_WrFL( bbc_bas_addr );
	MJPEG_WrFL( bbc_endaddr );
	MJPEG_WrFL( bbc_rd_ptr );
	MJPEG_WrFL( bbc_wr_ptr );
	MJPEG_WrFL( bbc_ext_addr );
	MJPEG_WrFL( bbc_cur_pos );

	return ui32Size;
}


/**
 * End encoding
 *
 */
UINT32 MJPEG_ENC_End(void)
{
	UINT32 ui32BaseAddress, ui32EndAddress, ui32RdPtr, ui32WrPtr;
	UINT32 ui32Size = 0;

	MJPEG_RdFL( bbc_bas_addr );
	MJPEG_RdFL( bbc_endaddr );
	MJPEG_RdFL( bbc_rd_ptr );
	MJPEG_RdFL( bbc_wr_ptr );

	ui32BaseAddress = MJPEG_Rd( bbc_bas_addr );
	ui32EndAddress = MJPEG_Rd( bbc_endaddr );
	ui32RdPtr = MJPEG_Rd( bbc_rd_ptr );
	ui32WrPtr = MJPEG_Rd( bbc_wr_ptr );

	if (ui32RdPtr == ui32WrPtr)
		ui32Size = ui32EndAddress - ui32BaseAddress;
	else if (ui32RdPtr < ui32WrPtr)
		ui32Size = ui32WrPtr - ui32RdPtr;
	else if (ui32RdPtr > ui32WrPtr)
		ui32Size = ui32EndAddress - (ui32RdPtr - ui32WrPtr);

	MJPEG_RdFL( bbc_flush_cmd );
	MJPEG_Wr01( bbc_flush_cmd, bbc_flush_cmd, 0 );
	MJPEG_WrFL( bbc_flush_cmd );

	MJPEG_INFO("Finalize encoder.\n");

	return ui32Size;
}

/**
 * Initialize decoder
 *
 */
UINT32	MJPEG_DEC_Init(LX_MJPEG_HEADER_T *i_pstHeader, LX_MJPEG_IMAGE_BUF_T *i_pstImageBuf,
									LX_MJPEG_OPTION_T *i_pstOption, LX_MJPEG_IMAGE_INFO_T *o_pstImageInfo )
{
	SINT32 iCount;
	SAMPLE_FACTOR_T eSampleFactor = MJPEG_SAMPLE_400;
	UINT32 ui32OutputSize;
	LX_MJPEG_FRAME_HEADER_T *pstFrameHeader;

	_g_pstHeader = i_pstHeader;
	_g_pstImageInfo = o_pstImageInfo;
	g_pstImageBuffer = i_pstImageBuf;
	_g_pstOption = i_pstOption;
		
	if ( lx_chip_rev() >= LX_CHIP_REV(  H13, B0 ) )
	{
		if ( _g_pstOption->eOperation == LX_MJPEG_OPERATION_PARTIAL )
		{
			_g_pstOption->eRotation = LX_MJPEG_ROTATE_0;
			_g_pstOption->eMirroring = LX_MJPEG_MIRROR_NONE;
			//_g_pstOption->eScaleHorizontal = LX_MJPEG_SCALE_NONE;
			//_g_pstOption->eScaleVertical = LX_MJPEG_SCALE_NONE;
		}
	}
	
	pstFrameHeader	= &_g_pstHeader->stFrameHeader;

	// Check satisfies
	if ( ( _g_pstHeader->stFrameHeader.ui8SamplingPrecision != 8 )	||
		( _g_pstHeader->stFrameHeader.ui16Height < MIN_VSIZE ) 		||
		( _g_pstHeader->stFrameHeader.ui16Height > MAX_VSIZE ) 		||
		( _g_pstHeader->stFrameHeader.ui16Width < MIN_HSIZE )		||
		( _g_pstHeader->stFrameHeader.ui16Width > MAX_HSIZE )		||
		 ( ( _g_pstHeader->stFrameHeader.ui8NumberOfComponents != 1 ) &&
		   ( _g_pstHeader->stFrameHeader.ui8NumberOfComponents != 3 ) )	||
		 ( _g_pstHeader->stScanHeader.ui8Ss != 0)						||
		 ( _g_pstHeader->stScanHeader.ui8Se != 0x3F)						||
		 ( _g_pstHeader->stScanHeader.ui8Ah != 0)						||
		 ( _g_pstHeader->stScanHeader.ui8Al != 0)	)
	{
		return 0;
	}

	// Check invalide option
	if ( ( (_g_pstOption->eMirroring!=LX_MJPEG_MIRROR_NONE)||(_g_pstOption->eRotation!=LX_MJPEG_ROTATE_0) ) &&
		 ( (_g_pstOption->eScaleHorizontal!=LX_MJPEG_SCALE_NONE)||(_g_pstOption->eScaleVertical!=LX_MJPEG_SCALE_NONE) ) )
	{
		return 0;
	}

	// Calculate format
	if ( pstFrameHeader->ui8NumberOfComponents == 3 )
	{
		UINT8 min_comp_id = 255;
		
		for ( iCount = 0 ; iCount < pstFrameHeader->ui8NumberOfComponents ; ++iCount )
		{
			min_comp_id = MIN( min_comp_id, pstFrameHeader->stComponents[iCount].ui8ComponentID );
		}

		for ( iCount = 0 ; iCount < pstFrameHeader->ui8NumberOfComponents ; ++iCount )
		{
			if ( pstFrameHeader->stComponents[iCount].ui8ComponentID == min_comp_id )
			{
				eSampleFactor = ( (pstFrameHeader->stComponents[iCount].ui8HSampleFreq&3) << 2 )
								|(pstFrameHeader->stComponents[iCount].ui8VSampleFreq&3);
			}
			else if ( pstFrameHeader->stComponents[iCount].ui8HSampleFreq != 1 ||
					  pstFrameHeader->stComponents[iCount].ui8VSampleFreq != 1 )
			{
				MJPEG_DEBUG(("[DEC] Not Supported Sampling Factor\n"));
				return 0;
			}
		}
	}
	else if (pstFrameHeader->ui8NumberOfComponents == 1 )
	{
		eSampleFactor = MJPEG_SAMPLE_400;
	}
	else
	{
		return 0;
	}
	switch ( eSampleFactor )
	{
		case MJPEG_SAMPLE_420:	{ _g_pstImageInfo->eFormat = LX_MJPEG_FORMAT_420; break; }
		case MJPEG_SAMPLE_H422:	{ _g_pstImageInfo->eFormat = LX_MJPEG_FORMAT_422; break; }
		case MJPEG_SAMPLE_V422:	{ _g_pstImageInfo->eFormat = LX_MJPEG_FORMAT_224; break; }
		case MJPEG_SAMPLE_444:	{ _g_pstImageInfo->eFormat = LX_MJPEG_FORMAT_444; break; }
		default: /* 4:0:0*/		{ _g_pstImageInfo->eFormat = LX_MJPEG_FORMAT_400; }
	}

	// Picture size alignment
	if (_g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_420 || _g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_422)
		_g_pstImageInfo->ui16Width = ((pstFrameHeader->ui16Width+15)/16) * 16;
	else
		_g_pstImageInfo->ui16Width = ((pstFrameHeader->ui16Width+7)/8) * 8;

	if (_g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_420 || _g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_224)
		_g_pstImageInfo->ui16Height = ((pstFrameHeader->ui16Height+15)/16) * 16;
	else
		_g_pstImageInfo->ui16Height = ((pstFrameHeader->ui16Height+7)/8) * 8;

	// Calculate output image size
	switch (_g_pstImageInfo->eFormat)
	{
		case LX_MJPEG_FORMAT_420:
		{	ui32OutputSize = _g_pstImageInfo->ui16Width * _g_pstImageInfo->ui16Height * 3/2; break;	}
		case LX_MJPEG_FORMAT_422:
		{	ui32OutputSize = _g_pstImageInfo->ui16Width * _g_pstImageInfo->ui16Height * 2; break;	}
		case LX_MJPEG_FORMAT_224:
		{	ui32OutputSize = _g_pstImageInfo->ui16Width * _g_pstImageInfo->ui16Height * 2; break;	}
		case LX_MJPEG_FORMAT_444:
		{	ui32OutputSize = _g_pstImageInfo->ui16Width * _g_pstImageInfo->ui16Height * 3; break;	}
		default: /* 4:0:0*/
		{	ui32OutputSize = _g_pstImageInfo->ui16Width * _g_pstImageInfo->ui16Height * 1;	}
	}

	ui32OutputSize = (ui32OutputSize>>_g_pstOption->eScaleHorizontal)>>_g_pstOption->eScaleVertical;

	MJPEG_INFO("Initialized decoder.\n");
	
	return ui32OutputSize;
}

/**
 * Start decoding
 *
 */
void	MJPEG_DEC_Start(LX_MEM_BUF_T *i_pstDataBuf, UINT32 i_ui32DataLength, BOOLEAN bEndOfStream)
{
	LX_MEM_STREAM_T stInputStream;
	UINT32 ui32PagePointer, ui32WordPointer, ui32ReadPointer;
	BOOLEAN bUseUserHuffmanTable;
	UINT16 ui16Temp;
	BOOLEAN bSwap;

	MJPEG_LOG_TRACE();
	
	stInputStream.mem_buf.addr = LX_CALC_ALIGNED_VALUE( i_pstDataBuf->addr - 3, 2 );
	stInputStream.mem_buf.length = i_pstDataBuf->length;
	stInputStream.curr_offset = i_pstDataBuf->addr - stInputStream.mem_buf.addr;

	ui32PagePointer = stInputStream.curr_offset/256;
	ui32WordPointer = (stInputStream.curr_offset%256)/4 + ((ui32PagePointer&1)? 64 : 0);
	ui32WordPointer -= ( (ui32WordPointer&1)? 1 : 0 );
	ui32ReadPointer = (stInputStream.curr_offset%4)*8 + ( (((stInputStream.curr_offset%256)/4)&1)? 32 : 0 );

	// Initialize registers
	_Initialize_Reg();
	_Decode_SetBBC_Reg( &stInputStream, i_ui32DataLength, ui32PagePointer, bEndOfStream);
	_SetRST_Reg( _g_pstHeader->ui16ResetInterval );
	bUseUserHuffmanTable = _Decode_SetHuffmanTable_Reg( _g_pstHeader->stHuffmanTables );
	_Decode_SetQTable_Reg( &_g_pstHeader->stFrameHeader, _g_pstHeader->stQuantiztionTables );
	_Decode_SetMain_Reg( _g_pstHeader, _g_pstImageInfo, bUseUserHuffmanTable, _g_pstOption );
	_Decode_SetDPCM_Reg();
	_Decode_SetGBU_Reg( ui32PagePointer, ui32WordPointer, ui32ReadPointer );

	// Recalculate output image
	bSwap = ((_g_pstOption->eRotation == LX_MJPEG_ROTATE_90)||(_g_pstOption->eRotation == LX_MJPEG_ROTATE_270))? TRUE : FALSE;
	ui16Temp = _g_pstImageInfo->ui16Width >> _g_pstOption->eScaleHorizontal;
	_g_pstImageInfo->ui16Width	= (bSwap)? _g_pstImageInfo->ui16Height >>_g_pstOption->eScaleVertical : ui16Temp;
	_g_pstImageInfo->ui16Height = (bSwap)? ui16Temp : _g_pstImageInfo->ui16Height>>_g_pstOption->eScaleVertical;

	if( bSwap == TRUE )
	{
		_g_pstImageInfo->eFormat =	(_g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_422)? LX_MJPEG_FORMAT_224 :
									(_g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_224)? LX_MJPEG_FORMAT_422 : _g_pstImageInfo->eFormat;
	}

	// output buffer information init
#if 0
	ui8DividerX = (_g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_420 || _g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_422 ||
				   _g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_400) ? 2 : 1;
	ui8DividerY = (_g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_420 || _g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_224 ||
				   _g_pstImageInfo->eFormat == LX_MJPEG_FORMAT_400) ? 2 : 1;

	_g_pstImageInfo->ui32StrideY= _g_pstImageInfo->ui16Width;
	_g_pstImageInfo->ui32StrideC = _g_pstImageInfo->ui32StrideY / ui8DividerX;

	_g_pstImageInfo->ui32OffsetY	= 0;
	_g_pstImageInfo->ui32OffsetCb	= _g_pstImageInfo->ui32OffsetY + _g_pstImageInfo->ui32StrideY * _g_pstImageInfo->ui16Height;
	_g_pstImageInfo->ui32OffsetCr = _g_pstImageInfo->ui32OffsetCb;

	if (_g_pstImageInfo->eFormat != LX_MJPEG_FORMAT_400)
	{
		_g_pstImageInfo->ui32OffsetCr += ( (_g_pstImageInfo->ui32StrideY/ui8DividerX) * (_g_pstImageInfo->ui16Height/ui8DividerY) );
	}
#endif
	_SetDPB_Reg( g_pstImageBuffer );

	_Start_Reg();

	MJPEG_INFO("Start decoding.\n");
}

/**
 * Continue partial decoding
 *
 */
void	MJPEG_DEC_PartialContinue( UINT32 u32PartialBuffer )
{
	//ULONG flags;

	MJPEG_LOG_TRACE();

	MJPEG_RdFL( pic_status );
	MJPEG_RdFL( intr_mask );

	if ( u32PartialBuffer & LX_MJPEG_RESULT_BUFFER0 )
	{
		MJPEG_Wr01( pic_status, partial_buffer0_status, 1 );
		MJPEG_Wr01( intr_mask, partial_buffer0, 0 );
	}

	if ( u32PartialBuffer & LX_MJPEG_RESULT_BUFFER1 )
	{
		MJPEG_Wr01( pic_status, partial_buffer1_status, 1 );
		MJPEG_Wr01( intr_mask, partial_buffer1, 0 );
	}

	if ( u32PartialBuffer & LX_MJPEG_RESULT_BUFFER2 )
	{
		MJPEG_Wr01( pic_status, partial_buffer2_status, 1 );
		MJPEG_Wr01( intr_mask, partial_buffer2, 0 );
	}

	if ( u32PartialBuffer & LX_MJPEG_RESULT_BUFFER3 )
	{
		MJPEG_Wr01( pic_status, partial_buffer3_status, 1 );
		MJPEG_Wr01( intr_mask, partial_buffer3, 0 );
	}

	MJPEG_WrFL( intr_mask );
	MJPEG_WrFL( pic_status );
	//MJPEG_WrFL( intr_mask );

}

/**
 * Continue decoding
 *
 */
void	MJPEG_DEC_Continue(LX_MEM_BUF_T *i_pstDataBuf, UINT32 i_ui32DataLength, BOOLEAN bEndOfStream)
{
	LX_MEM_STREAM_T stInputStream;
	UINT32 ui32PagePointer, ui32EndPageAddress, ui32WritePoint;
//	BOOLEAN bEndOfStream;

	MJPEG_LOG_TRACE();

	stInputStream.mem_buf.addr = LX_CALC_ALIGNED_VALUE( i_pstDataBuf->addr - 3, 2 );
	stInputStream.mem_buf.length = i_pstDataBuf->length;
	stInputStream.curr_offset = i_pstDataBuf->addr - stInputStream.mem_buf.addr;

	ui32PagePointer = stInputStream.curr_offset/256;

	ui32EndPageAddress = (i_pstDataBuf->addr + i_pstDataBuf->length)/256
								+ ( ((i_pstDataBuf->addr + i_pstDataBuf->length)%256)? 1 : 0 );

//	bEndOfStream = ( i_pstDataBuf->length < stInputStream.mem_buf.length )? TRUE : FALSE;
	ui32WritePoint = i_pstDataBuf->addr + i_ui32DataLength;

	if ( ui32WritePoint == stInputStream.mem_buf.addr + stInputStream.mem_buf.length )
		ui32WritePoint = stInputStream.mem_buf.addr;

	MJPEG_RdFL( bbc_strm_ctrl );
	MJPEG_Wr02( bbc_strm_ctrl,
				endofstream, bEndOfStream,
				stream_count, ui32EndPageAddress );
	MJPEG_WrFL( bbc_strm_ctrl );

	// Set write pointer
	MJPEG_Wr  ( bbc_wr_ptr, ui32WritePoint);
	MJPEG_WrFL( bbc_wr_ptr );

	// Base and End address of bit-stream buffer
	MJPEG_Wr  ( bbc_bas_addr, stInputStream.mem_buf.addr );
//#ifdef MJPEG_ENDOFSTREAM
	if (!bEndOfStream)
	{
		MJPEG_Wr  ( bbc_endaddr, stInputStream.mem_buf.addr + stInputStream.mem_buf.length);
	}
	else
	{
		MJPEG_Wr  ( bbc_endaddr, stInputStream.mem_buf.addr + stInputStream.mem_buf.length + 1);
	}
//#else
//	MJPEG_Wr  ( bbc_endaddr, stInputStream.mem_buf.addr + stInputStream.mem_buf.length);
//#endif

	MJPEG_WrFL( bbc_bas_addr );
	MJPEG_WrFL( bbc_endaddr );

	MJPEG_RdFL( bbc_cur_pos );
	MJPEG_Wr01( bbc_cur_pos, bbc_cur_pos, ui32PagePointer );
	MJPEG_WrFL( bbc_cur_pos );
}

/*
 * Calculate Quantization table by Q value.
 */
static int
_Encode_CreateQuantTable( LX_MJPEG_QUANTIZATION_TABLE_T *pstQTable, int q )
{
	int i = 0;
	int s = 0;
	
	if ( pstQTable == NULL )
	{
		return RET_ERROR;
	}

	if ( q <= 0 || q > 100 )
	{
		return RET_ERROR;
	}

	s = (q < 50) ? (5000 / q) : (200 - 2 * q);

	pstQTable[0].ui8Pq = _g_BaseQuantTables[0].ui8Pq;
	pstQTable[0].ui8Tq = _g_BaseQuantTables[0].ui8Tq;
	pstQTable[1].ui8Pq = _g_BaseQuantTables[1].ui8Pq;
	pstQTable[1].ui8Tq = _g_BaseQuantTables[1].ui8Tq;

	for ( i = 0; i < 64; i++ )
	{
		int l = QUANT_ELEMENT( s, _g_BaseQuantTables[0].ui8Elements[i] );
		int c = QUANT_ELEMENT( s, _g_BaseQuantTables[1].ui8Elements[i] );

		if ( l == 0 )	l = 1;
		if ( c == 0 )	c = 1;
		
#if (PRESET_SAMPLING_PRECISION == 8)
		if ( l > 0xFF )	l = 0xFF;
		if ( c > 0xFF ) c = 0xFF;
#endif

		pstQTable[0].ui8Elements[i] = l;
		pstQTable[1].ui8Elements[i] = c;
	}

	return RET_OK;
}


/**
 * Initialize device
 *
 */
static void
_Initialize_Reg(void)
{
	if ( lx_chip_rev() > LX_CHIP_REV( H13, B0 ) )
	{
		MJPEG_RdFL( pic_start );
		MJPEG_Wr01( pic_start, init_status, 1 );
		MJPEG_WrFL( pic_start );
	}
	else
	{
		MJPEG_Wr  ( pic_start, 0 );
		MJPEG_WrFL( pic_start );

		MJPEG_Wr04( pic_status,
					jpeg_overflow,	1,		// JPEG_DONE
					bbc_interrupt,	1,		// JPEG_ERROR
					jpeg_error,		1,		// BBC_INTERRUPT
					jpeg_done,	 	1 );	// JPEG_OVERFLOW
		MJPEG_WrFL( pic_status );
		MJPEG_RdFL( pic_status );
	}
}

/**
 * Initialize DPB register for encoding and decoding
 *
 */
static void
_SetDPB_Reg(LX_MJPEG_IMAGE_BUF_T *i_pstImageBuffer)
{
	MJPEG_RdFL( dpb_config );
	MJPEG_Wr02( dpb_config,
				endianess, FRAME_ENDIAN,
				uv_interleave_mode, i_pstImageBuffer->bUVInterleaved);
	MJPEG_WrFL( dpb_config );

	MJPEG_Wr  ( dpb_base00, i_pstImageBuffer->ui32AddressY );
	MJPEG_Wr  ( dpb_base01, i_pstImageBuffer->ui32AddressCb );
	MJPEG_Wr  ( dpb_base02, i_pstImageBuffer->ui32AddressCr );
	MJPEG_WrFL( dpb_base00 );
	MJPEG_WrFL( dpb_base01 );
	MJPEG_WrFL( dpb_base02 );

	MJPEG_RdFL( dpb_ystride );
	MJPEG_RdFL( dpb_cstride );
	MJPEG_Wr01( dpb_ystride, dpb_ystride, i_pstImageBuffer->ui32StrideY );
	MJPEG_Wr01( dpb_cstride, dpb_cstride, i_pstImageBuffer->ui32StrideC );
	MJPEG_WrFL( dpb_ystride );
	MJPEG_WrFL( dpb_cstride );
}

/**
 * Initialize RST register for encoding and decoding
 *
 */
static void
_SetRST_Reg(UINT16 i_ui16RstInterval)
{
	// Set reset interval
	MJPEG_Wr  ( rst_interval, i_ui16RstInterval );
	MJPEG_WrFL( rst_interval );

	// Init counter
	MJPEG_RdFL( rst_index );
	MJPEG_RdFL( rst_count );
	MJPEG_Wr01( rst_index, rst_index, 0 );
	MJPEG_Wr01( rst_count, rst_count, 0 );
	MJPEG_WrFL( rst_index );
	MJPEG_WrFL( rst_count );
}

/**
 * Start encoding or decoding
 *
 */
static void
_Start_Reg(void)
{
	MJPEG_RdFL( pic_start );
	MJPEG_Wr01( pic_start, pic_start, 1 );
	MJPEG_WrFL( pic_start );
}

/**
 * Initialize BBC register for encoding
 *
 */
static void
_Encode_SetBBC_Reg(LX_MEM_BUF_T *i_pstOutputBuffer)
{
	MJPEG_RdFL(bbc_cur_pos);
	MJPEG_RdFL(bbc_data_cnt);
	MJPEG_RdFL(bbc_int_addr);
	MJPEG_RdFL(bbc_ctrl);

	MJPEG_Wr  ( bbc_endaddr, i_pstOutputBuffer->addr + i_pstOutputBuffer->length );

	// Set read pointer
	MJPEG_Wr  ( bbc_rd_ptr, i_pstOutputBuffer->addr );
	MJPEG_Wr  ( bbc_wr_ptr, i_pstOutputBuffer->addr );

	MJPEG_Wr01( bbc_cur_pos, bbc_cur_pos, 0 );
	MJPEG_Wr01( bbc_data_cnt, bbc_data_cnt, 256/4 );
	MJPEG_Wr  ( bbc_ext_addr, i_pstOutputBuffer->addr );
	MJPEG_Wr01( bbc_int_addr, bbc_int_addr, 0 );
	MJPEG_Wr  ( bbc_bas_addr, i_pstOutputBuffer->addr );
	MJPEG_Wr02( bbc_ctrl,
				auto_run, 1,
				endianess, STREAM_ENDIAN );

	MJPEG_WrFL( bbc_endaddr );
	MJPEG_WrFL( bbc_rd_ptr );
	MJPEG_WrFL( bbc_wr_ptr );
	MJPEG_WrFL( bbc_cur_pos );
	MJPEG_WrFL( bbc_data_cnt );
	MJPEG_WrFL( bbc_ctrl );
	MJPEG_WrFL( bbc_ext_addr );
	MJPEG_WrFL( bbc_int_addr );
	MJPEG_WrFL( bbc_bas_addr );
	MJPEG_WrFL( bbc_ctrl );
}

/**
 * Initialize GBU register for encoding
 *
 */
static void
_Encode_SetGBU_Reg(void)
{
	// Set bit pointer
	MJPEG_Wr  ( gbu_bt_ptr, 0 );
	MJPEG_WrFL( gbu_bt_ptr );

	// Set word pointer
	MJPEG_RdFL( gbu_wd_ptr );
	MJPEG_Wr01( gbu_wd_ptr, gbu_wd_ptr, 0 );
	MJPEG_WrFL( gbu_wd_ptr );

	// Base and End address of GRAM
	MJPEG_RdFL( gbu_bbsr );
	MJPEG_RdFL( gbu_bber );
	MJPEG_Wr01( gbu_bbsr, gbu_bbsr, 0 );
	MJPEG_Wr01( gbu_bber, gbu_bber, 512/4 - 1 );
	MJPEG_WrFL( gbu_bbsr );
	MJPEG_WrFL( gbu_bber );

	// Interrupt pointer of GRAM
	MJPEG_RdFL( gbu_bbir );
	MJPEG_RdFL( gbu_bbhr );
	MJPEG_Wr01( gbu_bbir, gbu_bbir, 256/4 );
	MJPEG_Wr01( gbu_bbhr, gbu_bbhr, 256/4 );
	MJPEG_WrFL( gbu_bbir );
	MJPEG_WrFL( gbu_bbhr );
}

/**
 * Initialize main register for decoding
 *
 */
static void
_Encode_SetMain_Reg(LX_MJPEG_HEADER_T *i_pstHeader, LX_MJPEG_IMAGE_INFO_T *pstImageInfo, LX_MJPEG_OPTION_T *i_pstOption)
{
	LX_MJPEG_FORMAT_T eFormat = pstImageInfo->eFormat;
	BOOLEAN bMirrorRotation = FALSE;

	MJPEG_RdFL( op_info );
	MJPEG_RdFL( mcu_info );
	MJPEG_Wr03( op_info,
				number_of_request, _g_ui8ConfigPreset[eFormat][0],
				number_of_partial_buffer, 0,
				number_of_line_in_partial_buffer, 0 );
	MJPEG_Wr05( mcu_info,
				sampling_factor_2, _g_ui8ConfigPreset[eFormat][5],
				sampling_factor_1, _g_ui8ConfigPreset[eFormat][4],
				sampling_factor_0, _g_ui8ConfigPreset[eFormat][3],
				number_of_components, _g_ui8ConfigPreset[eFormat][2],
				total_block_number, _g_ui8ConfigPreset[eFormat][1] );
	MJPEG_WrFL( op_info );
	MJPEG_WrFL( mcu_info );

	if ( (i_pstOption->eMirroring != LX_MJPEG_MIRROR_NONE) || (i_pstOption->eRotation != LX_MJPEG_ROTATE_0) )
	{
		bMirrorRotation = TRUE;
	}

	MJPEG_RdFL( pic_ctrl );
	MJPEG_RdFL( pic_size );
	MJPEG_RdFL( rot_info );
	MJPEG_RdFL( scl_info );

	MJPEG_Wr07( pic_ctrl,
				partial_mode, 0,
				raw_image, 0,
				encoder_mode, 1,
				tc_direction, 1,
				user_huffman, 0,
				dc_huffman_index, 0,
				ac_huffman_index, 0 );
	MJPEG_Wr02( pic_size,
				vertical, pstImageInfo->ui16Height,
				horizontal, pstImageInfo->ui16Width );
	MJPEG_Wr04( rot_info,
				rotation_degree, i_pstOption->eRotation,
				vertical_mirroring, ((i_pstOption->eMirroring&LX_MJPEG_MIRROR_VERTICAL)? 1 : 0),
				horitontal_mirroring, ((i_pstOption->eMirroring&LX_MJPEG_MIRROR_HORIZONTAL)? 1 : 0),
				rotation_mirroring_info, bMirrorRotation );
	MJPEG_Wr03( scl_info,
				vertical_scale, 0,
				horizontal_scale, 0,
				scale_enable, 0 );

	MJPEG_WrFL( pic_ctrl );
	MJPEG_WrFL( pic_size );
	MJPEG_WrFL( rot_info );
	MJPEG_WrFL( scl_info );
}

/**
 * Set Huffman Table for encoding
 *
 */
static BOOLEAN
_Encode_SetHuffmanTable_Reg(LX_MJPEG_HUFFMAN_TABLE_T *i_pstHuffman)
{
	UINT8 *pui8Values, *pui8Bits;
	SINT32 iTableNumber, iCount;

	MJPEG_RdFL( huff_ctrl );
	MJPEG_Wr03( huff_ctrl, table_destination, 0,
						   ctrl_auto, ENABLE,
						   mode, ENABLE );
	MJPEG_WrFL( huff_ctrl );

	// Generate Huffman table
	for( iTableNumber = 0 ; iTableNumber < 4 ; ++iTableNumber )
	{
		SINT32 i32BitsCount, i32CurrentBits, i32MaxSymbols, i32TotalBitsCount;
		UINT16 ui16HuffCode, ui16HuffSize;
		UINT8 ui8TableNum = (iTableNumber==0)? HUFF_AC_INDEX0 :
							(iTableNumber==1)? HUFF_AC_INDEX1 :
							(iTableNumber==2)? HUFF_DC_INDEX0 : HUFF_DC_INDEX1;

		pui8Values	= i_pstHuffman[ui8TableNum].ui8CodeValues;
		pui8Bits	= i_pstHuffman[ui8TableNum].ui8CodeLengths;

		memset( (void *)&_g_ui16HuffSizes, 0, sizeof(UINT16)*256 );
		memset( (void *)&_g_ui16HuffCodes, 0, sizeof(UINT16)*256 );

		/* make table of Huffman code length for each symbol */
		i32MaxSymbols = ui8TableNum & 1 ? 256 : 16;

		i32BitsCount = 0;
		for( iCount = 1 ; iCount <= 16 ; ++iCount )
		{
			i32CurrentBits = pui8Bits[iCount-1];
			if ( i32CurrentBits < 0 || i32BitsCount + i32CurrentBits > i32MaxSymbols )
			{
				return FALSE;
			}

			while ( i32CurrentBits-- ) { _g_ui16HuffSizes[i32BitsCount++] = iCount; }
		}
		i32TotalBitsCount = i32BitsCount;

		/* generate the codes themselves */
		/* We also validate that the counts represent a legal Huffman code tree. */
		ui16HuffCode = 0;
		ui16HuffSize = _g_ui16HuffSizes[0];
		i32BitsCount = 0;
		while ( i32BitsCount < i32TotalBitsCount )
		{
			while ( _g_ui16HuffSizes[i32BitsCount] == ui16HuffSize )
			{
				_g_ui16HuffCodes[i32BitsCount++] = ui16HuffCode;
				ui16HuffCode++;
			}

			if ( ui16HuffCode >= (1 << ui16HuffSize) ) { return FALSE; }

			ui16HuffCode <<= 1;
			++ui16HuffSize;
		}

		/* Figure C.3: generate encoding tables */
		/* These are code and size indexed by symbol value */

		memset( (void *)&_g_ui16EncHuffSizes, 0, sizeof(UINT16)*256 );
		memset( (void *)&_g_ui16EncHuffCodes, 0, sizeof(UINT16)*256 );

		for ( i32BitsCount = 0 ; i32BitsCount < i32TotalBitsCount ; ++i32BitsCount ) {
			i32CurrentBits = pui8Values[i32BitsCount];
			if ( i32CurrentBits < 0 || i32CurrentBits >= i32MaxSymbols
				 || _g_ui16EncHuffSizes[i32CurrentBits] )
			{
				return FALSE;
			}
			_g_ui16EncHuffCodes[i32CurrentBits] = _g_ui16HuffCodes[i32BitsCount];
			_g_ui16EncHuffSizes[i32CurrentBits] = _g_ui16HuffSizes[i32BitsCount];
		}

		for ( iCount = 0 ; iCount < 256 ; ++iCount )
		{
			UINT32 ui32HuffData;
			if ( (ui8TableNum == HUFF_DC_INDEX0 || ui8TableNum == HUFF_DC_INDEX1)
				 && (iCount > 15) )
			{ break; }

			if ( (_g_ui16EncHuffCodes[iCount]) == 0 && (_g_ui16EncHuffSizes[iCount]== 0) )
			{
				ui32HuffData = 0;
			}
			else
			{
				ui32HuffData = _g_ui16EncHuffSizes[iCount] - 1;
				ui32HuffData = (ui32HuffData << 16) | _g_ui16EncHuffCodes[iCount];
			}
			MJPEG_Wr( huff_data, ui32HuffData );
			MJPEG_WrFL( huff_data );
		}

	}

	MJPEG_Wr03( huff_ctrl, table_destination, 0,
						   ctrl_auto, DISABLE,
						   mode, DISABLE );
	MJPEG_WrFL( huff_ctrl );

	return TRUE;
}

/**
 * Set Quantization Table for encoding
 *
 */
static void
_Encode_SetQTable_Reg(LX_MJPEG_FRAME_HEADER_T *i_pstFrameInfo, LX_MJPEG_QUANTIZATION_TABLE_T *i_pstQTable)
{
	SINT32 iCompID, iCount;

	MJPEG_RdFL( qmat_ctrl );

	for( iCompID = 0 ; iCompID < i_pstFrameInfo->ui8NumberOfComponents ; ++iCompID )
	{
		UINT8 ui8QTableID = i_pstFrameInfo->stComponents[iCompID].ui8QTableID;

		MJPEG_Wr03( qmat_ctrl, table_destination, iCompID,
							   ctrl_auto, ENABLE,
							   mode, ENABLE );
		MJPEG_WrFL( qmat_ctrl );

		for( iCount = 0 ; iCount < 64 ; ++iCount )
		{
			UINT32 i32Dividend = 0x80000, i32Quotient;
			UINT8 i8Divisor = i_pstQTable[ui8QTableID].ui8Elements[iCount];
			if ( i8Divisor == 0 ) { i8Divisor = 1; }
			i32Quotient = i32Dividend / i8Divisor;
			MJPEG_Wr( qmat_data, i32Quotient );
			MJPEG_WrFL( qmat_data );
		}
		MJPEG_Wr03( qmat_ctrl, table_destination, iCompID,
							   ctrl_auto, DISABLE,
							   mode, DISABLE );
		MJPEG_WrFL( qmat_ctrl );
	}
}

/**
 * Initialize BBC register for decoding
 *
 */
static void
_Decode_SetBBC_Reg(LX_MEM_STREAM_T *i_pstInputStream, UINT32 i_i32Size, UINT32 i_ui32PagePointer, BOOLEAN bEndOfStream)
{
	BOOLEAN bBusy;
	UINT32 ui32BitStreamStartAddress = i_pstInputStream->mem_buf.addr + i_pstInputStream->curr_offset;
	UINT32 ui32EndPageAddress = (ui32BitStreamStartAddress + i_i32Size)/256 + ( ((ui32BitStreamStartAddress + i_i32Size)%256)? 1 : 0 );
//	BOOLEAN bEndOfStream = ( i_pstInputStream->curr_offset + i_i32Size < i_pstInputStream->mem_buf.length )? TRUE : FALSE;
	UINT32 ui32WritePoint = i_pstInputStream->mem_buf.addr + i_pstInputStream->curr_offset + i_i32Size;

	if ( ui32WritePoint == i_pstInputStream->mem_buf.addr + i_pstInputStream->mem_buf.length )
		ui32WritePoint = i_pstInputStream->mem_buf.addr;

	MJPEG_RdFL( bbc_strm_ctrl );
	MJPEG_Wr02( bbc_strm_ctrl,
				endofstream, bEndOfStream,
				stream_count, ui32EndPageAddress );
	MJPEG_WrFL( bbc_strm_ctrl );

	// Set read pointer
	MJPEG_Wr  ( bbc_rd_ptr, i_pstInputStream->mem_buf.addr+i_pstInputStream->curr_offset);
	MJPEG_WrFL( bbc_rd_ptr );
	// Set write pointer
	MJPEG_Wr  ( bbc_wr_ptr, ui32WritePoint);
	MJPEG_WrFL( bbc_wr_ptr );

	// Base and End address of bit-stream buffer
	MJPEG_Wr  ( bbc_bas_addr, (UINT32)i_pstInputStream->mem_buf.addr );

//#ifdef MJPEG_ENDOFSTREAM
	//	jaeseop.so (2011/10/28): scan data 사이즈가 512 align이 되는경우 BBC Interrupt 발생하고
	//	decoding 진행되지 않는 문제 해결하기 위해서 bbc_endaddr + 1 하도록 임시 조치
	//	In reference s/w
	//		bbc_enaddr의 경우 전체 buffer의 endaddr로 설정 (GP 드라이버 코드와의 차이점임)
	//		bbc_strm_ctrl의 값으로 data크기 및 eof를 설정
	//  (H/W Buf로 추정)
	if (!bEndOfStream)
	{
		MJPEG_Wr  ( bbc_endaddr, (UINT32)i_pstInputStream->mem_buf.addr+i_pstInputStream->mem_buf.length );
	}
	else
	{
		MJPEG_Wr  ( bbc_endaddr, (UINT32)i_pstInputStream->mem_buf.addr + i_pstInputStream->mem_buf.length + 1 );
	}
//#else
//		MJPEG_Wr  ( bbc_endaddr, (UINT32)i_pstInputStream->mem_buf.addr+i_pstInputStream->mem_buf.length );
//#endif

	MJPEG_WrFL( bbc_bas_addr );
	MJPEG_WrFL( bbc_endaddr );

	// Copy to GRAM first page
	MJPEG_RdFL( bbc_cur_pos );
	MJPEG_RdFL( bbc_int_addr );
	MJPEG_RdFL( bbc_data_cnt );
	MJPEG_RdFL( bbc_command );
	MJPEG_Wr01( bbc_cur_pos, bbc_cur_pos, i_ui32PagePointer );
	MJPEG_Wr  ( bbc_ext_addr, (UINT32)(ui32BitStreamStartAddress/256)*256);
	MJPEG_Wr01( bbc_int_addr, bbc_int_addr, (i_ui32PagePointer&1) << 6 );
	MJPEG_Wr01( bbc_data_cnt, bbc_data_cnt, 256/4 );
	MJPEG_Wr02( bbc_command,
				endianess, STREAM_ENDIAN,
				command, 0);
	MJPEG_WrFL( bbc_cur_pos );
	MJPEG_WrFL( bbc_ext_addr );
	MJPEG_WrFL( bbc_int_addr );
	MJPEG_WrFL( bbc_data_cnt );
	MJPEG_WrFL( bbc_command );

	// Wait
	do
	{
		MJPEG_RdFL( bbc_busy );
		MJPEG_Rd01( bbc_busy, bbc_busy, bBusy);
	}while( bBusy == 1 );

	// Copy to GRAM second page
	MJPEG_Wr01( bbc_cur_pos, bbc_cur_pos, ++i_ui32PagePointer );
	MJPEG_Wr  ( bbc_ext_addr, (UINT32)(ui32BitStreamStartAddress/256+1)*256 );
	MJPEG_Wr01( bbc_int_addr, bbc_int_addr, (i_ui32PagePointer&1) << 6 );
	MJPEG_Wr01( bbc_data_cnt, bbc_data_cnt, 256/4 );
	MJPEG_Wr02( bbc_command,
				endianess, STREAM_ENDIAN,
				command, 0 );
	MJPEG_WrFL( bbc_cur_pos );
	MJPEG_WrFL( bbc_ext_addr );
	MJPEG_WrFL( bbc_int_addr );
	MJPEG_WrFL( bbc_data_cnt );
	MJPEG_WrFL( bbc_command );

	// Wait
	do
	{
		MJPEG_RdFL( bbc_busy );
		MJPEG_Rd01( bbc_busy, bbc_busy, bBusy);
	}while( bBusy == 1 );

	// Set next page
	MJPEG_Wr01( bbc_cur_pos, bbc_cur_pos, ++i_ui32PagePointer );
	MJPEG_WrFL( bbc_cur_pos );

	// Set BBC control
	MJPEG_RdFL( bbc_ctrl );
	MJPEG_Wr02( bbc_ctrl,
				auto_run, 1,
				endianess, STREAM_ENDIAN );
	MJPEG_WrFL( bbc_ctrl );
}

/**
 * Initialize GBU register for decoding
 *
 */
static void
_Decode_SetGBU_Reg(UINT32 i_ui32PagePointer, UINT32 i_ui32WordPointer, UINT32 i_ui32ReadPointer)
{
	// Base and End address of GRAM
	MJPEG_RdFL( gbu_bbsr );
	MJPEG_RdFL( gbu_bber );
	MJPEG_Wr01( gbu_bbsr, gbu_bbsr, 0 );
	MJPEG_Wr01( gbu_bber, gbu_bber, 512/4 - 1 );
	MJPEG_WrFL( gbu_bbsr );
	MJPEG_WrFL( gbu_bber );

	// Interrupt pointer of GRAM
	MJPEG_RdFL( gbu_bbir );
	MJPEG_RdFL( gbu_bbhr );
	MJPEG_Wr01( gbu_bbir, gbu_bbir, ((i_ui32PagePointer&1)? 0 : 256/4) );
	MJPEG_Wr01( gbu_bbhr, gbu_bbhr, ((i_ui32PagePointer&1)? 0 : 256/4) );
	MJPEG_WrFL( gbu_bbir );
	MJPEG_WrFL( gbu_bbhr );

	// Set word pointer
	MJPEG_RdFL( gbu_wd_ptr );
	MJPEG_Wr01( gbu_wd_ptr, gbu_wd_ptr, i_ui32WordPointer );
	MJPEG_WrFL( gbu_wd_ptr );

	// Initialize GBU
	MJPEG_Wr01( gbu_ctrl, initialize_gbu, 1 );
	MJPEG_WrFL( gbu_ctrl );

	// Set pointer of MCU
	MJPEG_RdFL( gbu_ff_rptr );
	MJPEG_Wr01( gbu_ff_rptr, gbu_ff_rptr, i_ui32ReadPointer );
	MJPEG_WrFL( gbu_ff_rptr );

	// Initiailize GBU FF emulation module
	//MJPEG_Wr01( gbu_ctrl, initialize_ff_emulation_modulef, 1 );
	MJPEG_Wr( gbu_ctrl, 3 );		// must have....
	MJPEG_WrFL( gbu_ctrl );
}

/**
 * Initialize main register for decoding
 *
 */
static void
_Decode_SetMain_Reg(LX_MJPEG_HEADER_T *i_pstHeader, LX_MJPEG_IMAGE_INFO_T *i_pstImageInfo, BOOLEAN i_bUseUserHuff, LX_MJPEG_OPTION_T *i_pstOption)
{
	LX_MJPEG_SCAN_HEADER_T *pstScanInfo = &i_pstHeader->stScanHeader;
	SINT32 iCount;
	LX_MJPEG_FORMAT_T eFormat = i_pstImageInfo->eFormat;
	UINT8 HuffDCIndex = 0, HuffACIndex = 0;
	BOOLEAN bMirrorRotation = FALSE, bScale = FALSE;

	MJPEG_RdFL( op_info );
	MJPEG_RdFL( mcu_info );

	if ( i_pstOption->eOperation == LX_MJPEG_OPERATION_PARTIAL )
	{
		MJPEG_Wr03( op_info,
					number_of_request, _g_ui8ConfigPreset[eFormat][0],
					number_of_partial_buffer, 1,
					number_of_line_in_partial_buffer, 16);
	}
	else
	{
		MJPEG_Wr03( op_info,
					number_of_request, _g_ui8ConfigPreset[eFormat][0],
					number_of_partial_buffer, 0,
					number_of_line_in_partial_buffer, 0 );
	}
	
	MJPEG_Wr05( mcu_info,
				sampling_factor_2, _g_ui8ConfigPreset[eFormat][5],
				sampling_factor_1, _g_ui8ConfigPreset[eFormat][4],
				sampling_factor_0, _g_ui8ConfigPreset[eFormat][3],
				number_of_components, _g_ui8ConfigPreset[eFormat][2],
				total_block_number, _g_ui8ConfigPreset[eFormat][1] );
	MJPEG_WrFL( op_info );
	MJPEG_WrFL( mcu_info );

	for( iCount = 0 ; iCount < pstScanInfo->ui8NumberOfComponents ; ++iCount )
	{
		HuffDCIndex = HuffDCIndex << 1 | pstScanInfo->stComponents[iCount].ui8DC_HuffmanTableID;
		HuffACIndex = HuffACIndex << 1 | pstScanInfo->stComponents[iCount].ui8AC_HuffmanTableID;
	}

	if ( (i_pstOption->eMirroring != LX_MJPEG_MIRROR_NONE) || (i_pstOption->eRotation != LX_MJPEG_ROTATE_0) )
	{
		bMirrorRotation = TRUE;
	}

	if ( (i_pstOption->eScaleHorizontal != LX_MJPEG_SCALE_NONE) || (i_pstOption->eScaleVertical != LX_MJPEG_SCALE_NONE) )
	{
		bScale = TRUE;
	}

	MJPEG_RdFL( pic_ctrl );
	MJPEG_RdFL( pic_size );
	MJPEG_RdFL( rot_info );
	MJPEG_RdFL( scl_info );

	MJPEG_Wr07( pic_ctrl,
				partial_mode, i_pstOption->eOperation,
				raw_image, 0,
				encoder_mode, 0,
				tc_direction, 0,
				user_huffman, i_bUseUserHuff,
				dc_huffman_index, HuffDCIndex,
				ac_huffman_index, HuffACIndex );
	MJPEG_Wr02( pic_size,
				vertical, i_pstImageInfo->ui16Height,
				horizontal, i_pstImageInfo->ui16Width );
	MJPEG_Wr04( rot_info,
				rotation_degree, i_pstOption->eRotation,
				vertical_mirroring, ((i_pstOption->eMirroring&LX_MJPEG_MIRROR_VERTICAL)? 1 : 0),
				horitontal_mirroring, ((i_pstOption->eMirroring&LX_MJPEG_MIRROR_HORIZONTAL)? 1 : 0),
				rotation_mirroring_info, bMirrorRotation );
	MJPEG_Wr03( scl_info,
				vertical_scale, i_pstOption->eScaleVertical,
				horizontal_scale, i_pstOption->eScaleHorizontal,
				scale_enable, bScale );

	MJPEG_WrFL( pic_ctrl );
	MJPEG_WrFL( pic_size );
	MJPEG_WrFL( rot_info );
	MJPEG_WrFL( scl_info );
}

/**
 * Initialize DPCM registers for decoding
 *
 */
static void
_Decode_SetDPCM_Reg(void)
{
	MJPEG_RdFL( dpcm_diff_y );
	MJPEG_RdFL( dpcm_diff_cb );
	MJPEG_RdFL( dpcm_diff_cr );

	MJPEG_Wr01( dpcm_diff_y,
				dpcm_diff_y, 0 );
	MJPEG_Wr01( dpcm_diff_cb,
				dpcm_diff_cb, 0 );
	MJPEG_Wr01( dpcm_diff_cr,
				dpcm_diff_cr, 0 );

	MJPEG_RdFL( dpcm_diff_y );
	MJPEG_RdFL( dpcm_diff_cb );
	MJPEG_RdFL( dpcm_diff_cr );
}

/**
 * Set Huffman Table for decoding
 *
 */
static BOOLEAN
_Decode_SetHuffmanTable_Reg(LX_MJPEG_HUFFMAN_TABLE_T *i_pstHuffman)
{
	BOOLEAN bUseUserHuffman = FALSE;
	UINT8 Ptr[4][16];
	UINT16 Min[4][16], Max[4][16];
	UINT8 *Value, *Bits;
	SINT32 iTableNumber, iCount;
	SINT32 HuffSize;

	// Generate Huffman table
	for( iTableNumber = 0 ; iTableNumber < 4 ; ++iTableNumber )
	{
		SINT32 PtrCnt = 0;
		UINT16 HuffCode = 0;
		SINT32 ZeroFlag = 0;
		SINT32 DataFlag = 0;

		Value 	= i_pstHuffman[iTableNumber].ui8CodeValues;
		Bits	= i_pstHuffman[iTableNumber].ui8CodeLengths;

		for( iCount = 0 ; iCount < 16 ; ++iCount )
		{
			if (Bits[iCount])
			{	// if there is bit cnt value
				Ptr[iTableNumber][iCount] = PtrCnt;
				PtrCnt += Bits[iCount];
				Min[iTableNumber][iCount] = HuffCode;
				Max[iTableNumber][iCount] = HuffCode + (Bits[iCount] - 1);
				DataFlag = 1;
				ZeroFlag = 0;
			} else {
				Ptr[iTableNumber][iCount] = 0xFF;
				Min[iTableNumber][iCount] = 0xFFFF;
				Max[iTableNumber][iCount] = 0xFFFF;
				ZeroFlag = 1;
			}

			if (DataFlag == 1) {
				if (ZeroFlag == 1)
					HuffCode <<= 1;
				else
					HuffCode = (Max[iTableNumber][iCount] + 1) << 1;
			}

			if( ( bUseUserHuffman == FALSE ) &&
				( Bits[iCount] != _g_ui8PresetHuffTables[0][iTableNumber].ui8CodeLengths[iCount] ||
				  Value[iCount] != _g_ui8PresetHuffTables[0][iTableNumber].ui8CodeValues[iCount] ) )
				bUseUserHuffman = TRUE;

		}

	}

	if ( bUseUserHuffman == FALSE )
		return bUseUserHuffman;

	MJPEG_RdFL( huff_ctrl );
	MJPEG_RdFL( huff_addr );

	// MIN Table
	MJPEG_Wr03( huff_ctrl, table_destination, HUFF_MIN_TABLE,
						   ctrl_auto, ENABLE,
						   mode, ENABLE );
	MJPEG_Wr02( huff_addr, table_destination, HUFF_MIN_TABLE,
						   base_addr, 0x000 );
	MJPEG_WrFL( huff_ctrl );
	MJPEG_WrFL( huff_addr );

	for( iTableNumber = 0 ; iTableNumber < 4 ; ++iTableNumber )
	{
		UINT8 ui8TableNum = (iTableNumber==0)? HUFF_DC_INDEX0 :
							(iTableNumber==1)? HUFF_DC_INDEX1 :
							(iTableNumber==2)? HUFF_AC_INDEX0 : HUFF_AC_INDEX1;

		for( iCount = 0 ; iCount < 16 ; ++iCount )
		{
			UINT16 temp = ( Min[ui8TableNum][iCount] & 0x8000 ) >> 15;
			temp = (temp << 15) | (temp << 14) | (temp << 13) | (temp << 12) | (temp << 11) | (temp << 10) | (temp << 9) | (temp << 8) | (temp << 7 ) | (temp << 6) | (temp <<5) | (temp<<4) | (temp<<3) | (temp<<2) | (temp<<1)| (temp);
			MJPEG_Wr( huff_data, ( (temp & 0xFFFF) << 16 ) | Min[ui8TableNum][iCount] );
			MJPEG_WrFL( huff_data );
		}
	}

	// MAX Table
	MJPEG_Wr03( huff_ctrl, table_destination, HUFF_MAX_TABLE,
						   ctrl_auto, ENABLE,
						   mode, ENABLE );
	MJPEG_Wr02( huff_addr, table_destination, HUFF_MAX_TABLE,
						   base_addr, 0x040 );
	MJPEG_WrFL( huff_ctrl );
	MJPEG_WrFL( huff_addr );

	for( iTableNumber = 0 ; iTableNumber < 4 ; ++iTableNumber )
	{
		UINT8 ui8TableNum = (iTableNumber==0)? HUFF_DC_INDEX0 :
							(iTableNumber==1)? HUFF_DC_INDEX1 :
							(iTableNumber==2)? HUFF_AC_INDEX0 : HUFF_AC_INDEX1;

		for( iCount = 0 ; iCount < 16 ; ++iCount )
		{
			UINT16 temp = ( Max[ui8TableNum][iCount] & 0x8000 ) >> 15;
			temp = (temp << 15) | (temp << 14) | (temp << 13) | (temp << 12) | (temp << 11) | (temp << 10) | (temp << 9) | (temp << 8) | (temp << 7 ) | (temp << 6) | (temp <<5) | (temp<<4) | (temp<<3) | (temp<<2) | (temp<<1)| (temp);
			MJPEG_Wr( huff_data, ( (temp & 0xFFFF) << 16 ) | Max[ui8TableNum][iCount] );
			MJPEG_WrFL( huff_data );
		}
	}

	// PTR Table
	MJPEG_Wr03( huff_ctrl, table_destination, HUFF_POINTER_TABLE,
						   ctrl_auto, ENABLE,
						   mode, ENABLE );
	MJPEG_Wr02( huff_addr, table_destination, HUFF_POINTER_TABLE,
						   base_addr, 0x080 );
	MJPEG_WrFL( huff_ctrl );
	MJPEG_WrFL( huff_addr );

	for( iTableNumber = 0 ; iTableNumber < 4 ; ++iTableNumber )
	{
		UINT8 ui8TableNum = (iTableNumber==0)? HUFF_DC_INDEX0 :
							(iTableNumber==1)? HUFF_DC_INDEX1 :
							(iTableNumber==2)? HUFF_AC_INDEX0 : HUFF_AC_INDEX1;

		for( iCount = 0 ; iCount < 16 ; ++iCount )
		{
			UINT32 temp = ( Ptr[ui8TableNum][iCount] & 0x80 ) >> 7;
			temp = (temp<<23)|(temp<<22)|(temp<<21)|(temp<<20)|(temp<<19)|(temp<<18)|(temp<<17)|(temp<<16)|(temp<<15)|(temp<<14)|(temp<<13)|(temp<<12)|(temp<<11)|(temp<<10)|(temp<<9)|(temp<<8)|(temp<<7)|(temp<<6)|(temp<<5)|(temp<<4)|(temp<<3)|(temp<<2)|(temp<<1)|(temp);
			MJPEG_Wr( huff_data, ( (temp & 0xFFFFFF) << 8 ) | Ptr[ui8TableNum][iCount] );
			MJPEG_WrFL( huff_data );
		}
	}

	// VAL Table
	MJPEG_Wr03( huff_ctrl, table_destination, HUFF_VALUE_TABLE,
						   ctrl_auto, ENABLE,
						   mode, ENABLE );
	MJPEG_WrFL( huff_ctrl );

	for( iTableNumber = 0 ; iTableNumber < 4 ; ++iTableNumber )
	{
		UINT8 ui8TableNum = (iTableNumber==0)? HUFF_DC_INDEX0 : // VAL DC luminance table
							(iTableNumber==1)? HUFF_DC_INDEX1 : // VAL DC Chroma
							(iTableNumber==2)? HUFF_AC_INDEX0 : HUFF_AC_INDEX1; // VAL AC Luma  // VAL AC Chroma
		UINT8 ui8Count = (iTableNumber<2)? 12 : 162;

		Value 	= i_pstHuffman[ui8TableNum].ui8CodeValues;
		Bits	= i_pstHuffman[ui8TableNum].ui8CodeLengths;
		HuffSize = 0;

		for( iCount = 0 ; iCount < 16 ; ++iCount )
		{
			HuffSize += Bits[iCount];
		}

		for( iCount = 0 ; iCount < HuffSize ; ++iCount )
		{
			UINT32 temp = (Value[iCount] & 0x80) >> 7;
			temp = (temp<<23)|(temp<<22)|(temp<<21)|(temp<<20)|(temp<<19)|(temp<<18)|(temp<<17)|(temp<<16)|(temp<<15)|(temp<<14)|(temp<<13)|(temp<<12)|(temp<<11)|(temp<<10)|(temp<<9)|(temp<<8)|(temp<<7)|(temp<<6)|(temp<<5)|(temp<<4)|(temp<<3)|(temp<<2)|(temp<<1)|(temp);
			MJPEG_Wr( huff_data, ( (temp & 0xFFFFFF) << 8 ) | Value[iCount] );
			MJPEG_WrFL( huff_data );
		}

		for( iCount = 0 ; iCount < ui8Count-HuffSize ; ++iCount )
		{
			MJPEG_Wr( huff_data, 0xFFFFFFFF );
			MJPEG_WrFL( huff_data );
		}
	}

	MJPEG_Wr03( huff_ctrl, table_destination, HUFF_MIN_TABLE,
						   ctrl_auto, DISABLE,
						   mode, DISABLE );
	MJPEG_WrFL( huff_ctrl );

	return bUseUserHuffman;
}

/**
 * Set Quantization Table for decoding
 *
 */
static void
_Decode_SetQTable_Reg(LX_MJPEG_FRAME_HEADER_T *i_pstFrameInfo, LX_MJPEG_QUANTIZATION_TABLE_T *i_pstQTable)
{
	SINT32 iCompID, iCount;

	MJPEG_RdFL( qmat_ctrl );

	for( iCompID = 0 ; iCompID < i_pstFrameInfo->ui8NumberOfComponents ; ++iCompID )
	{
		SINT32 QTableID = i_pstFrameInfo->stComponents[iCompID].ui8QTableID;

		MJPEG_Wr03( qmat_ctrl, table_destination, iCompID,
							   ctrl_auto, ENABLE,
							   mode, ENABLE );
		MJPEG_WrFL( qmat_ctrl );

		for( iCount = 0 ; iCount < 64 ; ++iCount )
		{
			MJPEG_Wr( qmat_data, i_pstQTable[QTableID].ui8Elements[iCount] );
			MJPEG_WrFL( qmat_data );
		}
		MJPEG_Wr03( qmat_ctrl, table_destination, 0x0,
							   ctrl_auto, DISABLE,
							   mode, DISABLE );
		MJPEG_WrFL( qmat_ctrl );
	}
}

