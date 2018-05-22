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
int ms_ehci_get_frame_idx (struct usb_hcd *hcd);

/*-------------------------------------------------------------------------*/
static int ms_periodic_unlink (struct ehci_hcd *pEhci, U32 u32Frame, void *ptr)
{
  union ehci_qh_shadow  *prev_p = &pEhci->pshadow [u32Frame];
  U32   *hw_p = &pEhci->pPeriodic [u32Frame];
  union ehci_qh_shadow  here = *prev_p;
  union ehci_qh_shadow  *next_p1;  
  
  while (here.ptr && here.ptr != ptr) 
  { 
    prev_p = (Q_NEXT_TYPE (*hw_p) == QH_TYPE)         
        ? (union ehci_qh_shadow *)(&prev_p->qh->qh_next)
        : NULL;    
    hw_p = &here.qh->hw_next_qh;    
    here = *prev_p;
  }  
  if (!here.ptr) {
    ms_debug_msg ("[periodic_unlink] entry %p no longer on u32Frame [%d]", ptr, (int)u32Frame);
    return 0;
  } 

  next_p1 = (Q_NEXT_TYPE (*hw_p) == QH_TYPE) 
        ? (union ehci_qh_shadow *)(&here.qh->qh_next)
        : NULL; 
  *hw_p = here.qh->hw_next_qh;  
  *prev_p = *next_p1;
  next_p1->ptr = 0;

  return 1;
}

static U16
ms_periodic_usecs (struct ehci_hcd *pEhci, U32 u32Frame, U32 u32uFrame)
{
  U32      *phw_p = &pEhci->pPeriodic [u32Frame];
  union ehci_qh_shadow  *pQh = &pEhci->pshadow [u32Frame];
  U32    u32uSecs = 0;

  while (pQh->ptr) 
  {
    if (Q_NEXT_TYPE (*phw_p) == QH_TYPE) 
	{                
      if (pQh->qh->hw_ep_state2 & (1 << u32uFrame))
        u32uSecs += pQh->qh->u8Usecs;      
      if (pQh->qh->hw_ep_state2 & (1 << (8 + u32uFrame)))
        u32uSecs += pQh->qh->c_usecs;
      pQh = &pQh->qh->qh_next;
    }
	else
      USB_ASSERT (0, "Next list type not qH type!\n");    
  }
#ifdef  DEBUG
  if (u32uSecs > 100)
    ms_debug_err("overallocated u32uFrame %d, periodic is %d u32uSecs",
      u32Frame * 8 + u32uFrame, u32uSecs);
#endif
  return u32uSecs;
}

/*-------------------------------------------------------------------------*/
static int ms_enable_periodic (struct ehci_hcd *pEhci)
{
  U32  u32Cmd;
  int  iStatus;
 
  iStatus = ms_check_status (&pEhci->op_regs->usbsts, USBSTS_PSS, 0, 9 * 125);
  if (iStatus != 0) {
    pEhci->hcd.state = USB_STATE_HALT;
    diag_printf("enable periodic schedule time out!!!!\n");
    return iStatus;
  }

  u32Cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd) | USBCMD_PSE;
  hcd_reg_writel (u32Cmd, (U32)&pEhci->op_regs->usbcmd);  
  pEhci->hcd.state = USB_STATE_RUNNING;  
  pEhci->iNextUframe = hcd_reg_readl ((U32)&pEhci->op_regs->frindex)
        % (pEhci->u32PeriodicSize << 3);
  return 0;
}

static int ms_disable_periodic (struct ehci_hcd *pEhci)
{
  U32  u32Cmd;
  int  iStatus;
  
  iStatus = ms_check_status (&pEhci->op_regs->usbsts, USBSTS_PSS, USBSTS_PSS, 9 * 125);
  if (iStatus != 0) {  
    diag_printf("disable periodic schedule time out!!!!\n");
    return iStatus;
  }

  u32Cmd = hcd_reg_readl ((U32)&pEhci->op_regs->usbcmd) & ~USBCMD_PSE;
  hcd_reg_writel (u32Cmd, (U32)&pEhci->op_regs->usbcmd);
  pEhci->iNextUframe = -1;
  return 0;
}

/*-------------------------------------------------------------------------*/
static void ms_intr_deschedule (
  struct ehci_hcd  *pEhci,
  struct ehci_qh  *pQh1,
  int    iWait
) {
  int  iStatus;
  U32  u32Frame = pQh1->u16Start;

  do {
    ms_periodic_unlink (pEhci, u32Frame, pQh1);
    ms_qh_put (pEhci, pQh1);
    u32Frame += pQh1->u16Period;
  } while (u32Frame < pEhci->u32PeriodicSize);

  pQh1->qh_status = QH_STS_UNLINK;
  pQh1->qh_next.ptr = 0;
  
  pEhci->u32PeriodicSched--;
  if (!pEhci->u32PeriodicSched)
    iStatus = ms_disable_periodic (pEhci);
  else 
  {
    iStatus = 0;
    ms_debug_msg ("periodic schedule still enabled\n");
  }
  
  if (((ms_ehci_get_frame_idx (&pEhci->hcd) - u32Frame) % pQh1->u16Period) == 0) 
  {
    if (iWait) 
	{
      udelay (125);
      pQh1->hw_next_qh = EHCI_LIST_END;
    } 
	else
	{      
      ms_debug_msg ("ms_intr_deschedule...\n");
    }
  } else
    pQh1->hw_next_qh = EHCI_LIST_END;

  pQh1->qh_status = QH_STS_IDLE;  
  
  ms_debug_msg ("descheduled qh %p, period = %d u32Frame = %d count = %d, urbs = %d\n",
    pQh1, pQh1->u16Period, (int)u32Frame,
    osapi_atomic_read(&pQh1->tRefCnt), (int)pEhci->u32PeriodicSched);
}

static int ms_check_period (
  struct ehci_hcd *pEhci,
  U32  u32Frame,
  U32  u32uFrame,
  U32  u32Period,
  U32  u32Usecs
) {  
  if (u32uFrame >= 8)
    return 0;
  
  u32Usecs = 100 - u32Usecs; // high speed case
#if 0  
  if ( pEhci->hcd.eSpeed == USB_HIGH_SPEED )
    u32Usecs = 100 - u32Usecs;
  else
    u32Usecs = 900 - u32Usecs;
#endif  

  do {
    int  claimed;

    if (pEhci->pshadow [u32Frame].ptr)
      return 0;

    claimed = ms_periodic_usecs (pEhci, u32Frame, u32uFrame);
    if ((U32)claimed > u32Usecs)
      return 0;

  } while ((u32Frame += u32Period) < pEhci->u32PeriodicSize);

  // success!
  return 1;
}

static int ms_check_intr_schedule (
  struct ehci_hcd    *pEhci,
  U32    u32Frame,
  U32    u32uFrame,
  const struct ehci_qh  *pQh,
  U32      *c_maskp
)
{
      int    iRetVal = -ENOSPC;

  if (!ms_check_period (pEhci, u32Frame, u32uFrame, pQh->u16Period, pQh->u8Usecs))
    goto done;
  if (!pQh->c_usecs) {
    iRetVal = 0;
    *c_maskp = 0;
    goto done;
  }
 
  if (!ms_check_period (pEhci, u32Frame, u32uFrame + pQh->u8Gap_uf + 1,
        pQh->u16Period, pQh->c_usecs))
    goto done;
  if (!ms_check_period (pEhci, u32Frame, u32uFrame + pQh->u8Gap_uf,
        pQh->u16Period, pQh->c_usecs))
    goto done;

  *c_maskp = (0x03 << (8 + u32uFrame + pQh->u8Gap_uf));
  iRetVal = 0;
done:
  return iRetVal;
}

static int ms_qh_schedule (struct ehci_hcd *pEhci, struct ehci_qh *pQh)
{
  int  iStatus;
  U32  u32uFrame1;
  U32  u32_C_mask1;
  U32  u32Frame1;    

  pQh->hw_next_qh = EHCI_LIST_END;
  u32Frame1 = pQh->u16Start;
  
  if (u32Frame1 < pQh->u16Period) {
    u32uFrame1 = ms_find_1st_set (pQh->hw_ep_state2 & 0x00ff);
    iStatus = ms_check_intr_schedule (pEhci, u32Frame1, --u32uFrame1,
        pQh, &u32_C_mask1);
  } else {
    u32uFrame1 = 0;
    u32_C_mask1 = 0;
    iStatus = -ENOSPC;
  }
  
  if (iStatus) {
    u32Frame1 = pQh->u16Period - 1;
    do {
      for (u32uFrame1 = 0; u32uFrame1 < 8; u32uFrame1++) {
        iStatus = ms_check_intr_schedule (pEhci,
            u32Frame1, u32uFrame1, pQh,
            &u32_C_mask1);
        if (iStatus == 0)
          break;
      }
    } while (iStatus && u32Frame1--);
    if (iStatus)
      goto done;
    pQh->u16Start = u32Frame1;    
    pQh->hw_ep_state2 &= ~0xffff;
    pQh->hw_ep_state2 |= (1 << u32uFrame1) | u32_C_mask1;
  } else
    ms_debug_msg ("reused previous pQh %p schedule", pQh);

  pQh->qh_status = QH_STS_LINKED;
  ms_debug_msg ("[ms_qh_schedule] scheduled pQh %p usecs %d/%d period %d.0 starting %d.%d (gap %d)\n",
    pQh, pQh->u8Usecs, pQh->c_usecs,
    pQh->u16Period, (int)u32Frame1, (int)u32uFrame1, pQh->u8Gap_uf);
  do {
    if (pEhci->pshadow [u32Frame1].ptr != 0) {
      USB_ASSERT (0, "pEhci->pshadow [u32Frame1].ptr != 0\n");
    } else {
      pEhci->pshadow [u32Frame1].qh = ms_qh_get (pQh);
      pEhci->pPeriodic [u32Frame1] =
        QH_NEXT (pQh->qh_dma_addr);
    }
    wmb ();
    u32Frame1 += pQh->u16Period;
  } while (u32Frame1 < pEhci->u32PeriodicSize); 

  if (!pEhci->u32PeriodicSched++)
    iStatus = ms_enable_periodic (pEhci);
done:
  return iStatus;
}

static int ms_intr_submit (
  struct ehci_hcd    *pEhci,
  struct urb    *pUrb,
  struct list_head  *qtd_list  
) {
  U32    epnum1;
  U32    u32Flags;
  struct ehci_qh    *pQh1;
  struct s_hcd_dev    *dev;
  int      is_input;
  int      status = 0;
  struct list_head  stEmpty;
 
  epnum1 = usb_pipeendpoint (pUrb->u32Pipe);
  is_input = usb_pipein (pUrb->u32Pipe);
  if (is_input)
    epnum1 |= 0x10;

  osapi_spin_lock_irqsave (&pEhci->tHcdLock, u32Flags);
  dev = (struct s_hcd_dev *)pUrb->dev->hcpriv;
  
  ms_list_init (&stEmpty);
  pQh1 = ms_qh_append_tds (pEhci, pUrb, &stEmpty, epnum1, &dev->ep [epnum1]);  
  if (pQh1 == 0) {
    status = -ENOMEM;
    goto done;
  }
  if (pQh1->qh_status == QH_STS_IDLE) {
    if ((status = ms_qh_schedule (pEhci, pQh1)) != 0)
      goto done;
  }

  pQh1 = ms_qh_append_tds (pEhci, pUrb, qtd_list, epnum1, &dev->ep [epnum1]);   

  Chip_Flush_Memory();

done:	
  osapi_spin_unlock_irqrestore (&pEhci->tHcdLock, u32Flags);  
  if (status)
    ms_qtd_register_free (pEhci, qtd_list);

  return status;
}

static U32
ms_intr_complete (
  struct ehci_hcd  *pEhci, 
  struct ehci_qh  *pQh,
  struct stPtRegs  *pRegs
) {
  U32  u32Count;  
  if ((pQh->hw_token & QTD_STS_ACT)
      != 0)
    return 0;  
  
  if (ms_is_empty_list (&pQh->qtd_list)) {
    ms_debug_msg ("[intr qh] %p no TDs?", pQh);
    return 0;
  }
  
  u32Count = ms_qh_finish (pEhci, pQh, pRegs);  
  if (ms_is_empty_list (&pQh->qtd_list))
    ms_intr_deschedule (pEhci, pQh, 0); 
  return u32Count;
}

/*-------------------------------------------------------------------------*/

static void
ms_scan_periodic (struct ehci_hcd *pEhci, struct stPtRegs *pRegs)
{
  U32  u32Frame, u32Clock, u32Now_uframe, u32Mod;
  U32  u32Count = 0;

  u32Mod = pEhci->u32PeriodicSize << 3;  
  u32Frame = pEhci->iNextUframe >> 3;
  if (HCD_IS_RUNNING (pEhci->hcd.state))
    u32Now_uframe = hcd_reg_readl ((U32)&pEhci->op_regs->frindex);
  else
    u32Now_uframe = (u32Frame << 3) - 1;
  u32Now_uframe %= u32Mod;
  u32Clock = u32Now_uframe >> 3;

  for (;;) {
    union ehci_qh_shadow  q, *q_p;
    U32      u32Type, *hw_p;
    U32    u32uFrames;

restart:    
    if (u32Frame == u32Clock)
      u32uFrames = u32Now_uframe & 0x07;
    else
      u32uFrames = 8;
    
    q_p = &pEhci->pshadow [u32Frame];
    hw_p = &pEhci->pPeriodic [u32Frame];
    q.ptr = q_p->ptr;
    u32Type = Q_NEXT_TYPE (*hw_p);
    
    while (q.ptr != 0) {
      int      last;      
      union ehci_qh_shadow  temp;

      if (u32Type == QH_TYPE) 
	  {                 	
        last = (q.qh->hw_next_qh == EHCI_LIST_END);        
        temp = q.qh->qh_next;        
        u32Type = Q_NEXT_TYPE (q.qh->hw_next_qh);        
        u32Count += ms_intr_complete (pEhci, ms_qh_get (q.qh), pRegs);         
        ms_qh_put (pEhci, q.qh);
        q = temp;
      }
	  else
	  {            	
        ms_debug_msg ("corrupt u32Type %d u32Frame %d shadow %p",
          (int)u32Type, (int)u32Frame, q.ptr);
        // BUG ();
        last = 1;
        q.ptr = 0;
      }
      
      if (q.ptr == 0 && !last)
        goto restart;
    }
    
    if (u32Frame == u32Clock) 
	{
      U32  now; 
	  
      if (!HCD_IS_RUNNING (pEhci->hcd.state) || pEhci->u32PeriodicSched == 0)
        break;
      
      pEhci->iNextUframe = u32Now_uframe;      
      now = hcd_reg_readl ((U32)&pEhci->op_regs->frindex) % u32Mod;
      if (u32Now_uframe == now)
        break;
      
      u32Now_uframe = now;
      u32Clock = u32Now_uframe >> 3;
    } 
	else
      u32Frame = (u32Frame + 1) % pEhci->u32PeriodicSize;
  }
}

