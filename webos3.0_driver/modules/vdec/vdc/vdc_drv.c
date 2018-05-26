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
 * Video decoding driver implementation for VDEC device.
 * VDEC device will teach you how to make device driver with lg115x platform.
 *
 * @author     Junghyun Son(junghyun.son@lge.com)
 * @version    1.0
 * @date       2012.05.10
 * @note       Additional information.
 *
 */

/*----------------------------------------------------------------------------------------
  Control Constant
  ----------------------------------------------------------------------------------------*/
#define LOG_NAME	vdec_vdc

//#define USE_ZEROPADDING_DETECT
#define CODE_FOR_CHECK_LOCK_TIME
#define USE_WORKQUEUE_SCHEDULE

/*----------------------------------------------------------------------------------------
  File Inclusion
  ----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/string.h>	// memset
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/seq_file.h>  // seq_file
#include <linux/sched.h>
#include <linux/kfifo.h>    // kfifo
#include <linux/version.h>

#include "vdc_drv.h"
#include "vdec_stc_timer.h"
#include "vdu/vdu_drv.h"

#include "ves/ves_drv.h"

#include "vds/disp_clear_q.h"

#include "hal/top_hal_api.h"
#include "hal/pdec_hal_api.h"
#include "hal/ipc_reg_api.h"
#include "hal/lq_hal_api.h"

#include "vdec_drv.h"

#include "log.h"
#include "vdc_util.h"

static int vdc_unlimit_retry;
module_param(vdc_unlimit_retry, int, 0644);

static int vdc_test_linebuf=0;
module_param(vdc_test_linebuf, int, 0644);

/*----------------------------------------------------------------------------------------
  Constant Definition
  ----------------------------------------------------------------------------------------*/
#define VDC_RESET_THRESHOLD			18000	// 200ms to 90k tick
#define VDC_SEQUENCE_INIT_THRESHOLD 		135000	// 1.5s to 90k tick
#define VDC_FEEDING_WAIT_THRESHOLD		900000	// 10s to 90k tick
#define VDC_WATCHDOG_TIME			100	// 100m
#define VDC_MAX_RETRY_COUNT_FOR_DECODING   	2
#define VDC_MAX_RETRY_COUNT_FOR_NOT_FOUND  	20
#define VDC_MAX_RETRY_COUNT_FOR_FAILED   	3
#define VDC_MAX_DPB_SIZE			32
#define VDC_READPOINTER_MISMATCH_THRESHOLD 	0xA
#define VDC_INITIAL_DPB_SIZE			0
#define	VDC_UNKNOWN_PTS				0xFFFFFFFE
#define VDC_INVALID_ADDRESS			0x0
#define VDC_INVALID_TIMESTAMP			-1
#define VDC_FEEDING_NUMBER_THERSHOLD 		450
#define VDC_TSPOOL_CLEAR_DENOM 			6
#define VDC_TIMER_DOMAIN 			90000
#define VDC_INVALID_HANDLE			0xFFFFFFFF
#define VDC_WRAPAROUND_PTS_BOUNDARY		900000  // 10s for wraparound
#define VDC_PACKED_SIZE				6
#define VDC_LIVE_PSEARCH_THRESHOLD		20
#define VDC_FILEPLAY_PSEARCH_THRESHOLD		120
/*----------------------------------------------------------------------------------------
  Macro Definition
  ----------------------------------------------------------------------------------------*/
#define GET_SIZE(_start, _end, _total)	( (_start > _end)?_total-(_start-_end) : _end - _start)
#define GET_DIFF(_a, _b) ((_a > _b)? _a-_b : _b-_a)
#define	CEILING_BYTES( _addr ) (((_addr) + 0x1FF) & (~0x1FF))

#define DEFINE_VDC_STATE( doing )\
    doing( VDC_NULL )\
doing( VDC_CLOSING )\
doing( VDC_PLAY )\
doing( VDC_READY )\
doing( VDC_STOP )\

#define DO_STRING( msg ) #msg,
#define DO_ENUM( msg ) msg,
/*----------------------------------------------------------------------------------------
  Type Definition
  ----------------------------------------------------------------------------------------*/
typedef enum
{
    DEFINE_VDC_STATE( DO_ENUM )
} VDC_STATE_T;
CHAR* gsStateName[] = { DEFINE_VDC_STATE( DO_STRING ) };

typedef enum
{
    VDC_DEBUG_CMD_IDLE,
    VDC_DEBUG_CMD_INF_RETRY,
    VDC_DEBUG_CMD_FORC_CODEC,
    VDC_DEBUG_CMD_LINEBUF,
} VDC_DEBUG_CMD_T;

typedef enum
{
    VDC_NOT_SUPPORT_NOTI_NONE,
    VDC_NOT_SUPPORT_NOTI_TRIGGERED,
    VDC_NOT_SUPPORT_NOTI_REPORTED,
} VDC_NOT_SUPPORT_NOTI_T;

typedef enum
{
    VDC_USRDPB_STATE_EMPTY,
    VDC_USRDPB_STATE_SET,
    VDC_USRDPB_STATE_NOR,
} VDC_USRDPB_STATE_T;

typedef enum
{
    VDC_TS_UNDEFINED,
    VDC_TS_PTS_ORDER,
    VDC_TS_DTS_ORDER,
    VDC_TS_UNKNOWN_ORDER,
} VDC_TS_ORDER_T;

typedef struct
{
    VDC_CODEC_T		eCodecType;
    CHAR*			sName;
} VDC_CODEC_INFO_T;

typedef struct
{
    UINT32			ui32UseChNum;
    UINT32          ui32NumofIdleCh;
    UINT32          ui32PrevSelCh;
} VDC_CORE_INFO_T;

typedef struct
{
    BOOLEAN			bFieldCh;
    UINT32			ui32ConsumedAU;
    UINT32			ui32ConsumedSize;
    UINT32			ui32DisplayedPic;
    UINT32			ui32Width;
    UINT32			ui32Height;
    UINT32			ui32TotDecodingTime;
    UINT32			ui32TotConsumedAU;
    UINT32			ui32TotConsumedSize;
    UINT32			ui32TotDisplayedPic;
    UINT32			ui32FramePackingArrange;
    UINT32			ui32LRInfo;
    UINT32			ui32NumofDecodedWithError;
    UINT32			ui32NumofDQOccupied;
    CHAR*			sInterProg;
    UINT64			ui64TotalPlayedTime;
    UINT64			ui64LastPts;
    UINT64			ui64AccumAuSize;
    CHAR			sDpbState[VDC_MAX_DPB_SIZE];
} VDC_DEBUG_INFO_T;

typedef struct timer_list	VDC_TIMER_T;

static struct
{
    // Parameter
    VDC_STATE_T		state;

    // channel information
    BOOLEAN			bLive;
	BOOLEAN			bUsePdec;
    UINT32			ui32VesCh;
    UINT32			ui32DPBSize;
    VDU_HANDLE_T 		hVduCh;
    VDC_CODEC_INFO_T	stCodecInfo;
    VDC_TIMER_T		stChTimer;
    VDC_LINEAR_MODE_T	eLinearBufMode;
    VDC_NOT_SUPPORT_NOTI_T  eNotSupportNotiValue;
    VDC_USRDPB_STATE_T	eWaitingBuffer;
    VDC_SRC_T		eSrc;
    VDC_TS_ORDER_T  eTsOrder;

    void			*priv;
    clear_q_t 		*clear_q;
    decoded_q_t		*decoded_q;
    wait_queue_head_t 	wait_state_change;
    spinlock_t		instance_lock;

    //decoding status
    atomic_t		atFeedHandle;
    atomic_t		atIsDecoding;
    BOOLEAN			bWaitingClear;
    BOOLEAN			bPopped;
    BOOLEAN			bFlushed;
    BOOLEAN			bRemainedData;
    BOOLEAN         bForceDecoding;
    BOOLEAN			bSequenceInitFailed;
    BOOLEAN			bFeedEOS;
    BOOLEAN         bSuspended;
    UINT32			ui32NumofDPBOccupied;
    UINT32			ui32SeqRetryCounter;
    UINT32			ui32DecRetryCounter;
    UINT32			ui32ScanMode;
    UINT32          ui32DecodedFlags;
    UINT32			ui32FrameFlags[VDC_MAX_DPB_SIZE];
    UINT32			ui32FeededCounter;
    UINT32			ui32FeedStartTime;
    UINT32			ui32DiscontTotal;
    UINT32			ui32DecodingTime;
    UINT32			ui32LastFeededPtr;
    UINT32			ui32FrameRate;
    UINT32			ui32EOSWaitCounter;

    //for performance (field feeding mode)
    BOOLEAN			bFieldFeeded;
    BOOLEAN			bNPFOccured;

    //for timestamp matching
    UINT32			ui32PrevRdPtr;
    UINT32          ui32NumofConsumedBeforeDefineOrder;
    SINT64			si64prevSavedTS;
    SINT64          si64prevSentTS;

    SINT64			si64TSInfo[VDC_MAX_DPB_SIZE];
    SINT64          si64UTSInfo[VDC_MAX_DPB_SIZE];
    struct kfifo    stTSfifo;

    VDC_DEBUG_INFO_T	stDebugInfo;

    /* for decode-only */
    VDU_LINEAR_FRAMES_T	stExtBufsInfo;
    VDC_STATE_CHANGED	state_changed;
} gstVdcDb[VDEC_NUM_OF_CHANNEL];

/*----------------------------------------------------------------------------------------
  Extern Function Prototype Declaration
  ----------------------------------------------------------------------------------------*/
extern void VDC_ProcInit(void);
extern void VDC_ProcCleanup(void);
extern int feeddump_dump(int ves, int vdc, unsigned int cmd, int addr, unsigned char *data, int size);

/*----------------------------------------------------------------------------------------
  Extern Variable
  ----------------------------------------------------------------------------------------*/
extern int feeddump_dumped_count[VDEC_NUM_OF_CHANNEL];
extern feeddump_mode_t feeddump_mode;
extern bool feeddump_header_found;

/*----------------------------------------------------------------------------------------
  Global Function
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Global Variable
  ----------------------------------------------------------------------------------------*/
static VDC_CORE_INFO_T		gstVdcCoreInfo;
/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declaration
  ----------------------------------------------------------------------------------------*/
static DEFINE_SPINLOCK(_VDC_Spinlock);
static DEFINE_MUTEX (_VDC_IoMutex);
static UINT32 _VDC_WorkReadPtr(VDC_HANDLE_T hVdcCh, UINT32 ui32ReadPtr, VDU_NOTI_T eNotiType, SINT8 si8DecodedIndex, SINT8 si8DisplayIndex);
static unsigned int _VDC_WorkReport(VDC_HANDLE_T hVdcCh, VDU_NOTI_T eNotiType, decoded_buffer_t *stDecBuf, VDU_FRM_INFO_T *pstFrameInfo);
static void _VDC_MakeDispInfo(VDC_HANDLE_T hVdcCh, decoded_buffer_t *stDecBuf, VDU_FRM_INFO_T *pstFrameInfo);
static void _VDC_MakeDebugInfo(VDC_HANDLE_T hVdcCh, CHAR* *pic_type, CHAR* *interlace, decoded_buffer_t *stDecBuf, UINT32 ui32Consumed, SINT64 ts);
static void _VDC_PrintPushInfo(VDC_HANDLE_T hVdcCh, CHAR* pic_type, CHAR* interlace, decoded_buffer_t *stDecBuf, UINT64 ui64TsForDiv, UINT64 ui64_90kts);
static void _VDC_WorkNotiType(VDC_HANDLE_T hVdcCh, SINT8 si8FrameIndex, VDU_NOTI_T eNotiType);
static void _VDC_DecCallback(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult);
static BOOLEAN _VDC_CheckUnknownOrder(VDC_HANDLE_T hVdcCh, SINT64 cur_ts);
static void _VDC_ChWatchDog(void);
static void _VDC_Scheduler(void);
static VDC_HANDLE_T _VDC_SellectChannel(void);
static BOOLEAN _VDC_FeedPossible(VDC_HANDLE_T hVdcCh);
static BOOLEAN _VDC_IsPicture(VDC_HANDLE_T hVdcCh, E_VES_AU_T eAuType);
static void _VDC_Main(VDC_HANDLE_T hVdcCh);
static void _VDC_InitializeDB(VDC_HANDLE_T hVdcCh);
static void _VDC_InitInnerDatas(VDC_HANDLE_T hVdcCh);
static void _VDC_ClearUsedDPB(VDC_HANDLE_T hVdcCh);
static BOOLEAN _VDC_CheckUsableDPB(VDC_HANDLE_T hVdcCh);
static BOOLEAN _VDC_PrepareFeed(VDC_HANDLE_T hVdcCh,VDU_AU_T *stBodaAU);
static void _VDC_DumpHeader(VDC_HANDLE_T hVdcCh, VDU_AU_T *stBodaAU);
static void _VDC_DumpFirst8Bytes(VDC_HANDLE_T hVdcCh, VDU_AU_T *stBodaAU);
static BOOLEAN _VDC_PrepareDec(VDC_HANDLE_T hVdcCh, VDU_LINEAR_FRAMES_T *stFrmsInfo);
static BOOLEAN _VDC_DecPostProc(VDC_HANDLE_T hVdcCh,VDU_RET_T eRetVal);
static BOOLEAN _VDC_Close(VDC_HANDLE_T hVdcCh);
static void _VDC_InitExtBufsInfo(VDU_LINEAR_FRAMES_T *stFrmsInfo);
static void _VDC_ChMonitoring(ULONG arg);
//static CHAR* _VDC_GetTailString(CHAR* sOrigin);
static void _VDC_SaveTS(VDC_HANDLE_T hVdcCh, UINT32 ui32StartAddr, UINT32 ui32EndAddr, SINT8 si8DecodedIndex);
static SINT64 _VDC_GenerateTimestamp(VDC_HANDLE_T hVdcCh, SINT64 *si64duration);
static SINT64 _VDC_DecideCurrentTs(VDC_HANDLE_T hVdcCh, SINT64 ts, SINT64 si64TimeStamp, UINT32 ui32AuStart, UINT32 ui32AuEnd, UINT32 ui32End);
static BOOLEAN _VDC_CheckContainToAddress(UINT32 ui32Start, UINT32 ui32End, UINT32 ui32TargetS, UINT32 ui32TargetE);
static BOOLEAN _VDC_CompenWraparound(VDC_HANDLE_T hVdcCh, UINT32 *ui32AuStart, UINT32 *ui32AuEnd, UINT32 *ui32Start,UINT32 *ui32End);
static UINT64 _VDC_DecideTS(VDC_HANDLE_T hVdcCh);
static void _VDC_DefineTsOrder(VDC_HANDLE_T hVdcCh);
static void _VDC_InsertTs(VDC_HANDLE_T hVdcCh, SINT64 ts);
#ifdef USE_ZEROPADDING_DETECT
static BOOLEAN _VDC_IsZeroPadded(VDC_HANDLE_T hVdcCh, UINT32 ui32Start, UINT32 ui32End);
#endif

#ifdef USE_WORKQUEUE_SCHEDULE
static void _VDC_KickScheduler(struct work_struct *work);
#endif

/*----------------------------------------------------------------------------------------
  Static Variable
  ----------------------------------------------------------------------------------------*/
static int vdc_force_codec_test = 0;
static VDC_CODEC_T vdc_force_codec_type;

#ifdef USE_WORKQUEUE_SCHEDULE
static DECLARE_DELAYED_WORK (kick_scheduler_work, _VDC_KickScheduler);
struct workqueue_struct	*vdc_wq;
#endif

/*----------------------------------------------------------------------------------------
  Generic Usage Function
  ----------------------------------------------------------------------------------------*/

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *	None
 * @return
 *	None
 ********************************************************************************
 */
void VDC_Init(void)
{
    VDC_HANDLE_T hVdcCh;

    // Initialize VDC DB
    for(hVdcCh=0; hVdcCh<VDEC_NUM_OF_CHANNEL; hVdcCh++)
    {
        init_waitqueue_head(&gstVdcDb[hVdcCh].wait_state_change);
        spin_lock_init(&gstVdcDb[hVdcCh].instance_lock);
        _VDC_InitializeDB(hVdcCh);
        init_timer( &gstVdcDb[hVdcCh].stChTimer );
        gstVdcDb[hVdcCh].stChTimer.function = _VDC_ChMonitoring;
        gstVdcDb[hVdcCh].stChTimer.data = (ULONG)hVdcCh;
    }

    gstVdcCoreInfo.ui32UseChNum = 0;
    gstVdcCoreInfo.ui32NumofIdleCh = 0;
    gstVdcCoreInfo.ui32PrevSelCh = VDC_INVALID_HANDLE;

#ifdef USE_WORKQUEUE_SCHEDULE
#if defined(CHIP_NAME_d13) || defined(CHIP_NAME_d14)
	vdc_wq = alloc_workqueue("vdc_events", 0, 0);
#else
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3,7,0))
	vdc_wq = alloc_workqueue("vdc_events", 0, 0);
#else
	vdc_wq = alloc_workqueue("vdc_events_highpri", WQ_HIGHPRI, 0);
#endif
#endif

	BUG_ON(!vdc_wq);
#endif

    // Init sub-module
    VDU_Init(_VDC_DecCallback);
    STC_TIMER_Init(_VDC_ChWatchDog);

    log_noti("Init done \n");

    /* make proc debug interface */
    VDC_ProcInit();
}

void VDC_Cleanup (void)
{
    VDC_ProcCleanup();
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *typedef struct
 {
 UINT8			ui8VesCh;    	// ves channel
 void			*priv;	     	// private datas
 VDC_SRC_T		ui8Src;      	// source of vdec (SDEC? Fileplay? TVP?)
 VDC_CODEC_T		ui8Vcodec;   	// video codec
 UINT32			ui32CpbBufAddr; // base address of cpb
 UINT32			ui32CpbBufSize; // total size of cpb
 clear_q_t		*clear_q;	// pointer of clear queue
 decoded_q_t		*decoded_q;	// pointer of decoded queue
 VDC_MEMORY_FORMAT_T	output_memory_format;	// map type of output
 VDC_LINEAR_MODE_T	extern_linear_buf_mode; // user allocation dpb mode
 VDC_OPEN_CONF_E eOpenConf; // open configuration for vdc
 } VDC_OPEN_PARAM_T;
 * @return
 *	VDC_HANDLE_T
 ********************************************************************************
 */
VDC_HANDLE_T VDC_Open(VDC_OPEN_PARAM_T* pstOpenParam)
{
    UINT32			ui32index;
    VDC_HANDLE_T		hVdcCh=VDC_INVALID_HANDLE;
    VDU_CODEC_T		eCodecType;
    VDU_OPEN_PARAM_T 	stOpenParam;
    CHAR*			sCodecName = NULL;
    //UINT32          chip;

    mutex_lock (&_VDC_IoMutex);

    if(pstOpenParam == NULL)
    {
        log_error("parameter is NULL\n");
        goto finish;
    }

    // allocate channel
    for(ui32index=0; ui32index<VDEC_NUM_OF_CHANNEL; ui32index++){
        if(gstVdcDb[ui32index].state == VDC_NULL){
            hVdcCh = ui32index;
            break;
        }
    }
    if( hVdcCh==VDC_INVALID_HANDLE ){
        log_error("open failed: no more available channel \n");
        goto finish;
    }

    if( pstOpenParam->eOpenConf & VDC_LIVE_CHANNEL ) {
        gstVdcDb[hVdcCh].bLive = TRUE;
		gstVdcDb[hVdcCh].eTsOrder = VDC_TS_PTS_ORDER;
    } else {
		gstVdcDb[hVdcCh].bLive = FALSE;
	}

    gstVdcDb[hVdcCh].eSrc = pstOpenParam->ui8Src;

	if (gstVdcDb[hVdcCh].eSrc <= VDC_SRC_SDEC2)
		gstVdcDb[hVdcCh].bUsePdec = TRUE;
	else
		gstVdcDb[hVdcCh].bUsePdec = FALSE;

    if( vdc_force_codec_test ) {
        pstOpenParam->ui8Vcodec = vdc_force_codec_type;
    }

    // TODO: is this converting process necessary?
    switch(pstOpenParam->ui8Vcodec)
    {
        case VDC_CODEC_H264_HP:		sCodecName="AVC"; eCodecType = VDU_CODEC_H264_AVC; break;
        case VDC_CODEC_MPEG2_HP:	sCodecName="MPEG2"; eCodecType = VDU_CODEC_MPEG2; break;
        case VDC_CODEC_MPEG4_ASP:	sCodecName="MPEG4"; eCodecType = VDU_CODEC_H263; break;
        case VDC_CODEC_DIVX3:		sCodecName="DIVX3"; eCodecType = VDU_CODEC_DIVX3; break;
        case VDC_CODEC_DIVX4:		sCodecName="DIVX4"; eCodecType = VDU_CODEC_DIVX4; break;
        case VDC_CODEC_DIVX5:		sCodecName="DIVX5"; eCodecType = VDU_CODEC_DIVX5; break;
        case VDC_CODEC_XVID:		sCodecName="XVID"; eCodecType = VDU_CODEC_XVID; break;
        case VDC_CODEC_VC1_RCV_V1:	sCodecName="VC1V1"; eCodecType = VDU_CODEC_VC1_RCV_V1; break;
        case VDC_CODEC_VC1_RCV_V2:	sCodecName="VC1V2"; eCodecType = VDU_CODEC_VC1_RCV_V2; break;
        case VDC_CODEC_VC1_ES:		sCodecName="VC1ES"; eCodecType = VDU_CODEC_VC1_ES; break;
        case VDC_CODEC_AVS:		sCodecName="AVS"; eCodecType = VDU_CODEC_AVS; break;
        case VDC_CODEC_RVX:		sCodecName="RVx"; eCodecType = VDU_CODEC_RVX; break;
        case VDC_CODEC_VP8:		sCodecName="VP8"; eCodecType = VDU_CODEC_VP8; break;
        case VDC_CODEC_H264_MVC:	sCodecName="MVC"; eCodecType = VDU_CODEC_H264_MVC; break;
        case VDC_CODEC_THEORA:		sCodecName="THEORA"; eCodecType = VDU_CODEC_THEORA; break;
        case VDC_CODEC_HEVC:		sCodecName="HEVC"; eCodecType = VDU_CODEC_HEVC; break;
        case VDC_CODEC_H263_SORENSON:	sCodecName="H.263"; eCodecType = VDU_CODEC_SORENSON_SPARK; break;
        case VDC_CODEC_VP9:			sCodecName="VP9"; eCodecType = VDU_CODEC_VP9; break;
        default: eCodecType = VDU_CODEC_INVALID; break;
    }

    gstVdcDb[hVdcCh].stCodecInfo.eCodecType = pstOpenParam->ui8Vcodec;
    gstVdcDb[hVdcCh].stCodecInfo.sName = sCodecName;
    stOpenParam.eCodecType = eCodecType;

    // set open parameters for openning vdu
    stOpenParam.ui32CPBAddr = pstOpenParam->ui32CpbBufAddr;
    stOpenParam.ui32CPBSize = pstOpenParam->ui32CpbBufSize;
    stOpenParam.bNoDelayMode = !!(pstOpenParam->eOpenConf & VDC_NO_DELAY);
    stOpenParam.bOneFrameDecoding = !!(pstOpenParam->eOpenConf & VDC_SINGLE_FRAME_DECODE);
    stOpenParam.bForceUhdEnable = !!(pstOpenParam->eOpenConf & VDC_ULTRA_HD);

    switch (pstOpenParam->eOpenDec)
    {
        default: // FALL-THROUGH
        case VDC_DEC_BODA:
            stOpenParam.eOpenDecoder = VDU_DEC_BODA_OPEN;
            break;
        case VDC_DEC_G1:
            stOpenParam.eOpenDecoder = VDU_DEC_G1_OPEN;
            break;
        case VDC_DEC_G2:
            stOpenParam.eOpenDecoder = VDU_DEC_G2_OPEN;
            break;
        case VDC_DEC_HEVC:
            stOpenParam.eOpenDecoder = VDU_DEC_HEVC_OPEN;
            break;
    }

	if (pstOpenParam->output_memory_format == VDC_MEMORY_FORMAT_RASTER) {
	    switch(pstOpenParam->extern_linear_buf_mode)
	    {
	        default: // FALL-THROUGH
	        case VDC_LINEAR_MODE_NONE:
	            stOpenParam.eExternLinearBufMode = VDU_LINEAR_MODE_NONE;
	            break;
	        case VDC_LINEAR_MODE_STATIC:
	            stOpenParam.eExternLinearBufMode = VDU_LINEAR_MODE_STATIC;
	            break;
	        case VDC_LINEAR_MODE_DYNAMIC:
	            stOpenParam.eExternLinearBufMode = VDU_LINEAR_MODE_DYNAMIC;
	            break;
	    }
	} else {
		stOpenParam.eExternLinearBufMode = VDU_LINEAR_MODE_OFF;
	}
    stOpenParam.ui16ValidThreshold = gstVdcDb[hVdcCh].bLive?VDC_LIVE_PSEARCH_THRESHOLD:VDC_FILEPLAY_PSEARCH_THRESHOLD;

    // open vdu
    if( VDU_RET_OK != VDU_Open(&gstVdcDb[hVdcCh].hVduCh, &stOpenParam) )
    {
        log_error("open failed: invalid vdu channel \n");
        hVdcCh = VDC_INVALID_HANDLE;
        goto finish;
    }

    // managing vdc channel info
    gstVdcDb[hVdcCh].state = VDC_READY;
    gstVdcDb[hVdcCh].priv = pstOpenParam->priv;
    gstVdcDb[hVdcCh].ui32VesCh = pstOpenParam->ui8VesCh;
    gstVdcDb[hVdcCh].clear_q = pstOpenParam->clear_q;
    gstVdcDb[hVdcCh].decoded_q = pstOpenParam->decoded_q;
    gstVdcDb[hVdcCh].eLinearBufMode = pstOpenParam->extern_linear_buf_mode;
	gstVdcDb[hVdcCh].state_changed = pstOpenParam->state_changed;

    /* timestamp fifo for dts order */
    if (kfifo_alloc(&gstVdcDb[hVdcCh].stTSfifo,sizeof(SINT64)*VDC_MAX_DPB_SIZE,GFP_KERNEL))
    {
        log_error("ch:%d kfifo alloc failed \n",hVdcCh);
        hVdcCh = VDC_INVALID_HANDLE;
        goto finish;
    }

    if( pstOpenParam->extern_linear_buf_mode > VDC_LINEAR_MODE_NONE ) {

        gstVdcDb[hVdcCh].stExtBufsInfo.aui32AddressList
            = (UINT32*)kmalloc(sizeof(UINT32) * VDC_MAX_DPB_SIZE, GFP_KERNEL);
        if( gstVdcDb[hVdcCh].stExtBufsInfo.aui32AddressList == NULL )
        {
            log_error("ch:%d kmalloc failed \n",hVdcCh);
            hVdcCh = VDC_INVALID_HANDLE;
            kfifo_free(&gstVdcDb[hVdcCh].stTSfifo);
            goto finish;
        }
        _VDC_InitExtBufsInfo(&gstVdcDb[hVdcCh].stExtBufsInfo);
    }
    gstVdcCoreInfo.ui32UseChNum++;

    feeddump_dumped_count[hVdcCh] = 0;
    feeddump_header_found = FALSE;
    feeddump_dump(gstVdcDb[hVdcCh].ui32VesCh, hVdcCh,
            FEED_CMD_OPEN,
            0, (unsigned char*)&pstOpenParam->ui8Vcodec,
            sizeof(UINT32));

    log_noti("opened VDCch:%d VDUch:%d VESch:%d clear_q:%p decoded_q:%p codec:%s usage:%d linear_buf_mode:%d psearch:%d \n"
            ,hVdcCh, gstVdcDb[hVdcCh].hVduCh, gstVdcDb[hVdcCh].ui32VesCh
            ,gstVdcDb[hVdcCh].clear_q,gstVdcDb[hVdcCh].decoded_q, gstVdcDb[hVdcCh].stCodecInfo.sName
            ,gstVdcCoreInfo.ui32UseChNum, pstOpenParam->extern_linear_buf_mode
            ,stOpenParam.ui16ValidThreshold);

finish:
    mutex_unlock (&_VDC_IoMutex);
    return hVdcCh;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 ********************************************************************************
 */
VDC_RET_T VDC_Close(VDC_HANDLE_T hVdcCh)
{
    //UINT32 		irq_enabled;
    VDC_RET_T	eRetCode = VDC_RET_INVALID;
    ULONG 		flags=0;
    ULONG 		flags2=0;

    if( hVdcCh >= VDEC_NUM_OF_CHANNEL )
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    spin_lock_irqsave(&gstVdcDb[hVdcCh].instance_lock, flags);
    spin_lock_irqsave(&_VDC_Spinlock, flags2);

    /* disable irq */
    //irq_enabled = TOP_HAL_DisableIntr (BUFFER_STATUS);

    gstVdcDb[hVdcCh].state = VDC_CLOSING;

    spin_unlock_irqrestore(&_VDC_Spinlock, flags2);
    spin_unlock_irqrestore(&gstVdcDb[hVdcCh].instance_lock, flags);

    mutex_lock( &_VDC_IoMutex );

    feeddump_dump(gstVdcDb[hVdcCh].ui32VesCh, hVdcCh,
            FEED_CMD_CLOSE,
            0, NULL, 0);

    /* close vdc */
    if(_VDC_Close(hVdcCh))
        eRetCode = VDC_RET_OK;
    else
        eRetCode = VDC_RET_ERROR;

    mutex_unlock( &_VDC_IoMutex );

    /* enable the irq */

    /*
       if (irq_enabled)
       TOP_HAL_EnableIntr (BUFFER_STATUS);
       else
       log_warning ("do not enable interrupt\n");*/
finish:
    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_Reset(VDC_HANDLE_T hVdcCh)
{
    VDC_RET_T eRetCode = VDC_RET_INVALID;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    mutex_lock (&_VDC_IoMutex);

    // VDU Instance Reset
    if(gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE
            && VDU_Reset(gstVdcDb[hVdcCh].hVduCh) == VDU_RET_OK)
    {
        // rd off set to wr off set
        log_noti("ch:%d reset success \n",hVdcCh);
        _VDC_InitInnerDatas(hVdcCh);
        eRetCode = VDC_RET_OK;
    }
    else
    {
        log_error("ch:%d reset failed \n",hVdcCh);
        eRetCode = VDC_RET_ERROR;
    }

    mutex_unlock (&_VDC_IoMutex);
finish:
    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_Start(VDC_HANDLE_T hVdcCh)
{
    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        return VDC_RET_ERROR;
    }
    log_noti("ch:%d Starts \n", hVdcCh);

    // monitoring timer for debugging
    if( gstVdcDb[hVdcCh].state != VDC_PLAY ) {
        del_timer( &gstVdcDb[hVdcCh].stChTimer );
        gstVdcDb[hVdcCh].stDebugInfo.ui64TotalPlayedTime = get_jiffies_64();
        mod_timer( &gstVdcDb[hVdcCh].stChTimer, get_jiffies_64() + 3 * HZ );
    }
    gstVdcDb[hVdcCh].state = VDC_PLAY;

    return VDC_RET_OK;
}
/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_Update(VDC_HANDLE_T hVdcCh)
{
    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        return VDC_RET_ERROR;
    }
    _VDC_ClearUsedDPB(hVdcCh);
    log_trace("ch:%d, Feed by VDC Update \n",hVdcCh);

#ifdef USE_WORKQUEUE_SCHEDULE
	queue_delayed_work(vdc_wq, &kick_scheduler_work, 0);
#else
	_VDC_Scheduler();
#endif
    return VDC_RET_OK;
}
/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_Flush(VDC_HANDLE_T hVdcCh)
{
    UINT32		idx;
    VDC_RET_T	eRetCode = VDC_RET_INVALID;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL ||gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    mutex_lock (&_VDC_IoMutex);
    feeddump_dumped_count[hVdcCh] = 0;
    feeddump_header_found = FALSE;

    if( gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE
            && VDU_Flush(gstVdcDb[hVdcCh].hVduCh) == VDU_RET_OK )
    {
        // initialize timestamp & feeded addr
        gstVdcDb[hVdcCh].si64prevSavedTS=VDC_UNKNOWN_PTS;
        gstVdcDb[hVdcCh].si64prevSentTS=VDC_UNKNOWN_PTS;
        kfifo_reset(&gstVdcDb[hVdcCh].stTSfifo);
        gstVdcDb[hVdcCh].bRemainedData = FALSE;
        gstVdcDb[hVdcCh].bForceDecoding = FALSE;
        gstVdcDb[hVdcCh].bFlushed = TRUE;
        gstVdcDb[hVdcCh].bNPFOccured = FALSE;
        gstVdcDb[hVdcCh].ui32FeededCounter = 0;
        atomic_set(&gstVdcDb[hVdcCh].atIsDecoding, 0);
        gstVdcDb[hVdcCh].ui32PrevRdPtr = VDC_INVALID_ADDRESS;
        gstVdcDb[hVdcCh].ui32FeedStartTime = TOP_HAL_GetGSTCC();
        for(idx = 0;idx<gstVdcDb[hVdcCh].ui32DPBSize;idx++)
        {
            gstVdcDb[hVdcCh].si64UTSInfo[idx] = VDC_INVALID_TIMESTAMP;
            gstVdcDb[hVdcCh].si64TSInfo[idx] = VDC_INVALID_TIMESTAMP;
            // clear decoded but not displaying flags
            if(gstVdcDb[hVdcCh].ui32FrameFlags[idx] == 0)
                gstVdcDb[hVdcCh].ui32DecodedFlags &= ~(1<<idx);
        }

        gstVdcDb[hVdcCh].ui32EOSWaitCounter = 0;
        gstVdcDb[hVdcCh].bFeedEOS = FALSE;

        log_noti("ch:%d state:%d Flush Done \n",hVdcCh,gstVdcDb[hVdcCh].state);
        eRetCode = VDC_RET_OK;
    }
    else
    {
        log_error("ch:%d state:%d Flush failed \n",hVdcCh,gstVdcDb[hVdcCh].state);
        eRetCode = VDC_RET_ERROR;
    }

    feeddump_dump(gstVdcDb[hVdcCh].ui32VesCh,hVdcCh,
            FEED_CMD_FLUSH,
            0,NULL,0);

    mutex_unlock (&_VDC_IoMutex);

#ifdef USE_WORKQUEUE_SCHEDULE
	flush_workqueue(vdc_wq);
#endif

finish:
    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_Stop(VDC_HANDLE_T hVdcCh)
{
    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        return VDC_RET_ERROR;
    }
    gstVdcDb[hVdcCh].state = VDC_STOP;
    log_trace("ch:%d, Stopped \n",hVdcCh);
    return VDC_RET_OK;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_CODEC_STATE_T VDC_GetCoreState(VDC_HANDLE_T hVdcCh, wait_queue_head_t **wait)
{
    VDC_CODEC_STATE_T chStatus = VDC_CODEC_STATE_NULL;
    VDU_STATUS_T eStatus = VDU_STATUS_NULL;
    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        goto finish;
    }

    if(gstVdcDb[hVdcCh].hVduCh == VDU_INVALID_HANDLE ||
            VDU_GetStatus(gstVdcDb[hVdcCh].hVduCh,&eStatus) != VDU_RET_OK)
        goto finish;
    else{
        switch(eStatus){
            case VDU_STATUS_BUSY:
                chStatus = VDC_CODEC_STATE_BUSY;
                break;
            case VDU_STATUS_READY:
                chStatus = VDC_CODEC_STATE_READY;
                break;
            default:
                break;
        }
    }

    if (wait)
        *wait = &gstVdcDb[hVdcCh].wait_state_change;
finish:
    return chStatus;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_SetSkipMode(VDC_HANDLE_T hVdcCh, VDC_SKIP_T eSkipMode)
{
    VDC_RET_T 	eRetCode = VDC_RET_INVALID;
    VDU_OPTION_T 	stOption = VDU_OPTION_INIT_VALUE;
    FEEDDUMP_CMD_T  eSkipCmd = FEED_CMD_NONE;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    switch(eSkipMode){
        case VDC_SKIP_NONE:
            stOption.ePicScanMode = VDU_PIC_SCAN_ALL;
            eSkipCmd = FEED_CMD_SCAN_ALL;
            break;
        case VDC_SKIP_B:
            stOption.ePicScanMode = VDU_PIC_SCAN_IP;
            eSkipCmd = FEED_CMD_SKIP_B;
            break;
        case VDC_SKIP_PB:
            stOption.ePicScanMode = VDU_PIC_SCAN_I;
            eSkipCmd = FEED_CMD_SCAN_I;
            break;
        case VDC_SKIP_DISH:
            stOption.ePicScanMode = VDU_PIC_SCAN_I_FIELD;
            eSkipCmd = FEED_CMD_SKIP_DISH;
            break;
    }

    if(gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE &&
            VDU_SetOption(gstVdcDb[hVdcCh].hVduCh, &stOption) == VDU_RET_OK){
        gstVdcDb[hVdcCh].ui32ScanMode = eSkipMode;
        // for feeddump
        feeddump_dump(gstVdcDb[hVdcCh].ui32VesCh,hVdcCh,
                eSkipCmd,
                0,NULL,0);
        log_noti("ch:%d skip mode set done:%d \n",hVdcCh,stOption.ePicScanMode);
        eRetCode = VDC_RET_OK;
    }
    else{
        log_error("ch:%d skip mode set failed \n",hVdcCh);
        eRetCode = VDC_RET_ERROR;
    }
finish:
    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
VDC_RET_T VDC_SetUserDataOutput(VDC_HANDLE_T hVdcCh, BOOLEAN bEnable)
{
    VDC_RET_T    eRetCode = VDC_RET_INVALID;
    VDU_OPTION_T stOption = VDU_OPTION_INIT_VALUE;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY
            || gstVdcDb[hVdcCh].hVduCh == VDU_INVALID_HANDLE)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    if( !bEnable ) {
        log_noti("ch:%d no userdata \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    stOption.eUserData = bEnable;
    if(VDU_SetOption(gstVdcDb[hVdcCh].hVduCh, &stOption) == VDU_RET_OK){
        log_noti("ch:%d userdata enabled \n",hVdcCh);
    } else {
        log_noti("ch:%d userdata disabled \n",hVdcCh);
    }
    eRetCode = VDC_RET_OK;
finish:
    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 0 = succe
 -1 = fail
 *
 ********************************************************************************
 */

VDC_RET_T VDC_GetFrame(VDC_HANDLE_T hVdcCh, UINT8 ui8FrameNum, void* vpFrameBuf)
{
    VDC_RET_T	eRetCode = VDC_RET_INVALID;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    if( gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE &&
            VDU_GetLinearFrame(gstVdcDb[hVdcCh].hVduCh, ui8FrameNum, vpFrameBuf) == VDU_RET_OK){
        log_trace("ch:%d get frame success idx:%d \n",hVdcCh,ui8FrameNum);
        eRetCode = VDC_RET_OK;
    }
    else {
        log_warning("ch:%d get frame failed idx:%d \n", hVdcCh,ui8FrameNum);
        eRetCode = VDC_RET_ERROR;
    }
finish:
    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 0 = succe
 -1 = fail
 *
 ********************************************************************************
 */

VDC_RET_T VDC_Suspend(void)
{
    VDC_RET_T	eRetCode = VDC_RET_INVALID;

    VDU_Suspend();
    log_noti("vdc suspended \n");
    eRetCode = VDC_RET_OK;

    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 0 = succe
 -1 = fail
 *
 ********************************************************************************
 */

VDC_RET_T VDC_Resume(void)
{
    VDC_RET_T	eRetCode = VDC_RET_INVALID;

    VDU_Resume();
    log_noti("vdc resumed \n");
    eRetCode = VDC_RET_OK;

    return eRetCode;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 0 = succe
 -1 = fail
 *
 ********************************************************************************
 */
VDC_RET_T VDC_SetExternDpb(VDC_HANDLE_T hVdcCh, VDC_LINEAR_FRAME_BUF_T stFrameBufInfo)
{
    VDC_RET_T	eRetCode = VDC_RET_INVALID;
    UINT8		*ui8Cur;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        eRetCode = VDC_RET_ERROR;
        goto finish;
    }

    mutex_lock( &_VDC_IoMutex );

    ui8Cur = &gstVdcDb[hVdcCh].stExtBufsInfo.ui8NumberOfFrames;

    gstVdcDb[hVdcCh].stExtBufsInfo.aui32AddressList[*ui8Cur] = stFrameBufInfo.ui32BufAddr;

    log_noti("ch:%d stride:%d addr:0x%X list size:%d \n", hVdcCh, stFrameBufInfo.ui32Stride,
            gstVdcDb[hVdcCh].stExtBufsInfo.aui32AddressList[*ui8Cur],
            gstVdcDb[hVdcCh].stExtBufsInfo.ui8NumberOfFrames);

    (*ui8Cur)++;
    gstVdcDb[hVdcCh].stExtBufsInfo.ui32Stride = stFrameBufInfo.ui32Stride;

    gstVdcDb[hVdcCh].eWaitingBuffer = VDC_USRDPB_STATE_SET;

    mutex_unlock( &_VDC_IoMutex );

finish:
    return eRetCode;
}

/*----------------------------------------------------------------------------------------
  Static Function Definition
  ----------------------------------------------------------------------------------------*/
static UINT32 _VDC_WorkReadPtr(VDC_HANDLE_T hVdcCh, UINT32 ui32ReadPtr, VDU_NOTI_T eNotiType, SINT8 si8DecodedIndex, SINT8 si8DisplayIndex)
{
    UINT32 ui32Consumed = 0;
    UINT32 ui32Diff;
    BOOLEAN bWraparound = FALSE;
    // below condition means 1 frame decoded successfully
    if( gstVdcDb[hVdcCh].ui32PrevRdPtr != VDC_INVALID_ADDRESS &&
            ui32ReadPtr != VDU_INVALID_ADDRESS )
    {
        /* adjust boda decoder's read pointer what vdc needs */
        switch (gstVdcDb[hVdcCh].stCodecInfo.eCodecType)
        {
            default:
                break;
            case VDC_CODEC_H264_HP:
                /* in case of h264 live, hw pdec aligning 512 bytes &
                 * boda's readptr consuming without zero-padded region.
                 * so compensating the changes */
                if (gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec)
                {
                    log_user1("ch:%d h264 read ptr: 0x%X, aligned:0x%X \n"
                            ,hVdcCh,ui32ReadPtr,CEILING_BYTES(ui32ReadPtr));
                    ui32ReadPtr = CEILING_BYTES(ui32ReadPtr);
                }
                break;
        }

        switch(eNotiType)
        {
            default:
                break;
            case VDU_NOTI_DISPLAY:
                /* some case of vc1 decodes one frame and display it many time.
                 * so that case needs another 5 bytes garbage chunk which
                 * has timestamp. */
                log_user1("ch:%d save vc1 displaying ts rd:%X nt:%d \n",hVdcCh, ui32ReadPtr,
                          eNotiType);
                if (unlikely(gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_VC1_ES
                            && si8DisplayIndex >= 0
                        && gstVdcDb[hVdcCh].ui32FrameFlags[si8DisplayIndex]>0))
                {
                    _VDC_SaveTS(hVdcCh,gstVdcDb[hVdcCh].ui32PrevRdPtr,ui32ReadPtr,si8DisplayIndex);
                }
                break;
                // handling one frame decoding success case
            case VDU_NOTI_SUCCESS:
            case VDU_NOTI_CPB_EMPTY:
                log_user1("ch:%d save ts rd:%X nt:%d \n",hVdcCh, ui32ReadPtr,
                          eNotiType);
                // decide one timestamp for current decoded frame
                _VDC_SaveTS(hVdcCh,gstVdcDb[hVdcCh].ui32PrevRdPtr,ui32ReadPtr,si8DecodedIndex);
                // exception for handling dynamic(user) dpb allocation mode
                if( gstVdcDb[hVdcCh].eLinearBufMode != VDC_LINEAR_MODE_DYNAMIC
                        && si8DecodedIndex >= 0)
                    gstVdcDb[hVdcCh].ui32DecodedFlags |= (1<<si8DecodedIndex);
                VES_CPB_UpdateRdPtr (gstVdcDb[hVdcCh].ui32VesCh, ui32ReadPtr);
                break;
            case VDU_NOTI_FRAME_SKIP:
            case VDU_NOTI_SKIP_EMPTY:
                log_user1("ch:%d frame skipped rd:%X nt:%d \n",hVdcCh, ui32ReadPtr,
                          eNotiType);
                /* just for update current read pointer */
                _VDC_SaveTS(hVdcCh,gstVdcDb[hVdcCh].ui32PrevRdPtr,ui32ReadPtr,si8DecodedIndex);
                VES_CPB_UpdateRdPtr (gstVdcDb[hVdcCh].ui32VesCh, ui32ReadPtr);
                break;
            case VDU_NOTI_DECODING_ERROR:
                /* update ves cpb read pointer to last feeded pointer for the error case */
                VES_CPB_UpdateRdPtr (gstVdcDb[hVdcCh].ui32VesCh, gstVdcDb[hVdcCh].ui32LastFeededPtr);
                break;
        }
        // consumed information for debugging
        if( gstVdcDb[hVdcCh].ui32PrevRdPtr < ui32ReadPtr )
            ui32Consumed = GET_DIFF(gstVdcDb[hVdcCh].ui32PrevRdPtr,ui32ReadPtr);

        // update current readptr to previous
        gstVdcDb[hVdcCh].ui32PrevRdPtr = ui32ReadPtr;

        // decide remained data at hw decoder
        if( gstVdcDb[hVdcCh].ui32LastFeededPtr != ui32ReadPtr)
            gstVdcDb[hVdcCh].bRemainedData = TRUE;
        else
            gstVdcDb[hVdcCh].bRemainedData = FALSE;

        ui32Diff = GET_DIFF(gstVdcDb[hVdcCh].ui32LastFeededPtr, ui32ReadPtr);
        bWraparound = (gstVdcDb[hVdcCh].ui32LastFeededPtr < ui32ReadPtr)?TRUE:FALSE;

        log_trace ("ch:%d remained?%d rd:%x feeded:%x diff:%x wrap:%d \n",
                hVdcCh, gstVdcDb[hVdcCh].bRemainedData,
                ui32ReadPtr, gstVdcDb[hVdcCh].ui32LastFeededPtr,
                ui32Diff, bWraparound);
    }

    return ui32Consumed;

}

static unsigned int _VDC_WorkReport(VDC_HANDLE_T hVdcCh, VDU_NOTI_T eNotiType, decoded_buffer_t *stDecBuf, VDU_FRM_INFO_T *pstFrameInfo)
{
    unsigned int report = 0;
    UINT32 ui32Diff = 0;
    UINT32 ui32CurTime = 0;

    ui32CurTime = TOP_HAL_GetGSTCC();
    ui32Diff = GET_SIZE(gstVdcDb[hVdcCh].ui32FeedStartTime, ui32CurTime, 0xFFFFFFFF);

    // If feeded enough chunks but decoder cannot decode or skip,
    // closing channel report sent
    if(gstVdcDb[hVdcCh].ui32DPBSize!=VDC_INITIAL_DPB_SIZE
            && ui32Diff > VDC_FEEDING_WAIT_THRESHOLD
            && gstVdcDb[hVdcCh].ui32FeededCounter > VDC_FEEDING_NUMBER_THERSHOLD){
        log_noti("ch:%d FeededTime(%d) exceeded over threshold (count:%d)", hVdcCh,ui32Diff,gstVdcDb[hVdcCh].ui32FeededCounter);
        report |= DECODED_REPORT_DECODE_FAILED;
        gstVdcDb[hVdcCh].ui32FeededCounter = 0;
        gstVdcDb[hVdcCh].ui32FeedStartTime = TOP_HAL_GetGSTCC();
    }

    if (gstVdcDb[hVdcCh].bFeedEOS)
    {
        if (eNotiType == VDU_NOTI_SUCCESS || eNotiType == VDU_NOTI_DISPLAY)
        {
            gstVdcDb[hVdcCh].ui32EOSWaitCounter = 0;

            log_noti("ch:%d reset ui32EOSWaitCounter\n", hVdcCh);
        }
        else if (eNotiType == VDU_NOTI_CPB_EMPTY || eNotiType == VDU_NOTI_DECODING_ERROR)
        {
            gstVdcDb[hVdcCh].ui32EOSWaitCounter++;

            if (gstVdcDb[hVdcCh].ui32EOSWaitCounter > gstVdcDb[hVdcCh].ui32DPBSize)
            {
                gstVdcDb[hVdcCh].bForceDecoding = FALSE;
                gstVdcDb[hVdcCh].bFeedEOS = FALSE;
                gstVdcDb[hVdcCh].ui32EOSWaitCounter = 0;

                report |= DECODED_REPORT_EOS;

                log_noti("ch:%d send EOS by timeout\n", hVdcCh);
            }
        }
    }

    // processing for other report info
    switch (eNotiType)
    {
        case VDU_NOTI_HEADER_NOT_FOUND:
            if(!gstVdcDb[hVdcCh].bLive && vdc_unlimit_retry == 0){
                if(gstVdcDb[hVdcCh].eNotSupportNotiValue == VDC_NOT_SUPPORT_NOTI_TRIGGERED &&
                        gstVdcDb[hVdcCh].ui32SeqRetryCounter >= VDC_MAX_RETRY_COUNT_FOR_FAILED){
                    log_noti("ch:%d VDC report not supported, flag : %d\n", hVdcCh, gstVdcDb[hVdcCh].eNotSupportNotiValue);

                    report |= DECODED_REPORT_NOT_SUPPORTED_STREAM;
                    gstVdcDb[hVdcCh].eNotSupportNotiValue = VDC_NOT_SUPPORT_NOTI_REPORTED;
                }
                else if(gstVdcDb[hVdcCh].ui32SeqRetryCounter >= VDC_MAX_RETRY_COUNT_FOR_NOT_FOUND){
                    report |= DECODED_REPORT_SEQUENCE_INIT_FAILED;
                }
            }
            break;
        case VDU_NOTI_NOT_SUPPORTED:
            log_noti("ch:%d VDU notice Not Supported, flag : %d\n", hVdcCh, gstVdcDb[hVdcCh].eNotSupportNotiValue);
            if(!gstVdcDb[hVdcCh].bLive && vdc_unlimit_retry == 0){

                if(gstVdcDb[hVdcCh].eNotSupportNotiValue != VDC_NOT_SUPPORT_NOTI_TRIGGERED){
                    gstVdcDb[hVdcCh].eNotSupportNotiValue = VDC_NOT_SUPPORT_NOTI_TRIGGERED;
                }
                else if(gstVdcDb[hVdcCh].ui32SeqRetryCounter >= VDC_MAX_RETRY_COUNT_FOR_FAILED){
                    log_noti("ch:%d VDC report not supported, flag : %d\n", hVdcCh, gstVdcDb[hVdcCh].eNotSupportNotiValue);

                    report |= DECODED_REPORT_NOT_SUPPORTED_STREAM;
                    gstVdcDb[hVdcCh].eNotSupportNotiValue = VDC_NOT_SUPPORT_NOTI_REPORTED;
                }
            }
            break;
            //case VDU_NOTI_DECODING_ERROR:
            //report += DECODED_REPORT_DECODE_FAILED; break;
        case VDU_NOTI_LACK_OF_RESOURCES:
            report |= DECODED_REPORT_RESOURCE_BUSY;
            break;
        case VDU_NOTI_END_OF_STREAM:
            report |= DECODED_REPORT_EOS;
            log_noti("ch:%d VDU notice EOS\n", hVdcCh);
            break;
        case VDU_NOTI_DPB_EMPTY:
            {
                if( gstVdcDb[hVdcCh].stExtBufsInfo.ui8NumberOfFrames == 0 )
                {
                    gstVdcDb[hVdcCh].eWaitingBuffer = VDC_USRDPB_STATE_EMPTY;
                    stDecBuf->num_of_buffer_required = pstFrameInfo->ui8NumOfFrames;
                    stDecBuf->width = pstFrameInfo->stRect.ui32PicWidth;
                    stDecBuf->height = pstFrameInfo->stRect.ui32PicHeight;
                    gstVdcDb[hVdcCh].ui32DPBSize = pstFrameInfo->ui8NumOfFrames;
                    report |= DECODED_REPORT_REQUEST_DPB;
                }
            }
            break;
        default: break;
    }

    return report;
}

static void _VDC_MakeDispInfo(VDC_HANDLE_T hVdcCh, decoded_buffer_t *stDecBuf, VDU_FRM_INFO_T *pstFrameInfo)
{
    stDecBuf->stride = pstFrameInfo->stRect.ui32Stride;
    stDecBuf->width = pstFrameInfo->stRect.ui32PicWidth;
    stDecBuf->height = pstFrameInfo->stRect.ui32PicHeight;
    stDecBuf->crop_left = pstFrameInfo->stRect.ui16CropLeft;
    stDecBuf->crop_right = pstFrameInfo->stRect.ui16CropRight;
    stDecBuf->crop_top = pstFrameInfo->stRect.ui16CropTop;
    stDecBuf->crop_bottom = pstFrameInfo->stRect.ui16CropBottom;

    stDecBuf->error_blocks = pstFrameInfo->stDispInfo.ui8ErrorRate;
    switch (pstFrameInfo->stDispInfo.eFrameScanType)
    {
        default: // FALL-THROUGH
        case VDU_FRM_SCAN_PROGRESSIVE:
            stDecBuf->interlace = decoded_buffer_interlace_none; break;
        case VDU_FRM_SCAN_TOP_FIRST:
            stDecBuf->interlace = decoded_buffer_interlace_top_first; break;
        case VDU_FRM_SCAN_BOTTOM_FIRST:
            stDecBuf->interlace = decoded_buffer_interlace_bottom_first; break;
        case VDU_FRM_SCAN_TOP_ONLY:
            stDecBuf->interlace = decoded_buffer_interlace_top_only; break;
        case VDU_FRM_SCAN_BOTTOM_ONLY:
            stDecBuf->interlace = decoded_buffer_interlace_bottom_only; break;
    }
    switch (pstFrameInfo->stDispInfo.eOrderOf3D)
    {
        default: // FALL-THROUGH
        case VDU_FRM_3D_NONE:
            stDecBuf->multi_picture = decoded_buffer_multi_picture_none;	break;
        case VDU_FRM_3D_LEFT:
            stDecBuf->multi_picture = decoded_buffer_multi_picture_left;	break;
        case VDU_FRM_3D_RIGHT:
            stDecBuf->multi_picture = decoded_buffer_multi_picture_right;break;
    }

    stDecBuf->frame_packing_arrangement = pstFrameInfo->stDispInfo.si8FramePackArrange;
    stDecBuf->display_width = pstFrameInfo->stDispInfo.si32Mp2DispWidth;
    stDecBuf->display_height = pstFrameInfo->stDispInfo.si32Mp2DispHeight;

    stDecBuf->addr_y = pstFrameInfo->stAddress.ui32Y;
    stDecBuf->addr_cb = pstFrameInfo->stAddress.ui32Cb;
    stDecBuf->addr_cr = pstFrameInfo->stAddress.ui32Cr;
    if(pstFrameInfo->stFrameRate.ui32Divider == 0){
        stDecBuf->framerate_num = 0;
        stDecBuf->framerate_den = 1;
    }
    else if(pstFrameInfo->stFrameRate.ui32Residual / pstFrameInfo->stFrameRate.ui32Divider < 1)
    {
        stDecBuf->framerate_num = 0;
        stDecBuf->framerate_den = 1;
    }
    else{
        stDecBuf->framerate_num = pstFrameInfo->stFrameRate.ui32Residual;
        if(gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_H264_MVC)
            stDecBuf->framerate_num *= 2;
        stDecBuf->framerate_den = pstFrameInfo->stFrameRate.ui32Divider;

        gstVdcDb[hVdcCh].ui32FrameRate = stDecBuf->framerate_num / stDecBuf->framerate_den *100;
        gstVdcDb[hVdcCh].ui32FrameRate += (stDecBuf->framerate_num % stDecBuf->framerate_den * 100 ) / stDecBuf->framerate_den;
    }
    stDecBuf->ui8Mpeg2Dar = pstFrameInfo->stAspectRatio.ui8Mpeg2Dar;
    stDecBuf->par_w = pstFrameInfo->stAspectRatio.ui16ParW;
    stDecBuf->par_h = pstFrameInfo->stAspectRatio.ui16ParH;
    // variable frame rate not used

    if(pstFrameInfo->stDispInfo.bLowDelay == 1) stDecBuf->report += DECODED_REPORT_LOW_DELAY;

    // push display info
    stDecBuf->picture_type = pstFrameInfo->stDispInfo.ui8PictureType;
    stDecBuf->display_period = pstFrameInfo->stDispInfo.ui8DisplayPeriod;
    stDecBuf->active_format = pstFrameInfo->stUserData.ui8ActiveFmtDesc;

    // user data
    stDecBuf->user_data_addr = (void*)pstFrameInfo->stUserData.ui32Address;
    stDecBuf->user_data_size = pstFrameInfo->stUserData.ui32Size;
    stDecBuf->top_field_first = pstFrameInfo->stDispInfo.bTopFieldFirst;
    stDecBuf->repeat_first_field = pstFrameInfo->stDispInfo.bRepeatFirstField;

    // default value(pushing dq): bFieldPicture=FALSE, ui32UId = 0

    // buffer private
    stDecBuf->decoder_priv = pstFrameInfo->pstMapInfo;
}

static void _VDC_MakeDebugInfo(VDC_HANDLE_T hVdcCh, CHAR* *pic_type, CHAR* *interlace, decoded_buffer_t *stDecBuf, UINT32 ui32Consumed, SINT64 ts)
{
    switch (stDecBuf->picture_type)
    {
        case decoded_buffer_picture_type_i:	*pic_type = "I";	break;
        case decoded_buffer_picture_type_p:	*pic_type = "P";	break;
        case decoded_buffer_picture_type_b:	*pic_type = "B";	break;
        case decoded_buffer_picture_type_bi: 	*pic_type = "BI"; 	break;
        case decoded_buffer_picture_type_d: 	*pic_type = "D";	break;
        case decoded_buffer_picture_type_s:	*pic_type = "S";	break;
        case decoded_buffer_picture_type_pskip:	*pic_type = "PSKIP";	break;
        default: *pic_type = "NONE"; break;
    }

    switch (stDecBuf->interlace)
    {
        case decoded_buffer_interlace_top_first: // FALL-THROUGH
        case decoded_buffer_interlace_bottom_first: *interlace = "I"; break;
        case decoded_buffer_interlace_none: *interlace = "P"; break;
        case decoded_buffer_interlace_top_only: // FALL-THROUGH
        case decoded_buffer_interlace_bottom_only: *interlace = "HI"; break; // "HI":Half Intarlace
        default: *interlace = "N"; break;
    }

    gstVdcDb[hVdcCh].stDebugInfo.sInterProg = *interlace;
    gstVdcDb[hVdcCh].stDebugInfo.ui32Width = stDecBuf->width;
    gstVdcDb[hVdcCh].stDebugInfo.ui32Height = stDecBuf->height;
    gstVdcDb[hVdcCh].stDebugInfo.ui64LastPts = ts;
    gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic++;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotDisplayedPic++;
    gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedSize += ui32Consumed;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotConsumedSize += ui32Consumed;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotDecodingTime += GET_DIFF(gstVdcDb[hVdcCh].ui32DecodingTime,TOP_HAL_GetGSTCC());
    gstVdcDb[hVdcCh].stDebugInfo.ui32FramePackingArrange = stDecBuf->frame_packing_arrangement;
    gstVdcDb[hVdcCh].stDebugInfo.ui32LRInfo = stDecBuf->multi_picture;
    if(stDecBuf->error_blocks>0)
        gstVdcDb[hVdcCh].stDebugInfo.ui32NumofDecodedWithError++;

    if( feeddump_mode == feeddump_decdone )
        feeddump_dumped_count[hVdcCh]++;
}

static void _VDC_PrintPushInfo(VDC_HANDLE_T hVdcCh, CHAR* pic_type, CHAR* interlace, decoded_buffer_t *stDecBuf, UINT64 ui64TsForDiv, UINT64 ui64_90kts)
{
    if(gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec){
        log_info("ch:%d PUSH!![%s][%s][%dx%d@%2d.%02d%s(%d:%d)]disp:%2d auib:%03d "
                 "cpb:%06X rd:%06X dpb:%02d/%d LR:%d err:%2d fpa:%d fld:%d "
                 "dt:%5dus pts:%9u(0x%9u) \n"
                ,hVdcCh,pic_type,gstVdcDb[hVdcCh].stCodecInfo.sName
                ,stDecBuf->width,stDecBuf->height
                ,gstVdcDb[hVdcCh].ui32FrameRate/100,gstVdcDb[hVdcCh].ui32FrameRate%100
                ,interlace,stDecBuf->interlace,stDecBuf->display_period&0x3
                ,stDecBuf->buffer_index,VES_AUIB_NumActive(gstVdcDb[hVdcCh].ui32VesCh)
                ,VES_CPB_GetUsedBuffer(gstVdcDb[hVdcCh].ui32VesCh)
                ,GET_DIFF(gstVdcDb[hVdcCh].ui32LastFeededPtr,gstVdcDb[hVdcCh].ui32PrevRdPtr)
                ,gstVdcDb[hVdcCh].ui32NumofDPBOccupied
                ,gstVdcDb[hVdcCh].ui32DPBSize,stDecBuf->multi_picture
                ,stDecBuf->error_blocks,stDecBuf->frame_packing_arrangement
                ,gstVdcDb[hVdcCh].stDebugInfo.bFieldCh
                ,GET_DIFF(gstVdcDb[hVdcCh].ui32DecodingTime,TOP_HAL_GetGSTCC())*1000/90
                ,stDecBuf->pts,stDecBuf->pts);
    } else {
        log_info("ch:%d PUSH!![%s][%s][%dx%d@%2d.%02d%s(%d:%d)]disp:%2d auib:%03d "
                 "cpb:%06X rd:%06X dpb:%02d/%d LR:%d err:%2d fpa:%d fld:%d "
                 "dt:%5dus to:%d ts:%6lldms(0x%9lld) \n"
                ,hVdcCh,pic_type,gstVdcDb[hVdcCh].stCodecInfo.sName
                ,stDecBuf->width,stDecBuf->height
                ,gstVdcDb[hVdcCh].ui32FrameRate/100,gstVdcDb[hVdcCh].ui32FrameRate%100
                ,interlace,stDecBuf->interlace,stDecBuf->display_period&0x3
                ,stDecBuf->buffer_index,VES_AUIB_NumActive(gstVdcDb[hVdcCh].ui32VesCh)
                ,VES_CPB_GetUsedBuffer(gstVdcDb[hVdcCh].ui32VesCh)
                ,GET_DIFF(gstVdcDb[hVdcCh].ui32LastFeededPtr,gstVdcDb[hVdcCh].ui32PrevRdPtr)
                ,gstVdcDb[hVdcCh].ui32NumofDPBOccupied
                ,gstVdcDb[hVdcCh].ui32DPBSize,stDecBuf->multi_picture
                ,stDecBuf->error_blocks,stDecBuf->frame_packing_arrangement
                ,gstVdcDb[hVdcCh].stDebugInfo.bFieldCh
                ,GET_DIFF(gstVdcDb[hVdcCh].ui32DecodingTime,TOP_HAL_GetGSTCC())*1000/90
                ,gstVdcDb[hVdcCh].eTsOrder
                ,(stDecBuf->timestamp==VDC_INVALID_TIMESTAMP)?VDC_INVALID_TIMESTAMP:ui64TsForDiv,ui64_90kts);
    }
}

static void _VDC_WorkNotiType(VDC_HANDLE_T hVdcCh, SINT8 si8FrameIndex, VDU_NOTI_T eNotiType)
{
    switch( eNotiType )
    {
        case VDU_NOTI_FRAME_SKIP: // FALL-THROUGH
            log_trace("ch:%d Frame skipped idx:%d \n",hVdcCh,si8FrameIndex);
            break;
        case VDU_NOTI_SUCCESS:
            log_trace("ch:%d Success Disp Index:%d \n",hVdcCh,si8FrameIndex);
            break;
        case VDU_NOTI_DISPLAY:
            log_trace("ch:%d NOTI Display idx:%d \n",hVdcCh,si8FrameIndex);
            break;
        case VDU_NOTI_CPB_EMPTY:
        case VDU_NOTI_SKIP_EMPTY:
            log_trace("ch:%d CPB Empty %d idx:%d \n",hVdcCh, eNotiType,
                                                    si8FrameIndex);
            gstVdcDb[hVdcCh].bRemainedData = FALSE;
            gstVdcDb[hVdcCh].bForceDecoding = FALSE;
            break;
        case VDU_NOTI_DPB_FULL:
            log_trace("ch:%d dpb full disp:%d \n",hVdcCh,si8FrameIndex);
            gstVdcDb[hVdcCh].bWaitingClear = TRUE;
            gstVdcDb[hVdcCh].bPopped = FALSE;
            break;
        case VDU_NOTI_DECODING_ERROR:
            log_noti("ch:%d Decoding Failed(disp:%d) \n",hVdcCh, si8FrameIndex);
            break;
        case VDU_NOTI_HEADER_NOT_FOUND:
            if(vdc_unlimit_retry==0 && !gstVdcDb[hVdcCh].bLive && gstVdcDb[hVdcCh].ui32SeqRetryCounter>=VDC_MAX_RETRY_COUNT_FOR_NOT_FOUND){
                log_warning("ch:%d sequence init failed for no header, Codec:%s Tried:%d \n"
                        ,hVdcCh,gstVdcDb[hVdcCh].stCodecInfo.sName,gstVdcDb[hVdcCh].ui32SeqRetryCounter);
                gstVdcDb[hVdcCh].bSequenceInitFailed=TRUE;
                gstVdcDb[hVdcCh].ui32SeqRetryCounter=0;
            }else {
                gstVdcDb[hVdcCh].ui32SeqRetryCounter++;
                log_noti("ch:%d Header not found, SO Retry(%d) \n",hVdcCh,gstVdcDb[hVdcCh].ui32SeqRetryCounter);
            }
            break;
        case VDU_NOTI_NOT_SUPPORTED:
            if(vdc_unlimit_retry==0 && !gstVdcDb[hVdcCh].bLive && gstVdcDb[hVdcCh].ui32SeqRetryCounter>=VDC_MAX_RETRY_COUNT_FOR_FAILED){
                log_warning("ch:%d sequence init failed for not supported stream, Codec:%s Tried:%d \n"
                        ,hVdcCh,gstVdcDb[hVdcCh].stCodecInfo.sName,gstVdcDb[hVdcCh].ui32SeqRetryCounter);
                gstVdcDb[hVdcCh].bSequenceInitFailed=TRUE;
                gstVdcDb[hVdcCh].ui32SeqRetryCounter=0;
            }else {
                gstVdcDb[hVdcCh].ui32SeqRetryCounter++;
                log_noti("ch:%d Sequence Init Failed, SO Retry(%d) \n",hVdcCh,gstVdcDb[hVdcCh].ui32SeqRetryCounter);
            }
            break;
        case VDU_NOTI_LACK_OF_RESOURCES:
            log_trace("ch:%d not enough resource\n",hVdcCh);
            break;
        case VDU_NOTI_SUSPEND:
            log_trace("ch:%d noti suspended(disp:%d) \n",hVdcCh,si8FrameIndex);
            gstVdcDb[hVdcCh].bSuspended = TRUE;
            break;
        case VDU_NOTI_DPB_EMPTY:
            log_trace("ch:%d noti dpb empty \n",hVdcCh);
            break;
        case VDU_NOTI_END_OF_STREAM:
            log_noti("ch:%d eos noti received \n",hVdcCh);
            gstVdcDb[hVdcCh].bRemainedData = FALSE;
            gstVdcDb[hVdcCh].bForceDecoding = FALSE;
            gstVdcDb[hVdcCh].bFeedEOS = FALSE;
            break;
        default:
            log_warning("ch:%d wrong Callback, Type is %d \n",hVdcCh,eNotiType);
            break;
    }

}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */

//static UINT32 ui32checkTime=0;
static void _VDC_DecCallback(VDU_HANDLE_T hInstance, VDU_RESULT_T* pstResult)
{
    VDC_HANDLE_T hVdcCh;
    SINT8 si8FrameIndex = pstResult->si8FrmIndex;
    UINT32 ui32Consumed = 0;
    CHAR* pic_type = 0;
    CHAR* interlace = 0;
    decoded_buffer_t stDecBuf = {0,};
    SINT64 ts = VDC_UNKNOWN_PTS;
    SINT64 slotts;
    SINT64 fifots;
    UINT64 ui64TsForDiv = 0;
    VDU_FRM_INFO_T* pstFrameInfo = &pstResult->stFrameInfo;
    int len;
    unsigned long flag;
#ifdef CODE_FOR_CHECK_LOCK_TIME
    UINT32 ui32LockTime;
#endif
    // exceptions
    for(hVdcCh=0; hVdcCh<VDEC_NUM_OF_CHANNEL; hVdcCh++){
        if(gstVdcDb[hVdcCh].hVduCh == hInstance)
            break;
    }
    if( hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state == VDC_NULL
            || gstVdcDb[hVdcCh].hVduCh == VDU_INVALID_HANDLE )
    {
        log_error("ch:%d Invalid channel \n", hVdcCh);
        return;
    }
#ifdef CODE_FOR_CHECK_LOCK_TIME
    ui32LockTime = TOP_HAL_GetGSTCC();
#endif
    spin_lock_irqsave(&gstVdcDb[hVdcCh].instance_lock, flag);

    if(gstVdcDb[hVdcCh].state == VDC_CLOSING)
    {
        log_noti("ch:%d Closing Channel received Callback\n", hVdcCh);
        spin_unlock_irqrestore(&gstVdcDb[hVdcCh].instance_lock, flag);
        return;
    }

    // initializing
    stDecBuf.buffer_index = -1;

    ui32Consumed = _VDC_WorkReadPtr(hVdcCh, pstResult->ui32ReadPtr, pstResult->eNotiType, pstResult->si8DecodedIndex, si8FrameIndex);

    stDecBuf.report = _VDC_WorkReport(hVdcCh, pstResult->eNotiType, &stDecBuf, pstFrameInfo);

    if (gstVdcDb[hVdcCh].ui32DPBSize != pstFrameInfo->ui8NumOfFrames)
    {
        log_noti("ch:%d dpb size changed.%d->%d \n",
                hVdcCh,gstVdcDb[hVdcCh].ui32DPBSize,pstFrameInfo->ui8NumOfFrames);
        /* clear previous dpb size string for debug */
        for (len=0; len<gstVdcDb[hVdcCh].ui32DPBSize; len++)
            gstVdcDb[hVdcCh].stDebugInfo.sDpbState[len]=0;

    }
    /* update dpb size early for sequence chage */
    gstVdcDb[hVdcCh].ui32DPBSize = pstFrameInfo->ui8NumOfFrames;

    // process making decoded buffer info (will be display)
    if( si8FrameIndex >= 0 )
    {
        if( gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE )
        {
            /* to filter error stream */
            gstVdcDb[hVdcCh].ui32FeededCounter = 0;
            gstVdcDb[hVdcCh].ui32FeedStartTime = TOP_HAL_GetGSTCC();

            // receive field or frame pic info in displaying time and compensate timestamp.
            if(pstFrameInfo->stDispInfo.bFieldPicture){
                log_user1("ch:%d field pic detected \n",hVdcCh);
                gstVdcDb[hVdcCh].stDebugInfo.bFieldCh = TRUE;
                gstVdcDb[hVdcCh].bFieldFeeded = TRUE;
            }else{
                log_user1("ch:%d frame pic detected \n",hVdcCh);
                gstVdcDb[hVdcCh].stDebugInfo.bFieldCh = FALSE;
            }

            // print npf occured log and marking npf occured information
            if( pstFrameInfo->stDispInfo.bNonPairedField ) {
                log_noti("ch:%d npf skip:%d, scanmode?%d occured \n"
                        ,hVdcCh, si8FrameIndex, gstVdcDb[hVdcCh].ui32ScanMode);
                gstVdcDb[hVdcCh].bNPFOccured = TRUE;
            }
            // case 1. check npf skip occured. considering dish-trick case
            // case 2. force clear and no display decoded but not displayed frame after flushing
            if( (pstFrameInfo->stDispInfo.bNonPairedField &&
                        gstVdcDb[hVdcCh].ui32ScanMode != VDC_SKIP_DISH ) ||
                    gstVdcDb[hVdcCh].state == VDC_STOP ) {
                // clearing occupied dpb
                if ( VDU_ClearFrame(gstVdcDb[hVdcCh].hVduCh, si8FrameIndex & 0xFFFF) == VDU_RET_OK) {
                    log_trace("ch:%d forced clear %d frame \n",hVdcCh, si8FrameIndex);
                } else {
                    log_warning("ch:%d forced clear %d failed \n",hVdcCh, si8FrameIndex);
                }
                // exception for handling dynamic(user) dpb allocation mode
                if( gstVdcDb[hVdcCh].eLinearBufMode != VDC_LINEAR_MODE_DYNAMIC )
                    gstVdcDb[hVdcCh].ui32DecodedFlags &= ~(1<<si8FrameIndex);
                goto skip_displaying;
            }

            /* makes display callback information */
            stDecBuf.buffer_index = si8FrameIndex;
            _VDC_MakeDispInfo(hVdcCh, &stDecBuf, pstFrameInfo);

            /* decide timestamp */
            if(unlikely(gstVdcDb[hVdcCh].eTsOrder == VDC_TS_UNDEFINED ))
                _VDC_DefineTsOrder(hVdcCh);

            /* prepare two kind of timestamp */
            slotts = gstVdcDb[hVdcCh].si64TSInfo[si8FrameIndex];
            gstVdcDb[hVdcCh].si64TSInfo[si8FrameIndex] = VDC_INVALID_TIMESTAMP;
            if (kfifo_len(&gstVdcDb[hVdcCh].stTSfifo) > 0)
                len = kfifo_out(&gstVdcDb[hVdcCh].stTSfifo,&fifots,sizeof(SINT64));
            else
                fifots = VDC_UNKNOWN_PTS;

            /* NOTE: it's not recommended mapping logic.
             *  if reverse ts detected in spite of using pts
             * ordering, it will be error file or error case from upper layer.
             * we have to be extremely careful when change it to
             * ascending ts order because it is related with live.
             * and it's also dangerour scheme for hevc multi-slice (svp).
             * so added exception for hevc.*/
            if ( !gstVdcDb[hVdcCh].bLive && !gstVdcDb[hVdcCh].bUsePdec &&
                 gstVdcDb[hVdcCh].stCodecInfo.eCodecType != VDC_CODEC_HEVC &&
                 gstVdcDb[hVdcCh].eTsOrder == VDC_TS_PTS_ORDER &&
                 gstVdcDb[hVdcCh].ui32ScanMode == VDC_SKIP_NONE &&
                 _VDC_CheckUnknownOrder(hVdcCh,slotts))
            {
                log_noti("ch:%d ts order changed to unknown order \n",hVdcCh);
                gstVdcDb[hVdcCh].eTsOrder = VDC_TS_UNKNOWN_ORDER;
            }

            log_user1("ch:%d ts order:%d fifo:%lld slot:%lld fifo_len:%d \n",
                    hVdcCh,gstVdcDb[hVdcCh].eTsOrder,fifots,slotts,kfifo_len(&gstVdcDb[hVdcCh].stTSfifo)/sizeof(SINT64));

            /* intended code for unknown order. always popping one timestamp
             * at one picture decoded. and use that ts only in unknown order  */
            ts = _VDC_DecideTS(hVdcCh);

            switch (gstVdcDb[hVdcCh].eTsOrder)
            {
                case VDC_TS_DTS_ORDER:
                    ts = fifots;
                    break;
                case VDC_TS_PTS_ORDER:
                    ts = slotts;
                    break;
                case VDC_TS_UNKNOWN_ORDER:
                    if (gstVdcDb[hVdcCh].ui32ScanMode != VDC_SKIP_NONE)
                        ts = slotts;
                    break;
                default:
                    log_warning("ch:%d ts order undefined \n",hVdcCh);
                    break;
            }

            gstVdcDb[hVdcCh].si64prevSentTS = ts;

            /* decide pts or ts */
            if(gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec) {
                stDecBuf.pts = ts;
                stDecBuf.timestamp = VDC_INVALID_TIMESTAMP;
            } else {
                /* scaling to nano second */
                stDecBuf.pts = VDC_UNKNOWN_PTS;
                if( ts >= 0 && ts != VDC_UNKNOWN_PTS ) {
                    ui64TsForDiv = ts * 100000;
                    do_div(ui64TsForDiv, 9);
                    stDecBuf.timestamp = ui64TsForDiv;
                    do_div(ui64TsForDiv, 1000000);

                } else {
                    stDecBuf.timestamp = VDC_INVALID_TIMESTAMP;
                }
            }

            _VDC_MakeDebugInfo(hVdcCh, &pic_type, &interlace, &stDecBuf, ui32Consumed, ts);
            _VDC_PrintPushInfo(hVdcCh, pic_type, interlace, &stDecBuf, ui64TsForDiv, ts);

            // exception for handling dynamic(user) dpb allocation mode
            if( gstVdcDb[hVdcCh].eLinearBufMode != VDC_LINEAR_MODE_DYNAMIC )
                gstVdcDb[hVdcCh].ui32FrameFlags[si8FrameIndex]++;
        }
    }

skip_displaying:
    spin_unlock_irqrestore(&gstVdcDb[hVdcCh].instance_lock, flag);
#ifdef CODE_FOR_CHECK_LOCK_TIME
    log_user2("callback process lock time:%dus \n",GET_DIFF(ui32LockTime,TOP_HAL_GetGSTCC())*1000/90);
#endif

    if(gstVdcDb[hVdcCh].decoded_q != NULL) {
        decoded_q_push(gstVdcDb[hVdcCh].decoded_q, &stDecBuf);
    }

    _VDC_WorkNotiType(hVdcCh, si8FrameIndex, pstResult->eNotiType);
    // channel state to idle
    atomic_set(&gstVdcDb[hVdcCh].atIsDecoding, 0);
    if(!gstVdcDb[hVdcCh].bLive)
    {
        if (gstVdcDb[hVdcCh].state_changed != NULL)
            gstVdcDb[hVdcCh].state_changed(hVdcCh);
        else
            wake_up_all(&gstVdcDb[hVdcCh].wait_state_change);
    }

    if (atomic_read (&gstVdcDb[hVdcCh].atFeedHandle) > 0)
    {
        log_trace("ch:%d, Feed will be done by PostProc, end context \n",hVdcCh);
    }
    else
    {
        log_trace("ch:%d, Feed by Decoder's Callback \n",hVdcCh);

#ifdef USE_WORKQUEUE_SCHEDULE
        if (gstVdcDb[hVdcCh].bSuspended)
        {
            gstVdcDb[hVdcCh].bSuspended = FALSE;
            queue_delayed_work(vdc_wq, &kick_scheduler_work, msecs_to_jiffies(5));
        }
        else
        {
            queue_delayed_work(vdc_wq, &kick_scheduler_work, 0);
        }
#else
        _VDC_Scheduler();
#endif
    }
}

static BOOLEAN _VDC_CheckUnknownOrder(VDC_HANDLE_T hVdcCh, SINT64 cur_ts)
{
    UINT32 index;
    /* unknown order must be searched at pts pool */
    for (index=0; index<VDC_MAX_DPB_SIZE; index++)
    {
        if (gstVdcDb[hVdcCh].si64TSInfo[index] == VDC_INVALID_TIMESTAMP)
            continue;

        if (cur_ts > gstVdcDb[hVdcCh].si64TSInfo[index]
                && cur_ts != VDC_UNKNOWN_PTS &&
                gstVdcDb[hVdcCh].si64TSInfo[index] != VDC_UNKNOWN_PTS)
        {
            log_user1("ch:%d unknown order! idx:%d cur:%lld candidate:%lld\n",
                    hVdcCh,index,cur_ts,gstVdcDb[hVdcCh].si64TSInfo[index]);
            return TRUE;
        }
    }
    return FALSE;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static void _VDC_ChWatchDog(void)
{
    VDU_STATUS_T eStatus;
    UINT32 ui32idx;
    UINT32 ui32prevTime, ui32CurTime,ui32Diff;
    decoded_buffer_t stDecBuf = {0,};
    for(ui32idx=0; ui32idx<VDEC_NUM_OF_CHANNEL; ui32idx++){
        // check playing channel
        if(gstVdcDb[ui32idx].state != VDC_PLAY || gstVdcDb[ui32idx].bSequenceInitFailed)
            continue;
        if(gstVdcDb[ui32idx].hVduCh == VDU_INVALID_HANDLE ||
                VDU_GetStatus(gstVdcDb[ui32idx].hVduCh,&eStatus) != VDU_RET_OK)
            continue;
        if(eStatus < VDU_STATUS_BUSY)
            continue;
        //not started channel
        if(gstVdcDb[ui32idx].ui32DecodingTime == 0)
            continue;
        ui32prevTime = gstVdcDb[ui32idx].ui32DecodingTime;
        ui32CurTime = TOP_HAL_GetGSTCC();
        ui32Diff = GET_DIFF(ui32prevTime, ui32CurTime);
        log_user2("ch:%d Watchdog, prev:%u, cur:%u, diff:%d \n",ui32idx, ui32prevTime,ui32CurTime,ui32Diff);
        if(ui32Diff >= VDC_RESET_THRESHOLD && (SINT32)gstVdcDb[ui32idx].ui32DecodingTime > 0
                && (SINT32)ui32CurTime > 0)
        {
            log_noti("ch:%d Watchdog Channel HW Reset Occurs:PrevRunningTime:%d,CurTime:%d,Diff:%d \n",ui32idx,ui32prevTime,ui32CurTime,ui32Diff);
            gstVdcDb[ui32idx].ui32DecodingTime = TOP_HAL_GetGSTCC();
            stDecBuf.buffer_index = -1;
            stDecBuf.report = DECODED_REPORT_HW_RESET;
            decoded_q_push(gstVdcDb[ui32idx].decoded_q, &stDecBuf);
        }
    }
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static void _VDC_Scheduler(void)
{
    VDC_HANDLE_T hVdcCh=VDC_INVALID_HANDLE;
    ULONG flags = 0;
#ifdef CODE_FOR_CHECK_LOCK_TIME
    UINT32 ui32LockTime;
#endif
    spin_lock_irqsave(&_VDC_Spinlock, flags);

#ifdef CODE_FOR_CHECK_LOCK_TIME
    ui32LockTime = TOP_HAL_GetGSTCC();
#endif
    hVdcCh = _VDC_SellectChannel();
    if(hVdcCh==VDC_INVALID_HANDLE) {
        goto finish;
    }
    atomic_set(&gstVdcDb[hVdcCh].atFeedHandle, 1);
#ifdef CODE_FOR_CHECK_LOCK_TIME
    log_user2("ch:%d sellect channel lock time:%dus \n",hVdcCh,GET_DIFF(ui32LockTime,TOP_HAL_GetGSTCC())*1000/90);
#endif
    spin_unlock_irqrestore(&_VDC_Spinlock, flags);

    _VDC_Main(hVdcCh);

    spin_lock_irqsave(&_VDC_Spinlock, flags);
    atomic_set(&gstVdcDb[hVdcCh].atFeedHandle, 0);
finish:
    spin_unlock_irqrestore(&_VDC_Spinlock, flags);
}

static void _VDC_KickScheduler(struct work_struct *work)
{
	_VDC_Scheduler();
}


/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static VDC_HANDLE_T _VDC_SellectChannel(void)
{
    VDC_HANDLE_T hTempCh;
    VDC_HANDLE_T hVdcCh = VDC_INVALID_HANDLE;
    VDU_STATUS_T eStatus = VDU_STATUS_NULL;

    gstVdcCoreInfo.ui32NumofIdleCh = 0;

    for(hTempCh=0; hTempCh<VDEC_NUM_OF_CHANNEL; hTempCh++)
    {
        // 1. check playing channel
        if(gstVdcDb[hTempCh].state != VDC_PLAY || gstVdcDb[hTempCh].bSequenceInitFailed ||
                gstVdcDb[hTempCh].eNotSupportNotiValue == VDC_NOT_SUPPORT_NOTI_REPORTED)
            continue;

        if (atomic_read(&gstVdcDb[hTempCh].atFeedHandle) > 0) {
            log_trace ("ch:%d is still decoding \n", hTempCh);
            continue;
        }

        // 2. Core's work done but vdc is still processing
        if( atomic_read(&gstVdcDb[hTempCh].atIsDecoding) > 0) {
            log_trace("ch:%d vdc busy \n", hTempCh);
            continue;
        }

        // 3. Check sub-module's channel state
        if(gstVdcDb[hTempCh].hVduCh == VDU_INVALID_HANDLE ||
                VDU_GetStatus(gstVdcDb[hTempCh].hVduCh,&eStatus) != VDU_RET_OK){
            log_trace("ch:%d invalid vdu handle \n",hTempCh);
            continue;
        }
        if(eStatus == VDU_STATUS_BUSY){
            log_trace("ch:%d VDU Status:%d(busy)\n",hTempCh,eStatus);
            continue;
        }

        // 4. Check usable Source(VES) Data
        if (gstVdcDb[hTempCh].bRemainedData ||
            (gstVdcDb[hTempCh].bWaitingClear && !gstVdcDb[hTempCh].bFlushed) ||
            (gstVdcDb[hTempCh].eWaitingBuffer == VDC_USRDPB_STATE_SET) ||
            gstVdcDb[hTempCh].bFeedEOS)
        {
            log_trace("ch:%d will directly decode remain?%d wait clear?%d \n",
                    hTempCh,gstVdcDb[hTempCh].bRemainedData,
                    gstVdcDb[hTempCh].bWaitingClear);
            gstVdcDb[hTempCh].bForceDecoding = TRUE;
        }
        else if (gstVdcDb[hTempCh].ui32VesCh != 0xFF &&
                 !_VDC_FeedPossible(hTempCh))
        {
            continue;
        }

        // 5. Check Dpb Empty
        if( gstVdcDb[hTempCh].eWaitingBuffer == VDC_USRDPB_STATE_EMPTY ) {
            log_trace("ch:%d waiting user's dpb \n", hTempCh);
            continue;
        }

        // 6. Check usable Destination Queue or Buffer
        if(gstVdcDb[hTempCh].ui32DPBSize != VDC_INITIAL_DPB_SIZE
                && gstVdcDb[hTempCh].decoded_q != NULL) // it means dpb alloced
        {
            UINT32 ui32NumDecodedQueue, ui32maxDecodedQSize;
            // 6-1. Check usable decoded queue
            ui32NumDecodedQueue = decoded_q_size(gstVdcDb[hTempCh].decoded_q);
            ui32maxDecodedQSize = decoded_q_max_size(gstVdcDb[hTempCh].decoded_q);
            if(ui32maxDecodedQSize <= ui32NumDecodedQueue)
            {
                log_trace("ch:%d, decoded q full, %d(%d) \n",
                        hTempCh, ui32NumDecodedQueue, ui32maxDecodedQSize);
                continue;
            }
            // 6-2. Check usable DPB
            if(_VDC_CheckUsableDPB(hTempCh)==TRUE)
            {
                goto decide_ch;
            }else continue;
        }
        else {
            goto decide_ch;
        }

decide_ch:
        gstVdcCoreInfo.ui32NumofIdleCh++;
        if (gstVdcCoreInfo.ui32NumofIdleCh>1)
        {
            if (gstVdcCoreInfo.ui32PrevSelCh != hTempCh)
            {
                hVdcCh = hTempCh;
            }
        }
        else
        {
            hVdcCh = hTempCh;
        }

    }
    if (hVdcCh != VDC_INVALID_HANDLE)
        gstVdcCoreInfo.ui32PrevSelCh = hVdcCh;
    log_trace ("ch:%d selected\n", hVdcCh);
    return hVdcCh;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static BOOLEAN _VDC_FeedPossible(VDC_HANDLE_T hVdcCh)
{
    UINT32 index = 0;
    UINT32 ui32PopNum = gstVdcDb[hVdcCh].bFieldFeeded?2:1;
    UINT32 ui32ActiveNum = 0;
    UINT32 ui32Popped = 0;
    E_VES_AU_T eAuType;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Invalid channel \n", hVdcCh);
        return FALSE;
    }

    if( gstVdcDb[hVdcCh].bFieldFeeded && gstVdcDb[hVdcCh].bNPFOccured )
        ui32PopNum = 1;

    while( VES_AUIB_NumActive(gstVdcDb[hVdcCh].ui32VesCh) &&
            VES_AUIB_CheckAuType(gstVdcDb[hVdcCh].ui32VesCh,&eAuType) ) {
        // no need to check picture in hevc
        if( gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_HEVC
                || _VDC_IsPicture(hVdcCh,eAuType) )
        {
            ui32ActiveNum++;
            if(ui32ActiveNum >= ui32PopNum)
                break;
        } else if( eAuType == AU_EOS ) {
            ui32ActiveNum = ui32PopNum;
            break;
        }
        ui32Popped++;
        VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,TRUE);
    };

    for(index=0; index<ui32Popped; index++)
        VES_AUIB_RestoreOneAU(gstVdcDb[hVdcCh].ui32VesCh,TRUE);

    log_trace("ch:%d active num:%d pop num:%d \n",hVdcCh,ui32ActiveNum,ui32PopNum);

    return ( ui32ActiveNum >= ui32PopNum );
}

static BOOLEAN _VDC_IsPicture(VDC_HANDLE_T hVdcCh, E_VES_AU_T eAuType)
{
    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Invalid channel \n", hVdcCh);
        return FALSE;
    }

    if( gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_HEVC )
    {
        if( (E_PDEC_HEVC_AU_T)eAuType == AU_HEVC_SLICE_SEGMENT )
            return TRUE;
    }
    else if (gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_AVS)
    {
        if (eAuType != AU_SEQUENCE_HEADER)
            return TRUE;
    }
    else
    {
        if( eAuType >= AU_PICTURE_I && eAuType <= AU_PICTURE_B_SKIP )
            return TRUE;
    }

    return FALSE;
}
/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static void _VDC_Main(VDC_HANDLE_T hVdcCh)
{
    ULONG flags=0;
    VDU_RET_T eRetVal;
    VDU_AU_T stBodaAU = {0, };
    VDU_LINEAR_FRAMES_T stFrmsInfo = {0, };
#ifdef CODE_FOR_CHECK_LOCK_TIME
    UINT32 ui32LockTime;
#endif

    spin_lock_irqsave(&_VDC_Spinlock, flags);

#ifdef CODE_FOR_CHECK_LOCK_TIME
    ui32LockTime = TOP_HAL_GetGSTCC();
#endif

    // 1. check waiting Clear queue condition
    if(gstVdcDb[hVdcCh].bWaitingClear == TRUE && gstVdcDb[hVdcCh].bFlushed == FALSE)
    {
        gstVdcDb[hVdcCh].bPopped = FALSE;
        goto decode;
    }
    if(gstVdcDb[hVdcCh].eWaitingBuffer == VDC_USRDPB_STATE_SET)
    {
        gstVdcDb[hVdcCh].eWaitingBuffer = VDC_USRDPB_STATE_NOR;
        goto decode;
    }

    // 2. feeding AU
    if(gstVdcDb[hVdcCh].bForceDecoding){
        goto decode;
    } else if(_VDC_PrepareFeed(hVdcCh,&stBodaAU)){
        goto feed;
    }
    else {
        goto finish;
    }
feed:
#ifdef CODE_FOR_CHECK_LOCK_TIME
    log_user2("ch:%d prepare feed lock time:%dus \n",hVdcCh,GET_DIFF(ui32LockTime,TOP_HAL_GetGSTCC())*1000/90);
#endif
    spin_unlock_irqrestore(&_VDC_Spinlock, flags);
    eRetVal = VDU_FeedAu(gstVdcDb[hVdcCh].hVduCh, &stBodaAU);
    spin_lock_irqsave(&_VDC_Spinlock, flags);
    if(eRetVal != VDU_RET_OK){
        log_warning("ch:%d Feed AU Failed START:%X - SIZE:%X \n",hVdcCh,stBodaAU.ui32StartAddr-VES_CPB_GetBufferBaseAddr(gstVdcDb[hVdcCh].ui32VesCh),stBodaAU.ui32Size);
        goto finish;
    }
decode:
    if(_VDC_PrepareDec(hVdcCh, &stFrmsInfo) == FALSE)
        goto finish;
    spin_unlock_irqrestore(&_VDC_Spinlock, flags);
    eRetVal = VDU_StartDecoding(gstVdcDb[hVdcCh].hVduCh, &stFrmsInfo);
    spin_lock_irqsave(&_VDC_Spinlock, flags);
	if (_VDC_DecPostProc(hVdcCh,eRetVal))
	{
		spin_unlock_irqrestore(&_VDC_Spinlock, flags);

#ifdef USE_WORKQUEUE_SCHEDULE
        if (gstVdcDb[hVdcCh].bSuspended)
        {
            gstVdcDb[hVdcCh].bSuspended = FALSE;
            queue_delayed_work(vdc_wq, &kick_scheduler_work, msecs_to_jiffies(5));
        }
        else
        {
            queue_delayed_work(vdc_wq, &kick_scheduler_work, 0);
        }
#else
		_VDC_Scheduler();
#endif

		return;
	}
finish:
    spin_unlock_irqrestore(&_VDC_Spinlock, flags);
    return;
}

static void _VDC_InitializeDB(VDC_HANDLE_T hVdcCh)
{
    gstVdcDb[hVdcCh].state = VDC_NULL;
    gstVdcDb[hVdcCh].priv = NULL;
    gstVdcDb[hVdcCh].ui32VesCh = 0xFF;
    gstVdcDb[hVdcCh].stCodecInfo.eCodecType = VDC_CODEC_INVALID;
    gstVdcDb[hVdcCh].stCodecInfo.sName = NULL;
    gstVdcDb[hVdcCh].hVduCh = VDU_INVALID_HANDLE;
    gstVdcDb[hVdcCh].decoded_q = NULL;
    gstVdcDb[hVdcCh].clear_q = NULL;

    gstVdcDb[hVdcCh].eNotSupportNotiValue = FALSE;

    _VDC_InitInnerDatas(hVdcCh);

    log_noti("ch:%d initialized db ok \n",hVdcCh);
}

static void _VDC_InitInnerDatas(VDC_HANDLE_T hVdcCh)
{
    UINT32 ui32idx;

    gstVdcDb[hVdcCh].ui32DPBSize = VDC_INITIAL_DPB_SIZE;
    gstVdcDb[hVdcCh].ui32NumofDPBOccupied = 0;
    gstVdcDb[hVdcCh].ui32SeqRetryCounter = 0;
    gstVdcDb[hVdcCh].ui32DecRetryCounter = 0;
    gstVdcDb[hVdcCh].ui32ScanMode = VDC_SKIP_NONE;

    gstVdcDb[hVdcCh].ui32DiscontTotal = 0;
    gstVdcDb[hVdcCh].ui32FeededCounter = 0;
    gstVdcDb[hVdcCh].ui32FeedStartTime = 0;
    gstVdcDb[hVdcCh].ui32DecodingTime = 0;
    gstVdcDb[hVdcCh].ui32FrameRate = 0;
    gstVdcDb[hVdcCh].ui32LastFeededPtr = VDC_INVALID_ADDRESS;
    gstVdcDb[hVdcCh].si64prevSavedTS = VDC_UNKNOWN_PTS;
    gstVdcDb[hVdcCh].si64prevSentTS = VDC_UNKNOWN_PTS;

    gstVdcDb[hVdcCh].bWaitingClear = FALSE;
    gstVdcDb[hVdcCh].eWaitingBuffer = VDC_USRDPB_STATE_NOR;
    gstVdcDb[hVdcCh].bFlushed = FALSE;
    gstVdcDb[hVdcCh].bPopped = FALSE;
    gstVdcDb[hVdcCh].bLive = FALSE;
    gstVdcDb[hVdcCh].bUsePdec = FALSE;
    gstVdcDb[hVdcCh].bRemainedData = FALSE;
    gstVdcDb[hVdcCh].bForceDecoding = FALSE;
    gstVdcDb[hVdcCh].bSequenceInitFailed = FALSE;
    gstVdcDb[hVdcCh].bFieldFeeded = FALSE;
    atomic_set(&gstVdcDb[hVdcCh].atIsDecoding, 0);
    atomic_set(&gstVdcDb[hVdcCh].atFeedHandle, 0);
    gstVdcDb[hVdcCh].bNPFOccured = FALSE;
    gstVdcDb[hVdcCh].eTsOrder = VDC_TS_UNDEFINED;
    gstVdcDb[hVdcCh].ui32DecodedFlags = 0;
    gstVdcDb[hVdcCh].ui32EOSWaitCounter = 0;
    gstVdcDb[hVdcCh].bFeedEOS = FALSE;
    gstVdcDb[hVdcCh].bSuspended = FALSE;

    // clear debug info
    gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedAU = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedSize = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotDecodingTime = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotConsumedAU = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotConsumedSize = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotDisplayedPic = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32NumofDecodedWithError = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui64TotalPlayedTime = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui64AccumAuSize = 0;

    for(ui32idx=0;ui32idx<VDC_MAX_DPB_SIZE;ui32idx++)
    {
        gstVdcDb[hVdcCh].si64UTSInfo[ui32idx] = VDC_INVALID_TIMESTAMP;
        gstVdcDb[hVdcCh].si64TSInfo[ui32idx] = VDC_INVALID_TIMESTAMP;
        gstVdcDb[hVdcCh].ui32FrameFlags[ui32idx] = 0;
        gstVdcDb[hVdcCh].stDebugInfo.sDpbState[ui32idx] = 0;
    }

    gstVdcDb[hVdcCh].ui32PrevRdPtr = VDC_INVALID_ADDRESS;
    gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder = 0;

    log_noti("ch:%d initialized inner datas ok \n",hVdcCh);
}

static void _VDC_InitExtBufsInfo(VDU_LINEAR_FRAMES_T *stFrmsInfo)
{
    stFrmsInfo->ui8NumberOfFrames = 0;
    stFrmsInfo->ui32Stride = 0;
}

static void _VDC_ChMonitoring(ULONG arg)
{
    UINT32 ui32AvgDecodingTime = 0;
    UINT64 ui64DiffTime;
    VDC_HANDLE_T hVdcCh = (VDC_HANDLE_T)arg;
    ULONG flags = 0;

    if( hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state == VDC_NULL )
    {
        log_warning("ch:%d Invalid channel \n", hVdcCh);
        return;
    }

    spin_lock_irqsave(&_VDC_Spinlock, flags);

    ui64DiffTime = GET_DIFF(gstVdcDb[hVdcCh].stDebugInfo.ui64TotalPlayedTime,get_jiffies_64());
    do_div(ui64DiffTime,100);

    if(gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic > 0)
        ui32AvgDecodingTime = gstVdcDb[hVdcCh].stDebugInfo.ui32TotDecodingTime/(gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic*90); //ms


    // ch_num,[codec_type][framerate/maptype] played
    log_noti("ch:%d%4llds,%s|%dx%d@%2d.%02d%s|AU%03d|CPB:%06x|rd:%06x|DQ%2d/%2d|ptsL%9lld|in%02dout%02d|con:%06x|avgdt:%03dms \n"
            ,hVdcCh
            ,ui64DiffTime
            ,gstVdcDb[hVdcCh].stCodecInfo.sName
            ,gstVdcDb[hVdcCh].stDebugInfo.ui32Width
            ,gstVdcDb[hVdcCh].stDebugInfo.ui32Height
            ,gstVdcDb[hVdcCh].ui32FrameRate/100
            ,gstVdcDb[hVdcCh].ui32FrameRate%100
            ,gstVdcDb[hVdcCh].stDebugInfo.sInterProg
            ,VES_AUIB_NumActive(gstVdcDb[hVdcCh].ui32VesCh)
            ,VES_CPB_GetUsedBuffer(gstVdcDb[hVdcCh].ui32VesCh)
            ,GET_DIFF(gstVdcDb[hVdcCh].ui32LastFeededPtr,gstVdcDb[hVdcCh].ui32PrevRdPtr)
            ,gstVdcDb[hVdcCh].ui32NumofDPBOccupied
            ,gstVdcDb[hVdcCh].ui32DPBSize
            ,gstVdcDb[hVdcCh].stDebugInfo.ui64LastPts
            ,gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedAU
            ,gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic
            ,gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedSize
            ,ui32AvgDecodingTime);

    gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedAU = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedSize = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32TotDecodingTime = 0;

    del_timer( &gstVdcDb[hVdcCh].stChTimer );
    mod_timer( &gstVdcDb[hVdcCh].stChTimer, get_jiffies_64() + HZ );

    spin_unlock_irqrestore(&_VDC_Spinlock, flags);
}

/*
#define MAX_LENGTH 20
#define SEPARATOR "_"
static CHAR* _VDC_GetTailString(CHAR* sOrigin)
{
CHAR* sParsed = NULL;
CHAR* sResult = NULL;
CHAR sTmpOrig[MAX_LENGTH] = {0, };
CHAR* sTemp;

strcpy(sTmpOrig, sOrigin);
sTemp = sTmpOrig;
sParsed = strsep( &sTemp, SEPARATOR);
while (sParsed)
{
sResult = sParsed;
sParsed = strsep( &sTemp, SEPARATOR);
}
return sResult;
}*/

static void _VDC_ClearUsedDPB(VDC_HANDLE_T hVdcCh)
{
    S_DISPCLEARQ_BUF_T stClearQ;
    // display clear queue pop
    while( DISP_CLEAR_Q_Pop( gstVdcDb[hVdcCh].clear_q, &stClearQ) )
    {
        if(gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE &&
                VDU_ClearFrame(gstVdcDb[hVdcCh].hVduCh, stClearQ.ui32FrameIdx & 0xFFFF) == VDU_RET_OK){
            gstVdcDb[hVdcCh].bPopped = TRUE;
            gstVdcDb[hVdcCh].ui32FrameFlags[stClearQ.ui32FrameIdx]--;
            if (gstVdcDb[hVdcCh].ui32FrameFlags[stClearQ.ui32FrameIdx]==0)
                gstVdcDb[hVdcCh].ui32DecodedFlags &= ~(1<<stClearQ.ui32FrameIdx);
            log_trace("ch:%d clear %d frame \n",hVdcCh, stClearQ.ui32FrameIdx);
        } else {
            log_warning("ch:%d clear %d failed \n",hVdcCh, stClearQ.ui32FrameIdx);
        }
    }
}

static BOOLEAN _VDC_CheckUsableDPB(VDC_HANDLE_T hVdcCh)
{
    UINT32 idx;
    BOOLEAN bSpace = FALSE;
    gstVdcDb[hVdcCh].ui32NumofDPBOccupied = 0;
    gstVdcDb[hVdcCh].stDebugInfo.ui32NumofDQOccupied = 0;

    if(gstVdcDb[hVdcCh].clear_q == NULL)
        goto finish;

    // Check usable DPB
    for(idx = 0;idx<gstVdcDb[hVdcCh].ui32DPBSize;idx++)
    {
        /* multiple displayed */
        if(gstVdcDb[hVdcCh].ui32FrameFlags[idx] > 1)
        {
            gstVdcDb[hVdcCh].stDebugInfo.sDpbState[idx] = (char)(((int)'0')+gstVdcDb[hVdcCh].ui32FrameFlags[idx]);
            gstVdcDb[hVdcCh].ui32NumofDPBOccupied++;
        }
        else
        {
            /* displayed one time */
            if (gstVdcDb[hVdcCh].ui32FrameFlags[idx] == 1)
            {
                gstVdcDb[hVdcCh].stDebugInfo.sDpbState[idx] = '#';
                gstVdcDb[hVdcCh].stDebugInfo.ui32NumofDQOccupied++;
                gstVdcDb[hVdcCh].ui32NumofDPBOccupied++;
            } /* decoded but not displayed yet */
            else if ((gstVdcDb[hVdcCh].ui32DecodedFlags>>idx)&0x01)
            {
                gstVdcDb[hVdcCh].stDebugInfo.sDpbState[idx] = '*';
                gstVdcDb[hVdcCh].ui32NumofDPBOccupied++;
            } /* empty */
            else
            {
                gstVdcDb[hVdcCh].stDebugInfo.sDpbState[idx] = '-';
                bSpace = TRUE;
            }
        }
    }

    if(bSpace == FALSE)
    {
        log_trace("ch:%d dpb is full \n",hVdcCh);
    }
    else
    {
        log_trace("ch:%d dpb: %s waitclear?%d \n",hVdcCh
                ,gstVdcDb[hVdcCh].stDebugInfo.sDpbState
                ,gstVdcDb[hVdcCh].bWaitingClear);
    }

    // first feeding started
    if(gstVdcDb[hVdcCh].ui32DPBSize == VDC_INITIAL_DPB_SIZE) bSpace = TRUE;

    // dpb full case, one or more display clear queue popping needed
    if(gstVdcDb[hVdcCh].bWaitingClear == TRUE && gstVdcDb[hVdcCh].bPopped == FALSE) bSpace = FALSE;

    log_trace("ch:%d waitclear:%d popped:%d space:%d \n",hVdcCh,gstVdcDb[hVdcCh].bWaitingClear,gstVdcDb[hVdcCh].bPopped,bSpace);
finish:
    return bSpace;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static BOOLEAN _VDC_PrepareFeed(VDC_HANDLE_T hVdcCh, VDU_AU_T *stBodaAU)
{
    SINT64 si64TimeStamp=0;
    UINT64 ui64TsForDiv;
    UINT32 ui32PopNum=gstVdcDb[hVdcCh].bFieldFeeded?2:1;
    S_VES_AU_T stAUInfo = {0, };

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Invalid channel \n", hVdcCh);
        return FALSE;
    }

    stBodaAU->bEndOfStream = FALSE;

    if( gstVdcDb[hVdcCh].bFieldFeeded && gstVdcDb[hVdcCh].bNPFOccured ) {
        ui32PopNum = 1;
        gstVdcDb[hVdcCh].bNPFOccured = FALSE;
    }

auib_pop:
    // 2. pop pdec auib
    if(gstVdcDb[hVdcCh].ui32VesCh != 0xFF &&
            VES_AUIB_Pop(gstVdcDb[hVdcCh].ui32VesCh, &stAUInfo))
    {
        switch(stAUInfo.eAuType)
        {
            default: break;
            case AU_EOS:
                     if (gstVdcDb[hVdcCh].bLive)
                     {
                         /* live doesn't use eos typed au
                          * handle error exception */
                         log_warning("ch:%d live has eos au, discard it (0x%08X:%d)\n",hVdcCh, stBodaAU->ui32StartAddr, stBodaAU->ui32Size);
                         VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,TRUE);
                         /* has update size ? */
                         if (stBodaAU->ui32StartAddr==0 || stBodaAU->ui32Size==0)
                             return FALSE;
                         else
                             return TRUE;
                     }
                     else
                     {
                         // feed dummy au
                         log_debug("ch:%d Feed Eos Au \n",hVdcCh);
                         stBodaAU->bEndOfStream = TRUE;
                         gstVdcDb[hVdcCh].bFeedEOS = TRUE;
                         VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,TRUE);
                         return TRUE;
                     }
            case AU_SEQUENCE_HEADER:
                     log_debug("ch:%d Seq Header Detected, start:%X, size:%X, au_num:%d pts:%u ts64:%lld \n",
                             hVdcCh,stAUInfo.ui32AuStartAddr,stAUInfo.ui32AuSize
                             ,VES_AUIB_NumActive(gstVdcDb[hVdcCh].ui32VesCh),stAUInfo.ui32PTS,stAUInfo.ui64TimeStamp);

                     if( feeddump_mode == feeddump_header )
                         feeddump_header_found = TRUE;

                     if( gstVdcDb[hVdcCh].stCodecInfo.eCodecType != VDC_CODEC_HEVC && gstVdcDb[hVdcCh].stCodecInfo.eCodecType != VDC_CODEC_VP9)
                     {
                         if(stBodaAU->ui32StartAddr == 0)
                             stBodaAU->ui32StartAddr = stAUInfo.ui32AuStartAddr;
                         stBodaAU->ui32Size += stAUInfo.ui32AuSize;
                         VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,TRUE);
                         _VDC_DumpHeader(hVdcCh,stBodaAU);

                         goto auib_pop;
                     }
                     break;
        }

        // timestamp 90k tick changes for fileplay
        si64TimeStamp = (gstVdcDb[hVdcCh].bLive&&gstVdcDb[hVdcCh].bUsePdec)?stAUInfo.ui32PTS:stAUInfo.ui64TimeStamp;

        if( !gstVdcDb[hVdcCh].bLive && !gstVdcDb[hVdcCh].bUsePdec && si64TimeStamp != VDC_UNKNOWN_PTS) {
            ui64TsForDiv = si64TimeStamp * 9;
            do_div(ui64TsForDiv,100000);
            si64TimeStamp = ui64TsForDiv;
        }

        if(stBodaAU->ui32StartAddr == 0)
            stBodaAU->ui32StartAddr = stAUInfo.ui32AuStartAddr;
        stBodaAU->ui32Size += stAUInfo.ui32AuSize;

        if(gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec)
            log_debug("ch:%d Feed AU Success START:%8X(%7X) - END:%8X(%7X) SIZE:%7d(%5X) TYPE:%d PTS:%9u \n",hVdcCh,stBodaAU->ui32StartAddr,stBodaAU->ui32StartAddr-VES_CPB_GetBufferBaseAddr(gstVdcDb[hVdcCh].ui32VesCh),stAUInfo.ui32AuEndAddr,stAUInfo.ui32AuEndAddr-VES_CPB_GetBufferBaseAddr(gstVdcDb[hVdcCh].ui32VesCh),stBodaAU->ui32Size,stBodaAU->ui32Size,stAUInfo.eAuType,stAUInfo.ui32PTS);
        else
            log_debug("ch:%d Feed AU Success START:%8X(%11llX) - END:%8X(%11llX) SIZE:%7d(%5X) TYPE:%d TS:%9lld \n",hVdcCh,stBodaAU->ui32StartAddr,gstVdcDb[hVdcCh].stDebugInfo.ui64AccumAuSize,stAUInfo.ui32AuEndAddr,gstVdcDb[hVdcCh].stDebugInfo.ui64AccumAuSize+stBodaAU->ui32Size,stBodaAU->ui32Size,stBodaAU->ui32Size,stAUInfo.eAuType,si64TimeStamp);

        //log_user3("ch:%d Feed AU PTS:%9u DT S:%9u PTSorDTS:%d \n", hVdcCh,
        //		stAUInfo.ui32PTS, stAUInfo.ui32DTS, stAUInfo.DTS_PTS_flag);

        _VDC_DumpFirst8Bytes(hVdcCh,stBodaAU);

        if(gstVdcDb[hVdcCh].ui32FeededCounter==0){
            gstVdcDb[hVdcCh].ui32FeedStartTime = TOP_HAL_GetGSTCC();
        }
        gstVdcDb[hVdcCh].ui32FeededCounter++;
        if( gstVdcDb[hVdcCh].ui32PrevRdPtr == VDC_INVALID_ADDRESS )
            gstVdcDb[hVdcCh].ui32PrevRdPtr = stBodaAU->ui32StartAddr;
        gstVdcDb[hVdcCh].ui32LastFeededPtr = stAUInfo.ui32AuEndAddr;
        gstVdcDb[hVdcCh].ui32DecRetryCounter = 0;

        gstVdcDb[hVdcCh].stDebugInfo.ui32ConsumedAU++;
        gstVdcDb[hVdcCh].stDebugInfo.ui32TotConsumedAU++;

        VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,TRUE);
        //VES_CPB_UpdateRdPtr(gstVdcDb[hVdcCh].ui32VesCh, stAUInfo.ui32AuEndAddr);

        // if field feeding mode
        if(ui32PopNum > 1 && gstVdcDb[hVdcCh].ui32ScanMode < VDC_SKIP_PB ){
            ui32PopNum--;
            goto auib_pop;
        } else {

            feeddump_dump(gstVdcDb[hVdcCh].ui32VesCh,
                    hVdcCh,FEED_TIMESTAMP, 0,
                    (unsigned char*)&si64TimeStamp,
                    sizeof(si64TimeStamp));

            feeddump_dump(gstVdcDb[hVdcCh].ui32VesCh,
                    hVdcCh,FEED_DATA,
                    stBodaAU->ui32StartAddr,NULL,
                    stBodaAU->ui32Size);

            gstVdcDb[hVdcCh].stDebugInfo.ui64AccumAuSize += stBodaAU->ui32Size;

            return TRUE;
        }
    }
    return FALSE;
}

static void _VDC_DumpHeader(VDC_HANDLE_T hVdcCh, VDU_AU_T *stBodaAU)
{
    if(log_enabled (log_level_user3) && gstVdcDb[hVdcCh].eSrc != VDC_SRC_BUFFTVP)
    {
        char tmp[64];
        int a, b;
        unsigned long cpb_base;
        unsigned char *cpb_base_virt;
        int cpb_size;

        cpb_base = VES_CPB_GetBufferBaseAddr (gstVdcDb[hVdcCh].ui32VesCh);
        cpb_size = VES_CPB_GetBufferSize (gstVdcDb[hVdcCh].ui32VesCh);
        cpb_base_virt = VES_CPB_TranslatePhyWrPtr (gstVdcDb[hVdcCh].ui32VesCh, cpb_base);

        for (b=0; b<8; b++)
        {
            for (a=0; a<16; a++)
            {
                int offset;

                offset = stBodaAU->ui32StartAddr - cpb_base;
                sprintf (tmp+a*3, "%02x ", cpb_base_virt[(offset+a+16*b)%cpb_size]);
            }

            log_user3 ("ch:%d %08x(%6d): %s\n",
                    hVdcCh, stBodaAU->ui32StartAddr, stBodaAU->ui32Size, tmp);
        }

    }
}

static void _VDC_DumpFirst8Bytes(VDC_HANDLE_T hVdcCh, VDU_AU_T *stBodaAU)
{
    if(log_enabled (log_level_user3) && gstVdcDb[hVdcCh].eSrc != VDC_SRC_BUFFTVP)
    {
        char tmp[64];
        int a;
        unsigned long cpb_base;
        unsigned char *cpb_base_virt;
        int cpb_size;

        cpb_base = VES_CPB_GetBufferBaseAddr (gstVdcDb[hVdcCh].ui32VesCh);
        cpb_size = VES_CPB_GetBufferSize (gstVdcDb[hVdcCh].ui32VesCh);
        cpb_base_virt = VES_CPB_TranslatePhyWrPtr (gstVdcDb[hVdcCh].ui32VesCh, cpb_base);

        for (a=0; a<8; a++)
        {
            int offset;

            offset = stBodaAU->ui32StartAddr - cpb_base;
            sprintf (tmp+a*3, "%02x ", cpb_base_virt[(offset+a)%cpb_size]);
        }

        log_user3 ("ch:%d %08x(%6d): %s\n",
                hVdcCh, stBodaAU->ui32StartAddr, stBodaAU->ui32Size, tmp);

    }
}
/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static BOOLEAN _VDC_PrepareDec(VDC_HANDLE_T hVdcCh,VDU_LINEAR_FRAMES_T *stFrmsInfo)
{
    CHAR tmpBuf[256]={0,};
    UINT8 ui8Idx;
    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Invalid channel \n", hVdcCh);
        return FALSE;
    }

    gstVdcDb[hVdcCh].bFlushed = FALSE;
    gstVdcDb[hVdcCh].bWaitingClear = FALSE;

    log_trace("ch:%d Decoding start force:%d wait_clear:%d \n",hVdcCh,
                gstVdcDb[hVdcCh].bForceDecoding, gstVdcDb[hVdcCh].bWaitingClear);

    if( gstVdcDb[hVdcCh].eLinearBufMode > 0 &&
            gstVdcDb[hVdcCh].stExtBufsInfo.ui8NumberOfFrames > 0 ) {

        // copy top's buffer info to vdu's buffer info
        *stFrmsInfo = gstVdcDb[hVdcCh].stExtBufsInfo;

        // clear buffer info from top
        _VDC_InitExtBufsInfo(&gstVdcDb[hVdcCh].stExtBufsInfo);

        for( ui8Idx = 0; ui8Idx < stFrmsInfo->ui8NumberOfFrames; ui8Idx++ ) {
            sprintf(tmpBuf+ui8Idx*9,"%08x ", stFrmsInfo->aui32AddressList[ui8Idx]);
        }

        log_trace("ch:%d stride:%d list_length:%d buf_addr:%s \n"
                ,hVdcCh,stFrmsInfo->ui32Stride,stFrmsInfo->ui8NumberOfFrames,tmpBuf);
    }
    atomic_set(&gstVdcDb[hVdcCh].atIsDecoding,1);
    gstVdcDb[hVdcCh].ui32DecodingTime = TOP_HAL_GetGSTCC();
    if(STC_TIMER_GetStatus()==0) STC_TIMER_Start(VDC_WATCHDOG_TIME);
    return TRUE;
}

static BOOLEAN _VDC_DecPostProc(VDC_HANDLE_T hVdcCh,VDU_RET_T eRetVal)
{
    if (gstVdcDb[hVdcCh].state == VDC_CLOSING)
    {
        log_noti ("ch:%d Closing Channel returns decoding \n",hVdcCh);
        return FALSE;
    }


    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Invalid channel \n", hVdcCh);
        return FALSE;
    }

    if(eRetVal==VDU_RET_DPB_FULL)
    {
        log_warning("ch:%d DPB FULL \n",hVdcCh);
        atomic_set(&gstVdcDb[hVdcCh].atIsDecoding,0);
        gstVdcDb[hVdcCh].bWaitingClear = TRUE;
        gstVdcDb[hVdcCh].bPopped = FALSE;
    }
    else if(eRetVal!=VDU_RET_OK)
    {
        log_warning("ch:%d Decoding Failed,Retcode:%d \n",hVdcCh,eRetVal);
        atomic_set(&gstVdcDb[hVdcCh].atIsDecoding,0);

        if (gstVdcDb[hVdcCh].bFeedEOS)
        {
            gstVdcDb[hVdcCh].ui32EOSWaitCounter++;

            if (gstVdcDb[hVdcCh].ui32EOSWaitCounter > gstVdcDb[hVdcCh].ui32DPBSize)
            {
                if(gstVdcDb[hVdcCh].decoded_q != NULL)
                {
                    decoded_buffer_t stDecBuf = {0,};
                    stDecBuf.report = DECODED_REPORT_EOS;

                    log_noti("ch:%d send EOS by timeout\n", hVdcCh);

                    decoded_q_push(gstVdcDb[hVdcCh].decoded_q, &stDecBuf);
                }
            }
        }
    }
    else
    {
        gstVdcDb[hVdcCh].bForceDecoding = FALSE;
        /* 1. callback called before function call end, reschedule.
         * 2. reschedule for multi-channel */
        if (atomic_read (&gstVdcDb[hVdcCh].atFeedHandle) >
             atomic_read (&gstVdcDb[hVdcCh].atIsDecoding) ||
             gstVdcCoreInfo.ui32NumofIdleCh > 1 )
        {
            log_trace("ch:%d will reschedule. feed_handle:%d is_decoding:%d use:%d idle:%d suspended:%d \n",
                        hVdcCh,atomic_read (&gstVdcDb[hVdcCh].atFeedHandle),
                        atomic_read (&gstVdcDb[hVdcCh].atIsDecoding),
                        gstVdcCoreInfo.ui32UseChNum,
                        gstVdcCoreInfo.ui32NumofIdleCh,
                        gstVdcDb[hVdcCh].bSuspended);
            atomic_set(&gstVdcDb[hVdcCh].atFeedHandle, 0);
            return TRUE;
        }
    }

    return FALSE;
}

/**
 ********************************************************************************
 * @brief
 *
 * @remark
 *  DETAIL INFORMATION
 * @par requirements:
 *
 * @param
 *
 * @return
 *
 ********************************************************************************
 */
static BOOLEAN _VDC_Close(VDC_HANDLE_T hVdcCh)
{
    VDU_STATUS_T 	eStatus;

    VDU_GetStatus(gstVdcDb[hVdcCh].hVduCh, &eStatus);

    if(gstVdcDb[hVdcCh].hVduCh != VDU_INVALID_HANDLE &&
            VDU_Close(gstVdcDb[hVdcCh].hVduCh) == VDU_RET_OK) {
        log_noti("ch:%d sub module(%d) close done \n",hVdcCh,gstVdcDb[hVdcCh].hVduCh);
    }
    else {
        log_error("ch:%d sub module(%d) close failed \n",hVdcCh,gstVdcDb[hVdcCh].hVduCh);
        return FALSE;
    }

    // free extern bufs info
    if( gstVdcDb[hVdcCh].eLinearBufMode > 0 ) {
        if( gstVdcDb[hVdcCh].stExtBufsInfo.aui32AddressList != NULL )
            kfree( gstVdcDb[hVdcCh].stExtBufsInfo.aui32AddressList );
    }
    kfifo_free(&gstVdcDb[hVdcCh].stTSfifo);

    _VDC_InitializeDB(hVdcCh);

    del_timer( &gstVdcDb[hVdcCh].stChTimer );

    gstVdcCoreInfo.ui32UseChNum--;
    if(gstVdcCoreInfo.ui32UseChNum == 0){
        STC_TIMER_Stop();
    }


    log_noti("ch:%d closed, feed_handle:%d, decoding:%d callback_processing:%d idle chs:%d total chs:%d \n"
            , hVdcCh, atomic_read(&gstVdcDb[hVdcCh].atFeedHandle), eStatus, atomic_read(&gstVdcDb[hVdcCh].atIsDecoding),gstVdcCoreInfo.ui32NumofIdleCh,
            gstVdcCoreInfo.ui32UseChNum);

    return TRUE;
}

/* NOTE : actually mpeg-4 packed stream 2 au has 3 frames and extra 8-bytes-dummy
 * au for timestamp. below bCompenPacked is workaround codes for compensating
 * packed mode stream. but it is unuseful code now because upper layer gives
 * dismatched timestamp with frame. in current operation, on the contrary it
 * makes wrong timestamp matching. maybe it can be reuse fixed upper layer in
 * future.
 * */
static void _VDC_SaveTS(VDC_HANDLE_T hVdcCh,UINT32 ui32StartAddr, UINT32 ui32EndAddr, SINT8 si8FrameIndex)
{
    UINT32 index;
    UINT32 ui32AuStart, ui32AuEnd, ui32Start, ui32End;
    UINT32 ui32NumofAu;
    UINT32 ui32consumed=0;
    //BOOLEAN bCompenPacked = FALSE;
    S_VES_AU_T stAUInfo = {0,};
    long long ts = LLONG_MAX;
    long long prev_ts;
    SINT64 si64TimeStamp = VDC_INVALID_TIMESTAMP;
    SINT64 si64duration = 0;
    int len;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Wrong Channel \n", hVdcCh);
        goto finish;
    }

    ui32NumofAu = VES_AUIB_NumActiveTsDb(gstVdcDb[hVdcCh].ui32VesCh);
    log_user1("ch:%d %d au in db \n", hVdcCh,ui32NumofAu);
    for(index=0; index<ui32NumofAu; index++)
    {
        log_user1("ch:%d index:%d \n",hVdcCh,index);
        if(VES_AUIB_GetAUInfo(gstVdcDb[hVdcCh].ui32VesCh, &stAUInfo))
        {
            ui32AuStart = stAUInfo.ui32AuStartAddr;
            ui32AuEnd = stAUInfo.ui32AuEndAddr;
            ui32Start = ui32StartAddr;
            ui32End = ui32EndAddr;
            // second condition is work-around for no timestamp in first picture
            /*if( bCompenPacked && gstVdcDb[hVdcCh].ui32DPBSize != VDC_INITIAL_DPB_SIZE )
                ui32End += VDC_PACKED_SIZE;*/

            log_user1("ch:%d index:%d org au_start:%8X au_end:%8X au_size:%8X rd_start:%8X rd_end:%8X rd_size:%8X decoded:%d \n",hVdcCh,index,
                    ui32AuStart,ui32AuEnd, GET_DIFF(ui32AuStart,ui32AuEnd),
                    ui32Start, ui32End, GET_DIFF(ui32Start,ui32End), si8FrameIndex );

            if( _VDC_CompenWraparound(hVdcCh,&ui32AuStart,&ui32AuEnd,&ui32Start,&ui32End) )
                log_user1("ch:%d after compen wrap-around au_start:%8X au_end:%8X au_size:%8X rd_start:%8X rd_end:%8X rd_size:%8X \n",
                        hVdcCh,ui32AuStart,ui32AuEnd,GET_DIFF(ui32AuStart,ui32AuEnd)
                        ,ui32Start,ui32End,GET_DIFF(ui32Start,ui32End));

            // check previous
            /* discard header's pts in live or hevc case  */
            if( ui32AuEnd <= ui32Start || (( gstVdcDb[hVdcCh].bLive ||
                        gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_HEVC)
                        && !_VDC_IsPicture(hVdcCh,stAUInfo.eAuType) )){
                log_user1("ch:%d consumed previous(or header) au in db \n",hVdcCh);
                VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
                if (unlikely(gstVdcDb[hVdcCh].eTsOrder == VDC_TS_UNDEFINED))
                    gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder++;
                continue;
            }

            si64TimeStamp = (gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec)?stAUInfo.ui32PTS:stAUInfo.ui64TimeStamp;
            // work-around for packed mode
            /*
            if( si64TimeStamp < 0 && ( gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_MPEG4_ASP
                        || gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_DIVX4
                        || gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_DIVX5
                        || gstVdcDb[hVdcCh].stCodecInfo.eCodecType == VDC_CODEC_XVID ) )
            {
                bCompenPacked = TRUE;
                log_user1("ch:%d endaddr:%X changed for packed mode\n",hVdcCh,ui32End);
            }*/
            /* work-around for h264 live. decoder notifies before address of aud.
             * and pdec hardware merges aud after slice. so this difference makes
             * mismatch between au end address and read pointer.
             * this problem must be fixed by pdec after m16u.
             * NOTE: below scheme must be used on when has updated pointer */
            if ( (gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec) &&
                    gstVdcDb[hVdcCh].stCodecInfo.eCodecType  == VDC_CODEC_H264_HP
                    && ui32AuEnd > ui32End && ui32End > ui32Start)
            {
                /* in some error scenario(found one wrap-around channel
                 * (SIG3-ATSC-US-AVC-11-1), but im not sure abt real bad-singal)
                 * ---------------------------------
                 * |<au_start               au_end>|
                 * | frm1(err) | frm2(err) |aud|sei|
                 * ---------------------------------
                 *             ^           ^
                 *            rd          rd
                 * pdec gathered non-complete 2 frames to one au,
                 * but boda decodes two error frames each. so work-around it. */
                if (unlikely(ui32AuStart==ui32End))
                {
                    log_noti("ch:%d h264 with sei, and pdec gathered 2 frames at one au.. error stream \n",hVdcCh);
                    /* not update au end. will choose unknown pts */
                }
                else
                {
                    log_user1("ch:%d h264 rd end compensated for aud & sei. rd_end:%X->%X \n",
                            hVdcCh,ui32End,ui32AuEnd);
                    /* for clearing current au from db*/
                    ui32End = ui32AuEnd;
                }
            }

            // check current
            if( ( ui32AuStart < ui32End && ui32AuEnd >= ui32End )
                    || ( ui32AuStart <= ui32Start && ui32AuEnd >= ui32Start )
                    || _VDC_CheckContainToAddress(ui32AuStart,ui32AuEnd,ui32Start,ui32End) ) {
                if( si64TimeStamp >= 0 )
                    ts = _VDC_DecideCurrentTs(hVdcCh, ts, si64TimeStamp, ui32AuStart, ui32AuEnd, ui32End);
                // consume totally used one
                if( ui32AuEnd <= ui32End ) {
                    log_user1("ch:%d consumed current au in db \n",hVdcCh);
                    VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
                    if (unlikely(gstVdcDb[hVdcCh].eTsOrder == VDC_TS_UNDEFINED))
                        gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder++;
                    if( ui32AuEnd == ui32End  )//&& !bCompenPacked)
                        goto save;
                    else
                        continue;
                }
            }

            // check future
            if( ui32AuStart >= ui32End ){
                log_user1("ch:%d stop searching au db \n",hVdcCh);
                goto save;
            }

            // for finding next AU DB
            VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
            ui32consumed++;
        }
        else
            log_user1("ch:%d get au info failed \n", hVdcCh);
    }
save:
    // restoring DB for next ts deciding to save
    for(index=0; index<ui32consumed; index++)
    {
        VES_AUIB_RestoreOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
    }

    /* no valid candidate exception handling ( workaround for no valid
     * timestamp in first picture ). we must be extremely careful when using */
    if( unlikely (ts == LLONG_MAX && gstVdcDb[hVdcCh].ui32DPBSize == VDC_INITIAL_DPB_SIZE && !gstVdcDb[hVdcCh].bLive) )
    {
        ts = _VDC_GenerateTimestamp(hVdcCh, &si64duration);
        log_user1("ch:%d generated minimum timestamp:%lld, frame_duration:%lld",
                hVdcCh, ts, si64duration);
    }

    /* must update timestamp pool even there is not valid timstamp */
    if (unlikely(ts == LLONG_MAX))
        ts = VDC_UNKNOWN_PTS;

    /* [5]TB_1280x1088_23.976.mkv(h264), PITBULL.m2ts,
     * feeds dts order timestamp first one or some seconds,
     * but it changes to pts order timestamp after one or some seconds.
     * so below code needs for changing order */
    if (unlikely(gstVdcDb[hVdcCh].eTsOrder == VDC_TS_DTS_ORDER &&
                ts != VDC_UNKNOWN_PTS &&
                gstVdcDb[hVdcCh].si64prevSavedTS != VDC_UNKNOWN_PTS &&
                gstVdcDb[hVdcCh].si64prevSavedTS > ts &&
                gstVdcDb[hVdcCh].ui32ScanMode == VDC_SKIP_NONE))
    {
        log_noti("ch:%d ts order changed to pts order, prevts:%lld ts:%lld\n",
                hVdcCh,gstVdcDb[hVdcCh].si64prevSavedTS,ts);
        gstVdcDb[hVdcCh].eTsOrder = VDC_TS_PTS_ORDER;
    }

    /* don't allow saving same pts for two picture except for MVC
     * and it's also dangerous at ascending matching logic in dts order */
    if( unlikely ( (ts == gstVdcDb[hVdcCh].si64prevSavedTS &&
                ts != VDC_UNKNOWN_PTS &&
                gstVdcDb[hVdcCh].stCodecInfo.eCodecType != VDC_CODEC_H264_MVC &&
                gstVdcDb[hVdcCh].eTsOrder != VDC_TS_UNKNOWN_ORDER ) ||
                si8FrameIndex < 0))
    {
        log_user1("ch:%d discard same or skipped pts %llu \n",hVdcCh,ts);
        ts = VDC_UNKNOWN_PTS;
    }
    else /* don't update prev saved ts above case */
    {
        gstVdcDb[hVdcCh].si64prevSavedTS = ts;
    }

    if( likely(si8FrameIndex >= 0) ) {

        /* prepare both fifo ,ts slot, rbtree for dts/pts/unknown case */
        kfifo_in(&gstVdcDb[hVdcCh].stTSfifo,&ts,sizeof(SINT64));
        gstVdcDb[hVdcCh].si64TSInfo[si8FrameIndex] = ts;
        _VDC_InsertTs(hVdcCh,ts);
        log_user1("ch:%d detect empty pool, so save %lld at %d fifo_len:%d \n",
                hVdcCh,ts,si8FrameIndex,kfifo_len(&gstVdcDb[hVdcCh].stTSfifo)/sizeof(SINT64));

    }
    else /* frame skipped */
    {
        /* update for dts & unknown order without trick play */
        if (gstVdcDb[hVdcCh].ui32ScanMode == VDC_SKIP_NONE)
        {
            /* maybe queue can be starved initial playing */
            if (kfifo_len(&gstVdcDb[hVdcCh].stTSfifo) > 0)
            {
                len = kfifo_out(&gstVdcDb[hVdcCh].stTSfifo,&prev_ts,sizeof(SINT64));
            }
            kfifo_in(&gstVdcDb[hVdcCh].stTSfifo,&ts,sizeof(SINT64));
            log_user1("ch:%d fifo out:%lld in:%lld size:%d \n",
                    hVdcCh,prev_ts,ts,kfifo_len(&gstVdcDb[hVdcCh].stTSfifo)
                    /sizeof(SINT64));
            /* update skipped picture's timestamp for unknown order */
            _VDC_DecideTS(hVdcCh);
            _VDC_InsertTs(hVdcCh,ts);
        }
        else
            log_user1("ch:%d found ts but no decoded idx(%d), %lld discarded.. \n",hVdcCh,si8FrameIndex,ts);
    }
finish:
    return;
}

// this function is now using for no timestamp in first picture that sent with sequence header
#define VDC_SORT_DB_SIZE    32
static SINT64 _VDC_GenerateTimestamp(VDC_HANDLE_T hVdcCh, SINT64 *si64duration)
{
    UINT32 index;
    UINT32 ui32NumofAu,ui32Max;
    UINT32 saved=0;
    SINT64 ts = LLONG_MAX;
    SINT64 prevts = LLONG_MAX;
    SINT64 diff_cur,diff = LLONG_MAX;
    SINT64 genTs = LLONG_MAX;
    SINT64 si64SortedDb[VDC_SORT_DB_SIZE] = {0, };
    UINT64 ui64TsForDiv;
    S_VES_AU_T stAuInfo = {0, };

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Wrong Channel \n", hVdcCh);
        goto finish;
    }

    /* maked temporary db */
    ui32NumofAu = VES_AUIB_NumActiveTsDb(gstVdcDb[hVdcCh].ui32VesCh);
    ui32Max = ui32NumofAu<VDC_SORT_DB_SIZE?ui32NumofAu:VDC_SORT_DB_SIZE;
    log_user1("ch:%d num_au:%d \n",hVdcCh,ui32NumofAu);
    for(index=0; index<ui32Max; index++){
        if(VES_AUIB_GetAUInfo(gstVdcDb[hVdcCh].ui32VesCh,&stAuInfo))
        {
            /* ignore -1 timestamp at sequence header
             * and 0 ts at frame (case of h264 ts fileplay)
             * but must accept first 0 timestamp */
            if ( ((SINT64)stAuInfo.ui64TimeStamp == -1 ) ||
               (stAuInfo.ui64TimeStamp == 0 && ts != LLONG_MAX )||
                    stAuInfo.ui64TimeStamp == VDC_UNKNOWN_PTS)
                goto next;

            ts = stAuInfo.ui64TimeStamp;

            /* convert 90k domain */
            ui64TsForDiv = ts * 9;
            do_div(ui64TsForDiv,100000);
            ts = ui64TsForDiv;

            log_user1("ch:%d index:%d ts:%lld ts90k:%lld \n",
                    hVdcCh,index,stAuInfo.ui64TimeStamp,ts);

            si64SortedDb[saved] = ts;
            saved++;
next:
            VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,FALSE);
        }
    }
    /* sort current timestamp db for calculating right fake-generated timestamp */
    BubbleSort(si64SortedDb,saved);

    for(index=0; index<saved; index++){

        ts = si64SortedDb[index];
        log_user1("ch:%d sorted index:%d ts:%lld \n",hVdcCh,index,ts);

        // make frame duration if there is no framerate in es (or it didn't report yet)
        if( *si64duration == 0  && prevts != LLONG_MAX
                && prevts != ts ) {
            diff_cur = GET_DIFF(prevts,ts);
            if(diff_cur < diff) {
                log_user1("ch:%d found min diff:%lld prev diff:%lld \n"
                        ,hVdcCh,diff_cur,diff);
                diff = diff_cur;
            }
        }

        // find smallest timestamp
        if( ts < genTs ) {
            log_user1("ch:%d found min ts:%lld prev ts:%lld \n"
                    ,hVdcCh,ts,genTs);
            genTs = ts;
        }

        // non update prevts when ts = 0 (in case of one valid ts in both field)
        prevts = ts;
    }
    for(index=0; index<ui32Max; index++)
        VES_AUIB_RestoreOneAU(gstVdcDb[hVdcCh].ui32VesCh,FALSE);
    /* if durations is not found, it means all timestamp are same */
    if (diff == LLONG_MAX)
        diff = 0;
    *si64duration = diff;
    if( genTs != LLONG_MAX && genTs > *si64duration)
        genTs -= *si64duration;
    else
        genTs = 0;
finish:
    return genTs;
}

/* ts : will decided ts for current frame,
 * si64TimeStamp: timestamp of current au*/
static SINT64 _VDC_DecideCurrentTs(VDC_HANDLE_T hVdcCh, SINT64 ts, SINT64 si64TimeStamp, UINT32 ui32AuStart, UINT32 ui32AuEnd, UINT32 ui32End)
{
    UINT64 ui64TsForDiv;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Wrong Channel \n", hVdcCh);
        goto finish;
    }

    // timestamp domain change to 90k tick at fileplay
    if( !gstVdcDb[hVdcCh].bLive &&
            !gstVdcDb[hVdcCh].bUsePdec &&
            si64TimeStamp != VDC_UNKNOWN_PTS) {
        ui64TsForDiv = si64TimeStamp * 9;
        do_div(ui64TsForDiv,100000);
        si64TimeStamp = ui64TsForDiv;
    }

    /* before ignoring ts at SaveTS func, give a chance for getting another ts */
    if (unlikely(ts == gstVdcDb[hVdcCh].si64prevSavedTS)) {
        log_user1("ch:%d ignored same with prev saved ts: %lld \n",hVdcCh,ts);
        ts = UNKNOWN_PTS;
    }

    /* h264 ts fileplay some stream gives invalid zero timestamp at top filed */
    if (unlikely(si64TimeStamp == 0 &&
				gstVdcDb[hVdcCh].si64prevSavedTS != UNKNOWN_PTS &&
				gstVdcDb[hVdcCh].si64prevSavedTS>0)) {
        log_user1("ch:%d ignored zero timestamp prev saved ts:%lld \n",
                hVdcCh,gstVdcDb[hVdcCh].si64prevSavedTS);
        si64TimeStamp = UNKNOWN_PTS;
    }

    if( ts == LLONG_MAX ) {
        ts = si64TimeStamp;
        log_user1("ch:%d got first timestamp:%lld \n",hVdcCh,ts);
    } /* only change timestamp when there is no candidate yet */
    else if( ts == UNKNOWN_PTS && si64TimeStamp != UNKNOWN_PTS )
    {
        log_user1("ch:%d timestamp changed %lld to %lld \n",hVdcCh,ts,si64TimeStamp);
        ts = si64TimeStamp;
    }
finish:
    return ts;
}

static BOOLEAN _VDC_CheckContainToAddress(UINT32 ui32Start, UINT32 ui32End, UINT32 ui32TargetS, UINT32 ui32TargetE)
{
    if( ( ( ui32TargetS <= ui32Start ) && ( ui32End <= ui32TargetE ) ) &&
            ( ( ui32Start < ui32End ) || ( ui32TargetE < ui32TargetS ) ) ) {
        log_user1(" taget contained \n");
        return TRUE;
    }
    return FALSE;
}

static BOOLEAN _VDC_CompenWraparound(VDC_HANDLE_T hVdcCh, UINT32 *ui32AuStart, UINT32 *ui32AuEnd, UINT32 *ui32Start, UINT32 *ui32End)
{
    UINT32 ui32CPBSize;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Wrong Channel \n", hVdcCh);
        goto finish;
    }

    ui32CPBSize = VES_CPB_GetBufferSize(gstVdcDb[hVdcCh].ui32VesCh);

    // check AU wraparounded (case 1)
    if( *ui32AuStart > *ui32AuEnd ) {
        if( *ui32Start > *ui32End ) {
            *ui32End += ui32CPBSize;
            *ui32AuEnd += ui32CPBSize;
        }
        else {
            if( GET_DIFF(*ui32AuStart,*ui32Start) > GET_DIFF(*ui32AuEnd,*ui32Start) ) {
                *ui32AuStart -= ui32CPBSize;
            } else {
                *ui32AuEnd += ui32CPBSize;
            }
        }
        log_user1("ch:%d au wraparounded compen case 1 \n",hVdcCh);
        return TRUE;
    }

    // check read pointer wraparounded (case 2)
    if( *ui32Start > *ui32End ) {
        if( *ui32AuStart > *ui32AuEnd ) {
            *ui32AuEnd += ui32CPBSize;
        }
        if( GET_DIFF(*ui32Start,*ui32AuStart) > GET_DIFF(*ui32End,*ui32AuStart) ) {
            *ui32Start -= ui32CPBSize;
        } else {
            *ui32End += ui32CPBSize;
        }
        log_user1("ch:%d au wraparounded compen case 2 \n",hVdcCh);
        return TRUE;
    }

    // one side complete wraparounded (case 3)
    if( *ui32AuStart < *ui32AuEnd )
    {
        if( GET_DIFF(*ui32AuStart,*ui32Start) > GET_DIFF(*ui32AuStart+ui32CPBSize,*ui32Start) ) {
            *ui32AuStart += ui32CPBSize;
            *ui32AuEnd += ui32CPBSize;
            log_user1("ch:%d au wraparounded compen case 3 \n",hVdcCh);
            return TRUE;
        }
    }
finish:
    return FALSE;
}

static UINT64 _VDC_DecideTS(VDC_HANDLE_T hVdcCh)
{
    UINT32 index;
    UINT32 mintsidx;
    UINT32 maxIndex;
    long long ts = LLONG_MAX;

    if(hVdcCh >= VDEC_NUM_OF_CHANNEL || gstVdcDb[hVdcCh].state < VDC_PLAY)
    {
        log_warning("ch:%d Wrong Channel \n", hVdcCh);
        goto finish;
    }

    maxIndex = (gstVdcDb[hVdcCh].ui32DPBSize==VDC_INITIAL_DPB_SIZE)?VDC_MAX_DPB_SIZE:gstVdcDb[hVdcCh].ui32DPBSize;

    // timestamp matching
    for(index=0; index<maxIndex; index++)
    {
        if(gstVdcDb[hVdcCh].si64UTSInfo[index] == VDC_INVALID_TIMESTAMP) continue;

        if (!gstVdcDb[hVdcCh].bLive)
        {
            log_user1("ch:%d index:%d ts:%lld \n",hVdcCh,index
                    ,gstVdcDb[hVdcCh].si64UTSInfo[index]);
        }

        // timestamp can be minus value cuz of first frame that has no timestamp
        // if use minus timestamp, comment below first condition
        if(gstVdcDb[hVdcCh].si64UTSInfo[index] >= 0 &&
                gstVdcDb[hVdcCh].si64UTSInfo[index] < ts){
            mintsidx = index;
            ts = gstVdcDb[hVdcCh].si64UTSInfo[index];
        }
    }
    if(ts == LLONG_MAX){
        if(gstVdcDb[hVdcCh].bLive || gstVdcDb[hVdcCh].bUsePdec) ts = VDC_UNKNOWN_PTS;
        else ts = VDC_INVALID_TIMESTAMP;
    } else {
        gstVdcDb[hVdcCh].si64UTSInfo[mintsidx] = VDC_INVALID_TIMESTAMP;
        log_user1("ch:%d selected & cleared ts info %d : %lld \n"
                ,hVdcCh,mintsidx,ts);
    }
finish:
    return ts;
}

static void _VDC_DefineTsOrder(VDC_HANDLE_T hVdcCh)
{
    UINT32 index;
    UINT32 ui32NumofAu;
    UINT32 ui32Consumed = 0;
    UINT32 ui32SamePtsCounter = 0;
    BOOLEAN bFoundPtsOrder = FALSE;
    SINT64 prevTs = -1;
    SINT64 curTs;
    S_VES_AU_T stAUInfo = {0,};

    /* for more accurate prediction */
    for(index=0;index<gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder;index++)
    {
        VES_AUIB_RestoreOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
    }

    ui32NumofAu = VES_AUIB_NumActiveTsDb(gstVdcDb[hVdcCh].ui32VesCh);
    log_user1("ch:%d %d au in db \n", hVdcCh, ui32NumofAu);
    for(index=0;index<ui32NumofAu;index++)
    {
        if(VES_AUIB_GetAUInfo(gstVdcDb[hVdcCh].ui32VesCh, &stAUInfo))
        {
            /* skip 0 or -1 timestamps */
            curTs = gstVdcDb[hVdcCh].bUsePdec?stAUInfo.ui32PTS:stAUInfo.ui64TimeStamp;
            /* first boot-up mcu1 and first playing netflix seamless hevc,
             * wrong pts in first sps/pps/vps
             * so only consider picture type au.
             * needs to clarify why first pts is wrong.*/
            if ( curTs <= 0 || !_VDC_IsPicture(hVdcCh,stAUInfo.eAuType) )
                goto next;

            log_user1("ch:%d index:%d ts:%lld \n",
                    hVdcCh, index, curTs);
            /* break if timestamp order is pts */
            if( (prevTs > 0 && curTs < prevTs
                 && curTs != VDC_UNKNOWN_PTS )
                 || ui32SamePtsCounter > 1)
            {
                bFoundPtsOrder = TRUE;
                break;
            }
            /* for some mpeg2 which has one timestamp in one GOP */
            if (prevTs == curTs || curTs == VDC_UNKNOWN_PTS)
            {
                if (gstVdcDb[hVdcCh].stCodecInfo.eCodecType != VDC_CODEC_HEVC)
                {
                    log_user1("ch:%d index:%d prevts:%lld ts:%lld \n",
                            hVdcCh, index, prevTs,curTs);
                    ui32SamePtsCounter++;
                }
                /* else do nothing
                 * hevc can splitted many slices for one frame.
                 * so don't count same pts for hevc
                 * */
            }
            else
            {
                log_user1("ch:%d index:%d prevts:%lld ts:%lld \n",
                        hVdcCh, index, prevTs,curTs);
                ui32SamePtsCounter=0;
                prevTs = curTs;
            }
next:
            VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
            ui32Consumed++;
        }
    }

    if( bFoundPtsOrder )
        gstVdcDb[hVdcCh].eTsOrder = VDC_TS_PTS_ORDER;
    else
        gstVdcDb[hVdcCh].eTsOrder = VDC_TS_DTS_ORDER;

    log_noti("ch:%d pts order found:%d, ts order is %d \n"
            ,hVdcCh,bFoundPtsOrder,gstVdcDb[hVdcCh].eTsOrder);
    if (gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder > ui32Consumed)
    {
        /* more consuming previous decoded */
        for (index=0;index<gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder-ui32Consumed; index++)
            VES_AUIB_ConsumeOneAU(gstVdcDb[hVdcCh].ui32VesCh,FALSE);
    }
    else
    {
        /* only restore this time consumed */
        for(index=0;index<ui32Consumed-gstVdcDb[hVdcCh].ui32NumofConsumedBeforeDefineOrder;index++)
        {
            VES_AUIB_RestoreOneAU(gstVdcDb[hVdcCh].ui32VesCh, FALSE);
        }
    }
}

static void _VDC_InsertTs(VDC_HANDLE_T hVdcCh, SINT64 ts)
{
    UINT32 ui32Index;

    /* unknown order doesn't allow the unknown pts */
    if (ts==VDC_UNKNOWN_PTS)
        return;

    for (ui32Index=0; ui32Index<VDC_MAX_DPB_SIZE; ui32Index++)
    {
        if (gstVdcDb[hVdcCh].si64UTSInfo[ui32Index] == VDC_INVALID_TIMESTAMP)
        {
            gstVdcDb[hVdcCh].si64UTSInfo[ui32Index] = ts;
            break;
        }
    }
}

#ifdef USE_ZEROPADDING_DETECT
#define PADDING_SIZE_MAX 0x100
static BOOLEAN _VDC_IsZeroPadded(VDC_HANDLE_T hVdcCh, UINT32 ui32Start, UINT32 ui32End)
{
    UINT32 idx;
    static UINT32 tempBuf[PADDING_SIZE_MAX]={0,};
    UINT32 size = GET_SIZE(ui32Start,ui32End,VES_CPB_GetBufferSize(gstVdcDb[hVdcCh].ui32VesCh));
    BOOLEAN bNonzeroDetected=TRUE;

    if(size>PADDING_SIZE_MAX) {
        ui32Start = ui32End - PADDING_SIZE_MAX;
        size=PADDING_SIZE_MAX;
    }

    if(VES_CPB_Read(gstVdcDb[hVdcCh].ui32VesCh, ui32Start, size, tempBuf)){
        for(idx=0;idx<size; idx++)
        {
            log_user1("ch:%d idx:%d , val:%2x \n",hVdcCh,idx, tempBuf[idx]);
            if(tempBuf[idx] != 0)
            {
                bNonzeroDetected = FALSE;
                break;
            }
        }
        log_user1("ch:%d start:%08x end:%08x zero_padded:%d \n",hVdcCh,ui32Start,ui32End,bNonzeroDetected);
    }
    else bNonzeroDetected = FALSE;

    return bNonzeroDetected;
}
#endif

#define VDC_DPRINT(frm, args...)	seq_printf(file, frm, ##args)
void VDC_Print_Stat(struct seq_file *file)
{
    VDC_HANDLE_T hVdcCh;
    UINT32 ui32AvgDecodingTime=0;
    UINT64 ui64DiffTime;
    UINT64 ui64ConsumedInASecond;

    for(hVdcCh=0; hVdcCh<VDEC_NUM_OF_CHANNEL; hVdcCh++)
    {
        if( gstVdcDb[hVdcCh].state != VDC_PLAY )
            continue;

        ui64DiffTime = GET_DIFF(gstVdcDb[hVdcCh].stDebugInfo.ui64TotalPlayedTime,get_jiffies_64());
        do_div(ui64DiffTime,100);

        ui64ConsumedInASecond = gstVdcDb[hVdcCh].stDebugInfo.ui32TotConsumedSize;

        if(ui64DiffTime > 0)
            do_div(ui64ConsumedInASecond, ui64DiffTime);

        if(gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic > 0)
            ui32AvgDecodingTime = gstVdcDb[hVdcCh].stDebugInfo.ui32TotDecodingTime/(gstVdcDb[hVdcCh].stDebugInfo.ui32DisplayedPic*90); //ms

        VDC_DPRINT("\n");
        VDC_DPRINT("Playing Ch                  : %d \n",hVdcCh);
        VDC_DPRINT("Total Playing Time          : %lld second \n",ui64DiffTime);
        VDC_DPRINT("Codec Name                  : %s \n",gstVdcDb[hVdcCh].stCodecInfo.sName);
        VDC_DPRINT("Resolution                  : %dx%d \n",gstVdcDb[hVdcCh].stDebugInfo.ui32Width,gstVdcDb[hVdcCh].stDebugInfo.ui32Height);
        VDC_DPRINT("Framerate.Inter/Prog        : %2d.%02d%s \n",gstVdcDb[hVdcCh].ui32FrameRate/100,gstVdcDb[hVdcCh].ui32FrameRate%100,gstVdcDb[hVdcCh].stDebugInfo.sInterProg);
        VDC_DPRINT("CPB Base / Size             : 0x%X / 0x%X \n",VES_CPB_GetBufferBaseAddr(gstVdcDb[hVdcCh].ui32VesCh),VES_CPB_GetBufferSize(gstVdcDb[hVdcCh].ui32VesCh));
        VDC_DPRINT("AUIB / CPB depth            : %d / %X \n",VES_AUIB_NumActive(gstVdcDb[hVdcCh].ui32VesCh),VES_CPB_GetUsedBuffer(gstVdcDb[hVdcCh].ui32VesCh));
        VDC_DPRINT("Decoder Remaining           : %d bytes \n",GET_DIFF(gstVdcDb[hVdcCh].ui32LastFeededPtr,gstVdcDb[hVdcCh].ui32PrevRdPtr));
        VDC_DPRINT("DPB current / total         : %d / %d \n",gstVdcDb[hVdcCh].ui32NumofDPBOccupied,gstVdcDb[hVdcCh].ui32DPBSize);
        VDC_DPRINT("Decoding Delay(Reordering)  : %d frame(s) \n",gstVdcDb[hVdcCh].ui32NumofDPBOccupied-gstVdcDb[hVdcCh].stDebugInfo.ui32NumofDQOccupied);
        VDC_DPRINT("DPB state(*:dec,#:dis,-:emp): %s \n",gstVdcDb[hVdcCh].stDebugInfo.sDpbState);
        VDC_DPRINT("FPA(3D) / LR(seq_3d) / map  : %d / %d\n",gstVdcDb[hVdcCh].stDebugInfo.ui32FramePackingArrange,gstVdcDb[hVdcCh].stDebugInfo.ui32LRInfo);
        VDC_DPRINT("Field-coded Picture?        : %d \n",gstVdcDb[hVdcCh].stDebugInfo.bFieldCh);
        VDC_DPRINT("Last Displaying PTS         : %lld \n",gstVdcDb[hVdcCh].stDebugInfo.ui64LastPts);
        VDC_DPRINT("Consumed es                 : %lld bytes/second \n",ui64ConsumedInASecond);
        VDC_DPRINT("Avrg Decoding Time          : %d milli second/ea \n",ui32AvgDecodingTime);
        VDC_DPRINT("Total Consumed / Displayed  : %d / %d \n",gstVdcDb[hVdcCh].stDebugInfo.ui32TotConsumedAU, gstVdcDb[hVdcCh].stDebugInfo.ui32TotDisplayedPic);
        VDC_DPRINT("Total Dispd / with Errors   : %d / %d \n",gstVdcDb[hVdcCh].stDebugInfo.ui32TotDisplayedPic,gstVdcDb[hVdcCh].stDebugInfo.ui32NumofDecodedWithError);
        VDC_DPRINT("\n\n");
    }
}

void VDC_Print_Cmd(struct seq_file *file)
{
    VDC_DPRINT("\n\n");
    VDC_DPRINT("0                           :>>>>> Feeddump Ctrl Menu <<<<< \n\n");
    VDC_DPRINT("  c <0:disable,1:enable>    :enable/disable command dump mode [default:enabled] \n");
    VDC_DPRINT("  n <number>                :define number of es(pic) for dump [default:unlimited] \n");
    VDC_DPRINT("                            :given 0 or minus value will dump infinitely \n");
    VDC_DPRINT("  m <mode:f,d,h>            :select feed mode [default:f(feed)] \n");
    VDC_DPRINT(" NOTE: f will dump <n> feeded aus as soon as starting dump \n");
    VDC_DPRINT("       d <n> will dump all aus used for decode <n> pics \n");
    VDC_DPRINT("       h <n> will dump <n> feeded aus from sequence header, aus before header will be discarded \n");
    VDC_DPRINT("       one mode will disable after other mode enables \n");
    VDC_DPRINT("\n\n");
    VDC_DPRINT("1                           :>>>>> VDC Debug Ctrl Menu <<<<< \n\n");
    VDC_DPRINT("  u <0:disable,1:enable>    :enable/disable unlimit seq_init retry [default:disabled] \n");
    VDC_DPRINT("  f <codec_type>            :forced define video codec type(VDC_CODEC_T) \n");
    VDC_DPRINT("                            :0.h264,1~3.vc1,4.mpg2,5.mpg4,6.xvid,7.divx3 \n");
    VDC_DPRINT("                            :8.divx4,9.divx5,10:rv,11:avs,12.vp8,13.mvc \n");
    VDC_DPRINT("                            :14.theora,15.hevc,16.h263_sorenson, 17.vp9 \n");
    VDC_DPRINT("                            :-1.disable force codec mode \n");
    VDC_DPRINT("  l                         :enable linebuf mode \n");
    VDC_DPRINT("Usage: echo <Ctrl Menu> <opt> <arg> <opt2> <arg> ... \n");
}
#undef VDC_DPRINT

void VDC_DebugCmd(UINT32 ui32Param, CHAR *apParams[])
{
    UINT32 i;
    SINT32 i32Params[10];
    VDC_DEBUG_CMD_T eCurState = VDC_DEBUG_CMD_IDLE;

    for(i=0;i<10 && i<ui32Param;i++)
    {
        i32Params[i] = simple_strtol(apParams[i], NULL, 0);
        switch (eCurState )
        {
            case VDC_DEBUG_CMD_IDLE:
                {
                    switch (*apParams[i])
                    {
                        case 'u':
                        case 'U':
                            log_debug("unlimit retry \n");
                            eCurState = VDC_DEBUG_CMD_INF_RETRY;
                            break;
                        case 'f':
                        case 'F':
                            log_debug("forced codec type \n");
                            eCurState = VDC_DEBUG_CMD_FORC_CODEC;
                            break;
                        case 'l':
                        case 'L':
                            log_debug("line buf mode \n");
                            eCurState = VDC_DEBUG_CMD_LINEBUF;
                            break;
                        default:
                            break;
                    }
                }
                break;
            case VDC_DEBUG_CMD_INF_RETRY:
                {
                    log_debug("unlimit? %d \n",i32Params[i]);
                    vdc_unlimit_retry = i32Params[i];
                }
                eCurState = VDC_DEBUG_CMD_IDLE;
                break;
            case VDC_DEBUG_CMD_FORC_CODEC:
                {
                    log_debug("force codec? %d \n",i32Params[i]);
                    if( i32Params[i] == -1 )
                        vdc_force_codec_test = 0;
                    else {
                        vdc_force_codec_test = 1;
                        vdc_force_codec_type = i32Params[i];
                    }
                }
                eCurState = VDC_DEBUG_CMD_IDLE;
                break;
            case VDC_DEBUG_CMD_LINEBUF:
                {
                    vdc_test_linebuf = 1;
                }
                eCurState = VDC_DEBUG_CMD_IDLE;
                break;
            default:
                log_warning("state error? %d \n",eCurState);
                break;
        }
    }
}

