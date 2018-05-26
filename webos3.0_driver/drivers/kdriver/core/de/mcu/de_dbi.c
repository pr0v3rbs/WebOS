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

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/ctype.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <asm-generic/errno-base.h>
#include <linux/string.h>
#include <linux/version.h>
#include <asm/io.h>

//#include "../../../include/common/DDI_DTV.h"
//#include "../../../include/common/DBI_DTV.h"
//#include "../../../include/common/CLI_DTV.h"

#include "de_drv.h"
#include "de_dbi.h"
#include "de_ipc_def.h"

#define DBI_PRINT(format, args...)	DBG_PRINT(  g_de_debug_fd, 0, format, ##args)

DTV_STATUS_T DBI_VIDEO_FIRWARE(int argc, char **argv);
int VIDEO_CPU_PutStr(char *msgBuff, VIDEO_IPC_DATA_TYPE_T ipcType);

#define DBI_XDE_CMD_BASE  5300
#define MCU_XDE_CMD_BASE  5500 
#define MCU_XDE_CMN_BASE  5800 

#define MCU2CMM(cmd, id) {#cmd, MCU_XDE_CMN_BASE+id, DBI_VIDEO_FIRWARE}
#define DBI2CMD(cmd, id) {#cmd, DBI_XDE_CMD_BASE+id, cmd}
#define DBI2MCU(cmd, id) {#cmd, MCU_XDE_CMD_BASE+id, DBI_VIDEO_FIRWARE}

static char sndBuff[VIDEO_MSG_STR_MAX];
DBI_CMD_T saVideoDbgList[] = {

	MCU2CMM(MR          ,13),
	MCU2CMM(MW          ,14),
	MCU2CMM(MFILL       ,15),
	MCU2CMM(RD          ,16),
	MCU2CMM(WR          ,17),
	MCU2CMM(BW          ,18),
	MCU2CMM(RC          ,19),

	DBI2MCU(MV_SetRegister				 ,1),
	DBI2MCU(MV_GetRegister				 ,2),
	DBI2MCU(MV_SetValue					 ,3),
	DBI2MCU(MV_GetValue					 ,4),
	DBI2MCU(MV_SetSrc					 ,5),
	DBI2MCU(MV_InitFrameMemory			 ,6),
	DBI2MCU(MV_PrintCurrentFrame		 ,7),
	DBI2MCU(MV_SetWinOprMode			 ,8),
	DBI2MCU(MV_GetWinOprMode			 ,9),
	DBI2MCU(MV_SetWinPath				 ,10),
	DBI2MCU(MV_GetWinPath				 ,11),
	DBI2MCU(MV_SetFormat				 ,12),
	DBI2MCU(MV_GetFormat				 ,13),
	DBI2MCU(MV_SetOswMode				 ,14),
	DBI2MCU(MV_GetOswMode				 ,15),
#ifdef USE_TEMPORAL_DEFINITION
	DBI2MCU(MV_SetCviSync				 ,16),
	DBI2MCU(MV_GetCviSync				 ,17),
#endif
	DBI2MCU(MV_SetMode3DF				 ,18),
	DBI2MCU(MV_GetMode3DF				 ,19),
	DBI2MCU(MV_SetFrameOfBlock			 ,20),
	DBI2MCU(MV_GetFrameOfBlock			 ,21),
	DBI2MCU(MV_SetModeOfECanvas			 ,22),
	DBI2MCU(MV_GetModeOfECanvas			 ,23),
	DBI2MCU(MV_SetSyncParamOfDisp		 ,24),
	DBI2MCU(MV_GetSyncParamOfDisp		 ,25),
	DBI2MCU(MV_SetSyncOfDisplay			 ,26),
	DBI2MCU(MV_SetSyncParamOfVCR		 ,27),
	DBI2MCU(MV_GetSyncParamOfVCR		 ,28),
	DBI2MCU(MV_SetSyncOfVCR				 ,29),
	DBI2MCU(MV_SetSyncParamOfPIP		 ,30),
	DBI2MCU(MV_GetSyncParamOfPIP		 ,31),
	DBI2MCU(MV_SetSyncOfPIP				 ,32),
	DBI2MCU(MV_SetFormatOfImageForDlr	 ,33),
	DBI2MCU(MV_SetModeOfProcForDlr		 ,34),
	DBI2MCU(MV_SetSizeOfImageForDlr		 ,35),
	DBI2MCU(MV_SetSyncPos				 ,36),
	DBI2MCU(MV_SetSizeOfSsrcPath		 ,37),
	DBI2MCU(MV_SetSsrcCoefByPath		 ,38),
	DBI2MCU(MV_SetSsrcPhaseByPath		 ,39),
	DBI2MCU(MV_SetSsrcFmtByPath			 ,40),
	DBI2MCU(MV_SetSyncMuxByPath			 ,41),
	DBI2MCU(MV_SetMviSyncPath			 ,42),
	DBI2MCU(MV_PrintDiag				 ,43),
	DBI2MCU(MV_DiagnosticsInit			 ,44),
	DBI2MCU(MV_SetModeIPC				 ,45),
	DBI2MCU(MV_GetModeIPC				 ,46),
	DBI2MCU(MV_SetInfoOfWCP				 ,47),
	DBI2MCU(MV_SetPattern				 ,48),
	DBI2MCU(MV_SetUartTimeOut			 ,49),
	DBI2MCU(MV_SetInterrupt				 ,50),
	DBI2MCU(MV_UpdateCoef				 ,51),
	DBI2MCU(MV_SetFrameManualMode		 ,52),
	DBI2MCU(MV_SetScenario				 ,53),
	DBI2MCU(MV_SetDebugManualMode		 ,54),
	DBI2MCU(MV_SetIndexOfFrame			 ,55),
	DBI2MCU(MV_SetTNROn					 ,56),
	DBI2MCU(MV_SetPathOfMDI				 ,57),
	DBI2MCU(MV_PrintAddrOfBlock			 ,58),
	DBI2MCU(MV_SetStepOfWCP				 ,59),
	DBI2MCU(MV_SetStepMode				 ,60),
	DBI2MCU(MV_SetDump2DDR				 ,61),
	DBI2MCU(MV_SetTraceAddrOfBlock		 ,62),
	DBI2MCU(MV_SetBitsOfMSCR			 ,63),
	DBI2MCU(MV_SetCscBypassOfCVI		 ,64),
	DBI2MCU(MV_SetAutoOffsetOfCVI		 ,65),
#ifdef USE_TEMPORAL_DEFINITION
	  DBI2MCU(MV_SetSampleWithPath		 ,66),
#endif
	DBI2MCU(MV_SetCSampleOfCVI			 ,67),
	DBI2MCU(MV_SetCSample				 ,68),
	DBI2MCU(MV_SetCSampleOfWin			 ,69),
	DBI2MCU(MV_SetViaCAP				 ,70),
	DBI2MCU(MV_SetFrameSeqOfTNR			 ,71),
	DBI2MCU(MV_SetDefinitionOfMSCR		 ,72),
	DBI2MCU(MV_SetIrqShowOn				 ,73),
	DBI2MCU(MV_SetTrigOSD				 ,74),
	DBI2MCU(MV_FillOSDBitmap			 ,75),
	DBI2MCU(MV_FillOSDColor				 ,76),
	DBI2MCU(MV_SetNumberOfFrame			 ,77),
	DBI2MCU(MV_GetNumberOfFrame			 ,78),
	DBI2MCU(MV_PrintVdec				 ,79),
	DBI2MCU(MV_SetInverserFieldFromMVI	 ,80),
	DBI2MCU(MV_SetPrintTNR				 ,81),
	DBI2MCU(MV_SetTraceAddrOfOPR		 ,82),
	DBI2MCU(MV_SetSyncModeOfVDEC		 ,83),
	DBI2MCU(MV_SetSceneOf3DF			 ,84),
	DBI2MCU(MV_SetAspecOfDisplay		 ,85),
	DBI2MCU(MV_SetZoomIn				 ,86),
	DBI2MCU(MV_SetZoomOut				 ,87),
	DBI2MCU(MV_SetTraceOfSSCR			 ,88),
	DBI2MCU(MV_PrintDvr					 ,89),
	DBI2MCU(MV_SetTraceWinSize			 ,90),
	DBI2MCU(MV_SetTraceOfInitStart		 ,91),
	DBI2MCU(MV_SetWinInfoOfPIP			 ,92),
	DBI2MCU(MV_Set10BitsByOPR			 ,93),
	DBI2MCU(MV_SetBankOfMIF				 ,94),
	DBI2MCU(MV_SetWaitingDoneOfGFX		 ,95),
	DBI2MCU(MV_SetFormatOf3DF			 ,96),
	DBI2MCU(MV_SetPrintOf3DF			 ,97),
	DBI2MCU(MV_SetPrintOfIPC			 ,98),
	DBI2MCU(MV_SetPrintToCPU			 ,99),
	DBI2MCU(MV_IPC4CPU					 ,100),
	DBI2MCU(MV_SetDeMode				 ,108),
	DBI2MCU(MV_SetOutType				 ,109),
	DBI2MCU(MV_SetOutLrOrder			 ,110),
	DBI2MCU(MV_SetIPCOn					 ,111),
	DBI2MCU(MV_SetSyncOfWCP				 ,112),
	DBI2MCU(MV_SetSyncOfFRC				 ,113),

	DBI2MCU(CVBW						 ,200),
	DBI2MCU(CVRD						 ,201),
	DBI2MCU(CVWD						 ,202),
	DBI2MCU(CVWR						 ,203),
	DBI2MCU(MV2CPU						 ,204),
	DBI2MCU(MV2URT						 ,205),
	DBI2MCU(MV3D						 ,206),
	DBI2MCU(MVBIT						 ,207),
	DBI2MCU(MVBW						 ,208),
	DBI2MCU(MVCAP						 ,209),
	DBI2MCU(MVCP						 ,210),
	DBI2MCU(MVCS						 ,211),
	DBI2MCU(MVDE						 ,212),
	DBI2MCU(MVDVR						 ,213),
	DBI2MCU(MVDVRP						 ,214),
	DBI2MCU(MVFMT						 ,215),
	DBI2MCU(MVFRI						 ,216),
	DBI2MCU(MVFRM						 ,217),
	DBI2MCU(MVIPC						 ,218),
	DBI2MCU(MVTRC						 ,219),
	DBI2MCU(MVPIP						 ,220),
	DBI2MCU(MVRC						 ,221),
	DBI2MCU(MVRD						 ,222),
	DBI2MCU(MVRP						 ,223),
	DBI2MCU(MVRV						 ,224),
	DBI2MCU(MVSRC						 ,225),
	DBI2MCU(MVTNR						 ,226),
	DBI2MCU(MVVCR						 ,227),
	DBI2MCU(MVVDECP						 ,228),
	DBI2MCU(MVWO						 ,229),
	DBI2MCU(MVWD						 ,230),
	DBI2MCU(MVWP						 ,231),
	DBI2MCU(MVWR						 ,232),
	DBI2MCU(MVZI						 ,233),
	DBI2MCU(MVZO						 ,234),
	DBI2MCU(MVPOP						 ,235),
	DBI2MCU(MVOPR						 ,236),
	DBI2MCU(MVCSC						 ,237),
	DBI2MCU(MVZL						 ,238),
	DBI2MCU(MVDF						 ,239),
	DBI2MCU(MVCA						 ,240),
	DBI2MCU(MVOIF						 ,241),
	DBI2MCU(MVWZ						 ,242),
	DBI2MCU(MVCVI						 ,243),
	DBI2MCU(MVMNT						 ,244),
	DBI2MCU(MVRPT						 ,245),
	DBI2MCU(MVWFZ						 ,246),
	DBI2MCU(MVIRQ						 ,247),
	DBI2MCU(MVCUP						 ,248),
	DBI2MCU(MVCFM						 ,249),
	DBI2MCU(MVCCS						 ,250),
	DBI2MCU(MVFRZ						 ,251),
	DBI2MCU(MVSFZ						 ,252),
	DBI2MCU(MVCRST						 ,253),
	DBI2MCU(MVWBLK						 ,254),
	DBI2MCU(MVMVT						 ,255),
	DBI2MCU(MVFLD						 ,256),
	DBI2MCU(MVWNR						 ,257),

	DBI2MCU(MVA3D						 ,258),
	DBI2MCU(MVLOW						 ,259),
	DBI2MCU(MVINF						 ,260),
	DBI2MCU(MVUD						 ,261),
	DBI2MCU(MVGCM						 ,262),
	DBI2MCU(MVUT						 ,263),
	DBI2MCU(MVBP						 ,264),
	DBI2MCU(MVMCK						 ,265),
	DBI2MCU(MVCMP						 ,266),
	DBI2MCU(MVFRD						 ,267),
	DBI2MCU(MVMCL						 ,268),
	DBI2MCU(MVMEM						 ,269),
	DBI2MCU(MVPMI						 ,270),
	DBI2MCU(MVMDC						 ,271),
	DBI2MCU(MVGRB						 ,272),
	DBI2MCU(MVINV						 ,273),
	DBI2MCU(MVPTS						 ,274),
	DBI2MCU(MVMV						 ,275),
	DBI2MCU(MVSYD						 ,276),
	DBI2MCU(MVWEB						 ,277),
	DBI2MCU(MVPWS						 ,278),
	DBI2MCU(MVFPD						 ,279),
	DBI2MCU(MVRCL						 ,280),
	DBI2MCU(MVPORT						 ,281),
	DBI2MCU(MVCNV						 ,282),
	DBI2MCU(MV3BLK						 ,283),
	DBI2MCU(MVDTR						 ,284),
	DBI2MCU(MVHALF						 ,285),
	DBI2MCU(MVPTN						 ,286),
	DBI2MCU(MVMOF						 ,287),
	DBI2MCU(MVWB						 ,288),
	DBI2MCU(MVSZ						 ,289),

	DBI2MCU(MVWRP						 ,290),
	DBI2MCU(MVQW						 ,291),
	DBI2MCU(MVGSC						 ,292),
	DBI2MCU(MVUHD						 ,293),
	DBI2MCU(MVGTD						 ,294),
	DBI2MCU(MVVDO						 ,295),
	DBI2MCU(MVNI						 ,296),
	DBI2MCU(MVNM						 ,297),
	DBI2MCU(MVCPORT						 ,298),
	DBI2MCU(MVINT						 ,299),
	DBI2MCU(MVMD						 ,300),
//	DBI2MCU(MVDW						 ,303),

	DBI2MCU(MVWV						 ,301),
	DBI2MCU(MVSCN						 ,302),
	DBI2MCU(MVUTB						 ,303),
	DBI2MCU(MVWRND						 ,304),

	DBI2MCU(MVSR						 ,350),
	DBI2MCU(MVSCN						 ,360),

	DBI2MCU(reset						 ,900)
};

BOOLEAN isXDigit(char *pString)
{
	BOOLEAN rtn		   = TRUE;
	BOOLEAN thereIsNum = FALSE;

	if ( pString ) {
		if (*pString == '-') pString++;
		if (*pString == '0') {
			pString++;
			if ((toupper(*pString) == 'X') && isxdigit(pString[1])) {
				pString++;
			} else if ((toupper(*pString) == 'U') && isdigit(pString[1])) {
				pString++;
			} else if ((toupper(*pString) == 'Z') && isxdigit(pString[1])) {
				pString++;
			}
			thereIsNum = TRUE;
		} else if ((toupper(*pString) == 'X') && isxdigit(pString[1])) {
			pString++;
			thereIsNum = TRUE;
		} else if ((toupper(*pString) == 'U') && isdigit(pString[1])) {
			pString++;
			thereIsNum = TRUE;
		} else if ((toupper(*pString) == 'Z') && isxdigit(pString[1])) {
			pString++;
			thereIsNum = TRUE;
		} else if (isdigit(*pString)) {
			pString++;
			thereIsNum = TRUE;
		}

		for ( ;*pString != '\0';++pString ) {
			if (isdigit(*pString)) {
				thereIsNum = TRUE;
			} else if (!isxdigit(*pString)) {
				rtn = FALSE;
				break;
			}
		}

//        if (thereIsNum == FALSE) rtn = FALSE;
	}

	return rtn;
}

DTV_STATUS_T VIDEO_IPC_MakeArg2Str(int argc, char **argv, char *pStr)
{
	DTV_STATUS_T ret = NOT_OK;
	int i;

	do {
		if (!argc) break;
		if (!pStr) {
			//ffprintk("pStr is Null\n");
			break;
		}
		ret = OK;
		strcpy(pStr, argv[0]);
		for (i=1;i<argc;i++) sprintf(pStr, "%s %s", pStr, argv[i]);
	} while (0);

	return ret;
}

DTV_STATUS_T DBI_VIDEO_FIRWARE(int argc, char **argv)
{
	DTV_STATUS_T		 ret;

	do {
		ret = VIDEO_IPC_MakeArg2Str(argc, argv, sndBuff);
		if (ret) break;
		ret = VIDEO_CPU_PutStr(sndBuff, 1);
	} while (0);

	return ret;
}


DTV_STATUS_T DBI_VIDEO_Execute(int argc, char **argv)
{
	DTV_STATUS_T ret = NOT_OK;
	BOOLEAN		 isNum;
	char		 *cmd;
	int i;

	cmd = argv[0];
	isNum = isXDigit(cmd);

	for(i=0;i<ARRAY_SIZE(saVideoDbgList);i++) {
		if (!isNum) {
			if (strcasecmp(cmd, saVideoDbgList[i].CmdStr)) continue;
		} else {
			if (saVideoDbgList[i].cmdId <= 0) continue;
			if(simple_strtoul(cmd ,NULL ,0) != saVideoDbgList[i].cmdId ) continue;
		}
		saVideoDbgList[i].func(argc, argv);
		ret = OK;
		break;
	}

	return ret;
}

