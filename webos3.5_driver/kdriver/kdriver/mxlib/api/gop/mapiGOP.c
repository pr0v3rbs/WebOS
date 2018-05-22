//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#else
#include <linux/slab.h>
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "util_symbol.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "apiGOP.h"
#include "apiGOP_v2.h"
#include "drvGOP.h"
#include "drvGFLIP.h"
#include "drvGOP_priv.h"
#include "halGOP.h"
#include "halCHIP.h"
#include "drvSYS.h"


enum
{
    GOP_POOL_ID_GOP0 = 0
} eGOPPoolID;

#ifndef UTOPIA_STRIP
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define DUMP_INFO   0
#define DEBUG_INFO(y)   //y;_GOP_GWIN_UpdateAllInfo()
#define WARNNING_INFO(y)   y

//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...
#define DBGCHECK_INFO(y)   //y
#define CRITICAL_INFO(y)   y
#define FPGA_TEST   0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define atoi(str) simple_strtoul(((str != NULL) ? str : ""), NULL, 0)
#endif

#define GetMaxActiveGwinFalse 4
#define GetMaxActiveGwinFalse_op 5
#define GetMaxActiveGwinFalse_opened 6
#define GWIN_SDRAM_NULL 0x30
#define msWarning(c)    do {} while (0)
#define XC_MAIN_WINDOW  0
#define INVALID_GOP_NUM  0xFF

// Define return values of check align
#define CHECKALIGN_SUCCESS      1
#define CHECKALIGN_FORMAT_FAIL  2
#define CHECKALIGN_PARA_FAIL    3

//#define GWIN_SDRAM_PG_UNIT 0x00000010  // 16-byte page-unit for gwin_sdram
#define GWIN_SDRAM_PG_ALIGN(_x)    ALIGN_32(_x)
#define IS_GWIN_SDRAM_ALIGN(_x)  ((_x)== GWIN_SDRAM_PG_ALIGN(_x))

static MS_BOOL bInitFWR = FALSE;
static MS_BOOL bFirstInit = FALSE;

#ifdef INSTANT_PRIVATE
#define g_pGOPCtxLocal  psGOPInstPri->g_pGOPCtxLocal
#else
MS_GOP_CTX_LOCAL *g_pGOPCtxLocal = NULL;
#endif
static GOP_TYPE_DEF GOPTYPE;

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_U32 u32XCHStart = 0;
MS_BOOL bXCInterlace = FALSE;
#endif

#define APIGOP_RetFailPri(FailType) \
               do {\
                    switch ((E_GOP_API_Result)FailType)\
                    {\
                        case GOP_API_FAIL:\
                            MS_CRITICAL_MSG(printf("\nGOP_FAIL"));\
                            break;\
                        case GOP_API_NON_ALIGN_ADDRESS:\
                            MS_CRITICAL_MSG(printf("\nGOP_NON_ALIGN_ADDRESS"));\
                            break;\
                        case GOP_API_NON_ALIGN_PITCH:\
                            MS_CRITICAL_MSG(printf("\nGOP_NON_ALIGN_PITCH"));\
                            break;\
                        case GOP_API_DEPEND_NOAVAIL:\
                            MS_CRITICAL_MSG(printf("\nGOP_DEPEND_NOAVAIL"));\
                            break;\
                        case GOP_API_MUTEX_OBTAIN_FAIL:\
                            MS_CRITICAL_MSG(printf("\nGOP_MUTEX_OBTAIN_FAIL"));\
                            break;\
                        case GOP_API_MUTEX_OBTAINED:\
                            MS_CRITICAL_MSG(printf("\nGOP_MUTEX_OBTAINED"));\
                            break;\
                        case GOP_API_INVALID_BUFF_INFO:\
                            MS_CRITICAL_MSG(printf("\nGOP_INVALID_BUFF_INFO"));\
                            break;\
                        case GOP_API_INVALID_PARAMETERS:\
                            MS_CRITICAL_MSG(printf("\nGOP_INVALID_PARAMETERS"));\
                            break;\
                        case GOP_API_FUN_NOT_SUPPORTED:\
                            MS_CRITICAL_MSG(printf("\nGOP_FUN_NOT_SUPPORTED"));\
                            break;\
                        case GOP_API_ENUM_NOT_SUPPORTED:\
                            MS_CRITICAL_MSG(printf("\nGOP_ENUM_NOT_SUPPORTED"));\
                            break;\
                        case GOP_API_CRT_GWIN_FAIL:\
                            MS_CRITICAL_MSG(printf("\nGOP_CRT_GWIN_FAIL"));\
                            break;\
                        case GOP_API_CRT_GWIN_NOAVAIL:\
                            MS_CRITICAL_MSG(printf("\nGOP_CRT_GWIN_NOAVAIL"));\
                            break;\
                        case GOP_API_SUCCESS:\
                            break;\
                        default:\
                            break;\
                    }\
                }while(0)


#define APIGOP_CHECK_INIT() \
{                            \
    if(g_pGOPCtxLocal ==NULL) \
    {                        \
        APIGOP_ASSERT(FALSE, printf("\nshould call MApi_GOP_Init first\n")); \
        return GOP_API_FAIL;\
    }                        \
}

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
/// GOP HVMirror GWIN position option define
typedef enum
{
    E_Set_HPos,
    E_Set_VPos,
    E_Set_X,
    E_Set_Y,
}HMirror_Opt;



#ifdef MSOS_TYPE_LINUX_KERNEL
static MS_U32 _fpSetFBFmt(MS_U16 u16Pitch, MS_U32 u32Address, MS_U16 u16Fmt)
{
    return 0;
}
static MS_BOOL _fpXCIsInterlace(void)
{
    return bXCInterlace;
}
static MS_U16 _fpXCGetCapHStart(void)
{
    return u32XCHStart;
}
static void _fpXCReduceBWForOSD(MS_U8 u8PQWin, MS_BOOL bOSD_on)
{
    return;
}
static void _fpEventNotify(MS_U32 u32Event, void *pArgs)
{
    return;
}
static MS_BOOL _fpXCSetDwinInfo(MS_U16 cmd, MS_U32 *arg, MS_U16 size)
{
    return FALSE;
}

static MS_U32(*fpSetFBFmt)(MS_U16, MS_U32, MS_U16) = _fpSetFBFmt;
static MS_BOOL(*fpXCIsInterlace)(void) = _fpXCIsInterlace;
static MS_U16(*fpXCGetCapHStart)(void) = _fpXCGetCapHStart;
static void (*fpXCReduceBWForOSD)(MS_U8, MS_BOOL) = _fpXCReduceBWForOSD;
static void (*fpEventNotify)(MS_U32, void *) = _fpEventNotify;
static MS_BOOL(*fpXCSetDwinInfo)(MS_U16 cmd, MS_U32 *arg, MS_U16 size) = _fpXCSetDwinInfo;
#else
static MS_U32(*fpSetFBFmt)(MS_U16, MS_U32, MS_U16) = NULL;
static MS_BOOL(*fpXCIsInterlace)(void) = NULL;
static MS_U16(*fpXCGetCapHStart)(void) = NULL;
static void (*fpXCReduceBWForOSD)(MS_U8, MS_BOOL) = NULL;
static void (*fpEventNotify)(MS_U32, void *) = NULL;
static MS_BOOL(*fpXCSetDwinInfo)(MS_U16 cmd, MS_U32 *arg, MS_U16 size) = NULL;
#endif



static MS_BOOL _GOP_IsFbIdValid(void *pInstance, MS_U32 u32FbId)
{
    if (u32FbId > DRV_MAX_GWIN_FB_SUPPORT)
        return FALSE;
    else
        return TRUE;
}


static GOP_WinFB_INFO* _GetWinFB(void *pInstance, MS_U32 FbId)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsFbIdValid(pInstance, FbId))
    {
        WARNNING_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)FbId));
        return NULL;
    }
#if WINFB_INSHARED
    //memcpy(pWinFB,&(g_pGOPCtxLocal->pGOPCtxShared->winFB[FbId]),sizeof(GOP_WinFB_INFO));
    return &(g_pGOPCtxLocal->pGOPCtxShared->winFB[FbId]);
#else
    //memcpy(pWinFB,g_pGOPCtxLocal->winFB[FbId],sizeof(GOP_WinFB_INFO));
    return &g_pGOPCtxLocal->winFB[FbId];
#endif

}

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

static MS_U8 _GOP_GetMaxGOPNum(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    return MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal);
}


static MS_BOOL _GOP_IsGopNumValid(void *pInstance, MS_U32 gop)
{
    if (gop >= _GOP_GetMaxGOPNum(pInstance))
        return FALSE;
    else
        return TRUE;
}

static MS_U8 _DumpGopByGwinId(void *pInstance, MS_U32 GwinID)
{
    MS_U8 gop0 = 0, gop1 = 1, gop2 = 2, gop3 = 3, gop4 = 4, gop5 = 5;

    MS_U8 gop_num = 0;
    MS_U8 gop_gwinnum[MAX_NUM_GOP_API_INF_SUPPORT];
    MS_U8 gop_start[MAX_NUM_GOP_API_INF_SUPPORT];
    MS_U8 gop_num_max = 0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    for (gop_num = 0; gop_num < MAX_NUM_GOP_API_INF_SUPPORT; gop_num++)
    {
        gop_start[gop_num] = 0;
        gop_gwinnum[gop_num] = 0;
    }

    gop_num_max = MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal);

    for (gop_num = 0; gop_num < gop_num_max; gop_num++)
    {
        gop_gwinnum[gop_num] = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, gop_num);
    }

    gop_start[gop0] = 0;
    for (gop_num = 1; gop_num < MAX_NUM_GOP_API_INF_SUPPORT; gop_num++)
    {
        gop_start[gop_num] = gop_start[gop_num - 1] + gop_gwinnum[gop_num - 1];
    }

    if (GwinID < gop_start[gop1])
        return gop0;
    else if (GwinID >= gop_start[gop1] && GwinID < gop_start[gop2])
        return gop1;
    else if (GwinID >= gop_start[gop2] && GwinID < gop_start[gop3])
        return gop2;
    else if (GwinID >= gop_start[gop3] && GwinID < gop_start[gop4])
        return gop3;
    else if (GwinID >= gop_start[gop4] && GwinID < gop_start[gop5])
        return gop4;
    else if (GwinID >= gop_start[gop5] && GwinID < g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum)
        return gop5;
    else
    {
        MS_ASSERT(0);
        return INVALID_GOP_NUM;
    }


}
static MS_BOOL _GOP_IsGwinIdValid(void *pInstance, MS_U8 u8GwinID)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (u8GwinID >= g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum)
        return FALSE;
    else
        return TRUE;
}


static void _GOP_GetGop0WinInfo(void *pInstance, MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    DRV_GOP_GWIN_INFO sGetInfo;
    DRV_GOPDstType Gop0Dst = E_DRV_GOP_DST_INVALID;
    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u8win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8win));
        return;
    }


    pwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u8win].u32CurFBId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : gwinMap[%d].u32CurFBId=%d\n",__FUNCTION__,__LINE__,u8win,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u8win].u32CurFBId));
        return;
    }

    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, 0, &Gop0Dst);
    if (Gop0Dst == E_DRV_GOP_DST_INVALID)
    {
        APIGOP_ASSERT(FALSE, printf("\n Get GOP destination fail!\n"));
        return;
    }
    memset(&sGetInfo, 0, sizeof(DRV_GOP_GWIN_INFO));
    MDrv_GOP_GWIN_GetGwinInfo(g_pGOPCtxLocal, u8win, &sGetInfo);
    pinfo->u16RBlkHPixSize = sGetInfo.u16RBlkHPixSize;
    pinfo->u16RBlkVPixSize = sGetInfo.u16RBlkVPixSize;
    //2009.03.14 Fix the TTX,CC position error
    sGetInfo.u16WinX = pwinFB->s_x;
    sGetInfo.u16WinY = pwinFB->s_y;

    pinfo->clrType = (EN_GOP_COLOR_TYPE)sGetInfo.clrType;

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_VMirror_VPos, u8win))
    {
        pinfo->u16DispVPixelStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVStr;
        pinfo->u16DispVPixelEnd = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVEnd;

        if (g_pGOPCtxLocal->Gwin_V_Dup)
            pinfo->u16DispVPixelEnd = (pinfo->u16DispVPixelEnd + pinfo->u16DispVPixelStart) >> 1;        // pix
    }
    else
    {
        pinfo->u16DispVPixelStart = sGetInfo.u16DispVPixelStart;              // pix
        if (g_pGOPCtxLocal->Gwin_V_Dup)
        {

            if (((Gop0Dst == E_DRV_GOP_DST_IP0) || (Gop0Dst == E_DRV_GOP_DST_IP1)) && ((fpXCIsInterlace != NULL) && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = (((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart) >> 1;
            else if ((Gop0Dst == E_DRV_GOP_DST_VE) || (Gop0Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = (((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart) >> 1;
            else
                pinfo->u16DispVPixelEnd = (sGetInfo.u16DispVPixelEnd + pinfo->u16DispVPixelStart) >> 1;        // pix
        }
        else
        {
            if (((Gop0Dst == E_DRV_GOP_DST_IP0) || (Gop0Dst == E_DRV_GOP_DST_IP1)) && ((fpXCIsInterlace != NULL) && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            else if ((Gop0Dst == E_DRV_GOP_DST_VE) || (Gop0Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            else if (((Gop0Dst == E_DRV_GOP_DST_OP0) || (Gop0Dst == E_DRV_GOP_DST_FRC) || (Gop0Dst == E_DRV_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, 0))
            {
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            }
            else
                pinfo->u16DispVPixelEnd = sGetInfo.u16DispVPixelEnd;            // pix
        }
    }

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_HMirror_HPos, u8win))
    {
        pinfo->u16DispHPixelStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHStr;
        pinfo->u16DispHPixelEnd = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHEnd;

        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (pinfo->u16DispHPixelEnd + pinfo->u16DispHPixelStart) >> 1;     // pix
    }
    else
    {
        pinfo->u16DispHPixelStart = sGetInfo.u16DispHPixelStart;               // pix
        pinfo->u16DispHPixelEnd = sGetInfo.u16DispHPixelEnd;                             // pix

        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (pinfo->u16DispHPixelEnd + pinfo->u16DispHPixelStart) >> 1;     // pix
    }

    pinfo->u16RBlkHRblkSize = sGetInfo.u16RBlkHRblkSize; //3
    pinfo->u32DRAMRBlkStart = sGetInfo.u32DRAMRBlkStart;
    pinfo->u16WinY = sGetInfo.u16WinY;  //Original Y pix of FB
    pinfo->u16WinX = sGetInfo.u16WinX;   //Original X pix of FB
}

static void _GOP_GetGop1WinInfo(void *pInstance, MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    DRV_GOP_GWIN_INFO sGetInfo;
    DRV_GOPDstType Gop1Dst = E_DRV_GOP_DST_INVALID;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u8win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8win));
        return;
    }


    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, 1, &Gop1Dst);
    if (Gop1Dst == E_DRV_GOP_DST_INVALID)
    {
        APIGOP_ASSERT(FALSE, printf("\n Get GOP destination fail!\n"));
        return;
    }

    memset(&sGetInfo, 0, sizeof(DRV_GOP_GWIN_INFO));
    MDrv_GOP_GWIN_GetGwinInfo(g_pGOPCtxLocal, u8win, &sGetInfo);
    pinfo->u16RBlkHPixSize = sGetInfo.u16RBlkHPixSize;
    pinfo->u16RBlkVPixSize = sGetInfo.u16RBlkVPixSize;
    pinfo->clrType = (EN_GOP_COLOR_TYPE)sGetInfo.clrType;

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_VMirror_VPos, u8win))
    {
        pinfo->u16DispVPixelStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVStr;
        pinfo->u16DispVPixelEnd = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVEnd;

        if (g_pGOPCtxLocal->Gwin_V_Dup)
            pinfo->u16DispVPixelEnd = (pinfo->u16DispVPixelEnd + pinfo->u16DispVPixelStart) >> 1;       // pix
    }
    else
    {
        pinfo->u16DispVPixelStart = sGetInfo.u16DispVPixelStart;            // pix

        if (g_pGOPCtxLocal->Gwin_V_Dup)
        {
            if (((Gop1Dst == E_DRV_GOP_DST_IP0) || (Gop1Dst == E_DRV_GOP_DST_IP1)) && ((fpXCIsInterlace != NULL) && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = (((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart) >> 1;
            else if ((Gop1Dst == E_DRV_GOP_DST_VE) || (Gop1Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = (((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart) >> 1;
            else
                pinfo->u16DispVPixelEnd = (sGetInfo.u16DispVPixelEnd + pinfo->u16DispVPixelStart) >> 1;        // pix
        }
        else
        {
            if (((Gop1Dst == E_DRV_GOP_DST_IP0) || (Gop1Dst == E_DRV_GOP_DST_IP1)) && ((fpXCIsInterlace != NULL) && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            else if ((Gop1Dst == E_DRV_GOP_DST_VE) || (Gop1Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            else if (((Gop1Dst == E_DRV_GOP_DST_OP0) || (Gop1Dst == E_DRV_GOP_DST_FRC) || (Gop1Dst == E_DRV_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, 1))
            {
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            }
            else
                pinfo->u16DispVPixelEnd = sGetInfo.u16DispVPixelEnd;            // pix
        }
    }

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_HMirror_HPos, u8win))
    {
        pinfo->u16DispHPixelStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHStr;
        pinfo->u16DispHPixelEnd = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHEnd;

        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (pinfo->u16DispHPixelEnd + pinfo->u16DispHPixelStart) >> 1;   // pix
    }
    else
    {
        pinfo->u16DispHPixelStart = sGetInfo.u16DispHPixelStart;  // pix

        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (sGetInfo.u16DispHPixelEnd + pinfo->u16DispHPixelStart) >> 1;     // pix
        else
            pinfo->u16DispHPixelEnd = sGetInfo.u16DispHPixelEnd;    // pix
    }

    pinfo->u16RBlkHRblkSize = sGetInfo.u16RBlkHRblkSize;
    pinfo->u32DRAMRBlkStart = sGetInfo.u32DRAMRBlkStart;

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_HMirrorRBLK_Adr, u8win) ||
        HAS_BIT(g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr, u8win))
    {
        pinfo->u32DRAMRBlkStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u32NonMirrorFBAdr;
    }
}

static void _GOP_GetGop23WinInfo(void *pInstance, MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    DRV_GOP_GWIN_INFO sGetInfo;
    DRV_GOPDstType Gop23Dst = E_DRV_GOP_DST_INVALID;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u8win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8win));
        return;
    }


    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), &Gop23Dst);
    if (Gop23Dst == E_DRV_GOP_DST_INVALID)
    {
        APIGOP_ASSERT(FALSE, printf("\n Get GOP destination fail!\n"));
        return;
    }

    memset(&sGetInfo, 0, sizeof(DRV_GOP_GWIN_INFO));
    MDrv_GOP_GWIN_GetGwinInfo(g_pGOPCtxLocal, u8win, &sGetInfo);
    pinfo->u16RBlkHPixSize = sGetInfo.u16RBlkHPixSize;
    pinfo->u16RBlkVPixSize = sGetInfo.u16RBlkVPixSize;
    pinfo->clrType = (EN_GOP_COLOR_TYPE)sGetInfo.clrType;

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_VMirror_VPos, u8win))
    {
        pinfo->u16DispVPixelStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVStr;
        pinfo->u16DispVPixelEnd = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVEnd;

        if (g_pGOPCtxLocal->Gwin_V_Dup)
            pinfo->u16DispVPixelEnd = (pinfo->u16DispVPixelEnd + pinfo->u16DispVPixelStart) >> 1;       // pix
    }
    else
    {
        pinfo->u16DispVPixelStart = sGetInfo.u16DispVPixelStart;            // pix

        if (g_pGOPCtxLocal->Gwin_V_Dup)
        {
            if (((Gop23Dst == E_DRV_GOP_DST_IP0) || (Gop23Dst == E_DRV_GOP_DST_IP1)) && ((fpXCIsInterlace != NULL) && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = (((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart) >> 1;
            else if ((Gop23Dst == E_DRV_GOP_DST_VE) || (Gop23Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = (((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart) >> 1;
            else
                pinfo->u16DispVPixelEnd = (sGetInfo.u16DispVPixelEnd + pinfo->u16DispVPixelStart) >> 1;        // pix
        }
        else
        {
            if (((Gop23Dst == E_DRV_GOP_DST_IP0) || (Gop23Dst == E_DRV_GOP_DST_IP1)) && ((fpXCIsInterlace != NULL) && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            else if ((Gop23Dst == E_DRV_GOP_DST_VE) || (Gop23Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            else if (((Gop23Dst == E_DRV_GOP_DST_OP0) || (Gop23Dst == E_DRV_GOP_DST_FRC) || (Gop23Dst == E_DRV_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win)))
            {
                pinfo->u16DispVPixelEnd = ((sGetInfo.u16DispVPixelEnd - pinfo->u16DispVPixelStart) << 1) + pinfo->u16DispVPixelStart;
            }
            else
                pinfo->u16DispVPixelEnd = sGetInfo.u16DispVPixelEnd;            // pix
        }
    }

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_HMirror_HPos, u8win))
    {
        pinfo->u16DispHPixelStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHStr;
        pinfo->u16DispHPixelEnd = g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHEnd;

        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (pinfo->u16DispHPixelEnd + pinfo->u16DispHPixelStart) >> 1;   // pix
    }
    else
    {
        pinfo->u16DispHPixelStart = sGetInfo.u16DispHPixelStart;  // pix

        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (sGetInfo.u16DispHPixelEnd + pinfo->u16DispHPixelStart) >> 1;     // pix
        else
            pinfo->u16DispHPixelEnd = sGetInfo.u16DispHPixelEnd;    // pix
    }

    pinfo->u16RBlkHRblkSize = sGetInfo.u16RBlkHRblkSize;
    pinfo->u32DRAMRBlkStart = sGetInfo.u32DRAMRBlkStart;

    if (HAS_BIT(g_pGOPCtxLocal->u16GOP_HMirrorRBLK_Adr, u8win) ||
        HAS_BIT(g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr, u8win))
    {
        pinfo->u32DRAMRBlkStart = g_pGOPCtxLocal->sMirrorInfo[u8win].u32NonMirrorFBAdr;
    }
}




MS_U8 _GOP_SelGwinId2(void *pInstance, MS_U32 u32GOP, MS_U32 u8GwinIdx)
{
    MS_U8 tmp;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u32GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32GOP));
        return GWIN_NO_AVAILABLE;
    }
    tmp = MDrv_GOP_SelGwinIdByGOP(g_pGOPCtxLocal, u32GOP, u8GwinIdx);

    return tmp;

}

static E_GOP_API_Result _GOP_MAP_MUX_Enum(void *pInstance, EN_Gop_MuxSel GOPMux, Gop_MuxSel *pGOPMux)
{
    E_GOP_API_Result ret = GOP_API_FAIL;

    switch (GOPMux)
    {
    case EN_GOP_MUX0:
        *pGOPMux = E_GOP_MUX0;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_MUX1:
        *pGOPMux = E_GOP_MUX1;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_MUX2:
        *pGOPMux = E_GOP_MUX2;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_MUX3:
        *pGOPMux = E_GOP_MUX3;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_MUX4:
        *pGOPMux = E_GOP_MUX4;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_IP0_MUX:
        *pGOPMux = E_GOP_IP0_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_IP1_MUX:
        *pGOPMux = E_GOP_IP1_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_VOP0_MUX:
        *pGOPMux = E_GOP_VOP0_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_VOP1_MUX:
        *pGOPMux = E_GOP_VOP1_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_FRC_MUX0:
        *pGOPMux = E_GOP_FRC_MUX0;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_FRC_MUX1:
        *pGOPMux = E_GOP_FRC_MUX1;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_FRC_MUX2:
        *pGOPMux = E_GOP_FRC_MUX2;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_FRC_MUX3:
        *pGOPMux = E_GOP_FRC_MUX3;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_BYPASS_MUX0:
        *pGOPMux = E_GOP_BYPASS_MUX0;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_IP0_SUB_MUX:
        *pGOPMux = E_GOP_IP0_SUB_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_VOP0_SUB_MUX:
        *pGOPMux = E_GOP_VOP0_SUB_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_OP1_MUX:
        *pGOPMux = E_GOP_OP1_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_DIP_MUX:
        *pGOPMux = E_GOP_DIP_MUX;
        ret = GOP_API_SUCCESS;
        break;
    case EN_GOP_GS_MUX:
        *pGOPMux = E_GOP_GS_MUX;
        ret = GOP_API_SUCCESS;
        break;
    default:
        *pGOPMux = E_GOP_MUX_INVAILD;
        MS_CRITICAL_MSG(printf("\n MAP GOP Mux error!! \n"));
        ret = GOP_API_FAIL;
        break;
    }
    return ret;
}
static MS_U32 _GOP_Map_APIDst2DRV_Enum(void *pInstance, EN_GOP_DST_TYPE GopDst, DRV_GOPDstType *pGopDst)
{
    MS_U32 ret = GOP_API_FAIL;

    switch (GopDst)
    {
    case E_GOP_DST_IP0:
        *pGopDst = E_DRV_GOP_DST_IP0;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_MIXER2VE:
        *pGopDst = E_DRV_GOP_DST_MIXER2VE;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_OP0:
        *pGopDst = E_DRV_GOP_DST_OP0;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_VOP:
        *pGopDst = E_DRV_GOP_DST_VOP;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_IP1:
        *pGopDst = E_DRV_GOP_DST_IP1;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_IP_MAIN:
        *pGopDst = E_DRV_GOP_DST_IP0; //For compatible temporaily
        MS_CRITICAL_MSG(printf("\n %s, E_GOP_DST_IP_MAIN is going to be removed, please choose E_GOP_DST_IP0!! \n", __FUNCTION__));
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_IP_SUB:            //For compatible temporaily
        *pGopDst = E_DRV_GOP_DST_IP0_SUB;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_MIXER2OP:
        *pGopDst = E_DRV_GOP_DST_MIXER2OP;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_VOP_SUB:
        *pGopDst = E_DRV_GOP_DST_VOP_SUB;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_FRC:
        *pGopDst = E_DRV_GOP_DST_FRC;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_VE:
        *pGopDst = E_DRV_GOP_DST_VE;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_BYPASS:
        *pGopDst = E_DRV_GOP_DST_BYPASS;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_OP1:
        *pGopDst = E_DRV_GOP_DST_OP1;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_MIXER2OP1:
        *pGopDst = E_DRV_GOP_DST_MIXER2OP1;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_DIP:
        *pGopDst = E_DRV_GOP_DST_DIP;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_GOP_DST_GOPScaling:
        *pGopDst = E_DRV_GOP_DST_GOPScaling;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    default:
        *pGopDst = E_DRV_GOP_DST_INVALID;
        MS_CRITICAL_MSG(printf("\n MAP GOP Dst plane error!! \n"));
        ret = (MS_U32)GOP_API_FAIL;
        break;
    }
    return ret;
}

static MS_U32 _GOP_Map_DRVDst2API_Enum_(void *pInstance, EN_GOP_DST_TYPE *pGopDst, DRV_GOPDstType GopDst)
{
    MS_U32 ret = 0;

    switch (GopDst)
    {
    case E_DRV_GOP_DST_IP0:
        *pGopDst = E_GOP_DST_IP0;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_MIXER2VE:
        *pGopDst = E_GOP_DST_MIXER2VE;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_OP0:
        *pGopDst = E_GOP_DST_OP0;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_VOP:
        *pGopDst = E_GOP_DST_VOP;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_IP1:
        *pGopDst = E_GOP_DST_IP1;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_IP0_SUB:            //For compatible temporaily
        *pGopDst = E_GOP_DST_IP_SUB;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_MIXER2OP:
        *pGopDst = E_GOP_DST_MIXER2OP;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_VOP_SUB:
        *pGopDst = E_GOP_DST_VOP_SUB;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_FRC:
        *pGopDst = E_GOP_DST_FRC;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_VE:
        *pGopDst = E_GOP_DST_VE;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_BYPASS:
        *pGopDst = E_GOP_DST_BYPASS;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_OP1:
        *pGopDst = E_GOP_DST_OP1;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_MIXER2OP1:
        *pGopDst = E_GOP_DST_MIXER2OP1;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_DIP:
        *pGopDst = E_GOP_DST_DIP;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    case E_DRV_GOP_DST_GOPScaling:
        *pGopDst = E_GOP_DST_GOPScaling;
        ret = (MS_U32)GOP_API_SUCCESS;
        break;
    default:
        *pGopDst = (EN_GOP_DST_TYPE)0xff;
        MS_CRITICAL_MSG(printf("\n MAP GOP Dst plane error!! \n"));
        ret = (MS_U32)GOP_API_FAIL;
        break;
    }
    return ret;
}

static MS_U32 _GOP_CalcPitch(void *pInstance, MS_U8 fbFmt, MS_U16 width)
{
    MS_U16 pitch = 0;

    switch (fbFmt)
    {
    case E_MS_FMT_I1 :
        pitch = (width) >> 3;
        break;
    case E_MS_FMT_I2 :
        pitch = (width << 1) >> 3;
        break;
    case E_MS_FMT_I4 :
        pitch = (width << 2) >> 3;
        break;
    case E_MS_FMT_I8 :
        pitch = width;
        break;
    case E_MS_FMT_RGB565 :
    case E_MS_FMT_BGR565 :
    case E_MS_FMT_ARGB1555 :
    case E_MS_FMT_ABGR1555 :
    case E_MS_FMT_BGRA5551 :
    case E_MS_FMT_RGBA5551 :
    case E_MS_FMT_ARGB4444 :
    case E_MS_FMT_RGBA4444 :
    case E_MS_FMT_ABGR4444 :
    case E_MS_FMT_BGRA4444 :
    case E_MS_FMT_1ABFgBg12355:
    case E_MS_FMT_FaBaFgBg2266:
    case E_MS_FMT_YUV422:
    case E_MS_FMT_ARGB1555_DST :
        pitch = width << 1;
        break;
    case E_MS_FMT_AYUV8888 :
    case E_MS_FMT_ARGB8888 :
    case E_MS_FMT_RGBA8888 :
    case E_MS_FMT_BGRA8888:
    case E_MS_FMT_ABGR8888 :
        pitch = width << 2;
        break;
    default :
        //print err
        pitch = 0;
        break;
    }
    return pitch;
}

static MS_U16 _GOP_GetBPP(void *pInstance, EN_GOP_COLOR_TYPE fbFmt)
{
    MS_U16 bpp = 0;

    switch (fbFmt)
    {
    case E_GOP_COLOR_RGB555_BLINK :
    case E_GOP_COLOR_RGB565 :
    case E_GOP_COLOR_BGR565 :
    case E_GOP_COLOR_ARGB1555:
    case E_GOP_COLOR_ABGR1555:
    case E_GOP_COLOR_ARGB4444 :
    case E_GOP_COLOR_RGBA4444 :
    case E_GOP_COLOR_ABGR4444 :
    case E_GOP_COLOR_BGRA4444 :
    case E_GOP_COLOR_RGB555YUV422:
    case E_GOP_COLOR_YUV422:
    case E_GOP_COLOR_2266:
    case E_GOP_COLOR_RGBA5551:
    case E_GOP_COLOR_BGRA5551:
        bpp = 16;
        break;
    case E_GOP_COLOR_AYUV8888 :
    case E_GOP_COLOR_ARGB8888 :
    case E_GOP_COLOR_ABGR8888 :
    case E_GOP_COLOR_RGBA8888 :
    case E_GOP_COLOR_BGRA8888 :
        bpp = 32;
        break;

    case E_GOP_COLOR_I8 :
        bpp = 8;
        break;
    default :
        //print err
        //__ASSERT(0);
        bpp = 0xFFFF;
        break;
    }
    return bpp;

}

static MS_U8 _GOP_GWIN_IsGwinCreated(void *pInstance, MS_U8 gId)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, gId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, gId));
        return FALSE;
    }

    if ((gId < g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum)
        && (GWIN_ID_INVALID == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId))
    {
        return FALSE;
    }

    DEBUG_INFO(printf("IsGwinCreated Debug: gId=%d\n", gId));
    return TRUE;
}


static MS_BOOL  bCursorSupport(void *pInstance, MS_U8 u8GOP)
{
#if CURSOR_SUPPORT
    //if cursor GOP, dont need to do adjust
    if (u8GOP == 2)
        return TRUE;
    else
        return FALSE;
#else
    return FALSE;
#endif
}

// Alignment stretch window value
static void _GOP_GWIN_Align_StretchWin(void *pInstance, MS_U8 u8GOP, EN_GOP_DST_TYPE eDstType, MS_U16 *pu16x, MS_U16 *pu16y, MS_U16 *pu16Width, MS_U16 *pu16height, MS_U16 BPP)
{
    MS_U16 u16align_offset;
    MS_U16 u16GOP_Unit;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    u16GOP_Unit = MDrv_GOP_GetWordUnit(g_pGOPCtxLocal, MDrv_GOP_Get(g_pGOPCtxLocal));

    if ((eDstType == E_GOP_DST_IP0) || (eDstType == E_GOP_DST_IP1))
    {
        if ((fpXCIsInterlace != NULL) && (fpXCIsInterlace()))
        {
            *pu16height /= 2;
            *pu16y = *pu16y / 2;
        }
    }
    else if (eDstType == E_GOP_DST_VE)
    {
        *pu16height /= 2;
        *pu16y = *pu16y / 2 + g_pGOPCtxLocal->pGopChipProperty->GOP_VE_V_Offset;
    }
    else if (eDstType == E_GOP_DST_OP1)
    {
        *pu16height /= 2;
        *pu16y = *pu16y / 2;
    }
    else if (((eDstType == E_GOP_DST_OP0) || (eDstType == E_GOP_DST_FRC) || (eDstType == E_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, u8GOP))
    {
        *pu16height /= 2;
        *pu16y = *pu16y / 2;
    }


    if (u16GOP_Unit == 1)
    {
        *pu16Width = ((*pu16Width + 1) >> 1) << 1; //do 2 pixel align
    }
    else
    {
        u16align_offset = (u16GOP_Unit * 8 / BPP - 1);
        *pu16Width = (*pu16Width + u16align_offset) & (~u16align_offset); //do 8-byte align
    }
}

//Adjust stretch window for GOP 3D
static void _GOP_GWIN_Adjust_3DStretchWin(void *pInstance, MS_U8 u8Gop, MS_U16 *pu16x, MS_U16 *pu16y, MS_U16 *pu16Width, MS_U16 *pu16Height)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (g_pGOPCtxLocal->pGopChipProperty->enGOP3DType == E_DRV_3D_DUP_HALF)
    {
        if ((g_pGOPCtxLocal->pGOPCtxShared->GOP_StereoMode[u8Gop] == E_DRV_GOP_3D_LINE_ALTERNATIVE) ||
            (g_pGOPCtxLocal->pGOPCtxShared->GOP_StereoMode[u8Gop] == E_DRV_GOP_3D_TOP_BOTTOM)
           )
        {
            *pu16Height = *pu16Height << 1;
            //Line by line, do not need middle
            if (g_pGOPCtxLocal->pGOPCtxShared->GOP_StereoMode[u8Gop] == E_DRV_GOP_3D_TOP_BOTTOM)
            {
                *pu16Height += g_pGOPCtxLocal->u16GOPGWinMiddle[u8Gop]; //Stretch win includes middle
            }
        }
        else if (g_pGOPCtxLocal->pGOPCtxShared->GOP_StereoMode[u8Gop] == E_DRV_GOP_3D_LINE_FRAMEPACKING)
        {
            //Frame packing, no middle, always full screen
            //Adjust Gap only
            *pu16Height = (*pu16Height << 1) + g_pGOPCtxLocal->GOP_FramePacking_Gap;
        }
        else if (g_pGOPCtxLocal->pGOPCtxShared->GOP_StereoMode[u8Gop] == E_DRV_GOP_3D_SIDE_BY_SYDE)
        {
            *pu16Width = *pu16Width << 1;
            *pu16Width += g_pGOPCtxLocal->u16GOPGWinMiddle[u8Gop]; //Stretch win includes middle
        }
    }
}

static void _GOP_GWIN_SetHVMirrorWinPos(void *pInstance, MS_U8 u8GwinID, HMirror_Opt opt, GOP_GwinInfo *pGWINInfo)
{
    MS_U16 u16GWINWidth = 0, u16GWINHeight = 0;
    MS_S16 s16MirrorHStr = -1, s16MirrorVStr = -1;
    MS_U8 u8Gop = INVALID_GOP_NUM;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u8GwinID))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8GwinID));
        return;
    }
    u8Gop = _DumpGopByGwinId(pInstance, u8GwinID);
    u16GWINWidth = pGWINInfo->u16DispHPixelEnd - pGWINInfo->u16DispHPixelStart;
    u16GWINHeight = pGWINInfo->u16DispVPixelEnd - pGWINInfo->u16DispVPixelStart;

    switch (opt)
    {
    case E_Set_HPos:
        {
            pGWINInfo->u16DispHPixelEnd = g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchWidth[u8Gop] - pGWINInfo->u16DispHPixelStart;
            s16MirrorHStr = pGWINInfo->u16DispHPixelEnd - u16GWINWidth;
            if (s16MirrorHStr < 0)
            {
                pGWINInfo->u16DispHPixelStart = 0;
                pGWINInfo->u16DispHPixelEnd = u16GWINWidth;
            }
            else
            {
                pGWINInfo->u16DispHPixelStart = s16MirrorHStr;
            }
            SET_BIT(g_pGOPCtxLocal->u16GOP_HMirror_HPos, u8GwinID);
            break;
        }
    case E_Set_VPos:
        {
            pGWINInfo->u16DispVPixelEnd = g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchHeight[u8Gop] - pGWINInfo->u16DispVPixelStart;
            s16MirrorVStr = pGWINInfo->u16DispVPixelEnd - u16GWINHeight;
            if (s16MirrorVStr < 0)
            {
                pGWINInfo->u16DispVPixelStart = 0;
                pGWINInfo->u16DispVPixelEnd = u16GWINHeight;
            }
            else
            {
                pGWINInfo->u16DispVPixelStart = s16MirrorVStr;
            }
            SET_BIT(g_pGOPCtxLocal->u16GOP_VMirror_VPos, u8GwinID);
            break;
        }
    case E_Set_X:
        {
            if (u8Gop == GOPTYPE.GOP0)
            {
#if ENABLE_GOP0_RBLK_MIRROR
                MS_U16 bpp = MDrv_GOP_GetBPP(g_pGOPCtxLocal, (DRV_GOPColorType)pGWINInfo->clrType);
                MS_U16 u16HPixelSize;
                if ((bpp != 0) && (bpp != 0xFFFF))
                {
                    u16HPixelSize = pGWINInfo->u16RBlkHRblkSize / (bpp >> 3);
                }
                else
                {
                    u16HPixelSize = pGWINInfo->u16RBlkHPixSize;
                }
                if ((u16HPixelSize != 0))
                {
                    pGWINInfo->u16WinX = u16HPixelSize - ((pGWINInfo->u16WinX + pGWINInfo->u16DispHPixelEnd - pGWINInfo->u16DispHPixelStart) % u16HPixelSize);
                    pGWINInfo->u16WinX %= u16HPixelSize;
                }
                else
#endif //ENABLE_GOP0_RBLK_MIRROR
                {
                    pGWINInfo->u16WinX = 0;
                }
            }
            break;
        }
    case E_Set_Y:
        {
            if (u8Gop == GOPTYPE.GOP0)
            {
#if ENABLE_GOP0_RBLK_MIRROR
                if (pGWINInfo->u16RBlkVPixSize != 0)
                {
                    pGWINInfo->u16WinY = (pGWINInfo->u16WinY + (pGWINInfo->u16DispVPixelEnd - pGWINInfo->u16DispVPixelStart - 1)) % pGWINInfo->u16RBlkVPixSize;
                }
                else
#endif//ENABLE_GOP0_RBLK_MIRROR
                {
                    pGWINInfo->u16WinY = (pGWINInfo->u16DispVPixelEnd - pGWINInfo->u16DispVPixelStart - 1);
                }
            }
            break;
        }
    default:
        {
            //ASSERT(0);
            break;
        }
    }
}


static MS_U8 _GOP_GWIN_IsGwinExistInClient(void *pInstance, MS_U8 gId)
{
    MS_BOOL result0, result1, result2, result3, result4;
    MS_U32 MaxGwinSupport;

    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, gId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, gId));
        return FALSE;
    }

    MaxGwinSupport = g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum;

    pwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : gwinMap[%d].u32CurFBId=%ld\n", __FUNCTION__, __LINE__, gId, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId));
        return FALSE;
    }

    result0 = (MaxGwinSupport <= gId);
    result1 = (FALSE == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].bIsShared);
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
    MS_U32 pid = getpid();
    result2 = (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32GOPClientId != pid);
#else
    result2 = (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32GOPClientId != g_pGOPCtxLocal->u32GOPClientId);
#endif
    result3 = (DRV_MAX_GWIN_FB_SUPPORT <= g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId);
    result4 = (0 == pwinFB->in_use);

    if (result0 || (result1 && (result2 || result3 || result4)))
    {
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
        WARNNING_INFO(printf("result0 = %d,result1 = %d,result2 = %d,result3 = %d,result4 = %d\n", result0, result1, result2, result3, result4));
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is not exist\n", __FUNCTION__, __LINE__, gId));
#else
        DEBUG_INFO(printf("IsGwinExistInClient Debug: gId=%d, \n \
                           gwinMap[gId].u32GOPClientId:=%ld,\n \
                           _pGOPCtxLocal->u32GOPClientId=%ld, \n \
                           gwinMap[gId].u32CurFBId=%ld,\n \
                           gwinMap[gId].u32CurFBId].in_use=%d\n", gId,\
                          g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32GOPClientId,  g_pGOPCtxLocal->u32GOPClientId,\
                          g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId,\
                          pwinFB->in_use));
#endif
        return FALSE;
    }
    return TRUE;

}



static void _GOP_GWIN_IsEnableMirror(void *pInstance, DRV_GOPDstType eGopDst, MS_BOOL *pbHMirror, MS_BOOL *pbVMirror)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if ((pbHMirror == NULL) || (pbVMirror == NULL))
    {
        return;
    }

    if ((eGopDst == E_DRV_GOP_DST_OP0)
        || (eGopDst == E_DRV_GOP_DST_BYPASS)
        || (eGopDst == E_DRV_GOP_DST_FRC)
        || (((eGopDst == E_DRV_GOP_DST_IP0) || (eGopDst == E_DRV_GOP_DST_IP0_SUB))
            && (E_GOP_VIDEOTIMING_MIRROR_BYMVOP == Mdrv_GOP_GetVideoTimingMirrorType(g_pGOPCtxLocal, TRUE)))
       )
    {
        *pbHMirror = g_pGOPCtxLocal->pGOPCtxShared->bHMirror;
        *pbVMirror = g_pGOPCtxLocal->pGOPCtxShared->bVMirror;
    }
    else
    {
#if (FPGA_TEST)  //FPGA use VOP path to verify mirror.
        *pbHMirror = g_pGOPCtxLocal->pGOPCtxShared->bHMirror;
        *pbVMirror = g_pGOPCtxLocal->pGOPCtxShared->bVMirror;
#else
        *pbHMirror = FALSE;
        *pbVMirror = FALSE;
#endif
    }
}

static MS_BOOL _GetGOPEnum(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    memset(&GOPTYPE, 0xff, sizeof(GOP_TYPE_DEF));
    return MDrv_GOP_GetGOPEnum(g_pGOPCtxLocal, &GOPTYPE);
}


static MS_BOOL _GOP_Init_Ctx(void *pInstance, MS_BOOL *pbFirstDrvInstant)
{
    MS_BOOL bNeedInitShared = FALSE;
    MS_U16 u16LoopTime = 0x0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    _GetGOPEnum(pInstance);

    if (g_pGOPCtxLocal)
    {
        if (bFirstInit)
        {
            *pbFirstDrvInstant = TRUE;
        }
        else
        {
            *pbFirstDrvInstant = FALSE;
        }

        return TRUE;
    }
    g_pGOPCtxLocal = Drv_GOP_Init_Context(pInstance, &bNeedInitShared);
    if (NULL == g_pGOPCtxLocal)
    {
        printf("Error : g_pGOPCtxLocal = NULL\n");
        return FALSE;
    }
    g_pGOPCtxLocal->pGOPCtxShared->bInitShared = bNeedInitShared;

    if (pbFirstDrvInstant)
        *pbFirstDrvInstant = bNeedInitShared;


#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
    g_pGOPCtxLocal->u32GOPClientId = getpid();
#else
    g_pGOPCtxLocal->u32GOPClientId = ++g_pGOPCtxLocal->pGOPCtxShared->u32ClientIdAllocator;
    if(0 == g_pGOPCtxLocal->u32GOPClientId)
    g_pGOPCtxLocal->u32GOPClientId = ++g_pGOPCtxLocal->pGOPCtxShared->u32ClientIdAllocator;
#endif

    g_pGOPCtxLocal->sMirrorInfo = NULL;
    u16LoopTime = 0;
    while (u16LoopTime < 0xFF)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        if (g_pGOPCtxLocal->sMirrorInfo == NULL)
            g_pGOPCtxLocal->sMirrorInfo = kmalloc(g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum * sizeof(GOP_GwinMirror_Info), GFP_KERNEL);
#else
        if (g_pGOPCtxLocal->sMirrorInfo == NULL)
            g_pGOPCtxLocal->sMirrorInfo = malloc(g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum * sizeof(GOP_GwinMirror_Info));
#endif

        if (g_pGOPCtxLocal->sMirrorInfo != NULL)
        {
            break;
        }
        u16LoopTime++;

    }

    if (g_pGOPCtxLocal->sMirrorInfo == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nallocate memory for gwin mirror info fail!!!"));
    }
    else
    {
        memset(g_pGOPCtxLocal->sMirrorInfo, 0, g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum * sizeof(GOP_GwinMirror_Info));
    }

    memset(&g_pGOPCtxLocal->sGOPConfig, 0, sizeof(GOP_Config));

    memset(g_pGOPCtxLocal->MS_MuxGop, 0, MAX_GOP_MUX_SUPPORT * sizeof(MS_U8));
    g_pGOPCtxLocal->IsChgMux = FALSE;
    g_pGOPCtxLocal->IsClkClosed = FALSE;
    g_pGOPCtxLocal->u8ChgIpMuxGop = 0xFF;
#if GOP_LOCK_SUPPORT
    char GOP_MUTEX[] = "GOP_Mutex";

    g_pGOPCtxLocal->s32GOPMutexCnt = 0;
    g_pGOPCtxLocal->s32GOPMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)GOP_MUTEX, MSOS_PROCESS_SHARED);
    if (0 > g_pGOPCtxLocal->s32GOPMutex)
    {
        MS_CRITICAL_MSG(printf("%screate gop mutex fail\n", __FUNCTION__));
        return FALSE;
    }
    g_pGOPCtxLocal->s32GOPLock = -1;
#endif

#if GOP_XC_LOCK_SUPPORT
    char XC_MUTEX[] = "_XC_Mutex";

    g_pGOPCtxLocal->s32GOPXC_MutexCnt = 0;
    g_pGOPCtxLocal->s32GOPXC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)XC_MUTEX, MSOS_PROCESS_SHARED);
    if (0 > g_pGOPCtxLocal->s32GOPXC_Mutex)
    {
        MS_CRITICAL_MSG(printf("[%s][%06d] create XC mutex fail _GOPXC_Mutex=0x%lx\n", __FUNCTION__, __LINE__, g_pGOPCtxLocal->s32GOPXC_Mutex));
    }
    g_pGOPCtxLocal->s32GOPXC_Lock = -1;
#endif
    return TRUE;
}





static void _GOP_TestPattern(void *pInstance, DRV_GOP_TSTPATTERN *TstPattern)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_TestPattern_SetHVDuplicate(g_pGOPCtxLocal, TstPattern->u8TSTClr_Hdup, TstPattern->u8TSTClr_Vdup);
    MDrv_GOP_TestPattern_SetStartClr(g_pGOPCtxLocal, TstPattern->u8R_stc, TstPattern->u8G_stc, TstPattern->u8B_stc);

    MDrv_GOP_TestPattern_SetHInitColor(g_pGOPCtxLocal, TstPattern->u8HR_inc, TstPattern->u8HG_inc, TstPattern->u8HB_inc);
    MDrv_GOP_TestPattern_SetHIncremental_Signz(g_pGOPCtxLocal, TstPattern->u8HR_inc_signz, TstPattern->u8HG_inc_signz, TstPattern->u8HB_inc_signz);
    MDrv_GOP_TestPattern_SetHStep(g_pGOPCtxLocal, TstPattern->u8HR_step, TstPattern->u8HG_step, TstPattern->u8HB_step);

    MDrv_GOP_TestPattern_SetVInitColor(g_pGOPCtxLocal, TstPattern->u8VR_inc, TstPattern->u8VG_inc, TstPattern->u8VB_inc);
    MDrv_GOP_TestPattern_SetVIncremental_Signz(g_pGOPCtxLocal, TstPattern->u8VR_inc_signz, TstPattern->u8VG_inc_signz, TstPattern->u8VB_inc_signz);
    MDrv_GOP_TestPattern_SetVStep(g_pGOPCtxLocal, TstPattern->u8VR_step, TstPattern->u8VG_step, TstPattern->u8VB_step);

    MDrv_GOP_TestPattern(g_pGOPCtxLocal, TstPattern->bTSTClr_En, TstPattern->u8TSTClr_Alpha);

    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, 0x0);  //only gop0 has test pattern
}






static void _GOP_GWIN_SetHVMirrorDRAMAddr(void *pInstance, MS_U8 u8win, MS_BOOL bHMirror, MS_BOOL bVMirror, GOP_GwinInfo *pinfo)
{
    MS_U32 u32AddrTemp = 0;
    MS_U8 u8Gop = _DumpGopByGwinId(pInstance, u8win);
    APIGOP_ASSERT(pinfo, printf("\npinfo is Null\n"));

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u8win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8win));
        return;
    }

    if (u8Gop != GOPTYPE.GOP0)
    {
        if (bHMirror)
        {
            if (!g_pGOPCtxLocal->pGopChipProperty->bAutoAdjustMirrorHSize)
            {
                //if Vmirror is not enable, shift HRblkSize. Otherwise, Dram start will be re-calculated at v-mirror.
                if (!bVMirror)
                {
                    pinfo->u32DRAMRBlkStart = pinfo->u32DRAMRBlkStart + (MS_U32)pinfo->u16RBlkHRblkSize;
                }

                u32AddrTemp = pinfo->u32DRAMRBlkStart + (MS_U32)pinfo->u16RBlkHRblkSize;

                if (u32AddrTemp >= MDrv_GOP_GetWordUnit(g_pGOPCtxLocal, u8Gop))
                {
                    if (MDrv_GOP_GetWordUnit(g_pGOPCtxLocal, u8Gop) != 1)
                    {
                        pinfo->u32DRAMRBlkStart = u32AddrTemp - MDrv_GOP_GetWordUnit(g_pGOPCtxLocal, u8Gop); //Back to last MIU word of the line
                    }
                    else
                    {
                        pinfo->u32DRAMRBlkStart = u32AddrTemp;
                    }
                }
                else
                {
                    pinfo->u32DRAMRBlkStart = u32AddrTemp;
                }

                if (pinfo->u16RBlkHRblkSize > (pinfo->u16DispHPixelEnd * _GOP_GetBPP(pInstance, pinfo->clrType) / 8)) //Pitch and width not equal
                {
                    pinfo->u32DRAMRBlkStart -= (pinfo->u16RBlkHRblkSize - (pinfo->u16DispHPixelEnd * _GOP_GetBPP(pInstance, pinfo->clrType) / 8));
                }
            }
            else
            {
                if (pinfo->u16RBlkHRblkSize > ((pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart) * _GOP_GetBPP(pInstance, pinfo->clrType) / 8)) //Pitch and width not equal
                {
                    pinfo->u32DRAMRBlkStart -= (pinfo->u16RBlkHRblkSize - ((pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart) * _GOP_GetBPP(pInstance, pinfo->clrType) / 8));
                }
            }
            SET_BIT(g_pGOPCtxLocal->u16GOP_HMirrorRBLK_Adr, u8win);
        }
        if (bVMirror)
        {
            //reg_dram_rblk_str += reg_rblk_hsize * (reg_gwin_vend - reg_gwin_vstr)
            if (!g_pGOPCtxLocal->pGOPCtxShared->b32TileMode[u8Gop])  //if tile mode, we should not change dram address
            {
                //Warning message
                if (pinfo->u16RBlkVPixSize != (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart))
                {
                    MS_DEBUG_MSG(printf("[%s][%d]Please to Check GwinInfo: u16RBlkVPixSize:0x%x not match (u16DispVPixelEnd=0x%x- u16DispVPixelStart=0x%x)\n",
                                        __FUNCTION__, __LINE__, pinfo->u16RBlkVPixSize, pinfo->u16DispVPixelEnd, pinfo->u16DispVPixelStart));
                }
#if 0
                if(pinfo->u16RBlkVPixSize != 0)
                {
                    pinfo->u32DRAMRBlkStart += (pinfo->u16RBlkHRblkSize*(pinfo->u16RBlkVPixSize-1));
                }
                else
#endif
                {
                    pinfo->u32DRAMRBlkStart += (pinfo->u16RBlkHRblkSize * (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart - 1));
                }
                SET_BIT(g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr, u8win);
            }
        }
    }
}
MS_U32 _GOP_IsFBExist(void *pInstance, MS_U32 fbid)
{
    GOP_WinFB_INFO *pwinFB;


    if ((fbid >= DRV_MAX_GWIN_FB_SUPPORT))
    {
        return FALSE;

    }

    pwinFB = _GetWinFB(pInstance, fbid);

    if (pwinFB == NULL)
    {
        DEBUG_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, fbid));
        return FALSE;
    }

    return pwinFB->in_use;

}

MS_BOOL GOP_GetWinInfo(void* pInstance,MS_U32 u32win, GOP_GwinInfo* pinfo)
{
    MS_U8 u8GOP;

    if (!_GOP_IsGwinIdValid(pInstance,u32win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32win));
        return FALSE;
    }
    memset(pinfo, 0, sizeof(GOP_GwinInfo));
    u8GOP = _DumpGopByGwinId(pInstance, u32win);

    if (u8GOP == GOPTYPE.GOP0)
    {
        _GOP_GetGop0WinInfo(pInstance,u32win,  pinfo);
    }
    else if (u8GOP == GOPTYPE.GOP1)
    {
        _GOP_GetGop1WinInfo(pInstance,u32win,  pinfo);
    }
    else
    {
        _GOP_GetGop23WinInfo(pInstance,u32win, pinfo);
    }

    return TRUE;
}

static void _SetGop0WinInfo(void *pInstance, MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    DRV_GOPDstType Gop0Dst = E_DRV_GOP_DST_INVALID;
    GOP_OupputColor gopOut;
    MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
    MS_U8 u8GOP = 0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    u8GOP = MDrv_DumpGopByGwinId(g_pGOPCtxLocal,u8win);

    if (fpXCIsInterlace == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nfpXCIsInterlace is Null"));
        MS_DEBUG_MSG(printf("%s: fpXCIsInterlace is not hooked. Bypass switch...\n", __FUNCTION__));
    }

    //MDrv_GOP_GWIN_UpdateRegOnce(g_pGOPCtxLocal, TRUE);

    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, E_GOP0, &Gop0Dst);

    if (Gop0Dst == E_DRV_GOP_DST_INVALID)
    {
        APIGOP_ASSERT(FALSE, printf("\n Get GOP destination fail!\n"));
        return;
    }

    _GOP_GWIN_IsEnableMirror(pInstance, Gop0Dst, &bHMirror, &bVMirror);

    if ((Gop0Dst == E_DRV_GOP_DST_OP0) || (Gop0Dst == E_DRV_GOP_DST_FRC) || (Gop0Dst == E_DRV_GOP_DST_BYPASS)) //GOP destination is OP
    {

        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP0])
        {
            gopOut = DRV_GOPOUT_RGB;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP0];
        }

        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
    }
    else  if (Gop0Dst == E_DRV_GOP_DST_IP0 || Gop0Dst == E_DRV_GOP_DST_IP0_SUB || Gop0Dst == E_DRV_GOP_DST_IP1 || Gop0Dst == E_DRV_GOP_DST_VOP || Gop0Dst == E_DRV_GOP_DST_VOP_SUB) //GOP destination is IP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP0])
        {
            gopOut = DRV_GOPOUT_YUV;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP0];
        }

        if ((fpXCIsInterlace != NULL && fpXCIsInterlace())) //interlace mode
        {
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP0, FALSE);
        }
        else    //Progressive mode
        {
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP0, TRUE);
        }
    }
    else    // GOP destination is VE,MVOP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP0])
        {
            gopOut = DRV_GOPOUT_YUV;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP0];
        }

#if (FPGA_TEST)
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP0, TRUE);  //progressive output in FPGA environement.
        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_RGB);
#else
        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
        if ((Gop0Dst == E_DRV_GOP_DST_MIXER2VE) || (Gop0Dst == E_DRV_GOP_DST_MIXER2OP))
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP0, TRUE);
        else
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP0, FALSE);
#endif
    }

    if (bHMirror)
    {
        /*save gwin postion before h mirror setting*/
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHStr = pinfo->u16DispHPixelStart;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHEnd = pinfo->u16DispHPixelEnd;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorGOP0WinX = pinfo->u16WinX;

        MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, E_GOP0, TRUE);
        _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_HPos, pinfo);
        _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_X, pinfo);
    }
    else
    {
        //System is mirror, but bHMirror=FALSE, so need GOP mirror off
        if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror)
        {
            MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, E_GOP0, FALSE);
        }
    }

    if (!(g_pGOPCtxLocal->pGOPCtxShared->bHMirror && ((Gop0Dst == E_DRV_GOP_DST_OP0) || (Gop0Dst == E_DRV_GOP_DST_FRC) || (Gop0Dst == E_DRV_GOP_DST_BYPASS))))
    {
        if (g_pGOPCtxLocal->Gwin_H_Dup)
            pinfo->u16DispHPixelEnd = (pinfo->u16DispHPixelEnd << 1) - pinfo->u16DispHPixelStart;
    }

    if (bVMirror)
    {
        /*save gwin postion before V mirror setting*/
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVStr = pinfo->u16DispVPixelStart;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVEnd = pinfo->u16DispVPixelEnd;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorGOP0WinY = pinfo->u16WinY;

        MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, E_GOP0, TRUE);
        //SET_BIT(g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr, u8win);
        _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_VPos, pinfo);
        _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_Y, pinfo);
    }
    else
    {
        if (g_pGOPCtxLocal->pGOPCtxShared->bVMirror) //System is mirror, but bHMirror=FALSE, so need GOP mirror off
        {
            MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, E_GOP0, FALSE);
        }
    }

    //if (!(g_pGOPCtxLocal->pGOPCtxShared->bVMirror))
    {
        if (g_pGOPCtxLocal->Gwin_V_Dup)
        {

            if (((Gop0Dst == E_DRV_GOP_DST_IP0) || (Gop0Dst == E_DRV_GOP_DST_IP1) || (Gop0Dst == E_DRV_GOP_DST_IP0_SUB) || (Gop0Dst == E_DRV_GOP_DST_VOP) || (Gop0Dst == E_DRV_GOP_DST_VOP_SUB)) && (fpXCIsInterlace != NULL && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (((pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart) / 2);
            else if ((Gop0Dst == E_DRV_GOP_DST_VE) || (Gop0Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (((pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart) / 2);
            else
                pinfo->u16DispVPixelEnd = (pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart;
        }
        else
        {
            if (((Gop0Dst == E_DRV_GOP_DST_IP0) || (Gop0Dst == E_DRV_GOP_DST_IP1) || (Gop0Dst == E_DRV_GOP_DST_IP0_SUB) || (Gop0Dst == E_DRV_GOP_DST_VOP) || (Gop0Dst == E_DRV_GOP_DST_VOP_SUB)) && (fpXCIsInterlace != NULL && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart) / 2;
            else if ((Gop0Dst == E_DRV_GOP_DST_VE) || (Gop0Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart) / 2;
            else if (((Gop0Dst == E_DRV_GOP_DST_OP0) || (Gop0Dst == E_DRV_GOP_DST_FRC) || (Gop0Dst == E_DRV_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, 0))
            {
                pinfo->u16DispVPixelStart = pinfo->u16DispVPixelStart / 2;
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelEnd / 2;
            }
        }
    }

    MDrv_GOP_GWIN_SetWinFmt(g_pGOPCtxLocal, u8win, (DRV_GOPColorType)pinfo->clrType);
    //MDrv_GOP_GWIN_UpdateRegOnce(g_pGOPCtxLocal, FALSE);
    MDrv_GOP_GWIN_SetGwinInfo(g_pGOPCtxLocal, u8win, (DRV_GOP_GWIN_INFO *)pinfo);
    DEBUG_INFO(printf("GWIN_SetWin(%d): [adr(B), RBsz, offset] = [%lx, %lx, %lx]\n", u8win,\
                          pinfo->u32DRAMRBlkStart,\
                          ((MS_U32)pinfo->u16RBlkHPixSize * (MS_U32)pinfo->u16RBlkVPixSize) / (64 / _GOP_GetBPP(pInstance, pinfo->clrType)),\
                          ((MS_U32)pinfo->u16WinY * (MS_U32)pinfo->u16RBlkHPixSize + (MS_U32)pinfo->u16WinX) / (64 / _GOP_GetBPP(pInstance, pinfo->clrType))));
    DEBUG_INFO(printf("\t[Vst, Vend, Hst, Hend, GwinHsz] = [%d, %d, %d, %d, %d]\n",\
                          pinfo->u16DispVPixelStart, pinfo->u16DispVPixelEnd, pinfo->u16DispHPixelStart / (64 / _GOP_GetBPP(pInstance, pinfo->clrType)), pinfo->u16DispHPixelEnd / (64 / _GOP_GetBPP(pInstance, pinfo->clrType)),\
                          pinfo->u16RBlkHPixSize / (64 / _GOP_GetBPP(pInstance, pinfo->clrType))));
}

static void _SetGop1WinInfo(void *pInstance, MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    DRV_GOPDstType Gop1Dst = E_DRV_GOP_DST_INVALID;
    GOP_OupputColor gopOut;
    MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
    MS_U8 u8GOP = 0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    u8GOP = MDrv_DumpGopByGwinId(g_pGOPCtxLocal,u8win);


    if (fpXCIsInterlace == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nfpXCIsInterlace is Null"));
        MS_DEBUG_MSG(printf("%s: fpXCIsInterlace is not hooked. Bypass switch...\n", __FUNCTION__));
    }

    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, E_GOP1, &Gop1Dst);
    if (Gop1Dst == E_DRV_GOP_DST_INVALID)
    {
        APIGOP_ASSERT(FALSE, printf("\n Get GOP destination fail!\n"));
        return;
    }
    _GOP_GWIN_IsEnableMirror(pInstance, Gop1Dst, &bHMirror, &bVMirror);
    if ((Gop1Dst == E_DRV_GOP_DST_OP0) || (Gop1Dst == E_DRV_GOP_DST_FRC) || (Gop1Dst == E_DRV_GOP_DST_BYPASS)) //GOP destination is OP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP1])
        {
            gopOut = DRV_GOPOUT_RGB;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP1];
        }

        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
    }
    else if (Gop1Dst == E_DRV_GOP_DST_IP0 || Gop1Dst == E_DRV_GOP_DST_IP0_SUB || Gop1Dst == E_DRV_GOP_DST_IP1 || Gop1Dst == E_DRV_GOP_DST_VOP || Gop1Dst == E_DRV_GOP_DST_VOP_SUB) // GOP destination is IP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP1])
        {
            gopOut = DRV_GOPOUT_YUV;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP1];
        }

        if ((fpXCIsInterlace != NULL) && fpXCIsInterlace()) //interlace mode
        {
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP1, FALSE);
        }
        else    //Progressive mode
        {
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP1, TRUE);
        }
    }
    else    // GOP destination is VE,MVOP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[E_GOP1])
        {
            gopOut = DRV_GOPOUT_YUV;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[1];
        }

#if(FPGA_TEST)
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP1, TRUE);  //progressive output in FPGA environement.
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_RGB);
#else
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_YUV);
        if ((Gop1Dst == E_DRV_GOP_DST_MIXER2VE) || (Gop1Dst == E_DRV_GOP_DST_MIXER2OP))
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP1, TRUE);
        else
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, E_GOP1, FALSE);
#endif

    }

    if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror || g_pGOPCtxLocal->pGOPCtxShared->bVMirror)
    {
        g_pGOPCtxLocal->sMirrorInfo[u8win].u32NonMirrorFBAdr = pinfo->u32DRAMRBlkStart;
        _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, u8win, bHMirror, bVMirror, pinfo);
    }

    if (bHMirror)
    {
        /*save gwin postion before h mirror setting*/
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHStr = pinfo->u16DispHPixelStart;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHEnd = pinfo->u16DispHPixelEnd;

        MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, E_GOP1, TRUE);
        _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_HPos, pinfo);
    }
    else
    {
        if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror) //System is mirror, but bHMirror=FALSE, so need GOP mirror off
        {
            MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, E_GOP1, FALSE);
        }
    }

    if (bVMirror)
    {
        /*save gwin postion before V mirror setting*/
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVStr = pinfo->u16DispVPixelStart;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVEnd = pinfo->u16DispVPixelEnd;

        MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, E_GOP1, TRUE);
        _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_VPos, pinfo);
    }
    else
    {
        if (g_pGOPCtxLocal->pGOPCtxShared->bVMirror) //System is mirror, but bHMirror=FALSE, so need GOP mirror off
        {
            MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, E_GOP1, FALSE);
        }
    }

    //if (!g_pGOPCtxLocal->pGOPCtxShared->bVMirror)
    {
        if (g_pGOPCtxLocal->Gwin_V_Dup)
        {
            if (((Gop1Dst == E_DRV_GOP_DST_IP0) || (Gop1Dst == E_DRV_GOP_DST_IP1) || (Gop1Dst == E_DRV_GOP_DST_IP0_SUB) || (Gop1Dst == E_DRV_GOP_DST_VOP) || (Gop1Dst == E_DRV_GOP_DST_VOP_SUB)) && (fpXCIsInterlace != NULL && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (((pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart) / 2);
            else if ((Gop1Dst == E_DRV_GOP_DST_VE) || (Gop1Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (((pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart) / 2);
            else
                pinfo->u16DispVPixelEnd = (pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart;
        }
        else
        {
            if (((Gop1Dst == E_DRV_GOP_DST_IP0) || (Gop1Dst == E_DRV_GOP_DST_IP1) || (Gop1Dst == E_DRV_GOP_DST_IP0_SUB) || (Gop1Dst == E_DRV_GOP_DST_VOP) || (Gop1Dst == E_DRV_GOP_DST_VOP_SUB)) && (fpXCIsInterlace != NULL && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart) / 2;
            else if ((Gop1Dst == E_DRV_GOP_DST_VE) || (Gop1Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart) / 2;
            else if (((Gop1Dst == E_DRV_GOP_DST_OP0) || (Gop1Dst == E_DRV_GOP_DST_FRC) || (Gop1Dst == E_DRV_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, 1))
            {
                pinfo->u16DispVPixelStart = pinfo->u16DispVPixelStart / 2;
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelEnd / 2;
            }
        }
    }

    MDrv_GOP_GWIN_SetWinFmt(g_pGOPCtxLocal, u8win, (DRV_GOPColorType)pinfo->clrType);
    MDrv_GOP_GWIN_SetGwinInfo(g_pGOPCtxLocal, u8win, (DRV_GOP_GWIN_INFO *)pinfo);
}

static void _SetGop23WinInfo(void *pInstance, MS_U8 u8win, GOP_GwinInfo *pinfo)
{
    DRV_GOPDstType Gop23Dst = E_DRV_GOP_DST_INVALID;
    GOP_OupputColor gopOut;
    MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
    MS_U8 u8GOP=0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (fpXCIsInterlace == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nfpXCIsInterlace is Null"));
        MS_DEBUG_MSG(printf("%s: fpXCIsInterlace is not hooked. Bypass switch...\n", __FUNCTION__));
    }

    if (!MDrv_GOP_HasGop1GPalette(g_pGOPCtxLocal) && (pinfo->clrType == E_GOP_COLOR_I8 || pinfo->clrType == E_GOP_COLOR_2266\
                                                          || pinfo->clrType == E_GOP_COLOR_RGB555_BLINK))
    {
        MS_CRITICAL_MSG(printf("\n[%s] not support gwin id:%d index mode", __FUNCTION__, u8win));
        return;
    }

    u8GOP=MDrv_DumpGopByGwinId(g_pGOPCtxLocal,u8win);
    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), &Gop23Dst);
    if (Gop23Dst == E_DRV_GOP_DST_INVALID)
    {
        APIGOP_ASSERT(FALSE, printf("\n Get GOP destination fail!\n"));
        return;
    }
    _GOP_GWIN_IsEnableMirror(pInstance, Gop23Dst, &bHMirror, &bVMirror);
    if ((Gop23Dst == E_DRV_GOP_DST_OP0) || (Gop23Dst == E_DRV_GOP_DST_FRC) || (Gop23Dst == E_DRV_GOP_DST_BYPASS)) //GOP destination is OP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[_DumpGopByGwinId(pInstance, u8win)])
        {
            gopOut = DRV_GOPOUT_RGB;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[_DumpGopByGwinId(pInstance, u8win)];
        }

        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
    }
    else  if (Gop23Dst == E_DRV_GOP_DST_IP0 || Gop23Dst == E_DRV_GOP_DST_IP0_SUB || Gop23Dst == E_DRV_GOP_DST_IP1 || Gop23Dst == E_DRV_GOP_DST_VOP || Gop23Dst == E_DRV_GOP_DST_VOP_SUB)  // GOP destination is IP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[_DumpGopByGwinId(pInstance, u8win)])
        {
            gopOut = DRV_GOPOUT_YUV;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[_DumpGopByGwinId(pInstance, u8win)];
        }

        if ((fpXCIsInterlace != NULL) && fpXCIsInterlace()) //interlace mode
        {
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), FALSE);
        }
        else    //Progressive mode
        {
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, gopOut);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), TRUE);
        }
    }
    else    // GOP destination is VE,MVOP
    {
        if ((-1) == g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[_DumpGopByGwinId(pInstance, u8win)])
        {
            gopOut = DRV_GOPOUT_YUV;
        }
        else
        {
            gopOut = (GOP_OupputColor)g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[_DumpGopByGwinId(pInstance, u8win)];
        }

#if(FPGA_TEST)
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), TRUE);  //progressive output in FPGA environement.
        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_RGB);
#else
        MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_YUV);
        if ((Gop23Dst == E_DRV_GOP_DST_MIXER2VE) || (Gop23Dst == E_DRV_GOP_DST_MIXER2OP))
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), TRUE);
        else
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), FALSE);
#endif

    }

    if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror || g_pGOPCtxLocal->pGOPCtxShared->bVMirror)
    {
        g_pGOPCtxLocal->sMirrorInfo[u8win].u32NonMirrorFBAdr = pinfo->u32DRAMRBlkStart;
        _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, u8win, bHMirror, bVMirror, pinfo);
    }

    if (bHMirror)
    {
        /*save gwin postion before h mirror setting*/
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHStr = pinfo->u16DispHPixelStart;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorHEnd = pinfo->u16DispHPixelEnd;

        MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), TRUE);

        if (!bCursorSupport(pInstance, _DumpGopByGwinId(pInstance, u8win)))
        {
            _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_HPos, pinfo);
        }
    }
    else
    {
        if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror) //System is mirror, but bHMirror=FALSE, so need GOP mirror off
        {
            MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), FALSE);
        }
    }

    if (bVMirror)
    {
        /*save gwin postion before V mirror setting*/
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVStr = pinfo->u16DispVPixelStart;
        g_pGOPCtxLocal->sMirrorInfo[u8win].u16NonMirrorVEnd = pinfo->u16DispVPixelEnd;

        MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), TRUE);

        if (!bCursorSupport(pInstance, _DumpGopByGwinId(pInstance, u8win)))
        {
            _GOP_GWIN_SetHVMirrorWinPos(pInstance, u8win, E_Set_VPos, pinfo);
        }
    }
    else
    {
        if (g_pGOPCtxLocal->pGOPCtxShared->bVMirror) //System is mirror, but bHMirror=FALSE, so need GOP mirror off
        {
            MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win), FALSE);
        }
    }

    //if (!g_pGOPCtxLocal->pGOPCtxShared->bVMirror)
    {
        if (g_pGOPCtxLocal->Gwin_V_Dup)
        {

            if (((Gop23Dst == E_DRV_GOP_DST_IP0) || (Gop23Dst == E_DRV_GOP_DST_IP1) || (Gop23Dst == E_DRV_GOP_DST_IP0_SUB) || (Gop23Dst == E_DRV_GOP_DST_VOP) || (Gop23Dst == E_DRV_GOP_DST_VOP_SUB)) && (fpXCIsInterlace != NULL && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (((pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart) / 2);
            else if ((Gop23Dst == E_DRV_GOP_DST_VE) || (Gop23Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (((pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart) / 2);
            else
                pinfo->u16DispVPixelEnd = (pinfo->u16DispVPixelEnd << 1) - pinfo->u16DispVPixelStart;
        }
        else
        {
            if (((Gop23Dst == E_DRV_GOP_DST_IP0) || (Gop23Dst == E_DRV_GOP_DST_IP1) || (Gop23Dst == E_DRV_GOP_DST_IP0_SUB) || (Gop23Dst == E_DRV_GOP_DST_VOP) || (Gop23Dst == E_DRV_GOP_DST_VOP_SUB)) && (fpXCIsInterlace != NULL && fpXCIsInterlace()))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart) / 2;
            else if ((Gop23Dst == E_DRV_GOP_DST_VE) || (Gop23Dst == E_DRV_GOP_DST_OP1))
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelStart + (pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart) / 2;
            else if (((Gop23Dst == E_DRV_GOP_DST_OP0) || (Gop23Dst == E_DRV_GOP_DST_FRC) || (Gop23Dst == E_DRV_GOP_DST_BYPASS)) && !MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, _DumpGopByGwinId(pInstance, u8win)))
            {
                pinfo->u16DispVPixelStart = pinfo->u16DispVPixelStart / 2;
                pinfo->u16DispVPixelEnd = pinfo->u16DispVPixelEnd / 2;
            }
        }
    }

    MDrv_GOP_GWIN_SetGwinInfo(g_pGOPCtxLocal, u8win, (DRV_GOP_GWIN_INFO *)pinfo);
    MDrv_GOP_GWIN_SetWinFmt(g_pGOPCtxLocal, u8win, (DRV_GOPColorType)pinfo->clrType);
}

static void _GOP_ArrangePoolMem(void *pInstance, GOP_FB_POOL *pFBPool)
{
    MS_U32 lastAddr, curStartAdr, size2move, moveSize, gapSize, count;
    GOP_GwinInfo gWin;
    MS_U32 * src_PA,*dst_PA;
    GOP_WinFB_INFO *pwinFB;
    MS_U8 u8GOP;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MS_U32 u32NextFBId = pFBPool->u32FirstFBIdInPool;

    memset(&gWin, 0,  sizeof(GOP_GwinInfo));

    if (0 == pFBPool->u32GWinFB_Pool_MemLen)
        return;
    WARNNING_INFO(printf("Arrange GOP pool %d\n", pFBPool->poolId));
    lastAddr = pFBPool->u32GWinFB_Pool_BaseAddr;


    if (fpEventNotify != NULL)
        fpEventNotify(E_GOP_EVENT_BUF_ARRANGE_BEGIN, NULL);
    else
    {
        //APIGOP_ASSERT(TRUE, printf("\nfpEvenNotifyBegin is Null"));
        DEBUG_INFO(printf("%s: fpEvenNotifyBegin is not hooked. Bypass switch...\n", __FUNCTION__));
    }

    while (INVALID_POOL_NEXT_FBID != u32NextFBId)
    {
        pwinFB = _GetWinFB(pInstance, u32NextFBId);

        if (pwinFB == NULL)
        {
            CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32NextFBId));
            return;
        }

        APIGOP_ASSERT(DRV_MAX_GWIN_FB_SUPPORT>u32NextFBId, printf("\nInvalid next fbid %d", (unsigned int)u32NextFBId));
        APIGOP_ASSERT(pwinFB->in_use && pwinFB->poolId == pFBPool->poolId
                                , printf("\nSerious error, inconsistant Pool list at  fbid %d", (unsigned int)u32NextFBId));
        APIGOP_ASSERT(pwinFB->addr >=  lastAddr
                                , printf("\nSerious error, inconsistant Pool list at  fbid %d", (unsigned int)u32NextFBId));
        APIGOP_ASSERT(pwinFB->addr >= pFBPool->u32GWinFB_Pool_BaseAddr &&
                      (pwinFB->addr + pwinFB->size) <= (pFBPool->u32GWinFB_Pool_BaseAddr + pFBPool->u32GWinFB_Pool_MemLen)
                                , printf("\nSerious error, inconsistant Pool list at  fbid %d", (unsigned int)u32NextFBId));
        APIGOP_ASSERT(IS_GWIN_SDRAM_ALIGN(pwinFB->addr)
                      , printf("\nInvalid alignment for addr %d", (unsigned int)pwinFB->addr));

        if (pwinFB->addr == lastAddr)
        {
            lastAddr += GWIN_SDRAM_PG_ALIGN(pwinFB->size);
            u32NextFBId = pwinFB->nextFBIdInPool;
            continue;
        }

        curStartAdr = pwinFB->addr;
        gapSize = curStartAdr - lastAddr;
        moveSize = count = 0;
        size2move = pwinFB->size;

        while (moveSize < size2move)
        {
            count = (moveSize + gapSize > size2move) ? (size2move - moveSize) : gapSize;
            src_PA = (MS_U32 *)MS_PA2KSEG1((MS_U32)(curStartAdr + moveSize));
            dst_PA = (MS_U32 *)MS_PA2KSEG1((MS_U32)(lastAddr + moveSize));
            memcpy(dst_PA, src_PA, count);
            moveSize += count;
        }

        pwinFB->addr = lastAddr;

        for (curStartAdr = 0; (MS_U8)curStartAdr < MAX_GWIN_SUPPORT; curStartAdr++)
        {
            if ((g_pGOPCtxLocal->pGOPCtxShared->gwinMap[(MS_U8)curStartAdr].u32CurFBId == u32NextFBId)
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                && (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[(MS_U8)curStartAdr].u32GOPClientId == (MS_U32)getpid())
#else
                && (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[(MS_U8)curStartAdr].u32GOPClientId ==g_pGOPCtxLocal->u32GOPClientId)
#endif
               )
            {
                if (GOP_GetWinInfo(pInstance, (MS_U8)curStartAdr, &gWin))
                {
                    gWin.u32DRAMRBlkStart = lastAddr;
                    u8GOP = _DumpGopByGwinId(pInstance, curStartAdr);
                    MDrv_GOP_Sel(g_pGOPCtxLocal, u8GOP);
                    if (u8GOP == GOPTYPE.GOP0)
                    {
                        _SetGop0WinInfo(pInstance, (MS_U8)curStartAdr, &gWin);
                    }
                    else if (u8GOP == GOPTYPE.GOP1)
                    {
                        _SetGop1WinInfo(pInstance, (MS_U8)curStartAdr, &gWin);
                    }
                    else
                    {
                        _SetGop23WinInfo(pInstance, (MS_U8)curStartAdr, &gWin);
                    }
                    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
                }
                else
                {
                    APIGOP_ASSERT(FALSE, printf("!!!Alert!!! Serious error in arrange pool memory\n"));
                }
            }
        }

        lastAddr = GWIN_SDRAM_PG_ALIGN(pwinFB->addr + pwinFB->size);
        u32NextFBId = pwinFB->nextFBIdInPool;

    }

    if (fpEventNotify != NULL)
        fpEventNotify(E_GOP_EVENT_BUF_ARRANGE_END, NULL);
    else
    {
        APIGOP_ASSERT(TRUE, printf("\nfpEvenNotifyEnd is Null"));
        DEBUG_INFO(printf("%s: fpEvenNotifyEnd is not hooked. Bypass switch...\n", __FUNCTION__));
    }
}

static void _GOP_FreePoolMem(void *pInstance, GOP_FB_POOL *pFBPool, MS_U32 u32fbId)
{
    MS_U32 u32CurFBId, u32PreFBId;
    MS_U32 lastAddr;
    GOP_WinFB_INFO *pwinFB;
    GOP_WinFB_INFO *pPrewinFB;

    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return;
    }

    u32CurFBId = pFBPool->u32FirstFBIdInPool;
    u32PreFBId = INVALID_POOL_NEXT_FBID;
    lastAddr = pFBPool->u32GWinFB_Pool_BaseAddr;

    while (INVALID_POOL_NEXT_FBID != u32CurFBId)
    {
        pwinFB = _GetWinFB(pInstance, u32CurFBId);

        if (pwinFB == NULL)
        {
            CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32CurFBId));
            return;
        }

        APIGOP_ASSERT(DRV_MAX_GWIN_FB_SUPPORT>u32CurFBId, printf("\nInvalid next fbid %d", (unsigned int)u32CurFBId));
        APIGOP_ASSERT(pwinFB->in_use && pwinFB->poolId == pFBPool->poolId
                                , printf("\nSerious error, inconsistant Pool list at  fbid %d", (unsigned int)u32CurFBId));
        APIGOP_ASSERT(pwinFB->addr >=  lastAddr
                                , printf("\nSerious error, inconsistant Pool list at  fbid %d", (unsigned int)u32CurFBId));
        APIGOP_ASSERT(pwinFB->addr >= pFBPool->u32GWinFB_Pool_BaseAddr &&
                      (pwinFB->addr + pwinFB->size) <= (pFBPool->u32GWinFB_Pool_BaseAddr + pFBPool->u32GWinFB_Pool_MemLen)
                                , printf("\nSerious error, inconsistant Pool list at  fbid %d", (unsigned int)u32CurFBId));
        if (u32CurFBId == u32fbId)
            break;
        lastAddr = pwinFB->addr + pwinFB->size;
        u32PreFBId = u32CurFBId;
        u32CurFBId = pwinFB->nextFBIdInPool;
    }
    APIGOP_ASSERT(INVALID_POOL_NEXT_FBID!=u32CurFBId, printf("\n Inconsistent pool list for %d", (unsigned int)u32CurFBId));

    pwinFB = _GetWinFB(pInstance, u32CurFBId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32CurFBId));
        return;
    }

    if (INVALID_POOL_NEXT_FBID == u32PreFBId)
        pFBPool->u32FirstFBIdInPool =  pwinFB->nextFBIdInPool;
    else
    {
        pPrewinFB = _GetWinFB(pInstance, u32PreFBId);

        if (pPrewinFB == NULL)
        {
            CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32PreFBId));
            return;
        }

        pPrewinFB->nextFBIdInPool = pwinFB->nextFBIdInPool;
    }
    pwinFB->nextFBIdInPool = INVALID_POOL_NEXT_FBID;

}

static MS_U8 _GOP_GWIN_CreatePoolFB(void *pInstance, MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString)
{
    MS_U32 size;
    GOP_FB_POOL *pFBPool;
    MS_U32 u32CurFBId, u32PreFBId;
    MS_U32 lastAddr;
    MS_BOOL bPoolArranged = FALSE;
    GOP_WinFB_INFO *pwinFB;
    GOP_WinFB_INFO *pCurwinFB;
    GOP_WinFB_INFO *pPrewinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GWIN_NO_AVAILABLE;
    }

    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GWIN_NO_AVAILABLE;
    }

    if (pwinFB->in_use)
    {
        msWarning(ERR_FB_ID_ALREADY_ALLOCATED);
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is already exist\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GWIN_NO_AVAILABLE;

    }

    switch (g_pGOPCtxLocal->pGOPCtxShared->fb_currentPoolId)
    {
    case GOP_WINFB_POOL1:
        pFBPool = &g_pGOPCtxLocal->pGOPCtxShared->fbPool1;
        break;
    case GOP_WINFB_POOL2:
        pFBPool = &g_pGOPCtxLocal->pGOPCtxShared->fbPool2;
        break;
    default:
        msWarning(ERR_FB_OUT_OF_MEMORY);
        return GWIN_NO_AVAILABLE;
    }

TRY_AGAIN:

    u32CurFBId = pFBPool->u32FirstFBIdInPool;
    u32PreFBId = INVALID_POOL_NEXT_FBID;
    lastAddr = pFBPool->u32GWinFB_Pool_BaseAddr;

    while (INVALID_POOL_NEXT_FBID != u32CurFBId)
    {
        pCurwinFB = _GetWinFB(pInstance, u32CurFBId);

        if (pCurwinFB == NULL)
        {
            CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32CurFBId));
            return GWIN_NO_AVAILABLE;
        }

        APIGOP_ASSERT(DRV_MAX_GWIN_FB_SUPPORT>u32CurFBId, printf("\nInvalid next u32fbId %d", (unsigned int)u32CurFBId));
        APIGOP_ASSERT(pCurwinFB->in_use && pCurwinFB->poolId == pFBPool->poolId
                      , printf("\nSerious error, inconsistant Pool list at  u32fbId %d", (unsigned int)u32CurFBId));
        APIGOP_ASSERT(pCurwinFB->addr >=  lastAddr
                      , printf("\nSerious error, inconsistant Pool list at  u32fbId %d", (unsigned int)u32CurFBId));
        APIGOP_ASSERT(pCurwinFB->addr >= pFBPool->u32GWinFB_Pool_BaseAddr &&
                      (pCurwinFB->addr + pCurwinFB->size) <= (pFBPool->u32GWinFB_Pool_BaseAddr + pFBPool->u32GWinFB_Pool_MemLen)
                      , printf("\nSerious error, inconsistant Pool list at  u32fbId %d", (unsigned int)u32CurFBId));
        lastAddr = GWIN_SDRAM_PG_ALIGN(pCurwinFB->addr + pCurwinFB->size);
        u32PreFBId = u32CurFBId;
        u32CurFBId = pCurwinFB->nextFBIdInPool;

    }

    size =  _GOP_CalcPitch(pInstance, fbFmt, width) * height;

    if ((pFBPool->u32GWinFB_Pool_MemLen - (lastAddr - pFBPool->u32GWinFB_Pool_BaseAddr)) < size)
    {
        if (bPoolArranged)
        {
            msWarning(ERR_FB_OUT_OF_MEMORY);
            return GWIN_NO_AVAILABLE;
        }
        _GOP_ArrangePoolMem(pInstance, pFBPool);
        bPoolArranged = TRUE;
        goto TRY_AGAIN;
    }

    pwinFB->enable = FALSE;
    pwinFB->in_use = 1;
    //g_pGOPCtxLocal->winFB[u32fbId].gWinId = gId;  // set by calling functions
    pwinFB->x0 = dispX;
    pwinFB->y0 = dispY;
    pwinFB->width = width;
    pwinFB->height = height;
    pwinFB->x1 = pwinFB->x0 + pwinFB->width;
    pwinFB->y1 = pwinFB->y0 + pwinFB->height;
//    g_pGOPCtxLocal->winFB[u32fbId].pitch = width & 0xFFFC;//width; //(width & 0xFFF8);  // pitch must be 4-pix alignment
    pwinFB->addr  = lastAddr;
    pwinFB->size  = size;

    pwinFB->s_x = dispX;
    pwinFB->s_y = dispY;
    pwinFB->dispWidth   = width;
    pwinFB->dispHeight  = height;
    pwinFB->poolId = pFBPool->poolId;
    pwinFB->pitch = _GOP_CalcPitch(pInstance, fbFmt, width);


    pCurwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GWIN_OSD_DEFAULT].u32CurFBId);

    if (pCurwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GWIN_OSD_DEFAULT].u32CurFBId));
        return GWIN_NO_AVAILABLE;
    }

    // FB format
    pwinFB->fbFmt  = (fbFmt != FB_FMT_AS_DEFAULT) ? fbFmt : pCurwinFB->fbFmt;


    pwinFB->string = FBString;
    pwinFB->u32GOPClientId = getpid();
    pwinFB->nextFBIdInPool = INVALID_POOL_NEXT_FBID;
    if (INVALID_POOL_NEXT_FBID == u32PreFBId)
        pFBPool->u32FirstFBIdInPool = u32fbId;
    else
    {
        pPrewinFB = _GetWinFB(pInstance, u32PreFBId);

        if (pPrewinFB == NULL)
        {
            CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32PreFBId));
            return GWIN_NO_AVAILABLE;
        }

        pPrewinFB->nextFBIdInPool = u32fbId;
    }
    return GWIN_OK;
}

static void _GOP_GetPDByDst(void *pInstance, MS_U8 u8GOP_num, DRV_GOPDstType GopDst, MS_U16 *u16StrwinStr)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (GopDst)
    {
    case E_DRV_GOP_DST_IP0:
        if (g_pGOPCtxLocal->pGopChipProperty->bIgnoreIPHPD)
            *u16StrwinStr = 0;
        else
        {
            if (fpXCGetCapHStart != NULL)
            {
                *u16StrwinStr = fpXCGetCapHStart() + g_pGOPCtxLocal->pGopChipProperty->GOP_IP_PD;
            }
            else
            {
                DEBUG_INFO(printf("[%s] [%d]Callback function fpXCGetCapHStart is NULL\n", __FUNCTION__, __LINE__));
                *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_IP_PD;
            }
        }
        break;
    case E_DRV_GOP_DST_OP1:
        *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_OP1_PD;
        break;
    case E_DRV_GOP_DST_OP0:
    case E_DRV_GOP_DST_GOPScaling:
        *u16StrwinStr = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[u8GOP_num] + g_pGOPCtxLocal->pGopChipProperty->GOP_PD;
        if ((g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8GOP_num] == 0xF00) && (g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8GOP_num] == 0x870))
        {
            *u16StrwinStr += g_pGOPCtxLocal->pGopChipProperty->GOP_UHD_PD_Offset;
        }
        break;
    case E_DRV_GOP_DST_FRC:
        *u16StrwinStr = 0;
        break;
    case E_DRV_GOP_DST_BYPASS:
        *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_Mux_FRC_offset;
        break;
    case E_DRV_GOP_DST_VOP:
        *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_MVOP_PD;
        break;
    case E_DRV_GOP_DST_MIXER2VE:
    case E_DRV_GOP_DST_MIXER2OP:
        *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_MIXER_PD;
        break;
    case E_DRV_GOP_DST_VE:
        *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_VE_PD;
        break;
    default:
        *u16StrwinStr = g_pGOPCtxLocal->pGopChipProperty->GOP_PD;
        break;
    }

}

static void _GOP_AdjustHSPD(void *pInstance, MS_U8 u8GOP_num, MS_U16 u16StrwinStr, DRV_GOPDstType GopDst)
{
    MS_U8 u8Offset;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP_num))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP_num));
        return;
    }

    switch (GopDst)
    {
    case E_DRV_GOP_DST_IP0:
        u8Offset = 0;
        break;
    case E_DRV_GOP_DST_MIXER2VE:
    case E_DRV_GOP_DST_MIXER2OP:
    case E_DRV_GOP_DST_VE:
    case E_DRV_GOP_DST_FRC:
#if 1
        u8Offset = MDrv_GOP_GetHPipeOfst(g_pGOPCtxLocal, u8GOP_num, GopDst);
        //printf("[%s][%d]GOP %d,offset = %d \n",__FUNCTION__,__LINE__,u8GOP_num,u8Offset);
#else
        if(g_pGOPCtxLocal->pGopChipProperty->bIgnoreVEHPD)
        u8Offset = 0;
        else
        u8Offset = MDrv_GOP_GetHPipeOfst(g_pGOPCtxLocal,u8GOP_num, GopDst);
#endif
        break;
    case E_DRV_GOP_DST_OP0:
    case E_DRV_GOP_DST_VOP:
    case E_DRV_GOP_DST_BYPASS:
    default:
        u8Offset = MDrv_GOP_GetHPipeOfst(g_pGOPCtxLocal, u8GOP_num, GopDst);
        break;
    }

    MDrv_GOP_GWIN_SetHSPipe(g_pGOPCtxLocal, u8GOP_num, u8Offset + u16StrwinStr);
}

static void _GOP_InitHSPDByGOP(void *pInstance, MS_U8 u8GOP_num)
{
    MS_U16 u16StrwinStr = 0;
    DRV_GOPDstType GopDst = E_DRV_GOP_DST_OP0;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP_num))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP_num));
        return;
    }

    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GOP_num, &GopDst);
    _GOP_GetPDByDst(pInstance, u8GOP_num, GopDst, &u16StrwinStr);
    _GOP_AdjustHSPD(pInstance, u8GOP_num, u16StrwinStr, GopDst);
}



static MS_BOOL _IsGopInMux0(void *pInstance, MS_U8 u8GopNum)
{
    MS_U8 GetGopNum = 0;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &GetGopNum, E_GOP_MUX0);

    if (u8GopNum == GetGopNum)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static MS_BOOL _IsGopInMux1(void *pInstance, MS_U8 u8GopNum)
{
    MS_U8 GetGopNum;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &GetGopNum, E_GOP_MUX1);
    if (u8GopNum == GetGopNum)
    {
        return TRUE;
    }
    return FALSE;
}
static MS_BOOL _IsGopInMux2(void *pInstance, MS_U8 u8GopNum)
{
    MS_U8 GetGopNum = 0;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &GetGopNum, E_GOP_MUX2);

    if (u8GopNum == GetGopNum)
    {
        return TRUE;
    }
    return FALSE;
}
static MS_BOOL _IsGopInMux3(void *pInstance, MS_U8 u8GopNum)
{
    MS_U8 GetGopNum;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &GetGopNum, E_GOP_MUX3);
    if (u8GopNum == GetGopNum)
    {
        return TRUE;
    }
    return FALSE;
}

static MS_BOOL _IsGopInMux0To2(void *pInstance, MS_U8 u8GopNum)
{
    if (_IsGopInMux0(pInstance, u8GopNum) || _IsGopInMux1(pInstance, u8GopNum) || _IsGopInMux2(pInstance, u8GopNum))
    {
        return TRUE;
    }
    return FALSE;
}

static MS_BOOL _IsGopInMux0To3(void *pInstance, MS_U8 u8GopNum)
{
    if (_IsGopInMux0(pInstance, u8GopNum) || _IsGopInMux1(pInstance, u8GopNum) || _IsGopInMux2(pInstance, u8GopNum) || _IsGopInMux3(pInstance, u8GopNum))
    {
        return TRUE;
    }
    return FALSE;
}

static Gop_MuxSel _GetNonUseOPMux(void *pInstance)
{
    MS_S8 i;
    MS_U8 GetGopNum;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    for (i = MAX_GOP_MUX_OPNum - 1; i >= 0; i--)
    {
        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &GetGopNum, (Gop_MuxSel)i);
        if (GetGopNum == 0x7)
            return (Gop_MuxSel)i;
    }
    return MAX_GOP_MUX_SUPPORT;
}

static void _GOP_Get_StretchWin(void *pInstance, MS_U8 gop, MS_U16 *x, MS_U16 *y, MS_U16 *w, MS_U16 *h)
{
    DRV_GOPDstType eDstType = E_DRV_GOP_DST_INVALID;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    MDrv_GOP_GWIN_Get_StretchWin(g_pGOPCtxLocal, gop, x, y, w, h);
    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, gop, &eDstType);

    if ((eDstType == E_DRV_GOP_DST_IP0) || (eDstType == E_DRV_GOP_DST_IP1))
    {
        if ((fpXCIsInterlace != NULL) && (fpXCIsInterlace()))
        {
            (*h) *= 2;
            (*y) = (*y) * 2;
        }
    }
    else if (eDstType == E_DRV_GOP_DST_VE)
    {
        (*h) *= 2;
        (*y) = ((*y) * 2) - g_pGOPCtxLocal->pGopChipProperty->GOP_VE_V_Offset;
    }
    else if (eDstType == E_DRV_GOP_DST_OP1)
    {
        (*h) *= 2;
        (*y) = ((*y) * 2);
    }
    if ((eDstType == E_DRV_GOP_DST_OP0) || (eDstType == E_DRV_GOP_DST_FRC) || (eDstType == E_DRV_GOP_DST_BYPASS))
    {
        if (!MDrv_GOP_GWIN_IsProgressive(g_pGOPCtxLocal, gop))
        {
            (*h) *= 2;
            (*y) = (*y) * 2;
        }
    }
}

static void _GOP_SetOCCapability(void *pInstance, MS_U8 u8GOP, DRV_GOPDstType GopDst)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return;
    }

    switch (GopDst)
    {
    case E_DRV_GOP_DST_IP0:
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_OC_SetOCEn(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_VOP:
        MDrv_GOP_OC_SetOCEn(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_OP0:
    case E_DRV_GOP_DST_BYPASS:
    case E_DRV_GOP_DST_GOPScaling:
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);
        MDrv_GOP_OC_SetOCEn(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_FRC:
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);
        MDrv_GOP_OC_SetOCEn(g_pGOPCtxLocal, u8GOP, TRUE);
        break;

    default:
        break;
    }

}

#if (DUMP_INFO)
static void _GOP_GWIN_UpdateAllInfo(void *pInstance, )
{
    MS_U8 u8Index = 0,u16Index = 0;
    GOP_WinFB_INFO* pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE* psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance,(void*)&psGOPInstPri);
#endif
    printf("GwinNo.         CurFBId         IsShared        SharedCnt       ClientId\n");
    printf("========================================================================\n");
    for (u8Index = 0; u8Index<MAX_GWIN_SUPPORT; u8Index++)
    {
        printf("%02d\t\t%04ld\t\t%02d\t\t%04d\t\t%04ld\n",u8Index,g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u8Index].u32CurFBId,g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u8Index].bIsShared ,g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u8Index].u16SharedCnt,g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u8Index].u32GOPClientId);
    }
    printf("========================================================================\n");
    printf("FbNo.   in_use  addr            size            widthxheight    pitch   fmt  ClientId\n");
    printf("===========================================================================\n");
    for (u16Index = 0; u16Index<DRV_MAX_GWIN_FB_SUPPORT; u16Index++)
    {
        pwinFB = _GetWinFB(pInstance,u16Index);
        if(pwinFB)
        {
            printf("[%02d]\t%02d\t%08lx\t%08lx\t%04d x %04d\t%04d\t%02d\t%04ld\n",u16Index,pwinFB->in_use,pwinFB->addr,pwinFB->size,pwinFB->width,pwinFB->height,pwinFB->pitch,pwinFB->fbFmt,pwinFB->u32GOPClientId);
        }
    }
    printf("===========================================================================\n");
}
#endif

int GOP_SetWinInfo(void *pInstance, MS_U32 u32Gwin, GOP_GwinInfo *pinfo)
{
    MS_U8 u8GOP;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u32Gwin))
    {
        MS_CRITICAL_MSG(printf("\n[%s] not support gwin id:%d in this chip version", __FUNCTION__, (unsigned int)u32Gwin));
        return GOP_API_FAIL;
    }

    if (fpXCIsInterlace == NULL)
    {
        //APIGOP_ASSERT(FALSE, printf("\nfpXCIsInterlace is Null"));
        MS_DEBUG_MSG(printf("%s: fpXCIsInterlace is not hooked. Bypass switch...\n", __FUNCTION__));
        return GOP_API_FAIL;
    }

    u8GOP = _DumpGopByGwinId(pInstance, u32Gwin);


#if GFLIP_ENABLE
    //MDrv_GOP_GWIN_SendGwinInfo2Flip(g_pGOPCtxLocal, u8win, (DRV_GOP_GWIN_INFO*)pinfo);
#endif


    switch (u8GOP)
    {
    case E_GOP0:
        _SetGop0WinInfo(pInstance, u32Gwin, pinfo);
        break;
    case E_GOP1:
        _SetGop1WinInfo(pInstance, u32Gwin, pinfo);
        break;
    case E_GOP2:
    case E_GOP3:
    case E_GOP4:
    case E_GOP5:
        _SetGop23WinInfo(pInstance, u32Gwin, pinfo);
        break;
    default:
        break;
    }

    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);

    return GOP_API_SUCCESS;
}
int GOP_MapFB2Win(void *pInstance, MS_U32 u32fbId, MS_U32 GwinId)
{
    GOP_GwinInfo  gWin;
    MS_U32 u32Temp;
    GOP_WinFB_INFO *pwinFB;
    GOP_WinFB_INFO *pwinFB2;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (u32fbId >= DRV_MAX_GWIN_FB_SUPPORT)
    {
        msWarning(ERR_FB_ID_OUT_OF_RANGE);
        DEBUG_INFO(printf("MDrv_GOP_GWIN_MapFB2Win: fbId max already reached! FBId: %d, winId: %d\n",(unsigned int)u32fbId, (unsigned int)GwinId));
        return (GOP_API_FAIL);
    }

    if (!_GOP_IsGwinIdValid(pInstance, GwinId))
    {
        MS_CRITICAL_MSG(printf("\n[%s] not support gwin id:%d in this chip version",__FUNCTION__, (unsigned int)GwinId));
        return (GOP_API_FAIL);
    }
    memset(&gWin, 0, sizeof(GOP_GwinInfo));
    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        DEBUG_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return (GOP_API_FAIL);
    }
    //Add support to Map a Non-Created Gwin with a dedicated FB:
    if (!_GOP_GWIN_IsGwinCreated(pInstance, GwinId))
    {
        //Create GWin for The client:
        g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GwinId].u32CurFBId = u32fbId;

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
        g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GwinId].u32GOPClientId = getpid();
#else
        g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GwinId].u32GOPClientId= g_pGOPCtxLocal->u32GOPClientId;
#endif

        MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, GwinId, FALSE);

        pwinFB->enable = TRUE;
    }
    u32Temp = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GwinId].u32CurFBId;
    if (DRV_MAX_GWIN_FB_SUPPORT > u32Temp)
    {
        pwinFB2 = _GetWinFB(pInstance, u32Temp);

        if (pwinFB2 == NULL)
        {
            DEBUG_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32Temp));
            return (GOP_API_FAIL);
        }

        msWarning(ERR_GWIN_ID_ALREADY_MAPPED);
        DEBUG_INFO(printf("Warning: MDrv_GOP_GWIN_MapFB2Win: u8gwinId already assigned to fbId2(%d), fbId(%d)\n", (unsigned int)u32Temp, (unsigned int)u32fbId));
        pwinFB2->gWinId = GWIN_ID_INVALID;
    }

    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GwinId].u32CurFBId = u32fbId;

    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, GwinId))
    {
        msWarning(ERR_GWIN_ID_OUT_OF_RANGE);
        MS_CRITICAL_MSG( printf("[%s]: winId=%d is not in existence\n",__FUNCTION__,(unsigned int)GwinId));
        return (GOP_API_FAIL);
    }

    pwinFB->gWinId = GwinId;

    gWin.u16DispHPixelStart = pwinFB->x0;
    gWin.u16DispVPixelStart = pwinFB->y0;
    gWin.u16RBlkHPixSize = pwinFB->width;
    gWin.u16RBlkVPixSize = pwinFB->height;
    u32Temp = _GOP_CalcPitch(pInstance, pwinFB->fbFmt, pwinFB->width);
    if (pwinFB->pitch < u32Temp)
    {
        gWin.u16RBlkHRblkSize = (MS_U16)u32Temp; //FB's pitch is invalid
    }
    else
    {
        gWin.u16RBlkHRblkSize = pwinFB->pitch; //FB's pitch is ok
    }
    gWin.u16DispHPixelEnd = pwinFB->x1;
    gWin.u16DispVPixelEnd = pwinFB->y1;
    gWin.u32DRAMRBlkStart = pwinFB->addr;
    gWin.u16WinX = gWin.u16WinY = 0;
    gWin.clrType = pwinFB->fbFmt;

    GOP_SetWinInfo(pInstance, GwinId, &gWin);

    return GOP_API_SUCCESS;

}


int GOP_Set_Hscale(void *pInstance, MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!g_pGOPCtxLocal->pGopChipProperty->bPixelModeSupport)
    {
        if (src > dst)
        {
            WARNNING_INFO(printf("[%s][%d]GOP can not scaling down\n", __FUNCTION__, __LINE__));
            return GOP_API_FAIL;
        }
    }

    if (bEnable)
    {
        g_pGOPCtxLocal->pGOPCtxShared->u16HScaleDst[u8GOP] = dst;
    }
    else
    {
        g_pGOPCtxLocal->pGOPCtxShared->u16HScaleDst[u8GOP] = 0;
    }
    MDrv_GOP_GWIN_Set_HSCALE(g_pGOPCtxLocal,u8GOP, bEnable, src, dst);
    MDrv_GOP_Set3DOSD_Middle(g_pGOPCtxLocal, u8GOP);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
    return GOP_API_SUCCESS;
}

MS_U16 GOP_Get_Hscale(void *pInstance, MS_U32 u32GopId)
{
    MS_U16 HScaleRatio = SCALING_MULITPLIER;
    MS_U16 VScaleRatio = SCALING_MULITPLIER;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_Get_HVScaleRatio(g_pGOPCtxLocal, (MS_U8)u32GopId, &HScaleRatio, &VScaleRatio);
    return HScaleRatio;
}

MS_U16 GOP_Get_Vscale(void *pInstance, MS_U32 u32GopId)
{
    MS_U16 HScaleRatio = SCALING_MULITPLIER;
    MS_U16 VScaleRatio = SCALING_MULITPLIER;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_Get_HVScaleRatio(g_pGOPCtxLocal, (MS_U8)u32GopId, &HScaleRatio, &VScaleRatio);
    return VScaleRatio;
}

int GOP_Set_Vscale(void *pInstance, MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!g_pGOPCtxLocal->pGopChipProperty->bPixelModeSupport)
    {
        if (src > dst)
        {
            WARNNING_INFO(printf("[%s][%d]GOP can not scaling down\n", __FUNCTION__, __LINE__));
            return GOP_API_FAIL;
        }
    }

    if (bEnable)
    {
        g_pGOPCtxLocal->pGOPCtxShared->u16VScaleDst[u8GOP] = dst;
    }
    else
    {
        g_pGOPCtxLocal->pGOPCtxShared->u16VScaleDst[u8GOP] = 0;
    }
    MDrv_GOP_GWIN_Set_VSCALE(g_pGOPCtxLocal, u8GOP,bEnable, src, dst);
    MDrv_GOP_Set3DOSD_Middle(g_pGOPCtxLocal, u8GOP);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
    return GOP_API_SUCCESS;

}


MS_U32 GOP_GetFreeFBID(void *pInstance)
{
    MS_U32 i;
    MS_U32 u32FBId = 0xFFFFFFFF;


    for (i = 0; i < DRV_MAX_GWIN_FB_SUPPORT; i++)
    {
        if (!_GOP_IsFBExist(pInstance, i))
        {
            u32FBId = i;
            break;
        }
    }
    return u32FBId;
}

int GOP_SetStretchWin(void *pInstance, MS_U8 u8GOP_num, EN_GOP_DST_TYPE eDstType, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height)
{
    MS_U16 u16OrgWidth, u16Temp;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP_num))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP_num));
        return GOP_API_FAIL;
    }

    //Get real original hw Stretch win width
    _GOP_Get_StretchWin(pInstance, u8GOP_num, &u16Temp, &u16Temp, &u16OrgWidth, &u16Temp);
    _GOP_GWIN_Align_StretchWin(pInstance, u8GOP_num, eDstType, &x, &y, &width, &height, 16);
    //Store API use stretch window
    g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchWidth[u8GOP_num] = width;
    g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchHeight[u8GOP_num] = height;

    _GOP_GWIN_Adjust_3DStretchWin(pInstance, u8GOP_num, &x, &y, &width, &height);
    if (u16OrgWidth <= width) //Set a large one, set htt first
    {
        MDrv_GOP_GWIN_SetHTotal(g_pGOPCtxLocal, u8GOP_num, width);
    }
    MDrv_GOP_GWIN_SetStretchWin(g_pGOPCtxLocal, u8GOP_num, x, y, width, height);
    _GOP_InitHSPDByGOP(pInstance, u8GOP_num);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP_num);

    if (u16OrgWidth > width) //Set a small one, set htt after Gwin regsiter updated
    {
        MDrv_GOP_GWIN_SetHTotal(g_pGOPCtxLocal, u8GOP_num, width);
    }

    return GOP_API_SUCCESS;
}

int GOP_SetStretchWinPosition(void *pInstance, MS_U8 u8GOPNum, MS_U16 u16DispX, MS_U16 u16DispY)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MS_U16 u16SrcWidth;
    MS_U16 u16SrcHeight;
    MS_U16 u16StrenthWidth = g_pGOPCtxLocal->pGOPCtxShared->u16HScaleDst[u8GOPNum];
    MS_U16 u16StrenthHeight = g_pGOPCtxLocal->pGOPCtxShared->u16VScaleDst[u8GOPNum];
    MS_U16 u16PnlWidth = g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8GOPNum];
    MS_U16 u16PnlHeight = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8GOPNum];
    MS_U16 u16HStart = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[u8GOPNum];
    EN_GOP_DST_TYPE GopDst = MAX_GOP_DST_SUPPORT;
    DRV_GOPDstType enDst = E_DRV_GOP_DST_INVALID;
    MS_U16 u16Tmp = 0;

    (MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GOPNum, &enDst));
    //Fix me : tmp only for OP
    if (enDst != E_DRV_GOP_DST_OP0)
    {
        return GOP_API_SUCCESS;
    }

    MDrv_GOP_GWIN_Get_StretchWin(g_pGOPCtxLocal, u8GOPNum, &u16Tmp, &u16Tmp, &u16SrcWidth, &u16SrcHeight);

    if ((0 == u16StrenthWidth) || (0 == u16StrenthHeight))
    {
        MS_U16 u16HRatio = SCALING_MULITPLIER;
        MS_U16 u16VRatio = SCALING_MULITPLIER;
        MDrv_GOP_GWIN_Get_HVScaleRatio(g_pGOPCtxLocal, u8GOPNum, &u16HRatio, &u16VRatio);
        //get u16StrenthWidth or u16StrenthHeight from current ratio

        if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror && (0 != u16DispX))
        {
            // if x!=0 bHMirror = 1, up alignment the u16StrenthWidth, to make sure there is space lefted in the left of mirror panel
            u16StrenthWidth = (MS_U16)(((u16SrcWidth * SCALING_MULITPLIER) + u16HRatio) / u16HRatio);
        }
        else //None mirror or mirror+x=0
        {
            // if x=0 bHMirror = 1, down alignment the u16StrenthWidth, to make sure there is no space lefted in the left of mirror panel
            u16StrenthWidth = (MS_U16)((u16SrcWidth * SCALING_MULITPLIER) / u16HRatio);
        }
        if (g_pGOPCtxLocal->pGOPCtxShared->bVMirror && (0 != u16DispY))
        {
            // if y!=0 bVMirror = 1, up alignment the u16StrenthHeight, to make sure there is space lefted in the top of mirror panel
            u16StrenthHeight = (MS_U16)(((u16SrcHeight * SCALING_MULITPLIER) + u16VRatio) / u16VRatio);
        }
        else
        {
            // if y=0 bVMirror = 1, down alignment the u16StrenthHeight, to make sure there is no space lefted in the top of mirror panel
            u16StrenthHeight = (MS_U16)((u16SrcHeight * SCALING_MULITPLIER) / u16VRatio);
        }
    }

    if (g_pGOPCtxLocal->pGopChipProperty->enGOP3DType == E_DRV_3D_DUP_FULL)
    {
        switch (g_pGOPCtxLocal->pGOPCtxShared->GOP_StereoMode[u8GOPNum])
        {
        case E_DRV_GOP_3D_LINE_ALTERNATIVE:
        case E_DRV_GOP_3D_TOP_BOTTOM:
            u16StrenthHeight = u16StrenthHeight << 1;
            break;
        case E_DRV_GOP_3D_LINE_FRAMEPACKING:
            u16StrenthHeight = u16StrenthHeight << 1;
            u16StrenthHeight += g_pGOPCtxLocal->u16GOPGWinMiddle[u8GOPNum];
            break;
        case E_DRV_GOP_3D_SIDE_BY_SYDE:
            u16StrenthWidth = u16StrenthWidth << 1;
            break;
        default:
            break;
        }
    }
    //error protect: width < panel width
    if (u16PnlWidth < u16StrenthWidth)
    {
        u16DispX = 0;
        u16StrenthWidth = u16PnlWidth;
    }
    else
    {
        //error protect: x+width < panel width +Hstart
        if (u16PnlWidth + u16HStart < u16DispX + u16StrenthWidth)
        {
            // allow strench win out of panel limited by HTT with OP dst
            u16DispX = 0;
        }
    }
    if (g_pGOPCtxLocal->pGOPCtxShared->bHMirror)
    {
        MS_U16 u16RightEnd = u16DispX + u16StrenthWidth;
        // adjust x axis for mirror
        if (u16PnlWidth > u16RightEnd)
        {
            u16DispX = u16PnlWidth - u16RightEnd;
        }
        else
        {
            u16DispX = 0;
        }
    }

    //error protect: height < panel height
    if (u16PnlHeight < u16StrenthHeight)
    {
        u16DispY = 0;
        u16StrenthHeight = u16PnlHeight;
    }
    //error protect: y+height < panel height
    else if (u16PnlHeight < u16DispY + u16StrenthHeight)
    {
        u16DispY = 0;
    }
    if (g_pGOPCtxLocal->pGOPCtxShared->bVMirror)
    {
        // adjust y axis for mirror
        u16DispY = u16PnlHeight - u16DispY - u16StrenthHeight;
    }

    _GOP_Map_DRVDst2API_Enum_(pInstance, &GopDst, enDst);
    _GOP_GWIN_Align_StretchWin(pInstance, u8GOPNum, GopDst, &u16DispX, &u16DispY, &u16Tmp, &u16Tmp, 16);

    MDrv_GOP_GWIN_SetStretchWinPosition(g_pGOPCtxLocal, u8GOPNum, u16DispX, u16DispY);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOPNum);

    return GOP_API_SUCCESS;
}

MS_BOOL GOP_MultiSwitchByAddr(void *pInstance, PGOP_GWIN_FLIP_MULTI_WIN_INFO pMultiflip)
{
    MS_U8  u8GwinGOP, i = 0;
    MS_U32 u32GwinIdx = 0;
    GOP_WinFB_INFO *pwinFB;
    MS_GFLIP_MULTIINFO Multiinfo;
    MS_U8 u8MiuSel;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    Multiinfo.u8GopInfoCnt = pMultiflip->u8InfoCnt;
    for (i = 0; i < (pMultiflip->u8InfoCnt); i++)
    {
        if (!_GOP_IsGwinIdValid(pInstance, pMultiflip->FlipInfo[i].GwinId))
        {
            CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, pMultiflip->FlipInfo[i].GwinId));
            return FALSE;
        }

        u8GwinGOP = _DumpGopByGwinId(pInstance, pMultiflip->FlipInfo[i].GwinId);
        switch (u8GwinGOP)
        {
        case E_GOP0:
            u32GwinIdx = pMultiflip->FlipInfo[i].GwinId;
            break;
        case E_GOP1:
            u32GwinIdx = pMultiflip->FlipInfo[i].GwinId - MAX_GOP0_GWIN;
            break;
        case E_GOP2:
            u32GwinIdx = pMultiflip->FlipInfo[i].GwinId - GOP2_Gwin0Id;
            break;
        case E_GOP3:
            u32GwinIdx = pMultiflip->FlipInfo[i].GwinId - GOP3_Gwin0Id;
            break;
        case E_GOP4:
            u32GwinIdx = pMultiflip->FlipInfo[i].GwinId - GOP4_Gwin0Id;
            break;
        case E_GOP5:
            u32GwinIdx = pMultiflip->FlipInfo[i].GwinId - GOP5_Gwin0Id;
            break;
        default:
            CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GwinGOP));
            break;
        }

        Multiinfo.astGopInfo[i].u32GopIdx = (MS_U32)u8GwinGOP;
        Multiinfo.astGopInfo[i].u32GwinIdx = u32GwinIdx;
        Multiinfo.astGopInfo[i].u32MainAddr = pMultiflip->FlipInfo[i].FlipAddr;
        Multiinfo.astGopInfo[i].u32SubAddr = pMultiflip->FlipInfo[i].SubFlipAddr;
        Multiinfo.astGopInfo[i].u32TagId = pMultiflip->FlipInfo[i].WaitTagID;
        Multiinfo.astGopInfo[i].u32QEntry = *(pMultiflip->FlipInfo[i].pQueueCnt);
        Multiinfo.astGopInfo[i].u32Result = TRUE;

        if (g_pGOPCtxLocal->pGOPCtxShared->bGOP3DLRSwitch[u8GwinGOP])
        {
            //Do L/R switch
            MS_U32 u32Temp = Multiinfo.astGopInfo[i].u32MainAddr;
            Multiinfo.astGopInfo[i].u32MainAddr = Multiinfo.astGopInfo[i].u32SubAddr;
            Multiinfo.astGopInfo[i].u32SubAddr  = u32Temp;
        }

        if (u8GwinGOP > GOPTYPE.GOP0)
        {
            MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
            DRV_GOPDstType GopDst = E_DRV_GOP_DST_INVALID;
            MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GwinGOP, &GopDst);
            _GOP_GWIN_IsEnableMirror(pInstance, GopDst, &bHMirror, &bVMirror);
            if (bHMirror || bVMirror)
            {
                GOP_GwinInfo stGwinInfo;
                GOP_GetWinInfo(pInstance, pMultiflip->FlipInfo[i].GwinId, &stGwinInfo);
                stGwinInfo.u32DRAMRBlkStart = Multiinfo.astGopInfo[i].u32MainAddr;
                g_pGOPCtxLocal->sMirrorInfo[u32GwinIdx].u32NonMirrorFBAdr = stGwinInfo.u32DRAMRBlkStart;
                _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, pMultiflip->FlipInfo[i].GwinId, bHMirror, bVMirror, &stGwinInfo);
                Multiinfo.astGopInfo[i].u32MainAddr = stGwinInfo.u32DRAMRBlkStart;

                stGwinInfo.u32DRAMRBlkStart = Multiinfo.astGopInfo[i].u32SubAddr;
                _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, pMultiflip->FlipInfo[i].GwinId, bHMirror, bVMirror, &stGwinInfo);
                Multiinfo.astGopInfo[i].u32SubAddr = stGwinInfo.u32DRAMRBlkStart;

            }
        }
        //GOP HW just read the relative offset of each MIU
        _phy_to_miu_offset(u8MiuSel, Multiinfo.astGopInfo[i].u32MainAddr, Multiinfo.astGopInfo[i].u32MainAddr);

        //GOP HW just read the relative offset of each MIU
        _phy_to_miu_offset(u8MiuSel, Multiinfo.astGopInfo[i].u32SubAddr, Multiinfo.astGopInfo[i].u32SubAddr);

    }

    if (MDrv_GFLIP_SetMultiFlipInfo(g_pGOPCtxLocal, &Multiinfo))
    {
        for (i = 0; i < (pMultiflip->u8InfoCnt); i++)
        {
            if (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[pMultiflip->FlipInfo[i].GwinId].u32CurFBId < DRV_MAX_GWIN_FB_SUPPORT)
            {
                pwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[pMultiflip->FlipInfo[i].GwinId].u32CurFBId);
                if (pwinFB == NULL)
                {
                    return FALSE;
                }
                pwinFB->gWinId = GWIN_ID_INVALID;
            }
            *(pMultiflip->FlipInfo[i].pQueueCnt) = Multiinfo.astGopInfo[i].u32QEntry;
        }
        return TRUE;
    }
    return FALSE;
}

MS_BOOL GOP_SwitchByAddr(void *pInstance, MS_U32 gWinId, MS_U32 u32MainFlipAddr, MS_U32 u32SubFlipAddr, MS_U16 u32WaitTagID, MS_U32 *pU32QueueCnt, MS_BOOL b3DEn)
{
    MS_U8  u8GwinGOP;
    MS_U32 u32QueueCnt = 0;
    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, gWinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, gWinId));
        return FALSE;
    }

    u8GwinGOP = _DumpGopByGwinId(pInstance, gWinId);

    if (g_pGOPCtxLocal->pGOPCtxShared->bGOP3DLRSwitch[u8GwinGOP])
    {
        //Do L/R switch
        MS_U32 u32Temp = u32MainFlipAddr;
        u32MainFlipAddr = u32SubFlipAddr;
        u32SubFlipAddr  = u32Temp;
    }

    if (u8GwinGOP > GOPTYPE.GOP0)
    {
        MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
        DRV_GOPDstType GopDst = E_DRV_GOP_DST_INVALID;
        MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GwinGOP, &GopDst);
        _GOP_GWIN_IsEnableMirror(pInstance, GopDst, &bHMirror, &bVMirror);
        if (bHMirror || bVMirror)
        {
            GOP_GwinInfo stGwinInfo;
            GOP_GetWinInfo(pInstance, gWinId, &stGwinInfo);
            stGwinInfo.u32DRAMRBlkStart = u32MainFlipAddr;
            g_pGOPCtxLocal->sMirrorInfo[gWinId].u32NonMirrorFBAdr = u32MainFlipAddr;
            _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, gWinId, bHMirror, bVMirror, &stGwinInfo);
            u32MainFlipAddr = stGwinInfo.u32DRAMRBlkStart;

            stGwinInfo.u32DRAMRBlkStart = u32SubFlipAddr;
            _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, gWinId, bHMirror, bVMirror, &stGwinInfo);
            u32SubFlipAddr = stGwinInfo.u32DRAMRBlkStart;

        }
    }

    u32QueueCnt = *pU32QueueCnt;

    pwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gWinId].u32CurFBId);

    if (b3DEn == 0)
    {
        u32SubFlipAddr = 0;
    }

    if (MDrv_GOP_SwitchGWIN2Addr(g_pGOPCtxLocal, gWinId, u32MainFlipAddr, u32SubFlipAddr, u32WaitTagID, &u32QueueCnt))
    {
        if (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gWinId].u32CurFBId < DRV_MAX_GWIN_FB_SUPPORT)
            pwinFB->gWinId = GWIN_ID_INVALID;

        *pU32QueueCnt = (MS_U16)u32QueueCnt;
        return TRUE;
    }

    return FALSE;
}

MS_BOOL GOP_SwitchByFBID(void *pInstance, MS_U32 gWinId, MS_U32 u32fbId, MS_U32 u32WaitTagID, MS_U32 *pU32QueueCnt)
{
    MS_U8 u8GwinGop;
    MS_U32 u32FlipAddr;
    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, u32fbId));
        return FALSE;
    }
    if (!_GOP_IsGwinIdValid(pInstance, gWinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, gWinId));
        return FALSE;
    }
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, gWinId))
    {
        msWarning(ERR_FB_ID_OUT_OF_RANGE);
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is not exist\n", __FUNCTION__, __LINE__, gWinId));
        return FALSE;
    }

    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, u32fbId));
        return FALSE;
    }

    if (pwinFB->in_use == 0)
    {
        msWarning(ERR_FB_ID_NOT_ALLOCATED);
        CRITICAL_INFO(printf("%s: fbId=%ld is not in existence\n", __FUNCTION__, u32fbId));
        return FALSE;
    }

    u8GwinGop = _DumpGopByGwinId(pInstance, gWinId);
    u32FlipAddr = pwinFB->addr;

    if (u8GwinGop > GOPTYPE.GOP0)
    {
        MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
        DRV_GOPDstType GopDst = E_DRV_GOP_DST_INVALID;
        MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GwinGop, &GopDst);
        _GOP_GWIN_IsEnableMirror(pInstance, GopDst, &bHMirror, &bVMirror);
        if (bHMirror || bVMirror)
        {
            GOP_GwinInfo stGwinInfo;
            GOP_GetWinInfo(pInstance, gWinId, &stGwinInfo);
            stGwinInfo.u32DRAMRBlkStart = u32FlipAddr;
            g_pGOPCtxLocal->sMirrorInfo[gWinId].u32NonMirrorFBAdr = u32FlipAddr;
            _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, gWinId, bHMirror, bVMirror, &stGwinInfo);
            u32FlipAddr = stGwinInfo.u32DRAMRBlkStart;
        }
    }
    if (MDrv_GOP_SwitchGWIN2Addr(g_pGOPCtxLocal, gWinId, u32FlipAddr, (MS_U32)0, u32WaitTagID, pU32QueueCnt))
    {
        GOP_WinFB_INFO *pCurwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gWinId].u32CurFBId);

        if (pCurwinFB == NULL)
        {
            CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gWinId].u32CurFBId));
            return FALSE;
        }

        if (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gWinId].u32CurFBId < DRV_MAX_GWIN_FB_SUPPORT)
            pCurwinFB->gWinId = GWIN_ID_INVALID;

        g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gWinId].u32CurFBId = u32fbId;
        return TRUE;
    }

    return FALSE;
}


int GOP_MIUSel(void *pInstance, MS_U8 u8GOP, EN_GOP_SEL_TYPE MiuSel)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_MIU_Sel(g_pGOPCtxLocal, u8GOP, (E_DRV_GOP_SEL_TYPE)MiuSel);

    return GOP_API_SUCCESS;
}

MS_U8 GOP_GetMIUSel(void *pInstance, MS_U8 u8GOP)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    return MDrv_GOP_Get_MIU_Sel(g_pGOPCtxLocal, u8GOP);

}

int GOP_SetAlphaInverse(void *pInstance, MS_U32 gop, MS_BOOL bEn)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, gop, bEn);
    return GOP_API_SUCCESS;
}
MS_BOOL GOP_GetAlphaInverse(void *pInstance, MS_U32 gop)
{
    MS_BOOL bEn;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    MDrv_GOP_GWIN_GetAlphaInverse(g_pGOPCtxLocal,gop, &bEn);
    return GOP_API_SUCCESS;
}

int GOP_SetContrast(void *pInstance, MS_U8 u8GOP, MS_U16 u16YContrast, MS_U16 u16UContrast, MS_U16 u16VContrast)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    enRet = (E_GOP_API_Result)MDrv_GOP_SetGOPContrast(g_pGOPCtxLocal, u8GOP, u16YContrast, u16UContrast, u16VContrast);

    return enRet;
}

int GOP_GetContrast(void *pInstance, MS_U8 u8GOP, MS_U32 *YContrast, MS_U32 *UContrast, MS_U32 *VContrast)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    enRet = (E_GOP_API_Result)MDrv_GOP_GetGOPContrast(g_pGOPCtxLocal, u8GOP, YContrast, UContrast, VContrast);
    return enRet;
}



int GOP_SetBrightness(void *pInstance, MS_U8 u8GOP, MS_U16 u16BriVal, MS_BOOL bMSB)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    enRet = (E_GOP_API_Result)MDrv_GOP_SetGOPBrightness(g_pGOPCtxLocal, u8GOP, u16BriVal, bMSB);

    return enRet;
}

int GOP_GetBrightness(void *pInstance, MS_U8 u8GOP, MS_U16 *u16BriVal, MS_BOOL *bMSB)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    enRet = (E_GOP_API_Result)MDrv_GOP_GetGOPBrightness(g_pGOPCtxLocal, u8GOP, u16BriVal, bMSB);

    return enRet;
}


int GOP_SetOutputColor(void *pInstance, MS_U8 u8GOP, EN_GOP_OUTPUT_COLOR type)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[u8GOP] = (MS_S32)type;
    MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, (GOP_OupputColor)type);

    return GOP_API_SUCCESS;
}

int GOP_SetUVSwap(void *pInstance, MS_U8 u8GOP, MS_BOOL bEn)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }

    MDrv_GOP_SetUVSwap(g_pGOPCtxLocal, u8GOP, bEn);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);

    return GOP_API_SUCCESS;

}


int GOP_SetYCSwap(void *pInstance, MS_U8 u8GOP, MS_BOOL bEn)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }


    MDrv_GOP_SetYCSwap(g_pGOPCtxLocal, u8GOP, bEn);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);

    return GOP_API_SUCCESS;

}

int GOP_SetProgressive(void *pInstance, MS_U8 gop, MS_BOOL bEn)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, gop, bEn);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, gop);

    return GOP_API_SUCCESS;
}

int GOP_SetHStart(void *pInstance, MS_U8 u8GOP, MS_U16 u16PanelHStr)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }

    g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[u8GOP] = u16PanelHStr;
    _GOP_InitHSPDByGOP(pInstance, u8GOP);

    return GOP_API_SUCCESS;

}

MS_U32 GOP_SetPanelTiming(void *pInstance, MS_U8 u8GOP, PST_GOP_TIMING_INFO pstInfo)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }

    g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[u8GOP] = pstInfo->u16DEHStart;
    g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8GOP] = pstInfo->u16DEHSize;
    g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8GOP] = pstInfo->u16DEVSize;
    _GOP_InitHSPDByGOP(pInstance, u8GOP);
    return GOP_API_SUCCESS;
}

MS_U32 GOP_GetPanelTiming(void *pInstance, MS_U8 u8GOP, PST_GOP_TIMING_INFO pstInfo)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }

    pstInfo->u16DEHStart = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[u8GOP];
    pstInfo->u16DEHSize = g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8GOP];
    pstInfo->u16DEVSize = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8GOP];
    return GOP_API_SUCCESS;
}

int GOP_CreateBuffer(void *pInstance, PGOP_BUFFER_INFO  pBuff, MS_U32 fbid)
{
    _GOP_GWIN_CreatePoolFB(pInstance, fbid, pBuff->disp_rect.x, pBuff->disp_rect.y, pBuff->disp_rect.w, pBuff->disp_rect.h, pBuff->fbFmt, E_GOP_FB_NULL);
    return GOP_API_SUCCESS;
}
int GOP_CreateBufferByAddr(void *pInstance, PGOP_BUFFER_INFO  pBuff, MS_U32 u32fbId)
{
    MS_U32 size;
    GOP_WinFB_INFO *pwinFB;
    MS_U32 width;
    MS_U32 height;
    MS_U32 pitch;
    MS_U32 fbFmt;
    MS_U32 u32FbAddr;
    MS_U32 dispX, dispY;


    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GWIN_NO_AVAILABLE;
    }

    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GWIN_NO_AVAILABLE;
    }

    if (pwinFB->in_use)
    {
        msWarning(ERR_FB_ID_ALREADY_ALLOCATED);
        CRITICAL_INFO( printf("[%s][%d]FbId %d is already allocated\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GWIN_NO_AVAILABLE;

    }

    DEBUG_INFO(printf("MDrv_GOP_GWIN_CreateFB: new gwinFBId = %d \n", (unsigned int)u32fbId));


    width     = pBuff->width;
    height    = pBuff->height;
    pitch     = pBuff->pitch;
    fbFmt     = pBuff->fbFmt;
    dispX     = pBuff->disp_rect.x;
    dispY     = pBuff->disp_rect.y;
    u32FbAddr = pBuff->addr;

    size =  _GOP_CalcPitch(pInstance, fbFmt, width) * height;

    pwinFB->enable = FALSE;
    pwinFB->in_use = 1;
    pwinFB->x0 = dispX;
    pwinFB->y0 = dispY;
    pwinFB->width = width;
    pwinFB->height = height;
    pwinFB->x1 = pwinFB->x0 + width;
    pwinFB->y1 = pwinFB->y0 + height;
    pwinFB->addr  = u32FbAddr; //addr;
    pwinFB->size  = size;

    // FB format
    pwinFB->fbFmt  = (fbFmt != FB_FMT_AS_DEFAULT) ? fbFmt : E_MS_FMT_ARGB1555; //g_pGOPCtxLocal->winFB[g_pGOPCtxLocal->pGOPCtxShared->gwinMap[GWIN_OSD_DEFAULT]].fbFmt;
    if (pitch == 0)
    {
        pwinFB->pitch = _GOP_CalcPitch(pInstance, fbFmt, width);
    }
    else
    {
        pwinFB->pitch = pitch;
    }
    // update ptrs
    pwinFB->poolId = GOP_WINFB_POOL_NULL;
    pwinFB->nextFBIdInPool = INVALID_POOL_NEXT_FBID;
    pwinFB->string = E_GOP_FB_NULL;
    pwinFB->u32GOPClientId = getpid();
    DEBUG_INFO(printf("\33[0;36m   %s:%d   FBId = %ld \33[m \n", __FUNCTION__, __LINE__, u32fbId));

    return GOP_API_SUCCESS;
}


int GOP_FB_Destroy(void *pInstance, MS_U32 u32fbId)
{
    GOP_WinFB_INFO *pwinFB;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        return GWIN_FAIL;
    }
    switch (pwinFB->poolId)
    {
    case GOP_WINFB_POOL1:
        _GOP_FreePoolMem(pInstance, &g_pGOPCtxLocal->pGOPCtxShared->fbPool1, u32fbId);
        break;
    case GOP_WINFB_POOL2:
        _GOP_FreePoolMem(pInstance, &g_pGOPCtxLocal->pGOPCtxShared->fbPool1, u32fbId);
        break;
    default:
        break;
    }

    pwinFB->poolId = GOP_WINFB_POOL_NULL;
    pwinFB->enable = FALSE;
    pwinFB->in_use = 0;
    pwinFB->x0 = 0;
    pwinFB->y0 = 0;
    pwinFB->x1 = 0;
    pwinFB->y1 = 0;

    pwinFB->gWinId = GWIN_ID_INVALID;
    pwinFB->width = 0;
    pwinFB->height = 0;
    pwinFB->pitch = 0;
    pwinFB->fbFmt = 0;
    pwinFB->addr  = 0;
    pwinFB->size  = 0;
    pwinFB->dispHeight = 0;
    pwinFB->dispWidth  = 0;
    pwinFB->s_x = 0;
    pwinFB->s_y = 0;
    pwinFB->string = E_GOP_FB_NULL;
    pwinFB->u32GOPClientId = INVALID_CLIENT_PID;

    return GWIN_OK;

}
int GOP_Win_Destroy(void *pInstance, MS_U32 gId)
{
    MS_U32 u32fbId;
    GOP_GwinInfo gWin;
    GOP_WinFB_INFO *pwinFB;
    MS_U8 u8GOP, u8MIU;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    DEBUG_INFO(printf("\33[0;36m   %s:%d   gId = %ld \33[m \n", __FUNCTION__, __LINE__, gId));

    if (!_GOP_IsGwinIdValid(pInstance, gId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, gId));
        return GOP_API_FAIL;
    }
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, gId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is not exist\n", __FUNCTION__, __LINE__, gId));
        return GOP_API_FAIL;
    }

    MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, gId, FALSE);
    u32fbId = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId;

    pwinFB = _GetWinFB(pInstance, u32fbId);
    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }

    if ((GOP_WINFB_POOL_NULL != pwinFB->poolId) && (GOP_FB_Destroy(pInstance, u32fbId) != GWIN_OK))
    {
        MS_DEBUG_MSG(printf("MDrv_GOP_GWIN_DestroyWin(Id....), failed to delete FB %ld\n", u32fbId));

    }

    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId = GWIN_ID_INVALID;
    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32GOPClientId = 0;
    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].bIsShared = FALSE;
    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u16SharedCnt = 0;

    memset(&gWin, 0, sizeof(GOP_GwinInfo));
    gWin.clrType = E_GOP_COLOR_ARGB8888;
    u8GOP = _DumpGopByGwinId(pInstance, (gId));
    u8MIU = MDrv_GOP_Get_MIU_Sel(g_pGOPCtxLocal, u8GOP);
    _miu_offset_to_phy(u8MIU, gWin.u32DRAMRBlkStart, gWin.u32DRAMRBlkStart);

    GOP_SetWinInfo(pInstance, gId, &gWin);

    RESET_BIT(g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr, gId);
    RESET_BIT(g_pGOPCtxLocal->u16GOP_HMirrorRBLK_Adr, gId);
    RESET_BIT(g_pGOPCtxLocal->u16GOP_HMirror_HPos, gId);
    RESET_BIT(g_pGOPCtxLocal->u16GOP_VMirror_VPos, gId);

    MDrv_GOP_GWIN_ClearFlipQueue(g_pGOPCtxLocal, gId);
    return GOP_API_SUCCESS;

}

int GOP_Win_Destroy_Abnormal(void *pInstance, MS_U32 gId)
{
    MS_U32 u32fbId;
    GOP_GwinInfo gWin;
    GOP_WinFB_INFO *pwinFB;
    MS_U8 u8GOP, u8MIU;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    DEBUG_INFO(printf("\33[0;36m   %s:%d   gId = %d \33[m \n",__FUNCTION__,__LINE__,(unsigned int)gId));

    if (!_GOP_IsGwinIdValid(pInstance, gId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)gId));
        return GOP_API_FAIL;
    }

    MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, gId, FALSE);
    u32fbId = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId;

    pwinFB = _GetWinFB(pInstance, u32fbId);
    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return GOP_API_FAIL;
    }

    if ((GOP_WINFB_POOL_NULL != pwinFB->poolId) && (GOP_FB_Destroy(pInstance, u32fbId) != GWIN_OK))
    {
        MS_DEBUG_MSG( printf( "MDrv_GOP_GWIN_DestroyWin(Id....), failed to delete FB %d\n", (unsigned int)u32fbId ) );

    }

    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId = GWIN_ID_INVALID;
    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32GOPClientId = 0;
    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].bIsShared = FALSE;
    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u16SharedCnt = 0;

    memset(&gWin, 0, sizeof(GOP_GwinInfo));
    gWin.clrType = E_GOP_COLOR_ARGB8888;
    u8GOP = _DumpGopByGwinId(pInstance, (gId));
    u8MIU = MDrv_GOP_Get_MIU_Sel(g_pGOPCtxLocal, u8GOP);
    _miu_offset_to_phy(u8MIU, gWin.u32DRAMRBlkStart, gWin.u32DRAMRBlkStart);

    GOP_SetWinInfo(pInstance, gId, &gWin);

    RESET_BIT(g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr, gId);
    RESET_BIT(g_pGOPCtxLocal->u16GOP_HMirrorRBLK_Adr, gId);
    RESET_BIT(g_pGOPCtxLocal->u16GOP_HMirror_HPos, gId);
    RESET_BIT(g_pGOPCtxLocal->u16GOP_VMirror_VPos, gId);

    MDrv_GOP_GWIN_ClearFlipQueue(g_pGOPCtxLocal, gId);
    return GOP_API_SUCCESS;

}


void GOP_ResetGOP(void *pInstance, MS_U8 u8GOP)
{
    MS_U32 i = 0, j = 0;
    MS_U8 u8GwinId;
    GOP_WinFB_INFO *pwinFB;
    E_GOP_API_Result enRet = GOP_API_FAIL;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (g_pGOPCtxLocal == NULL)
    {
        return;
    }
    u8GwinId = _GOP_SelGwinId2(pInstance, u8GOP, 0);
    for (i = u8GwinId; i < u8GwinId + MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, u8GOP); i++)
    {
        //Check and release APP's GWIN resource
        if (((FALSE == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].bIsShared)
             && (DRV_MAX_GWIN_FB_SUPPORT > g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].u32CurFBId))
            || ((TRUE == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].bIsShared)
                && (0x0 == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].u16SharedCnt)))

        {
            for (j = 0; j < DRV_MAX_GWIN_FB_SUPPORT; j++)
            {
                pwinFB = _GetWinFB(pInstance, j);
                if (pwinFB != NULL)
                {
                    if ((pwinFB->u32GOPClientId == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].u32GOPClientId)
                        && (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].u32CurFBId != j))
                    {
                        enRet = (E_GOP_API_Result)GOP_FB_Destroy(pInstance, j);
                        printf("\n---%s %d: Try DestroyWIN's FB[%u], return[%u]\n", __FUNCTION__, __LINE__, (unsigned int)j, (MS_U8)enRet);
                    }
                }
            }
            if (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].u32GOPClientId != 0)
            {
                enRet = GOP_Win_Destroy_Abnormal(pInstance, i);
                printf("\n---%s %d: Try DestroyWIN[%u], return[%u]\n", __FUNCTION__, __LINE__, (unsigned int)i, (MS_U8)enRet);
            }
        }
    }
}

static void GOP_GWIN_InitByGOP(void *pInstance, MS_BOOL bFirstIntance, MS_U8 u8GOP)
{
    MS_U8 i;
    MS_U8 bytePerPix = 2;
    MS_U16 width = 0, height = 0, pitch = 0, fbFmt = 0;
    MS_U32 addr = 0;
    MS_U32 u32index;
    GOP_GwinInfo gWin;
    GOP_WinFB_INFO *pwinFB;
    MS_U32 MaxGwinSupport;
    MS_U8 u8MIU;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    //GE_BufInfo buffinfo;
    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return;
    }
    DEBUG_INFO(printf("MDrv_GOP_GWIN_Init\n"));
    memset(&gWin, 0, sizeof(GOP_GwinInfo));
    if (g_pGOPCtxLocal->pGOPCtxShared->bInitShared)
    {
        g_pGOPCtxLocal->pGOPCtxShared->bInitShared = FALSE;
        //First init of whole GOP Driver
        for (u32index = 0; u32index < DRV_MAX_GWIN_FB_SUPPORT; u32index++)
        {
            pwinFB = _GetWinFB(pInstance, u32index);

            if (pwinFB == NULL)
            {
                CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32index));
                return;
            }

            pwinFB->enable = FALSE;
            pwinFB->in_use = 0;
            pwinFB->gWinId = GWIN_ID_INVALID; // orig GWIN_OSD_DEFAULT;
            pwinFB->x0 = gWin.u16DispHPixelStart = 0;
            pwinFB->y0 = gWin.u16DispVPixelStart = 0;
            pwinFB->width = gWin.u16RBlkHPixSize = 0;
            pwinFB->height = gWin.u16RBlkVPixSize = 0;
            pwinFB->x1 = gWin.u16DispHPixelEnd = 0;
            pwinFB->y1 = gWin.u16DispVPixelEnd = 0;
            pwinFB->pitch  = 0;
            pwinFB->addr  = gWin.u32DRAMRBlkStart = 0;
            pwinFB->size   = 0;
            gWin.u16WinX = gWin.u16WinY = 0;
            // FB format
            pwinFB->fbFmt  = FB_FMT_AS_DEFAULT;
            // update ptrs
            pwinFB->poolId = GOP_WINFB_POOL_NULL;
            pwinFB->nextFBIdInPool = INVALID_POOL_NEXT_FBID;
            pwinFB->string = E_GOP_FB_NULL;
            pwinFB->u32GOPClientId = INVALID_CLIENT_PID;
        }
    }
    if (FALSE == g_pGOPCtxLocal->pGOPCtxShared->bGopHasInitialized[u8GOP])
    {
        memset(&gWin, 0, sizeof(GOP_GwinInfo));
        MS_BOOL bGwinEnable = FALSE;
        for (i = 0; i < MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, u8GOP); i++)
        {
            u8MIU = MDrv_GOP_Get_MIU_Sel(g_pGOPCtxLocal, u8GOP);
            _miu_offset_to_phy(u8MIU, gWin.u32DRAMRBlkStart, gWin.u32DRAMRBlkStart);
            u32index = _GOP_SelGwinId2(pInstance, u8GOP, i);
            g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32index].u32CurFBId = GWIN_ID_INVALID;
            gWin.clrType = (EN_GOP_COLOR_TYPE)E_GOP_COLOR_ARGB1555;
            //Disable all Gwin when GOP first init
            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_GWIN))
            {
                MDrv_GOP_GWIN_IsGWINEnabled(g_pGOPCtxLocal, u32index, &bGwinEnable);
                if (bGwinEnable == TRUE)
                {
                    //GWIN must be turn off in Vsync to avoid garbage
                    MDrv_GOP_GWIN_SetForceWrite(g_pGOPCtxLocal, FALSE);
                    MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, u32index, FALSE);
                    MDrv_GOP_GWIN_SetForceWrite(g_pGOPCtxLocal, TRUE);
                }
                GOP_SetWinInfo(pInstance, u32index, &gWin);
                MDrv_GOP_GWIN_SetBlending(g_pGOPCtxLocal, u32index, FALSE, 0x0);
            }
        }
    }
    /*
    width = MDrv_Read2Byte(GE_REG_STBB_S_W+GE_REG_BASE)+1;
    height = MDrv_Read2Byte(GE_REG_STBB_S_H+GE_REG_BASE)+1;
    pitch = MDrv_Read2Byte(GE_REG_DB_PIT+GE_REG_BASE);
    fbFmt = MDrv_Read2Byte(GE_REG_B_FM+GE_REG_BASE);
    addr = MDrv_Read4Byte(GE_REG_DB_BASE0+GE_REG_BASE);
    MDrv_GE_GetFrameBufferInfo(&width, &height, &pitch, &fbFmt, &addr);
    */
    //MDrv_GE_GetBufferInfo(&buffinfo);
    pwinFB = _GetWinFB(pInstance, DRV_MAX_GWIN_FB_SUPPORT);
    MaxGwinSupport = g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum;

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n", __FUNCTION__, __LINE__, DRV_MAX_GWIN_FB_SUPPORT));
        return;
    }

    pwinFB->enable = TRUE;
    pwinFB->in_use = 1;
    pwinFB->gWinId = MaxGwinSupport;
    pwinFB->x0 = 0;
    pwinFB->y0 = 0;
    pwinFB->width = width;
    pwinFB->height = height;
    pwinFB->x1  = pwinFB->x0 + pwinFB->width;
    pwinFB->y1  = pwinFB->y0 + pwinFB->height;
    pwinFB->pitch = (pitch & 0xFFF8);  // pitch must be 4-pix alignment;
    pwinFB->addr   = addr;
    pwinFB->size  =  ((MS_U32)width) * ((MS_U32)height) * ((MS_U32)bytePerPix);
    // FB format
    pwinFB->fbFmt  = fbFmt;
    // update ptrs
    pwinFB->poolId = GOP_WINFB_POOL_NULL;
    pwinFB->nextFBIdInPool = INVALID_POOL_NEXT_FBID;
    pwinFB->string = E_GOP_FB_NULL;
    pwinFB->u32GOPClientId = INVALID_CLIENT_PID;

    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[MaxGwinSupport].u32CurFBId = DRV_MAX_GWIN_FB_SUPPORT;

}


int GOP_GWIN_OutputLayerSwitch(void *pInstance, MS_U8 u8GOP)
{
    E_GOP_API_Result GOPRet;


//#ifndef MSOS_TYPE_LINUX
    MS_U8 i;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }

    GOPRet = (E_GOP_API_Result)MDrv_GOP_SetGOPHighPri(g_pGOPCtxLocal, u8GOP);
    for (i = 0; i < _GOP_GetMaxGOPNum(pInstance); i++)
    {
        _GOP_InitHSPDByGOP(pInstance, i);
    }

    return GOPRet;
}
int GOP_GetCurrentGOP(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    return MDrv_GOP_Get(g_pGOPCtxLocal);
}

int GOP_GetCurrentGWin(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    return g_pGOPCtxLocal->current_winId;
}

int GOP_GetCurrentFBID(void *pInstance)
{
    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MS_U8 curWinId = GOP_GetCurrentGWin(pInstance);
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, curWinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is not exist\n", __FUNCTION__, __LINE__, curWinId));
        return GWIN_ID_INVALID;
    }

    pwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[curWinId].u32CurFBId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[curWinId].u32CurFBId));
        return GWIN_ID_INVALID;
    }
    if (!_GOP_IsFbIdValid(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[curWinId].u32CurFBId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[curWinId].u32CurFBId));
        return GWIN_ID_INVALID;
    }

    if (0 == pwinFB->in_use)
    {
        CRITICAL_INFO( printf("[%s][%d]FbId %d is not allocated\n",__FUNCTION__,__LINE__,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[curWinId].u32CurFBId));
        return GWIN_ID_INVALID;

    }
    return g_pGOPCtxLocal->pGOPCtxShared->gwinMap[curWinId].u32CurFBId;

}


int GOP_GWIN_SetHMirror(void *pInstance, MS_BOOL bEnable)
{
    MS_U8 u8GwinID = 0, u8GWinBase = 0, u8GOPWinNum = 0, u8GOPIdx = 0;
    MS_U8 u8MaxGOPGwinNum =0;
    MS_U16 u16GwinState = 0;
    GOP_GwinInfo sGWININfo;
    MS_BOOL bUpdateRegOnce = FALSE, bHMirror = FALSE, bVMirror = FALSE;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    bUpdateRegOnce = g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE];
    g_pGOPCtxLocal->pGOPCtxShared->bHMirror = bEnable;

    for (u8GOPIdx = 0; u8GOPIdx < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); u8GOPIdx++)
    {
        MDrv_GOP_IsGOPMirrorEnable(g_pGOPCtxLocal, u8GOPIdx, &bHMirror, &bVMirror);
        if (bHMirror == bEnable)
        {
            continue; //Duplicate mirror setting, skip it
        }

        //Turn off all on state GWIN
        u8GOPWinNum = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, u8GOPIdx);
        u8GWinBase = _GOP_SelGwinId2(pInstance, u8GOPIdx, 0);
        u16GwinState = 0; //Clear GWIN on/off state
        u8MaxGOPGwinNum = u8GWinBase+u8GOPWinNum;

        if(u8MaxGOPGwinNum > (MS_U8)g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum)
        {
            CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n",__FUNCTION__,__LINE__,u8GwinID));
            return GOP_API_FAIL;
        }

        for (u8GwinID=u8GWinBase; u8GwinID < u8MaxGOPGwinNum; u8GwinID++)
        {
            if (!_GOP_IsGwinIdValid(pInstance, u8GwinID))
            {
                CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8GwinID));
                return GOP_API_FAIL;
            }

            if (g_pGOPCtxLocal->pGOPCtxShared->bGWINEnable[u8GwinID] != TRUE)
            {
                continue;
            }

            if (_GOP_GWIN_IsGwinExistInClient(pInstance, u8GwinID) == TRUE)
            {
                MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, u8GwinID, FALSE);
                u16GwinState |= (1 << u8GwinID);
            }
            else
            {
                u16GwinState = 0xFFFF;
                break; //This GWIn does not exist in current client, skip gop mirror setting
            }
        }

        if (u16GwinState == 0xFFFF)
        {
            continue; //This GWIn does not exist in current client, skip gop mirror setting
        }

        //Enable mirror
        MDrv_GOP_GWIN_EnableHMirror(g_pGOPCtxLocal, u8GOPIdx, bEnable);
        //Reset ON state GWIN info after mirror enable
        if (u16GwinState != 0)
        {
            g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = TRUE; //Update GWIN info and GWIN enable one time
            for (u8GwinID = u8GWinBase; u8GwinID < u8GWinBase + u8GOPWinNum; u8GwinID++)
            {
                if (u16GwinState & (1 << u8GwinID))
                {
                    if (GOP_GetWinInfo(pInstance, u8GwinID, &sGWININfo) == TRUE)
                    {
                        GOP_SetWinInfo(pInstance, u8GwinID, &sGWININfo);
                    }
                    MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, u8GwinID, TRUE);
                }
            }
            g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = bUpdateRegOnce;
        }
        MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOPIdx);
    }

    if (bEnable == FALSE)
    {
        g_pGOPCtxLocal->u16GOP_HMirror_HPos = 0;
        g_pGOPCtxLocal->u16GOP_HMirrorRBLK_Adr = 0;
    }
    return GOP_API_SUCCESS;
}


int GOP_GWIN_SetVMirror(void *pInstance, MS_BOOL bEnable)
{
    MS_U8 u8GwinID = 0, u8GWinBase = 0, u8GOPWinNum = 0, u8GOPIdx = 0;
    MS_U8 u8MaxGOPGwinNum =0;
    MS_U16 u16GwinState = 0;
    GOP_GwinInfo sGWININfo;
    MS_BOOL bUpdateRegOnce = FALSE, bHMirror = FALSE, bVMirror = FALSE;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    bUpdateRegOnce = g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE];
    g_pGOPCtxLocal->pGOPCtxShared->bVMirror = bEnable;

    for (u8GOPIdx = 0; u8GOPIdx < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); u8GOPIdx++)
    {
        MDrv_GOP_IsGOPMirrorEnable(g_pGOPCtxLocal, u8GOPIdx, &bHMirror, &bVMirror);
        if (bVMirror == bEnable)
        {
            continue; //Duplicate mirror setting, skip it
        }

        u8GOPWinNum = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, u8GOPIdx);
        u8GWinBase = _GOP_SelGwinId2(pInstance, u8GOPIdx, 0);
        u16GwinState = 0; //Clear GWIN on/off state
        u8MaxGOPGwinNum = u8GWinBase+u8GOPWinNum;
        if(u8MaxGOPGwinNum > (MS_U8)g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum)
        {
            CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n",__FUNCTION__,__LINE__,u8GwinID));
            return GOP_API_FAIL;
        }

                          //Turn off all on state GWIN
        for (u8GwinID=u8GWinBase; u8GwinID < u8MaxGOPGwinNum; u8GwinID++)
        {
            if (!_GOP_IsGwinIdValid(pInstance, u8GwinID))
            {
                CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8GwinID));
                return GOP_API_FAIL;
            }

            if (g_pGOPCtxLocal->pGOPCtxShared->bGWINEnable[u8GwinID] != TRUE)
            {
                continue;
            }

            if (_GOP_GWIN_IsGwinExistInClient(pInstance, u8GwinID) == TRUE)
            {
                MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, u8GwinID, FALSE);
                u16GwinState |= (1 << u8GwinID);
            }
            else
            {
                u16GwinState = 0xFFFF;
                break; //This GWIn does not exist in current client, skip gop mirror setting
            }
        }

        if (u16GwinState == 0xFFFF)
        {
            continue; //There is GWIn does not exist in current client, skip gop mirror setting
        }

        //Enable mirror
        MDrv_GOP_GWIN_EnableVMirror(g_pGOPCtxLocal, u8GOPIdx, bEnable);
        //Reset ON state GWIN info after mirror enable
        if (u16GwinState != 0)
        {
            g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = TRUE; //Update GWIN info and GWIN enable one time
            for (u8GwinID = u8GWinBase; u8GwinID < u8GWinBase + u8GOPWinNum; u8GwinID++)
            {
                if (u16GwinState & (1 << u8GwinID))
                {
                    if (GOP_GetWinInfo(pInstance, u8GwinID, &sGWININfo) == TRUE)
                    {
                        GOP_SetWinInfo(pInstance, u8GwinID, &sGWININfo);
                    }
                    MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, u8GwinID, TRUE);
                }
            }
            g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = bUpdateRegOnce;
        }
        MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOPIdx);
    }

    if (bEnable == FALSE)
    {
        g_pGOPCtxLocal->u16GOP_VMirrorRBLK_Adr = 0;
        g_pGOPCtxLocal->u16GOP_VMirror_VPos = 0;
    }

    return GOP_API_SUCCESS;

}

int GOP_GWIN_SetNewAlphaMode(void *pInstance, MS_U8 gwin, MS_BOOL bEn)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, gwin))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, gwin));
        return GOP_API_FAIL;
    }

    MDrv_GOP_GWIN_SetNewAlphaMode(g_pGOPCtxLocal, gwin, bEn);

    return GOP_API_SUCCESS;

}

int GOP_GWIN_IsEnable(void *pInstance, MS_U8 winId)
{
    MS_BOOL IsEnable = FALSE;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, winId));
        return FALSE;
    }
    MDrv_GOP_GWIN_IsGWINEnabled(g_pGOPCtxLocal, winId, &IsEnable);
    return IsEnable;
}

int GOP_GWIN_SetEnable(void *pInstance, MS_U8 winId, MS_BOOL bEnable)
{
    MS_U16 regval2 = 0, i;
    MS_U32 u32fbId;
    GOP_WinFB_INFO *pwinFB;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif


    if (fpXCReduceBWForOSD == NULL)
    {
        APIGOP_ASSERT(TRUE, printf("\nfpXCReduceBWForOSD is Null"));
        MS_DEBUG_MSG(printf("%s: fpXCReduceBWForOSD is not hooked. Bypass switch...\n", __FUNCTION__));
    }
    if (!_GOP_IsGwinIdValid(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, winId));
        goto fail;
    }

    //printf("GWINID=%d,Enable=%d\n",(MS_U8)winId,(MS_U8)bEnable);
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is not exist\n", __FUNCTION__, __LINE__, winId));
        //__ASSERT(0);
    }

    //   printf(" u8win=%02bx, bEnable=%02bx\n",winId,bEnable);
    u32fbId = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[winId].u32CurFBId;
    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        goto fail;
    }

    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        goto fail;
    }

    if (0 == pwinFB->in_use)
    {
        msWarning(ERR_GWIN_ID_NOT_ALLOCATED);
        CRITICAL_INFO(printf("[%s][%d]GWIN %d not allocated\n", __FUNCTION__, __LINE__, winId));
        goto fail;
    }
    pwinFB->enable = bEnable;

    //regval2 = MApi_GOP_GWIN_GetActiveGWIN();

    for (i = 0; i < g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum; i++)
    {
        if (GOP_GWIN_IsEnable(pInstance, i) == TRUE)
        {
            regval2 |= 1 << i;
        }
    }

    if (bEnable == 0) // Turn off GOP
    {
        MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, winId, FALSE);
        if ((regval2 & ~(1 << winId)) == 0)
        {
            if (fpXCReduceBWForOSD != NULL)
            {
                fpXCReduceBWForOSD(XC_MAIN_WINDOW, FALSE);
            }
        }
    }
    else //turn on GOP
    {
        //printf("gop on, interlace=%bx\n", IsSrcInterlace());
        if (fpXCReduceBWForOSD != NULL)
        {
            fpXCReduceBWForOSD(XC_MAIN_WINDOW, TRUE);
        }
        MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, winId, TRUE);
    }
    return GOP_API_SUCCESS;

fail:
    if (bEnable == 0) // Turn off GOP
    {
        MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, winId, FALSE);
    }
    return GOP_API_FAIL;
}

int GOP_GWIN_SetBlending(void *pInstance, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, u8win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8win));
        return GOP_API_FAIL;
    }
    MDrv_GOP_GWIN_SetBlending(g_pGOPCtxLocal, u8win, bEnable, u8coef);
    return GOP_API_SUCCESS;

}

int GOP_GWIN_SetBlink(void* pInstance,MS_U8 winId,MS_BOOL bEnable, MS_U8 u8rate)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_SetBlink(g_pGOPCtxLocal,winId, bEnable, u8rate);
    return GOP_API_SUCCESS;
}
int GOP_GWIN_Set_VStretchMode(void *pInstance, MS_U8 u8GOP, EN_GOP_STRETCH_VMODE VStrchMode)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    switch (VStrchMode)
    {
    case E_GOP_VSTRCH_LINEAR:
    case E_GOP_VSTRCH_LINEAR_GAIN0:
    case E_GOP_VSTRCH_LINEAR_GAIN1:
        MDrv_GOP_GWIN_Set_VStretchMode(g_pGOPCtxLocal,u8GOP,E_DRV_GOP_VSTRCH_LINEAR);
            MDrv_GOP_GWIN_Load_VStretchModeTable(g_pGOPCtxLocal,u8GOP,(DRV_GOPStrchVMode)VStrchMode);
        break;
    case E_GOP_VSTRCH_DUPLICATE:
    case E_GOP_VSTRCH_NEAREST:
    case E_GOP_VSTRCH_LINEAR_GAIN2:
            MDrv_GOP_GWIN_Set_VStretchMode(g_pGOPCtxLocal,u8GOP, (DRV_GOPStrchVMode)VStrchMode);
        break;
    default:
        return (GOP_API_ENUM_NOT_SUPPORTED);
        break;
    }
    return GOP_API_SUCCESS;
}
int GOP_GWIN_Set_HStretchMode(void *pInstance, MS_U8 u8GOP, EN_GOP_STRETCH_HMODE HStrchMode)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    switch (HStrchMode)
    {
    case E_GOP_HSTRCH_DUPLICATE:
            MDrv_GOP_GWIN_Set_HStretchMode(g_pGOPCtxLocal,u8GOP, (DRV_GOPStrchHMode)E_GOP_HSTRCH_DUPLICATE);
        break;
    case E_GOP_HSTRCH_6TAPE:
    case E_GOP_HSTRCH_6TAPE_LINEAR:
    case E_GOP_HSTRCH_6TAPE_NEAREST:
    case E_GOP_HSTRCH_6TAPE_GAIN0:
    case E_GOP_HSTRCH_6TAPE_GAIN1:
    case E_GOP_HSTRCH_6TAPE_GAIN2:
    case E_GOP_HSTRCH_6TAPE_GAIN3:
    case E_GOP_HSTRCH_6TAPE_GAIN4:
    case E_GOP_HSTRCH_6TAPE_GAIN5:
    case E_GOP_HSTRCH_2TAPE:
        MDrv_GOP_GWIN_Set_HStretchMode(g_pGOPCtxLocal,u8GOP, (DRV_GOPStrchHMode)E_GOP_HSTRCH_6TAPE);
            MDrv_GOP_GWIN_Load_HStretchModeTable(g_pGOPCtxLocal,u8GOP,(DRV_GOPStrchHMode)HStrchMode);
        break;
    case E_GOP_HSTRCH_4TAPE:
            MDrv_GOP_GWIN_Set_HStretchMode(g_pGOPCtxLocal,u8GOP, (DRV_GOPStrchHMode)E_GOP_HSTRCH_4TAPE);
        break;
    default:
        return (GOP_API_ENUM_NOT_SUPPORTED);
        break;
    }

    return GOP_API_SUCCESS;
}
int GOP_GWIN_Set_TranspColorStretchMode(void *pInstance, MS_U8 u8GOP, EN_GOP_STRCH_TRANSPCOLORMODE  TColorStrchMode)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    enRet = (E_GOP_API_Result)MDrv_GOP_GWIN_Set_TranspColorStretchMode(g_pGOPCtxLocal,u8GOP, (DRV_GOPSTRCH_TRANSPCOLORMODE)TColorStrchMode);
    return enRet;
}

int GOP_DWIN_GetDWinIntInfoTimeout(void *pInstance, GOP_DWinIntInfo *pDWinIntInfo, MS_U32 u32Timeout)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;
    DRV_GOP_DWinIntInfo gopDWinIntInfo;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    enRet = (E_GOP_API_Result)MDrv_GOP_DWIN_GetDWinIntInfo(g_pGOPCtxLocal, &gopDWinIntInfo, u32Timeout);
    pDWinIntInfo->u8DWinIntInfo = gopDWinIntInfo.u8DWinIntInfo;
    return enRet;
}

int GOP_GWIN_EnableT3DMode(void *pInstance, MS_BOOL bEnable)
{
    MS_BOOL bResult = FALSE;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

#ifdef ENABLE_GOP_T3DPATCH
    MS_U8 u8GOPNum = 0;
    DRV_GOPDstType enGopDst = E_DRV_GOP_DST_INVALID;
    MS_U16 u16StrwinStr;

    if (bEnable)
    {
        g_pGOPCtxLocal->pGopChipProperty->GOP_PD = GOP_PD_T3D;
    }
    else
    {
        g_pGOPCtxLocal->pGopChipProperty->GOP_PD = GOP_PD_NORMAL;
    }

    for (u8GOPNum = 0; u8GOPNum < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); u8GOPNum++)
    {
        if (!(g_pGOPCtxLocal->pGOPCtxShared->bGopHasInitialized[u8GOPNum]))
        {
            continue;
        }

        MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GOPNum, &enGopDst);
        if (E_DRV_GOP_DST_OP0 == enGopDst)
        {
            _GOP_GetPDByDst(pInstance, u8GOPNum, enGopDst, &u16StrwinStr);
            _GOP_AdjustHSPD(pInstance, u8GOPNum, u16StrwinStr, enGopDst);
        }
    }
    MDrv_GOP_GWIN_UpdateRegWithSync(g_pGOPCtxLocal, u8GOPNum, TRUE);
#endif

    bResult = TRUE;

    if (bResult == TRUE)
    {
        return (GOP_API_SUCCESS);
    }
    else
    {
        return (GOP_API_FAIL);
    }
}

int GOP_PowerOn(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (g_pGOPCtxLocal->IsClkClosed)
    {
        MS_CRITICAL_MSG(printf("\n%s, The GOP has been powered on!!!\n", __FUNCTION__));
        MDrv_GOP_SetClock(g_pGOPCtxLocal, TRUE);
        g_pGOPCtxLocal->IsClkClosed = FALSE;
        return (GOP_API_SUCCESS);
    }
    else
    {
        MS_CRITICAL_MSG(printf("\n%s,Warning!!! No need to power on again!\n", __FUNCTION__));
        return (GOP_API_FAIL);
    }
}
int GOP_PowerOff(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!g_pGOPCtxLocal->IsClkClosed)
    {
        MS_CRITICAL_MSG(printf("\n%s,Warning!!! The GOP has been powered off, please turn on it if you want to use\n", __FUNCTION__));
        MDrv_GOP_SetClock(g_pGOPCtxLocal, FALSE);
        g_pGOPCtxLocal->IsClkClosed = TRUE;
        return (GOP_API_SUCCESS);
    }
    else
    {
        MS_CRITICAL_MSG(printf("\n%s,Warning!!! No need to power off again!\n", __FUNCTION__));
        return (GOP_API_FAIL);
    }
}

int GOP_GetRTStatus(void *pInstance, GOP_ApiStatus *pGopApiSts)
{
    MS_U8 i, j;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    for (i = 0; i < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); i++)
    {
        pGopApiSts->eGOPNumDstType[i] = (EN_GOP_DST_TYPE)0xFF;
        pGopApiSts->u16GopNumStrWinHSize[i] = pGopApiSts->u16GopNumStrWinVSize[i] = 0xFF;

        switch (i)
        {
        case 0: //gop0
            {
                MS_U8 u8GWinNum = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, i);
                for (j = 0; j < u8GWinNum; j++)
                {
                    pGopApiSts->Gop0GwinIsEnable[j] = FALSE;
                }
            }
            break;
        case 1: //gop1
            {
                MS_U8 u8GWinNum = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, i);
                for (j = 0; j < u8GWinNum; j++)
                {
                    pGopApiSts->Gop1GwinIsEnable[j] = FALSE;
                }
            }
            break;
        case 2: //gop2
            {
                pGopApiSts->Gop2GwinIsEnable = FALSE;
            }
            break;
        case 3: //gop3
            {
                pGopApiSts->Gop3GwinIsEnable = FALSE;
            }
            break;
        case 4: //gop4
            {
                pGopApiSts->Gop4GwinIsEnable = FALSE;
            }
            break;
        case 5: //gop5
            {
                pGopApiSts->Gop5GwinIsEnable = FALSE;
            }
            break;
        default:
            break;
        }
    }

    for (i = 0; i < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); i++)
    {
        //need refine check using DRV_GOPDstType or EN_GOP_DST_TYPE
        DRV_GOPDstType enGOPDst = E_DRV_GOP_DST_INVALID;
        pGopApiSts->u16GopNumStrWinHSize[i] = g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchWidth[i];
        pGopApiSts->u16GopNumStrWinVSize[i] = g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchHeight[i];
        //need refine check using DRV_GOPDstType or EN_GOP_DST_TYPE
        (MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, i, &enGOPDst));
        (_GOP_Map_DRVDst2API_Enum_(pInstance, &(pGopApiSts->eGOPNumDstType[i]), enGOPDst));

        switch (i)
        {
        case 0: //gop0
            {
                MS_U8 u8GWinNum = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, i);
                for (j = 0; j < u8GWinNum; j++)
                {
                    pGopApiSts->Gop0GwinIsEnable[j] = GOP_GWIN_IsEnable(pInstance, j);
                }
            }
            break;
        case 1: //gop1
            {
                MS_U8 u8GWinNum = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, i);
                for (j = 0; j < u8GWinNum; j++)
                {
                    pGopApiSts->Gop1GwinIsEnable[j] = GOP_GWIN_IsEnable(pInstance, _GOP_SelGwinId2(pInstance, GOPTYPE.GOP1, (MS_U32)j));
                }
            }
            break;
        case 2: //gop2
            {
                pGopApiSts->Gop2GwinIsEnable = GOP_GWIN_IsEnable(pInstance, _GOP_SelGwinId2(pInstance, GOPTYPE.GOP2, 0));
            }
            break;
        case 3: //gop3
            {
                pGopApiSts->Gop3GwinIsEnable = GOP_GWIN_IsEnable(pInstance, _GOP_SelGwinId2(pInstance, GOPTYPE.GOP3, 0));
            }
            break;
        case 4: //gop4
            {
                pGopApiSts->Gop4GwinIsEnable = GOP_GWIN_IsEnable(pInstance, _GOP_SelGwinId2(pInstance, GOPTYPE.GOP4, 0));
            }
            break;
        case 5: //gop5
            {
                pGopApiSts->Gop5GwinIsEnable = GOP_GWIN_IsEnable(pInstance, _GOP_SelGwinId2(pInstance, GOPTYPE.GOP5, 0));
            }
            break;
        default:
            break;
        }
    }
    return (GOP_API_SUCCESS);
}

int GOP_GetInfo(void *pInstance, GOP_ApiInfo *pGopApiInfo)
{
    MS_U8 u8GwinNum = 0, i;
    EN_GOP_DST_TYPE GopDst;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    for (i = 0; i < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); i++)
    {
        u8GwinNum += MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, i);
    }

    for (i = 0; i < MAX_GOP_DST_SUPPORT; i++)
    {
        //need refine check using DRV_GOPDstType or EN_GOP_DST_TYPE
        if (MDrv_GOP_GWIN_SetDstPlane(g_pGOPCtxLocal, 0, (DRV_GOPDstType)i,TRUE)==GOP_SUCCESS)
        {
            _GOP_Map_DRVDst2API_Enum_(pInstance, &GopDst, (DRV_GOPDstType)i);
            pGopApiInfo->SupportGOPDst[i] = GopDst;
        }
        else
        {
            pGopApiInfo->SupportGOPDst[i] = (EN_GOP_DST_TYPE)0xFF;
        }
    }

    pGopApiInfo->u8MaxGOPNum = MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal);
    pGopApiInfo->u8MaxGwinNum = u8GwinNum;
    return (GOP_API_SUCCESS);
}
int GOP_GWIN_SetGWinShared(void *pInstance, MS_U8 winId, MS_BOOL bIsShared)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, winId));
        return (GOP_API_FAIL);
    }

    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[winId].bIsShared = bIsShared;
    return GOP_API_SUCCESS;
}
int GOP_GWIN_SetGWinSharedCnt(void *pInstance, MS_U8 winId, MS_U16 u16SharedCnt)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, winId));
        return (GOP_API_FAIL);
    }

    g_pGOPCtxLocal->pGOPCtxShared->gwinMap[winId].u16SharedCnt = u16SharedCnt;
    return GOP_API_SUCCESS;
}

int GOP_GWIN_Switch2Gwin(void *pInstance, MS_U8 winId, MS_U32 *pSet)
{
    MS_U32 u32fbId;
    MS_U16 fbFmt;
    MS_U32 u32GeDstAdr = 0;
    GOP_WinFB_INFO *pwinFB;
    DRV_GOP_CBFmtInfo *FmtInfo = (DRV_GOP_CBFmtInfo *)pSet;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, winId));
        return (GOP_API_FAIL);
    }
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, winId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is not exist\n", __FUNCTION__, __LINE__, winId));
        return (GOP_API_FAIL);
    }

    u32fbId = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[winId].u32CurFBId;
    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        DEBUG_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, u32fbId));
        return (GOP_API_FAIL);
    }
    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        DEBUG_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, u32fbId));
        return (GOP_API_FAIL);
    }

    if (0 == pwinFB->in_use)
    {
        DEBUG_INFO(printf("[%s][%d]FbId %ld is not allocated\n", __FUNCTION__, __LINE__, u32fbId));
        return (GOP_API_FAIL);

    }

    g_pGOPCtxLocal->current_winId = winId;
    u32GeDstAdr = pwinFB->addr;
    MDrv_GOP_ConvertAPIAddr2HAL(g_pGOPCtxLocal, winId, &u32GeDstAdr);
    fbFmt = pwinFB->fbFmt;
    if ((pwinFB->fbFmt & 0xFF) == E_MS_FMT_ARGB1555)
        fbFmt = E_MS_FMT_ARGB1555_DST | (E_MS_FMT_ARGB1555_DST << 8);

    if (fpSetFBFmt != NULL)
    {
        fpSetFBFmt(pwinFB->pitch, u32GeDstAdr, fbFmt);
        FmtInfo->u32Addr = u32GeDstAdr;
        FmtInfo->u16Pitch = pwinFB->pitch;
        FmtInfo->u16Fmt = fbFmt;
    }
    else
    {
        MS_DEBUG_MSG(printf("%s: SetFBFmt is not hooked. Bypass switch...\n", __FUNCTION__));
    }
    return (GOP_API_SUCCESS);
}

int GOP_GWIN_IsGwinExist(void *pInstance, MS_U8 gId)
{
    int ret;
    if (!_GOP_IsGwinIdValid(pInstance, gId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, gId));
        return (GOP_API_FAIL);
    }
    ret = _GOP_GWIN_IsGwinExistInClient(pInstance, gId);
    return ret;
}
int GOP_GWIN_GetFreeWinID(void *pInstance)
{
    MS_U8 GWID = 0xFF;
    MS_U8 i, j, k;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, MDrv_GOP_Get(g_pGOPCtxLocal)))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, MDrv_GOP_Get(g_pGOPCtxLocal)));
        return (GWIN_ID_INVALID);
    }

    if (MDrv_GOP_Get(g_pGOPCtxLocal) == 1) //gop1
    {
        i = _GOP_SelGwinId2(pInstance, GOPTYPE.GOP1, 0);
        j = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, GOPTYPE.GOP1);
    }
    else if (MDrv_GOP_Get(g_pGOPCtxLocal) == 0)
    {
        i = _GOP_SelGwinId2(pInstance, GOPTYPE.GOP0, 0);
        j = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, GOPTYPE.GOP0);
    }
    else if (MDrv_GOP_Get(g_pGOPCtxLocal) == 2)
    {
        i = _GOP_SelGwinId2(pInstance, GOPTYPE.GOP2, 0);
        j = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, GOPTYPE.GOP2);
    }
    else if (MDrv_GOP_Get(g_pGOPCtxLocal) == 3)
    {
        i = _GOP_SelGwinId2(pInstance, GOPTYPE.GOP3, 0);
        j = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, GOPTYPE.GOP3);
    }
    else if (MDrv_GOP_Get(g_pGOPCtxLocal) == 4)
    {
        i = _GOP_SelGwinId2(pInstance, GOPTYPE.GOP4, 0);
        j = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, GOPTYPE.GOP4);
    }
    else if (MDrv_GOP_Get(g_pGOPCtxLocal) == 5)
    {
        i = _GOP_SelGwinId2(pInstance, GOPTYPE.GOP5, 0);
        j = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, GOPTYPE.GOP5);
    }
    else
    {
        return (GWIN_ID_INVALID);
    }

    if (!_GOP_IsGwinIdValid(pInstance, i))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, i));
        return (GWIN_ID_INVALID);
    }
    k = i + j;
    for (; i < k; i++)
    {
        if (DRV_MAX_GWIN_FB_SUPPORT <= g_pGOPCtxLocal->pGOPCtxShared->gwinMap[i].u32CurFBId)
        {
            GWID = i;
            break;
        }
    }
    return (GWID);
}
MS_U32 GOP_GWIN_GetFBfromGWIN(void *pInstance, MS_U8 gwinId)
{
    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGwinIdValid(pInstance, gwinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, gwinId));
        return (GWIN_ID_INVALID);
    }
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, gwinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is not exist\n", __FUNCTION__, __LINE__, gwinId));
        return (GWIN_ID_INVALID);
    }

    pwinFB = _GetWinFB(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gwinId].u32CurFBId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gwinId].u32CurFBId));
        return (GWIN_ID_INVALID);
    }
    if (!_GOP_IsFbIdValid(pInstance, g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gwinId].u32CurFBId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gwinId].u32CurFBId));
        return (GWIN_ID_INVALID);
    }
    if (0 == pwinFB->in_use)
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d not allocated\n", __FUNCTION__, __LINE__, gwinId));
        return (GWIN_ID_INVALID);
    }

    return (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gwinId].u32CurFBId);
}

int GOP_FB_Get(void *pInstance)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (GOP_WINFB_POOL2 == g_pGOPCtxLocal->pGOPCtxShared->fb_currentPoolId)
    {
        return 1;
    }
    else
    {
        return 0;
    }
/* //Change to if to fix parasoft error
    switch(g_pGOPCtxLocal->pGOPCtxShared->fb_currentPoolId)
    {
        case GOP_WINFB_POOL2:
            return 1;
         default:
            return 0;
    }*/
    return GOP_API_SUCCESS;
}
int GOP_FB_SEL(void *pInstance, MS_U8 u8FB)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (u8FB)
    {
    case 0: // gwinFB1
        g_pGOPCtxLocal->pGOPCtxShared->fb_currentPoolId = GOP_WINFB_POOL1;
        break;
    case 1: // gwinFB2
        g_pGOPCtxLocal->pGOPCtxShared->fb_currentPoolId = GOP_WINFB_POOL2;
        break;
    default:
        break;
    }
    return GOP_API_SUCCESS;
}
int GOP_GWIN_Is32FBExist(void *pInstance, MS_U32 u32fbId)
{
    GOP_WinFB_INFO *pwinFB;

    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %d  is out of range\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return FALSE;
    }

    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n",__FUNCTION__,__LINE__,(unsigned int)u32fbId));
        return FALSE;
    }

    return pwinFB->in_use;
}

int GOP_Set3DOSDMode(void *pInstance, MS_U8 u8Gwin, MS_U16 u16MainFBId, MS_U16 u16SubFBId, EN_GOP_3D_MODETYPE en3DMode)
{
    MS_U8 u8GOP;
    GOP_Result enRet = GOP_FAIL;
    GOP_WinFB_INFO *pwinFB;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    pwinFB = _GetWinFB(pInstance, u16SubFBId);

    u8GOP = _DumpGopByGwinId(pInstance, u8Gwin);

    switch (en3DMode)
    {
    case E_GOP_3D_DISABLE:
        {
            enRet = MDrv_GOP_Set3DOSD_Mode(g_pGOPCtxLocal, u8GOP, E_DRV_GOP_3D_DISABLE);
            if (enRet != GOP_SUCCESS)
            {
                return ((E_GOP_API_Result)enRet);
            }
            break;
        }
    case E_GOP_3D_SWITH_BY_FRAME:
    case E_GOP_3D_SIDE_BY_SYDE:
    case E_GOP_3D_TOP_BOTTOM:
    case E_GOP_3D_LINE_ALTERNATIVE:
    case E_GOP_3D_FRAMEPACKING:
        {

            if (pwinFB == NULL)
            {
                CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%d\n", __FUNCTION__, __LINE__, u16SubFBId));
                return (GWIN_NO_AVAILABLE);
            }

            if (pwinFB->height == 1080)
            {
                g_pGOPCtxLocal->GOP_FramePacking_Gap = 45;
            }
            else if (pwinFB->height == 720)
            {
                g_pGOPCtxLocal->GOP_FramePacking_Gap = 30;
            }
            else
            {
                g_pGOPCtxLocal->GOP_FramePacking_Gap = 0;
            }

            enRet = MDrv_GOP_Set3DOSD_Mode(g_pGOPCtxLocal, u8GOP, (EN_DRV_GOP_3D_MODE)en3DMode);
            if (enRet != GOP_SUCCESS)
            {
                return ((E_GOP_API_Result)enRet);
            }
            break;
        }
    default:
        printf("[%s]not support 3D OSD case: %d\n", __FUNCTION__, en3DMode);
        return (GOP_API_FAIL);
    }

    if (GOP_MapFB2Win(pInstance, u16MainFBId, u8Gwin) != GOP_API_SUCCESS)
    {
        printf("[%s] can't map Main fbid:%d to gwin:%d\n", __FUNCTION__, u16MainFBId, u8Gwin);
        return (GOP_API_FAIL);
    }

    if (en3DMode != E_GOP_3D_DISABLE)
    {
        if (u8GOP > GOPTYPE.GOP0)
        {
            MS_BOOL bHMirror = FALSE, bVMirror = FALSE;
            DRV_GOPDstType GopDst = E_DRV_GOP_DST_INVALID;
            MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GOP, &GopDst);
            _GOP_GWIN_IsEnableMirror(pInstance, GopDst, &bHMirror, &bVMirror);
            if (bHMirror || bVMirror)
            {
                GOP_GwinInfo stGwinInfo;
                GOP_GetWinInfo(pInstance, u8Gwin, &stGwinInfo);
                stGwinInfo.u32DRAMRBlkStart = pwinFB->addr;
                _GOP_GWIN_SetHVMirrorDRAMAddr(pInstance, u8Gwin, bHMirror, bVMirror, &stGwinInfo);
                pwinFB->addr = stGwinInfo.u32DRAMRBlkStart;
            }
        }
        MDrv_GOP_Set3DOSD_Sub(g_pGOPCtxLocal, u8Gwin, pwinFB->addr);
    }
    else
    {
        MDrv_GOP_Set3DOSD_Sub(g_pGOPCtxLocal, u8Gwin, 0x0);  //clear sub address
    }
    MDrv_GOP_Set3DOSD_Middle(g_pGOPCtxLocal, u8GOP);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
    return (GOP_API_SUCCESS);
}

int GOP_SetGOPClk(void *pInstance, MS_U8 gopNum, EN_GOP_DST_TYPE eDstType)
{
    DRV_GOPDstType GopDst;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, gopNum))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, gopNum));
        return (GOP_API_FAIL);
    }
    (_GOP_Map_APIDst2DRV_Enum(pInstance, eDstType, &GopDst));
    (MDrv_GOP_SetGOPClk(g_pGOPCtxLocal, gopNum, GopDst));
    return (GOP_API_SUCCESS);
}
int GOP_GWIN_SetBnkForceWrite(void *pInstance, MS_U8 u8GOP, MS_BOOL bEnable)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (g_pGOPCtxLocal == NULL)
    {
        bInitFWR = bEnable;
        return GOP_API_SUCCESS;
    }
    else
    {
        if (!g_pGOPCtxLocal->pGopChipProperty->bBnkForceWrite)
        {
            printf("[%s][%d] GOP not support bank force write function in this chip version \n", __FUNCTION__, __LINE__);
            return (GOP_API_FAIL);
        }
        else
        {
            MDrv_GOP_GWIN_SetBnkForceWrite(g_pGOPCtxLocal, u8GOP, bEnable);
            return (GOP_API_SUCCESS);
        }
    }
}
int GOP_GWIN_SetForceWrite(void *pInstance, MS_BOOL bEnable)
{
    //It's used for supernova special used. (by Jupiter)
    //There should no driver request before init.
    //Supernova wants to easly control AP flow, it will set gop force write before gop init.
    //Driver must recode the FRWR status and set it during gop init.
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (g_pGOPCtxLocal == NULL)
    {
        bInitFWR = bEnable;
        return GOP_API_SUCCESS;
    }
    else
    {
        MDrv_GOP_GWIN_SetForceWrite(g_pGOPCtxLocal, bEnable);
        return (GOP_API_SUCCESS);
    }
}
int GOP_Set3D_LR_FrameExchange(void *pInstance, MS_U8 u8GOP)
{
    GOP_Result enRet = GOP_FAIL;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return (GOP_API_FAIL);
    }

    enRet = MDrv_GOP_Set3D_LR_FrameExchange(g_pGOPCtxLocal, u8GOP);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
    return enRet;
}
int GOP_Enable_VEOSD(void *pInstance, MS_U8 u8GOP, MS_BOOL bEn)
{
    MS_U8 GOPNum = u8GOP;
    MS_BOOL bret;
    DRV_GOPDstType pGopDst = E_DRV_GOP_DST_INVALID;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, u8GOP, &pGopDst);

    if ((pGopDst == E_DRV_GOP_DST_MIXER2VE) || (pGopDst == E_DRV_GOP_DST_MIXER2OP) || (pGopDst == E_DRV_GOP_DST_MIXER2OP1))
    {
        GOPNum = GOPTYPE.MIXER - 1;
    }
    else if (pGopDst == E_DRV_GOP_DST_INVALID)
    {
        printf("[%s][%d]Wrong GOP Destination!", __FUNCTION__, __LINE__);
        return GOP_API_FAIL;
    }

    bret = MDrv_GOP_Set_VE_MUX(g_pGOPCtxLocal, GOPNum, bEn);

    if (bret)
    {
        return GOP_API_SUCCESS;
    }
    else
    {
        return GOP_API_FAIL;
    }
}

int GOP_OutputLayerSwitch(void *pInstance, MS_U8 u8GOP)
{
    MS_U8 i;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return (GOP_API_FAIL);
    }

    DBGCHECK_INFO(printf("---%s %d: PID[%lu], TID[%ld] u8GOP=%u\n", __FUNCTION__, __LINE__, (MS_U32)getpid(), MsOS_GetOSThreadID(), u8GOP));

    MDrv_GOP_SetGOPHighPri(g_pGOPCtxLocal, u8GOP);
    for (i = 0; i < MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal); i++)
    {
        _GOP_InitHSPDByGOP(pInstance, i);
    }
    return GOP_API_SUCCESS;
}
int GOP_GWIN_SwapOverlapWin(void *pInstance, MS_U8 u8GOP, MS_U8 u8win)
{
    MS_U8 i, *temp;
    GOP_GwinPriority GopGwinPri;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif


    memset(&GopGwinPri, 0, sizeof(GOP_GwinPriority));

    if (!_GOP_IsGwinIdValid(pInstance, u8win))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %d  is out of range\n", __FUNCTION__, __LINE__, u8win));
        return (GOP_API_FAIL);
    }
    if (!_GOP_IsGopNumValid(pInstance, u8GOP))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return (GOP_API_FAIL);
    }

    if (u8GOP == 1)  //GOP1
    {
        if (4 == MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 1))
        {
            MDrv_GOP_GWIN_GetGwinPriority(g_pGOPCtxLocal, 1, &GopGwinPri);
            temp = (MS_U8 *)&GopGwinPri.Gop0_GwinPri;

            for (i = 0; i < MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 1); i++)
            {
                if (temp[i] == (u8win - MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 0))) //find
                {
                    temp[i] = temp[0];
                    temp[0] = (u8win - MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 0));
                    break;
                }
            }

            MDrv_GOP_GWIN_SetGwinPriority(g_pGOPCtxLocal, 1, &GopGwinPri);
        }
        else
        {
            GopGwinPri.Gop1_1stGwin = u8win - MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 0);
            MDrv_GOP_GWIN_SetGwinPriority(g_pGOPCtxLocal, 1, &GopGwinPri);
        }
    }
    else    //GOP0
    {
        MDrv_GOP_GWIN_GetGwinPriority(g_pGOPCtxLocal, 0, &GopGwinPri);
        temp = (MS_U8 *)&GopGwinPri.Gop0_GwinPri;

        for (i = 0; i < MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 0); i++)
        {
            if (temp[i] == u8win) //find
            {
                temp[i] = temp[0];
                temp[0] = u8win;
                break;
            }
        }

        MDrv_GOP_GWIN_SetGwinPriority(g_pGOPCtxLocal, 0, &GopGwinPri);
    }
    return (GOP_API_SUCCESS);
}
int GOP_SetGOPBWStrength(void *pInstance, EN_GOP_BW_STRENGTH eGOPBWStr, MS_U8 u8GOP)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (u8GOP != 0xc && !_GOP_IsGwinIdValid(pInstance, u8GOP))
    {
        APIGOP_ASSERT(FALSE, printf("\nInvaild gop num!!"));
        return (GOP_API_FAIL);
    }

    if (eGOPBWStr == E_GOP_BW_STRONG)
        MDrv_GOP_SetGOPBWStrength(g_pGOPCtxLocal, u8GOP, E_GOP_BURSTLEN_LONG);
    else if (eGOPBWStr == E_GOP_BW_MIDDLE)
        MDrv_GOP_SetGOPBWStrength(g_pGOPCtxLocal, u8GOP, E_GOP_BURSTLEN_MIDDLE);
    else if (eGOPBWStr == E_GOP_BW_WEAK)
        MDrv_GOP_SetGOPBWStrength(g_pGOPCtxLocal, u8GOP, E_GOP_BURSTLEN_SHORT);
    else
    {
        return (GOP_API_INVALID_PARAMETERS);
    }

    return (GOP_API_SUCCESS);
}
int GOP_GetGOPBWStrength(void *pInstance, MS_U8 u8GOP, EN_GOP_BW_STRENGTH *curBW)
{
    MS_U32 gopbw;
    EN_GOP_BW_STRENGTH ret;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif


    if (!_GOP_IsGwinIdValid(pInstance, u8GOP))
    {
        APIGOP_ASSERT(FALSE, printf("\nInvaild gop num!!"));
        return (GOP_API_FAIL);
    }
    gopbw = MDrv_GOP_GetGOPBWStrength(g_pGOPCtxLocal, u8GOP);

    if (gopbw == E_GOP_BURSTLEN_LONG)
        ret = E_GOP_BW_STRONG;
    else if (gopbw == E_GOP_BURSTLEN_MIDDLE)
        ret = E_GOP_BW_MIDDLE;
    else if (gopbw == E_GOP_BURSTLEN_SHORT)
        ret = E_GOP_BW_WEAK;
    else
        ret = E_GOP_BW_UNKNOWN;

    if (curBW != NULL)
        *curBW = ret;
    else
        return (GOP_API_INVALID_PARAMETERS);

    return (GOP_API_SUCCESS);
}
int GOP_EnableLBCouple(void *pInstance, MS_U8 u8GOP, MS_BOOL bEnable)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_EnableLBCouple(g_pGOPCtxLocal, u8GOP, bEnable);
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
    return GOP_API_SUCCESS;
}

int GOP_SetGOPYUV(void *pInstance, MS_U8 u8GOP)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (u8GOP != 0xC && !_GOP_IsGwinIdValid(pInstance, u8GOP))
    {
        APIGOP_ASSERT(FALSE, printf("\nInvaild gop num!!"));
        return (GOP_API_FAIL);
    }
    MDrv_GOP_SetGOPYUV(g_pGOPCtxLocal, u8GOP);
    return (GOP_API_SUCCESS);
}

int GOP_IsRegUpdated(void *pInstance, MS_U8 u8GopType)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (MDrv_GOP_GetGOPACK(g_pGOPCtxLocal, u8GopType))
    {
        return (GOP_API_SUCCESS);
    }
    return (GOP_API_FAIL);
}

int GOP_TestPatternAlpha_Enable(void *pInstance, MS_BOOL bTSTAlpha)
{
    DRV_GOP_TSTPATTERN drv_TstPattern;
    memset(&drv_TstPattern, 0, sizeof(DRV_GOP_TSTPATTERN));
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    drv_TstPattern.bTSTClr_Alpha_En = bTSTAlpha;
    MDrv_GOP_TestPatternAlpha_Enable(g_pGOPCtxLocal, drv_TstPattern.bTSTClr_Alpha_En);
    return GOP_API_SUCCESS;
}

int GOP_OC_SetOCFBinfo(void *pInstance, MS_PHYADDR u32Addr, EN_GOP_OC_SEL_TYPE BankMode, MS_U32 enBufSize)
{
    DRV_GOP_OC_INFO OCinfo;
    MS_U16 PanelWidth, PanelHeight;
    E_GOP_API_Result enRet = GOP_API_FAIL;
    MS_U8 u8CurGOP = 0;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    u8CurGOP = MDrv_GOP_Get(g_pGOPCtxLocal);
    memset(&OCinfo, 0, sizeof(DRV_GOP_OC_INFO));

    PanelWidth  = g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP];
    PanelHeight = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8CurGOP];

    OCinfo.u16GOP_OC_HSIZE = PanelWidth;
    OCinfo.u16GOP_OC_VSIZE = PanelHeight;
    OCinfo.u32GOP_OC_FETCH = (((PanelWidth >> 1) + (PanelWidth / 10)) << 2) * PanelHeight;                               // PanelWidth *0.6(Compression Ratio) *4(8888fmt)

    if (enBufSize < (OCinfo.u32GOP_OC_FETCH << BankMode))
    {
        printf("[%s] ERROR, OC Buffer size not enough , input BufSize:0x%x\n",__FUNCTION__,(unsigned int)enBufSize);
        return (GOP_API_FAIL);
    }
    else
    {
        switch (BankMode)
        {
        case E_GOP_OC_SEL_2BankMode:
            OCinfo.u8GOP_OC_BANK_MODE = 1;
            break;
        case E_GOP_OC_SEL_4BankMode:
            OCinfo.u8GOP_OC_BANK_MODE = 0;
            break;
        default:
            OCinfo.u8GOP_OC_BANK_MODE = 1;
            break;
        }
    }

    OCinfo.u32GOP_OC_DRAM_RBLK = u32Addr;
    OCinfo.u32GOP_OC_ADDRPROTECT_STR = u32Addr;
    OCinfo.u32GOP_OC_ADDRPROTECT_END = u32Addr + enBufSize;

    enRet = (E_GOP_API_Result)MDrv_GOP_OC_SetOCInfo(g_pGOPCtxLocal, &OCinfo);

    return (enRet);
}

int GOP_VE_SetOutputTiming(void *pInstance, GOP_VE_TIMINGTYPE mode)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    E_GOP_API_Result enRet = GOP_API_FAIL;
    enRet = (E_GOP_API_Result)MDrv_GOP_VE_SetOutputTiming(g_pGOPCtxLocal, mode);
    return GOP_SUCCESS;
}

int GOP_MIXER_SetOutputTiming(void *pInstance, GOP_MIXER_TIMINGTYPE mode,  GOP_MixerTiming *pMT)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;
    GOP_DRV_MixerTiming MT;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (pMT != NULL)
    {
        MT.hstart = pMT->hstart;
        MT.vstart = pMT->vstart;
        MT.vend = pMT->vend;
        MT.hend = pMT->hend;
        MT.htotal = pMT->htotal;
        MT.hsyncdelay = 0;
    }

    if (pMT == NULL)
    {
        return (GOP_API_INVALID_PARAMETERS);
    }

    enRet = (E_GOP_API_Result)MDrv_GOP_MIXER_SetOutputTiming(g_pGOPCtxLocal, mode, &MT);
    return (enRet);
}
int GOP_MIXER_SetMIXER2OPOutputTiming(void *pInstance, GOP_Mixer2OPTiming *pMT)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;
    GOP_DRV_MixerTiming MT;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif


    if (pMT != NULL)
    {
        MT.hstart = pMT->hstart;
        MT.vstart = pMT->vstart;
        MT.vend = pMT->vend;
        MT.hend = pMT->hend;
        MT.htotal = pMT->htotal;
        MT.hsyncdelay = pMT->hsyncdelay;
    }

    if (pMT == NULL)
    {
        return (GOP_API_INVALID_PARAMETERS);
    }

    enRet = (E_GOP_API_Result)MDrv_GOP_MIXER_SetOutputTiming(g_pGOPCtxLocal, GOP_CUSTOM_OP, &MT);
    return (enRet);
}
int GOP_MIXER_EnableVfilter(void *pInstance, MS_BOOL bEnable)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    enRet = (E_GOP_API_Result)MDrv_GOP_MIXER_EnableVfilter(g_pGOPCtxLocal, bEnable);

    return (enRet);
}

int GOP_MIXER_EnableOldBlendingMode(void *pInstance, MS_U8 u8GOP, MS_BOOL bEnable)
{
    E_GOP_API_Result enRet = GOP_API_FAIL;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    enRet = (E_GOP_API_Result)MDrv_GOP_MIXER_EnableOldBlendMode(g_pGOPCtxLocal, u8GOP, bEnable);

    return (enRet);
}

int GOP_GWIN_SetRelativeWinPrio(void *pInstance, MS_U8 u8GOP, GOP_GwinPri *pGwinPri)
{
    GOP_GwinPriority GopGwinPri;
    MS_U8 u8Idx;
    MS_U8 u8Idx2;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    //Parameter check:
    if (u8GOP >= MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal) || (NULL == pGwinPri)
        || (pGwinPri->u8GwinNum > 4) || (pGwinPri->u8GwinNum <= 0))
    {
        return (GOP_API_INVALID_PARAMETERS);
    }

    u8Idx2 = (u8GOP == 0) ? MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 0) : MAX_GWIN_SUPPORT;
    for (u8Idx = 0; u8Idx < pGwinPri->u8GwinNum; u8Idx++)
    {
        if (pGwinPri->u8GwinPri[u8Idx] >= u8Idx2)
        {
            return (GOP_API_INVALID_PARAMETERS);
        }
    }

    //for GOP2,GOP3.no need to set the relative prio
    if ((u8GOP == 2) || (u8GOP == 3))
    {
        return (GOP_API_SUCCESS);
    }

    //Handle GOP1:
    if (u8GOP == 1)
    {
        u8Idx2 = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, 0);
        if (pGwinPri->u8GwinPri[0] < u8Idx2)
        {
            return (GOP_API_INVALID_PARAMETERS);
        }
        GopGwinPri.Gop1_1stGwin = pGwinPri->u8GwinPri[0] - u8Idx2;
        //Adjust currently hw priority to app requested relative priority:
        MDrv_GOP_GWIN_SetGwinPriority(g_pGOPCtxLocal, 1, &GopGwinPri);
        return (GOP_API_SUCCESS);
    }

    //Handle GOP0:
    //Get Currently HW Setting:
    MDrv_GOP_GWIN_GetGwinPriority(g_pGOPCtxLocal, u8GOP, &GopGwinPri);


    u8Idx2 = 0x0;
    for (u8Idx = 0; u8Idx < pGwinPri->u8GwinNum; u8Idx++)
    {
        if (pGwinPri->u8GwinPri[u8Idx] == GopGwinPri.Gop0_GwinPri.u81stGwin)
        {
            if (u8Idx2 & 0x1)
            {
                return (GOP_API_INVALID_PARAMETERS);
            }

            u8Idx2 |= 0x1;
        }
        else if (pGwinPri->u8GwinPri[u8Idx] == GopGwinPri.Gop0_GwinPri.u82stGwin)
        {
            if (u8Idx2 & 0x2)
            {
                return (GOP_API_INVALID_PARAMETERS);
            }

            u8Idx2 |= 0x2;
        }
        else if (pGwinPri->u8GwinPri[u8Idx] == GopGwinPri.Gop0_GwinPri.u83stGwin)
        {
            if (u8Idx2 & 0x4)
            {
                return (GOP_API_INVALID_PARAMETERS);
            }

            u8Idx2 |= 0x4;
        }
        else if (pGwinPri->u8GwinPri[u8Idx] == GopGwinPri.Gop0_GwinPri.u84stGwin)
        {
            if (u8Idx2 & 0x8)
            {
                return (GOP_API_INVALID_PARAMETERS);
            }

            u8Idx2 |= 0x8;
        }
    }

    u8Idx = 0;
    if (u8Idx2 & 0x1)
    {
        GopGwinPri.Gop0_GwinPri.u81stGwin = pGwinPri->u8GwinPri[u8Idx];
        u8Idx++;
    }

    if (u8Idx2 & 0x2)
    {
        GopGwinPri.Gop0_GwinPri.u82stGwin = pGwinPri->u8GwinPri[u8Idx];
        u8Idx++;
    }

    if (u8Idx2 & 0x4)
    {
        GopGwinPri.Gop0_GwinPri.u83stGwin = pGwinPri->u8GwinPri[u8Idx];
        u8Idx++;
    }

    if (u8Idx2 & 0x8)
    {
        GopGwinPri.Gop0_GwinPri.u84stGwin = pGwinPri->u8GwinPri[u8Idx];
        u8Idx++;
    }

    MDrv_GOP_GWIN_SetGwinPriority(g_pGOPCtxLocal, 0, &GopGwinPri);

    return (GOP_API_SUCCESS);
}

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
static void GOP_AtExit(void *pInstance)
{
    MS_U32 u32TempID = 0, u32GWinFBID = DRV_MAX_GWIN_FB_SUPPORT;
    MS_U32 u32CurPID = getpid();
    E_GOP_API_Result enRet = GOP_API_FAIL;
    GOP_WinFB_INFO *pwinFB;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (g_pGOPCtxLocal == NULL)
    {
        return;
    }

#if GOP_LOCK_SUPPORT
    printf("---%s %d: PID[%lu], TID[%ld] exited, MutexCnt=%ld\n", __FUNCTION__, __LINE__, u32CurPID, MsOS_GetOSThreadID(), g_pGOPCtxLocal->s32GOPMutexCnt);
#else
    printf("---%s %d: PID[%lu], TID[%ld] exited\n", __FUNCTION__, __LINE__, u32CurPID, MsOS_GetOSThreadID());
#endif
    for (u32TempID = 0; u32TempID < g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum; u32TempID++)
    {
        //Check and release APP's GWIN resource
        if (((FALSE == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32TempID].bIsShared)
             && (DRV_MAX_GWIN_FB_SUPPORT > g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32TempID].u32CurFBId)
             && (g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32TempID].u32GOPClientId == u32CurPID))
            || ((TRUE == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32TempID].bIsShared)
                && (0x0 == g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32TempID].u16SharedCnt)))

        {
            u32GWinFBID = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[u32TempID].u32CurFBId;
            enRet = GOP_Win_Destroy(pInstance, u32TempID);
            printf("\n---%s %d: Try DestroyWIN[%lu], return[%u]\n", __FUNCTION__, __LINE__, u32TempID, (MS_U8)enRet);
            enRet = (E_GOP_API_Result)GOP_FB_Destroy(pInstance, u32GWinFBID);
            printf("\n---%s %d: Try DestroyWIN's FB[%lu], return[%u]\n", __FUNCTION__, __LINE__, u32GWinFBID, (MS_U8)enRet);
        }
    }


    for (u32TempID = 0; u32TempID < DRV_MAX_GWIN_FB_SUPPORT; u32TempID++)
    {
        pwinFB = _GetWinFB(pInstance, u32TempID);
        // Check and release APP1's un-maped FB resource
        // Note: If this APP1's FB is currently mapped to GWIN, means this FB is used by APP2
        if ((pwinFB->u32GOPClientId == u32CurPID)
            && (pwinFB->gWinId == GWIN_ID_INVALID))
        {
            enRet = (E_GOP_API_Result)GOP_Win_Destroy(pInstance, u32TempID);
            printf("\n---%s %d: Try DestroyFB[%lu], return[%u]\n", __FUNCTION__, __LINE__, u32TempID, (MS_U8)enRet);
        }
    }

#if GOP_LOCK_SUPPORT

    if (g_pGOPCtxLocal->s32GOPMutexCnt > 0)
    {
        g_pGOPCtxLocal->s32GOPMutexCnt = 0;
        MsOS_ReleaseMutex(g_pGOPCtxLocal->s32GOPMutex);
    }
#endif

#if GOP_XC_LOCK_SUPPORT
    if (g_pGOPCtxLocal->s32GOPXC_MutexCnt > 0)
    {
        g_pGOPCtxLocal->s32GOPXC_MutexCnt = 0;
        MsOS_ReleaseMutex(g_pGOPCtxLocal->s32GOPXC_Mutex);
    }
#endif
}
#endif

//==================================================================
int Ioctl_GOP_DWIN_Init(void *pInstance)
{
    E_GOPD_FIFO_DEPTH gopdFifoDepth;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    //  MDrv_GOP_DWIN_SelectSourceFmt(GOPDWIN_DATA_FMT_YUV);
    //MDrv_GOP_DWIN_SelectSource(GOPDWIN_DATA_SRC_SCALAR);
    MDrv_GOP_DWIN_Set_CTRL0(g_pGOPCtxLocal, 0x0000, 0xFFFF); // Clear all
    MDrv_GOP_DWIN_Set_CTRL0(g_pGOPCtxLocal, 0x0000, 0x0080); // Set continuous capture


    // Set DMA burst access length
    gopdFifoDepth = (E_GOPD_FIFO_DEPTH)MDrv_GOP_GetRegForm(g_pGOPCtxLocal, GOPTYPE.DWIN, 0xff);
    switch (gopdFifoDepth)
    {
    case E_GOPD_FIFO_DEPTH_64:
        MDrv_GOP_SetGOPBWStrength(g_pGOPCtxLocal, 0xC, E_GOP_BURSTLEN_MIDDLE);
        break;
    case E_GOPD_FIFO_DEPTH_128:
        MDrv_GOP_SetGOPBWStrength(g_pGOPCtxLocal, 0xC, E_GOP_BURSTLEN_LONG);
        break;
    case E_GOPD_FIFO_DEPTH_256:
        MDrv_GOP_SetGOPBWStrength(g_pGOPCtxLocal, 0xC, E_GOP_BURSTLEN_LONG);
        break;
    default:
        printf("\nNot supported GOPD FIFO depth: %d\n", gopdFifoDepth); // bob.fu
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_Get_ScaleDst(void *pInstance, MS_U8 u8GOPNum, MS_U16 *pHScaleDst, MS_U16 *pVScaleDst)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    *pHScaleDst = g_pGOPCtxLocal->pGOPCtxShared->u16HScaleDst[u8GOPNum];
    *pVScaleDst = g_pGOPCtxLocal->pGOPCtxShared->u16VScaleDst[u8GOPNum];
    return GOP_API_SUCCESS;
}

//==================================================================
//////////////////////////////////////////////////
//  - GOP ioctl function.
//////////////////////////////////////////////////

int Ioctl_GOP_Init(void *pInstance, MS_U8 u8GOP, GOP_InitInfo *pGopInit)
{
    E_GOP_API_Result _GOPReslut;
    MS_U16 u8Idx;
#if 0	
    DRV_GopPaletteEntry pPalArray[256];
#else
    DRV_GopPaletteEntry *pPalArray = NULL;
#endif
    MS_BOOL bFirstInstance = FALSE;
    MS_BOOL resCtxInit;
    DRV_GOPDstType GopDst;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    //NULL pointer protect
    if (NULL == pGopInit)
    {
        WARNNING_INFO(printf("[%s][%d] input parameter is NULL\n", __FUNCTION__, __LINE__));
        //GOP_LIB_VER GOPLib;
        resCtxInit = _GOP_Init_Ctx(pInstance, &bFirstInstance);
        APIGOP_ASSERT(resCtxInit, printf("failed to init GOP context\n"));
        if (bFirstInstance)
        {
            g_pGOPCtxLocal->pGOPCtxShared->bDummyInit = TRUE;
        }

        if (MDrv_GOP_SetIOMapBase(g_pGOPCtxLocal) != TRUE)
        {
            APIGOP_ASSERT(FALSE, printf("\nget IO base fail"));
            return GOP_API_FAIL;
        }
    }
    else
    {
        //GOP_LIB_VER GOPLib;
        resCtxInit = _GOP_Init_Ctx(pInstance, &bFirstInstance);
        APIGOP_ASSERT(resCtxInit, printf("failed to init GOP context\n"));
        bFirstInit = FALSE;
        if (g_pGOPCtxLocal->pGOPCtxShared->bDummyInit)
        {
            //other GOP init should work as the first instance flow
            bFirstInstance = TRUE;
            g_pGOPCtxLocal->pGOPCtxShared->bInitShared = TRUE;
            g_pGOPCtxLocal->pGOPCtxShared->bDummyInit = FALSE;
        }

        if (!_GOP_IsGopNumValid(pInstance, u8GOP))
        {
            CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
            return GOP_API_FAIL;
        }

        //init GOP global variable
        _GOPReslut = GOP_API_SUCCESS;
        if (g_pGOPCtxLocal->pGOPCtxShared->bInitShared)
        {
            g_pGOPCtxLocal->Gwin_H_Dup = FALSE;
            g_pGOPCtxLocal->Gwin_V_Dup = FALSE;
            g_pGOPCtxLocal->current_winId = GOP0_Gwin0Id;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool1.u32GWinFB_Pool_BaseAddr = pGopInit->u32GOPRBAdr;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool1.u32GWinFB_Pool_MemLen = pGopInit->u32GOPRBLen;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool1.u32FirstFBIdInPool = INVALID_POOL_NEXT_FBID;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool1.poolId = GOP_WINFB_POOL1;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool2.u32GWinFB_Pool_BaseAddr = 0;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool2.u32GWinFB_Pool_MemLen = 0;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool2.u32FirstFBIdInPool = INVALID_POOL_NEXT_FBID;
            g_pGOPCtxLocal->pGOPCtxShared->fbPool2.poolId = GOP_WINFB_POOL2;
            g_pGOPCtxLocal->pGOPCtxShared->fb_currentPoolId = GOP_WINFB_POOL1;
        }

        if(pGopInit->bEnableVsyncIntFlip == TRUE)
            g_pGOPCtxLocal->pGOPCtxShared->bEnableVsyncIntFlip = TRUE;

        if (MDrv_GOP_SetIOMapBase(g_pGOPCtxLocal) != TRUE)
        {
            APIGOP_ASSERT(FALSE, printf("\nget IO base fail"));
            return GOP_API_FAIL;
        }

        g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8GOP] = pGopInit->u16PanelWidth;
        g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8GOP] = pGopInit->u16PanelHeight;
        g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[u8GOP] = pGopInit->u16PanelHStr;

        MDrv_GOP_GWIN_SetForceWrite(g_pGOPCtxLocal, TRUE);   //if use double buffer write in GOP init state, GOP HW cannot wait V-Sync to update register

#if 0
        MApi_GOP_GetChipCaps(E_GOP_CAP_VERSION,(MS_U32*)(&GOPLib),sizeof(GOP_LIB_VER));
        printf("\n[GOP%u, PID %d, TID 0x%ld][Driver Version]: %04ld, BuildNum: %04ld, ChangeList: %08ld\n", u8GOP, getpid(), MsOS_GetOSThreadID(), GOPLib.GOP_LibVer, GOPLib.GOP_BuildNum, GOPLib.GOP_ChangeList);
#endif

        if (g_pGOPCtxLocal->pGOPCtxShared->bInitShared)
        {
            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_MUX))
                MDrv_GOP_InitMux(g_pGOPCtxLocal);
        }
        if (FALSE == g_pGOPCtxLocal->pGOPCtxShared->bGopHasInitialized[u8GOP])
        {

            if (g_pGOPCtxLocal->pGopChipProperty->bPixelModeSupport)
            {
                g_pGOPCtxLocal->pGOPCtxShared->bPixelMode[u8GOP] = TRUE;
            }
            //Not initialized yet:
            MDrv_GOP_Sel(g_pGOPCtxLocal, u8GOP);
            GOP_GWIN_InitByGOP(pInstance, bFirstInstance, u8GOP);

            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_GWIN))
                MDrv_GOP_Init(g_pGOPCtxLocal, u8GOP, pGopInit->u32GOPRegdmaAdr, pGopInit->u32GOPRegdmaLen, pGopInit->bEnableVsyncIntFlip);
#if (FPGA_TEST)
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, TRUE);
            MDrv_GOP_GWIN_SetDstPlane(g_pGOPCtxLocal, u8GOP, E_DRV_GOP_DST_VOP,FALSE);
            MDrv_GOP_GWIN_SetStretchWin(u8GOP, E_GOP_DST_VOP, 0, 0, g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth, g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight);

            (_GOP_Map_APIDst2DRV_Enum(pInstance, E_GOP_DST_VOP, &GopDst));
            MDrv_GOP_SetGOPClk(g_pGOPCtxLocal, u8GOP, E_GOP_DST_VOP);
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP,DRV_GOPOUT_RGB);
#else
            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_SET_DST_OP))
            {
                MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, TRUE);
                MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);
                MDrv_GOP_GWIN_SetDstPlane(g_pGOPCtxLocal, u8GOP, E_DRV_GOP_DST_OP0,FALSE);
                (_GOP_Map_APIDst2DRV_Enum(pInstance, E_GOP_DST_OP0, &GopDst));
                MDrv_GOP_SetGOPClk(g_pGOPCtxLocal, u8GOP, GopDst);
            }

            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_STRETCHWIN))
                MDrv_GOP_GWIN_SetStretchWin(g_pGOPCtxLocal, u8GOP, 0, 0, g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8GOP], g_pGOPCtxLocal->pGOPCtxShared->u16PnlHeight[u8GOP]);
#endif
            if (u8GOP == 0)
            {
                MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP0);
            }
            else if (u8GOP == 1)
            {
                MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP1);
            }
            else if (u8GOP == 2)
            {
                MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP2);
            }
            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_ENABLE_TRANSCLR))
                MDrv_GOP_GWIN_EnableTransClr(g_pGOPCtxLocal,u8GOP, (GOP_TransClrFmt)0, FALSE);

            if (!g_pGOPCtxLocal->pGopChipProperty->bInitNotEnableGOPToSC)  //u4 mux init setting, enable GOP to SC in GOP init would cause problem
                MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, TRUE);
            if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_GWIN))
            {
                g_pGOPCtxLocal->pGOPCtxShared->bGopHasInitialized[u8GOP] = TRUE;
            }
        }

        MDrv_GOP_PerClientInit(g_pGOPCtxLocal, u8GOP, pGopInit->u32GOPRegdmaAdr, pGopInit->u32GOPRegdmaLen, pGopInit->bEnableVsyncIntFlip);

        if(!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit == (DRV_GOP_IGNOREINIT)E_DRV_GOP_ALL_IGNORE))
        {
            _GOP_InitHSPDByGOP(pInstance,u8GOP);
        }

        if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_IGNORE_PALETTE))
        {
	  	pPalArray  = malloc(sizeof(DRV_GopPaletteEntry)*256);
		if(pPalArray != NULL)
		{
			for (u8Idx = 0; u8Idx < 256; u8Idx++)
			{
			    pPalArray[u8Idx].RGB.u8A = pPalArray[u8Idx].RGB.u8R = pPalArray[u8Idx].RGB.u8G = pPalArray[u8Idx].RGB.u8B = 0;
			}

			MDrv_GOP_GWIN_SetPaletteOpt(g_pGOPCtxLocal,u8GOP, pPalArray, 0, 255, E_DRV_GOP_PAL_ARGB8888);
			MDrv_GOP_GWIN_2GSetPaletteOpt(g_pGOPCtxLocal,u8GOP, pPalArray, 0, 255);
			//MApi_GOP_GWIN_SetPalette();

			if(pPalArray != NULL)
			{
			   free(pPalArray);
			   pPalArray = NULL;
			}
		}
		else
		{
		   CRITICAL_INFO(printf("[%s][%d]Error!! malloc fail, pPalArray[256]\n", __FUNCTION__, __LINE__));
		}
		    
        }

        if (bFirstInstance)
        {
            MDrv_GOP_SetVOPNBL(g_pGOPCtxLocal, FALSE);
        }
        MDrv_GOP_GWIN_SetForceWrite(g_pGOPCtxLocal, FALSE);
    }
// apiGOP_DBG("[OUT]\n");
    return GOP_API_SUCCESS;
}


int Ioctl_GOP_GetCaps(void *pInstance, EN_GOP_CAPS eCapType, MS_U32 *pRet, MS_U32 ret_size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (eCapType != E_GOP_CAP_VERSION)
        APIGOP_CHECK_INIT();

    E_GOP_API_Result enRet = GOP_API_SUCCESS;
    switch (eCapType)
    {
    case E_GOP_CAP_WORD_UNIT:
        {
            *((MS_U16*)pRet) = g_pGOPCtxLocal->pGopChipProperty->WordUnit;
            break;
        }
    case E_GOP_CAP_GWIN_NUM:
        {
            *((MS_U16*)pRet) = g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum;
            break;
        }
    case E_GOP_CAP_VERSION:
        {
            char ver[] = MSIF_GOP_LIBVER;
            char blnum[] = MSIF_GOP_BUILDNUM;
            char cl[] = MSIF_GOP_CHANGELIST;
            GOP_LIB_VER *pLibVer;

            if (ret_size != sizeof(GOP_LIB_VER))
            {
                printf("[%s] ERROR, invalid input structure size :%ld on E_GOP_CAP_VERSION\n", __FUNCTION__, ret_size);
                return GOP_API_FAIL;
            }

            pLibVer = (GOP_LIB_VER *)pRet;

            pLibVer->GOP_LibVer = atoi(ver);
            pLibVer->GOP_BuildNum = atoi(blnum);
            pLibVer->GOP_ChangeList = atoi(cl);

            break;
        }
    case E_GOP_CAP_CONSALPHA_VALIDBITS:
        {
                *((DRV_GOP_CONSALPHA_BITS*)pRet) = g_pGOPCtxLocal->pGopChipProperty->Default_ConsAlpha_bits;
            break;
        }
    case E_GOP_CAP_PALETTE:
        {
            MS_U32 i;
            MS_U16 tmp;
            GOP_CAP_PAL_TYPE *pPal;

            pPal = (GOP_CAP_PAL_TYPE *)pRet;
            memset(pPal, 0, sizeof(GOP_CAP_PAL_TYPE));

            for (i = 0; i < _GOP_GetMaxGOPNum(pInstance); i++)
            {
                tmp = MDrv_GOP_GetRegForm(g_pGOPCtxLocal, i, 0xff);
                if (tmp & E_GOP_CAP_PAL_SIZE_MASK)
                {
                    pPal->GOP_NumOfTbl++;
                    pPal->GOP_PalTbl[i] = tmp & E_GOP_CAP_PAL_SIZE_MASK;
                    pPal->GOP_PalTblIdx |= 1 << i;
                }
            }

            break;
        }
    case E_GOP_CAP_DWIN:
        {
            PGOP_DWIN_CAP pDwinCap;
            pDwinCap = (PGOP_DWIN_CAP)pRet;
            Mdrv_GOP_GetDWinCapability(g_pGOPCtxLocal, (PDRV_GOP_DWIN_CAP)pDwinCap);
            break;
        }
    case E_GOP_CAP_GOP_MUX:
        {
            if (pRet != NULL)
            {
                    PST_DRV_GOP_MUX_CAP_EX pstDrvMuxCapEx = (PST_DRV_GOP_MUX_CAP_EX)pRet;
                    if(Mdrv_GOP_GetMuxPriority(g_pGOPCtxLocal, (PST_DRV_GOP_MUX_CAP_EX)pstDrvMuxCapEx))
                {
                        pstDrvMuxCapEx->bNeedSetMUX1ToIP0 = g_pGOPCtxLocal->pGopChipProperty->bNeedSetMUX1ToIP0;
                        pstDrvMuxCapEx->bNeedSetMUX3ToIP0 = g_pGOPCtxLocal->pGopChipProperty->bNeedSetMUX3ToIP0;
                        pstDrvMuxCapEx->bNewMux = g_pGOPCtxLocal->pGopChipProperty->bNewMux;
                }
                else
                {
                    enRet = GOP_API_FAIL;
                }
            }
            else
            {
                APIGOP_ASSERT(0, printf("Invalid input parameter: NULL pointer\n"));
            }

            break;
        }
    case E_GOP_CAP_PIXELMODE_SUPPORT:
        {
                *((MS_BOOL*)pRet) = g_pGOPCtxLocal->pGopChipProperty->bPixelModeSupport;
            break;
        }
    case E_GOP_CAP_STRETCH:
        {
            if (pRet != NULL)
            {
                MS_U8 i;
                PGOP_CAP_STRETCH_INFO pGOPStretchInfo = (PGOP_CAP_STRETCH_INFO)pRet;
                if (ret_size != sizeof(GOP_CAP_STRETCH_INFO))
                {
                    printf("[%s] ERROR, invalid input structure size :%ld on E_GOP_CAP_STRETCH\n", __FUNCTION__, ret_size);
                    return GOP_API_FAIL;
                }
                pGOPStretchInfo->GOP_VStretch_Support = 0;
                for (i = 0; i < MIN(sizeof(g_pGOPCtxLocal->pGopChipProperty->bGOPWithVscale), (sizeof(pGOPStretchInfo->GOP_VStretch_Support) * 8)); i++)
                {
                    if (g_pGOPCtxLocal->pGopChipProperty->bGOPWithVscale[i])
                    {
                        pGOPStretchInfo->GOP_VStretch_Support |= BIT(i);
                    }
                    else
                    {
                        pGOPStretchInfo->GOP_VStretch_Support &= ~(BIT(i));
                    }
                }
            }
            else
            {
                APIGOP_ASSERT(0, printf("Invalid input parameter: NULL pointer\n"));
            }
            break;
        }
    default:
        printf("[%s]not support GOP capability case: %d\n", __FUNCTION__, eCapType);
        enRet = GOP_API_FAIL;
    }

    return enRet;
}


static MS_BOOL GOP_PreInit_Ctx(void *pInstance, MS_BOOL *pbFirstDrvInstant)
{
    MS_BOOL bNeedInitShared = FALSE;
    MS_U16 u16LoopTime = 0x0;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    _GetGOPEnum(pInstance);

    if (g_pGOPCtxLocal)
    {
        if (bFirstInit)
        {
            *pbFirstDrvInstant = TRUE;
        }
        else
        {
            *pbFirstDrvInstant = FALSE;
        }

        return TRUE;
    }
    g_pGOPCtxLocal = Drv_GOP_Init_Context(pInstance, &bNeedInitShared);
    if (NULL == g_pGOPCtxLocal)
        return FALSE;
    if (pbFirstDrvInstant)
        *pbFirstDrvInstant = bNeedInitShared;

#if 0
    if(bNeedInitShared)
    {
        MS_U32 gId;

        u16LoopTime = 0;
        g_pGOPCtxLocal->pGOPCtxShared->gwinMap = NULL;
        while (u16LoopTime<0xFF)
        {
            g_pGOPCtxLocal->pGOPCtxShared->gwinMap = malloc(MDrv_GOP_GetTotalGwinNum(g_pGOPCtxLocal)*sizeof(GWIN_MAP));
            if (g_pGOPCtxLocal->pGOPCtxShared->gwinMap!=NULL)
            {
                break;
            }
            u16LoopTime++;
        }
        if (g_pGOPCtxLocal->pGOPCtxShared->gwinMap!=NULL)
        {
            for( gId=0; gId<MApi_GOP_GWIN_GetTotalGwinNum(); gId++)
            g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gId].u32CurFBId= GWIN_ID_INVALID;
        }
        else
        {
            printf("\n[%s]allocate memory for gwinMap info fail!!!!!!!", __FUNCTION__);
        }

        u16LoopTime = 0;
        g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType = NULL;
        while (u16LoopTime<0xFF)
        {
            g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType = malloc(MApi_GOP_GWIN_GetMaxGOPNum()*sizeof(MS_S32));
            if (g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType!=NULL)
            {
                break;
            }
            u16LoopTime++;
        }
        if (g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType!=NULL)
        {
            for( gId=0; gId<MApi_GOP_GWIN_GetMaxGOPNum(); gId++)
            g_pGOPCtxLocal->pGOPCtxShared->s32OutputColorType[gId] = -1;
        }
        else
        {
            printf("\n[%s]allocate memory for s32OutputColorType info fail!!!!!!!", __FUNCTION__);
        }
    }
#endif

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
    g_pGOPCtxLocal->u32GOPClientId = getpid();
#else
    g_pGOPCtxLocal->u32GOPClientId = ++g_pGOPCtxLocal->pGOPCtxShared->u32ClientIdAllocator;
    if(0 == g_pGOPCtxLocal->u32GOPClientId)
    g_pGOPCtxLocal->u32GOPClientId = ++g_pGOPCtxLocal->pGOPCtxShared->u32ClientIdAllocator;
#endif

    g_pGOPCtxLocal->sMirrorInfo = NULL;
    u16LoopTime = 0;
    while (u16LoopTime < 0xFF)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        if (g_pGOPCtxLocal->sMirrorInfo == NULL)
            g_pGOPCtxLocal->sMirrorInfo = kmalloc(g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum * sizeof(GOP_GwinMirror_Info), GFP_KERNEL);
#else
        if (g_pGOPCtxLocal->sMirrorInfo == NULL)
            g_pGOPCtxLocal->sMirrorInfo = malloc(g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum * sizeof(GOP_GwinMirror_Info));
#endif

        if (g_pGOPCtxLocal->sMirrorInfo != NULL)
        {
            break;
        }
        u16LoopTime++;

    }

    if (g_pGOPCtxLocal->sMirrorInfo == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nallocate memory for gwin mirror info fail!!!"));
    }
    else
    {
        memset(g_pGOPCtxLocal->sMirrorInfo, 0, g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum * sizeof(GOP_GwinMirror_Info));
    }

    memset(&g_pGOPCtxLocal->sGOPConfig, 0, sizeof(GOP_Config));

    memset(g_pGOPCtxLocal->MS_MuxGop, 0, MAX_GOP_MUX_SUPPORT * sizeof(MS_U8));
    g_pGOPCtxLocal->IsChgMux = FALSE;
    g_pGOPCtxLocal->IsClkClosed = FALSE;
    g_pGOPCtxLocal->u8ChgIpMuxGop = 0xFF;
#if GOP_LOCK_SUPPORT
    char GOP_MUTEX[] = "GOP_Mutex";

    g_pGOPCtxLocal->s32GOPMutexCnt = 0;
    g_pGOPCtxLocal->s32GOPMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)GOP_MUTEX, MSOS_PROCESS_SHARED);
    if (0 > g_pGOPCtxLocal->s32GOPMutex)
    {
        MS_CRITICAL_MSG(printf("%screate gop mutex fail\n", __FUNCTION__));
        return FALSE;
    }
    g_pGOPCtxLocal->s32GOPLock = -1;
#endif

#if GOP_XC_LOCK_SUPPORT
    char XC_MUTEX[] = "_XC_Mutex";

    g_pGOPCtxLocal->s32GOPXC_MutexCnt = 0;
    g_pGOPCtxLocal->s32GOPXC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char *)XC_MUTEX, MSOS_PROCESS_SHARED);
    if (0 > g_pGOPCtxLocal->s32GOPXC_Mutex)
    {
        MS_CRITICAL_MSG(printf("[%s][%06d] create XC mutex fail _GOPXC_Mutex=0x%lx\n", __FUNCTION__, __LINE__, g_pGOPCtxLocal->s32GOPXC_Mutex));
    }
    g_pGOPCtxLocal->s32GOPXC_Lock = -1;
#endif

    return TRUE;
}


int Ioctl_GOP_SetConfig(void *pInstance, EN_GOP_CONFIG_TYPE cfg_type, MS_U32 *pCfg, MS_U32 u32Size)
{
    MS_BOOL bFirstInstance;
    MS_BOOL resCtxInit;
    EN_GOP_IGNOREINIT *pGOPIgnoreInit;
    DRV_GOP_CONSALPHA_BITS *pGOPAlphaBits;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (cfg_type)
    {
    case E_GOP_IGNOREINIT:
        resCtxInit = GOP_PreInit_Ctx(pInstance, &bFirstInstance);
        APIGOP_ASSERT(resCtxInit, printf("failed to init GOP context\n"));

        /*To control that do not affect the normal flow in MApi_GOP_Init and MApi_GOP_InitByGOP*/
        bFirstInit = TRUE;



        pGOPIgnoreInit = (EN_GOP_IGNOREINIT *)(pCfg);
        g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit = (DRV_GOP_IGNOREINIT)(*pGOPIgnoreInit);
        break;
    case E_GOP_CONSALPHA_BITS:


        pGOPAlphaBits = (DRV_GOP_CONSALPHA_BITS *)(pCfg);
        *(g_pGOPCtxLocal->pGopConsAlphaBits) =  *pGOPAlphaBits;
        break;
    default:

        break;
    }

    return GOP_API_SUCCESS;




}

MS_U32 Ioctl_GOP_GetConfig(void *pInstance, EN_GOP_CONFIG_TYPE cfg_type, MS_U32 *pCfg, MS_U32 u32Size)
{
    EN_GOP_IGNOREINIT *penGOPIgnoreInit;
    DRV_GOP_CONSALPHA_BITS *penGOPAlphaBits;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (cfg_type)
    {
    case E_GOP_IGNOREINIT:
        penGOPIgnoreInit = (EN_GOP_IGNOREINIT *)pCfg;
        *penGOPIgnoreInit = g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit;
        break;
    case E_GOP_CONSALPHA_BITS:
        penGOPAlphaBits = (DRV_GOP_CONSALPHA_BITS *)pCfg;
        *penGOPAlphaBits = *(g_pGOPCtxLocal->pGopConsAlphaBits);
        break;
    default:

        break;
    }

    return GOP_API_SUCCESS;
}

MS_U32 Ioctl_GOP_SetProperty(void *pInstance, EN_GOP_PROPERTY type, MS_U32 gop, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (type)
    {
    case E_GOP_MIUSEL:
        {
            E_DRV_GOP_SEL_TYPE miusel;
            if (u32Size != sizeof(EN_GOP_SEL_TYPE))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            miusel = *((EN_GOP_SEL_TYPE*)pSet);
            GOP_MIUSel(pInstance, gop, miusel);

            break;
        }
    case E_GOP_CONTRAST:
        {
            PGOP_CONTRAST pRGB = (PGOP_CONTRAST)pSet;
            if (u32Size != sizeof(GOP_CONTRAST))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            GOP_SetContrast(pInstance, gop, pRGB->y, pRGB->u, pRGB->v);


            break;
        }
    case E_GOP_BRIGHTNESS:
        {
            PGOP_BRIGHTNESS pBri = (PGOP_BRIGHTNESS)pSet;
            if (u32Size != sizeof(GOP_BRIGHTNESS))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            GOP_SetBrightness(pInstance, gop, pBri->Brightness, pBri->bMSB);
            break;
        }
    case E_GOP_OUTPUT_COLOR:
        {
            EN_GOP_OUTPUT_COLOR output;
            if (u32Size != sizeof(EN_GOP_OUTPUT_COLOR))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            output = *((EN_GOP_OUTPUT_COLOR*)pSet);
            GOP_SetOutputColor(pInstance, gop, output);

            break;

        }
    case E_GOP_UV_SWAP:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            bEn = *((MS_BOOL*)pSet);
            GOP_SetUVSwap(pInstance, gop, bEn);

            break;
        }
    case E_GOP_YC_SWAP:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            bEn = *((MS_BOOL*)pSet);
            GOP_SetYCSwap(pInstance, gop, bEn);

            break;
        }
    case E_GOP_HSTART:
        {
            MS_U16 u16PanelHStr;

            if (u32Size != sizeof(MS_U16))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            u16PanelHStr = *((MS_U16*)pSet);
            GOP_SetHStart(pInstance, gop, u16PanelHStr);

            break;
        }
    case E_GOP_TRANSCOLOR:
        {
            PGOP_TRANSCOLOR ptrans_clr;
            MS_U32 clr;
            MS_U32 fullclr = 0;
            EN_GOP_TRANSCLR_FMT fmt;

            if (u32Size != sizeof(GOP_TRANSCOLOR))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            ptrans_clr = (PGOP_TRANSCOLOR)pSet;

            clr = ptrans_clr->color;
            fmt = ptrans_clr->fmt;

            if (fmt == GOPTRANSCLR_FMT0)
            {
                /***** !RGB555 *****/
                fullclr = ((clr >> 10) & 0x1f) << 3;      // R
                fullclr |= ((clr >> 12) & 0x7);
                fullclr = fullclr << 8;
                fullclr |= ((clr >> 5) & 0x1f) << 3;      // G
                fullclr |= ((clr >> 7) & 0x7);
                fullclr = fullclr << 8;
                fullclr |= (clr & 0x1f) << 3;           // B
                fullclr |= ((clr >> 2) & 0x7);           // B
            }
            else if (fmt == GOPTRANSCLR_FMT1)
            {
                /***** index mode *****/
                    MDrv_GOP_GWIN_ReadPalette(g_pGOPCtxLocal,gop, clr, &fullclr);
            }
            else if (fmt == GOPTRANSCLR_FMT2)  //ARGB8888
            {
                fullclr = ptrans_clr->color;
            }

            MDrv_GOP_GWIN_SetTransClr_8888(g_pGOPCtxLocal,gop, fullclr, 0);

            MDrv_GOP_GWIN_EnableTransClr(g_pGOPCtxLocal,gop, (GOP_TransClrFmt)GOPTRANSCLR_FMT0,ptrans_clr->bEn);
            break;

        }
    case E_GOP_PROGRESSIVE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            bEn = *((MS_BOOL*)pSet);
            GOP_SetProgressive(pInstance, gop, bEn);
            break;
        }
    case E_GOP_ALPHAINVERSE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);
            GOP_SetAlphaInverse(pInstance, gop, bEn);
            break;
        }
    case E_GOP_PREALPHAMODE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);

            MDrv_GOP_SetGOPEnable2Mode1(g_pGOPCtxLocal, gop, bEn);

            break;
        }
    case E_GOP_FIELD_INVERSE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);

            MDrv_GOP_GWIN_SetFieldInver(g_pGOPCtxLocal,gop ,bEn);

            break;
        }
    case E_GOP_OUTPUT_TIMING:
        {
            PST_GOP_TIMING_INFO pstInfo;
            if (u32Size != sizeof(ST_GOP_TIMING_INFO))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            pstInfo = (PST_GOP_TIMING_INFO)pSet;
            GOP_SetPanelTiming(pInstance, gop, pstInfo);
            break;
        }
    case E_GOP_PIXELBASE_ENABLE:
        {
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            g_pGOPCtxLocal->pGOPCtxShared->bPixelMode[gop]=*((MS_BOOL*)pSet);
            break;
        }
    case E_GOP_CLK:
        {
            EN_GOP_DST_TYPE eDstType;
            if (u32Size != sizeof(EN_GOP_DST_TYPE))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            eDstType = *((EN_GOP_DST_TYPE*)pSet);
            GOP_SetGOPClk(pInstance, gop, eDstType);
            break;
        }
    case E_GOP_FORCE_WRITE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);
            GOP_GWIN_SetForceWrite(pInstance, bEn);
            break;
        }
    case E_GOP_BANK_FORCE_WRITE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);
            GOP_GWIN_SetBnkForceWrite(pInstance, gop, bEn);
            break;
        }
    case E_GOP_3D_LR_EXCHANGE:
        {
            GOP_Set3D_LR_FrameExchange(pInstance, gop);
            break;
        }
    case E_GOP_VE_OSD:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);
            GOP_Enable_VEOSD(pInstance, gop, bEn);
            break;
        }
    case E_GOP_OUTPUT_LAYER_SWITCH:
        {
            GOP_OutputLayerSwitch(pInstance, gop);
            break;
        }
    case E_GOP_SWAP_OVERLAP_WIN:
        {
            MS_U8 u8win;
            if (u32Size != sizeof(MS_U8))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            u8win = *((MS_U8*)pSet);
            GOP_GWIN_SwapOverlapWin(pInstance, gop, u8win);
            break;
        }
    case E_GOP_BW_STRENGTH:
        {
            EN_GOP_BW_STRENGTH eGOPBWStr;
            if (u32Size != sizeof(EN_GOP_BW_STRENGTH))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            eGOPBWStr = (EN_GOP_BW_STRENGTH)*pSet;
            GOP_SetGOPBWStrength(pInstance, eGOPBWStr, gop);
            break;
        }
    case E_GOP_LB_COUPLE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);
            GOP_EnableLBCouple(pInstance, gop, bEn);
            break;
        }
    case E_GOP_RELATIVE_WIN_PRIO:
        {
            GOP_GwinPri *pGwinPri = (GOP_GwinPri *)pSet;
            if (u32Size != sizeof(GOP_GwinPri))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            GOP_GWIN_SetRelativeWinPrio(pInstance, gop, pGwinPri);
            break;
        }
    case E_GOP_YUV:
        {
            GOP_SetGOPYUV(pInstance, gop);
            break;
        }
    case E_GOP_REG_UPDATED:
        {
            break;
        }
    case E_GOP_RESOURCE:
        {
            GOP_ResetGOP(pInstance, gop);
            break;
        }

    case E_GOP_MULTI_ALPHA_ENABLE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            bEn = *((MS_BOOL*)pSet);

            MDrv_GOP_GWIN_EnableMultiAlpha(g_pGOPCtxLocal, gop, bEn);
            break;
        }
    default:
        {
            printf("[%s] invalid input case:%d\n", __FUNCTION__, type);
        }

    }

    return GOP_API_SUCCESS;

}


MS_U32 Ioctl_GOP_GetProperty(void *pInstance, EN_GOP_PROPERTY type, MS_U32 gop, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (type)
    {
    case E_GOP_MIUSEL:
        {
            E_DRV_GOP_SEL_TYPE *pMIUSel = (E_DRV_GOP_SEL_TYPE *)pSet;
            if (u32Size != sizeof(E_DRV_GOP_SEL_TYPE))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            *pMIUSel = (E_DRV_GOP_SEL_TYPE)GOP_GetMIUSel(pInstance, gop);

            break;
        }
    case E_GOP_CONTRAST:
        {
            PGOP_CONTRAST pRGB = (PGOP_CONTRAST)pSet;
            if (u32Size != sizeof(GOP_CONTRAST))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            GOP_GetContrast(pInstance, gop, &(pRGB->y), &(pRGB->u), &(pRGB->v));

            break;
        }
    case E_GOP_BRIGHTNESS:
        {
            PGOP_BRIGHTNESS pBri = (PGOP_BRIGHTNESS)pSet;
            MS_U16 *pu16Brightness = (MS_U16 *)&(pBri->Brightness);
            if (u32Size != sizeof(GOP_BRIGHTNESS))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            GOP_GetBrightness(pInstance, gop, pu16Brightness, &(pBri->bMSB));
            break;
        }
    case E_GOP_ALPHAINVERSE:
        {
            MS_BOOL *pbAlphaInverse = (MS_BOOL *)pSet;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            *pbAlphaInverse = GOP_GetAlphaInverse(pInstance, gop);

            break;
        }
    case E_GOP_HSCALE_RATIO:
        {
            MS_U16 *pHscale = (MS_U16 *)pSet;
            if (u32Size != sizeof(MS_U16))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            *pHscale = GOP_Get_Hscale(pInstance, gop);
            break;
        }
    case E_GOP_VSCALE_RATIO:
        {
            MS_U16 *pVscale = (MS_U16 *)pSet;
            if (u32Size != sizeof(MS_U16))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            *pVscale = GOP_Get_Vscale(pInstance, gop);
            break;
        }
    case E_GOP_HSTART:
        {
            MS_U16 *pu16HStart = (MS_U16 *)pSet;
            if (u32Size != sizeof(MS_U16))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            *pu16HStart = g_pGOPCtxLocal->pGOPCtxShared->u16PnlHStr[gop];
            break;
        }
    case E_GOP_OUTPUT_TIMING:
        {
            PST_GOP_TIMING_INFO pstInfo;
            if (u32Size != sizeof(ST_GOP_TIMING_INFO))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            pstInfo = (PST_GOP_TIMING_INFO)pSet;
            GOP_GetPanelTiming(pInstance, gop, pstInfo);
            break;
        }
    case E_GOP_PIXELBASE_ENABLE:
        {
            PGOP_SET_PROPERTY_PARAM pstInfo;
            MS_BOOL *pbEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            pstInfo = (PGOP_SET_PROPERTY_PARAM)pSet;
            pbEn = (MS_BOOL *)(pstInfo->pSetting);
            *pbEn = g_pGOPCtxLocal->pGOPCtxShared->bPixelMode[pstInfo->gop_idx];
            break;
        }
    case E_GOP_MULTI_ALPHA_ENABLE:
        {
            PGOP_SET_PROPERTY_PARAM pstInfo;
            MS_BOOL *pbEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            pstInfo = (PGOP_SET_PROPERTY_PARAM)pSet;
            pbEn = (MS_BOOL *)(pstInfo->pSetting);
            *pbEn = MDrv_GOP_GWIN_IsMultiAlphaEnable(g_pGOPCtxLocal, pstInfo->gop_idx);
            break;
        }

    case E_GOP_PREALPHAMODE:
    case E_GOP_FIELD_INVERSE:
    case E_GOP_FORCE_WRITE:
    case E_GOP_BANK_FORCE_WRITE:
    case E_GOP_3D_LR_EXCHANGE:
    case E_GOP_VE_OSD:
    case E_GOP_OUTPUT_LAYER_SWITCH:
    case E_GOP_SWAP_OVERLAP_WIN:
    case E_GOP_LB_COUPLE:
    case E_GOP_YUV:
    case E_GOP_OUTPUT_COLOR:
    case E_GOP_UV_SWAP:
    case E_GOP_YC_SWAP:
    case E_GOP_RELATIVE_WIN_PRIO:
        break;
    case E_GOP_BW_STRENGTH:
        {
            EN_GOP_BW_STRENGTH *peGOPBWStr = (EN_GOP_BW_STRENGTH *)pSet;
            if (u32Size != sizeof(EN_GOP_BW_STRENGTH))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            GOP_GetGOPBWStrength(pInstance, gop, peGOPBWStr);
            break;
        }
    case E_GOP_REG_UPDATED:
        {
            MS_BOOL *pUpdated = (MS_BOOL *)pSet;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            *pUpdated = GOP_IsRegUpdated(pInstance, gop);
            break;
        }
    case E_GOP_MAXFBNUM:
        {
            MS_U32 *pMAXFBNum = (MS_U32 *)pSet;
            if (u32Size != sizeof(MS_U32))
            {
                CRITICAL_INFO(printf("[%s][%d] input structure size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            *pMAXFBNum = DRV_MAX_GWIN_FB_SUPPORT;
            break;
        }
    default:
        printf("[%s] This status(%d) can't be query\n", __FUNCTION__, type);
        break;

    }

    return GOP_API_SUCCESS;

}

static MS_BOOL _IsGopInOPMux(void *pInstance, MS_U8 u8GopNum)
{
    MS_U8 i, GetGopNum;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    for (i = 0; i < MDrv_GetMaxMuxOPNum(); i++)
    {
        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &GetGopNum, (Gop_MuxSel)i);
        if (u8GopNum == GetGopNum)
        {
            return TRUE;
        }
    }
    return FALSE;
}


int Ioctl_GOP_SetDst(void *pInstance, MS_U8 u8GOP, EN_GOP_DST_TYPE dsttype)
{
    MS_U8 u8GetGOPNum, u8Mux0GopNum, i;
    DRV_GOPDstType GopDst;
    Gop_MuxSel OPMux = MAX_GOP_MUX_SUPPORT;

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if ((!_GOP_IsGopNumValid(pInstance, u8GOP)))
    {
        CRITICAL_INFO(printf("[%s][%d]GOP %d  is out of range\n", __FUNCTION__, __LINE__, u8GOP));
        return GOP_API_FAIL;
    }

    DBGCHECK_INFO(printf("---%s %d: PID[%lu], TID[%ld] u8GOP=%u, DstType=0x%lx\n", __FUNCTION__, __LINE__, (MS_U32)getpid(), MsOS_GetOSThreadID(), u8GOP, (MS_U32)dsttype));

    /*Set GOP dst and check input dst type is vaild or not for different chip first */
    (_GOP_Map_APIDst2DRV_Enum(pInstance, dsttype, &GopDst));
    (MDrv_GOP_GWIN_SetDstPlane(g_pGOPCtxLocal, u8GOP, GopDst,FALSE));

    // Set GOP dst output blending to SC/Mixer
    switch (GopDst)
    {

    case E_DRV_GOP_DST_IP0:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)  //t8, there are new mux for ip/mvop
        {
            //printf("\n=============%s===============\n",__FUNCTION__);
            //printf("Set to New IP Mux GOP:%d\n",u8GOP);
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_IP0_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }
        else         //In IP case, gop should open in OP mux0 ~ mux3
        {

            if (g_pGOPCtxLocal->pGopChipProperty->bNeedSetMUX1ToIP0)
            {
                if (!_IsGopInMux1(pInstance, u8GOP))
                {
                    /*save current mux setting*/
                    for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                    {
                        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
                    }
                    g_pGOPCtxLocal->IsChgMux = TRUE;
                    /*force change input parameter u8GOP to mux1*/
                    for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                    {

                        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                        if (u8GOP == u8GetGOPNum)
                        {
                            MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8Mux0GopNum, E_GOP_MUX1);
                            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8Mux0GopNum, (Gop_MuxSel)i);
                            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_MUX1);
                            g_pGOPCtxLocal->u8ChgIpMuxGop = u8Mux0GopNum;
                            if (u8Mux0GopNum < _GOP_GetMaxGOPNum(pInstance))
                                _GOP_InitHSPDByGOP(pInstance, u8Mux0GopNum);
                            _GOP_InitHSPDByGOP(pInstance, u8GOP);
                        }
                    }

                }
            }
            else if (g_pGOPCtxLocal->pGopChipProperty->bNeedSetMUX3ToIP0)
            {
                if (!_IsGopInMux3(pInstance, u8GOP))
                {
                    /*save current mux setting*/
                    for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                    {
                        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
                    }
                    g_pGOPCtxLocal->IsChgMux = TRUE;
                    /*force change input parameter u8GOP to mux3*/
                    for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                    {
                        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                        if (u8GOP == u8GetGOPNum)
                        {
                            MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8Mux0GopNum, E_GOP_MUX3);
                            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8Mux0GopNum, (Gop_MuxSel)i);
                            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_MUX3);
                            if (u8Mux0GopNum < _GOP_GetMaxGOPNum(pInstance))
                                _GOP_InitHSPDByGOP(pInstance, u8Mux0GopNum);
                            _GOP_InitHSPDByGOP(pInstance, u8GOP);
                        }
                    }
                }
            }
            else
            {
                if (!_IsGopInMux0(pInstance, u8GOP)) //jasmine:FIXME
                {
                    /*save current mux setting*/
                    for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                    {
                        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
                    }
                    g_pGOPCtxLocal->IsChgMux = TRUE;
                    /*force change input parameter u8GOP to mux0*/
                    for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                    {
                        MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                        if (u8GOP == u8GetGOPNum)
                        {
                            MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8Mux0GopNum, E_GOP_MUX0);
                            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8Mux0GopNum, (Gop_MuxSel)i);
                            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_MUX0);
                            if (u8Mux0GopNum < _GOP_GetMaxGOPNum(pInstance))
                                _GOP_InitHSPDByGOP(pInstance, u8Mux0GopNum);
                            _GOP_InitHSPDByGOP(pInstance, u8GOP);
                        }
                    }
                }
            }
        }
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        _GOP_SetOCCapability(pInstance, u8GOP, GopDst);
        if (u8GOP == 0)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_IP0_SEL_GOP0);
        else if (u8GOP == 1)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_IP0_SEL_GOP1);
        else
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_IP0_SEL_GOP2);
        //MApi_GOP_GWIN_OutputColor(GOPOUT_YUV);
            MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, TRUE);
        break;

    case E_DRV_GOP_DST_MIXER2VE:
        //jasmine:FIXME
        if (g_pGOPCtxLocal->pGopChipProperty->bGOPMixerToVE)
        {
            MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
            MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, TRUE);
                MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP,DRV_GOPOUT_YUV);
            MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);

            MDrv_GOP_SetGOPClk(g_pGOPCtxLocal, u8GOP, E_DRV_GOP_DST_MIXER2VE);

            MDrv_GOP_GWIN_SetMixerDstPlane(g_pGOPCtxLocal, u8GOP, GopDst);

            (MDrv_GOP_MIXER_SetGOPEnable2Mixer(g_pGOPCtxLocal, u8GOP, TRUE));
            (MDrv_GOP_Set_VE_MUX(g_pGOPCtxLocal, (GOPTYPE.MIXER - 1), TRUE));
        }
        else
        {
            printf("Not Support GOPMixerToVE in this chip version.[%s][%d]\n", __FUNCTION__, __LINE__);
        }
            MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_OP0:
        _GOP_SetOCCapability(pInstance, u8GOP, GopDst);

        if ((g_pGOPCtxLocal->pGopChipProperty->bNewMux) && (!_IsGopInOPMux(pInstance, u8GOP)))
        {
            //_CleanGopMux(u8GOP);
            OPMux = _GetNonUseOPMux(pInstance);
            if (OPMux != MAX_GOP_MUX_SUPPORT)
            {
                MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, OPMux);
            }
            else
            {
                MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_MUX2);
            }
        }

        /*
                  For U4, mux value may has mixer and at least one GOP is not in mux
                  To check if the GOP number is in MUX0 to MUX3
                  If not, find the mux value which is mixer, replace mixer by the GOP number
        */
        if (!_IsGopInMux0To3(pInstance, u8GOP))
        {
            /*save current mux setting*/
            for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
            {
                MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
            }
            g_pGOPCtxLocal->IsChgMux = TRUE;
            /*force set input parameter u8GOP to the mux which is mixer*/
            for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
            {
                MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                if ((GOPTYPE.MIXER - 1) == u8GetGOPNum) //Find mixer(0x4)
                {
                    MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, (Gop_MuxSel)i);
                }
            }
        }

        if (g_pGOPCtxLocal->pGopChipProperty->bNeedSetMUX3ToIP0) //For u4 and k1, mux3 only to IP (can not to OP)
        {
            if (_IsGopInMux3(pInstance, u8GOP))
            {
                /*save current mux setting*/
                for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                {
                    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
                }
                g_pGOPCtxLocal->IsChgMux = TRUE;

                /*force change input parameter u8GOP exchange mux with GOP3*/
                for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                {
                    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                    if (GOPTYPE.GOP3 == u8GetGOPNum)
                    {
                        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, (Gop_MuxSel)i);
                        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, GOPTYPE.GOP3, E_GOP_MUX3);
                    }
                }
            }
        }
        else if (g_pGOPCtxLocal->pGopChipProperty->bNeedSetMUX1ToIP0)
        {
            if (_IsGopInMux1(pInstance, u8GOP) && _IsGopInMux0To3(pInstance, g_pGOPCtxLocal->u8ChgIpMuxGop))
            {
                /*save current mux setting*/
                for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                {
                    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
                }
                g_pGOPCtxLocal->IsChgMux = TRUE;

                /*force change input parameter u8GOP exchange mux with GOP3*/
                for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
                {
                    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                    if (g_pGOPCtxLocal->u8ChgIpMuxGop == u8GetGOPNum)
                    {
                        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, (Gop_MuxSel)i);
                        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GetGOPNum, E_GOP_MUX1);
                        g_pGOPCtxLocal->u8ChgIpMuxGop = 0xFF;
                        if (u8GetGOPNum < _GOP_GetMaxGOPNum(pInstance))
                            _GOP_InitHSPDByGOP(pInstance, u8GetGOPNum);
                    }
                }
            }
        }

        if (u8GOP == 0)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP0);
        else if (u8GOP == 1)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP1);
        else
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP2);

        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, TRUE);

        _GOP_InitHSPDByGOP(pInstance, u8GOP);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_OP1:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            //_CleanGopMux(u8GOP);
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_OP1_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }

        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_SC1OP_SEL);
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);
        MDrv_GOP_GWIN_SetVSyncinverse(g_pGOPCtxLocal,u8GOP, FALSE);
        _GOP_InitHSPDByGOP(pInstance, u8GOP);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_VOP:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            //_CleanGopMux(u8GOP);
            printf("\n=============%s===============\n", __FUNCTION__);
            printf("Set to New MVOP Mux GOP:%d\n", u8GOP);
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_VOP0_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }
        else if (!_IsGopInMux0(pInstance, u8GOP))
        {
            g_pGOPCtxLocal->IsChgMux = TRUE;
            /*save current mux setting*/
            for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
            {
                MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
            }

            /*force change input parameter u8GOP to mux0*/
            for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
            {
                MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                if (u8GOP == u8GetGOPNum)
                {
                    MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8Mux0GopNum, E_GOP_MUX0);
                    MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8Mux0GopNum, (Gop_MuxSel)i);
                    MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_MUX0);
                    if (u8Mux0GopNum < _GOP_GetMaxGOPNum(pInstance))
                        _GOP_InitHSPDByGOP(pInstance, u8Mux0GopNum);
                    _GOP_InitHSPDByGOP(pInstance, u8GOP);
                }
            }
        }

#if (FPGA_TEST)
        GOP_SetOutputColor(pInstance, u8GOP, GOPOUT_RGB);
#else
        GOP_SetOutputColor(pInstance, u8GOP, GOPOUT_YUV);
#endif

        if (u8GOP == 0)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP0);
        else if (u8GOP == 1)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP1);
        else
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP2);

        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_MVOP_SEL);
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        _GOP_SetOCCapability(pInstance, u8GOP, GopDst);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_VOP_SUB:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_VOP0_SUB_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }

#if (FPGA_TEST)
            GOP_SetOutputColor(pInstance,u8GOP,GOPOUT_RGB);
#else
            GOP_SetOutputColor(pInstance,u8GOP,GOPOUT_YUV);
#endif
        if (u8GOP == 0)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP0);
        else if (u8GOP == 1)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP1);
        else
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP2);

        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_SUBMVOP_SEL);
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;
    case E_DRV_GOP_DST_IP1:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            //_CleanGopMux(u8GOP);
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_IP1_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_IP1_SEL);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, TRUE);
        break;

    case E_DRV_GOP_DST_IP0_SUB:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            //_CleanGopMux(u8GOP);
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_IP0_SUB_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_SUBIP0_SEL);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, TRUE);
        break;
    case E_DRV_GOP_DST_DIP:
        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            //_CleanGopMux(u8GOP);
            MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_DIP_MUX);
            _GOP_InitHSPDByGOP(pInstance, u8GOP);
        }
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_DIP_SEL);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;
    case E_DRV_GOP_DST_MIXER2OP:

        if (g_pGOPCtxLocal->pGopChipProperty->bNewMux)
        {
            //_CleanGopMux(u8GOP);
            if (!_IsGopInMux0To2(pInstance, (GOPTYPE.MIXER - 1)))
            {
                OPMux = _GetNonUseOPMux(pInstance);
                if (OPMux != MAX_GOP_MUX_SUPPORT)
                {
                    MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, (GOPTYPE.MIXER - 1), OPMux);
                }
                else
                {
                    MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, (GOPTYPE.MIXER - 1), E_GOP_MUX2);
                }
            }
        }
        else
        {

            g_pGOPCtxLocal->IsChgMux = TRUE;
            /*save current mux setting*/
            for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
            {
                MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &g_pGOPCtxLocal->MS_MuxGop[i], (Gop_MuxSel)i);
                //printf("\n %s, before E_GOP_DST_MIXER2VE--->MS_MuxGop[%d]=%d\n",__FUNCTION__,i,u8GetGOPNum);
            }
            /*Set current mux to mixer.*/
            for (i = 0; i < MAX_GOP_MUX_SUPPORT; i++)
            {
                MDrv_GOP_GWIN_GetMux(g_pGOPCtxLocal, &u8GetGOPNum, (Gop_MuxSel)i);
                if (u8GOP == u8GetGOPNum)
                {
                    //Set current GOP mux to mixer
                    MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, (GOPTYPE.MIXER - 1), (Gop_MuxSel)i);
                    break;
                }
            }

        }
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, (GOPTYPE.MIXER - 1), TRUE); //To find the mux which is mixer, open SC blending
                                                                             //MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, TRUE);
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_YUV);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);

        MDrv_GOP_GWIN_SetMixerDstPlane(g_pGOPCtxLocal, u8GOP, GopDst);
        //MDrv_GOP_GWIN_SetHSPipe(g_pGOPCtxLocal, u8GOP, 0);
        _GOP_InitHSPDByGOP(pInstance, u8GOP);
        (MDrv_GOP_MIXER_SetGOPEnable2Mixer(g_pGOPCtxLocal, u8GOP, TRUE));
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_MIXER2OP1:
        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, (GOPTYPE.MIXER - 1), E_GOP_OP1_MUX);
        MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_SC1OP_SEL);
        //MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, GOPTYPE.MIXER, TRUE); //To find the mux which is mixer, open SC blending
        //MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, FALSE);
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP, DRV_GOPOUT_YUV);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, TRUE);

        MDrv_GOP_GWIN_SetMixerDstPlane(g_pGOPCtxLocal, u8GOP, GopDst);
        //MDrv_GOP_GWIN_SetHSPipe(g_pGOPCtxLocal, u8GOP, 0);
        _GOP_InitHSPDByGOP(pInstance, u8GOP);
        (MDrv_GOP_MIXER_SetGOPEnable2Mixer(g_pGOPCtxLocal, u8GOP, TRUE));
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;

    case E_DRV_GOP_DST_FRC:
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        _GOP_InitHSPDByGOP(pInstance, u8GOP); //require update registers at the end of this function after dwin capture in OP
        _GOP_SetOCCapability(pInstance, u8GOP, GopDst);
            MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;
    case E_DRV_GOP_DST_VE:
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_GWIN_EnableProgressive(g_pGOPCtxLocal, u8GOP, FALSE);
            MDrv_GOP_GWIN_OutputColor(g_pGOPCtxLocal,u8GOP,DRV_GOPOUT_YUV);
        MDrv_GOP_GWIN_SetAlphaInverse(g_pGOPCtxLocal, u8GOP, FALSE);

        MDrv_GOP_SetGOPClk(g_pGOPCtxLocal, u8GOP, E_DRV_GOP_DST_VE);

            MDrv_GOP_GWIN_SetVSyncinverse(g_pGOPCtxLocal,u8GOP, TRUE);
        //MDrv_GOP_Mask_First_Hsync(g_pGOPCtxLocal,u8GOP, TRUE );
        if (!(g_pGOPCtxLocal->sGOPConfig.eGopIgnoreInit & E_GOP_BOOTLOGO_IGNORE_VEOSDEN))
        {
            (MDrv_GOP_Set_VE_MUX(g_pGOPCtxLocal, u8GOP, TRUE));
        }
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;
    case E_DRV_GOP_DST_GOPScaling:
        _GOP_SetOCCapability(pInstance, u8GOP, GopDst);
        if ((g_pGOPCtxLocal->pGopChipProperty->bNewMux) && (!_IsGopInMux0To2(pInstance, u8GOP)))
        {
            //_CleanGopMux(u8GOP);
            OPMux = _GetNonUseOPMux(pInstance);
            if (OPMux != MAX_GOP_MUX_SUPPORT)
            {
                MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, OPMux);
            }
            else
            {
                MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_MUX2);
            }
        }

        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_GS_MUX);

        if (u8GOP == 0)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP0);
        else if (u8GOP == 1)
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP1);
        else
            MDrv_GOP_SetIPSel2SC(g_pGOPCtxLocal, MS_DRV_NIP_SEL_GOP2);

        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, TRUE);
        _GOP_InitHSPDByGOP(pInstance, u8GOP);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;
    case E_DRV_GOP_DST_BYPASS:
        _GOP_SetOCCapability(pInstance, u8GOP, GopDst);
        _GOP_InitHSPDByGOP(pInstance, u8GOP);
        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, u8GOP, E_GOP_BYPASS_MUX0);
        MDrv_GOP_SetGOPEnable2SC(g_pGOPCtxLocal, u8GOP, FALSE);
        MDrv_GOP_SelfFirstHs(g_pGOPCtxLocal, u8GOP, FALSE);
        break;
    default:
        MS_ASSERT(0);
        return GOP_API_ENUM_NOT_SUPPORTED;
    }

    (MDrv_GOP_SetGOPClk(g_pGOPCtxLocal, u8GOP, GopDst));
    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_GetDst(void *pInstance, MS_U32 gop, MS_U32 *pDst)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    DRV_GOPDstType enDst = E_DRV_GOP_DST_INVALID;

    (MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, gop, &enDst));
    _GOP_Map_DRVDst2API_Enum_(pInstance, (EN_GOP_DST_TYPE *)pDst, enDst);
    return GOP_API_SUCCESS;

}

int Ioctl_GOP_SetMux(void *pInstance, PGOP_SETMUX pGopMuxConfig, MS_U32 u32SizeOfMuxInfo)
{
    MS_U8 i;
    Gop_MuxSel EnGOPMux;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (u32SizeOfMuxInfo != sizeof(GOP_SETMUX) || (pGopMuxConfig == NULL))
    {
        MS_CRITICAL_MSG(printf("\nthe GOP_MuxConfig is NULL or struct mismatch in drivr\n"));
        return GOP_API_FAIL;
    }
    DBGCHECK_INFO(printf("---%s %d: PID[%lu], TID[%ld] MuxCount=%u\n", __FUNCTION__, __LINE__, (MS_U32)getpid(), MsOS_GetOSThreadID(), pGopMuxConfig->u8MuxCounts));

#if 0
    for(i=0; i<pGopMuxConfig->u8MuxCounts; i++)
    {
        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, pGopMuxConfig->GopMux[i].u8GopIndex, (Gop_MuxSel) pGopMuxConfig->GopMux[i].u8MuxIndex);
    }

#endif

    for (i = 0; i < pGopMuxConfig->MuxCount; i++)
    {
        _GOP_MAP_MUX_Enum(pInstance, (EN_Gop_MuxSel)pGopMuxConfig->mux[i], &EnGOPMux);
        MDrv_GOP_GWIN_SetMux(g_pGOPCtxLocal, pGopMuxConfig->gop[i], EnGOPMux);
    }

    return GOP_API_SUCCESS;

}


int Ioctl_GOP_SetMirror(void *pInstance, MS_U32 gop, EN_GOP_MIRROR_TYPE mirror)
{
    // To do: to modify and control each gop mirror setting.
    switch (mirror)
    {
    case E_GOP_MIRROR_NONE:
    default:
        {
            GOP_GWIN_SetHMirror(pInstance, FALSE);
            GOP_GWIN_SetVMirror(pInstance, FALSE);
            break;
        }
    case E_GOP_MIRROR_H_ONLY:
        {
            GOP_GWIN_SetHMirror(pInstance, TRUE);
            GOP_GWIN_SetVMirror(pInstance, FALSE);
            break;
        }
    case E_GOP_MIRROR_V_ONLY:
        {
            GOP_GWIN_SetHMirror(pInstance, FALSE);
            GOP_GWIN_SetVMirror(pInstance, TRUE);
            break;
        }
    case E_GOP_MIRROR_HV:
        {
            GOP_GWIN_SetHMirror(pInstance, TRUE);
            GOP_GWIN_SetVMirror(pInstance, TRUE);
            break;
        }

    }

    return GOP_API_SUCCESS;
}

int Ioctl_GOP_GetMirror(void *pInstance, MS_U32 gop, EN_GOP_MIRROR_TYPE *mirror)
{
    MS_BOOL bHMirror;
    MS_BOOL bVMirror;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    //need refine
    bHMirror = g_pGOPCtxLocal->pGOPCtxShared->bHMirror;
    bVMirror = g_pGOPCtxLocal->pGOPCtxShared->bVMirror;

    if ((bHMirror == TRUE) && (bVMirror == TRUE))
    {
        *mirror = E_GOP_MIRROR_HV;
    }
    else if ((bHMirror == TRUE) && (bVMirror == FALSE))
    {
        *mirror = E_GOP_MIRROR_H_ONLY;
    }
    else if ((bHMirror == FALSE) && (bVMirror == TRUE))
    {
        *mirror = E_GOP_MIRROR_V_ONLY;
    }
    else
    {
        *mirror = E_GOP_MIRROR_NONE;
    }

    return GOP_API_SUCCESS;
}


int Ioctl_GOP_GetStatus(void *pInstance, EN_GOP_STATUS type, MS_U32 *pStatus, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (pStatus == NULL)
    {
        printf("[%s] ERROR - Input null pointer on type:%d\n", __FUNCTION__, type);
        return GOP_API_INVALID_PARAMETERS;
    }

    switch (type)
    {
    case E_GOP_STATUS_INIT:
        {
            PGOP_INIT_STATUS ptr = (PGOP_INIT_STATUS)pStatus;
            if (u32Size != sizeof(GOP_INIT_STATUS))
            {
                return GOP_API_INVALID_PARAMETERS;
            }
            ptr->bInit = g_pGOPCtxLocal->pGOPCtxShared->bGopHasInitialized[ptr->gop_idx];

            break;
        }
    case E_GOP_STATUS_GOP_MAXNUM:
        {
            MS_U8 *pMaxGopNum = (MS_U8 *)pStatus;
            *pMaxGopNum = MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal);
            break;
        }
    case E_GOP_STATUS_GWIN_MAXNUM:
        {
            PGOP_GWIN_NUM ptr = (PGOP_GWIN_NUM)pStatus;
            if (u32Size != sizeof(GOP_GWIN_NUM))
            {
                printf("[%s] Error (%d)\n", __FUNCTION__, __LINE__);
                return (GOP_API_INVALID_PARAMETERS);
            }
            if (ptr->gop_idx > MDrv_GOP_GetMaxGOPNum(g_pGOPCtxLocal))
            {
                printf("[%s] Error (%d)\n", __FUNCTION__, __LINE__);
                return (GOP_API_INVALID_PARAMETERS);
            }

            ptr->gwin_num = MDrv_GOP_GetGwinNum(g_pGOPCtxLocal, ptr->gop_idx);
            break;
        }
    case E_GOP_STATUS_GWIN_TOTALNUM:
        {
            MS_U8 *pGwinNum = (MS_U8 *)pStatus;
            if (u32Size != sizeof(MS_U8))
            {
                printf("[%s] Error (%d)\n", __FUNCTION__, __LINE__);
                return (GOP_API_INVALID_PARAMETERS);
            }
            *pGwinNum = (MS_U8)g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum;
            break;

        }
    case E_GOP_STATUS_VERSION:
        {
            break;
        }
    case E_GOP_STATUS_MUX:
        {

            break;
        }
    case E_GOP_STATUS_CURRENT_GOP:
        {
            MS_U32 *pCurrentGop = (MS_U32 *)pStatus;
            *pCurrentGop = GOP_GetCurrentGOP(pInstance);
            break;
        }
    case E_GOP_STATUS_CURRENT_GWIN:
        {
            MS_U32 *pCurrentGwin = (MS_U32 *)pStatus;
            *pCurrentGwin = GOP_GetCurrentGWin(pInstance);
            break;
        }
    case E_GOP_STATUS_CURRENT_FBID:
        {
            MS_U32 *pCurrentFbid = (MS_U32 *)pStatus;
            *pCurrentFbid = GOP_GetCurrentFBID(pInstance);
            break;
        }
    default:
        {
            printf("[%s] invalid input case:%d\n", __FUNCTION__, type);
        }

    }

    return GOP_API_SUCCESS;

}




//******************************************************************************
/// API for Test Pattern Type Select
/// @param eTstPatternType \b IN: Test Pattern Type
/// @param MS_U32 u32ARGB  \b IN: ARGB Color
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
int Ioctl_GOP_TestPattern(void *pInstance, EN_GOP_TST_PATTERN eTstPatternType, MS_U32 u32ARGB)
{
    DRV_GOP_TSTPATTERN drv_TstPattern;
    EN_GOP_TST_PATTERN eCurTPMode;
    MS_U8 u8CurGOP = 0;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif


    u8CurGOP = MDrv_GOP_Get(g_pGOPCtxLocal);

    if (MDrv_GOP_TestPattern_IsVaild(g_pGOPCtxLocal) != GOP_SUCCESS)
    {
        return GOP_API_FAIL;
    }

    //Back up
    eCurTPMode = (EN_GOP_TST_PATTERN)g_pGOPCtxLocal->pGOPCtxShared->GOP_TestPatternMode;
    if (eCurTPMode == E_GOP_TP_DISABLE)
    {
        printf("[%s (%d)] cur:%d input mode:%d GOPBK Read\n", __FUNCTION__, __LINE__, eCurTPMode, eTstPatternType);
        MDrv_GOP_BNK(g_pGOPCtxLocal, E_DRV_GOP_BNK_READ);
    }

    g_pGOPCtxLocal->pGOPCtxShared->GOP_TestPatternMode = eTstPatternType;

    memset(&drv_TstPattern, 0, sizeof(DRV_GOP_TSTPATTERN));

    switch (eTstPatternType)
    {
    case E_GOP_TP_DISABLE:
        drv_TstPattern.bTSTClr_En = 0;
        printf("[%s (%d)] cur:%d input mode:%d GOPBK write\n", __FUNCTION__, __LINE__, eCurTPMode, eTstPatternType);
        MDrv_GOP_BNK(g_pGOPCtxLocal, E_DRV_GOP_BNK_WRITE);
        return GOP_API_SUCCESS;
        break;
    case E_GOP_TP_GRAY16:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (16 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 1;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 16);
        break;
    case E_GOP_TP_GRAY16_INVERSE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8R_stc = 0xFF;
        drv_TstPattern.u8G_stc = 0xFF;
        drv_TstPattern.u8B_stc = 0xFF;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (16 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 0;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 16);
        break;
    case E_GOP_TP_GRAY32:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (32 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 1;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 32) - 1;
        break;
    case E_GOP_TP_GRAY32_INVERSE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8R_stc = 0xFF;
        drv_TstPattern.u8G_stc = 0xFF;
        drv_TstPattern.u8B_stc = 0xFF;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (32 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 0;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 32) - 1;
        break;
    case E_GOP_TP_GRAY64:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (64 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 1;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 64);
        break;
    case E_GOP_TP_GRAY64_INVERSE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8R_stc = 0xFF;
        drv_TstPattern.u8G_stc = 0xFF;
        drv_TstPattern.u8B_stc = 0xFF;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (64 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 0;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 64);
        break;
    case E_GOP_TP_GRAY256:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (256 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 1;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 256);
        break;
    case E_GOP_TP_GRAY256_INVERSE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8R_stc = 0xFF;
        drv_TstPattern.u8G_stc = 0xFF;
        drv_TstPattern.u8B_stc = 0xFF;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 256 / (256 - 1);
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 0;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = (g_pGOPCtxLocal->pGOPCtxShared->u16PnlWidth[u8CurGOP] / 256);
        break;
    case E_GOP_TP_BLACK:
        drv_TstPattern.bTSTClr_En = 1;
        break;
    case E_GOP_TP_WHITE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8R_stc = 0xFF;
        drv_TstPattern.u8G_stc = 0xFF;
        drv_TstPattern.u8B_stc = 0xFF;
        break;
    case E_GOP_TP_CLR:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8R_stc = u32ARGB >> 16;
        drv_TstPattern.u8G_stc = u32ARGB >> 8;
        drv_TstPattern.u8B_stc = u32ARGB;
        break;
    case E_GOP_TP_BW_DOT:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 127;
        drv_TstPattern.u8VR_inc = drv_TstPattern.u8VG_inc = drv_TstPattern.u8VB_inc = 127;
        drv_TstPattern.u8HR_step = 0x1;
        drv_TstPattern.u8HG_step = 0x1;
        drv_TstPattern.u8HB_step = 0x1;
        drv_TstPattern.u8VR_step = 0x1;
        drv_TstPattern.u8VG_step = 0x1;
        drv_TstPattern.u8VB_step = 0x1;
        break;
    case E_GOP_TP_BW_CROSS:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 0xFF;
        drv_TstPattern.u8VR_inc = drv_TstPattern.u8VG_inc = drv_TstPattern.u8VB_inc = 0xFF;
        drv_TstPattern.u8HR_step = drv_TstPattern.u8HG_step = drv_TstPattern.u8HB_step = 127;
        drv_TstPattern.u8VR_step = drv_TstPattern.u8VG_step = drv_TstPattern.u8VB_step = 127;
        break;
    case E_GOP_TP_BW_LINE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 127;
        drv_TstPattern.u8VR_inc = drv_TstPattern.u8VG_inc = drv_TstPattern.u8VB_inc = 0;
        break;
    case E_GOP_TP_CLR_BAR:
        drv_TstPattern.u8R_stc = 0xFF;
        drv_TstPattern.u8G_stc = 0xFF;
        drv_TstPattern.u8B_stc = 0xFF;
        break;
    case E_GOP_TP_CLR_BAR_INVERSE:
        drv_TstPattern.bTSTClr_En = 1;
        drv_TstPattern.u8TSTClr_Hdup = 3;
        drv_TstPattern.u8TSTClr_Vdup = 0;

        drv_TstPattern.u8HR_inc = drv_TstPattern.u8HG_inc = drv_TstPattern.u8HB_inc = 255;
        drv_TstPattern.u8HR_inc_signz = drv_TstPattern.u8HG_inc_signz = drv_TstPattern.u8HB_inc_signz = 1;
        drv_TstPattern.u8HR_step = 121;
        drv_TstPattern.u8HG_step = 243;
        drv_TstPattern.u8HB_step = 60;
        break;
    default:
        return GOP_API_ENUM_NOT_SUPPORTED;
        break;
    }

    drv_TstPattern.u8TSTClr_Alpha = u32ARGB >> 24;

    _GOP_TestPattern(pInstance, &drv_TstPattern);

    return GOP_API_SUCCESS;

}



int Ioctl_GOP_Set_Stretch(void *pInstance, EN_GOP_STRETCH_TYPE enStretchType, MS_U32 gop, PGOP_STRETCH_INFO pStretchInfo)
{
    MS_BOOL bEn = FALSE;
    DRV_GOPDstType enGopDst = E_DRV_GOP_DST_INVALID;
    EN_GOP_DST_TYPE enApiGopDst = MAX_GOP_DST_SUPPORT;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_GWIN_GetDstPlane(g_pGOPCtxLocal, gop, &enGopDst);

    if (enStretchType & E_GOP_STRETCH_WIN)
    {
        _GOP_Map_DRVDst2API_Enum_(pInstance, &enApiGopDst, enGopDst);
        GOP_SetStretchWin(pInstance, gop, enApiGopDst, pStretchInfo->SrcRect.x, pStretchInfo->SrcRect.y, pStretchInfo->SrcRect.w, pStretchInfo->SrcRect.h);
    }

    if (enStretchType & E_GOP_STRETCH_WIN_POSITION)
    {
        GOP_SetStretchWinPosition(pInstance, gop, pStretchInfo->SrcRect.x, pStretchInfo->SrcRect.y);
    }

    if (enStretchType & E_GOP_STRETCH_HSCALE)
    {
        if (pStretchInfo->SrcRect.w != pStretchInfo->DstRect.w)
        {
            bEn = TRUE;
        }
        GOP_Set_Hscale(pInstance, gop, bEn, pStretchInfo->SrcRect.w, pStretchInfo->DstRect.w);
    }

    if (enStretchType & E_GOP_STRETCH_VSCALE)
    {
        if (pStretchInfo->SrcRect.h != pStretchInfo->DstRect.h)
        {
            bEn = TRUE;
        }
        GOP_Set_Vscale(pInstance, gop, bEn, pStretchInfo->SrcRect.h, pStretchInfo->DstRect.h);
    }

    if (enStretchType & E_GOP_STRETCH_HSTRETCH_MODE)
    {
        GOP_GWIN_Set_HStretchMode(pInstance, gop, pStretchInfo->enHMode);
    }

    if (enStretchType & E_GOP_STRETCH_VSTRETCH_MODE)
    {
        GOP_GWIN_Set_VStretchMode(pInstance, gop, pStretchInfo->enVMode);
    }

    if (enStretchType & E_GOP_STRETCH_TRANSPCOLOR_MODE)
    {
        GOP_GWIN_Set_TranspColorStretchMode(pInstance, gop, pStretchInfo->enTColorMode);
    }
    return GOP_API_SUCCESS;
}


int Ioctl_GOP_Get_Stretch(void *pInstance, EN_GOP_STRETCH_TYPE enStretchType, MS_U32 gop, PGOP_STRETCH_INFO pStretchInfo)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (enStretchType & E_GOP_STRETCH_WIN)
    {
        MS_U16 x, y, w, h;

        MDrv_GOP_GWIN_Get_StretchWin(g_pGOPCtxLocal, gop, &x, &y, &w, &h);
        pStretchInfo->SrcRect.x = (MS_U32)x;
        pStretchInfo->SrcRect.y = (MS_U32)y;
        pStretchInfo->SrcRect.w = (MS_U32)w;
        pStretchInfo->SrcRect.h = (MS_U32)h;
    }

    return GOP_API_SUCCESS;
}


int Ioctl_GOP_GWin_SetProperty(void *pInstance, EN_GOP_GWIN_PROPERTY type, MS_U32 gwin, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_GWIN_BLENDING:
        {
            PGOP_GWIN_BLENDING pInfo = (PGOP_GWIN_BLENDING)pSet;

            if (u32Size != sizeof(GOP_GWIN_BLENDING))
                return GOP_API_INVALID_PARAMETERS;

            GOP_GWIN_SetBlending(pInstance,(MS_U8)gwin,pInfo->bEn, (MS_U8)pInfo->Coef);

            break;
        }
    case E_GOP_GWIN_NEWAPLHA:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = *((MS_BOOL*)pSet);

            GOP_GWIN_SetNewAlphaMode(pInstance, gwin, bEn);
            break;
        }
    case E_GOP_GWIN_ENABLE:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = *((MS_BOOL*)pSet);

            GOP_GWIN_SetEnable(pInstance, gwin, bEn);

            break;
        }
    case E_GOP_GWIN_BLINK:
        {
            PGOP_GWIN_BLINK pRate = (PGOP_GWIN_BLINK)pSet;
            if (u32Size != sizeof(GOP_GWIN_BLINK))
                return GOP_API_INVALID_PARAMETERS;

            GOP_GWIN_SetBlink(pInstance,pRate->bEn,gwin, pRate->rate);
            break;
        }
    case E_GOP_GWIN_SHARE:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;
            bEn = *((MS_BOOL*)pSet);
            GOP_GWIN_SetGWinShared(pInstance,(MS_U8)gwin, bEn);
            break;
        }
    case E_GOP_GWIN_SHARE_CNT:
        {
            MS_U16 u16SharedCnt;
            if (u32Size != sizeof(MS_U16))
                return GOP_API_INVALID_PARAMETERS;
            u16SharedCnt = *((MS_U16*)pSet);
            GOP_GWIN_SetGWinSharedCnt(pInstance,(MS_U8)gwin, u16SharedCnt);
            break;
        }
    case E_GOP_GWIN_SWITCH_2_GWIN:
        {
            GOP_GWIN_Switch2Gwin(pInstance,(MS_U8)gwin,pSet);
            break;
        }
    case E_GOP_GWIN_3D_MODE:
        {
            GOP_GWIN_3D_MODE *pst3DMode = (GOP_GWIN_3D_MODE *)pSet;
            if (u32Size != sizeof(GOP_GWIN_3D_MODE))
                return GOP_API_INVALID_PARAMETERS;
            GOP_Set3DOSDMode(pInstance,(MS_U8)gwin,(MS_U16)pst3DMode->u32MainFBId,(MS_U16)pst3DMode->u32SubFBId,pst3DMode->en3DMode);
            break;
        }
    case E_GOP_GWIN_EXIST:
        {
            break;
        }
    case E_GOP_GWIN_FREE_ID:
        {
            break;
        }
    case E_GOP_GWIN_GET_FB:
        {
            break;
        }
    case E_GOP_GWIN_GET_GOP:
        {
            break;
        }
    }


    return GOP_API_SUCCESS;

}



int Ioctl_GOP_GWin_GetProperty(void *pInstance, EN_GOP_GWIN_PROPERTY type, MS_U32 gwin, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_GWIN_BLENDING:
        {
            break;
        }
    case E_GOP_GWIN_NEWAPLHA:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = (MS_BOOL)*pSet;

            break;
        }
    case E_GOP_GWIN_ENABLE:
        {
            MS_BOOL *pbEn = (MS_BOOL *)pSet;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            *pbEn = GOP_GWIN_IsEnable(pInstance, gwin);

            break;
        }
    case E_GOP_GWIN_BLINK:
        {
            break;
        }
    case E_GOP_GWIN_SHARE:
        {
            break;
        }
    case E_GOP_GWIN_SHARE_CNT:
        {
            break;
        }
    case E_GOP_GWIN_SWITCH_2_GWIN:
        {
            break;
        }
    case E_GOP_GWIN_3D_MODE:
        {
            break;
        }
    case E_GOP_GWIN_EXIST:
        {
            MS_U8 *pExist = (MS_U8 *)pSet;

            if (u32Size != sizeof(MS_U8))
                return GOP_API_INVALID_PARAMETERS;

            *pExist = (MS_U8)GOP_GWIN_IsGwinExist(pInstance, gwin);
            break;
        }
    case E_GOP_GWIN_FREE_ID:
        {
            MS_U8 *pFreeId = (MS_U8 *)pSet;

            if (u32Size != sizeof(MS_U8))
                return GOP_API_INVALID_PARAMETERS;
            *pFreeId = (MS_U8)GOP_GWIN_GetFreeWinID(pInstance);
            break;
        }
    case E_GOP_GWIN_GET_FB:
        {
            MS_U32 *pFbId = (MS_U32 *)pSet;

            if (u32Size != sizeof(MS_U32))
                return GOP_API_INVALID_PARAMETERS;

            *pFbId = GOP_GWIN_GetFBfromGWIN(pInstance, gwin);
            break;
        }
    case E_GOP_GWIN_GET_GOP:
        {
            MS_U32 *pGOP = (MS_U32 *)pSet;
            if (u32Size != sizeof(MS_U32))
                return GOP_API_INVALID_PARAMETERS;

            *pGOP = (MS_U32)_DumpGopByGwinId(pInstance, gwin);
            break;
        }

    }


    return GOP_API_SUCCESS;

}

int Ioctl_GOP_FB_SetProperty(void *pInstance, EN_GOP_FB_PROPERTY type, MS_U32 FBId, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_FB_POOLID:
        {
            MS_U8 *pPool = (MS_U8 *)pSet;

            if (u32Size != sizeof(MS_U8))
                return GOP_API_INVALID_PARAMETERS;

            GOP_FB_SEL(pInstance, *pPool);
            break;
        }

    case E_GOP_FB_EXIST:
        {
            break;
        }
    }

    return GOP_API_SUCCESS;
}

int Ioctl_GOP_FB_GetProperty(void *pInstance, EN_GOP_FB_PROPERTY type, MS_U32 FBId, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_FB_POOLID:
        {
            MS_U8 *pPool = (MS_U8 *)pSet;

            if (u32Size != sizeof(MS_U8))
                return GOP_API_INVALID_PARAMETERS;

            *pPool = (MS_U8)GOP_FB_Get(pInstance);
            break;
        }

    case E_GOP_FB_EXIST:
        {
            MS_U8 *pExist = (MS_U8 *)pSet;

            if (u32Size != sizeof(MS_U8))
                return GOP_API_INVALID_PARAMETERS;
            *pExist = (MS_U8)GOP_GWIN_Is32FBExist(pInstance, FBId);
            break;
        }
    }

    return GOP_API_SUCCESS;
}

int Ioctl_GOP_DWIN_SetProperty(void *pInstance, EN_GOP_DWIN_PROPERTY type, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (type)
    {
    case E_GOP_DWIN_R2Y:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = (MS_BOOL)*pSet;
            MDrv_GOP_DWIN_EnableR2YCSC(g_pGOPCtxLocal, bEn);
            break;
        }
    case E_GOP_DWIN_UV_SWAP:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = (MS_BOOL)*pSet;
            MDrv_GOP_DWIN_SetUVSwap(g_pGOPCtxLocal, bEn);
            break;
        }
    case E_GOP_DWIN_UV_SAMPLE:
        {
            EN_GOP_DWIN_UVSAMPLE_Mode enMode;

            if (u32Size != sizeof(EN_GOP_DWIN_UVSAMPLE_Mode))
                return GOP_API_INVALID_PARAMETERS;

            enMode = (EN_GOP_DWIN_UVSAMPLE_Mode)*pSet;
            MDrv_GOP_DWIN_SetUVSample(g_pGOPCtxLocal, (MS_U8)enMode);
            break;
        }
    case E_GOP_DWIN_SOURCE_SCAN_TYPE:
        {
            EN_GOP_DWIN_SCAN_MODE enMode;

            if (u32Size != sizeof(EN_GOP_DWIN_SCAN_MODE))
                return GOP_API_INVALID_PARAMETERS;

            enMode = (EN_GOP_DWIN_SCAN_MODE)*pSet;
            MDrv_GOP_DWIN_SelectSourceScanType(g_pGOPCtxLocal, enMode);
            break;
        }
    case E_GOP_DWIN_ALPHA_VALUE:
        {
            MS_U8 u8Value;

            if (u32Size != sizeof(MS_U8))
                return GOP_API_INVALID_PARAMETERS;

            u8Value = (MS_U8)*pSet;
            MDrv_GOP_DWIN_SetAlphaValue(g_pGOPCtxLocal, u8Value);
            break;
        }
    case E_GOP_DWIN_ALPHA_SOURCE:
        {
            EN_GOP_DWIN_ALPHA_SRC enSource;

            if (u32Size != sizeof(EN_GOP_DWIN_ALPHA_SRC))
                return GOP_API_INVALID_PARAMETERS;

            enSource = (EN_GOP_DWIN_ALPHA_SRC)*pSet;
            MDrv_GOP_DWIN_SetAlphaSrc(g_pGOPCtxLocal, (MS_U8)enSource);
            break;
        }
    case E_GOP_DWIN_ALPHA_INVERSE:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = (MS_BOOL)*pSet;
            MDrv_GOP_DWIN_SetAlphaInverse(g_pGOPCtxLocal, bEn);
            break;
        }
    case E_GOP_DWIN_SKIP_FRAME:
        {
            MS_U32 u32Count;

            if (u32Size != sizeof(MS_U32))
                return GOP_API_INVALID_PARAMETERS;

            u32Count = (MS_U32)*pSet;
            MDrv_GOP_DWIN_SetSkipFrame(g_pGOPCtxLocal, u32Count);
            break;
        }
    case E_GOP_DWIN_PINPON:
        {
            PGOP_PINPON_INFO pPinpon = (PGOP_PINPON_INFO)pSet;
            if (u32Size != sizeof(GOP_PINPON_INFO))
                return GOP_API_INVALID_PARAMETERS;
            MDrv_GOP_Set_BufferPINPON(g_pGOPCtxLocal, pPinpon->u32Addr0, pPinpon->u32Addr1);
            break;
        }
    case E_GOP_DWIN_INTERRUP_INFO:
        {
            break;
        }
    case E_GOP_DWIN_CAPTURE_INFO:
        {
            break;
        }
    }
    return GOP_API_SUCCESS;
}
int Ioctl_GOP_DWIN_GetProperty(void *pInstance, EN_GOP_DWIN_PROPERTY type, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_DWIN_R2Y:
        {
            break;
        }
    case E_GOP_DWIN_UV_SWAP:
        {
            break;
        }
    case E_GOP_DWIN_UV_SAMPLE:
        {
            break;
        }
    case E_GOP_DWIN_SOURCE_SCAN_TYPE:
        {
            break;
        }
    case E_GOP_DWIN_ALPHA_VALUE:
        {
            break;
        }
    case E_GOP_DWIN_ALPHA_SOURCE:
        {
            break;
        }
    case E_GOP_DWIN_ALPHA_INVERSE:
        {
            break;
        }
    case E_GOP_DWIN_SKIP_FRAME:
        {
            break;
        }
    case E_GOP_DWIN_PINPON:
        {
            break;
        }
    case E_GOP_DWIN_INTERRUP_INFO:
        {
            break;
        }
    case E_GOP_DWIN_CAPTURE_INFO:
        {
            break;
        }

    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_MIXER_SetProperty(void *pInstance, EN_GOP_MIXER_PROPERTY type, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_MIXER_OUTPUT_TIMING:
        {
            PGOP_MIXER_TIMING_INFO pMTInfo = (PGOP_MIXER_TIMING_INFO)pSet;

            if (u32Size != sizeof(GOP_MIXER_TIMING_INFO))
                return GOP_API_INVALID_PARAMETERS;
            GOP_MIXER_SetOutputTiming(pInstance, pMTInfo->mode, &(pMTInfo->pMT));
            break;
        }
    case E_GOP_MIXER_TO_OP_OUTPUT_TIMING:
        {
            GOP_Mixer2OPTiming *pMT = (GOP_Mixer2OPTiming *)pSet;

            if (u32Size != sizeof(GOP_Mixer2OPTiming))
                return GOP_API_INVALID_PARAMETERS;
            GOP_MIXER_SetMIXER2OPOutputTiming(pInstance, pMT);
            break;
        }
    case E_GOP_MIXER_V_FILTER:
        {
            MS_BOOL bEn;

            if (u32Size != sizeof(MS_BOOL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = (MS_BOOL)*pSet;
            GOP_MIXER_EnableVfilter(pInstance, bEn);
            break;
        }
    case E_GOP_MIXER_OLD_BLENDING_MODE:
        {
            GOP_MixerOldBlendingMode *pMOBM;

            if (u32Size != sizeof(GOP_MixerOldBlendingMode))
                return GOP_API_INVALID_PARAMETERS;

            pMOBM = (GOP_MixerOldBlendingMode *)pSet;
            GOP_MIXER_EnableOldBlendingMode(pInstance, pMOBM->u8GOP, pMOBM->bEnable);
            break;
        }
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_MIXER_GetProperty(void *pInstance, EN_GOP_MIXER_PROPERTY type, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_MIXER_OUTPUT_TIMING:
        {
            break;
        }
    case E_GOP_MIXER_TO_OP_OUTPUT_TIMING:
        {
            break;
        }
    case E_GOP_MIXER_V_FILTER:
        {
            break;
        }
    case E_GOP_MIXER_OLD_BLENDING_MODE:
        {
            break;
        }
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_VE_SetProperty(void *pInstance, EN_GOP_VE_PROPERTY type, MS_U32 *pSet, MS_U32 u32Size)
{
    switch (type)
    {
    case E_GOP_VE_OUTPUT_TIMING:
        {
            PGOP_VE_TIMING_INFO pVEInfo = (PGOP_VE_TIMING_INFO)pSet;

            if (u32Size != sizeof(GOP_VE_TIMING_INFO))
                return GOP_API_INVALID_PARAMETERS;
            GOP_VE_SetOutputTiming(pInstance, pVEInfo->mode);
            break;
        }
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_MapFB2Win(void *pInstance, MS_U32 fbid, MS_U32 gwin)
{
    return GOP_MapFB2Win(pInstance, fbid, gwin);
}

int Ioctl_GOP_Dwin_Capture(void *pInstance, PGOP_CAPTURE_INFO pinfo)
{
    DRV_GOP_DWIN_INFO  DwinInfo;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    memset(&DwinInfo, 0, sizeof(DRV_GOP_DWIN_INFO));

    DwinInfo.u16HPixelStart = pinfo->rect.x;
    DwinInfo.u16HPixelEnd   = pinfo->rect.x + pinfo->rect.w;
    DwinInfo.u16VPixelStart = pinfo->rect.y;
    DwinInfo.u16VPixelEnd   = pinfo->rect.y + pinfo->rect.h;

    DwinInfo.u32TFDRAMAddr = pinfo->addr[0];
    DwinInfo.u32BFDRAMAddr = pinfo->addr1[0];

    DwinInfo.u8fmt = pinfo->fmt;
    //Fix me : pinfo->pitch is Number of pixels per horizontal line.
    if (pinfo->pitch == pinfo->rect.w)
    {
        DwinInfo.u16DRAMJumpLen = 0; //Pitch and width are qual
    }
    else
    {
        if (GOP_DWIN_FMT_ARGB8888 == pinfo->fmt)
        {
            DwinInfo.u16DRAMJumpLen = (pinfo->pitch - pinfo->rect.w) * 4;  // 1 pixel occupy 4 bytes
        }
        else
        {
            DwinInfo.u16DRAMJumpLen = (pinfo->pitch - pinfo->rect.w) * 2;  // 1 pixel occupy 2 bytes
        }
    }

    MDrv_GOP_DWIN_SetDataFmt(g_pGOPCtxLocal, (DRV_GOP_DWIN_DATA_FMT)pinfo->fmt);
    MDrv_GOP_DWIN_SetSourceSel(g_pGOPCtxLocal, (DRV_GOP_DWIN_SRC_SEL)pinfo->src);
    MDrv_GOP_SetClkForCapture(g_pGOPCtxLocal, (DRV_GOP_DWIN_SRC_SEL)pinfo->src);


    if (MDrv_GOP_GetMIULen(g_pGOPCtxLocal) < pinfo->addr[0])
    {
        //Check DWIN MIU Setting is correct
        if (MDrv_GOP_GetDWINMIU(g_pGOPCtxLocal) != 1)
        {
            MS_CRITICAL_MSG(printf("\n %s DWIN fb addr > MIUO length\n", __FUNCTION__));
            MDrv_GOP_SetDWINMIU(g_pGOPCtxLocal, 1);
        }
    }
    else //dwin FB address < MIU0, FB should be in MIU 0
    {
        //Check DWIN MIU Setting is correct, Error handling
        if (MDrv_GOP_GetDWINMIU(g_pGOPCtxLocal) == 1)
        {
            MS_CRITICAL_MSG(printf("\n %s, DWIN MIU setting should be MIU 0\n", __FUNCTION__));
            MDrv_GOP_SetDWINMIU(g_pGOPCtxLocal, 0);
        }
    }
    if (MDrv_GOP_GetMIULen(g_pGOPCtxLocal) < pinfo->addr1[0])
    {
        //Check DWIN MIU Setting is correct
        if (MDrv_GOP_GetDWINMIU(g_pGOPCtxLocal) != 1)
        {
            MS_CRITICAL_MSG(printf("\n %s DWIN fb addr > MIUO length\n", __FUNCTION__));
            MDrv_GOP_SetDWINMIU(g_pGOPCtxLocal, 1);
        }
    }
    else //dwin FB address < MIU0, FB should be in MIU 0
    {
        //Check DWIN MIU Setting is correct, Error handling
        if (MDrv_GOP_GetDWINMIU(g_pGOPCtxLocal) == 1)
        {
            MS_CRITICAL_MSG(printf("\n %s, DWIN MIU setting should be MIU 0\n", __FUNCTION__));
            MDrv_GOP_SetDWINMIU(g_pGOPCtxLocal, 0);
        }
    }

    if ((DwinInfo.u32TFDRAMAddr - DwinInfo.u32BFDRAMAddr) != 0)
    {
        MDrv_GOP_DWIN_SetWinInfo(g_pGOPCtxLocal, &DwinInfo);
    }
    else
    {
        if (pinfo->bEn == TRUE)
        {
            MS_CRITICAL_MSG(printf("\n %s, DWIN fb addr is invalid\n", __FUNCTION__));
        }
    }
    MDrv_GOP_DWIN_SelectSourceScanType(g_pGOPCtxLocal, (EN_GOP_DWIN_SCAN_TYPE)pinfo->bInterlace);

    if ((pinfo->framecount == 1) && (pinfo->bEn == FALSE))
        MDrv_GOP_DWIN_CaptureOneFrame(g_pGOPCtxLocal);
    else
        MDrv_GOP_DWIN_EnableCaptureStream(g_pGOPCtxLocal, pinfo->bEn);

    return GOP_API_SUCCESS;
}


int Ioctl_GOP_Dwin_Intr(void *pInstance, EN_GOP_DWIN_INTR intr_type, MS_U32 *pIntr, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (intr_type)
    {
    case E_GOP_DWIN_INTR_CONTROL:
        {
            MS_BOOL bEn;
            MS_U32 IntrMask;
            PGOP_DWIN_INTR_CTRL pIntrCtrl = (PGOP_DWIN_INTR_CTRL)pIntr;

            if (u32Size != sizeof(GOP_DWIN_INTR_CTRL))
                return GOP_API_INVALID_PARAMETERS;

            bEn = (MS_BOOL)pIntrCtrl->intrEn;
            IntrMask = pIntrCtrl->mask;

            MDrv_GOP_DWIN_EnableIntr(g_pGOPCtxLocal, IntrMask, bEn);

            break;
        }
    case E_GOP_DWIN_INTR_GETINFO:
        {
            if (u32Size != sizeof(MS_U16))
                return GOP_API_INVALID_PARAMETERS;

            *pIntr =  MDrv_GOP_DWIN_GetIntrStatus(g_pGOPCtxLocal);

            break;
        }
    case E_GOP_DWIN_INTR_GETINFO_TIMEOUT:
        {
            PGOP_DWIN_INTR_TIME_OUT pIntrTimeout = (PGOP_DWIN_INTR_TIME_OUT)pIntr;

            if (u32Size != sizeof(GOP_DWIN_INTR_TIME_OUT))
                return GOP_API_INVALID_PARAMETERS;
            GOP_DWIN_GetDWinIntInfoTimeout(pInstance, &pIntrTimeout->DWinIntInfo, pIntrTimeout->u32Timeout);
            break;
        }
    }

    return GOP_API_SUCCESS;

}



int Ioctl_GOP_SetWinInfo(void *pInstance, MS_U32 gwinId, PGOP_BUFFER_INFO pInfo)
{
    MS_U8 u8GOP;
    GOP_GwinInfo  wininfo;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    wininfo.clrType = pInfo->fbFmt;
    wininfo.u16DispHPixelStart = pInfo->disp_rect.x;
    wininfo.u16DispVPixelStart = pInfo->disp_rect.y;
    wininfo.u16RBlkHPixSize = pInfo->width;
    wininfo.u16RBlkVPixSize = pInfo->height;
    wininfo.u32DRAMRBlkStart = pInfo->addr;
    wininfo.u16RBlkHRblkSize = pInfo->pitch;
    wininfo.u16DispHPixelEnd = pInfo->disp_rect.x + pInfo->disp_rect.w;
    wininfo.u16DispVPixelEnd = pInfo->disp_rect.y + pInfo->disp_rect.h;
    wininfo.u16WinX = 0;
    wininfo.u16WinY = 0;

    DEBUG_INFO(printf("\33[0;36m   %s:%ld   u8win = %d \33[m \n", __FUNCTION__, __LINE__, gwinId));

    if (!_GOP_IsGwinIdValid(pInstance, gwinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, gwinId));
        return GOP_API_FAIL;
    }

    if (fpXCIsInterlace == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nfpXCIsInterlace is Null"));
        MS_DEBUG_MSG(printf("%s: fpXCIsInterlace is not hooked. Bypass switch...\n", __FUNCTION__));
        return GOP_API_FAIL;
    }

    u8GOP = _DumpGopByGwinId(pInstance, gwinId);

    MDrv_GOP_Sel(g_pGOPCtxLocal, u8GOP);

#if GFLIP_ENABLE
    //MDrv_GOP_GWIN_SendGwinInfo2Flip(g_pGOPCtxLocal, u8win, (DRV_GOP_GWIN_INFO*)pinfo);
#endif

    if (u8GOP == GOPTYPE.GOP0)
    {
        _SetGop0WinInfo(pInstance, gwinId, &wininfo);
    }
    else if (u8GOP == GOPTYPE.GOP1)
    {
        _SetGop1WinInfo(pInstance, gwinId, &wininfo);
    }
    else
    {
        _SetGop23WinInfo(pInstance, gwinId, &wininfo);
    }

    MDrv_GOP_GWIN_UpdateReg(g_pGOPCtxLocal, u8GOP);

    return GOP_API_SUCCESS;
}



int Ioctl_GOP_GetWinInfo(void *pInstance, MS_U32 gwinId, PGOP_BUFFER_INFO pInfo)
{
    GOP_GwinInfo wininfo;


    if (!_GOP_IsGwinIdValid(pInstance, gwinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, gwinId));
        return GOP_API_FAIL;
    }

    if (pInfo == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]pinfo is NULL\n", __FUNCTION__, __LINE__));
        return GOP_API_FAIL;
    }

    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, gwinId)) //To check the gwin is exist
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is not exist\n", __FUNCTION__, __LINE__, gwinId));
        return GOP_API_FAIL;
        //ASSERT(0);
    }

    GOP_GetWinInfo(pInstance, gwinId, &wininfo);

    pInfo->addr            = wininfo.u32DRAMRBlkStart;
    pInfo->disp_rect.x     = wininfo.u16DispHPixelStart;
    pInfo->disp_rect.y     = wininfo.u16DispVPixelStart;
    pInfo->disp_rect.w     = wininfo.u16DispHPixelEnd - wininfo.u16DispHPixelStart;
    pInfo->disp_rect.h     = wininfo.u16DispVPixelEnd - wininfo.u16DispVPixelStart;
    pInfo->width           = wininfo.u16RBlkHPixSize;
    pInfo->height          = wininfo.u16RBlkVPixSize;
    pInfo->pitch           = wininfo.u16RBlkHRblkSize;
    pInfo->fbFmt           = wininfo.clrType;

    return GOP_API_SUCCESS;

}


int Ioctl_GOP_Win_Enable(void *pInstance, MS_U32 gwinId, MS_BOOL bEn)
{
    MS_U16 regval2;
    MS_U32 u32fbId;
    GOP_WinFB_INFO *pwinFB;
    MS_U8 gwinNum = 0;
    MS_U8 i;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif


    if (!_GOP_IsGwinIdValid(pInstance, gwinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, gwinId));
        return GOP_API_FAIL;
    }

    //printf("GWINID=%d,Enable=%d\n",(MS_U8)winId,(MS_U8)bEnable);
    if (!_GOP_GWIN_IsGwinExistInClient(pInstance, gwinId))
    {
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is not exist\n", __FUNCTION__, __LINE__, gwinId));
        //__ASSERT(0);
        return GOP_API_FAIL;
    }

    //   printf(" u8win=%02bx, bEnable=%02bx\n",u8win,bEnable);
    u32fbId = g_pGOPCtxLocal->pGOPCtxShared->gwinMap[gwinId].u32CurFBId;
    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }

    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }

    if (0 == pwinFB->in_use)
    {
        msWarning(ERR_GWIN_ID_NOT_ALLOCATED);
        CRITICAL_INFO(printf("[%s][%d]GWIN %ld not allocated\n", __FUNCTION__, __LINE__, gwinId));
        return GOP_API_FAIL;
    }
    pwinFB->enable = bEn;

    for (i = 0; i < MAX_GWIN_SUPPORT; i++)
    {
        if (GOP_GWIN_IsEnable(pInstance, i) == TRUE)
        {
            gwinNum |= 1 << i;
        }
    }

    regval2 = gwinNum;

    if (bEn == 0) // Turn off GOP
    {
        MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, gwinId, FALSE);
        if ((regval2 & ~(1 << gwinId)) == 0)
        {
            if (fpXCReduceBWForOSD != NULL)
            {
                fpXCReduceBWForOSD(XC_MAIN_WINDOW, FALSE);
            }
        }
    }
    else //turn on GOP
    {
        //printf("gop on, interlace=%bx\n", IsSrcInterlace());
        if (fpXCReduceBWForOSD != NULL)
        {
            fpXCReduceBWForOSD(XC_MAIN_WINDOW, TRUE);
        }
        MDrv_GOP_GWIN_EnableGwin(g_pGOPCtxLocal, gwinId, TRUE);
    }

    return GOP_API_SUCCESS;

}


//******************************************************************************
/// API for set GWIN resolution in one function
/// @param u8GwinId \b IN: GWin ID
/// @param u8FbId \b IN: Frame Buffer ID
/// @param pGwinInfo \b IN: pointer to GOP_GwinInfo structure
/// @param pStretchInfo \b IN: pointer to GOP_StretchInfo
/// @param direction \b IN: to decide which direction to stretch
/// @param u16DstWidth \b IN: set scaled width if H direction is specified
/// @param u16DstHeight \b IN: set scaled height if V direction is specified
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
int Ioctl_GOP_SetDisplay(void *pInstance, PGOP_GWINDISPLAY_INFO pInfo)
{
    MS_U8 u8GOP;
    MS_U32 GwinId = pInfo->gwin;
    MS_U16 u16Tmp, u16OrigWidth = 0;
    MS_U16 gwin_w, stretch_w;
    MS_BOOL u16OrgUpdateRegOnce = FALSE;
    GOP_StretchInfo *pStretchInfo;
    GOP_GwinInfo *pGwinInfo = NULL;
    MS_U32 u32FbId;
    EN_GOP_STRETCH_DIRECTION  direction;
    MS_U16 u16DstWidth, u16DstHeight;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    u32FbId = pInfo->fbid;
    direction = pInfo->dir;
    u16DstWidth = pInfo->dst_size.w;
    u16DstHeight = pInfo->dst_size.h;

    pGwinInfo = &(pInfo->gwin_info);
    pStretchInfo = &(pInfo->stretch_info);

    u16OrgUpdateRegOnce = g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE];
    //Save old update_reg_once state, and change to update_reg_once=TRUE internally
    if (u16OrgUpdateRegOnce == FALSE)
    {
        g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = TRUE;
    }
    GOP_MapFB2Win(pInstance, u32FbId, GwinId);

    u8GOP = _DumpGopByGwinId(pInstance, GwinId);
    //MApi_GOP_GWIN_Set_STRETCHWIN(u8GOP, pStretchInfo->eDstType,pStretchInfo->x, pStretchInfo->y
    //    , pStretchInfo->width, pStretchInfo->height);
    _GOP_Get_StretchWin(pInstance, u8GOP, &u16Tmp, &u16Tmp, &u16OrigWidth, &u16Tmp);

    gwin_w = pGwinInfo->u16DispHPixelEnd - pGwinInfo->u16DispHPixelStart;

    if (gwin_w > pStretchInfo->width)
        stretch_w = gwin_w;
    else
        stretch_w = pStretchInfo->width;

    _GOP_GWIN_Align_StretchWin(pInstance, u8GOP, pStretchInfo->eDstType, &pStretchInfo->x, &pStretchInfo->y, &stretch_w, &pStretchInfo->height, _GOP_GetBPP(pInstance, pGwinInfo->clrType));
    //Store API use stretch window
    g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchWidth[u8GOP] = stretch_w;
    g_pGOPCtxLocal->pGOPCtxShared->u16APIStretchHeight[u8GOP] = pStretchInfo->height;

    _GOP_GWIN_Adjust_3DStretchWin(pInstance, u8GOP, &pStretchInfo->x, &pStretchInfo->y, &stretch_w, &pStretchInfo->height);
    if (u16OrigWidth <= stretch_w) //Set a large one, set htt first
        MDrv_GOP_GWIN_SetHTotal(g_pGOPCtxLocal, u8GOP, stretch_w);

    MDrv_GOP_GWIN_SetStretchWin(g_pGOPCtxLocal, u8GOP, pStretchInfo->x, pStretchInfo->y, stretch_w, pStretchInfo->height);

    if (u16DstWidth <= pStretchInfo->width)
        GOP_Set_Hscale(pInstance, u8GOP, FALSE, pStretchInfo->width, u16DstWidth);
    else if (direction & E_GOP_H_STRETCH)
        GOP_Set_Hscale(pInstance, u8GOP, TRUE, pStretchInfo->width, u16DstWidth);

    if (u16DstHeight < pStretchInfo->height)
        GOP_Set_Vscale(pInstance, u8GOP, FALSE, pStretchInfo->height, u16DstHeight);
    else if (direction & E_GOP_V_STRETCH)
        GOP_Set_Vscale(pInstance, u8GOP, TRUE, pStretchInfo->height, u16DstHeight);

    GOP_SetWinInfo(pInstance, GwinId, pGwinInfo);

    //Restore update_reg_once and trigger register writes in
    if (u16OrgUpdateRegOnce == FALSE)
    {
        g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = u16OrgUpdateRegOnce;
    }

    if (u16OrigWidth > stretch_w) //Set a small one, set htt after Gwin regsiter updated
        MDrv_GOP_GWIN_SetHTotal(g_pGOPCtxLocal, u8GOP, stretch_w);

    return GOP_API_SUCCESS;
}



int Ioctl_GOP_Win_Destroy(void *pInstance, MS_U32 gId)
{
    GOP_Win_Destroy(pInstance, gId);
    return GOP_API_SUCCESS;

}


int Ioctl_GFlip_ClearFlipQueue(void *pInstance, MS_U32 gwin)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    return (E_GOP_API_Result)MDrv_GOP_GWIN_ClearFlipQueue(g_pGOPCtxLocal, gwin);

}



int Ioctl_GFlip_SwitchGwin(void *pInstance, PGOP_GWIN_FLIP_WIN_INFO pflip)
{


    switch (pflip->enFlip)
    {
    case GOP_FLIP_BY_FBID:
        GOP_SwitchByFBID(pInstance, pflip->GwinId, pflip->FbId, pflip->WaitTagID, pflip->pQueueCnt);
        break;
    case GOP_FLIP_BY_ADDR:
        GOP_SwitchByAddr(pInstance, pflip->GwinId, pflip->FlipAddr, pflip->SubFlipAddr, pflip->WaitTagID, pflip->pQueueCnt, pflip->b3DEnable);
        break;

    }
    return GOP_API_SUCCESS;

}

int Ioctl_GFlip_SwitchMultiGwin(void *pInstance, PGOP_GWIN_FLIP_MULTI_WIN_INFO pMultiflip)
{


    GOP_MultiSwitchByAddr(pInstance, pMultiflip);
    return GOP_API_SUCCESS;

}


int Ioctl_GOP_FBCreate(void *pInstance, EN_GOP_CREATEBUFFER_TYPE fbtype, PGOP_BUFFER_INFO  pBuff, MS_U32 fbId)
{


    if (fbtype == GOP_CREATE_BUFFER)
        GOP_CreateBuffer(pInstance, pBuff, fbId);
    else
        GOP_CreateBufferByAddr(pInstance, pBuff, fbId);

    return GOP_API_SUCCESS;

}


int Ioctl_GOP_FBDestroy(void *pInstance, MS_U32 *pId)
{
    GOP_WinFB_INFO *pwinFB;
    MS_U32 u32fbId;

    u32fbId = *pId;

    if (DRV_MAX_GWIN_FB_SUPPORT <= u32fbId)
    {
        msWarning(ERR_FB_ID_OUT_OF_RANGE);
        MS_DEBUG_MSG(printf("%s(u32fbId:%ld....), fbId out of bound\n", __FUNCTION__, u32fbId));
        return GWIN_NO_AVAILABLE;

    }
    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        DEBUG_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, u32fbId));
        return GWIN_NO_AVAILABLE;
    }

    if (0 == pwinFB->in_use)
    {
        msWarning(ERR_FB_ID_NOT_ALLOCATED);
        MS_DEBUG_MSG(printf("MApi_GOP_GWIN_Destroy32FB: u32fbId=%ld is not in existence\n", u32fbId));
        return GWIN_NO_AVAILABLE;

    }

    GOP_FB_Destroy(pInstance, u32fbId);

    return GWIN_OK;

}

int Ioctl_GOP_GetFBInfo(void *pInstance, MS_U32 u32fbId, PGOP_BUFFER_INFO pBuff)
{
    GOP_WinFB_INFO *pwinFB;


    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }

    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }

    if (pwinFB->in_use == 0)
    {
        msWarning(ERR_FB_ID_NOT_ALLOCATED);
        CRITICAL_INFO(printf("MDrv_GOP_GWIN_ResizeFB: u32fbId=%ld is not in existence\n", u32fbId));
        return GOP_API_FAIL;
    }

    pBuff->addr = pwinFB->addr;
    pBuff->fbFmt = pwinFB->fbFmt;
    pBuff->width = pwinFB->width;
    pBuff->height = pwinFB->height;
    pBuff->pitch = pwinFB->pitch;
    pBuff->disp_rect.x = pwinFB->x0;
    pBuff->disp_rect.w = pwinFB->x1 - pwinFB->x0;
    pBuff->disp_rect.y = pwinFB->y0;
    pBuff->disp_rect.h = pwinFB->y1 - pwinFB->y0;

    return GOP_API_SUCCESS;

}

int Ioctl_GOP_SetFBInfo(void *pInstance, MS_U32 u32fbId, PGOP_BUFFER_INFO pBuff)
{
    GOP_WinFB_INFO *pwinFB;


    if (!_GOP_IsFbIdValid(pInstance, u32fbId))
    {
        CRITICAL_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }
    pwinFB = _GetWinFB(pInstance, u32fbId);

    if (pwinFB == NULL)
    {
        CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, u32fbId));
        return GOP_API_FAIL;
    }

    pwinFB->enable      = 1;
    pwinFB->gWinId      = 0xFF;
    pwinFB->fbFmt       = pBuff->fbFmt;
    pwinFB->width       = pBuff->width;
    pwinFB->height      = pBuff->height;

    pwinFB->x0          = pBuff->disp_rect.x;
    pwinFB->x1          = pBuff->disp_rect.x + pBuff->disp_rect.w;
    pwinFB->y0          = pBuff->disp_rect.y;
    pwinFB->y1          = pBuff->disp_rect.y + pBuff->disp_rect.h;
    pwinFB->s_x         = 0;
    pwinFB->s_y         = 0;
    pwinFB->dispWidth   = pBuff->disp_rect.w;
    pwinFB->dispHeight  = pBuff->disp_rect.h;

    if (pwinFB->in_use &&  GOP_WINFB_POOL_NULL == pwinFB->poolId)
    {
        pwinFB->addr        = pBuff->addr;
        pwinFB->size        = pBuff->pitch * pBuff->height;
        pwinFB->pitch       = pBuff->pitch;
    }

    return GOP_API_SUCCESS;
}


int GOP_TriggerRegWriteIn(void *pInstance, MS_U32 gop, MS_BOOL bForceWriteIn, MS_BOOL bSync)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    MDrv_GOP_TriggerRegWriteIn(g_pGOPCtxLocal, gop, bForceWriteIn, bSync);
    return GOP_API_SUCCESS;


}
int GOP_UpdateOnce(void *pInstance, MS_U32 gop, MS_BOOL bUpdateOnce, MS_BOOL bSync)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    g_pGOPCtxLocal->bUpdateRegOnce[GOP_PUBLIC_UPDATE] = bUpdateOnce;
    if (bUpdateOnce == FALSE)
    {
        MDrv_GOP_GWIN_UpdateRegWithSync(g_pGOPCtxLocal, gop, bSync);
    }
    return GOP_API_SUCCESS;

}

int GOP_UpdateCurrentOnce(void *pInstance, MS_U16 u16GopMask, MS_BOOL bUpdateOnce, MS_BOOL bSync)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    MS_U16 u16GopIdx = 0;
    for (u16GopIdx = 0; u16GopIdx < MAX_GOP_SUPPORT; u16GopIdx++)
    {
        if (u16GopMask | (1 << u16GopIdx))
        {
            g_pGOPCtxLocal->bUpdateRegOnce[u16GopIdx] = bUpdateOnce;
        }
    }
    if (bUpdateOnce == FALSE)
    {
        MDrv_GOP_GWIN_UpdateRegWithMaskSync(g_pGOPCtxLocal, u16GopMask, bSync);
    }
    return GOP_API_SUCCESS;

}

int Ioctl_GOP_TriggerRegWriteIn(void *pInstance, MS_U32 gop, EN_GOP_UPDATE_TYPE update_type, MS_BOOL bEn, MS_BOOL bSync)
{

#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (update_type)
    {
    case E_GOP_UPDATE_FORCEWRITE:
        {
            GOP_TriggerRegWriteIn(pInstance, gop, bEn, bSync);
            break;
        }
    case E_GOP_UPDATE_CURRENT_ONCE:
        {
            GOP_UpdateCurrentOnce(pInstance, gop, bEn, bSync);
            break;
        }
    default:
    case E_GOP_UPDATE_ONCE:
        {
            GOP_UpdateOnce(pInstance, gop, bEn, bSync);
            break;
        }

    }

    return GOP_API_SUCCESS;

}


int Ioctl_GOP_Select(void *pInstance, EN_GOP_SELECT_TYPE sel, MS_U32 id, MS_U32 *pSet)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    DRV_GOP_CBFmtInfo *FmtInfo = (DRV_GOP_CBFmtInfo *)pSet;

    switch (sel)
    {
    case EN_GOP_SEL_GOP:
        {
            if (!_GOP_IsGopNumValid(pInstance, id))
            {
                CRITICAL_INFO(printf("[%s][%d]GOP %ld  is out of range\n", __FUNCTION__, __LINE__, id));
                return GOP_API_FAIL;
            }
            MDrv_GOP_Sel(g_pGOPCtxLocal, id);
            break;
        }
    case EN_GOP_SEL_GWIN:
        break;
    case EN_GOP_SEL_FB:
        {
            MS_U16 fbFmt;
            GOP_WinFB_INFO *pwinFB;

            // printf("[%s] We DO NOT suggest to use this function\n",__FUNCTION__);
            // printf("[%s] Please use GFX MApi_GFX_SetDstBufferInfo() !!\n",__FUNCTION__);

            if (!_GOP_IsFbIdValid(pInstance, id))
            {
                CRITICAL_INFO(printf("[%s][%d]FbId %ld  is out of range\n", __FUNCTION__, __LINE__, id));
                return GOP_API_FAIL;
            }
            pwinFB = _GetWinFB(pInstance, id);

            if (pwinFB == NULL)
            {
                CRITICAL_INFO(printf("[%s][%d]GetWinFB Fail : WrongFBID=%ld\n", __FUNCTION__, __LINE__, id));
                return GOP_API_FAIL;
            }

            if (0 == pwinFB->in_use)
            {
                msWarning(ERR_FB_ID_NOT_ALLOCATED);
                CRITICAL_INFO(printf("MApi_GOP_GWIN_Switch2_32FB: u32fbId=%ld is not in existence\n", id));
                return GOP_API_FAIL;
            }

            fbFmt = pwinFB->fbFmt;
            if ((pwinFB->fbFmt & 0xFF) == E_MS_FMT_ARGB1555)
                fbFmt = E_MS_FMT_ARGB1555_DST | (E_MS_FMT_ARGB1555_DST << 8);

            if (fpSetFBFmt != NULL)
            {
                fpSetFBFmt(pwinFB->pitch, pwinFB->addr, fbFmt);
                FmtInfo->u32Addr = pwinFB->addr;
                FmtInfo->u16Pitch = pwinFB->pitch;
                FmtInfo->u16Fmt = fbFmt;
            }
            else
            {
                printf("%s: SetFBFmt is not hooked. Bypass switch...\n", __FUNCTION__);
            }
            break;
        }
    }

    return GOP_API_SUCCESS;

}

int Ioctl_GOP_Register_CB(void *pInstance, GOP_CB_TYPE cb_type, void *pCB, MS_U32 u32Size)
{
    if (pCB == NULL)
    {
        APIGOP_ASSERT(FALSE, printf("\nregister CB function is Null"));
        return GOP_API_FAIL;
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    switch (cb_type)
    {
    case GOP_CB_XC_INTERLACE:
        {
            MS_BOOL *bInterlace;
            bInterlace = (MS_BOOL *)pCB;
            bXCInterlace = *bInterlace;
            break;
        }
    case GOP_CB_XC_PANEL_HSTART:
        {
            MS_U32 *u32HStart;
            u32HStart = (MS_U32 *)pCB;
            u32XCHStart = *u32HStart;
            break;
        }
    }
#else
    switch (cb_type)
    {
    case GOP_CB_XC_INTERLACE:
        {
            fpXCIsInterlace = pCB;
            break;
        }
    case GOP_CB_XC_REDUCE_BW:
        {
            fpXCReduceBWForOSD = pCB;
            break;
        }
    case GOP_CB_XC_PANEL_HSTART:
        {
            fpXCGetCapHStart = pCB;
            break;
        }
    case GOP_CB_GFX_FBFMT:
        {
            fpSetFBFmt = pCB;
            break;
        }
    case GOP_CB_EVENT_NOTIFY:
        {
            fpEventNotify = pCB;
            break;
        }
    case GOP_CB_DWIN_INFO:
        {
            fpXCSetDwinInfo = pCB;
            break;
        }
    default:
        {
            printf("[%s] invalid input call back function type\n", __FUNCTION__);
            return GOP_API_INVALID_PARAMETERS;
        }
    }
#endif
    return (GOP_API_SUCCESS);
}





int Ioctl_GOP_Palette_EntrySet(void *pInstance, PGOP_PALETTE_ENTRY pClutEntry)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (pClutEntry->palSrc == E_GOP_4G_PALETTE)
    {
        MDrv_GOP_GWIN_SetPaletteOpt(g_pGOPCtxLocal,pClutEntry->gop_idx, (DRV_GopPaletteEntry*)pClutEntry->pClut, pClutEntry->start, pClutEntry->end, E_DRV_GOP_PAL_ARGB8888);
    }
    else
    {
        MDrv_GOP_GWIN_2GSetPaletteOpt(g_pGOPCtxLocal,pClutEntry->gop_idx, (DRV_GopPaletteEntry*)pClutEntry->pClut, pClutEntry->start, pClutEntry->end);
    }
    return GOP_API_SUCCESS;

}

int Ioctl_GOP_Palette_EntryGet(void *pInstance, PGOP_PALETTE_ENTRY pClutEntry)
{
    int i = 0;
    MS_U32 PalVal = 0;
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (pClutEntry->palSrc == E_GOP_4G_PALETTE)
    {
        for (i = pClutEntry->start; i < pClutEntry->end; i++)
        {
            MDrv_GOP_GWIN_ReadPalette(g_pGOPCtxLocal,pClutEntry->gop_idx, i, &PalVal);
            *pClutEntry->pClut = PalVal;
        }
    }
    else
    {
        for (i = pClutEntry->start; i < pClutEntry->end; i++)
        {
            MDrv_GOP_GWIN_2GReadPalette(g_pGOPCtxLocal,pClutEntry->gop_idx, i, &PalVal);
        }
    }
    return GOP_API_SUCCESS;

}


int Ioctl_GOP_Palette_Set_Config(void *pInstance, MS_U32 gop, DRV_GopPalReadType type, EN_GOP_PALETTE src)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    if (src == E_GOP_4G_PALETTE)
    {
        MDrv_GOP_GWIN_SetPaletteRead(g_pGOPCtxLocal,gop, type);
    }
    else
    {
        MDrv_GOP_GWIN_2GSetPaletteRead(g_pGOPCtxLocal,gop, type);
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_MISC(void *pInstance, EN_GOP_MISC_TYPE type, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (type)
    {
    case E_GOP_MISC_ENABLE_T3D:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            bEn = *((MS_BOOL*)pSet);
            GOP_GWIN_EnableT3DMode(pInstance, bEn);
            break;
        }
    case E_GOP_MISC_SET_CAPTURE_CLK:
        {
            if (u32Size != sizeof(int))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            MDrv_GOP_SetClkForCapture(g_pGOPCtxLocal, GOP_DRV_DWIN_SRC_OP);
            break;
        }
    case E_GOP_MISC_POWER_OFF:
        {
            if (u32Size != sizeof(int))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            GOP_PowerOff(pInstance);
            break;
        }
    case E_GOP_MISC_POWER_ON:
        {
            if (u32Size != sizeof(int))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            GOP_PowerOn(pInstance);
            break;
        }
    case E_GOP_MISC_GET_RT_STATUS:
        {
            if (u32Size != sizeof(GOP_ApiStatus))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            GOP_ApiStatus *pGopApiSts = (GOP_ApiStatus *)pSet;
            GOP_GetRTStatus(pInstance, pGopApiSts);
            break;
        }
    case E_GOP_MISC_GET_INFO:
        {
            if (u32Size != sizeof(GOP_ApiInfo))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            GOP_ApiInfo *pGopApiInfo = (GOP_ApiInfo *)pSet;
            GOP_GetInfo(pInstance, pGopApiInfo);
            break;
        }
    case E_GOP_MISC_AT_EXIT:
        {
            if (u32Size != sizeof(int))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            Ioctl_GOP_AtExit(pInstance);
            break;
        }
    case E_GOP_MISC_OC_FB_INFO:
        {
            if (u32Size != sizeof(GOP_OC_FB_INFO))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            PGOP_OC_FB_INFO pOCFbInfo = (PGOP_OC_FB_INFO)pSet;
            GOP_OC_SetOCFBinfo(pInstance, pOCFbInfo->u32Addr, pOCFbInfo->BankMode, pOCFbInfo->enBufSize);
            break;
        }
    case E_GOP_MISC_TESTPATTERN_ALPHA:
        {
            MS_BOOL bEn;
            if (u32Size != sizeof(MS_BOOL))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            bEn = *((MS_BOOL*)pSet);
            GOP_TestPatternAlpha_Enable(pInstance, bEn);
            break;
        }
    case E_GOP_MISC_DELETE_GWINSIZE:
        {
            if (u32Size != sizeof(GOP_DeleteWinSize_PARAM))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            GOP_DeleteWinSize_PARAM *gop_VaildSize = (GOP_DeleteWinSize_PARAM *)pSet;
            MDrv_GOP_GWIN_DeleteWinHVSize(g_pGOPCtxLocal, gop_VaildSize->u32GOP_idx, gop_VaildSize->u32HSize, gop_VaildSize->u32VSize);
            break;
        }
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_PINPON(void *pInstance, EN_GOP_PINPON_PROPERTY en_pro, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    switch (en_pro)
    {
    case E_GOP_SET_PINPON:
        {
            if (u32Size != sizeof(GOP_SET_PINPON_INFO))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            PGOP_SET_PINPON_INFO pPINPONInfo = (PGOP_SET_PINPON_INFO)pSet;

            MS_U8 u8GOP;
            MS_U8 u8FirstGwinByGOP;

            u8GOP = _DumpGopByGwinId(pInstance, pPINPONInfo->GwinId);
            u8FirstGwinByGOP = MDrv_GOP_SelGwinIdByGOP(g_pGOPCtxLocal, u8GOP, 0);

            //check it is the first gwin on this GOP
            if (pPINPONInfo->GwinId != u8FirstGwinByGOP)
            {
                printf("gwin:%d PINPON mode only support the first gwin on GOP:%d\n", pPINPONInfo->GwinId, u8GOP);
                return GOP_API_FAIL;
            }

            MDrv_GOP_Set_PINPON(g_pGOPCtxLocal, u8GOP, pPINPONInfo->bEnable, pPINPONInfo->mode);
            break;
        }
    }
    return GOP_API_SUCCESS;
}

int Ioctl_GOP_Fade(void *pInstance, EN_GOP_FADE_SET_TYPE type, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif
    E_GOP_API_Result ret = GOP_API_FAIL;
    switch (type)
    {
    case E_GOP_FADE_CONFIG:
        {
            if (u32Size != sizeof(GOP_FADE_INFO))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            PGOP_FADE_INFO pFadeInfo = (PGOP_FADE_INFO)pSet;
            if (!_GOP_IsGwinIdValid(pInstance, pFadeInfo->gwin_idx))
            {
                CRITICAL_INFO(printf("[%s][%d]GWIN %ld  is out of range\n", __FUNCTION__, __LINE__, pFadeInfo->gwin_idx));
                return GOP_API_FAIL;
            }
            if ((pFadeInfo->eGwinFadeType != E_GOP_GWIN_FADE_TYPE_IN) && (pFadeInfo->eGwinFadeType != E_GOP_GWIN_FADE_TYPE_OUT))
            {
                APIGOP_ASSERT(FALSE, printf("\n[%s] unknow gop fade type", __FUNCTION__));
                return GOP_API_FAIL;
            }

            ret = (E_GOP_API_Result)MDrv_GOP_GWIN_SetFadeInOut(g_pGOPCtxLocal, pFadeInfo->gwin_idx, (DRV_GOP_FADE_TYPE)pFadeInfo->eGwinFadeType, pFadeInfo->bEnable, pFadeInfo->u8FadeRate);
            break;
        }
    }
    return ret;
}


int Ioctl_GOP_Scroll(void *pInstance, EN_GOP_SCROLL_TYPE type, MS_U32 *pSet, MS_U32 u32Size)
{
#ifdef INSTANT_PRIVATE
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void *)&psGOPInstPri);
#endif

    switch (type)
    {
    case E_GOP_SCROLL_CONFIG:
        {
            if (u32Size != sizeof(GOP_SCROLL_INFO))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }
            PGOP_SCROLL_INFO pScrollInfo = (PGOP_SCROLL_INFO)pSet;

            //printf("%s,%d,pScrollInfo->bEnable = %d,pScrollInfo->type = %d,pScrollInfo->u16Offset = %d,pScrollInfo->u8WinId = %d \n",__FUNCTION__,__LINE__,pScrollInfo->bEnable,pScrollInfo->type,pScrollInfo->u16Offset,pScrollInfo->u8WinId);
            if (pScrollInfo->type == E_GOP_SCROLL_DOWN || pScrollInfo->type == E_GOP_SCROLL_UP)
            {
                MDrv_GOP_GWIN_SetVScroll(g_pGOPCtxLocal, pScrollInfo->u8WinId,  (E_GOP_SCROLL_TYPE)pScrollInfo->type,  (MS_U32)pScrollInfo->u16Offset, pScrollInfo->bEnable);
            }
            else
            {
                MDrv_GOP_GWIN_SetHScroll(g_pGOPCtxLocal, pScrollInfo->u8WinId,  (E_GOP_SCROLL_TYPE)pScrollInfo->type,  (MS_U32)pScrollInfo->u16Offset, pScrollInfo->bEnable);
            }
            break;
        }
    case E_GOP_SCROLL_RATE:
        {
            MS_U8 u8rate;
            if (u32Size != sizeof(MS_U8))
            {
                CRITICAL_INFO(printf("[%s][%d] size ERROR!! \n", __FUNCTION__, __LINE__));
                return GOP_API_FAIL;
            }

            u8rate = (MS_U8)*pSet;
            MDrv_GOP_GWIN_SetScrollRate(g_pGOPCtxLocal, u8rate);
            break;
        }
    }
    return GOP_API_SUCCESS;
}
void Ioctl_GOP_AtExit(void *pInstance)
{
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
    GOP_AtExit(pInstance);
#else
    printf("not enable MSOS_TYPE_LINUX\n");
#endif
}

#ifdef MSOS_TYPE_LINUX_KERNEL



MS_U32 GOPStr(MS_U32 u32PowerState, void *pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

#if(GFLIP_ENABLE==FALSE)
    GFLIP_REGS_SAVE_AREA* pGOP_STRPrivate;
    UtopiaModuleGetPtr(MODULE_GOP, &pModule);
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pGOP_STRPrivate);


    switch (u32PowerState)
    {
    case E_POWER_SUSPEND:
        printf("[%s][%d] Suspend Start!!!!!\n", __FUNCTION__, __LINE__);
        if (pGOP_STRPrivate->u16PowerOnMode == E_GOP_POWERON_TYPE_NONE
            || pGOP_STRPrivate->u16PowerOnMode == E_GOP_POWERON_TYPE_COLD_BOOT)
        {
            pGOP_STRPrivate->u16PowerOnMode = E_GOP_POWERON_TYPE_STD_SUSPEND;
        }
        MDrv_GOP_GWIN_PowerState(NULL, u32PowerState, pModule);
        printf("[%s][%d] Suspend End!!!!!\n", __FUNCTION__, __LINE__);
        u32Return = UTOPIA_STATUS_SUCCESS;
        break;
    case E_POWER_RESUME:
        printf("[%s][%d] Resume Start!!!!!\n", __FUNCTION__, __LINE__);
        if (pGOP_STRPrivate->u16PowerOnMode == E_GOP_POWERON_TYPE_NONE)
        {
            pGOP_STRPrivate->u16PowerOnMode = E_GOP_POWERON_TYPE_STD_RESUME;
        }
        MDrv_GOP_GWIN_PowerState(NULL, u32PowerState, pModule);
        printf("[%s][%d] Resume End!!!!!\n", __FUNCTION__, __LINE__);
        u32Return = UTOPIA_STATUS_SUCCESS;
        break;
    default:
        printf("[%s][%d] PowerState:%lx not Implement now!!!\n", __FUNCTION__, __LINE__, u32PowerState);
        break;
    }
#else
//Kernel RedLine:GFLIP  Suspend/Resume
    u32Return = UTOPIA_STATUS_SUCCESS;
#endif
    return u32Return;
}

extern int suspend_gop(void)
{
    void *pModule = NULL;
    GFLIP_REGS_SAVE_AREA* pGOP_STRPrivate;
    UtopiaModuleGetPtr(MODULE_GOP, &pModule);
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pGOP_STRPrivate);
    pGOP_STRPrivate->u16PowerOnMode = E_GOP_POWERON_TYPE_STR_SUSPEND;
    GOPStr(E_POWER_SUSPEND, pModule);
    return 0;
}

extern int resume_gop(void)
{
    void *pModule = NULL;
    GFLIP_REGS_SAVE_AREA* pGOP_STRPrivate;
    UtopiaModuleGetPtr(MODULE_GOP, &pModule);
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pGOP_STRPrivate);
    pGOP_STRPrivate->u16PowerOnMode = E_GOP_POWERON_TYPE_STR_RESUME;
    GOPStr(E_POWER_RESUME, pModule);
    return 0;
}

#endif

#endif //UTOPIA_STRIP
// this func will be call to init by utopia20 framework
void GOPRegisterToUtopia(void)
{
    // 1. deal with module
    void *pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_GOP, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)GOPOpen, (FUtopiaClose)GOPClose, (FUtopiaIOctl)GOPIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule, (FUtopiaSTR)GOPStr);
    UtopiaModuleSetSTRPrivate(pUtopiaModule, sizeof(GFLIP_REGS_SAVE_AREA));
#endif

    // 2. deal with resource
    void *psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
    UtopiaModuleAddResourceStart(pUtopiaModule, GOP_POOL_ID_GOP0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("gop0", sizeof(GOP_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, GOP_POOL_ID_GOP0);

    UtopiaModuleAddResourceEnd(pUtopiaModule, GOP_POOL_ID_GOP0);


}

MS_U32 GOPOpen(void **ppInstance, MS_U32 u32ModuleVersion, void *pAttribute)
{
    GOP_INSTANT_PRIVATE *pGopPri = NULL;
    printf("\n[GOP INFO] gop open\n");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(GOP_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void **)&pGopPri);
    return UTOPIA_STATUS_SUCCESS;
}
#if defined(MSOS_TYPE_LINUX) /*|| defined(MSOS_TYPE_LINUX_KERNEL)*/
#include <linux/types.h>
#endif
MS_U32 GOPIoctl(void *pInstance, MS_U32 u32Cmd, void *pArgs)
{
#ifndef UTOPIA_STRIP
    void *pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    MS_U32 u32Ret = GOP_API_SUCCESS;
    GOP_INSTANT_PRIVATE *psGOPInstPri = NULL;
    MS_BOOL bLocked = FALSE;
    UtopiaInstanceGetPrivate(pInstance, (void **)&psGOPInstPri);

    if (u32Cmd == MAPI_CMD_GOP_RESETPOOL)
    {
        GOP_CTX_DRV_SHARED *pDrvGOPShared = NULL;
        MS_BOOL bNeedInitShared = FALSE;
#if defined(MSOS_TYPE_LINUX) /* || defined(MSOS_TYPE_LINUX_KERNEL)*/
        pDrvGOPShared = (GOP_CTX_DRV_SHARED *)(void *)MDrv_GOP_GetShareMemory(&bNeedInitShared);
        if (pDrvGOPShared == NULL)
        {
            printf("[%s] [%d] Shm pointer is NULL!!!! \n", __FUNCTION__, __LINE__);
            return UTOPIA_STATUS_FAIL;
        }
        if ((pDrvGOPShared->apiCtxShared.GOPLockPid != 0))
        {
            if (kill(pDrvGOPShared->apiCtxShared.GOPLockPid, 0) == ESRCH)
            {
                //printf("[%s] [%d] apiCtxShared.GOPLockPid 0x%tx not existance!!! \n", __FUNCTION__, __LINE__,  (ptrdiff_t)pDrvGOPShared->apiCtxShared.GOPLockPid);
                UtopiaModuleResetPool(pModule, GOP_POOL_ID_GOP0);
            }
        }
#endif
        return UTOPIA_STATUS_SUCCESS;
    }

    if ((u32Cmd != MAPI_CMD_GOP_MUTEX) && (g_pGOPCtxLocal != NULL) && (g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid != MsOS_GetOSThreadID()))
    {
        if (UtopiaResourceObtain(pModule, GOP_POOL_ID_GOP0, &(psGOPInstPri->pResource)) != 0)
        {
            printf("[%s %d]UtopiaResourceObtainToInstant fail\n", __FUNCTION__, __LINE__);
            return 0xFFFFFFFF;
        }
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
        g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = getpid();
#endif
        bLocked = TRUE;
    }
    //printf("[%s] cmd:%lx\n",__FUNCTION__,u32Cmd);
    if (g_pGOPCtxLocal == NULL)
    {
      switch (u32Cmd)
      {
        case MAPI_CMD_GOP_INIT:
            break;
        case MAPI_CMD_GOP_SET_CONFIG:
            {
                PGOP_SETCONFIG_PARAM ptrCfg = (PGOP_SETCONFIG_PARAM)pArgs;
                if (ptrCfg->cfg_type != E_GOP_IGNOREINIT )
                {
                    printf("[%s %d]g_pGOPCtxLocal is NULL, u32Cmd = 0x%lX , ptrCfg->cfg_type = 0x%x\n", __FUNCTION__, __LINE__, u32Cmd,ptrCfg->cfg_type);
                    return UTOPIA_STATUS_FAIL;
                }
            }
            break;
        default:
            {
                printf("[%s %d]g_pGOPCtxLocal is NULL, u32Cmd = 0x%lX \n", __FUNCTION__, __LINE__,u32Cmd);
                return UTOPIA_STATUS_FAIL;
            }
            break;
        }
    }
    switch (u32Cmd)
    {
    case MAPI_CMD_GOP_INIT:
        {
            PGOP_INIT_PARAM ptr = (PGOP_INIT_PARAM)pArgs;

            if (ptr->u32Size != sizeof(GOP_InitInfo))
            {
                printf("[%s] input structure size non match!! (input:%lx require:%x\n", __FUNCTION__, ptr->u32Size, sizeof(GOP_INIT_PARAM));
                if (g_pGOPCtxLocal != NULL)
                {
                    g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
    #if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                    g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
    #endif
                }
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_Init(pInstance, ptr->gop_idx, (GOP_InitInfo *)ptr->pInfo);

            break;
        }
    case MAPI_CMD_GOP_GET_CHIPCAPS:
        {
            PGOP_GETCAPS_PARAM ptr = (PGOP_GETCAPS_PARAM)pArgs;

            u32Ret = Ioctl_GOP_GetCaps(pInstance, (EN_GOP_CAPS)ptr->caps, ptr->pInfo, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_SET_CONFIG:
        {
            PGOP_SETCONFIG_PARAM ptr = (PGOP_SETCONFIG_PARAM)pArgs;
            u32Ret = Ioctl_GOP_SetConfig(pInstance, ptr->cfg_type, ptr->pCfg, ptr->u32Size);
            break;

        }
    case MAPI_CMD_GOP_GET_CONFIG:
        {
            PGOP_SETCONFIG_PARAM ptr = (PGOP_SETCONFIG_PARAM)pArgs;
            u32Ret = Ioctl_GOP_GetConfig(pInstance, ptr->cfg_type, ptr->pCfg, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_SET_PROPERTY:
        {
            PGOP_SET_PROPERTY_PARAM ptr = (PGOP_SET_PROPERTY_PARAM)pArgs;


            u32Ret = Ioctl_GOP_SetProperty(pInstance, ptr->en_pro, ptr->gop_idx, ptr->pSetting, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_GET_PROPERTY:
        {
            PGOP_SET_PROPERTY_PARAM ptr = (PGOP_SET_PROPERTY_PARAM)pArgs;


            u32Ret = Ioctl_GOP_GetProperty(pInstance, ptr->en_pro, ptr->gop_idx, ptr->pSetting, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_SET_DST:
        {
            PGOP_SETDST_PARAM  ptr = (PGOP_SETDST_PARAM)pArgs;

            u32Ret = Ioctl_GOP_SetDst(pInstance, ptr->gop_idx, ptr->en_dst);
            break;
        }
    case MAPI_CMD_GOP_GET_DST:
        {
            PGOP_GETDST_PARAM  ptr = (PGOP_GETDST_PARAM)pArgs;
            if (ptr->u32Size != sizeof(EN_GOP_DST_TYPE))
            {
                printf("[%s] Cmd:%ld input structure size non match!! (input:%lx require:%x\n", __FUNCTION__, u32Cmd, ptr->u32Size, sizeof(EN_GOP_DST_TYPE));
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_GetDst(pInstance, ptr->gop_idx, ptr->pDst);
            break;
        }
    case MAPI_CMD_GOP_SET_MUX:
        {
            PGOP_SETMUX_PARAM  ptr = (PGOP_SETMUX_PARAM)pArgs;
            //PGOP_MuxConfig  pMux = (PGOP_MuxConfig)ptr->pMuxInfo;
            PGOP_SETMUX  pMux = (PGOP_SETMUX)ptr->pMuxInfo;

            if (ptr->u32Size != sizeof(GOP_SETMUX))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            u32Ret = Ioctl_GOP_SetMux(pInstance, pMux, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_SET_LAYER:
        {
            PGOP_SETLAYER_PARAM ptr = (PGOP_SETLAYER_PARAM)pArgs;
            //PGOP_MuxConfig  pMux = (PGOP_MuxConfig)ptr->pMuxInfo;
            PGOP_SETLayer pLayer = (PGOP_SETLayer)ptr->pLayerInfo;
            MS_U16 i = 0;
            GOP_SETMUX stMuxInfo;
            if (ptr->u32Size != sizeof(GOP_SETLayer))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            memset(&stMuxInfo, 0, sizeof(GOP_SETMUX));
            for (i = 0; i < pLayer->u32LayerCount; i++)
            {
                stMuxInfo.gop[i] = pLayer->u32Gop[i];
                if (g_pGOPCtxLocal != NULL)
                {
                    MDrv_GOP_MapLayer2Mux(g_pGOPCtxLocal, pLayer->u32Layer[i], &(stMuxInfo.mux[i]));
                }
            }
            stMuxInfo.MuxCount = pLayer->u32LayerCount;
            u32Ret = Ioctl_GOP_SetMux(pInstance, &stMuxInfo, sizeof(GOP_SETMUX));
            break;
        }

    case MAPI_CMD_GOP_SET_MIRROR:
        {
            PGOP_SETMIRROR_PARAM ptr = (PGOP_SETMIRROR_PARAM)pArgs;
            u32Ret = Ioctl_GOP_SetMirror(pInstance, ptr->gop_idx, ptr->dir);

            break;
        }
    case MAPI_CMD_GOP_GET_MIRROR:
        {
            PGOP_GETMIRROR_PARAM ptr = (PGOP_GETMIRROR_PARAM)pArgs;
            u32Ret = Ioctl_GOP_GetMirror(pInstance, ptr->gop_idx, ptr->pdir);

            break;
        }
    case MAPI_CMD_GOP_GET_STATUS:
        {
            PGOP_GET_STATUS_PARAM ptr = (PGOP_GET_STATUS_PARAM)pArgs;

            u32Ret = Ioctl_GOP_GetStatus(pInstance, ptr->en_status, ptr->pStatus, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_TESTPATTERN:
        {
            PGOP_TEST_PATTERN_PARAM ptr = (PGOP_TEST_PATTERN_PARAM)pArgs;

            u32Ret = Ioctl_GOP_TestPattern(pInstance, ptr->TestPatternType, ptr->u32ARGB);
            break;
        }
        //Stretch Win
    case MAPI_CMD_GOP_GWIN_SET_STRETCH:
        {
            PGOP_STRETCH_SET_PARAM ptr = (PGOP_STRETCH_SET_PARAM)pArgs;
            PGOP_STRETCH_INFO pInfo = (PGOP_STRETCH_INFO)ptr->pStretch;
            if (ptr->u32Size != sizeof(GOP_STRETCH_INFO))
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                return GOP_API_INVALID_PARAMETERS;
            }

            u32Ret = Ioctl_GOP_Set_Stretch(pInstance, ptr->enStrtchType, ptr->gop_idx, pInfo);
            break;
        }
    case MAPI_CMD_GOP_GWIN_GET_STRETCH:
        {
            PGOP_STRETCH_SET_PARAM ptr = (PGOP_STRETCH_SET_PARAM)pArgs;
            PGOP_STRETCH_INFO pInfo = (PGOP_STRETCH_INFO)ptr->pStretch;
            if (ptr->u32Size != sizeof(GOP_STRETCH_INFO))
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                return GOP_API_INVALID_PARAMETERS;
            }

            u32Ret = Ioctl_GOP_Get_Stretch(pInstance, ptr->enStrtchType, ptr->gop_idx, pInfo);
            break;
        }

        //GWIN info
    case MAPI_CMD_GOP_GWIN_SET_PROPERTY:
        {
            PGOP_GWIN_PROPERTY_PARAM ptr = (PGOP_GWIN_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_GWin_SetProperty(pInstance, ptr->en_property, ptr->GwinId, ptr->pSet, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_GWIN_GET_PROPERTY:
        {
            PGOP_GWIN_PROPERTY_PARAM ptr = (PGOP_GWIN_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_GWin_GetProperty(pInstance, ptr->en_property, ptr->GwinId, ptr->pSet, ptr->u32Size);

            break;
        }

    case MAPI_CMD_GOP_GWIN_MAPFB2WIN:
        {
            PGOP_GWIN_MAPFBINFO_PARAM ptr = (PGOP_GWIN_MAPFBINFO_PARAM)pArgs;
            //PGOP_BUFFER_INFO pInfo = (PGOP_BUFFER_INFO)ptr->pinfo;

            u32Ret = Ioctl_GOP_MapFB2Win(pInstance, ptr->fbid, ptr->GwinId);

            break;
        }
    case MAPI_CMD_GOP_GWIN_SET_WININFO:
        {
            PGOP_GWIN_SETWININFO_PARAM ptr = (PGOP_GWIN_SETWININFO_PARAM)pArgs;
            PGOP_BUFFER_INFO pInfo = (PGOP_BUFFER_INFO)ptr->pinfo;

            if (ptr->u32Size != sizeof(GOP_BUFFER_INFO))
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            u32Ret = Ioctl_GOP_SetWinInfo(pInstance, ptr->GwinId, pInfo);

            break;
        }
    case MAPI_CMD_GOP_GWIN_GET_WININFO:
        {
            PGOP_GWIN_SETWININFO_PARAM ptr = (PGOP_GWIN_SETWININFO_PARAM)pArgs;
            PGOP_BUFFER_INFO pInfo = (PGOP_BUFFER_INFO)ptr->pinfo;

            if (ptr->u32Size != sizeof(GOP_BUFFER_INFO))
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            u32Ret = Ioctl_GOP_GetWinInfo(pInstance, ptr->GwinId, pInfo);

            break;
        }

#if 0
    case MAPI_CMD_GOP_GWIN_ENABLE:
        {
            PGOP_GWIN_WIN_ENABLE_PARAM ptr = (PGOP_GWIN_WIN_ENABLE_PARAM)pArgs;
            MS_BOOL bEn;

            bEn = (MS_BOOL)*(ptr->pEn);
            u32Ret = Ioctl_GOP_Win_Enable(ptr->GwinId, bEn);

            break;
        }
#endif
    case MAPI_CMD_GOP_GWIN_SETDISPLAY:
        {
            PGOP_GWIN_DISPLAY_PARAM ptr = (PGOP_GWIN_DISPLAY_PARAM)pArgs;
            PGOP_GWINDISPLAY_INFO pInfo = (PGOP_GWINDISPLAY_INFO)ptr->pDisplayInfo;
            if (ptr->u32Size != sizeof(GOP_GWINDISPLAY_INFO))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_SetDisplay(pInstance, pInfo);
            break;
        }
    case MAPI_CMD_GOP_GWIN_DESTROY:
        {
            PGOP_GWIN_DESTROY_PARAM ptr = (PGOP_GWIN_DESTROY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_Win_Destroy(pInstance, ptr->GwinId);
        }
        break;
        //GFLIP
    case MAPI_CMD_GOP_GFLIP_CLEARQUEUE:
        {
            PGOP_GWIN_CLEARQUEUE_PARAM ptr = (PGOP_GWIN_CLEARQUEUE_PARAM)pArgs;
            PGOP_GWIN_CLEARQUEUE_INFO pInfo = (PGOP_GWIN_CLEARQUEUE_INFO)ptr->pClearInfo;
            if (ptr->u32Size != sizeof(GOP_GWIN_CLEARQUEUE_INFO))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GFlip_ClearFlipQueue(pInstance, pInfo->GwinId);
        }
        break;

    case MAPI_CMD_GOP_GFLIP_SWITCHGWIN:
        {
            PGOP_GWIN_GFLIP_SWITCH_GWIN_PARAM ptr = (PGOP_GWIN_GFLIP_SWITCH_GWIN_PARAM)pArgs;
            PGOP_GWIN_FLIP_WIN_INFO pFlip = (PGOP_GWIN_FLIP_WIN_INFO)ptr->pFlipInfo;
            if (ptr->u32Size != sizeof(GOP_GWIN_FLIP_WIN_INFO))
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            if (bLocked)
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                bLocked = FALSE;
            }
            u32Ret = Ioctl_GFlip_SwitchGwin(pInstance, pFlip);
            break;
        }
    case MAPI_CMD_GOP_GFLIP_SWITCHMULTIGWIN:
        {
            PGOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM ptr = (PGOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM)pArgs;
            PGOP_GWIN_FLIP_MULTI_WIN_INFO pMultiFlip = (PGOP_GWIN_FLIP_MULTI_WIN_INFO)ptr->pMultiFlipInfo;
            if (ptr->u32Size != sizeof(GOP_GWIN_FLIP_MULTI_WIN_INFO))
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            if (bLocked)
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                bLocked = FALSE;
            }

            u32Ret = Ioctl_GFlip_SwitchMultiGwin(pInstance, pMultiFlip);
            break;
        }
    case MAPI_CMD_GOP_GFLIP_RESTORE_VSYNC_LMT:
        {
            if (g_pGOPCtxLocal != NULL)
            {
                if (FALSE == MDrv_GOP_RestoreFromVsyncLimitation(g_pGOPCtxLocal))
                {
                    u32Ret = GOP_API_FAIL;
                }
                u32Ret = GOP_API_SUCCESS;
            }
            else
            {
                u32Ret = GOP_API_FAIL;
            }


            break;
        }
        //FB Info
    case MAPI_CMD_GOP_FB_CREATE:
        {
            PGOP_CREATE_BUFFER_PARAM ptr = (PGOP_CREATE_BUFFER_PARAM)pArgs;
            PGOP_BUFFER_INFO pBuff = (PGOP_BUFFER_INFO)ptr->pBufInfo;

            if (ptr->u32Size != sizeof(GOP_BUFFER_INFO))  //error handling for invalid ptr
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            //fbid = GOP_GetFreeFBID();

            u32Ret = Ioctl_GOP_FBCreate(pInstance, ptr->fb_type, pBuff, ptr->fbid);

            break;
        }
    case MAPI_CMD_GOP_FB_DESTROY:
        {
            PGOP_DELETE_BUFFER_PARAM ptr = (PGOP_DELETE_BUFFER_PARAM)pArgs;

            if (ptr->u32Size != sizeof(MS_U32))  //error handling for invalid ptr
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            u32Ret = Ioctl_GOP_FBDestroy(pInstance, ptr->pBufId);

            break;
        }
    case MAPI_CMD_GOP_FB_SETINFO:
        {
            PGOP_FB_INFO_PARAM ptr = (PGOP_FB_INFO_PARAM)pArgs;
            PGOP_BUFFER_INFO pBuff  = (PGOP_BUFFER_INFO)ptr->pBufInfo;
            if (ptr->u32Size != sizeof(GOP_BUFFER_INFO))  //error handling for invalid ptr
            {
                printf("[%s] (%d) Info structure Error!!\n", __FUNCTION__, __LINE__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            u32Ret = Ioctl_GOP_SetFBInfo(pInstance, ptr->fbid, pBuff);

            break;
        }
    case MAPI_CMD_GOP_FB_GETINFO:
        {
            PGOP_FB_INFO_PARAM ptr = (PGOP_FB_INFO_PARAM)pArgs;
            PGOP_BUFFER_INFO pBuff  = (PGOP_BUFFER_INFO)ptr->pBufInfo;

            u32Ret = Ioctl_GOP_GetFBInfo(pInstance, ptr->fbid, pBuff);
            break;
        }

    case MAPI_CMD_GOP_FB_SET_PROPERTY:
        {
            PGOP_FB_PROPERTY_PARAM ptr = (PGOP_FB_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_FB_SetProperty(pInstance, ptr->en_property, ptr->FBId, ptr->pSet, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_FB_GET_PROPERTY:
        {
            PGOP_FB_PROPERTY_PARAM ptr = (PGOP_FB_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_FB_GetProperty(pInstance, ptr->en_property, ptr->FBId, ptr->pSet, ptr->u32Size);

            break;
        }

        //palette
    case MAPI_CMD_GOP_PALETTE_SET_CONFIG:
        {
            PGOP_PALETTE_CONFIG_PARAM ptr = (PGOP_PALETTE_CONFIG_PARAM)pArgs;
            PGOP_PAL_OPT pPalSet = (PGOP_PAL_OPT)ptr->pPaletteInfo;

            if (ptr->u32Size != sizeof(GOP_PAL_OPT))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_Palette_Set_Config(pInstance, pPalSet->gop_idx, (DRV_GopPalReadType)pPalSet->paltype, pPalSet->palSrc);

        }
        break;
    case MAPI_CMD_GOP_PALETTE_SET_ENTRY:
        {
            PGOP_PALETTE_SET_PARAM ptr = (PGOP_PALETTE_SET_PARAM)pArgs;
            PGOP_PALETTE_ENTRY pPal = (PGOP_PALETTE_ENTRY)ptr->pClut;

            if (ptr->u32Size != sizeof(GOP_PALETTE_ENTRY))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_Palette_EntrySet(pInstance, pPal);
        }
        break;
    case MAPI_CMD_GOP_PALETTE_GET_ENTRY:
        {
            PGOP_PALETTE_SET_PARAM ptr = (PGOP_PALETTE_SET_PARAM)pArgs;
            PGOP_PALETTE_ENTRY pPal = (PGOP_PALETTE_ENTRY)ptr->pClut;

            if (ptr->u32Size != sizeof(GOP_PALETTE_ENTRY))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            u32Ret = Ioctl_GOP_Palette_EntryGet(pInstance, pPal);
            break;
        }
        //Fade
    case MAPI_CMD_GOP_GWIN_SET_FADE:
        {
            PGOP_SET_FADE_PARAM ptr = (PGOP_SET_FADE_PARAM)pArgs;
            u32Ret = Ioctl_GOP_Fade(pInstance, ptr->en_type, ptr->pSetting, ptr->u32Size);
            break;
        }
        //Scroll
    case MAPI_CMD_GOP_SET_SCROLL:
        {
            PGOP_SCROLL_PARAM ptr = (PGOP_SCROLL_PARAM)pArgs;
            u32Ret = Ioctl_GOP_Scroll(pInstance, ptr->scroll_type, ptr->pCfg, ptr->u32Size);
            break;
        }
        //PINPON
    case MAPI_CMD_GOP_SET_PINPON:
        {
            PGOP_SET_PINPON_PARAM ptr = (PGOP_SET_PINPON_PARAM)pArgs;
            u32Ret = Ioctl_GOP_PINPON(pInstance, ptr->en_pro, ptr->pSetting, ptr->u32Size);
            break;
        }
        //DWIN
    case MAPI_CMD_GOP_DWIN_INIT:
        {
            Ioctl_GOP_DWIN_Init(pInstance);
            break;
        }
    case MAPI_CMD_GOP_DWIN_CAPTURE:
        {
            PGOP_DWIN_CAPTURE_PARAM pDwinCapture = (PGOP_DWIN_CAPTURE_PARAM)pArgs;
            PGOP_CAPTURE_INFO pinfo = (PGOP_CAPTURE_INFO)pDwinCapture->pDwin;

            if (pDwinCapture->u32Size != sizeof(GOP_CAPTURE_INFO))  //error handling for invalid ptr
            {
                printf("[%s] Capture Info structure Error!!\n", __FUNCTION__);
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_Dwin_Capture(pInstance, pinfo);
            break;
        }
    case MAPI_CMD_GOP_DWIN_SET_PROPERTY:
        {
            PGOP_DWIN_PROPERTY_PARAM ptr = (PGOP_DWIN_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_DWIN_SetProperty(pInstance, ptr->en_dwin_property, ptr->pDwinProp, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_DWIN_GET_PROPERTY:
        {
            PGOP_DWIN_PROPERTY_PARAM ptr = (PGOP_DWIN_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_DWIN_GetProperty(pInstance, ptr->en_dwin_property, ptr->pDwinProp, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_DWIN_INTR:
        {
            PGOP_DWIN_INTR_STATUS_PARAM pDwinIntr = (PGOP_DWIN_INTR_STATUS_PARAM)pArgs;

            u32Ret = Ioctl_GOP_Dwin_Intr(pInstance, pDwinIntr->intr_type, pDwinIntr->pIntrInfo, pDwinIntr->u32Size);
            break;
        }


        //MISC
    case MAPI_CMD_GOP_UPDATE:
        {
            PGOP_UPDATE_PARAM ptr = (PGOP_UPDATE_PARAM)pArgs;
            PGOP_UPDATE_INFO pInfo = (PGOP_UPDATE_INFO)ptr->pUpdateInfo;

            if (ptr->u32Size != sizeof(GOP_UPDATE_INFO))
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }

            u32Ret = Ioctl_GOP_TriggerRegWriteIn(pInstance, pInfo->gop_idx, pInfo->update_type, pInfo->bEn, pInfo->bSync);
            break;
        }
    case MAPI_CMD_GOP_SELECTION:
        {
            PGOP_SELECTION_PROPERTY_PARAM ptr = (PGOP_SELECTION_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_Select(pInstance, ptr->sel_type, ptr->id, ptr->pinfo);

            break;
        }
    case MAPI_CMD_GOP_REGISTER_CB:
        {
            PGOP_REGISTER_CB_PARAM ptr = (PGOP_REGISTER_CB_PARAM)pArgs;

            u32Ret = Ioctl_GOP_Register_CB(pInstance, ptr->cb_type, ptr->pCB, ptr->u32Size);

            break;
        }
    case MAPI_CMD_GOP_MUTEX:
        {
            GOP_MUTEX_PARAM *ptr = (GOP_MUTEX_PARAM *)pArgs;

            if (ptr->u32Size != sizeof(GOP_MUTEX_PARAM))
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            if (ptr->en_mutex == E_GOP_LOCK)
            {
                if (UtopiaResourceObtain(pModule, GOP_POOL_ID_GOP0, &(psGOPInstPri->pResource)) != 0)
                {
                    printf("[%s %d]UtopiaResourceObtainToInstant fail\n", __FUNCTION__, __LINE__);
                    return 0xFFFFFFFF;
                }
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = MsOS_GetOSThreadID();
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = getpid();
#endif
            }
            else if (ptr->en_mutex == E_GOP_UNLOCK)
            {
                g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
                g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
                UtopiaResourceRelease(psGOPInstPri->pResource);
            }
            break;
        }
    case MAPI_CMD_GOP_MIXER_SET_PROPERTY:
        {
            PGOP_MIXER_PROPERTY_PARAM ptr = (PGOP_MIXER_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_MIXER_SetProperty(pInstance, ptr->en_mixer_property, ptr->pMixerProp, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_MIXER_GET_PROPERTY:
        {
            PGOP_MIXER_PROPERTY_PARAM ptr = (PGOP_MIXER_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_MIXER_GetProperty(pInstance, ptr->en_mixer_property, ptr->pMixerProp, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_VE_SET_PROPERTY:
        {
            PGOP_VE_PROPERTY_PARAM ptr = (PGOP_VE_PROPERTY_PARAM)pArgs;

            u32Ret = Ioctl_GOP_VE_SetProperty(pInstance, ptr->en_ve_property, ptr->pVEProp, ptr->u32Size);
            break;
        }
    case MAPI_CMD_GOP_MISC:
        {
            PGOP_MISC_PARAM ptr = (PGOP_MISC_PARAM)pArgs;
            u32Ret = Ioctl_GOP_MISC(pInstance, ptr->misc_type, ptr->pMISC, ptr->u32Size);
            break;
        }
    default:
        break;
    }

    if (bLocked)
    {
        g_pGOPCtxLocal->pGOPCtxShared->s32GOPLockTid = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
        g_pGOPCtxLocal->pGOPCtxShared->GOPLockPid = 0;
#endif
        UtopiaResourceRelease(psGOPInstPri->pResource);
    }
    if (u32Ret != GOP_API_SUCCESS)
    {
        printf("[%s] ERROR on cmd:%lx\n", __FUNCTION__, u32Cmd);
    }

    //printf("(%s) Done\n\n",__FUNCTION__);
#endif //UTOPIA_STRIP
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GOPClose(void *pInstance)
{
#ifndef UTOPIA_STRIP
    Ioctl_GOP_AtExit(pInstance);
#endif //UTOPIA_STRIP
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}


