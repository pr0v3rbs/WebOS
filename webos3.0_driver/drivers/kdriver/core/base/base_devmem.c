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
 *	main configuration for lg1150 driver
 *
 *  author		daeyoung lim ( raxis.lim@lge.com )
 *  version		1.0
 *  date		2009.11.04
 *
 *  @addtogroup lg1150_base
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define DEVMEM_DEBUG 			2		// 1 : summary for each module, 2 : detailed print enabled.
#define DEVMEM_CHECK			1
#define DEVMEM_ALLOC			1
#define	DEVMEM_ALIGN_VALUE		14		/* 16K align */

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>

#include "base_types.h"
#include "base_device.h"
#include "debug_util.h"
#include "os_util.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define	DEVMEM_HDR_STRING	"[header]"

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define SWAP(_p1,_p2)		({typeof(*_p1) _tmp = *_p2;  *_p2 = *_p1; *_p1 = _tmp; })

#define _MB_(_sz)	(( _sz)>>20)
#define _KB_(_sz)	((_sz)>1024)?(((_sz)>>10)&0x3ff):1
#define _BB_(_sz)	(( _sz)     &0x3ff)

#define LX_MEMCFG_ENTRY_NAME(Entry)	((LX_MEMCFG_T*)&(Entry))->name
#define LX_MEMCFG_ENTRY_BASE(Entry)	((LX_MEMCFG_T*)&(Entry))->base
#define LX_MEMCFG_ENTRY_SIZE(Entry)	((LX_MEMCFG_T*)&(Entry))->size

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern char *simple_strtok(char *s, const char *delim, char **save_ptr);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
/*
 * In order to maintain memory configuration without overlapping between kdrv_lg1150 and mali kernel driver,
 * mali kernel driver SHOULD BE INITIALIZED ONLY AFTER lg1150 kernel driver module.
 *
 * after BASE_DEVMEM_AllocDevMem() shall calculate base address for mali gpu, then mali kernel module
 * depends on below variable to initialize its memory.
 * 20100813 seokjoo.lee.
 */
unsigned long kdrv_gpu_pmem_base;
unsigned long kdrv_gpu_pmem_size;

EXPORT_SYMBOL(kdrv_gpu_pmem_base);
EXPORT_SYMBOL(kdrv_gpu_pmem_size);

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void BASE_DEVMEM_PRINT			(struct seq_file *m, LX_MEMCFG_T *pM, int addr_shift, UINT32 prvEnd);

static int BASE_DEVMEM_AllocDevMem		(LX_MEMCFG_ENTRY_T* pEntry); 
static void BASE_DEVMEM_Print			(struct seq_file *m, LX_MEMCFG_ENTRY_T *pMemCfgEntry, UINT32 option);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_MEMCFG_ENTRY_T*	sMemCfgList = NULL;

/** Print format string.
 *
 * @see BASE_DEVMEM_Print BASE_DEVMEM_PRINT
 */
										// 12345678901234567890,0x12345678,+0x12345678,->,0x12345678,0x12345678,1234MB+1234kB,1234MB+1234kB,~,1234MB+1234kB
static const char *fmtMemCfgHeadLine	= "---------------------------------------------------------------------------------------------------------------\n"
										  "Name                ,Alloc Base  ~ Alloc End [Alloc Size]     Alloc Base MB [Alloc Size MB]\n"
										  "---------------------------------------------------------------------------------------------------------------\n";
static const char *fmtEntryFormat		= "%-20.20s,0x%08x ~ 0x%08x [0x%08x]    %4dMB+%4dkB [%4dMB+%4dkB]\n";
static const char *fmtEntryFormatSub	= " %-19.19s,0x%08x ~ 0x%08x [0x%08x]    %4dMB+%4dkB [%4dMB+%4dkB]\n";

/*========================================================================================
	Implementation Group
========================================================================================*/
int		BASE_DEVMEM_InitDevMem	( LX_MEMCFG_ENTRY_T* pEntry )
{
	int	ret;

	/* set memcfg list */
	sMemCfgList  = pEntry;

	/* start memory allocation */
	ret = BASE_DEVMEM_AllocDevMem( pEntry );
	BASE_DEVMEM_Print( NULL, NULL, 0 );

	return ret;
}

/**
 * Alloc(Reserve) Media memory.
 *
 * Actually, modify base addresses of each module's MemCfg.*base listed before sMemCfgHeader[1] in sMemCfgList array.
 *
 * @param pMemCfgEntry	[IN] if NULL, use global memory configuration array @ref sMemCfgList.
 *
 * @return 0 if success, negative for error(allocation failed)
 */
static int BASE_DEVMEM_AllocDevMem( LX_MEMCFG_ENTRY_T* pEntry )
{
	int					nEntries= 0;
	UINT32				EndAddr = 0;

#if (DEVMEM_CHECK>0)
	int					ErrCnt			= 0;
	UINT32				DDRMemAddr		= 0;
	UINT32				DDRMemLen		= 0;
	LX_MEMCFG_T*		pPrevM			= NULL;
#endif

	int					ret		= RET_OK;

	while ( pEntry && pEntry->pM )
	{
		LX_MEMCFG_T* pM = pEntry->pM;
		int			 nM = pEntry->nM;

#if (DEVMEM_ALLOC > 0)
		/* raxis.lim (2011/09/15) -- I want to use automatic allocation by default */
		#if (DEVMEM_DEBUG > 3)
		printk("%s: %d: ddr%d pM->name '%20.20s', %08x -> EndAddr %08x\n", __FUNCTION__, __LINE__, ddr, pM->name, pM->base, EndAddr);
		#endif
#endif	/* DEVMEM_ALLOC */

		// select allocation chunk in a module.
		for ( ; nM > 0; nM--, pM++)
		{
#if (DEVMEM_DEBUG > 3)
			printed += BASE_DEVMEM_PRINT(NULL, pM, 0, EndAddr);
#endif	
			/* check if current entry is for header. if header found, just update base address and skip further processing */
			if ( pM->name && !strncmp( pM->name, DEVMEM_HDR_STRING, strlen(DEVMEM_HDR_STRING) ) )
			{
				EndAddr		= LX_CALC_ALIGNED_VALUE(pM->base, DEVMEM_ALIGN_VALUE );
			
#if (DEVMEM_CHECK>0)
				DDRMemAddr	= LX_CALC_ALIGNED_VALUE(pM->base, DEVMEM_ALIGN_VALUE );
				DDRMemLen	= pM->size;
#endif
				break;
			}

			/* if base is NULL, try to set address automatically and forward EndAddr */
			if ( 0x0 == pM->base )
			{
				pM->base = LX_CALC_ALIGNED_VALUE(EndAddr,  DEVMEM_ALIGN_VALUE);
				EndAddr += LX_CALC_ALIGNED_VALUE(pM->size, DEVMEM_ALIGN_VALUE);
			}
			/* if base valid, keep the address and just forward EndAddr. not that EndAddr may jump to the long distance */
			else
			{
				EndAddr = pM->base + pM->size;
				EndAddr = LX_CALC_ALIGNED_VALUE(EndAddr, DEVMEM_ALIGN_VALUE );
			}
#if (DEVMEM_CHECK>0)
			{
				UINT32	DDRMemEndAddr	  = LX_CALC_ALIGNED_VALUE( DDRMemAddr + DDRMemLen, DEVMEM_ALIGN_VALUE );
				UINT32	CurrModuleAddr	  = pM->base;
				UINT32	CurrModuleEndAddr = LX_CALC_ALIGNED_VALUE( pM->base + pM->size, DEVMEM_ALIGN_VALUE );
				UINT32	PrevModuleEndAddr = (pPrevM)? LX_CALC_ALIGNED_VALUE( pPrevM->base + pPrevM->size, DEVMEM_ALIGN_VALUE ) : 0x0;

				/* check if current memory is inside DDR memory */
				__CHECK_IF_ERROR( CurrModuleAddr 	< DDRMemAddr, 			printk, ErrCnt++ /* go on ?? */, 
										"memchk error #1 (%s) .. module base address is outside DDR range \n", (pM->name)? pM->name:"(nil)" );
				__CHECK_IF_ERROR( CurrModuleEndAddr > DDRMemEndAddr, 		printk, ErrCnt++ /* go on ?? */, 
										"memchk error #2 (%s) .. module end  address is outside DDR range \n", (pM->name)? pM->name:"(nil)" );
				/* check if memory overlap */
				__CHECK_IF_ERROR( CurrModuleAddr 	< PrevModuleEndAddr,	printk, ErrCnt++ /* go on ?? */, 
										"memchk error #3 (%s) .. module base address overlaps with previous module\n", (pM->name)? pM->name:"(nil)" );

				pPrevM = pM;
			}
#endif

#if 1
			/* hook GPU memory info */
			if ( pM->name && !strncmp( pM->name, "GPU", 3 ) )
			{
				kdrv_gpu_pmem_base = pM->base;
				kdrv_gpu_pmem_size = pM->size;
			}
#endif
		}

		pEntry++;
		nEntries++;
	}

#if (DEVMEM_CHECK>0)
	printk("memchk error count = %d\n", ErrCnt );
//	if ( ErrCnt > 0 ) ret = RET_ERROR;
#endif

	return ret;
}

/**
 * Prints Memory Configuration.
 *
 * @param m				[IN] if NULL, then use printk, if not NULL, then use seq_prinf for proc interface.
 * @param pMemCfgEntry	[IN] LX_MEMCFG_ENTRY_T array which terminated by LX_MEMCFG_ENTRY_END
 * @param option		[IN] if 0, then prints brief output,if mask with 0x1, prints detailed output(sub-module's detailed memory).
 */
static void BASE_DEVMEM_Print(struct seq_file *m, LX_MEMCFG_ENTRY_T *pMemCfgEntry, UINT32 option)
{
	BOOLEAN detailed = option & 1;
	LX_MEMCFG_ENTRY_T	*pEntry;

	pEntry = pMemCfgEntry ? pMemCfgEntry : sMemCfgList;

//	if (m)	seq_printf(m, fmtMemCfgHeadLine);
//	else	printk(       fmtMemCfgHeadLine);

	// select Module
	while ( pEntry && pEntry->pM )
	{
		LX_MEMCFG_T	*pM = pEntry->pM;
		int			nM;
		UINT32		prvEnd = 0;
		UINT32		addr_start=0xffffffff, addr_end   = 0;	// per-module min/max.
		UINT32		module_mem_size;

		/* check if current entry is for header */
		if ( pM->name && !strncmp( pM->name, DEVMEM_HDR_STRING, strlen(DEVMEM_HDR_STRING) ) )
		{
			if (m)	seq_printf(m,fmtMemCfgHeadLine);
			else    printk(fmtMemCfgHeadLine);

			prvEnd = pM->base;
			pEntry++;

			continue;
		}

		// select allocation chunk in a module & search star/end address
		for (nM = pEntry->nM ; nM > 0; nM--,pM++) {

			if (detailed)		BASE_DEVMEM_PRINT(m,	pM, 0, prvEnd);

			if ( !pM->base )	prvEnd += pM->size;
			else				prvEnd = pM->base + pM->size;

			addr_end   = max(addr_end  , prvEnd );
			addr_start = min(addr_start, prvEnd - pM->size);
		}

		module_mem_size = addr_end - addr_start;

		if (m)
		{
			seq_printf(m, fmtEntryFormat,
							pEntry->name, addr_start, addr_end, module_mem_size,
							_MB_(addr_start     ), _KB_(addr_start     ), //_BB_(addr_start     ),
							_MB_(module_mem_size), _KB_(module_mem_size)  //_BB_(module_mem_size),
						);
			if (detailed) seq_putc(m,'\n');
		}
		else
		{
			printk(   fmtEntryFormat,
						pEntry->name, addr_start, addr_end, module_mem_size,
						_MB_(addr_start     ), _KB_(addr_start     ), //_BB_(addr_start     ),
						_MB_(module_mem_size), _KB_(module_mem_size)  //_BB_(module_mem_size),
					);

			if (detailed) printk("\n");
		}

		pEntry++;

	}	// end of while pEntry.

	return;
}

/**
 * Get total memory configuration.
 *
 * @param stat			[OUT] stat
 */
void BASE_DEVMEM_GetStat(LX_MEMCFG_STAT_T *stat)
{
	if(!stat) return;

	DBG_PRINT_ERROR("not supported\n");
}

/**
 * Sub-module Print (for LX_MEMCFG_T)
 *
 * @param m				[IN] if NULL, use printk, if not NULL, use seq_printf.
 * @param pM			[IN] pointer to print.
 * @param addr_shift	[IN] address shift value (to support FBDEV WORD_ADDR)
 * @param prvEnd		[IN] end address of previous allocation.
 */
static void BASE_DEVMEM_PRINT(struct seq_file *m, LX_MEMCFG_T *pM, int addr_shift, UINT32 prvEnd)
{
	UINT32	size= pM->size;
	UINT32	base0= pM->base;
	UINT32	base = base0;
	UINT32	end;

	if ( !base0 && prvEnd) base = prvEnd;

	end = base + size;

	if (m) seq_printf(m, fmtEntryFormatSub,
					pM->name, base, end, size,
					_MB_(base), _KB_(base), //_BB_(base),
					_MB_(size), _KB_(size)  //_BB_(size),
					);
	else	printk(		 fmtEntryFormatSub,
					pM->name, base, size, end,
					_MB_(base), _KB_(base), //_BB_(base),
					_MB_(size), _KB_(size)  //_BB_(size),
					);
	return;
}

/* { proc entry for lg/bae/memcfg */
void BASE_DEVMEM_DumpMemCfg(struct seq_file *m )
{
	BASE_DEVMEM_Print(m, NULL, 1);	// detailed dump
}

/* } end of proc entry for lg/memcfg */

/** @} */

