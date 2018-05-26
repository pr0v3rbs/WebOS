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
 *  register api configuration for venc device of H13.
 *	venc device will teach you how to make device driver with new platform.
 *
 *  author		jaeseop.so (jaeseop.so@lge.com)
 *  version		1.0
 *  date		2012.06.12
 *  note		Additional information.
 *
 *  @addtogroup lg1154_venc
 *	@{
 */

#ifndef	_H1ENCODE_H_
#define	_H1ENCODE_H_

/*-----------------------------------------------------------------------------
	Control Constants
-----------------------------------------------------------------------------*/
// FOR DEBUG FUNCTION ON/OFF
//#define H1ENCODE_DEBUG_DUMP
//#define H1ENCODE_DEBUG_TEST
//#define H1ENCODE_DEBUG_STREAM_LIST
//#define H1ENCODE_DEBUG_SAVE_DURATION
//#define H1ENCODE_DEBUG_AUI_SEQ				// for testing AUI sequence changes.
//#define H1ENCODE_DEBUG_SWAP_TB				// if interlaced intput, Swap field type between Top and Bottom field.
//#define H1ENCODE_DEBUG_INTERLACED_DUMP
//#define H1ENCODE_DEBUG_CHECK_VSYNC				// check vsync interval.
//#define H1ENCODE_DEBUG_MEM_LEAK

// FOR Implementation FUNCTION ON/OFF
#define H1ENCODE_USE_LOCK
#define H1ENCODE_USE_STRUCT_ASSIGNMENT
#define H1ENCODE_USE_VDEC_PTS
#define H1ENCODE_CHECK_INTERLACED_INPUT			// for check type of field when input is interlaced
#define H1ENCODE_ENABLE_SAR						// enable to insert SAR infomation
//#define H1ENCODE_ENABLE_SCD			            // enable SCD(Scence Change Detect)
#define H1ENCODE_ENABLE_TASK_PRIORITY_HIGH
#define H1ENCODE_ENABLE_DROP_FIRST_FRAME
//#define H1ENCODE_USE_HMA
//#define H1ENCODE_ENABLE_FIELDORDER
//#define H1ENCODE_ENABLE_FIELD_REPEAT			// to repeat current field when incoming unexected field type.
//#define H1ENCODE_ENABLE_SEI_USERDATA			// adding sei infomation for userdata
//#define H1ENCODE_ENABLE_INTERLACED_BF			// Bottom-field first
//#define H1ENCODE_ENABLE_COMBINED_FIELD_AUI
//#define H1ENCODE_USE_NONBLOCKING_MODE			// = H1ENCODE_UNUSE_ENCODE_THREAD
//#define H1ENCODE_USE_VSYNC_BUSYWAIT
//#define H1ENCODE_USE_LOGFILE
//#define H1ENCODE_USE_PREP_STC
//#define H1ENCODE_USE_WORKAROUND_HW_DELAY

/*-----------------------------------------------------------------------------
    File Inclusions
-----------------------------------------------------------------------------*/
#include <linux/workqueue.h>
#include <linux/list.h>

#include "venc_kapi.h"
#include "venc_reg.h"

/*-----------------------------------------------------------------------------
	Constant Definitions
-----------------------------------------------------------------------------*/
//#define DEFAULT 					-255
#define H1ENCODE_OUTBUF_MIN			0x80000	// 512kb

// FOR VP8
#define IVF_HDR_BYTES       32
#define IVF_FRM_BYTES       12

#define H1ENCODE_CHECK_FRAME_MAX	30
#define H1ENCODE_BITRATE_MIN		10000		// bitrate mininum value at On2 H/W
#define H1ENCODE_BITRATE_HD			4000000		// bitrate for HD, SD
#define H1ENCODE_BITRATE_FHD		8000000		// bitrate for Full HD
#define H1ENCODE_BITRATE_MAX		60000000	// bitrate maximum value at On2 H/W

#define H1ENCODE_GOPLENGTH_DEFAULT	30
#define H1ENCODE_GOPLENGTH_MIN		1
#define H1ENCODE_GOPLENGTH_MAX		300

#define H1ENCODE_QP_MIN				0
#define H1ENCODE_QP_MAX				51

#define H1ENCODE_FRAMERATECODE_MIN	1
#define H1ENCODE_FRAMERATECODE_MAX	9

#ifdef H1ENCODE_ENABLE_TASK_PRIORITY_HIGH
#define H1ENCODE_SCHED_PRIORITY		2
#endif

#ifdef H1ENCODE_ENABLE_DROP_FIRST_FRAME
#define H1ENCODE_DROP_FIRST_FRAME_COUNT 2
#endif

/*-----------------------------------------------------------------------------
	Macro Definitions
-----------------------------------------------------------------------------*/
#if 0
#define TRACE_ENTER()		VENC_TRACE("%s: ENTER.\n", __FUNCTION__)
#define TRACE_EXIT()		VENC_TRACE("%s: EXIT.\n", __FUNCTION__)
#else
#define TRACE_ENTER()
#define TRACE_EXIT()
#endif

#ifndef MROUND
#define MROUND( number, multiple ) ((((number) / (multiple)) + (((number) % (multiple)) ? 1 : 0) ) * (multiple) )
#endif

#ifndef MROUND64
#define MROUND64( number )			MROUND( number, 64 )
#endif

#ifndef MIN
#define MIN( a, b ) 	( (a) <= (b) ? (a) : (b) )
#endif

#ifndef MAX
#define MAX( a, b )		( (a) >= (b) ? (a) : (b) )
#endif

#ifdef H1ENCODE_USE_LOGFILE
#define LOGFILE_NAME "/tmp/hxenc.log"
#define H1ENCODE_LOG( format, args... )		_LOG_Write( (format), ##args )
int _LOG_Write( const char * format, ... );
#else
#define H1ENCODE_LOG( format, args... )		// NOP
#endif

/*-----------------------------------------------------------------------------
    Type Definitions
-----------------------------------------------------------------------------*/
typedef LX_VENC_FRAME_TYPE_T H1ENCODE_FRAME_TYPE_T;

typedef enum
{
	LX_VENC_FIELD_ORDER_BOTTOM = 0,
	LX_VENC_FIELD_ORDER_TOP = 1,
} LX_VENC_FILED_ORDER_T;

typedef enum
{
	H1ENCODE_AU_TYPE_I = 0,
	// non IDR I Picure = 1
	H1ENCODE_AU_TYPE_P = 2,
	// B Picture = 3
} H1ENCODE_AU_TYPE_T;

typedef enum
{
	H1ENCODE_EVENT_THUMBNAIL 	= 1 << 0,
	H1ENCODE_EVENT_VSYNC 		= 1 << 1,
	H1ENCODE_EVENT_FRAMERATE	= 1 << 2,
	H1ENCODE_EVENT_INPUT		= 1 << 3,
} H1ENCODE_EVENT_T;

typedef enum
{
	H1ENCODE_STATUS_TYPE_STOP = 0,
	H1ENCODE_STATUS_TYPE_STOP_PREPARE,
	H1ENCODE_STATUS_TYPE_START,
	H1ENCODE_STATUS_TYPE_START_PREPARE,
} H1ENCODE_STATUS_TYPE_T;

typedef struct
{
	// Read from PREP Register
	u32 yBase;
	u32 cBase;
	u16 inWidth;
	u16 inHeight;

#ifdef H1ENCODE_ENABLE_SAR
	u16 decWidth;
	u16 decHeight;
#endif

	u16 yStride;
	u16 cStride;
	H1ENCODE_FRAME_TYPE_T frameType;
	BOOLEAN	topFieldFirst;

	u16 frameRateCode;
} H1ENCODE_IPC_STATUS_T;

typedef struct
{
	u32 u32Phys;
	u32 u32Virt;
	u32 u32Size;

	u32 u32ReadOffset;
	u32 u32WriteOffset;
} H1ENCODE_MEM_T;

typedef struct
{
	struct work_struct data;
	int id;

	// Input
	u32 u32YBase;
	u32 u32CBase;
#ifdef H1ENCODE_ENABLE_SCD
	u32 u32YBaseNext;		// for SCD
#endif
	u8	u8GopLength;
	u32	u32FrameIndex;
	u32	u32Timestamp;		// for ts packetizing
	H1ENCODE_FRAME_TYPE_T frameType;

	// Enc result
	H1ENCODE_AU_TYPE_T		au_type;
	u32	u32EncodingTime;

	struct list_head *pStreamList;
	H1ENCODE_MEM_T	*pMemESB;
	H1ENCODE_MEM_T	*pMemAUI;

#ifdef H1ENCODE_ENABLE_COMBINED_FIELD_AUI
	u8 tb;					// 0:top-field, 1:bottom-field
	u8 interlaced;			// 0:progressive, 1:interlaced
#endif
} H1ENCODE_ENC_PARAMS_T;


typedef struct tagH1ENCODE_API_T H1ENCODE_API_T;
struct tagH1ENCODE_API_T
{
	int 	id;								// Core ID

	LX_VENC_ENCODE_TYPE_T eEncodeType;	// H.264/VP8
	BOOLEAN	isEncodeOpened;
	BOOLEAN	isBusy;
	int intraPeriodCnt;
	int codedFrameCnt;
	void * 	pEncInst;					// H264EncInst or VP8EncInst
	u8		tb;							// top/bottom field
	u32		minBufferLength;

// BEGIN of config
	H1ENCODE_FRAME_TYPE_T	eFrameType;
	BOOLEAN	topFieldFirst;

	u32		width;
	u32		height;
#ifdef H1ENCODE_ENABLE_SAR
	u32		decWidth;
	u32		decHeight;
#endif
	u32		stride;
	u32		bitrate;	// [10000..60000000]
	BOOLEAN bEnableCBR;
	u32		gopLength;	// [1..300]
	u32		frameRateCode;
	u32 	frameRateDenom;
	u32		frameRateNum;
	u32		qp;			// [0..51]
// END of config

//	u8	type;
	BOOLEAN bIsFirstFrame;
	BOOLEAN bIncludeIVFHeader;			// Use VP8 only

// BEGIN of buffers
	H1ENCODE_MEM_T	memESB;
	H1ENCODE_MEM_T	memAUI;
	H1ENCODE_MEM_T	memOSB;	//Output Stream Buffer
	H1ENCODE_MEM_T	memOutputFrame[8];
	u32 	outputFrameCount;
	u32 	outputFrameIndex;
// END of buffers

// BEGIN of control apis
	int (* Init) ( H1ENCODE_API_T * );
	int (* Open) ( H1ENCODE_API_T * ); //, H1ENCODE_INPUT_CONFIG_T *);
	int (* Close) ( H1ENCODE_API_T * );
	int (* Encode) ( H1ENCODE_API_T *, H1ENCODE_ENC_PARAMS_T * );
	int (* EncodeAsync) ( H1ENCODE_API_T *, H1ENCODE_ENC_PARAMS_T * );
	int (* EncodeDone) ( H1ENCODE_API_T *, H1ENCODE_ENC_PARAMS_T * );

	// Setter functions
	void (* SetMemESB) ( H1ENCODE_API_T *, H1ENCODE_MEM_T );
	void (* SetMemAUI) ( H1ENCODE_API_T *, H1ENCODE_MEM_T );
	void (* SetMemOSB) ( H1ENCODE_API_T *, H1ENCODE_MEM_T );

	int (* SetFrameType) ( H1ENCODE_API_T *, H1ENCODE_FRAME_TYPE_T );	// frame type
	int (* SetFrameSize) ( H1ENCODE_API_T *, u32, u32, u32 );	// width, height, stride
	int (* SetGOPLength) ( H1ENCODE_API_T *, u32 );	// set length of gop
	int (* SetFrameRate) ( H1ENCODE_API_T *, u32 );	// set frame rate number
	int (* SetBitrate) ( H1ENCODE_API_T *, u32, BOOLEAN );		// set target bitrate
	int (* SetQPValue) ( H1ENCODE_API_T *, u32 );		// set qp value

	// Getter functions
	//int (* SetFrameType) ( H1ENCODE_API_T *, H1ENCODE_FRAME_TYPE_T );	// frame type
	int (* GetGOPLength) ( H1ENCODE_API_T *, u32 * );	// get length of gop
	int (* GetFrameRate) ( H1ENCODE_API_T *, u32 * );	// get frame rate number
	int (* GetBitrate) ( H1ENCODE_API_T *, u32 * );	// get target bitrate
	int (* GetQPValue) ( H1ENCODE_API_T *, u32 * );	// get qp value
// END of control apis
} ;

typedef struct
{
	int id;
#ifdef H1ENCODE_USE_LOCK
	spinlock_t	lock;
#endif

	LX_VENC_INPUT_SRC_T   eInputSource;	// DTV, ATV, AV_INPUT, ...
	LX_VENC_ENCODE_TYPE_T outputType;	//H264, VP8
	LX_VENC_RECORD_COMMAND_T cmd;
	H1ENCODE_STATUS_TYPE_T eStatus;

	H1ENCODE_API_T	*pstEncoder;
	int (*ISRHandler)( void );
	struct task_struct *taskEncode;

	// Save number of encoded frame
	u32 u32EncodedFrames;	// the number of total encoded frames

	// Check for encoding duration
	u32 arEncodeTimes[H1ENCODE_CHECK_FRAME_MAX];
	u32 timeIndex;
	u32 u32EncodedTime;		// last encoding time	(§Á)

	// Check for encoding bitrate
	u32 arStreamSizes[H1ENCODE_CHECK_FRAME_MAX];
	u32 sizeIndex;
	u32 u32EncodedBitrate;	// encoding bitrate of latest 30 frames

//	H1ENCODE_MEM_T	memIFB;		// Input frame buffer
	H1ENCODE_MEM_T	memESB;		// ES Buffer
	H1ENCODE_MEM_T	memAUI;		// AUI Buffer
	H1ENCODE_MEM_T	memOSB;		// Output stream buffer
	H1ENCODE_MEM_T	memTFB; 	// Input frame buffer for thumbnail

	u32 u32AUIIndex;
	u32 u32AUIStreamSize;

	struct work_struct stEncWork;
	//struct workqueue_struct *venc_workqueue;

	u32 ui32CountVsync;
	u32 ui32TotalVsync;
	u32 ui32CountFrameSkip;
	u32 ui32CountFrameError;
	u32 ui32CountFrameDelay;
	u32 u32CurrentPTS;

	H1ENCODE_IPC_STATUS_T stPrepStatus;

	struct {
		u32 width;
		u32 height;
		u32 targetBitrate;
		u32 frameRateCode;
		u32 frameRateDenom;
		u32 frameRateNum;
		u32 gopLength;
		int qpValue;
		u32 sarWidth;
		u32 sarHeight;
		BOOLEAN bEnableCBR;
	} stInput;

#ifdef H1ENCODE_ENABLE_DROP_FIRST_FRAME
	u32 remain_drop;
#endif
	BOOLEAN	enableInputDetect;
	BOOLEAN enableFramerateDetect;
	struct {
		int count;
		u32 gstc[6];
	} stFramerate;

	wait_queue_head_t	wqEncodeVsync;
	wait_queue_head_t	wqEncodeDone;

#ifdef H1ENCODE_CHECK_INTERLACED_INPUT
	H1ENCODE_FRAME_TYPE_T prevFrameType;
#endif

#ifdef H1ENCODE_ENABLE_SCD
	H1ENCODE_ENC_PARAMS_T *pstEncParamsPrev;
#endif

	// Status..
	int	eventEncodeDoneWakeup;
	int encodedCountBeforeDiscard;

	// Use recovery h/w delay
	H1ENCODE_IPC_STATUS_T *savedIPC;
	UINT32	savedGSTC;
} H1ENCODE_T;

typedef struct {
	UINT32				ui32ImageSize;

	// for ES Buffer
	UINT32				ui32ESWptr;
	UINT32				ui32ESRptr;

	// for AUI Buffer
	UINT32				ui32AUIWptr;
	UINT32				ui32AUIRptr;
} H1ENCODE_OUTPUT_STATUS_T;


typedef struct
{
	u32 unit_size	: 24,
		au_type		:  8;
	u32 unit_start		;
#ifdef H1ENCODE_DEBUG_SAVE_DURATION
	u32	index		: 16,
		duration	: 16;	// micro seconds
#else
	u32 index			;
#endif
	u32 timestamp 		;
} H1ENCODE_AUI_T;

#if 0
typedef struct {
    struct work_struct data;

    u32 *pYBase;
    u32 *pCBase;
    u32 width;
    u32 height;
    u32 stride;

} H1ENCODE_WORK_INPUT_T;
#endif

/*-----------------------------------------------------------------------------
	Extern Function Prototype Declaration
-----------------------------------------------------------------------------*/
int MemallocInit( u32 startAddress, u32 endAddress );

/*-----------------------------------------------------------------------------
	Extern Variables
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Generic Usage Functions
-----------------------------------------------------------------------------*/
// HAL - H1Encoder interface API
void H1EncodeInitialize( void );
void H1EncodeFinalize( void );
int H1EncodeOpen( int id );
int H1EncodeClose( int id );
void H1EncodeHxencInit( void );
void H1EncodeSetEncodeType( int id, LX_VENC_ENCODE_TYPE_T eEncodeType );
int H1EncodeCommand( int id, LX_VENC_RECORD_COMMAND_T cmd );
u32 H1EncodeGetEncodedFrameCount( int id );
u32 H1EncodeGetEncodeMsec( int id );
s32 H1EncodeSetFramerate( int id, u32 frameRate );
s32 H1EncodeGetFramerate(	int id,
							LX_VENC_RECORD_FRAME_RATE_T *peFramerate,
							UINT32	*pFramerate,
							LX_VENC_SOURCE_TYPE_T	*peSourceType );
s32	H1EncodeSetResolution( int id, u16 width, u16 height );
s32 H1EncodeGetResolution( int id, u16 *pWidth, u16 *pHeight );
s32 H1EncodeSetBitrate( int id, u32 bitrate, BOOLEAN bEnableCBR );
u32 H1EncodeGetBitrate( int id );
s32 H1EncodeSetGOP( int id, u32 gopLength );
u32 H1EncodeGetGOP( int id );
s32 H1EncodeSetQPValue( int id, u32 qp );
u32 H1EncodeGetQPValue( int id );
int H1EncodeGetOutputBuffer( int id, LX_VENC_RECORD_OUTPUT_T* pstEncOutput );
int H1EncodeGetFrameImage( int id, LX_VENC_RECORD_FRAME_IMAGE_T *pstFrameImage );
s32 H1EncodeSetSAR( int id, u32 sarWdith, u32 sarHeight );
s32 H1EncodeGetSAR( int id, u32 *pSarWidth, u32 *pSarHeight );
s32 H1EncodeSetInputSource( int id, LX_VENC_INPUT_SRC_T eInputSource );
int H1EncodeReadStatus( char * buffer );
int H1EncodeCheckIPCRegister( int id );
int H1EncodeISRHandler( int id );
int H1EncodeGetBufferInfo( int channel, LX_VENC_BUFFER_T *pstParams );

// H1Encoder Common functions
void H1EncodeSetTime( int id, u32 clock );
u32 H1EncodeGetTime( int id );
void H1EncodeAUIWrite( H1ENCODE_MEM_T *pMemAUI, H1ENCODE_AUI_T stAUI );
void H1EncodeNotifyDone( int id, H1ENCODE_MEM_T *pCPB, H1ENCODE_MEM_T *pAUI );
int H1EncodeStreamWrite( H1ENCODE_AU_TYPE_T au_type, u32 streamBase, u32 streamSize,
	H1ENCODE_MEM_T	*pMemCPB, H1ENCODE_MEM_T *pMemAUI, struct list_head *pStreamList  );
void H1EncodeUpdateStatus( int id, u32 frameCount, u32 streamSize, u32 encodingTime );

// H.246
H1ENCODE_API_T *H264Alloc( void );
void H264Release(H1ENCODE_API_T * pstEncoder);

// VP8
H1ENCODE_API_T *VP8Alloc( void );
void VP8Release(H1ENCODE_API_T * pstEncoder);

// H1Encoder external functions
void _StreamInfoClear( struct list_head *pList );
void _StreamInfoPush( struct list_head *pList, u32 streamBase, u32 streamSize );
void _StreamInfoFlush( struct list_head *pList, u32 *pu32StreamStart, u32 *pu32StreamEnd, u32 *pu32StreamSize );

struct list_head * _StreamListNew( void );
void _StreamListFree( struct list_head *pStreamList );
int _StreamListCopy2Mem( struct list_head *pList, H1ENCODE_MEM_T *pstMem );

// Logger
int _LOG_Write( const char * format, ... );

// Memory Allocator
void _EncParamInit( void );
void _EncParamFinal( void );
H1ENCODE_ENC_PARAMS_T * _EncParamAlloc( void );
void _EncParamFree( H1ENCODE_ENC_PARAMS_T * pstEncParams );
void _EncParamClear( void );

// WorkQ functions
int _WorkQInit( void );
int _WorkQCleanup( void );

int _WorkQEnqueue( H1ENCODE_API_T *pstEncoder, H1ENCODE_ENC_PARAMS_T *pstEncParams );
int _WorkQCleanup( void );
void _WorkQFlush( void );
void _WorkQEncodeBH( struct work_struct *work );

#if 0
int _EncodeWorkQInit( void );
int _EncodeWorkQCleanup( void );
H1ENCODE_WORK_INPUT_T *_EncodeWorkQAlloc( void );
void _EncodeWorkQFree( H1ENCODE_WORK_INPUT_T *pstWork );
int _EncodeWorkQCopyDE2MEM( H1ENCODE_WORK_INPUT_T *pstWork, u32 *pYBase, u32 *pCBase );
int _EncodeWorkQEnqueue( H1ENCODE_WORK_INPUT_T *pstWork );
#endif

#ifdef SUPPORT_VENC_DEVICE_ENC_API
int ENC_Create(int id, LX_VENC_ENC_CREATE_T *pstCreate);
int ENC_Destroy(int id, LX_VENC_ENC_DESTROY_T *pstDestroy);
int ENC_Encode(int id, LX_VENC_ENC_ENCODE_T *pstEncode);
#endif

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _H1ENCODE_H_ */

/** @} */

