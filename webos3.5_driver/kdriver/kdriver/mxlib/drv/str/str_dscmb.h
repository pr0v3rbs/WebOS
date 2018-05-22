#ifndef __STR_DSCMB_H
#define __STR_DSCMB_H

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include <mst_str.h>
#include "mst_reg_bank.h"
#include "utopia.h"

#include "drvDSCMB.h"
#include "drvDSCMB_v2.h"

MS_BOOL Utopia_MApi_Dscmb_UtopiaOpen(void);
MS_BOOL Utopia_MApi_Dscmb_Init(void);
MS_BOOL Utopia_MApi_Dscmb_SetPowerState(void);
MS_BOOL Utopia_MApi_Dscmb_UtopiaClose(void);


#endif //End of  __STR_TSP_H

