/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author	  jaemo.kim (jaemo.kim@lge.com)
 * version	  1.0
 * date		  2011.02.16
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  DE_REG_MAC_INC
#define  DE_REG_MAC_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_ver_def.h"

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define REG_PHYS_COPY(_dst,  _src,	  _r)						((_dst->_r)=(_src->_r))
#define REG_SHDW_READ(_shdw,		  _r)						(*((UINT32*)(&(_shdw->_r))))
#define REG_SHDW_WfCM(_shdw,		  _r, _f, _c, _m)			((_shdw->_r._f)=(_c)?((_m)|_shdw->_r._f):(~(_m)&_shdw->_r._f))
#define REG_SHDW_WfCV(_shdw,		  _r, _f, _c, _v1, _v2)		((_shdw->_r._f)=(_c)?(_v1):(_v2))

#define REG_PHYS_VER_RdFL(_reg, _ver, _r)						REG_PHYS_COPY(_reg.shdw._ver, _reg.phys._ver, _r)
#define REG_PHYS_VER_WrFL(_reg, _ver, _r)						REG_PHYS_COPY(_reg.phys._ver, _reg.shdw._ver, _r)
#define REG_SHDW_VER_Rd32(_reg, _ver, _r)						REG_SHDW_READ(_reg.shdw._ver, _r)
#define REG_SHDW_VER_Wr32(_reg, _ver, _r, _v)					REG_SHDW_READ(_reg.shdw._ver, _r) = ((UINT32)(_v))
#define REG_SHDW_VER_WfCM(_reg, _ver, _r, _f, _c, _m)			REG_SHDW_WfCM(_reg.shdw._ver, _r, _f, _c, _m)
#define REG_SHDW_VER_WfCV(_reg, _ver, _r, _f, _c, _v1, _v2)		REG_SHDW_WfCV(_reg.shdw._ver, _r, _f, _c, _v1, _v2)

#define REG_SHDW_Rd01(_r, _f01, _v01)																								\
do {																																\
	(_v01) = (_r._f01);																												\
} while (0)

#define REG_SHDW_Rd02(_r, _f01, _v01, _f02, _v02)																					\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
} while (0)

#define REG_SHDW_Rd03(_r, _f01, _v01, _f02, _v02, _f03, _v03)																		\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
} while (0)

#define REG_SHDW_Rd04(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)															\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
} while (0)

#define REG_SHDW_Rd05(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05)																													\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
} while (0)

#define REG_SHDW_Rd06(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06)																										\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
} while (0)

#define REG_SHDW_Rd07(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07)																							\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
} while (0)

#define REG_SHDW_Rd08(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)																				\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
} while (0)

#define REG_SHDW_Rd09(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)																	\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
} while (0)

#define REG_SHDW_Rd10(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)														\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
} while (0)

#define REG_SHDW_Rd11(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)																													\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
	(_v11) = (_r._f11);																												\
} while (0)

#define REG_SHDW_Rd12(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)																										\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
	(_v11) = (_r._f11);																												\
	(_v12) = (_r._f12);																												\
} while (0)

#define REG_SHDW_Rd13(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)																							\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
	(_v11) = (_r._f11);																												\
	(_v12) = (_r._f12);																												\
	(_v13) = (_r._f13);																												\
} while (0)

#define REG_SHDW_Rd14(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)																				\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
	(_v11) = (_r._f11);																												\
	(_v12) = (_r._f12);																												\
	(_v13) = (_r._f13);																												\
	(_v14) = (_r._f14);																												\
} while (0)

#define REG_SHDW_Rd15(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)																	\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
	(_v11) = (_r._f11);																												\
	(_v12) = (_r._f12);																												\
	(_v13) = (_r._f13);																												\
	(_v14) = (_r._f14);																												\
	(_v15) = (_r._f15);																												\
} while (0)

#define REG_SHDW_Rd16(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)														\
do {																																\
	(_v01) = (_r._f01);																												\
	(_v02) = (_r._f02);																												\
	(_v03) = (_r._f03);																												\
	(_v04) = (_r._f04);																												\
	(_v05) = (_r._f05);																												\
	(_v06) = (_r._f06);																												\
	(_v07) = (_r._f07);																												\
	(_v08) = (_r._f08);																												\
	(_v09) = (_r._f09);																												\
	(_v10) = (_r._f10);																												\
	(_v11) = (_r._f11);																												\
	(_v12) = (_r._f12);																												\
	(_v13) = (_r._f13);																												\
	(_v14) = (_r._f14);																												\
	(_v15) = (_r._f15);																												\
	(_v16) = (_r._f16);																												\
} while (0)

#define REG_SHDW_Wr01(_r, _f01, _v01)																								\
do {																																\
	(_r._f01) = (_v01);																												\
} while (0)

#define REG_SHDW_Wr02(_r, _f01, _v01, _f02, _v02)																					\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
} while (0)

#define REG_SHDW_Wr03(_r, _f01, _v01, _f02, _v02, _f03, _v03)																		\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
} while (0)

#define REG_SHDW_Wr04(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)															\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
} while (0)

#define REG_SHDW_Wr05(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05)																													\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
} while (0)

#define REG_SHDW_Wr06(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06)																										\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
} while (0)

#define REG_SHDW_Wr07(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07)																							\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
} while (0)

#define REG_SHDW_Wr08(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)																				\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
} while (0)

#define REG_SHDW_Wr09(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)																	\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
} while (0)

#define REG_SHDW_Wr10(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)														\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
} while (0)

#define REG_SHDW_Wr11(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)																													\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
	(_r._f11) = (_v11);																												\
} while (0)

#define REG_SHDW_Wr12(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)																										\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
	(_r._f11) = (_v11);																												\
	(_r._f12) = (_v12);																												\
} while (0)

#define REG_SHDW_Wr13(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)																							\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
	(_r._f11) = (_v11);																												\
	(_r._f12) = (_v12);																												\
	(_r._f13) = (_v13);																												\
} while (0)

#define REG_SHDW_Wr14(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)																				\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
	(_r._f11) = (_v11);																												\
	(_r._f12) = (_v12);																												\
	(_r._f13) = (_v13);																												\
	(_r._f14) = (_v14);																												\
} while (0)

#define REG_SHDW_Wr15(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)																	\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
	(_r._f11) = (_v11);																												\
	(_r._f12) = (_v12);																												\
	(_r._f13) = (_v13);																												\
	(_r._f14) = (_v14);																												\
	(_r._f15) = (_v15);																												\
} while (0)

#define REG_SHDW_Wr16(_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,															\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)														\
do {																																\
	(_r._f01) = (_v01);																												\
	(_r._f02) = (_v02);																												\
	(_r._f03) = (_v03);																												\
	(_r._f04) = (_v04);																												\
	(_r._f05) = (_v05);																												\
	(_r._f06) = (_v06);																												\
	(_r._f07) = (_v07);																												\
	(_r._f08) = (_v08);																												\
	(_r._f09) = (_v09);																												\
	(_r._f10) = (_v10);																												\
	(_r._f11) = (_v11);																												\
	(_r._f12) = (_v12);																												\
	(_r._f13) = (_v13);																												\
	(_r._f14) = (_v14);																												\
	(_r._f15) = (_v15);																												\
	(_r._f16) = (_v16);																												\
} while (0)

#define FN_CHIP_RdFL(_fn, _chip, _r)																								\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_RdFL(g##_fn##_##_chip, a0, _r);																			\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_WrFL(_fn, _chip, _r)																								\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_WrFL(g##_fn##_##_chip, a0, _r);																			\
			break;																													\
	}																																\
} while (0)

#if 0
#define FN_CHIP_Rd32(_fn, _chip, _r)																								\
	(LX_CHIP_REV(_chip, A1) == lx_chip_rev())?REG_SHDW_VER_Rd32(g##_fn##_##_chip, a0, _r):											\
	(LX_CHIP_REV(_chip, A0) == lx_chip_rev())?REG_SHDW_VER_Rd32(g##_fn##_##_chip, a0, _r):											\
	REG_SHDW_VER_Rd32(g##_fn##_##_chip, b0, _r)
#else
#define FN_CHIP_Rd32(_fn, _chip, _r)																								\
	(LX_CHIP_REV(_chip, A1) == lx_chip_rev())?REG_SHDW_VER_Rd32(g##_fn##_##_chip, a0, _r):											\
	(LX_CHIP_REV(_chip, A0) == lx_chip_rev())?REG_SHDW_VER_Rd32(g##_fn##_##_chip, a0, _r):											\
	REG_SHDW_VER_Rd32(g##_fn##_##_chip, a0, _r)
#endif

#define FN_CHIP_Wr32(_fn, _chip, _r, _v)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr32(g##_fn##_##_chip, a0, _r, _v);																		\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_WfCM(_fn, _chip, _r, _f, _c, _m)																					\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_WfCM(g##_fn##_##_chip, a0, _r, _f, _c, _m);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_WfCV(_fn, _chip, _r, _f, _c, _v1, _v2)																				\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_WfCV(g##_fn##_##_chip, a0, _r, _f, _c, _v1, _v2);															\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd01(_fn, _chip, _r, _f01, _v01)																					\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd01(g##_fn##_##_chip, a0, _r, _f01, _v01);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd02(_fn, _chip, _r, _f01, _v01, _f02, _v02)																		\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd02(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02);													\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd03(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03)															\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd03(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03);										\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd04(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)												\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd04(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04);							\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd05(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05)																													\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd05(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05);																									\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd06(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06)																										\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd06(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06);																						\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd07(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd07(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07);																			\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd08(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)																				\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd08(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd09(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)																	\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd09(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09);													\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd10(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)														\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd10(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10);										\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd11(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)																													\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd11(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11);																									\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd12(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)																										\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd12(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12);																						\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd13(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd13(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13);																			\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd14(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)																				\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd14(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd15(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)																	\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd15(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15);													\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Rd16(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)														\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Rd16(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16);										\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr01(_fn, _chip, _r, _f01, _v01)																					\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr01(g##_fn##_##_chip, a0, _r, _f01, _v01);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr02(_fn, _chip, _r, _f01, _v01, _f02, _v02)																		\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr02(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02);													\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr03(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03)															\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr03(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03);										\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr04(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)												\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr04(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04);							\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr05(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05)																													\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr05(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05);																									\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr06(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06)																										\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr06(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06);																						\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr07(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr07(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07);																			\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr08(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)																				\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr08(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr09(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)																	\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr09(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09);													\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr10(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)														\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr10(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10);										\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr11(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)																													\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr11(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11);																									\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr12(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)																										\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr12(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12);																						\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr13(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr13(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13);																			\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr14(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)																				\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr14(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr15(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)																	\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr15(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15);													\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_Wr16(_fn, _chip, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)														\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_SHDW_VER_Wr16(g##_fn##_##_chip, a0, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,								\
					_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,											\
					_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16);										\
			break;																													\
	}																																\
} while (0)

#define REG_SHDW_VER_Rd01(_reg, _ver, _r, _f01, _v01)																				\
	REG_SHDW_Rd01(_reg.shdw._ver->_r, _f01, _v01)

#define REG_SHDW_VER_Rd02(_reg, _ver, _r, _f01, _v01, _f02, _v02)																	\
	REG_SHDW_Rd02(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02)

#define REG_SHDW_VER_Rd03(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03)														\
	REG_SHDW_Rd03(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03)

#define REG_SHDW_VER_Rd04(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)											\
	REG_SHDW_Rd04(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)

#define REG_SHDW_VER_Rd05(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05)																													\
	REG_SHDW_Rd05(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05)

#define REG_SHDW_VER_Rd06(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06)																										\
	REG_SHDW_Rd06(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06)

#define REG_SHDW_VER_Rd07(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07)																							\
	REG_SHDW_Rd07(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07)

#define REG_SHDW_VER_Rd08(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)																				\
	REG_SHDW_Rd08(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)

#define REG_SHDW_VER_Rd09(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)																	\
	REG_SHDW_Rd09(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)

#define REG_SHDW_VER_Rd10(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)														\
	REG_SHDW_Rd10(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)

#define REG_SHDW_VER_Rd11(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)																													\
	REG_SHDW_Rd11(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)

#define REG_SHDW_VER_Rd12(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)																										\
	REG_SHDW_Rd12(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)

#define REG_SHDW_VER_Rd13(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)																							\
	REG_SHDW_Rd13(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)

#define REG_SHDW_VER_Rd14(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)																				\
	REG_SHDW_Rd14(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)

#define REG_SHDW_VER_Rd15(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)																	\
	REG_SHDW_Rd15(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)

#define REG_SHDW_VER_Rd16(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)														\
	REG_SHDW_Rd16(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)

#define REG_SHDW_VER_Wr01(_reg, _ver, _r, _f01, _v01)																				\
	REG_SHDW_Wr01(_reg.shdw._ver->_r, _f01, _v01)

#define REG_SHDW_VER_Wr02(_reg, _ver, _r, _f01, _v01, _f02, _v02)																	\
	REG_SHDW_Wr02(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02)

#define REG_SHDW_VER_Wr03(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03)														\
	REG_SHDW_Wr03(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03)

#define REG_SHDW_VER_Wr04(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)											\
	REG_SHDW_Wr04(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)

#define REG_SHDW_VER_Wr05(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05)																													\
	REG_SHDW_Wr05(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05)

#define REG_SHDW_VER_Wr06(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06)																										\
	REG_SHDW_Wr06(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06)

#define REG_SHDW_VER_Wr07(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07)																							\
	REG_SHDW_Wr07(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07)

#define REG_SHDW_VER_Wr08(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)																				\
	REG_SHDW_Wr08(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08)

#define REG_SHDW_VER_Wr09(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)																	\
	REG_SHDW_Wr09(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09)

#define REG_SHDW_VER_Wr10(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)														\
	REG_SHDW_Wr10(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10)

#define REG_SHDW_VER_Wr11(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)																													\
	REG_SHDW_Wr11(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11)

#define REG_SHDW_VER_Wr12(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)																										\
	REG_SHDW_Wr12(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12)

#define REG_SHDW_VER_Wr13(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)																							\
	REG_SHDW_Wr13(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13)

#define REG_SHDW_VER_Wr14(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)																				\
	REG_SHDW_Wr14(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14)

#define REG_SHDW_VER_Wr15(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)																	\
	REG_SHDW_Wr15(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15)

#define REG_SHDW_VER_Wr16(_reg, _ver, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,											\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)														\
	REG_SHDW_Wr16(_reg.shdw._ver->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,												\
		_f05, _v05, _f06, _v06, _f07, _v07, _f08, _v08, _f09, _v09, _f10, _v10,														\
		_f11, _v11, _f12, _v12, _f13, _v13, _f14, _v14, _f15, _v15, _f16, _v16)

#define REG_PHYS_FLRd(_phys, _shdw, _r, _v)																							\
do {																																\
	REG_PHYS_COPY(_shdw, _phys, _r);																								\
	(*((UINT32*)(&(_v)))) = REG_SHDW_READ(_shdw, _r);																				\
} while (0)

#define REG_PHYS_FLWr(_phys, _shdw, _r, _v)																							\
do {																																\
	REG_SHDW_READ(_shdw, _r) = ((UINT32)(_v));																						\
	REG_PHYS_COPY(_phys, _shdw, _r);																								\
} while (0)

#define REG_PHYS_FLRf(_dst, _src, _r, _f01, _v01)																					\
do {																																\
	REG_PHYS_COPY(_src, _dst, _r);																									\
	REG_SHDW_Rd01(_src->_r, _f01, _v01);																							\
} while (0)

#define REG_PHYS_FLWf(_dst, _src, _r, _f01, _v01)																					\
do {																																\
	REG_PHYS_COPY(_src, _dst, _r);																									\
	REG_SHDW_Wr01(_src->_r, _f01, _v01);																							\
	REG_PHYS_COPY(_dst, _src, _r);																									\
} while (0)

#define REG_PHYS_FLCM(_dst, _src, _r, _f, _c, _m)																					\
do {																																\
	REG_PHYS_COPY(_src, _dst, _r);																									\
	REG_SHDW_WfCM(_src, _r, _f, _c, _m);																							\
	REG_PHYS_COPY(_dst, _src, _r);																									\
} while (0)

#define REG_PHYS_FLCV(_dst, _src, _r, _f, _c, _v1, v2)																				\
do {																																\
	REG_PHYS_COPY(_src, _dst, _r);																									\
	REG_SHDW_WfCV(_src, _r, _c, _v1, _v2);																							\
	REG_PHYS_COPY(_dst, _src, _r);																									\
} while (0)

#define REG_PHYS_VER_FLRd(_reg, _ver, _r, _v)				 REG_PHYS_FLRd(_reg.phys._ver, _reg.shdw._ver, _r, _v)
#define REG_PHYS_VER_FLWr(_reg, _ver, _r, _v)				 REG_PHYS_FLWr(_reg.phys._ver, _reg.shdw._ver, _r, _v)
#define REG_PHYS_VER_FLRf(_reg, _ver, _r, _f01, _v01)		 REG_PHYS_FLRf(_reg.phys._ver, _reg.shdw._ver, _r, _f01, _v01)
#define REG_PHYS_VER_FLWf(_reg, _ver, _r, _f01, _v01)		 REG_PHYS_FLWf(_reg.phys._ver, _reg.shdw._ver, _r, _f01, _v01)
#define REG_PHYS_VER_FLCM(_reg, _ver, _r, _f, _c, _m)		 REG_PHYS_FLCM(_reg.phys._ver, _reg.shdw._ver, _r, _f, _c, _m)
#define REG_PHYS_VER_FLCV(_reg, _ver, _r, _f, _c, _v1, _v2)  REG_PHYS_FLCV(_reg.phys._ver, _reg.shdw._ver, _r, _f, _c, _v1, _v2)

#define FN_CHIP_FLRd(_fn, _chip, _r, _v)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_FLRd(g##_fn##_##_chip, a0, _r, _v);																		\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_FLWr(_fn, _chip, _r, _v)																							\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_FLWr(g##_fn##_##_chip, a0, _r, _v);																		\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_FLRf(_fn, _chip, _r, _f01, _v01)																					\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_FLRf(g##_fn##_##_chip, a0, _r, _f01, _v01);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_FLWf(_fn, _chip, _r, _f01, _v01)																					\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_FLWf(g##_fn##_##_chip, a0, _r, _f01, _v01);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_FLCM(_fn, _chip, _r, _f, _c, _m)																					\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_FLCM(g##_fn##_##_chip, a0, _r, _f, _c, _m);																\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_FLCV(_fn, _chip, _r, _f, _c, _v1, _v2)																				\
do {																																\
	switch (lx_chip_rev()) {																										\
		default :																													\
		case LX_CHIP_REV(_chip, A1) :																								\
		case LX_CHIP_REV(_chip, A0) :																								\
			REG_PHYS_VER_FLCV(g##_fn##_##_chip, a0, _r, _f, _c, _v1, _v2);															\
			break;																													\
	}																																\
} while (0)

#define FN_CHIP_VER_RdFL(_fn, _chip, _ver, _r)						REG_PHYS_VER_RdFL(g##_fn##_##_chip, _ver, _r)
#define FN_CHIP_VER_WrFL(_fn, _chip, _ver, _r)						REG_PHYS_VER_WrFL(g##_fn##_##_chip, _ver, _r)
#define FN_CHIP_VER_Rd32(_fn, _chip, _ver, _r)						REG_SHDW_VER_Rd32(g##_fn##_##_chip, _ver, _r)
#define FN_CHIP_VER_Wr32(_fn, _chip, _ver, _r, _v)					REG_SHDW_VER_Wr32(g##_fn##_##_chip, _ver, _r, _v)
#define FN_CHIP_VER_Rd01(_fn, _chip, _ver, _r, _f01, _v01)			REG_SHDW_VER_Rd01(g##_fn##_##_chip, _ver, _r, _f01, _v01)
#define FN_CHIP_VER_Wr01(_fn, _chip, _ver, _r, _f01, _v01)			REG_SHDW_VER_Wr01(g##_fn##_##_chip, _ver, _r, _f01, _v01)
#define FN_CHIP_VER_WfCM(_fn, _chip, _ver, _r, _f, _c, _m)			REG_SHDW_VER_WfCM(g##_fn##_##_chip, _ver, _r, _f, _c, _m)
#define FN_CHIP_VER_WfCV(_fn, _chip, _ver, _r, _f, _c, _v1, _v2)	REG_SHDW_VER_WfCV(g##_fn##_##_chip, _ver, _r, _f, _c, _v1, _v2)
#define FN_CHIP_VER_FLRd(_fn, _chip, _ver, _r, _v) 					REG_PHYS_VER_FLRd(g##_fn##_##_chip, _ver, _r, _v)
#define FN_CHIP_VER_FLWr(_fn, _chip, _ver, _r, _v) 					REG_PHYS_VER_FLWr(g##_fn##_##_chip, _ver, _r, _v)
#define FN_CHIP_VER_FLRf(_fn, _chip, _ver, _r, _f01, _v01) 			REG_PHYS_VER_FLRf(g##_fn##_##_chip, _ver, _r, _f01, _v01)
#define FN_CHIP_VER_FLWf(_fn, _chip, _ver, _r, _f01, _v01) 			REG_PHYS_VER_FLWf(g##_fn##_##_chip, _ver, _r, _f01, _v01)
#define FN_CHIP_VER_FLCM(_fn, _chip, _ver, _r, _f, _c, _m) 			REG_PHYS_VER_FLCM(g##_fn##_##_chip, _ver, _r, _f, _c, _m)
#define FN_CHIP_VER_FLCV(_fn, _chip, _ver, _r, _f, _c, _v1, _v2) 	REG_PHYS_VER_FLCV(g##_fn##_##_chip, _ver, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_REG_MAC_INC  ----- */
/**  @} */
