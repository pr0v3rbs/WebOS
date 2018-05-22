#include <linux/device.h>
#include <linux/export.h>
#include <linux/io.h>
#include "drvPM.h"
#include "drvGPIO.h"
#include "mst_str.h"
#include "mst_reg_bank.h"

//#define STR_REG_RW_DEBUG

typedef struct
{
    MS_U16 miu0_config_bank[6];//4sel,2ssc
    MS_U16 miu1_config_bank[6];//4sel,2ssc
} sys_MIU_Select_save_reg_t;

static sys_MIU_Select_save_reg_t MiuSelect_save_reg;

static void sys_MiuSelect_store_reg(void)
{
    MiuSelect_save_reg.miu0_config_bank[0] = MDrv_STR_Read2Byte(MIU0_BASE,0x78);
    MiuSelect_save_reg.miu0_config_bank[1] = MDrv_STR_Read2Byte(MIU0_BASE,0x79);
    MiuSelect_save_reg.miu0_config_bank[2] = MDrv_STR_Read2Byte(MIU0_BASE,0x7A);
    MiuSelect_save_reg.miu0_config_bank[3] = MDrv_STR_Read2Byte(MIU0_BASE,0x7B);
    MiuSelect_save_reg.miu0_config_bank[4] = MDrv_STR_Read2Byte(MIU_ATOP0,0x14);//ssc
    MiuSelect_save_reg.miu0_config_bank[5] = MDrv_STR_Read2Byte(MIU_ATOP0,0x15);//ssc

    MiuSelect_save_reg.miu1_config_bank[0] = MDrv_STR_Read2Byte(MIU1_BASE,0x78);
    MiuSelect_save_reg.miu1_config_bank[1] = MDrv_STR_Read2Byte(MIU1_BASE,0x79);
    MiuSelect_save_reg.miu1_config_bank[2] = MDrv_STR_Read2Byte(MIU1_BASE,0x7A);
    MiuSelect_save_reg.miu1_config_bank[3] = MDrv_STR_Read2Byte(MIU1_BASE,0x7B);
    MiuSelect_save_reg.miu1_config_bank[4] = MDrv_STR_Read2Byte(MIU_ATOP1,0x14);//ssc
    MiuSelect_save_reg.miu1_config_bank[5] = MDrv_STR_Read2Byte(MIU_ATOP1,0x15);//ssc
}
static void sys_MiuSelect_load_reg(void)
{
    //set group 0 MIU Selection
    MDrv_STR_Write2Byte(MIU0_BASE, 0x78, MiuSelect_save_reg.miu0_config_bank[0]);
    MDrv_STR_Write2Byte(MIU1_BASE, 0x78, MiuSelect_save_reg.miu1_config_bank[0]);

    //set group 1 MIU Selection
    MDrv_STR_Write2Byte(MIU0_BASE, 0x79, MiuSelect_save_reg.miu0_config_bank[1]);
    MDrv_STR_Write2Byte(MIU1_BASE, 0x79, MiuSelect_save_reg.miu1_config_bank[1]);

    //set group 2 MIU Selection
    MDrv_STR_Write2Byte(MIU0_BASE, 0x7A, MiuSelect_save_reg.miu0_config_bank[2]);
    MDrv_STR_Write2Byte(MIU1_BASE, 0x7A, MiuSelect_save_reg.miu1_config_bank[2]);

    //set group 3 MIU Selection
    MDrv_STR_Write2Byte(MIU0_BASE, 0x7B, MiuSelect_save_reg.miu0_config_bank[3]);
    MDrv_STR_Write2Byte(MIU1_BASE, 0x7B, MiuSelect_save_reg.miu1_config_bank[3]);

    //ssc
    MDrv_STR_Write2Byte(MIU_ATOP0, 0x14, MiuSelect_save_reg.miu0_config_bank[4]);
    MDrv_STR_Write2Byte(MIU_ATOP0, 0x15, MiuSelect_save_reg.miu0_config_bank[5]);
    MDrv_STR_Write2Byte(MIU_ATOP1, 0x14, MiuSelect_save_reg.miu1_config_bank[4]);
    MDrv_STR_Write2Byte(MIU_ATOP1, 0x15, MiuSelect_save_reg.miu1_config_bank[5]);
}

extern int suspend_sys(void)
{
    STR_PRINT("enter\n");

#define GPI_PM_OPT1 43
        //Low:       Internal Micom
        //HIGH:     External Micom
    if(_mdrv_gpio_get_level(GPI_PM_OPT1))
        MDrv_PM_LoadWakeUp51_Internal();

    sys_MiuSelect_store_reg();
    STR_PRINT("exit\n");
    return 0;
}

extern int resume_sys(void)
{
    STR_PRINT("enter\n");
    sys_MiuSelect_load_reg();

#define GPI_PM_OPT1 43
        //Low:       Internal Micom
        //HIGH:     External Micom
    if(_mdrv_gpio_get_level(GPI_PM_OPT1))
        MDrv_PM_DisablePM51_Internal();

    STR_PRINT("exit\n");
    return 0;
}

extern int resume_hw_preinit(void)
{
    // declare prototype
    extern void mstar_mci_preinit_emmc(void);
    extern void preinit_pcmcia(void);

    STR_PRINT("enter\n");
//    mstar_mci_preinit_emmc();
//    preinit_pcmcia();
    STR_PRINT("exit\n");
    return 0;
}

