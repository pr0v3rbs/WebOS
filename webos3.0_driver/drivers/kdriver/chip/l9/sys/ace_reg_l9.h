#ifndef __ACE_REG_L9_H__
#define __ACE_REG_L9_H__

#include "reg_ctrl.h"
#include "sys_reg_base.h"

#include "ace_reg_l9b0.h"


typedef struct
{
	union {
		UINT32          *addr;
//		ACE_REG_L9A0_T	*a0;
		ACE_REG_L9B0_T	*b0;
	} shdw;
} ACE_REG_CTRL_L9_T;

#define ACE_REG_L9_I2C_IDX		(0)

#define ACE_REG_L9A0_SHDW		(gACE_REG_CTRL_L9.shdw.a0)
#define ACE_REG_L9B0_SHDW		(gACE_REG_CTRL_L9.shdw.b0)



#define ACE_REG_L9_RdFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_RdFL(ACE_REG_L9_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1, 	\
					  _r##_##_chip##_REG_ADDR, 			\
					  ACE_REG_##_chip##_SHDW->_r);		\
	} while(0)

#define ACE_REG_L9_WrFL(_chip, _r)						\
	do{ 												\
		_REG_I2C_WrFL(ACE_REG_L9_I2C_IDX,				\
					  (_r##_##_chip##_SLAVE_ADDR)<<1,	\
					  _r##_##_chip##_REG_ADDR,			\
					  ACE_REG_##_chip##_SHDW->_r);		\
	} while(0)


#define ACE_REG_L9_Rd(_chip,_r)				_REG_I2C_Rd(ACE_REG_##_chip##_SHDW,_r)
#define ACE_REG_L9_Wr(_chip,_r,_v)			_REG_I2C_Wr(ACE_REG_##_chip##_SHDW,_r,_v)

#define ACE_REG_L9_RdFd(_chip,_r,_f01) \
	_REG_RdFd(ACE_REG_##_chip##_SHDW,_r,_f01)

#define ACE_REG_L9_Rd01(_chip,_r,_f01,_v01) \
	_REG_Rd01(ACE_REG_##_chip##_SHDW,_r,_f01,_v01)

#define ACE_REG_L9_Rd02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Rd02(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define ACE_REG_L9_Rd03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Rd03(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)


#define ACE_REG_L9_Wr01(_chip,_r,_f01,_v01) \
	_REG_Wr01(ACE_REG_##_chip##_SHDW,_r,_f01,_v01)

#define ACE_REG_L9_Wr02(_chip,_r,_f01,_v01,_f02,_v02) \
	_REG_Wr02(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02)

#define ACE_REG_L9_Wr03(_chip,_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	_REG_Wr03(ACE_REG_##_chip##_SHDW,_r,_f01,_v01,_f02,_v02,_f03,_v03)




/* L9B0 */
#define ACE_REG_L9B0_RdFL(_r)		ACE_REG_L9_RdFL(L9B0,_r)
#define ACE_REG_L9B0_WrFL(_r)		ACE_REG_L9_WrFL(L9B0,_r)

#define ACE_REG_L9B0_RdFd(_r,_f01)					ACE_REG_L9_RdFd(L9B0,_r,_f01)

#define ACE_REG_L9B0_Rd01(_r,_f01,_v01)				ACE_REG_L9_Rd01(L9B0,_r,_f01,_v01)
#define ACE_REG_L9B0_Rd02(_r,_f01,_v01,_f02,_v02)	ACE_REG_L9_Rd02(L9B0,_r,_f01,_v01,_f02,_v02)
#define ACE_REG_L9B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ACE_REG_L9_Rd03(L9B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)

#define ACE_REG_L9B0_Wr01(_r,_f01,_v01)				ACE_REG_L9_Wr01(L9B0,_r,_f01,_v01)
#define ACE_REG_L9B0_Wr02(_r,_f01,_v01,_f02,_v02)	ACE_REG_L9_Wr02(L9B0,_r,_f01,_v01,_f02,_v02)
#define ACE_REG_L9B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03) \
	ACE_REG_L9_Wr03(L9B0,_r,_f01,_v01,_f02,_v02,_f03,_v03)



/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern ACE_REG_CTRL_L9_T	gACE_REG_CTRL_L9;


#endif
