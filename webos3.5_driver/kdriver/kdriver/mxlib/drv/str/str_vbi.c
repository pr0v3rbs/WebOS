#include <linux/device.h>
#include <linux/export.h>
#include <linux/io.h>
#include "mst_str.h"
//#include "mdrv_types.h"
//#include "mdrv_system.h"
#include "drvVBI.h"


//----------------------------[Compiler falg]--------[Start]
#define RIU_BASE_ADDR  0xFD000000
#define VBI_BANK_ADDR  0x103700
#define AFEC_REG_BASE  0x103500  // 0x3500 - 0x35FF
#define BANK_SIZE 0x80
#define STR_VBI_DEBUG(x)    //x
#define IO_MAP  1    //using io map funciton to get buffer address
#define VBI_BANK_APPROACH 1

#if VBI_BANK_APPROACH 
#define VBI_BANK_SIZE 0x80
MS_U16 u16_VBIbank[VBI_BANK_SIZE] ;
#endif

#define VBI_STANDARD_REG_LENGTH 0x80 
MS_U16 u16_VBIstandardReg[VBI_STANDARD_REG_LENGTH] = {0};


#if IO_MAP 
#if 0
char* dram_VBIbuf = NULL;
#endif
#define VBI_BUF_START_LEN      0x0000024000
#define MCU_FIFO_LENGTH        31
#define CC_BUF_LEN_MAX         MCU_FIFO_LENGTH
#define VBI_TTX_DATA_LENGTH    48
#define TTX_MAX_BUFFERED_PACCKETS   720
#endif
//----------------------------[Compiler falg]--------[End]

//extern const MMapInfo_t *MDrv_SYS_GetMMAPInfo(EN_MMAP_ID id);

//----------------------------[ TypeDef and Struct]--------[Start]
/*typedef enum
{
    E_VBI_TELETEXT,
    E_VBI_CC
} VBI_INIT_TYPE;*/


typedef struct
{
	MS_U16 vbi_bank[BANK_SIZE];
}vbi_save_reg_t;

typedef struct
{
    MS_BOOL _bSuspend;
    VBI_INIT_TYPE eInitType;
	MS_PHYADDR u32Addr;
	MS_U16 u16Len_CC;
	MS_U16 u16PktCnt_TTX;
} VBI_STR_StoreInfo;

typedef struct
{
    VBI_INIT_TYPE   eInitType;
} VBI_STR_DrvStatus;


static VBI_STR_StoreInfo _pStoreInfo = {
    ._bSuspend = FALSE,
    .eInitType = E_VBI_TELETEXT,
    .u32Addr = 0,
    .u16Len_CC = 0,
};
//----------------------------[Typedef and Struct]--------[End]


//----------------------------[Global var]---------------[Start]
MS_U16 u16Htemp;
MS_U16 u16Ltemp;
MS_U32 u32Addr_3_10 = 0;
MS_U32 u32Addr_11_18 = 0;
MS_U32 u32Addr_19_26 = 0;
MS_U32 u32Addr_27_31 = 0;
//----------------------------[Global var]---------------[End]



//----------------------------[local macro]---------------[Start]
#define VBI_W2BYTE_H(bank ,addr, val)                     \
	     u16Htemp = MDrv_STR_Read2Byte(bank,addr);        \
	     u16Htemp = (u16Htemp & (0x00FF));                \
         MDrv_STR_Write2Byte(bank, addr, (val | u16Htemp)); \




#define VBI_W2BYTE_L(bank ,addr, val)                            \
        u16Ltemp  = MDrv_STR_Read2Byte(bank,addr);               \
        u16Ltemp = (u16Ltemp & (0xFF00));                        \
        MDrv_STR_Write2Byte(VBI_BANK_ADDR, addr, (val | u16Ltemp)); \

//----------------------------[local macro]---------------[End]



static void VBI_CC_EnableSlicer_Reset(void)
{
    //HAL_VBI_WriteByte(BK_VBI_46, HAL_VBI_ReadByte(BK_VBI_46) & 0xFE);
    //0x1037 46   read out, and 0XFE
    MS_U16 u16Tmp = 0x0000;
    u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x23);
	u16Tmp = (u16Tmp & 0xFFFE);
	MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x23, u16Tmp);   
}

static void VBI_TTX_InitSlicer(unsigned long addr, MS_U16 packetCount)
{
#if 0
    addr = addr >> 3; /* 8 byte-aligned */

    /* Initial VBI Buffer Start Address */
    HAL_VBI_Write2Byte(VBI_BASEADDR_L, addr);
    HAL_VBI_WriteByte(VBI_BASEADDR_H, addr >> 16);
	
    if((addr>>24) & 0x0F)
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, (HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~0xF))|((addr>>24) & 0x0f));
    else
        HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~0xF));


    /* Initial VBI Buffer Field Number */
    HAL_VBI_Write2Byte(VBI_BUF_LEN, packetCount);   /* no need to minus 1, follow Venus design */


    /* change dram access mode
      * 0x371F, BIT7 must be always 1 (otherwise the DMA access related function could be error) */
    HAL_VBI_WriteByte(TTDEC_COMMAND, _BIT1|_BIT7);   /* put header packet into VBI without decoder */
#else
    MS_U8  u8Tmp = 0;
    MS_U16 u16Tmp = 0;

    addr = addr >> 3; /* 8 byte-aligned */
    /* Initial VBI Buffer Start Address */
	//1037 38
	MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x1C, addr); 
	// 1037 3A
    MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x1D, addr >> 16); 

	if((addr>>24) & 0x0F)
	{
        //HAL_VBI_WriteByte(VBI_BIT24_ADDR, (HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~0xF))|((addr>>24) & 0x0f));
        u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x0E);
		u8Tmp = (u16Tmp >> 8);
		u8Tmp = (u8Tmp &(~0xF))|((addr>>24) & 0x0f);
		u16Tmp = u8Tmp;
		u16Tmp = (u16Tmp << 8);
		VBI_W2BYTE_H(VBI_BANK_ADDR, 0x0E, u16Tmp);
		
        
	}else
	{
        //HAL_VBI_WriteByte(VBI_BIT24_ADDR, HAL_VBI_ReadByte(VBI_BIT24_ADDR)&(~0xF));
		u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x0E);
		u8Tmp = (u16Tmp >> 8);
		u8Tmp = (u8Tmp &(~0xF));
		u16Tmp = u8Tmp;
		u16Tmp = (u16Tmp << 8);
		VBI_W2BYTE_H(VBI_BANK_ADDR, 0x0E, u16Tmp);
	}

	
	/* Initial VBI Buffer Field Number */
	//HAL_VBI_Write2Byte(VBI_BUF_LEN, packetCount);   /* no need to minus 1, follow Venus design */
	//0x1037 3b
	u16Tmp = (packetCount << 8);
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x1D, u16Tmp);
	u16Tmp = (packetCount >> 8);
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x1E, u16Tmp);

	/* change dram access mode
      * 0x371F, BIT7 must be always 1 (otherwise the DMA access related function could be error) */
    //HAL_VBI_WriteByte(TTDEC_COMMAND, _BIT1|_BIT7);   /* put header packet into VBI without decoder */
	//0x1037 1F 0x0082
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x0F, 0x8200);

#endif
}

static void VBI_TTXInit(void )
{

    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
    MS_U8 u8value = 0;
	MS_U16 u16value =0;
//MDrv_STR_Write2Byte(CLKGEN,0x4c,vdec_save_reg.vdec_ip_clock[0x4c]);
    // close caption slicer threshold mode
	// close caption line start 1 (lower 3 bits) = 0
	// close caption lin end 1 = 0
    MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x20, 0x0001);
    // close caption line start 2 = 0
    // close caption CRI zero crossing type :  positive edge
    // close caption clock run-in amplitude upper threshold (upper 2 bits) = 0b01
    // close caption line end 2 = 0
    // close caption multi-line acquisition mode : 1
    // close caption zero crossing mode : normal.
    // close caption SYNC Found enable mode : 1
    MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x28, 0xA060);


    // teletext clock run-in amplitude accumulation start point. : 0b00010001
    // For eye-height testing
	//u16Val = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x3B);
	//u16Val = (u16Val & (0x00FF));
    //MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x3B, (0x1100 | u16Val));
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x3B, 0x1100);

    // teletext clock run-in amplitude accumulation start point. : 0b00011010
    // For SuperVHS decode issue
    //HAL_VBI_WriteByte(TT_CLK_RUN_IN_START_POINT, 0x1A);
    // teletext VBI line start 1 (odd field) : 0b00100
    // teletext VBI line end (lower 3 bits) 0b000
    //u16Val = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x3E);
	//u16Val = (u16Val & (0xFF00));
	//MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x3E, (u16Val | 0x0004));
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x3E, 0x0004);

    // teletext data line end 1 (odd field) : 0b10110
    // teletext slicer read mode : 0b1
    // teletext framing code error bond value : 0b0 fully match framing code.
    // teletext framing code windows mode : 0b0
    VBI_W2BYTE_H(VBI_BANK_ADDR, 0x3E, 0x3600);

    // teletext data line start 2 (even field) : 0b00100
    // teletext slicer threshold fixing mode : 0b0 adjust automatically according to TtSidDetSel
    // teletext slicer level mode : 0b0 original mode.
    // teletext initial packet counter : 0b1 packet counter increases when teletext packet is detected without upper-bound.
    // teletext data line end 2 (even field) : 0b10110
    // teletext single line point mode : 0b11 Enable ttslptrmode, start from the line when previous line is no teletext.
    // teletext base address source selecion : 0b1
    MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x3F, 0xF684);

    VBI_W2BYTE_H(VBI_BANK_ADDR, 0x40, 0x5200);
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x43, 0x00D6);

    u16value = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x44);
	u16value = (u16value >> 8);
	u8value = (MS_U8)(u16value);
	u8value = ((u8value & 0x20)|0xC2);
	u16value = (MS_U16)u8value;
	u16value = (u16value << 8);
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x44, u16value);

	MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x45, 0x2442);
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x46, 0xA500);
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x62, 0x0032);
    VBI_W2BYTE_H(VBI_BANK_ADDR, 0x65, 0xC400);
    VBI_W2BYTE_L(VBI_BANK_ADDR, 0x66, 0x00BD);

    // For VPS detect speed up
    MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x5A, 0x6142);
    VBI_W2BYTE_H(VBI_BANK_ADDR, 0x5D, 0x0600);
    VBI_W2BYTE_L(VBI_BANK_ADDR, 0x70, 0x0080);

}

static void VBI_CC_Init(MS_PHYADDR u32Addr, MS_U16 u16Len)
{
    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
    MS_U8 u8Tmp;
    MS_U16 u16Tmp;
    // U8 u8Bank; <- remove the bank change code
    //reset and enable closed caption
    // u8Bank = XBYTE[BK_SELECT_00]; <- remove the bank change code
    // XBYTE[BK_SELECT_00] = REG_BANK_VBI; <- remove the bank change code
    VBI_W2BYTE_L(VBI_BANK_ADDR, 0x23, 0x0000);//disable VBI
#if 1 //debug usage for register io correct check
    VBI_W2BYTE_H(VBI_BANK_ADDR, 0x01, 0xAE00);//debug
    STR_VBI_DEBUG(printk("[STR_VBI][%s]On purpose 0x01 high [0xAE]\n", __FUNCTION__));
#endif

    /* --- setup CC Bytes buffer --- */
    /* set cc base address */
    //printf("\n init adr=0x%x, %d", u32Addr, u8Len);
    u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x0E);
	u8Tmp = (u16Tmp >> 8);
    u8Tmp &= ~(0xF0);
    u8Tmp |= (((u32Addr >> 27)& 0x0F) << 4);
    u16Tmp = u8Tmp;
	u16Tmp = (u16Tmp << 8);
    VBI_W2BYTE_H(VBI_BANK_ADDR , 0x0E, u16Tmp); // CcBaseAddr_24, 25

    u8Tmp = (u32Addr >> 19) & 0xFF;
	u16Tmp = u8Tmp;
	u16Tmp = (u16Tmp << 8);
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x2E, u16Tmp);// CcBaseAddr_23_16 (default value is 0xFF so it needs to specify to 0x00)

    u16Tmp = u8Tmp = (u32Addr >> 11) & 0xFF;
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x2F, u16Tmp);// CcBaseAddr_15_8 (high order)


    u16Tmp = u8Tmp =  (u32Addr >> 3) & 0xFF;
	u16Tmp = (u16Tmp << 8);
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x2F, u16Tmp);// CCBaseAddr_7_0 (low order)

    /* set cc buffer length */
    u16Tmp = u16Len;
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x2E, u16Tmp);

	STR_VBI_DEBUG(printk("[STR_VBI][%s]enable VBI [0x2E]lowByte=leng[0x%x]\n", __FUNCTION__, u16Tmp));

    // CJ
	u16Tmp = MDrv_STR_Read2Byte(AFEC_REG_BASE, 0x35);
	u16Tmp = u16Tmp & 0xF700;
	VBI_W2BYTE_H(VBI_BANK_ADDR, 0x35, u16Tmp);

    // Set a constraint for CC patterns in case of wrong encoder's behavior
    u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x25);
	u16Tmp = (u16Tmp & 0x00F0)|(0x0003);
	VBI_W2BYTE_L(VBI_BANK_ADDR, 0x25, u16Tmp);

    VBI_W2BYTE_L(VBI_BANK_ADDR, 0x23, 0x0001);//enable VBI
    STR_VBI_DEBUG(printk("[STR_VBI][%s]enable VBI [0x23]lowByte[0x0001]\n", __FUNCTION__));


    // disable vbi software reset
    u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x38);
    u16Tmp &= ~(0x0008);
    VBI_W2BYTE_L(VBI_BANK_ADDR, 0x38, u16Tmp);

}



static MS_BOOL MDrv_VBI_ReInit(VBI_INIT_TYPE type)
{
#if IO_MAP		
		MS_U32 u32_VBIbufferAddr = 0;
		MS_U16 u16_VBICClength = CC_BUF_LEN_MAX;
		MS_U32 bufferedPacketNum = 0;

		
		u32_VBIbufferAddr = MDrv_VBI_GetBuff(type);
		if (u32_VBIbufferAddr==0)
			return FALSE;
	#if 0	
		if(!dram_VBIbuf)
		{
			STR_VBI_DEBUG(printk("[STR_VBI]ioremap E_MMAP_ID_VBI_BUF_START\n"));
			dram_VBIbuf = (char *)(ioremap(u32_VBIbufferAddr+CPU_BUS_INTERVAL,VBI_BUF_START_LEN));
		}else
		{
			STR_VBI_DEBUG(printk("[STR_VBI][ERR]ioremap E_MMAP_ID_VBI_BUF_START already !!\n"));
		}
		
		if(dram_VBIbuf == NULL)
		{
			STR_VBI_DEBUG(printk("[STR_VBI][ERR][%s][%d]dram_VBIbuf == NULL\n", __FUNCTION__, __LINE__));
				//return;
		}

			//STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]Into GetMMAPInfo BufAddr=[0x%lx] Len[%d]\n", __FUNCTION__, __LINE__, u32_VBIbufferAddr, u16_VBICClength));

		iounmap(dram_VBIbuf);
		dram_VBIbuf = NULL;
	#endif	
#endif


	if(type==E_VBI_TELETEXT)
    {
        STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
		//step 1 [VBI_TTXInit]
        VBI_TTXInit();
		
#if IO_MAP
        bufferedPacketNum = (VBI_BUF_START_LEN)/(VBI_TTX_DATA_LENGTH);
        if(bufferedPacketNum > TTX_MAX_BUFFERED_PACCKETS)
        {
            bufferedPacketNum = TTX_MAX_BUFFERED_PACCKETS;
        }
		STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]Into TTX GetMMAPInfo BufAddr=[0x%lx] PktCnt[%d]\n", __FUNCTION__, __LINE__, u32_VBIbufferAddr, bufferedPacketNum));
        VBI_TTX_InitSlicer(u32_VBIbufferAddr, bufferedPacketNum);
#else
		
        //step 2[VBI_TTX_InitSlicer]
        STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]Into TTX BufAddr=[0x%lx] PktCnt[%d]\n", __FUNCTION__, __LINE__,_pStoreInfo.u32Addr,  _pStoreInfo.u16PktCnt_TTX));
        VBI_TTX_InitSlicer(_pStoreInfo.u32Addr, _pStoreInfo.u16PktCnt_TTX);
#endif
		//step 3[MDrv_VBI_RingBuffer_Reset]
		//_RingBuffer_Reset(); //No need for update the driver flag.
		
    }
    else  //type = E_VBI_CC
    {
        STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
		//step 1 [ VBI_CC_Init]
#if IO_MAP		
		STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]Into CC GetMMAPInfo BufAddr=[0x%lx] Len[%d]\n", __FUNCTION__, __LINE__, u32_VBIbufferAddr, u16_VBICClength));
        VBI_CC_Init(u32_VBIbufferAddr, u16_VBICClength);
#else
        STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]Into CC BufAddr=[0x%lx] Len[%d]\n", __FUNCTION__, __LINE__,_pStoreInfo.u32Addr, _pStoreInfo.u16Len_CC));
        VBI_CC_Init(_pStoreInfo.u32Addr, _pStoreInfo.u16Len_CC);
#endif
		//step 2  [MDrv_VBI_CC_EnableSlicer]
		VBI_CC_EnableSlicer_Reset();

    }
	return TRUE;
}

static MS_BOOL MDrv_VBI_STR_Suspend(void)
{
    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
    MS_U16 u16Tmp;
    MS_U16 u16Tmp2;
	if(_pStoreInfo._bSuspend != FALSE)
    {
        STR_VBI_DEBUG(printk("[STR_VBI][Err][%s][%d] Already suspend\n", __FUNCTION__, __LINE__));
        return FALSE;
    }
    _pStoreInfo._bSuspend = TRUE;

    u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x38);
	u16Tmp2 = MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x24);
	if((u16Tmp & 0x0080) == 0x0080)   //VBI Teletext case
	{
	    STR_VBI_DEBUG(printk("[STR_VBI][%s]Suspend TTX\n", __FUNCTION__));
	    _pStoreInfo.eInitType = E_VBI_TELETEXT;
		//saving the buffer address for TTX  --[START]

		//addr = addr >> 3; /* 8 byte-aligned */
        /* Initial VBI Buffer Start Address */
	    //1037 38
	    //MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x1C, addr); 
		u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x1C);
	    // 1037 3A
        //MDrv_STR_Write2Byte(VBI_BANK_ADDR, 0x1D, addr >> 16); 
        
		u16Tmp2 = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x1D);
		_pStoreInfo.u32Addr = u16Tmp2; 
		_pStoreInfo.u32Addr = (_pStoreInfo.u32Addr << 16) + u16Tmp;
		_pStoreInfo.u32Addr = _pStoreInfo.u32Addr << 3 ;


        //u16Tmp = (packetCount << 8);
	    //VBI_W2BYTE_H(VBI_BANK_ADDR, 0x1D, u16Tmp);
	    //u16Tmp = (packetCount >> 8);
	    //VBI_W2BYTE_L(VBI_BANK_ADDR, 0x1E, u16Tmp);
		_pStoreInfo.u16PktCnt_TTX =  MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x1D);
		_pStoreInfo.u16PktCnt_TTX = (_pStoreInfo.u16PktCnt_TTX >> 8) ;
		u16Tmp = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x1E);
		_pStoreInfo.u16PktCnt_TTX = _pStoreInfo.u16PktCnt_TTX + (u16Tmp << 8);
		
        STR_VBI_DEBUG(printk("[STR_VBI][%s]Suspend TTX Addr[0x%lx] PktCnt[%d]\n", __FUNCTION__, _pStoreInfo.u32Addr,_pStoreInfo.u16PktCnt_TTX ));		
		//saving the buffer address for TTX  --[END]

	}else if((u16Tmp2 & 0x0001) == 0x0001)  //VBI CC case
	{
	    STR_VBI_DEBUG(printk("[STR_VBI][%s]Suspend CC\n", __FUNCTION__));
        _pStoreInfo.eInitType = E_VBI_CC;
		_pStoreInfo.u16Len_CC = ((MDrv_STR_Read2Byte(VBI_BANK_ADDR, 0x2E)) & (0x00FF)) ;
    
		//saving the buffer address for CC  --[START]
		u32Addr_27_31 = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x0E);
		STR_VBI_DEBUG(printk("[STR_VBI][%s]0x0E[0x%x] \n", __FUNCTION__, u32Addr_27_31));
		u32Addr_27_31 = (u32Addr_27_31>>12);

		u32Addr_19_26 = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x2E);
		STR_VBI_DEBUG(printk("[STR_VBI][%s]0x2E[0x%x] \n", __FUNCTION__, u32Addr_19_26));
		u32Addr_19_26 = (u32Addr_19_26>>8);

		u32Addr_11_18 = MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x2F);
		STR_VBI_DEBUG(printk("[STR_VBI][%s]0x2F[0x%x] \n", __FUNCTION__, u32Addr_11_18));

		u32Addr_3_10 =  MDrv_STR_Read2Byte(VBI_BANK_ADDR,0x2F);
		u32Addr_3_10 = (u32Addr_3_10>>8);

		_pStoreInfo.u32Addr = (u32Addr_3_10 << 3) + (u32Addr_11_18 << 11) + (u32Addr_19_26 << 19) + (u32Addr_27_31 << 27 );
        STR_VBI_DEBUG(printk("[STR_VBI][%s]Suspend CC Addr[0x%lx] Length[%d]\n", __FUNCTION__, _pStoreInfo.u32Addr, _pStoreInfo.u16Len_CC ));
		//saving the buffer address for CC  --[END]

	}else
	    return FALSE;

		
	return TRUE;
}

#if VBI_BANK_APPROACH

static void vbi_store_bank(void)
{
    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
    MS_U16 reg16Index = 0;
    for(reg16Index = 0; reg16Index < VBI_BANK_SIZE; reg16Index++)
    {
        u16_VBIbank[reg16Index] = MDrv_STR_Read2Byte(VBI_BANK_ADDR, reg16Index);
    }
	STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]Store vbi bank complete\n", __FUNCTION__, __LINE__));
}

static void vbi_load_bank(void)
{
    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
    MS_U16 reg16Index = 0;
    for(reg16Index = 0; reg16Index < VBI_BANK_SIZE; reg16Index++)
    {
        MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIbank[reg16Index]);
    }
	STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]load(resume) vbi bank complete\n", __FUNCTION__, __LINE__));
}

#endif

static void _vbi_save_standard_reg(void)
{
//save the standard reg both for CC and TTX 
//Base on function : MApi_VBI_SetVideoStandaredCC and  MApi_VBI_SetVideoStandardTTX 
//key register for standard ttx 8 bit [ 0x82 0x83 0x89 0x99 0x9A]
//key register for CC frame rate 8 bit[0x41 0x42 0x44 0x4b 0x4d 0x50 0x51]

    //Save the CC frame rate reg
    MS_U16 reg16Index = 0;
    for(reg16Index = 0x20; reg16Index < 0x28; reg16Index++)
    {
        u16_VBIstandardReg[reg16Index] = MDrv_STR_Read2Byte(VBI_BANK_ADDR, reg16Index);
    }

    //Save the TTX standard reg 16bit [0x41 0x44 0x4c 0x4d]
    reg16Index = 0x41;
    u16_VBIstandardReg[reg16Index] = MDrv_STR_Read2Byte(VBI_BANK_ADDR, reg16Index);
	reg16Index = 0x44;
    u16_VBIstandardReg[reg16Index] = MDrv_STR_Read2Byte(VBI_BANK_ADDR, reg16Index);
	reg16Index = 0x4c;
    u16_VBIstandardReg[reg16Index] = MDrv_STR_Read2Byte(VBI_BANK_ADDR, reg16Index);
	reg16Index = 0x4d;
    u16_VBIstandardReg[reg16Index] = MDrv_STR_Read2Byte(VBI_BANK_ADDR, reg16Index);
	
}

static void _vbi_load_standard_reg(void)
{
    MS_U16 reg16Index = 0;
	if(_pStoreInfo.eInitType == E_VBI_CC)
	{
	    //restore the CC frame rate reg copy from [VBI_CC_DataRateSet]
        reg16Index =  0x20; //8bit 0x41
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
		
		reg16Index =  0x21; //8bit 0x42
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, (u16_VBIstandardReg[reg16Index] & 0x00C0));
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
		
		reg16Index =  0x22; //8bit 0x44
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
		
		reg16Index =  0x25; //8bit 0x4B
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, (u16_VBIstandardReg[reg16Index] & 0xC000));
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);

		
		reg16Index =  0x26; //8bit 0x4D
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);

		
		reg16Index =  0x28; //8bit 0x50
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, (u16_VBIstandardReg[reg16Index] & 0x00E0));
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);

		reg16Index =  0x28; //8bit 0x51
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, (u16_VBIstandardReg[reg16Index] & 0xE000));
		MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);		

	}else
	{
        //restore the TTX standard reg copy from[VBI_Set_PalNC_VideoStandard]
        reg16Index =  0x41;
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
		// HAL_VBI_WriteByte(BK_VBI_89, HAL_VBI_ReadByte(BK_VBI_89)|(_BIT6));
	    reg16Index =  0x44;
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, (u16_VBIstandardReg[reg16Index]| 0x4000));
		// for VPS
	    reg16Index =  0x44;
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
	    reg16Index =  0x4c;
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
	    reg16Index =  0x4d;
	    MDrv_STR_Write2Byte(VBI_BANK_ADDR, reg16Index, u16_VBIstandardReg[reg16Index]);
	}
}

static void vbi_store_reg(void)
{
    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
	MDrv_VBI_STR_Suspend();
	_vbi_save_standard_reg();
}

static void vbi_load_reg(void)
{
    STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
	MDrv_VBI_ReInit(_pStoreInfo.eInitType);
	_vbi_load_standard_reg();
}

extern int suspend_vbi(void)
{
    STR_PRINT("enter\n");
	STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
#if VBI_BANK_APPROACH 
    vbi_store_bank();
#else
    vbi_store_reg();
#endif
    STR_PRINT("exit\n");
    return 0;
}

extern int resume_vbi(void)
{
    STR_PRINT("enter\n");
	STR_VBI_DEBUG(printk("[STR_VBI][%s][%d]\n", __FUNCTION__, __LINE__));
#if VBI_BANK_APPROACH
    vbi_load_bank();
#else
    vbi_load_reg();
#endif
    STR_PRINT("exit\n");
    return 0;
}


