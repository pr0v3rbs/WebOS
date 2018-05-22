//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#include <MsCommon.h>
#ifdef CONFIG_USB_DEBUG
#define DEBUG
#endif


#include  "include/drvConfig.h"
#include  "include/drvCompiler.h"
#include  "include/drvErrno.h"
#include  "include/drvPorts.h"
#include  "include/drvPCIMEM.h"
#include  "include/drvTimer.h"
#include  "include/drvList.h"
#include  "include/drvCompletion.h"
#include  "include/drvOSAPI.h"
#include  "include/drvKernel.h"
#include  "include/drvBitops.h"
//#include <cyg/hal/hal_if.h> // NUSED
//#include <cyg/hal/hal_cache.h> // NUSED

// USB related header files
#include "include/drvUSBHost.h"
#include "drvUsbd.h"
#include "drvEHCI.h"
#include "drvMassStor.h"

extern void ms_ehci_disable_ep (struct usb_hcd *pHcd, struct s_hcd_dev *pDev, int iEp);
extern int ms_hub_status_data (struct usb_hcd *pHcd, char *pBuf);
extern int ms_hub_control (  struct usb_hcd  *pHcd,  U16    typeReq,  U16    u16Value,  U16    u16Index,  char    *pBuf) ;

struct list_head usb_bus_list = { &(usb_bus_list), &(usb_bus_list) };

#define MS_USB_MAXBUS    32
struct ms_usb_busmap {
  U32 busmap [MS_USB_MAXBUS / (8*sizeof (U32))];
};
static struct ms_usb_busmap busmap;

/*-------------------------------------------------------------------------*/

#define KERNEL_REL  1
#define KERNEL_VER  10

#define DBG_MSG
#define DBG_WARN
//#define DBG_FUNC

#undef  ms_debug_msg
#undef  ms_debug_warn
#undef  ms_debug_func

#ifdef DBG_MSG
#define ms_debug_msg(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_msg(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_WARN
#define ms_debug_warn(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_warn(fmt, arg...) do {} while (0)
#endif

#ifdef DBG_FUNC
#define ms_debug_func(fmt, arg...)    \
        do {diag_printf(fmt, ##arg);} while(0)
#else
#define ms_debug_func(fmt, arg...) do {} while (0)
#endif


static const unsigned char hs_rh_dev_descriptor [18] = {
  0x12,       
  0x01,       
  0x00, 0x02, 

  0x09,     
  0x00,     
  0x01,      
  0x08,      

  0x00, 0x00, 
  0x00, 0x00, 
  KERNEL_VER, KERNEL_REL, 

  0x03,       
  0x02,      
  0x01,      
  0x01        
};

static const unsigned char fs_rh_dev_descriptor [18] = {
  0x12,       
  0x01,       
  0x10, 0x01, 

  0x09,      
  0x00,      
  0x00,       
  0x08,       

  0x00, 0x00, 
  0x00, 0x00, 
  KERNEL_VER, KERNEL_REL, 

  0x03,      
  0x02,       
  0x01,       
  0x01        
};

static const unsigned char fs_rh_config_descriptor [] = {

  0x09,       
  0x02,       
  0x19, 0x00, 
  0x01,       
  0x01,       
  0x00,       
  0x40,       
  0x00,       

  0x09,       
  0x04,       
  0x00,       
  0x00,       
  0x01,       
  0x09,       
  0x00,       
  0x00,       
  0x00,       

  0x07,      
  0x05,      
  0x81,      
  0x03,     
  0x02, 0x00,
  0xff        
};

static const unsigned char hs_rh_config_descriptor [] = {

  0x09,       
  0x02,       
  0x19, 0x00, 
  0x01,       
  0x01,       
  0x00,       
  0x40,       
  0x00,         
  0x09,       
  0x04,       
  0x00,       
  0x00,       
  0x01,       
  0x09,       
  0x00,       
  0x00,       
  0x00,       
  0x07,       
  0x05,       
  0x81,       
  0x03,      
  0x02, 0x00,
  0x0c        
};

/*-------------------------------------------------------------------------*/
static int ms_ascii2utf (char *s, unsigned char *pUtf, int iUtfMax)
{
  int iRetval;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  for (iRetval = 0; *s && iUtfMax > 1; iUtfMax -= 2, iRetval += 2)
  {
    *pUtf++ = *s++;
    *pUtf++ = 0;
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  return iRetval;
}

static int ms_rh_string (
  int    iId,
  struct usb_hcd  *pHcd,
  unsigned char    *pData,
  int    iLen
) 
{
  char pu8Buf [100];

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);    
  if (iId == 0) 
  {
    *pData++ = 4; *pData++ = 3;  
    *pData++ = 0x09; *pData++ = 0x04;  
    return 4;
  }
  else if (iId == 1) 
  {
    strncpy(pu8Buf, pHcd->self.bus_name, sizeof(pu8Buf)-1);
    pu8Buf[sizeof(pu8Buf)-1] = '\0';
  } 
  else if (iId == 2) 
  {
    strncpy(pu8Buf, pHcd->product_desc, sizeof(pu8Buf));
  } 
  else if (iId == 3)
  {
    USB_sprintf(pu8Buf,"%s %s ehci_hcd", "Non-OS", "1.0");
  } 
  else
    return 0;
  pData [0] = 2 * (strlen(pu8Buf) + 1);
  pData [1] = 3;  /* type == string */
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  return 2 + ms_ascii2utf (pu8Buf, pData + 2, iLen - 2);
}

/* 
  * @brief             process request from root hub urb
  *
  * @param          struct usb_hcd *hcd
  * @param          struct urb *urb  
  *
  * @return           0
  */

static int ms_rh_call_control (struct usb_hcd *pHcd, struct urb *pUrb)
{
  struct usb_ctrlrequest *cmd = (struct usb_ctrlrequest *) pUrb->pSetupPacket;
   U16    u16TypeReq, u16Value, u16Index, u16Length;
  const unsigned char  *bufp = 0;
  unsigned char    *ubuf = (unsigned char*) pUrb->pTransferBuffer;
  int    iLen = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  pUrb->hcpriv = pHcd;  

  u16TypeReq  = (cmd->bRequestType << 8) | cmd->bRequest;
  u16Value   = cmd->wValue;
  u16Index   = cmd->wIndex;
  u16Length  = cmd->wLength;

  if (u16Length > pUrb->u32TransferBufferLength)
    goto error;

  pUrb->s32Status = 0;
  pUrb->u32ActualLength = u16Length;
  switch (u16TypeReq) 
  {
  case ReqType_Device | USB_REQ_GET_STATUS:  	
    ubuf [0] = 1;
    ubuf [1] = 0;      
  case ReqType_DeviceOut | USB_REQ_CLEAR_FEATURE:
  case ReqType_DeviceOut | USB_REQ_SET_FEATURE:
    ms_debug_msg ("no device features set yet\n");
    break;
  case ReqType_Device | USB_REQ_GET_CONFIGURATION:
    ubuf [0] = 1;
  case ReqType_DeviceOut | USB_REQ_SET_CONFIGURATION:
    break;
  case ReqType_Device | USB_REQ_GET_DESCRIPTOR:
    switch (u16Value & 0xff00) 
    {
    case USB_DT_DEVICE << 8:
      if (pHcd->hcd_flags & HCD_HS)
        bufp = hs_rh_dev_descriptor;
      else if (pHcd->hcd_flags & HCD_FS)
        bufp = fs_rh_dev_descriptor;
      else
        goto error;
      iLen = 18;
      break;
    case USB_DT_CONFIG << 8:
      if (pHcd->hcd_flags & HCD_HS) 
      {
        bufp = hs_rh_config_descriptor;
        iLen = sizeof hs_rh_config_descriptor;
      }
      else 
      {
        bufp = fs_rh_config_descriptor;
        iLen = sizeof fs_rh_config_descriptor;
      }
      break;
    case USB_DT_STRING << 8:
      pUrb->u32ActualLength = ms_rh_string (
        u16Value & 0xff, pHcd,
        ubuf, u16Length);
      break;
    default:
      goto error;
    }
    break;
  case ReqType_Device | USB_REQ_GET_INTERFACE:
    ubuf [0] = 0;
  case ReqType_DeviceOut | USB_REQ_SET_INTERFACE:
    break;
  case ReqType_DeviceOut | USB_REQ_SET_ADDRESS:
    ms_debug_msg ("[root hub] device address %d\n", u16Value);
    break;
  case ReqType_Endpoint | USB_REQ_GET_STATUS:
    ubuf [0] = 0;
    ubuf [1] = 0;
  case ReqType_EndpointOut | USB_REQ_CLEAR_FEATURE:
  case ReqType_EndpointOut | USB_REQ_SET_FEATURE:
    ms_debug_msg ("[no endpoint features yet]\n");
    break;
  default:
    pUrb->s32Status = ms_hub_control (pHcd, u16TypeReq, u16Value, u16Index,
      (char*) ubuf);
    break;
error:
    pUrb->s32Status = -EPIPE;
    ms_debug_msg ("[unsupported hub control message] (maxchild %d)\n", (int)pUrb->dev->u32MaxChild);
  }
  if (pUrb->s32Status)
  {
    pUrb->u32ActualLength = 0;
    ms_debug_msg ("CTRL: TypeReq=0x%x val=0x%x idx=0x%x iLen=%d ==> %d\n",
      u16TypeReq, u16Value, u16Index, u16Length, (int)pUrb->s32Status);
  }
  if (bufp)
  {
    if (pUrb->u32TransferBufferLength < iLen)
      iLen = pUrb->u32TransferBufferLength;
    pUrb->u32ActualLength = iLen;
    memcpy(ubuf, bufp, iLen);
  }
  osapi_local_irq_save (flags);
  ms_usb_hcd_giveback_urb (pUrb, NULL);
  osapi_local_irq_restore (flags);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  return 0;
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             check root hub port connect status change and 
  *                        run urb's completion funciton
  *  
  * @param          struct usb_hcd *pHcd
  *
  * @return           none
  */

void ms_hcd_poll_rh_status(struct usb_hcd *pHcd)
{
    struct urb	*pUrb;
    int     iLen;
    unsigned long   ulFlags;
    char    aBuf[4];

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if (!pHcd->roothub_registered)
    {   
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
        return;
    }        
    iLen = ms_hub_status_data(pHcd, aBuf);
    if (iLen > 0) 
    {

        osapi_spin_lock_irqsave(&hcd_root_hub_lock, ulFlags);
        pUrb = pHcd->ucd_status_urb;
        if (pUrb) 
        {
            pHcd->hcd_poll_pending = 0;
            pHcd->ucd_status_urb = NULL;
            pUrb->u32ActualLength = iLen;
            pUrb->s32Status = 0; // update status
            pUrb->hcpriv = 0;
            memcpy(pUrb->pTransferBuffer, aBuf, iLen);
            osapi_spin_unlock(&hcd_root_hub_lock);
            ms_usb_hcd_giveback_urb(pUrb, NULL);
            osapi_spin_lock(&hcd_root_hub_lock);
        } 
        else
        {
            diag_printf("#### null urb in usb_hcd_poll_rh_status\n");
            iLen = 0;
            pHcd->hcd_poll_pending = 1;
        }
        osapi_spin_unlock_irqrestore(&hcd_root_hub_lock, ulFlags);
    }
    
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);             
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             assign urb to hcd and check need report hub status change or not?                        
  *  
  * @param          struct usb_hcd *pHcd
  * @param          struct urb *pUrb
  *
  * @return           error code
  */

static int ms_rh_queue_status (struct usb_hcd *pHcd, struct urb *pUrb)
{
    int     iRet;
    unsigned long   ulFlags;
    int     iLen = 1 + (pUrb->dev->u32MaxChild / 8);

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
    osapi_spin_lock_irqsave (&hcd_root_hub_lock, ulFlags);
    if (pHcd->ucd_status_urb || pUrb->u32TransferBufferLength < iLen) 
    {
        ms_debug_warn ("not queuing rh status urb\n");
        iRet = -EINVAL;
        goto done;
    }

    pHcd->ucd_status_urb = pUrb;
    pUrb->hcpriv = pHcd;	  

    /* quickly report hub status change event */
    if (pHcd->hcd_poll_pending)
        ms_update_timer(&pHcd->roothub_timer, jiffies, 0);
    iRet = 0;
 done:
    osapi_spin_unlock_irqrestore (&hcd_root_hub_lock, ulFlags);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
    return iRet;
}
/*-------------------------------------------------------------------------*/
/* 
  * @brief             dispatch urb for HW                       
  *  
  * @param          struct usb_hcd *pHcd
  * @param          struct urb *pUrb
  *
  * @return           error code
  */

static int ms_rh_urb_enqueue (struct usb_hcd *pHcd, struct urb *pUrb)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  if (usb_pipeint (pUrb->u32Pipe)) 
  {
    int  iRetval;
    U32  u32Flags;

    osapi_spin_lock_irqsave (&hcd_data_lock, u32Flags);
    iRetval = ms_rh_queue_status (pHcd, pUrb);
    osapi_spin_unlock_irqrestore (&hcd_data_lock, u32Flags);
    return iRetval;
  }
  else if (usb_pipecontrol (pUrb->u32Pipe))
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
    return ms_rh_call_control (pHcd, pUrb);
  }  
  else
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
    return -EINVAL;
  }  
}

/*-------------------------------------------------------------------------*/

void ms_rh_status_dequeue (struct usb_hcd *pHcd, struct urb *pUrb)
{
   unsigned long   ulFlags;   

   ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
   osapi_spin_lock_irqsave(&hcd_root_hub_lock, ulFlags);
   if (usb_pipecontrol (pUrb->u32Pipe)) 
   {       /* Control URB */
       ;   /* Do nothing */
   } 
   else 
   {   
       /* Status URB */       
       if (pUrb == pHcd->ucd_status_urb) 
       {
           pHcd->ucd_status_urb = NULL;
           // TODO: FIXME
           //usb_hcd_unlink_urb_from_ep(hcd, urb);

           osapi_spin_unlock(&hcd_root_hub_lock);
           ms_usb_hcd_giveback_urb(pUrb, NULL);
           osapi_spin_lock(&hcd_root_hub_lock);
       }
   }
   osapi_spin_unlock_irqrestore(&hcd_root_hub_lock, ulFlags);
   ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
}

/*-------------------------------------------------------------------------*/
void ms_usb_bus_init (struct usb_bus *bus)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  memset (&bus->devmap, 0, sizeof(struct ms_usb_devmap));

  bus->devnum_next = 1;
  bus->root_hub = NULL;
  bus->hcpriv = NULL;  
  bus->busnum = -1;

  ms_list_init (&bus->bus_list);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
}

int ms_usb_register_bus(struct usb_bus *pBus)
{
  int iBusNum;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, (int)__LINE__);  
  osapi_down (&usb_bus_list_lock);
  iBusNum = find_next_zero_bit (busmap.busmap, MS_USB_MAXBUS, 1);
  if (iBusNum < MS_USB_MAXBUS) 
  {
    ms_set_bit (iBusNum, busmap.busmap, U32);
    pBus->busnum = iBusNum;
  } 
  else
    ms_debug_warn ("too many buses %s\n", pBus->bus_name);
  ms_insert_list_after (&pBus->bus_list, &usb_bus_list);
  osapi_up (&usb_bus_list_lock);

  ms_debug_msg ("new USB Bus, assigned pBus number %d\n", (int)pBus->busnum);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, (int)__LINE__);  
  return 0;
}

void ms_usb_deregister_bus (struct usb_bus *pBus)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  ms_debug_msg ("USB pBus %d deregistered\n", (int)pBus->busnum);
  osapi_down (&usb_bus_list_lock);
  ms_list_remove (&pBus->bus_list);
  osapi_up (&usb_bus_list_lock);
  ms_clear_bit (pBus->busnum, busmap.busmap, U32);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
}

int ms_register_root_hub(struct usb_hcd *pHcd)
{
	//struct device *parent_dev = pHcd->self.controller;
    struct device_s *pParent_dev = pHcd->controller;
	struct usb_device *pUsb_dev = pHcd->self.root_hub;
	const int iDevnum = 1;
	int iRetval;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
    pUsb_dev->eState = USB_STATE_DEFAULT;
        
    pUsb_dev->u32DevNum = iDevnum;
    pUsb_dev->bus->devnum_next = iDevnum + 1;
    ms_set_bit (iDevnum, pUsb_dev->bus->devmap.usb_devicemap, U32);

    iRetval = ms_usb_new_device (pUsb_dev, pParent_dev);
    if (iRetval)
      ms_debug_err ("can't register root hub for %s, %d\n",
          pUsb_dev->dev.bus_id, iRetval);

    if (iRetval == 0) 
    {
        osapi_spin_lock_irq (&hcd_root_hub_lock);
        pHcd->roothub_registered = 1;
        osapi_spin_unlock_irq (&hcd_root_hub_lock);
    }

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
    return iRetval;
}

/*-------------------------------------------------------------------------*/
int ms_usb_calc_bus_time (int iSpeed, int is_input, int iBytes)
{
  U32  u32Tmp;
  int  iRet;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  switch (iSpeed) 
  {
  case USB_LOW_SPEED:   
    if (is_input) 
    {
      u32Tmp = (67667L * (31L + 10L * ms_CalcBitTime (iBytes))) / 1000L;
      iRet = (64060L + (2 * HUB_BW_LS_SETUP) + EHCI_BW_DELAY + u32Tmp);
    } 
    else 
    {
      u32Tmp = (66700L * (31L + 10L * ms_CalcBitTime (iBytes))) / 1000L;
      iRet = (64107L + (2 * HUB_BW_LS_SETUP) + EHCI_BW_DELAY + u32Tmp);
    }
    break;
  case USB_FULL_SPEED:      
    u32Tmp = (8354L * (31L + 10L * ms_CalcBitTime (iBytes))) / 1000L;
    iRet = (9107L + EHCI_BW_DELAY + u32Tmp);    
    break;
  case USB_HIGH_SPEED:      
    u32Tmp = HIGHSPEED_US_ISO (iBytes);
    iRet = (int)u32Tmp;
    break;
  default:
    ms_debug_err ("Unknown device iSpeed!\n");
    iRet = -1;
  }

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  return iRet;
}

/*-------------------------------------------------------------------------*/
static int ms_hcd_alloc_dev (struct usb_device *iUdev)
{
  struct s_hcd_dev    *pDev;
  struct usb_hcd    *pHcd;
  U32    u32Flags;
  int    iRet = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  if (!iUdev || iUdev->hcpriv)
    iRet = -EINVAL;
  if (!iUdev->bus || !iUdev->bus->hcpriv)
    iRet = -ENODEV;
  pHcd = (struct usb_hcd*) iUdev->bus->hcpriv;
  if (pHcd->state == USB_STATE_QUIESCING)
    iRet = -MS_ERR_NOLINK;

  pDev = (struct s_hcd_dev *) kmalloc (sizeof *pDev, GFP_KERNEL);
  if (pDev == NULL)
    iRet = -ENOMEM;
  memset (pDev, 0, sizeof *pDev);

  ms_list_init (&pDev->dev_list);
  ms_list_init (&pDev->urb_list);

  osapi_spin_lock_irqsave (&hcd_data_lock, u32Flags);
  ms_insert_list_after (&pDev->dev_list, &pHcd->hcd_dev_list);
  
  iUdev->hcpriv = pDev;
  osapi_spin_unlock_irqrestore (&hcd_data_lock, u32Flags);

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  return iRet;
}

/*-------------------------------------------------------------------------*/
static void ms_urb_unlink (struct urb *pUrb)
{
  U32    u32Flags;
  struct usb_device  *dev;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  /* clear and release pUrb allocate resource */

  osapi_spin_lock_irqsave (&hcd_data_lock, u32Flags);
  ms_list_remove_and_init (&pUrb->urb_list);
  dev = pUrb->dev;
  osapi_spin_unlock_irqrestore (&hcd_data_lock, u32Flags);
  ms_usb_put_dev (dev);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
}

/* 
  * @brief             allocate struct usb_hcd and initial it
  *
  * @param          struct urb *urb
  * @param          int mem_flags
  *
  * @return           status
  */

static int ms_hcd_submit_urb (struct urb *pUrb, int iMem_flags)
{
  int      iStatus;
  struct usb_hcd    *pHcd = (struct usb_hcd*) pUrb->dev->bus->hcpriv;
  struct s_hcd_dev    *pDev = (struct s_hcd_dev*) pUrb->dev->hcpriv;
  //struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  U32    u32Flags;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);   
  if (!pHcd || !pDev)
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
    return -ENODEV;
  }  

  //pEhci->uDontSendIAA = 0;	//Set IAA interrupt (default)

  osapi_spin_lock_irqsave (&hcd_data_lock, u32Flags);
  // ROOTHUB_INTERRUPT_MODE should adding urb into the list in any condition
  if ((HCD_IS_RUNNING (pHcd->state) && pHcd->state != USB_STATE_QUIESCING) || (pUrb->dev == pHcd->self.root_hub))
  {
    ms_usb_get_dev (pUrb->dev);
    ms_insert_list_before (&pUrb->urb_list, &pDev->urb_list);
    iStatus = 0;
  } 
  else 
  {
    ms_list_init (&pUrb->urb_list);
    ms_debug_msg("[ms_hcd_submit_urb] return ESHUTDOWN !!! pHcd->state = %x\n", pHcd->state);
    iStatus = -ESHUTDOWN;
  }
  osapi_spin_unlock_irqrestore (&hcd_data_lock, u32Flags);
  if (iStatus)
    return iStatus;

  pUrb = ms_usb_get_urb (pUrb);
  if (pUrb->dev == pHcd->self.root_hub) 
  {
    pUrb->u32TransferFlags |= (MS_FLAG_URB_NO_TRANSFER_DMA_MAP
          | MS_FLAG_URB_NO_SETUP_DMA_MAP);
    iStatus = ms_rh_urb_enqueue (pHcd, pUrb);
    goto done;
  }

  if (pHcd->controller->dma_mask) 
  {
  }
  else
  {
    // Non DMA support
    if (usb_pipecontrol (pUrb->u32Pipe))
    {
        //if ((((U32) pUrb->setup_packet) & 0xF) != 0 || ((sizeof(struct usb_ctrlrequest)  & 0xF) != 0) )
        if ((((U32) pUrb->pSetupPacket) & CPU_L1_CACHE_BOUND) != 0 || ((sizeof(struct usb_ctrlrequest)  & CPU_L1_CACHE_BOUND) != 0) ) // J
        {
            pUrb->SetDMALen = sizeof(struct usb_ctrlrequest);
            //pUrb->SetDMABuf = LIB_MemoryAllocate(pUrb->SetDMALen | 0xF, MALLOC_UNCACHED_ADDR);
            //pUrb->SetDMABuf = MsOS_AllocateMemory(((pUrb->SetDMALen+15) & ~0xF), gs32NonCachedPoolID_MIU0 );
            //pUrb->SetDMABuf = Usb_AllocateNonCachedMemory(((pUrb->SetDMALen+15) & ~0xF));
            pUrb->SetDMABuf = Usb_AllocateNonCachedMemory(((pUrb->SetDMALen+CPU_L1_CACHE_BOUND) & ~CPU_L1_CACHE_BOUND)); // J
            USB_ASSERT(pUrb->SetDMABuf != NULL, "Allocate SetDMABuf error\n");

            memcpy(pUrb->SetDMABuf, pUrb->pSetupPacket, pUrb->SetDMALen);
            //HAL_DCACHE_FLUSH((void*) pUrb->SetDMABuf, pUrb->SetDMALen);

            pUrb->tSetupDma = (dma_addr_t) USB_VA2PA((U32)pUrb->SetDMABuf);
            //diag_printf("[1]SetDMA address %x SetDMA length %x\n", pUrb->tSetupDma, pUrb->SetDMALen);
        }
        else
        {
            if ( pUrb->pSetupPacket != KSEG02KSEG1(pUrb->pSetupPacket) )
                //HAL_DCACHE_FLUSH((void*) pUrb->setup_packet, sizeof(struct usb_ctrlrequest));
                MsOS_Dcache_Flush((U32) pUrb->pSetupPacket, sizeof(struct usb_ctrlrequest));

            pUrb->tSetupDma = (dma_addr_t) USB_VA2PA((U32)pUrb->pSetupPacket);
            //diag_printf("[2]Setup DMA address %x DMA length %x\n", pUrb->tSetupDma, sizeof(struct usb_ctrlrequest));
        }
        USB_ASSERT((pUrb->tSetupDma & 0xF) == 0, "DMA address is not 16 bytes aligned\n");

   }

    if (pUrb->u32TransferBufferLength != 0)
    {
#if 0           //yuwen, mass storage 
        if ((Mass_stor_us!=NULL)&& ( pUrb->pTransferBuffer == Mass_stor_us->iobuf ))
        {
            pUrb->tTransferDma= (dma_addr_t) USB_VA2PA((U32)pUrb->pTransferBuffer);
        }
        else
#endif
        {
            //if ((((U32) pUrb->transfer_buffer) & 0xF) != 0 || ((pUrb->transfer_buffer_length  & 0xF) != 0) )
            if ((((U32) pUrb->pTransferBuffer) & CPU_L1_CACHE_BOUND) != 0 || ((pUrb->u32TransferBufferLength  & CPU_L1_CACHE_BOUND) != 0) ) // J
            {
                //diag_printf("<ms_hcd_submit_urb> bouncing A[%x], L[%d]\n", pUrb->transfer_buffer, pUrb->transfer_buffer_length);
                #ifdef DEBUG_PERFORMANCE
                if (pUrb->u32TransferBufferLength >= 512)
                    ms_debug_msg("DMA address %x DMA length %x\n", pUrb->pTransferBuffer, pUrb->u32TransferBufferLength);
                #endif
                pUrb->TxDMALen= pUrb->u32TransferBufferLength;
                //pUrb->TxDMABuf = LIB_MemoryAllocate(pUrb->TxDMALen | 0xF, MALLOC_UNCACHED_ADDR);
                //pUrb->TxDMABuf = MsOS_AllocateMemory(((pUrb->TxDMALen+15) & ~0xF), gs32NonCachedPoolID_MIU0 );
                //pUrb->TxDMABuf = Usb_AllocateNonCachedMemory(((pUrb->TxDMALen+15) & ~0xF));
                pUrb->TxDMABuf = Usb_AllocateNonCachedMemory(((pUrb->TxDMALen+CPU_L1_CACHE_BOUND) & ~CPU_L1_CACHE_BOUND)); // J
                USB_ASSERT(pUrb->TxDMABuf != NULL, "Allocate TxDMABuf error\n");

                if (!usb_pipein(pUrb->u32Pipe))
                {
                    memcpy(pUrb->TxDMABuf, pUrb->pTransferBuffer, pUrb->u32TransferBufferLength);
                }

                pUrb->tTransferDma= (dma_addr_t) USB_VA2PA((U32)pUrb->TxDMABuf);
                //diag_printf("TxDMA address %x TxDMA length %x\n", pUrb->transfer_dma, pUrb->TxDMALen);
            }
            else
            {
                if (  pUrb->pTransferBuffer != KSEG02KSEG1(pUrb->pTransferBuffer) )
                {
                    if (usb_pipein(pUrb->u32Pipe))
                        //HAL_DCACHE_INVALIDATE((void*) pUrb->transfer_buffer, pUrb->transfer_buffer_length);
                        //MsOS_Dcache_Invalidate((U32) pUrb->transfer_buffer, pUrb->transfer_buffer_length);
                        MsOS_Dcache_Flush((U32) pUrb->pTransferBuffer, pUrb->u32TransferBufferLength);
                        //MsOS_Dcache_Invalidate seems to cause stack crashed... , change to MsOS_Dcache_Flush temporiarly

                    else
                        //HAL_DCACHE_FLUSH((void*) pUrb->transfer_buffer, pUrb->transfer_buffer_length);
                        MsOS_Dcache_Flush((U32) pUrb->pTransferBuffer, pUrb->u32TransferBufferLength);

                }

                pUrb->tTransferDma= (dma_addr_t) USB_VA2PA((U32)pUrb->pTransferBuffer);
                //diag_printf("DMA address %x DMA length %x\n", pUrb->transfer_dma, pUrb->transfer_buffer_length);
            }
        }
        USB_ASSERT((pUrb->tTransferDma & 0xF) == 0, "DMA address is not 16 bytes aligned\n");
    }
  }


  iStatus = pHcd->ms_urb_enqueue (pHcd, pUrb, iMem_flags);
done:
  if (iStatus) 
  {
    if (pUrb->SetDMABuf != NULL)
    {
        Usb_FreeNonCachedMemory(pUrb->SetDMABuf);
        pUrb->SetDMABuf = NULL;
    }    
    if (pUrb->TxDMABuf != NULL)
    {
        Usb_FreeNonCachedMemory(pUrb->TxDMABuf);
        pUrb->TxDMABuf = NULL;
    }
    ms_urb_unlink (pUrb);
    usb_put_urb (pUrb);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  return iStatus;
}

/*-------------------------------------------------------------------------*/
static void
ms_unlink1 (struct usb_hcd *pHcd, struct urb *pUrb)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  

  //if (pUrb == (struct urb *) pHcd->roothub_timer.data)
  if (pUrb->dev->parent == NULL)
    ms_rh_status_dequeue (pHcd, pUrb);
  else 
  {
    int    value;

    value = pHcd->ms_urb_dequeue (pHcd, pUrb);
    if (value != 0)
      ms_debug_msg ("dequeue %p --> %d\n", pUrb, value);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
}

struct ms_completion_splice {    // for modified urb context
  struct stCompletion  done;
  usb_complete_t    stComplete;
  void      *pContext;
};

static void ms_unlink_complete (struct urb *pUrb, struct stPtRegs *pRegs)
{
  struct ms_completion_splice  *pSplice;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);   
  pSplice = (struct ms_completion_splice *) pUrb->pContext;
  pUrb->complete_func = pSplice->stComplete;
  pUrb->pContext = pSplice->pContext;
  pUrb->complete_func (pUrb, pRegs);  //Callback function
  complete (&pSplice->done);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
}

extern void ms_usb_hw_reset(struct usb_hcd* pUsbHcd/*, struct usb_device *pUsbDev*/);
static int ms_hcd_unlink_urb (struct urb *pUrb)
{
  struct s_hcd_dev      *dev;
  struct usb_hcd      *pHcd = 0;
  struct device_s      *pSys = 0;
  U32      u32Flags;
  struct ms_completion_splice  stSplice;
  int        iRetval;
  U32    u32Cnt = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  if (!pUrb)
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
    return -EINVAL;
  }  

  osapi_spin_lock_irqsave (&pUrb->lock, u32Flags);
  osapi_spin_lock (&hcd_data_lock);

  if (!pUrb->dev || !pUrb->dev->bus) 
  {
    iRetval = -ENODEV;
    goto done;
  }

  dev = (struct s_hcd_dev*) pUrb->dev->hcpriv;
  pSys = &pUrb->dev->dev;
  pHcd = (struct usb_hcd*) pUrb->dev->bus->hcpriv;
  if (!dev || !pHcd) 
  {
    iRetval = -ENODEV;
    goto done;
  }

  if (!pUrb->hcpriv)
  {
    iRetval = -EINVAL;
    goto done;
  }

  if (pUrb->s32Status != -EINPROGRESS) 
  {
    iRetval = -EBUSY;
    goto done;
  }

  if (!(pUrb->u32TransferFlags & MS_FLAG_URB_ASYNC_UNLINK)) 
  {
    init_completion (&stSplice.done);
    stSplice.stComplete = pUrb->complete_func;
    stSplice.pContext = pUrb->pContext;
    pUrb->complete_func = ms_unlink_complete;
    pUrb->pContext = &stSplice;
    pUrb->s32Status = -ENOENT;
  } 
  else
  {
    pUrb->s32Status = -ECONNRESET;
  }
  osapi_spin_unlock (&hcd_data_lock);
  osapi_spin_unlock_irqrestore (&pUrb->lock, u32Flags);

  //if (pUrb == (struct urb *) pHcd->roothub_timer.data)
  if (pUrb->dev->parent == NULL)  
  {
    ms_rh_status_dequeue (pHcd, pUrb);
    iRetval = 0;
  }
  else
  {
    iRetval = pHcd->ms_urb_dequeue (pHcd, pUrb);

    if (iRetval)
    {
      ms_debug_msg ("dequeue %p --> %d\n", pUrb, iRetval);
      if (!(pUrb->u32TransferFlags & MS_FLAG_URB_ASYNC_UNLINK)) 
      {
        osapi_spin_lock_irqsave (&pUrb->lock, u32Flags);
        pUrb->complete_func = stSplice.stComplete;
        pUrb->pContext = stSplice.pContext;
        osapi_spin_unlock_irqrestore (&pUrb->lock, u32Flags);
      }
      goto bye;
    }
  }

  if (pUrb->u32TransferFlags & MS_FLAG_URB_ASYNC_UNLINK)
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
    return -EINPROGRESS;
  }  

  while ( stSplice.done.done == 0 )
  {
    if ( u32Cnt > (U32) 3000)
    {
      break;
    }
    else
    {
      //tick base is 1 ms
      mdelay(1);
      //HAL_DELAY_US(1000); // NUSED
      u32Cnt+=1;
    }
  }

  if (u32Cnt > (U32) 3000)
  {
     ms_debug_msg("ms_hcd_unlink_urb timeout\n");
     ms_debug_msg("Reset HC HW\n");
     ms_usb_hw_reset(pHcd);     
        return -ETIMEDOUT;
  }
  return 0;

done:
  osapi_spin_unlock (&hcd_data_lock);
  osapi_spin_unlock_irqrestore (&pUrb->lock, u32Flags);
bye:
  if (iRetval && pSys && pSys->driver)
    ms_debug_msg ("ms_hcd_unlink_urb %p fail %d\n", pUrb, iRetval);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  return iRetval;
}

static void ms_hcd_endpoint_disable (struct usb_device *pUdev, int iEndpoint)
{
  struct list_head *__mptr;
  struct s_hcd_dev  *pDev;
  struct usb_hcd  *pHcd;
  struct urb  *pUrb;  
  U32  u32Epnum = iEndpoint & USB_ENDPOINT_NUMBER_MASK;

  pDev = (struct s_hcd_dev*) pUdev->hcpriv;
  pHcd = (struct usb_hcd*) pUdev->bus->hcpriv;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  osapi_local_irq_disable ();

hcd_rescan:
  if (iEndpoint & USB_DIR_IN) 
  {
    usb_endpoint_halt (pUdev, u32Epnum, 0);
    pUdev->pEpMaxPacketIn [u32Epnum] = 0;
  } 
  else
  {
    usb_endpoint_halt (pUdev, u32Epnum, 1);
    pUdev->pEpMaxPacketOut [u32Epnum] = 0;
  }

  osapi_spin_lock (&hcd_data_lock);

  pUrb = entry_to_container((&pDev->urb_list)->next, struct urb, urb_list);
  for( ; &(pUrb->urb_list) != &(pDev->urb_list);
    __mptr = pUrb->urb_list.next,
    pUrb = (struct urb *)( (char *)__mptr - (char *)offsetof(struct urb,urb_list) )
  )
  {
    int  tmp = pUrb->u32Pipe;

    if ((U32)usb_pipeendpoint (tmp) != u32Epnum)
      continue;
    
    if (u32Epnum != 0 && ((tmp ^ iEndpoint) & USB_DIR_IN))
      continue;

    if (pUrb->s32Status != -EINPROGRESS)
      continue;
    ms_usb_get_urb (pUrb);
    osapi_spin_unlock (&hcd_data_lock);

    osapi_spin_lock (&pUrb->lock);
    tmp = pUrb->s32Status;
    if (tmp == -EINPROGRESS)
    {
      ms_debug_msg("ms_hcd_endpoint_disable return ESHUTDOWN !!!\n");    
      pUrb->s32Status = -ESHUTDOWN;
    }
    osapi_spin_unlock (&pUrb->lock);

    if (tmp == -EINPROGRESS) 
    {
      tmp = pUrb->u32Pipe;
      ms_unlink1 (pHcd, pUrb);
      ms_debug_msg ("shutdown pUrb %p pipe %08x ep%d%s, ep type = %d\n",
        pUrb, tmp, usb_pipeendpoint (tmp),
        (tmp & USB_DIR_IN) ? "in" : "out", usb_pipetype (tmp));
    }
    usb_put_urb (pUrb);

    goto hcd_rescan;
  }
  osapi_spin_unlock (&hcd_data_lock);
  osapi_local_irq_enable();  
  ms_ehci_disable_ep(pHcd, pDev, iEndpoint);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
}

/*-------------------------------------------------------------------------*/
static int ms_hcd_free_dev (struct usb_device *pUdev)
{
  struct s_hcd_dev    *pDev;
  struct usb_hcd    *pHcd;
  U32    u32Flags;
  int iRet = ENOERR;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  if (!pUdev || !pUdev->hcpriv)  
  {
    iRet = -EINVAL;    
    goto hcd_free_dev_err;
  }  

  if (!pUdev->bus || !pUdev->bus->hcpriv)  
  {
    iRet = -ENODEV; 
    goto hcd_free_dev_err;
  }  

  // should pUdev->devnum == -1 ??

  pDev = (struct s_hcd_dev*) pUdev->hcpriv;
  pHcd = (struct usb_hcd*) pUdev->bus->hcpriv;

  if (!ms_is_empty_list (&pDev->urb_list)) 
  {
    ms_debug_err ("free busy pDev, %s devnum %d (bug!)\n",
      pHcd->self.bus_name, (int)pUdev->u32DevNum);
    iRet = -EINVAL;
    goto hcd_free_dev_err;
  }

  osapi_spin_lock_irqsave (&hcd_data_lock, u32Flags);
  ms_list_remove (&pDev->dev_list);
  pUdev->hcpriv = NULL;
  osapi_spin_unlock_irqrestore (&hcd_data_lock, u32Flags);

  kfree (pDev);
  
hcd_free_dev_err:  
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  return iRet;
}

struct ms_usb_bus_ops ms_usb_hcd_operations = {
  ms_hcd_alloc_dev,
  ms_hcd_free_dev,  
  ms_hcd_submit_urb,
  ms_hcd_unlink_urb,
  ms_hcd_buffer_alloc,
  ms_hcd_buffer_free,
  ms_hcd_endpoint_disable,
};

/*-------------------------------------------------------------------------*/
void ms_usb_hcd_giveback_urb (struct urb *urb, struct stPtRegs *regs)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  ms_urb_unlink (urb);
  urb->complete_func (urb, regs);
  usb_put_urb (urb);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
}
/*-------------------------------------------------------------------------*/
