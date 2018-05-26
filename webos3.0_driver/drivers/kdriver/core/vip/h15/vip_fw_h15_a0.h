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

/**
 * @file
 *
 *  main driver implementation for vision processor device.
 *	vision processor device will teach you how to make device driver with new platform.
 *
 *  author		hyunho kim (hyunho747.kim@lge.com)
 *  version		1.0
 *  date		2014.02.06
 *  note		Additional information.
 *
 *  @addtogroup lg1210_vip
 * @{
 */

#ifndef _VIP_FW_H15_A0_
#define	_VIP_FW_H15_A0_

#if !defined(VIP_FIRM_DOWN_FROM_KADAP)

#if defined(VIP_GESTURE_ON)

//#include "./firmware/H15_VIP_GRE_20140723a.h"
//#include "./firmware/H15_VIP_GRE_20140723b.h"
//#include "./firmware/H15_VIP_GRE_20140724a.h"
//#include "./firmware/H15_VIP_GRE_20140724b.h"
//#include "./firmware/H15_VIP_GRE_20140725a.h"
#include "./firmware/H15_VIP_GRE_47260021a.h"

#else
//#include "./firmware/H15_VIP_FW_v0_8.h"
//#include "./firmware/VP_FD_Printf.h"
//#include "./firmware/VP_FD_noPrintf.h"
//#include "./firmware/VP_FD_DMA_Printf.h"
//#include "./firmware/VP_FD_DMA_noPrintf.h"

//#include "./firmware/H15_VIP_FD_FW_140429.h"
//#include "./firmware/H15_VIP_FD_FW_140430b_64M.h"
#include "./firmware/H15_VIP_FD_FW_140430c_64M.h"
#endif

#endif	//__VIP_FIRM_DOWN_FROM_KADAP

#endif	/* _VIP_FW_H15_A0_ */

