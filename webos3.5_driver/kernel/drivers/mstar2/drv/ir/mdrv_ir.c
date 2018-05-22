////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvIR.c
/// @brief  IR Control Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
//#include "MsCommon.h"
#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/time.h>  //added
#include <linux/timer.h> //added
#include <linux/types.h> //added
#include <asm/io.h>
#include <linux/semaphore.h>
#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
#ifdef CONFIG_ARM
#include <linux/version.h>
#elif CONFIG_ARM64
#include <generated/uapi/linux/version.h>
#else
#include <version.h>
#endif
#include <linux/mutex.h>
#include <freezer.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
#include <media/rc-core.h>
#else
#include <media/ir-common.h>
#endif
#endif
#ifdef CONFIG_MSTAR_IR_POWER_KEY_LONG_PRESS
#include <linux/reboot.h>
#endif

#include "board/Board.h"
#include "mst_devid.h"
#include "mhal_ir_reg.h"
//#include "mdrv_ir_io.h"
#include "mdrv_ir.h"
#include "chip_int.h"
#include "mdrv_system.h"

#if defined(IR_TYPE_RCMM) && (IR_TYPE_SEL == IR_TYPE_RCMM)
#include "IR_RCMM.h"
#elif (IR_TYPE_SEL == IR_TYPE_OLD)
#include "IR_MSTAR_OLD.h"
#elif (IR_TYPE_SEL == IR_TYPE_NEW)
#include "IR_MSTAR_NEW.h"
#elif (IR_TYPE_SEL == IR_TYPE_MSTAR_DTV)
#include "IR_MSTAR_DTV.h"
#elif (IR_TYPE_SEL == IR_TYPE_RC_V16)
#include "IR_MSTAR_RC_V16.h"
#elif (IR_TYPE_SEL == IR_TYPE_MSTAR_RAW)
#include "IR_MSTAR_RAW.h"
#elif (IR_TYPE_SEL == IR_TYPE_CUS03_DTV)
#ifdef CONFIG_MSTAR_MUSTANG
#include "IR_CUS03_DTV.h"
#else // CONFIG_MSTAR_MUNICH
#include "IR_CUS03_DTV_munich.h"
#endif
#elif (IR_TYPE_SEL == IR_TYPE_MSTAR_FANTASY)
#include "IR_MSTAR_FANTASY.h"
#elif (IR_TYPE_SEL == IR_TYPE_MSTAR_SZ1)
#include "IR_MSTAR_SZ1.h"
#elif (IR_TYPE_SEL==IR_TYPE_CUS08_RC5)
#include "IR_CUS08_RC5.h"
#elif (IR_TYPE_SEL==IR_TYPE_CUS21SH)
#include "IR_MSTAR_CUS21SH.h"
#elif (IR_TYPE_SEL == IR_TYPE_TOSHIBA)
#include "IR_MSTAR_CUS22T.h"
#elif (defined(IR_TYPE_CHANGHONG) && IR_TYPE_SEL == IR_TYPE_CHANGHONG)
#include "IR_CHANGHONG.h"
#elif (defined(IR_TYPE_HAIER) && IR_TYPE_SEL == IR_TYPE_HAIER)
#include "IR_HAIER.h"
#elif (defined(IR_TYPE_HISENSE) && IR_TYPE_SEL == IR_TYPE_HISENSE)
#include "IR_HISENSE.h"
#elif (IR_TYPE_SEL == IR_TYPE_KONKA)
#include "IR_KONKA.h"
#elif (defined(IR_TYPE_SKYWORTH) && IR_TYPE_SEL == IR_TYPE_SKYWORTH)
#include "IR_SKYWORTH.h"
#elif (defined(IR_TYPE_TCL) && IR_TYPE_SEL==IR_TYPE_TCL)
#include "IR_TCL.h"
#elif (defined(IR_TYPE_HWRC5) && IR_TYPE_SEL==IR_TYPE_HWRC5)
#include "IR_MSTAR_HWRC5.h"
#else
#include "IR_MSTAR_DTV.h"
#endif

#define SWIR_FILEOPS 17

#ifdef CONFIG_MSTAR_PM_SWIR
#include "mdrv_mstypes.h"
#include "mdrv_mbx.h"

#include <fcntl.h>
#include <linux/signal.h>
#include <unistd.h>
#endif


#ifdef CONFIG_IR_SUPPLY_RNG
#include <linux/input.h>
#include <random.h>
#include "mhal_rng_reg.h"
#endif

#ifndef INT_NUM_IR_ALL
#define INT_NUM_IR_ALL (E_FIQ_IR) 
#endif

#if (IR_TYPE_SEL == IR_TYPE_MSTAR_FANTASY)
#define MSTAR_FANTASY_PROTOCOL_SUPPORTED 1
#else
#define MSTAR_FANTASY_PROTOCOL_SUPPORTED 0
#endif

#ifdef CONFIG_MSTAR_IR_FANTASY_MODE
#ifdef MSTAR_FANTASY_PROTOCOL_SUPPORTED
#undef MSTAR_FANTASY_PROTOCOL_SUPPORTED
#endif
#define MSTAR_FANTASY_PROTOCOL_SUPPORTED 1

#if 0
#ifdef IR_MODE_SEL
#undef IR_MODE_SEL
#endif
#define IR_MODE_SEL             IR_TYPE_RAWDATA_MODE
#endif
#endif      //#if (MSTAR_IR_FANTASY_MODE)

#include "IR_PROTOCOL_INSTALL.h"
#ifdef SUPPORT_MULTI_PROTOCOL
#ifdef IR_MODE_SEL
#undef IR_MODE_SEL
#define IR_MODE_SEL IR_TYPE_SWDECODE_MODE
#endif
#ifndef IR_INT_NP_EDGE_TRIG
#define IR_INT_NP_EDGE_TRIG
#endif
#endif

#if ((defined(IR_SWFIFO_MODE) && (IR_SWFIFO_MODE==ENABLE)) && ((IR_MODE_SEL!=IR_TYPE_FULLDECODE_MODE)&& (IR_MODE_SEL!=IR_TYPE_RAWDATA_MODE)))
#define IR_SWFIFO_DECODE_MODE ENABLE
#else
#define IR_SWFIFO_DECODE_MODE DISABLE
#endif

extern IRModHandle IRDev;

#ifdef CONFIG_MSTAR_SOFTWARE_IR_MODULE
extern int take_over_by_software_ir(unsigned char, unsigned char, unsigned char, struct input_dev *, struct ir_input_state *);
extern int software_ir_enable();
extern int take_over_by_software_ir_dfb(unsigned char, unsigned char);
extern int software_ir_processing_undone();
extern int set_software_ir_processing_undone();
#endif

//static irqreturn_t  MDrv_IR_ISRParseKey(void);

#ifndef IR_EVENT_TIMEOUT
#define IR_EVENT_TIMEOUT 220
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
#define REG(addr)                   (*(volatile u32 *)(addr))
#define IR_PRINT(fmt, args...)      //printk(KERN_EMERG "IR: [%05d] " fmt, __LINE__, ## args)
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static U8 _u8IRHeaderCode0 = IR_HEADER_CODE0;
static U8 _u8IRHeaderCode1 = IR_HEADER_CODE1;
static U8 ir_irq_depth;

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
#ifndef IR2_HEADER_CODE0
#define IR2_HEADER_CODE0 0xffUL
#endif
#ifndef IR2_HEADER_CODE1
#define IR2_HEADER_CODE1 0xffUL
#endif
static U8 _u8IR2HeaderCode0 = IR2_HEADER_CODE0;
static U8 _u8IR2HeaderCode1 = IR2_HEADER_CODE1;
#endif

#if defined(IR_TYPE_HISENSE) && (IR_TYPE_SEL == IR_TYPE_HISENSE)
static U8 _u8FactoryIRHeaderCode0 = IR_FHEADER_CODE0;
static U8 _u8FactoryIRHeaderCode1 = IR_FHEADER_CODE1;
static U8 _u8PCCommandHeaderCode0 = IR_PHEADER_CODE0;
static U8 _u8PCCommandHeaderCode1 = IR_PHEADER_CODE1;
#elif defined(IR_TYPE_HAIER) && (IR_TYPE_SEL == IR_TYPE_HAIER)
static U8 _u8FactoryIRHeaderCode0 = IR_HEADER_CODE3;
static U8 _u8FactoryIRHeaderCode1 = IR_HEADER_CODE4;
#elif (IR_TYPE_SEL == IR_TYPE_TOSHIBA)
static U8 _u8FactoryIRHeaderCode0 = IR_FHEADER_CODE0_0;
static U8 _u8FactoryIRHeaderCode1 = IR_FHEADER_CODE1_0;

#define	IR_REPEAT_SKIP_COUNT	2

#else
static U8 _u8FactoryIRHeaderCode0 = 0x00UL;
static U8 _u8FactoryIRHeaderCode1 = 0x00UL;
#endif

#define IR_RAW_DATA_NUM	        4
//#define IR_FILTER_REPEAT_NUM    1

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE) || defined(IR_INCLUDE_TV_LINK_AND_WB_CODE)
#define IR_SWDECODE_MODE_BUF_LEN        100
#endif

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE) || defined(IR_INCLUDE_TV_LINK_AND_WB_CODE)
#if(IR_TYPE_SEL != IR_TYPE_RCMM)
static U32  _u32IRData[IR_SWDECODE_MODE_BUF_LEN];
static U32  _u32IRCount=0;
static U32  _u32IRAllCount=0;
#endif
#endif

#ifdef CONFIG_MSTAR_IR_POWER_KEY_LONG_PRESS
static unsigned long _u16IRLongPressPrevTime = 0;
#endif

#if(IR_MODE_SEL == IR_TYPE_SWDECODE_KON_MODE)// pulse width modulation format: Header(1)+CusterCode(8)+Data(8)+Stop(1)
#define IR_KON_DATA_BITS          18       //
#define IR_KON_DETECT_START         BIT0   // start to detect IR int
#define IR_KON_DETECT_END             BIT1   // end to detect IR int
#define IR_KON_COUNTER_ERROR        BIT6   // for temp use
#define IR_KON_REPEATE_TIMEOUT      BIT5
static volatile U8 g_u8IrKonFlag;            // for store bit var
static volatile U8 g_u8IrKonBits;                // for store bit count
static volatile U16 g_u16IrKonCounter;         // for read counter from register
static volatile U16 g_u16IrKonData;    // store shift data
static volatile U16 g_u16IrKonDecode;    // store shift data
static U8 g_u8IrKonRepeatCount;    // for repeat
static U8 g_u8IrKonPreKey;
static unsigned long g_u8IrKonRepeatTimeout;
#define IR_KON_PWS3_HEADER_CNT_LB       3000
#define IR_KON_PWS3_HEADER_CNT_UB       4000
#define IR_KON_PWS3_LOGIC0_CNT_LB       1500
#define IR_KON_PWS3_LOGIC0_CNT_UB       2500
#define IR_KON_PWS3_LOGIC1_CNT_LB       2500
#define IR_KON_PWS3_LOGIC1_CNT_UB       3500
#define IR_KON_PWS3_STOP_CNT_LB       4000
#define IR_KON_PWS3_STOP_CNT_UB       5000
#define IR_KON_PWS3_REPEATE_TIMEOUT     150
#endif

#ifdef CONFIG_MSTAR_MUSTANG
#define IR_DEBUG
#endif

#ifdef IR_DEBUG
#define DEBUG_IR(x) (x)
#else
#define DEBUG_IR(x)
#endif

#ifdef CONFIG_MP_DEBUG_TOOL_CHANGELIST
char g_sChangeList[] = KERM_CL;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_IR_KEY_PROPERTY_INIT,
    E_IR_KEY_PROPERTY_1st,
    E_IR_KEY_PROPERTY_FOLLOWING
} IRKeyProperty;

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)) || (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20))

#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
static struct ir_scancode ir_codes_mstar_tv[] = {
    { 0x46UL, KEY_POWER },
    { 0x5AUL, KEY_MUTE },

    { 0x45UL, KEY_SLEEP },
    { 0x41UL, KEY_F1 }, // MTS
    { 0x44UL, KEY_AUDIO },
    { 0x4CUL, KEY_ZOOM },

    { 0x0BUL, KEY_F2 }, // BALANCE
    { 0x5CUL, KEY_F3 }, // CHANNEL_RETURN

    { 0x50UL, KEY_0 },
    { 0x49UL, KEY_1 },
    { 0x55UL, KEY_2 },
    { 0x59UL, KEY_3 },
    { 0x4DUL, KEY_4 },
    { 0x51UL, KEY_5 },
    { 0x5DUL, KEY_6 },
    { 0x48UL, KEY_7 },
    { 0x54UL, KEY_8 },
    { 0x58UL, KEY_9 },

    { 0x47UL, KEY_RED },
    { 0x4BUL, KEY_GREEN },
    { 0x57UL, KEY_YELLOW },
    { 0x5BUL, KEY_BLUE },

    { 0x07UL, KEY_MENU },
    { 0x1BUL, KEY_EXIT },
    { 0x4AUL, KEY_EPG },
    { 0x56UL, KEY_CAMERA }, // PHOTO

    { 0x52UL, KEY_UP },
    { 0x13UL, KEY_DOWN },
    { 0x06UL, KEY_LEFT },
    { 0x1AUL, KEY_RIGHT },
    { 0x0FUL, KEY_ENTER },

    { 0x0EUL, KEY_REWIND },
    { 0x12UL, KEY_FORWARD },
    { 0x02UL, KEY_PREVIOUSSONG },
    { 0x1EUL, KEY_NEXTSONG },
    { 0x01UL, KEY_PLAY },
    { 0x1DUL, KEY_PAUSE },
    { 0x11UL, KEY_STOP },

    { 0x1FUL, KEY_CHANNELUP },
    { 0x19UL, KEY_CHANNELDOWN },
    { 0x16UL, KEY_VOLUMEUP },
    { 0x15UL, KEY_VOLUMEDOWN },
    { 0x03UL, KEY_PAGEUP },
    { 0x05UL, KEY_PAGEDOWN },
    { 0x17UL, KEY_HOME},
    { 0x08UL, KEY_END},

    { 0x4EUL, KEY_F4 }, // FREEZE
    { 0x53UL, KEY_SUBTITLE },
    { 0x0AUL, KEY_F5 }, // TTX
    { 0x09UL, KEY_F6 }, // CC

    { 0x0DUL, KEY_RECORD },
    { 0x40UL, KEY_F7 }, // PIP
    { 0x18UL, KEY_F8 }, // INDEX
    { 0x1CUL, KEY_F9 }, // MIX

    { 0x04UL, KEY_F10 }, // SOURCE
    { 0x42UL, KEY_F11 }, // GUIDE
    { 0x14UL, KEY_INFO },
    { 0x10UL, KEY_CHANNEL }, // CHANNEL_LIST

    { 0x00UL, KEY_F12 }, // HOLD
    { 0x0CUL, KEY_F13 }, // UPDATE
    { 0x4FUL, KEY_F14 }, // REVEAL
    { 0x5EUL, KEY_F15 }, // SUBCODE

    { 0x43UL, KEY_F16 }, // SIZE
    { 0x5FUL, KEY_F17 }, // CLOCK
};

struct ir_scancode_table ir_codes_mstar_tv_table = {
    .scan = ir_codes_mstar_tv,
    .size = ARRAY_SIZE(ir_codes_mstar_tv),
};
EXPORT_SYMBOL_GPL(ir_codes_mstar_tv_table);
#else
IR_KEYTAB_TYPE ir_codes_mstar_tv[IR_KEYTAB_SIZE] =
{
	//
	//  <<-!!! THIS IRCODE IS MAPPED TO MSTAR DTV IR !!!->>
	//
	[ 0x50UL ] = KEY_0,
	[ 0x49UL ] = KEY_1,
	[ 0x55UL ] = KEY_2,
	[ 0x59UL ] = KEY_3,
	[ 0x4DUL ] = KEY_4,
	[ 0x51UL ] = KEY_5,
	[ 0x5DUL ] = KEY_6,
	[ 0x48UL ] = KEY_7,
	[ 0x54UL ] = KEY_8,
	[ 0x58UL ] = KEY_9,

	[ 0x52UL ] = KEY_UP,
	[ 0x13UL ] = KEY_DOWN,
	[ 0x06UL ] = KEY_LEFT,
	[ 0x1AUL ] = KEY_RIGHT,
	[ 0x0FUL ] = KEY_ENTER,

	[ 0x07UL ] = KEY_MENU,
	[ 0x1BUL ] = KEY_BACK,
	[ 0x47UL ] = KEY_HOME,
	[ 0x4BUL ] = KEY_SEARCH,

	[ 0x5AUL ] = KEY_MUTE,
	[ 0x16UL ] = KEY_VOLUMEUP,
	[ 0x15UL ] = KEY_VOLUMEDOWN,

	[ 0x46UL ] = KEY_POWER,
};

EXPORT_SYMBOL_GPL(ir_codes_mstar_tv);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
struct mstar_ir
{
    struct rc_dev *dev;
};
#else
struct mstar_ir
{
    struct input_dev *dev;
    struct ir_input_state ir;
};
#endif
#endif // CONFIG_MSTAR_IR_INPUT_DEVICE

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static U8 bIRPass = 0;
static pid_t MasterPid = 0;
#ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE
extern wait_queue_head_t	key_wait_q;
#endif

#ifdef CONFIG_MSTAR_SOFTWARE_IR_MODULE
U8 u8Key_for_mdrv_software_ir = 0;
U8 u8RepeatFlag_for_mdrv_software_ir = 0;
#endif

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
struct mstar_ir *ir;
struct completion key_completion;
#endif

//--------------------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
#if (IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
static U8   _u8IRRawModeBuf[IR_RAW_DATA_NUM];
static U32  _u8IRRawModeCount;
static unsigned long  _ulPrevKeyTime;
#endif

#ifdef SUPPORT_MULTI_PROTOCOL
 struct
 {
     wait_queue_head_t inq;
     struct semaphore sem;
     long long data; // 0=nothing, defined in fantasy protocol
 }fantasy_protocol;
#else
struct
{
    wait_queue_head_t inq;
    struct semaphore sem;
    U32 data; // 0=nothing, defined in fantasy protocol
}fantasy_protocol;
#endif

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)

#if(IR_TYPE_SEL == IR_TYPE_RCMM)
#define MAX_RCBYTE_LEN  4
static BOOL StartDecodeFlag = FALSE;
static U8 RCBitsCnt;
static U32 tgbits = 0;
static U8 RCByte[MAX_RCBYTE_LEN];
static U8 _u8IrPreRcmmData[MAX_RCBYTE_LEN];
static U16 u16CustomerID;
static BOOL UpDataFlage = FALSE;
static U8 RCMode;
static unsigned long  _ulPrevKeyTime;

#endif


#if ((IR_TYPE_SEL != IR_TYPE_CUS21SH) && (IR_TYPE_SEL != IR_TYPE_TOSHIBA))
#if ((IR_TYPE_SEL != IR_TYPE_CUS08_RC5) && (IR_TYPE_SEL != IR_TYPE_RCMM))
static U32  _u32IRData[IR_SWDECODE_MODE_BUF_LEN];
#endif
#else
static U16  _u16IRData[IR_SWDECODE_MODE_BUF_LEN];
#endif
//static U32  _u32IRCount=0;

#if (IR_TYPE_SEL == IR_TYPE_HISENSE || IR_TYPE_SEL == IR_TYPE_MSTAR_DTV || IR_TYPE_SEL == IR_TYPE_CHANGHONG)
static U8   _u8IRHeadReceived=0;
static U8   _u8IRRepeateDetect=0;
static U8   _u8IRRepeated=0;
static U8   _u8IRRepeatedNum=0;
static U8   _u8IRType=0;

#elif (IR_TYPE_SEL == IR_TYPE_CUS08_RC5)
static U16  _u16IrRc5Data=0;          // for store shift ir data
static U16  _u16PreIrRc5Data=0;          // for store previous ir data
static U8 _u8IrRc5Bits=0;                // for store bit count
static U8 _u8IrRc5LastBit=0;//IR_RC5_LAST_BIT;            // for store bit var
static unsigned long  _ulPrevKeyTime;

#elif(IR_TYPE_SEL == IR_TYPE_TCL)
static BOOL _brepeat_flag;
static unsigned long  _ulPrevKeyTime;
static unsigned long  _ulPrevKeyTimeFirstPress;	//qiyx add for "OK" Long Press Key

#elif (IR_TYPE_SEL == IR_TYPE_CUS21SH)
static U8   _u8IRReceiveDetect = 0;
static U8   _u8IRRepeateDetect = 0;
static U16  _u16IRKeyMap = 0;
static BOOL _bKeyValueHit = FALSE;

/////////
U8 g_bIrDetect;//!<IR command detect flag

U8 g_ucIrRepeatSkipCnt = 0; //IR_REPEAT_SKIP_COUNT;
U32 gIRTimeOutCount = 0;
static U16 _u16BufferCurrent=0;
static U16 _u16BufferPrev=0;
static U8 LastKeyData=0xFF;
static U8 IsRepeatData=0;
static U8 keyDatArrayWTIdx=0 ;
static U8 keyDatArrayRDIdx=0 ;
static BOOL _bExpectedTCome=TRUE;
static BOOL    FirstRXFlag=FALSE;
BOOL   SecondRXFlag=FALSE;
static U32 RxTimeOutCount;
static U32 CurrentTime;
static BOOL RxInProcess=FALSE;
static U32 _u32KeyOffTimeOutCount;
static BOOL ReceivingMode=FALSE;
static BOOL SetToStandbyMode=FALSE;

U8 g_u8Key;
U8 g_u8Flag;

////////

#elif (IR_TYPE_SEL == IR_TYPE_TOSHIBA)
static U32  _u32IRKeyMap = 0;
static BOOL _bRxInProcess = FALSE;
static BOOL _bIrRepeat = FALSE;
static BOOL _bIrDetect = FALSE;
static U32	_u32KeyOffTimeOutCount = 0;
static U8	_u8IrRepeatSkipCnt = IR_REPEAT_SKIP_COUNT;

#endif
#endif

static U32  _u32_1stDelayTimeMs;
static U32  _u32_2ndDelayTimeMs;
static IRKeyProperty _ePrevKeyProperty;

#if (IR_MODE_SEL != IR_TYPE_HWRC_MODE)
static U8   _u8PrevKeyCode;
#endif
static U8   _u8PrevSystemCode;

#if !defined(IR_TYPE_SKYWORTH) || (IR_TYPE_SEL != IR_TYPE_SKYWORTH)
#if (IR_MODE_SEL == IR_TYPE_FULLDECODE_MODE)
static unsigned long  _ulPrevKeyTime;
#endif
#endif

#if defined(IR_TYPE_HAIER) && (IR_TYPE_SEL == IR_TYPE_HAIER)
static BOOL _btoshiba_code;
static BOOL _brepeat_flag;
static BOOL _bBeginRepeat_flag;
static U8 	_Repeat_Num;
#endif
#if defined(IR_TYPE_SKYWORTH) && (IR_TYPE_SEL == IR_TYPE_SKYWORTH )
#if ENABLE_PIONEER_IR
static BOOL _brepeat_flag;
static unsigned long ulPreTime;
static U8 _u8BeforeHeader=0;
static unsigned long  _ulPrevKeyTime;
static BOOL _bIRStart;
static BOOL _bskyworth_pioneer_code;
static BOOL _bskyworth_normal_code;
static BOOL _bNextCode = FALSE;
static U8     _u8PioneerFirstKey;
static U8     _u8PioneerSecondKey;
static U32  _u32PioneerIRData[2][IR_SWDECODE_MODE_BUF_LEN];
#else
static BOOL _brepeat_flag;
static unsigned long ulPreTime;
static U8 _u8BeforeHeader=0;
static unsigned long  _ulPrevKeyTime;
static BOOL _bIRStart;
static BOOL	_bskyworth_shuttle_code;
static BOOL _bskyworth_normal_code;
#endif
#endif

static unsigned long  _ulLastKeyPresentTime;

static MS_IR_KeyInfo _KeyReceived;   //temporary solution
#ifdef SUPPORT_MULTI_PROTOCOL
static IR_PROCOCOL_TYPE _eCurentProtocol=E_IR_PROTOCOL_NONE;
IR_PROCOCOL_TYPE _eLastKeyProtocol=E_IR_PROTOCOL_NONE;//last used protocol of processing key success without timeout
static unsigned long long _u64LastData=0;//last rawdata of processing key success without timeout
static unsigned long _ulPreShotTime=0;
static IR_PROCOCOL_TYPE _eDetectList[PROTOCOL_SUPPORT_MAX];
static struct timer_list    _key_MultiProtocol_timer;
#endif

#ifdef	IR_INCLUDE_TV_LINK_AND_WB_CODE

static U8 	_u8IRMode = IR_MODE_NORMAL;
#if 0   // not support WD CODE when TV LINK MODE
static BOOL	_IsWBCodeMode = FALSE;
#endif
//static BOOL	_IsTvLinkMode = FALSE;
static BOOL _IsTvLinkActive = FALSE; //dongick.lee
static BOOL _IsTvLinkBitErr = FALSE; //dongick.lee
static U8   _u8DataTemp[IR_TV_LINK_DATA_NUM] = {0,};   //dongick.lee

static struct _TV_LINK_KEY_INFO_T
{
	U8	u8Data[IR_TV_LINK_DATA_NUM];
	U8	u8Flag;
	U8	u8Valid;

} _TvLinkReceived;

static const U8 _LSB[] =
{
/* 0x00 : 0x00 */ 0x00,
/* 0x01 : 0x80 */ 0x80,
/* 0x02 : 0x40 */ 0x40,
/* 0x03 : 0xc0 */ 0xc0,
/* 0x04 : 0x20 */ 0x20,
/* 0x05 : 0xa0 */ 0xa0,
/* 0x06 : 0x60 */ 0x60,
/* 0x07 : 0xe0 */ 0xe0,
/* 0x08 : 0x10 */ 0x10,
/* 0x09 : 0x90 */ 0x90,
/* 0x0a : 0x50 */ 0x50,
/* 0x0b : 0xd0 */ 0xd0,
/* 0x0c : 0x30 */ 0x30,
/* 0x0d : 0xb0 */ 0xb0,
/* 0x0e : 0x70 */ 0x70,
/* 0x0f : 0xf0 */ 0xf0,
/* 0x10 : 0x08 */ 0x08,
/* 0x11 : 0x88 */ 0x88,
/* 0x12 : 0x48 */ 0x48,
/* 0x13 : 0xc8 */ 0xc8,
/* 0x14 : 0x28 */ 0x28,
/* 0x15 : 0xa8 */ 0xa8,
/* 0x16 : 0x68 */ 0x68,
/* 0x17 : 0xe8 */ 0xe8,
/* 0x18 : 0x18 */ 0x18,
/* 0x19 : 0x98 */ 0x98,
/* 0x1a : 0x58 */ 0x58,
/* 0x1b : 0xd8 */ 0xd8,
/* 0x1c : 0x38 */ 0x38,
/* 0x1d : 0xb8 */ 0xb8,
/* 0x1e : 0x78 */ 0x78,
/* 0x1f : 0xf8 */ 0xf8,
/* 0x20 : 0x04 */ 0x04,
/* 0x21 : 0x84 */ 0x84,
/* 0x22 : 0x44 */ 0x44,
/* 0x23 : 0xc4 */ 0xc4,
/* 0x24 : 0x24 */ 0x24,
/* 0x25 : 0xa4 */ 0xa4,
/* 0x26 : 0x64 */ 0x64,
/* 0x27 : 0xe4 */ 0xe4,
/* 0x28 : 0x14 */ 0x14,
/* 0x29 : 0x94 */ 0x94,
/* 0x2a : 0x54 */ 0x54,
/* 0x2b : 0xd4 */ 0xd4,
/* 0x2c : 0x34 */ 0x34,
/* 0x2d : 0xb4 */ 0xb4,
/* 0x2e : 0x74 */ 0x74,
/* 0x2f : 0xf4 */ 0xf4,
/* 0x30 : 0x0c */ 0x0c,
/* 0x31 : 0x8c */ 0x8c,
/* 0x32 : 0x4c */ 0x4c,
/* 0x33 : 0xcc */ 0xcc,
/* 0x34 : 0x2c */ 0x2c,
/* 0x35 : 0xac */ 0xac,
/* 0x36 : 0x6c */ 0x6c,
/* 0x37 : 0xec */ 0xec,
/* 0x38 : 0x1c */ 0x1c,
/* 0x39 : 0x9c */ 0x9c,
/* 0x3a : 0x5c */ 0x5c,
/* 0x3b : 0xdc */ 0xdc,
/* 0x3c : 0x3c */ 0x3c,
/* 0x3d : 0xbc */ 0xbc,
/* 0x3e : 0x7c */ 0x7c,
/* 0x3f : 0xfc */ 0xfc,
/* 0x40 : 0x02 */ 0x02,
/* 0x41 : 0x82 */ 0x82,
/* 0x42 : 0x42 */ 0x42,
/* 0x43 : 0xc2 */ 0xc2,
/* 0x44 : 0x22 */ 0x22,
/* 0x45 : 0xa2 */ 0xa2,
/* 0x46 : 0x62 */ 0x62,
/* 0x47 : 0xe2 */ 0xe2,
/* 0x48 : 0x12 */ 0x12,
/* 0x49 : 0x92 */ 0x92,
/* 0x4a : 0x52 */ 0x52,
/* 0x4b : 0xd2 */ 0xd2,
/* 0x4c : 0x32 */ 0x32,
/* 0x4d : 0xb2 */ 0xb2,
/* 0x4e : 0x72 */ 0x72,
/* 0x4f : 0xf2 */ 0xf2,
/* 0x50 : 0x0a */ 0x0a,
/* 0x51 : 0x8a */ 0x8a,
/* 0x52 : 0x4a */ 0x4a,
/* 0x53 : 0xca */ 0xca,
/* 0x54 : 0x2a */ 0x2a,
/* 0x55 : 0xaa */ 0xaa,
/* 0x56 : 0x6a */ 0x6a,
/* 0x57 : 0xea */ 0xea,
/* 0x58 : 0x1a */ 0x1a,
/* 0x59 : 0x9a */ 0x9a,
/* 0x5a : 0x5a */ 0x5a,
/* 0x5b : 0xda */ 0xda,
/* 0x5c : 0x3a */ 0x3a,
/* 0x5d : 0xba */ 0xba,
/* 0x5e : 0x7a */ 0x7a,
/* 0x5f : 0xfa */ 0xfa,
/* 0x60 : 0x06 */ 0x06,
/* 0x61 : 0x86 */ 0x86,
/* 0x62 : 0x46 */ 0x46,
/* 0x63 : 0xc6 */ 0xc6,
/* 0x64 : 0x26 */ 0x26,
/* 0x65 : 0xa6 */ 0xa6,
/* 0x66 : 0x66 */ 0x66,
/* 0x67 : 0xe6 */ 0xe6,
/* 0x68 : 0x16 */ 0x16,
/* 0x69 : 0x96 */ 0x96,
/* 0x6a : 0x56 */ 0x56,
/* 0x6b : 0xd6 */ 0xd6,
/* 0x6c : 0x36 */ 0x36,
/* 0x6d : 0xb6 */ 0xb6,
/* 0x6e : 0x76 */ 0x76,
/* 0x6f : 0xf6 */ 0xf6,
/* 0x70 : 0x0e */ 0x0e,
/* 0x71 : 0x8e */ 0x8e,
/* 0x72 : 0x4e */ 0x4e,
/* 0x73 : 0xce */ 0xce,
/* 0x74 : 0x2e */ 0x2e,
/* 0x75 : 0xae */ 0xae,
/* 0x76 : 0x6e */ 0x6e,
/* 0x77 : 0xee */ 0xee,
/* 0x78 : 0x1e */ 0x1e,
/* 0x79 : 0x9e */ 0x9e,
/* 0x7a : 0x5e */ 0x5e,
/* 0x7b : 0xde */ 0xde,
/* 0x7c : 0x3e */ 0x3e,
/* 0x7d : 0xbe */ 0xbe,
/* 0x7e : 0x7e */ 0x7e,
/* 0x7f : 0xfe */ 0xfe,
/* 0x80 : 0x01 */ 0x01,
/* 0x81 : 0x81 */ 0x81,
/* 0x82 : 0x41 */ 0x41,
/* 0x83 : 0xc1 */ 0xc1,
/* 0x84 : 0x21 */ 0x21,
/* 0x85 : 0xa1 */ 0xa1,
/* 0x86 : 0x61 */ 0x61,
/* 0x87 : 0xe1 */ 0xe1,
/* 0x88 : 0x11 */ 0x11,
/* 0x89 : 0x91 */ 0x91,
/* 0x8a : 0x51 */ 0x51,
/* 0x8b : 0xd1 */ 0xd1,
/* 0x8c : 0x31 */ 0x31,
/* 0x8d : 0xb1 */ 0xb1,
/* 0x8e : 0x71 */ 0x71,
/* 0x8f : 0xf1 */ 0xf1,
/* 0x90 : 0x09 */ 0x09,
/* 0x91 : 0x89 */ 0x89,
/* 0x92 : 0x49 */ 0x49,
/* 0x93 : 0xc9 */ 0xc9,
/* 0x94 : 0x29 */ 0x29,
/* 0x95 : 0xa9 */ 0xa9,
/* 0x96 : 0x69 */ 0x69,
/* 0x97 : 0xe9 */ 0xe9,
/* 0x98 : 0x19 */ 0x19,
/* 0x99 : 0x99 */ 0x99,
/* 0x9a : 0x59 */ 0x59,
/* 0x9b : 0xd9 */ 0xd9,
/* 0x9c : 0x39 */ 0x39,
/* 0x9d : 0xb9 */ 0xb9,
/* 0x9e : 0x79 */ 0x79,
/* 0x9f : 0xf9 */ 0xf9,
/* 0xa0 : 0x05 */ 0x05,
/* 0xa1 : 0x85 */ 0x85,
/* 0xa2 : 0x45 */ 0x45,
/* 0xa3 : 0xc5 */ 0xc5,
/* 0xa4 : 0x25 */ 0x25,
/* 0xa5 : 0xa5 */ 0xa5,
/* 0xa6 : 0x65 */ 0x65,
/* 0xa7 : 0xe5 */ 0xe5,
/* 0xa8 : 0x15 */ 0x15,
/* 0xa9 : 0x95 */ 0x95,
/* 0xaa : 0x55 */ 0x55,
/* 0xab : 0xd5 */ 0xd5,
/* 0xac : 0x35 */ 0x35,
/* 0xad : 0xb5 */ 0xb5,
/* 0xae : 0x75 */ 0x75,
/* 0xaf : 0xf5 */ 0xf5,
/* 0xb0 : 0x0d */ 0x0d,
/* 0xb1 : 0x8d */ 0x8d,
/* 0xb2 : 0x4d */ 0x4d,
/* 0xb3 : 0xcd */ 0xcd,
/* 0xb4 : 0x2d */ 0x2d,
/* 0xb5 : 0xad */ 0xad,
/* 0xb6 : 0x6d */ 0x6d,
/* 0xb7 : 0xed */ 0xed,
/* 0xb8 : 0x1d */ 0x1d,
/* 0xb9 : 0x9d */ 0x9d,
/* 0xba : 0x5d */ 0x5d,
/* 0xbb : 0xdd */ 0xdd,
/* 0xbc : 0x3d */ 0x3d,
/* 0xbd : 0xbd */ 0xbd,
/* 0xbe : 0x7d */ 0x7d,
/* 0xbf : 0xfd */ 0xfd,
/* 0xc0 : 0x03 */ 0x03,
/* 0xc1 : 0x83 */ 0x83,
/* 0xc2 : 0x43 */ 0x43,
/* 0xc3 : 0xc3 */ 0xc3,
/* 0xc4 : 0x23 */ 0x23,
/* 0xc5 : 0xa3 */ 0xa3,
/* 0xc6 : 0x63 */ 0x63,
/* 0xc7 : 0xe3 */ 0xe3,
/* 0xc8 : 0x13 */ 0x13,
/* 0xc9 : 0x93 */ 0x93,
/* 0xca : 0x53 */ 0x53,
/* 0xcb : 0xd3 */ 0xd3,
/* 0xcc : 0x33 */ 0x33,
/* 0xcd : 0xb3 */ 0xb3,
/* 0xce : 0x73 */ 0x73,
/* 0xcf : 0xf3 */ 0xf3,
/* 0xd0 : 0x0b */ 0x0b,
/* 0xd1 : 0x8b */ 0x8b,
/* 0xd2 : 0x4b */ 0x4b,
/* 0xd3 : 0xcb */ 0xcb,
/* 0xd4 : 0x2b */ 0x2b,
/* 0xd5 : 0xab */ 0xab,
/* 0xd6 : 0x6b */ 0x6b,
/* 0xd7 : 0xeb */ 0xeb,
/* 0xd8 : 0x1b */ 0x1b,
/* 0xd9 : 0x9b */ 0x9b,
/* 0xda : 0x5b */ 0x5b,
/* 0xdb : 0xdb */ 0xdb,
/* 0xdc : 0x3b */ 0x3b,
/* 0xdd : 0xbb */ 0xbb,
/* 0xde : 0x7b */ 0x7b,
/* 0xdf : 0xfb */ 0xfb,
/* 0xe0 : 0x07 */ 0x07,
/* 0xe1 : 0x87 */ 0x87,
/* 0xe2 : 0x47 */ 0x47,
/* 0xe3 : 0xc7 */ 0xc7,
/* 0xe4 : 0x27 */ 0x27,
/* 0xe5 : 0xa7 */ 0xa7,
/* 0xe6 : 0x67 */ 0x67,
/* 0xe7 : 0xe7 */ 0xe7,
/* 0xe8 : 0x17 */ 0x17,
/* 0xe9 : 0x97 */ 0x97,
/* 0xea : 0x57 */ 0x57,
/* 0xeb : 0xd7 */ 0xd7,
/* 0xec : 0x37 */ 0x37,
/* 0xed : 0xb7 */ 0xb7,
/* 0xee : 0x77 */ 0x77,
/* 0xef : 0xf7 */ 0xf7,
/* 0xf0 : 0x0f */ 0x0f,
/* 0xf1 : 0x8f */ 0x8f,
/* 0xf2 : 0x4f */ 0x4f,
/* 0xf3 : 0xcf */ 0xcf,
/* 0xf4 : 0x2f */ 0x2f,
/* 0xf5 : 0xaf */ 0xaf,
/* 0xf6 : 0x6f */ 0x6f,
/* 0xf7 : 0xef */ 0xef,
/* 0xf8 : 0x1f */ 0x1f,
/* 0xf9 : 0x9f */ 0x9f,
/* 0xfa : 0x5f */ 0x5f,
/* 0xfb : 0xdf */ 0xdf,
/* 0xfc : 0x3f */ 0x3f,
/* 0xfd : 0xbf */ 0xbf,
/* 0xfe : 0x7f */ 0x7f,
/* 0xff : 0xff */ 0xff

};
#else	/* IR_INCLUDE_TV_LINK_AND_WB_CODE */

static MS_IR_KeyInfo _KeyReceived;   //temporary solution

#endif	/* IR_INCLUDE_TV_LINK_AND_WB_CODE */

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _MDrv_IR_Timing(void);
#if (IR_MODE_SEL == IR_TYPE_HWRC_MODE)
irqreturn_t _MDrv_IR_RC_ISR(int irq, void *dev_id);
#else
irqreturn_t _MDrv_IR_ISR(int irq, void *dev_id);
#endif
static U8   _MDrv_IR_ParseKey(U8 u8KeyData);
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag);
#if 0
static BOOL _MDrv_IR_GetInputData(U32* rawData);
#endif
unsigned long _MDrv_IR_GetSystemTime(void);

#ifdef	IR_INCLUDE_TV_LINK_AND_WB_CODE
static BOOL _MDrv_IR_GetKeyOfWBCode(U8 *pu8Key, U8 *pu8Flag);
static BOOL _MDrv_IR_GetKeyOfTVLink(U8 *pu8Key, U8 *pu8Flag);
#endif

#if ((IR_TYPE_SEL == IR_TYPE_TOSHIBA) || (IR_TYPE_SEL == IR_TYPE_CUS21SH))
static unsigned long _MDrv_IR_DiffTimeFromNow(unsigned long time)
{
    return (((unsigned long)((jiffies)*(1000/HZ))) - time);
}

void ResetKeyoffTimer(void)
{
    _u32KeyOffTimeOutCount = _MDrv_IR_GetSystemTime();
}

U32 GetKeyoffTimer(void)
{
	return (_MDrv_IR_GetSystemTime() - _u32KeyOffTimeOutCount);
}
#endif

#if (IR_TYPE_SEL == IR_TYPE_TOSHIBA)

#if (IR_MODE_SEL == IR_TYPE_FULLDECODE_MODE)
static U8 _MDrv_IR_CUS22T_ParseKey(U8 u8KeyData)
{
	U8 u8RetVal = 0xFFUL;

	//printk("Linux: %s:%s:%d u8KeyData:%x\n",__FILE__,__FUNCTION__,__LINE__,u8KeyData);

	switch(u8KeyData)
	{
		case CUS22T_IRKEY_TV_RADIO		  : u8RetVal = IRKEY_TV_RADIO;		   break;
		case CUS22T_IRKEY_CHANNEL_LIST	  : u8RetVal = IRKEY_CHANNEL_LIST;	   break;
		case CUS22T_IRKEY_CHANNEL_FAV_LIST: u8RetVal = IRKEY_CHANNEL_FAV_LIST; break;
		case CUS22T_IRKEY_CHANNEL_RETURN  : u8RetVal = IRKEY_CHANNEL_RETURN;   break;
		case CUS22T_IRKEY_CHANNEL_PLUS	  : u8RetVal = IRKEY_CHANNEL_PLUS;	   break;
		case CUS22T_IRKEY_CHANNEL_MINUS   : u8RetVal = IRKEY_CHANNEL_MINUS;    break;

		case CUS22T_IRKEY_AUDIO 		  : u8RetVal = IRKEY_AUDIO; 		   break;
		case CUS22T_IRKEY_VOLUME_PLUS	  : u8RetVal = IRKEY_VOLUME_PLUS;	   break;
		case CUS22T_IRKEY_VOLUME_MINUS	  : u8RetVal = IRKEY_VOLUME_MINUS;	   break;

		case CUS22T_IRKEY_UP			  : u8RetVal = IRKEY_UP;			   break;
		case CUS22T_IRKEY_POWER 		  : u8RetVal = IRKEY_POWER; 		   break;
		case CUS22T_IRKEY_EXIT			  : u8RetVal = IRKEY_EXIT;			   break;
		case CUS22T_IRKEY_MENU			  : u8RetVal = IRKEY_MENU;			   break;
		case CUS22T_IRKEY_DOWN			  : u8RetVal = IRKEY_DOWN;			   break;
		case CUS22T_IRKEY_LEFT			  : u8RetVal = IRKEY_LEFT;			   break;
		case CUS22T_IRKEY_SELECT		  : u8RetVal = IRKEY_SELECT;		   break;
		case CUS22T_IRKEY_RIGHT 		  : u8RetVal = IRKEY_RIGHT; 		   break;

		case CUS22T_IRKEY_NUM_0 		  : u8RetVal = IRKEY_NUM_0; 		   break;
		case CUS22T_IRKEY_NUM_1 		  : u8RetVal = IRKEY_NUM_1; 		   break;
		case CUS22T_IRKEY_NUM_2 		  : u8RetVal = IRKEY_NUM_2; 		   break;
		case CUS22T_IRKEY_NUM_3 		  : u8RetVal = IRKEY_NUM_3; 		   break;
		case CUS22T_IRKEY_NUM_4 		  : u8RetVal = IRKEY_NUM_4; 		   break;
		case CUS22T_IRKEY_NUM_5 		  : u8RetVal = IRKEY_NUM_5; 		   break;
		case CUS22T_IRKEY_NUM_6 		  : u8RetVal = IRKEY_NUM_6; 		   break;
		case CUS22T_IRKEY_NUM_7 		  : u8RetVal = IRKEY_NUM_7; 		   break;
		case CUS22T_IRKEY_NUM_8 		  : u8RetVal = IRKEY_NUM_8; 		   break;
		case CUS22T_IRKEY_NUM_9 		  : u8RetVal = IRKEY_NUM_9; 		   break;

		case CUS22T_IRKEY_MUTE			  : u8RetVal = IRKEY_MUTE;			   break;
		case CUS22T_IRKEY_PAGE_UP		  : u8RetVal = IRKEY_PAGE_UP;		   break;
		//case CUS22T_IRKEY_PAGE_DOWN		: u8RetVal = IRKEY_PAGE_DOWN;		 break;
		case CUS22T_IRKEY_CLOCK 		  : u8RetVal = IRKEY_CLOCK; 		   break;

		case CUS22T_IRKEY_INFO			  : u8RetVal = IRKEY_INFO;			   break;
		case CUS22T_IRKEY_RED			  : u8RetVal = IRKEY_RED;			   break;
		case CUS22T_IRKEY_GREEN 		  : u8RetVal = IRKEY_GREEN; 		   break;
		case CUS22T_IRKEY_YELLOW		  : u8RetVal = IRKEY_YELLOW;		   break;
		case CUS22T_IRKEY_BLUE			  : u8RetVal = IRKEY_BLUE;			   break;
		case CUS22T_IRKEY_MTS			  : u8RetVal = IRKEY_MTS;			   break;
		//case CUS22T_IRKEY_NINE_LATTICE	: u8RetVal = IRKEY_NINE_LATTICE;	 break;
#if defined(DVB_SYSTEM)
		case CUS22T_IRKEY_TTX			  : u8RetVal = IRKEY_TTX;			   break;
#elif defined(ATSC_SYSTEM)
		case CUS22T_IRKEY_CC			  : u8RetVal = IRKEY_CC;			   break;
#endif
		case CUS22T_IRKEY_INPUT_SOURCE	  : u8RetVal = IRKEY_INPUT_SOURCE;	   break;
		//case CUS22T_IRKEY_CRADRD			: u8RetVal = IRKEY_CRADRD;			 break;
	//	  case CUS22T_IRKEY_PICTURE 		: u8RetVal = IRKEY_PICTURE; 		 break;
		case CUS22T_IRKEY_ZOOM			  : u8RetVal = IRKEY_ZOOM;			   break;
		//case CUS22T_IRKEY_DASH			: u8RetVal = IRKEY_DASH;			 break;
		case CUS22T_IRKEY_SLEEP 		  : u8RetVal = IRKEY_SLEEP; 		   break;
		case CUS22T_IRKEY_EPG			  : u8RetVal = IRKEY_EPG;			   break;
		//case CUS22T_IRKEY_PIP 			: u8RetVal = IRKEY_PIP; 			 break;

		//case CUS22T_IRKEY_MIX 			: u8RetVal = IRKEY_MIX; 			 break;
		//case CUS22T_IRKEY_INDEX			: u8RetVal = IRKEY_INDEX;			 break;
		case CUS22T_IRKEY_HOLD			  : u8RetVal = IRKEY_HOLD;			   break;
		//case CUS22T_IRKEY_PREVIOUS		: u8RetVal = IRKEY_PREVIOUS;		 break;
		//case CUS22T_IRKEY_NEXT			: u8RetVal = IRKEY_NEXT;			 break;
		//case CUS22T_IRKEY_BACKWARD		: u8RetVal = IRKEY_BACKWARD;		 break;
		//case CUS22T_IRKEY_FORWARD 		: u8RetVal = IRKEY_FORWARD; 		 break;
		//case CUS22T_IRKEY_PLAY			: u8RetVal = IRKEY_PLAY;			 break;
		//case CUS22T_IRKEY_RECORD			: u8RetVal = IRKEY_RECORD;			 break;
		//case CUS22T_IRKEY_STOP			: u8RetVal = IRKEY_STOP;			 break;
		//case CUS22T_IRKEY_PAUSE			: u8RetVal = IRKEY_PAUSE;			 break;

		case CUS22T_IRKEY_SIZE			  : u8RetVal = IRKEY_SIZE;			   break;
		case CUS22T_IRKEY_REVEAL		  : u8RetVal = IRKEY_REVEAL;		   break;
		//case CUS22T_IRKEY_SUBCODE 		: u8RetVal = IRKEY_SUBCODE; 		 break;

        case CUS22T_40BE_3D             : u8RetVal = IRKEY_3D;                  break;
        case CUS22T_IRKEY_QUICK         : u8RetVal = IRKEY_QUICK;               break;
        case CUS22T_IRKEY_RETURN        : u8RetVal = IRKEY_RETURN;              break;
		//default					 : u8RetVal = IRKEY_DUMMY;			  break;
	}
	return u8RetVal;
}

#elif (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)

static U8 _MDrv_IR_CUS22T_Customer_ParseKey(U32 u32KeyData)
{
    U8 u8RetVal = CUS22T_IRKEY_DUMY;

    u32KeyData &= 0xFFFFFF00UL;

    //printk("Linux: %s:%s:%d u32KeyData:%x",__FILE__,__FUNCTION__,__LINE__,u32KeyData);

    switch(u32KeyData)
    {
        case CUS22T_SW_IRKEY_TV_RADIO        : u8RetVal = IRKEY_TV_RADIO;         break;
        case CUS22T_SW_IRKEY_CHANNEL_LIST    : u8RetVal = IRKEY_CHANNEL_LIST;     break;
        case CUS22T_SW_IRKEY_CHANNEL_FAV_LIST: u8RetVal = IRKEY_CHANNEL_FAV_LIST; break;
        case CUS22T_SW_IRKEY_CHANNEL_RETURN  : u8RetVal = IRKEY_CHANNEL_RETURN;   break;
        case CUS22T_SW_IRKEY_CHANNEL_PLUS    : u8RetVal = IRKEY_CHANNEL_PLUS;     break;
        case CUS22T_SW_IRKEY_CHANNEL_MINUS   : u8RetVal = IRKEY_CHANNEL_MINUS;    break;

        case CUS22T_SW_IRKEY_AUDIO           : u8RetVal = IRKEY_AUDIO;            break;
        case CUS22T_SW_IRKEY_VOLUME_PLUS     : u8RetVal = IRKEY_VOLUME_PLUS;      break;
        case CUS22T_SW_IRKEY_VOLUME_MINUS    : u8RetVal = IRKEY_VOLUME_MINUS;     break;

        case CUS22T_SW_IRKEY_UP              : u8RetVal = IRKEY_UP;               break;
        case CUS22T_SW_IRKEY_POWER           : u8RetVal = IRKEY_POWER;            break;
        case CUS22T_SW_IRKEY_EXIT            : u8RetVal = IRKEY_EXIT;             break;
        case CUS22T_SW_IRKEY_MENU            : u8RetVal = IRKEY_MENU;             break;
        case CUS22T_SW_IRKEY_DOWN            : u8RetVal = IRKEY_DOWN;             break;
        case CUS22T_SW_IRKEY_LEFT            : u8RetVal = IRKEY_LEFT;             break;
        case CUS22T_SW_IRKEY_SELECT          : u8RetVal = IRKEY_SELECT;           break;
        case CUS22T_SW_IRKEY_RIGHT           : u8RetVal = IRKEY_RIGHT;            break;

        case CUS22T_SW_IRKEY_NUM_0           : u8RetVal = IRKEY_NUM_0;            break;
        case CUS22T_SW_IRKEY_NUM_1           : u8RetVal = IRKEY_NUM_1;            break;
        case CUS22T_SW_IRKEY_NUM_2           : u8RetVal = IRKEY_NUM_2;            break;
        case CUS22T_SW_IRKEY_NUM_3           : u8RetVal = IRKEY_NUM_3;            break;
        case CUS22T_SW_IRKEY_NUM_4           : u8RetVal = IRKEY_NUM_4;            break;
        case CUS22T_SW_IRKEY_NUM_5           : u8RetVal = IRKEY_NUM_5;            break;
        case CUS22T_SW_IRKEY_NUM_6           : u8RetVal = IRKEY_NUM_6;            break;
        case CUS22T_SW_IRKEY_NUM_7           : u8RetVal = IRKEY_NUM_7;            break;
        case CUS22T_SW_IRKEY_NUM_8           : u8RetVal = IRKEY_NUM_8;            break;
        case CUS22T_SW_IRKEY_NUM_9           : u8RetVal = IRKEY_NUM_9;            break;

        case CUS22T_SW_IRKEY_MUTE            : u8RetVal = IRKEY_MUTE;             break;
        case CUS22T_SW_IRKEY_PAGE_UP         : u8RetVal = IRKEY_PAGE_UP;          break;
        //case CUS22T_SW_IRKEY_PAGE_DOWN       : u8RetVal = IRKEY_PAGE_DOWN;        break;
        case CUS22T_SW_IRKEY_CLOCK           : u8RetVal = IRKEY_CLOCK;            break;

        case CUS22T_SW_IRKEY_INFO            : u8RetVal = IRKEY_INFO;             break;
        case CUS22T_SW_IRKEY_RED             : u8RetVal = IRKEY_RED;              break;
        case CUS22T_SW_IRKEY_GREEN           : u8RetVal = IRKEY_GREEN;            break;
        case CUS22T_SW_IRKEY_YELLOW          : u8RetVal = IRKEY_YELLOW;           break;
        case CUS22T_SW_IRKEY_BLUE            : u8RetVal = IRKEY_BLUE;             break;
        case CUS22T_SW_IRKEY_MTS             : u8RetVal = IRKEY_MTS;              break;
        //case CUS22T_SW_IRKEY_NINE_LATTICE    : u8RetVal = IRKEY_NINE_LATTICE;     break;

        case CUS22T_SW_IRKEY_INPUT_SOURCE    : u8RetVal = IRKEY_INPUT_SOURCE;     break;
        //case CUS22T_SW_IRKEY_CRADRD          : u8RetVal = IRKEY_CRADRD;           break;
        case CUS22T_SW_IRKEY_PICTURE         : u8RetVal = IRKEY_PICTURE;          break;
        case CUS22T_SW_IRKEY_ZOOM            : u8RetVal = IRKEY_ZOOM;             break;
        //case CUS22T_SW_IRKEY_DASH            : u8RetVal = IRKEY_DASH;             break;
        case CUS22T_SW_IRKEY_SLEEP           : u8RetVal = IRKEY_SLEEP;            break;
        case CUS22T_SW_IRKEY_EPG             : u8RetVal = IRKEY_EPG;              break;
        //case CUS22T_SW_IRKEY_PIP             : u8RetVal = IRKEY_PIP;              break;

        //case CUS22T_SW_IRKEY_MIX             : u8RetVal = IRKEY_MIX;              break;
        //case CUS22T_SW_IRKEY_INDEX           : u8RetVal = IRKEY_INDEX;            break;
        //case CUS22T_SW_IRKEY_HOLD            : u8RetVal = IRKEY_HOLD;             break;
        //case CUS22T_SW_IRKEY_PREVIOUS        : u8RetVal = IRKEY_PREVIOUS;         break;
        //case CUS22T_SW_IRKEY_NEXT            : u8RetVal = IRKEY_NEXT;             break;
        //case CUS22T_SW_IRKEY_BACKWARD        : u8RetVal = IRKEY_BACKWARD;         break;
        //case CUS22T_SW_IRKEY_FORWARD         : u8RetVal = IRKEY_FORWARD;          break;
        //case CUS22T_SW_IRKEY_PLAY            : u8RetVal = IRKEY_PLAY;             break;
        //case CUS22T_SW_IRKEY_RECORD          : u8RetVal = IRKEY_RECORD;           break;
        //case CUS22T_SW_IRKEY_STOP            : u8RetVal = IRKEY_STOP;             break;
        //case CUS22T_SW_IRKEY_PAUSE           : u8RetVal = IRKEY_PAUSE;            break;

        case CUS22T_SW_IRKEY_TEXT               : u8RetVal = IRKEY_TTX;                 break;
        case CUS22T_SW_IRKEY_MEDIA_PLAYER       : u8RetVal = IRKEY_MEDIA_PLAYER;        break;
        case CUS22T_SW_IRKEY_SUBTITLE           : u8RetVal = IRKEY_SUBCODE;             break;

        case CUS22T_SW_IRKEY_REW                : u8RetVal = IRKEY_BACKWARD;            break;
        case CUS22T_SW_IRKEY_PLAY               : u8RetVal = IRKEY_PLAY;                break;
        case CUS22T_SW_IRKEY_FF                 : u8RetVal = IRKEY_FORWARD;             break;
        case CUS22T_SW_IRKEY_SKIP_MINUS         : u8RetVal = IRKEY_PREVIOUS;            break;
        case CUS22T_SW_IRKEY_STOP               : u8RetVal = IRKEY_STOP;                break;
        case CUS22T_SW_IRKEY_PAUSE              : u8RetVal = IRKEY_PAUSE;               break;
        case CUS22T_SW_IRKEY_SKIP_PLUS          : u8RetVal = IRKEY_NEXT;                break;

        case CUS22T_SW_IRKEY_SIZE            : u8RetVal = IRKEY_SIZE;             break;
        //case CUS22T_SW_IRKEY_REVEAL          : u8RetVal = IRKEY_REVEAL;           break;
        //case CUS22T_SW_IRKEY_SUBCODE         : u8RetVal = IRKEY_SUBCODE;          break;

        case CUS22T_SW_IRKEY_3D                 : u8RetVal = IRKEY_3D;                  break;
        case CUS22T_SW_IRKEY_QUICK              : u8RetVal = IRKEY_QUICK;               break;
        case CUS22T_SW_IRKEY_RETURN             : u8RetVal = IRKEY_RETURN;              break;
        case CUS22T_SW_IRKEY_TV_INPUT           : u8RetVal = IRKEY_TV_INPUT;            break;


        case CUS22T_SW_F_40BF_PIP_ON_OFF        : u8RetVal = F_40BF_PIP_ON_OFF;         break;
        case CUS22T_SW_F_40BF_TUNE_DOWN_SEARCH  : u8RetVal = F_40BF_TUNE_DOWN_SEARCH;   break;
        case CUS22T_SW_F_40BF_TUNE_UP_SEARCH    : u8RetVal = F_40BF_TUNE_UP_SEARCH;     break;
        case CUS22T_SW_F_40BF_MEM               : u8RetVal = F_40BF_MEM;                break;
        case CUS22T_SW_F_40BF_COLOR_SYS_CHECK   : u8RetVal = F_40BF_COLOR_SYS_CHECK;    break;
        case CUS22T_SW_F_40BF_SOUND_SYS_CHECK   : u8RetVal = F_40BF_SOUND_SYS_CHECK;    break;
        case CUS22T_SW_F_40BF_E_D_MODE          : u8RetVal = F_40BF_E_D_MODE;           break;


        default                             : u8RetVal = IRKEY_DUMY;            break;
    }
    printk("==>%x\n",u8RetVal);
    return u8RetVal;
}

static U8 _MDrv_IR_CUS22T_Factory_ParseKey(U32 u32KeyData)
{
    U8 u8RetVal = CUS22T_IRKEY_DUMY;

    printk("Linux: %s:%s:%d u32KeyData:%x\n",__FILE__,__FUNCTION__,__LINE__,u32KeyData);

    if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40E2_COMMAND)
    {
        u32KeyData &= 0xFFFFFF00UL;

        switch(u32KeyData)
        {
            case CUS22T_SW_F_40E2_CONTRAST        : u8RetVal = F_40E2_CONTRAST	;   break;
            case CUS22T_SW_F_40E2_BRIGHT          : u8RetVal = F_40E2_BRIGHT	;   break;
            case CUS22T_SW_F_40E2_COLOR           : u8RetVal = F_40E2_COLOR	    ;   break;
            case CUS22T_SW_F_40E2_TINT            : u8RetVal = F_40E2_TINT		;   break;
            case CUS22T_SW_F_40E2_SHARPNESS       : u8RetVal = F_40E2_SHARPNESS ;   break;
            case CUS22T_SW_F_40E2_VOLUME          : u8RetVal = F_40E2_VOLUME	;   break;
            case CUS22T_SW_F_40E2_BASS            : u8RetVal = F_40E2_BASS		;   break;
            case CUS22T_SW_F_40E2_TREBLE          : u8RetVal = F_40E2_TREBLE	;   break;
            case CUS22T_SW_F_40E2_BALANCE         : u8RetVal = F_40E2_BALANCE	;   break;
            case CUS22T_SW_F_40E2_RCUT            : u8RetVal = F_40E2_RCUT		;   break;
            case CUS22T_SW_F_40E2_GCUT            : u8RetVal = F_40E2_GCUT		;   break;
            case CUS22T_SW_F_40E2_BCUT            : u8RetVal = F_40E2_BCUT		;   break;
            case CUS22T_SW_F_40E2_RDRV            : u8RetVal = F_40E2_RDRV		;   break;
            case CUS22T_SW_F_40E2_BDRV            : u8RetVal = F_40E2_BDRV		;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40E3_COMMAND)
    {
        u32KeyData &= 0xFFFFFF00UL;

        switch(u32KeyData)
        {
            case CUS22T_SW_F_40E3_RCUT            : u8RetVal = F_40E3_RCUT		;   break;
            case CUS22T_SW_F_40E3_GCUT            : u8RetVal = F_40E3_GCUT		;   break;
            case CUS22T_SW_F_40E3_BCUT            : u8RetVal = F_40E3_BCUT		;   break;
            case CUS22T_SW_F_40E3_RDRV_GDRV       : u8RetVal = F_40E3_RDRV_GDRV ;   break;
            case CUS22T_SW_F_40E3_BDRV            : u8RetVal = F_40E3_BDRV      ;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40EA_COMMAND)
    {
        u32KeyData &= 0xFFFFFF00;

        switch(u32KeyData)
        {
            case CUS22T_SW_F_40EA_INTERNAL_PATTERN_OFF  : u8RetVal = F_40EA_INTERNAL_PATTERN_OFF ;   break;
            case CUS22T_SW_F_40EA_RED_RASTER            : u8RetVal = F_40EA_RED_RASTER           ;   break;
            case CUS22T_SW_F_40EA_GREEN_RASTER          : u8RetVal = F_40EA_GREEN_RASTER         ;   break;
            case CUS22T_SW_F_40EA_BLUE_RASTER           : u8RetVal = F_40EA_BLUE_RASTER          ;   break;
            case CUS22T_SW_F_40EA_BLACK_RASTER          : u8RetVal = F_40EA_BLACK_RASTER         ;   break;
            case CUS22T_SW_F_40EA_WHITE_RASTER          : u8RetVal = F_40EA_WHITE_RASTER         ;   break;
            case CUS22T_SW_F_40EA_AGING_MODE_1          : u8RetVal = F_40EA_AGING_MODE_1         ;   break;
            case CUS22T_SW_F_40EA_AGINE_MODE_2          : u8RetVal = F_40EA_AGINE_MODE_2         ;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40EB_COMMAND)
    {

        u32KeyData &= 0xFFFFFF00UL;

        switch(u32KeyData)
        {
            case CUS22T_SW_F_40EB_DIRECT_CH                 : u8RetVal = F_40EB_DIRECT_CH               ;   break;
            case CUS22T_SW_F_40EB_DIRECT_RF                 : u8RetVal = F_40EB_DIRECT_RF		        ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT1         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT1       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT2         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT2       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT3         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT3       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT5         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT5       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT6         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT6       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT7         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT7       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT8         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT8       ;   break;
            case CUS22T_SW_F_40EB_DIRECT_VIDEO_EXT9         : u8RetVal = F_40EB_DIRECT_VIDEO_EXT9       ;   break;
            case CUS22T_SW_F_40EB_SIDE_SHARED_AUDEO_EXT2    : u8RetVal = F_40EB_SIDE_SHARED_AUDEO_EXT2  ;   break;
            case CUS22T_SW_F_40EB_SIDE_SHARED_AUDIO_EXT3    : u8RetVal = F_40EB_SIDE_SHARED_AUDIO_EXT3  ;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40EE_COMMAND)
    {
        u32KeyData &= 0xFFFFFF00UL;

        switch(u32KeyData)
        {
            case CUS22T_SW_F_40EE_ALL_CH                     : u8RetVal = F_40EE_ALL_CH			         ;   break;
            case CUS22T_SW_F_40EE_M_MODE                     : u8RetVal = F_40EE_M_MODE			         ;   break;
            case CUS22T_SW_F_40EE_INITIALIZATION             : u8RetVal = F_40EE_INITIALIZATION	         ;   break;
            case CUS22T_SW_F_40EE_PICTURE_MODE_DYNAMIC       : u8RetVal = F_40EE_PICTURE_MODE_DYNAMIC	 ;   break;
            case CUS22T_SW_F_40EE_M_OSD_OFF                  : u8RetVal = F_40EE_M_OSD_OFF		         ;   break;
            case CUS22T_SW_F_40EE_TJP_FACTORY_SIGNAL_SETTING : u8RetVal = F_40EE_TJP_FACTORY_SIGNAL_SETTING   ;   break;
            case CUS22T_SW_F_40EE_3D_ON_SIDE_BY_SIDE         : u8RetVal = F_40EE_3D_ON_SIDE_BY_SIDE	     ;   break;
            case CUS22T_SW_F_40EE_DATA_1_UP                  : u8RetVal = F_40EE_DATA_1_UP		         ;   break;
            case CUS22T_SW_F_40EE_DATA_1_DN                  : u8RetVal = F_40EE_DATA_1_DN		         ;   break;
            case CUS22T_SW_F_40EE_DATA_4_UP                  : u8RetVal = F_40EE_DATA_4_UP		         ;   break;
            case CUS22T_SW_F_40EE_DATA_4_DN                  : u8RetVal = F_40EE_DATA_4_DN		         ;   break;
            case CUS22T_SW_F_40EE_2D_3D_CONVERSION           : u8RetVal = F_40EE_2D_3D_CONVERSION	     ;   break;
            case CUS22T_SW_F_40EE_AUDIO_BALANCE_TOGGLE       : u8RetVal = F_40EE_AUDIO_BALANCE_TOGGLE	 ;   break;
            case CUS22T_SW_F_40EE_LED_CHECK                  : u8RetVal = F_40EE_LED_CHECK		         ;   break;
            case CUS22T_SW_F_40EE_4_3_STRETCH_TOGGLE         : u8RetVal = F_40EE_4_3_STRETCH_TOGGLE	     ;   break;
            case CUS22T_SW_F_40EE_DNR_SELECT                 : u8RetVal = F_40EE_DNR_SELECT		         ;   break;
            case CUS22T_SW_F_40EE_AUTO_BRIGHTNESS_SENSOR     : u8RetVal = F_40EE_AUTO_BRIGHTNESS_SENSOR   ;   break;
            case CUS22T_SW_F_40EE_AUTO_BRIGHTNESS_SENSOR_DETECT_VALUE_DISPLAY : u8RetVal = F_40EE_AUTO_BRIGHTNESS_SENSOR_DETECT_VALUE_DISPLAY   ;   break;
            case CUS22T_SW_F_40EE_EDID_DATA_DOWNLOAD         : u8RetVal = F_40EE_EDID_DATA_DOWNLOAD	     ;   break;
            case CUS22T_SW_F_40EE_STABLE_SOUND_ON_OFF        : u8RetVal = F_40EE_STABLE_SOUND_ON_OFF	 ;   break;
            case CUS22T_SW_F_40EE_CLEAR_SCAN_100_ON_OFF      : u8RetVal = F_40EE_CLEAR_SCAN_100_ON_OFF   ;   break;
            case CUS22T_SW_F_40EE_CLEAR_SCAN_200_PRO_ON_OFF  : u8RetVal = F_40EE_CLEAR_SCAN_200_PRO_ON_OFF   ;   break;
            case CUS22T_SW_F_40EE_MEDIA_PLAYER_MOVIE         : u8RetVal = F_40EE_MEDIA_PLAYER_MOVIE	     ;   break;
            case CUS22T_SW_F_40EE_BASE_BOOST_ON_OFF          : u8RetVal = F_40EE_BASE_BOOST_ON_OFF	     ;   break;
            case CUS22T_SW_F_40EE_BLACK_WHITE_LEVEL          : u8RetVal = F_40EE_BLACK_WHITE_LEVEL	     ;   break;
            case CUS22T_SW_F_40EE_NEWZEALAND_PRESET          : u8RetVal = F_40EE_NEWZEALAND_PRESET	     ;   break;
            case CUS22T_SW_F_40EE_DLNA_TEST                  : u8RetVal = F_40EE_DLNA_TEST		         ;   break;
            case CUS22T_SW_F_40EE_COLOR_TEMPERATURE          : u8RetVal = F_40EE_COLOR_TEMPERATURE       ;   break;
            case CUS22T_SW_F_40EE_BLACK_LIGHT_CHECK          : u8RetVal = F_40EE_BLACK_LIGHT_CHECK       ;   break;
            case CUS22T_SW_F_40EE_DEFAULT_MAC_IP             : u8RetVal = F_40EE_DEFAULT_MAC_IP	         ;   break;
            case CUS22T_SW_F_40EE_MAC_ADDRESS_DELETE         : u8RetVal = F_40EE_MAC_ADDRESS_DELETE	     ;   break;
            case CUS22T_SW_F_40EE_ADC_ADJUSTMENT             : u8RetVal = F_40EE_ADC_ADJUSTMENT	         ;   break;
            case CUS22T_SW_F_40EE_DUAL_1                     : u8RetVal = F_40EE_DUAL_1			         ;   break;
            case CUS22T_SW_F_40EE_MONO_MODE                  : u8RetVal = F_40EE_MONO_MODE		         ;   break;
            case CUS22T_SW_F_40EE_STEREO_MODE                : u8RetVal = F_40EE_STEREO_MODE		     ;   break;
            case CUS22T_SW_F_40EE_DUAL_2                     : u8RetVal = F_40EE_DUAL_2		             ;   break;
            case CUS22T_SW_F_40EE_HDMI_2_AUDIO               : u8RetVal = F_40EE_HDMI_2_AUDIO            ;   break;
            case CUS22T_SW_F_40EE_HDMI_2_AUDIO_SELECT        : u8RetVal = F_40EE_HDMI_2_AUDIO_SELECT     ;   break;
            case CUS22T_SW_F_40EE_S_MODE                     : u8RetVal = F_40EE_S_MODE                  ;   break;
            case CUS22T_SW_F_40EE_SIDE_PANEL_SELECT          : u8RetVal = F_40EE_SIDE_PANEL_SELECT       ;   break;
            case CUS22T_SW_F_40EE_PICTURE_SIZE_POSITION      : u8RetVal = F_40EE_PICTURE_SIZE_POSITION   ;   break;
            case CUS22T_SW_F_40EE_MPEG_NR                    : u8RetVal = F_40EE_MPEG_NR                 ;   break;
            case CUS22T_SW_F_40EE_SIGNAL_BOOSTER_ON_OFF      : u8RetVal = F_40EE_SIGNAL_BOOSTER_ON_OFF   ;   break;
            case CUS22T_SW_F_40EE_PICTURE_RESET              : u8RetVal = F_40EE_PICTURE_RESET           ;   break;
            case CUS22T_SW_F_40EE_DIGITAL_AUDIO_OUT          : u8RetVal = F_40EE_DIGITAL_AUDIO_OUT       ;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40F0_COMMAND)
    {
        switch(u32KeyData)
        {
            case CUS22T_SW_F_40F0_EEPROM_MEM_DATE_UPDATE    : u8RetVal = F_40F0_EEPROM_MEM_DATE_UPDATE  ;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40F1_COMMAND)
    {
        switch(u32KeyData)
        {
            case CUS22T_SW_F_40F1_EEPROM_MEM_DATE_UPDATE    : u8RetVal = F_40F1_EEPROM_MEM_DATE_UPDATE  ;   break;
        }
    }
    else if((u32KeyData & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40F2_COMMAND)
    {
        switch(u32KeyData)
        {
            case CUS22T_SW_F_40F2_EEPROM_MEM_DATE_UPDATE    : u8RetVal = F_40F2_EEPROM_MEM_DATE_UPDATE  ;   break;
        }
    }
    else if((u32KeyData & 0xFF00FFFFUL) == CUS22T_SW_IRKEY_CF00_COMMAND)
    {
        switch(u32KeyData)
        {
            case CUS22T_SW_F_CFXX_WHITE_WINDOW_ON   : u8RetVal = F_CFXX_WHITE_WINDOW_ON ;   break;
        }
    }
    else if((u32KeyData & 0xFF000000UL) == CUS22T_SW_IRKEY_D000_COMMAND)
    {
        switch(u32KeyData)
        {
            case CUS22T_SW_F_D0XX_WHITE_WINDOW_OFF  : u8RetVal = F_D0XX_WHITE_WINDOW_ON ;   break;
        }
    }

    return u8RetVal;
}

static U8 _MDrv_IR_CUS22T_ParseKey(U32 u32KeyData)
{
    U8 u8RetVal = CUS22T_IRKEY_DUMY;
	U32 u32key = CUS22T_IRKEY_DUMY;

    u32key = (((u32KeyData & 0x000000FFUL) << 24)
				| ((u32KeyData & 0x0000FF00UL) << 8)
				| ((u32KeyData & 0x00FF0000UL) >> 8)
				| ((u32KeyData & 0xFF000000UL) >> 24)
				);

	//printk("Linux: %s:%s:%d u32key:%x\n",__FILE__,__FUNCTION__,__LINE__,u32key);

    if(((u32key & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40BE_COMMAND)
        || ((u32key & 0xFFFF0000UL) == CUS22T_SW_IRKEY_40BF_COMMAND)
        )
    {
        u8RetVal = _MDrv_IR_CUS22T_Customer_ParseKey(u32key);
    }
    else
    {
        u8RetVal = _MDrv_IR_CUS22T_Factory_ParseKey(u32key);
    }

    return u8RetVal;
}

static BOOL _MDrv_IR_CUS22T_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
	if(_bIrDetect)
	{
	    _bIrDetect = FALSE;

	    if(_bIrRepeat)
	    {
	        if(_u8IrRepeatSkipCnt > 0)
	        {
	            _u8IrRepeatSkipCnt--;
	            _bIrRepeat = FALSE;
	            _bRxInProcess = FALSE;
	            ResetKeyoffTimer();

	            return FALSE;
	        }
	        else
	        {
	            *pu8Key = _MDrv_IR_CUS22T_ParseKey(_u32IRKeyMap);//IS_SW_FACTORY_CODE()? IR_FACTORY_KEY: IR_CODE;
	            *pu8Flag = _bIrRepeat;
	        }
	    }
	    else
	    {
            *pu8Key = _MDrv_IR_CUS22T_ParseKey(_u32IRKeyMap);//IS_SW_FACTORY_CODE()? IR_FACTORY_KEY: IR_CODE;
            *pu8Flag = _bIrRepeat;
            _u8IrRepeatSkipCnt = IR_REPEAT_SKIP_COUNT;
	    }

	    _bIrRepeat = FALSE;
	    _bRxInProcess = FALSE;
	    ResetKeyoffTimer();

	    return TRUE;
	}
        return FALSE;
}
#endif

#endif

//-------------------------------------------------------------------------------------------------
/// Translate from IR key to internal key.
/// @param  u8KeyData  \b IN: IR key value.
/// @return translated internal IR key.
//-------------------------------------------------------------------------------------------------
#if (IR_TYPE_SEL == IR_TYPE_CUS21SH)
static U8 _MDrv_IR_CUS21SH_ParseKey(U16 u16KeyData)
{
    U8 u8RetVal;
    //printk("Linux: %s:%s:%d u8KeyData:%x\n",__FILE__,__FUNCTION__,__LINE__,u16KeyData);

    switch(u16KeyData)
    {
        //case SH_IRKEY_TV_RADIO        : u8RetVal = IRKEY_TV_RADIO;         break;
        case SH_IRKEY_CHANNEL_LIST    : u8RetVal = IRKEY_CHANNEL_LIST;     break;
        case SH_IRKEY_CHANNEL_FAV_LIST: u8RetVal = IRKEY_CHANNEL_FAV_LIST; break;
        case SH_IRKEY_CHANNEL_RETURN  : u8RetVal = IRKEY_CHANNEL_RETURN;   break;
        case SH_IRKEY_CHANNEL_PLUS    : u8RetVal = IRKEY_CHANNEL_PLUS;     break;
        case SH_IRKEY_CHANNEL_MINUS   : u8RetVal = IRKEY_CHANNEL_MINUS;    break;

        case SH_IRKEY_AUDIO           : u8RetVal = IRKEY_AUDIO;            break;
        case SH_IRKEY_VOLUME_PLUS     : u8RetVal = IRKEY_VOLUME_PLUS;      break;
        case SH_IRKEY_VOLUME_MINUS    : u8RetVal = IRKEY_VOLUME_MINUS;     break;

        case SH_IRKEY_UP              : u8RetVal = IRKEY_UP;               break;
        case SH_IRKEY_POWER           : u8RetVal = IRKEY_POWER;            break;
        case SH_IRKEY_EXIT            : u8RetVal = IRKEY_EXIT;             break;
        case SH_IRKEY_MENU            : u8RetVal = IRKEY_MENU;             break;
        case SH_IRKEY_DOWN            : u8RetVal = IRKEY_DOWN;             break;
        case SH_IRKEY_LEFT            : u8RetVal = IRKEY_LEFT;             break;
        case SH_IRKEY_SELECT          : u8RetVal = IRKEY_SELECT;           break;
        case SH_IRKEY_RIGHT           : u8RetVal = IRKEY_RIGHT;            break;

        case SH_IRKEY_NUM_0           : u8RetVal = IRKEY_NUM_0;            break;
        case SH_IRKEY_NUM_1           : u8RetVal = IRKEY_NUM_1;            break;
        case SH_IRKEY_NUM_2           : u8RetVal = IRKEY_NUM_2;            break;
        case SH_IRKEY_NUM_3           : u8RetVal = IRKEY_NUM_3;            break;
        case SH_IRKEY_NUM_4           : u8RetVal = IRKEY_NUM_4;            break;
        case SH_IRKEY_NUM_5           : u8RetVal = IRKEY_NUM_5;            break;
        case SH_IRKEY_NUM_6           : u8RetVal = IRKEY_NUM_6;            break;
        case SH_IRKEY_NUM_7           : u8RetVal = IRKEY_NUM_7;            break;
        case SH_IRKEY_NUM_8           : u8RetVal = IRKEY_NUM_8;            break;
        case SH_IRKEY_NUM_9           : u8RetVal = IRKEY_NUM_9;            break;

        case SH_IRKEY_MUTE            : u8RetVal = IRKEY_MUTE;             break;
        case SH_IRKEY_PAGE_UP         : u8RetVal = IRKEY_PAGE_UP;          break;
        //case SH_IRKEY_PAGE_DOWN       : u8RetVal = IRKEY_PAGE_DOWN;        break;
        case SH_IRKEY_CLOCK           : u8RetVal = IRKEY_CLOCK;            break;

        case SH_IRKEY_INFO            : u8RetVal = IRKEY_INFO;             break;
        case SH_IRKEY_RED             : u8RetVal = IRKEY_RED;              break;
        case SH_IRKEY_GREEN           : u8RetVal = IRKEY_GREEN;            break;
        case SH_IRKEY_YELLOW          : u8RetVal = IRKEY_YELLOW;           break;
        case SH_IRKEY_BLUE            : u8RetVal = IRKEY_BLUE;             break;
        case SH_IRKEY_MTS             : u8RetVal = IRKEY_MTS;              break;
        //case SH_IRKEY_NINE_LATTICE    : u8RetVal = IRKEY_NINE_LATTICE;     break;
#if defined(DVB_SYSTEM)
        case SH_IRKEY_TTX             : u8RetVal = IRKEY_TTX;              break;
#elif defined(ATSC_SYSTEM)
        case SH_IRKEY_CC              : u8RetVal = IRKEY_CC;               break;
#endif
        case SH_IRKEY_INPUT_SOURCE    : u8RetVal = IRKEY_INPUT_SOURCE;     break;
        //case SH_IRKEY_CRADRD          : u8RetVal = IRKEY_CRADRD;           break;
    //    case SH_IRKEY_PICTURE         : u8RetVal = IRKEY_PICTURE;          break;
        case SH_IRKEY_ZOOM            : u8RetVal = IRKEY_ZOOM;             break;
        //case SH_IRKEY_DASH            : u8RetVal = IRKEY_DASH;             break;
        case SH_IRKEY_SLEEP           : u8RetVal = IRKEY_SLEEP;            break;
        case SH_IRKEY_EPG             : u8RetVal = IRKEY_EPG;              break;
        //case SH_IRKEY_PIP             : u8RetVal = IRKEY_PIP;              break;

      	//case SH_IRKEY_MIX             : u8RetVal = IRKEY_MIX;              break;
        //case SH_IRKEY_INDEX           : u8RetVal = IRKEY_INDEX;            break;
        case SH_IRKEY_HOLD            : u8RetVal = IRKEY_HOLD;             break;
        //case SH_IRKEY_PREVIOUS        : u8RetVal = IRKEY_PREVIOUS;         break;
        //case SH_IRKEY_NEXT            : u8RetVal = IRKEY_NEXT;             break;
        //case SH_IRKEY_BACKWARD        : u8RetVal = IRKEY_BACKWARD;         break;
        //case SH_IRKEY_FORWARD         : u8RetVal = IRKEY_FORWARD;          break;
        //case SH_IRKEY_PLAY            : u8RetVal = IRKEY_PLAY;             break;
        //case SH_IRKEY_RECORD          : u8RetVal = IRKEY_RECORD;           break;
        //case SH_IRKEY_STOP            : u8RetVal = IRKEY_STOP;             break;
        //case SH_IRKEY_PAUSE           : u8RetVal = IRKEY_PAUSE;            break;

        case SH_IRKEY_SIZE            : u8RetVal = IRKEY_SIZE;             break;
        case SH_IRKEY_REVEAL          : u8RetVal = IRKEY_REVEAL;           break;
        //case SH_IRKEY_SUBCODE         : u8RetVal = IRKEY_SUBCODE;          break;

        //default                    : u8RetVal = IRKEY_DUMMY;            break;
    }
    return u8RetVal;
}

BOOL _MDrv_SH_IR_CheckKey(void)
{
    U8 j;
    U8 DataPlusExDat0 = 0;
    U8 DataPlusExDat1 = 0;
    U8 u8TableIdx = 0;
    BOOL DataMatched = FALSE;
    BOOL SystemBitOK = FALSE;
    BOOL DetectBitOK = FALSE;




    CurrentTime = _MDrv_IR_GetSystemTime();

    // 1
    if(_u32IRCount == 15)
    {
        // 1->a
        _bExpectedTCome = TRUE;
        _u16IRData[keyDatArrayWTIdx]=_u16IRKeyMap;
        keyDatArrayWTIdx++;
        keyDatArrayWTIdx%=IR_SWDECODE_MODE_BUF_LEN;
    }
    else
    {
        // 1->b
        _bExpectedTCome = FALSE;
        if(_u32IRCount >= 11)
        {
            // 1->b->b->6
            ResetKeyoffTimer();
        }
        // 1->b->a->7
        RxInProcess = FALSE;
        SetToStandbyMode = TRUE;

        _u32IRCount = 0;
        _u16IRKeyMap = 0;

        return FALSE;
    }

    if(ReceivingMode && _bExpectedTCome)
    {
        if(keyDatArrayRDIdx != keyDatArrayWTIdx)
        {
            _u16BufferCurrent=_u16IRData[keyDatArrayRDIdx];

            //printk("IR: %s:%d %x,%x\n",__FUNCTION__,__LINE__,_u16BufferCurrent,keyDatArray[keyDatArrayRDIdx]);
            keyDatArrayRDIdx++;
            keyDatArrayRDIdx%=IR_SWDECODE_MODE_BUF_LEN;

            if(((_u16BufferCurrent&0x1FUL)==0x01UL)||((_u16BufferCurrent&0x1FUL)==0x11UL)||((_u16BufferCurrent&0x1FUL)==0x1EUL))
            {
                SystemBitOK=TRUE;
            }
            else
            {
                 FirstRXFlag=FALSE;
                 _u16BufferPrev=0;
                 _u16BufferCurrent=0;
                 ResetKeyoffTimer();
                 return FALSE;
            }
            if(SystemBitOK)
            {
                SystemBitOK = FALSE;
                if((((_u16BufferCurrent>>13)&0x03UL)==0x01UL)||(((_u16BufferCurrent>>13)&0x03UL)==0x02UL))
                {
                    DetectBitOK=TRUE;
                }
                else
                {
                    FirstRXFlag=FALSE;
                    _u16BufferPrev=0;
                    _u16BufferCurrent=0;
                    ResetKeyoffTimer();
                    return FALSE;
                }
            }
            if(DetectBitOK)
            {
                DetectBitOK=FALSE;
                if(FirstRXFlag==TRUE)
                {
                    if(1)//(_u16BufferCurrent & 0x4000) == 0x0400)
                    {
                        SecondRXFlag=TRUE;
                    }
                    else
                    {
                        FirstRXFlag = FALSE;
                        SecondRXFlag = FALSE;
                        _u16BufferCurrent = 0;
                        _u16BufferPrev = 0;
                    }
                }
                else
                {
                    if((_u16BufferCurrent & 0x4000UL) == 0x0000UL)
                    {
                        FirstRXFlag = TRUE;
                        _u16BufferPrev = _u16BufferCurrent;
                    }
                    _u16BufferCurrent=0;
                    ResetKeyoffTimer();
                }
            }
            if(SecondRXFlag)
            {
                SecondRXFlag=FALSE;

                DataPlusExDat0=(U8)((_u16BufferCurrent&0x1FE0UL)>>5);
                DataPlusExDat1=(U8)((_u16BufferPrev&0x1FE0UL)>>5);
                //printk("## Data0=0x%x,Data1=0x%x,Cur=0x%x,Prev=0x%x(%d,%d)\n",DataPlusExDat0,DataPlusExDat1,_u16BufferCurrent,_u16BufferPrev,keyDatArrayRDIdx,keyDatArrayWTIdx);
                if((U8)DataPlusExDat0==(U8)(~DataPlusExDat1))
                {
                    DataMatched=TRUE;
                }
                if(DataMatched)
                {
                    g_bIrDetect=TRUE;

                    FirstRXFlag=FALSE;

                    //DebugIRCode = _u16BufferPrev;
                    u8TableIdx = _MDrv_IR_CUS21SH_ParseKey(_u16BufferPrev);

                    if(LastKeyData==u8TableIdx)
                    {
                        IsRepeatData=1;
                    }
                    else
                    {
                        IsRepeatData=0;
                    }
                    g_u8Key=u8TableIdx;
                    g_u8Flag=IsRepeatData;
                    LastKeyData=u8TableIdx;
                    _u16BufferCurrent=0;
                    _u16BufferPrev=0;

                    //printf("## Repeat=0x%bx, index=0x%bx,key=0x%x",IsRepeatData,u8Key,IR_CUS21SH_RAWDATA_TABLE[u8TableIdx]);
                    //printf("\n");
                    #if 0//SHA Standby Using Codes
                    if((gStandbyInto==TRUE)&&(g_u8Key!=IRKEY_POWER))
                    {
                        return MSRET_ERROR;
                    }
                    #endif
                    //printk("IR: %s:%d %x,%x\n",__FUNCTION__,__LINE__,g_u8Key,IsRepeatData);
                    return TRUE;
                }
                else
                {
                    _u16BufferPrev=_u16BufferCurrent;
                    _u16BufferCurrent=0;
                    ResetKeyoffTimer();
                }
            }
        }
        else
        {

            if((CurrentTime-_u32KeyOffTimeOutCount)>100)
            {
                //printk("IR: %s:%d (%d ,%d)\n",__FUNCTION__,__LINE__,CurrentTime,_u32KeyOffTimeOutCount);
                SetToStandbyMode=TRUE;
            }
            if(SetToStandbyMode)
            {
                SetToStandbyMode=FALSE;
                ReceivingMode=FALSE;
                keyDatArrayRDIdx=0;
                keyDatArrayWTIdx=0;
                for(j=0;j<10;j++)
                {
                    _u16IRData[j]=0;
                }
                FirstRXFlag=FALSE;
                _u16BufferPrev=0;
                _u16BufferCurrent=0;
                LastKeyData=0xFFUL;
                //ExpectedTCome=TRUE;

                _u32IRCount=0;
                _u16IRKeyMap=0;
                RxInProcess=FALSE;

            }
        }
    }
    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Get IR key.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_SH_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
#define SH_IR_REPEAT_THRESHOLD  200
#define SH_IR_REPEAT_THRESHOLD_PLAYBACK  900	//600
#define SH_IR_REPEAT_DELAY  4

    if(_MDrv_SH_IR_CheckKey() == TRUE)//FALSE)
    {
        *pu8Key = g_u8Key;
        *pu8Flag = IsRepeatData;

        if (g_u8Flag)
        {
            U32 u32RepeatThreshold = SH_IR_REPEAT_THRESHOLD;

            if (_MDrv_IR_DiffTimeFromNow(gIRTimeOutCount) < u32RepeatThreshold)
            {//keep press case
                gIRTimeOutCount= _MDrv_IR_GetSystemTime();
                _u8IRRepeateDetect= 1;
                g_ucIrRepeatSkipCnt++;
                if(g_ucIrRepeatSkipCnt < SH_IR_REPEAT_DELAY) //fine tune IR repeat speed
                {//first delay when keep press
                    *pu8Flag = FALSE;
                    //*pu8Key = u8Key;
                    return FALSE;
                }
                else
                {//repeat key case
                    g_ucIrRepeatSkipCnt = (SH_IR_REPEAT_DELAY+1); //to avoid overflow
                    *pu8Flag = g_u8Flag;
                    *pu8Key = g_u8Key;
                    return TRUE;
                }
            }
            else
            {//press frequently case (press-release-press-release...)
                gIRTimeOutCount= _MDrv_IR_GetSystemTime();
                if (0)//_u8IRRepeateDetect == 0)
                {
                    *pu8Flag = FALSE;
                    //*pu8Key = u8Key;
                    return FALSE;
                }
                *pu8Key=g_u8Key;
                *pu8Flag=g_u8Flag;
                _u8IRRepeateDetect = 0;
                g_ucIrRepeatSkipCnt= 0;
                return TRUE;
            }
        }
        else
        {//no repeat key case (maybe first loop of keep press or only press one time)
            gIRTimeOutCount= _MDrv_IR_GetSystemTime();
            *pu8Flag = g_u8Flag;
            *pu8Key = g_u8Key;
            _u8IRRepeateDetect = 0;
            g_ucIrRepeatSkipCnt= 0;
            return TRUE;
        }
    }
    return FALSE;
}


#endif

static U8 _MDrv_IR_ParseKey(U8 u8KeyData)
{
    U8 u8RetVal;

    if ( bIRPass ) return KEYCODE_DUMMY;

    switch(u8KeyData)
    {
    case IRKEY_TV_RADIO        : u8RetVal = KEYCODE_TV_RADIO;         break;
    case IRKEY_CHANNEL_LIST    : u8RetVal = KEYCODE_CHANNEL_LIST;     break;
    case IRKEY_CHANNEL_FAV_LIST: u8RetVal = KEYCODE_CHANNEL_FAV_LIST; break;
    case IRKEY_CHANNEL_RETURN  : u8RetVal = KEYCODE_CHANNEL_RETURN;   break;
    case IRKEY_CHANNEL_PLUS    : u8RetVal = KEYCODE_CHANNEL_PLUS;     break;
    case IRKEY_CHANNEL_MINUS   : u8RetVal = KEYCODE_CHANNEL_MINUS;    break;

    case IRKEY_AUDIO           : u8RetVal = KEYCODE_AUDIO;            break;
    case IRKEY_VOLUME_PLUS     : u8RetVal = KEYCODE_VOLUME_PLUS;      break;
    case IRKEY_VOLUME_MINUS    : u8RetVal = KEYCODE_VOLUME_MINUS;     break;

    case IRKEY_UP              : u8RetVal = KEYCODE_UP;               break;
    case IRKEY_POWER           : u8RetVal = KEYCODE_POWER;            break;
    case IRKEY_EXIT            : u8RetVal = KEYCODE_EXIT;             break;
    case IRKEY_MENU            : u8RetVal = KEYCODE_MENU;             break;
    case IRKEY_DOWN            : u8RetVal = KEYCODE_DOWN;             break;
    case IRKEY_LEFT            : u8RetVal = KEYCODE_LEFT;             break;
    case IRKEY_SELECT          : u8RetVal = KEYCODE_SELECT;           break;
    case IRKEY_RIGHT           : u8RetVal = KEYCODE_RIGHT;            break;

    case IRKEY_NUM_0           : u8RetVal = KEYCODE_NUMERIC_0;        break;
    case IRKEY_NUM_1           : u8RetVal = KEYCODE_NUMERIC_1;        break;
    case IRKEY_NUM_2           : u8RetVal = KEYCODE_NUMERIC_2;        break;
    case IRKEY_NUM_3           : u8RetVal = KEYCODE_NUMERIC_3;        break;
    case IRKEY_NUM_4           : u8RetVal = KEYCODE_NUMERIC_4;        break;
    case IRKEY_NUM_5           : u8RetVal = KEYCODE_NUMERIC_5;        break;
    case IRKEY_NUM_6           : u8RetVal = KEYCODE_NUMERIC_6;        break;
    case IRKEY_NUM_7           : u8RetVal = KEYCODE_NUMERIC_7;        break;
    case IRKEY_NUM_8           : u8RetVal = KEYCODE_NUMERIC_8;        break;
    case IRKEY_NUM_9           : u8RetVal = KEYCODE_NUMERIC_9;        break;

    case IRKEY_MUTE            : u8RetVal = KEYCODE_MUTE;             break;
    case IRKEY_PAGE_UP         : u8RetVal = KEYCODE_PAGE_UP;          break;
    case IRKEY_PAGE_DOWN       : u8RetVal = KEYCODE_PAGE_DOWN;        break;
    case IRKEY_CLOCK           : u8RetVal = KEYCODE_CLOCK;            break;

    case IRKEY_INFO            : u8RetVal = KEYCODE_INFO;             break;
    case IRKEY_RED             : u8RetVal = KEYCODE_RED;              break;
    case IRKEY_GREEN           : u8RetVal = KEYCODE_GREEN;            break;
    case IRKEY_YELLOW          : u8RetVal = KEYCODE_YELLOW;           break;
    case IRKEY_BLUE            : u8RetVal = KEYCODE_BLUE;             break;
    case IRKEY_MTS             : u8RetVal = KEYCODE_MTS;              break;
    case IRKEY_NINE_LATTICE    : u8RetVal = KEYCODE_NINE_LATTICE;     break;
#if defined(DVB_SYSTEM)
    case IRKEY_TTX             : u8RetVal = KEYCODE_TTX;              break;
#elif defined(ATSC_SYSTEM)
    case IRKEY_CC              : u8RetVal = KEYCODE_CC;               break;
#endif
    case IRKEY_INPUT_SOURCE    : u8RetVal = KEYCODE_INPUT_SOURCE;     break;
    case IRKEY_CRADRD          : u8RetVal = KEYCODE_CRADRD;           break;
//    case IRKEY_PICTURE         : u8RetVal = KEYCODE_PICTURE;          break;
    case IRKEY_ZOOM            : u8RetVal = KEYCODE_ZOOM;             break;
    case IRKEY_DASH            : u8RetVal = KEYCODE_DASH;             break;
    case IRKEY_SLEEP           : u8RetVal = KEYCODE_SLEEP;            break;
    case IRKEY_EPG             : u8RetVal = KEYCODE_EPG;              break;
    case IRKEY_PIP             : u8RetVal = KEYCODE_PIP;              break;

  	case IRKEY_MIX             : u8RetVal = KEYCODE_MIX;              break;
    case IRKEY_INDEX           : u8RetVal = KEYCODE_INDEX;            break;
    case IRKEY_HOLD            : u8RetVal = KEYCODE_HOLD;             break;
    case IRKEY_PREVIOUS        : u8RetVal = KEYCODE_PREVIOUS;         break;
    case IRKEY_NEXT            : u8RetVal = KEYCODE_NEXT;             break;
    case IRKEY_BACKWARD        : u8RetVal = KEYCODE_BACKWARD;         break;
    case IRKEY_FORWARD         : u8RetVal = KEYCODE_FORWARD;          break;
    case IRKEY_PLAY            : u8RetVal = KEYCODE_PLAY;             break;
    case IRKEY_RECORD          : u8RetVal = KEYCODE_RECORD;           break;
    case IRKEY_STOP            : u8RetVal = KEYCODE_STOP;             break;
    case IRKEY_PAUSE           : u8RetVal = KEYCODE_PAUSE;            break;

    case IRKEY_SIZE            : u8RetVal = KEYCODE_SIZE;             break;
    case IRKEY_REVEAL          : u8RetVal = KEYCODE_REVEAL;           break;
    case IRKEY_SUBCODE         : u8RetVal = KEYCODE_SUBCODE;          break;

    default                    : u8RetVal = KEYCODE_DUMMY;            break;
    }

    return u8RetVal;
}


//-------------------------------------------------------------------------------------------------
/// Set the timing of IrDa at BOOT stage.
/// @return None
//-------------------------------------------------------------------------------------------------
static void _MDrv_IR_Timing(void)
{
    // header code upper bound
    REG(REG_IR_HDC_UPB) = IR_HDC_UPB;

    // header code lower bound
    REG(REG_IR_HDC_LOB) = IR_HDC_LOB;

    // off code upper bound
    REG(REG_IR_OFC_UPB) = IR_OFC_UPB;

    // off code lower bound
    REG(REG_IR_OFC_LOB) = IR_OFC_LOB;

    // off code repeat upper bound
    REG(REG_IR_OFC_RP_UPB) = IR_OFC_RP_UPB;

    // off code repeat lower bound
    REG(REG_IR_OFC_RP_LOB) = IR_OFC_RP_LOB;

    // logical 0/1 high upper bound
    REG(REG_IR_LG01H_UPB) = IR_LG01H_UPB;

    // logical 0/1 high lower bound
    REG(REG_IR_LG01H_LOB) = IR_LG01H_LOB;

    // logical 0 upper bound
    REG(REG_IR_LG0_UPB) = IR_LG0_UPB;

    // logical 0 lower bound
    REG(REG_IR_LG0_LOB) = IR_LG0_LOB;

    // logical 1 upper bound
    REG(REG_IR_LG1_UPB) = IR_LG1_UPB;

    // logical 1 lower bound
    REG(REG_IR_LG1_LOB) = IR_LG1_LOB;

    // timeout cycles
    REG(REG_IR_TIMEOUT_CYC_L) = IR_RP_TIMEOUT & 0xFFFFUL;
    
    #if(IR_MODE_SEL == IR_TYPE_HWRC_MODE)
	REG(REG_IR_RC_LONGPULSE_THR) = IR_RC_LONGPULSE_THR;
	REG(REG_IR_RC_LONGPULSE_MAR) = IR_RC_LONGPULSE_MAR;
	REG(REG_IR_RC_CLK_INT_THR) = (IR_RC_INTG_THR(IR_RC_INTG_THR_TIME)<<1) + (IR_RC_CLK_DIV(IR_CLK)<<8) + IR_RC6_ECO_EN;
	REG(REG_IR_RC_WD_TIMEOUT_CNT) = IR_RC_WDOG_CNT(IR_RC_WDOG_TIME) + (IR_RC_TMOUT_CNT(IR_RC_TIMEOUT_TIME)<<8);
	REG(REG_IR_TIMEOUT_CYC_H_CODE_BYTE) = (IR_RC5_BITS<<8) | 0x30UL;//[6:4]=011 : timeout be clear at Key Data Code check pass
	#else
	//set up ccode bytes and code bytes/bits num
    REG(REG_IR_TIMEOUT_CYC_H_CODE_BYTE) = IR_CCB_CB | 0x30UL | ((IR_RP_TIMEOUT >> 16) & 0x0FUL);
    #endif
    #ifdef SUPPORT_MULTI_PROTOCOL
    REG(REG_IR_CKDIV_NUM_KEY_DATA) = IR_CKDIV_NUM-1;   // clock divider
    #else
    REG(REG_IR_CKDIV_NUM_KEY_DATA) = IR_CKDIV_NUM;   // clock divider
    #endif
}

unsigned long _MDrv_IR_GetSystemTime(void)
{
    return((unsigned long)((jiffies)*(1000/HZ)));
    //return 0;
}

#if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE) || (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)||IR_MODE_SEL==IR_TYPE_HWRC_MODE)
static void _MDrv_IR_ClearFIFO(void)
{
    unsigned long i;
    unsigned long fifo_status_addr,fifo_read_pulse_addr,fifo_data_addr;
	unsigned long fifo_empty_flag;

	if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE) || (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE))
	{
      fifo_status_addr = REG_IR_SHOT_CNT_H_FIFO_STATUS;
	  fifo_read_pulse_addr = REG_IR_FIFO_RD_PULSE;
	  fifo_data_addr = REG_IR_CKDIV_NUM_KEY_DATA;
	  fifo_empty_flag = IR_FIFO_EMPTY;
	}
	else
	{
      fifo_status_addr = REG_IR_RC_KEY_FIFO_STATUS;
	  fifo_read_pulse_addr = REG_IR_RC_FIFO_RD_PULSE;
	  fifo_data_addr = REG_IR_RC_KEY_COMMAND_ADD;
	  fifo_empty_flag = IR_RC_FIFO_EMPTY;
	}
	
    // Empty the FIFO
    for(i=0; i<8; i++)
    {
        U8 u8Garbage;

        if(REG(fifo_status_addr) & fifo_empty_flag)
            break;

        u8Garbage = REG(fifo_data_addr) >> 8;
        REG(fifo_read_pulse_addr) |= 0x0001UL; //read
    }

}
#endif

#if 0
static BOOL _MDrv_IR_GetInputData(U32* rawData)
{
    BOOL bRet = FALSE;
#if (IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
    U8 i, j;
    for (j=0; j<IR_RAW_DATA_NUM; j++)
    {
        if ( REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)  // check FIFO empty
            break;

        _u8IRRawModeBuf[_u8IRRawModeCount++] = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001UL; //read
#if 0
        if(_u8IRRawModeCount == IR_RAW_DATA_NUM)
        {
            _u8IRRawModeCount = 0;
            *rawData=((_u8IRRawModeBuf[3]<<24)|(_u8IRRawModeBuf[2]<<16)|(_u8IRRawModeBuf[1]<<8)|_u8IRRawModeBuf[0]);
            bRet = TRUE;
        }
#else
        if(_u8IRRawModeCount == IR_RAW_DATA_NUM)
        {
            _u8IRRawModeCount = 0;
            if( (_u8IRRawModeBuf[0]==_u8IRHeaderCode0) &&
                (_u8IRRawModeBuf[1]==_u8IRHeaderCode1) )
            {
                if(_u8IRRawModeBuf[2] == (U8)(~_u8IRRawModeBuf[3]))
                {
                    *rawData = _MDrv_IR_ParseKey(_u8IRRawModeBuf[2]);    // translate for MIPS

                    //set fantasy class to IR
                    *rawData |= (0x01UL << 28);

                    //TODO: Implement repeat code later.

                    bRet = TRUE;
                    break;
                }
            }
        }
#endif
    }

    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
#endif
    return bRet;
}
#endif

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
#define MaxQueue 100
static struct queue
{
    u32 item[MaxQueue];
    int front;
    int rear;
} q;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 38)
static DEFINE_SEMAPHORE(queue_lock);
#else
static DECLARE_MUTEX(queue_lock);
#endif
void enqueue(u32 data)
{
    if (down_trylock(&queue_lock))
       return;

    if (q.rear == ((q.front + 1) % MaxQueue))
    {
        IR_PRINT("queue is full \n");
    }
    else
    {
        q.rear = (q.rear + 1) % MaxQueue;
        q.item[q.rear] = data;
    }

    up(&queue_lock);
}

u32 dequeue(void)
{
    u32 data = 0xFFFFUL;

    down(&queue_lock);

    if (q.front == q.rear)
    {
        IR_PRINT("queue is empty \n");
    }
    else
    {
        q.front = (q.front + 1) % MaxQueue;
        data = q.item[q.front];
    }

    up(&queue_lock);
    return data;
}

int key_dispatch(void *p)
{
    static u32 prev_scancode = 0xFFFFUL;

    printk("[KEY_INPUT]:key_dispatch thread start\n");
    //daemonize("ir_key_dispatch");
    while(1)
    {
        int ret;
        u32 scancode;

        try_to_freeze();

        //IR_PRINT("xxxxx 0\n");
        if (prev_scancode == 0xFFFFUL)
        {
            //IR_PRINT("xxxxx 1\n");
            ret = wait_for_completion_interruptible(&key_completion);
        }
        else
        {
            //IR_PRINT("xxxxx 2\n");
            // Depend on different IR to wait timeout.
            // or IR_TYPE_MSTAR_DTV, 150 is better, because ISR need such time to get another ir key.
            //
            // NOTE:
            // Too small, you will find the repeat function in android don't work. (up immediately)
            // It will become down->up->down->down.....(not continue down)
            // In input driver(2.6.35), over REP_DELAY(250 msecs) will auto-repeat, and every REP_PERIOD(33 msecs) will send repeat key.
            // In input driver(3.0.20), over REP_DELAY(500 msecs) will auto-repeat, and every REP_PERIOD(125 msecs) will send repeat key.
            // In android, over DEFAULT_LONG_PRESS_TIMEOUT(500 mesc) will auto-repeat, and every KEY_REPEAT_DELAY(50 mesc) will send repeat key.
            ret = wait_for_completion_interruptible_timeout(&key_completion, msecs_to_jiffies(IR_EVENT_TIMEOUT));
        }
        if (ret < 0)
        {
            IR_PRINT("completion interruptible\n");
            continue;
        }

        scancode = dequeue();
        if ((prev_scancode != 0xFFFFUL) && (scancode == 0xFFFFUL))
        {
            //IR_PRINT("xxxxx 3\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
            rc_keyup(ir->dev);
#else
            ir_input_nokey(ir->dev, &ir->ir);
#endif
        }
        else if ((prev_scancode != 0xFFFFUL) && (scancode != 0xFFFFUL))
        {
           //IR_PRINT("xxxxx 4\n");
           if ((scancode != prev_scancode))
           {
               //IR_PRINT("xxxxx 5\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
               rc_keyup(ir->dev);
#else
               ir_input_nokey(ir->dev, &ir->ir);
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
               rc_keydown_notimeout(ir->dev, scancode, 0);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
               ir_input_keydown(ir->dev, &ir->ir, scancode);
#else
               ir_input_keydown(ir->dev, &ir->ir, scancode, 0);
#endif
           }
        }
        else if ((prev_scancode == 0xFFFFUL) && (scancode != 0xFFFFUL))
        {
            //IR_PRINT("xxxxx 6\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
            rc_keydown_notimeout(ir->dev, scancode, 0);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
            ir_input_keydown(ir->dev, &ir->ir, scancode);
#else
            ir_input_keydown(ir->dev, &ir->ir, scancode, 0);
#endif
        }

        //IR_PRINT("xxxxx 7, scancode=%d\n", scancode);
        prev_scancode = scancode;
    }
    printk(" key_dispatch thread end\n");

    return 0;
}
#endif // CONFIG_MSTAR_IR_INPUT_DEVICE

#ifdef CONFIG_MSTAR_PM_SWIR
static int swir_pm_status = 0;
#define IR_MBX_TIMEOUT 1500
#define IR_MBX_QUEUESIZE        8

typedef enum
{
    /// mbx command for initialization
    E_IR_CPUTo51_CMD_INIT=0x00,
    /// mbx command for configuration
    E_IR_CPUTo51_CMD_CONFIG,
    /// mbx command for key code
    E_IR_CPUTo51_CMD_KEYCODE,
    /// mbx command for set callback
    E_IR_CPUTo51_CMD_SETCLBK,
    /// mbx command for library version
    E_IR_CPUTo51_CMD_LIBVER,
    /// mbx command for status
    E_IR_CPUTo51_CMD_STATUS,
    /// mbx command for enable
    E_IR_CPUTo51_CMD_ENABLE,
    /// mbx command for driver info
    E_IR_CPUTo51_CMD_INFO,
    /// mbx command for sw init 
    E_IR_CPUTo51_CMD_SWIR_INT,
    /// mbx command for sw ir key code 
    E_IR_CPUTo51_CMD_SWIR_KEYCODE,
} IR_CPUTo51CmdIdx;

/// emurate IR mailbox commands ack from mcu51 to cpu
typedef enum
{
    //(1) Acknowledge from MCU51
    /// ack mbx command for initialization
    E_IR_51ToCPU_CMD_ACK_INIT=0x00,
    /// ack mbx command for configuration
    E_IR_51ToCPU_CMD_ACK_CONFIG,
    /// ack mbx command for key code
    E_IR_51ToCPU_CMD_ACK_KEYCODE,
    /// ack mbx command for set callback
    E_IR_51ToCPU_CMD_ACK_SETCLBK,
    /// ack mbx command for library version
    E_IR_51ToCPU_CMD_ACK_LIBVER,
    ///ack mbx command for status
    E_IR_51ToCPU_CMD_ACK_STATUS,
    ///ack mbx command for enable
    E_IR_51ToCPU_CMD_ACK_ENABLE,
    ///ack mbx command for driver info
    E_IR_51ToCPU_CMD_ACK_INFO,

    //(2) Notification from MCU51
    ///notification mbx command for key code
    E_IR_51ToCPU_CMD_KEYCODE,

} IR_51ToCPUCmdIdx;

/// emurate ack flags
typedef enum
{
    /// ack flag for null
    E_IR_ACKFLG_NULL         = 0,
    /// ack flag for wait initialization
    E_IR_ACKFLG_WAIT_INIT    = (1<<0),
    /// ack flag for wait configuration
    E_IR_ACKFLG_WAIT_CONFIG  = (1<<1),
    /// ack flag for wait key code
    E_IR_ACKFLG_WAIT_KEYCODE = (1<<2),
    /// ack flag for wait set callback
    E_IR_ACKFLG_WAIT_SETCLBK = (1<<3),
    /// ack flag for wait library version
    E_IR_ACKFLG_WAIT_LIBVER  = (1<<4),
    /// ack flag for wait status
    E_IR_ACKFLG_WAIT_STATUS  = (1<<5),
    /// ack flag for wait enable
    E_IR_ACKFLG_WAIT_ENABLE  = (1<<6),
    /// ack flag for wait driver info
    E_IR_ACKFLG_WAIT_INFO    = (1<<7),
} IR_AckFlags;

typedef enum
{
    /// IR result for failure
    E_IR_FAIL =0,
    /// IR result for OK
    E_IR_OK = 1,

} IR_Result;

typedef struct
{
    //MS_U8 u8ModeSel;          /// IR mode selection
    MS_U8 u8Ctrl0;          /// IR enable control 0
    MS_U8 u8Ctrl1;          /// IR enable control 1
    MS_U8 u8Clk_mhz;        /// IR required clock
    MS_U8 u8HdrCode0;       /// IR Header code 0
    MS_U8 u8HdrCode1;       /// IR Header code 1
    MS_U8 u8CCodeBytes;     /// Customer codes: 1 or 2 bytes
    MS_U8 u8CodeBits;       /// Code bits: 0x00~0x7F
    MS_U8 u8HdrCode20;      /// 2nd IR Header code 0
    MS_U8 u8HdrCode21;      /// 2nd IR Header code 1

} IR_RegCfg;

static IR_AckFlags gIRAckFlags;
static MBX_Result gMBXResult;
static IR_Result gIRResult;

static void _MDrv_IR_MailBoxHandler(MBX_Class eMsgClass, MBX_MSGQ_Status *pMsgQueueStatus)
{
    MBX_Msg MB_Command;

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gMBXResult = MDrv_MBX_RecvMsg(SWIR_FILEOPS, eMsgClass, &MB_Command, IR_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);
    if (gMBXResult == E_MBX_ERR_TIME_OUT)
    {
        printk(KERN_EMERG "Handler Timeout!\n");
        return;
    }

    if (gMBXResult == E_MBX_SUCCESS)
    {
        if ((MB_Command.u8Ctrl != 0) && (MB_Command.u8Ctrl != 1))
        {
            gIRResult = E_IR_FAIL;
            printk(KERN_EMERG "Not Implemented!\n");
            return;
        }
        //printk(KERN_EMERG "Get IR command: 0x%02x.\n", MB_Command.u8Index);
        //printk(KERN_EMERG "Parameter[0]=%d\n",  MB_Command.u8Parameters[0]);
        gIRResult = E_IR_FAIL;
        switch (MB_Command.u8Index)
        {
            case E_IR_51ToCPU_CMD_ACK_INIT:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_INIT);
                break;
            case E_IR_51ToCPU_CMD_ACK_CONFIG:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_CONFIG);
                break;
            case E_IR_51ToCPU_CMD_ACK_KEYCODE:
                gIRResult = E_IR_OK;
                //gu8IRKey = MB_Command.u8Parameters[0];
                //gu8IRRepeat = MB_Command.u8Parameters[1];
                _KeyReceived.u8Key = MB_Command.u8Parameters[0];
                _KeyReceived.u8Flag =  MB_Command.u8Parameters[1];
                _KeyReceived.u8Valid = 1;

                if (IRDev.async_queue)
    		        kill_fasync(&IRDev.async_queue, SIGIO, POLL_IN);
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_KEYCODE);

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
                enqueue((_KeyReceived.u8System << 8) | _KeyReceived.u8Key);
                complete(&key_completion);
#endif
                break;
            case E_IR_51ToCPU_CMD_ACK_SETCLBK:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_SETCLBK);
                break;
            case E_IR_51ToCPU_CMD_ACK_LIBVER:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_LIBVER);
                break;
            case E_IR_51ToCPU_CMD_ACK_STATUS:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_STATUS);
                break;
            case E_IR_51ToCPU_CMD_ACK_ENABLE:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_ENABLE);
                break;
            case E_IR_51ToCPU_CMD_ACK_INFO:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_INFO);
                break;
            default:
                break;
        }

    }

    return;

}

int MDrv_PMSWIR_ReceiveMsg(void)
{
    MBX_MSGQ_Status MbxMsgQStatus;

    _MDrv_IR_MailBoxHandler(E_MBX_CLASS_IRKEY_NOWAIT, &MbxMsgQStatus);

    return TRUE;
}
#endif

#ifdef SUPPORT_MULTI_PROTOCOL
inline static void _MDrv_MultiProtocol_RestartTimer(void)
{
    timer_pending(&_key_MultiProtocol_timer);
    mod_timer(&_key_MultiProtocol_timer, jiffies + 5);
}

inline static void _MDrv_MultiProtocol_StartTimer(void)
{
    DRV_IR_PROTOCOL_TYPE *pstProtocol;
    pstProtocol = _GetProtocolEntry(_eCurentProtocol);
    timer_pending(&_key_MultiProtocol_timer);
    mod_timer(&_key_MultiProtocol_timer, jiffies + (pstProtocol->u32Timeout/1000));
}

static void _MDrv_MultiProtocol_Cb(unsigned long params)
{
    if(_u64LastData==0||_u64LastData&E_IR_KEY_STATE_RELEASE)
    {
        return;
    }

    while(down_trylock(&fantasy_protocol.sem)!=0)
    {
        _MDrv_MultiProtocol_RestartTimer();
    }

    if (fantasy_protocol.data != 0)
    {
        _MDrv_MultiProtocol_RestartTimer();
    }
    else
    {
        _u64LastData = ((_u64LastData>>4)<<4)|E_IR_KEY_STATE_RELEASE;
        fantasy_protocol.data = _u64LastData;
    }
    up(&fantasy_protocol.sem);
    wake_up_interruptible(&fantasy_protocol.inq);

}

static void _MDrv_MultiProtocol_Init(void)
{
   init_timer(&_key_MultiProtocol_timer);
   _key_MultiProtocol_timer.expires = jiffies + 0xFFFFFFFFUL;
   _key_MultiProtocol_timer.function = _MDrv_MultiProtocol_Cb;
   add_timer(&_key_MultiProtocol_timer);
}
#endif

#ifdef SUPPORT_MULTI_PROTOCOL
static IR_PROCOCOL_TYPE _MDrv_MultiProtocol_GetProByLeadCode(void)
{
    U8 u8Index;
    U8 u8StartIdx=0;
    BOOL bResult = FALSE;
    IR_PROCOCOL_TYPE eTemp=E_IR_PROTOCOL_NONE;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;

    for(u8Index=0; u8Index<PROTOCOL_SUPPORT_MAX; u8Index++)
    {
        if(_eDetectList[u8Index]==E_IR_PROTOCOL_NONE)
            continue;

        pstProtocol = _GetProtocolEntry(_eDetectList[u8Index]);
        bResult = pstProtocol->findleadcode(&u8StartIdx);
        if(bResult==TRUE)
        {
            eTemp = _eDetectList[u8Index];
            if(u8Index!=0 && _eDetectList[0]!=E_IR_PROTOCOL_NONE)
            {//switch newest protocol to list head
                _eDetectList[u8Index] = _eDetectList[0];
                _eDetectList[0] = eTemp;
            }
            return eTemp;//return detect result
        }
    }

    return E_IR_PROTOCOL_NONE;
}

static IR_DECODE_STATUS _MDrv_MultiProtocol_ParseShotCount(IR_PROCOCOL_TYPE eProtocol, unsigned long long *pu64RawData)
{
    IR_DECODE_STATUS eResult = E_IR_DECODE_ERR;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;
    U32 u32CustCode =0;
    U16 u16KeyCode = 0;
    U8 u8State = 0;
    U8 u8Reserved = 0;

    if(eProtocol>E_IR_PROTOCOL_NONE && eProtocol<E_IR_PROTOCOL_MAX)
    {
        pstProtocol = _GetProtocolEntry(eProtocol);
        eResult = pstProtocol->parseprotocol(&u32CustCode, &u16KeyCode, &u8State, &u8Reserved);
    }

    if(eResult ==E_IR_DECODE_DATA_OK)
    {
        *pu64RawData = (((unsigned long long)u8Reserved)<<56)|(((unsigned long long)u32CustCode)<<24) | (u16KeyCode<<8) | u8State;
    }
    return eResult;
}

static U8 _MDrv_MultiProtocol_GetMaxLeadCodeShot(void)
{
    U8 u8Max=0;
    U8 i;
    IR_PROCOCOL_TYPE eProtocol;
    DRV_IR_PROTOCOL_TYPE *pstProtocol;

    for(i=0; i<PROTOCOL_SUPPORT_MAX; i++)
    {
        eProtocol =_eDetectList[i];
        if(eProtocol!= E_IR_PROTOCOL_NONE)
        {
            pstProtocol = _GetProtocolEntry(eProtocol);
            if(pstProtocol->u8LeadCodeMinCount > u8Max)
            {
                u8Max =pstProtocol->u8LeadCodeMinCount;
            }
        }
    }

    return u8Max;
}
#endif
//-------------------------------------------------------------------------------------------------
/// ISR when receive IR key.
/// @return None
//-------------------------------------------------------------------------------------------------
#if (IR_MODE_SEL == IR_TYPE_HWRC_MODE)
irqreturn_t _MDrv_IR_RC_ISR(int irq, void *dev_id)
//irqreturn_t _MDrv_IR_ISR(int irq, void *dev_id, struct pt_regs *regs)
{

    U8 u8Key=0, u8RepeatFlag=0;
    U8 u8System = 0;

    BOOL bHaveKey = FALSE;
    if(MSTAR_FANTASY_PROTOCOL_SUPPORTED)
    {
        if ((bHaveKey=_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag)) != 0)
        {
            //printk("==============\n");
			//printk("u8Key=0x%x,u8System=0x%x,u8RepeatFlag=0x%x\n",u8Key,u8System,u8RepeatFlag);
			//printk("==============\n");
            if(down_trylock(&fantasy_protocol.sem)==0)
            {
                if(fantasy_protocol.data!=0)
                {
                    printk(KERN_WARNING"unprocessed fantasy protocol data!!\n");
                }
                else
                {
                    fantasy_protocol.data = (u8Key<<8 | u8RepeatFlag);
                    fantasy_protocol.data |= (0x01UL << 28);
					//printk("fantasy_protocol.data = 0x%x", fantasy_protocol.data);
                }
                up(&fantasy_protocol.sem);
                wake_up_interruptible(&fantasy_protocol.inq);
            }
        }
    }
    else if ((bHaveKey=_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag)) != 0)
    {
        _ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();

        //temporary solution, need to implement ring buffer for this
        _KeyReceived.u8Key = u8Key;
        _KeyReceived.u8System = u8System;
        _KeyReceived.u8Flag = u8RepeatFlag;
        _KeyReceived.u8Valid = 1;

       	if (IRDev.async_queue)
    		kill_fasync(&IRDev.async_queue, SIGIO, POLL_IN);
    }
    IR_PRINT("HaveKey=%d, KEY=%d, RepeatFalg=%d\n", bHaveKey, u8Key, u8RepeatFlag);

    return IRQ_HANDLED;
}


#else
irqreturn_t _MDrv_IR_ISR(int irq, void *dev_id)
{
    U8 u8System = 0, u8RepeatFlag=0;
    U8 u8Key[IR_RAW_DATA_NUM]={0,0,0,0};

#ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE
    U8	u8KeyTvLink[IR_TV_LINK_DATA_NUM];
    BOOL bPass = FALSE;

    if( _u8IRMode == IR_MODE_WB_CODE )
    {
        if(_MDrv_IR_GetKeyOfWBCode( u8KeyTvLink, &u8RepeatFlag))
        {
#ifdef	NOT_USED_4_LGE
            _ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();
#endif
            IR_PRINT("_MDrv_IR_ISR() [%d]-> %x, %x\n", __LINE__,u8KeyTvLink[0], u8RepeatFlag);

            if( u8RepeatFlag == (_IR_READ_TYPE_WB_CODE - 1) )
            {
                memcpy( _TvLinkReceived.u8Data, u8KeyTvLink, IR_WB_CODE_DATA_NUM );
                _TvLinkReceived.u8Flag  	= u8RepeatFlag;
                _TvLinkReceived.u8Valid 	= 1;
            }
            else
            {
                //temporary solution, need to implement ring buffer for this
                _TvLinkReceived.u8Data[0]	= u8KeyTvLink[0];
                _TvLinkReceived.u8Flag  	= u8RepeatFlag;
                _TvLinkReceived.u8Valid 	= 1;
            }

            if (IRDev.async_queue)
                wake_up_interruptible(&key_wait_q);
                //kill_fasync(&IRDev.async_queue, SIGRTMIN, POLL_IN);
        }
    } 
    else if( _u8IRMode == IR_MODE_TV_LINK )
    {
        static unsigned long ulPreTime;

        if (_MDrv_IR_GetSystemTime() - ulPreTime > IR_TIMEOUT_CYC/1000) //timeout
        {
            _u32IRCount=0;
            _u32IRAllCount = 0;
        }


        if (_u32IRCount <IR_SWDECODE_MODE_BUF_LEN)
        {
#if 0
                if( _u32IRAllCount % 2 == 0 )
		{
                        _u32IRDataTmp[_u32IRCount] = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16) | REG(REG_IR_SHOT_CNT_L);
                        //_u32IRDataTmp[_u32IRCount] = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16);
                        //_u32IRDataTmp[_u32IRCount] = (*(volatile U8*)(REG_IR_SHOT_CNT_L + 1)) << 8;
                        //_u32IRDataTmp[_u32IRCount] += (*(volatile U8*)(REG_IR_SHOT_CNT_L));
		}
                else
#endif
		{
                        _u32IRData[_u32IRCount] = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16) | REG(REG_IR_SHOT_CNT_L);
                        //_u32IRData[_u32IRCount] = ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & 0xF) << 16);
                        //_u32IRData[_u32IRCount] += REG(REG_IR_SHOT_CNT_L);
                        //_u32IRData[_u32IRCount] = (*(volatile U8*)(REG_IR_SHOT_CNT_L + 1)) << 8;
                        //_u32IRData[_u32IRCount] += (*(volatile U8*)(REG_IR_SHOT_CNT_L));
                        _u32IRCount++;
                        bPass = TRUE;
		}
			_u32IRAllCount++;
        }
        ulPreTime = _MDrv_IR_GetSystemTime();

        if( (bPass == TRUE ) && (_MDrv_IR_GetKeyOfTVLink( u8KeyTvLink, &u8RepeatFlag )) )
        {
#ifdef	NOT_USED_4_LGE
            _ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();
#endif
            //IR_PRINT("_MDrv_IR_ISR_TV_LINK() -> %x, %x\n", u8KeyTvLink[0], u8RepeatFlag);
            if( u8RepeatFlag == (_IR_READ_TYPE_WB_CODE - 1) )
            {				
                memcpy( _TvLinkReceived.u8Data, u8KeyTvLink, IR_WB_CODE_DATA_NUM );
                _TvLinkReceived.u8Flag  	= u8RepeatFlag;
                _TvLinkReceived.u8Valid 	= 1;
            }
            else if( u8RepeatFlag == (_IR_READ_TYPE_TV_LINK - 1) )
            {
                memcpy( _TvLinkReceived.u8Data, u8KeyTvLink, IR_TV_LINK_DATA_NUM );
                _TvLinkReceived.u8Flag  	= u8RepeatFlag;
                _TvLinkReceived.u8Valid 	= 1;
            }
            else
            {
                //temporary solution, need to implement ring buffer for this
                _TvLinkReceived.u8Data[0]	= u8KeyTvLink[0];
                _TvLinkReceived.u8Flag  	= u8RepeatFlag;
                _TvLinkReceived.u8Valid 	= 1;
            }

            if (IRDev.async_queue)
                wake_up_interruptible(&key_wait_q);
                //kill_fasync(&IRDev.async_queue, SIGRTMIN, POLL_IN);
        }
    }
    else
    {
        if(_MDrv_IR_GetKey(&(u8Key[0]), &u8System, &u8RepeatFlag))
        {
#ifdef	NOT_USED_4_LGE
            _ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();
#endif
            IR_PRINT("_MDrv_IR_ISR() -> %02x, %02x\n", u8Key[2], u8RepeatFlag);

            memcpy( _TvLinkReceived.u8Data, &(u8Key[0]), IR_RAW_DATA_NUM );
            _TvLinkReceived.u8Flag  	= u8RepeatFlag;
            _TvLinkReceived.u8Valid 	= 1;

            if (IRDev.async_queue)
                wake_up_interruptible(&key_wait_q);
        }
    }
#else	/* #ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE */
    //regs = NULL;
    if(MSTAR_FANTASY_PROTOCOL_SUPPORTED)
    {
//        U32 u32InputData=0;
//        if(_MDrv_IR_GetInputData(&u32InputData))

        if (_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag))
        {
            if(down_trylock(&fantasy_protocol.sem)==0)
            {
                if(fantasy_protocol.data!=0)
                {
                    //printk(KERN_WARNING"unprocessed fantasy protocol data!!\n");
                }
                else
                {
                    //fantasy_protocol.data=u32InputData;
                    fantasy_protocol.data = (u8Key<<8 | u8RepeatFlag);

                    //set fantasy class to IR
                    fantasy_protocol.data |= (0x01 << 28);
                }
                up(&fantasy_protocol.sem);

                wake_up_interruptible(&fantasy_protocol.inq);
            }
        }
    }
    else if (_MDrv_IR_GetKey(&u8Key, &u8System, &u8RepeatFlag))
    {
#ifdef	NOT_USED_4_LGE
        _ulLastKeyPresentTime = _MDrv_IR_GetSystemTime();
#endif

        //IR_PRINT("_MDrv_IR_ISR() -> %x, %x\n", u8Key, u8RepeatFlag);

        //temporary solution, need to implement ring buffer for this
        _KeyReceived.u8Key = u8Key;
        _KeyReceived.u8System = u8System;
        _KeyReceived.u8Flag = u8RepeatFlag;
        _KeyReceived.u8Valid = 1;

       	if (IRDev.async_queue)
    		kill_fasync(&IRDev.async_queue, SIGIO, POLL_IN);
    }

#endif	/* #ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE */

    //Disabled already by SA_INTERRUPT at initial
    //MsOS_EnableInterrupt(E_FIQ_IR);

    return IRQ_HANDLED;
}
#endif

#ifdef CONFIG_MSTAR_MUSTANG
static unsigned long  _ulPrevKeyRepeatTime;
static BOOL  _bCheckQuickRepeat;
#endif

#if (IR_MODE_SEL == IR_TYPE_FULLDECODE_MODE)
//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key or repeat key is faster than the specified period
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
#ifndef CONFIG_MSTAR_IR_INPUT_DEVICE
    static unsigned long  _ulPrevKeyRepeatTime;
#endif
    static BOOL  _bCheckQuickRepeat;
    BOOL bRet=FALSE;
    *pu8System = 0;

    if(REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)
    {
        _bCheckQuickRepeat = 0;
        return FALSE;
    }

    if(((_MDrv_IR_GetSystemTime() - _ulPrevKeyTime) >= IR_TIMEOUT_CYC/1000))
    {
        *pu8Key = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001UL; //read
        #ifdef __arm__
	mdelay(10);
        #else
        udelay(1000*10);
        #endif
        #if(IR_TYPE_SEL == IR_TYPE_TOSHIBA)
        *pu8Key = _MDrv_IR_CUS22T_ParseKey(*pu8Key);
        #endif
        _u8PrevKeyCode = *pu8Key;
        *pu8Flag = 0;
        _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
        _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
        _bCheckQuickRepeat = 0;
        _MDrv_IR_ClearFIFO();
        return TRUE;
    }
    else
    {
        if(_bCheckQuickRepeat==0)
        {
            _bCheckQuickRepeat = 1;
            _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
            _MDrv_IR_ClearFIFO();
            return FALSE;
        }
        *pu8Key = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        *pu8Flag = (REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_RPT_FLAG)? 1 : 0;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001UL; //read
        bRet = FALSE;
        _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
        if ( (*pu8Flag == 1) && ( *pu8Key == _u8PrevKeyCode ))
        {
#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
            bRet = TRUE;
#else
            unsigned long i = _MDrv_IR_GetSystemTime();
            if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                _u8PrevKeyCode     = *pu8Key;
                _ulPrevKeyRepeatTime    = i;
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_1stDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
                    bRet = TRUE;
                }
            }
            else //E_IR_KEY_PROPERTY_FOLLOWING
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_2ndDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    bRet = TRUE;
                }
            }
#endif // CONFIG_MSTAR_IR_INPUT_DEVICE
        }
    }

    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
    return bRet;

}

#elif (IR_MODE_SEL == IR_TYPE_HWRC_MODE)
//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key or repeat key is faster than the specified period
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    U8 u8KeyAddr,u8KeyCmd;
    BOOL bRet=FALSE;
    *pu8System = 0;

    if(REG(REG_IR_RC_KEY_FIFO_STATUS) & IR_RC_FIFO_EMPTY)
    {
        return FALSE;
    }

    u8KeyAddr = REG(REG_IR_RC_KEY_COMMAND_ADD)&0x3fUL;//RC5: {2'b0,toggle,address[4:0]} reg[7:0]
    u8KeyCmd = (REG(REG_IR_RC_KEY_COMMAND_ADD)&0x3f00UL)>>8;//RC5: {repeat,1'b0,command[13:8]} reg[15:8]
    *pu8Flag = (REG(REG_IR_RC_KEY_COMMAND_ADD)&0x8000UL)>>15;//repeat
    *pu8Key = u8KeyCmd;

	//printk("REG_IR_RC_KEY_FIFO_STATUS = 0x%x\n",REG(REG_IR_RC_KEY_FIFO_STATUS));
	//printk("REG_IR_RC_KEY_COMMAND_ADD = 0x%x\n",REG(REG_IR_RC_KEY_COMMAND_ADD));
    //printk("@@@ RC5 protocol: Addr = 0x%x, Cmd = 0x%x \n",u8KeyAddr,u8KeyCmd);
	//printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	REG(REG_IR_RC_FIFO_RD_PULSE)|=0x1UL;

	bRet=TRUE;
    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
    return bRet;

}

#elif (IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
//-------------------------------------------------------------------------------------------------
/// Get IR key.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
#if 0
static unsigned long  _ulPrevKeyRepeatTime;
static BOOL  _bCheckQuickRepeat;
static BOOL msIR_GetRawKey(U8 *pu8key,U8 *pu8flag,BOOL bChkCCode)
{
    U8 u8IRRawModeDone;
    U8 u8IRRawModeCount;
    U8 k;

    u8IRRawModeCount = 0;

    *pu8key = 0xFFUL;
    *pu8flag = (REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_RPT_FLAG)? 1 : 0;

    for(k=0;k<IR_RAW_DATA_NUM;k++)
    {
        if ( REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)  // check FIFO empty
            break;
        _u8IRRawModeBuf[_u8IRRawModeCount++] = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001UL; //read
        u8IRRawModeCount++;
    }
    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x8000UL; //IMPORTANT!!!

    if ( u8IRRawModeCount >= 4 )
        u8IRRawModeDone = 1;

    if (u8IRRawModeDone)
    {
        u8IRRawModeDone = 0;
        if(bChkCCode==TRUE)
        {
            if ( (_u8IRRawModeBuf[0] == _u8IRHeaderCode0) && (_u8IRRawModeBuf[1] == _u8IRHeaderCode1) )
            {
                if(_u8IRRawModeBuf[2] == (U8)(~_u8IRRawModeBuf[3]))
                {
                    *pu8key = _u8IRRawModeBuf[2];
                    return TRUE;
                }
            }
        }
        else
        {
            *pu8key = _u8IRRawModeBuf[2]; //driectly assign key with index==2
            return TRUE;
        }
        u8IRRawModeCount = 0;
    }
    return FALSE;
}

static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    unsigned long i;
    BOOL bRet=FALSE;
    *pu8System = 0;

    if(REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)
    {
        _bCheckQuickRepeat = 0;
        return FALSE;
    }

    if(((_MDrv_IR_GetSystemTime() - _ulPrevKeyTime) >= IR_TIMEOUT_CYC/1000))
    {
        *pu8Key = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001UL; //read
        #ifdef __arm__
        __udelay(1000*10);
        #else
        udelay(1000*10);
        #endif
        msIR_GetRawKey(pu8Key,pu8Flag,TRUE);
        _u8PrevKeyCode = *pu8Key;
        *pu8Flag = 0;
        _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
        _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
        _bCheckQuickRepeat = 0;
        _MDrv_IR_ClearFIFO();
        return TRUE;
    }
    else
    {
        if(_bCheckQuickRepeat==0)
        {
           _bCheckQuickRepeat = 1;
            _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
            _MDrv_IR_ClearFIFO();
            return FALSE;
        }

        msIR_GetRawKey(pu8Key,pu8Flag,TRUE);
        bRet = FALSE;
        _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
        if ( (*pu8Flag == 1) && ( *pu8Key == _u8PrevKeyCode ))
        {
            i = _MDrv_IR_GetSystemTime();
            if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                _u8PrevKeyCode     = *pu8Key;
                _ulPrevKeyRepeatTime    = i;
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_1stDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
                    bRet = TRUE;
                }
            }
            else //E_IR_KEY_PROPERTY_FOLLOWING
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_2ndDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    bRet = TRUE;
                }
            }
        }
    }

    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
    return bRet;

}
#else
static BOOL flag_repeat = FALSE;
static U8  prev_key_rpt[IR_RAW_DATA_NUM];

static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    BOOL bRet = FALSE;

    u32 j;
    *pu8System = 0;
	if ((flag_repeat==TRUE) && ((REG(REG_IR_SHOT_CNT_H_FIFO_STATUS)&IR_RPT_FLAG) == IR_RPT_FLAG))
	{
		 memcpy( pu8Key, &(prev_key_rpt[0]), IR_RAW_DATA_NUM );
		 *pu8Flag = 1;
		 bRet = TRUE;
		 goto  Raw_Repeat;
	}
	flag_repeat=FALSE;

    for (j=0,_u8IRRawModeCount=0; j<IR_RAW_DATA_NUM; j++)
    {
        if ( REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)  // check FIFO empty
            break;

        _u8IRRawModeBuf[_u8IRRawModeCount++] = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
        REG(REG_IR_FIFO_RD_PULSE) |= 0x0001UL; //read

        if(_u8IRRawModeCount == IR_RAW_DATA_NUM)
        {
            _u8IRRawModeCount = 0;
            if( (_u8IRRawModeBuf[0]==_u8IRHeaderCode0) &&
                (_u8IRRawModeBuf[1]==_u8IRHeaderCode1) )
            {
                if(_u8IRRawModeBuf[2] == (U8)(~_u8IRRawModeBuf[3]))
                {
                    writeb(_u8IRRawModeBuf[2], (void *)0xFD001D55); // store IR key for the internal micom
#if 0
                    *pu8Key = _MDrv_IR_ParseKey(_u8IRRawModeBuf[2]);    // translate
#else
                    memcpy( pu8Key, &(_u8IRRawModeBuf[0]), IR_RAW_DATA_NUM );
                    memcpy( &(prev_key_rpt[0]), &(_u8IRRawModeBuf[0]), IR_RAW_DATA_NUM );
#endif
                    *pu8Flag = 0;
                    bRet = TRUE;
                    flag_repeat = TRUE;
                    break;
                }
            }
            else if( (_u8IRRawModeBuf[0] == 0x1F) ||(_u8IRRawModeBuf[0] >= 0x1C && _u8IRRawModeBuf[0] <= 0x1E) ) // WB protocol
            {
                memcpy( pu8Key, &(_u8IRRawModeBuf[0]), IR_RAW_DATA_NUM );
                memcpy( &(prev_key_rpt[0]), &(_u8IRRawModeBuf[0]), IR_RAW_DATA_NUM );
                *pu8Flag = 0; //RawData number
                bRet = TRUE;
                flag_repeat = FALSE;
                break;
            }
        }
    }
Raw_Repeat:
    // Empty the FIFO
    //_MDrv_IR_ClearFIFO();
    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= (0x1UL <<15);
    return bRet;
}
#endif

#elif(IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
//-------------------------------------------------------------------------------------------------
/// Get IR key.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    BOOL bRet = FALSE;

#if(IR_TYPE_SEL == IR_TYPE_RCMM)
    unsigned long i;
#else
#if ((IR_TYPE_SEL != IR_TYPE_CUS21SH) && (IR_TYPE_SEL != IR_TYPE_TOSHIBA))
    #if (IR_TYPE_SEL != IR_TYPE_CUS08_RC5)
    U32 u8Byte, u8Bit;
#if(IR_TYPE_SEL == IR_TYPE_SKYWORTH)
#if ENABLE_PIONEER_IR
    U8 u8IRSwModeBuf[IR_RAW_DATA_NUM];
#else
	U8 u8IRSwModeByte;
    U8 u8IRSwModeBuf[IR_RAW_DATA_NUM];
#endif
#else
    U8 u8IRSwModeBuf[IR_RAW_DATA_NUM];
#endif
    U32 *pu32IRData = NULL;
    *pu8System = 0;
    #else
    unsigned long i;
    #endif
#endif
#endif

#if(IR_TYPE_SEL == IR_TYPE_RCMM)

    if(UpDataFlage)
    {
        //printk("GetKey\n");
        UpDataFlage = FALSE;

        switch(RCMode)
        {
            case RCMMOEM_LONGID_MODE|RC_MODE:
            {
                if((_u8IrPreRcmmData[0] == RCByte[0]) && (_u8IrPreRcmmData[1] == RCByte[1]) && (_u8IrPreRcmmData[2] == RCByte[2]) && (_u8IrPreRcmmData[3] == RCByte[3]))
                {
                    *pu8Flag = TRUE;
                }

                u16CustomerID = ((RCByte[0] & 0x1FUL) << 4) | ((RCByte[1] & 0xF0UL) >> 4);

                if(u16CustomerID == 0x007DUL)
                {
                    *pu8Key = RCByte[3];
                    if(*pu8Key>=30 && *pu8Key<=132)
                        *pu8Key = RCByte[3]+111;
                     //printk("0x%x:,%d\n",*pu8Key,*pu8Key);
                    *pu8System = (RCByte[2] & 0x7FUL);

                    _u8IrPreRcmmData[0] = RCByte[0];
                    _u8IrPreRcmmData[1] = RCByte[1];
                    _u8IrPreRcmmData[2] = RCByte[2];
                    _u8IrPreRcmmData[3] = RCByte[3];

                    RCByte[0] = 0x0000UL;
                    RCByte[1] = 0x0000UL;
                    RCByte[2] = 0x0000UL;
                    RCByte[3] = 0x0000UL;

                    RCMode = 0;
                    RCBitsCnt = 0;
                    bRet = TRUE;
                }

                break;
            }
            case RCMMOEM_LONGID_MODE|KEYBOARD_MODE:
            {
                if((_u8IrPreRcmmData[0] == RCByte[0]) && (_u8IrPreRcmmData[1] == RCByte[1]) && (_u8IrPreRcmmData[2] == RCByte[2]))
                {
                    *pu8Flag = TRUE;
                }

                u16CustomerID = ((RCByte[0] & 0x1FUL) << 4) | ((RCByte[1] & 0xF0UL) >> 4);
                if(u16CustomerID == 0x007DUL)
                {
                    if(_u8PrevKeyCode==61)
                    {
                        *pu8Key = (RCByte[2] & 0x7FUL) + 0x3DUL;
                        if(*pu8Key>=97&&*pu8Key<=99)
                            *pu8Key += 150;
                    }
                    else
                    {
                        *pu8Key = RCByte[2] & 0x7FUL;
                    }
                    //printk("0x%x:,%d\n",*pu8Key,*pu8Key);
                    _u8IrPreRcmmData[0] = RCByte[0];
                    _u8IrPreRcmmData[1] = RCByte[1];
                    _u8IrPreRcmmData[2] = RCByte[2];

                    RCByte[0] = 0x0000UL;
                    RCByte[1] = 0x0000UL;
                    RCByte[2] = 0x0000UL;
                    RCByte[3] = 0x0000UL;

                    RCBitsCnt = 0;
                    bRet = TRUE;
                }


            break;
            }

            case RCMMOEM_LONGID_MODE|MOUSE_MODE:
            break;
            case RCMMOEM_LONGID_MODE|JOYSTICK_MODE:
            break;
            default:
                bRet = FALSE;
                break;
        }

    }

    if(bRet)
    {
        //printk("_u8PrevKeyCode=%d,*pu8Key=%d,_ePrevKeyProperty=%d",_u8PrevKeyCode,*pu8Key,_ePrevKeyProperty);
        if (_u8PrevKeyCode != *pu8Key)
        {
            _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
        }
        if ((_u8PrevKeyCode == (*pu8Key + 61))||(_u8PrevKeyCode == (*pu8Key + 211)))
        {
            _ePrevKeyProperty = E_IR_KEY_PROPERTY_1st;
        }

        i = _MDrv_IR_GetSystemTime();
        if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
        {
            _u8PrevKeyCode     = *pu8Key;
            _ulPrevKeyTime    = i;
            _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
        }
        else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
        {
        //printk("i=%ld,_ulPrevKeyTime=%ld",i,_ulPrevKeyTime);
            if( (i - _ulPrevKeyTime) > 150)
            {
                _ulPrevKeyTime = i;
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
            }
            else
            {
                bRet = FALSE;
            }
        }
        else //E_IR_KEY_PROPERTY_FOLLOWING
        {
            if( (i - _ulPrevKeyTime) > 150)
            {
                _ulPrevKeyTime = i;
            }
            else
            {
                bRet = FALSE;
            }
        }
    }

    return bRet;

#elif(IR_TYPE_SEL == IR_TYPE_SKYWORTH)
	#if ENABLE_PIONEER_IR
	if(_bskyworth_normal_code || _bskyworth_pioneer_code)
    {
        if(_brepeat_flag)
        {
            *pu8Key = _u8PrevKeyCode;
            *pu8Flag = 1;
            _u32IRCount = 0;
            _brepeat_flag=FALSE;
            _bskyworth_normal_code = FALSE;
            _bskyworth_pioneer_code = FALSE;
            _bNextCode = FALSE;
            DEBUG_IR(IR_PRINT("repeat input key is 0x%x\n",*pu8Key));
            return TRUE;
        }

        if (_u32IRCount < (34))  //multiple 8
        {
            return FALSE; //not complete yet
        }

        if (_bskyworth_pioneer_code)
        {
            //Pioneer IR sends two key code at one time when press one key.
            //So we should receive twice.
            if(FALSE == _bNextCode)
            {
                //The value of the first key code must start with "0101 0101" (0xAA).
                if( _u32IRData[2] > IR_LG0_LOB2 && _u32IRData[2] < IR_LG0_UPB2 ) //0
                {
                    //The first key code
                    memcpy(_u32PioneerIRData[0], _u32IRData, _u32IRCount * sizeof(U32) );
                    _bNextCode = TRUE;
                }
                return FALSE;
            }
            else
            {
                //The second key code
                memcpy(_u32PioneerIRData[1], _u32IRData, _u32IRCount * sizeof(U32));
                _bNextCode = FALSE;
            }
        }

        if(_bskyworth_normal_code)
        {
            if(_u32IRData[1] < (IR_OFC_LOB) || _u32IRData[1] > (IR_OFC_UPB))
            {
                _u32IRCount = 0;
                _bskyworth_normal_code = FALSE;
                _bIRStart = FALSE;
                //DEBUG_IR(IR_PRINT("no normal IR header\n"));
                return FALSE;
            }
        }
        else if (_bskyworth_pioneer_code)
        {
            if(_u32IRData[1] < (IR_OFC_LOB2) || _u32IRData[1] > (IR_OFC_UPB2))
            {
                _u32IRCount = 0;
                _bskyworth_pioneer_code = FALSE;
                _bIRStart = FALSE;
                //DEBUG_IR(IR_PRINT("no normal IR header\n"));
                return FALSE;
            }
        }

        #if 0 //DEBUG
        {
            int ii;
            if(_bskyworth_normal_code)
            {
                printk("Skyworth");
            }
            else  if (_bskyworth_pioneer_code)
            {
                printk("Pioneer");
            }
            if(_bskyworth_normal_code)
            {
                printk("==>u32IRData: \n");
                for (ii = 0; ii < _u32IRCount; ii++)
                {
                    printk("%d ", _u32IRData[ii]);
                }
            }
            else  if (_bskyworth_pioneer_code)
            {
                int j;
                for (j = 0; j < 2; j++)
                {
                    printk("==>u32PonieerIRData[%d]: \n", j);
                    for (ii = 0; ii < _u32IRCount; ii++)
                    {
                        printk("%d ", _u32PioneerIRData[j][ii]);
                    }
                    printk("\n");
                }
            }
            printk("\n");
            //_bskyworth_pioneer_code = FALSE;
            //_bskyworth_normal_code = FALSE;
            //_bIRStart = FALSE;
            //return FALSE;
        }
        #endif //DEBUG

        if(_bskyworth_normal_code)
        {
            //IR_PRINT("we enter if(_bskyworth_normal_code)\n");
            for( u8Byte = 0; u8Byte < IR_RAW_DATA_NUM; u8Byte++)
            {
                u8IRSwModeBuf[u8Byte] = 0;
            }
            pu32IRData = &_u32IRData[2]; 
            for(u8Byte = 0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
            {
                for( u8Bit=0; u8Bit<8; u8Bit++)
                {
                    u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
                    //printk("u32IRData[%d] = %d\n",u8Byte*8+u8Bit,u32BitLen);
                    u8IRSwModeBuf[u8Byte] >>= 1;

                    if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
                    {
                        u8IRSwModeBuf[u8Byte] |= 0x00UL;
                    }
                    else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) // 1
                    {
                        u8IRSwModeBuf[u8Byte] |= 0x80UL;
                    }
                    else
                    {
                        //DEBUG_IR(IR_PRINT(" invalid waveform,0x%x\n",u32BitLen));
                        _u32IRCount = 0;
                        _bskyworth_normal_code = FALSE;
                        _bIRStart = FALSE;
                        return FALSE;
                    }
                }

                if((u8IRSwModeBuf[0] != _u8IRHeaderCode0) || (u8Byte==1 && u8IRSwModeBuf[1] != _u8IRHeaderCode1))
                {
                    _u32IRCount = 0;
                    //DEBUG_IR(IR_PRINT("invalid header,0x%x,0x%x\n",u8IRSwModeBuf[0],u8IRSwModeBuf[1]));
                    _bskyworth_normal_code = FALSE;
                    _bIRStart = FALSE;
                    return FALSE;
                }
                else if(u8Byte == 3)
                {
                    if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
                    {
                        *pu8Key = u8IRSwModeBuf[2];
                        *pu8Flag = 0;
                        _u8PrevKeyCode=*pu8Key;
                        _u32IRCount = 0;
                        ulPreTime=_MDrv_IR_GetSystemTime();
                        //DEBUG_IR(IR_PRINT("input key is 0x%x\n",*pu8Key));
                        _bskyworth_normal_code = FALSE;
                        _bIRStart = FALSE;
                        printk("pu8Key = 0x%02x\n",*pu8Key);
                        return TRUE;
                    }
                }
            }//for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
        }
        else if(_bskyworth_pioneer_code) //we have to handle pioneer key
        {
            int j;
            U16 u16DoubleCode;
            U8 u8val = 0;
            U8 u8Key;
            for (j = 0; j < 2; j++)
            {
                for( u8Byte = 0; u8Byte < IR_RAW_DATA_NUM; u8Byte++)
                {
                    u8IRSwModeBuf[u8Byte] = 0;
                }
                pu32IRData = &_u32PioneerIRData[j][2];
                for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
                {
                    for( u8Bit=0; u8Bit<8; u8Bit++)
                    {
                        u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
                        //printk("u32IRData[%d] = %d\n",u8Byte*8+u8Bit,u32BitLen);
                        u8IRSwModeBuf[u8Byte] >>= 1;

                        if( u32BitLen > IR_LG0_LOB2 && u32BitLen < IR_LG0_UPB2 ) //0
                        {
                           u8IRSwModeBuf[u8Byte] |= 0x00UL;
                        }
                        else if (u32BitLen > IR_LG1_LOB2 && u32BitLen < IR_LG1_UPB2) // 1
                        {
                            u8IRSwModeBuf[u8Byte] |= 0x80UL;
                        }
                        else
                        {
                            //DEBUG_IR(IR_PRINT(" invalid waveform,0x%x\n",u32BitLen));
                            _u32IRCount = 0;
                            _bskyworth_pioneer_code = FALSE;
                            _bIRStart = FALSE;
                            return FALSE;
                        }
                    }

                    if(3 == u8Byte)
                    {
                        if((u8IRSwModeBuf[0] == IR_HEADER_CODE3) && (u8IRSwModeBuf[1] == IR_HEADER_CODE4)
                            && (u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3]))
                        {
                            if (0 == j)
                            {
                                _u8PioneerFirstKey = u8IRSwModeBuf[2];
                                //printk("_u8PioneerFirstKey = 0x%x\n", _u8PioneerFirstKey);
                            }
                            else
                            {
                                _u8PioneerSecondKey = u8IRSwModeBuf[2];
                                //printk("_u8PioneerSecondKey = 0x%x\n", _u8PioneerSecondKey);
                            }
                        }
                        else if((u8IRSwModeBuf[0] == IR_HEADER_CODE5) && (u8IRSwModeBuf[1] == IR_HEADER_CODE6)
                                && (u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3]))
                        {
                              _u8PioneerSecondKey = u8IRSwModeBuf[2];
                              //printk("_u8PioneerSecondKey = 0x%x\n", _u8PioneerSecondKey);
                        }
                        else
                        {
                            //DEBUG_IR(IR_PRINT(" invalid waveform,0x%x\n",u32BitLen));
                            _u32IRCount = 0;
                            _bskyworth_pioneer_code = FALSE;
                            _bIRStart = FALSE;
                            return FALSE;
                        }
                    }
                }
            }

            if (_u8PioneerFirstKey == _u8PioneerSecondKey)
            {
                //pioneer single code
                //printk("==>Pioneer single code = 0x%02x\n", _u8PioneerFirstKey);
                u16DoubleCode = 0x00UL << 8 | _u8PioneerFirstKey;
                *pu8Key = _u8PioneerFirstKey;
            }
            else
            {
                if (_u8PioneerFirstKey > 0x50UL)
                {
                    //pioneer double code
                    //printk("==>Pioneer double code = 0x%02x 0x%02x\n", _u8PioneerFirstKey, _u8PioneerSecondKey);
                    u16DoubleCode = _u8PioneerFirstKey << 8 | _u8PioneerSecondKey;
                }
                else
                {
                    //pioneer single code
                    //printk("==>Pioneer single code = 0x%02x\n", _u8PioneerSecondKey);
                    u16DoubleCode = 0x00UL << 8 | _u8PioneerSecondKey;
                }
                *pu8Key = _u8PioneerSecondKey;
            }

            // Convert Poineer key code to Skyworth key code
            while((stPioneerIRMap[u8val].u16IRRawDate != 0) && (stPioneerIRMap[u8val].u8IRKey != IRKEY_NULL))
            {
                if(stPioneerIRMap[u8val].u16IRRawDate == u16DoubleCode)
                {
                    *pu8Key = stPioneerIRMap[u8val].u8IRKey;
                    break;
                }
                else
                {
                    u8val++;
                }
            }
            *pu8Flag = 0;
            _u8PrevKeyCode=*pu8Key;
            _u32IRCount = 0;
            ulPreTime=_MDrv_IR_GetSystemTime();
            //DEBUG_IR(IR_PRINT("input key is 0x%x\n",*pu8Key));
            _bskyworth_pioneer_code = FALSE;
            _bIRStart = FALSE;
            printk("pu8Key = 0x%02x\n",*pu8Key);
            return TRUE;
        }

        _bskyworth_pioneer_code = FALSE;
        _bskyworth_normal_code = FALSE;
        _bIRStart = FALSE;
        _u32IRCount = 0;
    }

	#else
	if(_bskyworth_normal_code || _bskyworth_shuttle_code)
	{
		if(_brepeat_flag)
		{
			*pu8Key = _u8PrevKeyCode;
			*pu8Flag = 1;
			_u32IRCount = 0;
			_brepeat_flag=FALSE;
			_bskyworth_normal_code = FALSE;
			_bskyworth_shuttle_code = FALSE;
			DEBUG_IR(IR_PRINT("repeat input key is 0x%x\n",*pu8Key));
			return TRUE;
		}

		if(_bskyworth_normal_code)
		{
			//printk("enter _MDrv_IR_GetKey\n");
			if (_u32IRCount< (34))  //multiple 8
			{
				//IR_PRINT("we get IRCount error and _u32IRCount = %d\n",_u32IRCount);
				return FALSE; //not complete yet
			}
			if(_u32IRData[1]< (IR_HDC_LOB)||_u32IRData[1] > (IR_HDC_UPB))
			{
				_u32IRCount = 0;
				_bskyworth_normal_code = FALSE;
				_bIRStart = FALSE;
				//DEBUG_IR(IR_PRINT("no normal IR header\n"));
				return FALSE;
			}
			for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
			{
				u8IRSwModeBuf[u8Byte] = 0;
			}
			pu32IRData = &_u32IRData[2];
		}
		else if(_bskyworth_shuttle_code)
		{
			if(_u32IRCount < (10))
			{
				//IR_PRINT("we get IRCount error and _u32IRCount = %d\n",_u32IRCount);
				return FALSE; //not complete yet				
			}
			if(_u32IRData[1]< (IR_OFC_LOB2)||_u32IRData[1] > (IR_OFC_UPB2))
			{
				_u32IRCount = 0;
				//DEBUG_IR(IR_PRINT("no shuttle IR header\n"));
				_bskyworth_shuttle_code = FALSE;
				_bIRStart = FALSE;
				return FALSE;	
			}
			u8IRSwModeByte = 0;
			pu32IRData = &_u32IRData[2];
			//printk("StartSignal = %d\n", _u32IRData[0]);
			//printk("OFCSignal = %d\n",_u32IRData[1]);
		}

		if(_bskyworth_normal_code)
		{
			for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
			{
				for( u8Bit=0; u8Bit<8; u8Bit++)
				{
					u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
					u8IRSwModeBuf[u8Byte] >>= 1;

					if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
					{
						u8IRSwModeBuf[u8Byte] |= 0x00UL;
					}
					else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
					{
						u8IRSwModeBuf[u8Byte] |= 0x80UL;
					}
					else
					{
						//DEBUG_IR(IR_PRINT(" invalid waveform,0x%x\n",u32BitLen));
						_u32IRCount = 0;
						_bskyworth_normal_code = FALSE;
						_bIRStart = FALSE;
						return FALSE;
					}
				}
				if((u8IRSwModeBuf[0]!=_u8IRHeaderCode0)||(u8Byte==1&&u8IRSwModeBuf[1]!=_u8IRHeaderCode1))
				{
					_u32IRCount = 0;
					//DEBUG_IR(IR_PRINT("invalid header,0x%x,0x%x\n",u8IRSwModeBuf[0],u8IRSwModeBuf[1]));
					_bskyworth_normal_code = FALSE;
					_bIRStart = FALSE;
					return FALSE;
				}
				else if(u8Byte==3)
				{
					if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
					{
						*pu8Key = u8IRSwModeBuf[2];
						*pu8Flag = 0;
						_u8PrevKeyCode=*pu8Key;
						_u32IRCount = 0;
						ulPreTime=_MDrv_IR_GetSystemTime();
						//DEBUG_IR(IR_PRINT("input key is 0x%x\n",*pu8Key));
						_bskyworth_normal_code = FALSE;
						_bIRStart = FALSE;
						return TRUE;
					}
				}
			}//for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
		}
		else if(_bskyworth_shuttle_code) //we have to handle shuttle key
		{
			for( u8Bit=0; u8Bit<8; u8Bit++)
			{
				u32 u32BitLen = pu32IRData[u8Bit];
				u8IRSwModeByte >>= 1;
				
				if( u32BitLen > IR_LG0_LOB2 && u32BitLen < IR_LG0_UPB2 ) //0
				{
					u8IRSwModeByte |= 0x00UL;
				}
				else if (u32BitLen > IR_LG1_LOB2 && u32BitLen < IR_LG1_UPB2) //1
				{
					u8IRSwModeByte |= 0x80UL;
				}
				else
				{
					//DEBUG_IR(IR_PRINT(" invalid waveform,0x%x\n",u32BitLen));
					_u32IRCount = 0;
					_bskyworth_shuttle_code = FALSE;
					_bIRStart = FALSE;
					//printk("u32BitLen[%d] = error\n",u8Bit);
					return FALSE;
				}
			}//for( u8Bit=0; u8Bit<8; u8Bit++)	
			
			//printk("u8IRSwModeByte = %d\n",u8IRSwModeByte);
			if((u8IRSwModeByte&0x03UL) != 0x01UL ) // Customer_Code(01)
			{
				_u32IRCount = 0;
				_bskyworth_shuttle_code = FALSE;
				_bIRStart = FALSE;
				//DEBUG_IR(IR_PRINT("invalid header\n",));
				printk("invalid header\n");
				return FALSE;
			}

			if(((u8IRSwModeByte>>2)&0x07UL)+((u8IRSwModeByte>>5)&0x07UL) == 0x07UL)
			{
				if(0x01UL == ((u8IRSwModeByte>>2)&0x07UL))
				{
					*pu8Key = 0x15UL;
				}
				else
				{
					*pu8Key = 0x14UL;
				}
				*pu8Flag = 0;
				_u8PrevKeyCode=*pu8Key;
				_u32IRCount = 0;
				ulPreTime=_MDrv_IR_GetSystemTime();
				//DEBUG_IR(IR_PRINT("input key is 0x%x\n",*pu8Key));
				_bskyworth_shuttle_code = FALSE;
				_bIRStart = FALSE;
				//printk("pu8Key = %d\n", *pu8Key);
				return TRUE;
			}
		}	
		_bskyworth_shuttle_code = FALSE;
		_bskyworth_normal_code = FALSE;
		_bIRStart = FALSE;
	}
	#endif
	return bRet;

#elif(IR_TYPE_SEL == IR_TYPE_TCL)
    unsigned long i;
    u32 u32IRDataTmp = 0;

    if (_u32IRCount < 2+3*8) {
        return FALSE; //not complete yet
    }
    for (u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++) {
	    u8IRSwModeBuf[u8Byte] = 0;
    }

    if (_u32IRData[1] > IR_HDC_LOB && _u32IRData[1] < IR_HDC_UPB) {
        pu32IRData = &_u32IRData[2];
	} else {
        IR_PRINT("Invalid leader code\n");
        bRet = FALSE;
        goto done;
    }
    for (u8Bit=0; u8Bit<24; u8Bit++) {
        u32 u32BitLen = pu32IRData[u8Bit];
        u32IRDataTmp <<= 1;
        if (u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) {//0
            u32IRDataTmp |= 0x00UL;
        } else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) {//1
            u32IRDataTmp |= 0x01UL;
        } else {
            IR_PRINT("Invalid waveform,0x%x\n",u32BitLen);
            bRet = FALSE;
            goto done;
        }
    }

    u8IRSwModeBuf[0] = ((u32IRDataTmp & 0x00F00000UL) >> 20);
    u8IRSwModeBuf[1] = ((u32IRDataTmp & 0x000FF000UL) >> 12);
    u8IRSwModeBuf[2] = ((u32IRDataTmp & 0x00000F00UL) >> 8);
    u8IRSwModeBuf[3] = (u32IRDataTmp & 0x000000FFUL);
    if (u8IRSwModeBuf[0] == (((u8)~u8IRSwModeBuf[2]) & 0x0FUL)) {
        if (u8IRSwModeBuf[1] == (u8)~u8IRSwModeBuf[3]) {
            if ((_brepeat_flag == TRUE) && (u8IRSwModeBuf[1] == _u8PrevKeyCode)) {
                *pu8Key = _u8PrevKeyCode;
                *pu8Flag = 0x01UL;//(((u8IRSwModeBuf[0]<<4) & 0xF0)|0x01);
                *pu8System = (u8IRSwModeBuf[2]<<4)|u8IRSwModeBuf[0];
                _brepeat_flag = FALSE;
                bRet = TRUE;
                goto done;
            } else {
                *pu8Key = u8IRSwModeBuf[1];
                *pu8Flag = 0;//((u8IRSwModeBuf[0]<<4) & 0xF0);
                _u8PrevKeyCode=*pu8Key;
                *pu8System = (u8IRSwModeBuf[2]<<4)|u8IRSwModeBuf[0];
                bRet = TRUE;
                DIR_PRINT("########key is 0x%x\n", *pu8Key);
                goto done;
            }
        }
    }
    IR_PRINT("Invalid data\n");
    bRet = FALSE;

    done:
#ifndef CONFIG_MSTAR_IR_INPUT_DEVICE
    if (bRet) {
        if ((_u8PrevKeyCode != *pu8Key) || (!*pu8Flag)) {
            _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
        }

        i = _MDrv_IR_GetSystemTime();
        if (_ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT) {
            _u8PrevKeyCode = *pu8Key;
            _ulPrevKeyTime	= i;
            _ulPrevKeyTimeFirstPress = i;
            _ePrevKeyProperty = E_IR_KEY_PROPERTY_1st;
            *pu8Flag = 0;
        } else if (_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st) {
            if ((i - _ulPrevKeyTime) > 500) {//2*_u32_1stDelayTimeMs
                _ulPrevKeyTime = i;
                _ePrevKeyProperty = E_IR_KEY_PROPERTY_FOLLOWING;
                *pu8Flag = 0x01UL;
            } else {
                bRet = FALSE;
            }
        } else {
            if ((i - _ulPrevKeyTime) > 80) {//2*_u32_2ndDelayTimeMs
                _ulPrevKeyTime = i;
                *pu8Flag = 0x01UL;
            } else {
                bRet = FALSE;
            }

            if (_u8PrevKeyCode == 0x0BUL) {//OK key press
                if ((i-_ulPrevKeyTimeFirstPress) > 1500) {
                    _ulPrevKeyTimeFirstPress = i;
                    *pu8Key = 0x9AUL;
                    *pu8Flag = 0x00UL; //clear repeat flag
                    bRet = TRUE; //send long press key
                }
            }
        }
    }
#endif

    _u32IRCount = 0;
    return bRet;

 #elif(IR_TYPE_SEL == IR_TYPE_HAIER)
    //U8 u8tmp;

    if(_btoshiba_code)
    {
        if(_brepeat_flag)
        {

				if(_bBeginRepeat_flag)
				{
					_Repeat_Num++;
					if(_Repeat_Num == 1)
					{
						return FALSE;//drop first repeate for ir too smart
					}
		            *pu8Key = _u8PrevKeyCode;
		            *pu8Flag = 1;
		            _u32IRCount = 0;
		            _btoshiba_code=FALSE;
		            _brepeat_flag=FALSE;
					//printk("repeat input key is 0x%x\n",*pu8Key);
		            return TRUE;
				}
				else
				{
		            _u32IRCount = 0;
		            _btoshiba_code=FALSE;
		            _brepeat_flag=FALSE;
					printk("Bad Repeat Receive\r\n");
		            return FALSE;
				}
        }

        if (_u32IRCount< (34))  //multiple 8
            return FALSE; //not complete yet
        else
        {
            for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
            {
               u8IRSwModeBuf[u8Byte] = 0;
            }
            pu32IRData = &_u32IRData[2];

            for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
            {
                for( u8Bit=0; u8Bit<8; u8Bit++)
                {
                    u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
                    u8IRSwModeBuf[u8Byte] >>= 1;
                    if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
                    {
                        u8IRSwModeBuf[u8Byte] |= 0x00UL;
                    }
                    else // if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
                    {
                        u8IRSwModeBuf[u8Byte] |= 0x80UL;
                    }
                    /*else
                    {
                        DEBUG_IR(printk(" invalid waveform,0x%x\n",u32BitLen));
                        for(u8tmp=0;u8tmp<_u32IRCount;u8tmp++)
                            printk("%d data=%d\n",u8tmp,_u32IRData[u8tmp]);
                        _u32IRCount = 0;
                        btoshiba_code=FALSE;
                        return FALSE;
                    }*/
                }
				
                if((u8IRSwModeBuf[0]!=_u8IRHeaderCode0)||(u8Byte==1&&u8IRSwModeBuf[1]!=_u8IRHeaderCode1))
                {
                    U8 u8tmp;
                    for(u8tmp=0;u8tmp<_u32IRCount;u8tmp++)
                            printk("%d data=%d\n",u8tmp,_u32IRData[u8tmp]);
                    _u32IRCount = 0;
                    _btoshiba_code=FALSE;
                    DEBUG_IR(printk("invalid header\n"));
                    return FALSE;
                }
                else if(u8Byte==3)
                {
                    if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
                    {
                        *pu8Key = u8IRSwModeBuf[2];
                        *pu8Flag = 0;
                        _u8PrevKeyCode=*pu8Key;
                        _u32IRCount = 0;
                        _btoshiba_code=FALSE;
						_bBeginRepeat_flag = 1;
						_Repeat_Num = 0;
                        DEBUG_IR(printk("input key is 0x%x\n",*pu8Key));
                        //printk("input key is 0x%x\n",*pu8Key);
                        return TRUE;
                    }
                }
            }//for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
        }//else
    }//toshiba code
    else
    {
        if(_u32IRCount< (18))  //multiple 4
            return FALSE; //not complete yet
        else
        {
            for( u8Byte=0; u8Byte<2; u8Byte++)
            {
               u8IRSwModeBuf[u8Byte] = 0;
            }
            pu32IRData = &_u32IRData[2];
            for( u8Byte=0; u8Byte<2; u8Byte++)
            {
                for( u8Bit=0; u8Bit<8; u8Bit++)
                {
                    u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
                    u8IRSwModeBuf[u8Byte] >>= 1;

                    if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
                    {
                        u8IRSwModeBuf[u8Byte] |= 0x00UL;
                    }
                    else // if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
                    {
                        u8IRSwModeBuf[u8Byte] |= 0x80UL;
                    }
                    /*else
                    {
                        DEBUG_IR(printk(" invalid waveform,0x%x\n",u32BitLen));
                        bRet = FALSE;
                    }*/
                }
            }//for( u8Byte=0; u8Byte<2; u8Byte++)

            if(((u8IRSwModeBuf[0]&0xf0UL)>>4)==_u8FactoryIRHeaderCode0)
            {
                u8IRSwModeBuf[2]=u8IRSwModeBuf[0]&0x0fUL;
                u8IRSwModeBuf[3]=u8IRSwModeBuf[1]&0x0fUL;
                if((u8IRSwModeBuf[2]+u8IRSwModeBuf[3])==0x0fUL)
                {
                    switch(u8IRSwModeBuf[2])
                    {
                        case IRKEY_DESIL1:
                        case IRKEY_DESIL2:
                        case IRKEY_DESIL3:
                        case IRKEY_DESIL4:
                        case IRKEY_DESIL5:
                        case IRKEY_DESIL6:
                            *pu8Key=IRKEY_SLIDE_L1+(u8IRSwModeBuf[2]-IRKEY_DESIL1);
                             break;
                        case IRKEY_REVERSE1:
                        case IRKEY_REVERSE2:
                        case IRKEY_REVERSE3:
                        case IRKEY_REVERSE4:
                        case IRKEY_REVERSE5:
                        case IRKEY_REVERSE6:
                            *pu8Key=IRKEY_SLIDE_R1+(u8IRSwModeBuf[2]-IRKEY_REVERSE1);
                            break;
                    }
                    _u8PrevKeyCode=*pu8Key;
                    DEBUG_IR(printk("slide key 0x%x\n",*pu8Key));
                    printk("slide key 0x%x\n",*pu8Key);
                    *pu8Flag = 0;
                    bRet = TRUE;
                }
            }
            _u32IRCount = 0;
            _btoshiba_code=FALSE;
            return bRet;
        }
    }
    return bRet;

#elif (IR_TYPE_SEL == IR_TYPE_CUS08_RC5)

        if(_u8IrRc5Bits > IR_RC5_DATA_BITS)
        {
            if ((_u16IrRc5Data&0xF000UL)==0x3000UL)
            {
                if(_u16PreIrRc5Data==_u16IrRc5Data)
                    *pu8Flag = TRUE;

                *pu8Key = _u16IrRc5Data&0x3FUL;
                *pu8System = (_u16IrRc5Data&0x3C0UL)>>6;
                _u16PreIrRc5Data=_u16IrRc5Data;
                _u8IrRc5Bits=0;
                bRet = TRUE;
            }
        }

        if(bRet)
        {
            if ( (_u8PrevKeyCode != *pu8Key) || (!*pu8Flag) )
            {
                _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
            }

            i = _MDrv_IR_GetSystemTime();
            if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                _u8PrevKeyCode     = *pu8Key;
                _ulPrevKeyTime    = i;
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
            {
                if( (i - _ulPrevKeyTime) > _u32_1stDelayTimeMs)
                {
                    _ulPrevKeyTime = i;
                    _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
                }
                else
                {
                    bRet = FALSE;
                }
            }
            else //E_IR_KEY_PROPERTY_FOLLOWING
            {
                if( (i - _ulPrevKeyTime) > _u32_2ndDelayTimeMs)
                {
                    _ulPrevKeyTime = i;
                }
                else
                {
                    bRet = FALSE;
                }
            }
        }

        return bRet;

#elif (IR_TYPE_SEL == IR_TYPE_CUS21SH)

    #if (1)
    bRet = FALSE;

    if(_u32IRCount == 15)
    {
        if(_MDrv_SH_IR_GetKey(pu8Key, pu8System, pu8Flag))
        {
            bRet = TRUE;
        }
        _u32IRCount = 0;
        _u16IRKeyMap = 0;
    }
    #else
    //printk("Linux: %s:%s:%d \n",__FILE__,__FUNCTION__,__LINE__);
    bRet = FALSE;

    for( u8Byte=0; u8Byte<16/*IR_RAW_DATA_NUM*/; u8Byte++)
    {
       u8IRSwModeBuf[u8Byte] = 0;
    }

    if (_u32IRCount< 32)//3+IR_RAW_DATA_NUM*8)
        return FALSE; //not complete yet

    DEBUG_IR(printk("_u32IRCount=%d", _u32IRCount));
    for( u8Byte=0; u8Byte<_u32IRCount; u8Byte++)
    {
       DEBUG_IR(printk(" 0x%x", _u32IRData[u8Byte]));
    }
    printk("\n");
    printk("_u16IRKeyMap:0x%x \n",_u16IRKeyMap);

    if(_bKeyValueHit == FALSE)
    {
        *pu8Key = _MDrv_IR_CUS21SH_ParseKey(_u16IRKeyMap);    // translate for SH

        printk("_u16IRKeyMap:0x%x ==> *pu8Key:%x\n",_u16IRKeyMap,*pu8Key);
        //TODO: Implement repeat code later.
        *pu8Flag = 0;
        //_bKeyValueHit = 1;
        bRet = TRUE;
    }
    #endif

    return bRet;

#elif (IR_TYPE_SEL == IR_TYPE_TOSHIBA)

    bRet = FALSE;

    if(_MDrv_IR_CUS22T_GetKey(pu8Key, pu8System, pu8Flag))
    {
        bRet = TRUE;
    }

    return bRet;

#else
    for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
    {
       u8IRSwModeBuf[u8Byte] = 0;
    }
    #if (IR_TYPE_SEL == IR_TYPE_HISENSE || IR_TYPE_SEL == IR_TYPE_MSTAR_DTV || IR_TYPE_SEL == IR_TYPE_CHANGHONG)
    if(_u8IRRepeated)//repeate key
    {
        _u8IRRepeatedNum++;
        if (_u8IRRepeatedNum < 5)
        {
            return FALSE;
        }
        _u8IRRepeated = 0;
        _u8IRHeadReceived = 0;//clear head receive flag
        *pu8Key = _u8PrevKeyCode;
        *pu8Flag = 1;
        bRet = TRUE;
        #if (IR_TYPE_SEL == IR_TYPE_HISENSE)
        *pu8System = _u8PrevSystemCode;//timl
	if(_u8IRType == FCTORY_HEADER_RECEIVE)
            *pu8Flag |= FCTORY_HEADER_RECEIVE;
        else if(_u8IRType == PCCOMMAND_HEADER_RECEIVE)
            *pu8Flag |= PCCOMMAND_HEADER_RECEIVE;
        #endif

        goto done;
    }
    if (_u32IRCount<(2+IR_RAW_DATA_NUM*8))
        return FALSE; //not complete yet
    //#elif( IR_TYPE_SEL == IR_TYPE_CHANGHONG)
    //if (_u32IRCount<(2+IR_RAW_DATA_NUM*8))
        //return FALSE; //not complete yet
    #else
    if (_u32IRCount< 3+IR_RAW_DATA_NUM*8)
        return FALSE; //not complete yet
    #endif
    DEBUG_IR(printk("_u32IRCount=%d", _u32IRCount));
    for( u8Byte=0; u8Byte<_u32IRCount; u8Byte++)
    {
       DEBUG_IR(printk(" %d", _u32IRData[u8Byte]));
    }

    if( _u32IRData[0] > IR_HDC_LOB && _u32IRData[1] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[1] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
        pu32IRData = &_u32IRData[2];
        DEBUG_IR(printk(" H1 "));
    }
    else if( _u32IRData[1] > IR_HDC_LOB && _u32IRData[2] > IR_OFC_LOB+IR_LG01H_LOB && _u32IRData[2] < REG_IR_OFC_UPB+IR_LG01H_UPB )
    {
        pu32IRData = &_u32IRData[3];
        DEBUG_IR(printk(" H2 "));
    }
    else
    {
        DEBUG_IR(printk(" invalid leader code\n"));
        bRet = FALSE;
        goto done;
    }

    for( u8Byte=0; u8Byte<IR_RAW_DATA_NUM; u8Byte++)
    {
        for( u8Bit=0; u8Bit<8; u8Bit++)
        {
            u32 u32BitLen = pu32IRData[u8Byte*8+u8Bit];
            u8IRSwModeBuf[u8Byte] >>= 1;

            if( u32BitLen > IR_LG0_LOB && u32BitLen < IR_LG0_UPB ) //0
            {
                u8IRSwModeBuf[u8Byte] |= 0x00UL;
            }
            else if (u32BitLen > IR_LG1_LOB && u32BitLen < IR_LG1_UPB) //1
            {
                u8IRSwModeBuf[u8Byte] |= 0x80UL;
            }
            else
            {
                DEBUG_IR(printk(" invalid waveform,0x%x\n",u32BitLen));
                bRet = FALSE;
                goto done;
            }
        }
    }

    #if (IR_TYPE_SEL == IR_TYPE_HISENSE)
    *pu8System = u8IRSwModeBuf[1];//timl
    #endif
    if(u8IRSwModeBuf[0] == _u8IRHeaderCode0)
    {
        if(u8IRSwModeBuf[1] == _u8IRHeaderCode1)
        {
            if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
            {
                *pu8Key = u8IRSwModeBuf[2];
                *pu8Flag = 0;
                bRet = TRUE;
                #if (IR_TYPE_SEL == IR_TYPE_HISENSE || IR_TYPE_SEL == IR_TYPE_MSTAR_DTV || IR_TYPE_SEL == IR_TYPE_CHANGHONG)
                _u8PrevKeyCode = *pu8Key;
                _u8PrevSystemCode = *pu8System;
                _u8IRRepeateDetect = 1;
                _u8IRHeadReceived = 0;
                _u8IRType = 0;
                #endif

                goto done;
            }
        }
    }
    if(u8IRSwModeBuf[0] == _u8IR2HeaderCode0)
    {
        if(u8IRSwModeBuf[1] == _u8IR2HeaderCode1)
        {
            if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
            {
                *pu8Key = u8IRSwModeBuf[2];
                *pu8Flag = 0;
                bRet = TRUE;
                #if (IR_TYPE_SEL == IR_TYPE_HISENSE || IR_TYPE_SEL == IR_TYPE_MSTAR_DTV || IR_TYPE_SEL == IR_TYPE_CHANGHONG)
                _u8PrevKeyCode = *pu8Key;
                _u8PrevSystemCode = *pu8System;
                _u8IRRepeateDetect = 1;
                _u8IRHeadReceived = 0;
                _u8IRType = 0;
                #endif

                goto done;
            }
        }
    }
    #if (IR_TYPE_SEL == IR_TYPE_HISENSE)
    if(u8IRSwModeBuf[0] == _u8FactoryIRHeaderCode0)
    {
        if(u8IRSwModeBuf[1] == _u8FactoryIRHeaderCode1)
        {
            if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
            {
                *pu8Key = u8IRSwModeBuf[2];
                *pu8Flag = 0;
                *pu8Flag |= FCTORY_HEADER_RECEIVE;
                bRet = TRUE;
                _u8PrevKeyCode = *pu8Key;
                _u8PrevSystemCode = *pu8System;
                _u8IRRepeateDetect = 1;
                _u8IRHeadReceived = 0;
                _u8IRType = FCTORY_HEADER_RECEIVE;

                goto done;
            }
        }
    }

    if(u8IRSwModeBuf[0] == _u8PCCommandHeaderCode0)
    {
        if(u8IRSwModeBuf[1] == _u8PCCommandHeaderCode1)
        {
            if(u8IRSwModeBuf[2] == (u8)~u8IRSwModeBuf[3])
            {
                *pu8Key = u8IRSwModeBuf[2];
                *pu8Flag = 0;
                *pu8Flag |= PCCOMMAND_HEADER_RECEIVE;
                bRet = TRUE;
                _u8PrevKeyCode = *pu8Key;
                _u8PrevSystemCode = *pu8System;
                _u8IRRepeateDetect = 1;
                _u8IRHeadReceived = 0;
                _u8IRType = PCCOMMAND_HEADER_RECEIVE;

                goto done;
            }
        }
    }
    #endif

    DEBUG_IR(printk(" invalid data\n"));
    bRet = FALSE;

done:
    _u32IRCount = 0;
    return bRet;
#endif
}

#elif (IR_MODE_SEL == IR_TYPE_SWDECODE_KON_MODE)
//-------------------------------------------------------------------------------------------------
/// Get IR key.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
    U8 u8Temp;

    *pu8System = 0;

    if(g_u8IrKonFlag & IR_KON_DETECT_END)
    {
        printk("\nKey:%x",g_u16IrKonDecode);
        if (((g_u16IrKonDecode >> 8) & 0xFFUL) != _u8IRHeaderCode0)
        {
            g_u8IrKonFlag &= (~IR_KON_DETECT_END);
            printk("\nInvalid HC!");
            return FALSE;
        }

        u8Temp = (U8)(g_u16IrKonDecode & 0x00FFUL);
        *pu8Key = u8Temp;

        if ( (g_u8IrKonFlag & IR_KON_REPEATE_TIMEOUT) || (g_u8IrKonPreKey != u8Temp)
			|| (*pu8Key==70) || (*pu8Key==69))
        {
            *pu8Flag = 0;
            g_u8IrKonPreKey = u8Temp;
        }
        else
        {
            *pu8Flag = 1;
        }
//        printk("\n0x%x(%d)\n", (U16)(*pu8Key), (U16)(*pu8Flag));
        g_u8IrKonFlag &= (~IR_KON_DETECT_END);

        if (*pu8Flag == 0)
        {
            g_u8IrKonRepeatCount = 0;
            return TRUE;
        }
        else
        {
            g_u8IrKonRepeatCount++;
//#if 0	//modified by caomeng 20090713 for power key sensitivity
            if (g_u8IrKonRepeatCount >= 10)//IR_FILTER_REPEAT_NUM)
                return TRUE;
//#else
//            if ((u8Temp == IRKEY_POWER ||u8Temp == IRKEY_POWER_KEYPAD)&&
//			(g_u8IrKonRepeatCount >= 20))	//IR_FILTER_REPEAT_NUM
//                return MSRET_OK;
//	     else if(g_u8IrKonRepeatCount >= 10)
//                return MSRET_OK;
//#endif
        }
    }
    return FALSE;
}
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Initialize IR timing and enable IR.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_IR_Init(int bResumeInit)
{
    int result;

    ir_irq_depth = 1;
    _u8PrevSystemCode = 0;

#ifdef SUPPORT_MULTI_PROTOCOL
    _MDrv_MultiProtocol_Init();
#endif

#if (IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
    _u8IRRawModeCount = 0;
#endif
    #if(IR_MODE_SEL == IR_TYPE_SWDECODE_KON_MODE)
    g_u8IrKonBits = 0;
    g_u8IrKonFlag  = 0;
    g_u8IrKonRepeatCount = 10;
    g_u8IrKonRepeatTimeout = 0;
    g_u8IrKonPreKey = 0xFFUL;
    #endif
    if(!bResumeInit)
    {
    sema_init(&fantasy_protocol.sem, 1);
    init_waitqueue_head(&fantasy_protocol.inq);
    }
    fantasy_protocol.data=0;


#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
#if(IR_TYPE_SEL != IR_TYPE_RCMM)
    _u32IRCount = 0;
#endif
#if (IR_TYPE_SEL == IR_TYPE_HAIER)
	_bBeginRepeat_flag = 0;
	_Repeat_Num = 0;

#endif

#endif


    if(IR_MODE_SEL == IR_TYPE_RAWDATA_MODE)
    {
        REG(REG_IR_CTRL) = IR_TIMEOUT_CHK_EN |
                                   IR_INV            |
                                   IR_RPCODE_EN      |
                                   IR_LG01H_CHK_EN   |
                                   IR_LDCCHK_EN      |
                                   IR_EN;
    }
	else if(IR_MODE_SEL == IR_TYPE_HWRC_MODE)
	{
	    REG(REG_IR_CTRL) = IR_INV;
#if (defined(IR_TYPE_HWRC5) && IR_TYPE_SEL==IR_TYPE_HWRC5)
      REG(REG_IR_RC_CTRL) = IR_RC_EN;
#endif     
	}
    else
    {
        REG(REG_IR_CTRL) = IR_TIMEOUT_CHK_EN |
                                   IR_INV            |
                                   IR_RPCODE_EN      |
                                   IR_LG01H_CHK_EN   |
                                   IR_DCODE_PCHK_EN  |
                                   IR_CCODE_CHK_EN   |
                                   IR_LDCCHK_EN      |
                                   IR_EN;
    }

    _MDrv_IR_Timing();
	if(IR_MODE_SEL != IR_TYPE_HWRC_MODE)
	{
      REG(REG_IR_CCODE) = ((u16)_u8IRHeaderCode1<<8) | _u8IRHeaderCode0;
      REG(REG_IR_GLHRM_NUM) = 0x804UL;
	}

    REG(REG_IR_SEPR_BIT_FIFO_CTRL) = 0xF00UL;//[10:8]: FIFO depth, [11]:Enable FIFO full
#if (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE)
    REG(REG_IR_GLHRM_NUM) |= (0x3UL <<12);
    REG(REG_IR_FIFO_RD_PULSE) |= 0x0020UL; //wakeup key sel
#elif (IR_MODE_SEL==IR_TYPE_RAWDATA_MODE)
    REG(REG_IR_GLHRM_NUM) |= (0x2UL <<12);
    REG(REG_IR_FIFO_RD_PULSE) |= 0x0020UL; //wakeup key sel
#elif (IR_MODE_SEL == IR_TYPE_HWRC_MODE)
    //wakeup key sel
    REG(REG_IR_RC_COMP_KEY1_KEY2) = 0xffffUL;
    REG(REG_IR_RC_CMP_RCKEY) = IR_RC_POWER_WAKEUP_EN + IR_RC_POWER_WAKEUP_KEY;   
#else
    REG(REG_IR_GLHRM_NUM) |= (0x1UL <<12);
#if(IR_TYPE_SEL == IR_TYPE_RCMM)

    REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x1 <<12;

#else
	#ifdef IR_INT_NP_EDGE_TRIG	//for N/P edge trigger
    	REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x3UL <<12;
    #else
    	REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= 0x2UL <<12;
	#endif
#endif
#endif


#if((IR_MODE_SEL==IR_TYPE_RAWDATA_MODE) || (IR_MODE_SEL==IR_TYPE_FULLDECODE_MODE) || (IR_MODE_SEL == IR_TYPE_HWRC_MODE))
    // Empty the FIFO
    _MDrv_IR_ClearFIFO();
#endif

#if (IR_SWFIFO_DECODE_MODE==ENABLE) 
	 REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= (0x1UL <<14);
	 REG(REG_IR_CKDIV_NUM_KEY_DATA) = 0x00CFUL ;
	 REG(REG_IR_SEPR_BIT_FIFO_CTRL) |= (0x1UL <<6);
	 
	_MDrv_IR_ClearFIFO();
#endif




    _u32_1stDelayTimeMs = 0;
    _u32_2ndDelayTimeMs = 0;
    _ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;

    memset(&_KeyReceived, 0 , sizeof(_KeyReceived) );
    if(!bResumeInit)
    {
    #ifdef CONFIG_MSTAR_PM_SWIR
    //result = request_irq(E_FIQ_IR, MDrv_SWIR_PM51_ISR, SA_INTERRUPT, "IR", NULL); //Doyle change ISR
    #elif (IR_MODE_SEL == IR_TYPE_HWRC_MODE)
	result = request_irq(E_FIQEXPL_IR_INT_RC, _MDrv_IR_RC_ISR, SA_INTERRUPT, "IR_RC", NULL);
	#else
    result = request_irq(INT_NUM_IR_ALL, _MDrv_IR_ISR, SA_INTERRUPT, "IR", NULL);
    #endif

    ir_irq_depth = 0;
    if (result)
    {
        IR_PRINT("IR IRQ registartion ERROR\n");
	}
	else
    {
        IR_PRINT("IR IRQ registartion OK\n");
    }
}

#ifdef SUPPORT_MULTI_PROTOCOL
    memset(_eDetectList, 0, sizeof(_eDetectList));
    _eDetectList[0] = E_IR_PROTOCOL_NEC; //default used protocol
#endif

    #if (defined(CONFIG_MSTAR_TITANIA)||defined(CONFIG_MSTAR_TITANIA2))

    #else
    // unmask IR IRQ on PM
    #ifdef CONFIG_MSTAR_PM_SWIR //to -do
    REG(REG_IRQ_MASK_IR) |= 0x800UL;
    #else
    REG(REG_IRQ_MASK_IR) &= IRQ_UNMASK_IR;
    #endif
    #endif
    //enable_irq(E_FIQ_IR);

    #ifdef CONFIG_MSTAR_PM_SWIR
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;

extern int MDrv_MBX_NotifyMsgRecCbFunc(void *);

    if( E_MBX_SUCCESS != MDrv_MBX_Init(SWIR_FILEOPS, E_MBX_CPU_MIPS, E_MBX_ROLE_HK, IR_MBX_TIMEOUT))
	{
        swir_pm_status = -1;
	}
	else
	{
	    MDrv_MBX_Enable(SWIR_FILEOPS, TRUE);
        swir_pm_status = 1;

        MbxResult = MDrv_MBX_RegisterMSG(SWIR_FILEOPS, E_MBX_CLASS_IRKEY_NOWAIT, IR_MBX_QUEUESIZE);

        MDrv_MBX_NotifyMsgRecCbFunc(MDrv_PMSWIR_ReceiveMsg);



        //send msg to PM to unlock interrupt mask
        MBX_Msg MB_Command;
        MS_U8 password[] = {'M', 'S', 'T', 'A', 'R'};
        int i;

        memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
        MB_Command.eRoleID = E_MBX_ROLE_PM;
        MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        MB_Command.u8Ctrl = 0;
        MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
        MB_Command.u8Index = E_IR_CPUTo51_CMD_SWIR_INT;
        MB_Command.u8ParameterCount = sizeof(password);
        for(i = 0; i < sizeof(password); i++)
            MB_Command.u8Parameters[i] = password[i];
        MbxResult = MDrv_MBX_SendMsg(SWIR_FILEOPS, &MB_Command);
	}
    #endif
}
EXPORT_SYMBOL(MDrv_IR_Init);

//-------------------------------------------------------------------------------------------------
/// Set the initialized control parameters for IrDa at BOOT stage.
/// @param  pIRInitCfg \b IN: carry with initialized control parameters.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_InitCfg(MS_IR_InitCfg* pIRInitCfg)
{
    if (pIRInitCfg->u8DecMode != IR_DECMODE_EXT)
    {
        return FALSE;
    }

    _u8IRHeaderCode0 = pIRInitCfg->u8HdrCode0;
    _u8IRHeaderCode1 = pIRInitCfg->u8HdrCode1;

    if (pIRInitCfg->u8ExtFormat == IR_XFM_DUALRC)
    {
        _u8FactoryIRHeaderCode0 = pIRInitCfg->u8Ctrl0;
        _u8FactoryIRHeaderCode1 = pIRInitCfg->u8Ctrl1;
    }
    REG(REG_IR_CCODE) = ((u16)_u8IRHeaderCode1<<8) | _u8IRHeaderCode0;

    return TRUE;

}
EXPORT_SYMBOL(MDrv_IR_InitCfg);

//-------------------------------------------------------------------------------------------------
/// Set which protocol will be used in multiple protocol mode.
/// @param  pstProtocolCfg \b IN: carry with used protocol parameter.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_SetProtocol(MS_MultiProtocolCfg *pstProtocolCfg)
{
#ifdef SUPPORT_MULTI_PROTOCOL
    U8 u8Count=0;

    if(pstProtocolCfg==NULL)
    {
        return FALSE;
    }

    memset(_eDetectList, 0, sizeof(_eDetectList));
    _eDetectList[u8Count]=pstProtocolCfg->eProtocol;
    u8Count++;
    while(pstProtocolCfg->pNextProtCfg!=NULL && u8Count<PROTOCOL_SUPPORT_MAX)
    {
        pstProtocolCfg = (MS_MultiProtocolCfg *)pstProtocolCfg->pNextProtCfg;
        _eDetectList[u8Count]=pstProtocolCfg->eProtocol;
        u8Count++;
    }

    return TRUE;
#else
    printk("Multiple protocol not support");
    return FALSE;
#endif
}
EXPORT_SYMBOL(MDrv_IR_SetProtocol);

//-------------------------------------------------------------------------------------------------
/// Set the initialized time parameters for IrDa at BOOT stage.
/// @param  pIRTimeCfg \b IN: carry with initialized time parameter.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_TimeCfg(MS_IR_TimeCfg* pIRTimeCfg)
{
    pIRTimeCfg = pIRTimeCfg;
    return TRUE;

}
EXPORT_SYMBOL(MDrv_IR_TimeCfg);

//-------------------------------------------------------------------------------------------------
/// Get the shot count information from ping-pong/ring buffer
/// @param  pstShotInfo \b OUT: Get the read index,length and shot count sequence.
/// @return TRUE/FALSE
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_ReadShotBuffer(MS_IR_ShotInfo* pstShotInfo)//--@@@--IR Pulse Shot Mode
{
    pstShotInfo = pstShotInfo;
    return TRUE;
}
EXPORT_SYMBOL(MDrv_IR_ReadShotBuffer);

//-------------------------------------------------------------------------------------------------
/// Set the IR delay time to recognize a valid key.
/// @param  u32_1stDelayTimeMs \b IN: Set the delay time to get the 1st key.
/// @param  u32_2ndDelayTimeMs \b IN: Set the delay time to get the following keys.
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_IR_SetDelayTime(u32 u32_1stDelayTimeMs, u32 u32_2ndDelayTimeMs)
{
//TBD
    //u32 u32OldIntr;

    //u32OldIntr = MsOS_DisableAllInterrupts();

    _u32_1stDelayTimeMs = u32_1stDelayTimeMs;
    _u32_2ndDelayTimeMs = u32_2ndDelayTimeMs;

    //MsOS_RestoreAllInterrupts(u32OldIntr);
}
EXPORT_SYMBOL(MDrv_IR_SetDelayTime);

//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key.
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_GetKey(U8 *pu8Key, U8 *pu8System, U8 *pu8Flag)
{
#ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE
    *pu8Key = 0xFF;
    *pu8System = 0;
    *pu8Flag = 0;

    return(FALSE);
#else

    if (_KeyReceived.u8Valid)
    {
        *pu8Key = _KeyReceived.u8Key;
        *pu8System = _KeyReceived.u8System;
        *pu8Flag = _KeyReceived.u8Flag ;

        //memset(&_KeyReceived, 0 , sizeof(_KeyReceived) );

        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get IR Code. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key.
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_GetCode(U8 *pu8Key, U8 *pu8Flag)
{
    if (_KeyReceived.u8Valid)
    {
        *pu8Key = _KeyReceived.u8Key;
        *pu8Flag = _KeyReceived.u8Flag ;
        _KeyReceived.u8Valid = 0;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
EXPORT_SYMBOL(MDrv_IR_GetCode);

//-------------------------------------------------------------------------------------------------
/// Get IR key. It is a non-blocking function.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: No key.
//-------------------------------------------------------------------------------------------------
BOOL MDrv_IR_GetKeyOfTVLink(U8 *pu8Data, U8 *pu8Flag)
{
	BOOL	retVal = FALSE;

#ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE
	U32 	size2copy = IR_RAW_DATA_NUM;

    if (_TvLinkReceived.u8Valid)
	{
		retVal = TRUE;

		if( _TvLinkReceived.u8Flag == (_IR_READ_TYPE_WB_CODE - 1) )
		{
			size2copy = IR_WB_CODE_DATA_NUM;
		}
		else if( _TvLinkReceived.u8Flag == (_IR_READ_TYPE_TV_LINK - 1) )
		{
			size2copy = IR_TV_LINK_DATA_NUM;
		}

		memcpy( pu8Data, &(_TvLinkReceived.u8Data[0]), size2copy );

#if 0
		printk("CHECK0(%d): %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
					size2copy,
				 	pu8Data[0], pu8Data[1], pu8Data[2], pu8Data[3], pu8Data[4],
					pu8Data[5], pu8Data[6], pu8Data[7], pu8Data[8], pu8Data[9]);
#endif

		*pu8Flag = _TvLinkReceived.u8Flag ;
		memset(&_TvLinkReceived, 0 , sizeof(_TvLinkReceived) );
	}
#endif	/* #ifdef IR_INCLUDE_TV_LINK_AND_WB_CODE */

	return retVal;
}

//-----------------------------------------------------------------------------------------------
///set Multi ir header code
//-----------------------------------------------------------------------------------------------
BOOL MDrv_IR_SetHeaderCode(MS_MultiIR_HeaderInfo* pMIrHeaderInfo)
{

	_u8IRHeaderCode0 = pMIrHeaderInfo->_u8IRHeaderCode0;
	_u8IRHeaderCode1 = pMIrHeaderInfo->_u8IRHeaderCode1;
	#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
	_u8IR2HeaderCode0 = pMIrHeaderInfo->_u8IR2HeaderCode0;
	_u8IR2HeaderCode1 = pMIrHeaderInfo->_u8IR2HeaderCode1;
	printk("\r[%x][%x][%x][%x]\n",_u8IRHeaderCode0,_u8IRHeaderCode1,_u8IR2HeaderCode0,_u8IR2HeaderCode1);
	#endif
	return TRUE;
}
EXPORT_SYMBOL(MDrv_IR_SetHeaderCode);

//-------------------------------------------------------------------------------------------------
/// Return the time that IR key present. It is a non-blocking function.
/// @return Last key present time.
//-------------------------------------------------------------------------------------------------
unsigned long MDrv_IR_GetLastKeyTime(void)
{
    return _ulLastKeyPresentTime;
}

//-------------------------------------------------------------------------------------------------
/// Translate from IR key to internal key.
/// @param  u8Key  \b IN: IR key value.
/// @return translated internal IR key.
//-------------------------------------------------------------------------------------------------
U8 MDrv_IR_ParseKey(U8 u8Key)
{
    return _MDrv_IR_ParseKey(u8Key);
}

void MDrv_IR_EnableIR(U8 bEnable)
{
    //irqreturn_t isrStatus;
    bIRPass = !bEnable;
    if (bEnable)
    {
        //isrStatus = request_irq(E_FIQ_IR, _MDrv_IR_ISR, SA_INTERRUPT, "IR", NULL);
        if(ir_irq_depth > 0){
            enable_irq(E_FIQ_IR);
            ir_irq_depth--;
        }
    }
    else
    {
        disable_irq(E_FIQ_IR);
        ir_irq_depth++;
        //free_irq(E_FIQ_IR, NULL);
    }
}

#ifdef	IR_INCLUDE_TV_LINK_AND_WB_CODE
//-------------------------------------------------------------------------------------------------
/// Get IR key on WB Code mode.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_GetKeyOfWBCode(U8 *pu8Key, U8 *pu8Flag)
{
	BOOL	bRet = FALSE;
//	U32 	dummy;
	U32  	i, j;

	_u8IRRawModeCount = 0;
//20110601 khkh.lee
	if(REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)
	{
		   _bCheckQuickRepeat = 0;
		   return FALSE;
	}

	if(((_MDrv_IR_GetSystemTime() - _ulPrevKeyTime) >= IR_TIMEOUT_CYC/1000))
	{
		for (j=0; j<IR_RAW_DATA_NUM; j++)
		{
			if ( REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)  // check FIFO empty
				return FALSE;

			_u8IRRawModeBuf[_u8IRRawModeCount++] = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
			REG(REG_IR_FIFO_RD_PULSE) |= 0x0001; //read
			udelay(1000);//Delay

		}

		if(_u8IRRawModeCount == IR_RAW_DATA_NUM)
		{
			_u8IRRawModeCount = 0;

			if( (_u8IRRawModeBuf[0]==IR_HEADER_CODE0) && (_u8IRRawModeBuf[1]==IR_HEADER_CODE1) )
			{
				if(_u8IRRawModeBuf[2] == (U8)(~_u8IRRawModeBuf[3]))
				{
					pu8Key[0] = _u8IRRawModeBuf[2];
//					MDrv_IR_InverseKeyCode(pu8Key);

					*pu8Flag =	0;
	     			 _u8PrevKeyCode = *pu8Key;
					_ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
	 				bRet = TRUE;
				}
			}
			else if( (_u8IRRawModeBuf[0] == 0x1F) ||
				     (_u8IRRawModeBuf[0] >= 0x1C && _u8IRRawModeBuf[0] <= 0x1E) )
			{
				pu8Key[0] = _u8IRRawModeBuf[0];
				pu8Key[1] = _u8IRRawModeBuf[1];
				pu8Key[2] = _u8IRRawModeBuf[2];
				pu8Key[3] = _u8IRRawModeBuf[3];
				*pu8Flag = (_IR_READ_TYPE_WB_CODE - 1);
				bRet = TRUE;
			}
		}

		 _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
		 _bCheckQuickRepeat = 0;
        _MDrv_IR_ClearFIFO();
		return bRet;
	}
	else
	{
	   if(_bCheckQuickRepeat==0)
        {
           _bCheckQuickRepeat = 1;
            _ulPrevKeyTime = _MDrv_IR_GetSystemTime();
            _MDrv_IR_ClearFIFO();
			_u8IRRawModeCount = 0;

            return FALSE;
        }

	   	for (j=0; j<IR_RAW_DATA_NUM; j++)
		{
			if ( REG(REG_IR_SHOT_CNT_H_FIFO_STATUS) & IR_FIFO_EMPTY)  // check FIFO empty
				return FALSE;

			_u8IRRawModeBuf[_u8IRRawModeCount++] = REG(REG_IR_CKDIV_NUM_KEY_DATA) >> 8;
			REG(REG_IR_FIFO_RD_PULSE) |= 0x0001; //read
			udelay(1000);//Delay

		}

		_ulPrevKeyTime = _MDrv_IR_GetSystemTime();

		if( (_u8IRRawModeBuf[0]==IR_HEADER_CODE0) && (_u8IRRawModeBuf[1]==IR_HEADER_CODE1)
				&& (_u8IRRawModeBuf[2] == _u8PrevKeyCode )  && (_u8IRRawModeCount == IR_RAW_DATA_NUM) )
		{
			_u8IRRawModeCount=0;

			*pu8Key  = _u8IRRawModeBuf[2];
			*pu8Flag = 1;

			bRet = FALSE;

	        i = _MDrv_IR_GetSystemTime();
            if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
            {
                _u8PrevKeyCode     = *pu8Key;
                _ulPrevKeyRepeatTime    = i;
                _ePrevKeyProperty  = E_IR_KEY_PROPERTY_1st;
            }
            else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_1stDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    _ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
                    bRet = TRUE;
                }
            }
            else //E_IR_KEY_PROPERTY_FOLLOWING
            {
                if( (i - _ulPrevKeyRepeatTime) > _u32_2ndDelayTimeMs)
                {
                    _ulPrevKeyRepeatTime = i;
                    bRet = TRUE;
                }
            }

		}
		else if( (_u8IRRawModeBuf[0] == 0x1F) ||
				     (_u8IRRawModeBuf[0] >= 0x1C && _u8IRRawModeBuf[0] <= 0x1E) )
		{
			pu8Key[0] = _u8IRRawModeBuf[0];
			pu8Key[1] = _u8IRRawModeBuf[1];
			pu8Key[2] = _u8IRRawModeBuf[2];
			pu8Key[3] = _u8IRRawModeBuf[3];
			*pu8Flag = (_IR_READ_TYPE_WB_CODE - 1);
			bRet = TRUE;
		}
	}

	_MDrv_IR_ClearFIFO();

	return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Get IR key on TV LINK mode.
/// @param pu8Key  \b IN: Return IR key value.
/// @param pu8Flag \b IN: Return IR repeat code.
///
/// @return TRUE:  Success
/// @return FALSE: Failure
//-------------------------------------------------------------------------------------------------
static BOOL _MDrv_IR_GetKeyOfTVLink(U8 *pu8Key, U8 *pu8Flag)
{
	BOOL bRet = FALSE;

	static U8 u8CodeBuf[IR_TV_LINK_DATA_NUM];

	//printk("GetKeyOfTVLink %d(%4d)\n", _u32IRCount, _u32IRData[_u32IRCount - 1] );

	if ( _u32IRCount == 1 )
	{
        if( (_u32IRData[0] > IR_SW_TIMEOUT_CNT_LOB/*65535*/) ) //Time Out Flag
        {
            //printk("GetKeyOfTVLink %d(%6d)  < (IR_HDC_LOB = %6d, IR_OFC_RP_LOB+IR_LG01H_LOB = %6d , IR_OFC_RP_UPB+IR_LG01H_UPB = %6d)\n", _u32IRCount, _u32IRData[_u32IRCount - 1],IR_HDC_LOB,(IR_OFC_RP_LOB+IR_LG01H_LOB), (IR_OFC_RP_UPB+IR_LG01H_UPB) );

            // 처음으로 초기화
            //_u32IRCount = 0;
            //_u32IRAllCount = 0;
        }
#if 0
        if( (_u32IRData[0] > IR_SW_HDC_AND_RPOFC_LOB/*10350*/) && (_u32IRData[0] < IR_SW_HDC_AND_RPOFC_UPB/*12000*/) )//(9000+2500)*(1.1,0.9)  //Check if repeated key
        {
            //  printk("R(1)\n");
        }
#endif
        if( (_u32IRData[0] > IR_SW_TLL_PKT_TERM_LOB) && (_u32IRData[0] < IR_SW_TLL_PKT_TERM_UPB) )//45ms
        {
              //printk("PKT(1)\n");
        }
        else    // err
        {
#if 1
            //printk("ERR(0):%d\n",_u32IRData[0]);
            if(_IsTvLinkActive && _IsTvLinkBitErr)
                goto done;
#endif
        }
	}
	else if( _u32IRCount == 2 )
	{
#if 0
        if( (_u32IRData[1] > IR_SW_HDC_AND_RPOFC_LOB/*10350*/) && (_u32IRData[1] < IR_SW_HDC_AND_RPOFC_UPB/*12000*/) )//(9000+2500)*(1.1,0.9)  //Check if repeated key
        {
            //repeated key
            // printk("R(2)\n");
            pu8Key[0] = _LSB[u8CodeBuf[2]];
            *pu8Flag = (_IR_READ_TYPE_REPEAT - 1);
            bRet = TRUE;
            goto done;
        }
        else
#endif
        if( ( (_u32IRData[1] > IR_SW_HDC_AND_OFC_LOB/*12150*/) && (_u32IRData[1] < IR_SW_HDC_AND_OFC_UPB/*14850*/) ) )
        {
            //    printk("reset 2 ==> %d\n",_u32IRData[1]);
            // 처음으로 초기화
            //_u32IRCount = 0;
            //_u32IRAllCount = 0;
		}
        else
        {
            //printk("ERR(1):%d ",_u32IRData[1]);
            if(_IsTvLinkActive && _IsTvLinkBitErr)
                goto done;
        }
	}
	else if( _u32IRCount > 2 )
	{
		U32	index = (_u32IRCount - 3) / 8;
		U32 u32BitLen;

		if( (_u32IRCount - 3) % 8 == 0 )
		{
			u8CodeBuf[index] = 0;
		}
		else
		{
			u8CodeBuf[index] <<= 1;
		}

		u32BitLen = _u32IRData[ _u32IRCount - 1 ];

		//printk("BIT[%d:%d] (%8d): %8d %8d %8d %8d\n", index, (_u32IRCount - 3) % 8, u32BitLen, IR_LG0_LOB, IR_LG0_UPB, IR_LG1_LOB, IR_LG1_UPB );

		if( /*u32BitLen > IR_LG0_LOB &&*/ u32BitLen < IR_SW_LG0_UPB) //0
		{
			//u8CodeBuf[index] |= 0x00;
			//u8CodeBuf[index] &= ~(0x80);
			u8CodeBuf[index] &= ~(0x01);
		}
		else if ( /*u32BitLen > IR_LG1_LOB && */ u32BitLen < IR_SW_LG1_UPB) //1
		{
			//u8CodeBuf[index] |= 0x80;
			u8CodeBuf[index] |= 0x01;
		}
		else
		{
			//DEBUG_IR(printk(" invalid waveform\n"));
			//printk("BIT[%d:%d] (%8d): %2x\n", index, (_u32IRCount - 3) % 8, u32BitLen, u8CodeBuf[index] );

			//printk("BIT (%8d): %8d %8d %8d %8d\n", u32BitLen,IR_LG0_LOB, IR_LG0_UPB, IR_LG1_LOB, IR_LG1_UPB );
            _IsTvLinkBitErr = TRUE;
			bRet = FALSE;
			goto done;
		}
	}


	if( _u32IRCount < 2 + (IR_RAW_DATA_NUM * 8))
	{
		return FALSE;	//not complete yet
	}
	else if( _u32IRCount == 2 + (IR_RAW_DATA_NUM * 8) )
	{

		if(	(u8CodeBuf[0] == _LSB[IR_HEADER_CODE0]) && (u8CodeBuf[1] == _LSB[IR_HEADER_CODE1]) )
		{
			if(u8CodeBuf[2] == (u8)~u8CodeBuf[3])
			{
				pu8Key[0] = _LSB[u8CodeBuf[2]];
				//MDrv_IR_InverseKeyCode(pu8Key);
				*pu8Flag = (_IR_READ_TYPE_SINGLE - 1);
				bRet = TRUE;
                _IsTvLinkBitErr = FALSE;
				//printk("4 BYTE [%2x %2x %2x %2x]\n", u8CodeBuf[0], u8CodeBuf[1], u8CodeBuf[2], u8CodeBuf[3] );
				goto done;
			}
		}
		else if( 	(u8CodeBuf[0] == _LSB[0x7F]) && (u8CodeBuf[1] == _LSB[0x5F])
				 &&	(u8CodeBuf[2] == _LSB[0x3F]) && (u8CodeBuf[3] == _LSB[0x1F])	)
		{
			//printk("TV LINK START\n");
			//printk("4 BYTE [%2x %2x %2x %2x]\n", u8CodeBuf[0], u8CodeBuf[1], u8CodeBuf[2], u8CodeBuf[3] );
            _IsTvLinkBitErr = FALSE;
            _IsTvLinkActive = TRUE;
			bRet = FALSE;
			goto done;
		}
#if 0	// not support WD CODE when TV LINK MODE
		else if( ( _IsWBCodeMode ) && (u8CodeBuf[0] == _LSB[0x1F]) )
		{
			pu8Key[0] = _LSB[u8CodeBuf[0]];
			pu8Key[1] = _LSB[u8CodeBuf[1]];
			pu8Key[2] = _LSB[u8CodeBuf[2]];
			pu8Key[3] = _LSB[u8CodeBuf[3]];
			*pu8Flag = (_IR_READ_TYPE_WB_CODE - 1);
			bRet = TRUE;

			printk("IR CODE[%2x %2x %2x %2x]\n", pu8Key[0], pu8Key[1], pu8Key[2], pu8Key[3] );
			goto done;
		}
#endif
		else
		{
			return FALSE;	//not complete yet
		}
	}
	else if( _u32IRCount < 2 + (IR_TV_LINK_DATA_NUM * 8) )
	{
		//printk(" %d\n", _u32IRCount );
		return FALSE;	//not complete yet
	}
	else if( _u32IRCount == 2 + (IR_TV_LINK_DATA_NUM * 8) )
	{
        if(memcmp(_u8DataTemp, u8CodeBuf, IR_TV_LINK_DATA_NUM))
        {
    		memcpy( pu8Key, u8CodeBuf, IR_TV_LINK_DATA_NUM );
    		memcpy( _u8DataTemp, u8CodeBuf, IR_TV_LINK_DATA_NUM );
    		*pu8Flag = (_IR_READ_TYPE_TV_LINK - 1);
    		bRet = TRUE;
        }
        _IsTvLinkBitErr = FALSE;
#if 0
		{
			int i;

			printk("LINK(%3d) %02x", pu8Key[0] , pu8Key[0] );
			for( i =1; i< IR_TV_LINK_DATA_NUM; i++ )
			{
				printk(" %02x", pu8Key[i] );
			}
			printk("\n");
		}
#endif

		goto done;
	}

	//DEBUG_IR(printk(" invalid data\n"));
	bRet = FALSE;

done:

#if 0//def	IR_INCLUDE_REPEAT_TIME_TERM
	if( (bRet) && ((*pu8Flag) < _IR_READ_TYPE_REPEAT) )
	{
		unsigned long i;

		if ( (_u8PrevKeyCode != pu8Key[0]) || (!*pu8Flag) )
		{
			_ePrevKeyProperty = E_IR_KEY_PROPERTY_INIT;
		}

		i = _MDrv_IR_GetSystemTime();
		if( _ePrevKeyProperty == E_IR_KEY_PROPERTY_INIT)
		{
			_u8PrevKeyCode  	= pu8Key[0];
			_ulPrevKeyTime   	= i;
			_ePrevKeyProperty	= E_IR_KEY_PROPERTY_1st;
		}
		else if(_ePrevKeyProperty == E_IR_KEY_PROPERTY_1st)
		{
			if( (i - _ulPrevKeyTime) > _u32_1stDelayTimeMs)
			{
				_ulPrevKeyTime = i;
				_ePrevKeyProperty  = E_IR_KEY_PROPERTY_FOLLOWING;
			}
			else
			{
				bRet = FALSE;
			}
		}
		else //E_IR_KEY_PROPERTY_FOLLOWING
		{
			if( (i - _ulPrevKeyTime) > _u32_2ndDelayTimeMs)
			{
				_ulPrevKeyTime = i;
			}
			else
			{
				bRet = FALSE;
			}
		}
	}
#endif	//#ifdef	IR_INCLUDE_REPEAT_TIME_TERM

	_u32IRCount = 0;
	_u32IRAllCount = 0;

	return bRet;
}
#endif	//#ifdef	IR_INCLUDE_TV_LINK_AND_WB_CODE


BOOL MDrv_IR_IsFantasyProtocolSupported(void)
{
    return MSTAR_FANTASY_PROTOCOL_SUPPORTED;
}

ssize_t MDrv_IR_Read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
#ifdef SUPPORT_MULTI_PROTOCOL 
    if(MSTAR_FANTASY_PROTOCOL_SUPPORTED&&count==sizeof(long long))
#else
    if(MSTAR_FANTASY_PROTOCOL_SUPPORTED&&count==sizeof(U32))
#endif
    {
        if (down_interruptible(&fantasy_protocol.sem))
        {
            return -ERESTARTSYS;
        }

        while(fantasy_protocol.data==0)
        {
            up(&fantasy_protocol.sem); /* release the lock */
            if (filp->f_flags & O_NONBLOCK)
            {
                return -EAGAIN;
            }

            if (wait_event_interruptible(fantasy_protocol.inq, (fantasy_protocol.data!=0)))
            {
                return -ERESTARTSYS;
            }

            if (down_interruptible(&fantasy_protocol.sem))
            {
                return -ERESTARTSYS;
            }
        }

#ifdef CONFIG_MSTAR_SOFTWARE_IR_MODULE
        if(software_ir_enable())
        {
            take_over_by_software_ir_dfb(u8Key_for_mdrv_software_ir, u8RepeatFlag_for_mdrv_software_ir);
            set_software_ir_processing_undone();
            while(software_ir_processing_undone())
            {
                ;
            }
            set_software_ir_processing_undone();
        }
#endif

#ifdef SUPPORT_MULTI_PROTOCOL
		DEBUG_IR(IR_PRINT("fantasy_protocol.data = 0x%lx\n", fantasy_protocol.data));
        if(copy_to_user(buf,&fantasy_protocol.data, sizeof(long long)))
        {
            up(&fantasy_protocol.sem);
            return -EFAULT;
        }
#else
        if(copy_to_user(buf,&fantasy_protocol.data,sizeof(U32)))
        {
            up(&fantasy_protocol.sem);
            return -EFAULT;
        }
#endif
        //success
        fantasy_protocol.data=0;
        up(&fantasy_protocol.sem);

        return count;

    }
    else
    {

        return 0;
    }
}
EXPORT_SYMBOL(MDrv_IR_Read);

unsigned int MDrv_IR_Poll(struct file *filp, poll_table *wait)
{
    if (MSTAR_FANTASY_PROTOCOL_SUPPORTED)
    {
        unsigned int mask=0;

        down(&fantasy_protocol.sem);
        poll_wait(filp, &fantasy_protocol.inq, wait);
        if (fantasy_protocol.data != 0)
        {
            mask |= POLLIN|POLLRDNORM;
        }
        up(&fantasy_protocol.sem);

        return mask;
    }
    else
    {
        return 0;
    }
}
EXPORT_SYMBOL(MDrv_IR_Poll);

void MDrv_IR_SendKey(U8 u8Key, U8 u8RepeatFlag)
{
	#if defined(IR_TYPE_HISENSE) && (IR_TYPE_SEL == IR_TYPE_HISENSE)//for hisense need suport 2 ir head code ;
			U32 hisense_factory_ir=0;
			if(u8RepeatFlag&FCTORY_HEADER_RECEIVE)
			{
				u8RepeatFlag &= (~FCTORY_HEADER_RECEIVE);
                hisense_factory_ir=(0x04UL << 28);
			}
            else if(u8RepeatFlag&PCCOMMAND_HEADER_RECEIVE)
            {
                u8RepeatFlag &= (~PCCOMMAND_HEADER_RECEIVE);
                hisense_factory_ir=(0x08UL << 28);
            }
            else
            {
                hisense_factory_ir=(0x01UL << 28);
            }
            fantasy_protocol.data = (u8Key<<8 | u8RepeatFlag);
            fantasy_protocol.data |= hisense_factory_ir;
	#else //
//                    fantasy_protocol.data=u32InputData;
                    fantasy_protocol.data = (u8Key<<8 | u8RepeatFlag);

                    //set fantasy class to IR
                    fantasy_protocol.data |= (0x01 << 28);
	#endif
                wake_up_interruptible(&fantasy_protocol.inq);
}
EXPORT_SYMBOL(MDrv_IR_SendKey);

void MDrv_IR_SetMasterPid(pid_t pid)
{
    MasterPid = pid;
}

pid_t MDrv_IR_GetMasterPid(void)
{
    return MasterPid;
}

#ifdef CONFIG_MSTAR_IR_INPUT_DEVICE
static DECLARE_WORK(key_dispatch_thread,key_dispatch);
static struct workqueue_struct *key_dispatch_workqueue;
int MDrv_IR_Input_Init(void)
{
    printk("=====MDrv_IR_Input_Init====\n");
#if (IR_TYPE_SEL == IR_TYPE_CHANGHONG)
    char *map_name = RC_MAP_CHANGHONG_TV;
    char *input_name = "ChangHong Smart TV IR Receiver";
    __u16 vendor_id = 0x0037UL;
#elif (IR_TYPE_SEL == IR_TYPE_HAIER)
    char *map_name = RC_MAP_HAIER_TV;
    char *input_name = "Haier Smart TV IR Receiver";
    __u16 vendor_id = 0x201eUL;
#elif (IR_TYPE_SEL == IR_TYPE_HISENSE)
    char *map_name = RC_MAP_HISENSE_TV;
    char *input_name = "Hisense Smart TV IR Receiver";
    __u16 vendor_id = 0x0018UL;
#elif (IR_TYPE_SEL == IR_TYPE_KONKA)
    char *map_name = RC_MAP_KONKA_TV;
    char *input_name = "Konka Smart TV IR Receiver";
    __u16 vendor_id = 0x0416UL;
#elif (IR_TYPE_SEL == IR_TYPE_SKYWORTH)
    char *map_name = RC_MAP_SKYWORTH_TV;
    char *input_name = "Skyworth Smart TV IR Receiver";
    __u16 vendor_id = 0x1918UL;
#elif (IR_TYPE_SEL == IR_TYPE_TCL)
    char *map_name = RC_MAP_TCL_TV;
    char *input_name = "Tcl Smart TV IR Receiver";
    __u16 vendor_id = 0x0019UL;
#else
    char *map_name = RC_MAP_MSTAR_TV;
    char *input_name = "MStar Smart TV IR Receiver";
    __u16 vendor_id = 0x3697UL;
#endif

    int err = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
    struct rc_dev *dev;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
    struct input_dev *dev;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 32)
    struct input_dev *dev;
    struct ir_scancode_table *ir_codes = &ir_codes_mstar_tv_table;
#else
    struct input_dev *dev;
    IR_KEYTAB_TYPE *ir_codes = ir_codes_mstar_tv;
#endif

    ir = kzalloc(sizeof(struct mstar_ir), GFP_KERNEL);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
	dev = rc_allocate_device();
#else
    dev = input_allocate_device();
#endif
    if (!ir || !dev)
        return -ENOMEM;

    // init input device
    ir->dev = dev;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
    dev->driver_name = MDRV_NAME_IR;
    dev->map_name = map_name;
    dev->driver_type = RC_DRIVER_IR_RAW;
    dev->input_name = input_name;
    dev->input_phys = "/dev/ir";
    dev->input_id.bustype = BUS_I2C;
    dev->input_id.vendor = vendor_id;
    dev->input_id.product = 0x0001UL;
    dev->input_id.version = 1;
    dev->allowed_protos = RC_TYPE_ALL;
    //dev->open = fintek_open;
    //dev->close = fintek_close;
    //dev->min_timeout = ITE_MIN_IDLE_TIMEOUT;
    //dev->max_timeout = ITE_MAX_IDLE_TIMEOUT;
    //dev->timeout = MS_TO_NS(ITE_IDLE_TIMEOUT);
    /* rx resolution is hardwired to 50us atm, 1, 25, 100 also possible */
    //dev->rx_resolution = US_TO_NS(CIR_SAMPLE_PERIOD);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
    err = ir_input_init(dev, &ir->ir, IR_TYPE_RC5);
#else
    ir_input_init(dev, &ir->ir, IR_TYPE_RC5, ir_codes);
#endif
    if (err < 0)
    {
        input_free_device(dev);
        kfree(ir);
        return err;
    }

    dev->name = input_name;
    dev->phys = "/dev/ir";
    dev->id.bustype = BUS_I2C;
    dev->id.vendor = vendor_id;
    dev->id.product = 0x0001UL;
    dev->id.version = 1;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
    err = rc_register_device(dev);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
    err = ir_input_register(ir->dev, map_name, NULL, MDRV_NAME_IR);
#else
    err = input_register_device(ir->dev);
#endif
    if (err)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
        rc_free_device(dev);
#else
        input_free_device(dev);
#endif
        kfree(ir);
        return err;
    }

    // No auto-repeat.
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
    clear_bit(EV_REP, ir->dev->input_dev->evbit);
#else
    clear_bit(EV_REP, ir->dev->evbit);
#endif

    init_completion(&key_completion);
    key_dispatch_workqueue = create_workqueue("keydispatch_wq");
    queue_work(key_dispatch_workqueue, &key_dispatch_thread);
    return 0;
}

void MDrv_IR_Input_Exit(void)
{
    destroy_workqueue(key_dispatch_workqueue);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 0, 20)
    rc_register_device(ir->dev);
    rc_free_device(ir->dev);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 35)
    ir_input_unregister(ir->dev);
#else
    input_unregister_device(ir->dev);
#endif
    input_free_device(ir->dev);
#endif
    kfree(ir);
}
#endif // CONFIG_MSTAR_IR_INPUT_DEVICE
