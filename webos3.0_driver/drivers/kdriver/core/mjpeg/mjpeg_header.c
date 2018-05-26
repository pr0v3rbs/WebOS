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
 *  mjpeg header decoder interface implementation.
 *
 *  author		youngwoo.jin (youngwoo.jin@lge.com)
 *  version		1.0
 *  date		2011.04.09
 *  note		Additional information.
 *
 *  @addtogroup lg1152_mjpeg
 *	@{
 */

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/mm.h>
#include "mjpeg_header.h"
#include "mjpeg_drv.h" //for debug

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
#define MIN_VSIZE		LX_MJPEG_MIN_VSIZE
#define MIN_HSIZE		LX_MJPEG_MIN_HSIZE
#define MAX_VSIZE		LX_MJPEG_MAX_VSIZE
#define MAX_HSIZE		LX_MJPEG_MAX_HSIZE

enum {
	MJPEG_SAMPLE_420 	= 0xA,
	MJPEG_SAMPLE_H422	= 0x9,
	MJPEG_SAMPLE_V422	= 0x6,
	MJPEG_SAMPLE_444 	= 0x5,
	MJPEG_SAMPLE_400 	= 0x1
	};

enum {
	SOI_MARKER		= 0xFFD8,			// Start of image
	EOI_MARKER		= 0xFFD9,			// End of image

	JFIF_CODE		= 0xFFE0,			// Application
	EXIF_CODE		= 0xFFE1,

	DRI_MARKER		= 0xFFDD,			// Define restart interval
	RST_MARKER		= 0x000D,			// 0xD0 ~0xD7

	DQT_MARKER		= 0xFFDB,			// Define quantization table(s)
	DHT_MARKER		= 0xFFC4,			// Define Huffman table(s)

	SOF_MARKER		= 0xFFC0,			// Start of frame : Baseline DCT
	SOS_MARKER		= 0xFFDA			// Start of scan
};

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Type Definitions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Function Prototype Declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	External Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Functions
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	global Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Static Function Prototypes Declarations
-----------------------------------------------------------------------------*/
static SINT32	_DecodeAPPHeader(void);
static SINT32	_DecodeSOFHeader(void);
static SINT32	_DecodeSOSHeader(void);
static SINT32	_DecodeDRIHeader(void);
static SINT32	_DecodeDQTHeader(void);
static SINT32	_DecodeDHTHeader(void);

/*-----------------------------------------------------------------------------
	Static Variables
-----------------------------------------------------------------------------*/
static LX_MJPEG_HEADER_T *_g_pstHeader;
static UINT8 *_g_pui8Buffer;
static UINT32 _g_i32BufferSize;
static UINT32 _g_i32ReadBytes;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * Decode header
 *
 */
UINT32 MJPEG_HDR_Decode(LX_MEM_BUF_T *i_pstBuffer, LX_MJPEG_HEADER_T *o_pstHeader, UINT8 *o_pui8CheckList)
{
	SINT32 i32Count = 0;
	BOOLEAN bDecoding = TRUE;

	_g_pstHeader = o_pstHeader;
	_g_pui8Buffer = (UINT8 *)(i_pstBuffer->addr);
	_g_i32BufferSize = i_pstBuffer->length;
	_g_i32ReadBytes = 0;

	while ( bDecoding )
	{
		UINT16 MarkerCode;

		// Check buffer
		if ( _g_i32ReadBytes+2 >= _g_i32BufferSize ) { return _g_i32ReadBytes; }

		// Get Marker code
		MarkerCode = (_g_pui8Buffer[_g_i32ReadBytes]<<8) | _g_pui8Buffer[_g_i32ReadBytes+1];

		switch ( MarkerCode )
		{
			case SOI_MARKER:
			case EOI_MARKER:
				i32Count = 0;
				//MJPEG_PRINT("[%d] SOI or EOI\n", _g_i32ReadBytes);
				break;

			case JFIF_CODE:
			case EXIF_CODE:
				i32Count = _DecodeAPPHeader();
				//MJPEG_PRINT("[%d] JFIF or EXIF\n", _g_i32ReadBytes);
				break;

			case SOF_MARKER:
				*o_pui8CheckList |= (0x01<<MJPEG_FRAME_HEADER);
				i32Count = _DecodeSOFHeader();
				//MJPEG_PRINT("[%d] SOF\n", _g_i32ReadBytes);
				break;

			case SOS_MARKER:
				*o_pui8CheckList |= (0x01<<MJPEG_SCAN_HEADER);
				i32Count = _DecodeSOSHeader();
				//MJPEG_PRINT("[%d] SOS\n", _g_i32ReadBytes);
				bDecoding = FALSE;
				break;

			case DQT_MARKER:
				*o_pui8CheckList |= (0x01<<MJPEG_QUANTIZATION_TABLE);
				i32Count = _DecodeDQTHeader();
				//MJPEG_PRINT("[%d] DQT\n", _g_i32ReadBytes);
				break;

			case DHT_MARKER:
				*o_pui8CheckList |= (0x01<<MJPEG_HUFFMAN_TABLE);
				i32Count = _DecodeDHTHeader();
				//MJPEG_PRINT("[%d] DHT\n", _g_i32ReadBytes);
				break;

			case DRI_MARKER:
				*o_pui8CheckList |= (0x01<<MJPEG_RESET_INTERVAL);
				i32Count = _DecodeDRIHeader();
				//MJPEG_PRINT("[%d] DRI\n", _g_i32ReadBytes);
				break;

			default:
				switch ( MarkerCode&0xFFF0 )
				{
					case 0xFFE0:	// 0xFFEX
					case 0xFFF0:	// 0xFFFX
						i32Count = _DecodeAPPHeader();
						//MJPEG_PRINT("[%d] App%d\n", _g_i32ReadBytes, MarkerCode&0x000F);
						break;

					default:
						if ( (MarkerCode&0xFF00) == 0xFF00 )
						{
							MJPEG_DEBUG("[%d] Unknown code : [0x%X]\n", _g_i32ReadBytes, MarkerCode);
							return _g_i32ReadBytes;
						}
						else
						{
							++_g_i32ReadBytes;
							continue;
						}
				}
		}

		if ( i32Count < 0 ) { return _g_i32ReadBytes; }

		_g_i32ReadBytes += i32Count+2;
	}

	return _g_i32ReadBytes;
}

/**
 * Decode APP header
 *
 */
static SINT32
_DecodeAPPHeader(void)
{
	SINT32 ReadPointer = _g_i32ReadBytes+2;
	UINT16 Length;

	// Check buffer
	if ( ReadPointer+2 > _g_i32BufferSize ) { return -1; }

	Length = (_g_pui8Buffer[ReadPointer]<<8) |_g_pui8Buffer[ReadPointer+1];

	ReadPointer += 2;

	if ( ReadPointer+Length-2 > _g_i32BufferSize ) { return -1; }

	return Length;
}

/**
 * Decode SOF header
 *
 */
static SINT32
_DecodeSOFHeader(void)
{
	SINT32 ReadPointer = _g_i32ReadBytes+2;
	SINT32 i32Count;
	UINT16 Length;
	LX_MJPEG_FRAME_HEADER_T *pstFrameHeader = &(_g_pstHeader->stFrameHeader);

	// Check buffer
	if ( ReadPointer+2 > _g_i32BufferSize ) { return -1; }

	Length = (_g_pui8Buffer[ReadPointer]<<8) |_g_pui8Buffer[ReadPointer+1];
	ReadPointer += 2;

	if ( ReadPointer+Length-2 > _g_i32BufferSize ) { return -1; }

	// Sample Precision: Baseline(8), P
	pstFrameHeader->ui8SamplingPrecision = _g_pui8Buffer[ReadPointer++];
	pstFrameHeader->ui16Height = _g_pui8Buffer[ReadPointer++];
	pstFrameHeader->ui16Height = pstFrameHeader->ui16Height << 8 | _g_pui8Buffer[ReadPointer++];
	pstFrameHeader->ui16Width = _g_pui8Buffer[ReadPointer++];
	pstFrameHeader->ui16Width = pstFrameHeader->ui16Width << 8 | _g_pui8Buffer[ReadPointer++];
	//Number of Components in Frame: Nf
	pstFrameHeader->ui8NumberOfComponents = _g_pui8Buffer[ReadPointer++];


	if ( (pstFrameHeader->ui8SamplingPrecision != 8)	||
         (pstFrameHeader->ui16Height < MIN_VSIZE)   	||
		 (pstFrameHeader->ui16Height > MAX_VSIZE)	||
	     (pstFrameHeader->ui16Width < MIN_HSIZE)   	||
		 (pstFrameHeader->ui16Width > MAX_HSIZE)		||
		 (pstFrameHeader->ui8NumberOfComponents > 3) )
	{
		return -1;
	}

	for( i32Count = 0 ; i32Count < pstFrameHeader->ui8NumberOfComponents ; ++i32Count )
	{
       	// Component ID, Ci 0 ~ 255
        pstFrameHeader->stComponents[i32Count].ui8ComponentID = _g_pui8Buffer[ReadPointer++];
		// Horizontal Sampling Factor, Hi
		pstFrameHeader->stComponents[i32Count].ui8HSampleFreq = (_g_pui8Buffer[ReadPointer]&0xF0) >> 4;
		// Vertical Sampling Factor, Vi
		pstFrameHeader->stComponents[i32Count].ui8VSampleFreq = (_g_pui8Buffer[ReadPointer++]&0x0F);
		// Quantization Table Selector, Tqi
		pstFrameHeader->stComponents[i32Count].ui8QTableID = _g_pui8Buffer[ReadPointer++];
	}

	return Length;
}

/**
 * Decode SOS header
 *
 */
static SINT32
_DecodeSOSHeader(void)
{
	SINT32 ReadPointer = _g_i32ReadBytes+2;
	SINT32 i32Count;
	UINT16 Length;
	LX_MJPEG_SCAN_HEADER_T *pstScanHeader = &(_g_pstHeader->stScanHeader);

	// Check buffer
	if ( ReadPointer+2 > _g_i32BufferSize ) { return -1; }

	Length = (_g_pui8Buffer[ReadPointer]<<8) | _g_pui8Buffer[ReadPointer+1];
	ReadPointer += 2;

	if ( ReadPointer+Length-2 > _g_i32BufferSize ) { return -1; }

	pstScanHeader->ui8NumberOfComponents = _g_pui8Buffer[ReadPointer++];

	for( i32Count = 0 ; i32Count < pstScanHeader->ui8NumberOfComponents ; ++i32Count )
	{
		pstScanHeader->stComponents[i32Count].ui8ComponentID = _g_pui8Buffer[ReadPointer++];
		pstScanHeader->stComponents[i32Count].ui8DC_HuffmanTableID = (_g_pui8Buffer[ReadPointer]&0xF0) >> 4;
		pstScanHeader->stComponents[i32Count].ui8AC_HuffmanTableID = (_g_pui8Buffer[ReadPointer++]&0x0F);
	}

	// Ss 0
	pstScanHeader->ui8Ss = _g_pui8Buffer[ReadPointer++];
	// Se 3F
	pstScanHeader->ui8Se = _g_pui8Buffer[ReadPointer++];
	// Ah 0
	pstScanHeader->ui8Ah = (_g_pui8Buffer[ReadPointer]&0xF0) >> 4;
	// Al 0
	pstScanHeader->ui8Al = (_g_pui8Buffer[ReadPointer++]&0x0F);

	if ( (pstScanHeader->ui8Ss != 0) || (pstScanHeader->ui8Se != 0x3F) ||
		 (pstScanHeader->ui8Ah != 0) || (pstScanHeader->ui8Al != 0) )
	{
		return -1;
	}

	return Length;
}

/**
 * Decode DRI header
 *
 */
static SINT32
_DecodeDRIHeader(void)
{
	SINT32 ReadPointer = _g_i32ReadBytes+2;
	UINT16 Length;
	UINT16 *pui16ResetInterval = &(_g_pstHeader->ui16ResetInterval);

	// Check buffer
	if ( ReadPointer+2 > _g_i32BufferSize ) { return -1; }

	Length = (_g_pui8Buffer[ReadPointer]<<8) | _g_pui8Buffer[ReadPointer+1];
	ReadPointer += 2;

	if ( ReadPointer+Length-2 > _g_i32BufferSize ) { return -1; }

	// Interval
	*pui16ResetInterval = _g_pui8Buffer[ReadPointer++];
	*pui16ResetInterval = *pui16ResetInterval<<8 | _g_pui8Buffer[ReadPointer++];

	return Length;
}

/**
 * Decode DQT header
 *
 */
static SINT32
_DecodeDQTHeader(void)
{
	SINT32 ReadPointer = _g_i32ReadBytes+2;
	SINT32 i32Count;
	UINT16 Length;
	LX_MJPEG_QUANTIZATION_TABLE_T *pstQuantizationTable = _g_pstHeader->stQuantiztionTables;
	UINT8 ui8Pq = 0;
	UINT8 ui8Tq = 0;

	// Check buffer
	if ( ReadPointer+2 > _g_i32BufferSize ) { return -1; }

	Length = (_g_pui8Buffer[ReadPointer]<<8) | _g_pui8Buffer[ReadPointer+1];
	ReadPointer += 2;

	if ( ReadPointer+Length-2 > _g_i32BufferSize ) { return -1; }

	for( i32Count = 0 ; i32Count < Length-2 ; ++i32Count )
	{
		if( i32Count%65 == 0 )
		{
			ui8Tq = (_g_pui8Buffer[ReadPointer]&0x0F);
			ui8Pq = (_g_pui8Buffer[ReadPointer++]&0xF0) >> 4;
			if ( ui8Pq != 0 ) { return -1; }

			pstQuantizationTable[ui8Tq].ui8Pq = ui8Pq;
			pstQuantizationTable[ui8Tq].ui8Tq = ui8Tq;
		}
		else
		{
			pstQuantizationTable[ui8Tq].ui8Elements[(i32Count%65)-1] = _g_pui8Buffer[ReadPointer++];
		}
	}

	return Length;
}

 /**
  * Decode DHT header
  *
  */
static SINT32
_DecodeDHTHeader(void)
{
	SINT32 ReadPointer = _g_i32ReadBytes+2;
	SINT32 i32Count;
	UINT16 Length;
	LX_MJPEG_HUFFMAN_TABLE_T *pstHuffmanTables = _g_pstHeader->stHuffmanTables;

	// Check buffer
	if ( ReadPointer+2 > _g_i32BufferSize ) { return -1; }

	Length = (_g_pui8Buffer[ReadPointer]<<8) | _g_pui8Buffer[ReadPointer+1];
	ReadPointer += 2;

	if ( ReadPointer+Length-2 > _g_i32BufferSize ) { return -1; }

	for( i32Count = 0 ; i32Count < Length-2 ; ++i32Count )
	{
		SINT32 i32TableCount, i32BitCount;
		UINT8 ui8Th, ui8Tc, ui8ThTc;
		ui8Th = (_g_pui8Buffer[ReadPointer]&0x0F);
		ui8Tc = (_g_pui8Buffer[ReadPointer++] &0xF0) >> 4;
		// DC_ID0 (0x00) -> 0
		// AC_ID0 (0x10) -> 1
		// DC_ID1 (0x01) -> 2
		// AC_ID1 (0x11) -> 3
		ui8ThTc = ((ui8Th&1)<<1) | (ui8Tc&1);
		pstHuffmanTables[ui8ThTc].ui8Th = ui8Th;
		pstHuffmanTables[ui8ThTc].ui8Tc = ui8Tc;

		i32BitCount = 0;
		for ( i32TableCount = 0 ; i32TableCount < 16 ; ++i32Count, ++i32TableCount )
		{
			pstHuffmanTables[ui8ThTc].ui8CodeLengths[i32TableCount] = _g_pui8Buffer[ReadPointer++];
			i32BitCount += pstHuffmanTables[ui8ThTc].ui8CodeLengths[i32TableCount];
		}

		for ( i32TableCount = 0 ; i32TableCount < i32BitCount ; ++i32Count, ++i32TableCount )
		{
			pstHuffmanTables[ui8ThTc].ui8CodeValues[i32TableCount] = _g_pui8Buffer[ReadPointer++];
		}

	}

	 return Length;
}

