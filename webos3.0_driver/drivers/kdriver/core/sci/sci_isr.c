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


/** @file
 *
 *  driver interrupt service rouotine implementation for sci device.
 *	sci device will teach you how to make device driver with new platform.
 *
 *  author		bongrae.cho (bongrae.cho@lge.com)
 *  version		1.0
 *  date		2012.02.23
 *  note		Additional information.
 *
 *  @addtogroup lg1154_sci
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/


    
/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/wait.h>
#include <linux/irq.h>
#include <linux/sched.h>

#include <linux/jiffies.h>

#include "sci_isr.h"
#include "sci_reg.h"

#include "sci_core.h"


/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
    
/*----------------------------------------------------------------------------------------
    Macro Definitions
----------------------------------------------------------------------------------------*/


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
	Global Functions Declarations
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static DECLARE_WAIT_QUEUE_HEAD(_wq_sci);


static volatile int _pSemCD = 0;
static volatile int _pSemRX = 0;
static volatile int _pSemEOR = 0;
static volatile int _pSemEOT = 0;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/




/*========================================================================================
	Implementation Group
========================================================================================*/

irqreturn_t SCI_ISRHandler (int irq, void *dev_id, struct pt_regs *regs)
{
	UINT8 err = 0;
	UINT8 cardStatus;
	UINT8 irqFlag = 0, intrMask;

    unsigned int i, remainder;   

    SCI_INFO_T *info = (SCI_INFO_T *)dev_id;

	irqFlag = SCI_RegGetInterruptStatus();
    intrMask = SCI_RegGetInterruptMask();

	SCI_ISR("SCI_IRQ STATUS : %02x, MASK : %02x, TRIG : %02x, STAT : %02x\n",irqFlag, intrMask, (irqFlag & ~intrMask), SCI_RegGetStatus());
    
	irqFlag &= ~intrMask;
	if(irqFlag & SCI_IRQ_CDPWR)
	{	     
	    cardStatus = SCI_RegGetCardStatus();

	    if(cardStatus == STATUS_READY)        // Card rejected
	    {
	    	SCI_Deactivate(info);
	    }
	    else                            // Card inserted
	    {
	        if(cardStatus == STATUS_DETECTED)
	        {

	        }
	        else if(cardStatus == STATUS_ACTIVE)
	        {

	        }
	        else if(cardStatus == STATUS_FAILED)
	        {


	        }
	        else    // It doesn't happend!! but for exception
	            SCI_ISR("Card Status Error %d\n", cardStatus);
	    }

        info->SciStatus = SCI_STATUS_IDLE;
        //SCI_RegDisableInterrupt(SCI_IRQ_EOT | SCI_IRQ_EOR | SCI_IRQ_FTE| SCI_IRQ_FTF | SCI_IRQ_FRNE | SCI_IRQ_FRF);       

		// 20141124 Change interruptible location for SMP synch
		_pSemCD = 1;
		wake_up_interruptible(&_wq_sci);
    
	}

	if(irqFlag & SCI_IRQ_ERR)
	{
		err = SCI_RegGetErrorStatus();
		
		SCI_ISR("SCI_IRQ_ERR : %02x, State : %02x\n", err, SCI_RegGetStatus());
	}

	if(irqFlag & SCI_IRQ_EOT)
	{
        info->txBlock.flagCompleated = 1;
        info->txBlock.idx = 0;

        info->NumberTransmit++;
        
        if(info->SciAutoRecv)
        	//Receive enable
        	SCI_RegEnableReceive();
        
        info->SciStatus = SCI_STATUS_IDLE;
		
		// 20141124 Change interruptible location for SMP synch
		_pSemEOT = 1;
		wake_up_interruptible(&_wq_sci);
	
	}

	if(irqFlag & SCI_IRQ_EOR)
	{
        info->rxBlock.nad = SCI_RegGetNAD();
        info->rxBlock.pcb = SCI_RegGetPCB();
        info->rxBlock.len = SCI_RegGetLength();
		
		while(info->rxBlock.idx < info->rxBlock.len)
            info->rxBlock.inf[info->rxBlock.idx++] = SCI_RegReadData();
					
        info->rxBlock.flagCompleated = 1;
        info->rxBlock.idx = 0;
		
        info->NumberReceive++;
        
        info->SciStatus = SCI_STATUS_IDLE;
		
		// 20141124 Change interruptible location for SMP synch
		_pSemEOR = 1;
		wake_up_interruptible(&_wq_sci);	
	}

	if(irqFlag & SCI_IRQ_FTE)
	{
		if(info->SciStatus == SCI_STATUS_TRANSMIT)
		{
            if(info->txBlock.len - info->txBlock.idx > info->TxFIFODepth)
                remainder = info->TxFIFODepth;
            else
                remainder = info->txBlock.len - info->txBlock.idx;
            
    		for(i = 0; i < remainder; i++)
                SCI_RegWriteData(info->txBlock.inf[info->txBlock.idx + i]);

            info->txBlock.idx += remainder;
		}
	}

	if(irqFlag & SCI_IRQ_FTF)
	{
	
	}

	if(irqFlag & SCI_IRQ_FRNE)
	{
        info->rxBlock.len = SCI_RegGetLength();
        info->rxBlock.idx = 0;

        info->SciStatus = SCI_STATUS_RECEIVE;
	}

	if(irqFlag & SCI_IRQ_FRF)
	{
        if(info->SciStatus == SCI_STATUS_RECEIVE)
        {
            if(info->rxBlock.len - info->rxBlock.idx > info->RxFIFODepth)
                remainder = info->RxFIFODepth;
            else
                remainder = info->rxBlock.len - info->rxBlock.idx;
            
    		for(i = 0; i < remainder; i++)
                info->rxBlock.inf[info->rxBlock.idx + i] = SCI_RegReadData();

            info->rxBlock.idx += remainder;
        }
		
		// 20141124 Change interruptible location for SMP synch
		_pSemRX = 1;
		wake_up_interruptible(&_wq_sci);
	
	}

	return IRQ_HANDLED;
}


SCI_RETURN_T SCI_ISRWaitIRQ (SCI_IRQ_T irq, UINT32 timeout)
{
	SINT32 ret;
    
	switch(irq)
	{
	case SCI_IRQ_EOR:
		_pSemEOR = 0;
		ret = wait_event_interruptible_timeout(_wq_sci, _pSemEOR != 0, msecs_to_jiffies(timeout) );			
		break;
        
	case SCI_IRQ_FRF:
		_pSemRX = 0;
		ret = wait_event_interruptible_timeout(_wq_sci, _pSemRX != 0, msecs_to_jiffies(timeout) );			
		break;
        
	case SCI_IRQ_EOT:
		_pSemEOT = 0;
		ret = wait_event_interruptible_timeout(_wq_sci, _pSemEOT != 0, msecs_to_jiffies(timeout) );			
		break;
                  
	case SCI_IRQ_FTE:
	case SCI_IRQ_FTF:
	case SCI_IRQ_FRNE:
	default:
		SCI_ERROR("irq[%x] is not supported", irq);
		return SCI_FAIL;
	}

	if(ret == 0)
		return SCI_IRQ_TIME_OUT;
	else
		return SCI_SUCCESS;
}



void SCI_ISRWaitCDIntr()
{
    _pSemCD = 0;
    wait_event_interruptible(_wq_sci, _pSemCD != 0);
}

