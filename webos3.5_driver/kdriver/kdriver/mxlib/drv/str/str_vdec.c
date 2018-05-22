#include <linux/device.h>
#include <linux/export.h>
#include <linux/io.h>
#include "mst_str.h"
#include "mst_reg_bank.h"
#include "controller.h"
#include "../hvd_v3/drvHVD_def.h"
#include "../../hal/mustang/vpu_v3/halVPU_EX.h"
#include "../../hal/mustang/hvd_v3/regHVD_EX.h"


#define BANK_SIZE 0x80
#define SUPPORT_ALL  1
#define REG_BASE_ADDR  0x100000
#define EVD_BANK_ADDR  (REG_BASE_ADDR+REG_EVD_BASE)
typedef struct
{
    MS_U16 vdec_r2_bank[BANK_SIZE];
    MS_U16 vdec_mvop_bank[BANK_SIZE];
    MS_U16 vdec_submvop_bank[BANK_SIZE];
    MS_U16 vdec_ip_clock[BANK_SIZE];
    MS_U16 vdec_hvd_bank[BANK_SIZE];
    MS_U16 vdec_evd_bank[BANK_SIZE];
    //MS_U16 vdec_mvd_bank[BANK_SIZE]; // mvd no need to restore
    MS_U16 vdec_mau_bank[BANK_SIZE];
    MS_BOOL  vdec_is_work[4];
}vdec_save_reg_t;

static vdec_save_reg_t vdec_save_reg;

static void vdec_ip_clktrigger(void)
{
    int start_index = 0;

    MDrv_STR_Write2Byte(CLKGEN,0x05,vdec_save_reg.vdec_ip_clock[0x05]);
    MDrv_STR_Write2Byte(CLKGEN,0x06,vdec_save_reg.vdec_ip_clock[0x06]);
    MDrv_STR_Write2ByteMsk(CLKGEN,0x04,0x4,0x4);
    MDrv_STR_Write2ByteMsk(CLKGEN,0x04,0x0,0x4);
    MDrv_STR_Write2Byte(CLKGEN,0x74,vdec_save_reg.vdec_ip_clock[0x74]);
    MDrv_STR_Write2Byte(CLKGEN,0x75,vdec_save_reg.vdec_ip_clock[0x75]);
    MDrv_STR_Write2ByteMsk(CLKGEN,0x70,0x2,0x2);
    MDrv_STR_Write2ByteMsk(CLKGEN,0x70,0x0,0x2);
    MDrv_STR_Write2ByteMsk(CLKGEN,0x70,0x200,0x200);
    MDrv_STR_Write2ByteMsk(CLKGEN,0x70,0x0,0x200);
    MDrv_STR_Write2Byte(CLKGEN,0x4C,0x100);
    MDrv_STR_Write2Byte(MAU_BANK_ADDR,0x01,vdec_save_reg.vdec_mau_bank[0x01]);

    if(vdec_save_reg.vdec_is_work[0])
    {
        // build_error
        MDrv_STR_Write2ByteMsk(MVOP_BANK_ADDR,0x11,0x1,0x1);
        MDrv_STR_Write2ByteMsk(MVOP_BANK_ADDR,0x11,0x0,0x1);
        //MDrv_STR_LOAD2Byte_REGS_OFFSET(vdec_save_reg,vdec_mvop_bank,MVOP_BANK_ADDR,0,0x10);
        for(;(start_index<0x10);start_index++)
        {
            MDrv_STR_Write2Byte(MVOP_BANK_ADDR,start_index,vdec_save_reg.vdec_mvop_bank[start_index]);
        }
        for(start_index = 0x10;start_index<BANK_SIZE;start_index++)
        {
            MDrv_STR_Write2Byte(MVOP_BANK_ADDR,start_index,vdec_save_reg.vdec_mvop_bank[start_index]);
        }
        MDrv_STR_Write2ByteMsk(MVOP_BANK_ADDR,0x27,0x10,0x10);
        MDrv_STR_Write2ByteMsk(MVOP_BANK_ADDR,0x27,0x00,0x10);
    }
    if(vdec_save_reg.vdec_is_work[1])
    {
        MDrv_STR_Write2ByteMsk(SUBMVOP_BANK_ADDR,0x11,0x1,0x1);
        MDrv_STR_Write2ByteMsk(SUBMVOP_BANK_ADDR,0x11,0x0,0x1);
        // build_error
        //MDrv_STR_LOAD2Byte_REGS_OFFSET(vdec_save_reg,vdec_submvop_bank,SUBMVOP_BANK_ADDR,0,0x10);
        for(start_index = 0;(start_index<0x10);start_index++)
        {
            MDrv_STR_Write2Byte(SUBMVOP_BANK_ADDR,start_index,vdec_save_reg.vdec_submvop_bank[start_index]);
        }
        for(start_index = 0x10;start_index<BANK_SIZE;start_index++)
        {
            MDrv_STR_Write2Byte(SUBMVOP_BANK_ADDR,start_index,vdec_save_reg.vdec_submvop_bank[start_index]);
        }
        MDrv_STR_Write2ByteMsk(SUBMVOP_BANK_ADDR,0x27,0x10,0x10);
        MDrv_STR_Write2ByteMsk(SUBMVOP_BANK_ADDR,0x27,0x00,0x10);
    }
    // resume the HVD bank
    for(start_index = 0;(start_index<BANK_SIZE);start_index++)
    {
        MDrv_STR_Write2Byte(HVD_BANK_ADDR,start_index,vdec_save_reg.vdec_hvd_bank[start_index]);
    }
    #if SUPPORT_ALL
    // resume the MVD bank
    #if 0
    for(start_index = 0;(start_index<BANK_SIZE);start_index++)
    {
        MDrv_STR_Write2Byte(HVD_BANK_ADDR,start_index,vdec_save_reg.vdec_mvd_bank[start_index]);
    }
    #endif
    // resume the EVD bank
    for(start_index = 0;(start_index<BANK_SIZE);start_index++)
    {
        MDrv_STR_Write2Byte(EVD_BANK_ADDR,start_index,vdec_save_reg.vdec_evd_bank[start_index]);
    }
    #endif
    // build_error
     //R2 Clock Restore
    MDrv_STR_Write2Byte(CLKGEN,0x30,vdec_save_reg.vdec_ip_clock[0x30]);
    //MVD Clock Restore
    MDrv_STR_Write2Byte(CLKGEN,0x39,vdec_save_reg.vdec_ip_clock[0x39]);
    //HVD Clock Restore
    MDrv_STR_Write2Byte(CLKGEN,0x31,vdec_save_reg.vdec_ip_clock[0x31]);
    MDrv_STR_Write2Byte(CLKGEN,0x3F,vdec_save_reg.vdec_ip_clock[0x3F]);
    //EVD Clock Restore
    MDrv_STR_Write2Byte(CLKGEN,0x33,vdec_save_reg.vdec_ip_clock[0x33]);
    MDrv_STR_Write2Byte(CLKGEN,0x34,vdec_save_reg.vdec_ip_clock[0x34]);

    //add for MVD clk setting
    MDrv_STR_Write2Byte(CLKGEN,0x3A,vdec_save_reg.vdec_ip_clock[0x3A]);
    MDrv_STR_Write2Byte(CLKGEN,0x3B,vdec_save_reg.vdec_ip_clock[0x3B]);
    MDrv_STR_Write2Byte(CLKGEN,0x3C,vdec_save_reg.vdec_ip_clock[0x3C]);
    MDrv_STR_Write2Byte(CLKGEN,0x3E,vdec_save_reg.vdec_ip_clock[0x3E]);

}

static void vdec_r2_poweron(void)
{
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x40,0x0000);
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x42,vdec_save_reg.vdec_r2_bank[0x42]);
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x44,vdec_save_reg.vdec_r2_bank[0x44]);
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x48,0xC000); //solution
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x4F,0xC000);

      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x50,0xffff);

      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x56,0x8000);

      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x4D,0x0000);

      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x4E,0x2000);

      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x58,0x0017);
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x60,vdec_save_reg.vdec_r2_bank[0x60]);
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x40,0x0004);//solution
      MDrv_STR_Write2Byte(R2_BANK_ADDR,0x40,0x0007);//solution

}

static void vdec_store_reg(void)
{
    #ifdef SUPPORT_VDEC_STR
    vdec_save_reg.vdec_is_work[0] = HAL_VPU_EX_CheckSTRState(0);
    vdec_save_reg.vdec_is_work[1] = HAL_VPU_EX_CheckSTRState(1);
    #else
    vdec_save_reg.vdec_is_work[0] = FALSE;
    vdec_save_reg.vdec_is_work[1] = FALSE;
    #endif
    if(vdec_save_reg.vdec_is_work[0] || vdec_save_reg.vdec_is_work[1])
    {
        if(MDrv_STR_Read2Byte(CLKGEN,0x30)&0x1)
        {
            vdec_save_reg.vdec_is_work[0] = FALSE;
            vdec_save_reg.vdec_is_work[1] = FALSE;
        }
    }
    if(vdec_save_reg.vdec_is_work[0] || vdec_save_reg.vdec_is_work[1])
    {
        STR_PRINT("VDEC suspend to store register\n");
        int start_index = 0;
        for(start_index = 0x00;start_index<BANK_SIZE;start_index++)
        {
            vdec_save_reg.vdec_mvop_bank[start_index] = MDrv_STR_Read2Byte(MVOP_BANK_ADDR,start_index);
            vdec_save_reg.vdec_submvop_bank[start_index] = MDrv_STR_Read2Byte(SUBMVOP_BANK_ADDR,start_index);
            vdec_save_reg.vdec_ip_clock[start_index] = MDrv_STR_Read2Byte(CLKGEN,start_index);
            vdec_save_reg.vdec_r2_bank[start_index] = MDrv_STR_Read2Byte(R2_BANK_ADDR,start_index);
            vdec_save_reg.vdec_hvd_bank[start_index] = MDrv_STR_Read2Byte(HVD_BANK_ADDR,start_index);
            //vdec_save_reg.vdec_mvd_bank[start_index] = MDrv_STR_Read2Byte(HVD_BANK_ADDR,start_index);
            vdec_save_reg.vdec_evd_bank[start_index] = MDrv_STR_Read2Byte(EVD_BANK_ADDR,start_index);
            vdec_save_reg.vdec_mau_bank[start_index] = MDrv_STR_Read2Byte(MAU_BANK_ADDR,start_index);
        }
    }
    #ifdef SUPPORT_VDEC_STR
    HAL_VPU_EX_EnableSTRResume(0,vdec_save_reg.vdec_is_work[0]);
    HAL_VPU_EX_EnableSTRResume(1,vdec_save_reg.vdec_is_work[1]);
    #endif
}
static void vdec_load_reg(void)
{
    if(vdec_save_reg.vdec_is_work[0] || vdec_save_reg.vdec_is_work[1])
    {
        #ifdef SUPPORT_VDEC_STR
        STR_PRINT("VDEC resume to store register\n");
        HAL_VPU_EX_ResumeFW();
        if(vdec_save_reg.vdec_is_work[0])
        {
            HAL_VPU_EX_EnableSTRResume(0,TRUE);
        }
        if(vdec_save_reg.vdec_is_work[1])
        {
            HAL_VPU_EX_EnableSTRResume(1,TRUE);
        }
        vdec_ip_clktrigger();
        vdec_r2_poweron();
        #endif
    }
}

extern int suspend_vdec(void)
{
    STR_PRINT("enter\n");
    vdec_store_reg();
    STR_PRINT("exit\n");
    return 0;
}

extern int resume_vdec(void)
{
    STR_PRINT("enter\n");
    vdec_load_reg();
    STR_PRINT("exit\n");
    return 0;
}


extern int init_vdec(void)
{
    int ret = 0;
    STR_PRINT("enter\n");
    STR_PRINT("exit\n");
    return ret;
}

extern void exit_vdec(void)
{
    STR_PRINT("enter\n");
    STR_PRINT("exit\n");
}

