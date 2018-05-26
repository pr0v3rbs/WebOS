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

/** @file pe_ddr_reg_h15.h
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author	  jaemo.kim (jaemo.kim@lge.com)
 * version	  1.0
 * date		  2011.02.17
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef _PE_DDR_REG_H15_H_
#define _PE_DDR_REG_H15_H_
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/
/**
* version history
* 0x1 : 20140715, 1st edition
*/
#define PE_DDR_DB_VERSION					(0x2)

#define PE_DDR_H15_SEC_MAX_SIZE             50

/* section size */
#define PE_DDR_H15_MODE_SIZE                1
#define PE_DDR_H15_DRC_LUT_SIZE             1185
#define PE_DDR_H15_SCL_M_FLT_SIZE           896
#define PE_DDR_H15_SCL_S_FLT_SIZE           640
#define PE_DDR_H15_TNR_LUT_SIZE             43
/* cnt:1 + ui:1 + lut_lm:16 + lut_ui:16 + lut_hm:16 */
#define PE_DDR_H15_TNR_USER_SIZE            50
#define PE_DDR_H15_CVD_DFLT_SIZE            145

/* item number */
#define PE_DDR_H15_MODE_ITEM_NUM            1
#define PE_DDR_H15_DRC_LUT_ITEM_NUM         1
#define PE_DDR_H15_SCL_06T_M_ITEM_NUM       2
#define PE_DDR_H15_SCL_12T_M_ITEM_NUM       11
#define PE_DDR_H15_SCL_12T_S_ITEM_NUM       2
#define PE_DDR_H15_TNR_LUT_ITEM_NUM         4
#define PE_DDR_H15_TNR_USER_ITEM_NUM        1
#define PE_DDR_H15_CVD_DFLT_ITEM_NUM        12

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PE_DDR_REG_H15_SEC_TYPE_TO_STR(_x)		(\
	((_x)==PE_DDR_H15_SEC_MODE     )? "MODE     ":\
	((_x)==PE_DDR_H15_SEC_DRC_LUT  )? "DRC_LUT  ":\
	((_x)==PE_DDR_H15_SEC_SCL_06T_M)? "SCL_06T_M":\
	((_x)==PE_DDR_H15_SEC_SCL_12T_M)? "SCL_12T_M":\
	((_x)==PE_DDR_H15_SEC_SCL_12T_S)? "SCL_12T_S":\
	((_x)==PE_DDR_H15_SEC_TNR_LUT  )? "TNR_LUT  ":\
	((_x)==PE_DDR_H15_SEC_TNR_USER )? "TNR_USER ":\
	((_x)==PE_DDR_H15_SEC_CVD_DFLT )? "CVD_DFLT ":\
	"SEC_NUM  ")

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
/**
* section type enum
* - causion : should be PE_DDR_H15_SEC_NUM < PE_DDR_H15_SEC_MAX_SIZE
*/
typedef enum
{
	PE_DDR_H15_SEC_MODE          =  0,  ///< mode
	PE_DDR_H15_SEC_DRC_LUT       =  1,  ///< drc lut
	PE_DDR_H15_SEC_SCL_06T_M     =  2,  ///< msc 06tap main
	PE_DDR_H15_SEC_SCL_12T_M     =  3,  ///< msc 12tap main
	PE_DDR_H15_SEC_SCL_12T_S     =  4,  ///< msc 12tap sub
	PE_DDR_H15_SEC_TNR_LUT       =  5,  ///< tnr lut
	PE_DDR_H15_SEC_TNR_USER      =  6,  ///< tnr user
	PE_DDR_H15_SEC_CVD_DFLT      =  7,  ///< cvd default
	PE_DDR_H15_SEC_NUM                  ///< max num
}
PE_DDR_H15_SEC_TYPE;

/**
* top header type
* - sec_num : PE_DDR_H15_SEC_NUM
* - sec_base[N+1] = sec_base[N] + [N]header + ([N]item_num * [N]item_size)
*   ([N]header = sizeof(PE_DDR_H15_HEADER_T))
*/
typedef struct
{
	UINT32 version;      ///< version
	UINT32 db_base;      ///< db base addr
	UINT32 sec_num;      ///< section max number
	UINT32 sec_base[PE_DDR_H15_SEC_MAX_SIZE];   ///< each section start base
	UINT32 sec_size[PE_DDR_H15_SEC_MAX_SIZE];   ///< each section start size
}
PE_DDR_H15_TOP_HEADER_T;

/**
* header type
* - sec_type : see PE_DDR_H15_SEC_TYPE 
*/
typedef struct
{
	UINT32 version;    ///< version
	UINT32 sec_type;   ///< section type
	UINT32 item_num;   ///< item number
	UINT32 item_size;  ///< item size
}
PE_DDR_H15_HEADER_T;

/**
 *	mode0
 */
typedef struct
{
	UINT32 shp_fw_en        : 1;   ///< 0, shp fw enable, just ready
	UINT32 hst_fw_en        : 1;   ///< 1, hst fw enable, just ready
	UINT32 scl_fw_en        : 1;   ///< 2, scl fw enable
	UINT32 tnr_fw_en        : 1;   ///< 3, tnr fw enable
	UINT32 dnr_fw_en        : 1;   ///< 4, dnr fw enable
}
PE_DDR_H15_MD0_T;

/**
* mode
* - header.item_num = PE_DDR_H15_MODE_ITEM_NUM
* - header.item_size = PE_DDR_H15_MODE_SIZE
* - data, see PE_DDR_H15_MD0_T
*
* version history :
* 0x1 : 20140827, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_MODE_ITEM_NUM*PE_DDR_H15_MODE_SIZE]; ///< data
}
PE_DDR_H15_MODE_T;

/**
* drc lut type
* - header.item_num = PE_DDR_H15_DRC_LUT_ITEM_NUM
* - header.item_size = PE_DDR_H15_DRC_LUT_SIZE
*
* version history :
* 0x1 : 20140715, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_DRC_LUT_ITEM_NUM*PE_DDR_H15_DRC_LUT_SIZE]; ///< data
}
PE_DDR_H15_DRC_LUT_T;

/**
* scaler v06tap main filter
* - header.item_num = PE_DDR_H15_SCL_06T_M_ITEM_NUM
* - header.item_size = PE_DDR_H15_SCL_M_FLT_SIZE
*
* version history :
* 0x1 : 20140715, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_SCL_06T_M_ITEM_NUM*PE_DDR_H15_SCL_M_FLT_SIZE]; ///< data
}
PE_DDR_H15_SCL_06T_M_T;

/**
* scaler h12tap main filter
* - header.item_num = PE_DDR_H15_SCL_12T_M_ITEM_NUM
* - header.item_size = PE_DDR_H15_SCL_M_FLT_SIZE
*
* version history :
* 0x1 : 20140715, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_SCL_12T_M_ITEM_NUM*PE_DDR_H15_SCL_M_FLT_SIZE]; ///< data
}
PE_DDR_H15_SCL_12T_M_T;

/**
* scaler h12tap sub filter
* - header.item_num = PE_DDR_H15_SCL_12T_S_ITEM_NUM
* - header.item_size = PE_DDR_H15_SCL_S_FLT_SIZE
*
* version history :
* 0x1 : 20140715, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_SCL_12T_S_ITEM_NUM*PE_DDR_H15_SCL_S_FLT_SIZE]; ///< data
}
PE_DDR_H15_SCL_12T_S_T;

/**
* tnr lut type
* - header.item_num = PE_DDR_H15_TNR_LUT_ITEM_NUM
* - header.item_size = PE_DDR_H15_TNR_LUT_SIZE
*
* version history :
* 0x1 : 20140807, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_TNR_LUT_ITEM_NUM*PE_DDR_H15_TNR_LUT_SIZE]; ///< data
}
PE_DDR_H15_TNR_LUT_T;

/**
* tnr user type
* - header.item_num = PE_DDR_H15_TNR_USER_ITEM_NUM
* - header.item_size = PE_DDR_H15_TNR_USER_SIZE
*
* version history :
* 0x1 : 20140807, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_TNR_USER_ITEM_NUM*PE_DDR_H15_TNR_USER_SIZE]; ///< data
}
PE_DDR_H15_TNR_USER_T;

/**
* cvd default type
* - header.item_num = PE_DDR_H15_CVD_DFLT_ITEM_NUM
* - header.item_size = PE_DDR_H15_CVD_DFLT_SIZE
*
* version history :
* 0x1 : 20140807, 1st edition
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data[PE_DDR_H15_CVD_DFLT_ITEM_NUM*PE_DDR_H15_CVD_DFLT_SIZE]; ///< data
}
PE_DDR_H15_CVD_DFLT_T;

/**
* ddr data type
*/
typedef struct
{
	PE_DDR_H15_TOP_HEADER_T top_h;     ///< top header
	PE_DDR_H15_MODE_T mode;            ///< num:PE_DDR_H15_MODE_ITEM_NUM       , size:PE_DDR_H15_MODE_SIZE
	PE_DDR_H15_DRC_LUT_T drc_lut;      ///< num:PE_DDR_H15_DRC_LUT_ITEM_NUM    , size:PE_DDR_H15_DRC_LUT_SIZE
	PE_DDR_H15_SCL_06T_M_T t06_m;      ///< num:PE_DDR_H15_SCL_06T_M_ITEM_NUM  , size:PE_DDR_H15_SCL_M_FLT_SIZE
	PE_DDR_H15_SCL_12T_M_T t12_m;      ///< num:PE_DDR_H15_SCL_12T_M_ITEM_NUM  , size:PE_DDR_H15_SCL_M_FLT_SIZE 
	PE_DDR_H15_SCL_12T_S_T t12_s;      ///< num:PE_DDR_H15_SCL_12T_S_ITEM_NUM  , size:PE_DDR_H15_SCL_S_FLT_SIZE
	PE_DDR_H15_TNR_LUT_T tnr_lut;      ///< num:PE_DDR_H15_TNR_LUT_ITEM_NUM    , size:PE_DDR_H15_TNR_LUT_SIZE
	PE_DDR_H15_TNR_USER_T tnr_user;    ///< num:PE_DDR_H15_TNR_USER_ITEM_NUM   , size:PE_DDR_H15_TNR_USER_SIZE
	PE_DDR_H15_CVD_DFLT_T cvd_dflt;    ///< num:PE_DDR_H15_CVD_DFLT_ITEM_NUM   , size:PE_DDR_H15_CVD_DFLT_SIZE
}
PE_DDR_DB_PHYS_REG_H15_T;

/**
* data type
*/
typedef struct
{
	PE_DDR_H15_HEADER_T header; ///< header
	UINT32 data_base;           ///< data base
}
PE_DDR_H15_DATA_T;

/**
* ddr data type, should sync with PE_DDR_DB_PHYS_REG_H15_T
* - header.sec_type = PE_DDR_H15_SEC_HEADER
* - header.item_num = PE_DDR_H15_SEC_MAX_NUM
* - header.item_size = NA
*/
typedef struct
{
	PE_DDR_H15_TOP_HEADER_T top_h;                    ///< top header
	PE_DDR_H15_DATA_T sec_d[PE_DDR_H15_SEC_MAX_SIZE];  ///< section data
}
PE_DDR_DB_SHDW_REG_H15_T;

typedef struct
{
	union {
		UINT32                   *addr;
		PE_DDR_DB_SHDW_REG_H15_T *data;
	} shdw;

	union {
		volatile UINT32                   *addr;
		volatile PE_DDR_DB_PHYS_REG_H15_T *data;
	} phys;
}
PE_DDR_DB_REG_TYPE_H15_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* _PE_DDR_REG_H15_H_ */

