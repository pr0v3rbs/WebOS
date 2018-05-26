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

/** @file pe_reg_mac.h
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
#ifndef  _PE_REG_MAC_H_
#define  _PE_REG_MAC_H_
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PE_REG_PHYS_COPY(_dst, _src, _r)		((_dst->_r)=(_src->_r))
#define PE_REG_SHDW_READ(_shdw, _r)				(*((UINT32*)(&(_shdw->_r))))

#define PE_REG_PHYS_RdFL(_reg, _data, _r)		PE_REG_PHYS_COPY(_reg.shdw._data, _reg.phys._data, _r)
#define PE_REG_PHYS_WrFL(_reg, _data, _r)		PE_REG_PHYS_COPY(_reg.phys._data, _reg.shdw._data, _r)
#define PE_REG_SHDW_Rd32(_reg, _data, _r)		PE_REG_SHDW_READ(_reg.shdw._data, _r)
#define PE_REG_SHDW_Wr32(_reg, _data, _r, _v)	PE_REG_SHDW_READ(_reg.shdw._data, _r) = ((UINT32)(_v))

#define PE_REG_SHDW_Rd01(_reg, _data, _r, _f01, _v01)											\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
}while(0)

#define PE_REG_SHDW_Rd02(_reg, _data, _r, _f01, _v01, _f02, _v02)								\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
	(_v02) = (_reg.shdw._data->_r._f02);														\
}while(0)

#define PE_REG_SHDW_Rd03(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03)					\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
	(_v02) = (_reg.shdw._data->_r._f02);														\
	(_v03) = (_reg.shdw._data->_r._f03);														\
}while(0)

#define PE_REG_SHDW_Rd04(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)		\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
	(_v02) = (_reg.shdw._data->_r._f02);														\
	(_v03) = (_reg.shdw._data->_r._f03);														\
	(_v04) = (_reg.shdw._data->_r._f04);														\
}while(0)

#define PE_REG_SHDW_Rd05(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05)																				\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
	(_v02) = (_reg.shdw._data->_r._f02);														\
	(_v03) = (_reg.shdw._data->_r._f03);														\
	(_v04) = (_reg.shdw._data->_r._f04);														\
	(_v05) = (_reg.shdw._data->_r._f05);														\
}while(0)

#define PE_REG_SHDW_Rd06(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06)																	\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
	(_v02) = (_reg.shdw._data->_r._f02);														\
	(_v03) = (_reg.shdw._data->_r._f03);														\
	(_v04) = (_reg.shdw._data->_r._f04);														\
	(_v05) = (_reg.shdw._data->_r._f05);														\
	(_v06) = (_reg.shdw._data->_r._f06);														\
}while(0)

#define PE_REG_SHDW_Rd07(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06, _f07, _v07)														\
do{																								\
	(_v01) = (_reg.shdw._data->_r._f01);														\
	(_v02) = (_reg.shdw._data->_r._f02);														\
	(_v03) = (_reg.shdw._data->_r._f03);														\
	(_v04) = (_reg.shdw._data->_r._f04);														\
	(_v05) = (_reg.shdw._data->_r._f05);														\
	(_v06) = (_reg.shdw._data->_r._f06);														\
	(_v07) = (_reg.shdw._data->_r._f07);														\
}while(0)

#define PE_REG_SHDW_Wr01(_reg, _data, _r, _f01, _v01)											\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
}while(0)

#define PE_REG_SHDW_Wr02(_reg, _data, _r, _f01, _v01, _f02, _v02)								\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
	(_reg.shdw._data->_r._f02) = (_v02);														\
}while(0)

#define PE_REG_SHDW_Wr03(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03)					\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
	(_reg.shdw._data->_r._f02) = (_v02);														\
	(_reg.shdw._data->_r._f03) = (_v03);														\
}while(0)

#define PE_REG_SHDW_Wr04(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)		\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
	(_reg.shdw._data->_r._f02) = (_v02);														\
	(_reg.shdw._data->_r._f03) = (_v03);														\
	(_reg.shdw._data->_r._f04) = (_v04);														\
}while(0)

#define PE_REG_SHDW_Wr05(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05)																				\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
	(_reg.shdw._data->_r._f02) = (_v02);														\
	(_reg.shdw._data->_r._f03) = (_v03);														\
	(_reg.shdw._data->_r._f04) = (_v04);														\
	(_reg.shdw._data->_r._f05) = (_v05);														\
}while(0)

#define PE_REG_SHDW_Wr06(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06)																	\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
	(_reg.shdw._data->_r._f02) = (_v02);														\
	(_reg.shdw._data->_r._f03) = (_v03);														\
	(_reg.shdw._data->_r._f04) = (_v04);														\
	(_reg.shdw._data->_r._f05) = (_v05);														\
	(_reg.shdw._data->_r._f06) = (_v06);														\
}while(0)

#define PE_REG_SHDW_Wr07(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06, _f07, _v07)														\
do{																								\
	(_reg.shdw._data->_r._f01) = (_v01);														\
	(_reg.shdw._data->_r._f02) = (_v02);														\
	(_reg.shdw._data->_r._f03) = (_v03);														\
	(_reg.shdw._data->_r._f04) = (_v04);														\
	(_reg.shdw._data->_r._f05) = (_v05);														\
	(_reg.shdw._data->_r._f06) = (_v06);														\
	(_reg.shdw._data->_r._f07) = (_v07);														\
}while(0)

#if 0
#define PE_REG_SHDW_Rd01(_reg, _data, _r, _f01, _v01)												\
	PE_REG_SHDW_DATA_Rd01(_reg.shdw._data->_r, _f01, _v01)

#define PE_REG_SHDW_Rd02(_reg, _data, _r, _f01, _v01, _f02, _v02)									\
	PE_REG_SHDW_DATA_Rd02(_reg.shdw._data->_r, _f01, _v01, _f02, _v02)

#define PE_REG_SHDW_Rd03(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03)						\
	PE_REG_SHDW_DATA_Rd03(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03)

#define PE_REG_SHDW_Rd04(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)			\
	PE_REG_SHDW_DATA_Rd04(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)

#define PE_REG_SHDW_Rd05(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,			\
		_f05, _v05)																					\
	PE_REG_SHDW_DATA_Rd05(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05)

#define PE_REG_SHDW_Rd06(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,			\
		_f05, _v05, _f06, _v06)																		\
	PE_REG_SHDW_DATA_Rd06(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06)

#define PE_REG_SHDW_Rd07(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,			\
		_f05, _v05, _f06, _v06, _f07, _v07)															\
	PE_REG_SHDW_DATA_Rd07(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06, _f07, _v07)

#define PE_REG_SHDW_Wr01(_reg, _data, _r, _f01, _v01)												\
	PE_REG_SHDW_DATA_Wr01(_reg.shdw._data->_r, _f01, _v01)

#define PE_REG_SHDW_Wr02(_reg, _data, _r, _f01, _v01, _f02, _v02)									\
	PE_REG_SHDW_DATA_Wr02(_reg.shdw._data->_r, _f01, _v01, _f02, _v02)

#define PE_REG_SHDW_Wr03(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03)						\
	PE_REG_SHDW_DATA_Wr03(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03)

#define PE_REG_SHDW_Wr04(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)			\
	PE_REG_SHDW_DATA_Wr04(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04)

#define PE_REG_SHDW_Wr05(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,			\
		_f05, _v05)																					\
	PE_REG_SHDW_DATA_Wr05(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05)

#define PE_REG_SHDW_Wr06(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,			\
		_f05, _v05, _f06, _v06)																		\
	PE_REG_SHDW_DATA_Wr06(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06)

#define PE_REG_SHDW_Wr07(_reg, _data, _r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,			\
		_f05, _v05, _f06, _v06, _f07, _v07)															\
	PE_REG_SHDW_DATA_Wr07(_reg.shdw._data->_r, _f01, _v01, _f02, _v02, _f03, _v03, _f04, _v04,		\
		_f05, _v05, _f06, _v06, _f07, _v07)
#endif
/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* _PE_REG_MAC_H_ */

