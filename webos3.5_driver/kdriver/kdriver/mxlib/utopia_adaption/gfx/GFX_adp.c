#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include "GFX_adp.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "apiGFX.h"
#include "apiGFX_v2.h"





//MAPI_CMD_GFX_INIT           ,
UADP_SPT_NAMEnNXT_DEF(GFX_INIT_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_INIT,1);
//MAPI_CMD_GFX_GET_INFO       ,
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_BUFFERINFO_A0_B1_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_BUFFERINFO_A0_B1,1);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_BUFFERINFO_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_BUFFERINFO_A0_B0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_BUFFERINFO_A0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_BUFFERINFO,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_TAGID_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_TAGID,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_NEXTTAGID_A0_B0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_NEXTTAGID_A0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_NEXTTAGID,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_CLIP_A0_B1,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_CLIP_A0_B0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_CLIP_A0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_CLIP,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_INTENSITY_A0_B0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_INTENSITY_A0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_INTENSITY,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_DBGINFO_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETINFO_DBGINFO,1);
//MAPI_CMD_GFX_LINEDRAW       ,
UADP_SPT_NAMEnNXT_DEF(GFX_LINEDRAW_A0_B1,0);
UADP_SPT_NAMEnNXT_DEF(GFX_LINEDRAW_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_LINEDRAW_A0_B0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_LINEDRAW_A0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_LINEDRAW,1);
//MAPI_CMD_GFX_RECTFILL       ,
UADP_SPT_NAMEnNXT_DEF(GFX_RECTFILL_A0_B1,0);
UADP_SPT_NAMEnNXT_DEF(GFX_RECTFILL_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_RECTFILL_A0_B0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_RECTFILL_A0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_RECTFILL,1);
//MAPI_CMD_GFX_BITBLT         ,
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B4,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B3,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B2,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B1_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B1,1);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0_B0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT_A0,5);
UADP_SPT_NAMEnNXT_DEF(GFX_BITBLT,1);
//MAPI_CMD_GFX_SET_ABL        ,
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_ABL_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_ABL,1);

UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0_B1_C1,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0_B1_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0_B1,2);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0_B0_C1,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0_B0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY_A0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_COLORKEY,1);

UADP_SPT_NAMEnNXT_DEF(GFX_ABL_DFB_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_DFB,1);

UADP_SPT_NAMEnNXT_DEF(GFX_ABL_ROP_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_ROP,1);

UADP_SPT_NAMEnNXT_DEF(GFX_ABL_INTENSITY_A0_B0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_INTENSITY_A0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_ABL_INTENSITY,1);
//MAPI_CMD_GFX_SET_CONFIG     ,
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_POWERON_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_POWERON,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_FLUSHQUEUE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_FLUSHQUEUE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_TAGID_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_TAGID,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_NEXTTAGID_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_NEXTTAGID,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_WAITFORTAGID_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_WAITFORTAGID,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ONEPIXELMODE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ONEPIXELMODE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_DITHER_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_DITHER,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_VQ_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_VQ,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_MIRROR_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_MIRROR,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_DST_MIRROR_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_DST_MIRROR,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ROTATE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ROTATE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ITALIC_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ITALIC,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_NEARESTMODE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_NEARESTMODE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_CLIP_A0_B1,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_CLIP_A0_B0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_CLIP_A0,2);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_CLIP,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_WRITEPROTECT_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_WRITEPROTECT,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ALPHA_COMPARE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_ALPHA_COMPARE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_CSC_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_SETCONFIG_CSC,1);
//MAPI_CMD_GFX_BITMAP         ,
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP_A0_B2,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP_A0_B1_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP_A0_B1,1);
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP_A0_B0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP_A0,3);
UADP_SPT_NAMEnNXT_DEF(GFX_BITMAP,1);
//MAPI_CMD_GFX_FONT           ,
//MAPI_CMD_GFX_POWERSTATE     ,
UADP_SPT_NAMEnNXT_DEF(GFX_POWERSTATE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_POWERSTATE,1);
//MAPI_CMD_GFX_CB             ,
//MAPI_CMD_GFX_MISC           ,
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_EXIT_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_EXIT,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_CLEAR_FRAME_BY_WORLD_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_CLEAR_FRAME_BY_WORLD,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_CLEAR_FRAME_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_CLEAR_FRAME,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_LINE_PATTERN_RESET_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_LINE_PATTERN_RESET,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_BEGINE_DRAW_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_BEGINE_DRAW,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_END_DRAW_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_END_DRAW,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_DEBUG_LEVEL_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_DEBUG_LEVEL,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_SET_HK_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_SET_HK,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_GET_HK_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_GET_HK,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_PATCH_MODE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_PATCH_MODE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_STR_BLT_SCK_TYPE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_STR_BLT_SCK_TYPE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_SET_ALB_1555_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_SET_ALB_1555,1);

UADP_SPT_NAMEnNXT_DEF(GFX_MISC_GET_ALB_1555_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_MISC_GET_ALB_1555,1);
//MAPI_CMD_GFX_TWOSRCBITBLT   ,
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B3,0);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B2_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B2,1);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B1_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B1,1);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B0_C0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0_B0,1);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT_A0,4);
UADP_SPT_NAMEnNXT_DEF(GFX_TWOSRC_BITBLT,1);
//MAPI_CMD_GFX_GET_CAPS
UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_MULTI_PIXEL_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_MULTI_PIXEL,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_BLT_DOWN_SCALE_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_BLT_DOWN_SCALE,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_DFB_BLENDING_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_DFB_BLENDING,1);

UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_ALIGN_A0,0);
UADP_SPT_NAMEnNXT_DEF(GFX_GETCAPS_ALIGN,1);

#define SPT(Name) spt_##Name


MS_U32 GFX_adp_Init(FUtopiaIOctl* pIoctl)
{
    //=======================================MAPI_CMD_GFX_INIT=======================================================//
    UADP_SPT_NAME0NXT(GFX_INIT_A0,GFX_Init_Config);
    UADP_SPT_NAME1NXT(GFX_INIT,GFX_INIT_ARGS,pGFX_Init,GFX_INIT_A0);
    //=======================================MAPI_CMD_GFX_GET_INFO===================================================//
    UADP_SPT_NAME0NXT(GFX_GETINFO_BUFFERINFO_A0_B1_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_GETINFO_BUFFERINFO_A0_B1,GFX_Dst_BufferInfo,pBufInfo,GFX_GETINFO_BUFFERINFO_A0_B1_C0);
    UADP_SPT_NAME0NXT(GFX_GETINFO_BUFFERINFO_A0_B0_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_GETINFO_BUFFERINFO_A0_B0,GFX_Src_BufferInfo,pBufInfo,GFX_GETINFO_BUFFERINFO_A0_B0_C0);
    UADP_SPT_NAME2NXT(GFX_GETINFO_BUFFERINFO_A0,GFX_Get_BufferInfo,pSrcbufInfo,GFX_GETINFO_BUFFERINFO_A0_B0,pDstbufInfo,GFX_GETINFO_BUFFERINFO_A0_B1);
    UADP_SPT_NAME1NXT(GFX_GETINFO_BUFFERINFO,GFX_GETINFO_ARGS,pGFX_GetInfo,GFX_GETINFO_BUFFERINFO_A0);

    UADP_SPT_NAME0NXT(GFX_GETINFO_TAGID_A0,MS_U16);
    UADP_SPT_NAME1NXT(GFX_GETINFO_TAGID,GFX_GETINFO_ARGS,pGFX_GetInfo,GFX_GETINFO_TAGID_A0);

    UADP_SPT_NAME0NXT(GFX_GETINFO_NEXTTAGID_A0_B0,MS_U16);
    UADP_SPT_NAME1NXT(GFX_GETINFO_NEXTTAGID_A0,GFX_Get_NextTagID,pTagID,GFX_GETINFO_NEXTTAGID_A0_B0);
    UADP_SPT_NAME1NXT(GFX_GETINFO_NEXTTAGID,GFX_GETINFO_ARGS,pGFX_GetInfo,GFX_GETINFO_NEXTTAGID_A0);

    UADP_SPT_NAME0NXT(GFX_GETINFO_CLIP_A0_B1,GFX_Point);
    UADP_SPT_NAME0NXT(GFX_GETINFO_CLIP_A0_B0,GFX_Point);
    UADP_SPT_NAME2NXT(GFX_GETINFO_CLIP_A0,GFX_Get_Clip_Property,pV0,GFX_GETINFO_CLIP_A0_B0,pV1,GFX_GETINFO_CLIP_A0_B1);
    UADP_SPT_NAME1NXT(GFX_GETINFO_CLIP,GFX_GETINFO_ARGS,pGFX_GetInfo,GFX_GETINFO_CLIP_A0);

    UADP_SPT_NAME0NXT(GFX_GETINFO_INTENSITY_A0_B0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_GETINFO_INTENSITY_A0,GFX_Get_Intensity,pColor,GFX_GETINFO_INTENSITY_A0_B0);
    UADP_SPT_NAME1NXT(GFX_GETINFO_INTENSITY,GFX_GETINFO_ARGS,pGFX_GetInfo,GFX_GETINFO_INTENSITY_A0);

    UADP_SPT_NAME0NXT(GFX_GETINFO_DBGINFO_A0,GFX_DbgInfo);
    UADP_SPT_NAME1NXT(GFX_GETINFO_DBGINFO,GFX_GETINFO_ARGS,pGFX_GetInfo,GFX_GETINFO_DBGINFO_A0);
    //=======================================MAPI_CMD_GFX_LINEDRAW===================================================//
    UADP_SPT_NAME0NXT(GFX_LINEDRAW_A0_B1,GFX_DrawLineInfo);
    UADP_SPT_NAME0NXT(GFX_LINEDRAW_A0_B0_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_LINEDRAW_A0_B0,GFX_Dst_BufferInfo,pBufInfo,GFX_LINEDRAW_A0_B0_C0);
    UADP_SPT_NAME2NXT(GFX_LINEDRAW_A0,GFX_Set_DrawLineInfo,pDstbufInfo,GFX_LINEDRAW_A0_B0,pDrawLineInfo,GFX_LINEDRAW_A0_B1);
    UADP_SPT_NAME1NXT(GFX_LINEDRAW,GFX_LINEDRAW_ARGS,pLineInfo,GFX_LINEDRAW_A0);
    //=======================================MAPI_CMD_GFX_RECTFILL===================================================//
    UADP_SPT_NAME0NXT(GFX_RECTFILL_A0_B1,GFX_RectFillInfo);
    UADP_SPT_NAME0NXT(GFX_RECTFILL_A0_B0_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_RECTFILL_A0_B0,GFX_Dst_BufferInfo,pBufInfo,GFX_RECTFILL_A0_B0_C0);
    UADP_SPT_NAME2NXT(GFX_RECTFILL_A0,GFX_Set_RectFillInfo,pDstbufInfo,GFX_RECTFILL_A0_B0,pRectFillInfo,GFX_RECTFILL_A0_B1);
    UADP_SPT_NAME1NXT(GFX_RECTFILL,GFX_RECTFILL_ARGS,pFillBlock,GFX_RECTFILL_A0);
    //=======================================MAPI_CMD_GFX_BITBLT===================================================//
    UADP_SPT_NAME0NXT(GFX_BITBLT_A0_B4,MS_U32);
    UADP_SPT_NAME0NXT(GFX_BITBLT_A0_B3,GFX_ScaleInfo);
    UADP_SPT_NAME0NXT(GFX_BITBLT_A0_B2,GFX_DrawRect);
    UADP_SPT_NAME0NXT(GFX_BITBLT_A0_B1_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_BITBLT_A0_B1,GFX_Dst_BufferInfo,pBufInfo,GFX_BITBLT_A0_B1_C0);
    UADP_SPT_NAME0NXT(GFX_BITBLT_A0_B0_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_BITBLT_A0_B0,GFX_Src_BufferInfo,pBufInfo,GFX_BITBLT_A0_B0_C0);
    UADP_SPT_NAME5NXT(GFX_BITBLT_A0,GFX_BitBltInfo,pSrcbufInfo,GFX_BITBLT_A0_B0,pDstbufInfo,GFX_BITBLT_A0_B1,pDrawRect,GFX_BITBLT_A0_B2,pScaleInfo,GFX_BITBLT_A0_B3,pIntensity,GFX_BITBLT_A0_B4);
    UADP_SPT_NAME1NXT(GFX_BITBLT,GFX_BITBLT_ARGS,pGFX_BitBlt,GFX_BITBLT_A0);
    //=======================================MAPI_CMD_GFX_SET_ABL===================================================//
    //ABL
    UADP_SPT_NAME0NXT(GFX_ABL_ABL_A0,GFX_Set_ABL);
    UADP_SPT_NAME1NXT(GFX_ABL_ABL,GFX_ABL_ARGS,pGFX_ABL,GFX_ABL_ABL_A0);
    //ColorKey
    /*UADP_SPT_NAME0NXT(GFX_ABL_COLORKEY_A0_B1_C1,GFX_BlinkData);
    UADP_SPT_NAME0NXT(GFX_ABL_COLORKEY_A0_B1_C0,GFX_BlinkData);
    UADP_SPT_NAME2NXT(GFX_ABL_COLORKEY_A0_B1,GFX_Set_Dst_ColorKey,pS_color,GFX_ABL_COLORKEY_A0_B1_C0,pE_color,GFX_ABL_COLORKEY_A0_B1_C1);
    UADP_SPT_NAME0NXT(GFX_ABL_COLORKEY_A0_B0_C1,GFX_BlinkData);
    UADP_SPT_NAME0NXT(GFX_ABL_COLORKEY_A0_B0_C0,GFX_BlinkData);
    UADP_SPT_NAME2NXT(GFX_ABL_COLORKEY_A0_B0,GFX_Set_Src_ColorKey,pS_color,GFX_ABL_COLORKEY_A0_B0_C0,pE_color,GFX_ABL_COLORKEY_A0_B0_C1);
    UADP_SPT_NAME2NXT(GFX_ABL_COLORKEY_A0,GFX_Set_Colorkey,pSrcbufInfo,GFX_ABL_COLORKEY_A0_B0,pDstbufInfo,GFX_ABL_COLORKEY_A0_B1);
    UADP_SPT_NAME1NXT(GFX_ABL_COLORKEY,GFX_ABL_ARGS,pGFX_ABL,GFX_ABL_COLORKEY_A0);*/
    //DFB
    UADP_SPT_NAME0NXT(GFX_ABL_DFB_A0,GFX_Set_DFB);
    UADP_SPT_NAME1NXT(GFX_ABL_DFB,GFX_ABL_ARGS,pGFX_ABL,GFX_ABL_DFB_A0);
    //ROP
    UADP_SPT_NAME0NXT(GFX_ABL_ROP_A0,GFX_Set_DFB);
    UADP_SPT_NAME1NXT(GFX_ABL_ROP,GFX_ABL_ARGS,pGFX_ABL,GFX_ABL_ROP_A0);
    //INTENSITY
    UADP_SPT_NAME0NXT(GFX_ABL_INTENSITY_A0_B0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_ABL_INTENSITY_A0,GFX_Set_Intensity,pColor,GFX_ABL_INTENSITY_A0_B0);
    UADP_SPT_NAME1NXT(GFX_ABL_INTENSITY,GFX_ABL_ARGS,pGFX_ABL,GFX_ABL_INTENSITY_A0);
    //=======================================MAPI_CMD_GFX_SET_CONFIG===================================================/
    UADP_SPT_NAME0NXT(GFX_SETCONFIG_POWERON_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_POWERON,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_POWERON_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_FLUSHQUEUE_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_FLUSHQUEUE,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_FLUSHQUEUE_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_TAGID_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_TAGID,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_TAGID_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_NEXTTAGID_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_NEXTTAGID,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_NEXTTAGID_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_WAITFORTAGID_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_WAITFORTAGID,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_WAITFORTAGID_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_ONEPIXELMODE_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_ONEPIXELMODE,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_ONEPIXELMODE_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_DITHER_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_DITHER,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_DITHER_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_VQ_A0,GFX_Set_VQ);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_VQ,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_VQ_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_MIRROR_A0,GFX_Set_Mirror);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_MIRROR,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_MIRROR_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_DST_MIRROR_A0,GFX_Set_Mirror);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_DST_MIRROR,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_DST_MIRROR_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_ROTATE_A0,GFX_RotateAngle);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_ROTATE,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_ROTATE_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_ITALIC_A0,GFX_Set_Italic);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_ITALIC,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_ITALIC_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_NEARESTMODE_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_NEARESTMODE,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_NEARESTMODE_A0);

//    UADP_SPT_NAME0NXT(GFX_SETCONFIG_PALETTEOPT_A0_B0,GFX_PaletteEntry);
//    UADP_SPT_NAME1NXT(GFX_SETCONFIG_PALETTEOPT_A0,GFX_Set_PaletteOpt, pPalArray, GFX_SETCONFIG_PALETTEOPT_A0_B0);
//    UADP_SPT_NAME1NXT(GFX_SETCONFIG_PALETTEOPT,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_PALETTEOPT_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_CLIP_A0_B1,GFX_Point);
    UADP_SPT_NAME0NXT(GFX_SETCONFIG_CLIP_A0_B0,GFX_Point);
    UADP_SPT_NAME2NXT(GFX_SETCONFIG_CLIP_A0,GFX_Set_Clip_Property,pV0,GFX_SETCONFIG_CLIP_A0_B0,pV1,GFX_SETCONFIG_CLIP_A0_B1);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_CLIP,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_CLIP_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_WRITEPROTECT_A0,GFX_Set_WriteProtect);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_WRITEPROTECT,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_WRITEPROTECT_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_ALPHA_COMPARE_A0,GFX_Set_AlphaCompare);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_ALPHA_COMPARE,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_ALPHA_COMPARE_A0);

    UADP_SPT_NAME0NXT(GFX_SETCONFIG_CSC_A0,GFX_Set_CSC);
    UADP_SPT_NAME1NXT(GFX_SETCONFIG_CSC,GFX_SETCONFIG_ARGS,pGFX_ConfigInfo,GFX_SETCONFIG_CSC_A0);
    //=======================================MAPI_CMD_GFX_BITMAP===================================================/
    UADP_SPT_NAME0NXT(GFX_BITMAP_A0_B2,GFX_DrawBmpInfo);
    UADP_SPT_NAME0NXT(GFX_BITMAP_A0_B1_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_BITMAP_A0_B1,GFX_Dst_BufferInfo,pBufInfo,GFX_BITMAP_A0_B1_C0);
    UADP_SPT_NAME0NXT(GFX_BITMAP_A0_B0_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_BITMAP_A0_B0,GFX_Src_BufferInfo,pBufInfo,GFX_BITMAP_A0_B0_C0);
    UADP_SPT_NAME3NXT(GFX_BITMAP_A0,GFX_Bitmap,pSrcbufInfo,GFX_BITMAP_A0_B0,pDstbufInfo,GFX_BITMAP_A0_B1,pBmpInfo,GFX_BITMAP_A0_B2);
    UADP_SPT_NAME1NXT(GFX_BITMAP,GFX_BITMAP_ARGS,pGFX_Bitmap,GFX_BITMAP_A0);
    //=======================================MAPI_CMD_GFX_FONT===================================================/
        //This is for Nos, do not need implement
    //=======================================MAPI_CMD_GFX_POWERSTATE===================================================/
    UADP_SPT_NAME0NXT(GFX_POWERSTATE_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_POWERSTATE,GFX_POWERSTATE_ARGS,pGFX_PowerState,GFX_POWERSTATE_A0);
    //MAPI_CMD_GFX_CB             ,

    //=======================================MAPI_CMD_GFX_MISC===================================================/
    UADP_SPT_NAME0NXT(GFX_MISC_EXIT_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_MISC_EXIT,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_EXIT_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_CLEAR_FRAME_BY_WORLD_A0,GFX_ClearFrameBuffer);
    UADP_SPT_NAME1NXT(GFX_MISC_CLEAR_FRAME_BY_WORLD,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_CLEAR_FRAME_BY_WORLD_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_CLEAR_FRAME_A0,GFX_ClearFrameBuffer);
    UADP_SPT_NAME1NXT(GFX_MISC_CLEAR_FRAME,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_CLEAR_FRAME_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_LINE_PATTERN_RESET_A0,GFX_SetLinePattern);
    UADP_SPT_NAME1NXT(GFX_MISC_LINE_PATTERN_RESET,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_LINE_PATTERN_RESET_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_BEGINE_DRAW_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_MISC_BEGINE_DRAW,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_BEGINE_DRAW_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_END_DRAW_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_MISC_END_DRAW,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_END_DRAW_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_DEBUG_LEVEL_A0,MS_U32);
    UADP_SPT_NAME1NXT(GFX_MISC_DEBUG_LEVEL,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_DEBUG_LEVEL_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_SET_HK_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_MISC_SET_HK,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_SET_HK_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_GET_HK_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_MISC_GET_HK,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_GET_HK_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_PATCH_MODE_A0,MS_BOOL);
    UADP_SPT_NAME1NXT(GFX_MISC_PATCH_MODE,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_PATCH_MODE_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_STR_BLT_SCK_TYPE_A0,GFX_SetStrBltSckType);
    UADP_SPT_NAME1NXT(GFX_MISC_STR_BLT_SCK_TYPE,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_STR_BLT_SCK_TYPE_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_SET_ALB_1555_A0,MS_U8);
    UADP_SPT_NAME1NXT(GFX_MISC_SET_ALB_1555,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_SET_ALB_1555_A0);

    UADP_SPT_NAME0NXT(GFX_MISC_GET_ALB_1555_A0,MS_U8);
    UADP_SPT_NAME1NXT(GFX_MISC_GET_ALB_1555,GFX_MISC_ARGS,pGFX_Info,GFX_MISC_GET_ALB_1555_A0);
    //=======================================MAPI_CMD_GFX_TWOSRCBITBLT===================================================/
    UADP_SPT_NAME0NXT(GFX_TWOSRC_BITBLT_A0_B3,GFX_DrawTwoSourceRect);
    UADP_SPT_NAME0NXT(GFX_TWOSRC_BITBLT_A0_B2_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_TWOSRC_BITBLT_A0_B2,GFX_Dst_BufferInfo,pBufInfo,GFX_TWOSRC_BITBLT_A0_B2_C0);
    UADP_SPT_NAME0NXT(GFX_TWOSRC_BITBLT_A0_B1_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_TWOSRC_BITBLT_A0_B1,GFX_Src_BufferInfo,pBufInfo,GFX_TWOSRC_BITBLT_A0_B1_C0);
    UADP_SPT_NAME0NXT(GFX_TWOSRC_BITBLT_A0_B0_C0,GFX_BufferInfo);
    UADP_SPT_NAME1NXT(GFX_TWOSRC_BITBLT_A0_B0,GFX_Src_BufferInfo,pBufInfo,GFX_TWOSRC_BITBLT_A0_B0_C0);
    UADP_SPT_NAME4NXT(GFX_TWOSRC_BITBLT_A0,GFX_TwoSrc_BitBltInfo,pSrcbufInfo_0,GFX_TWOSRC_BITBLT_A0_B0,pSrcbufInfo_1,GFX_TWOSRC_BITBLT_A0_B1,pDstbufInfo,GFX_TWOSRC_BITBLT_A0_B2,pDrawTwoSrcRect,GFX_TWOSRC_BITBLT_A0_B3);
    UADP_SPT_NAME1NXT(GFX_TWOSRC_BITBLT,GFX_TWOSRC_BITBLT_ARGS,pGFX_TwoSrc_BitBlt,GFX_TWOSRC_BITBLT_A0);
    //=======================================MAPI_CMD_GFX_TLB===========================================================/
    //UADP_SPT_NAME0NXT(GFX_TLB_SET_MODE_A0,MS_U32);
    //UADP_SPT_NAME1NXT(GFX_TLB_SET_MODE,GFX_TLB_ARGS,psetting,GFX_TLB_SET_MODE_A0);
    //=======================================MAPI_CMD_GFX_GET_CAPS======================================================/
    UADP_SPT_NAME0NXT(GFX_GETCAPS_MULTI_PIXEL_A0,GFX_CapMultiPixelInfo);
    UADP_SPT_NAME1NXT(GFX_GETCAPS_MULTI_PIXEL,GFX_GETCAPS_ARGS,pGFX_GetCaps,GFX_GETCAPS_MULTI_PIXEL_A0);

    UADP_SPT_NAME0NXT(GFX_GETCAPS_BLT_DOWN_SCALE_A0,GFX_BLT_DownScaleInfo);
    UADP_SPT_NAME1NXT(GFX_GETCAPS_BLT_DOWN_SCALE,GFX_GETCAPS_ARGS,pGFX_GetCaps,GFX_GETCAPS_BLT_DOWN_SCALE_A0);

    UADP_SPT_NAME0NXT(GFX_GETCAPS_DFB_BLENDING_A0,MS_U16);
    UADP_SPT_NAME1NXT(GFX_GETCAPS_DFB_BLENDING,GFX_GETCAPS_ARGS,pGFX_GetCaps,GFX_GETCAPS_DFB_BLENDING_A0);

    UADP_SPT_NAME0NXT(GFX_GETCAPS_ALIGN_A0,GFX_FmtAlignCapsInfo);
    UADP_SPT_NAME1NXT(GFX_GETCAPS_ALIGN,GFX_GETCAPS_ARGS,pGFX_GetCaps,GFX_GETCAPS_ALIGN_A0);

    *pIoctl= (FUtopiaIOctl)GFX_adp_Ioctl;
    return 0;
}

/*
MS_U32 AESDMA_DDI_Open(void* pInstant,const void* const pAttribute)
{
    return UtopiaOpen(MODULE_AESDMA,pInstant,0,pAttribute);
}
*/
MS_U32 GFX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret =0;
    char buffer_arg[1];

    if (!access_ok(VERIFY_WRITE, (void *)pArgs, sizeof(*pArgs)))
    {
        printk("[%s] with bad address with cmd 0x%lx!\n", __func__, u32Cmd);
        return -EFAULT;
    }

   //printk("\33[0;31m [Start][%s][%d]u32Cmd=%d \33[m;\n",__FUNCTION__,__LINE__,u32Cmd);
    switch(u32Cmd)
    {
        case MAPI_CMD_GFX_INIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_INIT),NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MAPI_CMD_GFX_GET_INFO:
            {
                GFX_GETINFO_ARGS    str;
                if(copy_from_user(&str, pArgs, sizeof(GFX_GETINFO_ARGS)))
                {
                    return UTOPIA_STATUS_FAIL;
                }
                switch(str.eGFX_GetConfig)
                {
                    case E_GFX_GET_BUFFERINFO:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETINFO_BUFFERINFO),SPT(GFX_GETINFO_BUFFERINFO), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_GET_TAGID:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETINFO_TAGID),SPT(GFX_GETINFO_TAGID), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_GET_NEXTTAGID:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETINFO_NEXTTAGID),SPT(GFX_GETINFO_NEXTTAGID), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_GET_CLIP:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETINFO_CLIP),SPT(GFX_GETINFO_CLIP), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_GET_INTENSITY:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETINFO_INTENSITY),SPT(GFX_GETINFO_INTENSITY), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_GET_DBGINFO:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETINFO_DBGINFO),SPT(GFX_GETINFO_DBGINFO), buffer_arg,sizeof(buffer_arg));
                        break;
                    default:
                        break;
                }
                break;
            }
        case MAPI_CMD_GFX_LINEDRAW:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_LINEDRAW),NULL, buffer_arg,sizeof(buffer_arg));
            break;

        case MAPI_CMD_GFX_RECTFILL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_RECTFILL),NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case MAPI_CMD_GFX_BITBLT:
            {
            GFX_BITBLT_ARGS GFX_BITBLT;
            GFX_BitBltInfo GFX_BITBLT_A0;
            GFX_Src_BufferInfo GFX_BITBLT_A0B0;
            GFX_Dst_BufferInfo GFX_BITBLT_A0B1;
            GFX_DrawRect GFX_BITBLT_A0B2;
            GFX_ScaleInfo GFX_BITBLT_A0B3;
            MS_U32 GFX_BITBLT_A0_B4;

            GFX_BITBLT_ARGS* pGFX_BITBLT_Usr=NULL;
            GFX_BitBltInfo* pGFX_BITBLT_UsrA0=NULL;
            GFX_Src_BufferInfo* pGFX_BITBLT_UsrA0B0=NULL;
            GFX_Dst_BufferInfo* pGFX_BITBLT_UsrA0B1=NULL;
            GFX_DrawRect* pGFX_BITBLT_UsrA0B2=NULL;
            GFX_ScaleInfo* pGFX_BITBLT_UsrA0B3=NULL;

            pGFX_BITBLT_Usr=(GFX_BITBLT_ARGS*)pArgs;
            pGFX_BITBLT_UsrA0=pGFX_BITBLT_Usr->pGFX_BitBlt;
            pGFX_BITBLT_UsrA0B0=pGFX_BITBLT_UsrA0->pSrcbufInfo;
            pGFX_BITBLT_UsrA0B1=pGFX_BITBLT_UsrA0->pDstbufInfo;
            pGFX_BITBLT_UsrA0B2=pGFX_BITBLT_UsrA0->pDrawRect;
            pGFX_BITBLT_UsrA0B3=pGFX_BITBLT_UsrA0->pScaleInfo;

            GFX_BITBLT.pGFX_BitBlt = &GFX_BITBLT_A0;
            GFX_BITBLT_A0.pSrcbufInfo = &GFX_BITBLT_A0B0;
            GFX_BITBLT_A0.pDstbufInfo = &GFX_BITBLT_A0B1;
            GFX_BITBLT_A0.pDrawRect = &GFX_BITBLT_A0B2;
            if(copy_from_user(&GFX_BITBLT, pArgs, sizeof(GFX_BITBLT_ARGS)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            if(copy_from_user(&GFX_BITBLT_A0, pGFX_BITBLT_Usr->pGFX_BitBlt, sizeof(GFX_BitBltInfo)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            if(copy_from_user(&GFX_BITBLT_A0B0, pGFX_BITBLT_UsrA0->pSrcbufInfo, sizeof(GFX_Src_BufferInfo)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            if(copy_from_user(&GFX_BITBLT_A0B1, pGFX_BITBLT_UsrA0->pDstbufInfo, sizeof(GFX_Dst_BufferInfo)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            if(copy_from_user(&GFX_BITBLT_A0B2, pGFX_BITBLT_UsrA0->pDrawRect, sizeof(GFX_DrawRect)))
            {
                return UTOPIA_STATUS_FAIL;
            }

            if(pGFX_BITBLT_UsrA0->pScaleInfo !=NULL)
            {
                GFX_BITBLT_A0.pScaleInfo = &GFX_BITBLT_A0B3;
                if(copy_from_user(&GFX_BITBLT_A0B3, pGFX_BITBLT_UsrA0->pScaleInfo, sizeof(GFX_ScaleInfo)))
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }
            else{
                GFX_BITBLT_A0.pScaleInfo = NULL;
            }
            if(pGFX_BITBLT_UsrA0->pIntensity!=NULL)
            {
                GFX_BITBLT_A0.pIntensity = &GFX_BITBLT_A0_B4;
                if(copy_from_user(&GFX_BITBLT_A0_B4, pGFX_BITBLT_UsrA0->pIntensity, sizeof(unsigned long)))
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }
            else
            {
                GFX_BITBLT_A0.pIntensity = NULL;
            }

            u32Ret=UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)&GFX_BITBLT);
        }
        break;
        /*    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_BITBLT),NULL, buffer_arg,sizeof(buffer_arg));

        }*/
        case MAPI_CMD_GFX_SET_ABL:
            {
                GFX_ABL_ARGS    str;

                GFX_ABL_ARGS GFX_ABL_COLORKEY;
                GFX_Set_Colorkey GFX_ABL_COLORKEY_A0;
                GFX_Set_Src_ColorKey GFX_ABL_COLORKEY_A0B0;
                GFX_Set_Dst_ColorKey GFX_ABL_COLORKEY_A0B1;
                GFX_BlinkData GFX_ABL_COLORKEY_A0B0C0S;
                GFX_BlinkData GFX_ABL_COLORKEY_A0B0C0E;
                GFX_BlinkData GFX_ABL_COLORKEY_A0B0C1S;
                GFX_BlinkData GFX_ABL_COLORKEY_A0B0C1E;
                GFX_RgbColor GFX_ABL_COLORKEY_A0B0C2S;
                GFX_RgbColor GFX_ABL_COLORKEY_A0B0C2E;
                GFX_RgbColor GFX_ABL_COLORKEY_A0B0C3S;
                GFX_RgbColor GFX_ABL_COLORKEY_A0B0C3E;

                GFX_ABL_ARGS* pGFX_ABL_COLORKEY_Usr=NULL;
                GFX_Set_Colorkey* pGFX_ABL_COLORKEY_UsrA0=NULL;
                GFX_Set_Src_ColorKey* pGFX_ABL_COLORKEY_UsrA0B0=NULL;
                GFX_Set_Dst_ColorKey* pGFX_ABL_COLORKEY_UsrA0B1=NULL;

                if(copy_from_user(&str, pArgs, sizeof(GFX_ABL_ARGS)))
                {
                    return UTOPIA_STATUS_FAIL;
                }

                switch(str.eGFX_SetABL)
                {
                    case E_GFX_SET_ABL:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_ABL_ABL),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_COLORKEY:
                        pGFX_ABL_COLORKEY_Usr=(GFX_ABL_ARGS*)pArgs;
                        pGFX_ABL_COLORKEY_UsrA0 = pGFX_ABL_COLORKEY_Usr->pGFX_ABL;
                        pGFX_ABL_COLORKEY_UsrA0B0 = pGFX_ABL_COLORKEY_UsrA0->pSrcbufInfo;
                        pGFX_ABL_COLORKEY_UsrA0B1 = pGFX_ABL_COLORKEY_UsrA0->pDstbufInfo;

                        if(copy_from_user(&GFX_ABL_COLORKEY, pArgs, sizeof(GFX_ABL_ARGS)))
                        {
                            return UTOPIA_STATUS_FAIL;
                        }
                        GFX_ABL_COLORKEY.pGFX_ABL = &GFX_ABL_COLORKEY_A0;
                        if(copy_from_user(&GFX_ABL_COLORKEY_A0, pGFX_ABL_COLORKEY_Usr->pGFX_ABL, sizeof(GFX_Set_Colorkey)))
                        {
                            return UTOPIA_STATUS_FAIL;
                        }

                        if(pGFX_ABL_COLORKEY_UsrA0->ColorKey_Type & E_SRC_COLORKEY)
                        {
                            GFX_ABL_COLORKEY_A0.pSrcbufInfo=&GFX_ABL_COLORKEY_A0B0;
                            if(copy_from_user(&GFX_ABL_COLORKEY_A0B0, pGFX_ABL_COLORKEY_UsrA0->pSrcbufInfo, sizeof(GFX_Set_Src_ColorKey)))
                            {
                                return UTOPIA_STATUS_FAIL;
                            }

                            if(  (pGFX_ABL_COLORKEY_UsrA0B0->eFmt == GFX_FMT_1ABFGBG12355) || (pGFX_ABL_COLORKEY_UsrA0B0->eFmt == GFX_FMT_FABAFGBG2266))
                            {
                                GFX_ABL_COLORKEY_A0B0.pS_color = &GFX_ABL_COLORKEY_A0B0C0S;
                                GFX_ABL_COLORKEY_A0B0.pE_color = &GFX_ABL_COLORKEY_A0B0C0E;
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C0S, pGFX_ABL_COLORKEY_UsrA0B0->pS_color, sizeof(GFX_BlinkData)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C0E, pGFX_ABL_COLORKEY_UsrA0B0->pE_color, sizeof(GFX_BlinkData)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                            }else
                            {
                                GFX_ABL_COLORKEY_A0B0.pS_color = &GFX_ABL_COLORKEY_A0B0C2S;
                                GFX_ABL_COLORKEY_A0B0.pE_color = &GFX_ABL_COLORKEY_A0B0C2E;
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C2S, pGFX_ABL_COLORKEY_UsrA0B0->pS_color, sizeof(GFX_RgbColor)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C2E, pGFX_ABL_COLORKEY_UsrA0B0->pE_color, sizeof(GFX_RgbColor)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                            }

                        }


                        if(pGFX_ABL_COLORKEY_UsrA0->ColorKey_Type & E_DST_COLORKEY)
                        {
                            GFX_ABL_COLORKEY_A0.pDstbufInfo=&GFX_ABL_COLORKEY_A0B1;
                            if(copy_from_user(&GFX_ABL_COLORKEY_A0B1, pGFX_ABL_COLORKEY_UsrA0->pDstbufInfo, sizeof(GFX_Set_Dst_ColorKey)))
                            {
                                return UTOPIA_STATUS_FAIL;
                            }

                            if( (pGFX_ABL_COLORKEY_UsrA0B1->eFmt == GFX_FMT_1ABFGBG12355) || (pGFX_ABL_COLORKEY_UsrA0B1->eFmt == GFX_FMT_FABAFGBG2266))
                            {
                                GFX_ABL_COLORKEY_A0B1.pS_color = &GFX_ABL_COLORKEY_A0B0C1S;
                                GFX_ABL_COLORKEY_A0B1.pE_color = &GFX_ABL_COLORKEY_A0B0C1E;
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C1S, pGFX_ABL_COLORKEY_UsrA0B1->pS_color, sizeof(GFX_BlinkData)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C1E, pGFX_ABL_COLORKEY_UsrA0B1->pE_color, sizeof(GFX_BlinkData)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                            }else
                            {
                                GFX_ABL_COLORKEY_A0B1.pS_color = &GFX_ABL_COLORKEY_A0B0C3S;
                                GFX_ABL_COLORKEY_A0B1.pE_color = &GFX_ABL_COLORKEY_A0B0C3E;
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C3S, pGFX_ABL_COLORKEY_UsrA0B1->pS_color, sizeof(GFX_RgbColor)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                                if(copy_from_user(&GFX_ABL_COLORKEY_A0B0C3E, pGFX_ABL_COLORKEY_UsrA0B1->pE_color, sizeof(GFX_RgbColor)))
                                {
                                    return UTOPIA_STATUS_FAIL;
                                }
                            }
                        }

                        u32Ret=UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)&GFX_ABL_COLORKEY);
                        //u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_ABL_COLORKEY),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_ROP:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_ABL_ROP),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_DFB:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_ABL_DFB),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_INTENSITY:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_ABL_INTENSITY),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    default:
                        break;
                }
                break;
            }
        case MAPI_CMD_GFX_SET_CONFIG:
            {
                GFX_SETCONFIG_ARGS  str;

                GFX_SETCONFIG_ARGS PaletteOpt;
                GFX_Set_PaletteOpt PaletteOptA0;
                //GFX_PaletteEntry PaletteOptA0B0[256];
                GFX_PaletteEntry *PaletteOptA0B0 = NULL;
                GFX_SETCONFIG_ARGS* pPaletteOptUsr =NULL;
                GFX_Set_PaletteOpt* pPaletteOptUsrA0=NULL;

                if(copy_from_user(&str, pArgs, sizeof(GFX_SETCONFIG_ARGS)))
                {
                    return UTOPIA_STATUS_FAIL;
                }
                //printk("%s(),%d, CMD = %d",__FUNCTION__,__LINE__,str.eGFX_SetConfig);
                switch(str.eGFX_SetConfig)
                {
                    case E_GFX_SET_POWERON:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_POWERON),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_FLUSHQUEUE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_FLUSHQUEUE),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_TAGID:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_TAGID),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_NEXTTAGID:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_NEXTTAGID),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_WAITFORTAGID:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_WAITFORTAGID),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_ONEPIXELMODE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_ONEPIXELMODE),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_DITHER:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_DITHER),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_VQ:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_VQ),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_MIRROR:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_MIRROR),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_DST_MIRROR:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_DST_MIRROR),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_ROTATE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_ROTATE),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_ITALIC:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_ITALIC),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_NEARESTMODE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_NEARESTMODE),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_PALETTEOPT:
                        pPaletteOptUsr=(GFX_SETCONFIG_ARGS*)pArgs;
                        pPaletteOptUsrA0 = pPaletteOptUsr->pGFX_ConfigInfo;

                        if(copy_from_user(&PaletteOpt, pArgs, sizeof(GFX_SETCONFIG_ARGS)))
                        {
                            printk( "%s(),%d \n",__FUNCTION__,__LINE__);
                            return UTOPIA_STATUS_FAIL;
                        }
                        PaletteOpt.pGFX_ConfigInfo = &PaletteOptA0;
                        if(copy_from_user(&PaletteOptA0, pPaletteOptUsr->pGFX_ConfigInfo, sizeof(GFX_Set_PaletteOpt)))
                        {
                            printk( "%s(),%d \n",__FUNCTION__,__LINE__);
                            return UTOPIA_STATUS_FAIL;
                        }
                        PaletteOptA0B0 = malloc(sizeof(GFX_PaletteEntry)*256);

                        if(PaletteOptA0B0 != NULL)
                        {
                            memset(PaletteOptA0B0,0x0,sizeof(GFX_PaletteEntry)*256);
                        }
                        else
                        {
                            return UTOPIA_STATUS_FAIL;
                        }
                        PaletteOptA0.pPalArray = PaletteOptA0B0;
                        if(PaletteOptA0.u32PalEnd > 255 || PaletteOptA0.u32PalStart > PaletteOptA0.u32PalEnd  )
                        {
                            printk( "%s(),%d, pPaletteOptUsr = 0x%lx, cmd = %d,start = %d, end = %d \n",__FUNCTION__,__LINE__ ,pPaletteOptUsr->u32Size,pPaletteOptUsr->eGFX_SetConfig,pPaletteOptUsrA0->u32PalStart,pPaletteOptUsrA0->u32PalEnd);
                            PaletteOptA0.u32PalStart= 0;
                            PaletteOptA0.u32PalEnd = 255;
                        }
                        if(copy_from_user(PaletteOptA0B0, pPaletteOptUsrA0->pPalArray, sizeof(GFX_PaletteEntry) * (PaletteOptA0.u32PalEnd - PaletteOptA0.u32PalStart + 1)))
                        {
                            printk( "%s(),%d \n",__FUNCTION__,__LINE__);
                            if(PaletteOptA0B0 != NULL)
                            {
                                free(PaletteOptA0B0);
                                PaletteOptA0B0 = NULL;
                            }
                            return UTOPIA_STATUS_FAIL;
                        }
                        u32Ret=UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)&PaletteOpt);
                        if(PaletteOptA0B0 != NULL)
                        {
                            free(PaletteOptA0B0);
                            PaletteOptA0B0 = NULL;
                        }
                        break;
                    case E_GFX_SET_CLIP:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_CLIP),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_WRITEPROTECT:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_WRITEPROTECT),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_ALPHA_COMPARE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_ALPHA_COMPARE),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_SET_CSC:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_SETCONFIG_CSC),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    default:
                        break;
                }
                break;
            }
        case MAPI_CMD_GFX_BITMAP:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_BITMAP),SPT(GFX_BITMAP), buffer_arg,sizeof(buffer_arg));
            break;
        case MAPI_CMD_GFX_FONT:
            //This is for Nos, do not need implement
            break;
        case MAPI_CMD_GFX_POWERSTATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_POWERSTATE),SPT(GFX_POWERSTATE), buffer_arg,sizeof(buffer_arg));
            break;
        case MAPI_CMD_GFX_CB:
            break;
        case MAPI_CMD_GFX_TWOSRCBITBLT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_TWOSRC_BITBLT),SPT(GFX_TWOSRC_BITBLT), buffer_arg,sizeof(buffer_arg));
            break;
        case MAPI_CMD_GFX_TLB:
            {
                GFX_TLB_ARGS GFX_TLB_SET_MODE;
                MS_U32       GFX_TLB_SET_MODEA0;

                GFX_TLB_ARGS* pGFX_TLB_SET_MODEUsr=&GFX_TLB_SET_MODE;

                if(copy_from_user(&GFX_TLB_SET_MODE, pArgs, sizeof(GFX_TLB_ARGS)))
                {
                    return UTOPIA_STATUS_FAIL;
                }

                if(pGFX_TLB_SET_MODEUsr->psetting!=NULL)
                {
                    GFX_TLB_SET_MODE.psetting = &GFX_TLB_SET_MODEA0;
                    if(copy_from_user(&GFX_TLB_SET_MODEA0, pGFX_TLB_SET_MODEUsr->psetting, sizeof(MS_U32)))
                    {
                        return UTOPIA_STATUS_FAIL;
                    }
                }else
                {
                    GFX_TLB_SET_MODE.psetting = NULL;
                }
                u32Ret=UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)&GFX_TLB_SET_MODE);
            break;
            }
        case MAPI_CMD_GFX_MISC:
            {
                GFX_MISC_ARGS   str;
                if(copy_from_user(&str, pArgs, sizeof(GFX_MISC_ARGS)))
                {
                    return UTOPIA_STATUS_FAIL;
                }

                switch(str.eGFX_MISCType)
                {
                    case E_MISC_EXIT:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_EXIT),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_CLEAR_FRAME_BY_WORLD:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_CLEAR_FRAME_BY_WORLD),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_CLEAR_FRAME:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_CLEAR_FRAME),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_LINE_PATTERN_RESET:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_LINE_PATTERN_RESET),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_LINE_PATTERN:
                        u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
                        break;
                    case E_MISC_BEGINE_DRAW:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_BEGINE_DRAW),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_END_DRAW:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_END_DRAW),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_DEBUG_LEVEL:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_DEBUG_LEVEL),SPT(GFX_MISC_DEBUG_LEVEL), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_SET_HK:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_SET_HK),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_GET_HK:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_GET_HK),SPT(GFX_MISC_GET_HK), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_PATCH_MODE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_PATCH_MODE),SPT(GFX_MISC_PATCH_MODE), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_STR_BLT_SCK_TYPE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_STR_BLT_SCK_TYPE),SPT(GFX_MISC_STR_BLT_SCK_TYPE), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_SET_ALB_1555:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_SET_ALB_1555),NULL, buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_MISC_GET_ALB_1555:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_MISC_GET_ALB_1555),SPT(GFX_MISC_GET_ALB_1555), buffer_arg,sizeof(buffer_arg));
                        break;
                    default:
                        break;
                }

                break;
            }
        case MAPI_CMD_GFX_GET_CAPS:
            {
                GFX_GETCAPS_ARGS   str;
                if(copy_from_user(&str, pArgs, sizeof(GFX_GETCAPS_ARGS)))
                {
                    return UTOPIA_STATUS_FAIL;
                }

                switch(str.eCaps)
                {
                    case E_GFX_CAP_MULTI_PIXEL:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETCAPS_MULTI_PIXEL),SPT(GFX_GETCAPS_MULTI_PIXEL), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_CAP_BLT_DOWN_SCALE:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETCAPS_BLT_DOWN_SCALE),SPT(GFX_GETCAPS_BLT_DOWN_SCALE), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_CAP_DFB_BLENDING:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETCAPS_DFB_BLENDING),SPT(GFX_GETCAPS_DFB_BLENDING), buffer_arg,sizeof(buffer_arg));
                        break;
                    case E_GFX_CAP_ALIGN:
                        u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,SPT(GFX_GETCAPS_ALIGN),SPT(GFX_GETCAPS_ALIGN), buffer_arg,sizeof(buffer_arg));
                        break;
                    default:
                        break;
                }
                break;
            }
        default:
            break;

    }
   //printk("\33[0;34m [End][%s][%d]u32Cmd=%d \33[m;\n",__FUNCTION__,__LINE__,u32Cmd);


    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




