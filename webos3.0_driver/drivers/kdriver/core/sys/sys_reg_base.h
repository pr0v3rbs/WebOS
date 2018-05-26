#ifndef __SYS_REG_BASE__
#define __SYS_REG_BASE__
#include <linux/compiler.h>

#define _REG_Data(_addr)		(*(volatile UINT32 *)(_addr))
#define _REG_Addr(_base,_ofs)	(void __iomem*)((ULONG)_base + _ofs)

#define _REG_OfsRd(_shdw,_phys,_ofs)	\
	(_REG_Data(_REG_Addr(_shdw, _ofs)) = _REG_Data(_REG_Addr(_phys, _ofs)))

#define _REG_OfsWr(_shdw,_phys,_ofs,_v)	\
	(_REG_Data(_REG_Addr(_phys, _ofs)) = _REG_Data(_REG_Addr(_shdw, _ofs)) = _v)


#define _REG_RdFLOfs(_shdw,_phys,_ofs)	\
	(_REG_Data(_REG_Addr(_shdw, _ofs)) = _REG_Data(_REG_Addr(_phys, _ofs)))

#define _REG_WrFLOfs(_shdw,_phys,_ofs)	\
	(_REG_Data(_REG_Addr(_phys, _ofs)) = _REG_Data(_REG_Addr(_shdw, _ofs)))


#define _REG_RdFL(_shdw,_phys,_r)		((_shdw->_r)=(_phys->_r))
#define _REG_WrFL(_shdw,_phys,_r)		((_phys->_r)=(_shdw->_r))

#define _REG_Rd(_shdw, _r)				(*((UINT32*)(&(_shdw->_r))))
#define _REG_Wr(_shdw, _r,_v)			((_REG_Rd(_shdw,_r))=((UINT32)(_v)))

#define _REG_RdFd(_shdw,_r,_f01)		(_shdw->_r._f01)

#define _REG_Rd01(_shdw,_r,_f01,_v01)										\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
								} while(0)

#define _REG_Rd02(_shdw,_r,_f01,_v01,_f02,_v02)								\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
								} while(0)

#define _REG_Rd03(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03)					\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
								} while(0)

#define _REG_Rd04(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)			\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
								} while(0)

#define _REG_Rd05(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05)												\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
								} while(0)

#define _REG_Rd06(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06)									\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
								} while(0)

#define _REG_Rd07(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07)							\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
								} while(0)

#define _REG_Rd08(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)				\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
								} while(0)

#define _REG_Rd09(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09)												\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
								} while(0)

#define _REG_Rd10(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10)									\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
								} while(0)

#define _REG_Rd11(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11)							\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
									(_v11) = (_shdw->_r._f11);				\
								} while(0)

#define _REG_Rd12(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)				\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
									(_v11) = (_shdw->_r._f11);				\
									(_v12) = (_shdw->_r._f12);				\
								} while(0)

#define _REG_Rd13(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13)												\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
									(_v11) = (_shdw->_r._f11);				\
									(_v12) = (_shdw->_r._f12);				\
									(_v13) = (_shdw->_r._f13);				\
								} while(0)

#define _REG_Rd14(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13,_f14,_v14)									\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
									(_v11) = (_shdw->_r._f11);				\
									(_v12) = (_shdw->_r._f12);				\
									(_v13) = (_shdw->_r._f13);				\
									(_v14) = (_shdw->_r._f14);				\
								} while(0)

#define _REG_Rd15(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13,_f14,_v14,_f15,_v15)							\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
									(_v11) = (_shdw->_r._f11);				\
									(_v12) = (_shdw->_r._f12);				\
									(_v13) = (_shdw->_r._f13);				\
									(_v14) = (_shdw->_r._f14);				\
									(_v15) = (_shdw->_r._f15);				\
								} while(0)

#define _REG_Rd16(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)				\
								do { 										\
									(_v01) = (_shdw->_r._f01);				\
									(_v02) = (_shdw->_r._f02);				\
									(_v03) = (_shdw->_r._f03);				\
									(_v04) = (_shdw->_r._f04);				\
									(_v05) = (_shdw->_r._f05);				\
									(_v06) = (_shdw->_r._f06);				\
									(_v07) = (_shdw->_r._f07);				\
									(_v08) = (_shdw->_r._f08);				\
									(_v09) = (_shdw->_r._f09);				\
									(_v10) = (_shdw->_r._f10);				\
									(_v11) = (_shdw->_r._f11);				\
									(_v12) = (_shdw->_r._f12);				\
									(_v13) = (_shdw->_r._f13);				\
									(_v14) = (_shdw->_r._f14);				\
									(_v15) = (_shdw->_r._f15);				\
									(_v16) = (_shdw->_r._f16);				\
								} while(0)


#define _REG_Wr01(_shdw,_r,_f01,_v01)										\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
								} while(0)

#define _REG_Wr02(_shdw,_r,_f01,_v01,_f02,_v02)								\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
								} while(0)

#define _REG_Wr03(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03)					\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
								} while(0)

#define _REG_Wr04(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)			\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
								} while(0)

#define _REG_Wr05(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05)												\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
								} while(0)

#define _REG_Wr06(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06)									\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
								} while(0)

#define _REG_Wr07(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07)							\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
								} while(0)

#define _REG_Wr08(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)				\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
								} while(0)

#define _REG_Wr09(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09)												\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
								} while(0)

#define _REG_Wr10(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10)									\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
								} while(0)

#define _REG_Wr11(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11)							\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
									(_shdw->_r._f11) = (_v11);				\
								} while(0)

#define _REG_Wr12(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)				\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
									(_shdw->_r._f11) = (_v11);				\
									(_shdw->_r._f12) = (_v12);				\
								} while(0)

#define _REG_Wr13(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13)												\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
									(_shdw->_r._f11) = (_v11);				\
									(_shdw->_r._f12) = (_v12);				\
									(_shdw->_r._f13) = (_v13);				\
								} while(0)

#define _REG_Wr14(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13,_f14,_v14)									\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
									(_shdw->_r._f11) = (_v11);				\
									(_shdw->_r._f12) = (_v12);				\
									(_shdw->_r._f13) = (_v13);				\
									(_shdw->_r._f14) = (_v14);				\
								} while(0)

#define _REG_Wr15(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13,_f14,_v14,_f15,_v15)							\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
									(_shdw->_r._f11) = (_v11);				\
									(_shdw->_r._f12) = (_v12);				\
									(_shdw->_r._f13) = (_v13);				\
									(_shdw->_r._f14) = (_v14);				\
									(_shdw->_r._f15) = (_v15);				\
								} while(0)

#define _REG_Wr16(_shdw,_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,			\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,				\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,				\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)				\
								do { 										\
									(_shdw->_r._f01) = (_v01);				\
									(_shdw->_r._f02) = (_v02);				\
									(_shdw->_r._f03) = (_v03);				\
									(_shdw->_r._f04) = (_v04);				\
									(_shdw->_r._f05) = (_v05);				\
									(_shdw->_r._f06) = (_v06);				\
									(_shdw->_r._f07) = (_v07);				\
									(_shdw->_r._f08) = (_v08);				\
									(_shdw->_r._f09) = (_v09);				\
									(_shdw->_r._f10) = (_v10);				\
									(_shdw->_r._f11) = (_v11);				\
									(_shdw->_r._f12) = (_v12);				\
									(_shdw->_r._f13) = (_v13);				\
									(_shdw->_r._f14) = (_v14);				\
									(_shdw->_r._f15) = (_v15);				\
									(_shdw->_r._f16) = (_v16);				\
								} while(0)





/* Register Access with I2C */
#define _REG_I2C_RdFL(_idx,_slave,_reg,_data) \
	do{ REG_ReadI2C(_idx, _slave, _reg, (UINT8*)(&(_data))); }while(0)

#define _REG_I2C_WrFL(_idx,_slave,_reg,_data) \
	do{ REG_WriteI2C(_idx, _slave, _reg, *((UINT8*)(&(_data)))); } while(0)

#define _REG_I2C_Rd(_shdw,_r)			*((UINT8*)(&(_shdw->_r)))
#define _REG_I2C_Wr(_shdw,_r,_v)		((_REG_Rd(_shdw,_r))=((UINT8)(_v)))



#endif
