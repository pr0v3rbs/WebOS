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

#include "include/drvConfig.h"

#ifdef CONFIG_USB_DEBUG
  #define DEBUG
#else
  #undef DEBUG
#endif

#include <MsCommon.h>
#include "include/drvCompiler.h"
#include "include/drvPorts.h"
#include "include/drvErrno.h"
#include "include/drvPCIMEM.h"
#include "include/drvList.h"
#include "include/drvTimer.h"
#include "include/drvKernel.h"
// USB related implemented header files
#include "include/drvUSBHost.h"
#include "drvUsbd.h"
#include "include/drvCPE_EHCI.h"
#include "drvEHCI.h"
#include "drvUSBHwCtl.h"
#include "include/drvCPE_AMBA.h"
#include <drvUsbcommon.h> // public include
#include <drvUSB_eCos.h> // public include
#if 1

/*-------------------------------------------------------------------------*/


#define DRIVER_VERSION "eCos Newhost"
#define DRIVER_AUTHOR "Mstar USB Team"
#define DRIVER_DESC "USB 2.0 'Enhanced' Host Controller (EHCI) Driver"

// #define EHCI_VERBOSE_DEBUG

#ifdef DEBUG
#define EHCI_STATS
#endif

#define  EHCI_RL_NAKCNT     4  /* enable nak throttle mode, spec 4.9 */
#define  EHCI_RL_NAKCNT_TT  0  /* disable nak throttle mode when TT */
#define  EHCI_HI_BW_MULT_HS 1  /* high bandwidth file, transactions number/uframe, spec 4.10.3 */
#define  EHCI_HI_BW_MULT_TT 1

#ifdef CONFIG_HC_TEST
#define  EHCI_PERIODIC_FLS      0  /* (max) 1024 frame for schedule */
#else
#define  EHCI_PERIODIC_FLS      2  /* (minl) 256 frame schedule */
#endif
/* Initial IRQ latency:  lower than default */
static int log2_irq_thresh = 0;    // 0 to 6
#define    INTR_MASK (USBSTS_IAA | USBSTS_FATAL | USBSTS_PCD | USBSTS_ERR | USBSTS_INT)

#if defined(ENABLE_16US_EOF1)
static U32 park_eco = 3;
#else
static U32 park_eco = 0;
#endif

#include "drvEHCI.h"

/* Debug print definition */
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

extern int ms_hub_port_disable(struct usb_device *hub, int port);

extern struct list_head hub_event_list;
extern void ms_init_OS_Resource_EX(S32 *USBWaitFlg);
extern void ms_ehci_stoprun_setting(MS_U8 bOption, struct ehci_hcd *ehci);
extern void Chip_Flush_Memory(void);
extern void Chip_Read_Memory(void);

static int ms_check_status (U32 *pPtr, U32 u32Mask, U32 u32Done, int us)
{
    U32  u32regv;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
    do {
        u32regv = hcd_reg_readl ((U32)pPtr);
        if (u32regv == ~(U32)0)    /* card removed */
        {
            ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
            return -ENODEV;
        }    

        u32regv &= u32Mask;
        if (u32regv == u32Done)
            return 0;

        udelay (1);
        us--;
    } while (us > 0);

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
    return -ETIMEDOUT;
}

static int ms_host_halt (struct ehci_hcd *pEhci)
{
    U32  u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbsts);

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
    if ((u32regv & USBSTS_HALT) != 0)
    {
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
        return 0;
    }    

    u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    u32regv &= ~USBCMD_RUN;
    hcd_reg_writel (u32regv, (U32)&pEhci->op_regs->usbcmd);

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return ms_check_status (&pEhci->op_regs->usbsts, USBSTS_HALT, USBSTS_HALT, 16 * 125);
}

static int ms_host_reset (struct ehci_hcd *pEhci)
{
    U32  u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    u32regv |= USBCMD_RESET;
    hcd_reg_writel (u32regv, (U32)&pEhci->op_regs->usbcmd);
    pEhci->hcd.state = USB_STATE_HALT;
    //Wait for HC reset complete	
    u32regv = ms_check_status (&pEhci->op_regs->usbcmd, USBCMD_RESET, 0, 250 * 1000);	

    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return u32regv;
}

static void ms_host_ready (struct ehci_hcd *pEhci)
{
    U32  u32regv;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    u32regv = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    u32regv &= ~(USBCMD_ASE | USBCMD_IAAD | USBCMD_PSE);
    hcd_reg_writel (u32regv, (U32)&pEhci->op_regs->usbcmd);

    /* wait 16 microframes */
    if (ms_check_status (&pEhci->op_regs->usbsts, USBSTS_ASS | USBSTS_PSS,
    0, 16 * 125) != 0) 
    {
        ms_debug_err("Cannot stop scheduler\n");
        pEhci->hcd.state = USB_STATE_HALT;
        return;
    }
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/* 
  * @brief             clear reset timer
  *
  * @param          struct ehci_hcd  *ehci
  * @param          int  index
  * @param          int  port_status
  *
  * @return           port_status
  */

#if 1
static int ms_reset_complete (
  struct ehci_hcd  *pEhci,
  int    iIndex,
  int    port_status)
{
    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if (!(port_status & PORTSC_CONNECT)) 
    {
        pEhci->u32ResetEnd [iIndex] = 0;
        return port_status;
    }
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return port_status;
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             check hub ports status change
  *                       our echi ip support a port 
  *
  * @param          struct ehci_hcd  *ehci
  * @param          char *buf
  *
  * @return           none
  */

int
ms_hub_status_data (struct usb_hcd *pHcd, char *pBuf)
{
      struct ehci_hcd  *pEhci = hcd_to_ehci (pHcd);
      U32  u32Temp, u32Status = 0;     
      U32  u32Flags;

      ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      /* init u32Status to no-changes */
      pBuf [0] = 0;         
    
      /* port 1 changes ? */
      osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
      
      u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc[0]);

      if ( ((u32Temp & PORTSC_CONNECT) == 0) && ((pHcd->pre_sts & PORTSC_CONNECT) != 0) )
      {
          ms_debug_msg("<ms_hub_status_data> Add CSC @ root hub %d, DISCONNECT! (psc = 0x%x)\n", (int)pHcd->host_id, (unsigned int)u32Temp);
          u32Temp |= PORTSC_CSC;
      }
      pHcd->pre_sts = u32Temp;       
        
      if (!(u32Temp & PORTSC_CONNECT))
          pEhci->u32ResetEnd [0] = 0;

      if ((u32Temp & (PORTSC_CSC)) != 0) 
      {    
          /* bit 0 is for hub up port u32Status change  */
          pBuf [0] |= 1 << 1;            
          u32Status = USBSTS_PCD;
      }      
      
      osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
      ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      return u32Status ? 1 : 0;
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             fill hub descriptor
  *
  * @param          struct ehci_hcd  *ehci
  * @param          struct usb_hub_descriptor  *desc
  *
  * @return           none
  */

static void
ms_get_hub_desc (
  struct ehci_hcd      *pEhci,
  struct usb_hub_descriptor  *pDesc
) 
{
  int    iPorts = HCS_N_PORTS (pEhci->hcs_params);
  U16    u16Temp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pDesc->bDescriptorType = 0x29;
  pDesc->bPwrOn2PwrGood = 0;
  pDesc->bHubContrCurrent = 0;
  pDesc->bNbrPorts = iPorts;
  u16Temp = 1 + (iPorts / 8);
  pDesc->bDescLength = 7 + 2 * u16Temp;  

  /* two bitmaps:  iPorts removable, and usb 1.0 legacy PortPwrCtrlMask */
  memset (&pDesc->DeviceRemovable [0], 0, u16Temp);
  memset (&pDesc->DeviceRemovable [u16Temp], 0xff, u16Temp);

  u16Temp = 0x0008;       /* Individual Port Over-current Protection */
  if (HCS_PPC (pEhci->hcs_params))
    u16Temp |= 0x0001;    /* Ganged power switching (all iPorts' power at once) */
  if (HCS_P_INDICATOR (pEhci->hcs_params))
    u16Temp |= 0x0080;    /* Individual port indicators (LEDs) */
  pDesc->wHubCharacteristics = u16Temp;
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/
void ms_ehci_softrst(struct ehci_hcd  *pEhci)
{
    U32    regTmp[4];  

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

    /* ensure HC already stopped before HC Reset
     * otherwise, there will be a fatal error (DMA Error)
     */
    ms_host_halt(pEhci);

    /* sometimes, replug device quickly crashs aync_base, just restore it */
    hcd_reg_writel ((U32)pEhci->stAsync->qh_dma_addr, (U32)&pEhci->op_regs->asynclistaddr);

    regTmp[0] = hcd_reg_readl((U32)&pEhci->op_regs->usbcmd);
    regTmp[1] = hcd_reg_readl((U32)&pEhci->op_regs->usbintr);
    // HC soft reset
    ms_host_reset(pEhci);
    // restore regisger setting    
    hcd_reg_writel(regTmp[1],(U32)&pEhci->op_regs->usbintr);
    hcd_reg_writel(regTmp[0],(U32)&pEhci->op_regs->usbcmd);
    pEhci->hcd.state = USB_STATE_RUNNING;  
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}
/*-------------------------------------------------------------------------*/
/* 
  * @brief             process hub request
  *
  * @param          struct usb_hcd  *hcd
  * @param          U16    typeReq
  * @param          U16    wValue
  * @param          U16    wIndex
  * @param          char    *buf
  * @param          U16    wLength
  *
  * @return           error code
  *
  */

 int ms_hub_control (
  struct usb_hcd  *pHcd,
  U16    typeReq,
  U16    u16Value,
  U16    u16Index,
  char    *pBuf  
) 
{
  struct ehci_hcd  *pEhci = hcd_to_ehci (pHcd);
  int    iPorts = HCS_N_PORTS (pEhci->hcs_params);
  U32    u32Temp, u32Status, u32Bus_monitor;
  U32  u32Flags;
  int    iRetval = 0;
  int    time_out;
  struct cpe_dev *dev;
  const struct device_s *__mptr = pHcd->controller;
  dev = (struct cpe_dev *)( (char *)__mptr - offsetof(struct cpe_dev,dev) );
  U32 u32RegUTMI = dev->utmibase;
  U32 u32Temp2, u32Speed;  	  

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  switch (typeReq) 
  {
  case Req_ClearHubFeature:
    if ((u16Value != C_HUB_LOCAL_POWER)&&(u16Value != C_HUB_OVER_CURRENT))
      goto error;    
    break;
  case Req_ClearPortFeature:
    if (!u16Index || u16Index > iPorts)
      goto error;
    u16Index--;
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
    if (u32Temp & PORTSC_OWNER)
      break;

    switch (u16Value) 
    {
    case USB_PORT_FEAT_ENABLE:
      ms_debug_func("Disable port\n");
      if( pEhci->stAsync->qh_next.qh != 0 )
      {
          ms_debug_err("Not only cantain one qHD (%p and %p)in asyn. scheduler",pEhci->stAsync->qh_next.qh,pEhci->stAsync);
      }
      hcd_reg_writel (u32Temp & ~PORTSC_PE, (U32)&pEhci->op_regs->portsc [u16Index]);
      //裝置被移除的時候 停止scheduler
      
      hcd_reg_writel(hcd_reg_readl((U32)&pEhci->op_regs->usbcmd)&(~(USBCMD_ASE|USBCMD_PSE|USBCMD_IAAD)),(U32)&pEhci->op_regs->usbcmd);

      //Halt HCD   
      ms_host_halt(pEhci);
      ms_ehci_softrst(pEhci);
      break;
    case USB_PORT_FEAT_C_ENABLE:
      hcd_reg_writel ((u32Temp & ~PORTSC_RWC_BITS) | PORTSC_PEC,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_SUSPEND:
    case USB_PORT_FEAT_C_SUSPEND:     
      break;
    case USB_PORT_FEAT_POWER:
      if (HCS_PPC (pEhci->hcs_params))
        hcd_reg_writel (u32Temp & ~PORTSC_POWER,
          (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_C_CONNECTION:
      hcd_reg_writel ((u32Temp & ~PORTSC_RWC_BITS) | PORTSC_CSC,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;	  
    case USB_PORT_FEAT_C_OVER_CURRENT:
      hcd_reg_writel ((u32Temp & ~PORTSC_RWC_BITS) | PORTSC_OCC,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_C_RESET:
      /* GetPortStatus clears reset */
      break;
    default:
      goto error;
    }
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);  /* unblock posted write */
    break;
  case Req_GetHubDescriptor:
    ms_get_hub_desc (pEhci, (struct usb_hub_descriptor *)
      pBuf);
    break;
  case Req_GetHubStatus:
    /* no hub-wide feature/u32Status u32Flags */
    memset (pBuf, 0, 4);
    //cpu_to_le32s ((U32 *) pBuf);
    break;	
  case Req_GetPortStatus:
    if (!u16Index || u16Index > iPorts)
      goto error;
    u16Index--;
    u32Status = 0;
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
    if ( ((u32Temp & PORTSC_CONNECT) == 0) && ((u32Temp & PORTSC_CSC) == 0) )
    {
        if ((pHcd->pre_temp & PORTSC_CONNECT) != 0)
        {
            ms_debug_msg("<ms_hub_control> Req_GetPortStatus:Add CSC here, pStatus = %x\n", (unsigned int)u32Temp);
            u32Temp |= PORTSC_CSC;
        }
    }
        pHcd->pre_temp = u32Temp;

    // wPortChange bits
    if (u32Temp & PORTSC_CSC)
      u32Status |= 1 << USB_PORT_FEAT_C_CONNECTION;
    if (u32Temp & PORTSC_PEC)
      u32Status |= 1 << USB_PORT_FEAT_C_ENABLE;
    // USB_PORT_FEAT_C_SUSPEND
    if (u32Temp & PORTSC_OCC)
      u32Status |= 1 << USB_PORT_FEAT_C_OVER_CURRENT;
    
    if ((u32Temp & PORTSC_RESET)
        && ms_time_after (jiffies,
          pEhci->u32ResetEnd [u16Index])) 
    {
      u32Status |= 1 << USB_PORT_FEAT_C_RESET;
      time_out=0;
      
      writeb(0x0 ,(void*)(u32RegUTMI+0x2A*2)); // chirp patch
      mdelay(20); // chirp patch
      
      /* Do port reset until finish */
      hcd_reg_writel (u32Temp & ~PORTSC_RESET,
          (U32)&pEhci->op_regs->portsc [u16Index]);
      do {
        u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
        udelay (10);
      } while ((u32Temp&(PORTSC_RESET|PORTSC_CONNECT)) == (PORTSC_RESET|PORTSC_CONNECT)&&(time_out++<10000));
      
      writeb( UTMI_EYE_SETTING_2C, (void*) (u32RegUTMI+0x2c*2)); // chirp patch
      writeb( UTMI_EYE_SETTING_2D, (void*) (u32RegUTMI+0x2d*2-1)); // chirp patch
      writeb( UTMI_EYE_SETTING_2E, (void*) (u32RegUTMI+0x2e*2)); // chirp patch
      writeb( UTMI_EYE_SETTING_2F, (void*) (u32RegUTMI+0x2f*2-1)); // chirp patch
#ifdef ENABLE_TX_RX_RESET_CLK_GATING_ECO      
      // DO UTMI TX/RX RESET
      writeb( readb((void*)(u32RegUTMI+0x06*2)) | 0x03, (void*) (u32RegUTMI+0x2c*2));
      writeb( readb((void*)(u32RegUTMI+0x06*2)) & ~0x03, (void*) (u32RegUTMI+0x2c*2));
#endif
      
      //Add time-out mechanism to cover HW BUG
      //HW ISSUE?
      if( time_out >= 10000 )
      {
          //Maybe HW bug ==> check it out
          ms_debug_err("ERR: Port reset cannot force complete!!\n");//HW ISSUE?
          ms_ehci_softrst(pEhci);
      }      
      u32Speed = (hcd_reg_readl((U32)&pEhci->op_regs->bus_control) >> 9) & 0x3;
      ms_debug_msg("SPEED: %x\n", (unsigned int)u32Speed);
      
          u32Temp2 = hcd_reg_readl((U32)&pEhci->op_regs->hcmisc) & 0xfffffff3;

      /* unset UTMI force full speed hub driving timing */
      ms_UTMI_ANDXBYTE_EX(0x03, (~BIT2), u32RegUTMI);

      if (u32Speed == 2) // high u32Speed
      {
        ms_UTMI_ORXBYTE_EX(0x09, 0x80, u32RegUTMI);                            //HS rx robust enable
#if defined(ENABLE_16US_EOF1)
        /* set EOF1 to 16us for babble prvention under hub case */
        //diag_printf("[EHCI] set 16us EOF1\n ");  
        u32Temp2 |= (1 << 2);
#else
        u32Temp2 |= (3 << 2);
#endif
      }
      else if (u32Speed == 0 ) //full u32Speed
      {
        /* set UTMI force full speed hub driving timing */
        ms_UTMI_ORXBYTE_EX(0x03, BIT2, u32RegUTMI);

        ms_UTMI_ANDXBYTE_EX(0x09, 0x7F, u32RegUTMI);
        u32Temp2 |= (2 << 2);
      }
      else if (u32Speed == 1 ) //low u32Speed
      {
        ms_UTMI_ANDXBYTE_EX(0x09, 0x7F, u32RegUTMI);
        u32Temp2 |= (2 << 2);
      }
      hcd_reg_writel (u32Temp2, (U32)&pEhci->op_regs->hcmisc); // misc, EOF1
      
      u32Temp = ms_reset_complete (pEhci, u16Index, u32Temp);
      pHcd->isRootHubPortReset = FALSE;
    }

    // don't show wPortStatus if it's owned by a companion hc
    if (!(u32Temp & PORTSC_OWNER)) 
    {
      if (u32Temp & PORTSC_CONNECT) 
      {
        u32Status |= USB_PORT_STAT_CONNECTION;
        u32Bus_monitor=hcd_reg_readl ((U32)&pEhci->op_regs->bus_control);
        u32Bus_monitor>>=9;
        if (u32Bus_monitor==2)
            u32Status |= USB_PORT_STAT_HIGH_SPEED;
        else if (u32Bus_monitor==1)
            u32Status |= USB_PORT_STAT_LOW_SPEED;
        //else ==> full u32Speed don't set any bits

      }
      if (u32Temp & PORTSC_PE)
        u32Status |= USB_PORT_STAT_ENABLE;
      if (u32Temp & PORTSC_SUSPEND)
        u32Status |= USB_PORT_STAT_SUSPEND;	  
      if (u32Temp & PORTSC_OC)
        u32Status |= USB_PORT_STAT_OVERCURRENT;
      if (u32Temp & PORTSC_RESET)
        u32Status |= USB_PORT_STAT_RESET;
      if (u32Temp & PORTSC_POWER)
        u32Status |= USB_PORT_STAT_POWER;
    }
    
    *pBuf = u32Status;
    *(pBuf+1) = u32Status >> 8;
    *(pBuf+2) = u32Status >> 16;
    *(pBuf+3) = u32Status >> 24;
    break;
  case Req_SetHubFeature:
    if ((u16Value != C_HUB_LOCAL_POWER)&&(u16Value != C_HUB_OVER_CURRENT))
      goto error;    
    break;
  case Req_SetPortFeature:
    if (!u16Index || u16Index > iPorts)
      goto error;
    u16Index--;
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->portsc [u16Index]);
    #if 0
    if (u32Temp & PORTSC_OWNER)
      break;
    #endif

    switch (u16Value) 
    {
    case USB_PORT_FEAT_SUSPEND:
      ms_debug_msg("SetPortFeature suspend\n");
      hcd_reg_writel (u32Temp | PORTSC_SUSPEND,
        (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_POWER:
      if (HCS_PPC (pEhci->hcs_params))
        hcd_reg_writel (u32Temp | PORTSC_POWER,
          (U32)&pEhci->op_regs->portsc [u16Index]);
      break;
    case USB_PORT_FEAT_RESET:      
      {        
        u32Temp |= PORTSC_RESET;
        u32Temp &= ~PORTSC_PE;

        /*                
                 * 50 ms resets on root for usb spec.
                 */
        pEhci->u32ResetEnd [u16Index] = jiffies
              + ((50 /* msec */ * HZ) / 1000);
      }

      //Do port reset only if device attaches to this port
      if( u32Temp&PORTSC_CONNECT )
      {
        pHcd->isRootHubPortReset = TRUE;
        ms_ehci_stoprun_setting(HOST20_Enable, pEhci);

        writeb(0x10, (void*)(u32RegUTMI+0x2C*2)); // chirp patch
        writeb(0x00, (void*)(u32RegUTMI+0x2D*2-1)); // chirp patch
        writeb(0x00, (void*)(u32RegUTMI+0x2E*2)); // chirp patch
        writeb(0x00, (void*)(u32RegUTMI+0x2F*2-1)); // chirp patch
        // Chirp K detection level: 0x80 => 400mv, 0x20 => 575mv
        writeb(0x80 ,(void*)(u32RegUTMI+0x2A*2)); // chirp patch

        hcd_reg_writel (u32Temp, (U32)&pEhci->op_regs->portsc [u16Index]); // port reset
      }
      break;
    default:
      goto error;
    }
    u32Temp = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);  /* unblock posted writes */
    break;

  default:
error:
    /* issue stall */
    iRetval = -EPIPE;
  }
  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return iRetval;
}

#endif


/*------------------------------------------------------------------------------*/
#include "drvEHCI_MEM.cxx"


#if 1
/* 
  * @brief             fill urb buffer to qtd 
  *
  * @param          struct ehci_qtd *qtd
  * @param          dma_addr_t buf
  * @param          size_t total
  * @param          int token
  * @param          int maxpkt
  *
  * @return           u32Cnt
  */
static int
ms_qtd_pack (struct ehci_qtd *pQtd, dma_addr_t tBufAddr, size_t total,
    int iToken, int iMaxpkt)
{
  U32 u32Cnt;
  int  i, lastnums = 0;  
  unsigned char	u8PageShift;

  /* buffer must continuous, first page address may not on 4K boundary  */ 

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQtd->hw_buffer [0] = tBufAddr;  
  u32Cnt = 0x1000 - (tBufAddr & 0x0fff);  /* rest of that page */
  if (total < u32Cnt)    /* ... iff needed */
    u32Cnt = total;
  else 
  {
    tBufAddr +=  0x1000;
    tBufAddr &= ~0x0fff;

    /* pQtd buffer range : min 16K, max 20K */
    for (i = 1; u32Cnt < total && i < 5; i++) 
    {      
      pQtd->hw_buffer [i] = tBufAddr;      
      lastnums++;
      tBufAddr += 0x1000;
      u32Cnt = ((u32Cnt+0x1000) < total) ? (u32Cnt+0x1000) : total;      
    }

    /* short packets on transfer end */
    if (u32Cnt != total)
      u32Cnt -= (u32Cnt % iMaxpkt);
  }
  pQtd->hw_token = (u32Cnt << 16) | iToken;
  pQtd->length = u32Cnt;
//qtd_status=pQtd->hw_token;			//yuwen
  if ((u32Cnt > 0) && (lastnums < 4))
  {
      for (u8PageShift = lastnums + 1;u8PageShift < 5; u8PageShift++)
      {
          pQtd->hw_buffer[u8PageShift] = pQtd->hw_buffer [lastnums];          
      }
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return u32Cnt;
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             link qtd list to qh
  *
  * @param          struct ehci_hcd *ehci
  * @param          struct ehci_qh *qh
  * @param          struct ehci_qtd *qtd 
  *
  * @return           u32Cnt
  */

static __inline__ void
ms_qh_update_link (struct ehci_qh *pQh, struct ehci_qtd *pQtd)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQh->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
  pQh->hw_alt_next_qtd = EHCI_LIST_END;

  /* HC must see latest pQtd and pQh data before we clear ACTIVE+HALT */
  wmb ();
  pQh->hw_token &= (QTD_TOGGLE | QTD_STS_PERR);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             check qtd status register
  *
  * @param          struct urb *pUrb
  * @param          size_t length
  * @param          U32 u32Token
  *
  * @return           none
  */

static void ms_qtd_update_status (  
  struct urb *pUrb,
  size_t length,
  U32 u32Token
)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  if (QTD_PID (u32Token) != 2)
    pUrb->u32ActualLength += length - QTD_BYTES (u32Token);

  /* error codes 不要去改它 */
  if (pUrb->s32Status != -EINPROGRESS)
    return;

  /* force cleanup , may not a wrong */
  if (IS_SHORT_READ (u32Token))
    pUrb->s32Status = -MS_ERR_REMOTEIO;  

  if (u32Token & QTD_STS_HALT) 
  {
    if (u32Token & QTD_STS_BABBLE) 
    {
      diag_printf(" [ !!!!  USB   QTD_STS_BABBLE  !!!!] \n "); // SHOULD BE PRINT  
      pUrb->s32Status = -MS_ERR_OVERFLOW;
    } 
    else if (u32Token & QTD_STS_MISSMF) 
    {
        diag_printf(" [ !!!!  USB	QTD_STS_MISSMF	!!!!] \n "); // SHOULD BE PRINT  
      /* FS/LS transfer lost complete-split */
      pUrb->s32Status = -MS_ERR_PROTOCAL;
    }
    else if (u32Token & QTD_STS_DATERR) 
    {
        diag_printf(" [ !!!!  USB	QTD_STS_DATERR	!!!!] \n "); // SHOULD BE PRINT  
      pUrb->s32Status = (QTD_PID (u32Token) == 1) /* Is IN token? */
        ? -MS_ERR_RECEIVE   /* HCD can't read data */
        : -MS_ERR_SEND;     /* HCD can't write data */
    } 
    else if (u32Token & QTD_STS_XACTERR) 
    {
        ms_debug_msg(" [ USB	QTD_STS_XACTERR ] \n ");
      
      if (QTD_CERR (u32Token))    /* 錯誤型態timeout, bad crc, wrong PID */
        pUrb->s32Status = -EPIPE;
      else 
      {
          ms_debug_warn ("devpath %s ep%d%s 3strikes\n",
                pUrb->dev->devpath,
                (int)usb_pipeendpoint (pUrb->u32Pipe),
                usb_pipein (pUrb->u32Pipe) ? "in" : "out");
                pUrb->s32Status = -MS_ERR_PROTOCAL;
      }
    /* 沒有error bit 但是有halt, will stall */
    } 
    else if (QTD_CERR (u32Token))
      pUrb->s32Status = -EPIPE;
    else  /* 未知的錯誤 */
      pUrb->s32Status = -MS_ERR_PROTOCAL;    

    if( pUrb->s32Status < 0 )
    {      
      ms_debug_warn("dev%d ep%d%s qtd token %08x --> status %d, len=%d\n",
            (int)usb_pipedevice (pUrb->u32Pipe),
            (int)usb_pipeendpoint (pUrb->u32Pipe),
            usb_pipein (pUrb->u32Pipe) ? "in" : "out",
            (unsigned int)u32Token, (int)pUrb->s32Status, (int)pUrb->u32TransferBufferLength);
    }


    /* Stall 表示需要做error recovery */
    if (pUrb->s32Status == -EPIPE) 
    {
      int  pipe = pUrb->u32Pipe;

      if (!usb_pipecontrol (pipe))
          usb_endpoint_halt (pUrb->dev,
          usb_pipeendpoint (pipe),
          usb_pipeout (pipe));	  

    /* if async CSPLIT failed, try cleaning out the TT buffer */
#ifdef USB_EHCI_TT
    } 
    else if (pUrb->dev->tt && !usb_pipeint (pUrb->u32Pipe)
        && QTD_CERR(u32Token) == 0) 
    {
#ifdef DEBUG
      struct usb_device *tt = pUrb->dev->tt->hub;
      ms_debug_msg ("clear tt buffer port %d, a%d ep%d t%08x\n",
        pUrb->dev->u32TTPort, pUrb->dev->u32DevNum,
        usb_pipeendpoint (pUrb->u32Pipe), u32Token);
#endif 
      ms_usb_hub_tt_clear_buffer (pUrb->dev, pUrb->u32Pipe);
    }
#else
    }
#endif
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}


static void
ms_urb_done (struct ehci_hcd *pEhci, struct urb *pUrb, struct stPtRegs *pRegs)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (pUrb->hcpriv != 0)
  {
    struct ehci_qh  *qh = (struct ehci_qh *) pUrb->hcpriv;    
    ms_qh_put (pEhci, qh);
  }  

  osapi_spin_lock (&pUrb->lock);
  pUrb->hcpriv = 0;
  switch (pUrb->s32Status) 
  {
  case -EINPROGRESS:    /* success */
    pUrb->s32Status = 0;
  default:      /* fault */
    INCREASE (pEhci->stats.u32Complete);
    break;
  case -MS_ERR_REMOTEIO:    /* fault or normal */
    if (!(pUrb->u32TransferFlags & MS_FLAG_URB_SHORT_NOT_OK))
      pUrb->s32Status = 0;
    INCREASE (pEhci->stats.u32Complete);
    break;	
  case -ECONNRESET:    /* canceled */
  case -ENOENT:
    INCREASE(pEhci->stats.u32Unlink);
    break;
  }
  osapi_spin_unlock (&pUrb->lock);

  /* HCD run complete() funciton */
  osapi_spin_unlock (&pEhci->tHcdLock);
  ms_usb_hcd_giveback_urb (pUrb, pRegs);
  osapi_spin_lock (&pEhci->tHcdLock);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/* 
  * @brief             free qtd list and run urb's complet function  
  *
  * @param          struct ehci_hcd *ehci
  * @param          struct ehci_qh *pQh
  * @param          struct stPtRegs *regs
  *
  * @return           struct list_head *
  */

#define HALT_BIT QTD_STS_HALT
static unsigned
ms_qh_finish (struct ehci_hcd *pEhci, struct ehci_qh *pQh, struct stPtRegs *pRegs)
{
  struct ehci_qtd    *pLast = 0, *pEnd = pQh->pDummyQtd;
  struct list_head  *entry, *tmp;
  int      iStopped;
  unsigned    u8Count = 0;
  int      iDoStatus = 0;
  unsigned char      u8State;
  BOOL      bIsPageOver = FALSE; 

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (ms_is_empty_list (&pQh->qtd_list))
  {
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return u8Count;
  }  

  u8State = pQh->qh_status;
  pQh->qh_status = QH_STS_COMPLETING;
  iStopped = (u8State == QH_STS_IDLE);  

  list_for_loop_ex (entry, tmp, &pQh->qtd_list) 
  {
    struct ehci_qtd  *pQtd;
    struct urb  *pUrb;
    U32    u32Token = 0;

    pQtd = entry_to_container (entry, struct ehci_qtd, qtd_list);
    //list_entry (entry, struct ehci_qtd,qtd_list, struct list_head,qtd);
    pUrb = pQtd->urb;

    if ( ((pQtd->hw_token >> 12) & 0x7) > 5)	//For patching H/W bug
    {    
        bIsPageOver = TRUE;
    }

    /* free previous QTD ...*/
    if (pLast) 
    {
      if (pLast->urb != pUrb)
      {
        ms_urb_done (pEhci, pLast->urb, pRegs);
        u8Count++;
      }  
      ms_ehci_qtd_free (pEhci, pLast);
      pLast = 0;
    }
   
    if (pQtd == pEnd)
      break;

    /* HW implement qtds content copy */
    rmb ();

    u32Token = pQtd->hw_token;
    
    if ((u32Token & QTD_STS_ACT) == 0) 
    {
      if ((u32Token & QTD_STS_HALT) != 0) 
      {
        iStopped = 1;
      } 
      else if (IS_SHORT_READ (u32Token)
          && (pQh->hw_alt_next_qtd & QTD_MASK)
            == pEhci->stAsync->hw_alt_next_qtd) 
      {
        iStopped = 1;
        goto halt;
      }     
    } 
    else if(!iStopped && HCD_IS_RUNNING (pEhci->hcd.state)) 
    {
      break;
    }
    else
    {
      iStopped = 1;  

      if (pUrb->s32Status == -EINPROGRESS)
        continue;

      /* issue status for control transfer */
      if ((iDoStatus != 0)
          && QTD_PID (u32Token) == 0)   //Check is OUT token?
      {
        iDoStatus = 0;
        continue;
      }
    
      if (u8State == QH_STS_IDLE
          && pQtd->qtd_dma_addr
            == pQh->hw_current_qtd)
        u32Token = pQh->hw_token;

      if ((HALT_BIT & pQh->hw_token) == 0) 
      {
halt:
        pQh->hw_token |= HALT_BIT;
        wmb ();
      }
    }


    /* remove it from the queue */
    osapi_spin_lock (&pUrb->lock);
    ms_qtd_update_status (pUrb, pQtd->length, u32Token);
    if (bIsPageOver)
    {
        ms_debug_warn("urb error because C_Page > 5\n");
        pUrb->s32Status = (QTD_PID (u32Token) == 1) /* Is IN token? */
        ? -MS_ERR_RECEIVE   /* HCD can't read data */
        : -MS_ERR_SEND;     /* HCD can't write data */
    }
    iDoStatus = (pUrb->s32Status == -MS_ERR_REMOTEIO)
        && usb_pipecontrol (pUrb->u32Pipe);
    osapi_spin_unlock (&pUrb->lock);

    if (iStopped && pQtd->qtd_list.prev != &pQh->qtd_list) {
      pLast = entry_to_container (pQtd->qtd_list.prev, struct ehci_qtd, qtd_list);
      //list_entry (qtd->qtd_list.prev,struct ehci_qtd,qtd_list, struct list_head,last);
      pLast->hw_next_qtd = pQtd->hw_next_qtd;

    }
    ms_list_remove (&pQtd->qtd_list);
    pLast = pQtd;
  }

  if (pLast != 0) 
  {
    ms_urb_done (pEhci, pLast->urb, pRegs);
    u8Count++;
    ms_ehci_qtd_free (pEhci, pLast);
    pLast = 0;
  }  

  pQh->qh_status = u8State;

  if ((iStopped != 0)
      /* avoid some old EHCI vendor will overlay dummy qtds */
      || pQh->hw_next_qtd == EHCI_LIST_END) 
  {
    if (ms_is_empty_list (&pQh->qtd_list))
      pEnd = pQh->pDummyQtd;
    else
    {
      pEnd = entry_to_container (pQh->qtd_list.next,struct ehci_qtd, qtd_list);      
      if (pEnd->qtd_dma_addr == pQh->hw_current_qtd)
        pEnd = 0;
    }
    if (pEnd)
      ms_qh_update_link (pQh, pEnd);
  }

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  return u8Count;
}

/*-------------------------------------------------------------------------*/
// high bandwidth multiplier, as encoded in highspeed endpoint descriptors
#define hb_mult(wMaxPacketSize) (1 + (((wMaxPacketSize) >> 11) & 0x03))
// ... and packet size, for any kind of endpoint descriptor
#define max_packet(wMaxPacketSize) ((wMaxPacketSize) & 0x07ff)

static void ms_qtd_register_free (
  struct ehci_hcd    *pEhci, 
  struct list_head  *qtd_list
)
{
  struct list_head  *pEntry, *pTemp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  list_for_loop_ex (pEntry, pTemp, qtd_list) 
  {
    struct ehci_qtd  *pQtd;	

    pQtd = entry_to_container (pEntry, struct ehci_qtd, qtd_list);    
    ms_list_remove (&pQtd->qtd_list);
    ms_ehci_qtd_free (pEhci, pQtd);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/* 
  * @brief             prepare and allocate qtd_list
  *                       fill buffer to qtd from urb 
  *
  * @param          struct ehci_hcd    *ehci
  * @param          struct urb    *pUrb
  * @param          struct list_head  *pHead
  * @param          int      iFlags
  *
  * @return           struct list_head *
  */

static struct list_head *
ms_qh_urb_process (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *pHead,
  int      iFlags
) 
{
  struct ehci_qtd    *pQtd, *pQtdPrev;
  dma_addr_t    tBuf;
  int      iLen, iMaxpkt;
  int      iIsInput;
  U32      u32Token;  

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
  if (!pQtd)
  {
      ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      return 0;
  }    
  ms_insert_list_before (&pQtd->qtd_list, pHead);
  pQtd->urb = pUrb;

  u32Token = QTD_STS_ACT;
  u32Token |= (MAX_CERR_CNT << 10);
  /* for split transactions, SplitXState initialized to zero */
  
  iLen = pUrb->u32TransferBufferLength;
  iIsInput = usb_pipein (pUrb->u32Pipe);
  if (usb_pipecontrol (pUrb->u32Pipe)) 
  {    
    ms_qtd_pack (pQtd, pUrb->tSetupDma, sizeof (struct usb_ctrlrequest),
      u32Token | PID_SETUP, 8);

    /* ... and always at least one more pid */
    u32Token ^= QTD_TOGGLE;
    pQtdPrev = pQtd;
    pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
    if (!pQtd)
      goto cleanup;
    pQtd->urb = pUrb;
    pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
    ms_insert_list_before (&pQtd->qtd_list, pHead);
  }

  /*
      * data stage : prepare buffer
      */   
  
  if (iLen > 0)
    tBuf = pUrb->tTransferDma;
  else
    tBuf = 0;

  if (!tBuf || iIsInput)
    u32Token |= PID_IN;
  
  #if 0
  iMaxpkt = max_packet(usb_maxpacket(pUrb->dev, pUrb->u32Pipe, !iIsInput));
  #else
  if (usb_pipebulk(pUrb->u32Pipe)) 
  {
    switch (pUrb->dev->eSpeed) 
    {
      case USB_FULL_SPEED:
        iMaxpkt = max_packet(usb_maxpacket(pUrb->dev, pUrb->u32Pipe, !iIsInput));
        break;
      case USB_HIGH_SPEED:
      default:
        iMaxpkt = 512;
        break;
    }
  }
  else 
  {
    iMaxpkt = max_packet(usb_maxpacket(pUrb->dev, pUrb->u32Pipe, !iIsInput));
  }

  #endif  

  for (;;) 
  {
    int this_qtd_len;

    this_qtd_len = ms_qtd_pack (pQtd, tBuf, iLen, u32Token, iMaxpkt);
    iLen -= this_qtd_len;
    tBuf += this_qtd_len;
    if (iIsInput)
      pQtd->hw_alt_next_qtd = pEhci->stAsync->hw_alt_next_qtd;	

    /* for control transaction, maybe needs switch it */
    if ((iMaxpkt & (this_qtd_len + (iMaxpkt - 1))) == 0)
      u32Token ^= QTD_TOGGLE;

    if (iLen <= 0)
      break;

    pQtdPrev = pQtd;
    pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
    if (!pQtd)
      goto cleanup;
    pQtd->urb = pUrb;
    pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
    ms_insert_list_before (&pQtd->qtd_list, pHead);
  }  

  if ((pUrb->u32TransferFlags & MS_FLAG_URB_SHORT_NOT_OK) == 0
        || usb_pipecontrol (pUrb->u32Pipe))
    pQtd->hw_alt_next_qtd = EHCI_LIST_END;   
  
  if (tBuf != 0) 
  {
    int  one_more = 0;

    if (usb_pipecontrol (pUrb->u32Pipe)) 
    {
      one_more = 1;
      u32Token ^= 0x0100;      /* PID IN/OUT taggle  */
      u32Token |= QTD_TOGGLE;  /* set DATA1 */
    } 
    else if (usb_pipebulk (pUrb->u32Pipe)
        && (pUrb->u32TransferFlags & MS_FLAG_URB_ZERO_PACKET)
        && !(pUrb->u32TransferBufferLength % iMaxpkt)) 
    {
      one_more = 1;
    }

    if (one_more) 
    {
      pQtdPrev = pQtd;
      pQtd = ms_ehci_qtd_alloc (pEhci, iFlags);
      if (!pQtd)
        goto cleanup;
      pQtd->urb = pUrb;
      pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
      ms_insert_list_before (&pQtd->qtd_list, pHead);	  

      /* make NULL packet */
      ms_qtd_pack (pQtd, 0, 0, u32Token, 0);
    }
  }

  /* set interrupt enable when qtd completed */
  if (!(pUrb->u32TransferFlags & MS_FLAG_URB_NO_INTERRUPT))
    pQtd->hw_token |= QTD_IOC;  

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return pHead;

cleanup:
 
  ms_qtd_register_free (pEhci, pHead);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/

static __inline__ void
ms_clear_toggle (struct usb_device *pUdev, int iEp, int is_out, struct ehci_qh *pQh)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  ms_debug_msg ("dev %d iEp 0x%x-%s\n",
        (int)pUdev->u32DevNum, iEp, is_out ? "out" : "in");
  pQh->hw_token &= ~(QTD_TOGGLE);  
  
  usb_settoggle (pUdev, iEp, is_out, 1);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/* 
  * @brief             allocate and make qh                       
  *
  * @param          struct ehci_hcd    *ehci
  * @param          struct urb    *pUrb  
  * @param          int      iFlags
  *
  * @return           struct list_head *
  */

static struct ehci_qh *
ms_qh_create (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  int      iFlags
) 
{
  struct ehci_qh    *pQh = ms_ehci_qh_alloc (pEhci, iFlags);
  U32      u32Info1 = 0, u32Info2 = 0;
  int      iIsInput, iType;
  int      iMaxpkt = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (!pQh)
  {
      ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      return pQh;  
  }
  
  u32Info1 |= usb_pipeendpoint (pUrb->u32Pipe) << 8;
  u32Info1 |= usb_pipedevice (pUrb->u32Pipe) << 0;  

  iIsInput = usb_pipein (pUrb->u32Pipe);
  iType = usb_pipetype (pUrb->u32Pipe);
  iMaxpkt = usb_maxpacket (pUrb->dev, pUrb->u32Pipe, !iIsInput);

  if (iType == EP_INTERRUPT) 
  {
    pQh->u8Usecs = ms_usb_calc_bus_time (USB_HIGH_SPEED, iIsInput, 
        hb_mult (iMaxpkt) * max_packet (iMaxpkt));

    pQh->u16Start = NO_FRAME;

    if (pUrb->dev->eSpeed == USB_HIGH_SPEED) 
    {
      pQh->c_usecs = 0;
      pQh->u8Gap_uf = 0;

      /* Warning, highspeed periods < 1 frame. */
      pQh->u16Period = pUrb->u32Interval >> 3;
      if (pQh->u16Period < 1) 
      {
        ms_debug_warn ("intr period %d uframes, NYET!", (int)pUrb->u32Interval);
        goto done;
      }
    } 
    else 
    {
  
      /* FS/LS transfer times */
      pQh->u8Gap_uf = 1 + ms_usb_calc_bus_time (pUrb->dev->eSpeed,
          iIsInput, iMaxpkt) / (125 * 1000);

      /* Warning, let SPLIT/CSPLIT times to close */
      if (iIsInput) 
      {    // SPLIT, gap, CSPLIT+DATA
        pQh->c_usecs = pQh->u8Usecs + HIGHSPEED_US (0);
        pQh->u8Usecs = HIGHSPEED_US (1);
      }
      else 
      {    // SPLIT+DATA, gap, CSPLIT
        pQh->u8Usecs += HIGHSPEED_US (1);
        pQh->c_usecs = HIGHSPEED_US (0);
      }
      pQh->u16Period = pUrb->u32Interval;

    }
  }  

  if (pUrb->dev->eSpeed == USB_HIGH_SPEED) 
  {
    u32Info1 |= (2 << 12);  
  }
  else if (pUrb->dev->eSpeed == USB_LOW_SPEED)
  {
    u32Info1 |= (1 << 12);
  }
  else
  {
    if (pUrb->dev->eSpeed != USB_FULL_SPEED)
       ms_debug_warn ("unknow speed %d", pUrb->dev->eSpeed);  
  }  

  /* Is TT? */
  switch (pUrb->dev->eSpeed) 
  {

  case USB_LOW_SPEED:
    if (iType == EP_CONTROL) 
    {
      u32Info1 |= 8 << 16;  /* hard code maxpacket item */
    }
   
  case USB_FULL_SPEED:
    if (iType == EP_CONTROL && (pUrb->dev->eSpeed == USB_FULL_SPEED) ) 
    {
      //info1 |= 64 << 16;  // fixed maxpacket
      u32Info1 |= max_packet (iMaxpkt) << 16; // PIPE_INTERRUPT,PIPE_BULK,PIPE_ISOCHRONOUS
    }
    if (iType != EP_CONTROL) 
    {
      u32Info1 |= max_packet (iMaxpkt) << 16; // PIPE_INTERRUPT,PIPE_BULK,PIPE_ISOCHRONOUS
    }
    /* EPS 0 means "full" */
  //  info1 |= (3 << 12);  /* EPS "reserve" */
    if (iType != EP_INTERRUPT)
      u32Info1 |= (EHCI_RL_NAKCNT_TT << 28);
    if (iType == EP_CONTROL) 
    {
      u32Info1 |= (1 << 27);  /* for TT */
      u32Info1 |= 1 << 14;    /* toggle from qtd */
    }
    u32Info2 |= (EHCI_HI_BW_MULT_TT << 30);	

    if (pUrb->dev->tt->hub->u32DevNum != 1)     // Skip the Root Hub devnum == 1
    {
        u32Info2 |= pUrb->dev->u32TTPort << 23;
        u32Info2 |= pUrb->dev->tt->hub->u32DevNum << 16;
    }
    /* NOTE:  if (EP_INTERRUPT) { scheduler sets c-mask } */
    break;
  case USB_HIGH_SPEED:      /* 與 TT 無關 */
    u32Info1 |= (2 << 12);  /* EPS is 480Mb/s */	  

    if (iType == EP_CONTROL) 
    {
      u32Info1 |= (EHCI_RL_NAKCNT << 28);
      u32Info1 |= 64 << 16;  /* Endpoint (wMaxPacketSize) */
      u32Info1 |= 1 << 14;   /* Data toggle control */
      u32Info2 |= (EHCI_HI_BW_MULT_HS << 30);
    }
    else if (iType == EP_BULK) 
    {
      u32Info1 |= (EHCI_RL_NAKCNT << 28);      
      u32Info1 |= max_packet (iMaxpkt) << 16;  // Philips mp3 player endpoint descriptor bug      
      u32Info2 |= (EHCI_HI_BW_MULT_HS << 30);
    }
    else 
    {    /* Other transation type */
      u32Info1 |= max_packet (iMaxpkt) << 16;
      u32Info2 |= hb_mult (iMaxpkt) << 30;
    }
    break;
  default:
     ms_debug_msg ("usb dev %p speed %d", pUrb->dev, pUrb->dev->eSpeed);
done:
    ms_qh_put (pEhci, pQh);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return 0;
  }
 
  pQh->qh_status = QH_STS_IDLE;
  pQh->hw_ep_state1 = u32Info1;
  pQh->hw_ep_state2 = u32Info2;
  ms_qh_update_link (pQh, pQh->pDummyQtd);
  usb_settoggle (pUrb->dev, usb_pipeendpoint (pUrb->u32Pipe), !iIsInput, 1);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return pQh;
}
#undef hb_mult
#undef hb_packet

/*-------------------------------------------------------------------------*/
/* 
  * @brief             start async schedule                    
  *
  * @param          struct ehci_hcd    *ehci
  * @param          struct ehci_qh  *qh 
  *
  * @return           struct list_head *
  */

static void ms_qh_create_link_async (struct ehci_hcd *pEhci, struct ehci_qh  *pQh)
{
  U32    u32DmaAddr = QH_NEXT (pQh->qh_dma_addr);
  struct ehci_qh  *pHead;  

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  pHead = pEhci->stAsync;
  ms_timer_action_done (pEhci, TIMER_ASYNC_OFF);

  if (!pHead->qh_next.qh) 
  {
    U32  cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    //If disable => restart it
    if (!(cmd & USBCMD_ASE)) 
    {      
      (void) ms_check_status (&pEhci->op_regs->usbsts, USBSTS_ASS, 0, 150);
      cmd |= USBCMD_ASE | USBCMD_RUN;         //asynchronous scheduler enable
      hcd_reg_writel (cmd, (U32)&pEhci->op_regs->usbcmd);
      pEhci->hcd.state = USB_STATE_RUNNING;     
    }
  }

  /* Don't write QH memory if not necessary */
  if(pQh->hw_token & HALT_BIT)
    pQh->hw_token &= ~HALT_BIT; 

  pQh->qh_next = pHead->qh_next;
  pQh->hw_next_qh = pHead->hw_next_qh;
  wmb ();

  pHead->qh_next.qh = pQh;
  pHead->hw_next_qh = u32DmaAddr;
  pQh->qh_status = QH_STS_LINKED; 
  /* IRQ will report qtd completions event */
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/

#define  QH_ADDR_MASK  0x7f

static struct ehci_qh *ms_qh_append_tds (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *qtd_list,
  int      iEpNum,
  void      **ptr
)
{
  struct ehci_qh    *pQh = 0;
  //int maxp;
  pQh = (struct ehci_qh *) *ptr;
  
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (pQh == 0)
  {    
    pQh = ms_qh_create (pEhci, pUrb, SLAB_ATOMIC);
    *ptr = pQh;
  }  

  // ep0 maxpacket will be updated here after HCD issue get_descriptor.
  if ( usb_pipecontrol(pUrb->u32Pipe) )
  {
    //maxp = (usb_maxpacket (urb->dev, urb->pipe, !(usb_pipein (urb->pipe)))<<16);
    pQh->hw_ep_state1 = ((pQh->hw_ep_state1&(~(((1<<11)-1)<<16)))|(usb_maxpacket (pUrb->dev, pUrb->u32Pipe, !(usb_pipein (pUrb->u32Pipe)))<<16));
  }
  if (pQh != 0) 
  {
    struct ehci_qtd  *pQtd;

    if (ms_is_empty_list (qtd_list))
      pQtd = 0;
    else
    {
      pQtd = entry_to_container (qtd_list->next, struct ehci_qtd, qtd_list);
      //list_entry (qtd_list->next, struct ehci_qtd, qtd_list, struct list_head, qtd);
    }

    /* control qh may need patching after enumeration */
    if (iEpNum == 0)
    {
      /* modify device address */
      if ((pQh->hw_ep_state1 & QH_ADDR_MASK) == 0)
           pQh->hw_ep_state1 |= usb_pipedevice (pUrb->u32Pipe);	  
      
      else if (!(pQh->hw_ep_state1 & (0x3 << 12))) 
      {
        U32  u32Info, u32Max;

        u32Info = pQh->hw_ep_state1;
        u32Max = pUrb->dev->descriptor.bMaxPacketSize0;
        if (u32Max > (0x07ff & (u32Info >> 16))) 
        {
          u32Info &= ~(0x07ff << 16);
          u32Info |= u32Max << 16;
          pQh->hw_ep_state1 = u32Info;
        }
      }
      
      if (usb_pipedevice (pUrb->u32Pipe) == 0)
          pQh->hw_ep_state1 &= ~QH_ADDR_MASK;
    }

    if ((!usb_gettoggle (pUrb->dev,
          (iEpNum & 0x0f), !(iEpNum & 0x10)))
        && !usb_pipecontrol (pUrb->u32Pipe)) 
    {
      /* "never happens": drivers do stall cleanup right */
      if (pQh->qh_status != QH_STS_IDLE
          && !ms_is_empty_list (&pQh->qtd_list)
          && pQh->qh_status != QH_STS_COMPLETING)
        ms_debug_msg ("clear toggle dev%d ep%d%s: not idle\n", (int)usb_pipedevice (pUrb->u32Pipe), iEpNum & 0x0f,  usb_pipein (pUrb->u32Pipe) ? "in" : "out");        
        ms_clear_toggle (pUrb->dev,
        iEpNum & 0x0f, !(iEpNum & 0x10), pQh);
    }

    if (pQtd != 0)
    {
      struct ehci_qtd    *dummy;
      dma_addr_t    tDmaAddr;
      U32      pToken;

      pToken = pQtd->hw_token;
      pQtd->hw_token = HALT_BIT;
      wmb ();
      dummy = pQh->pDummyQtd;
      //Copy qtd's data to dummy except dumm->qtd_dma
      tDmaAddr = dummy->qtd_dma_addr;
      *dummy = *pQtd;
      dummy->qtd_dma_addr = tDmaAddr;

      ms_list_remove (&pQtd->qtd_list);
      ms_insert_list_after (&dummy->qtd_list, qtd_list);
      ms_list_join (qtd_list, pQh->qtd_list.prev);	  

      ms_ehci_qtd_init (pQtd, pQtd->qtd_dma_addr);
      pQh->pDummyQtd = pQtd;

      /* hc must see the new dummy at list end */
      tDmaAddr = pQtd->qtd_dma_addr;
      pQtd = entry_to_container (pQh->qtd_list.prev, struct ehci_qtd, qtd_list);
      //list_entry (qh->qtd_list.prev, struct ehci_qtd, qtd_list,struct list_head, qtd);
      //the last qtd of qh before spliceing qtd_list
      pQtd->hw_next_qtd = (U32)(tDmaAddr);

      /* let the hc process these next qtds */
      wmb ();
      dummy->hw_token = pToken;

      pUrb->hcpriv = ms_qh_get (pQh);
    }
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return pQh;
}

/*-------------------------------------------------------------------------*/
static int
ms_submit_async (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *qtd_list
) 
{
  struct ehci_qtd    *pQtd;
  struct s_hcd_dev    *pDev;
  int      iEpNum;
  U32    u32Flags;
  struct ehci_qh    *pQh = 0;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  pQtd = entry_to_container (qtd_list->next, struct ehci_qtd, qtd_list);
  //list_entry (qtd_list->next, struct ehci_qtd, qtd_list, struct list_head, qtd);
  pDev = (struct s_hcd_dev *)pUrb->dev->hcpriv;
  iEpNum = usb_pipeendpoint (pUrb->u32Pipe);
  if (usb_pipein (pUrb->u32Pipe) && !usb_pipecontrol (pUrb->u32Pipe))
    iEpNum |= 0x10;
  
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  pQh = ms_qh_append_tds (pEhci, pUrb, qtd_list, iEpNum, &pDev->ep [iEpNum]);
  /* EP Control/bulk 經過 TT 不需要 scheduling */   
  if (pQh != 0) 
  {
    if (pQh->qh_status == QH_STS_IDLE)
      ms_qh_create_link_async (pEhci, ms_qh_get (pQh));
  }

  Chip_Flush_Memory();

  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  if (pQh == 0)
  {
      ms_qtd_register_free (pEhci, qtd_list);
      return -ENOMEM;
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/

static void ms_begin_unlink_async (struct ehci_hcd *pEhci, struct ehci_qh *pQh);
static void ms_end_unlink_async (struct ehci_hcd *pEhci, struct stPtRegs *pRegs)
{
  struct ehci_qh    *pQh = pEhci->stReclaim;
  struct ehci_qh    *pNext;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if(pQh == NULL)
  {
     ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
     return;
  }   
  ms_timer_action_done (pEhci, TIMER_IAA_WATCHDOG);
  Chip_Read_Memory(); // 20121218, 20121225  

  pQh->qh_status = QH_STS_IDLE;
  pQh->qh_next.qh = 0;
  ms_qh_put (pEhci, pQh);      // pQh is directed to reclaim
  
  pNext = pQh->pReclaimQh;
  pEhci->stReclaim = pNext;
  pEhci->iReclaimReady = 0;
  pQh->pReclaimQh = 0;
  
  ms_qh_finish (pEhci, pQh, pRegs);

  if (!ms_is_empty_list (&pQh->qtd_list)
      && HCD_IS_RUNNING (pEhci->hcd.state))
    ms_qh_create_link_async (pEhci, pQh);
  else 
  {
    ms_qh_put (pEhci, pQh);    // pQh is directed from async list

    if (HCD_IS_RUNNING (pEhci->hcd.state)
        && pEhci->stAsync->qh_next.qh == 0)
      ms_timer_action (pEhci, TIMER_ASYNC_OFF);
  }

  if (pNext) 
  {
    pEhci->stReclaim = 0;
    ms_begin_unlink_async (pEhci, pNext);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

static void ms_begin_unlink_async (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
  int    iUsbCmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
  struct ehci_qh  *pPrev;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
#ifdef DEBUG
  if (pEhci->stReclaim
      || (pQh->qh_status != QH_STS_LINKED
        && pQh->qh_status != QH_STS_UNLINK_WAIT)
// If support SMP, needs to check locked
//      || !spin_is_locked (&pEhci->tHcdLock)
      )  
#endif
    
  if (pQh == pEhci->stAsync)
  {    
    if (pEhci->hcd.state != USB_STATE_HALT)
    {
      /* stop async schedule */ 
      hcd_reg_writel (iUsbCmd & ~USBCMD_ASE, (U32)&pEhci->op_regs->usbcmd);
      wmb ();
      // handshake later, if we need to
    }
    ms_timer_action_done (pEhci, TIMER_ASYNC_OFF);
    return;
  }

  pQh->qh_status = QH_STS_UNLINK;
  pEhci->stReclaim = pQh = ms_qh_get (pQh);

  pPrev = pEhci->stAsync;
  while (pPrev->qh_next.qh != pQh)
    pPrev = pPrev->qh_next.qh;

  pPrev->hw_next_qh = pQh->hw_next_qh;
  pPrev->qh_next = pQh->qh_next;
  wmb ();
  
  if (pEhci->hcd.state == USB_STATE_HALT)
  {    
    ms_end_unlink_async (pEhci, NULL);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return;
  }
  //inform HC thar something has been removed from asyn. schedule
  pEhci->iReclaimReady = 0;
#if 1   // For bug 125MHz copy error (HALT)
  //if (!pEhci->uDontSendIAA) // 20130110 no need flag control
  {
    iUsbCmd |= USBCMD_IAAD;
    hcd_reg_writel (iUsbCmd, (U32)&pEhci->op_regs->usbcmd);   
  }
#endif
  ms_timer_action (pEhci, TIMER_IAA_WATCHDOG);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             free qtd list but kept qh for plug test shall cause some qroblems
  *
  * @param          struct ehci_hcd *ehci
  * @param          struct stPtRegs *regs 
  *
  * @return           none
  */

static void
ms_search_async (struct ehci_hcd *pEhci, struct stPtRegs *pRegs)
{
  struct ehci_qh    *pQh;
  enum ehci_timer_event  eAction = TIMER_IO_WATCHDOG;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if (!++(pEhci->u32Stamp))
      pEhci->u32Stamp++;
  ms_timer_action_done (pEhci, TIMER_ASYNC_SHRINK);
rescan:
  pQh = pEhci->stAsync->qh_next.qh;
  if (pQh != 0) 
  {
    do 
    {
      /* free qtd for this qh */
      if (!ms_is_empty_list (&pQh->qtd_list)
          && pQh->u32Stamp != pEhci->u32Stamp) 
      {
        int temp;

        pQh = ms_qh_get (pQh);
        pQh->u32Stamp = pEhci->u32Stamp;
        temp = ms_qh_finish (pEhci, pQh, pRegs);
        ms_qh_put (pEhci, pQh);
        if (temp != 0) 
        {
            goto rescan;
        }
      }

      //FIXME: #if 1 is correct procedure but it will cause some problems when do plug testing
      //Watchdog timer will be unlink for unknow reson ==> Therefore, qHD can not be
      //unlinked from async list. (qh->state == USB_STATE_LINKED but USB_STATE_HALT)
      //So when disconnect attached device ep1in (Bulk in) cannot be releaseed.
      #if 0
      if (list_empty (&pQh->qtd_list)) {
        if (pQh->u32Stamp == pEhci->u32Stamp)
          eAction = TIMER_ASYNC_SHRINK;
        else if (!pEhci->stReclaim
              && pQh->qh_status == QH_STS_LINKED)
          ms_begin_unlink_async (pEhci, pQh);
      }	  
      #else
      if (ms_is_empty_list (&pQh->qtd_list))
      {
        if (!pEhci->stReclaim && pQh->qh_status == QH_STS_LINKED)
          ms_begin_unlink_async (pEhci, pQh);
      }
      #endif
      pQh = pQh->qh_next.qh;
    } while (pQh);
  }
  if (eAction == TIMER_ASYNC_SHRINK)
    ms_timer_action (pEhci, TIMER_ASYNC_SHRINK);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}
#endif

#include "drvEHCI_SCHD.cxx"

/*-------------------------------------------------------------------------*/

static void ms_ehci_active(struct ehci_hcd *ehci, struct stPtRegs *regs);

static void ms_ehci_watchdog (U32 u32Param)
{
    struct ehci_hcd    *pEhci = (struct ehci_hcd *) u32Param;
    U32    u32Flags;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    //ms_debug_msg("action: 0x%08X\n",(U32)pEhci->u32Actions);
    osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);

#if 0
    /* avoid vt8235 lost IAA irqs */
    if (pEhci->stReclaim) 
    {
        U32    status = hcd_reg_readl ((U32)&pEhci->op_regs->usbsts);

        if (status & USBSTS_IAA) 
        {
            INCREASE(pEhci->stats.u32LostIAA);
            hcd_reg_writel (USBSTS_IAA, (U32)&pEhci->op_regs->usbsts);
            pEhci->iReclaimReady = 1;
        }
        else if ( (hcd_reg_readl((U32)&pEhci->op_regs->usbcmd)&USBCMD_IAAD) && !(status & USBSTS_IAA))
        {
            ms_debug_warn ("Something wrong ==>lost IAA\n");//HW ISSUE?
            pEhci->iReclaimReady = 1;
        }
    }
#endif

    /* stop async processing after it's idled a bit */
    #if 1    //Periodic schedule issue was fixed, so reopen it 
    if (ms_test_bit (TIMER_ASYNC_OFF, &pEhci->u32Actions))
        ms_begin_unlink_async (pEhci, pEhci->stAsync);
    #endif
    /* pEhci could run by timer, without IRQs ... */
    ms_ehci_active (pEhci, NULL);

    osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);

}

int ms_host_init (struct usb_hcd *pHcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (pHcd); 

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  osapi_spin_lock_init (&ehci->tHcdLock);

  ehci->cap_regs = (struct ehci_cap_regs *) pHcd->uhc_regs;
  ehci->op_regs = (struct ehci_op_regs *) ( (U32)pHcd->uhc_regs +
        (U32)hcd_reg_readb ((U32)&ehci->cap_regs->caplength));  
  
  ehci->hcs_params = hcd_reg_readl ((U32)&ehci->cap_regs->hcsparams);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__); 
  return ms_host_halt (ehci);
}

/* 
  * @brief             start ehci controller
  *
  * @param          struct usb_hcd *hcd
  *
  * @return           none
  */ 

extern void ms_hcd_poll_rh_status(struct usb_hcd *pHcd);
extern MS_U8 MDrv_SYS_GetChipRev(void);
int ms_ehci_begin (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  U32      u32Temp;
  struct usb_device  *pUdev;
  struct usb_bus    *pBus;
  int      iRetval;
  U32      u32HccParams;
  unsigned char       u8Temp;
  
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  
  ehci->u32PeriodicSize = DEFAULT_I_TDPS;
  if ((iRetval = ms_ehci_mem_init (ehci, SLAB_KERNEL)) < 0)
    return iRetval;
  
  u32HccParams = hcd_reg_readl ((U32)&ehci->cap_regs->hcc_params);
  if (HCC_ISO_CACHE (u32HccParams))   // full frame cache
    ehci->u32IThresh = 8;
  else          // N microframes cached
    ehci->u32IThresh = 2 + HCC_ISO_THRES (u32HccParams);

  ehci->stReclaim = 0;
  ehci->iNextUframe = -1;

  if ((iRetval = ms_host_reset (ehci)) != 0)
  {
    ms_ehci_mem_cleanup (ehci);
    return iRetval;
  }

  hcd_reg_writel (INTR_MASK, (U32)&ehci->op_regs->usbintr);
  u32Temp=hcd_reg_readl((U32)&ehci->op_regs->bus_control);
 // temp|=INT_POLAR+HALF_SPEED;
  u32Temp|=INT_POLAR;
  u32Temp&=~VBUS_OFF;
  hcd_reg_writel(u32Temp,(U32)&ehci->op_regs->bus_control);       //set intr high active
  hcd_reg_writel (ehci->tPeriodicDma, (U32)&ehci->op_regs->periodiclistbase);

  ehci->stAsync->qh_next.qh = 0;  
  ehci->stAsync->hw_next_qh = QH_NEXT (ehci->stAsync->qh_dma_addr);
  ehci->stAsync->hw_ep_state1 = QH_H_BIT;
  ehci->stAsync->hw_token = QTD_STS_HALT;  
  ehci->stAsync->hw_next_qtd = EHCI_LIST_END;
  ehci->stAsync->qh_status = QH_STS_LINKED;
  ehci->stAsync->hw_alt_next_qtd = (U32)(ehci->stAsync->pDummyQtd->qtd_dma_addr);
  ms_debug_msg("qh dma addr: %p\n", (void *)ehci->stAsync->qh_dma_addr);
  hcd_reg_writel ((U32)ehci->stAsync->qh_dma_addr, (U32)&ehci->op_regs->asynclistaddr);

  if (HCC_64BIT_ADDR_CAP (u32HccParams)) 
  {
    hcd_reg_writel (0, (U32)&ehci->op_regs->ctrldssegment);	
  }
    
  u32Temp = hcd_reg_readl ((U32)&ehci->op_regs->usbcmd) & 0x0fff;
  if (log2_irq_thresh < 0 || log2_irq_thresh > 6)
    log2_irq_thresh = 0;
  u32Temp |= 1 << (16 + log2_irq_thresh);  
 
#if defined(CHIP_K2)
  // K2 U02 & Newer IC
  if (MDrv_SYS_GetChipRev() >= 0x01)
  {
    park_eco = 3;
  }
#endif

  if (park_eco && HCC_ASPC(u32HccParams)) 
  {
    ms_debug_msg("[EHCI] Enable Park Mode: %d\n", (int)park_eco);
    u32Temp |= USBCMD_PARK;
    u32Temp |= park_eco << 8;
  }
 
  if (HCC_PFLF (u32HccParams)) 
  {    
    u32Temp &= ~(3 << 2);
    u32Temp |= (EHCI_PERIODIC_FLS << 2);	
    switch (EHCI_PERIODIC_FLS) 
    {
    case 0: ehci->u32PeriodicSize = 1024; break;
    case 1: ehci->u32PeriodicSize = 512; break;
    case 2: ehci->u32PeriodicSize = 256; break;
    default:  USB_ASSERT(0, "Not alllowed periodic frame list size\n");
    }
  }
  u32Temp &= ~(USBCMD_IAAD | USBCMD_ASE | USBCMD_PSE),
  
  // Only make HC run when device connects to bus
  u32Temp |= USBCMD_RUN;//mark by yuwen
  hcd_reg_writel (u32Temp, (U32)&ehci->op_regs->usbcmd);  
    
  /* initialize watchdog timer function */

  ms_init_timer (&ehci->stWatchdog);
  ehci->stWatchdog.function = ms_ehci_watchdog;
  ehci->stWatchdog.data = (U32) ehci; 
  pBus = (struct usb_bus *)(&hcd->self);
  pBus->root_hub = pUdev = ms_usb_alloc_dev (NULL, pBus);
  if (!pUdev) 
  {
done2:
    ms_ehci_mem_cleanup (ehci);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return -ENOMEM;
  }

  ehci->hcd.state = USB_STATE_RUNNING;
 // ehci_writel (FLAG_CF, &ehci->regs->configured_flag);//marked by yuwen
  u32Temp = hcd_reg_readl ((U32)&ehci->op_regs->usbcmd);  /* unblock posted write */
  
  u8Temp = 0;
  
  u32Temp = hcd_reg_readw ((U32)&ehci->cap_regs->hciversion);
  ms_debug_msg ("EHCI %x.%02x, driver %s\n",
    (unsigned int)u32Temp >> 8, (unsigned int)u32Temp & 0xff, DRIVER_VERSION);   
 
  pUdev->eSpeed = USB_HIGH_SPEED;
  if (hcd_register_root (hcd) != 0)
  {
    if (hcd->state == USB_STATE_RUNNING)
      ms_host_ready (ehci);
    ms_host_reset (ehci);
    pBus->root_hub = 0;
    ms_usb_put_dev (pUdev);
    iRetval = -ENODEV;
    goto done2;
  } 

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}
 
 /* 
    * @brief             stop ehci controller
    *
    * @param          struct usb_hcd *hcd    
    *
    * @return           none
    */ 

 void ms_ehci_end (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);

  ms_debug_func("[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  
  if (hcd->state == USB_STATE_RUNNING)
    ms_host_ready (ehci);
  
  ms_del_timer_sync (&ehci->stWatchdog);
  ms_host_reset (ehci);
  
//  ehci_writel (0, &ehci->regs->configured_flag);       //marked by Yuwen
  osapi_spin_lock_irq (&ehci->tHcdLock);
  ms_ehci_active (ehci, NULL);
  osapi_spin_unlock_irq (&ehci->tHcdLock);
  ms_ehci_mem_cleanup (ehci);

#ifdef  EHCI_STATS
  ms_debug_msg ("[EHCI DEBUG] irq normal %ld err %ld reclaim %ld (lost %ld) ",
    ehci->stats.u32Normal, ehci->stats.u32Error, ehci->stats.u32Reclaim,
    ehci->stats.u32LostIAA);
  ms_debug_msg ("complete %ld unlink %ld\n",
    ehci->stats.u32Complete, ehci->stats.u32Unlink);
#endif

}

 int ms_ehci_get_frame_idx (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);  
  ms_debug_func("[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return (hcd_reg_readl ((U32)&ehci->op_regs->frindex) >> 3) % ehci->u32PeriodicSize;
}

/*-------------------------------------------------------------------------*/

 int ms_ehci_suspend (struct usb_hcd *hcd, U32 state)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  int      iPorts;
  int      i;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  iPorts = HCS_N_PORTS (ehci->hcs_params);

  if (hcd->state == USB_STATE_RUNNING)
    ms_host_ready (ehci);  
  ms_debug_msg("ms_ehci_suspend\n");
  hcd_reg_writel(hcd_reg_readl ((U32)&ehci->op_regs->usbcmd) & ~USBCMD_RUN, (U32)&ehci->op_regs->usbcmd);
  while((hcd_reg_readl((U32)&ehci->op_regs->usbsts)&USBSTS_HALT) == 0);

  /* suspend each port, then stop the hc */
  for (i = 0; i < iPorts; i++) {
    int  temp = hcd_reg_readl ((U32)&ehci->op_regs->portsc [i]);
    if ((temp & PORTSC_PE) == 0
        /* || (temp & PORTSC_OWNER) != 0 */)
      continue;
    ms_debug_msg ("suspend port %d", i);
    temp |= PORTSC_SUSPEND;
    hcd_reg_writel (temp, (U32)&ehci->op_regs->portsc [i]);
  }
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

 int ms_ehci_resume (struct usb_hcd *hcd)
{
  struct ehci_hcd    *ehci = hcd_to_ehci (hcd);
  int  iPorts;
  int  i;
  U32  U32Tmp;

  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);

  iPorts = HCS_N_PORTS (ehci->hcs_params);
  hcd->state = USB_STATE_RUNNING;  
  for (i = 0; i < iPorts; i++) 
  {
    int  temp = hcd_reg_readl ((U32)&ehci->op_regs->portsc [i]);
    if ((temp & PORTSC_PE) == 0
        || (temp & PORTSC_SUSPEND) != 0)
      continue;
    ms_debug_msg ("resume port %d", i);
    temp |= PORTSC_SUSPEND;
    hcd_reg_writel (temp, (U32)&ehci->op_regs->portsc [i]);
    U32Tmp = hcd_reg_readl ((U32)&ehci->op_regs->usbcmd);  /* unblock posted writes */
    wait_ms (20);
    temp &= ~PORTSC_SUSPEND;
    hcd_reg_writel (temp, (U32)&ehci->op_regs->portsc [i]);
  }
  U32Tmp = hcd_reg_readl ((U32)&ehci->op_regs->usbcmd);  /* unblock posted writes */
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/
static void ms_ehci_active (struct ehci_hcd *pEhci, struct stPtRegs *regs)
{
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  ms_timer_action_done (pEhci, TIMER_IO_WATCHDOG);  
  Chip_Read_Memory(); // 20121218, 20121225

  // This sequence will release Host task befor USB DSR complete.
  if (pEhci->iReclaimReady)
  {
    ms_end_unlink_async (pEhci, regs);
    ms_search_async (pEhci, regs);
    //ms_end_unlink_async (pEhci, regs); // move 2 lines ahead for IAA issue. 20140408
  }

  if (pEhci->iNextUframe != -1)
    ms_scan_periodic (pEhci, regs);
  
  if ((pEhci->stAsync->qh_next.ptr != 0) || (pEhci->u32PeriodicSched != 0))
    ms_timer_action (pEhci, TIMER_IO_WATCHDOG);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/* 
  * @brief             process ehci controller interrupt events
  *
  * @param          struct usb_hcd *hcd
  * @param          struct stPtRegs *regs  
  *
  * @return           none
  */ 

void ms_ehci_irq (struct usb_hcd *pHcd, struct stPtRegs *pRegs)
{
  struct ehci_hcd    *pEhci= hcd_to_ehci (pHcd);
  U32  u32Status;  
  int  bh;
  U32 u32Cmd;

  osapi_spin_lock (&pEhci->tHcdLock);

  u32Status = hcd_reg_readl ((U32)&pEhci->op_regs->usbsts);
  
  if (u32Status == ~(U32) 0) 
  {
    ms_debug_msg ("reg usbsts is 0xffff, device removed\n");
    goto dead;
  } 
  
  u32Status &= INTR_MASK;
  if (!u32Status)      /* No any our interrupt event */
  {
    diag_printf("%s: No any our interrupt status:0x%x mask:0x%x \n", __FUNCTION__, (unsigned int)u32Status, INTR_MASK);    
    diag_printf("ms_ehci_irq status: %x, intr_enable: %x\n", (unsigned int)u32Status, hcd_reg_readw((U32)&pEhci->op_regs->usbintr));
    goto done;
  }
  
  hcd_reg_writel (u32Status, (U32)&pEhci->op_regs->usbsts);
  u32Cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);  
  bh = 0;
  //pEhci->uDontSendIAA = 0;

  /* Notify controller for advance async schedule */
  /* USBSTS_INT for normal completion, see 4.15.1.2 
         USBSTS_ERR for error completion, see 4.15.1.1 */
  if ((u32Status & (USBSTS_INT|USBSTS_ERR)) != 0)
  {
    // Enable the interrupt for Async Advance Enable
    pEhci->iReclaimReady = 0;
    int    cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    cmd |= USBCMD_IAAD;
    hcd_reg_writel (cmd, (U32)&pEhci->op_regs->usbcmd);
    (void) hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd);
    //pEhci->uDontSendIAA = 1;

    if ((u32Status & USBSTS_ERR) == 0)
    {
      INCREASE(pEhci->stats.u32Normal);
    }
    else
    {
      INCREASE(pEhci->stats.u32Error);
      //bh = 1;	//Note, remove for command stall
    }
  }

  /* IAA is used for complete the unlinking of qh */
  if (u32Status & USBSTS_IAA) 
  {
    INCREASE(pEhci->stats.u32Reclaim);
    pEhci->iReclaimReady = 1;
    //pEhci->uDontSendIAA = 1;
    bh = 1;
  }

  /* See [4.3.1] */
  if (u32Status & USBSTS_PCD) 
  {      
      // Only support one port
      int iPsc, masked_psc;       
      int isHalt = 0;

      // root hub not run   
      if ((u32Cmd & USBCMD_RUN) == 0)
      {
          //u32Cmd |= USBCMD_RUN;
          //hcd_reg_writel(u32Cmd, (U32)&pEhci->op_regs->usbcmd);
          //while(hcd_reg_readw ((U32)&pEhci->op_regs->usbsts) & USBSTS_HALT);
          isHalt = 1;
          //hcd_reg_writel (u32Status, (U32)&pEhci->op_regs->usbsts);
          //diag_printf("clear USBSTS_PCD (0x%x) again\n", hcd_reg_readw ((U32)&pEhci->op_regs->usbsts));
      }

      iPsc = hcd_reg_readl((U32)&pEhci->op_regs->portsc [0]);      
      ms_debug_func("<ms_ehci_irq> portsc[0x%x], done[0] = 0x%x\n", iPsc, (unsigned int)pEhci->u32ResetEnd[0]);                    
      masked_psc = iPsc & 0xf;
      if ((masked_psc == 0xa /*|| masked_psc == 0xf */) && pEhci->u32ResetEnd[0] == 0)
      {
          if (isHalt)
            ms_ehci_softrst(pEhci);
          
          diag_printf("PCD not clear, reset UHC to get new CURRENT usbsts = 0x%x\n", hcd_reg_readl ((U32)&pEhci->op_regs->usbsts));
      }
      if ((((iPsc & PORTSC_RESUME) ||
           !(iPsc & PORTSC_SUSPEND)) &&
            (iPsc & PORTSC_PE) &&
             pEhci->u32ResetEnd[0] == 0))
      {
          /* The root hub port active 20 msec resume signal,
                    * PORTSC_SUSPEND to stop resume signal.
                    */          
          pEhci->u32ResetEnd [0] = jiffies + ((20 /* msec */ * HZ) / 1000);
          ms_debug_msg ("port 1 remote wakeup\n");
          ms_update_timer(&pHcd->roothub_timer, pEhci->u32ResetEnd[0], 0);
      }
  }


  /* See [4.15.2.4] */ 

  if ((u32Status & USBSTS_FATAL) != 0)
  {
     ms_debug_err ("fatal error\n");
dead:
     //ms_host_reset (pEhci); 
     ms_ehci_softrst(pEhci);
     /* uses ms_ehci_end to clean up the rest */        
     bh = 1;
  }

  if (bh || 
    (pEhci->u32PeriodicSched != 0) //If we has periodic transactions in Schedule, we must scan periodic when USBSTS_INT
    )  
    ms_ehci_active (pEhci, pRegs);
done:
  osapi_spin_unlock (&pEhci->tHcdLock);
  if (u32Status & USBSTS_PCD)        
      ms_hcd_poll_rh_status(pHcd);
  
}

/*-------------------------------------------------------------------------*/
/* 
  * @brief             transforms urb into qh+qtd list and submit to HCD hardware
  *
  * @param          struct usb_hcd  *hcd
  * @param          struct urb  *urb
  * @param          int  mem_flags
  *
  * @return           error code
  */ 
 int ms_urb_enqueue (
  struct usb_hcd  *pHcd,
  struct urb  *pUrb,
  int    iMem_flags
) 
{
  struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  struct list_head  qtd_list; 
  int iRetVal;
  
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  if( (hcd_reg_readl ((U32)&pEhci->op_regs->portsc[0])&PORTSC_CONNECT) == 0 )
  {    
    iRetVal = -ENODEV;
    goto urb_enqueue_fail;
  }    
  ms_list_init (&qtd_list);

  switch (usb_pipetype (pUrb->u32Pipe)) 
  {
    case EP_CONTROL:
    case EP_BULK:      
        if (!ms_qh_urb_process (pEhci, pUrb, &qtd_list, iMem_flags))
        {
            iRetVal = -ENOMEM;
            goto urb_enqueue_fail;
        }    
        iRetVal = ms_submit_async (pEhci, pUrb, &qtd_list);    
        break;

    case EP_INTERRUPT:
        if (!ms_qh_urb_process (pEhci, pUrb, &qtd_list, iMem_flags))
        {
            iRetVal = -ENOMEM;
            goto urb_enqueue_fail;
        }    
        iRetVal = ms_intr_submit (pEhci, pUrb, &qtd_list);
        break;

    default:    
        ms_debug_msg ("no support the transaction type!!\n");
        iRetVal = -ENOSYS;
  }
  
urb_enqueue_fail:    
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  return iRetVal;  
}

//extern void ms_ResetMstarUsb(struct ehci_hcd *ehci);
void ms_usb_hw_reset(struct usb_hcd* pUsbHcd)
{
    struct ehci_hcd *ehci = hcd_to_ehci(pUsbHcd);
    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    //ms_ResetMstarUsb(ehci);
    ms_ehci_softrst(ehci);
    ms_ehci_stoprun_setting(HOST20_Enable, ehci);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

/* 
  * @brief             remove qh from HW content
  *                       will happen a  completion event 
  * @param          struct usb_hcd *hcd
  * @param          struct urb *pUrb 
  *
  * @return           error code
  */
 int ms_urb_dequeue (struct usb_hcd *pHcd, struct urb *pUrb)
{
  struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  struct ehci_qh    *pQh = (struct ehci_qh *) pUrb->hcpriv;
  U32    u32Flags;

  if (!pQh)
      return 0; 
  
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);  
  
  switch (usb_pipetype (pUrb->u32Pipe)) 
  { 
  default:
    /* if we need to use IAA and it's busy, defer */
    if (pQh->qh_status == QH_STS_LINKED
        && pEhci->stReclaim
        && HCD_IS_RUNNING (pEhci->hcd.state)
        ) 
    {
      struct ehci_qh    *pLast;

      for (pLast = pEhci->stReclaim;
          pLast->pReclaimQh;
          pLast = pLast->pReclaimQh)
        continue;
      pQh->qh_status = QH_STS_UNLINK_WAIT;
      pLast->pReclaimQh = pQh;
      /* bypass IAA if the hc can't care */
    } 
    else if (!HCD_IS_RUNNING (pEhci->hcd.state) && pEhci->stReclaim)
      ms_end_unlink_async (pEhci, NULL);	

    /* something else might have unlinked the qh by now */
    if (pQh->qh_status == QH_STS_LINKED)
      ms_begin_unlink_async (pEhci, pQh);        
    //FIXME: cover FOTG200 bug
    if( (hcd_reg_readl ((U32)&pEhci->op_regs->portsc[0])&PORTSC_CONNECT) == 0 )
    {      
      diag_printf("[urb_dequeue] connect bit = 0 \n");
      //Finish ullinking procedure
      ms_end_unlink_async (pEhci, NULL);
      //Reset HC to ensure FOTG200 work correctly at the next time
      ms_hub_port_disable(pEhci->hcd.self.root_hub,0);
    }
    else if ( (hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd)&USBCMD_RUN) == 0 )
    {
      diag_printf("Something wrong (script auto running?) ==> Check it out\n");      	  
      //Finish ullinking procedure
      ms_end_unlink_async (pEhci, NULL);
      //Reset HC to ensure FOTG200 work correctly at the next time
      ms_hub_port_disable(pEhci->hcd.self.root_hub,0);
    }    
    break;
  case EP_INTERRUPT:    
    if (pQh->qh_status == QH_STS_LINKED) 
    {
      /* messy, can spin or block a microframe ... */
      ms_intr_deschedule (pEhci, pQh, 1);
      /* qh_state == IDLE */
    }
    ms_qh_finish (pEhci, pQh, NULL);

    /* reschedule QH iff another request is queued */	
    if (!ms_is_empty_list (&pQh->qtd_list)
        && HCD_IS_RUNNING (pEhci->hcd.state)) 
    {
      int iStatus;

      iStatus = ms_qh_schedule (pEhci, pQh);
      osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
      if (iStatus != 0) 
      {
        // Warning happened
        ms_debug_warn ("can't reschedule pQh %p, err %d", pQh, iStatus);
      }
      ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
      return iStatus;
    }
    break; 
    
  }

  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags); 
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

/*-------------------------------------------------------------------------*/

/* 
  * @brief             wait qh unlink complete then free qtd list.
  *
  * @param          struct usb_hcd *hcd
  * @param          struct s_hcd_dev *dev
  * @param          int ep
  *
  * @return           none
  */
  
void
ms_ehci_disable_ep (struct usb_hcd *pHcd, struct s_hcd_dev *pDev, int iEp)
{
  struct ehci_hcd    *pEhci = hcd_to_ehci (pHcd);
  int    iEpnum;
  U32    u32Flags;
  struct ehci_qh    *pQh;
  int numRescan = 7;
 
  ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  iEpnum = iEp & USB_ENDPOINT_NUMBER_MASK;
  if (iEpnum != 0 && (iEp & USB_DIR_IN))
    iEpnum |= 0x10; 

rescan:
  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  pQh = (struct ehci_qh *) pDev->ep [iEpnum];
  if (!pQh)
    goto done;

  if (!HCD_IS_RUNNING (pEhci->hcd.state))
    pQh->qh_status = QH_STS_IDLE;  
  switch (pQh->qh_status) 
  {
  case QH_STS_UNLINK:    
    osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
    osapi_schedule_timeout (1);
    diag_printf("[ms_ehci_disable_ep] goto rescan\n");
    if (--numRescan)
      goto rescan;
  case QH_STS_IDLE:    
    if (ms_is_empty_list (&pQh->qtd_list)) 
    {
      ms_qh_put (pEhci, pQh);
      break;
    }    
  default:    
    if ( !ms_is_empty_list (&pQh->qtd_list))
    {
        ms_debug_err ("pQh %p (#%d) state %d%s\n",
        pQh, iEpnum, pQh->qh_status,
        ms_is_empty_list (&pQh->qtd_list) ? "" : "(has tds)");
    }
    break;
  }
  pDev->ep [iEpnum] = 0;
done:
  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return;
}
#endif

#include <MsCommon.h>
#include "include/drvKernel.h"
#include "include/drvCPE_AMBA.h"
#include "include/drvCPE_EHCI.h"
#include "drvUsbd.h"

#if 1
extern MS_S32 ms_usb_disabled(void);
/*-------------------------------------------------------------------------*/

extern void ms_release_OS_Resource_EX(S32 *USBWaitFlg);
void ms_usb_hcd_cpe_ehci_remove (struct usb_hcd *pHcd)
{
    struct usb_device  *pHub;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    ms_debug_msg ("remove: %s, state %x\n", pHcd->self.bus_name, pHcd->state);
    pHub = pHcd->self.root_hub;
    if (HCD_IS_RUNNING (pHcd->state))
    pHcd->state = USB_STATE_QUIESCING;

    ms_debug_msg ("%s: roothub graceful disconnect\n", pHcd->self.bus_name);
    ms_usb_disconnect (&pHub);

    ms_ehci_end(pHcd);
    pHcd->state = USB_STATE_HALT;

    //free_irq (pHcd->irq, pHcd);
    ms_hcd_buffer_destroy (pHcd);
    ms_usb_deregister_bus (&pHcd->self);
    kfree (hcd_to_ehci (pHcd));
    ms_release_OS_Resource_EX(&pHcd->USBWaitFlg);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

#ifdef CONFIG_PM
int ms_usb_hcd_cpe_ehci_suspend (struct usb_hcd *hcd)
{
    int retval = 0;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    switch (hcd->state) 
    {
        case USB_STATE_HALT:
            ms_debug_msg ("halted; hcd not suspended\n");
            break;
        case USB_STATE_SUSPENDED:
            ms_debug_msg ("hcd already suspended\n");
            break;
        default:
            hcd->state = USB_STATE_QUIESCING;
            retval = ms_ehci_suspend (hcd, 0);
            if (retval)
                ms_debug_msg ("suspend fail, retval %d\n", retval);
            break;
    }
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return retval;    
}
int ms_usb_hcd_cpe_ehci_resume(struct usb_hcd *hcd)
{
    int retval=0;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if (hcd->state != USB_STATE_SUSPENDED) 
    {
        ms_debug_msg ("can't resume, not suspended!\n");
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
        return -46;//return -EL3HLT;
    }
    hcd->state = USB_STATE_RESUMING;
    /* remote wakeup needs hub->suspend() cooperation */
    retval = ms_ehci_resume (hcd);
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return retval;
}

#endif /* CONFIG_PM */

/*-------------------------------------------------------------------------*/

#endif

// -------------------------------------------------------------------------

void ms_ehci_stoprun_setting(MS_U8 bOption, struct ehci_hcd *ehci)
{
    U32 temp;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if (bOption==HOST20_Enable)
    {
        temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
        if (temp & USBCMD_RUN)
        {
            ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
            return;
        }
        temp |= USBCMD_RUN;
        hcd_reg_writel(temp, (U32)&ehci->op_regs->usbcmd);
        do 
        {
            temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
        } while ((temp & USBCMD_RUN) == 0);
    }
    else if (bOption==HOST20_Disable)
    {        
        temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
        if ((temp & USBCMD_RUN) == 0)
            return;

      temp &= ~USBCMD_RUN;
      hcd_reg_writel(temp, (U32)&ehci->op_regs->usbcmd);
      do {
          temp = hcd_reg_readl((U32)&ehci->op_regs->usbcmd);
      } while ((temp & USBCMD_RUN) > 0);
    }
    else
    {
      ms_debug_msg("??? Input Error 'ms_ehci_stoprun_setting'...");
      while(1);
    }
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
}

inline static void ms_rh_timer_func (unsigned long pHcd)
{
	ms_hcd_poll_rh_status((struct usb_hcd *) pHcd);
}

/* 
  * @brief             allocate struct usb_hcd and initial it
  *
  * @param          struct usb_hcd **hcd_out
  * @param          struct cpe_dev *dev 
  *
  * @return           error code
  */
int ms_new_usb_hcd(struct usb_hcd **pHcd_out, struct cpe_dev *pDev)
{
    int iRetVal;
    struct usb_hcd *pHcd = 0;
    struct ehci_hcd *pEhci;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);    
    pEhci = (struct ehci_hcd *)kmalloc (sizeof (struct ehci_hcd), GFP_KERNEL);  
    if (pEhci == NULL)
    {
        ms_debug_err ("ehci_hcd allocate failed\n");
        iRetVal = -ENOMEM;
        goto ex_err1;
    }    
    
    ms_debug_func("pEhci: %lx\n", (U32) pEhci);
    memset (pEhci, 0, sizeof (struct ehci_hcd));
    //pEhci->pHcd.product_desc = "EHCI Host Controller";
    strncpy(pEhci->hcd.product_desc, "EHCI Host Controller", sizeof(pEhci->hcd.product_desc));
    pHcd = (struct usb_hcd *)(&pEhci->hcd);        
    ms_debug_func("pHcd: %lx\n", (U32)pHcd);    

    pHcd->host_id = pDev->devid; // chcek the definition 
    pHcd->phub_event = pDev->pHubEvent;
    pHcd->ehci_irq = pDev->intNum;
    pHcd->uhc_regs = (U32*) pDev->uhcbase;
    pHcd->self.controller = &pDev->dev;
    pHcd->controller = pHcd->self.controller;
    pHcd->hcd_flags = HCD_HS; 

    iRetVal = ms_hcd_buffer_create (pHcd);
    if (iRetVal != 0) 
    {
        ms_debug_err ("pool alloc fail\n");
        goto ex_err1;
    }
    
    if ((iRetVal = ms_host_init(pHcd)) < 0) 
    {
        ms_debug_func ("can't reset\n");
        goto ex_err2;
    }

    ms_debug_msg ("ehci_hcd (CPE_AMBA) at 0x%p, irq %d\n",
    pHcd->uhc_regs, pHcd->ehci_irq);
    ms_usb_bus_init (&pHcd->self);
    pHcd->self.bus_ops = &ms_usb_hcd_operations;
    pHcd->self.hcpriv = (void *) pHcd;
        
    pHcd->self.bus_name = pDev->bus_name;
    //pHcd->product_desc = pDev->product_desc;
    memcpy(pHcd->product_desc, pDev->product_desc, sizeof(pHcd->product_desc));

    ms_init_timer(&pHcd->roothub_timer);
	pHcd->roothub_timer.function = ms_rh_timer_func;
	pHcd->roothub_timer.data = (unsigned long) pHcd;

    pHcd->ms_urb_enqueue = ms_urb_enqueue;
    pHcd->ms_urb_dequeue = ms_urb_dequeue;
    
    pHcd->pre_sts = 0; // new
    pHcd->pre_temp = 0; // new
    ms_init_OS_Resource_EX(&pHcd->USBWaitFlg);
    ms_list_init (&pHcd->hcd_dev_list);
    ms_usb_register_bus (&pHcd->self);

    if ((iRetVal = ms_ehci_begin(pHcd)) < 0) // start HCD
    {
        ms_usb_hcd_cpe_ehci_remove(pHcd);
        return iRetVal;
    }

    *pHcd_out = pHcd;

    return iRetVal;

ex_err2:
    ms_hcd_buffer_destroy (pHcd);
    if (pHcd)
        kfree (hcd_to_ehci (pHcd));
ex_err1:
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return iRetVal;
}

extern void ms_InitUSBIntr_EX(struct usb_hcd * hcd);
int ms_create_cpe_hcd(struct cpe_dev *dev)
{
    struct usb_hcd *hcd = NULL;
    int ret;

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    if (ms_usb_disabled())
    {
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
        return -ENODEV;
    }
    ms_device_initialize(&dev->dev);    
    ret = ms_new_usb_hcd(&hcd, dev);

    if (ret == 0)
    {
        ms_debug_msg("ms_ehci_hcd_cpe_ehci_drv_probe_EX -> allocate usb_hcd\n");    
        dev->dev.driver_data = hcd;
        ms_InitUSBIntr_EX(hcd);
    }
    ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return ret;
}

void ms_host_shutdown (struct usb_hcd *pHcd)
{
    struct ehci_hcd  *pEhci = hcd_to_ehci (pHcd);

    /* halt hcd */
    ms_host_halt(pEhci);

    /* reset to clear interrupt enable */
    ms_host_reset(pEhci);

    return;
}

#if USB_IF_EHSET_SUPPORT // Embedded host electrical test procedure
struct list_head *
ms_qh_urb_transaction_EHSET (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  struct list_head  *head,
  int      iFlags,
  int      iStage
) 
{
    struct ehci_qtd    *pQtd, *pQtdPrev;
    dma_addr_t    tBufAddr;
    int      iLen, iMaxpkt;
    int      iIsInput;
    U32      u32Token;	

    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    pQtd = ms_ehci_qtd_alloc (ehci, iFlags);
    if (!pQtd)
    {
        ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
        return 0;
    }
    ms_insert_list_before (&pQtd->qtd_list, head);
    pQtd->urb = urb;

    u32Token = QTD_STS_ACT;
    u32Token |= (MAX_CERR_CNT << 10);

    iLen = urb->u32TransferBufferLength;
    iIsInput = usb_pipein (urb->u32Pipe);
    if (!iStage)
    {        
        if (usb_pipecontrol (urb->u32Pipe)) 
        {
          ms_qtd_pack (pQtd, urb->tSetupDma, sizeof (struct usb_ctrlrequest),
          u32Token | (2 /* "setup" */ << 8), 8);
          pQtd->hw_alt_next_qtd = EHCI_LIST_END; // EHSET
          u32Token ^= QTD_TOGGLE;
        }
        
        if (!(urb->u32TransferFlags & MS_FLAG_URB_NO_INTERRUPT))
          pQtd->hw_token |= QTD_IOC; // EHSET
        return head;
    }
    else
        u32Token ^= QTD_TOGGLE;  

  tBufAddr = (iLen > 0) ? urb->tTransferDma : 0;    

  if (!tBufAddr || iIsInput)
    u32Token |= (1 /* "in" */ << 8);
  if (usb_pipebulk(urb->u32Pipe)) 
  {
    switch (urb->dev->eSpeed) 
    {
      case USB_FULL_SPEED:
        iMaxpkt = max_packet(usb_maxpacket(urb->dev, urb->u32Pipe, !iIsInput));
        break;
      case USB_HIGH_SPEED:
      default:
        iMaxpkt = 512;
        break;
    }
  }
  else 
  {
    iMaxpkt = max_packet(usb_maxpacket(urb->dev, urb->u32Pipe, !iIsInput));
  }  

  for (;;) 
  {
    int iThisQtdLen;

    iThisQtdLen = ms_qtd_pack (pQtd, tBufAddr, iLen, u32Token, iMaxpkt);
    iLen -= iThisQtdLen;
    tBufAddr += iThisQtdLen;
    if (iIsInput)
      pQtd->hw_alt_next_qtd = ehci->stAsync->hw_alt_next_qtd;	

    if ((iMaxpkt & (iThisQtdLen + (iMaxpkt - 1))) == 0)
      u32Token ^= QTD_TOGGLE;

    if (iLen <= 0)
      break;	

    pQtdPrev = pQtd;
    pQtd = ms_ehci_qtd_alloc (ehci, iFlags);
    if (!pQtd)
      goto cleanup;
    pQtd->urb = urb;
    pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
    ms_insert_list_before (&pQtd->qtd_list, head);
  }  

  if ((urb->u32TransferFlags & MS_FLAG_URB_SHORT_NOT_OK) == 0
        || usb_pipecontrol (urb->u32Pipe))
    pQtd->hw_alt_next_qtd = EHCI_LIST_END;

  if (tBufAddr != 0)
  {
    int  iOneMore = 0;

    if (usb_pipecontrol (urb->u32Pipe)) 
    {
      iOneMore = 1;
      u32Token ^= 0x0100;  /* "in" <--> "out"  */
      u32Token |= QTD_TOGGLE;  /* force DATA1 */
    } 
    else if (usb_pipebulk (urb->u32Pipe)
        && (urb->u32TransferFlags & MS_FLAG_URB_ZERO_PACKET)
        && !(urb->u32TransferBufferLength % iMaxpkt)) {
      iOneMore = 1;
    }
    if (iOneMore) 
    {
      pQtdPrev = pQtd;
      pQtd = ms_ehci_qtd_alloc (ehci, iFlags);
      if (!pQtd)
        goto cleanup;
      pQtd->urb = urb;
      pQtdPrev->hw_next_qtd = (U32)(pQtd->qtd_dma_addr);
      ms_insert_list_before (&pQtd->qtd_list, head);

      ms_qtd_pack (pQtd, 0, 0, u32Token, 0);
    }
  }  

  if (!(urb->u32TransferFlags & MS_FLAG_URB_NO_INTERRUPT))
    pQtd->hw_token |= QTD_IOC;

  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return head;

cleanup:

  ms_qtd_register_free (ehci, head);
  ms_debug_func("--[%s] line:[%d]\n", __FUNCTION__, __LINE__);
  return 0;
}

inline int ms_submit_async_EHSET (
  struct ehci_hcd    *ehci,
  struct urb    *urb,
  struct list_head  *qtd_list,
  int      mem_flags
) {
    ms_debug_func("++[%s] line:[%d]\n", __FUNCTION__, __LINE__);
    return ms_submit_async(ehci, urb, qtd_list);
}
#endif
