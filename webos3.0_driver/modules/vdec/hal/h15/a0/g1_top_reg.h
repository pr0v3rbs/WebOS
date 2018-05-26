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
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */
#ifndef _HEVC_G1_REG_H_
#define _HEVC_G1_REG_H_


#ifdef __cplusplus
extern "C" {
#endif

    /************   Includes  *******************/

    //0x000
    typedef struct{
        UINT32 
        reg_dec_mode    : 1;  // [ 0: 0]
    }DEC_MODE;

    //0x010
    typedef struct{
        UINT32 
        g1_fuse_dec;      // [31:0]
    }AD_G1_FUSE_DEC;

    //0x014
    typedef struct{
        UINT32 
        g1_fuse_pp;       // [31:0]
    }AD_G1_FUSE_PP;

    //0x018
    typedef struct{
        UINT32 
        g1_dec_core_amount   :3, // [ 2: 0]
        reserved             :1, // [ 3: 3]
        g1_dec_core_number   :3; // [ 6: 4]
    }AD_G1_DEC_CORE;

    //0x030
    typedef struct{
        UINT32  
        notused              :22,// [21: 0]
        g1_intr              :1; // 22
    }MCU_INTR_EN;

    //0x034
    typedef struct{
        UINT32  
        notused              :22,// [21: 0]
        g1_intr              :1; // 22
    }MCU_INTR_ST;

    //0x038
    typedef struct{
        UINT32  
        notused              :22,// [21: 0]
        g1_intr              :1; // 22
    }MCU_INTR_CL;

    typedef struct {
        DEC_MODE        dec_mode;       // 0x00      DEC_MODE           RW
        UINT32          __rsvd_00[3];   // 0x04~0x0C EMPTY              RO
        AD_G1_FUSE_DEC  ad_g1_fuse_dec; // 0x10      AD_G1_FUSE_DEC     RW
        AD_G1_FUSE_PP   ad_g1_fuse_pp;  // 0x14      AD_G1_FUSE_PP      RW
        AD_G1_DEC_CORE  ad_g1_dec_core; // 0x18      AD_G1_DEC_CORE     RW
        UINT32          __rsvd_01[5];   // 0x1C~0x2C EMPTY              RO
        MCU_INTR_EN     intr_mcu_en;    // 0x30      INTR_MCU_EN        RW
        MCU_INTR_ST     intr_mcu_st;    // 0x34      INTR_MCU_ST        RW
        MCU_INTR_CL     intr_mcu_cl;    // 0x38      INTR_MCU_CL        RW
    } G1_TOP_REG_T;

#ifdef __cplusplus
}
#endif
#endif
