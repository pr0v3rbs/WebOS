#include "mvideo_context.h"

#include "MsTypes.h"
typedef struct _DIP_SHARE_RESOURCE_PRIVATE
{
	XC_DIP_InternalStatus  g_DIPSrcInfo[MAX_DIP_WINDOW];
}DIP_SHARE_RESOURCE_PRIVATE;

typedef struct _DIP_RESOURCE_PRIVATE
{

}DIP_RESOURCE_PRIVATE;

typedef struct _DIP_INSTANCE_PRIVATE
{
    MS_BOOL bInit;
    SCALER_DIP_WIN eWindow;
    void* pResource;
	void* pShareResource;
}DIP_INSTANCE_PRIVATE;

void DIPRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 DIPOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DIPClose(void* pInstance);
MS_U32 DIPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
