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

/** @file pe_reg_h15.c
 *
 *  driver for picture enhance register read, write functions. ( used only within kdriver )
 *
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2011.06.11
 *	@see
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"
#include "debug_util.h"

#include "be_cfg.h"
#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_inf_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_REG_H15_CHECK_CODE(_checker,_action,fmt,args...)		\
	if (_checker)\
	{\
		PE_PRINT_ERROR(fmt,##args);\
		_action;\
	}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
PE_CE_PE1_REG_TYPE_H15A0_T   gPE_CE_PE1_H15A0;
PE_CO_PE1_REG_TYPE_H15A0_T   gPE_CO_PE1_H15A0;
PE_HDMI_A_REG_TYPE_H15A0_T   gPE_HDMI_A_H15A0;
PE_HDMI_B_REG_TYPE_H15A0_T   gPE_HDMI_B_H15A0;
PE_HDMI_C_REG_TYPE_H15A0_T   gPE_HDMI_C_H15A0;
PE_HDMI_D_REG_TYPE_H15A0_T   gPE_HDMI_D_H15A0;
PE_CVI_A_REG_TYPE_H15A0_T    gPE_CVI_A_H15A0;
PE_CVI_B_REG_TYPE_H15A0_T    gPE_CVI_B_H15A0;
PE_CVI_C_REG_TYPE_H15A0_T    gPE_CVI_C_H15A0;
PE_CVI_D_REG_TYPE_H15A0_T    gPE_CVI_D_H15A0;
PE_LED_RP_REG_TYPE_H15A0_T   gPE_LED_RP_H15A0;
PE_MSC_Y_REG_TYPE_H15A0_T    gPE_MSC_Y_H15A0;
PE_MSC_C_REG_TYPE_H15A0_T    gPE_MSC_C_H15A0;
PE_ND_ND1_REG_TYPE_H15A0_T   gPE_ND_ND1_H15A0;
PE_ND_ND2_REG_TYPE_H15A0_T   gPE_ND_ND2_H15A0;
PE_ND_NDS_REG_TYPE_H15A0_T   gPE_ND_NDS_H15A0;
PE_ND_DNR1_REG_TYPE_H15A0_T  gPE_DNR1_H15A0;
PE_ND_DNR2_REG_TYPE_H15A0_T  gPE_DNR2_H15A0;
PE_SHP_TOP_REG_TYPE_H15A0_T  gPE_SHP_TOP_H15A0;
PE_SHPL_REG_TYPE_H15A0_T     gPE_SHPL_H15A0;
PE_SHPR_REG_TYPE_H15A0_T     gPE_SHPR_H15A0;
PE_SR_F1_REG_TYPE_H15A0_T    gPE_SR_F1_H15A0;
PE_SR_S1_REG_TYPE_H15A0_T    gPE_SR_S1_H15A0;
PE_SR_SHP_REG_TYPE_H15A0_T   gPE_SR_SHP_H15A0;
PE_SR_GSC_REG_TYPE_H15A0_T   gPE_SR_GSC_H15A0;
PE_SSC_REG_TYPE_H15A0_T      gPE_SSC_H15A0;
PE_VCP_DE_REG_TYPE_H15A0_T   gPE_VCP_DE_H15A0;
PE_VCP_DISP_REG_TYPE_H15A0_T gPE_VCP_DISP_H15A0;
PE_VCP_SO_REG_TYPE_H15A0_T   gPE_VCP_SO_H15A0;
PE_VCP_HT_REG_TYPE_H15A0_T   gPE_VCP_HT_H15A0;
PE_DDR_DB_REG_TYPE_H15_T     gPE_DDR_DB_H15;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init phy to virtual addr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_H15_InitPhyToVirt(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		/* shdw */
		gPE_CE_PE1_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_CE_PE1_REG_H15A0_T));
		gPE_CO_PE1_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_CO_PE1_REG_H15A0_T));
		gPE_HDMI_A_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_HDMI_A_REG_H15A0_T));
		gPE_HDMI_B_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_HDMI_B_REG_H15A0_T));
		gPE_HDMI_C_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_HDMI_C_REG_H15A0_T));
		gPE_HDMI_D_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_HDMI_D_REG_H15A0_T));
		gPE_CVI_A_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_CVI_REG_H15A0_T));
		gPE_CVI_B_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_CVI_REG_H15A0_T));
		gPE_CVI_C_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_CVI_REG_H15A0_T));
		gPE_CVI_D_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_CVI_REG_H15A0_T));
		gPE_LED_RP_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_LED_RP_REG_H15A0_T));
		gPE_MSC_Y_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_MSC_Y_REG_H15A0_T));
		gPE_MSC_C_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_MSC_C_REG_H15A0_T));
		gPE_ND_ND1_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_ND_ND1_REG_H15A0_T));
		gPE_ND_ND2_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_ND_ND2_REG_H15A0_T));
		gPE_ND_NDS_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_ND_NDS_REG_H15A0_T));
		gPE_DNR1_H15A0.shdw.addr     = (UINT32 *)OS_KMalloc(sizeof(PE_ND_DNR1_REG_H15A0_T));
		gPE_DNR2_H15A0.shdw.addr     = (UINT32 *)OS_KMalloc(sizeof(PE_ND_DNR2_REG_H15A0_T));
		gPE_SHP_TOP_H15A0.shdw.addr  = (UINT32 *)OS_KMalloc(sizeof(PE_SHP_TOP_REG_H15A0_T));
		gPE_SHPL_H15A0.shdw.addr     = (UINT32 *)OS_KMalloc(sizeof(PE_SHPL_REG_H15A0_T));
		gPE_SHPR_H15A0.shdw.addr     = (UINT32 *)OS_KMalloc(sizeof(PE_SHPR_REG_H15A0_T));
		gPE_SR_F1_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_SR_F1_REG_H15A0_T));
		gPE_SR_S1_H15A0.shdw.addr    = (UINT32 *)OS_KMalloc(sizeof(PE_SR_S1_REG_H15A0_T));
		gPE_SR_SHP_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_SR_SHP_REG_H15A0_T));
		gPE_SR_GSC_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_SR_GSC_REG_H15A0_T));
		gPE_SSC_H15A0.shdw.addr      = (UINT32 *)OS_KMalloc(sizeof(PE_SSC_REG_H15A0_T));
		gPE_VCP_DE_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_VCP_DE_REG_H15A0_T));
		gPE_VCP_DISP_H15A0.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(PE_VCP_DISP_REG_H15A0_T));
		gPE_VCP_SO_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_VCP_SO_REG_H15A0_T));
		gPE_VCP_HT_H15A0.shdw.addr   = (UINT32 *)OS_KMalloc(sizeof(PE_VCP_HT_REG_H15A0_T));
		gPE_DDR_DB_H15.shdw.addr     = (UINT32 *)OS_KMalloc(sizeof(PE_DDR_DB_SHDW_REG_H15_T));
		/* phys */
		gPE_CE_PE1_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_P1_CCO_E_BASE, sizeof(PE_CE_PE1_REG_H15A0_T));
		gPE_CO_PE1_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_P1_CCO_O_BASE, sizeof(PE_CO_PE1_REG_H15A0_T));
		gPE_HDMI_A_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_HDMI_A_BASE,   sizeof(PE_HDMI_A_REG_H15A0_T));
		gPE_HDMI_B_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_HDMI_B_BASE,   sizeof(PE_HDMI_B_REG_H15A0_T));
		gPE_HDMI_C_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_HDMI_C_BASE,   sizeof(PE_HDMI_C_REG_H15A0_T));
		gPE_HDMI_D_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_HDMI_D_BASE,   sizeof(PE_HDMI_D_REG_H15A0_T));
		gPE_CVI_A_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_CVI_A_BASE,    sizeof(PE_CVI_REG_H15A0_T));
		gPE_CVI_B_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_CVI_B_BASE,    sizeof(PE_CVI_REG_H15A0_T));
		gPE_CVI_C_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_CVI_C_BASE,    sizeof(PE_CVI_REG_H15A0_T));
		gPE_CVI_D_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_CVI_D_BASE,    sizeof(PE_CVI_REG_H15A0_T));
		gPE_LED_RP_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_LED_RP_BASE,   sizeof(PE_LED_RP_REG_H15A0_T));
		gPE_MSC_Y_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_MSC_Y_BASE,    sizeof(PE_MSC_Y_REG_H15A0_T));
		gPE_MSC_C_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_MSC_C_BASE,    sizeof(PE_MSC_C_REG_H15A0_T));
		gPE_ND_ND1_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_ND1_BASE,      sizeof(PE_ND_ND1_REG_H15A0_T));
		gPE_ND_ND2_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_ND2_BASE,      sizeof(PE_ND_ND2_REG_H15A0_T));
		gPE_ND_NDS_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_NDS_BASE,      sizeof(PE_ND_NDS_REG_H15A0_T));
		gPE_DNR1_H15A0.phys.addr     = (volatile UINT32 *)ioremap(PE_H15A_DNR1_BASE,     sizeof(PE_ND_DNR1_REG_H15A0_T));
		gPE_DNR2_H15A0.phys.addr     = (volatile UINT32 *)ioremap(PE_H15A_DNR2_BASE,     sizeof(PE_ND_DNR2_REG_H15A0_T));
		gPE_SHP_TOP_H15A0.phys.addr  = (volatile UINT32 *)ioremap(PE_H15A_SHP_TOP_BASE,  sizeof(PE_SHP_TOP_REG_H15A0_T));
		gPE_SHPL_H15A0.phys.addr     = (volatile UINT32 *)ioremap(PE_H15A_P1_SHP_L_BASE, sizeof(PE_SHPL_REG_H15A0_T));
		gPE_SHPR_H15A0.phys.addr     = (volatile UINT32 *)ioremap(PE_H15A_P1_SHP_R_BASE, sizeof(PE_SHPR_REG_H15A0_T));
		gPE_SR_F1_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_SR_F1_BASE,    sizeof(PE_SR_F1_REG_H15A0_T));
		gPE_SR_S1_H15A0.phys.addr    = (volatile UINT32 *)ioremap(PE_H15A_SR_S1_BASE,    sizeof(PE_SR_S1_REG_H15A0_T));
		gPE_SR_SHP_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_SR_SHP_BASE,   sizeof(PE_SR_SHP_REG_H15A0_T));
		gPE_SR_GSC_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_SR_GSC_BASE,   sizeof(PE_SR_GSC_REG_H15A0_T));
		gPE_SSC_H15A0.phys.addr      = (volatile UINT32 *)ioremap(PE_H15A_SSC_BASE,      sizeof(PE_SSC_REG_H15A0_T));
		gPE_VCP_DE_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_VCP_DE_BASE,   sizeof(PE_VCP_DE_REG_H15A0_T));
		gPE_VCP_DISP_H15A0.phys.addr = (volatile UINT32 *)ioremap(PE_H15A_VCP_DISP_BASE, sizeof(PE_VCP_DISP_REG_H15A0_T));
		gPE_VCP_SO_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_VCP_SO_BASE,   sizeof(PE_VCP_SO_REG_H15A0_T));
		gPE_VCP_HT_H15A0.phys.addr   = (volatile UINT32 *)ioremap(PE_H15A_VCP_HT_BASE,   sizeof(PE_VCP_HT_REG_H15A0_T));
	}
	else
	{
		PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
	}
	return ret;
}
/**
 * free phy to virtual addr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_H15_FreePhyToVirt(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		/* shdw */
		if (gPE_CE_PE1_H15A0.shdw.addr  ) OS_Free((void *)gPE_CE_PE1_H15A0.shdw.addr  );
		if (gPE_CO_PE1_H15A0.shdw.addr  ) OS_Free((void *)gPE_CO_PE1_H15A0.shdw.addr  );
		if (gPE_HDMI_A_H15A0.shdw.addr  ) OS_Free((void *)gPE_HDMI_A_H15A0.shdw.addr  );
		if (gPE_HDMI_B_H15A0.shdw.addr  ) OS_Free((void *)gPE_HDMI_B_H15A0.shdw.addr  );
		if (gPE_HDMI_C_H15A0.shdw.addr  ) OS_Free((void *)gPE_HDMI_C_H15A0.shdw.addr  );
		if (gPE_HDMI_D_H15A0.shdw.addr  ) OS_Free((void *)gPE_HDMI_D_H15A0.shdw.addr  );
		if (gPE_CVI_A_H15A0.shdw.addr   ) OS_Free((void *)gPE_CVI_A_H15A0.shdw.addr   );
		if (gPE_CVI_B_H15A0.shdw.addr   ) OS_Free((void *)gPE_CVI_B_H15A0.shdw.addr   );
		if (gPE_CVI_C_H15A0.shdw.addr   ) OS_Free((void *)gPE_CVI_C_H15A0.shdw.addr   );
		if (gPE_CVI_D_H15A0.shdw.addr   ) OS_Free((void *)gPE_CVI_D_H15A0.shdw.addr   );
		if (gPE_LED_RP_H15A0.shdw.addr  ) OS_Free((void *)gPE_LED_RP_H15A0.shdw.addr  );
		if (gPE_MSC_Y_H15A0.shdw.addr   ) OS_Free((void *)gPE_MSC_Y_H15A0.shdw.addr   );
		if (gPE_MSC_C_H15A0.shdw.addr   ) OS_Free((void *)gPE_MSC_C_H15A0.shdw.addr   );
		if (gPE_ND_ND1_H15A0.shdw.addr  ) OS_Free((void *)gPE_ND_ND1_H15A0.shdw.addr  );
		if (gPE_ND_ND2_H15A0.shdw.addr  ) OS_Free((void *)gPE_ND_ND2_H15A0.shdw.addr  );
		if (gPE_ND_NDS_H15A0.shdw.addr  ) OS_Free((void *)gPE_ND_NDS_H15A0.shdw.addr  );
		if (gPE_DNR1_H15A0.shdw.addr    ) OS_Free((void *)gPE_DNR1_H15A0.shdw.addr    );
		if (gPE_DNR2_H15A0.shdw.addr    ) OS_Free((void *)gPE_DNR2_H15A0.shdw.addr    );
		if (gPE_SHP_TOP_H15A0.shdw.addr ) OS_Free((void *)gPE_SHP_TOP_H15A0.shdw.addr );
		if (gPE_SHPL_H15A0.shdw.addr    ) OS_Free((void *)gPE_SHPL_H15A0.shdw.addr    );
		if (gPE_SHPR_H15A0.shdw.addr    ) OS_Free((void *)gPE_SHPR_H15A0.shdw.addr    );
		if (gPE_SR_F1_H15A0.shdw.addr   ) OS_Free((void *)gPE_SR_F1_H15A0.shdw.addr   );
		if (gPE_SR_S1_H15A0.shdw.addr   ) OS_Free((void *)gPE_SR_S1_H15A0.shdw.addr   );
		if (gPE_SR_SHP_H15A0.shdw.addr  ) OS_Free((void *)gPE_SR_SHP_H15A0.shdw.addr  );
		if (gPE_SR_GSC_H15A0.shdw.addr  ) OS_Free((void *)gPE_SR_GSC_H15A0.shdw.addr  );
		if (gPE_SSC_H15A0.shdw.addr     ) OS_Free((void *)gPE_SSC_H15A0.shdw.addr     );
		if (gPE_VCP_DE_H15A0.shdw.addr  ) OS_Free((void *)gPE_VCP_DE_H15A0.shdw.addr  );
		if (gPE_VCP_DISP_H15A0.shdw.addr) OS_Free((void *)gPE_VCP_DISP_H15A0.shdw.addr);
		if (gPE_VCP_SO_H15A0.shdw.addr  ) OS_Free((void *)gPE_VCP_SO_H15A0.shdw.addr  );
		if (gPE_VCP_HT_H15A0.shdw.addr  ) OS_Free((void *)gPE_VCP_HT_H15A0.shdw.addr  );
		if (gPE_DDR_DB_H15.shdw.addr    ) OS_Free((void *)gPE_DDR_DB_H15.shdw.addr    );
		/* phys */
		if (gPE_CE_PE1_H15A0.phys.addr  ) iounmap((void *)gPE_CE_PE1_H15A0.phys.addr  );
		if (gPE_CO_PE1_H15A0.phys.addr  ) iounmap((void *)gPE_CO_PE1_H15A0.phys.addr  );
		if (gPE_HDMI_A_H15A0.phys.addr  ) iounmap((void *)gPE_HDMI_A_H15A0.phys.addr  );
		if (gPE_HDMI_B_H15A0.phys.addr  ) iounmap((void *)gPE_HDMI_B_H15A0.phys.addr  );
		if (gPE_HDMI_C_H15A0.phys.addr  ) iounmap((void *)gPE_HDMI_C_H15A0.phys.addr  );
		if (gPE_HDMI_D_H15A0.phys.addr  ) iounmap((void *)gPE_HDMI_D_H15A0.phys.addr  );
		if (gPE_CVI_A_H15A0.phys.addr   ) iounmap((void *)gPE_CVI_A_H15A0.phys.addr   );
		if (gPE_CVI_B_H15A0.phys.addr   ) iounmap((void *)gPE_CVI_B_H15A0.phys.addr   );
		if (gPE_CVI_C_H15A0.phys.addr   ) iounmap((void *)gPE_CVI_C_H15A0.phys.addr   );
		if (gPE_CVI_D_H15A0.phys.addr   ) iounmap((void *)gPE_CVI_D_H15A0.phys.addr   );
		if (gPE_LED_RP_H15A0.phys.addr  ) iounmap((void *)gPE_LED_RP_H15A0.phys.addr  );
		if (gPE_MSC_Y_H15A0.phys.addr   ) iounmap((void *)gPE_MSC_Y_H15A0.phys.addr   );
		if (gPE_MSC_C_H15A0.phys.addr   ) iounmap((void *)gPE_MSC_C_H15A0.phys.addr   );
		if (gPE_ND_ND1_H15A0.phys.addr  ) iounmap((void *)gPE_ND_ND1_H15A0.phys.addr  );
		if (gPE_ND_ND2_H15A0.phys.addr  ) iounmap((void *)gPE_ND_ND2_H15A0.phys.addr  );
		if (gPE_ND_NDS_H15A0.phys.addr  ) iounmap((void *)gPE_ND_NDS_H15A0.phys.addr  );
		if (gPE_DNR1_H15A0.phys.addr    ) iounmap((void *)gPE_DNR1_H15A0.phys.addr    );
		if (gPE_DNR2_H15A0.phys.addr    ) iounmap((void *)gPE_DNR2_H15A0.phys.addr    );
		if (gPE_SHP_TOP_H15A0.phys.addr ) iounmap((void *)gPE_SHP_TOP_H15A0.phys.addr );
		if (gPE_SHPL_H15A0.phys.addr    ) iounmap((void *)gPE_SHPL_H15A0.phys.addr    );
		if (gPE_SHPR_H15A0.phys.addr    ) iounmap((void *)gPE_SHPR_H15A0.phys.addr    );
		if (gPE_SR_F1_H15A0.phys.addr   ) iounmap((void *)gPE_SR_F1_H15A0.phys.addr   );
		if (gPE_SR_S1_H15A0.phys.addr   ) iounmap((void *)gPE_SR_S1_H15A0.phys.addr   );
		if (gPE_SR_SHP_H15A0.phys.addr  ) iounmap((void *)gPE_SR_SHP_H15A0.phys.addr  );
		if (gPE_SR_GSC_H15A0.phys.addr  ) iounmap((void *)gPE_SR_GSC_H15A0.phys.addr  );
		if (gPE_SSC_H15A0.phys.addr     ) iounmap((void *)gPE_SSC_H15A0.phys.addr     );
		if (gPE_VCP_DE_H15A0.phys.addr  ) iounmap((void *)gPE_VCP_DE_H15A0.phys.addr  );
		if (gPE_VCP_DISP_H15A0.phys.addr) iounmap((void *)gPE_VCP_DISP_H15A0.phys.addr);
		if (gPE_VCP_SO_H15A0.phys.addr  ) iounmap((void *)gPE_VCP_SO_H15A0.phys.addr  );
		if (gPE_VCP_HT_H15A0.phys.addr  ) iounmap((void *)gPE_VCP_HT_H15A0.phys.addr  );
	}
	else
	{
		PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
	}
	return ret;
}
/**
 * write address data
 *	- use address instantly allocated
 *
 * @param   addr [in] UINT32
 * @param   value [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_H15_WD(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 currValue;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_H15)
		{
			recvAddr = addr;
			if		(recvAddr <= 0x6000) recvAddr += PE_H15A_DE_BASE;
			else if (recvAddr <= 0xffff) recvAddr += PE_H15A_DE_BASE;
			currAddr = recvAddr;
			currAddr >>= 2;
			currAddr <<= 2;

			virtAddr = (UINT32)ioremap(currAddr, 0x8);
			if (currAddr == recvAddr) {
				REG_WD(virtAddr, value);
				break;
			}
			currValue = REG_RD(virtAddr);
			nextValue = REG_RD((virtAddr+4));

			dBit = (recvAddr - currAddr)<<3;
			wBit = (32 - dBit);

			currValue  = GET_BITS(currValue ,0	  ,dBit);
			currValue += GET_PVAL(value		,dBit ,wBit);

			nextValue  = GET_PVAL(nextValue ,dBit ,wBit);
			nextValue += GET_BITS(value		,0	  ,dBit);
			REG_WD(virtAddr		,currValue);
			REG_WD((virtAddr+4) ,nextValue);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	if (virtAddr) iounmap((void *)virtAddr);
	return ret;
}

/**
 * write address data
 *	- use address already allocated
 *
 * @param   addr [in] UINT32
 * @param   value [in] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_REG_H15_WrAllocatedAddr(UINT32 addr, UINT32 value)
{
	int ret = RET_OK;
	do{
		if(PE_KDRV_VER_H15)
		{
			if((addr)>=PE_H15A_ND1_OFST && (addr)<(PE_H15A_ND1_OFST+sizeof(PE_ND_ND1_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_ND_ND1_H15A0.phys.addr+((addr-PE_H15A_ND1_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_ND2_OFST && (addr)<(PE_H15A_ND2_OFST+sizeof(PE_ND_ND2_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_ND_ND2_H15A0.phys.addr+((addr-PE_H15A_ND2_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_NDS_OFST && (addr)<(PE_H15A_NDS_OFST+sizeof(PE_ND_NDS_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_ND_NDS_H15A0.phys.addr+((addr-PE_H15A_NDS_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_DNR1_OFST && (addr)<(PE_H15A_DNR1_OFST+sizeof(PE_ND_DNR1_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_DNR1_H15A0.phys.addr+((addr-PE_H15A_DNR1_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_DNR2_OFST && (addr)<(PE_H15A_DNR2_OFST+sizeof(PE_ND_DNR2_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_DNR2_H15A0.phys.addr+((addr-PE_H15A_DNR2_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_SR_SHP_OFST && (addr)<(PE_H15A_SR_SHP_OFST+sizeof(PE_SR_SHP_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_SR_SHP_H15A0.phys.addr+((addr-PE_H15A_SR_SHP_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_SR_F1_OFST && (addr)<(PE_H15A_SR_F1_OFST+sizeof(PE_SR_F1_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_SR_F1_H15A0.phys.addr+((addr-PE_H15A_SR_F1_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_SR_S1_OFST && (addr)<(PE_H15A_SR_S1_OFST+sizeof(PE_SR_S1_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_SR_S1_H15A0.phys.addr+((addr-PE_H15A_SR_S1_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_SR_GSC_OFST && (addr)<(PE_H15A_SR_GSC_OFST+sizeof(PE_SR_GSC_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_SR_GSC_H15A0.phys.addr+((addr-PE_H15A_SR_GSC_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_P1_SHP_L_OFST && (addr)<(PE_H15A_P1_SHP_L_OFST+sizeof(PE_SHPL_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_SHPL_H15A0.phys.addr+((addr-PE_H15A_P1_SHP_L_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_P1_SHP_R_OFST && (addr)<(PE_H15A_P1_SHP_R_OFST+sizeof(PE_SHPR_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_SHPR_H15A0.phys.addr+((addr-PE_H15A_P1_SHP_R_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_MSC_Y_OFST && (addr)<(PE_H15A_MSC_Y_OFST+sizeof(PE_MSC_Y_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_MSC_Y_H15A0.phys.addr+((addr-PE_H15A_MSC_Y_OFST)/sizeof(UINT32)))) = value;
			}
			else if((addr)>=PE_H15A_MSC_C_OFST && (addr)<(PE_H15A_MSC_C_OFST+sizeof(PE_MSC_C_REG_H15A0_T)))
			{
				*((UINT32 *)(gPE_MSC_C_H15A0.phys.addr+((addr-PE_H15A_MSC_C_OFST)/sizeof(UINT32)))) = value;
			}
			else
			{
				PE_PRINT_WARN("[%x]not support.(0x%04x, 0x%08x)\n",\
					PE_CHIP_VER,(addr),(value));
			}
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * read address data
 *
 * @param   addr [in] UINT32, accessing for register
 * @return  value of register
 * @see
 * @author
 */
UINT32 PE_REG_H15_RD(UINT32 addr)
{
	UINT32 value = 0x0;
	UINT32 recvAddr;
	UINT32 currAddr;
	UINT32 nextValue;
	UINT32 dBit;
	UINT32 wBit;
	UINT32 virtAddr = 0;
	do{
		if(PE_KDRV_VER_H15)
		{
			recvAddr = addr;
			if		(recvAddr <= 0x6000) recvAddr += PE_H15A_DE_BASE;
			else if (recvAddr <= 0xffff) recvAddr += PE_H15A_DE_BASE;
			currAddr = recvAddr;
			currAddr >>= 2;
			currAddr <<= 2;
			virtAddr = (UINT32)ioremap(currAddr, 0x8);

			value = REG_RD(virtAddr);
			if (currAddr == recvAddr) break;

			nextValue = REG_RD(virtAddr+4);
			dBit = (recvAddr - currAddr)<<3;
			wBit = (32 - dBit);
			value  = GET_BITS(value, dBit, wBit);
			value += GET_PVAL(nextValue, wBit, dBit);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	}while(0);
	if (virtAddr) iounmap((void *)virtAddr);
	return value;
}

/**
 * write address data for debug
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  void
 * @see
 * @author
 */
int PE_REG_H15_WrReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr,data;
	do {
		CHECK_KNULL(pstParams);
		PE_REG_H15_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_H15)
		{
			addr = pstParams->data[0];
			data = pstParams->data[1];
			ret = PE_REG_H15_WD(addr,data);
			PE_REG_H15_CHECK_CODE(ret,break,"[%s,%d] PE_REG_H15_WD() error.\n",__F__,__L__);
			PE_PRINT_DEBUG("set : addr:0x%x, data:0x%x\n",addr,data);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	} while(0);
	return ret;
}
/**
 * read address data for debug
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  void
 * @see
 * @author
 */
int PE_REG_H15_RdReg(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	UINT32 addr;
	do{
		CHECK_KNULL(pstParams);
		PE_REG_H15_CHECK_CODE(pstParams->size!=2,break,"[%s,%d] size mismatch.\n",__F__,__L__);
		if(PE_KDRV_VER_H15)
		{
			addr = pstParams->data[0];
			pstParams->data[1] = PE_REG_H15_RD(addr);
			PE_PRINT_DEBUG("get : addr:0x%x, data:0x%x\n",addr,pstParams->data[1]);
		}
		else
		{
			PE_PRINT_INFO("nothing to do.\n");
		}
	}while(0);
	return RET_OK;
}

/**
 * set trace table
 *
 * @param   *fn [in] const char, func name
 * @param   line [in] UINT32
 * @param   *tbl [in] char, table name
 * @param   ofst [in] UINT32, offset
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author	sj.youm@lge.com
 */ 
int PE_REG_H15_SetTraceTable(const char* fn,
							UINT32 line,
							char *tbl,
							UINT32 ofst)
{
    int ret = RET_OK;
	char str[PE_TRACE_STR_SIZE];
	do{
		CHECK_KNULL(tbl);
		sprintf(str, "[%s,%d]%s(+ 0x%x)", fn, line, tbl, ofst);
		ret = PE_INF_H15_SetTraceDataBuf(str);
		PE_REG_H15_CHECK_CODE(ret, break, \
			"[%s,%d] PE_INF_H15_SetTraceDataBuf() error.\n",__F__,__L__);
	}while (0);
	return ret;
}

