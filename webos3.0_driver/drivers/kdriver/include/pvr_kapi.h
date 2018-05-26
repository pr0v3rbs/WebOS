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
 *  application interface header for pvr device
 *
 *  author		Jihoon Lee (gaius.lee@lge.com)
 *  version		1.1
 *  date		2010.03.20
 *
 *  @addtogroup lg1150_pvr
 *	@{
 */

#ifndef	_PVR_KAPI_H_
#define	_PVR_KAPI_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	PVR_IOC_MAGIC		'a'
/**
@name PVR IOCTL List
ioctl list for pvr device.

@{
@def PVR_IOW_INIT
Initialize pvr module.
Set PVR to default status.

@def PVR_IOW_DN_START
Let PVR Module start processing packet from SDEC

@def PVR_IOW_DN_BUF_SET
Setting Ring buffer address to kernel driver.
Kernel driver management ringbuffer write address.

@def PVR_IOR_DN_GET_WRITE_ADD
Give read address to and
get write address from kernel driver.
Kernel driver return buffer status.
Handling both of Stream buffer and index buffer.
Normally, m/w take buffer 96KB by 2n. n is singed number.
Saturn 7 have download size range from 24K to 768K.

@def PVR_IOW_DN_SET_CONFIG
Reserved

@def PVR_IOW_PIE_START
Let PIE Module start

@def PVR_IOW_PIE_BUF_SET
Setting PIE buffer address to kernel driver.
Kernel driver management ringbuffer write address.

@def PVR_IOW_PIE_SET_PID
Setting PID and PID Type to PIE Module.
PID Type is MPEG2 TS and Others.
The uiDownChunkPktCount field specify the number of packets in each download chunk. The PIE numbering will restart with this number

@def PVR_IOW_PIE_SET_CONFIG
TBD

@def PVR_IOW_UP_INIT
Initialize PVR module and set buffer address and size.
This function is called when PVR up module is initialize.

@def PVR_IOW_UP_RESET
Reset the PVR Up module. Called before being used.

@def PVR_IOW_UP_UPLOAD_BUFFER
Provide kernel driver upload buffer.
User layer write buffer and let kernel driver knows address and size.
This buffer will be written continuosly. So kernel driver just increase write address.

@def PVR_IOW_UP_START
PVR Upload module start to processing buffer.
This IOCTL is called after first packet is loaded.

@def PVR_IOW_UP_PAUSE
Pause and resume processing packet.

@def PVR_IOR_UP_GET_STATE
Getting current PVR Upload module.
Upper layer ask to kernel driver about current status of upload module.
If kernel driver is ready to receive next buffer, it said ready to upper layer.
If not, it said busy. If upload processing is paused, it said pause."

*/
/*----------------------------------------------------------------------------------------
	IO comand naming rule  : MODULE_IO[R][W]_COMMAND
----------------------------------------------------------------------------------------*/
// General commands
#define	PVR_IOW_INIT								_IO(PVR_IOC_MAGIC,  0)
#define	PVR_IOW_TERMINATE							_IO(PVR_IOC_MAGIC,  1)
#define PVR_IOR_DN_GET_BUF							_IOWR(PVR_IOC_MAGIC,  2, LX_PVR_SET_BUFFER_T)
#define PVR_IOR_UP_GET_BUF							_IOWR(PVR_IOC_MAGIC,  3, LX_PVR_SET_BUFFER_T)
#define PVR_IOR_GET_HW_INFO							_IOWR(PVR_IOC_MAGIC,  4, LX_PVR_HW_INFO_T)

// Download
#define PVR_IOW_STOP_DN_SDT							_IO(PVR_IOC_MAGIC, 10)
#define PVR_IOW_DN_START							_IOWR(PVR_IOC_MAGIC, 11, LX_PVR_START_T)
#define PVR_IOW_DN_SET_BUF							_IOWR(PVR_IOC_MAGIC, 12, LX_PVR_SET_BUFFER_T)
#define PVR_IOR_DN_GET_WRITE_ADD					_IOWR(PVR_IOC_MAGIC, 13, LX_PVR_DN_GET_WRITE_ADD_T)
#define PVR_IOW_DN_SET_READ_ADDR					_IOWR(PVR_IOC_MAGIC, 14, LX_PVR_DN_SET_READ_ADD_T)
#define PVR_IOW_DN_STOP								_IOWR(PVR_IOC_MAGIC, 15, LX_PVR_START_T)

// Index File
#define PVR_IOW_PIE_START							_IOWR(PVR_IOC_MAGIC, 20, LX_PVR_START_T)
#define PVR_IOW_PIE_SET_BUF							_IOWR(PVR_IOC_MAGIC, 21, LX_PVR_SET_BUFFER_T)
#define PVR_IOW_PIE_SET_PID							_IOWR(PVR_IOC_MAGIC, 22, LX_PVR_PIE_SET_PID_T)
#define PVR_IOW_PIE_SET_CONFIG						_IOWR(PVR_IOC_MAGIC, 23, UINT32)
#define PVR_IOR_PIE_GET_TABLE						_IOWR(PVR_IOC_MAGIC, 24, LX_PVR_PIE_GET_TABLE_T)


// Upload
#define PVR_IOW_UP_INIT								_IOWR(PVR_IOC_MAGIC, 30, LX_PVR_SET_BUFFER_T)
#define PVR_IOW_UP_RESET							_IO(PVR_IOC_MAGIC, 31)
#define PVR_IOW_UP_UPLOAD_BUFFER					_IOWR(PVR_IOC_MAGIC, 32, LX_PVR_SET_BUFFER_T)
#define PVR_IOW_UP_START							_IOWR(PVR_IOC_MAGIC, 33, LX_PVR_START_T)
#define PVR_IOR_UP_GET_STATE						_IOWR(PVR_IOC_MAGIC, 34, LX_PVR_UP_GET_STATE_T)
#define PVR_IOW_UP_SET_BUF							_IOWR(PVR_IOC_MAGIC, 35, LX_PVR_SET_BUFFER_T)
#define PVR_IOW_UP_RESTART							_IOWR(PVR_IOC_MAGIC, 36, LX_PVR_SET_BUFFER_T)
#define PVR_IOW_UP_SET_SPEED						_IOWR(PVR_IOC_MAGIC, 37, LX_PVR_UP_SPEED_T)

//TBD - Trick play
#define PVR_IOW_UP_PAUSE							_IOWR(PVR_IOC_MAGIC, 38, LX_PVR_UP_PAUSE_T) /* Same for pause and resume */
#define PVR_IOW_UP_MODE								_IOWR(PVR_IOC_MAGIC, 39, LX_PVR_TRICK_MODE_T)


// Memory Pool
#define PVR_IOW_MM_GET_INFO							_IOWR(PVR_IOC_MAGIC, 45, LX_PVR_GPB_INFO_T)
#define PVR_IOW_MM_CACHE_CTRL						_IOWR(PVR_IOC_MAGIC, 46, LX_PVR_CACHE_CTRL_T)



#define PVR_IOR_GET_STREAM_INFO						_IOWR(PVR_IOC_MAGIC, 50, LX_PVR_STREAMINFO_T)

/* To control the prints from PVR Kdriver */
#define PVR_IOW_PRINT_CONTROL						_IOWR(PVR_IOC_MAGIC, 51, LX_PVR_PRINT_CONTROL_T)

/* marine temp */
#define PVR_IOW_PANIC								_IOWR(PVR_IOC_MAGIC, 52, LX_PVR_PANIC_T)



#define PVR_IOR_GET_REG								_IOWR(PVR_IOC_MAGIC, 54, LX_PVR_GET_REG_T)
#define PVR_IOW_SET_REG								_IOWR(PVR_IOC_MAGIC, 55, LX_PVR_SET_REG_T)
#define PVR_IOR_GET_STATUS							_IOWR(PVR_IOC_MAGIC, 56, LX_PVR_STATUS_T)

#define PVR_IOR_PIE_GET_DATA						_IOWR(PVR_IOC_MAGIC, 57, LX_PVR_PIE_GET_DATA_T)

#define PVR_IOC_MAXNR				60
/** @} */

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Enum Type Definitions
----------------------------------------------------------------------------------------*/

/**
 * PVR Module selection
 */
typedef enum
{
	LX_PVR_CH_A 					= 0,
	LX_PVR_CH_B 					= 1,
	LX_PVR_CH_MAX 					= LX_PVR_CH_B,
	LX_PVR_CH_NUM
}LX_PVR_CH_T;

/**
 * Return Code
 * return code of kernel driver/adaptation driver.
 */
typedef enum
{
	LX_PVR_E_OK		 			= 0,		/**< OK */
	LX_PVR_E_Error				= 1,		/**< Something is wrong */
	LX_PVR_E_InvalidParam		= 2			/**< Parameter is not valid*/
} LX_PVR_Error_T;

/**
 * Status of buffer
 * Kernel driver return this buffer status with data.
 * Empty/Full/Ready
 * @see LX_PVR_DN_GET_WRITE_ADD_T
 */
typedef enum
{
	LX_PVR_BUF_STAT_READY		= 0,		/**< Data is ready */
	LX_PVR_BUF_STAT_EMPTY		= 1,		/**< Buffer is empty */
	LX_PVR_BUF_STAT_FULL		= 2			/**< Buffer is full!!*/
} LX_PVR_BUF_STAT_T;

/**
 * PID Type
 * PIE module want to know this PID type is MPEG2 TS or not.
 * @see LX_PVR_PIE_SET_PID_T
 */
typedef enum
{
	LX_PVR_PIE_TYPE_NONE		= 0,
	LX_PVR_PIE_TYPE_MPEG2TS,				/**< PID Type is MPEG2 TS */
	LX_PVR_PIE_TYPE_H264TS,					/**< PID Type is MPEG4 H264 TS */
	LX_PVR_PIE_TYPE_HEVCTS,					/**< PID Type is MPEG4 HEVC TS */
	LX_PVR_PIE_TYPE_OTHERS,					/**< STREAM is not MPEG2 TS */
	LX_PVR_PIE_TYPE_MAX
} LX_PVR_PIE_TYPE_T;

/**
 * Play / pause mode
 * @see LX_PVR_UP_PAUSE_T
 */
typedef enum
{
	LX_PVR_UP_RESUME				= 0, 		/**< STREAM is not MPEG2 TS */
	LX_PVR_UP_PAUSE 				= 1			/**< PID Type is MPEG2 TS */
}LX_PVR_UP_PLAY_T;

typedef enum
{
	LX_PVR_UPMODE_NORMAL					= 0,		/**< Normal Play mode */
	LX_PVR_UPMODE_TRICK_MODE				= 1,  		/**< Trick Play mode */
	LX_PVR_UPMODE_SLOW						= 2,		/**< 0.5x Play mode */
	LX_PVR_UPMODE_MAX
} LX_PVR_UP_MODE_T;

/**
 * Packet length of upload stream
 * @see LX_PVR_START_T
 */
typedef enum
{
	LX_PVR_STREAM_TYPE_192		= 0,		/**< Stream with timestamp */
	LX_PVR_STREAM_TYPE_188		= 1,		/**< Stream without timestamp */
} LX_PVR_PACKET_LEN_T;


/**
 * PVR Upload Module State
 * If user layer give order to kernel layer 'start', state is PLAY.
 * @see LX_PVR_UP_GET_STATE_T
 */
 typedef enum
{
    LX_PVR_UP_STATE_IDLE			= 0,		/**< I'm stopped by user layer & not started yet */
	LX_PVR_UP_STATE_PLAY			= 1,		/**< I'm processing buffer from user layer */
    LX_PVR_UP_STATE_PAUSE		= 2			/**< I'm taking a rest now. user layer let me pause. */
} LX_PVR_UP_STATE_T;

typedef enum
{
	LX_PVR_DN_STATE_IDLE			= 0,		/**< Recording is in idle state */
    LX_PVR_DN_STATE_RECORD			= 1,		/**< Recording currently in progress */
    LX_PVR_DN_STATE_OVERFLOW		= 2			/**< Overflow state, will be reset after user reads data next time */
} LX_PVR_DN_STATE_T;

typedef enum
{
	LX_PVR_PIE_STATE_IDLE			= 0,		/**< Picture index is in idle state */
    LX_PVR_PIE_STATE_MP2			= 1,		/**< MP2 based PIE */
    LX_PVR_PIE_STATE_GSCD			= 2,		/**< GSCD based PIE active */
} LX_PVR_PIE_STATE_T;

typedef enum
{
	LX_PVR_DN_MIN_PKT_COUNT			= 0x80,			/**< Minimum chunk size 24K**/
	LX_PVR_DN_PKT_COUNT_DEFAULT		= 0x100,		/**< 48K */
    LX_PVR_DN_MAX_PKT_COUNT			= 0x1800,		/**< Max chunk size in number of packets */
} LX_PVR_DN_PACKET_COUNT;

#if 0
typedef enum
{
	LX_PVR_DN_MIN_PKT_COUNT			= 0x200,		/**< Minimum chunk size 512packets */
	LX_PVR_DN_PKT_COUNT_DEFAULT		= 0x800,		/**< Default packet count and chunk size - 384KB -2048packets*/
    LX_PVR_DN_MAX_PKT_COUNT			= 0x1800,		/**< Max chunk size in number of packets */
} LX_PVR_DN_PACKET_COUNT;
#endif

typedef enum
{
    LX_PVR_GEN_PRINT_ENABLE			= 1,		/**< General unclassified prints */
    LX_PVR_TRACE_ENABLE				= 2,		/**< IOCTL-Ok kind of tracing */
	LX_PVR_REG_PRINT_ENABLE			= 4,		/**< Register read/write value monitoring */
    LX_PVR_ISR_PRINT_ENABLE			= 8			/**< Prints from ISR */
} LX_PVR_PRINT_MASK_T;

typedef enum
{
	LX_PVR_FR_FORBIDDEN = 0,
	LX_PVR_FR_23_976HZ = 1,
	LX_PVR_FR_24HZ,
	LX_PVR_FR_25HZ,
	LX_PVR_FR_29_97HZ,
	LX_PVR_FR_30HZ,
	LX_PVR_FR_50HZ,
	LX_PVR_FR_59_94HZ,
	LX_PVR_FR_60HZ,
	LX_PVR_FR_UNKNOWN
} LX_PVR_FRAMERATE_T;


typedef enum
{
	LX_PVR_BLOCK_NONE		= 0,
	LX_PVR_BLOCK_TOP		= 0x01,
	LX_PVR_BLOCK_PIE_A,
	LX_PVR_BLOCK_PIE_B,
	LX_PVR_BLOCK_DN_A,
	LX_PVR_BLOCK_DN_B,
	LX_PVR_BLOCK_UP_A,
	LX_PVR_BLOCK_UP_B,
	LX_PVR_BLOCK_MAX
} LX_PVR_BLOCK_T;


/**
 * Buffer memory type
 */
typedef enum
{
	LX_PVR_MEM_DN					= 0,		/**< Download Buffer */
	LX_PVR_MEM_UP					= 1,		/**< Upload Buffer */
	LX_PVR_MEM_NUM								/**< Number of memory structure */
} LX_PVR_MEM_TYPE_T;



/*----------------------------------------------------------------------------------------
	Stucture Type Definitions
----------------------------------------------------------------------------------------*/


/**
 * PVR Start Mode.
 * Let PVR Module start processing packet from SDEC
 * @see PVR_IOW_DN_START, PVR_IOW_UP_START
 */
typedef struct
{
	LX_PVR_PACKET_LEN_T		ePacketLen;			/** < Packet length of stream. Only valid for upload */
	BOOLEAN					bStart;				/**<  start / stop */
} LX_PVR_START_T;


/**
 * Memory info base structure
 */
typedef struct
{
	UINT32				base;		/* Base address of memory*/
	UINT32				size;		/* Size of memory */
} LX_PVR_MEM_INFO_T;

/**
 * Get H/W information from kernel driver.
 * @see PVR_IOR_GET_HW_INFO
 */
typedef struct
{
	UINT8				num_dndev;		/* number of upload device */
	UINT8				num_updev;		/* number of download device */
	UINT8				mem_shared;		/* shared channel A mem in other channel */
} LX_PVR_HW_INFO_T;



/**
 * Setting Ring buffer address to kernel driver.
 * Kernel driver management ringbuffer write address.
 * @see PVR_IOW_DN_BUF_SET
 * @see PVR_IOW_M2T_BUF_CONFIG
 */

typedef struct
{
	UINT32					uiBufAddr;			/**< Start address of buffer */
	UINT32					uiBufSize;			/**< Size of buffer */
} LX_PVR_SET_BUFFER_T;

/**
 * Data structure for download stream data and index data
 * @see LX_PVR_DN_GET_WRITE_ADD_T
 */
typedef struct
{
	UINT32					uiReadAddr;		/**< give read address to kernel driver */
	UINT32					uiWriteAddr;		/**< get write address from kernel driver */
	LX_PVR_BUF_STAT_T		eStatus;			/** < get status from kernel driver */
} LX_PVR_BUFFER_T;


/**
 * getting write address from kernel driver.
 * give read address and let kernel driver decide current status.
 * pvr module return stream data and index data concurrently.
 * @see PVR_IOR_DN_GET_WRITE_ADD
 */
typedef struct
{
	LX_PVR_BUFFER_T			streamBuffer;		/**< information of stream buffer */
} LX_PVR_DN_GET_WRITE_ADD_T;

/**
 * setting read address to kernel driver.
 * give read address and let kernel driver decide current status.
 * @see PVR_IOR_DN_SET_READ_ADD
 */
typedef struct
{
	UINT32					readAddr;

} LX_PVR_DN_SET_READ_ADD_T;

/**
 * Setting pid to make index file.
 * @see PVR_IOW_PIE_SET_PID
 */
typedef struct
{
	LX_PVR_PIE_TYPE_T		ePidType;				/**< PID Type */
	UINT32					uiPid;					/**< PID which is source of making index file */
	UINT32					uiDownChunkPktCount;	/**< Num of 192 byte packets in each Down chunk Min-0x200Pkts, Max-0x1800Pkts */
} LX_PVR_PIE_SET_PID_T;


/**
 * PIE database
 */
typedef enum
{
	LX_PVR_IDX_SEQ			= 0x01,		/* sequence header */
	LX_PVR_IDX_I_PIC		= 0x02,		/* I picture */
	LX_PVR_IDX_SEQ_I_PIC	= 0x03,		/* sequence header and  I picture */
	LX_PVR_IDX_P_PIC		= 0x04,		/* P picture */
	LX_PVR_IDX_B_PIC		= 0x08,		/* B picture */
	LX_PVR_IDX_NONE			= 0xFF		/* nothing */
} LX_PVR_INDEX_TYPE_T;

/**
 * PVR Picture index
 */
typedef struct
{
	LX_PVR_INDEX_TYPE_T		type;	/* Index Type ( I/B/P ) */
	UINT32					offset;	/* Byte offset of index */
} LX_PVR_INDEX_T;


/**
 * getting write address of pie table from kernel driver.
 * pvr module return index data concurrently.
 * @see PVR_IOR_PIE_GET_TABLE
 */
#if 0
typedef struct
{
	LX_PVR_BUFFER_T			tsBuffer;			/**< information of TS buffer */
	LX_PVR_BUFFER_T			indexBuffer;		/**< information of index buffer */
} LX_PVR_PIE_GET_TABLE_T;
#else
typedef struct
{
	LX_PVR_BUFFER_T		ts;				/**< in - information of TS buffer */

	LX_PVR_INDEX_T		*index;			/**< out - index data */
	UINT32				size;			/**< in/out  - index size(num of index) */
} LX_PVR_PIE_GET_TABLE_T;
#endif


typedef union { // 0x2780
	UINT32	ui32Val;
	struct {
		UINT32 byte_info       : 8;   // 31:24   24 	//byte information
		UINT32 sdet            : 1;   // 23      23		// sequence header detected
		UINT32 idet            : 1;   // 22      22		// I picutre header detected
		UINT32 pdet            : 1;   // 21      21		// P picture header detected
		UINT32 bdet            : 1;   // 20      20		// B picutre header detected
		UINT32 maxp            : 1;   // 19      19		// maximum position
		UINT32 buf_num         : 5;   // 18:14   14		//buffer number
		UINT32 pack_cnt        : 14;  // 13:0    0		//buffer number
	}be;											//big endian
	struct {
		UINT32 pack_cnt        : 14;  // 13:0    0		//buffer number
		UINT32 buf_num         : 5;   // 18:14   14		//buffer number
		UINT32 maxp            : 1;   // 19      19		// maximum position
		UINT32 bdet            : 1;   // 20      20		// B picutre header detected
		UINT32 pdet            : 1;   // 21      21		// P picture header detected
		UINT32 idet            : 1;   // 22      22		// I picutre header detected
		UINT32 sdet            : 1;   // 23      23		// sequence header detected
		UINT32 byte_info       : 8;   // 31:24   24		//byte information
	}le;											//little endian
} LX_PVR_PIE_IND_T;

typedef struct
{
	UINT32					ui32PieDataCnt;		/* count of PIE datas */
	UINT32					ui32BufNum[64];		/* buffer number of TS buffer */
	LX_PVR_PIE_IND_T		ui32PieData[64];	/* PIE data */
} LX_PVR_PIE_GET_DATA_T;

/**
 * Pause Setting
 * @see PVR_IOW_UP_PAUSE
 */
typedef struct
{
	LX_PVR_UP_PLAY_T		ePlay;				/**< LX_PVR_UP_RESUME/LX_PVR_UP_PAUSE */
} LX_PVR_UP_PAUSE_T;

/**
 * Pause Setting
 * @see PVR_IOW_UP_FAST_FORWARD
 */
typedef struct
{
	LX_PVR_UP_MODE_T		eMode;				/**< LX_PVR_UP_RESUME/LX_PVR_UP_PAUSE */
} LX_PVR_TRICK_MODE_T;


/**
 * Upload Speed Setting
 * @see PVR_IOW_UP_SET_SPEED
 */
typedef struct
{
	UINT32					speed;				/**< speed - bytes/sec */
} LX_PVR_UP_SPEED_T;


/**
 * Getting current PVR Upload module
 * Upper layer ask to kernel driver about current status of  upload module.
 * Uppse layer know where is read address is now, so can manage buffer slot.
 * @see PVR_IOR_UP_GET_STATE
 */
typedef struct
{
	LX_PVR_BUFFER_T			upBuffer;			/**< information of stream buffer */
	LX_PVR_UP_STATE_T		eUpState;			/**< status of upload module. */
} LX_PVR_UP_GET_STATE_T;

/**
 * Getting current PVR sream info
 * @see PVR_IOR_GET_STREAM_INFO
 */
typedef struct
{
	BOOLEAN 			bFoundSeqSPS;
	UINT32				bitRate;
	LX_PVR_FRAMERATE_T 	frRate;
} LX_PVR_STREAMINFO_T;

typedef struct
{
	UINT32					ui32PrintMask;				/**< Mask of values ORed from LX_PVR_PRINT_MASK_T*/
} LX_PVR_PRINT_CONTROL_T;


typedef struct
{
	UINT32	dataaddress; 		/* data */
	UINT32	startBitPos;		/* Start bit position. 7 to 0. 7 for left first bit. */
	SINT32	dataSize;			/* data size */
	UINT32	bitSize;			/* parsed bit size */
} LX_PVR_PANIC_T;



/**
 * Information of PVR gpb
 * @see PVR_IOW_MM_Init
 */
typedef struct
{
	UINT32					base;		/* Base address of GPB */
	UINT32					size;		/* Size of GPB */
	LX_PVR_MEM_INFO_T		buf[LX_PVR_CH_NUM][LX_PVR_MEM_NUM];
} LX_PVR_GPB_INFO_T, *P_LX_PVR_GPB_INFO_T;



/**
 * read register with register direct address
 * @see PVR_IOR_GET_REG, PVR_IOW_SET_REG
 */
typedef struct
{
	LX_PVR_BLOCK_T			block;			/**< in  -  PVR Block */
	UINT32					offset;			/**< in  -  offset of register from block start address */
	UINT32					value;			/**< in/out -  value of register written by kernel drvier */
} LX_PVR_GET_REG_T, LX_PVR_SET_REG_T;


typedef struct
{
	UINT32		data_size[2];			/**< out - data size in buffer */
} LX_PVR_STATUS_T;



/**
 * cache control for SDEC memory
 *
 */
typedef enum
{
	LX_PVR_CACHE_OP_INVAL = 0x0,
	LX_PVR_CACHE_OP_CLEAN = 0x1,
} LX_PVR_CACHE_OP_T;

typedef struct
{
	LX_PVR_CACHE_OP_T		op;

	void					*paddr;				/**< physical address */
	void					*vaddr;				/**< virtual address returned by mmap */
	UINT32					size;				/**< memory length */
} LX_PVR_CACHE_CTRL_T;





/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PVR_DRV_H_ */

/** @} */
