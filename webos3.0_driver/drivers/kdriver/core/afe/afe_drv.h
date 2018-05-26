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
 *  driver interface header for afe device. ( used only within kdriver )
 *	afe device will teach you how to make device driver with new platform.
 *
 *  @author		wonsik.do (wonsik.do@lge.com)
 *  @version	1.0 
 *  @date		2009.12.30
 *
 *  @addtogroup lg1150_afe
 *	@{
 */

#ifndef	_AFE_DRV_H_
#define	_AFE_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "debug_util.h"
#include "afe_cfg.h"
#include "afe_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define	AFE_PRINT(format, args...)			DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_NOTI, format, ##args)
#define	CVD_DEBUG(format, args...)			DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE, format, ##args)
#define	ADC_DEBUG(format, args...)			DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+1, format, ##args)
#define	AFE_TRACE(format, args...)			DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+2, format, ##args)
#define	AFE_ERROR(format, args...)			DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_ERROR, format, ##args)
#define	CVD_CSD_DEBUG(format, args...)		DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+3, format, ##args)
#define	CVD_WA_DEBUG(format, args...)		DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+4, format, ##args)
#define	ADC_CALIB_DEBUG(format, args...)	DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+5, format, ##args)
#define	ADC_APA_DEBUG(format, args...)		DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+6, format, ##args)
#define	ADC_IFM_DEBUG(format, args...)		DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+7, format, ##args)
#define	CVD_THREAD_DEBUG(format, args...)		DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+8, format, ##args)
#define	ADC_THREAD_DEBUG(format, args...)		DBG_PRINT( g_afe_debug_fd, LX_LOGM_LEVEL_TRACE+9, format, ##args)

#define AFE_CHECK_CODE(__checker,__if_action,fmt,args...)   \
             __CHECK_IF_ERROR(__checker, AFE_ERROR, __if_action , fmt, ##args )
/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
extern	void	 AFE_PreInit(void);
extern	int      AFE_Init(void);
extern	void     AFE_Cleanup(void);

extern void 	AFE_GetMemCfg( UINT32* mem_base_comb_buffer, UINT32* mem_size_comb_buffer, UINT32* mem_base_cvd_reg, UINT32* mem_size_cvd_reg);
/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern	int		g_afe_debug_fd;

extern void    AFE_InitCfg ( void );
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AFE_DRV_H_ */

/** @} */
