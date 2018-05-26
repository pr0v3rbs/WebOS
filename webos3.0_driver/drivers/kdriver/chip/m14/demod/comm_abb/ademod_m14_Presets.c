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

/*****************************************************************************
**
**  Name: ademod_common_Preset.c
**
**  Description:   Preset user settings.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_Preset_Set
**
**  References:     
**
**  Exports:  
**
**  Dependencies:   ademod_common.h 
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/


#if defined( __cplusplus )
extern "C"
{
#endif

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "ademod_common.h"
#include "ademod_common_Menu.h"
#include "ademod_common_Presets.h"
#include "ademod_m14_UserSettings.c"


LX_ADEMOD_Result DEMOD_M14_Analog_Preset_Set(Handle_t hDevice, UINT32 smartTuneFilter, UINT32 Section)
{
		LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
		UINT32 Size = 0;
		const LX_ADEMOD_PRESET_TABLE *ADEMOD_UserSettings = NULL;
		UINT32 value[LX_ADEMOD_MAX_PARAM_SIZE];
		UINT32 param_id;
		UINT32 i, j;
		UINT32 reg_addr;
		UINT32 reg_size;
		UINT32 reg_data;

		if (Section == LX_ADEMOD_PRESET_SYSTEM_INIT) { // System init
		#ifdef INIT_ABB_USER
			ADEMOD_UserSettings = &ADEMOD_UserSettings_Init;
			Size = sizeof(ADEMOD_UserSettings_Init) / sizeof(LX_ADEMOD_PARAM);
		#endif
		}
		else { // ADEMOD_PRESET_MODE, ADEMOD_PRESET_REGS
			if (smartTuneFilter & LX_ADEMOD_SmartTuneFilterAnalog) { // Analog mode
			
				if(lx_chip_rev() <= LX_CHIP_REV( M14, A1))
				{
					switch (smartTuneFilter & LX_ADEMOD_SmartTuneFilterMode) // Video Standard
					{
						case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
						#ifdef NTSC_M_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_NTSC_M_M14A;
							Size = sizeof(ADEMOD_UserSettings_NTSC_M_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
						#ifdef PAL_B_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_B_M14A;
							Size = sizeof(ADEMOD_UserSettings_PAL_B_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
						#ifdef PAL_D_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_D_M14A;
							Size = sizeof(ADEMOD_UserSettings_PAL_D_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
						#ifdef PAL_I_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_I_M14A;
							Size = sizeof(ADEMOD_UserSettings_PAL_I_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
						#ifdef SECAM_L_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_SECAM_L_M14A;
							Size = sizeof(ADEMOD_UserSettings_SECAM_L_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
						#ifdef SECAM_L1_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_SECAM_L1_M14A;
							Size = sizeof(ADEMOD_UserSettings_SECAM_L1_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
						#ifdef PAL_DK_NICAM_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_DK_NICAM_M14A;
							Size = sizeof(ADEMOD_UserSettings_PAL_DK_NICAM_M14A) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						default:
						break;
					}
				}
				else
				{
					switch (smartTuneFilter & LX_ADEMOD_SmartTuneFilterMode) // Video Standard
					{
						case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
							#ifdef NTSC_M_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_NTSC_M_M14B;
							Size = sizeof(ADEMOD_UserSettings_NTSC_M_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
							#ifdef PAL_B_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_B_M14B;
							Size = sizeof(ADEMOD_UserSettings_PAL_B_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
							#ifdef PAL_D_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_D_M14B;
							Size = sizeof(ADEMOD_UserSettings_PAL_D_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
							#ifdef PAL_I_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_I_M14B;
							Size = sizeof(ADEMOD_UserSettings_PAL_I_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
							#ifdef SECAM_L_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_SECAM_L_M14B;
							Size = sizeof(ADEMOD_UserSettings_SECAM_L_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
							#ifdef SECAM_L1_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_SECAM_L1_M14B;
							Size = sizeof(ADEMOD_UserSettings_SECAM_L1_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
							#ifdef PAL_DK_NICAM_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_PAL_DK_NICAM_M14B;
							Size = sizeof(ADEMOD_UserSettings_PAL_DK_NICAM_M14B) / sizeof(LX_ADEMOD_PARAM);
							#endif
						break;
						default:
						break;
					}

				}
			} // if Analog mode
			else { // Digital mode
#if 1
					DEMOD_PRINT(" (%s, %d)Wrong Mode (can not support Digital !!  \n", __F__, __L__);
#else
				if (smartTuneFilter & LX_ADEMOD_SmartTuneFilterCable) { // Cable input
					switch (smartTuneFilter & LX_ADEMOD_SmartTuneFilterMode) // Video Standard
					{
						case LX_ADEMOD_LOWIF_MODE_8MHZ:
						#ifdef LOIF_8MHZ_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_LowIF_DVBC_8MHz;
							Size = sizeof(ADEMOD_UserSettings_LowIF_DVBC_8MHz) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_LOWIF_MODE_7MHZ:
						#ifdef LOIF_7MHZ_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_LowIF_DVBC_7MHz;
							Size = sizeof(ADEMOD_UserSettings_LowIF_DVBC_7MHz) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_LOWIF_MODE_6MHZ:
						#ifdef LOIF_6MHZ_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_LowIF_DVBC_6MHz;
							Size = sizeof(ADEMOD_UserSettings_LowIF_DVBC_6MHz) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						default:
						break;
					} // Video standard
				} // if Cable
				else { // Antenna input
					switch (smartTuneFilter & LX_ADEMOD_SmartTuneFilterMode) // Video Standard
					{
						case LX_ADEMOD_LOWIF_MODE_8MHZ:
						#ifdef LOIF_8MHZ_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_LowIF_8MHz;
							Size = sizeof(ADEMOD_UserSettings_LowIF_8MHz) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_LOWIF_MODE_7MHZ:
						#ifdef LOIF_7MHZ_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_LowIF_7MHz;
							Size = sizeof(ADEMOD_UserSettings_LowIF_7MHz) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						case LX_ADEMOD_LOWIF_MODE_6MHZ:
						#ifdef LOIF_6MHZ_USER
							ADEMOD_UserSettings = &ADEMOD_UserSettings_LowIF_6MHz;
							Size = sizeof(ADEMOD_UserSettings_LowIF_6MHz) / sizeof(LX_ADEMOD_PARAM);
						#endif
						break;
						default:
						break;
					} // Video standard
				} // if Antenna
#endif
			} // if Digital mode
		} // if ADEMOD_PRESET_MODE
		if ((Section == LX_ADEMOD_PRESET_SYSTEM_INIT) || (Section == LX_ADEMOD_PRESET_MODE))
		{ // Parameter section
			for (i = 0; i < Size; i++) {
				memset(value, 0, sizeof(value));
				param_id = (UINT32)((*ADEMOD_UserSettings)[i][0]);
				switch (param_id)
				{
					case LX_ADEMOD_PARM_DIGAGC_GAIN_THR:
						// 5- byte values
						value[0] = (UINT32) ((*ADEMOD_UserSettings)[i][1]);
						value[1] = (UINT32) ((*ADEMOD_UserSettings)[i][2]);
						value[2] = (UINT32)(((*ADEMOD_UserSettings)[i][3] & 0xffffffff));
						value[3] = (UINT32)(((*ADEMOD_UserSettings)[i][3] >> 32));
						value[4] = (UINT32)(((*ADEMOD_UserSettings)[i][4] & 0xffffffff));
						value[5] = (UINT32)(((*ADEMOD_UserSettings)[i][4] >> 32));

						break;
					case LX_ADEMOD_PARM_DIGAGC_GAIN_PRESET:
						// 5- byte values
						value[0] = (UINT32) ((*ADEMOD_UserSettings)[i][1]);
						value[1] = (UINT32) ((*ADEMOD_UserSettings)[i][2]);
						value[2] = (UINT32)(((*ADEMOD_UserSettings)[i][3] & 0xffffffff));
						value[3] = (UINT32)(((*ADEMOD_UserSettings)[i][3] >> 32));
						break;
					case LX_ADEMOD_REGISTER_SET:
						reg_addr = (UINT32)((*ADEMOD_UserSettings)[i][1]);
						reg_data = (UINT32)((*ADEMOD_UserSettings)[i][2]);
						reg_size = (UINT32)((*ADEMOD_UserSettings)[i][3]) / 8;
						ADEMOD_Register_SetX(hDevice, reg_addr, reg_size, reg_data, FALSE);
						break;
					default:
						for (j = 0; j < LX_ADEMOD_MAX_PARAM_SIZE-1; j++) {
							value[j] = (UINT32)((*ADEMOD_UserSettings)[i][j+1]);
							// some parameters are int64, TBD
						}
						break;
				}
				ADEMOD_Parameter_Set(hDevice, param_id, value, FALSE);
			}
		}
		else if (Section == LX_ADEMOD_PRESET_REGS)
		{ // Registers section
			UINT32 param_id;
			UINT32 reg_addr;
			UINT32 reg_size;
			UINT32 reg_data;
			//UINT64 x; //LGIT 091010
			for (i = 0; i < Size; i++) {
				memset(value, 0, sizeof(value));
				param_id = (UINT32)((*ADEMOD_UserSettings)[i][0]);
				switch (param_id)
				{
					case LX_ADEMOD_REGISTER_SET:
						reg_addr = (UINT32)((*ADEMOD_UserSettings)[i][1]);
						reg_data = (UINT32)((*ADEMOD_UserSettings)[i][2]);
						reg_size = (UINT32)((*ADEMOD_UserSettings)[i][3]) / 8;
						ADEMOD_Register_SetX(hDevice, reg_addr, reg_size, reg_data, FALSE);
						break;
					default:
						// skip
						break;
				}
			}
		}

		return (status);
}
#if defined( __cplusplus )
}
#endif
