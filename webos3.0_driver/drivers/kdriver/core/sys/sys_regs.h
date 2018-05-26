#ifndef __SYS_REGS_H__
#define __SYS_REGS_H__

#include "base_types.h"
#include "reg_ctrl.h"

#ifdef INCLUDE_L9_CHIP_KDRV
#include "l9/sys/ctop_ctrl_reg_l9.h"
#include "l9/sys/actop_ctrl_reg_l9.h"
#include "l9/sys/ace_reg_l9.h"
#endif

#ifdef INCLUDE_H13_CHIP_KDRV
#include "h13/sys/ctop_ctrl_reg_h13.h"
#include "h13/sys/ace_reg_h13.h"
#include "h13/sys/cpu_top_reg_h13.h"
#endif

#ifdef INCLUDE_M14_CHIP_KDRV
#include "m14/sys/ctop_ctrl_reg_m14.h"
#include "m14/sys/atop_reg_m14.h"
#include "m14/sys/cpu_top_reg_m14.h"
#endif

#ifdef INCLUDE_H14_CHIP_KDRV
#include "h14/sys/ctop_ctrl_reg_h14.h"
#include "h14/sys/atop_reg_h14.h"
#include "h14/sys/cpu_top_reg_h14.h"
#endif

#ifdef INCLUDE_H15_CHIP_KDRV
#include "h15/sys/ctop_ctrl_reg_h15.h"
#include "h15/sys/atop_reg_h15.h"
//#include "h15/sys/cpu_top_reg_h15.h"
#endif

#endif
