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
 *  Linux proc interface for memprot device.
 *	memprot device will teach you how to make device driver with new platform.
 *
 *  author		sanghyeun.kim
 *  version		1.0
 *  date		2012.08.07
 *  note		Additional information.
 *
 *  @addtogroup lg1150_memprot
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "memprot_drv.h"
#include "proc_util.h"
#include "debug_util.h"
#include "memprot_cfg.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
enum {
	PROC_ID_AUTHOR	= 0,
	PROC_ID_USAGE,
	PROC_ID_COMMAND,
	PROC_ID_REGISTER,
	PROC_ID_MAX,
};

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

extern unsigned	MEMPROT_SetProtect(unsigned monitor, unsigned reg1_s, unsigned reg1_e,unsigned ,unsigned );
extern void		MEMPROT_ShowRegInfo(unsigned monitor);
extern char*	MEMPROT_GetMonitorName(unsigned monitor);
/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void	MEMPROT_PROC_CMDHandler (char*);
static void	MEMPROT_PROC_REGHandler (char*);
/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static OS_PROC_DESC_TABLE_T	_g_memprot_device_proc_table[] =
{
	{ "author",		PROC_ID_AUTHOR  , 	OS_PROC_FLAG_READ },
	{ "usage",		PROC_ID_USAGE , 	OS_PROC_FLAG_READ },
	{ "command",	PROC_ID_COMMAND , 	OS_PROC_FLAG_WRITE },
	{ "register",	PROC_ID_REGISTER , 	OS_PROC_FLAG_WRITE },
	{ NULL, 		PROC_ID_MAX		, 0 }
};

#if INCLUDE_KDRV_MEMPROT==0xB0
const char* usage= \
"\n \
1. command  : Mod_Name  Region1_start Region1_end  Region2_start Region2_end  \n \
2. register : Mod_name \n\n \
@Mod_Name(ALL for all monitor) \n \
\t + LBUS :  AUD, GPU0, GPU1, GFX,ICOD,TE,VDEC_P,VDEC_S,VENC_P,VENC_S,DE_E_CVD, DE_E_MCU, CPU_M\n \
\t + GBUS :  DE_A, DE_B, DE_C, DE_D,DPP_A, DVI, DVO, SRE, MEP_A, MEP_B, BVE_MCU, MC,TCON, CPU_S\n \
@Region addr.(hexa) : MSB 8bit for each region, ex) 0x20000000 => 0x20, disable => start:0xFF, end:0x00  \n \
\n";
#else
const char* usage= \
"\n \
1. command  : Mod_Name  Region1_start Region1_end  Region2_start Region2_end  \n \
2. register : Mod_name \n\n \
@Mod_Name(ALL for all monitor) \n \
\t + LBUS :  AUD, GPU0, GPU1, GFX,ICOD,TE,VDEC_P,VDEC_S,VENC_P,VENC_S,CPU_M\n \
\t + GBUS :  DE_A, DE_B, DE_C, DE_D, DE_E_CVD, DE_E_MCU, DPP_A, DVI, DVO, SRE, MEP_A, MEP_B, BVE_MCU, MC, CPU_S\n \
@Region addr.(hexa) : MSB 8bit for each region, ex) 0x20000000 => 0x20, disable => start:0xFF, end:0x00  \n \
\n";
#endif


/*========================================================================================
	Implementation Group
========================================================================================*/

/*
 * read_proc implementation of memprot device
 *
*/
static int	_MEMPROT_ReadProcFunction(	UINT32 procId, char* buffer )
{
	int		ret;

	/* TODO: add your proc_write implementation */
	switch( procId )
	{
		case PROC_ID_AUTHOR:
		{
			ret = sprintf( buffer, "%s\n", "sanghyeun.kim" );
		}
		break;

		case PROC_ID_USAGE:
		{
			ret = sprintf( buffer, "%s\n ",usage );
		}
		break;

		default:
		{
			ret = sprintf( buffer, "%s(%d)\n", "unimplemented read proc", procId );
		}
	}

	return ret;
}

/*
 * write_proc implementation of memprot device
 *
*/
static int _MEMPROT_WriteProcFunction( UINT32 procId, char* command )
{
	/* TODO: add your proc_write implementation */

	switch( procId )
	{
		case PROC_ID_COMMAND:
		{
			MEMPROT_PRINT("command string : %s\n", command );
			MEMPROT_PROC_CMDHandler(command);
		}
		break;

		case PROC_ID_REGISTER:
		{
			MEMPROT_PRINT("register  string : %s\n", command );
			MEMPROT_PROC_REGHandler(command);
		}
		break;

		default:
		{
			/* do nothing */
		}
		break;
	}

	return strlen(command);
}

/**
 * initialize proc utility for memprot device
 *
 * @see MEMPROT_Init
*/
void	MEMPROT_PROC_Init (void)
{
	OS_PROC_CreateEntryEx ( MEMPROT_MODULE, _g_memprot_device_proc_table,
											_MEMPROT_ReadProcFunction,
											_MEMPROT_WriteProcFunction );
}

/**
 * cleanup proc utility for memprot device
 *
 * @see MEMPROT_Cleanup
*/
void	MEMPROT_PROC_Cleanup (void)
{
	OS_PROC_RemoveEntry( MEMPROT_MODULE );
}

/**
 * Proc command hanlder for memprot
 *
 * @see MEMPROT_PROC_CMDHandler
*/
static void	MEMPROT_PROC_CMDHandler (char* cmd)
{

	char module[16]={0,};// module name buffer
	int i,input=0;
	int	r1_s,r1_e,r2_s,r2_e;
	r1_s=r2_s=0xFF;
	r1_e=r2_e=0x00;

	input=sscanf(cmd,"%s %x %x %x %x \n", module,&r1_s,&r1_e,&r2_s,&r2_e);

	if((input!=5) || (strlen(module)==0))
	{
		MEMPROT_PRINT("[ ERROR ] Wrong  parameter!!!\n");
		return ;
	}

	MEMPROT_PRINT("[ CMD ] %s %#x %#x %#x %#x \n", module,r1_s,r1_e,r2_s,r2_e);


	/* Matching monitor name and setting corresponding protection region*/
	for(i = ENUM_MEMPROT_AUDIO ; i < ENUM_MEMPROT_MAX; i++)
		if(!strcmp(module, MEMPROT_GetMonitorName(i)))
		{
			MEMPROT_PRINT("[ CMD status ]  %d\n",MEMPROT_SetProtect(i,r1_s,r1_e,r2_s,r2_e));
			return;
		}

	/* Set protection region for all monitors*/
	 if(!strcmp(module,"ALL"))
		for(i = ENUM_MEMPROT_AUDIO ; i < ENUM_MEMPROT_MAX; i++)
			MEMPROT_SetProtect(i,r1_s,r1_e,r2_s,r2_e);
	else
		MEMPROT_PRINT("[ ERROR ]  Wrong module name!!!\n");

}

/**
 * Proc register  hanlder for memprot
 *
 * @see MEMPROT_PROC_CMDHandler
*/
static void	MEMPROT_PROC_REGHandler (char* cmd)
{
	int i=0;
	char module[16]={0,};

	if((sscanf(cmd,"%s \n", module)!= 1) ||  strlen(module)==0)
	{
		MEMPROT_PRINT("[ ERROR ] NO module name specified!!!\n");
		return;
	}

	/* Matching  monitor name and show matched  register info.*/
	for(i = ENUM_MEMPROT_AUDIO ; i < ENUM_MEMPROT_MAX; i++)
		if(!strcmp(module, MEMPROT_GetMonitorName(i)))
		{
			MEMPROT_ShowRegInfo(i);
			return;
		}

	/* Show the register info. of all monitors*/
	 if(!strcmp(module,"ALL"))
		for(i = ENUM_MEMPROT_AUDIO ; i < ENUM_MEMPROT_MAX; i++)
			MEMPROT_ShowRegInfo(i);
	else
		MEMPROT_PRINT("[ ERROR ]  Wrong module name!!!\n");


}

/** @} */


