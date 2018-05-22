#include <linux/device.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/delay.h>

#include "mst_str.h"
#include "mst_reg_bank.h"

#define BIT0    (0x0001)
#define BIT1    (0x0002)
#define BIT2    (0x0004)
#define BIT3    (0x0008)
#define BIT4    (0x0010)
#define BIT5    (0x0020)
#define BIT6    (0x0040)
#define BIT7    (0x0080)
#define BIT8    (0x0100)
#define BIT9    (0x0200)
#define BIT10   (0x0400)
#define BIT11   (0x0800)
#define BIT12   (0x1000)
#define BIT13   (0x2000)
#define BIT14   (0x4000)
#define BIT15   (0x8000)

// This should be move to Common bank define //
//#define CHIPTOP_BANK_ADDR                   0x101E00
#define REG_TOP_PCM_PE_0                    0x09
#define REG_TOP_PCM_PE_1                    0x0A
#define REG_TOP_PCM_PE_MASK_0               0xFFFF
#define REG_TOP_PCM_PE_2                    0x0B
#define REG_TOP_PCM_PE_MASK_1               0x03

#define MIIC_BANK_ADDR                      0x103400
#define PCMCIA_BANK_ADDR                    0x100B00
#define PCMCIA_CLK                          0x1A
#define EN_PCMCIA_CLK_MASK                  0xFFFE
#define REG_PCMCIA_PCM_MEM_IO_CMD           0x20
#define REG_PCMCIA_ADDR                     0x21        //ADDR [14:0]
//#define REG_PCMCIA_ADDR1                    0x22
#define REG_PCMCIA_WRITE_DATA               0x22        //Write DATA [7:0]
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     0x23        //Bit0 = 1: Fire, Bit1 = 1: Clear read done
//#define REG_PCMCIA_READ_DATA                0x24
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  0x24        //Read Data [7:0]
                                                        //Bit8 = 1: Read CMD done, Bit9 = 1: Bus Idle,
                                                        //BitA = 1: Card Detect for A, BitB = 1: Card Detect for B,
#define REG_PCMCIA_INT_MASK_CLEAR           0x25        //Mask of INT
//#define REG_PCMCIA_INT_MASK_CLEAR1          0x2B
#define REG_PCMCIA_MODULE_VCC_OOB           0x28        //[1:0] Module Select, [2] Module A reset, [3] Module B reset, [4] Only support 1 Module,
                                                        //[5] VCC EN, [6] Timing adjust

#define REG_PCMCIA_2C                       0x2C
#define REG_PCMCIA_2D                       0x2D
//#define REG_PCMCIA_STAT_INT_RAW_INT         0x2E
//#define REG_PCMCIA_STAT_INT_RAW_INT1        0x2F

#define PCMCIA_ATTRIBMEMORY_READ            0x03
#define PCMCIA_ATTRIBMEMORY_WRITE           0x04
#define PCMCIA_IO_READ                      0x05
#define PCMCIA_IO_WRITE                     0x06

#define PCM_OOB_BIT_MASK                    0x03
#define PCM_OOB_BIT_SHFT                    6

#define PCM_8BIT_MASK                       0xF
#define PCM_16BIT_MASK                      0xFF
#define PCM_OOB_CYCLE_EXTEND                0x3
#define PCM_Delay_Duration                  20 //ms

typedef enum {
    E_PCMCIA_MODULE_A           = 0,
    E_PCMCIA_MODULE_B           = 1,
    E_PCMCIA_MODULE_MAX         = 2
} PCMCIA_MODULE;

MS_U16 u16PECtrl[3];            ///chip top 0x9 ~ 0xb 34 bits
MS_U16 u16PCM_VCC_OOB = 0;      //REG_PCMCIA_MODULE_VCC_OOB     0x30
MS_U16 u16PCMCIAIrg = 0;
#define DEBUG_PCM_MSG 1



/********************************************************************
    Common API
********************************************************************/
void PCMCIA_INT_Enable(PCMCIA_MODULE eModule, MS_BOOL bEnable)
{
    MS_U16 u16Reg = 0;
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif

    if(bEnable == TRUE)
    {
#if (DEBUG_PCM_MSG == 1)   //Read back
        STR_PRINT(" Enable INT 1st [reg = 0x%lx] = 0x%x ", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif

        u16Reg = MDrv_STR_Read2ByteMsk(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, PCM_16BIT_MASK);
        if(eModule == E_PCMCIA_MODULE_A)
        {
            u16Reg &= ( ~BIT2 );
            u16Reg &= ( ~BIT1 );
            u16Reg &= ( ~BIT0 );
        }
        else
        { // Module B
            u16Reg &= ( ~BIT5 );
            u16Reg &= ( ~BIT4 );
            u16Reg &= ( ~BIT3 );
        }
        MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, u16Reg);
#if (DEBUG_PCM_MSG == 1)   //Read back
        STR_PRINT(" 2st [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif

    }
    else
    {
        /* Here DON"T Disable MPU PCMCIA IRQ. */
        /* Disable IP PCMCIA IRQ. */
#if (DEBUG_PCM_MSG == 1)   //Read back
        STR_PRINT(" Disable INT 1st [reg = 0x%lx] = 0x%x ", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif
        u16Reg = MDrv_STR_Read2ByteMsk(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, PCM_16BIT_MASK);
        if(eModule == E_PCMCIA_MODULE_A)
        {
            u16Reg |= BIT2; //Don't mask cardA insert/remove
        }
        else
        { // Module B
            u16Reg |= BIT5;
        }
        MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, u16Reg);
#if (DEBUG_PCM_MSG == 1)   //Read back
        STR_PRINT(" 2st [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif
    }

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}

//------------- Save PCMCIA Reg  -------------//
void PCMCIA_Save_PAD_Ctrl(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    u16PECtrl[0] = MDrv_STR_Read2ByteMsk(ADC_CHIPTOP, REG_TOP_PCM_PE_0, REG_TOP_PCM_PE_MASK_0);
    u16PECtrl[1] = MDrv_STR_Read2ByteMsk(ADC_CHIPTOP, REG_TOP_PCM_PE_1, REG_TOP_PCM_PE_MASK_0);
    u16PECtrl[2] = MDrv_STR_Read2ByteMsk(ADC_CHIPTOP, REG_TOP_PCM_PE_2, REG_TOP_PCM_PE_MASK_1);

#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x ", (unsigned long)(ADC_CHIPTOP + REG_TOP_PCM_PE_0), (int)MDrv_STR_Read2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_0));
    STR_PRINT(" [reg = 0x%lx] = 0x%x ", (unsigned long)(ADC_CHIPTOP + REG_TOP_PCM_PE_1), (int)MDrv_STR_Read2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_1));
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(ADC_CHIPTOP + REG_TOP_PCM_PE_2), (int)MDrv_STR_Read2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_2));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}


void PCMCIA_Save_VCC_OOB(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    u16PCM_VCC_OOB = MDrv_STR_Read2ByteMsk(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB, PCM_16BIT_MASK);
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_MODULE_VCC_OOB), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}

void PCMCIA_Save_Irq(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    u16PCMCIAIrg = MDrv_STR_Read2ByteMsk(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, PCM_16BIT_MASK);
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}
//------------- Load PCMCIA Reg  -------------//
void PCMCIA_Load_PAD_Ctrl(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    MDrv_STR_Write2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_0, u16PECtrl[0]);
    MDrv_STR_Write2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_1, u16PECtrl[1]);
    MDrv_STR_Write2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_2,
        ( (MDrv_STR_Read2ByteMsk(ADC_CHIPTOP, REG_TOP_PCM_PE_2, ~REG_TOP_PCM_PE_MASK_1)) | u16PECtrl[2] ));

#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x ", (unsigned long)(ADC_CHIPTOP + REG_TOP_PCM_PE_0), (int)MDrv_STR_Read2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_0));
    STR_PRINT(" [reg = 0x%lx] = 0x%x ", (unsigned long)(ADC_CHIPTOP + REG_TOP_PCM_PE_1), (int)MDrv_STR_Read2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_1));
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(ADC_CHIPTOP + REG_TOP_PCM_PE_2), (int)MDrv_STR_Read2Byte(ADC_CHIPTOP, REG_TOP_PCM_PE_2));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}

void PCMCIA_Load_Irq(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, u16PCMCIAIrg);
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}

void PCMCIA_Load_VCC_OOB(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB, u16PCM_VCC_OOB);
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_MODULE_VCC_OOB), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}

void PMCIA_Init(void)
{
#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    PCMCIA_Load_VCC_OOB();
    PCMCIA_Load_Irq();
    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, 0x7C);   // unmask cardA insert/ remove
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif

}

void PMCIA_HW_Reset_part1(void)
{
    MS_U8 u8Reg = 0;
    MS_U8 bit = BIT2;

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif
    PCMCIA_INT_Enable(E_PCMCIA_MODULE_A, FALSE);

    u8Reg &= ~(BIT4);         // SINGLE_CARD: Only support single card
                            //                          0: support 2 cards
                            //                          1: support 1 card only
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" Read [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_MODULE_VCC_OOB), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB));
#endif

    u8Reg = MDrv_STR_Read2ByteMsk(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB, u16PCM_VCC_OOB);
    u8Reg |= bit;           //                          1: RESET = HIGH

    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB, u8Reg);
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" Write [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_MODULE_VCC_OOB), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB));
#endif

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif
}

void PMCIA_HW_Reset_part2(void)
{
    MS_U8 u8Reg = 0;
    MS_U8 bit = BIT2;

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("enter\n");
#endif

    u8Reg = MDrv_STR_Read2ByteMsk(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB, u16PCM_VCC_OOB);
    u8Reg &= ~bit;          //                          0: RESET = LOW

    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB, u8Reg);
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" Reset [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_MODULE_VCC_OOB), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_MODULE_VCC_OOB));
#endif

    /* Comment? */  // FIXME_Alec
    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_2C, 0x000C);
    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_2D, 0x0031);

    /* Reset PCMCIA IRQ Mask. */
    MDrv_STR_Write2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR, 0x7C);   // unmask cardA insert/ remove
#if (DEBUG_PCM_MSG == 1)   //Read back
    STR_PRINT(" [reg = 0x%lx] = 0x%x \n", (unsigned long)(MIIC_BANK_ADDR + REG_PCMCIA_INT_MASK_CLEAR), (int)MDrv_STR_Read2Byte(MIIC_BANK_ADDR, REG_PCMCIA_INT_MASK_CLEAR));
#endif

    /* Set PCMCIA CLK on */
    int reg = MDrv_STR_Read2Byte(PCMCIA_BANK_ADDR, PCMCIA_CLK);
    MDrv_STR_Write2Byte(PCMCIA_BANK_ADDR, PCMCIA_CLK, reg & EN_PCMCIA_CLK_MASK);

#if (DEBUG_PCM_MSG == 1)
    STR_PRINT("exit\n");
#endif

}

static void _save_pcmcia_reg(void)
{
    STR_PRINT("enter\n");
    PCMCIA_Save_PAD_Ctrl();
    PCMCIA_Save_VCC_OOB();
    PCMCIA_Save_Irq();
    STR_PRINT("exit\n");
}

static void _load_pcmcia_reg(void)
{
    STR_PRINT("enter\n");
    PMCIA_HW_Reset_part2();
    STR_PRINT("exit\n");
}

/********************************************************************
    MFE Define
********************************************************************/

extern void preinit_pcmcia(void)
{
    STR_PRINT("enter\n");
    PCMCIA_Load_PAD_Ctrl();
    PMCIA_Init();
    PMCIA_HW_Reset_part1();
    STR_PRINT("exit\n");
}

extern int suspend_pcmcia(void)
{
    STR_PRINT("enter\n");
    _save_pcmcia_reg();
    STR_PRINT("exit\n");
    return 0;
}

extern int resume_pcmcia(void)
{
    STR_PRINT("enter\n");
    preinit_pcmcia();
    _load_pcmcia_reg();
    STR_PRINT("exit\n");
    return 0;
}
