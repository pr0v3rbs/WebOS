#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#else
#include <linux/slab.h>
#endif
#include "utopia_dapi.h"
#include "utopia.h"
#include "_apigfx_type.h"
#include "apiGFX.h"
#include "apiGFX_v2.h"
#include "drvGE.h"
#include "halGE.h"
#include "drvGE_private.h"
#include "apiGFX_private.h"

//==========================================================
//Macro and Define
//==========================================================
#if(GFX_UTOPIA20)
GFX_INSTANT_PRIVATE*    psGFXInstPri;
#define g_apiGFXLocal   psGFXInstPri->GFXPrivate_g_apiGFXLocal
#endif

#define CheckSize(u32InputSize, u32StuctSize, eCmd) do{\
                                        if(u32InputSize != u32StuctSize)\
                                        {\
                                            GFX_DEBUG_MSG(printf("[%s][%d] GE Check Size Error, Invalid input structure size :0x%lx , except structure size is 0x%lx; [eCmd=%d]\n",__FUNCTION__,__LINE__,(MS_U32)u32InputSize, (MS_U32)u32StuctSize, eCmd));\
                                            API_GE_RETURN(g_apiGFXLocal.g_pGEContext, UTOPIA_STATUS_FAIL, eCmd);\
                                        }\
                                    }while(0)


#define APICheckU02()               {\
                                        UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);\
                                    }


#if (GE_API_MUTEX)
static MS_U32 u32CheckMutexCnt=0;
#define _GFX_CheckMutex(_ret, cmd)  do{\
                                        if(u32CheckMutexCnt !=0)\
                                        {\
                                            printf("\33[0;31m [%s][%d] Entry/Return Not pair!!!!! Cmd=%d , u32CheckMutexCnt=%ld \33[m;\n",__FUNCTION__,__LINE__, cmd, u32CheckMutexCnt);\
                                        }\
                                    }while(0)


#define GET_GE_ENTRY(pGECtx)       do{\
                                            if(u32CheckMutexCnt!=0)\
                                                printf("\33[0;31m [%s][%d] Entry not zero!!!! u32CheckMutexCnt=%ld \33[m;\n",__FUNCTION__,__LINE__, u32CheckMutexCnt);\
                                        _GE_MUXTEX_ENTRY(pGECtx, E_GE_POOL_ID_INTERNAL_VARIABLE);\
                                        u32CheckMutexCnt++;\
                                    }while(0)

#define RELEASE_GE_RETURN(pGECtx, _ret) do{\
                                        _GE_MUXTEX_RETURN(pGECtx, E_GE_POOL_ID_INTERNAL_VARIABLE);\
                                        u32CheckMutexCnt--;\
                                    }while(0)


#if GE_MUTEX_DBG
    #define API_GE_ENTRY(pGECtx)     do{\
                                        GET_GE_ENTRY(pGECtx);\
                                    }while(0)

    #define API_GE_RETURN(pGECtx, _ret, cmd) do{\
                                        RELEASE_GE_RETURN(pGECtx, _ret);\
                                        _GFX_CheckMutex(_ret, cmd);\
                                        return _ret;\
                                    }while(0)
#else/*In order to deduce code size*/
    #define API_GE_ENTRY(pGECtx)    do{\
                                        _API_GE_ENTRY(pGECtx);\
                                    }while(0)

    #define API_GE_RETURN(pGECtx, _ret , cmd) do{\
                                        _API_GE_RETURN(pGECtx, _ret);\
                                        _GFX_CheckMutex(_ret, cmd);\
                                        return _ret;\
                                    }while(0)
#endif

//===========================================================================================================================================
#else
    #define API_GE_ENTRY(pGECtx)    do{\
                                    }while(0)

    #define API_GE_RETURN(pGECtx, _ret , cmd) do{\
                                        return _ret;\
                                    }while(0)

#endif
// API level could only contains local parameters
#define DYNAMIC_POWER_ON_OFF 0

#ifndef UTOPIA_STRIP

#if (!GFX_UTOPIA20)
GFX_API_LOCAL g_apiGFXLocal =
{
u32dbglvl: -1,
fpGetBMP:NULL,
fpGetFont:NULL,
_blendcoef:COEF_ASRC,
_angle:GEROTATE_0,
_bNearest:FALSE,
_bPatchMode:FALSE,
_bMirrorH:FALSE,
_bMirrorV:FALSE,
_bDstMirrorH:FALSE,
_bDstMirrorV:FALSE,
_bItalic:FALSE,
_line_enable:FALSE,
_line_pattern:0x00,
_line_factor:0,
#ifdef DBGLOG
_bOutFileLog:FALSE,
_pu16OutLogAddr:NULL,
_u16LogCount:0,
#endif
g_pGEContext:NULL,
pGeChipProperty:NULL,
u32LockStatus:0,
_bInit:0,
u32geRgbColor:0,

};
#endif


#if (GE_API_MUTEX)
static void _API_GE_ENTRY(GE_Context* pGECtx)
{
    GET_GE_ENTRY(pGECtx);
}

static MS_U16 _API_GE_RETURN(GE_Context* pGECtx, MS_U16 ret)
{
    RELEASE_GE_RETURN(pGECtx, ret);
    return ret;
}
#endif

static int MapRet(GFX_Result eGFXResoult)
{
    MS_U32  u32Ret= UTOPIA_STATUS_FAIL;

    switch(eGFXResoult)
    {
        case GFX_FAIL:
               u32Ret = UTOPIA_STATUS_FAIL;
               break;
        case GFX_SUCCESS:
               u32Ret = UTOPIA_STATUS_SUCCESS;
               break;
        case GFX_DRV_NOT_SUPPORT:
               u32Ret = UTOPIA_STATUS_NOT_SUPPORTED;
               break;
        case GFX_NON_ALIGN_ADDRESS:
        case GFX_NON_ALIGN_PITCH:
        case GFX_INVALID_PARAMETERS:
               u32Ret = UTOPIA_STATUS_NOT_SUPPORTED;
               break;
        default:
               break;
    }
    return u32Ret;
}

static MS_U32 MApi_GFX_MapStrSCKType(MS_U32 gfxOP, MS_U32* geOP)
{

    switch(gfxOP)
    {
        case GFX_DONOTHING:
            *geOP = (MS_U32)E_GE_DONOTHING;
            break;
        case GFX_NEAREST:
            *geOP = (MS_U32)E_GE_NEAREST;
            break;
        case GFX_REPLACE_KEY_2_CUS:
            *geOP = (MS_U32)E_GE_REPLACE_KEY_2_CUS;
            break;

        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapYUVOp(MS_U32 OpType, MS_U32 gfxOp, MS_U32* geOP)
{

    switch(OpType)
    {
        case GFX_YUV_OP1:
            switch(gfxOp)
            {
                case GFX_YUV_RGB2YUV_PC:
                    *geOP = (MS_U32)E_GE_YUV_RGB2YUV_PC;
                    break;
                case GFX_YUV_RGB2YUV_255:
                    *geOP = (MS_U32)E_GE_YUV_RGB2YUV_255;
                    break;
                default:
                    return (MS_U32)GFX_INVALID_PARAMETERS;
                    break;
            }
            break;
        case GFX_YUV_OP2:
            switch(gfxOp)
            {
                case GFX_YUV_OUT_255:
                    *geOP = (MS_U32)E_GE_YUV_OUT_255;
                    break;
                case GFX_YUV_OUT_PC:
                    *geOP = (MS_U32)E_GE_YUV_OUT_PC;
                    break;
                default:
                    return (MS_U32)GFX_INVALID_PARAMETERS;
                    break;
            }
            break;
        case GFX_YUV_OP3:
            switch(gfxOp)
            {
                case GFX_YUV_IN_255:
                    *geOP = (MS_U32)E_GE_YUV_IN_255;
                    break;
                case GFX_YUV_IN_127:
                    *geOP = (MS_U32)E_GE_YUV_IN_127;
                    break;
                default:
                    return (MS_U32)GFX_INVALID_PARAMETERS;
                    break;
            }
            break;
        case GFX_YUV_OP4:
            switch(gfxOp)
            {
                case GFX_YUV_YVYU:
                    *geOP = (MS_U32)E_GE_YUV_YVYU;
                    break;
                case GFX_YUV_YUYV:
                    *geOP = (MS_U32)E_GE_YUV_YUYV;
                    break;
                case GFX_YUV_VYUY:
                    *geOP = (MS_U32)E_GE_YUV_VYUY;
                    break;
                case GFX_YUV_UYVY:
                    *geOP = (MS_U32)E_GE_YUV_UYVY;
                    break;
                default:
                    return (MS_U32)GFX_INVALID_PARAMETERS;
                    break;
            }
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;

    }
    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapACmp(MS_U32 gfxACmp, MS_U32* geACmp )
{

    switch(gfxACmp)
    {
        case GFX_ACMP_OP_MAX:
            *geACmp = (MS_U32)E_GE_ACMP_OP_MAX;
            break;
        case GFX_GE_ACMP_OP_MIN:
            *geACmp = (MS_U32)E_GE_ACMP_OP_MIN;
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapBLDCOEF(MS_U32 gfxCOEF, MS_U32* geCOEF )
{
    switch(gfxCOEF)
    {
        case COEF_ONE:
            *geCOEF = (MS_U32)E_GE_BLEND_ONE;
            break;
        case COEF_CONST:
            *geCOEF = (MS_U32)E_GE_BLEND_CONST;
            break;
        case COEF_ASRC:
            *geCOEF = (MS_U32)E_GE_BLEND_ASRC;
            break;
        case COEF_ADST:
            *geCOEF = (MS_U32)E_GE_BLEND_ADST;
            break;
        case COEF_ZERO:
            *geCOEF = (MS_U32)E_GE_BLEND_ZERO;
            break;
        case COEF_1_CONST:
            *geCOEF = (MS_U32)E_GE_BLEND_CONST_INV;
            break;
        case COEF_1_ASRC:
            *geCOEF = (MS_U32)E_GE_BLEND_ASRC_INV;
            break;
        case COEF_1_ADST:
            *geCOEF = (MS_U32)E_GE_BLEND_ADST_INV;
            break;

        case COEF_ROP8_ALPHA:
            *geCOEF = (MS_U32)E_GE_BLEND_ROP8_ALPHA;
            break;
        case COEF_ROP8_SRCOVER:
            *geCOEF = (MS_U32)E_GE_BLEND_ROP8_SRCOVER;
            break;
        case COEF_ROP8_DSTOVER:
            *geCOEF = (MS_U32)E_GE_BLEND_ROP8_DSTOVER;
            break;
        case COEF_CONST_SRC:
            *geCOEF = (MS_U32)E_GE_BLEND_ALPHA_ADST;
            break;
        case COEF_1_CONST_SRC:
            *geCOEF = (MS_U32)E_GE_BLEND_INV_CONST;
            break;
        case COEF_SRC_ATOP_DST:
            *geCOEF = (MS_U32)E_GE_BLEND_SRC_ATOP_DST;
            break;
        case COEF_DST_ATOP_SRC:
            *geCOEF = (MS_U32)E_GE_BLEND_DST_ATOP_SRC;
            break;
        case COEF_SRC_XOR_DST:
            *geCOEF = (MS_U32)E_GE_BLEND_SRC_XOR_DST;
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapABLCOEF(MS_U32 gfxABL, MS_U32* geABL )
{

    switch(gfxABL)
    {
        case ABL_FROM_CONST:
            *geABL = (MS_U32)E_GE_ALPHA_CONST;
            break;
        case ABL_FROM_ASRC:
            *geABL = (MS_U32)E_GE_ALPHA_ASRC;
            break;
        case ABL_FROM_ADST:
            *geABL = (MS_U32)E_GE_ALPHA_ADST;
            break;
        case ABL_FROM_ROP8_SRC:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_SRC;
            break;
        case ABL_FROM_ROP8_IN:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_IN;
            break;
        case ABL_FROM_ROP8_DSTOUT:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_DSTOUT;
            break;
        case ABL_FROM_ROP8_SRCOUT:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_SRCOUT;
            break;
        case ABL_FROM_ROP8_OVER:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_OVER;
            break;
        case ABL_FROM_ROP8_INV_CONST:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_INV_CONST;
            break;
        case ABL_FROM_ROP8_INV_ASRC:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_INV_ASRC;
            break;
        case ABL_FROM_ROP8_INV_ADST:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_INV_ADST;
            break;
        case ABL_FROM_ROP8_SRC_ATOP_DST:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_SRC_ATOP_DST;
            break;
        case ABL_FROM_ROP8_DST_ATOP_SRC:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_DST_ATOP_SRC;
            break;
        case ABL_FROM_ROP8_SRC_XOR_DST:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_SRC_XOR_DST;
            break;
        case ABL_FROM_ROP8_INV_SRC_ATOP_DST:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_INV_SRC_ATOP_DST;
            break;
        case ABL_FROM_ROP8_INV_DST_ATOP_SRC:
            *geABL = (MS_U32)E_GE_ALPHA_ROP8_INV_DST_ATOP_SRC;
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapROP2(MS_U32 gfxROP, MS_U32* geROP )
{

    switch(gfxROP)
    {
        case ROP2_OP_ZERO:
            *geROP = (MS_U32)E_GE_ROP2_ZERO;
            break;
        case ROP2_OP_NOT_PS_OR_PD:
            *geROP = (MS_U32)E_GE_ROP2_NOT_PS_OR_PD;
            break;
        case ROP2_OP_NS_AND_PD:
            *geROP = (MS_U32)E_GE_ROP2_NS_AND_PD;
            break;
        case ROP2_OP_NS:
            *geROP = (MS_U32)E_GE_ROP2_NS;
            break;
        case ROP2_OP_PS_AND_ND:
            *geROP = (MS_U32)E_GE_ROP2_PS_AND_ND;
            break;
        case ROP2_OP_ND:
            *geROP = (MS_U32)E_GE_ROP2_ND;
            break;
        case ROP2_OP_PS_XOR_PD:
            *geROP = (MS_U32)E_GE_ROP2_PS_XOR_PD;
            break;
        case ROP2_OP_NOT_PS_AND_PD:
            *geROP = (MS_U32)E_GE_ROP2_NOT_PS_AND_PD;
            break;
        case ROP2_OP_PS_AND_PD:
            *geROP = (MS_U32)E_GE_ROP2_PS_AND_PD;
            break;
        case ROP2_OP_NOT_PS_XOR_PD:
            *geROP = (MS_U32)E_GE_ROP2_NOT_PS_XOR_PD;
            break;
        case ROP2_OP_PD:
            *geROP = (MS_U32)E_GE_ROP2_PD;
            break;
        case ROP2_OP_NS_OR_PD:
            *geROP = (MS_U32)E_GE_ROP2_NS_OR_PD;
            break;
        case ROP2_OP_PS:
            *geROP = (MS_U32)E_GE_ROP2_PS;
            break;
        case ROP2_OP_PS_OR_ND:
            *geROP = (MS_U32)E_GE_ROP2_PS_OR_ND;
            break;
        case ROP2_OP_PD_OR_PS:
            *geROP = (MS_U32)E_GE_ROP2_PD_OR_PS;
            break;
        case ROP2_OP_ONE:
            *geROP = (MS_U32)E_GE_ROP2_ONE;
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}



static MS_U32 MApi_GFX_MapCKOP(MS_U32 gfxCKOP, MS_U32* geCKOP)
{

    switch(gfxCKOP)
    {
        case CK_OP_EQUAL:
            *geCKOP = (MS_U32)E_GE_CK_EQ;
            break;
        case CK_OP_NOT_EQUAL:
            *geCKOP = (MS_U32)E_GE_CK_NE;
            break;
        case AK_OP_EQUAL:
            *geCKOP = (MS_U32)E_GE_ALPHA_EQ;
            break;
        case AK_OP_NOT_EQUAL:
            *geCKOP = (MS_U32)E_GE_ALPHA_NE;
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 GFX_RectBltFlags(void) // for legacy driver BitBlt and FillRect
{
    MS_U32 flags = 0;

    if (g_apiGFXLocal._bItalic)
    {
        flags |= E_GE_FLAG_BLT_ITALIC;
    }
    if (g_apiGFXLocal._bMirrorH)
    {
        flags |= E_GE_FLAG_BLT_MIRROR_H;
    }
    if (g_apiGFXLocal._bMirrorV)
    {
        flags |= E_GE_FLAG_BLT_MIRROR_V;
    }
    if (g_apiGFXLocal._bDstMirrorH)
    {
        flags |= E_GE_FLAG_BLT_DST_MIRROR_H;
    }
    if (g_apiGFXLocal._bDstMirrorV)
    {
        flags |= E_GE_FLAG_BLT_DST_MIRROR_V;
    }
    switch(g_apiGFXLocal._angle)
    {
    case GEROTATE_90:
        flags |= E_GE_FLAG_BLT_ROTATE_90;
        break;
    case GEROTATE_180:
        flags |= E_GE_FLAG_BLT_ROTATE_180;
        break;
    case GEROTATE_270:
        flags |= E_GE_FLAG_BLT_ROTATE_270;
        break;
    default:
        break;
    }
    if (g_apiGFXLocal._bNearest)
    {
        flags |= E_GE_FLAG_STRETCH_NEAREST;
    }
    return flags;
}

static MS_U32 MApi_GFX_MapVCMQSZ(MS_U32 gfxOP, MS_U32* geOP)
{

    switch(gfxOP)
    {
        case GFX_VCMD_4K:
            *geOP = (MS_U32)E_GE_VCMD_4K;
            break;
        case GFX_VCMD_8K:
            *geOP = (MS_U32)E_GE_VCMD_8K;
            break;
        case GFX_VCMD_16K:
            *geOP = (MS_U32)E_GE_VCMD_16K;
            break;
        case GFX_VCMD_32K:
            *geOP = (MS_U32)E_GE_VCMD_32K;
            break;
        case GFX_VCMD_64K:
            *geOP = (MS_U32)E_GE_VCMD_64K;
            break;
        case GFX_VCMD_128K:
            *geOP = (MS_U32)E_GE_VCMD_128K;
            break;
        case GFX_VCMD_256K:
            *geOP = (MS_U32)E_GE_VCMD_256K;
            break;
        case GFX_VCMD_512K:
            *geOP = (MS_U32)E_GE_VCMD_512K;
            break;
        case GFX_VCMD_1024K:
            *geOP = (MS_U32)E_GE_VCMD_1024K;
            break;
        default:
            return (MS_U32)GFX_INVALID_PARAMETERS;
            break;
    }

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapDFBBldFlag(MS_U16 gfxBldFlag, MS_U16* geBldFlag)
{
    //the bld flag is one-to-one map:
    *geBldFlag = gfxBldFlag;

    return (MS_U32)GFX_SUCCESS;
}

static MS_U32 MApi_GFX_MapDFBBldOP(GFX_DFBBldOP gfxBldOP, GE_DFBBldOP* geBldOP)
{
    //the bld op is one-to-one map:
    *geBldOP = (GE_DFBBldOP)gfxBldOP;

    return (MS_U32)GFX_SUCCESS;
}

static MS_BOOL GFX_CheckInClipWindow(MS_U16 u16X1, MS_U16 u16Y1, MS_U16 u16X2, MS_U16 u16Y2)
{
    GFX_DEBUGINFO(printf( "%s Not support\n", __FUNCTION__));

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// GFX tool function: Convert ARGB/index into the color format for intensity register.
/// @param Fmt \b IN: type of target color format.
/// @param colorinfo \b IN: pointer to color/index structure.
/// @param low \b OUT: pointer to 16-bit data to be filled in Intensity low word.
/// @param high \b OUT: pointer to 16-bit data to be filled in Intensity high word.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
void GFX_ConvertRGB2DBFmt(GFX_Buffer_Format Fmt, MS_U32 *colorinfo, MS_U16* low, MS_U16* high)
{
    GFX_RgbColor   *color = NULL;
    GFX_BlinkData  *blinkData = NULL;
    //MS_U8 a, r, g, b;


    if ((NULL == colorinfo) || (NULL == low) || (NULL == high)){
        printf("Convert RGB2DB FAIL\n");
        return;
    }


    // @FIXME: Richard uses GFX_FMT_1ABFGBG12355 instead
    if ( (GFX_FMT_1ABFGBG12355== Fmt) || (GFX_FMT_FABAFGBG2266 == Fmt))
    // if (Fmt == GFX_FMT_1BAAFGBG123433)
    {
        blinkData =(GFX_BlinkData *)colorinfo;
    }
    else
    {
        color     =(GFX_RgbColor  *)colorinfo;
    }

    switch (Fmt)
    {
        case GFX_FMT_RGB565 :
            *low = ((color->b & 0xf8) + (color->b >> 5)) | (((color->g & 0xfc) + (color->g>>6))<<8);
            *high = ((color->r & 0xf8) + (color->r >> 5)) | ((color->a & 0xff) << 8);
            break;
        case GFX_FMT_RGBA5551:
            if (color->a > 0)
            {
                *low = ((color->g & 0xf8) + (color->g >> 5)) | (0xff << 8);
            }
            else
            {
                *low = ((color->g & 0xf8) + (color->g >> 5));
            }
            *high = ((color->g & 0xf8) + (color->g >> 5)) | (((color->r & 0xf8) + (color->r >> 5))<<8);
            break;
        case GFX_FMT_ARGB1555 :
            *low = ((color->b & 0xf8) + (color->b >> 5)) | (((color->g & 0xf8) + (color->g >> 5))<<8);
            if (color->a > 0)
            {
                *high = ((color->r & 0xf8) + (color->r >> 5)) | (0xff << 8);
            }
            else
            {
                *high = ((color->r & 0xf8) + (color->r >> 5));
            }
            break;
        case GFX_FMT_RGBA4444:
            *low = ((color->a & 0xf0) + (color->a >> 4)) | (((color->b & 0xf0) + (color->b >> 4))<<8);
            *high = ((color->g & 0xf0) + (color->g >> 4)) | (((color->r & 0xf0) + (color->r >> 4))<<8);
            break;
        case GFX_FMT_ARGB4444 :
            *low = ((color->b & 0xf0) + (color->b >> 4)) | (((color->g & 0xf0) + (color->g >> 4))<<8);
            *high = ((color->r & 0xf0) + (color->r >> 4)) | (((color->a & 0xf0) + (color->a >> 4))<<8);
            break;
        case GFX_FMT_ABGR8888:
            *low  =(color->r & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->b & 0xff) | ((color->a & 0xff) << 8);
            break;
        case GFX_FMT_ARGB8888 :
            *low  =(color->b & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->r & 0xff) | ((color->a & 0xff) << 8);
            break;
        case GFX_FMT_I8 :
            *low  = (color->b & 0xff)|((color->b & 0xff) << 8);
            *high = (color->b & 0xff)|((color->b & 0xff) << 8);
            break;
        // @FIXME: Richard uses GFX_FMT_1ABFGBG12355 instead
        //          1 A B Fg Bg
        //          1 2 3  5  5
        case GFX_FMT_1ABFGBG12355:
            *low = (0x1f & blinkData->background) |                     // Bg: 4..0
                   ((0x1f & blinkData->foreground)<< 5) |               // Fg: 9..5
                   ((0x1f & blinkData->ctrl_flag)<< 10) |               // [A, B]: [14..13, 12..10]
                   BIT15;                                               // Bit 15
            *high= (0x1f & blinkData->background) |                     // Bg: 4..0
                   ((0x1f & blinkData->foreground)<< 5) |               // Fg: 9..5
                   ((0x1f & blinkData->ctrl_flag)<< 10) |               // [A, B]: [14..13, 12..10]
                   BIT15;                                               // Bit 15
            break;

#if 0
                    1 B A A Fg Bg
                    1 2 3 4  3  3
        case GFX_FMT_1BAAFGBG123433 :
            *low = ((blinkData->background & 0x7)|((blinkData->foreground & 0x7) << 3)|((blinkData->ctrl_flag & 0x1ff)<<6)|(0x1 << 15));
            *high = ((blinkData->background & 0x7)|((blinkData->foreground & 0x7) << 3)|((blinkData->ctrl_flag & 0x1ff)<<6)|(0x1 << 15));
            break;
#endif
        case GFX_FMT_YUV422:
            //printf("[GE DRV][%06d] Are you sure to draw in YUV?\n", __LINE__);
            *low  =(color->b & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->r & 0xff) | ((color->a & 0xff) << 8);

            break;
        case GFX_FMT_FABAFGBG2266:
            *low = ((blinkData->background & 0x3f)|((blinkData->foreground & 0x3f) << 6)|((blinkData->Bits3.Ba & 0x3)<<12)|((blinkData->Bits3.Fa & 0x3)<<14));
            *high = *low;

            break;
        default:
            GFX_ASSERT(0, printf("[GE DRV][%06d] Bad color format\n", __LINE__));
            *low  =(color->b & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->r & 0xff) | ((color->a & 0xff) << 8);
            break;
    }

}

//-------------------------------------------------------------------------------------------------
/// GFX tool function: Convert ARGB/index into the color format for Primitive color.
/// @param Fmt \b IN: type of target color format.
/// @param colorinfo \b IN: pointer to color/index structure.
/// @param low \b OUT: pointer to 16-bit data to be filled in Intensity low word.
/// @param high \b OUT: pointer to 16-bit data to be filled in Intensity high word.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
void GFX_ConvertRGB2PCFmt(GFX_Buffer_Format Fmt, MS_U32 *colorinfo, MS_U16* low, MS_U16* high)
{
    GFX_RgbColor   *color = NULL;
    GFX_BlinkData  *blinkData = NULL;
    //MS_U8 a, r, g, b;


    if ((NULL == colorinfo) || (NULL == low) || (NULL == high)){
        printf("Convert RGB2DB FAIL\n");
        return;
    }


    // @FIXME: Richard uses GFX_FMT_1ABFGBG12355 instead
    if ( (GFX_FMT_1ABFGBG12355== Fmt) || (GFX_FMT_FABAFGBG2266 == Fmt))
    // if (Fmt == GFX_FMT_1BAAFGBG123433)
    {
        blinkData =(GFX_BlinkData *)colorinfo;
    }
    else
    {
        color     =(GFX_RgbColor  *)colorinfo;
    }

    switch (Fmt)
    {
        case GFX_FMT_RGB565 :
            *low = ((color->b & 0xf8) + (color->b >> 5)) | (((color->g & 0xfc) + (color->g>>6))<<8);
            *high = ((color->r & 0xf8) + (color->r >> 5)) | ((color->a & 0xff) << 8);
            break;
        case GFX_FMT_RGBA5551:
            if (color->a > 0)
            {
                *low = ((color->g & 0xf8) + (color->g >> 5)) | (0xff << 8);
            }
            else
            {
                *low = ((color->g & 0xf8) + (color->g >> 5));
            }
            *high = ((color->g & 0xf8) + (color->g >> 5)) | (((color->r & 0xf8) + (color->r >> 5))<<8);
            break;
        case GFX_FMT_ARGB1555 :
            *low = ((color->b & 0xf8) + (color->b >> 5)) | (((color->g & 0xf8) + (color->g >> 5))<<8);
            if (color->a > 0)
            {
                *high = ((color->r & 0xf8) + (color->r >> 5)) | (0xff << 8);
            }
            else
            {
                *high = ((color->r & 0xf8) + (color->r >> 5));
            }
            break;
        case GFX_FMT_RGBA4444:
            *low = ((color->a & 0xf0) + (color->a >> 4)) | (((color->b & 0xf0) + (color->b >> 4))<<8);
            *high = ((color->g & 0xf0) + (color->g >> 4)) | (((color->r & 0xf0) + (color->r >> 4))<<8);
            break;
        case GFX_FMT_ARGB4444 :
            *low = ((color->b & 0xf0) + (color->b >> 4)) | (((color->g & 0xf0) + (color->g >> 4))<<8);
            *high = ((color->r & 0xf0) + (color->r >> 4)) | (((color->a & 0xf0) + (color->a >> 4))<<8);
            break;
        case GFX_FMT_ABGR8888:
            *low  =(color->r & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->b & 0xff) | ((color->a & 0xff) << 8);
            break;
        case GFX_FMT_ARGB8888 :
            *low  =(color->b & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->r & 0xff) | ((color->a & 0xff) << 8);
            break;
        case GFX_FMT_I8 :
            *low  = (color->b & 0xff)|((color->b & 0xff) << 8);
            *high = (color->b & 0xff)|((color->b & 0xff) << 8);
            break;
        // @FIXME: Richard uses GFX_FMT_1ABFGBG12355 instead
        //          1 A B Fg Bg
        //          1 2 3  5  5
        case GFX_FMT_1ABFGBG12355:
            *low = ((0x1f & blinkData->background)<<3) |               //Bg
                   (((0x1f & blinkData->foreground)<<3)<<8);           //Fg

            *high= ((0x1f & blinkData->ctrl_flag)<<3) | BIT15;         //1AB
            break;
#if 0
                    1 B A A Fg Bg
                    1 2 3 4  3  3
        case GFX_FMT_1BAAFGBG123433 :
            *low = ((blinkData->background & 0x7)|((blinkData->foreground & 0x7) << 3)|((blinkData->ctrl_flag & 0x1ff)<<6)|(0x1 << 15));
            *high = ((blinkData->background & 0x7)|((blinkData->foreground & 0x7) << 3)|((blinkData->ctrl_flag & 0x1ff)<<6)|(0x1 << 15));
            break;
#endif
        case GFX_FMT_YUV422:
            //printf("[GE DRV][%06d] Are you sure to draw in YUV?\n", __LINE__);
            *low  =(color->b & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->r & 0xff) | ((color->a & 0xff) << 8);

            break;
        case GFX_FMT_FABAFGBG2266:
            *low = ((blinkData->background & 0x3f)<<2)|(((blinkData->foreground & 0x3f)<<2)<<8);
            *high = ((blinkData->Bits3.Ba & 0x3)<<6)|(((blinkData->Bits3.Fa & 0x3)<<6)<<8);

            break;
        default:
            GFX_ASSERT(0, printf("[GE DRV][%06d] Bad color format\n", __LINE__));
            *low  =(color->b & 0xff) | ((color->g & 0xff) << 8);
            *high =(color->r & 0xff) | ((color->a & 0xff) << 8);
            break;
    }

}

GFX_Result MApi_GFX_GetBufferInfo_U02(void* pInstance, PGFX_BufferInfo srcbufInfo, PGFX_BufferInfo dstbufInfo)
{
    GE_BufInfo bufinfo;
    GE_Result ret;

    APICheckU02();

    ret = MDrv_GE_GetBufferInfo(g_apiGFXLocal.g_pGEContext, &bufinfo);
    srcbufInfo->u32ColorFmt = (GFX_Buffer_Format)bufinfo.srcfmt;  // DANGER, need refine.
    srcbufInfo->u32Pitch = bufinfo.srcpit;
    srcbufInfo->u32Addr = bufinfo.srcaddr;

    dstbufInfo->u32ColorFmt = (GFX_Buffer_Format)bufinfo.dstfmt;
    dstbufInfo->u32Pitch = bufinfo.dstpit;
    dstbufInfo->u32Addr = bufinfo.dstaddr;

    return (GFX_Result)ret;   // DANGER, need refine.
}

GFX_Result MApi_GFX_GetClip_U02(void* pInstance, GFX_Point* v0, GFX_Point* v1)
{
    GE_Rect rect;

    APICheckU02();

    MDrv_GE_GetClipWindow(g_apiGFXLocal.g_pGEContext, &rect);

    v0->x = rect.x ; // dangerous if V0 > V1
    v0->y = rect.y ;
    v1->x = rect.width  + v0->x - 1;
    v1->y = rect.height + v0->y - 1;

    return GFX_SUCCESS;
}

GFX_Result MApi_GFX_GetGECaps_U02(void* pInstance, EN_GFX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
    APICheckU02();

    switch(eCapType)
    {
        case E_GFX_CAP_MULTI_PIXEL:
        {
            GFX_CapMultiPixelInfo *pMultiPixelInfo;
            if(ret_size != sizeof(GFX_CapMultiPixelInfo))
            {
                printf("[%s] ERROR, invalid input structure size :%ld on GFX_CapMultiPixelInfo\n",__FUNCTION__,ret_size);
                return GFX_INVALID_PARAMETERS;
            }
            pMultiPixelInfo = (GFX_CapMultiPixelInfo *)pRet;
            pMultiPixelInfo->bSupportMultiPixel = g_apiGFXLocal.pGeChipProperty->bSupportMultiPixel;
            pMultiPixelInfo->WordUnit = g_apiGFXLocal.pGeChipProperty->WordUnit;
        }
        break;
        case E_GFX_CAP_BLT_DOWN_SCALE:
        {
            GFX_BLT_DownScaleInfo *pBltDownScaleInfo;
            if(ret_size != sizeof(GFX_BLT_DownScaleInfo))
            {
                printf("[%s] ERROR, invalid input structure size :%ld on GFX_BLT_DownScaleInfo\n",__FUNCTION__,ret_size);
                return GFX_INVALID_PARAMETERS;
            }
            pBltDownScaleInfo = (GFX_BLT_DownScaleInfo *)pRet;
            pBltDownScaleInfo->bFullRangeSupport = g_apiGFXLocal.pGeChipProperty->BltDownScaleCaps.bFullRangeSupport;
            pBltDownScaleInfo->u8RangeMax = g_apiGFXLocal.pGeChipProperty->BltDownScaleCaps.u8RangeMax;
            pBltDownScaleInfo->u8RangeMin = g_apiGFXLocal.pGeChipProperty->BltDownScaleCaps.u8RangeMin;
            pBltDownScaleInfo->u8ContinuousRangeMin = g_apiGFXLocal.pGeChipProperty->BltDownScaleCaps.u8ContinuousRangeMin;
        }
        break;
        case E_GFX_CAP_DFB_BLENDING:
        {
            MS_U16 *pU16SupportedBldFlags;
            if(ret_size != sizeof(MS_U16))
            {
                printf("[%s] ERROR, invalid input structure size :%ld on GFX_CAP_DFB_BLENDING\n",__FUNCTION__,ret_size);
                return GFX_INVALID_PARAMETERS;
            }
            pU16SupportedBldFlags = (MS_U16 *)pRet;
            MDrv_GE_QueryDFBBldCaps(g_apiGFXLocal.g_pGEContext, pU16SupportedBldFlags);
        }
        break;
        case E_GFX_CAP_ALIGN:
        {
            GFX_FmtAlignCapsInfo* pAlignCapsInfo;
            GE_FmtCaps          caps ={0};
            pAlignCapsInfo = (GFX_FmtAlignCapsInfo *)pRet;
            caps.u8BaseAlign=0;
            caps.u8PitchAlign=0;
            caps.u8Non1pAlign=0;
            MDrv_GE_GetFmtCaps(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)pAlignCapsInfo->u32ColorFmt, (GE_BufType)pAlignCapsInfo->eBufferType, &caps);

            switch(pAlignCapsInfo->eFmtCapsType)
            {
                case E_GFX_FMT_CAP_NONE:
                    pAlignCapsInfo->u8BaseAlign = caps.u8BaseAlign;
                    pAlignCapsInfo->u8PitchAlign = caps.u8PitchAlign;
                    break;
                case E_GFX_FMT_CAP_MULTI_PIXEL:
                    pAlignCapsInfo->u8BaseAlign = caps.u8Non1pAlign;
                    pAlignCapsInfo->u8PitchAlign = caps.u8Non1pAlign;
                    if(FALSE == g_apiGFXLocal.pGeChipProperty->bSupportMultiPixel)
                    {
                        printf("[%s] not support multi-pixel mode\n",__FUNCTION__);
                        return GFX_INVALID_PARAMETERS;
                    }
                    break;
                default:
                    printf("[%s] not support GFX format capability case: %d\n",__FUNCTION__, pAlignCapsInfo->eFmtCapsType);
                    break;
            }
        }
        break;
        case E_GFX_CAP_TLB:
        {
            MS_BOOL *bSupportTLB;
            if(ret_size != sizeof(MS_BOOL))
            {
                printf("[%s] ERROR, invalid input structure size :%ld on MS_BOOL\n",__FUNCTION__,ret_size);
                return GFX_INVALID_PARAMETERS;
            }
            bSupportTLB = (MS_BOOL *)pRet;
            *bSupportTLB = g_apiGFXLocal.pGeChipProperty->bSupportTLBMode;
        }
        break;
        default:
            printf("[%s]not support GFX capability case: %d\n",__FUNCTION__, eCapType);
            return GFX_INVALID_PARAMETERS;
    }

    return GFX_SUCCESS;
}

GFX_Result MApi_GFX_GetDBGInfo_U02(void* pInstance, GFX_DbgInfo *dbg)
{
    GE_DbgInfo tGEDBG;

    APICheckU02();

    memset(&tGEDBG,0,sizeof(GE_DbgInfo));

    MDrv_GE_GetInfo(g_apiGFXLocal.g_pGEContext, &tGEDBG);

    memcpy(dbg, &tGEDBG, sizeof(GE_DbgInfo));

    return GFX_SUCCESS;
}

GFX_Result MApi_GFX_DrawLine_U02(void* pInstance, GFX_DrawLineInfo *pline)
{
    GFX_RgbColor color_s, color_e;
    MS_U32 u32data;
    MS_U16 u16Color0=0, u16Color1=0;

    GE_Point v0, v1;
    MS_U32 color, color2;
    MS_U32 flags = 0;

    APICheckU02();

    v0.x = pline->x1;
    v0.y = pline->y1;
    color_s = pline->colorRange.color_s;
    memcpy(&u32data, &color_s, 4);
    GFX_ConvertRGB2PCFmt(pline->fmt, (MS_U32*)&u32data, &u16Color0, &u16Color1);
    color = (u16Color1<<16) | u16Color0;

    v1.x = pline->x2;
    v1.y = pline->y2;
    color_e = pline->colorRange.color_e;
    memcpy(&u32data, &color_e, 4);
    GFX_ConvertRGB2PCFmt(pline->fmt, (MS_U32*)&u32data, &u16Color0, &u16Color1);
    color2 = (u16Color1<<16) | u16Color0;

    if (pline->flag & GFXLINE_FLAG_COLOR_GRADIENT)
    {
        flags |= E_GE_FLAG_LINE_GRADIENT;
    }

    return (GFX_Result) MDrv_GE_DrawLine(g_apiGFXLocal.g_pGEContext, &v0, &v1, color, color2, flags, pline->width);
}


GFX_Result MApi_GFX_RectFill_U02(void* pInstance, GFX_RectFillInfo *pfillblock)
{
    GFX_RgbColor color_s, color_e;
    MS_U16 u16Color0=0, u16Color1=0;
    GFX_BlinkData blinkData;

    GE_Rect rect;
    MS_U32  color, color2;
    MS_U32  flags = 0;

    APICheckU02();

    rect.x = pfillblock->dstBlock.x;
    rect.y = pfillblock->dstBlock.y;
    rect.width = pfillblock->dstBlock.width;
    rect.height = pfillblock->dstBlock.height;

    color_s = pfillblock->colorRange.color_s;
    if(pfillblock->fmt == GFX_FMT_1ABFGBG12355 || pfillblock->fmt == GFX_FMT_FABAFGBG2266)
    {
        if(pfillblock->fmt == GFX_FMT_1ABFGBG12355)
        {
            memcpy(&blinkData, &color_s, sizeof(GFX_BlinkData));
            u16Color0=      (0x1F & blinkData.background)<<3 |
                            ((0x1F & blinkData.foreground) << 11);
            u16Color1=      (0x7 & blinkData.Bits.Blink)<<3 |
                            ((0x3 & blinkData.Bits.Alpha) << 6) |
                            0xff00;
        }
        if(pfillblock->fmt == GFX_FMT_FABAFGBG2266)
        {
            memcpy(&blinkData, &color_s, sizeof(GFX_BlinkData));
            u16Color0=      ((0x3F & blinkData.background)<<2) |
                            (((0x3F & blinkData.foreground)<<2) << 8);
            u16Color1=      ((0x3 & blinkData.Bits3.Ba)<<6) |
                            (((0x3 & blinkData.Bits3.Fa)<<6) << 8);
        }
        color = (u16Color1<<16) | u16Color0;
    }
    else
    {
        memcpy(&color, &color_s, sizeof(color));
    }


    color_e = pfillblock->colorRange.color_e;
    if(pfillblock->fmt == GFX_FMT_1ABFGBG12355 || pfillblock->fmt == GFX_FMT_FABAFGBG2266)
    {
        if(pfillblock->fmt == GFX_FMT_1ABFGBG12355)
        {
            memcpy(&blinkData, &color_e, sizeof(GFX_BlinkData));
            u16Color0=      (0x1F & blinkData.background)<<3 |
                            ((0x1F & blinkData.foreground) << 11);
            u16Color1=      (0x7 & blinkData.Bits.Blink)<<3 |
                            ((0x3 & blinkData.Bits.Alpha) << 6) |
                            0xff00;
        }
        if(pfillblock->fmt == GFX_FMT_FABAFGBG2266)
        {
            memcpy(&blinkData, &color_e, sizeof(GFX_BlinkData));
            u16Color0=      ((0x3F & blinkData.background)<<2) |
                            (((0x3F & blinkData.foreground)<<2) << 8);
            u16Color1=      ((0x3 & blinkData.Bits3.Ba)<<6) |
                            (((0x3 & blinkData.Bits3.Fa)<<6) << 8);
        }
        color2 = (u16Color1<<16) | u16Color0;

    }
    else
    {
        memcpy(&color2, &color_e, sizeof(color));
    }

    flags |= (pfillblock->flag & GFXRECT_FLAG_COLOR_GRADIENT_X) ? E_GE_FLAG_RECT_GRADIENT_X : 0;
    flags |= (pfillblock->flag & GFXRECT_FLAG_COLOR_GRADIENT_Y) ? E_GE_FLAG_RECT_GRADIENT_Y : 0;

    return (GFX_Result) MDrv_GE_FillRect(g_apiGFXLocal.g_pGEContext, &rect, color, color2, flags);
}

GFX_Result MApi_GFX_BitBlt_U02(void* pInstance, GFX_DrawRect * drawbuf, MS_U32 drawflag, GFX_ScaleInfo * ScaleInfo)
{
    union
    {
        GFX_Block drawbufblk;
        GE_Rect blk;
        GE_DstBitBltType dstblk;
        GFX_Trapezoid  dsttrapeblk;
    }sBltSrcBlk, sBltDstBlk;


    GE_ScaleInfo gecaleInfo;
    MS_U32 flags = 0;

    APICheckU02();

    if(drawflag & GFXDRAW_FLAG_SCALE)
    {
        flags |= E_GE_FLAG_BLT_STRETCH;
    }

    if(drawflag & GFXDRAW_FLAG_TRAPEZOID_X)
    {
        flags |= E_GE_FLAG_TRAPEZOID_X;
    }

    if(drawflag & GFXDRAW_FLAG_TRAPEZOID_Y)
    {
        flags |= E_GE_FLAG_TRAPEZOID_Y;
    }

    flags |= GFX_RectBltFlags();

    if(NULL != ScaleInfo)
    {
        gecaleInfo.init_x = ScaleInfo->u32InitDelatX;
        gecaleInfo.init_y = ScaleInfo->u32InitDelatY;
        gecaleInfo.x = ScaleInfo->u32DeltaX;
        gecaleInfo.y = ScaleInfo->u32DeltaY;

        flags |= E_GE_FLAG_BYPASS_STBCOEF;
    }
    else  //Coverity by sam
    {
        gecaleInfo.init_x = 0;
        gecaleInfo.init_y = 0;
        gecaleInfo.x = 0;
        gecaleInfo.y = 0;
    }
#if 1    // enable overlap
    if ( !(flags & (
                     E_GE_FLAG_BLT_STRETCH     |
                     E_GE_FLAG_BLT_ITALIC      |
                     E_GE_FLAG_BLT_MIRROR_H    |
                     E_GE_FLAG_BLT_MIRROR_V    |
                     E_GE_FLAG_BLT_ROTATE_90   |
                     E_GE_FLAG_BLT_ROTATE_180  |
                     E_GE_FLAG_BLT_ROTATE_270  )) )
    {
        flags |= E_GE_FLAG_BLT_OVERLAP;
    }
#endif
    sBltSrcBlk.drawbufblk = drawbuf->srcblk;
    sBltDstBlk.dsttrapeblk = drawbuf->dsttrapeblk;
    return (GFX_Result) MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext, &sBltSrcBlk.blk, &sBltDstBlk.dstblk, flags, (ScaleInfo==NULL)? NULL: &gecaleInfo);
}

//-------------------------------------------------------------------------------------------------
/// Set GFX destination buffer info in two source buffer mode
/// Attention, when two source buffer mode enable, raw DstBuffer will be one of two source.
/// @param dstbufInfo   \b IN: pointer to destination frame buffer infomation.\n
/// @param pdrawbuf \b IN: pointer to two source rects info
/// @param drawflag \b IN: draw flag \n
///                  GFXDRAW_FLAG_DEFAULT \n
///                  GFXDRAW_FLAG_SCALE \n
///                  GFXDRAW_FLAG_DUPLICAPE \n
///                  GFXDRAW_FLAG_TRAPEZOID \n
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_BitbltByTwoSourceBuffer_U02(void* pInstance, PGFX_BufferInfo dstbufInfo, GFX_DrawTwoSourceRect *pdrawbuf, MS_U32 drawflag)
{
    GE_Rect bitbltSrc0Rect;
    GE_Rect bitbltSrc1Rect;
    GE_Rect bitbltDestRect;
    MS_U32 flags = 0;
    APICheckU02();

    if(NULL == dstbufInfo || NULL == pdrawbuf)
    {
        return GFX_INVALID_PARAMETERS;
    }

    if (drawflag & GFXDRAW_FLAG_SCALE)
    {
        if(pdrawbuf->srcblk0.width != pdrawbuf->dstblk.width &&
            pdrawbuf->srcblk0.height != pdrawbuf->dstblk.height)
        {
            flags |= E_GE_FLAG_BLT_STRETCH;
        }
    }
    if(drawflag & GFXDRAW_FLAG_TRAPEZOID_X)
    {
        flags |= E_GE_FLAG_TRAPEZOID_X;
    }
    if(drawflag & GFXDRAW_FLAG_TRAPEZOID_Y)
    {
        flags |= E_GE_FLAG_TRAPEZOID_Y;
    }
    flags |= GFX_RectBltFlags();
#if 1    // enable overlap
    if ( !(flags & (
                     E_GE_FLAG_BLT_STRETCH     |
                     E_GE_FLAG_BLT_ITALIC      |
                     E_GE_FLAG_BLT_MIRROR_H    |
                     E_GE_FLAG_BLT_MIRROR_V    |
                     E_GE_FLAG_BLT_ROTATE_90   |
                     E_GE_FLAG_BLT_ROTATE_180  |
                     E_GE_FLAG_BLT_ROTATE_270  )) )
    {
        flags |= E_GE_FLAG_BLT_OVERLAP;
    }
#endif
    bitbltSrc0Rect.x = pdrawbuf->srcblk0.x;
    bitbltSrc0Rect.y = pdrawbuf->srcblk0.y;
    bitbltSrc0Rect.width = pdrawbuf->srcblk0.width;
    bitbltSrc0Rect.height = pdrawbuf->srcblk0.height;
    bitbltSrc1Rect.x = pdrawbuf->srcblk1.x;
    bitbltSrc1Rect.y = pdrawbuf->srcblk1.y;
    bitbltSrc1Rect.width = pdrawbuf->dstblk.width; //srcbuf1 width must be the same as dest width
    bitbltSrc1Rect.height = pdrawbuf->dstblk.height; //srcbuf1 height must be the same as dest height
    bitbltDestRect.x = pdrawbuf->dstblk.x;
    bitbltDestRect.y = pdrawbuf->dstblk.y;
    bitbltDestRect.width = pdrawbuf->dstblk.width;
    bitbltDestRect.height = pdrawbuf->dstblk.height;
    return (GFX_Result) MDrv_GE_BitBltByTwoSourceBuffer(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)dstbufInfo->u32ColorFmt,
        &bitbltSrc0Rect, &bitbltSrc1Rect, &bitbltDestRect, dstbufInfo->u32Addr, dstbufInfo->u32Pitch, flags, NULL);
}

GFX_Result MApi_GFX_SetSrcColorKey_U02(void* pInstance, MS_BOOL enable,
                                   GFX_ColorKeyMode opMode,
                                   GFX_Buffer_Format fmt,
                                   void *ps_color,
                                   void *pe_color)
{
    MS_U32 ck_low, ck_high, u32op1=0, ret;
    MS_U16 u16Color0=0, u16Color1=0;

    APICheckU02();

    GFX_ConvertRGB2DBFmt(fmt, (MS_U32 *)ps_color, &u16Color0, &u16Color1);
    ck_low = (u16Color1<<16) | u16Color0;
    GFX_ConvertRGB2DBFmt(fmt, (MS_U32 *)pe_color, &u16Color0, &u16Color1);
    ck_high = (u16Color1<<16) | u16Color0;

    ret = MApi_GFX_MapCKOP(opMode, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));


    return (GFX_Result) MDrv_GE_SetSrcColorKey(g_apiGFXLocal.g_pGEContext, enable, (GE_CKOp)u32op1, ck_low, ck_high);
}

GFX_Result MApi_GFX_SetDstColorKey_U02(void* pInstance, MS_BOOL enable,
                                   GFX_ColorKeyMode opMode,
                                   GFX_Buffer_Format fmt,
                                   void *ps_color,
                                   void *pe_color)
{
    MS_U32 ck_low, ck_high, u32op1=0, ret;
    MS_U16 u16Color0=0, u16Color1=0;

    APICheckU02();

    GFX_ConvertRGB2DBFmt(fmt, (MS_U32 *)ps_color, &u16Color0, &u16Color1);
    ck_low = (u16Color1<<16) | u16Color0;
    GFX_ConvertRGB2DBFmt(fmt, (MS_U32 *)pe_color, &u16Color0, &u16Color1);
    ck_high = (u16Color1<<16) | u16Color0;


    ret = MApi_GFX_MapCKOP(opMode, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    return (GFX_Result) MDrv_GE_SetDstColorKey(g_apiGFXLocal.g_pGEContext, enable, (GE_CKOp)u32op1, ck_low, ck_high);
}


GFX_Result MApi_GFX_SetROP2_U02(void* pInstance, MS_BOOL enable, GFX_ROP2_Op eRopMode)
{
    MS_U32 u32ROPmode=0, ret;

    APICheckU02();

    ret = MApi_GFX_MapROP2( eRopMode, &u32ROPmode );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));
    return (GFX_Result) MDrv_GE_SetROP2(g_apiGFXLocal.g_pGEContext, enable, (GE_Rop2) u32ROPmode);
}

GFX_Result MApi_GFX_SetIntensity_U02(void* pInstance, MS_U32 id, GFX_Buffer_Format fmt, MS_U32 *pColor)
{
    MS_U16 u16Color0=0, u16Color1=0;

    APICheckU02();

    GFX_ConvertRGB2DBFmt(fmt, (MS_U32 *)pColor, &u16Color0, &u16Color1);

    return (GFX_Result) MDrv_GE_SetIntensity(g_apiGFXLocal.g_pGEContext, id, (u16Color1<<16)|u16Color0);
}

GFX_Result MApi_GFX_SetDFBBldOP_U02(void* pInstance, GFX_DFBBldOP gfxSrcBldOP, GFX_DFBBldOP gfxDstBldOP)
{
    GE_DFBBldOP geSrcBldOP, geDstBldOP;
    MS_U32 u32Ret;

    APICheckU02();

    u32Ret = MApi_GFX_MapDFBBldOP(gfxSrcBldOP, &geSrcBldOP);
    if(u32Ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, u32Ret));

    u32Ret = MApi_GFX_MapDFBBldOP(gfxDstBldOP, &geDstBldOP);
    if(u32Ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, u32Ret));

    return (GFX_Result) MDrv_GE_SetDFBBldOP(g_apiGFXLocal.g_pGEContext, geSrcBldOP, geDstBldOP);
}

GFX_Result MApi_GFX_SetDFBBldConstColor_U02(void* pInstance, GFX_RgbColor gfxRgbColor)
{
    GE_RgbColor geRgbColor;

    APICheckU02();

    geRgbColor.a = gfxRgbColor.a;
    geRgbColor.r = gfxRgbColor.r;
    geRgbColor.g = gfxRgbColor.g;
    geRgbColor.b = gfxRgbColor.b;

    return (GFX_Result) MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);
}

GFX_Result MApi_GFX_SetDFBBldFlags_U02(void* pInstance, MS_U16 u16DFBBldFlags)
{
    MS_U16 u16DrvDFBBldFlags;
    MS_U32 u32Ret;

    APICheckU02();

    u32Ret = MApi_GFX_MapDFBBldFlag(u16DFBBldFlags, &u16DrvDFBBldFlags);
    if(u32Ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, u32Ret));

    return (GFX_Result) MDrv_GE_SetDFBBldFlags(g_apiGFXLocal.g_pGEContext, u16DrvDFBBldFlags);
}

GFX_Result MApi_GFX_SetClip_U02(void* pInstance, GFX_Point* v0, GFX_Point* v1)
{
    GE_Rect rect;

    APICheckU02();

    rect.x = v0->x; // dangerous if V0 > V1
    rect.y = v0->y;
    rect.width = v1->x - v0->x + 1;
    rect.height = v1->y - v0->y + 1;

    return (GFX_Result) MDrv_GE_SetClipWindow(g_apiGFXLocal.g_pGEContext, &rect);
}

GFX_Result MApi_GFX_SetPaletteOpt_U02( void* pInstance, GFX_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd)
{
    MS_U16 i;
    //----------------------------------------------------------------------
    // Write palette
    //----------------------------------------------------------------------
    //U32 clr;
    MS_U16 j=0;
    MS_U32 u32data;

    APICheckU02();

    for(i=u32PalStart; i<=u32PalEnd ; i++)
    {

       GFX_DEBUGINFO(printf("MDrv_GE_SetPaletteOpt :  Array[%03d]. u8A %02bX | u8R %02bX | u8G %02bX |u8B %02bX\n",i,
                                                pPalArray[j].RGB.u8A,
                                                pPalArray[j].RGB.u8R,
                                                pPalArray[j].RGB.u8G,
                                                pPalArray[j].RGB.u8B));

        memcpy(&u32data, &pPalArray[j], 4);
        MDrv_GE_SetPalette(g_apiGFXLocal.g_pGEContext, i, 1, (MS_U32*)&u32data);

        j++;

    }
    return GFX_SUCCESS;

}

GFX_Result MApi_GFX_SetVCmdBuffer_U02(void* pInstance, MS_PHYADDR u32Addr, GFX_VcmqBufSize enBufSize)
{
    MS_U32 u32op1=0, ret;

    APICheckU02();

    ret = MApi_GFX_MapVCMQSZ(enBufSize, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    if(E_GE_OK != MDrv_GE_SetVCmdBuffer(g_apiGFXLocal.g_pGEContext,  u32Addr,  (GE_VcmqBufSize)u32op1))
    {
        return GFX_FAIL;
    }

    return GFX_SUCCESS;
}

GFX_Result MApi_GFX_EnableAlphaBlending_U02(void* pInstance, MS_BOOL enable)
{
    MS_U32 u32op1 =0, ret;
    APICheckU02();

    ret = MApi_GFX_MapBLDCOEF(g_apiGFXLocal.pABLInfo.eBldCoef, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    return (GFX_Result) MDrv_GE_SetAlphaBlend(g_apiGFXLocal.g_pGEContext, enable, (GE_BlendOp)u32op1);
}

GFX_Result MApi_GFX_SetAlpha_U02(void* pInstance, MS_BOOL enable, GFX_BlendCoef coef, GFX_AlphaSrcFrom db_abl, MS_U8 abl_const)
{
    MS_U32 u32op1=0, u32op2=0, ret;
    APICheckU02();

    ret = MApi_GFX_MapBLDCOEF(coef, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    ret = MApi_GFX_MapABLCOEF(db_abl, &u32op2 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    MDrv_GE_SetAlphaConst(g_apiGFXLocal.g_pGEContext, abl_const);
    MDrv_GE_SetAlphaSrc(g_apiGFXLocal.g_pGEContext, (GE_AlphaSrc)u32op2);
    MDrv_GE_SetAlphaBlend(g_apiGFXLocal.g_pGEContext, enable, (GE_BlendOp)u32op1);

    return GFX_SUCCESS;
}


GFX_Result MApi_GFX_DrawBitmap_U02(void* pInstance, MS_S32 handle, GFX_DrawBmpInfo *pbmpfmt)
{
    MS_U32          (*fpGFX_CBGetBMP)(MS_S16, GFX_BitmapInfo*)=NULL;

    APICheckU02();

    GFX_DEBUGINFO( printf( "MApi_GFX_DrawBitmap\n" ) );
    //OSD_RESOURCE_BITMAP_INFO osd_bmpinfo;
    GFX_BitmapInfo bmpinfo;
    //msAPI_OSD_RESOURCE_GetBitmapInfo(handle, &osd_bmpinfo);
    if(g_apiGFXLocal.fpGetBMP != NULL)
    {
        fpGFX_CBGetBMP = (void*)(g_apiGFXLocal.fpGetBMP);
        fpGFX_CBGetBMP(handle, &bmpinfo);
    }
    else
        return GFX_INVALID_BMP_HANDLE;
#if 0
    bmpinfo.addr = osd_bmpinfo.addr;
    bmpinfo.inUsed  = osd_bmpinfo.inUsed;
    bmpinfo.width = osd_bmpinfo.width;
    bmpinfo.pitch = osd_bmpinfo.pitch;
    bmpinfo.fmt = osd_bmpinfo.fmt;
    bmpinfo.height = osd_bmpinfo.height;
#endif
    if (!bmpinfo.inUsed)
    {
        return GFX_INVALID_BMP_HANDLE;
    }

    if (!GFX_CheckInClipWindow(pbmpfmt->x, pbmpfmt->y, pbmpfmt->x+pbmpfmt->width-1, pbmpfmt->y+pbmpfmt->height-1))
    {
        return GFX_FAIL;
    }

#if 1
{
    MS_U32      flags = 0;
    GE_Rect     src;
    union
    {
        GE_Rect dstblk;
        GE_DstBitBltType drvdstblk;
    }dst;
    MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)bmpinfo.fmt,
                         bmpinfo.width,
                         bmpinfo.height,
                         bmpinfo.addr,
                         bmpinfo.pitch, 0);
    src.x = 0;
    src.y = 0;
    src.width = bmpinfo.width;
    src.height = bmpinfo.height;
    dst.dstblk.x = pbmpfmt->x;
    dst.dstblk.y = pbmpfmt->y;
    dst.dstblk.width = pbmpfmt->width;
    dst.dstblk.height = pbmpfmt->height;
    if (pbmpfmt->bScale == true)
    {
        flags |= E_GE_FLAG_BLT_STRETCH;
    }

    flags |= GFX_RectBltFlags();

    MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext, &src, &dst.drvdstblk, flags, NULL);
}
#else
/*
    MS_U32 u32Width, u32Height;
    MS_U32 u32Value, u32Value2, u32Addr, u32Pitch;

    GE_WaitCmdQAvail(24);

    u32Value = PE_ReadReg(PE_REG_SB_DB_MODE);
    u32Value &= ~(PE_MSK_SB_FMT);

    PE_WriteReg(PE_REG_SB_DB_MODE, (u32Value|_BitmapTable[handle].fmt));

    u32Value2 = PE_ReadReg(PE_REG_FMT_BLT);

    u32Addr   = (_BitmapTable[handle].addr ) ;
    u32Width  = _BitmapTable[handle].width;
    u32Height = _BitmapTable[handle].height;
    u32Pitch  = _BitmapTable[handle].pitch;

    // Set source address
    PE_WriteReg(PE_REG_SB_BASE0, u32Addr & 0xffff);
    PE_WriteReg(PE_REG_SB_BASE1, u32Addr >> 16);

    // Set source pitch
    PE_WriteReg(PE_REG_SB_PIT, u32Pitch);

    PE_WriteReg(PE_REG_STBB_WIDTH, u32Width);
    PE_WriteReg(PE_REG_STBB_HEIGHT, u32Height);

    // Set source coordinate
    PE_WriteReg(PE_REG_PRI_V2_X, 0);
    PE_WriteReg(PE_REG_PRI_V2_Y, 0);

    //------------------------------------------------------------
    // BLT scale delta value
    //------------------------------------------------------------
    if ((u32Width != pbmpfmt->width) || (u32Height != pbmpfmt->height))
    {
        if (pbmpfmt->bScale == true)
        {
            u32Value = Divide2Fixed(u32Width, pbmpfmt->width, 1, 12);//<< 2 ; //sc
            PE_WriteReg(PE_REG_STBB_DX, u32Value & 0xffff);
            u32Value = Divide2Fixed((u32Width - pbmpfmt->width), 2* pbmpfmt->width, 1, 12);// << 2; //sc
            PE_WriteReg(PE_REG_STBB_INIT_DX, u32Value);


            u32Value = Divide2Fixed(u32Height, pbmpfmt->height, 1, 12);// << 2 ; //sc
            PE_WriteReg(PE_REG_STBB_DY, u32Value & 0xffff);
            u32Value = Divide2Fixed((u32Height- pbmpfmt->height), 2* pbmpfmt->height, 1, 12);// << 2; //sc
            PE_WriteReg(PE_REG_STBB_INIT_DY, u32Value);

            u32Width = pbmpfmt->width;
            u32Height = pbmpfmt->height;
            u32Value2 |= PE_VAL_EN_STRETCH_BITBLT;
        }
        else
        {
            u32Width = pbmpfmt->width;
            u32Height = pbmpfmt->height;
            PE_WriteReg(PE_REG_STBB_DX, 0x1000);
            PE_WriteReg(PE_REG_STBB_DY, 0x1000);
            PE_WriteReg(PE_REG_STBB_INIT_DX, 0);
            PE_WriteReg(PE_REG_STBB_INIT_DY, 0);
            u32Value2 |= PE_VAL_EN_STRETCH_BITBLT;
        }
    }
    else
    {
        PE_WriteReg(PE_REG_STBB_DX, 0x1000);
        PE_WriteReg(PE_REG_STBB_DY, 0x1000);
        u32Value2 &= ~PE_VAL_EN_STRETCH_BITBLT;
    }
    PE_WriteReg(PE_REG_FMT_BLT, u32Value2);

    if (u32Value2 & PE_VAL_EN_STRETCH_BITBLT)
    {
        _u32Reg60hFlag |= PE_VAL_STBB_PATCH;
    }

    //------------------------------------------------------------
    // Destination coordinate
    //------------------------------------------------------------
    PE_WriteReg(PE_REG_PRI_V0_X, pbmpfmt->x);
    PE_WriteReg(PE_REG_PRI_V0_Y, pbmpfmt->y);
    PE_WriteReg(PE_REG_PRI_V1_X, pbmpfmt->x + u32Width - 1);
    PE_WriteReg(PE_REG_PRI_V1_Y, pbmpfmt->y + u32Height - 1);

    if (_u32Reg60hFlag & PE_VAL_DRAW_SRC_DIR_X_NEG)
    {
        PE_WriteReg(PE_REG_PRI_V2_X, _BitmapTable[handle].width - 1);
    }

    if (_u32Reg60hFlag & PE_VAL_DRAW_SRC_DIR_Y_NEG)
    {
        PE_WriteReg(PE_REG_PRI_V2_Y, _BitmapTable[handle].height - 1);
    }

    PE_WriteReg(PE_REG_CMD, (PE_VAL_PRIM_BITBLT|_u32Reg60hFlag));
*/
#endif

    return GFX_SUCCESS;
}

static GFX_Result GFX_TextOutEx(MS_S32 fhandle,
                             MS_U8 *pindex,
                             MS_U32 strwidth,
                             GFX_TextOutInfo *pfmt)
{
    GFX_DEBUGINFO(printf( "MApi_GFX_TextOut\n" ));

    MS_U32          (*fpGFX_CBGetFont)(MS_S8, GFX_FontInfo*)=NULL;
    MS_U32              flags = 0, offset;
    MS_PHYADDR          addr;
    MS_U16              char_idx;
    MS_U32              pitch;
    MS_S32              dis;
    GE_Rect             src;
    GE_BufFmt           fmt;
    MS_PHYADDR              ptr_addr;
    union
    {
        GE_Rect dstblk;
        GE_DstBitBltType drvdstblk;
    }dst;
   // OSD_RESOURCE_FONT_INFO osd_fonttable;
    GFX_FontInfo fonttable;

    if(g_apiGFXLocal.fpGetFont != NULL)
    {
        fpGFX_CBGetFont = (void*)g_apiGFXLocal.fpGetFont;
        fpGFX_CBGetFont(fhandle, &fonttable);
    }
    else
        return GFX_INVALID_FONT_HANDLE;

//        msAPI_OSD_RESOURCE_GetFontInfo(fhandle, &osd_fonttable);
//    printf("%d, %02x:%02x:%02x:%02x:%02x:%02x\n", strwidth, pindex[0], pindex[1], pindex[2], pindex[3], pindex[4], pindex[5]);
#if 0
    fonttable.addr = osd_fonttable.glyph_addr;
    fonttable.fmt = osd_fonttable.fmt;
    fonttable.height = osd_fonttable.height;
    fonttable.len = osd_fonttable.char_glyph_bytes;
    fonttable.offset = osd_fonttable.char_glyph_bytes;
    fonttable.pBBox = (GFX_GlyphBbox*)osd_fonttable.bbox_addr;
    fonttable.pitch = osd_fonttable.pitch;
    fonttable.width = osd_fonttable.width;
    fonttable.inUsed = osd_fonttable.inUsed;
#endif

    if (!fonttable.inUsed)
    {
        return GFX_INVALID_FONT_HANDLE;
    }

    if (pindex == NULL)
    {
        return GFX_INVALID_PARAMETERS;
    }

    if (!GFX_CheckInClipWindow(pfmt->dstblk.x, pfmt->dstblk.y,
                              pfmt->dstblk.x+pfmt->dstblk.width-1, pfmt->dstblk.y+pfmt->dstblk.height-1))
    {
        return GFX_FAIL;
    }

    if(pfmt->flag & GFXFONT_FLAG_COMPACT)
    {
        dis = pfmt->dis;
    }
    else if (pfmt->flag & GFXFONT_FLAG_GAP)
    {
        dis = (0-pfmt->dis);
    }
    else
    {
        dis = 0;
    }

    offset = fonttable.offset;
    addr = fonttable.addr;
    fmt = (GE_BufFmt)fonttable.fmt;
    pitch = fonttable.pitch;

    // first font character
    if (strwidth == 2)
    {
        ptr_addr = (MS_PHYADDR)pindex;
        if(ptr_addr % 2 != 0)
        {
            GFX_DEBUGINFO(printf("%s: %d\n", __FUNCTION__ , __LINE__);)
            return GFX_INVALID_PARAMETERS;
        }
        char_idx = *(MS_U16*)ptr_addr;
        pindex += 2;
    }
    else
    {
        char_idx = *(MS_U8*)pindex;
        pindex += 1;
    }
    src.x = 0;
    src.y = 0;
    src.width = fonttable.width;
    src.height = fonttable.height;
    dst.dstblk.x = pfmt->dstblk.x;
    dst.dstblk.y = pfmt->dstblk.y;
    if (pfmt->flag & GFXFONT_FLAG_SCALE)
    {
        dst.dstblk.width = pfmt->dstblk.width;
        dst.dstblk.height = pfmt->dstblk.height;
        flags |= E_GE_FLAG_BLT_STRETCH;
    }
    else
    {
        //NOTE: it's because some legacy code does not specify dstblk.width/height
        dst.dstblk.width = fonttable.width;
        dst.dstblk.height = fonttable.height;
    }

    while (char_idx != '\0')
    {
        MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, fmt, src.width, src.height, (addr + char_idx*offset), pitch, 0);

        flags |= GFX_RectBltFlags();
        MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext,&src, &dst.drvdstblk, flags, NULL);

        if (strwidth == 2)
        {
            ptr_addr = (MS_PHYADDR)pindex;
            if(ptr_addr % 2 != 0)
            {
                GFX_DEBUGINFO(printf("%s: %d\n", __FUNCTION__ , __LINE__);)
                return GFX_INVALID_PARAMETERS;
            }
            char_idx = *(MS_U16*)ptr_addr;
            pindex += 2;
        }
        else
        {
            char_idx = *(MS_U8*)pindex;
            pindex += 1;
        }
        dst.dstblk.x += dst.dstblk.width - dis; // next position
    }
    return GFX_SUCCESS;
}

GFX_Result MApi_GFX_TextOut_U02(void* pInstance, MS_S32 fhandle, MS_U8 *pindex, MS_U32 strwidth, GFX_TextOutInfo *pfmt)
{
    MS_U32          (*fpGFX_CBGetFont)(MS_S8, GFX_FontInfo*)=NULL;

    GFX_DEBUGINFO(printf( "MApi_GFX_TextOut\n" ));
//    OSD_RESOURCE_FONT_INFO osd_fonttable;
    GFX_FontInfo fonttable;
    GFX_GlyphDispInfo *pGlyInfo = pfmt->pGlyphDispInfo;
    MS_U16 glyph_index, char_code;
    //msAPI_OSD_RESOURCE_GetFontInfo(fhandle, &osd_fonttable);
    GFX_RgbColor tmpclr1, tmpclr2;
    MS_PHYADDR ptr_addr;
    MS_U16* ptr_u16;
    MS_BOOL bTextOutPatch;
//    GFX_Result Gfx_Ret = GFX_SUCCESS;

    APICheckU02();

    if(g_apiGFXLocal.fpGetFont != NULL)
    {
        fpGFX_CBGetFont = (void*)(g_apiGFXLocal.fpGetFont);
        fpGFX_CBGetFont(fhandle, &fonttable);
    }
    else
        return GFX_INVALID_FONT_HANDLE;
#if 0
    fonttable.addr = osd_fonttable.glyph_addr;
    fonttable.fmt = osd_fonttable.fmt;
    fonttable.height = osd_fonttable.height;
    fonttable.len = osd_fonttable.char_glyph_bytes;
    fonttable.offset = osd_fonttable.char_glyph_bytes;
    fonttable.pBBox = (GFX_GlyphBbox*)osd_fonttable.bbox_addr;
    fonttable.pitch = osd_fonttable.pitch;
    fonttable.width = osd_fonttable.width;
    fonttable.inUsed = osd_fonttable.inUsed;
#endif
    bTextOutPatch = _GE_TextOutPatch(g_apiGFXLocal.g_pGEContext);

    if(bTextOutPatch == TRUE)
    {
        //This is for A6 U02 alpha constant HW bug, when alpha value = 0x0 in  alpha constant mode.It desnt work.
        //So we dont use alpha constant mode when color.a = 0x0
        if(pfmt->color.a != 0x0)
            MApi_GFX_SetAlpha_U02(pInstance, true, COEF_ASRC, ABL_FROM_CONST, pfmt->color.a);
    }
    else
    {
        MApi_GFX_SetAlpha_U02(pInstance, true, COEF_ASRC, ABL_FROM_CONST, pfmt->color.a);
        pfmt->color.a = 0xff;
    }
    // Forground color
    if (fonttable.fmt == GFX_FMT_I2)
    {
        pfmt->color.a?(pfmt->color.a = 0xff) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 3, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0xaa) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 2, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x55) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 1, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));

    }
    else if (fonttable.fmt == GFX_FMT_I4)
    {
        pfmt->color.a?(pfmt->color.a = 0xff) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 15, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0xee) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 14, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0xdd) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 13, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0xcc) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 12, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0xbb) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 11, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0xaa) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 10, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x99) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 9, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x88) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 8, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x77) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 7, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x66) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 6, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x55) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 5, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x44) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 4, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x33) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 3, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x22) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 2, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
        pfmt->color.a?(pfmt->color.a = 0x11) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 1, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
    }
    else
    {
        pfmt->color.a?(pfmt->color.a = 0xff) :(pfmt->color.a = 0x0);
        MApi_GFX_SetIntensity_U02(pInstance, 1, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));
    }
#if PATCH_T3_SCK_FAIL
        pfmt->color.a = 0xff; //[2009.07.02]T3/U3 GE HW issue:set source alpha is max for filtering the background color
        pfmt->color.r = pfmt->color.g = pfmt->color.b = 0x01;
#else
    pfmt->color.a = pfmt->color.r = pfmt->color.g = pfmt->color.b = 0x01;
#endif
    MApi_GFX_SetIntensity_U02(pInstance, 0, (GFX_Buffer_Format)GFX_FMT_ARGB8888, (MS_U32*)&(pfmt->u32data));

    // Color key
    tmpclr1.a=tmpclr1.r=tmpclr1.g=tmpclr1.b=0;
    tmpclr2.a = 0x01;
    tmpclr2.r = 0x01;
    tmpclr2.g = 0x01;
    tmpclr2.b = 0x01;

    //MApi_GFX_SetDither(FALSE);   // FIXME. Not need to be set here. Need to find root cause.

    MApi_GFX_SetSrcColorKey_U02(pInstance, TRUE, CK_OP_EQUAL, GFX_FMT_ARGB8888, &tmpclr1, &tmpclr2);


    if ((pfmt->flag & GFXFONT_FLAG_VARWIDTH) &&
        (fonttable.pBBox!= NULL))
    {
        MS_U32 i;
        MS_U32 u32PosX = 0;
        MS_U32 u32PosY = 0;
        MS_U32 u32Width = 0;
        MS_U32 u32Height = 0;
        MS_S8 dis;
        MS_U16 gap = 0;
        MS_U32 start = 0;
        GFX_FontInfo* pFont = &fonttable;

        GFX_CharInfo  Char;
        GFX_TextOutInfo fmt;
        MS_S32 spoint_overlay_x = 0;
        MS_S32 spoint_overlay_y = 0;

        if (!pFont->inUsed)
        {
            return GFX_INVALID_FONT_HANDLE;
        }
        if (pindex == NULL)
        {
            return GFX_INVALID_PARAMETERS;
        }

        if(pfmt->flag & GFXFONT_FLAG_COMPACT)
        {
            dis = pfmt->dis;
        }
        else if (pfmt->flag & GFXFONT_FLAG_GAP)
        {
            gap = pfmt->gap;
            dis = 0;
        }
        else
        {
            dis = 0;
        }


        memcpy((void*)&fmt, pfmt, sizeof(GFX_TextOutInfo));
        Char.fmt = pFont->fmt;
        Char.Voffset = 0;
        Char.height = pFont->height;
        Char.pitch = pFont->pitch;

        i = 0;

        //pu16TmpIndex =(MS_U16 *)pindex;
        //while (*pu16TmpIndex != '\0')
        for( i = 0; i < 250; i++)
        {


            if (strwidth == 2)
            {
                ptr_addr = (MS_PHYADDR)pindex;
                if(ptr_addr % 2 != 0)
                {
                    GFX_DEBUGINFO(printf("%s: %d\n", __FUNCTION__ , __LINE__);)
                    MApi_GFX_SetSrcColorKey_U02(pInstance, FALSE, CK_OP_EQUAL, GFX_FMT_ARGB8888, &tmpclr1, &tmpclr2);
                    return GFX_INVALID_PARAMETERS;
                }
                ptr_u16 = (MS_U16*)ptr_addr;
                char_code = ((MS_U16*)ptr_u16)[i];
            }
            else
            {
                char_code = (MS_U8)pindex[i];
            }

            if(char_code == 0 )
                break;

            if (pfmt->pGlyphDispInfo == NULL)
            {
                glyph_index = char_code;
            }
            else
            {
                glyph_index = pGlyInfo[i].u16Index;
            }

            if (i == 0)
            {
                u32PosX = pfmt->dstblk.x;
                u32PosY = pfmt->dstblk.y;
                start = spoint_overlay_x = pfmt->dstblk.x;
                spoint_overlay_y = pfmt->dstblk.y;
            }

            // Move to here: workaround for unicode string end detection issue.
            if(pGlyInfo[i].u16Index == ERR_NULL_INDEX)
            {
                GFX_DEBUGINFO(printf("\npGlyphDispInfo[%d].u16Index error", i););
                continue;
            }



            if (fmt.flag & GFXFONT_FLAG_SCALE)
            {
                u32Width = (pGlyInfo[i].u8Width)*(pfmt->dstblk.width/pFont->width);
                u32Height = (pFont->height)*(pfmt->dstblk.height/pFont->height);
            }
            else
            {
                u32Width = pGlyInfo[i].u8Width;
                u32Height = pFont->height;
            }
            fmt.dstblk.x = u32PosX;
            fmt.dstblk.y = u32PosY;
            fmt.dstblk.width = u32Width;
            fmt.dstblk.height = u32Height;

            Char.addr = pFont->addr + (glyph_index*pFont->offset);
            Char.Hoffset = pGlyInfo[i].u8X0;
            Char.width = pGlyInfo[i].u8Width;

            if ((pGlyInfo[i].overlap_info.s8Xoffset!=0)||(pGlyInfo[i].overlap_info.s8Yoffset!=0))
            {
                Char.Hoffset = 0;//pGlyInfo[i].u8X0;
                Char.width = pFont->width;//pGlyInfo[i].u8Width;
                fmt.dstblk.width = pFont->width;
                fmt.dstblk.height = pFont->height;
            }

#if 1
{
            MS_U32  flags = 0;
            GE_Rect src;
            union
            {
                GE_Rect dstblk;
                GE_DstBitBltType drvdstblk;
            }dst;

            MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)Char.fmt, 0, 0, Char.addr, Char.pitch, 0);

            src.x = Char.Hoffset;
            src.y = Char.Voffset;
            src.width = Char.width;
            src.height = Char.height;
            if (pfmt->flag & GFXFONT_FLAG_SCALE)
            {
                flags |= E_GE_FLAG_BLT_STRETCH;
            }

            if (g_apiGFXLocal._bMirrorH)
            {
                src.x =  pGlyInfo[i].u8X0 + pGlyInfo[i].u8Width - 1;//Char.Hoffset;
            }
            dst.dstblk.x = fmt.dstblk.x;
            dst.dstblk.y = fmt.dstblk.y;
            dst.dstblk.width = fmt.dstblk.width;
            dst.dstblk.height = fmt.dstblk.height;

            flags |= GFX_RectBltFlags();

            MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext,&src, &dst.drvdstblk, flags, NULL);

            if (pfmt->flag & GFXFONT_FLAG_BOLD)
            {
                //left side
                dst.dstblk.x -=1;
                MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext,&src, &dst.drvdstblk, flags, NULL);

                //right side
                dst.dstblk.x +=2;
                MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext,&src, &dst.drvdstblk, flags, NULL);

                //up side
                dst.dstblk.y -=1;
                MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext,&src, &dst.drvdstblk, flags, NULL);

                //down side
                dst.dstblk.y +=2;
                MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext,&src, &dst.drvdstblk, flags, NULL);
            }

}
#else
            PE_CharacterOut(&Char, &fmt);
#endif


            if ((pGlyInfo[i+1].overlap_info.s8Xoffset==0)&&(pGlyInfo[i+1].overlap_info.s8Yoffset==0))

            {
                spoint_overlay_x = spoint_overlay_x +u32Width -dis +gap;
                  u32PosX = spoint_overlay_x ;

                  u32PosY = pfmt->dstblk.y;

                if (u32PosX <= start)
                {
                   spoint_overlay_x = u32PosX = start;
                }
            }
            else
            {
                u32PosX = spoint_overlay_x; //- dis + gap;
                u32PosX += pGlyInfo[i+1].overlap_info.s8Xoffset;
                u32PosY = spoint_overlay_y; //- dis + gap;
                u32PosY += pGlyInfo[i+1].overlap_info.s8Yoffset;
                start = u32PosX + pFont->width;
            }
        }
        MApi_GFX_SetSrcColorKey_U02(pInstance, FALSE, CK_OP_EQUAL, GFX_FMT_ARGB8888, &tmpclr1, &tmpclr2);

        return GFX_SUCCESS;
    }
    else
    {
        MS_U32 ret;
        ret = (MS_U32)GFX_TextOutEx(fhandle, pindex, strwidth, pfmt);
        MApi_GFX_SetSrcColorKey_U02(pInstance, FALSE, CK_OP_EQUAL, GFX_FMT_ARGB8888, &tmpclr1, &tmpclr2);
        return (GFX_Result)ret ;
    }

}

GFX_Result MApi_GFX_CharacterOut_U02(void* pInstance, GFX_CharInfo*  pChar, GFX_TextOutInfo *pfmt)
{
#if 1
{
                MS_U32  flags = 0;
                GE_Rect src;
                union
                {
                    GE_Rect dstblk;
                    GE_DstBitBltType drvdstblk;
                }dst;

                APICheckU02();

                MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)pChar->fmt, 0, 0, pChar->addr, pChar->pitch, 0);
                src.x = pChar->Hoffset;
                src.y = pChar->Voffset;
                src.width = pChar->width;
                src.height = pChar->height;
                if (pfmt->flag & GFXFONT_FLAG_SCALE)
                {
                    flags |= E_GE_FLAG_BLT_STRETCH;
                }
                dst.dstblk.x = pfmt->dstblk.x;
                dst.dstblk.y = pfmt->dstblk.y;
                dst.dstblk.width = pfmt->dstblk.width;
                dst.dstblk.height = pfmt->dstblk.height;

                flags |= GFX_RectBltFlags();
                return (GFX_Result) MDrv_GE_BitBltEX(g_apiGFXLocal.g_pGEContext, &src, &dst.drvdstblk, flags, NULL);
}
#else
    return PE_CharacterOut(pChar, pfmt);
#endif
}
GFX_Result MApi_GFX_ClearFrameBufferByWord_U02(void* pInstance,MS_PHYADDR StrAddr, MS_U32 length, MS_U32 ClearValue)
{
#define CLRbW_FB_WIDTH    1024
#define CLRbW_FB_PITCH    (CLRbW_FB_WIDTH*4)
#define CLRbW_FB_HEIGHT   128
#define CLRbW_FB_SIZE     CLRbW_FB_HEIGHT * CLRbW_FB_PITCH

    GE_Rect rect;
    MS_U32  color, color2;
    MS_PHYADDR tmpaddr;
    GE_BufInfo bufinfo;

    GE_Rect clip;
    MS_U32  flags = 0;

    APICheckU02();

    if((length % 4) != 0)
        return GFX_INVALID_PARAMETERS;

    MDrv_GE_GetBufferInfo(g_apiGFXLocal.g_pGEContext, &bufinfo);
    MDrv_GE_SetAlphaSrc(g_apiGFXLocal.g_pGEContext, E_GE_ALPHA_ASRC);
    MDrv_GE_SetAlphaBlend(g_apiGFXLocal.g_pGEContext, FALSE, E_GE_BLEND_ONE);
    clip.x = 0;
    clip.y = 0;
    clip.width = 1920;
    clip.height = 1080;
    MDrv_GE_SetClipWindow(g_apiGFXLocal.g_pGEContext, &clip);


    color = ClearValue;
    color2 = color;

    flags = 0;
    tmpaddr = StrAddr;

    while(length >= CLRbW_FB_PITCH)
    {
        rect.height = length/CLRbW_FB_PITCH;
        if(rect.height > 1080)
            rect.height = 1080;

        rect.x = 0;
        rect.y = 0;
        rect.width = CLRbW_FB_WIDTH;
//        rect.height = CLR_FB_HEIGHT;
        MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, E_GE_FMT_ARGB8888, CLRbW_FB_WIDTH, 1, tmpaddr, CLRbW_FB_PITCH, 0);
        MDrv_GE_FillRect(g_apiGFXLocal.g_pGEContext, &rect, color, color2, flags);
        tmpaddr += (CLRbW_FB_PITCH*rect.height);
        length -= (CLRbW_FB_PITCH*rect.height);
    }


    if( length > 0) {
        rect.x = 0;
        rect.y = 0;
        rect.width = length/4;
        rect.height = 1;
        MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, E_GE_FMT_ARGB8888, CLRbW_FB_WIDTH, 1, tmpaddr, CLRbW_FB_PITCH, 0);
        MDrv_GE_FillRect(g_apiGFXLocal.g_pGEContext, &rect, color, color2, flags);
        tmpaddr += length;
    }

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)bufinfo.dstfmt, 0, 0, bufinfo.dstaddr, bufinfo.dstpit, 0);
    //MDrv_GE_WaitIdle();

    return GFX_SUCCESS;

}
GFX_Result MApi_GFX_ClearFrameBuffer_U02(void* pInstance,MS_PHYADDR StrAddr, MS_U32 length, MS_U8 ClearValue)
{
#define CLR_FB_PITCH    1024
#define CLR_FB_HEIGHT   256
#define CLR_FB_SIZE     CLR_FB_HEIGHT * CLR_FB_PITCH

    GE_Rect rect;
    MS_U32  color, color2;
    MS_PHYADDR tmpaddr;
    GE_BufInfo bufinfo;
    GE_Rect clip;
    MS_U32  flags = 0;

    APICheckU02();

    MDrv_GE_GetBufferInfo(g_apiGFXLocal.g_pGEContext, &bufinfo);
    MDrv_GE_SetAlphaSrc(g_apiGFXLocal.g_pGEContext, E_GE_ALPHA_ASRC);
    MDrv_GE_SetAlphaBlend(g_apiGFXLocal.g_pGEContext, FALSE, E_GE_BLEND_ONE);

    clip.x = 0;
    clip.y = 0;
    clip.width = 1920;
    clip.height = 1080;
    MDrv_GE_SetClipWindow(g_apiGFXLocal.g_pGEContext, &clip);

    color = (ClearValue << 8) + ClearValue;
    color2 = color;

    flags = 0;
    tmpaddr = StrAddr;

    while(length >= CLR_FB_PITCH)
    {
        rect.height = length/CLR_FB_PITCH;
        if(rect.height > 1080)
            rect.height = 1080;

        rect.x = 0;
        rect.y = 0;
        rect.width = CLR_FB_PITCH;
//        rect.height = CLR_FB_HEIGHT;
        MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, E_GE_FMT_I8, CLR_FB_PITCH, 1, tmpaddr, CLR_FB_PITCH, 0);
        MDrv_GE_FillRect(g_apiGFXLocal.g_pGEContext, &rect, color, color2, flags);
        tmpaddr += (CLR_FB_PITCH*rect.height);
        length -= (CLR_FB_PITCH*rect.height);
    }

    if( length > 0) {
        rect.x = 0;
        rect.y = 0;
        rect.width = length;
        rect.height = 1;
        MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, E_GE_FMT_I8, CLR_FB_PITCH, 1, tmpaddr, CLR_FB_PITCH, 0);
        MDrv_GE_FillRect(g_apiGFXLocal.g_pGEContext, &rect, color, color2, flags);
        tmpaddr += length;
    }

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, (GE_BufFmt)bufinfo.dstfmt, 0, 0, bufinfo.dstaddr, bufinfo.dstpit, 0);
    //MDrv_GE_WaitIdle();

    return GFX_SUCCESS;
}
GFX_Result MApi_GFX_SetAlphaCmp_U02(void* pInstance,MS_BOOL enable, GFX_ACmpOp eMode)
{
    MS_U32 u32op1=0, ret;

    APICheckU02();

    ret = MApi_GFX_MapACmp(eMode, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    return (GFX_Result) MDrv_GE_SetAlphaCmp(g_apiGFXLocal.g_pGEContext, enable, (GE_ACmpOp)u32op1);
}

GFX_Result MApi_GFX_Set_Line_Pattern_U02(void* pInstance,MS_BOOL enable, MS_U8 linePattern, MS_U8 repeatFactor)
{
    APICheckU02();

    g_apiGFXLocal._line_enable  = enable;
    g_apiGFXLocal._line_pattern = linePattern;
    g_apiGFXLocal._line_factor  = repeatFactor;
    return (GFX_Result) MDrv_GE_SetLinePattern(g_apiGFXLocal.g_pGEContext,
                             g_apiGFXLocal._line_enable, g_apiGFXLocal._line_pattern,
                               (GE_LinePatRep)g_apiGFXLocal._line_factor);
}
GFX_Result MApi_GFX_BeginDraw_U02(void* pInstance)
{
    APICheckU02();

    GFX_DEBUGINFO (printf( "MApi_GFX_BeginDraw\n"));

#if DYNAMIC_POWER_ON_OFF
    MDrv_PWR_ClockTurnOn(E_PWR_MODULE_GE);
#endif

    if(g_apiGFXLocal.g_pGEContext->bGEMode4MultiProcessAccess ==FALSE)
        GE_Get_Resource(g_apiGFXLocal.g_pGEContext);

    g_apiGFXLocal.u32LockStatus++;
    return (GFX_Result) E_GE_OK;
}
GFX_Result MApi_GFX_EndDraw_U02(void* pInstance)
{
    APICheckU02();

#if DYNAMIC_POWER_ON_OFF
    GE_WaitIdle();
    MDrv_PWR_ClockTurnOff(E_PWR_MODULE_GE);
#endif

   if(g_apiGFXLocal.u32LockStatus==0)
       return (GFX_Result)E_GE_FAIL_LOCKED;

   if(g_apiGFXLocal.g_pGEContext->bGEMode4MultiProcessAccess == FALSE)
        GE_Free_Resource(g_apiGFXLocal.g_pGEContext);


    g_apiGFXLocal.u32LockStatus--;
    return (GFX_Result) E_GE_OK;
}

void MApi_GFX_SetDbgLevel_U02(void* pInstance,MS_U32 level)
{
    g_apiGFXLocal.u32dbglvl = level;
    MDrv_GE_SetDbgLevel(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.u32dbglvl);
    GFX_DEBUGINFO_LEVEL(1, printf("%s, Set DBG LEVEL to %ld \n", GFX_DBG_HDR, g_apiGFXLocal.u32dbglvl));
}

GFX_Result MApi_GFX_GetHK_U02(void* pInstance,MS_BOOL *bIsHK)
{
    APICheckU02();

    MDrv_GE_Get_GetHK(g_apiGFXLocal.g_pGEContext, bIsHK);
    return GFX_SUCCESS;

}

GFX_Result MApi_GFX_SetHK_U02(void* pInstance,MS_BOOL bIsHK)
{
    APICheckU02();

    MDrv_GE_Set_SetHK(g_apiGFXLocal.g_pGEContext, bIsHK);
    return GFX_SUCCESS;

}

GFX_Result MApi_GFX_DrawOval_U02(void* pInstance,GFX_OvalFillInfo *pOval)
{
    GE_OVAL_FILL_INFO stOval;

    APICheckU02();

    memcpy(&stOval, pOval, sizeof(GE_OVAL_FILL_INFO));
    MDrv_GE_DrawOval(g_apiGFXLocal.g_pGEContext, &stOval);
    return GFX_SUCCESS;
}
GFX_Result MApi_GFX_SetDC_CSC_FMT_U02(void* pInstance,GFX_YUV_Rgb2Yuv mode, GFX_YUV_OutRange yuv_out_range, GFX_YUV_InRange uv_in_range, GFX_YUV_422 srcfmt, GFX_YUV_422 dstfmt)
{
    GE_YUVMode yuvmode;
    MS_U32 u32op1 =0, ret;

    APICheckU02();

    ret = MApi_GFX_MapYUVOp(GFX_YUV_OP1, mode, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    yuvmode.rgb2yuv = (GE_Csc_Rgb2Yuv)u32op1;

    ret = MApi_GFX_MapYUVOp(GFX_YUV_OP2, yuv_out_range, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    yuvmode.out_range = (GE_YUV_OutRange)u32op1;

    ret = MApi_GFX_MapYUVOp(GFX_YUV_OP3, uv_in_range, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    yuvmode.in_range = (GE_YUV_InRange)u32op1;

    ret = MApi_GFX_MapYUVOp(GFX_YUV_OP4, dstfmt, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    yuvmode.dst_fmt = (GE_YUV_422)u32op1;

    ret = MApi_GFX_MapYUVOp(GFX_YUV_OP4, srcfmt, &u32op1 );
    if(ret != GFX_SUCCESS)
        GFX_DEBUGINFO_LEVEL(1, printf("%s%s %d, ERROR: %08lx\n", GFX_DBG_HDR, __FUNCTION__,  __LINE__, ret));

    yuvmode.src_fmt = (GE_YUV_422)u32op1;

    MDrv_GE_SetYUVMode(g_apiGFXLocal.g_pGEContext, &yuvmode);

    return GFX_SUCCESS;
}

GFX_Result MApi_GFX_SetStrBltSckType_U02(void* pInstance,GFX_StretchCKType type, GFX_RgbColor *color)
{
    MS_U32 u32tmp=0, ret=0;

    APICheckU02();

    ret = MApi_GFX_MapStrSCKType(type, &u32tmp);

    if (ret == GFX_SUCCESS)
    {
        ret = (MS_U32)MDrv_GE_SetStrBltSckType(g_apiGFXLocal.g_pGEContext, (GE_StretchCKType)u32tmp, (MS_U32*)(void*)color);
        return (GFX_Result)ret;
    }
    else
    {
        return GFX_INVALID_PARAMETERS;
    }

}

GFX_Result MApi_GFX_SetAlpha_ARGB1555_U02(void* pInstance,MS_U8 coef)
{
    APICheckU02();

    return (GFX_Result)MDrv_GE_SetAlpha_ARGB1555(g_apiGFXLocal.g_pGEContext, coef);  // DANGER

}
GFX_Result MApi_GFX_GetAlpha_ARGB1555_U02(void* pInstance,MS_U8* coef)
{
    APICheckU02();

    return (GFX_Result)MDrv_GE_GetAlpha_ARGB1555(g_apiGFXLocal.g_pGEContext, coef);     // DANGER
}

static MS_U16 Ioctl_GFX_Init(void* pInstance, void* pArgs)
{
    GFX_INIT_ARGS*          pGFXInit            =NULL;
    GFX_Init_Config         cfg;
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;

    pGFXInit = (GFX_INIT_ARGS*)pArgs;

    if (NULL == pGFXInit->pGFX_Init){
        GFX_CRITIAL_MSG(printf("GFX init FAIL\n"));
        return UTOPIA_STATUS_FAIL;
    }
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    cfg.bIsCompt = pGFXInit->pGFX_Init->bIsCompt;
    cfg.bIsHK= pGFXInit->pGFX_Init->bIsHK;

    cfg.u32VCmdQSize = 0;
    cfg.u32VCmdQAddr = 0;

    //escape the many thread modify the global variable
    if(FALSE == g_apiGFXLocal._bInit)
    {
        MDrv_GE_Init(pInstance, (GE_Config*)&cfg, &g_apiGFXLocal.g_pGEContext);
        g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode= E_GE_TLB_NONE;
    }
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    CheckSize(pGFXInit->u32Size, sizeof(GFX_Init_Config), 0);
    MDrv_GE_Chip_Proprity_Init(g_apiGFXLocal.g_pGEContext, &g_apiGFXLocal.pGeChipProperty);
    MDrv_GE_SetOnePixelMode(g_apiGFXLocal.g_pGEContext,!(g_apiGFXLocal.pGeChipProperty->bFourPixelModeStable));
    g_apiGFXLocal._bInit = TRUE;

#if (GE_PERFORMANCE_TEST)
    MDrv_GE_BitbltPerformance(g_apiGFXLocal.g_pGEContext);
#endif
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(UTOPIA_STATUS_SUCCESS), 0);
}

static MS_U16 Ioctl_GFX_GetCaps(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_GETCAPS_ARGS*       pGFXGetCaps         =NULL;
    GFX_Get_CAPS*           pGFXGetCAPS         =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;

    CMD(0);
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXGetCaps = (GFX_GETCAPS_ARGS*)pArgs;

    pGFXGetCAPS = (GFX_Get_CAPS*)pGFXGetCaps->pGFX_GetCaps;
    u32Ret = MApi_GFX_GetGECaps_U02(pInstance, pGFXGetCAPS->eCapType, pGFXGetCAPS->pRet, pGFXGetCAPS->Ret_Size);
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret),0);
}

static MS_U16 Ioctl_GFX_GetInfo(void* pInstance, void* pArgs)
{
    EN_GFX_GET_CONFIG       eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_GETINFO_ARGS*       pGFXGetInfo         =NULL;
    GFX_Get_BufferInfo*     pGFXGetBufferInfo   =NULL;
    GFX_Get_NextTagID*      pGFXNextTagID       =NULL;
    GFX_Get_Clip_Property*  pGFXGetClip         =NULL;
    GFX_Get_Intensity*      pGFXGetIntensity    =NULL;
    GFX_DbgInfo*            pGFXGetDBGInfo      =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    MS_U16                  pu16Val             =0;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXGetInfo = (GFX_GETINFO_ARGS*)pArgs;
    eCmd = pGFXGetInfo->eGFX_GetConfig;
    CMD(eCmd);

    switch(eCmd)
    {
        case E_GFX_GET_BUFFERINFO:
            CheckSize(pGFXGetInfo->u32Size, sizeof(GFX_Get_BufferInfo), eCmd);
            pGFXGetBufferInfo = (GFX_Get_BufferInfo*)pGFXGetInfo->pGFX_GetInfo;
            u32Ret = MApi_GFX_GetBufferInfo_U02(pInstance, pGFXGetBufferInfo->pSrcbufInfo->pBufInfo, pGFXGetBufferInfo->pDstbufInfo->pBufInfo);
            break;

        case E_GFX_GET_TAGID:
            CheckSize(pGFXGetInfo->u32Size, sizeof(MS_U16), eCmd);
            u32Ret = MDrv_GE_GetTAGID(g_apiGFXLocal.g_pGEContext, &pu16Val);
            pGFXGetInfo->pGFX_GetInfo = (void*)&pu16Val;
            break;

        case E_GFX_GET_NEXTTAGID:
            CheckSize(pGFXGetInfo->u32Size, sizeof(GFX_Get_NextTagID), eCmd);
            pGFXNextTagID = (GFX_Get_NextTagID*)pGFXGetInfo->pGFX_GetInfo;
            u32Ret = MDrv_GE_GetNextTAGID(g_apiGFXLocal.g_pGEContext, pGFXNextTagID->bStepTagBefore, pGFXNextTagID->pTagID);
            break;

        case E_GFX_GET_CLIP:
            CheckSize(pGFXGetInfo->u32Size, sizeof(GFX_Get_Clip_Property), eCmd);
            pGFXGetClip = (GFX_Get_Clip_Property*)pGFXGetInfo->pGFX_GetInfo ;
            u32Ret =MApi_GFX_GetClip_U02(pInstance, pGFXGetClip->pV0, pGFXGetClip->pV1);
            break;

        case E_GFX_GET_INTENSITY:
            CheckSize(pGFXGetInfo->u32Size, sizeof(GFX_Get_Intensity), eCmd);
            pGFXGetIntensity = (GFX_Get_Intensity*)pGFXGetInfo->pGFX_GetInfo;
            u32Ret = MDrv_GE_GetIntensity(g_apiGFXLocal.g_pGEContext, pGFXGetIntensity->u32Id, pGFXGetIntensity->pColor);
            break;

        case E_GFX_GET_DBGINFO: //Use?
            CheckSize(pGFXGetInfo->u32Size, sizeof(GFX_DbgInfo), eCmd);
            pGFXGetDBGInfo = (GFX_DbgInfo*)pGFXGetInfo->pGFX_GetInfo;
            u32Ret = MApi_GFX_GetDBGInfo_U02(pInstance, pGFXGetDBGInfo);
            break;

        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            break;
    }
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret),eCmd);
}

static int Ioctl_GFX_LineDraw(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_LINEDRAW_ARGS*      pGFXLineDraw        =NULL;
    GFX_Set_DrawLineInfo*   pGFXLineInfo        =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    GE_RgbColor geRgbColor;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXLineDraw = (GFX_LINEDRAW_ARGS*)pArgs;
    CheckSize(pGFXLineDraw->u32Size, sizeof(GFX_Set_DrawLineInfo), 0);
    pGFXLineInfo = (GFX_Set_DrawLineInfo*)pGFXLineDraw->pLineInfo;
    MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
    }

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXLineInfo->pDstbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXLineInfo->pDstbufInfo->pBufInfo->u32Width, pGFXLineInfo->pDstbufInfo->pBufInfo->u32Height\
        ,pGFXLineInfo->pDstbufInfo->pBufInfo->u32Addr\
        ,pGFXLineInfo->pDstbufInfo->pBufInfo->u32Pitch\
        ,pGFXLineInfo->pDstbufInfo->u32OffsetByte);

    geRgbColor.a= (g_apiGFXLocal.u32geRgbColor&0xff000000)>>24;
    geRgbColor.r= (g_apiGFXLocal.u32geRgbColor&0x00ff0000)>>16;
    geRgbColor.g= (g_apiGFXLocal.u32geRgbColor&0x0000ff00)>>8;
    geRgbColor.b= (g_apiGFXLocal.u32geRgbColor&0x000000ff);
    MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);

    u32Ret = MApi_GFX_DrawLine_U02(pInstance, pGFXLineInfo->pDrawLineInfo);
    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }

    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);
}

static int Ioctl_GFX_RectFill(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_RECTFILL_ARGS*      pGFXRectFill        =NULL;
    GFX_Set_RectFillInfo*   pGFXRectInfo        =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    GE_RgbColor geRgbColor;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXRectFill = (GFX_RECTFILL_ARGS*)pArgs;
    CheckSize(pGFXRectFill->u32Size, sizeof(GFX_Set_RectFillInfo), 0);
    pGFXRectInfo = (GFX_Set_RectFillInfo*)pGFXRectFill->pFillBlock;

    MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
    }

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXRectInfo->pDstbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXRectInfo->pDstbufInfo->pBufInfo->u32Width, pGFXRectInfo->pDstbufInfo->pBufInfo->u32Height\
        ,pGFXRectInfo->pDstbufInfo->pBufInfo->u32Addr\
        ,pGFXRectInfo->pDstbufInfo->pBufInfo->u32Pitch\
        ,pGFXRectInfo->pDstbufInfo->u32OffsetByte);

    geRgbColor.a= (g_apiGFXLocal.u32geRgbColor&0xff000000)>>24;
    geRgbColor.r= (g_apiGFXLocal.u32geRgbColor&0x00ff0000)>>16;
    geRgbColor.g= (g_apiGFXLocal.u32geRgbColor&0x0000ff00)>>8;
    geRgbColor.b= (g_apiGFXLocal.u32geRgbColor&0x000000ff);
    MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);

    u32Ret = MApi_GFX_RectFill_U02(pInstance, pGFXRectInfo->pRectFillInfo);
    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }


    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);
}

static int Ioctl_GFX_BitBlt(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_BITBLT_ARGS*        pGFXBitblt          =NULL;
    GFX_BitBltInfo*         pGFXBitBltInfo      =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    GE_RgbColor geRgbColor;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXBitblt = (GFX_BITBLT_ARGS*)pArgs;
    CheckSize(pGFXBitblt->u32Size, sizeof(GFX_BitBltInfo), 0);
    pGFXBitBltInfo = (GFX_BitBltInfo*)pGFXBitblt->pGFX_BitBlt;

    MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBSrcBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbsrcaddr);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
    }

    MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, pGFXBitBltInfo->pSrcbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXBitBltInfo->pSrcbufInfo->pBufInfo->u32Width, pGFXBitBltInfo->pSrcbufInfo->pBufInfo->u32Height\
        ,pGFXBitBltInfo->pSrcbufInfo->pBufInfo->u32Addr\
        ,pGFXBitBltInfo->pSrcbufInfo->pBufInfo->u32Pitch\
        ,pGFXBitBltInfo->pSrcbufInfo->u32OffsetByte);

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXBitBltInfo->pDstbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXBitBltInfo->pDstbufInfo->pBufInfo->u32Width, pGFXBitBltInfo->pDstbufInfo->pBufInfo->u32Height\
        ,pGFXBitBltInfo->pDstbufInfo->pBufInfo->u32Addr\
        ,pGFXBitBltInfo->pDstbufInfo->pBufInfo->u32Pitch\
        ,pGFXBitBltInfo->pDstbufInfo->u32OffsetByte);

    geRgbColor.a= (g_apiGFXLocal.u32geRgbColor&0xff000000)>>24;
    geRgbColor.r= (g_apiGFXLocal.u32geRgbColor&0x00ff0000)>>16;
    geRgbColor.g= (g_apiGFXLocal.u32geRgbColor&0x0000ff00)>>8;
    geRgbColor.b= (g_apiGFXLocal.u32geRgbColor&0x000000ff);
    MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);

    u32Ret = MApi_GFX_BitBlt_U02(pInstance, pGFXBitBltInfo->pDrawRect, pGFXBitBltInfo->u32DrawFlag, pGFXBitBltInfo->pScaleInfo);

  /*  psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo = pGFXBitBltInfo->pBufInfo->pSrcbufInfo;
        printf("\33[0;34m[GFX_INFO]Src BufferInfo [Addr=0x%lx],[Width=0x%lx], [Height=0x%lx], [Pitch=0x%lx], [ColorFmt=0x%x], [OffsetByte=0x%lx] \33[m\n"                                 \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo->pBufInfo->u32Addr      \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo->pBufInfo->u32Width     \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo->pBufInfo->u32Height    \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo->pBufInfo->u32Pitch     \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo->pBufInfo->u32ColorFmt  \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pSrcbufInfo->u32OffsetByte          );
    psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo = pGFXBitBltInfo->pBufInfo->pDstbufInfo;
        printf("\33[0;34m[GFX_INFO]Dst BufferInfo [Addr=0x%lx],[Width=0x%lx], [Height=0x%lx], [Pitch=0x%lx], [ColorFmt=0x%x], [OffsetByte=0x%lx] \33[m\n"                                 \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo->pBufInfo->u32Addr      \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo->pBufInfo->u32Width     \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo->pBufInfo->u32Height    \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo->pBufInfo->u32Pitch     \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo->pBufInfo->u32ColorFmt  \
        ,psGFXInstPri->pGFXBufferInfo->pBufInfo->pDstbufInfo->u32OffsetByte          );*/

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }

    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);

}

static int Ioctl_GFX_TwoSrcBitBlt(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE*    psGFXInstPri         =NULL;
    GFX_TWOSRC_BITBLT_ARGS* pGFXTwoSrcBitblt     =NULL;
    GFX_TwoSrc_BitBltInfo*  pGFXTwoSrcBitBltInfo =NULL;
    MS_U32                  u32Ret               =GFX_FAIL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXTwoSrcBitblt = (GFX_TWOSRC_BITBLT_ARGS*)pArgs;
    CheckSize(pGFXTwoSrcBitblt->u32Size, sizeof(GFX_TwoSrc_BitBltInfo), 0);
    pGFXTwoSrcBitBltInfo = (GFX_TwoSrc_BitBltInfo*)pGFXTwoSrcBitblt->pGFX_TwoSrc_BitBlt;

    MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);
    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBSrcBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbsrcaddr);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
    }

    MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, pGFXTwoSrcBitBltInfo->pSrcbufInfo_0->pBufInfo->u32ColorFmt\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_0->pBufInfo->u32Width, pGFXTwoSrcBitBltInfo->pSrcbufInfo_0->pBufInfo->u32Height\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_0->pBufInfo->u32Addr\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_0->pBufInfo->u32Pitch\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_0->u32OffsetByte);

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXTwoSrcBitBltInfo->pSrcbufInfo_1->pBufInfo->u32ColorFmt\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_1->pBufInfo->u32Width, pGFXTwoSrcBitBltInfo->pSrcbufInfo_1->pBufInfo->u32Height\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_1->pBufInfo->u32Addr\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_1->pBufInfo->u32Pitch\
        ,pGFXTwoSrcBitBltInfo->pSrcbufInfo_1->u32OffsetByte);

    u32Ret = MApi_GFX_BitbltByTwoSourceBuffer_U02(pInstance, pGFXTwoSrcBitBltInfo->pDstbufInfo->pBufInfo, pGFXTwoSrcBitBltInfo->pDrawTwoSrcRect, pGFXTwoSrcBitBltInfo->u32DrawFlag);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }

    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);
}

static int Ioctl_GFX_SetABL(void* pInstance, void* pArgs)
{
    EN_GFX_SET_ABL          eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_ABL_ARGS*           pGFXABL             =NULL;
    GFX_Set_ABL*            pGFXSetABL          =NULL;
    GFX_Set_Colorkey*       pGFXSetColorKey     =NULL;
    GFX_Set_ROP*            pGFXSetROP          =NULL;
    GFX_Set_DFB*            pGFXSetDFB          =NULL;
    GFX_Set_Intensity*      pGFXSetIntensity    =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    MS_U32                  u32BLDCOEF = 0;
    MS_U32                  u32ABLSRC = 0;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXABL = (GFX_ABL_ARGS*)pArgs;
    eCmd = pGFXABL->eGFX_SetABL;
    CMD(eCmd);

    switch(eCmd)
    {
        case E_GFX_SET_ABL:
            CheckSize(pGFXABL->u32Size, sizeof(GFX_Set_ABL),eCmd);
            pGFXSetABL = (GFX_Set_ABL*)pGFXABL->pGFX_ABL;
            g_apiGFXLocal.u32geRgbColor = (g_apiGFXLocal.u32geRgbColor&0x00ffffff)|((pGFXSetABL->u8Alpha_Const)<<24);
            MApi_GFX_MapBLDCOEF(pGFXSetABL->eABLCoef, &u32BLDCOEF);
            MApi_GFX_MapABLCOEF(pGFXSetABL->eDb_abl, &u32ABLSRC);
            g_apiGFXLocal.pABLInfo.eBldCoef =         (GE_BlendOp)u32BLDCOEF;
            g_apiGFXLocal.pABLInfo.eABLSrc =          (GE_AlphaSrc)u32ABLSRC;
            g_apiGFXLocal.pABLInfo.u32ABLConstCoef =  (MS_U32)pGFXSetABL->u8Alpha_Const;
            MApi_GFX_SetAlpha_U02(pInstance, pGFXSetABL->bEnable, pGFXSetABL->eABLCoef, pGFXSetABL->eDb_abl, pGFXSetABL->u8Alpha_Const);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_COLORKEY:
            CheckSize(pGFXABL->u32Size, sizeof(GFX_Set_Colorkey), eCmd);
            pGFXSetColorKey = (GFX_Set_Colorkey*)pGFXABL->pGFX_ABL;
            if( pGFXSetColorKey->ColorKey_Type & E_SRC_COLORKEY)
            {
                MApi_GFX_SetSrcColorKey_U02(pInstance, pGFXSetColorKey->pSrcbufInfo->bEnable, pGFXSetColorKey->pSrcbufInfo->eOpMode, \
                    pGFXSetColorKey->pSrcbufInfo->eFmt, pGFXSetColorKey->pSrcbufInfo->pS_color, pGFXSetColorKey->pSrcbufInfo->pE_color);
            }
            if( pGFXSetColorKey->ColorKey_Type & E_DST_COLORKEY)
            {
                MApi_GFX_SetDstColorKey_U02(pInstance, pGFXSetColorKey->pDstbufInfo->bEnable, pGFXSetColorKey->pDstbufInfo->eOpMode, \
                    pGFXSetColorKey->pDstbufInfo->eFmt, pGFXSetColorKey->pDstbufInfo->pS_color, pGFXSetColorKey->pDstbufInfo->pE_color);
            }
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_ROP:
            CheckSize(pGFXABL->u32Size, sizeof(GFX_Set_ROP), eCmd);
            pGFXSetROP = (GFX_Set_ROP*)pGFXABL->pGFX_ABL;
            MApi_GFX_SetROP2_U02(pInstance, pGFXSetROP->bEnable, pGFXSetROP->eRopMode);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_DFB:
            CheckSize(pGFXABL->u32Size, sizeof(GFX_Set_DFB), eCmd);
            pGFXSetDFB = (GFX_Set_DFB*)pGFXABL->pGFX_ABL;
            g_apiGFXLocal.u32geRgbColor = (pGFXSetDFB->sRGBColor.a)<<24|(pGFXSetDFB->sRGBColor.r)<<16|(pGFXSetDFB->sRGBColor.g)<<8|(pGFXSetDFB->sRGBColor.b);
            MDrv_GE_EnableDFBBlending(g_apiGFXLocal.g_pGEContext, pGFXSetDFB->bEnable);
            MApi_GFX_SetDFBBldOP_U02(pInstance, pGFXSetDFB->eSrcBldOP, pGFXSetDFB->eDstBldOP);
            MApi_GFX_SetDFBBldConstColor_U02(pInstance, pGFXSetDFB->sRGBColor);
            MApi_GFX_SetDFBBldFlags_U02(pInstance, pGFXSetDFB->u16DFBBldFlags);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_INTENSITY:
            CheckSize(pGFXABL->u32Size, sizeof(GFX_Set_Intensity), eCmd);
            pGFXSetIntensity = (GFX_Set_Intensity*)pGFXABL->pGFX_ABL;
            MApi_GFX_SetIntensity_U02(pInstance, pGFXSetIntensity->u32Id, pGFXSetIntensity->eFmt, pGFXSetIntensity->pColor);
            u32Ret = GFX_SUCCESS;
            break;
        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            u32Ret = GFX_FAIL;
            break;

    }
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), eCmd);
}


static int Ioctl_GFX_SetConfig(void* pInstance, void* pArgs)
{
    EN_GFX_SET_CONFIG       eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_SETCONFIG_ARGS*     pGFXSetConfig       =NULL;
    GFX_Set_VQ*             pGFXSetVQ           =NULL;
    GFX_Set_Mirror*         pGFXSetMirror       =NULL;
    GFX_Set_Italic*         pGFXSetItalic       =NULL;
    GFX_Set_PaletteOpt*     pGFXSetPaletteOpt   =NULL;
    GFX_Set_Clip_Property*  pGFXSetClip         =NULL;
    GFX_Set_WriteProtect*   pGFXSetWP           =NULL;
    GFX_Set_AlphaCompare*   pGFXSetCMP          =NULL;
    GFX_Set_CSC*            pGFXSetCSC          =NULL;
    MS_BOOL*                pbVal               =NULL;
    MS_U32*                 pu32Val             =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXSetConfig = (GFX_SETCONFIG_ARGS*)pArgs;
    eCmd = pGFXSetConfig->eGFX_SetConfig;
    CMD(eCmd);

    switch(eCmd)
    {
        case E_GFX_SET_POWERON:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_BOOL), eCmd);
            pbVal = (MS_BOOL*)pGFXSetConfig->pGFX_ConfigInfo;
            MDrv_GE_SetClock(g_apiGFXLocal.g_pGEContext, *pbVal);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_FLUSHQUEUE:
            MDrv_GE_WaitIdle(g_apiGFXLocal.g_pGEContext);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_TAGID:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_U32), eCmd);
            pu32Val = (MS_U32*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->u32TagID = *pu32Val;
            MDrv_GE_SetTAGID(g_apiGFXLocal.g_pGEContext, (MS_U16)*pu32Val);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_NEXTTAGID:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_U32), eCmd);
            MDrv_GE_SetNextTAGID(g_apiGFXLocal.g_pGEContext, (MS_U16*)pGFXSetConfig->pGFX_ConfigInfo);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_WAITFORTAGID:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_U32), eCmd);
            pu32Val = (MS_U32*)pGFXSetConfig->pGFX_ConfigInfo;
            MDrv_GE_WaitTAGID(g_apiGFXLocal.g_pGEContext, (MS_U16)*pu32Val);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_ONEPIXELMODE:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_BOOL), eCmd);
            pbVal = (MS_BOOL*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->bOnePixelMode = *pbVal;
            MDrv_GE_SetOnePixelMode(g_apiGFXLocal.g_pGEContext, *pbVal);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_DITHER:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_BOOL), eCmd);
            pbVal = (MS_BOOL*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->bDither = *pbVal;
            g_apiGFXLocal.bDither = *pbVal;
            u32Ret = GFX_SUCCESS;
            break;

        //VQ switch can't dynamic disablb, cause of cmd losing.
        case E_GFX_SET_VQ:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_VQ), eCmd);
            pGFXSetVQ = (GFX_Set_VQ*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->bVQEnable = pGFXSetVQ->bEnable;
            MDrv_GE_EnableVCmdQueue(g_apiGFXLocal.g_pGEContext, pGFXSetVQ->bEnable);
            MApi_GFX_SetVCmdBuffer_U02(pInstance, pGFXSetVQ->u32Addr, pGFXSetVQ->enBufSize);
            MDrv_GE_SetVCmd_R_Thread(g_apiGFXLocal.g_pGEContext, pGFXSetVQ->u8R_Threshold);
            MDrv_GE_SetVCmd_W_Thread(g_apiGFXLocal.g_pGEContext, pGFXSetVQ->u8W_Threshold);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_MIRROR:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_Mirror), eCmd);
            pGFXSetMirror=(GFX_Set_Mirror*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->pMirror->bMirrorX = pGFXSetMirror->bMirrorX;
            //psGFXInstPri->pGFXSetConfig->pMirror->bMirrorY = pGFXSetMirror->bMirrorY;

            g_apiGFXLocal._bMirrorH = pGFXSetMirror->bMirrorX;
            g_apiGFXLocal._bMirrorV = pGFXSetMirror->bMirrorY;
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_DST_MIRROR:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_Mirror), eCmd);
            pGFXSetMirror=(GFX_Set_Mirror*)pGFXSetConfig->pGFX_ConfigInfo;
            g_apiGFXLocal._bDstMirrorH = pGFXSetMirror->bMirrorX;
            g_apiGFXLocal._bDstMirrorV = pGFXSetMirror->bMirrorY;
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_ROTATE:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_RotateAngle), eCmd);
            pu32Val = (MS_U32*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->eRotate = pGFXSetRotate;

            g_apiGFXLocal._angle = (GFX_RotateAngle)*pu32Val;
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_ITALIC:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_Italic), eCmd);
            pGFXSetItalic = (GFX_Set_Italic*)pGFXSetConfig->pGFX_ConfigInfo;
            MDrv_GE_SetItalic(g_apiGFXLocal.g_pGEContext, pGFXSetItalic->u8Ini_dis, pGFXSetItalic->u8Ini_line, pGFXSetItalic->u8Delta);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_NEARESTMODE:
            CheckSize(pGFXSetConfig->u32Size, sizeof(MS_BOOL), eCmd);
            pbVal = (MS_BOOL*)pGFXSetConfig->pGFX_ConfigInfo;
            g_apiGFXLocal._bNearest = *pbVal;
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_PALETTEOPT:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_PaletteOpt), eCmd);
            pGFXSetPaletteOpt = (GFX_Set_PaletteOpt*)pGFXSetConfig->pGFX_ConfigInfo;
            MApi_GFX_SetPaletteOpt_U02(pInstance, pGFXSetPaletteOpt->pPalArray, pGFXSetPaletteOpt->u32PalStart, pGFXSetPaletteOpt->u32PalEnd);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_CLIP:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_Clip_Property), eCmd);
            pGFXSetClip = (GFX_Set_Clip_Property*)pGFXSetConfig->pGFX_ConfigInfo;
            //psGFXInstPri->pGFXSetConfig->pClip->pV0 = pGFXSetClip->pV0;
            //psGFXInstPri->pGFXSetConfig->pClip->pV1 = pGFXSetClip->pV1;
            MApi_GFX_SetClip_U02(pInstance, pGFXSetClip->pV0, pGFXSetClip->pV1);
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_SET_WRITEPROTECT:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_WriteProtect), eCmd);
            pGFXSetWP = (GFX_Set_WriteProtect*)pGFXSetConfig->pGFX_ConfigInfo;
            MDrv_GE_WriteProtect(g_apiGFXLocal.g_pGEContext, pGFXSetWP->u8Miu, pGFXSetWP->u32Addr_Low, pGFXSetWP->u32Addr_High, pGFXSetWP->eWPType);
            u32Ret = GFX_SUCCESS;
            break;
        case E_GFX_SET_ALPHA_COMPARE:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_AlphaCompare), eCmd);
            pGFXSetCMP = (GFX_Set_AlphaCompare*)pGFXSetConfig->pGFX_ConfigInfo;
            MApi_GFX_SetAlphaCmp_U02(pInstance,pGFXSetCMP->enable,pGFXSetCMP->eMode);
            u32Ret = GFX_SUCCESS;
            break;
        case E_GFX_SET_CSC:
            CheckSize(pGFXSetConfig->u32Size, sizeof(GFX_Set_CSC), eCmd);
            pGFXSetCSC = (GFX_Set_CSC*)pGFXSetConfig->pGFX_ConfigInfo;
            MApi_GFX_SetDC_CSC_FMT_U02(pInstance,pGFXSetCSC->mode,pGFXSetCSC->yuv_out_range,pGFXSetCSC->uv_in_range,pGFXSetCSC->srcfmt,pGFXSetCSC->dstfmt);
            u32Ret = GFX_SUCCESS;
            break;
        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            u32Ret = GFX_FAIL;
            break;
    }

    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), eCmd);
}

static int Ioctl_GFX_Bitmap(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_BITMAP_ARGS*        pGFXBitmap          =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    GE_RgbColor geRgbColor;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXBitmap = (GFX_BITMAP_ARGS*)pArgs;
    CheckSize(pGFXBitmap->u32Size, sizeof(GFX_Bitmap), 0);

    MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBSrcBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbsrcaddr);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
    }

    MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, pGFXBitmap->pGFX_Bitmap->pSrcbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXBitmap->pGFX_Bitmap->pSrcbufInfo->pBufInfo->u32Width, pGFXBitmap->pGFX_Bitmap->pSrcbufInfo->pBufInfo->u32Height\
        ,pGFXBitmap->pGFX_Bitmap->pSrcbufInfo->pBufInfo->u32Addr\
        ,pGFXBitmap->pGFX_Bitmap->pSrcbufInfo->pBufInfo->u32Pitch\
        ,pGFXBitmap->pGFX_Bitmap->pSrcbufInfo->u32OffsetByte);

    MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXBitmap->pGFX_Bitmap->pDstbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXBitmap->pGFX_Bitmap->pDstbufInfo->pBufInfo->u32Width, pGFXBitmap->pGFX_Bitmap->pDstbufInfo->pBufInfo->u32Height\
        ,pGFXBitmap->pGFX_Bitmap->pDstbufInfo->pBufInfo->u32Addr\
        ,pGFXBitmap->pGFX_Bitmap->pDstbufInfo->pBufInfo->u32Pitch\
        ,pGFXBitmap->pGFX_Bitmap->pDstbufInfo->u32OffsetByte);

    geRgbColor.a= (g_apiGFXLocal.u32geRgbColor&0xff000000)>>24;
    geRgbColor.r= (g_apiGFXLocal.u32geRgbColor&0x00ff0000)>>16;
    geRgbColor.g= (g_apiGFXLocal.u32geRgbColor&0x0000ff00)>>8;
    geRgbColor.b= (g_apiGFXLocal.u32geRgbColor&0x000000ff);
    MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);

    u32Ret = MApi_GFX_DrawBitmap_U02(pInstance,pGFXBitmap->pGFX_Bitmap->handle, pGFXBitmap->pGFX_Bitmap->pBmpInfo);
    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);
}

static int Ioctl_GFX_Font(void* pInstance, void* pArgs)
{
    GFX_TEXTTYPE            eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri        =NULL;
    GFX_TEXTOUT_ARGS*       pGFXTextOutArg      =NULL;
    GFX_TextOut*            pGFXTextOut         =NULL;
    GFX_CharacterOut*       pGFXCharacterOut    =NULL;
    MS_U32                  u32Ret              =GFX_FAIL;
    GE_RgbColor geRgbColor;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXTextOutArg = (GFX_TEXTOUT_ARGS*)pArgs;
    eCmd = pGFXTextOutArg->eGFX_TextType;
    CMD(eCmd);

    switch(eCmd)
    {
        case E_LOADTEXTOUT:
            CheckSize(pGFXTextOutArg->u32Size, sizeof(GFX_TextOut), eCmd);
            pGFXTextOut = (GFX_TextOut*)pGFXTextOutArg->pGFX_TextOut;

            MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

            if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
            {
                MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
            }

            if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
            {
                MDrv_GE_SetTLBSrcBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbsrcaddr);
            }

            if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
            {
                MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
            }

            MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, pGFXTextOut->pSrcbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXTextOut->pSrcbufInfo->pBufInfo->u32Width, pGFXTextOut->pSrcbufInfo->pBufInfo->u32Height\
        ,pGFXTextOut->pSrcbufInfo->pBufInfo->u32Addr\
        ,pGFXTextOut->pSrcbufInfo->pBufInfo->u32Pitch\
        ,pGFXTextOut->pSrcbufInfo->u32OffsetByte);


            MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXTextOut->pDstbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXTextOut->pDstbufInfo->pBufInfo->u32Width, pGFXTextOut->pDstbufInfo->pBufInfo->u32Height\
        ,pGFXTextOut->pDstbufInfo->pBufInfo->u32Addr\
        ,pGFXTextOut->pDstbufInfo->pBufInfo->u32Pitch\
        ,pGFXTextOut->pDstbufInfo->u32OffsetByte);

            geRgbColor.a= (g_apiGFXLocal.u32geRgbColor&0xff000000)>>24;
            geRgbColor.r= (g_apiGFXLocal.u32geRgbColor&0x00ff0000)>>16;
            geRgbColor.g= (g_apiGFXLocal.u32geRgbColor&0x0000ff00)>>8;
            geRgbColor.b= (g_apiGFXLocal.u32geRgbColor&0x000000ff);
            MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);

            MApi_GFX_TextOut_U02(pInstance, pGFXTextOut->fHandle, (MS_U8*)pGFXTextOut->pIndex, pGFXTextOut->u32StrWidth, pGFXTextOut->pTextInfo);
            u32Ret = GFX_SUCCESS;
            break;

        case E_CHARACTEROUT:
            CheckSize(pGFXTextOutArg->u32Size, sizeof(GFX_CharacterOut), eCmd);
            pGFXCharacterOut = (GFX_CharacterOut*)pGFXTextOutArg->pGFX_TextOut;

            MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

            if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
            {
                MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
            }

            if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
            {
                MDrv_GE_SetTLBSrcBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbsrcaddr);
            }

            if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
            {
                MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
            }

            MDrv_GE_SetSrcBuffer(g_apiGFXLocal.g_pGEContext, pGFXCharacterOut->pSrcbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXCharacterOut->pSrcbufInfo->pBufInfo->u32Width, pGFXCharacterOut->pSrcbufInfo->pBufInfo->u32Height\
        ,pGFXCharacterOut->pSrcbufInfo->pBufInfo->u32Addr\
        ,pGFXCharacterOut->pSrcbufInfo->pBufInfo->u32Pitch\
        ,pGFXCharacterOut->pSrcbufInfo->u32OffsetByte);

            MDrv_GE_SetDstBuffer(g_apiGFXLocal.g_pGEContext, pGFXCharacterOut->pDstbufInfo->pBufInfo->u32ColorFmt\
        ,pGFXCharacterOut->pDstbufInfo->pBufInfo->u32Width, pGFXCharacterOut->pDstbufInfo->pBufInfo->u32Height\
        ,pGFXCharacterOut->pDstbufInfo->pBufInfo->u32Addr\
        ,pGFXCharacterOut->pDstbufInfo->pBufInfo->u32Pitch\
        ,pGFXCharacterOut->pDstbufInfo->u32OffsetByte);

            geRgbColor.a= (g_apiGFXLocal.u32geRgbColor&0xff000000)>>24;
            geRgbColor.r= (g_apiGFXLocal.u32geRgbColor&0x00ff0000)>>16;
            geRgbColor.g= (g_apiGFXLocal.u32geRgbColor&0x0000ff00)>>8;
            geRgbColor.b= (g_apiGFXLocal.u32geRgbColor&0x000000ff);
            MDrv_GE_SetDFBBldConstColor(g_apiGFXLocal.g_pGEContext, geRgbColor);

            MApi_GFX_CharacterOut_U02(pInstance, pGFXCharacterOut->pChar, pGFXCharacterOut->pTextInfo);
            u32Ret = GFX_SUCCESS;
            break;

        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            u32Ret = GFX_FAIL;
            break;
    }
    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }

    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), eCmd);
}

static int Ioctl_GFX_PowerState(void* pInstance, void* pArgs)
{
    EN_POWER_MODE           eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri            =NULL;
    GFX_POWERSTATE_ARGS*    pGFXPowerStateArg       =NULL;
    MS_U32                  u32Ret                  =GFX_FAIL;
    void*                   pModule                =NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    pGFXPowerStateArg = (GFX_POWERSTATE_ARGS*)pArgs;
    eCmd = pGFXPowerStateArg->eGFX_PowerStateType;
    CMD(eCmd);

    switch(eCmd)
    {
        case E_POWER_SUSPEND:
            GFX_DEBUG_MSG(printf(" [%s][%d] E_POWER_SUSPEND Start\n",__FUNCTION__,__LINE__));
            MDrv_GE_WaitIdle(g_apiGFXLocal.g_pGEContext);
            GE_Get_Resource(g_apiGFXLocal.g_pGEContext);
            GFX_DEBUG_MSG(printf(" [%s][%d] E_POWER_SUSPEND End\n",__FUNCTION__,__LINE__));
            u32Ret = GFX_SUCCESS;
            break;

        case E_POWER_RESUME:
            GFX_DEBUG_MSG(printf(" [%s][%d] E_POWER_RESUME Start\n",__FUNCTION__,__LINE__));
            MDrv_GE_Exit(pInstance);
            //API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
            GE_Get_Resource(g_apiGFXLocal.g_pGEContext);
            MDrv_GE_SetClock(g_apiGFXLocal.g_pGEContext, TRUE);
            UtopiaInstanceGetModule(pInstance, &pModule);
            // HW Init
        MDrv_GE_SetPowerState(g_apiGFXLocal.g_pGEContext ,pGFXPowerStateArg->eGFX_PowerStateType, pModule);
            // Default setting : To avoid GE issues commands too frequently when VC in enabled
            MDrv_GE_SetVCmd_W_Thread(g_apiGFXLocal.g_pGEContext, 0x4);
            MDrv_GE_SetVCmd_R_Thread(g_apiGFXLocal.g_pGEContext, 0x4);
            MDrv_GE_SetOnePixelMode(g_apiGFXLocal.g_pGEContext,!(g_apiGFXLocal.pGeChipProperty->bFourPixelModeStable));
            GE_Free_Resource(g_apiGFXLocal.g_pGEContext);
            GFX_DEBUG_MSG(printf(" [%s][%d] E_POWER_RESUME End\n",__FUNCTION__,__LINE__));
            u32Ret = GFX_SUCCESS;
            break;

        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            u32Ret = GFX_FAIL;
            break;
    }
    return u32Ret;
}

static int Ioctl_GFX_CB(void* pInstance, void* pArgs)
{
    EN_POWER_MODE           eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri            =NULL;
    GFX_CB_ARGS*            pGFXCBArg               =NULL;
    MS_U32                  u32Ret                  =GFX_FAIL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);
    pGFXCBArg = (GFX_CB_ARGS*)pArgs;
    eCmd = pGFXCBArg->eGFX_CBType;
    CMD(eCmd);

    switch(eCmd)
    {
        case E_CB_GETFONT:
            g_apiGFXLocal.fpGetFont = pGFXCBArg->pGFX_CBInfo;
            u32Ret = GFX_SUCCESS;
            break;

        case E_CB_GETBMP:
            g_apiGFXLocal.fpGetBMP = pGFXCBArg->pGFX_CBInfo;
            u32Ret = GFX_SUCCESS;
            break;

        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            u32Ret = GFX_FAIL;
            break;
    }
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), eCmd);
}

static int Ioctl_GFX_MISC(void* pInstance, void* pArgs)
{
    EN_GFX_MISC_MODE        eCmd;
    GFX_INSTANT_PRIVATE*    psGFXInstPri            =NULL;
    GFX_MISC_ARGS*          pGFXMISCArg             =NULL;
    GFX_ClearFrameBuffer*   pGFXClearFrame          =NULL;
    GFX_SetLinePattern*     pGFXSetLinePattern      =NULL;
    GFX_SetStrBltSckType*   pStrBltSckType          =NULL;
    MS_U32*                 pLevel                  =NULL;
    MS_BOOL*                pIsHK                   =NULL;
    MS_BOOL*                pbPatch                 =NULL;
    MS_U8*                  pALB1555                =NULL;
    MS_U32                  u32Ret                  =GFX_FAIL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);

    pGFXMISCArg = (GFX_MISC_ARGS*)pArgs;
    eCmd = pGFXMISCArg->eGFX_MISCType;
    switch(eCmd)
    {
        case E_MISC_EXIT:
            MDrv_GE_Exit(pInstance);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_CLEAR_FRAME_BY_WORLD:
            CheckSize(pGFXMISCArg->u32Size, sizeof(GFX_ClearFrameBuffer),eCmd);
            pGFXClearFrame = (GFX_ClearFrameBuffer*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_ClearFrameBufferByWord_U02(pInstance,pGFXClearFrame->StrAddr,pGFXClearFrame->length,pGFXClearFrame->ClearValue);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_CLEAR_FRAME:
            CheckSize(pGFXMISCArg->u32Size, sizeof(GFX_ClearFrameBuffer),eCmd);
            pGFXClearFrame = (GFX_ClearFrameBuffer*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_ClearFrameBuffer_U02(pInstance,pGFXClearFrame->StrAddr,pGFXClearFrame->length,pGFXClearFrame->ClearValue);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_LINE_PATTERN_RESET:
            MDrv_GE_ResetLinePattern(g_apiGFXLocal.g_pGEContext);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_LINE_PATTERN:
            CheckSize(pGFXMISCArg->u32Size, sizeof(GFX_SetLinePattern),eCmd);
            pGFXSetLinePattern = (GFX_SetLinePattern*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_Set_Line_Pattern_U02(pInstance,pGFXSetLinePattern->enable,pGFXSetLinePattern->linePattern,pGFXSetLinePattern->repeatFactor);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_BEGINE_DRAW:
            MApi_GFX_BeginDraw_U02(pInstance);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_END_DRAW:
            MApi_GFX_EndDraw_U02(pInstance);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_DEBUG_LEVEL:
            CheckSize(pGFXMISCArg->u32Size, sizeof(MS_U32),eCmd);
            pLevel = (MS_U32*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_SetDbgLevel_U02(pInstance,*pLevel);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_SET_HK:
            CheckSize(pGFXMISCArg->u32Size, sizeof(MS_BOOL),eCmd);
            pIsHK = (MS_BOOL*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_SetHK_U02(pInstance,*pIsHK);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_GET_HK:
            CheckSize(pGFXMISCArg->u32Size, sizeof(MS_BOOL),eCmd);
            pIsHK = (MS_BOOL*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_GetHK_U02(pInstance,pIsHK);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_PATCH_MODE:
            CheckSize(pGFXMISCArg->u32Size, sizeof(MS_BOOL),eCmd);
            pbPatch = (MS_BOOL*)pGFXMISCArg->pGFX_Info;
            g_apiGFXLocal._bPatchMode = *pbPatch;
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_STR_BLT_SCK_TYPE:
            CheckSize(pGFXMISCArg->u32Size, sizeof(GFX_SetStrBltSckType),eCmd);
            pStrBltSckType = (GFX_SetStrBltSckType*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_SetStrBltSckType_U02(pInstance,pStrBltSckType->type, &(pStrBltSckType->color));
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_SET_ALB_1555:
            CheckSize(pGFXMISCArg->u32Size, sizeof(MS_U8),eCmd);
            pALB1555 = (MS_U8*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_SetAlpha_ARGB1555_U02(pInstance,*pALB1555);
            u32Ret = GFX_SUCCESS;
            break;
        case E_MISC_GET_ALB_1555:
            CheckSize(pGFXMISCArg->u32Size, sizeof(MS_U8),eCmd);
            pALB1555 = (MS_U8*)pGFXMISCArg->pGFX_Info;
            MApi_GFX_GetAlpha_ARGB1555_U02(pInstance,pALB1555);
            u32Ret = GFX_SUCCESS;
            break;
        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, eCmd);
            u32Ret = GFX_FAIL;
            break;
    }
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), eCmd);

}

static int Ioctl_GFX_TLB(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE* psGFXInstPri   =NULL;
    GFX_TLB_ARGS* pGFXTLB               =NULL;
    EN_GFX_TLB_CMD                      en_cmd;
    MS_U32 u32Ret                       =GFX_FAIL;
    GFX_SetTLB_Addr* pGFX_SetTLB_Addr   =NULL;
    MS_BOOL* tlb_flush=FALSE;
    MS_U16* u16tag=0;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);

    pGFXTLB = (GFX_TLB_ARGS*)pArgs;
    en_cmd = pGFXTLB->en_cmd;
    CMD(en_cmd);

    switch(en_cmd)
    {
        case E_GFX_TLB_SET_MODE:
            CheckSize(pGFXTLB->u32Size, sizeof(GFX_TLB_ARGS), 0);
            g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode = pGFXTLB->TLB_mode;
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_TLB_SET_ADDR:
            CheckSize(pGFXTLB->u32Size, sizeof(GFX_SetTLB_Addr), 0);
            pGFX_SetTLB_Addr = (GFX_SetTLB_Addr*)pGFXTLB->psetting;
            g_apiGFXLocal.g_pGEContext->pBufInfo.tlbsrcaddr = pGFX_SetTLB_Addr->tlb_src_addr;
            g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr = pGFX_SetTLB_Addr->tlb_dst_addr;
            u32Ret = GFX_SUCCESS;
            break;

        case E_GFX_TLB_FLUSH_TABLE:
            CheckSize(pGFXTLB->u32Size, sizeof(MS_BOOL), 0);
            tlb_flush = (MS_BOOL*)pGFXTLB->psetting;
            g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable= *tlb_flush;
            break;

        case E_GFX_TLB_TAG:
            CheckSize(pGFXTLB->u32Size, sizeof(MS_U16), 0);
            u16tag = (MS_U16*)pGFXTLB->psetting;
            MDrv_GE_SetTLBTag(g_apiGFXLocal.g_pGEContext,*u16tag);
            break;

        default:
            printf("[%s][%d] Error Cmd=%d \n",__FUNCTION__,__LINE__, en_cmd);
            u32Ret = UTOPIA_STATUS_FAIL;
            break;
    }

    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);
}
static int Ioctl_GFX_DrawOval(void* pInstance, void* pArgs)
{
    GFX_INSTANT_PRIVATE* psGFXInstPri   =NULL;
    GFX_DRAW_OVAL_ARGS* pGFXDrawOval    =NULL;
    MS_U32 u32Ret                       =GFX_FAIL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGFXInstPri);
    API_GE_ENTRY(g_apiGFXLocal.g_pGEContext);

    pGFXDrawOval = (GFX_DRAW_OVAL_ARGS*)pArgs;
    CheckSize(pGFXDrawOval->u32Size, sizeof(GFX_OvalFillInfo), 0);

    MDrv_GE_SetTLBMode(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,TRUE);
    }

    if((g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_DST)||(g_apiGFXLocal.g_pGEContext->pBufInfo.tlbmode==E_GE_TLB_SRC_DST))
    {
        MDrv_GE_SetTLBDstBaseAddr(g_apiGFXLocal.g_pGEContext, g_apiGFXLocal.g_pGEContext->pBufInfo.tlbdstaddr);
    }

    u32Ret = MApi_GFX_DrawOval_U02(pInstance,(GFX_OvalFillInfo *)pGFXDrawOval->psetting);

    if(g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable)
    {
        MDrv_GE_TLBFlushTable(g_apiGFXLocal.g_pGEContext,FALSE);
        g_apiGFXLocal.g_pGEContext->pBufInfo.bEnflushtlbtable=FALSE;
    }
    API_GE_RETURN(g_apiGFXLocal.g_pGEContext, MapRet(u32Ret), 0);
}

MS_U32 GFXStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            printf("[%s][%d] Suspend Start!!!!!\n",__FUNCTION__,__LINE__);
            MDrv_GE_SetPowerState(NULL, u32PowerState, pModule);
            printf("[%s][%d] Suspend End!!!!!\n",__FUNCTION__,__LINE__);
            u32Return=UTOPIA_STATUS_SUCCESS;
            break;
        case E_POWER_RESUME:
            printf("[%s][%d] Resume Start!!!!!\n",__FUNCTION__,__LINE__);
            MDrv_GE_SetPowerState(NULL, u32PowerState, pModule);
            printf("[%s][%d] Resume End!!!!!\n",__FUNCTION__,__LINE__);
            u32Return=UTOPIA_STATUS_SUCCESS;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}

extern int suspend_gfx(void)
{
   void* pModule = NULL;
    UtopiaModuleGetPtr(MODULE_GFX,&pModule);
    GFXStr(E_POWER_SUSPEND, pModule);
    return 0;
}

extern int resume_gfx(void)
{
    void* pModule = NULL;
    UtopiaModuleGetPtr(MODULE_GFX,&pModule);
    GFXStr(E_POWER_RESUME, pModule);
    return 0;
}
#endif //UTOPIA_STRIP

void GFXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_GFX, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)GFXOpen, (FUtopiaClose)GFXClose, (FUtopiaIOctl)GFXIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule, (FUtopiaSTR)GFXStr);
    UtopiaModuleSetSTRPrivate(pUtopiaModule, sizeof(GE_STR_SAVE_AREA));
#endif

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, E_GE_POOL_ID_INTERNAL_REGISTER);
    UtopiaResourceCreate("ge0", sizeof(GFX_Resource_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, E_GE_POOL_ID_INTERNAL_REGISTER);
    UtopiaModuleAddResourceEnd(pUtopiaModule, E_GE_POOL_ID_INTERNAL_REGISTER);
}

MS_U32 GFXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    UtopiaInstanceCreate(sizeof(GFX_INSTANT_PRIVATE), ppInstance);
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GFXIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{

#ifndef UTOPIA_STRIP
    if(pInstance == NULL)
    {
        GFX_CRITIAL_MSG(printf("[%s:%s:%d]GFXIoctl's pInstance =NULL\n",__FILE__,__FUNCTION__,__LINE__));
        return UTOPIA_STATUS_ERR_NOT_AVAIL;
    }

    switch(u32Cmd)
    {
        case MAPI_CMD_GFX_INIT:
            Ioctl_GFX_Init(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_GET_INFO:
            Ioctl_GFX_GetInfo(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_GET_CAPS:
            Ioctl_GFX_GetCaps(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_LINEDRAW:
            Ioctl_GFX_LineDraw(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_RECTFILL:
            Ioctl_GFX_RectFill(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_BITBLT:
            Ioctl_GFX_BitBlt(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_TWOSRCBITBLT:
            Ioctl_GFX_TwoSrcBitBlt(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_SET_ABL:
            Ioctl_GFX_SetABL(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_SET_CONFIG:
            Ioctl_GFX_SetConfig(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_BITMAP:
            Ioctl_GFX_Bitmap(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_FONT:
            Ioctl_GFX_Font(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_POWERSTATE:
            Ioctl_GFX_PowerState(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_CB:
            Ioctl_GFX_CB(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_MISC:
            Ioctl_GFX_MISC(pInstance, pArgs);
            break;

        case MAPI_CMD_GFX_TLB:
            Ioctl_GFX_TLB(pInstance, pArgs);
            break;
        case MAPI_CMD_GFX_DRAW_OVAL:
            Ioctl_GFX_DrawOval(pInstance, pArgs);
            break;
        default:
            printf("[%s][%d]\n",__FUNCTION__,__LINE__);
            break;
    }
#endif //UTOPIA_STRIP
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GFXClose(void* pInstance)
{
    printf("\n[GFX INFO] GFX close");
    UtopiaInstanceDelete(pInstance);
    return TRUE;
}



