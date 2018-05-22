#include <linux/device.h>
#include <linux/export.h>
#include <linux/io.h>

#include "mst_str.h"


/********************************************************************
    Common API
********************************************************************/
#define MDrv_STR_MFE_SAVE_REG(x, y)         x = readl((void *) y)
#define MDrv_STR_MFE_LOAD_REG(x, y)         writel(x, ((void *) y))
#define MDrv_STR_MFE_SAVE_REGS(x, y, n)do { static int i=0; for(i = 0; i < n ; i++) x[i] = readl((void *) y + i * 4);} while(0)
#define MDrv_STR_MFE_LOAD_REGS(x, y, n)do { static int i=0; for(i = 0; i < n ; i++) writel(x[i], ((void *) y + i * 4));} while(0)

#define DUMP_REG_VALUE(u16Reg_in,reg_start)                    \
    do{ static int cnt=0;static int sz =0; int reg_idx = reg_start ;   \
            sz = sizeof(u16Reg_in)/sizeof(MS_U16);                         \
            for(cnt = 0; cnt < sz ; cnt++)                                        \
            {                                                                               \
                printk("[%02X]0x%04X  ",(int)reg_idx,u16Reg_in[cnt]);       \
                reg_idx+=1;                                                             \
                if((cnt+1) % 8 == 0)  printk("\n");                               \
            } printk("\n");                                                                \
        }while(0)


/********************************************************************
    MFE Define
********************************************************************/
#define MFE_STR_DEBUG   0
#define MFE_BANK0_BASE 0x111000
#define MFE_BANK1_BASE 0x111100

#define MFE_SAVE_BANK0_SIZE 32
#define MFE_SAVE_BANK1_SIZE 5
#define MDrv_STR_GET_REG_ADDRESS(BANK_IN,REG_IN, OUT) do {OUT = (((BANK_IN + (REG_IN<<1)) <<1) + RIU_ADDR) ;} while(0)


static struct str_save_mfe_t {
    MS_U16 mfe_Bank0_Reg00_03[MFE_SAVE_BANK0_SIZE];
    MS_U16 mfe_Bank1_Reg34_38[MFE_SAVE_BANK1_SIZE];
#if MFE_STR_DEBUG
    MS_U16 verify_Bank0_Reg00_03[MFE_SAVE_BANK0_SIZE];
    MS_U16 verify_Bank1_Reg34_38[MFE_SAVE_BANK1_SIZE];
#endif
} str_save_mfe;


static void str_save_mfe_reg(void)
{
    STR_PRINT("enter\n");
    MS_U16 u16Reg = 0x00;
    MS_U32 u32RegAddress = 0x00;

    memset(&str_save_mfe,0x00,sizeof(str_save_mfe));

    //save mfe bank0
    MDrv_STR_GET_REG_ADDRESS(MFE_BANK0_BASE,u16Reg,u32RegAddress);
    //STR_PRINT("u32RegAddress =0x%lx\n",u32RegAddress);
    MDrv_STR_MFE_SAVE_REGS(str_save_mfe.mfe_Bank0_Reg00_03,u32RegAddress,MFE_SAVE_BANK0_SIZE);

#if MFE_STR_DEBUG
    STR_PRINT("DUMP mfe_Bank0_Reg00_03  =====================================+\n");
    DUMP_REG_VALUE(str_save_mfe.mfe_Bank0_Reg00_03, u16Reg);
    STR_PRINT("DUMP mfe_Bank0_Reg00_03  END =================================-\n\n");
#endif

    //save mfe bank1
    u16Reg = 0x34;
    u32RegAddress = 0; //reset
    MDrv_STR_GET_REG_ADDRESS(MFE_BANK1_BASE,u16Reg,u32RegAddress);
    MDrv_STR_MFE_SAVE_REGS(str_save_mfe.mfe_Bank1_Reg34_38,u32RegAddress,MFE_SAVE_BANK1_SIZE);

#if MFE_STR_DEBUG
    STR_PRINT("DUMP mfe_Bank1_Reg34_38  =====================================+\n");
    DUMP_REG_VALUE(str_save_mfe.mfe_Bank1_Reg34_38,u16Reg);
    STR_PRINT("DUMP mfe_Bank1_Reg34_38  END =================================-\n\n");
#endif

    //Reset Resume Flag to Reg
    u16Reg = 0x7F; //Bank1, reserved
    MS_U16 u16RegValue = 0x00;
    MDrv_STR_GET_REG_ADDRESS(MFE_BANK1_BASE,u16Reg,u32RegAddress);
    u16RegValue = 0x00; //reset to 0x00
    MDrv_STR_MFE_LOAD_REG(u16RegValue,u32RegAddress);
#if MFE_STR_DEBUG
    //confrim again for Debug
    STR_PRINT("[Suspend] Check MFE Suspend Flag ===========================\n");
    MDrv_STR_MFE_SAVE_REG(u16RegValue,u32RegAddress);
    STR_PRINT("[Suspend][After] MFE1, 0x7F is 0x%04X\n",u16RegValue);
#endif

    STR_PRINT("exit\n");
}
static void str_load_mfe_reg(void)
{
    STR_PRINT("enter\n");

    MS_U16 u16Reg = 0x00;
    MS_U32 u32RegAddress = 0x00;

    //load mfe bank0
    u16Reg = 0x00;
    MDrv_STR_GET_REG_ADDRESS(MFE_BANK0_BASE,u16Reg,u32RegAddress);
    MDrv_STR_MFE_LOAD_REGS(str_save_mfe.mfe_Bank0_Reg00_03,u32RegAddress,MFE_SAVE_BANK0_SIZE);

#if MFE_STR_DEBUG
    STR_PRINT("Load mfe_Bank0_Reg00_03 to Reg  ==============================+\n");
    DUMP_REG_VALUE(str_save_mfe.mfe_Bank0_Reg00_03,u16Reg);
    STR_PRINT("DUMP mfe_Bank0_Reg00_03  END =================================-\n\n");

    //verify mfe bank0
    memset(&str_save_mfe.verify_Bank0_Reg00_03,0x00,sizeof(str_save_mfe.verify_Bank0_Reg00_03));
    MDrv_STR_MFE_SAVE_REGS(str_save_mfe.verify_Bank0_Reg00_03,u32RegAddress,MFE_SAVE_BANK0_SIZE);

    STR_PRINT("DUMP verify_Bank0_Reg00_03  ==================================+\n");
    DUMP_REG_VALUE(str_save_mfe.verify_Bank0_Reg00_03,u16Reg);
    STR_PRINT("DUMP verify_Bank0_Reg00_03  END ================================-\n\n");
#endif

    //load mfe bank1
    u16Reg = 0x34;
    MDrv_STR_GET_REG_ADDRESS(MFE_BANK1_BASE,u16Reg,u32RegAddress);
    MDrv_STR_MFE_LOAD_REGS(str_save_mfe.mfe_Bank1_Reg34_38,u32RegAddress,MFE_SAVE_BANK1_SIZE);

#if MFE_STR_DEBUG
    STR_PRINT("Load mfe_Bank1_Reg34_38 to Reg  ==========================+\n");
    DUMP_REG_VALUE(str_save_mfe.mfe_Bank1_Reg34_38,u16Reg);
    STR_PRINT("DUMP mfe_Bank1_Reg34_38  END =============================-\n\n");

    //verify mfe bank0
    memset(&str_save_mfe.verify_Bank1_Reg34_38,0x00,sizeof(str_save_mfe.verify_Bank1_Reg34_38));
    MDrv_STR_MFE_SAVE_REGS(str_save_mfe.verify_Bank1_Reg34_38,u32RegAddress,MFE_SAVE_BANK1_SIZE);

    STR_PRINT("DUMP verify_Bank1_Reg34_38 ==============================+\n");
    DUMP_REG_VALUE(str_save_mfe.verify_Bank1_Reg34_38,u16Reg);
    STR_PRINT("DUMP verify_Bank1_Reg34_38 END ==========================-\n\n");
#endif


    //Write down a Resume Flag to Reg
    u16Reg = 0x7F; //Bank1, reserved
    MS_U16 u16RegValue = 0x00;
    MDrv_STR_GET_REG_ADDRESS(MFE_BANK1_BASE,u16Reg,u32RegAddress);
#if MFE_STR_DEBUG
    STR_PRINT("[Resume] Check MFE Resume Flag ===========================\n");
    MDrv_STR_MFE_SAVE_REG(u16RegValue,u32RegAddress);
    STR_PRINT("[Before] MFE1, 0x7F is 0x%04X\n",u16RegValue);
#endif
    u16RegValue = 0x0001;
    MDrv_STR_MFE_LOAD_REG(u16RegValue,u32RegAddress);
#if MFE_STR_DEBUG
    //confrim again
    MDrv_STR_MFE_SAVE_REG(u16RegValue,u32RegAddress);
    STR_PRINT("[Resume][After] MFE1, 0x7F is 0x%04X\n",u16RegValue);
#endif

    STR_PRINT("exit\n");
}

extern int suspend_mfe(void)
{
    STR_PRINT("enter\n");
    str_save_mfe_reg();
    STR_PRINT("exit\n");
    return 0;
}

extern int resume_mfe(void)
{
    STR_PRINT("enter\n");
    str_load_mfe_reg();
    STR_PRINT("exit\n");
    return 0;
}
