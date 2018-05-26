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

/** @file pe_reg.h
 *
 *  driver header for picture enhance register read, write functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
 *	@see		
 */

#ifndef	_PE_REG_H_
#define	_PE_REG_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

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
#define PE_SET_REG_TABLE(_mdl_,_tbl,_ofs)	\
	do{\
		PE_##_mdl_##_DBG_PRINT("write %s(+ 0x%08x).\n",#_tbl,(_ofs));\
		ret = PE_REG_SetTraceTable(__F__,__L__,#_tbl,(_ofs));\
		ret = PE_REG_SetTable((_tbl),(_ofs),sizeof(_tbl));\
		PE_##_mdl_##_CHECK_CODE(ret,break,"[%s,%d] PE_REG_SetTable() error.\n",__F__,__L__);\
	}while(0)

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 *	pe register data setting type
 */
typedef struct {
	UINT32 addr;
	UINT32 data;
}
PE_REG_PARAM_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_REG_RegisterCbFunc(void);
int PE_REG_SetTraceTable(const char* fn, UINT32 line, char *tbl, UINT32 ofst);
int PE_REG_SetTable(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);
int PE_REG_GetTable(PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);
int PE_REG_InitPhyToVirt(void);
int PE_REG_FreePhyToVirt(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_REG_H_ */
