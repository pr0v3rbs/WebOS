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

/** @file
 *
 *	kdriver main
 *
 *  author      daeyoung lim ( raxis.lim@lge.com )
 *  version		1.0
 *  date		2009.11.04
 *
 *  @addtogroup lg1150_base
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/mm.h>		// si_meminfo()
#include <linux/spinlock.h>

#include "base_drv.h"

#if 1//default LOGM
#include "logm_drv.h"
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
DEFINE_SPINLOCK (g_Base_AVLipsync_Lock);
EXPORT_SYMBOL (g_Base_AVLipsync_Lock);

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
    /**< digital chip revision information
     *
     * [note] please refer to LX_CHIP_REV macro in base_types.h
     */
    UINT32                  chip_rev;

    /**< analog chip revision information
     *
     * [note] please refer to LX_CHIP_REV macro in base_types.h
     */
    UINT32                  ace_rev;

    UINT32                  chip_plt;
}
LX_BASE_DEV_CFG_T;

/*----------------------------------------------------------------------------------------
    Board Basic Configuration
----------------------------------------------------------------------------------------*/
static LX_BASE_DEV_CFG_T   g_base_dev_cfg;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern char *simple_strtok(char *s, const char *delim, char **save_ptr);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern void	BASE_DEV_PreInit(void);
extern void	BASE_DEV_Init(void);
extern void BASE_DEV_Cleanup(void);

extern UINT32 BASE_PLT_Probe(void);		/* platform prober */
extern void	BASE_PLT_Init(void);		/* platform depdendent initializer */

extern void	BASE_PROC_Init(void);

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void	BASE_ProbeChipRev(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * initialize base device.
 * if global link mode, this device calls all init function of each module.
 *
*/
int BASE_Init(void)
{
	OS_CORE_Init(); 		/* initialize OS framework */
	BASE_ProbeChipRev();	/* read chip info */
    LOGM_Init();			/* initialize logm */

	OS_DEBUG_Init(); 		/* initialize debug framework 	*/
	OS_PROC_Init();			/* initialize proc framework 	*/

	BASE_DEV_PreInit();		/* preconfigure devices */
	BASE_PLT_Init();		/* initialize target platform environment */
	BASE_DEV_Init();		/* initialize SOC devices */

	BASE_PROC_Init();		/* initialize base module proc */
	return 0;
}

/**
 * cleanup base device.
 * if global link mode, this function calls all cleanup function of each module.
*/
void BASE_Cleanup(void)
{
	/* cleanup custom board */
	BASE_DEV_Cleanup();

	DBG_MSG("<!> custom board  cleanup..\n");

	/* cleanup all the base system */
	OS_PROC_Cleanup();
	OS_DEBUG_Cleanup();
	OS_CORE_Cleanup();

    LOGM_Cleanup();
}

static UINT32 BASE_ScanChipName( char chp_class, int chp_num )
{
	int i;
	static const struct
	{ 	char	chp_class;
		int		chp_num;
        UINT32	chp_name;
	}
	chip_list[] =   {   { 'L',  9, LX_CHIP_L9  },
                        { 'H', 13, LX_CHIP_H13 },
                        { 'M', 14, LX_CHIP_M14 },
                        { 'H', 14, LX_CHIP_H14 },
                        { 'H', 15, LX_CHIP_H15 },
						{ 'M', 16, LX_CHIP_M16 } };

    const int num_list = sizeof(chip_list)/sizeof(chip_list[0]);

	for ( i=0; i< num_list; i++ )
	{
		if ( chip_list[i].chp_class == chp_class && chip_list[i].chp_num == chp_num )   return chip_list[i].chp_name;
	}

    return LX_CHIP_UNKNOWN;
}

/**
 *
 *
 */
static void	BASE_ProbeChipRev(void)
{
	char*	cmd_buf;
	int		cmd_buf_sz = 511;

	char *tok, *sav_tok;
	char delim[] = " \t\n";

	if( NULL == ( cmd_buf = OS_Malloc( cmd_buf_sz+1 )  ) ) goto func_exit;

#ifdef MODULE // saved_command_line will be exported by kernel..but not yet
	{
		OS_FILE_T	file;
		if( RET_OK != OS_OpenFile( &file, "/proc/cmdline", O_RDONLY|O_LARGEFILE, 0666 ) )
		{
			printk("<error> can't open /proc/cmdline\n"); goto func_exit;
		}

		if ( 0 >= OS_ReadFile( &file, cmd_buf, cmd_buf_sz ) )
		{
			OS_CloseFile(&file);
			printk("<error> can't read /proc/cmdline\n"); goto func_exit;
		}
		OS_CloseFile(&file);
	}
#else
	{
		extern char *saved_command_line;
		cmd_buf_sz = snprintf( cmd_buf, cmd_buf_sz, "%s", saved_command_line );
	}
#endif

	/* initialize data */
	g_base_dev_cfg.chip_rev = 0x0;
	g_base_dev_cfg.ace_rev  = 0x0;
	g_base_dev_cfg.chip_plt = 0x0;

	{
#define	TOKEN_DCHIP		0x1
#define TOKEN_ACHIP		0x2
#define	TOKEN_PLATFORM	0x4
		UINT8	status = TOKEN_DCHIP|TOKEN_ACHIP; //TOKEN_PLATFORM
		UINT8	chip_model;
		UINT32	chip_rev;

		char	chp_class,	rev_class;
		int		chp_num,	rev_num;

		tok = simple_strtok( cmd_buf, delim, &sav_tok );

		while (tok)
		{
			chip_model  = 0x0;
			chip_rev 	= 0x0;

			/* find chip = L,9,A,0 or H,13,A,0 .. */
	        if ( (status & TOKEN_DCHIP) && sscanf( tok, "chip=%c%d%c%d", &chp_class, &chp_num, &rev_class, &rev_num ) == 4 )
			{
				chip_model	= BASE_ScanChipName( chp_class, chp_num );
				chip_rev 	= ((( rev_class - 'A' ) + 0xA) << 4 ) + rev_num ;

				g_base_dev_cfg.chip_rev = MAKE_CHIP_REV( chip_model, chip_rev );
				status &= ~TOKEN_DCHIP;
			}

			/* find ace = L,9,A,0 or H,13,A,0 .. */
	        if ( (status & TOKEN_ACHIP) && sscanf( tok, "ace=%c%d%c%d", &chp_class, &chp_num, &rev_class, &rev_num ) == 4 )
			{
				chip_model	= BASE_ScanChipName( chp_class, chp_num );
				chip_rev 	= ((( rev_class - 'A' ) + 0xA) << 4 ) + rev_num ;

				g_base_dev_cfg.ace_rev = MAKE_CHIP_REV( chip_model, chip_rev );
			}
#if 0
			if ( (status & TOKEN_PLATFORM) && sscanf( tok, "platform=%s", plat_str ) == 1 /* num of token */ )
			{
				g_base_dev_cfg.chip_plt = !strncmp( plat_str, "FPGA", 4 )? LX_CHIP_PLATFORM_FPGA :
										  !strncmp( plat_str, "COSMO",5 )? LX_CHIP_PLATFORM_COSMO: LX_CHIP_PLATFORM_GP;
				status &= ~TOKEN_PLATFORM;
			}
#endif
			/* all tokens done, break loop !! */
			if ( !status ) break;

			tok = simple_strtok( NULL, delim, &sav_tok );
		}
	}
func_exit:
	if ( cmd_buf ) OS_Free( cmd_buf );

	/* check if all parameters are parsed successfully.
	 * when valid token is not found, use the some default value.
	 * espacially, the default value of ace_rev is the same value as chip_rev
	 */
	if ( g_base_dev_cfg.chip_rev == 0x0 ) g_base_dev_cfg.chip_rev = LX_CHIP_UNKNOWN;
	if ( g_base_dev_cfg.ace_rev  == 0x0 ) g_base_dev_cfg.ace_rev  = g_base_dev_cfg.chip_rev;

#ifdef CONFIG_LX_CHIP_EMUL
	#if CONFIG_LX_CHIP_EMUL
	g_base_dev_cfg.chip_rev = CONFIG_LX_CHIP_EMUL;
	g_base_dev_cfg.ace_rev	= CONFIG_LX_CHIP_EMUL;
	#endif
#endif

	g_base_dev_cfg.chip_plt = BASE_PLT_Probe(); /* probe chip platform */

	printk("+ CHIP_REV=0x%04X ACE_REV=0x%04X CHIP_PLT=0x%04X GFX_RES=0x%08x\n",
			g_base_dev_cfg.chip_rev, g_base_dev_cfg.ace_rev, g_base_dev_cfg.chip_plt, lx_chip_graphic_res() );
}

UINT32	lx_chip		( void )
{
	return (g_base_dev_cfg.chip_rev>>8) & 0xff;
}
EXPORT_SYMBOL(lx_chip);

UINT32	lx_chip_rev	( void )
{
    return g_base_dev_cfg.chip_rev;
}
EXPORT_SYMBOL(lx_chip_rev);

UINT32	lx_ace		( void )
{
	return (g_base_dev_cfg.ace_rev>>8) & 0xff;
}
EXPORT_SYMBOL(lx_ace);

UINT32	lx_ace_rev	( void )
{
    return g_base_dev_cfg.ace_rev;
}
EXPORT_SYMBOL(lx_ace_rev);

UINT32  lx_chip_plt ( void )
{
    return g_base_dev_cfg.chip_plt;
}
EXPORT_SYMBOL(lx_chip_plt);

/////////////////////////////////////////////////////////////////////////////////////////
#if defined(CONFIG_LG_BUILTIN_KDRIVER) && defined(CONFIG_LGSNAP)
user_initcall_grp("kdrv",BASE_Init);
#else
module_init(BASE_Init);
#endif
module_exit(BASE_Cleanup);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("BASE driver");
MODULE_LICENSE("GPL");

/** @} */

