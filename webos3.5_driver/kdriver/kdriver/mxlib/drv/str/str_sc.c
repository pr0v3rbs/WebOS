#include <linux/device.h>
#include <linux/export.h>

#include <linux/io.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "drvGPIO.h"
#include "mst_str.h"
#include "mst_reg_bank.h"

//#define BIT(_bit_)          (1 << (_bit_))

/// data type unsigned char, data length 1 byte
//typedef unsigned char               MS_U8;                              // 1 byte
/// data type unsigned short, data length 2 byte
//typedef unsigned short              MS_U16;                             // 2 bytes
/// data type unsigned int, data length 4 byte
//typedef unsigned long               MS_U32;                             // 4 bytes

typedef unsigned char               MS_BOOL;

/// definition for TRUE
#define TRUE                        1
/// definition for FALSE
#define FALSE                       0

/// definition for ENABLE
#define UNUSE                      0
#define RW_ONLY                      1
#define R_ONLY                      2
#define W_ONLY                      3
#define FOLLOW_ONE           4
/// definition for DISABLE

#define CEC_CMD_Addr 0x68       //CEC command addr
#define CEC_LEN_Addr  0x70       //CEC length addr
#define CEC_CMD_VALUE 0x0000 //0x0000 means "ping"
#define CEC_LEN_VALUE 0x00     //lowbyte is length vaue = 0x00
#define CEC_SAMPLE_SEL  0x10  //Default is 0x10
#define HDCP_KEY_SIZE       289
#define EFUSE_BANK 0x2000
#define REG_EFUSE_GADRR             (0x2C)
#define REG_EFUSE_RDATA_ADR         (0x01)
#define REG_EFUSE_RDATA_TRI         (0x00)
#define REG_EFUSE_RDATA_LOBANK_HI   (0x31)
#define REG_EFUSE_RDATA_LOBANK_LO   (0x30)
#define REG_EFUSE_RDATA_HIBANK_HI   (0x33)
#define REG_EFUSE_RDATA_HIBANK_LO   (0x32)
#define REG_EFUSE_RDATA_HI          (0x31)
#define REG_EFUSE_RDATA_LO          (0x30)
#define REG_EFUSE_RDATA_ADR2        (0x27)
#define REG_EFUSE_RDATA_TRI2        (0x26)
#define REG_EFUSE_RDATA_LOBANK_HI2  (0x41)
#define REG_EFUSE_RDATA_LOBANK_LO2  (0x40)
#define REG_EFUSE_RDATA_HIBANK_HI2  (0x43)
#define REG_EFUSE_RDATA_HIBANK_LO2  (0x42)

#define REG_EFUSE_WRITE_MASK(adr, value, mask) MDrv_STR_Write2ByteMsk(EFUSE_BANK, adr, value, mask)
#define REG_EFUSE_READ(adr) MDrv_STR_Read2Byte(EFUSE_BANK, adr)

#define HDCP_BK 0x110A00
#define HDCP_OFFSET 0x60
#define INTERNAL_HDCP 0
#define DUMP_DATA 0

//scaler bank, now number is 0x80
#define SC_BKNUM                    0x80

//move to mst_reg_bank.h
#define FRC_INSIDE FALSE

#define MIU0_G0_REQUEST_MASK MIU0_BASE + 0x46
#define MIU0_G1_REQUEST_MASK MIU0_BASE + 0x66
#define MIU0_G2_REQUEST_MASK MIU0_BASE + 0x86
#define MIU0_G3_REQUEST_MASK MIU0_BASE + 0xA6
#define MIU1_G0_REQUEST_MASK MIU1_BASE + 0x46
#define MIU1_G1_REQUEST_MASK MIU1_BASE + 0x66
#define MIU1_G2_REQUEST_MASK MIU1_BASE + 0x86
#define MIU1_G3_REQUEST_MASK MIU1_BASE + 0xA6
#define MIU_SC_G0REQUEST_MASK   (0x0000)
#define MIU_SC_G1REQUEST_MASK   (0x8060)
#define MIU_SC_G2REQUEST_MASK   (0x0000)
#define MIU_SC_G3REQUEST_MASK   (0x0FC0)


#define BANK_NUM                    XC_RELATED_BANKNUM + SC_BKNUM
#define SUBBANK_NUM                 0x02//0x4
#define REG_PERBANK_NUM             0x80
#define ADC_Init_SUM               0x60

#define YSRAM_SIZE                  512
#define YSRAM_UNIT_SIZE             4//5
#define CSRAM_SIZE                  512
#define CSRAM_UNIT_SIZE             3

#define SRAM1_IHC_COUNT 289//81
#define SRAM2_IHC_COUNT 272//72
#define SRAM3_IHC_COUNT 272//72
#define SRAM4_IHC_COUNT 256//64
#define SRAM_ICC_COUNT 1024
#define SRAM_XVYCC_COUNT 300
#define SRAM_XVYCC_GAMMA_COUNT 256

#define GAMMASRAM_DE_SIZE   691
#define GAMMASRAM_SIZE      256
#define GAMMASRAM_UNIT_SIZE 4   //add, data{R, G, B}
#define GAMMASRAM_CHANNEL_SIZE 3
#define XVYCCSRAM_SIZE 2
#define DEVICE_NAME     		"hdmi"

//enum

typedef enum {
    GAMMASRAM_ADD           = 0,
    GAMMASRAM_R           = 1,
    GAMMASRAM_G         =  2,
    GAMMASRAM_B         = 3,
    GAMMASRAM_MAX
} GAMMASRAM_UNIT;

//============================================//
//!!!!!!!!! this part depend on chip spec. !!!!!!!!!!!!!!!!!!!!!!!//
//1Please follow chip spec. to update this array
//-------------------------------------------------------//
//Local struct//
static MS_U32 bank_need_backup[BANK_NUM][2] =
{
    {MIU0_BASE  ,        RW_ONLY},           //  0x00
    {MIU1_BASE  ,        RW_ONLY},            //  0x01
    {MIU_ARB    ,           RW_ONLY},                   //  0x02
    {MIU_ARB2   ,           RW_ONLY},                   //  0x03
    {CLKGEN     ,            R_ONLY},                         //  0x04
    {ADC_ATOP   ,        RW_ONLY},           //  0x05
    {ADC_DTOP,          RW_ONLY},            //  0x06
    {ADC_CHIPTOP,   R_ONLY},                          //  0x07
    {HDMI       ,               RW_ONLY},             //  0x08
    {ADC_ATOPB  ,       RW_ONLY},            //  0x09
    {HDMI2,                    RW_ONLY},     //  0xA
    {SC_0       ,                   RW_ONLY},   //  0x0B
    {SC_2       ,                   RW_ONLY},   //  0x0C
    {VE_0       ,                   RW_ONLY},   //  0x0D
    {VE_1       ,                   RW_ONLY},   //  0x0E
    {VE_2       ,                   RW_ONLY},   //  0x0F
    {DVI_ATOP   ,           RW_ONLY},    //  0x10
    {HDMIA      ,              RW_ONLY},     //  0x11
    {ADC_DTOPB  ,       RW_ONLY},    //  0x12
    {DVI_ATOP1  ,          RW_ONLY},    //  0x13
    {HDMIB      ,               RW_ONLY},    //  0x14
    {DVI_ATOP2  ,           RW_ONLY},    //  0x15
    {HDMID      ,               RW_ONLY},    //  0x16
    {DVI_PS     ,                 RW_ONLY},    //  0x17
    {HDMIC      ,               RW_ONLY},    //  0x18
    {HDCPKEY    ,             RW_ONLY},    //  0x19
    {MIU_ARB3  ,           RW_ONLY}, //0x1A
    {SC_BK00    ,               RW_ONLY},    //  XC_RELATED_BANKNUM + 0x00
    {SC_BK01    ,               RW_ONLY},
    {SC_BK02    ,           RW_ONLY},
    {SC_BK03    ,           RW_ONLY},
    {SC_BK04    ,           RW_ONLY},
    {SC_BK05    ,           RW_ONLY},
    {SC_BK06    ,           RW_ONLY},
    {SC_BK07    ,           UNUSE},
    {SC_BK08    ,           RW_ONLY},
    {SC_BK09    ,           UNUSE},
    {SC_BK0A    ,           RW_ONLY},
    {SC_BK0B    ,           UNUSE},
    {SC_BK0C    ,           RW_ONLY},
    {SC_BK0D    ,           R_ONLY},
    {SC_BK0E    ,           UNUSE},
    {SC_BK0F    ,           RW_ONLY},
    {SC_BK10    ,           RW_ONLY},
    {SC_BK11    ,           RW_ONLY},
    {SC_BK12    ,           RW_ONLY},
    {SC_BK13    ,           RW_ONLY},
    {SC_BK14    ,           UNUSE},
    {SC_BK15    ,           RW_ONLY},
    {SC_BK16    ,           RW_ONLY},
    {SC_BK17    ,           UNUSE},
    {SC_BK18    ,           RW_ONLY},
    {SC_BK19    ,           RW_ONLY},
    {SC_BK1A    ,           RW_ONLY},
    {SC_BK1B    ,           RW_ONLY},
    {SC_BK1C    ,           RW_ONLY},
    {SC_BK1D    ,           UNUSE},
    {SC_BK1E    ,            RW_ONLY},
    {SC_BK1F    ,           RW_ONLY},
    {SC_BK20    ,           RW_ONLY},
    {SC_BK21    ,           RW_ONLY},
    {SC_BK22    ,           RW_ONLY},
    {SC_BK23    ,           RW_ONLY},
    {SC_BK24    ,           RW_ONLY},
    {SC_BK25    ,           RW_ONLY},
    {SC_BK26    ,           RW_ONLY},
    {SC_BK27    ,           RW_ONLY},
    {SC_BK28    ,           UNUSE},
    {SC_BK29    ,           RW_ONLY},
    {SC_BK2A    ,           RW_ONLY},
    {SC_BK2B    ,           RW_ONLY},
    {SC_BK2C    ,           RW_ONLY},
    {SC_BK2D    ,           RW_ONLY},
    {SC_BK2E    ,           RW_ONLY},
    {SC_BK2F    ,           UNUSE},
    {SC_BK30    ,           UNUSE},
    {SC_BK31    ,           UNUSE},
    {SC_BK32    ,           UNUSE},
    {SC_BK33    ,           UNUSE},
    {SC_BK34    ,           RW_ONLY},
    {SC_BK35    ,           UNUSE},
    {SC_BK36    ,           RW_ONLY},
    {SC_BK37    ,           UNUSE},
    {SC_BK38    ,           RW_ONLY},
    {SC_BK39    ,           UNUSE},
    {SC_BK3A    ,           UNUSE},
    {SC_BK3B    ,           RW_ONLY},
    {SC_BK3C    ,           UNUSE},
    {SC_BK3D    ,           UNUSE},
    {SC_BK3E    ,           UNUSE},
    {SC_BK3F    ,           RW_ONLY},
    {SC_BK40    ,           UNUSE},
    {SC_BK41    ,           UNUSE},
    {SC_BK42    ,           UNUSE},
    {SC_BK43    ,           UNUSE},
    {SC_BK44    ,           UNUSE},
    {SC_BK45    ,           UNUSE},
    {SC_BK46    ,           UNUSE},
    {SC_BK47    ,           UNUSE},
    {SC_BK48    ,           UNUSE},
    {SC_BK49    ,           UNUSE},
    {SC_BK4A    ,           UNUSE},
    {SC_BK4B    ,           UNUSE},
    {SC_BK4C    ,           UNUSE},
    {SC_BK4D    ,           UNUSE},
    {SC_BK4E    ,           UNUSE},
    {SC_BK4F    ,           UNUSE},
    {SC_BK50    ,           UNUSE},
    {SC_BK51    ,           UNUSE},
    {SC_BK52    ,           UNUSE},
    {SC_BK53    ,           UNUSE},
    {SC_BK54    ,           UNUSE},
    {SC_BK55    ,           UNUSE},
    {SC_BK56    ,           UNUSE},
    {SC_BK57    ,           UNUSE},
    {SC_BK58    ,           UNUSE},
    {SC_BK59    ,           UNUSE},
    {SC_BK5A    ,           UNUSE},
    {SC_BK5B    ,           UNUSE},
    {SC_BK5C    ,           UNUSE},
    {SC_BK5D    ,           UNUSE},
    {SC_BK5E    ,           UNUSE},
    {SC_BK5F    ,           UNUSE},
    {SC_BK60    ,           RW_ONLY},
    {SC_BK61    ,           UNUSE},
    {SC_BK62    ,           RW_ONLY},
    {SC_BK63    ,           RW_ONLY},
    {SC_BK64    ,           RW_ONLY},
    {SC_BK65    ,           UNUSE},
    {SC_BK66    ,           RW_ONLY},
    {SC_BK67    ,           UNUSE},
    {SC_BK68    ,           UNUSE},
    {SC_BK69    ,           UNUSE},
    {SC_BK6A    ,           UNUSE},
    {SC_BK6B    ,           UNUSE},
    {SC_BK6C    ,           UNUSE},
    {SC_BK6D    ,           UNUSE},
    {SC_BK6E    ,           RW_ONLY},
    {SC_BK6F    ,           UNUSE},
    {SC_BK70    ,           UNUSE},
    {SC_BK71    ,           UNUSE},
    {SC_BK72    ,           UNUSE},
    {SC_BK73    ,           UNUSE},
    {SC_BK74    ,           UNUSE},
    {SC_BK75    ,           UNUSE},
    {SC_BK76    ,           UNUSE},
    {SC_BK77    ,           UNUSE},
    {SC_BK78    ,           UNUSE},
    {SC_BK79    ,           UNUSE},
    {SC_BK7A    ,           UNUSE},
    {SC_BK7B    ,           UNUSE},
    {SC_BK7C    ,           UNUSE},
    {SC_BK7D    ,           UNUSE},
    {SC_BK7E    ,           UNUSE},
    {SC_BK7F    ,           UNUSE}
};

static MS_U32 subbank_need_backup[SUBBANK_NUM][2] =
{
    {LPLL_BK, 0},
    {LPLL_BK, 1}
    //{MOD_BK, 0},      //LPLL must run resume function. root cause: It will effect the Gamma tabe download.
    //{MOD_BK, 1}       //MOD bank can remove in resume function. It will be set in panel init function
};
static MS_U32 ADC_init_Table[ADC_Init_SUM][4] =
{
   {ADC_ATOP,0x2, 0x00 ,0x7000},
   {ADC_ATOP,0xE, 0, 0x0001},
   {ADC_ATOP,0x12, 0, 0x0001} ,
   {ADC_ATOP, 0x5E,0, 0x00C0},
   {ADC_ATOP,0x5E ,0x1A00, 0x3F00},
   {ADC_ATOP,0x20 ,0x0004, 0x003F },
   {ADC_ATOP, 0x10,0x0100, 0x0700 },
   {ADC_ATOP, 0x0A ,0, 0x0002 },
   {ADC_ATOP,0x0B  ,0, 0x1800 },
   {ADC_ATOP, 0x20,0x7700, 0x7700},
   {ADC_ATOP, 0x39,0x1F,  0x00},
   {ADC_ATOP, 0x39,0x2000, 0x2000},
   {ADC_ATOP, 0x3B ,0x0090, 0x00FF},
   {ADC_ATOP,0x3B ,0 ,0x0C00},
   {ADC_ATOP, 0x3C,0,0x001F },
   {ADC_ATOP,0x3C ,0x2000, 0x2000},
   {ADC_ATOP,0x3E, 0x0080, 0x00FF},
   {ADC_ATOP, 0x3E,0, 0x0C00},
   {ADC_ATOP, 0x7A,0x3200, 0xFF00},
   {ADC_ATOP,0x7B  ,0x00, 0x002A },
   {ADC_ATOP, 0x19,0, 0x0001 },
   {ADC_ATOP ,0x30,0, 0x0007 },
   {ADC_DTOP,0x76 ,0x00E0, 0x00FF },
   {ADC_DTOP, 0x76 ,0x0700, 0x0F00},
   {ADC_DTOP, 0x0D,0x1000, 0x1000},
   {ADC_DTOP, 0x07 ,0, 0x0018},
   {ADC_DTOP,0x07 ,0x0400, 0xFF00},
   {ADC_DTOP, 0x63 ,0x0017, 0x003F },
   {ADC_DTOP,0x64 ,0x0017, 0x003F},
   {ADC_DTOP,0x18 ,0x0020, 0x00FF },
   {ADC_DTOP,0x18 ,0x0700, 0x0700},
   {ADC_DTOP,0x19 ,0x0400, 0x1E00 },
   {ADC_DTOP,0x65 ,0x00CC, 0x00FF},
   {ADC_DTOP,  0x65,0xCC00, 0xFF00 },
   {ADC_DTOP, 0x22,0x0005, 0x00FF},
   {ADC_DTOP,0x22 ,0x0000, 0x0F00},
   {ADC_DTOP,0x23,0x00D0, 0x00FF},
   {ADC_DTOP,0x23,0x0700, 0x1F00},
   {ADC_DTOP,  0x24,0x0060, 0x00FF},
   {ADC_DTOP, 0x24,0x2000, 0x3F00},
   {ADC_DTOP,0x25 ,0x00A0, 0x00FF},
   {ADC_DTOP,  0x25,0x0300, 0x1F00},
   {ADC_DTOP, 0x26,0, 0x00FF},
   {ADC_DTOP, 0x26,0x1C00, 0x3F00},
   {ADC_DTOP,0x27 ,0x0040, 0x00FF },
   {ADC_DTOP, 0x27,0x0500, 0x3F00 },
   {ADC_DTOP, 0x28 ,0x004A, 0x00FF },
   {ADC_DTOP, 0x28,0x0800, 0xFF00},
   {ADC_DTOP,0x29 ,0x0010, 0x00FF },
   {ADC_DTOP, 0x29,0x2000, 0x3F00},
   {ADC_DTOP,0x2C ,0x0005, 0x00FF },
   {ADC_DTOP, 0x2C,0, 0x0F00},
   {ADC_DTOP,0x2D ,0x00D0, 0x00FF },
   {ADC_DTOP, 0x2D ,0x0700, 0x1F00},
   {ADC_DTOP,0x2E ,0x0060, 0x00FF },
   {ADC_DTOP,0x2E ,0x2000, 0x3F00},
   {ADC_DTOP, 0x2F,0x00A0, 0x00FF },
   {ADC_DTOP, 0x2F,0x0300, 0x1F00 },
   {ADC_DTOP,0x30 ,0, 0x00FF },
   {ADC_DTOP,0x30,0x1C00, 0x3F00 },
   {ADC_DTOP, 0x31,0x0040, 0x00FF},
   {ADC_DTOP, 0x31 ,0x0500, 0x3F00},
   {ADC_DTOP, 0x32,0x004A, 0x00FF},
   {ADC_DTOP, 0x32 ,0x0800, 0xFF00 },
   {ADC_DTOP, 0x33 ,0x0010, 0x00FF },
   {ADC_DTOP, 0x33 ,0x2000, 0x3F00},
   {ADC_DTOP, 0x36 ,0x0005, 0x00FF},
   {ADC_DTOP,0x36 ,0, 0x0F00},
   {ADC_DTOP, 0x37,0x00D0, 0x00FF },
   {ADC_DTOP,0x37 ,0x0700, 0x1F00 },
   {ADC_DTOP, 0x38,0x0060, 0x00FF},
   {ADC_DTOP,0x38, 0x2000, 0x3F00},
   {ADC_DTOP, 0x39,0x00A0, 0x00FF},
   {ADC_DTOP, 0x39,0x0300, 0x1F00 },
   {ADC_DTOP, 0x3A ,0x00, 0x00FF },
   {ADC_DTOP,0x3A ,0x1C00, 0x3F00 },
   {ADC_DTOP, 0x3B ,0x0040, 0x00FF},
   {ADC_DTOP,0x3B ,0x0500, 0x3F00},
   {ADC_DTOP, 0x3C ,0x004A, 0x00FF },
   {ADC_DTOP, 0x3C,0x0800, 0xFF00 },
   {ADC_DTOP, 0x3D ,0x0010, 0x00FF },
   {ADC_DTOP, 0x3D ,0x2000, 0x3F00 },
   {ADC_DTOP, 0x62,0,0x6000},
   {ADC_DTOP,0x68, 0, 0x0008},
   {ADC_ATOP, 0x40,0x0008, 0x008F},
   {ADC_ATOP,0x40 ,0, 0x0100},
   {ADC_ATOP, 0x42, 0x00EC, 0x00FF},
   {ADC_ATOP,0x42 ,0, 0x0F00},
   {ADC_ATOP, 0x43,0,0x007F},
   {ADC_ATOP, 0x43,0, 0x7F00 },
   {ADC_ATOP, 0x44,0x001C, 0x003F},
   {ADC_ATOP, 0x44,0xFF00, 0xFF00 },
   {ADC_ATOP, 0x45,0, 0x003F },
   {ADC_ATOP, 0x46,0x000B, 0x001B},
   {ADC_ATOP, 0x47,0x0800,0x0800 },
   {ADC_ATOP, 0x53,0x0008, 0x001F }
};
//-------------------------------------------------------//
//============================================//


static MS_U32 bank_backup[BANK_NUM][REG_PERBANK_NUM] = {} ;

static MS_U32 subbank_backup[SUBBANK_NUM][REG_PERBANK_NUM] = {} ;
static MS_U32 YSram_backup[YSRAM_SIZE][YSRAM_UNIT_SIZE] = {};
static MS_U32 CSram_backup[CSRAM_SIZE][CSRAM_UNIT_SIZE] = {};
static MS_U32 SRAM1_IHC_backup[SRAM1_IHC_COUNT] = {};
static MS_U32 SRAM2_IHC_backup[SRAM2_IHC_COUNT] = {};
static MS_U32 SRAM3_IHC_backup[SRAM3_IHC_COUNT] = {};
static MS_U32 SRAM4_IHC_backup[SRAM4_IHC_COUNT] = {};
static MS_U32 SRAM_ICC_backup[SRAM_ICC_COUNT] = {};
static MS_U32 GammaSram_DE_backup[GAMMASRAM_DE_SIZE][GAMMASRAM_UNIT_SIZE] = {};
static MS_U16 GammaSram_backup[GAMMASRAM_SIZE][GAMMASRAM_UNIT_SIZE] = {};
static MS_U32 SRAM_XVYCC_R_backup[SRAM_XVYCC_COUNT][XVYCCSRAM_SIZE] = {};
static MS_U32 SRAM_XVYCC_GAMMA_R_backup[SRAM_XVYCC_GAMMA_COUNT] = {};
static MS_U16 u16DeGammaStatus = 0;
static MS_U16 u16XVYCCBypassEn = 0;
static MS_U16 u16OverDriveStatus = 0;
//static MS_U16 u16OverDriveInit[15] = {};
static MS_U8  u8OverDriveTable[1089] ={};

static uint8_t	uEDITName = 0x00;
static dev_t dev;
static int32_t major;
static int32_t minor;

static long ioctl_hdmi(struct file *file, uint32_t cmd, unsigned long arg)
{
	long ret = 0;
	switch (cmd)	{
		case 0x14:
			if (get_user(uEDITName, (uint8_t *)arg) < 0)
				printk(KERN_INFO "[ioctl_hdmi] bEDIDName get Logic Type, tv_ioctl Error\n");
			break;
		default:
			break;
		}
	return ret;
}


static struct file_operations fops =	{
	.owner          = THIS_MODULE,
	.unlocked_ioctl =ioctl_hdmi,
};

static struct cdev cdev;

extern int init_hdmi(void)
{
	STR_PRINT(KERN_INFO "init_hdmi call\n");
	major = 304;
	minor = 0;
	dev = 0;
	dev = MKDEV(major, minor);
	register_chrdev_region(dev, 1, DEVICE_NAME);
	cdev_init(&cdev, &fops);
	cdev.owner = THIS_MODULE;
	cdev.ops   = &fops;
	cdev_add(&cdev, dev, 1);
	return 0;
}

extern void exit_hdmi(void)
{
	STR_PRINT(KERN_INFO "exit_hdmi call\n");
	unregister_chrdev_region(dev, 1);
	cdev_del(&cdev);
	return;
}
#if 0
static void Hal_SetHPD(DVI_PORT_TYPE eHDMIPort,MS_BOOL bClkPull)
{

  ////Set HPD
        switch(eHDMIPort)
        {
            case DVI_PORT_0://A
                PM_W2BYTE(REG_PM_SLEEP_27_L, bClkPull ? 0 : BIT(4), BIT(4)|BIT(0));  //Bit (4) enable =  pull low
                break;
            case DVI_PORT_1://B
                PM_W2BYTE(REG_PM_SLEEP_27_L, bClkPull ? 0 : BIT(5), BIT(5)|BIT(1));
                break;
            case DVI_PORT_2://C
                PM_W2BYTE(REG_PM_SLEEP_27_L, bClkPull ? 0 : BIT(6), BIT(6)|BIT(2));
                break;
            case DVI_PORT_3://D
                PM_W2BYTE(REG_PM_SLEEP_27_L, bClkPull ? 0 : BIT(7), BIT(7)|BIT(3));
                break;
            default:
                PM_W2BYTE(REG_PM_SLEEP_27_L, bClkPull ? 0 : BMASK(7:4), LBMASK);
                break;
        }
}
#endif
static void SetHPD(uint8_t eHDMIPort , MS_BOOL bLow )
{
     MS_U32  u32Pulldownport =0;
     // bLow = 1 means pull low   bLow =0 mwans pull low
     STR_PRINT("[SetHPD]    Note PC eHDMIPort =0x%x \n",eHDMIPort);
     u32Pulldownport = (~eHDMIPort) & 0xF;
     STR_PRINT("[SetHPD]    Need pull down eHDMIPort =0x%lx \n",u32Pulldownport);
     MDrv_STR_Write2ByteMsk(PM_SLEEP, 0x27, bLow?(u32Pulldownport<<4):0 , ( 0x00FF & (u32Pulldownport<<4) ));
     STR_PRINT("[SetHPD]   value =0x%lx \n", bLow?(u32Pulldownport<<4):0);
     STR_PRINT("[SetHPD]   Mask =0x%lx \n",0x00FF & (u32Pulldownport<<4) );
}

static void SetHPDSuspend(void)
{
#define GPI_PM_OPT1 43
            //Low:       Internal Micom
            //HIGH:     External Micom
        if(_mdrv_gpio_get_level(GPI_PM_OPT1))
            MDrv_STR_Write2ByteMsk(PM_SLEEP, 0x27, 0x0F , 0xFF);
        else
            MDrv_STR_Write2ByteMsk(PM_SLEEP, 0x27, 0x00 , 0xFF); //LMTASKWBS-69904
}

#if 0
static void Hal_SetHDMIClk(DVI_PORT_TYPE eHDMIPort,MS_BOOL bClkPull)
{
   switch(eHDMIPort)
   {
       case DVI_PORT_0://A
           PM_W2BYTE(REG_PM_SLEEP_4B_L, bClkPull ? BIT(8) : 0, BMASK(8:8));
           break;
       case DVI_PORT_1://B
           PM_W2BYTE(REG_PM_SLEEP_4B_L, bClkPull ? BIT(9) : 0, BMASK(9:9));
           break;
       case DVI_PORT_3://D
           PM_W2BYTE(REG_PM_SLEEP_4B_L, bClkPull ? BIT(10) : 0, BMASK(10:10));
           break;
       case DVI_PORT_2://C
           PM_W2BYTE(REG_PM_SLEEP_4B_L, bClkPull ? BIT(11) : 0, BMASK(11:11));
           break;
       default: // For system first power on with the unknow HDMI port.
           PM_W2BYTE(REG_PM_SLEEP_4B_L, bClkPull ? BMASK(11:8) : 0, BMASK(11:8));
           break;
   }

}
#endif
static void SetHDMIClk(uint8_t eHDMIPort , MS_BOOL bDown)
{
    //printk("[SetHDMIClk]   eHDMIPort =0x%x \n",eHDMIPort);
    MS_U32 temp =0 ;
    MS_U8  u16Pulldownport =0;
    u16Pulldownport = (~eHDMIPort) &0xF;

    //printk("[SetHDMIClk]   Need pull down eHDMIPort  =0x%lx \n",u16Pulldownport);
    temp = ((u16Pulldownport & 0x8)>>1) |((u16Pulldownport & 0x4)<<1) | ( u16Pulldownport  & (BIT(1)|BIT(0))) ;
    //temp = (((~u16Pulldownport)>>2)<<2 )|( u16Pulldownport  & (BIT(1)|BIT(0))) ;
    temp = temp << 8 ;
    //printk("[SetHDMIClk]   final  eHDMIPort  =0x%lx \n",temp);
    MDrv_STR_Write2ByteMsk(PM_SLEEP, 0x4B, bDown?temp:0, 0x0F00 & temp);
    //printk("[SetHDMIClk]   value =0x%lx \n",bDown?temp:0);
    //printk("[SetHDMIClk]   Mask =0x%lx \n",0x0F00 & temp);
}
static void  StoreBank(void)
{
    STR_PRINT("----- [%s],StoreBank  enter\n",__DATE__);
    int Bank, reg;
    int pre_i;

    for(Bank = 0; Bank < BANK_NUM; Bank++)
    {
        //The Bank of  unuse and W-only don't Read the data from buffer
        if ((bank_need_backup[Bank][1] == UNUSE) ||(bank_need_backup[Bank][1] == W_ONLY))
        {
            continue ;
        }

          for(reg = 0; reg < REG_PERBANK_NUM ; reg++)
          {
            bank_backup[Bank][reg] = MDrv_STR_Read2Byte(bank_need_backup[Bank][0], reg);
#if DUMP_DATA
                if( Bank != pre_i)
                {
                    pre_i = Bank;
                    printk("\n\n\nhankw_Bank_backup[%x][%x]",Bank,reg);
                }

                if(Bank==0)
                    printk("\n [%x]=>",reg);
                else if( (reg % 16) == 0)
                    printk("\n [%x]=>",reg);

                    printk(" {%x}",(int)bank_backup[Bank][reg]);
#endif
          }
    }
}
static void Set_Skip_StoreCECCmd(void)
{
    bank_backup[HDMI_IDX][CEC_CMD_Addr] = CEC_CMD_VALUE;
    bank_backup[HDMI_IDX][CEC_LEN_Addr]  = (CEC_SAMPLE_SEL<< 4)&CEC_LEN_VALUE; //set CMD length to 0
}
static void  ReStoreBank(void)
{
    STR_PRINT("----- [%s],ReStoreBank  enter\n",__DATE__);
    MS_U16 Bank, reg;
    MS_U16 pre_i;

    for(Bank = 0; Bank < BANK_NUM; Bank++)
    {
        //The Bank of  unuse and R-only don't write the data from buffer
        if ((bank_need_backup[Bank][1] == UNUSE) ||(bank_need_backup[Bank][1] == R_ONLY))
          {
		  continue ;
          }

        for(reg = 0; reg < REG_PERBANK_NUM ; reg++)
          {
            if (bank_need_backup[Bank][0] == SC_BK20)
                {
                if ((reg>=0x31) && (reg<=0x32))
                    {
                        continue;
                    }
                }
                if(((Bank==XC_RELATED_BANKNUM+0xF) && (reg==0x18) ) || ((Bank==XC_RELATED_BANKNUM+0x10) && (reg==0x50)))
                {
                 MDrv_STR_Write2ByteMsk(bank_need_backup[Bank][0],reg,bank_backup[Bank][reg],0xFFFE);//bypass sub and main mlg gamma for demo
#if DUMP_DATA
                    if( Bank != pre_i)
                    {
                        pre_i = Bank;
                        printk("\nhankw_Bank_Restore_bypass[%x][%x]",Bank,reg);
                    }

                    if(Bank==0)
                        printk("\n [%x]=>",reg);
                    else if( (reg % 16) == 0)
                        printk("\n [%x]=>",reg);

                        printk(" {%x}",(int)bank_backup[Bank][reg]);
#endif
                }
                else
		        {
                    MDrv_STR_Write2Byte(bank_need_backup[Bank][0], reg, bank_backup[Bank][reg]);
#if DUMP_DATA
                    if( Bank != pre_i)
                    {
                        pre_i = Bank;
                        printk("\nhankw_Bank_Restore_nobypass[%x][%x]",Bank,reg);
                    }

                    if(Bank==0)
                        printk("\n [%x]=>",reg);
                    else if( (reg % 16) == 0)
                        printk("\n [%x]=>",reg);

                        printk(" {%x}",(int)bank_backup[Bank][reg]);
#endif
                }
          }
    }
    // Chiptop
    MDrv_STR_Write2Byte(ADC_CHIPTOP,0x12, bank_backup[ADC_CHIPTOP_IDX][0x12]); //ADC_CHIPTOP

    //Clkgen
    MDrv_STR_Write2Byte(CLKGEN,0x50, bank_backup[CLKGEN_IDX][0x50]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x51, bank_backup[CLKGEN_IDX][0x51]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x52, bank_backup[CLKGEN_IDX][0x52]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x53, bank_backup[CLKGEN_IDX][0x53]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x54, bank_backup[CLKGEN_IDX][0x54]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x55, bank_backup[CLKGEN_IDX][0x55]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x56, bank_backup[CLKGEN_IDX][0x56]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x57, bank_backup[CLKGEN_IDX][0x57]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x58, bank_backup[CLKGEN_IDX][0x58]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x59, bank_backup[CLKGEN_IDX][0x59]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x5E, bank_backup[CLKGEN_IDX][0x5E]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x63, bank_backup[CLKGEN_IDX][0x63]); //CLKGEN
    MDrv_STR_Write2Byte(CLKGEN,0x26, bank_backup[CLKGEN_IDX][0x26]); //CLKGEN

}

static void StoreSubBank(void)
{
    STR_PRINT("----- [%s],StoreSubBank  enter\n",__DATE__);
    int Bank, reg;
    int pre_i = 0xff;
    for(Bank = 0; Bank < SUBBANK_NUM; Bank++)
    {
          for(reg = 0; reg < REG_PERBANK_NUM ; reg++)
          {
                if(reg == 0)
                {
                    MDrv_STR_Write2Byte(subbank_need_backup[Bank][0], 0x0, subbank_need_backup[Bank][1]);
                    continue;
                }
                subbank_backup[Bank][reg] = MDrv_STR_Read2Byte(subbank_need_backup[Bank][0], reg);
#if DUMP_DATA
                if( Bank != pre_i)
                {
                    pre_i = Bank;
                    printk("\n\n\nhankw_subbank_backup[%x][%x]",Bank,reg);
                }

                if(Bank==0)
                    printk("\n [%x]=>",reg);
                else if( (reg % 16) == 0)
                    printk("\n [%x]=>",reg);
                else
                    printk(" {%x}",(int)subbank_backup[Bank][reg]);
#endif
          }
    }
}

static void ReStoreSubBank(void)
{
    STR_PRINT("----- [%s],ReStoreSubBank  enter\n",__DATE__);
    int Bank, reg;
    int pre_i=0xff;
    for(Bank = 0; Bank < SUBBANK_NUM; Bank++)
    {
          for(reg = 0; reg < REG_PERBANK_NUM ; reg++)
          {
                if(reg == 0)
                {
                    MDrv_STR_Write2Byte(subbank_need_backup[Bank][0], 0x0, subbank_need_backup[Bank][1]);
                    continue;
                }
                MDrv_STR_Write2Byte(subbank_need_backup[Bank][0], reg, subbank_backup[Bank][reg]);
#if DUMP_DATA
                if( Bank != pre_i)
                {
                    pre_i = Bank;
                    printk("\n\n\nhankw_subbank_Restore[%x][%x]",Bank,reg);
                }

                if(Bank==0)
                    printk("\n [%x]=>",reg);
                else if( (reg % 16) == 0)
                    printk("\n [%x]=>",reg);
                else
                    printk(" {%x}",(int)subbank_backup[Bank][reg]);
#endif
          }

          //After restoring the registers ,need to switch the sub bank 0.
          //Bank == 1 : LPLL Sub bank 1
          //Bank == 1 : MOD sub bank 1
          if(Bank == 1)                //if(i == Bank || Bank == 3)  CID:117617
          {
              MDrv_STR_Write2Byte(subbank_need_backup[Bank][0], 0x0, 0x0);
              continue;
          }
    }
}

static void ReStoreBWTable(void)
{
    /* BWTABLE_COM
      { REG(0x101249), 0xFF, 0xFF, 0xFF },
      { REG(0x101268), 0xFF, 0xFF, 0xFF },
      { REG(0x101269), 0xFF, 0xFF, 0xF1 },

      { REG(0x1012FE), 0xFF, 0xFF, 0xE1 },
      { REG(0x1012FF), 0xFF, 0xFF, 0x00 },
      { REG(0x1012FF), 0xFF, 0x01, 0x01 },

      { REG(0x100668), 0xFF, 0xFF, 0xD7 },
      { REG(0x100669), 0xFF, 0xFF, 0xFF },

      { REG(0x1006FE), 0xFF, 0xFF, 0xE1 },
      { REG(0x1006FF), 0xFF, 0xFF, 0x20 },
      { REG(0x1006FF), 0xFF, 0x01, 0x01 },

      { REG(0x1615E2), 0xFF, 0xFF, 0x0C },
      { REG(0x1615F8), 0xFF, 0xFF, 0x72 },
      { REG(0x1615E2), 0xFF, 0xFF, 0x0E },

      { REG(0x1622E2), 0xFF, 0xFF, 0x0C },
      { REG(0x1622F8), 0xFF, 0xFF, 0x72 },
      { REG(0x1622E2), 0xFF, 0xFF, 0x0E },
      */

    MDrv_STR_Write2ByteMsk(MIU0_BASE, (0x24), (bank_backup[MIU0_BASE_IDX][0x24]),0xFF00);
    MDrv_STR_Write2ByteMsk(MIU0_BASE, (0x34), (bank_backup[MIU0_BASE_IDX][0x34]),0xFFFF);
    MDrv_STR_Write2ByteMsk(MIU0_BASE, (0x7F), (bank_backup[MIU0_BASE_IDX][0x7F]),0x00FF);

    //toggle to update settings
    MDrv_STR_Write2ByteMsk(MIU0_BASE, (0x7F), 0x00 ,0xFF00);
    MDrv_STR_Write2ByteMsk(MIU0_BASE, (0x7F), (bank_backup[MIU0_BASE_IDX][0x7F]),0x0100);

    MDrv_STR_Write2ByteMsk(MIU1_BASE, (0x34), (bank_backup[MIU1_BASE_IDX][0x34]),0xFFFF);
    MDrv_STR_Write2ByteMsk(MIU1_BASE, (0x7F), (bank_backup[MIU1_BASE_IDX][0x7F]),0x00FF);

    //toggle to update settings
    MDrv_STR_Write2ByteMsk(MIU1_BASE, (0x7F), 0x20 ,0xFF00);
    MDrv_STR_Write2ByteMsk(MIU1_BASE, (0x7F), (bank_backup[MIU1_BASE_IDX][0x7F]),0x0100);


    //toggle to update settings
    MDrv_STR_Write2ByteMsk(MIU_ARB, (0x71), 0x0C,0x00FF);
    MDrv_STR_Write2ByteMsk(MIU_ARB, (0x7C), (bank_backup[MIU_ARB_IDX][0x7C]),0x00FF);
    MDrv_STR_Write2ByteMsk(MIU_ARB, (0x71), (bank_backup[MIU_ARB_IDX][0x71]),0x00FF);

    MDrv_STR_Write2ByteMsk(MIU_ARB2, (0x71), 0x0C,0x00FF);
    MDrv_STR_Write2ByteMsk(MIU_ARB2, (0x7C), (bank_backup[MIU_ARB2_IDX][0x7C]),0x00FF);
    MDrv_STR_Write2ByteMsk(MIU_ARB2, (0x71), (bank_backup[MIU_ARB2_IDX][0x71]),0x00FF);
}

static void ReStoreYSram(void)
{
    STR_PRINT("----- [%s],ReStoreYSram  enter\n",__DATE__);
     int Bank;
     MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x0000, 0x1000);
     MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0001, 0x0001); // enable y_sram_rw
     for(Bank = 0; Bank<512 ; Bank++)
     {
        while(MDrv_STR_Read2Byte(SC_BK23, 0x41) & 0x0100)
        {
            //printk("REG_SC_BK23_41_L=%u.\n",MDrv_STR_Read2Byte(SC_BK23, 0x41));
        };
        MDrv_STR_Write2ByteMsk(SC_BK23, 0x42, Bank, 0x01FF);
        udelay(50);
        MDrv_STR_Write2Byte(SC_BK23, 0x43, YSram_backup[Bank][0]);
        MDrv_STR_Write2Byte(SC_BK23, 0x44, YSram_backup[Bank][1]);
        MDrv_STR_Write2Byte(SC_BK23, 0x45, YSram_backup[Bank][2]);
        MDrv_STR_Write2Byte(SC_BK23, 0x49, YSram_backup[Bank][3]);
        //MDrv_STR_Write2Byte(SC_BK23, 0x4A, YSram_backup[Bank][4]);
#if DUMP_DATA
        if( (Bank % 4) == 0)
            printk("\n");
        else
            printk("    [%x] = {%lx %lx %lx %lx}",Bank,YSram_backup[Bank][0],YSram_backup[Bank][1],YSram_backup[Bank][2],YSram_backup[Bank][3]);
            //printk("    [%x] = {%lx %lx %lx %lx %lx}",Bank,YSram_backup[Bank][4],YSram_backup[Bank][3],YSram_backup[Bank][2],YSram_backup[Bank][1],YSram_backup[Bank][0]);
#endif


        MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0100, 0x0100);
     }
    MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x00, 0x007F);
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x1000, 0x1000);
}


static void StoreYSram(void)
{
    STR_PRINT("----- [%s],StoreYSram  enter\n",__DATE__);
    int i;
    MS_U16 u16Temp;
    MS_U16 u16VStart_De, u16HStart_De, u16VEnd_De, u16HEnd_De;
    MS_U16 u16VStart_Disp, u16HStart_Disp, u16VEnd_Disp, u16HEnd_Disp;

/*
    u16VStart_De = (MDrv_STR_Read2Byte(SC_BK10, 0x06) & 0x0FFF);
    u16VEnd_De   = (MDrv_STR_Read2Byte(SC_BK10, 0x07) & 0x0FFF);
    u16HStart_De = (MDrv_STR_Read2Byte(SC_BK10, 0x04) & 0x0FFF);
    u16HEnd_De   = (MDrv_STR_Read2Byte(SC_BK10, 0x05) & 0x0FFF);
    u16VStart_Disp = (MDrv_STR_Read2Byte(SC_BK10, 0x0A) & 0x0FFF);
    u16VEnd_Disp   = (MDrv_STR_Read2Byte(SC_BK10, 0x0B) & 0x0FFF);
    u16HStart_Disp = (MDrv_STR_Read2Byte(SC_BK10, 0x08) & 0x0FFF);
    u16HEnd_Disp   = (MDrv_STR_Read2Byte(SC_BK10, 0x09) & 0x0FFF);

    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0A, u16VStart_De, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0B, u16VEnd_De, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x08, u16HStart_De, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x09, u16HEnd_De, 0x0FFF);
*/
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x0000, 0x1000);
    MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0009, 0x0009); // enable y_sram_rw and select read.
    for(i=0;i<512;i++)
    {
        while(MDrv_STR_Read2Byte(SC_BK23, 0x41) & 0x0200)
        {
            //printk("REG_SC_BK23_41_L=%u.\n",MDrv_STR_Read2Byte(SC_BK23, 0x41));
        };
        MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0200, 0x0200);

        MDrv_STR_Write2ByteMsk(SC_BK23, 0x42, i, 0x01FF);

        //MsOS_DelayTaskUs(100);
        udelay(100);

        u16Temp = MDrv_STR_Read2Byte(SC_BK23, 0x46);
        YSram_backup[i][0]= u16Temp;
        u16Temp = MDrv_STR_Read2Byte(SC_BK23, 0x47);
        YSram_backup[i][1]= u16Temp;
        u16Temp = MDrv_STR_Read2Byte(SC_BK23, 0x48);
        YSram_backup[i][2]= u16Temp;
        u16Temp = MDrv_STR_Read2Byte(SC_BK23,0x4A);
        YSram_backup[i][3]= u16Temp;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n");
        else
            printk("    [%x] = {%lx %lx %lx %lx}",i,YSram_backup[i][0],YSram_backup[i][1],YSram_backup[i][2],YSram_backup[i][3]);
#endif
    }

    MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x00, 0x007F);
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x1000, 0x1000);
/*
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0A, u16VStart_Disp, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0B, u16VEnd_Disp, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x08, u16HStart_Disp, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x09, u16HEnd_Disp, 0x0FFF);
 */
}

static void ReStoreCSram(void)
{
    STR_PRINT("----- [%s],ReStoreCSram  enter\n",__DATE__);
     int i, addr;
     MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x0000, 0x1000);
     MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0002, 0x0002); // enable c_sram_rw
     for(i = 0; i<256 ; i++)
     {
        if(i >= 128)
             addr = i + 0x100;
        else
            addr = i;
        while(MDrv_STR_Read2Byte(SC_BK23, 0x41) & 0x0100)
        {
           // printk("REG_SC_BK23_41_L=%u.\n",MDrv_STR_Read2Byte(SC_BK23, 0x41));
        };
        MDrv_STR_Write2ByteMsk(SC_BK23, 0x42, addr, 0x01FF);
        udelay(50);
        MDrv_STR_Write2Byte(SC_BK23, 0x43, CSram_backup[i][0]);
        MDrv_STR_Write2Byte(SC_BK23, 0x44, CSram_backup[i][1]);
        MDrv_STR_Write2ByteMsk(SC_BK23, 0x45, CSram_backup[i][2], 0x00FF);
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n");
        else
            printk("    [%x] = {%lx %lx %lx}",i,YSram_backup[i][0],YSram_backup[i][1],YSram_backup[i][2]);
#endif


        MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0100, 0x0100);
     }
    MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x00, 0x007F);
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x1000, 0x1000);
}


static void StoreCSram(void)
{
    STR_PRINT("----- [%s],StoreCSram  enter\n",__DATE__);
    int i, addr;
    MS_U16 u16Temp;
    MS_U16 u16VStart_De, u16HStart_De, u16VEnd_De, u16HEnd_De;
    MS_U16 u16VStart_Disp, u16HStart_Disp, u16VEnd_Disp, u16HEnd_Disp;

    u16VStart_De = (MDrv_STR_Read2Byte(SC_BK10, 0x06) & 0x0FFF);
    u16VEnd_De   = (MDrv_STR_Read2Byte(SC_BK10, 0x07) & 0x0FFF);
    u16HStart_De = (MDrv_STR_Read2Byte(SC_BK10, 0x04) & 0x0FFF);
    u16HEnd_De   = (MDrv_STR_Read2Byte(SC_BK10, 0x05) & 0x0FFF);
    u16VStart_Disp = (MDrv_STR_Read2Byte(SC_BK10, 0x0A) & 0x0FFF);
    u16VEnd_Disp   = (MDrv_STR_Read2Byte(SC_BK10, 0x0B) & 0x0FFF);
    u16HStart_Disp = (MDrv_STR_Read2Byte(SC_BK10, 0x08) & 0x0FFF);
    u16HEnd_Disp   = (MDrv_STR_Read2Byte(SC_BK10, 0x09) & 0x0FFF);

    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0A, u16VStart_De, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0B, u16VEnd_De, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x08, u16HStart_De, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x09, u16HEnd_De, 0x0FFF);

    MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x0000, 0x1000);
    MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x000A, 0x000A); // enable c_sram_rw
    for(i=0;i<256;i++)
    {
        if(i >= 128)
             addr = i + 0x100;
        else
            addr = i;

        while(MDrv_STR_Read2Byte(SC_BK23, 0x41) & 0x0200)
        {
            //printk("REG_SC_BK23_41_L=%u.\n",MDrv_STR_Read2Byte(SC_BK23, 0x41));
        };
        MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x0200, 0x0200);
        MDrv_STR_Write2ByteMsk(SC_BK23, 0x42, addr, 0x01FF);

        //MsOS_DelayTaskUs(100);
        udelay(100);

        u16Temp = MDrv_STR_Read2Byte(SC_BK23, 0x46);
        CSram_backup[i][0]= u16Temp;
        u16Temp = MDrv_STR_Read2Byte(SC_BK23, 0x47);
        CSram_backup[i][1]= u16Temp;
        u16Temp = MDrv_STR_Read2Byte(SC_BK23, 0x48);
        CSram_backup[i][2]= u16Temp;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n");
        else
            printk("    [%x] = {%lx %lx %lx}",i,YSram_backup[i][0],YSram_backup[i][1],YSram_backup[i][2]);
#endif

    }

    MDrv_STR_Write2ByteMsk(SC_BK23, 0x41, 0x00, 0x007F);
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x03, 0x1000, 0x1000);

    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0A, u16VStart_Disp, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x0B, u16VEnd_Disp, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x08, u16HStart_Disp, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK10, 0x09, u16HEnd_Disp, 0x0FFF);
}


static void ReStore_IHC_SRAM(void)
{
    STR_PRINT("----- [%s],ReStore_IHC_SRAM  enter\n",__DATE__);
     int i;
     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, BIT(0), BIT(0)); // io_en disable
     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 0 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM1_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, SRAM1_IHC_backup[i], 0x01FF); //data[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, BIT(15), BIT(15)); // io_w enable
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM1_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 1 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM2_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, SRAM2_IHC_backup[i], 0x01FF); //data[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, BIT(15), BIT(15)); // io_w enable
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM2_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 2 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM3_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, SRAM3_IHC_backup[i], 0x01FF); //data[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, BIT(15), BIT(15)); // io_w enable
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM3_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 3 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM4_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, SRAM4_IHC_backup[i], 0x01FF); //data[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7E, BIT(15), BIT(15)); // io_w enable
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM4_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 0, BIT(0)); // io_en enable
}

static void Store_IHC_SRAM(void)
{
    STR_PRINT("----- [%s],Store_IHC_SRAM  enter\n",__DATE__);
     int i;
     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, BIT(0), BIT(0)); // io_en disable
     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 0 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM1_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, BIT(15), BIT(15)); // io_r enable
           SRAM1_IHC_backup[i] = MDrv_STR_Read2Byte(SC_BK1C, 0x7F) & 0x01FF;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM1_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 1 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM2_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, BIT(15), BIT(15)); // io_r enable
           SRAM2_IHC_backup[i] = MDrv_STR_Read2Byte(SC_BK1C, 0x7F) & 0x01FF;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM2_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 2 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM3_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, BIT(15), BIT(15)); // io_r enable
           SRAM3_IHC_backup[i] = MDrv_STR_Read2Byte(SC_BK1C, 0x7F) & 0x01FF;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM3_IHC_backup[i]);
#endif
     }

     MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7C, 3 <<1, BIT(2)|BIT(1)); // sram select
     for(i = 0;i< SRAM4_IHC_COUNT; i++)
     {
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, i, 0x01FF); // address[8:0]
           MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7D, BIT(15), BIT(15)); // io_r enable
           SRAM4_IHC_backup[i] = MDrv_STR_Read2Byte(SC_BK1C, 0x7F) & 0x01FF;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM4_IHC_backup[i]);
#endif
     }
}

static void ReStore_ICC_SRAM(void)
{
    STR_PRINT("----- [%s],ReStore_ICC_SRAM  enter\n",__DATE__);
    int i;
    MDrv_STR_Write2ByteMsk(SC_BK1C, 0x78, BIT(0), BIT(0)); // io_en enable
    for(i = 0;i< SRAM_ICC_COUNT; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK1C, 0x79, i, 0x03FF); // address
        MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7A, SRAM_ICC_backup[i] , 0x01FF); //data
        MDrv_STR_Write2ByteMsk(SC_BK1C, 0x7A, BIT(15), BIT(15)); // io_w enable
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM_ICC_backup[i]);
#endif
    }
    MDrv_STR_Write2ByteMsk(SC_BK1C, 0x78, 0, BIT(0)); // io_en enable
}

static void Store_ICC_SRAM(void)
{
    STR_PRINT("----- [%s],Store_ICC_SRAM  enter\n",__DATE__);
    int i;
    MDrv_STR_Write2ByteMsk(SC_BK1C, 0x78, BIT(0), BIT(0)); // io_en enable
    for(i = 0;i< SRAM_ICC_COUNT; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK1C, 0x79, i, 0x03FF); // address
        MDrv_STR_Write2ByteMsk(SC_BK1C, 0x79, BIT(15), BIT(15)); // io_w enable
        SRAM_ICC_backup[i] = MDrv_STR_Read2Byte(SC_BK1C, 0x7b) & 0x01FF;
#if DUMP_DATA
        if( (i % 4) == 0)
            printk("\n}");
        else
            printk("    [%x] = {%lx }",i,SRAM_ICC_backup[i]);
#endif
    }
    MDrv_STR_Write2ByteMsk(SC_BK1C, 0x78, 0, BIT(0)); // io_en enable
}

static void ReStore_XVYCC_SRAM(void)
{
    STR_PRINT("----- [%s %s],ReStore_XVYCC_SRAM  enter\n",__DATE__,__TIME__);
/*
    int i;

    MDrv_STR_Write2ByteMsk(SC_BK25, 0x01, 0, BIT(0)|BIT(2));
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x11, BIT(8), BIT(8));
    MDrv_STR_Write2ByteMsk(SC_BK2D, 0x60, BIT(5)|BIT(6), 0xF8); // io_en disable
    for(i=0; i<SRAM_XVYCC_COUNT; i++)
    {
        // waiting ready.
        while (MDrv_STR_Read2Byte(SC_BK2D, 0x60) & BIT(3)){;}

        MDrv_STR_Write2Byte(SC_BK2D, 0x61, i); // address
        MDrv_STR_Write2Byte(SC_BK2D, 0x62, SRAM_XVYCC_R_backup[i][0]); //data low btye
        MDrv_STR_Write2ByteMsk(SC_BK2D, 0x63, SRAM_XVYCC_R_backup[i][1], 0x01FF); //data high byte
        MDrv_STR_Write2ByteMsk(SC_BK2D, 0x60, BIT(3), BIT(3)); // io_w enable
    }
    while (MDrv_STR_Read2Byte(SC_BK2D, 0x60) & BIT(3)){;}
    //MDrv_STR_Write2ByteMsk(SC_BK2D, 0x60, 0, 0xFFFF); // Disable Write

    //  start XVYCC GAMMA
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x78, BIT(5)|BIT(6), 0xF8); // io_en disable
    for(i=0; i<SRAM_XVYCC_GAMMA_COUNT; i++)
    {
        // waiting ready.
        while (MDrv_STR_Read2Byte(SC_BK25, 0x78) & BIT(3));

        MDrv_STR_Write2Byte(SC_BK25, 0x79, i); // address
        MDrv_STR_Write2Byte(SC_BK25, 0x7A, SRAM_XVYCC_GAMMA_R_backup[i]); //data
        MDrv_STR_Write2ByteMsk(SC_BK25, 0x78, BIT(3), BIT(3)); // io_w enable
    }
    while (MDrv_STR_Read2Byte(SC_BK25, 0x78) & BIT(3)){;}
    //MDrv_STR_Write2ByteMsk(SC_BK25, 0x78, 0, 0xFFFF); // io_en enable

    MDrv_STR_Write2ByteMsk(SC_BK25, 0x01, u16DeGammaStatus, BIT(0)|BIT(2));
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x11, u16XVYCCBypassEn, BIT(8));
    */
}

static void Store_XVYCC_SRAM(void)
{
    STR_PRINT("----- [%s %s],Store_XVYCC_SRAM  enter\n",__DATE__,__TIME__);
/*
    int i;

    u16DeGammaStatus = MDrv_STR_Read2Byte(SC_BK25, 0x01);
    u16XVYCCBypassEn = MDrv_STR_Read2Byte(SC_BK25, 0x11);

    //while (MDrv_STR_Read2Byte(SC_BK2D, 0x60) & BIT(4)){;} // waiting ready
    //Select R Channel
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x01, 0, BIT(0)|BIT(2));
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x11, BIT(8), BIT(8));
    MDrv_STR_Write2ByteMsk(SC_BK2D, 0x60, 0, 0xF8); // io_en disable
    for(i=0; i<SRAM_XVYCC_COUNT; i++)
    {
        MDrv_STR_Write2Byte(SC_BK2D, 0x61, i); // address
        MDrv_STR_Write2ByteMsk(SC_BK2D, 0x60, BIT(4), BIT(4));
        // waiting ready.
        while (MDrv_STR_Read2Byte(SC_BK2D, 0x60) & BIT(4)){;}

        SRAM_XVYCC_R_backup[i][0] = MDrv_STR_Read2Byte(SC_BK2D, 0x64);
        SRAM_XVYCC_R_backup[i][1] = MDrv_STR_Read2Byte(SC_BK2D, 0x65);
    }
    while (MDrv_STR_Read2Byte(SC_BK2D, 0x60) & BIT(4)){;}
    //MDrv_STR_Write2ByteMsk(SC_BK2D, 0x60, 0, 0xFFFF); // Disable Write


    //  start XVYCC GAMMA
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x78, 0, 0xF8); // io_en disable
    for(i=0; i<SRAM_XVYCC_GAMMA_COUNT; i++)
    {
        MDrv_STR_Write2Byte(SC_BK25, 0x79, i); // address
        MDrv_STR_Write2ByteMsk(SC_BK25, 0x78, BIT(4), BIT(4));
        // waiting ready.
        while (MDrv_STR_Read2Byte(SC_BK25, 0x78) & BIT(4)) {;}

        SRAM_XVYCC_GAMMA_R_backup[i] = MDrv_STR_Read2Byte(SC_BK25, 0x7D);
    }
    while (MDrv_STR_Read2Byte(SC_BK25, 0x78) & BIT(4)){;}
    //MDrv_STR_Write2ByteMsk(SC_BK25, 0x78, 0, 0xFFFF); // io_en enable

    MDrv_STR_Write2ByteMsk(SC_BK25, 0x01, u16DeGammaStatus, BIT(0)|BIT(2));
    MDrv_STR_Write2ByteMsk(SC_BK25, 0x11, u16XVYCCBypassEn, BIT(8));
*/
}

static void ReStoreGammaSram(void)
{
    STR_PRINT("----- [%s],ReStoreGammaSram  enter\n",__DATE__);
    int i,channel;
    MS_U16 u16Delay = 0xFFFF;

    for(i=0; i<GAMMASRAM_SIZE; i++)
    {
        while (MDrv_STR_Read2ByteMsk(SC_BK10, 0x6D,0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
        MDrv_STR_Write2ByteMsk(SC_BK10, 0x6C,GammaSram_backup[i][GAMMASRAM_ADD]  ,0xFF);                          // get address port

        for(channel=GAMMASRAM_R; channel<GAMMASRAM_MAX; channel++)
        {
            MDrv_STR_Write2ByteMsk(SC_BK10,(0x6E + (channel-1) ), GammaSram_backup[i][channel], 0xFFF);      // Set channel data

            // kick off write
            switch(channel)
            {
                case GAMMASRAM_R:  // Red
                    MDrv_STR_Write2ByteMsk(SC_BK10,0x6D, BIT(5), BIT(5));
                    break;

                case GAMMASRAM_G:  // Green
                    MDrv_STR_Write2ByteMsk(SC_BK10,0x6D, BIT(6), BIT(6));
                    break;

                case GAMMASRAM_B:  // Blue
                    MDrv_STR_Write2ByteMsk(SC_BK10,0x6D, BIT(7), BIT(7));
                    break;
            }
#if DUMP_DATA
            if( (i % 4) == 0)
                printk("\n}");
            //else
            {
                if(( channel % 4) == 3)
                    printk("    [%x] = {[%x] %x %x %x}",i,GammaSram_backup[i][GAMMASRAM_ADD],
                        GammaSram_backup[i][GAMMASRAM_R],GammaSram_backup[i][GAMMASRAM_G],GammaSram_backup[i][GAMMASRAM_B]);
            }
 #endif
        }
        while (MDrv_STR_Read2ByteMsk(SC_BK10,0x6D, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
    }

}

static void StoreGammaSram(void)
{
    STR_PRINT("----- [%s],StoreGammaSram  enter\n",__DATE__);
    int i,channel;

    MS_U16 u16Delay = 0xFFFF;

    for(i=0; i<GAMMASRAM_SIZE; i++)
    {
        while (MDrv_STR_Read2ByteMsk(SC_BK10, 0x6D,0xE000) && (--u16Delay));          // Check whether the read chanel is ready
        GammaSram_backup[i][GAMMASRAM_ADD] = i;
        MDrv_STR_Write2ByteMsk(SC_BK10, 0x6C,GammaSram_backup[i][GAMMASRAM_ADD]  ,0xFF);                          // get address port

        for(channel=GAMMASRAM_R; channel<GAMMASRAM_MAX; channel++)
        {
            // kick off write
            switch(channel)
            {
                case GAMMASRAM_R:  // Red
                    MDrv_STR_Write2ByteMsk(SC_BK10,0x6D, BIT(13), BIT(13));
                    break;

                case GAMMASRAM_G:  // Green
                    MDrv_STR_Write2ByteMsk(SC_BK10,0x6D, BIT(14), BIT(14));
                    break;

                case GAMMASRAM_B:  // Blue
                    MDrv_STR_Write2ByteMsk(SC_BK10,0x6D, BIT(15), BIT(15));
                    break;
            }

            while (MDrv_STR_Read2ByteMsk(SC_BK10, 0x6D,0xE000) && (--u16Delay));          // Check whether the read chanel is ready
            GammaSram_backup[i][channel] = MDrv_STR_Read2ByteMsk(SC_BK10,(0x71 +( channel-1) ), 0xFFF);      // Get channel data
#if DUMP_DATA
            if( (i % 4) == 0)
            {
                printk("\n}");
            }
            //else
            {
                if(( channel % 4) == 3)
                    printk("    [%x] = {[%x] %x %x %x}",i,GammaSram_backup[i][GAMMASRAM_ADD],
                            GammaSram_backup[i][GAMMASRAM_R],GammaSram_backup[i][GAMMASRAM_G],GammaSram_backup[i][GAMMASRAM_B]);
            }
#endif
        }
        while (MDrv_STR_Read2ByteMsk(SC_BK10,0x6D, 0xE000) && (--u16Delay));          // Check whether the Write chanel is ready
    }

}

//static MS_U16 u16OverDriveStatus = 0;
//static MS_U16 u16OverDriveInit[15] = {};
//static MS_U8  u8OverDriveTable[1089] ={};

static void ReStoreOverDriver(void)
{
#if 0
    //step 1, init OD RIU setting
    MDrv_STR_Write2Byte(SC_BK16, 0x15, u16OverDriveInit[0]);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x16, u16OverDriveInit[1], 0x03FF);
    MDrv_STR_Write2Byte(SC_BK16, 0x17, u16OverDriveInit[2]);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x18, u16OverDriveInit[3], 0x03FF);
    MDrv_STR_Write2Byte(SC_BK16, 0x39, u16OverDriveInit[4]);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x3A, u16OverDriveInit[5], 0x00FF);
    MDrv_STR_Write2Byte(SC_BK16, 0x3B, u16OverDriveInit[6]);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x3C, u16OverDriveInit[7], 0x00FF);
    MDrv_STR_Write2Byte(SC_BK16, 0x4F, u16OverDriveInit[8]);
    MDrv_STR_Write2Byte(SC_BK16, 0x50, u16OverDriveInit[9]);
    MDrv_STR_Write2Byte(SC_BK16, 0x51, u16OverDriveInit[10]);
    MDrv_STR_Write2Byte(SC_BK16, 0x1A, u16OverDriveInit[11]);
    MDrv_STR_Write2Byte(SC_BK16, 0x1C, u16OverDriveInit[12]);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x3A, u16OverDriveInit[13], 0x7F00);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x12, u16OverDriveInit[14], 0x00FF);
#endif
    // step 2, re store OD table sram
    MS_U16 i;

#if 1
    //u16OverDriveStatus = MDrv_STR_Read2Byte(SC_BK16, 0x10);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x10, 0x00, BIT(0)); // OD disable
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x01, 0x0E, 0x0E); // OD table SRAM enable, RGB channel
    for (i=0; i<289; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x03, u8OverDriveTable[i], 0x00FF);
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x02, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x02, BIT(15)));
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x06, u8OverDriveTable[i+289], 0x00FF);
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x05, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x05, BIT(15)));
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x09, u8OverDriveTable[i+289+272], 0x00FF);
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x08, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x08, BIT(15)));
    }
    for (i=0; i<256; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x0C, u8OverDriveTable[i+289+272+272], 0x00FF);
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x0B, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x0B, BIT(15)));
    }
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x01, 0x0, 0x000E); // OD table SRAM enable, RGB channel
    MDrv_STR_Write2Byte(SC_BK16, 0x10, u16OverDriveStatus); // OD disable
#else
    MDrv_STR_WriteByteMsk(SC_BK16, 0x10, 0x00, BIT(0)); // OD disable
    MDrv_STR_WriteByteMsk(SC_BK16, 0x01, 0x0E, 0x0E); // OD table SRAM enable, RGB channel
    for (i=0; i<289; i++)
    {
        MDrv_STR_WriteByte(SC_BK16, 0x03, u8OverDriveTable[i]);
        MDrv_STR_WriteByteMsk(SC_BK16, 0x02, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x02, BIT(15)));
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_WriteByte(SC_BK16, 0x06, u8OverDriveTable[i+289]);
        MDrv_STR_WriteByteMsk(SC_BK16, 0x05, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x05, BIT(15)));
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_WriteByte(SC_BK16, 0x09, u8OverDriveTable[i+289+272]);
        MDrv_STR_WriteByteMsk(SC_BK16, 0x08, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x08, BIT(15)));
    }
    for (i=0; i<256; i++)
    {
        MDrv_STR_WriteByte(SC_BK16, 0x0C, u8OverDriveTable[i+289+272+272]);
        MDrv_STR_WriteByteMsk(SC_BK16, 0x0B, (i|0x8000), 0x81FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x0B, BIT(15)));
    }
    MDrv_STR_WriteByteMsk(SC_BK16, 0x01, 0x0, 0x0E); // OD table SRAM enable, RGB channel

#endif
    //step 3
    //MDrv_STR_Write2ByteMsk(SC_BK16, 0x10, u16OverDriveStatus, 0x002F);
}


static void StoreOverDriver(void)
{
    // step 1
    //u16OverDriveStatus = MDrv_STR_Read2ByteMsk(SC_BK16, 0x10, 0x002F);
#if 0
    //step 2, store OD RIU setting
    u16OverDriveInit[0] = MDrv_STR_Read2Byte(SC_BK16, 0x15);
    u16OverDriveInit[1] = MDrv_STR_Read2ByteMsk(SC_BK16, 0x16, 0x03FF);
    u16OverDriveInit[2] = MDrv_STR_Read2Byte(SC_BK16, 0x17);
    u16OverDriveInit[3] = MDrv_STR_Read2ByteMsk(SC_BK16, 0x18, 0x03FF);
    u16OverDriveInit[4] = MDrv_STR_Read2Byte(SC_BK16, 0x39);
    u16OverDriveInit[5] = MDrv_STR_Read2ByteMsk(SC_BK16, 0x3A, 0x00FF);
    u16OverDriveInit[6] = MDrv_STR_Read2Byte(SC_BK16, 0x3B);
    u16OverDriveInit[7] = MDrv_STR_Read2ByteMsk(SC_BK16, 0x3C, 0x00FF);
    u16OverDriveInit[8] = MDrv_STR_Read2Byte(SC_BK16, 0x4F);
    u16OverDriveInit[9] = MDrv_STR_Read2Byte(SC_BK16, 0x50);
    u16OverDriveInit[10] = MDrv_STR_Read2Byte(SC_BK16, 0x51);
    u16OverDriveInit[11] = MDrv_STR_Read2Byte(SC_BK16, 0x1A);
    u16OverDriveInit[12] = MDrv_STR_Read2Byte(SC_BK16, 0x1C);
    u16OverDriveInit[13] = MDrv_STR_Read2ByteMsk(SC_BK16, 0x3A, 0x7F00);
    u16OverDriveInit[14] = MDrv_STR_Read2ByteMsk(SC_BK16, 0x12, 0x00FF);
#endif
    //step 3, dump OverDrive table Sram
    MS_U16 i;

#if 1
    u16OverDriveStatus = MDrv_STR_Read2Byte(SC_BK16, 0x10);
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x10, 0x00, BIT(0)); // OD disable
    MDrv_STR_Write2ByteMsk(SC_BK16, 0x01, 0x0E, 0x0E); // OD table SRAM enable, RGB channel

    for (i=0; i<289; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x02, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x02, BIT(14)));

        u8OverDriveTable[i] = MDrv_STR_Read2Byte(SC_BK16, 0x04);
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x05, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x05, BIT(14)));

        u8OverDriveTable[i+289] = MDrv_STR_Read2Byte(SC_BK16, 0x07);
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x08, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x08, BIT(14)));

        u8OverDriveTable[i+289+272] = MDrv_STR_Read2Byte(SC_BK16, 0x0A);
    }
    for (i=0; i<256; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x0B, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x0B, BIT(14)));

        u8OverDriveTable[i+289+272+272] = MDrv_STR_Read2Byte(SC_BK16, 0x0D);
    }

    MDrv_STR_Write2ByteMsk(SC_BK16, 0x01, 0x00, 0x0E); // OD table SRAM enable, RGB channel
    MDrv_STR_Write2Byte(SC_BK16, 0x10, u16OverDriveStatus); // OD disable
#else
    MDrv_STR_WriteByteMsk(SC_BK16, 0x10, 0x00, BIT(0)); // OD disable
    MDrv_STR_WriteByteMsk(SC_BK16, 0x01, 0x0E, 0x0E); // OD table SRAM enable, RGB channel

    for (i=0; i<289; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x02, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x02, BIT(14)));

        u8OverDriveTable[i] = MDrv_STR_ReadByte(SC_BK16, 0x04);
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x05, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x05, BIT(14)));

        u8OverDriveTable[i+289] = MDrv_STR_ReadByte(SC_BK16, 0x07);
    }
    for (i=0; i<272; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x08, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x08, BIT(14)));

        u8OverDriveTable[i+289+272] = MDrv_STR_ReadByte(SC_BK16, 0x0A);
    }
    for (i=0; i<256; i++)
    {
        MDrv_STR_Write2ByteMsk(SC_BK16, 0x0B, (i|0x4000), 0x41FF);
        while(MDrv_STR_Read2ByteMsk(SC_BK16, 0x0B, BIT(14)));

        u8OverDriveTable[i+289+272+272] = MDrv_STR_ReadByte(SC_BK16, 0x0D);
    }

#endif
}

//========================================================//
//------------------------------init--------------------------------------//
static void Init_MApi_XC_Init_WithoutCreateMutex(void)
{
    //MDrv_XC_SoftwareReset(REST_IP_ALL, MAIN_WINDOW);
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x02, BIT(4), 0xFF );
    MDrv_STR_Write2ByteMsk(CLKGEN, 0x54, 0x2000, BIT(10)|BIT(11)|BIT(12)|BIT(13));
    MDrv_STR_Write2ByteMsk(CLKGEN, 0x54, (bank_backup[CLKGEN_IDX][0x54]), BIT(10)|BIT(11)|BIT(12)|BIT(13));
    MDrv_STR_Write2ByteMsk(CLKGEN, 0x55, 0x0020, BIT(2)|BIT(3)|BIT(4)|BIT(5));
    MDrv_STR_Write2ByteMsk(CLKGEN, 0x55, (bank_backup[CLKGEN_IDX][0x55]), BIT(2)|BIT(3)|BIT(4)|BIT(5));
    MDrv_STR_Write2ByteMsk( SC_BK00, 0x02, 0, 0xFF );
    //Hal_SC_enable_window(FALSE,SUB_WINDOW);
    MDrv_STR_Write2ByteMsk(SC_BK20, 0x10, BIT(11) , BIT(11));
    MDrv_STR_Write2ByteMsk(SC_BK20, 0x10, 0, BIT(1));
    //MDrv_XC_DisableInputSource(ENABLE, MAIN_WINDOW);
    MDrv_STR_Write2ByteMsk(SC_BK01, 0x02, BIT(7) , BIT(7));
    //MDrv_XC_DisableInputSource(ENABLE, SUB_WINDOW);
    MDrv_STR_Write2ByteMsk(SC_BK03, 0x02, 0 , BIT(7));
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x47, BIT(0) , BIT(0));
    /*MDrv_XC_SetFrameBufferAddress*/
    //Hal_SC_Enable_MiuMask()
    MDrv_STR_Write2Byte(MIU0_BASE, 0x23, (bank_backup[MIU0_BASE_IDX][0x23]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x33, (bank_backup[MIU0_BASE_IDX][0x33]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x43, (bank_backup[MIU0_BASE_IDX][0x43]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x53, (bank_backup[MIU0_BASE_IDX][0x53]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x23, (bank_backup[MIU1_BASE_IDX][0x23]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x33, (bank_backup[MIU1_BASE_IDX][0x33]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x43, (bank_backup[MIU1_BASE_IDX][0x43]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x53, (bank_backup[MIU1_BASE_IDX][0x53]));
    //Hal_SC_set_freezeimg(ENABLE, eWindow);
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x01, BIT(11), BIT(11));
    //Hal_SC_set_miusel(u8MIUSel);
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x05, 0x0000, 0x0010);     // reg_miu_select_f2 to MIU0
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x44, 0x0000, 0x4000);     // reg_miu_select_f1 to MIU0
    MDrv_STR_Write2ByteMsk(MIU0_BASE , 0x79, 0, 0x300E); // MIU select
    MDrv_STR_Write2ByteMsk(MIU1_BASE , 0x79, 0x300E, 0x300E); // MIU select
    //MDrv_STR_Write2ByteMsk(MIU1_BASE , 0x7B, 0, 0x300E); // IP select
        /* Munich Special Items There are two MIU in this chip, but scaler related IP only can use one MIU.
        ** 0x130008 BITS[7:5] miu to scaler read data / scaler to miu write byte / scaler to miu write data
        ** Set Both to 0:miu0,1:miu1
        */
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x40, 0 , 0x00E0);      //MIU 0
    MDrv_STR_Write2ByteMsk(SC_BK00, 0x40, 0x00E0 , 0x00E0);     //MIU1
    //Hal_SC_set_memoryaddress
        //Hal_SC_set_DNRBase0(u32DNRBase0, eWindow);
        MDrv_STR_Write2Byte(SC_BK12, 0x08, (bank_backup[XC_RELATED_BANKNUM+0x12][0x08]));  // main
        MDrv_STR_Write2Byte(SC_BK12, 0x09, (bank_backup[XC_RELATED_BANKNUM+0x12][0x09]));
        MDrv_STR_Write2Byte(SC_BK12, 0x48, (bank_backup[XC_RELATED_BANKNUM+0x12][0x48]));  // sub
        MDrv_STR_Write2Byte(SC_BK12, 0x49, (bank_backup[XC_RELATED_BANKNUM+0x12][0x49]));
        //Hal_SC_set_DNRBase1(u32DNRBase1, eWindow); // UNUSE
        //Hal_SC_set_DNRBase2(u32DNRBase2, eWindow); // UNUSE
        //Hal_SC_set_OPMBase0(u32OPMBase0, eWindow);
        MDrv_STR_Write2Byte(SC_BK12, 0x10, (bank_backup[XC_RELATED_BANKNUM+0x12][0x10]));  // main
        MDrv_STR_Write2Byte(SC_BK12, 0x11, (bank_backup[XC_RELATED_BANKNUM+0x12][0x11]));
        MDrv_STR_Write2Byte(SC_BK12, 0x50, (bank_backup[XC_RELATED_BANKNUM+0x12][0x50]));  // sub
        MDrv_STR_Write2Byte(SC_BK12, 0x51, (bank_backup[XC_RELATED_BANKNUM+0x12][0x51]));
        //Hal_SC_set_OPMBase1(u32OPMBase1, eWindow);
        MDrv_STR_Write2Byte(SC_BK12, 0x12, (bank_backup[XC_RELATED_BANKNUM+0x12][0x12]));  // main
        MDrv_STR_Write2Byte(SC_BK12, 0x13, (bank_backup[XC_RELATED_BANKNUM+0x12][0x13]));
        MDrv_STR_Write2Byte(SC_BK12, 0x52, (bank_backup[XC_RELATED_BANKNUM+0x12][0x52]));  // sub
        MDrv_STR_Write2Byte(SC_BK12, 0x53, (bank_backup[XC_RELATED_BANKNUM+0x12][0x53]));
        //Hal_SC_set_OPMBase2(u32OPMBase2, eWindow); // UNUSE
        //Hal_SC_set_WriteVLength(eWindow); // Write V length
        MDrv_STR_Write2Byte(SC_BK20, 0x15, 0x0);   // main
        MDrv_STR_Write2Byte(SC_BK20, 0x16, 0x0);   // sub
    //MDrv_SC_set_mirrorEx
        //Hal_SC_set_write_limit
        MDrv_STR_Write2Byte(SC_BK12, 0x1A, (bank_backup[XC_RELATED_BANKNUM+0x12][0x1A]));  // main
        MDrv_STR_Write2Byte(SC_BK12, 0x1B, (bank_backup[XC_RELATED_BANKNUM+0x12][0x1B]));
        MDrv_STR_Write2Byte(SC_BK12, 0x5A, (bank_backup[XC_RELATED_BANKNUM+0x12][0x5A]));  // sub
        MDrv_STR_Write2Byte(SC_BK12, 0x5B, (bank_backup[XC_RELATED_BANKNUM+0x12][0x5B]));
        //_MDrv_SC_Set_Mirror
        MDrv_STR_Write2ByteMsk(SC_BK01, 0x02, BIT(7), BIT(7));  // main
        MDrv_STR_Write2ByteMsk(SC_BK12, 0x03, (bank_backup[XC_RELATED_BANKNUM+0x12][0x03]), (BIT(12)|BIT(13)));
        MDrv_STR_Write2ByteMsk(SC_BK01, 0x02, (bank_backup[XC_RELATED_BANKNUM+0x01][0x02]), BIT(7));
        MDrv_STR_Write2ByteMsk(SC_BK03, 0x02, BIT(7), BIT(7));  // sub
        MDrv_STR_Write2ByteMsk(SC_BK12, 0x43, (bank_backup[XC_RELATED_BANKNUM+0x42][0x03]), (BIT(12)|BIT(13)));
        MDrv_STR_Write2ByteMsk(SC_BK03, 0x02, (bank_backup[XC_RELATED_BANKNUM+0x03][0x02]), BIT(7));
    //MDrv_SC_set_opm_write_limit
    MDrv_STR_Write2Byte(SC_BK12, 0x5E, (bank_backup[XC_RELATED_BANKNUM+0x12][0x5E]));
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x5F, (bank_backup[XC_RELATED_BANKNUM+0x12][0x5F]), 0x01FF);
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x5F, (bank_backup[XC_RELATED_BANKNUM+0x12][0x5F]), (BIT(9)|BIT(10)));

	MDrv_STR_Write2ByteMsk(SC_BK0D, 0x38, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x38]), BIT(0)|BIT(1)); // memory address limit enable
	MDrv_STR_Write2Byte(SC_BK0D, 0x40, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x40])); // F2 memory min address low
	MDrv_STR_Write2Byte(SC_BK0D, 0x41, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x41])); //F2 memory min address hi
	MDrv_STR_Write2Byte(SC_BK0D, 0x42, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x42])); //F2 memory max address low
	MDrv_STR_Write2Byte(SC_BK0D, 0x43, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x43])); //F2 memory max address hi
	MDrv_STR_Write2Byte(SC_BK0D, 0x44, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x44])); // F1memory min address low
	MDrv_STR_Write2Byte(SC_BK0D, 0x45, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x45])); // F1memory min address hi
	MDrv_STR_Write2Byte(SC_BK0D, 0x46, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x46])); // F1memory max address low
	MDrv_STR_Write2Byte(SC_BK0D, 0x47, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x47])); // F1memory max address low
	MDrv_STR_Write2ByteMsk(SC_BK0D, 0x38, (bank_backup[XC_RELATED_BANKNUM+0x0D][0x38]), BIT(2)); // opm write limit enable

    //Hal_SC_set_freezeimg(DISABLE, eWindow);
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x01, 0x0, BIT(11));
    MDrv_STR_Write2ByteMsk(SC_BK12, 0x41, 0x0, BIT(11));
    //Hal_SC_Disable_MiuMask


    MDrv_STR_Write2Byte(MIU0_BASE, 0x23, (bank_backup[MIU0_BASE_IDX][0x23]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x33, (bank_backup[MIU0_BASE_IDX][0x33]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x43, (bank_backup[MIU0_BASE_IDX][0x43]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x53, (bank_backup[MIU0_BASE_IDX][0x53]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x23, (bank_backup[MIU1_BASE_IDX][0x23]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x33, (bank_backup[MIU1_BASE_IDX][0x33]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x43, (bank_backup[MIU1_BASE_IDX][0x43]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x53, (bank_backup[MIU1_BASE_IDX][0x53]));
}

static void Init_MDrv_XC_Init_MISC(void)
{
#if FRC_INSIDE
    STR_PRINT("no support FRC_INSIDE now...\n");
#else

    MDrv_STR_Write2Byte(CLKGEN1_BK, 0x31, 0x0000); //[11:8]ckg_odclk_frc
    MDrv_STR_Write2Byte(RVD_BK, 0x09, 0x0000); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo

    //MDrv_FRC_ByPass_Enable(TRUE);
        //MHal_CLKGEN_FRC_Bypass_Enable(bEnable);
        //MApi_W2BYTEMSK(CLKGEN, 0x53,0x00,0x02); // Not Invert
        //MApi_W2BYTEMSK(CLKGEN, 0x53,0x00,0x01); // Enable clock
        // Enable CLK_ODCLK
        //MApi_W2BYTEMSK(CLKGEN, 0x53,0xC0C,0xC0C);   // LPLL output clock

        MDrv_STR_Write2ByteMsk(CLKGEN, 0x53,0x0C0C,0x0C0F);

#endif
}

static void Init_MDrv_XC_MLoad_Init(void)
{
    //Hal_XC_MLoad_set_trigger_timing(TRIG_SRC_DELAY_LINE); //By REG_MLOAD_TRIG_DLY
    MDrv_STR_Write2ByteMsk(SC_BK20, 0x19, (bank_backup[XC_RELATED_BANKNUM+0x20][0x19]), 0x3000);
    //Hal_XC_MLoad_set_trigger_delay(0x05);
    MDrv_STR_Write2ByteMsk(SC_BK20, 0x22, (0x05), 0x0FFF);
    //Hal_XC_MLoad_set_trig_p(0x08, 0x0A);
    MDrv_STR_Write2ByteMsk(SC_BK20, 0x1A, 0x08, 0x0FFF);
    MDrv_STR_Write2ByteMsk(SC_BK20, 0x1B, 0x0A, 0x0FFF);

    //Hal_XC_MLoad_set_miusel(u8MIUSel);
    MDrv_STR_Write2ByteMsk(SC_BK1F, 0x1E, 0x0000, 0x2000); // u8MIUSel=MIU0
    //MDrv_STR_Write2ByteMsk(SC_BK1F, 0x1E, 0x2000, 0x2000); // u8MIUSel=MIU1

    //Hal_XC_MLoad_set_riu_cs(TRUE);
    //MDrv_WriteByteMask(0x100104, 0x10, 0x10);
    MDrv_STR_Write2ByteMsk(0x100100, 0x02, 0x0010, 0x0010);
    //MsOS_DelayTask(1);

    //Hal_XC_MLoad_Set_riu(1);
    MDrv_STR_Write2ByteMsk(SC_BK1F, 0x10, 0x1000, 0x1000);

    // 切bank到0xFF, 搶bank switch的主動權
    MDrv_STR_Write2ByteMsk(0x102F00, 0x00, 0x00FF, 0x00FF);

    //Hal_XC_MLoad_set_riu_cs(FALSE);
    MDrv_STR_Write2ByteMsk(0x100100, 0x02, 0x0000, 0x0010);
    //MsOS_DelayTask(1);

    //MDrv_XC_MLoad_KickOff
        //MDrv_XC_MLoad_Trigger(CmdBufAddr, u16CmdCnt);
            //Hal_XC_MLoad_set_base_addr(startAddr);
            MDrv_STR_Write2Byte(SC_BK1F, 0x03, (bank_backup[XC_RELATED_BANKNUM+0x1F][0x03]));
            MDrv_STR_Write2ByteMsk(SC_BK1F, 0x04, (bank_backup[XC_RELATED_BANKNUM+0x1F][0x04]), 0x03FF);
            //Hal_XC_MLoad_set_depth(u16CmdCnt);
            MDrv_STR_Write2Byte(SC_BK1F, 0x01, (bank_backup[XC_RELATED_BANKNUM+0x1F][0x01]));
            //Hal_XC_MLoad_set_len(MS_MLOAD_REG_LEN);//length of DMA request
            MDrv_STR_Write2ByteMsk(SC_BK1F, 0x02, (bank_backup[XC_RELATED_BANKNUM+0x1F][0x02]), 0x07FF);
            //Hal_XC_MLoad_set_on_off(ENABLE);
            MDrv_STR_Write2ByteMsk(SC_BK1F, 0x02, 0x8000, 0x8000);
            //MsOS_DelayTaskUs(3);
            //Hal_XC_MLoad_Set_riu(ENABLE);
            MDrv_STR_Write2ByteMsk(SC_BK1F, 0x10, 0x1000, 0x1000);
}


static void Init_MDrv_SC_SetMCDIBufferAddress(void)
{
//    Hal_SC_Enable_MiuMask();
    MDrv_STR_Write2ByteMsk(MIU0_BASE, 0x23, (bank_backup[MIU0_BASE_IDX][0x23]), MIU_SC_G0REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU0_BASE, 0x33, (bank_backup[MIU0_BASE_IDX][0x33]), MIU_SC_G1REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU0_BASE, 0x43, (bank_backup[MIU0_BASE_IDX][0x43]), MIU_SC_G2REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU0_BASE, 0x53, (bank_backup[MIU0_BASE_IDX][0x53]), MIU_SC_G3REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU1_BASE, 0x23, (bank_backup[MIU1_BASE_IDX][0x23]), MIU_SC_G0REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU1_BASE, 0x33, (bank_backup[MIU1_BASE_IDX][0x33]), MIU_SC_G1REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU1_BASE, 0x43, (bank_backup[MIU1_BASE_IDX][0x43]), MIU_SC_G2REQUEST_MASK);
    MDrv_STR_Write2ByteMsk(MIU1_BASE, 0x53, (bank_backup[MIU1_BASE_IDX][0x53]), MIU_SC_G3REQUEST_MASK);

//    Hal_SC_set_mrq_miusel(u8MIUSel);
    MDrv_STR_Write2ByteMsk(SC_BK0D, (0x07), 0, BIT(8));     // reg_mrq_miu_sel to MIU0
    MDrv_STR_Write2ByteMsk(MIU0_BASE, 0x79, 0x0000, 0x8000); // MIU select (Group1 BIT15)
    MDrv_STR_Write2ByteMsk(MIU1_BASE, 0x79, 0x8000, 0x8000); // IP select

//    Hal_SC_set_mcdi_memoryaddress(u32FBAddress, eType);
    MDrv_STR_Write2Byte(SC_BK17, (0x0A), (bank_backup[XC_RELATED_BANKNUM+0x17][0x0A]));     // MAIN ME1 address
    MDrv_STR_Write2Byte(SC_BK17, (0x0B), (bank_backup[XC_RELATED_BANKNUM+0x17][0x0B]));     // MAIN ME1 address
    MDrv_STR_Write2Byte(SC_BK17, (0x12), (bank_backup[XC_RELATED_BANKNUM+0x17][0x12]));     // MAIN ME2 address
    MDrv_STR_Write2Byte(SC_BK17, (0x13), (bank_backup[XC_RELATED_BANKNUM+0x17][0x13]));     // MAIN ME2 address

//    Hal_SC_set_mcdi_write_limit(ENABLE, (u32FBAddress + u32FBSize), eType);
    // Main/Sub shares the same enable bit of ME1
    MDrv_STR_Write2ByteMsk(SC_BK17, (0x08), BIT(0), BIT(0));
    // Bit 0 ~ 26
    MDrv_STR_Write2Byte(SC_BK17, (0x0C), (bank_backup[XC_RELATED_BANKNUM+0x17][0x0C]));
    MDrv_STR_Write2ByteMsk(SC_BK17, (0x0D), (bank_backup[XC_RELATED_BANKNUM+0x17][0x0D]), 0x03FF);

    // Main/Sub shares the same enable bit of ME2
    MDrv_STR_Write2ByteMsk(SC_BK17, (0x10), BIT(0), BIT(0));
    // Bit 0 ~ 26
    MDrv_STR_Write2Byte(SC_BK17, (0x14), (bank_backup[XC_RELATED_BANKNUM+0x17][0x14]));
    MDrv_STR_Write2ByteMsk(SC_BK17, (0x15), (bank_backup[XC_RELATED_BANKNUM+0x17][0x15]), 0x03FF);

//    Hal_SC_Disable_MiuMask()
    MDrv_STR_Write2Byte(MIU0_BASE, 0x23, (bank_backup[MIU0_BASE_IDX][0x23]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x33, (bank_backup[MIU0_BASE_IDX][0x33]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x43, (bank_backup[MIU0_BASE_IDX][0x43]));
    MDrv_STR_Write2Byte(MIU0_BASE, 0x53, (bank_backup[MIU0_BASE_IDX][0x53]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x23, (bank_backup[MIU1_BASE_IDX][0x23]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x33, (bank_backup[MIU1_BASE_IDX][0x33]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x43, (bank_backup[MIU1_BASE_IDX][0x43]));
    MDrv_STR_Write2Byte(MIU1_BASE, 0x53, (bank_backup[MIU1_BASE_IDX][0x53]));

}

static void Init_MDrv_SC_EnableMCDI(void)
{
    MDrv_STR_Write2ByteMsk(SC_BK17, (0x07), 0x00, (BIT(1)|BIT(2)));

    // Turn on MIU access arbitor
    MDrv_STR_Write2ByteMsk(SC_BK17, (0x04), 0x00, BIT(6));
}

static void Init_MDrv_XC_SetDispWindowColor(void)
{
    MDrv_STR_Write2ByteMsk(SC_BK10, (0x24), (bank_backup[XC_RELATED_BANKNUM+0x10][0x24]),0x00FF);
    MDrv_STR_Write2ByteMsk(SC_BK0F, (0x17), (bank_backup[XC_RELATED_BANKNUM+0x0F][0x17]),0xFF00);
}

static void Init_MDrv_XC_SetFrameColor(void)
{
    MDrv_STR_Write2ByteMsk(SC_BK10, (0x19), (bank_backup[XC_RELATED_BANKNUM+0x10][0x19]),0xFF00); ///<R
    MDrv_STR_Write2ByteMsk(SC_BK10, (0x1A), (bank_backup[XC_RELATED_BANKNUM+0x10][0x19]),0x00FF); ///<G
    MDrv_STR_Write2ByteMsk(SC_BK10, (0x1A), (bank_backup[XC_RELATED_BANKNUM+0x10][0x19]),0xFF00); ///<B
}


static void  Init_ADC(void)
{
    STR_PRINT("----- [%s],Init_ADC  enter\n",__DATE__);

    MS_U32 i ;
    for(i = 0; i < ADC_Init_SUM; i++)
    {
        MDrv_STR_Write2ByteMsk(ADC_init_Table[i][0], ADC_init_Table[i][1], ADC_init_Table[i][2],ADC_init_Table[i][3]);
#if DUMP_DATA
            printk("    [%lx] = {%lx %lx %lx %lx}",i,ADC_init_Table[i][0], ADC_init_Table[i][1], ADC_init_Table[i][2],ADC_init_Table[i][3]);
#endif
    }
}
static void Init_MApi_XC_SetPowerState_U2(void)
{
    Init_MApi_XC_Init_WithoutCreateMutex();
    //Init_ADC();
    Init_MDrv_XC_Init_MISC();
    Init_MDrv_XC_MLoad_Init();
    Init_MDrv_SC_SetMCDIBufferAddress();
    Init_MDrv_SC_EnableMCDI();
    Init_MDrv_XC_SetDispWindowColor();
    Init_MDrv_XC_SetFrameColor();
}

static void  Init_VE(void)
{

    MS_U16 u16temp1;
    MS_U16 u16temp2;
    MS_U16 u16temp3;
    MS_U16 u16temp4 ;
    //MDrv_WriteByte(H_BK_CHIPTOP(0x19), 0x80);

    MDrv_STR_Write2ByteMsk(CLKGEN,(0x24), 0x0800, 0x0F0F);   // clock of ve
    MDrv_STR_Write2ByteMsk(VE_1,(0x03),0x0000,0x0010);

    // read FIFO control
    MDrv_STR_Write2ByteMsk(VE_0,(0x32),0x0018,0x00FF);
    MDrv_STR_Write2Byte(VE_0,(0x37),0x0605); // T2 set 0x0B


    // initial TTX VBI
    MDrv_STR_Write2Byte(VE_0,0x06,0);
    u16temp1 = (MDrv_STR_Read2Byte(VE_0, 0x43)&0xff00)|0x0039;
    MDrv_STR_Write2Byte(VE_0,0x43,u16temp1);
    u16temp2 = (MDrv_STR_Read2Byte(VE_0,0x08)& 0x00FF)|0xFB00;
    MDrv_STR_Write2Byte(VE_0,(0x08), u16temp2);
    u16temp3 = (MDrv_STR_Read2Byte(VE_0,0x46)&0xFF00)|0x0003;
    MDrv_STR_Write2Byte(VE_0,0x46,u16temp3);
    MDrv_STR_Write2ByteMsk(VE_1,0x07, 0x00,0x00FF);
    u16temp4 = MDrv_STR_Read2Byte(VE_0,0x00)|0x0008;
    MDrv_STR_Write2Byte(VE_0,0x00,u16temp4);

    MDrv_STR_Write2ByteMsk(VE_0,0x44,0x00F0,0x00F0);
    MDrv_STR_Write2ByteMsk(VE_0,0x46,0x00D0,0x00F0);

    MDrv_STR_Write2ByteMsk(VE_0,0x7E, 0x0100, 0x0100); //enable black boundary

    MDrv_STR_Write2Byte(VE_0,0x11, 0x0080); //default VE Hardware debug mode VE ToDo
    MDrv_STR_Write2ByteMsk(VE_0,0x5A, 0x0000, 0x0001);// disable scaler in

    //sync delay pipe from write clock to read clock domain (add 2 pipe flip-flop)
    MDrv_STR_Write2ByteMsk(VE_0,0x7E,0x0002,0x0002);

    // Clock Gen
    MDrv_STR_Write2ByteMsk(CLKGEN,0x25, bank_backup[CLKGEN_IDX][0x25] & 0x00FF , 0x00FF);  // idclk
}

static void Init_DIPW(void)
{
    STR_PRINT("Init_DIPW  enter\n");
    MS_U16 u16H_CapStart =0x1 ;
    MS_U16 u16H_CapSize =0x10 ;
    MS_U16 u16V_CapStart =0x1 ;
    MS_U16 u16V_CapSize = 0x10 ;

    MS_U16 u32H_PreScalingRatio = 0x0 ;
    MS_U16 u32V_PreScalingRatio = 0x0 ;

    MDrv_STR_Write2Byte(SC_BK34, 0x04 , u32H_PreScalingRatio);   // H pre-scaling
    MDrv_STR_Write2Byte(SC_BK34, 0x05, u32H_PreScalingRatio);   // H pre-scaling
    MDrv_STR_Write2Byte(SC_BK34, 0x08,u32V_PreScalingRatio);   // V pre-scaling
    MDrv_STR_Write2Byte(SC_BK34, 0x09,u32V_PreScalingRatio);   // V pre-scaling
    MDrv_STR_Write2ByteMsk(SC_BK34, 0x60,0x0003 ,0x0003);
    MDrv_STR_Write2Byte(SC_BK34, 0x61,u16H_CapStart+1);
    MDrv_STR_Write2Byte(SC_BK34, 0x62,u16H_CapStart+u16H_CapSize);
    MDrv_STR_Write2Byte(SC_BK34, 0x63,u16V_CapStart+1);
    MDrv_STR_Write2Byte(SC_BK34, 0x64,u16V_CapStart+u16V_CapSize);
    MDrv_STR_Write2Byte(SC_BK3B, 0x15,u32H_PreScalingRatio);   // H pre-scaling
    MDrv_STR_Write2Byte(SC_BK3B, 0x16,u32H_PreScalingRatio);   // H pre-scaling
    MDrv_STR_Write2Byte(SC_BK3B, 0x1B,u32V_PreScalingRatio);   // V pre-scaling
    MDrv_STR_Write2Byte(SC_BK3B, 0x1C,u32V_PreScalingRatio);   // V pre-scaling
    MDrv_STR_Write2ByteMsk(SC_BK3B, 0x62,0x0003,0x0003);
    MDrv_STR_Write2Byte(SC_BK3B, 0x63,u16H_CapStart+1);
    MDrv_STR_Write2Byte(SC_BK3B, 0x64,u16H_CapStart+u16H_CapSize);
    MDrv_STR_Write2Byte(SC_BK3B, 0x65,u16V_CapStart+1);
    MDrv_STR_Write2Byte(SC_BK3B, 0x66,u16V_CapStart+u16V_CapSize);
    MDrv_STR_Write2Byte(SC_BK3C, 0x15,u32H_PreScalingRatio);   // H pre-scaling
    MDrv_STR_Write2Byte(SC_BK3C, 0x16,u32H_PreScalingRatio);   // H pre-scaling
    MDrv_STR_Write2Byte(SC_BK3C, 0x1B,u32V_PreScalingRatio);   // V pre-scaling
    MDrv_STR_Write2Byte(SC_BK3C, 0x1C,u32V_PreScalingRatio);   // V pre-scaling
    MDrv_STR_Write2ByteMsk(SC_BK3C, 0x62,0x0003,0x0003);
    MDrv_STR_Write2Byte(SC_BK3C, 0x63,u16H_CapStart+1);
    MDrv_STR_Write2Byte(SC_BK3C, 0x64,u16H_CapStart+u16H_CapSize);
    MDrv_STR_Write2Byte(SC_BK3C, 0x64,u16V_CapStart+1);
    MDrv_STR_Write2Byte(SC_BK3C, 0x66,u16V_CapStart+u16V_CapSize);
    //[15]intlac. src;[12:8]422to420 ratio;[6]dwin_en;[5:4]format(rgb888);[3]intlac_w;[2:0]frm_buf_num
    MDrv_STR_Write2Byte(SC_BK36, 0x01,0x0840);
    // [10]pdw_off;[9:8] reg_dip_pdw_src_sel;[7:0]alpha
    MDrv_STR_Write2Byte(SC_BK36, 0x02,0x0400);
    //[8] write once; [9] write once trig
    MDrv_STR_Write2Byte(SC_BK36, 0x03,0x0000);
    //[15:8]wreq max ; [7:0]wreq threshold
    MDrv_STR_Write2Byte(SC_BK36, 0x6F,0x200A);
    //tile request number
    MDrv_STR_Write2Byte(SC_BK36, 0x7E,0x0010);
    //enable dip clk
    MDrv_STR_Write2ByteMsk(CLKGEN, 0x59 , 0, 0x0002);                   // Not Invert
    MDrv_STR_Write2ByteMsk(CLKGEN, 0x59 , 0, 0x0001);                    // Enable clock
    STR_PRINT("Init_DIPW  enter\n");
    //[15]intlac. src;[12:8]422to420 ratio;[6]dwin_en;[5:4]format(rgb888);[3]intlac_w;[2:0]frm_buf_num
    MDrv_STR_Write2Byte(SC_BK3B, 0x01,0x0840);
    // [10]pdw_off;[9:8] reg_dip_pdw_src_sel;[7:0]alpha
    MDrv_STR_Write2Byte(SC_BK3B, 0x02,0x0400);
    //[8] write once; [9] write once trig[11]256mode
    MDrv_STR_Write2Byte(SC_BK3B, 0x03,0x0800);
    //[15:8]wreq max ; [7:0]wreq threshold
    MDrv_STR_Write2Byte(SC_BK3B, 0x6F,0x200A);
    //tile request number
    MDrv_STR_Write2Byte(SC_BK3B, 0x7E,0x0010);
     //enable dwin0 clk
     MDrv_STR_Write2ByteMsk(CLKGEN,0x5F, 0, 0x0002);                   // Not Invert
     MDrv_STR_Write2ByteMsk(CLKGEN,0x5F, 0, 0x0001);                    // Enable clock

     //[15]intlac. src;[12:8]422to420 ratio;[6]dwin_en;[5:4]format(rgb888);[3]intlac_w;[2:0]frm_buf_num
     MDrv_STR_Write2Byte(SC_BK3C, 0x01,0x0840);
     // [10]pdw_off;[9:8] reg_dip_pdw_src_sel;[7:0]alpha
     MDrv_STR_Write2Byte(SC_BK3C, 0x02,0x0400);
     //[8] write once; [9] write once trig[11]256mode
     MDrv_STR_Write2Byte(SC_BK3C, 0x03,0x0800);
     //[15:8]wreq max ; [7:0]wreq threshold
     MDrv_STR_Write2Byte(SC_BK3C, 0x6F,0x200A);
     //tile request number
     MDrv_STR_Write2Byte(SC_BK3C, 0x7E,0x0010);

     //enable dwin1 clk
     MDrv_STR_Write2ByteMsk(CLKGEN, 0x5F , 0 , 0x0200);                   // Not Invert
     MDrv_STR_Write2ByteMsk(CLKGEN, 0x5F , 0 , 0x0100);                    // Enable clock
     STR_PRINT("Init_DIPW  enter\n");
     //for DIP R
     //read request maximum length and 2的倍數, tile 420 used
     MDrv_STR_Write2Byte(SC_BK36, 0x7F,0x4010);
     //read request threshold
     MDrv_STR_Write2ByteMsk(SC_BK36,0x7D,0x000A,0x003F);
}

void getHDCPKey(MS_U8 * user_buffer, MS_U32 u32Size)
{
    unsigned int    dwDataCounter = 0;
    MS_U16  reg_512x8_addr = 0x10, reg_512x8_trig = 0x0001, reg_gaddr = 0x20, reg_512x8_rdata_lo = 0x0000 , reg_512x8_rdata_hi = 0x0000;
    MS_U16  reg_512x8_addr2 = 0x0, reg_512x8_trig2 = 0x0001;


    //sd_print("HDCP in HDCP_KEY_IN_EFUSE_EDISON\n");
    //sd_print("HDCP_KEY_SIZE = %d  \n",HDCP_KEY_SIZE);

    while(dwDataCounter < HDCP_KEY_SIZE)
    {

#if 0
        REG_EFUSE(REG_EFUSE_GADRR) = reg_gaddr;
        REG_EFUSE(REG_EFUSE_RDATA_ADR) = reg_512x8_addr << 2;
        REG_EFUSE(REG_EFUSE_RDATA_ADR) |= reg_512x8_trig;
#endif

        MDrv_STR_Write2Byte(EFUSE_BANK, REG_EFUSE_GADRR, reg_gaddr);
        MDrv_STR_Write2Byte(EFUSE_BANK, REG_EFUSE_RDATA_ADR, reg_512x8_addr);
        REG_EFUSE_WRITE_MASK(REG_EFUSE_RDATA_TRI, reg_512x8_trig, reg_512x8_trig);

        while((REG_EFUSE_READ(REG_EFUSE_RDATA_TRI) & BIT(0)));//waiting data ready

        reg_512x8_rdata_lo = REG_EFUSE_READ(REG_EFUSE_RDATA_LOBANK_LO);
        reg_512x8_rdata_hi = REG_EFUSE_READ(REG_EFUSE_RDATA_LOBANK_HI);

        if(dwDataCounter < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo >> 8;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi >> 8;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }

        reg_512x8_rdata_lo = REG_EFUSE_READ(REG_EFUSE_RDATA_HIBANK_LO);
        reg_512x8_rdata_hi = REG_EFUSE_READ(REG_EFUSE_RDATA_HIBANK_HI);

        if(dwDataCounter < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo >> 8;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi >> 8;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi;
            //printf("%02x ", user_buffer[dwDataCounter-1]);
        }

        reg_512x8_addr += 2;

        if((dwDataCounter % 16) == 0)
        {
            //printf("\r\n");
        }
        if(reg_512x8_addr >= 0x38)
            break;

    }


    while(dwDataCounter < HDCP_KEY_SIZE)
    {

        MDrv_STR_Write2Byte(EFUSE_BANK, REG_EFUSE_GADRR, reg_gaddr);
        MDrv_STR_Write2Byte(EFUSE_BANK, REG_EFUSE_RDATA_ADR2, reg_512x8_addr2);
        REG_EFUSE_WRITE_MASK(REG_EFUSE_RDATA_TRI2, reg_512x8_trig2, reg_512x8_trig2);


        while((REG_EFUSE_READ(REG_EFUSE_RDATA_TRI2) & BIT(0)));//waiting data ready


        reg_512x8_rdata_lo = REG_EFUSE_READ(REG_EFUSE_RDATA_LOBANK_LO2);
        reg_512x8_rdata_hi = REG_EFUSE_READ(REG_EFUSE_RDATA_LOBANK_HI2);





        if(dwDataCounter < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo >> 8;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi >> 8;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }

        reg_512x8_rdata_lo = REG_EFUSE_READ(REG_EFUSE_RDATA_HIBANK_LO2);
        reg_512x8_rdata_hi = REG_EFUSE_READ(REG_EFUSE_RDATA_HIBANK_HI2);


        if(dwDataCounter < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo >> 8;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_lo;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi >> 8;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }
        if((dwDataCounter) < HDCP_KEY_SIZE)
        {
            user_buffer[dwDataCounter++] = reg_512x8_rdata_hi;
            //printk("user_buffer[%d] = %02x", dwDataCounter-1, user_buffer[dwDataCounter-1]);
        }


        reg_512x8_addr2 += 2;

        if((dwDataCounter % 16) == 0)
        {
            printk("\r\n");
        }

    }



#if 0 //Open for debug
        (printk("msAPI_EFUSE_Read\n"));
        (printk("HDCP_KEY_SIZE = %lu  \n", u32Size));
        unsigned int i;
        for(i = 0; i < u32Size; i++)
        {
            if(i % 16 == 0) (printk("\n"));
            (printk("%02x ", user_buffer[i]));
        }
        (printk("\n"));
#endif

        return; //Success

}


#if INTERNAL_HDCP
static MS_U8 _pu8HdcpKeyData[289] =
{
    0x89,0x38,0xAE,0x0D,0xED,0x40,0xE6,0xBB,0xFA,0x4E,0xDE,0x51,0xFB,0x8E,0xD9,0xAA,
    0x34,0xA8,0xC4,0xEA,0xD8,0x6C,0xDC,0x5C,0x91,0x5C,0xB1,0xA6,0x13,0x2B,0x8B,0x8B,
    0xF7,0x46,0xCC,0x1C,0x88,0x20,0xA3,0x27,0x0E,0xE1,0x28,0x84,0x89,0x39,0xA3,0xE2,
    0x36,0x86,0xCE,0x67,0xEB,0xA0,0xF2,0x35,0x6B,0x86,0xF5,0x21,0x71,0x95,0x8A,0x77,
    0xA1,0x28,0x77,0x97,0xD3,0x7B,0xEF,0x5C,0x15,0x48,0xAA,0x9E,0x97,0x39,0xCD,0x98,
    0x40,0x5E,0x68,0x56,0x66,0xEF,0xC1,0x3C,0xE1,0x8F,0x2A,0x82,0xDE,0x8F,0x52,0xCC,
    0xA8,0x1F,0x37,0xD9,0xD4,0xC6,0x24,0x16,0x7E,0x42,0xFF,0x57,0xCD,0x6B,0xE0,0x86,
    0x00,0x1A,0xF1,0x19,0x5A,0xAF,0x37,0x97,0x86,0xBA,0x83,0x29,0xFE,0x41,0xA8,0xD5,
    0xF4,0x73,0x43,0x03,0x23,0x22,0xC5,0x28,0x96,0x9E,0x35,0x0D,0x67,0xA8,0x8B,0xDD,
    0x7A,0x89,0x38,0xE0,0x94,0xF0,0xFF,0xF5,0x8F,0xF3,0x4E,0x5C,0x82,0x09,0xF3,0x97,
    0xEB,0x01,0x52,0xEC,0xD8,0x98,0x5C,0x4F,0x43,0x2E,0xE7,0x9F,0xF5,0x85,0x6D,0x15,
    0xB1,0x83,0x20,0xF8,0x5E,0xD0,0x33,0x4F,0xF0,0xC1,0x8F,0x65,0x77,0x3D,0x31,0xB2,
    0xFB,0xA1,0x6E,0xCA,0xA6,0xD3,0xA2,0x35,0x1D,0x16,0x41,0xC3,0x89,0x86,0x98,0x78,
    0x8E,0x3E,0xC1,0x64,0x01,0x79,0x05,0x21,0x47,0xAF,0x6A,0x6F,0x5B,0xE1,0x4D,0x2B,
    0x2F,0xCC,0x18,0x8E,0x42,0xDC,0x9A,0xF8,0x3C,0xD0,0xD0,0x57,0x04,0xFB,0x14,0x42,
    0x8C,0x54,0x9D,0xA9,0x06,0xEB,0xE7,0x48,0xE2,0x29,0xEF,0x7E,0xFD,0xF6,0x45,0x12,
    0xAC,0xE4,0xBC,0x45,0x67,0xA3,0x9B,0x65,0xA1,0x0E,0xED,0x1A,0x84,0xAD,0x49,0x87,
    0xA2,0x77,0x3F,0x11,0xA7,0x1B,0xD1,0x7F,0x25,0x36,0x6c,0x6f,0xd3,0xdf,0x25,0xd0,
    0xFB,
};
#else
static MS_U8 _pu8HdcpKeyData[HDCP_KEY_SIZE];
#endif

MS_U8* GET_EFUSE_HDCP(void)
{
#if INTERNAL_HDCP
#else
    getHDCPKey(&_pu8HdcpKeyData[0], HDCP_KEY_SIZE);
#endif
    return &_pu8HdcpKeyData[0];
}

void Init_HDCPKEY_Efuse(void)//( MS_U8 * pu8HdcpKeyData )
{
    MS_U16 i, delay=0, u16bank_offset=0, u16loop;
    MS_U8 * pu8HdcpKeyData_bk;
    MS_U8 * pu8HdcpKeyData;

    //GET_EFUSE_HDCP(pu8HdcpKeyData);


    pu8HdcpKeyData = GET_EFUSE_HDCP();
    pu8HdcpKeyData_bk = pu8HdcpKeyData;

    // write BKSV/Bcaps into 4 HDCP engines
    for(u16loop=0; u16loop<4; u16loop++)
    {
        switch(u16loop)
        {
            //default:
            case 0:
                u16bank_offset = 0x0000;
                break; // BK 0x110A60
            case 1:
                u16bank_offset = 0x2900;
                break; // BK 0x113360
            case 2:
                u16bank_offset = 0x2B00;
                break; // BK 0x113560
            case 3:
                u16bank_offset = 0x2D00;
                break; // BK 0x113760
        }

        pu8HdcpKeyData = pu8HdcpKeyData_bk;
        // Bksv
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x17), BIT(10), BIT(10)); // [10]: HDCP enable for DDC
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x19), BIT(15), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM

        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x17), 0x00, 0x3FF); // address
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x19), BIT(5), BIT(5)); // trigger latch address
        for ( i = 0; i < 5; i++ )
        {
            MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x18), *pu8HdcpKeyData, 0xFF); // data
            MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x19), BIT(4), BIT(4)); // trigger latch data
            //delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
            pu8HdcpKeyData += 1;
            //while(MDrv_STR_Read2Byte(HDCP_BK+u16bank_offset, HDCP_OFFSET+0x19) & BIT(7)); // wait write ready
        }

        // Bcaps = 0x80
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x17), 0x40, 0x3FF); // address
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x19), BIT(5), BIT(5)); // trigger latch address

        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x18), 0x80, 0xFF); // data
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x19), BIT(4), BIT(4)); // trigger latch data
        //delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x19), 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM

        // [10:8]: 3'b111 determine Encrp_En during Vblank in DVI mode; [5]:HDCP enable; [0]: EESS mode deglitch Vsync mode
        MDrv_STR_Write2ByteMsk((HDCP_BK+u16bank_offset), (HDCP_OFFSET+0x00), (BIT(10)|BIT(9)|BIT(8)|BIT(5)|BIT(0)), (BIT(10)|BIT(9)|BIT(8)|BIT(5)|BIT(0)));
    }

    // HDCP key
    MDrv_STR_Write2ByteMsk(HDCP_BK,( HDCP_OFFSET+0x19), BIT(15)|BIT(14), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM
    // burst write from address 0x00
    MDrv_STR_Write2ByteMsk(HDCP_BK, (HDCP_OFFSET+0x17), 0x00, 0x3FF); // address
    MDrv_STR_Write2ByteMsk(HDCP_BK, (HDCP_OFFSET+0x19), BIT(5), BIT(5)); // trigger latch address

    for ( i = 0; i < 284; i++ )
    {
        MDrv_STR_Write2ByteMsk(HDCP_BK, (HDCP_OFFSET+0x18), *pu8HdcpKeyData, 0xFF); // data
        MDrv_STR_Write2ByteMsk(HDCP_BK, (HDCP_OFFSET+0x19), BIT(4), BIT(4)); // trigger latch data
        //delay=PM_R2BYTE(TIMER_0_CNT_REG,0xFFFF);
        pu8HdcpKeyData += 1;
        //while(MDrv_STR_Read2Byte(HDCP_BK, HDCP_OFFSET+0x19) & BIT(7)); // wait write ready
    }

    MDrv_STR_Write2ByteMsk(HDCP_BK, (HDCP_OFFSET+0x19), 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM
}
//------------------------------init--------------------------------------//
//========================================================//
int suspend_sc(void)
{
    STR_PRINT("----- [%s];TIME=%s,DATE=%s. -----enter.\n",__func__,__TIME__,__DATE__);
    //Hot plug need first than Clock
    //SetHPD(uEDITName, TRUE);
    SetHDMIClk(uEDITName , TRUE);
    StoreBank();
    Set_Skip_StoreCECCmd();
    StoreSubBank();
    StoreGammaSram();
    Store_ICC_SRAM();
    Store_IHC_SRAM();
  //  StoreCSram();
  //  StoreYSram();
    StoreOverDriver();
    //Store_XVYCC_SRAM();
    SetHPDSuspend();
    STR_PRINT("----- [%s];TIME=%s,DATE=%s. -----exit.\n",__func__,__TIME__,__DATE__);

    return 0;
}

int resume_sc(void)
{
    STR_PRINT("----- [%s];TIME=%s,DATE=%s. -----enter.\n",__func__,__TIME__,__DATE__);
    ReStoreBWTable();
    //Clk need first than hot plug
    Init_HDCPKEY_Efuse();
    SetHPD(uEDITName, TRUE);
    SetHDMIClk(uEDITName , TRUE);
    SetHDMIClk(uEDITName, FALSE );
    // move HPD high to the the end of resume process to save time (no more delay is needed here)
    //application will call HAL_VFE_HDMI_EnableHPD at the end of resume
    Init_MApi_XC_SetPowerState_U2();
    Init_VE();
    Init_DIPW() ;
    ReStoreSubBank();
    ReStoreBank();
   // ReStoreYSram();
   // ReStoreCSram();
    ReStore_ICC_SRAM();
    ReStore_IHC_SRAM();
    ReStoreGammaSram();
    ReStoreOverDriver();
    //ReStore_XVYCC_SRAM();
    STR_PRINT("----- [%s];TIME=%s,DATE=%s. -----exit.\n",__func__,__TIME__,__DATE__);
    return 0;
}

int suspend_ADC(void)
{
    STR_PRINT("\nsuspend_ADC  enter\n");

    STR_PRINT("\nADC don't do any thing.\n");

    STR_PRINT("\nsuspend_ADC exit\n");
    return 0;
}

int resume_ADC(void)
{
    STR_PRINT("resume_ADC enter\n");

    Init_ADC();

    STR_PRINT("resume_ADC exit\n");
	return 0;
}

