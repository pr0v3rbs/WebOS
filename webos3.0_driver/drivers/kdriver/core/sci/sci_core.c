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
 *  main driver core implementation for sci device.
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
#include "sci_core.h"
#include "sci_reg.h"
#include "sci_isr.h"

#include "sci_kapi.h"

#include "os_util.h"

#include <linux/uaccess.h>
#include <linux/delay.h>

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
//#define SCI_RETRY

/*----------------------------------------------------------------------------------------
*	function prototypes
*----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
    Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void SCI_CoreSetBaud(UINT32 FS, UINT32 DI, UINT32 operting_clock_khz);

static void SCI_CoreConfigInterface(SCI_INFO_T* pSCIinfo);
static SCI_RETURN_T SCI_CoreDecodeATR (LX_SCI_ATR_T *pATR, UINT8 dataLength);
static SCI_RETURN_T SCI_CoreReadATR(UINT8 *atrData);
static SCI_RETURN_T SCI_CoreRequestIFS(SCI_INFO_T* pSCIinfo);
static unsigned char SCI_CoreSendAndReceiveBlock(SCI_INFO_T* pSCIinfo);

#if 0 // jinhwan.bae 20140205 , compile warning
static SCI_ANALYZE_STATUS SCI_CoreAnalyzeRxBlock(SCI_INFO_T* pSCIinfo);
#endif

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Global variables
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Global Functions Definition
----------------------------------------------------------------------------------------*/

static void SCI_CoreSetBaud(UINT32 FS, UINT32 DI, UINT32 operting_clock_khz)
{
    unsigned int baud, br, ft;
	const unsigned int clk_div_table[8] = { 2, 4, 6, 8, 12, 16, 24, 32};

	SCI_PRINT("enter SCI_CoreSetBaud\n");

    baud = (SCI_INPUT_CLK * SCI_CRD_CLK_ADJ_MHZ_TO_KHZ * FS / DI / clk_div_table[SCI_RegGetClkDiv()] / operting_clock_khz);// baud;

    br = baud / 10;
    ft = baud % 10;

    SCI_RegSetBaud(br , ft);
}

static SCI_RETURN_T SCI_CoreDecodeATR (LX_SCI_ATR_T *pATR, UINT8 dataLength)
{
    int iErr = 0;
    int offset = 0, iCount = 0;
    int flags = 0, i= 0;

	SCI_PRINT("enter SCI_CoreDecodeATR\n");

    /* sanity check parameters */
    if((pATR == NULL) || (dataLength < 3))
    {
        iErr = -1;
        goto bad;
    }

    /* more than 33 bytes is not valid according to the spec ... so
    * we truncate what we look at in the name of sanity preservation
    */
    if(dataLength > SCI_MAX_ATR_DATA_BUFFER_LENGTH)
        dataLength = SCI_MAX_ATR_DATA_BUFFER_LENGTH;

    /* first byte is TS */
    pATR->TS = pATR->data[offset++];
    dataLength--;

    /* check the first byte - it can have only two values */
    if (pATR->TS == 0x3f)
        pATR->conversion = 0;
    else
        pATR->conversion = 1;

    if (dataLength > 0)
    {
        /* next byte is always T0 */
        pATR->T0 = pATR->data[offset++];
        dataLength--;

        /* Validate T0 */
        if ( pATR->T0 != 0xF0 )
        {
            iErr = -2;
            goto bad;
        }

        /* extract flags for T[A-D] and historical count */
        flags = pATR->T0;
        pATR->historical_count = pATR->T0 & 0x0f;

        /* now read in the interleaved additional bytes */
        while((dataLength > 0) && (flags & 0xf0))
        {   // grab the TA byte
            if(flags & 0x10)
            {
                pATR->TA[pATR->TA_count++] = pATR->data[offset++];

                if(iCount == 0) // TA1
                {
                    pATR->FI = pATR->TA[pATR->TA_count - 1]  >> 4 & 0x0f ;
                    pATR->FI = (pATR->FI == 3) ? 744 : (pATR->FI == 1) ? 372 : 0;

                    /* Validate FI */
                    if ( pATR->FI == 0 )
                    {
                        pATR->FI = 372;
                        iErr = -3;
                        goto bad;
                    }

                    pATR->DI = pATR->TA[pATR->TA_count - 1] & 0x0F;
                    pATR->DI = (pATR->DI == 3) ? 0x04 : (pATR->DI == 4) ? 0x08 : (pATR->DI == 2) ? 0x02 : 0;

                    /* Validate DI */
                    if (pATR->DI == 0)
                    {
                        pATR->DI = 0x02;
                        iErr = -4;
                        goto bad;
                    }
                }
                else if(iCount == 1) // TA2
                    pATR->T = pATR->TA[pATR->TA_count - 1] & 0x0f ;

                else if (iCount == 2) // TA3
                    pATR->ifsc = pATR->TA[pATR->TA_count - 1];
                else if (iCount == 3) // TA4
                {
                    pATR->power = pATR->TA[pATR->TA_count - 1] & 0x01F;
/*
                    // Validate Power
                    if ((pATR->power != 0x01) && (pATR->power != 0x03))
                    {
                        iErr = -5;
                        goto bad;
                    }*/
                }

                dataLength--;
                if (dataLength <= 0)
                    break;
            }
            // grab the TB byte
            if (flags & 0x20)
            {
                pATR->TB[pATR->TB_count++] = pATR->data[offset++];
                dataLength--;

                if (iCount == 0)
                {
                    /* Validate TB0 */
                    if (pATR->TB[pATR->TB_count - 1] != 0x00)
                    {
                        iErr = -6;
                        goto bad;
                    }
                }
                else if(iCount == 2)
                {
                    pATR->bwi = pATR->TB[pATR->TB_count - 1] >> 4;
                    pATR->cwi = pATR->TB[pATR->TB_count - 1] & 0x0F;
                }

                if (dataLength <= 0)
                    break;
            }
            // grab the TC byte
            if (flags & 0x40)
            {
                pATR->TC[pATR->TC_count++] = pATR->data[offset++];
                dataLength--;

                if (iCount == 0)
                {
                    pATR->guard_time = pATR->TC[pATR->TC_count - 1];
                    //if (pATR->guard_time == 255)
                    //    pATR->guard_time = 0;

                    //pATR->crc = pATR->TC[pATR->TC_count - 1] & 0x01;
                }

                if (dataLength <= 0)
                    break;
            }
            /* we have another TD to start the next block */
            if (flags & 0x80)
            {

                flags = pATR->data[offset++];

                if((flags & 0x0F) != 15)
                    pATR->transmission_protocol = (flags & 0x0F);

                dataLength--;
                if (dataLength <= 0)
                    break;
            }

            ++iCount;
        }

        /* at this point we should have any historical chars
        * and TCK left to read ... check that we have enough
        * data to cover this and abort if not
        */
        if (dataLength < pATR->historical_count)
        {
            iErr = -7;
            goto bad;
        }

        if ( pATR->historical_count > 0 )
        {
            /* copy historical */
            memcpy(pATR->historical,pATR->data + offset, pATR->historical_count);
            offset += pATR->historical_count;
            dataLength -= pATR->historical_count;
        }

        /* check to see if we have 9x 00 and in which case it
        * probably isn't the TCK but is the response to the
        * implied select - there doesn't appear to be a nice
        * way of determining if TCK is transmitted or not
        *
        * for the moment at least this data is considered
        * as part of the ATR ... it is transmitted always on
        * reset but isn't strictly/logically in the ATR
        * but such things we simply have to live with
        */
        if (dataLength >= 2)
        {
            if ( ((pATR->data[offset] & 0x90) == 0x90) && (pATR->data[offset + 1] == 0x00) )
            {
                offset += 2;
                dataLength -= 2;
            }
        }

        /* anything remaining should be the TCK */
        if (dataLength > 0)
        {
            /* now we should just have TCK left */
            pATR->TCK = 1;
            pATR->TCK_sent = pATR->data[offset++];

            /* now we calculate what the TCK should be so it can be checked
            * by user logic if desired
            */
            pATR->TCK_calc = 0;

            for(i = 1; i < (offset - 1); i++)
                pATR->TCK_calc ^= (pATR->data[i]);

            dataLength--;

            /* Validate TCK */
            if (pATR->TCK_sent != pATR->TCK_calc)
            {
                iErr = -8;
                goto bad;
            }
        }

        /* extra data should be "deleted" from the ATR */
        if (dataLength > 0)
            goto bad;

    }

    return 0;

bad:

    SCI_ERROR("Decode ATR Failed : Err[ %d ]\n", iErr);

    if ( iErr < -2 )
        iErr = -2;

    return iErr;
}


static int SCI_CoreReadATR(UINT8 *atrData)
{
	int receivedLength = -1, nextDepth = 0;
	unsigned char historicCnt = 0;
	SCI_RETURN_T waitFull = SCI_SUCCESS;
	unsigned int i;
    unsigned char statFlag;
    unsigned char status, intr_status, intr_mask;

	SCI_PRINT("enter SCI_CoreReadATR\n");

    //Clear FIFO
    SCI_RegClearFIFO();

	/* ATR Reception using burst mode for TS & TA0 */
    SCI_RegSetReceiveFIFODepth(2);

    SCI_RegSetCardResetActiveLow(ENABLE);
    SCI_RegSetPwrMonHigh(DISABLE);

	/* Mask IRQs */
	SCI_RegEnableInterrupt(SCI_IRQ_FRF);

	//cold reset when first start or warm reset when active
	SCI_RegPowerOn();

	//Receive enable
	SCI_RegEnableReceive();


	/* wait for full */
	waitFull = SCI_ISRWaitIRQ(SCI_IRQ_FRF, 250);

	if (waitFull != SCI_SUCCESS)
	{
        status = SCI_RegGetStatus();
        intr_status = SCI_RegGetInterruptStatus();
        intr_mask = SCI_RegGetInterruptMask();

		/* sci pwr off */
        SCI_RegPowerOff();
		SCI_ERROR("There was error during BCAS read1.\n");
        SCI_ERROR("stat : 0x%02X, intr_stat : 0x%02X, intr_mask : 0x%02X, intr_trig : 0x%02X\n", status, intr_status, intr_mask, intr_status & ~intr_mask);
		return -31;//SCI_IRQ_TIME_OUT;
	}
	receivedLength = 2;

	/* read TS, T0 */
	for(i = 0; i < receivedLength; i++)
        atrData[i] = SCI_RegReadData();

	historicCnt = atrData[1] & 0x0f;  // read historic Count value from T0

	do
	{
		/* Count TAi~TDi */
		nextDepth = (atrData[receivedLength - 1] >> 4 & 0x01) +
                    (atrData[receivedLength - 1] >> 5 & 0x01) +
                    (atrData[receivedLength - 1] >> 6 & 0x01) +
                    (atrData[receivedLength - 1] >> 7 & 0x01);

		if(nextDepth == 0)
			break;

		if(receivedLength + nextDepth <= SCI_MAX_ATR_DATA_BUFFER_LENGTH)
		{
			SCI_RegSetReceiveFIFODepth(nextDepth);

			/* wait for full */
			waitFull = SCI_ISRWaitIRQ(SCI_IRQ_FRF, 250);

			if (waitFull != SCI_SUCCESS)
			{
                status = SCI_RegGetStatus();
                intr_status = SCI_RegGetInterruptStatus();
                intr_mask = SCI_RegGetInterruptMask();

				/* sci off */
                SCI_RegPowerOff();

				SCI_ERROR("There was error during BCAS read2.\n");
                SCI_ERROR("stat : 0x%02X, intr_stat : 0x%02X, intr_mask : 0x%02X, intr_trig : 0x%02X\n", status, intr_status, intr_mask, intr_status & ~intr_mask);
				return -32;//SCI_NO_CARD;
			}

			/* read TAi~TDi if it is exists */
			for(i = 0; i < nextDepth; i++)
                (atrData + receivedLength)[i] = SCI_RegReadData();

			receivedLength += nextDepth;

            statFlag = SCI_RegGetCardStatus();

			if(!statFlag)
			{
                status = SCI_RegGetStatus();
                intr_status = SCI_RegGetInterruptStatus();
                intr_mask = SCI_RegGetInterruptMask();

				/* sci off */
                SCI_RegPowerOff();

				SCI_ERROR("There was state error during BCAS read3.\n");
                SCI_ERROR("stat : 0x%02X, intr_stat : 0x%02X, intr_mask : 0x%02X, intr_trig : 0x%02X\n", status, intr_status, intr_mask, intr_status & ~intr_mask);
				return -33;//SCI_NO_CARD;
			}
		}
		else
			return SCI_OVER_MAX_ATR_DATA;

        statFlag = SCI_RegGetCardStatus();

        if(!statFlag)
		{
            status = SCI_RegGetStatus();
            intr_status = SCI_RegGetInterruptStatus();
            intr_mask = SCI_RegGetInterruptMask();

			/* sci off */
            SCI_RegPowerOff();
            SCI_ERROR("There was state error during BCAS read4.\n");
                SCI_ERROR("stat : 0x%02X, intr_stat : 0x%02X, intr_mask : 0x%02X, intr_trig : 0x%02X\n", status, intr_status, intr_mask, intr_status & ~intr_mask);
			return -35;//SCI_NO_CARD;
		}

	}while (receivedLength < SCI_MAX_ATR_DATA_BUFFER_LENGTH);

	if (historicCnt != 0)   /* read historic bytes & clk*/
		nextDepth = historicCnt + 1;
	else                    /* read clk */
		nextDepth = 1;

	if(receivedLength + nextDepth <= SCI_MAX_ATR_DATA_BUFFER_LENGTH)
	{
		/* read operation */
		SCI_RegSetReceiveFIFODepth(nextDepth);

		/* wait for full */
		waitFull = SCI_ISRWaitIRQ(SCI_IRQ_FRF, 250);
		if (waitFull != SCI_SUCCESS)
		{
            status = SCI_RegGetStatus();
            intr_status = SCI_RegGetInterruptStatus();
            intr_mask = SCI_RegGetInterruptMask();

			/* sci off */
            SCI_RegPowerOff();
			SCI_ERROR("There was error during BCAS read5.\n");
            SCI_ERROR("stat : 0x%02X, intr_stat : 0x%02X, intr_mask : 0x%02X, intr_trig : 0x%02X\n", status, intr_status, intr_mask, intr_status & ~intr_mask);
			return -36;//SCI_IRQ_TIME_OUT;
		}

		for(i = 0; i < nextDepth; i++)
            (atrData + receivedLength)[i] = SCI_RegReadData();

		receivedLength += nextDepth;

        statFlag = SCI_RegGetCardStatus();

        if(!statFlag)
		{
            status = SCI_RegGetStatus();
            intr_status = SCI_RegGetInterruptStatus();
            intr_mask = SCI_RegGetInterruptMask();

			/* sci off */
            SCI_RegPowerOff();
			SCI_ERROR("There was error during BCAS read6.\n");
            SCI_ERROR("stat : 0x%02X, intr_stat : 0x%02X, intr_mask : 0x%02X, intr_trig : 0x%02X\n", status, intr_status, intr_mask, intr_status & ~intr_mask);
			return -37;//SCI_NO_CARD;
		}
	}
	else
		return SCI_OVER_MAX_ATR_DATA;

	SCI_RegDisableInterrupt(SCI_IRQ_FRF);

 	/* send a Null command */
    SCI_RegSendNullCmd();

#if 0
    // DisplayATR
	for( i = 0; i < receivedLength; i++)
		SCI_TRACE("ATR[%02d]  =  0x%02X \n", i, atrData[i]);
#endif

	return receivedLength;
}


static unsigned char SCI_CoreSendAndReceiveBlock(SCI_INFO_T* pSCIinfo)
{
	unsigned char cardStatus;
    unsigned int i, remainder;
	
    cardStatus =  SCI_RegGetCardStatus();
    if(cardStatus != STATUS_ACTIVE)
		return SCI_NO_CARD;

	/* Rx, Tx Fifo Clear */
    SCI_RegClearFIFO();
	
	// Initialyze RxBlock 
    //memset(&pSCIinfo->rxBlock, 0, sizeof(pSCIinfo->rxBlock));
	pSCIinfo->txBlock.flagCompleated = 0;
	pSCIinfo->rxBlock.flagCompleated = 0;

    // wait to transmit and receive
    pSCIinfo->SciStatus = SCI_STATUS_TRANSMIT;

    SCI_RegSetNAD(pSCIinfo->txBlock.nad);
    SCI_RegSetPCB(pSCIinfo->txBlock.pcb);
    SCI_RegSetLength(pSCIinfo->txBlock.len);

	SCI_RegEnableInterrupt(SCI_IRQ_EOT|SCI_IRQ_EOR|SCI_IRQ_FTE|SCI_IRQ_FRNE|SCI_IRQ_FRF);
	
    /* TX on */
    SCI_RegEnableTransmit();
    
    if(pSCIinfo->txBlock.len > pSCIinfo->TxFIFODepth)
        remainder = pSCIinfo->TxFIFODepth;
    else
        remainder = pSCIinfo->txBlock.len;
    
    for(i = 0; i < remainder; i++)
        SCI_RegWriteData(pSCIinfo->txBlock.inf[i]);
    
    pSCIinfo->txBlock.idx = remainder;
    
    SCI_TRACE("TX nad : %02x, TX pcb : %02x, TX len : %02x\n", pSCIinfo->txBlock.nad, pSCIinfo->txBlock.pcb, pSCIinfo->txBlock.len);

    // Wait Event End (wait for Tx and Rx)
    if(pSCIinfo->SciAutoRecv)
        SCI_ISRWaitIRQ(SCI_IRQ_EOR, pSCIinfo->txBlock.len + 60); //100);
    else
    {
        SCI_ISRWaitIRQ(SCI_IRQ_EOT, pSCIinfo->txBlock.len + 10);
    	SCI_RegEnableReceive();
        SCI_ISRWaitIRQ(SCI_IRQ_EOR, 50);
    }
	
	SCI_RegDisableInterrupt(SCI_IRQ_EOT|SCI_IRQ_EOR|SCI_IRQ_FTE|SCI_IRQ_FRNE|SCI_IRQ_FRF);
	return pSCIinfo->rxBlock.flagCompleated;
}


static SCI_RETURN_T SCI_CoreRequestIFS(SCI_INFO_T* pSCIinfo)
{
    int ret = SCI_FAIL;
	
	int RxResult = 0;
	//SCI_TRANSMIT_INFO_T txBlock = { 0,};
	//SCI_TRANSMIT_INFO_T rxBlock = { 0,};

	SCI_PRINT("enter SCI_CoreRequestIFS[%d]\n", pSCIinfo->ifs);

	pSCIinfo->txBlock.nad = 0;
	pSCIinfo->txBlock.pcb = IFS_REQUEST;
	pSCIinfo->txBlock.len = 1;
	pSCIinfo->txBlock.inf[0] = pSCIinfo->ifs;

	
	RxResult = SCI_CoreSendAndReceiveBlock(pSCIinfo);	
	
	if(RxResult)  // Received Success!!
	{		
	    SCI_TRACE("RX nad : %02x, RX pcb : %02x, RX len : %02x\n", pSCIinfo->rxBlock.nad, pSCIinfo->rxBlock.pcb, pSCIinfo->rxBlock.len);
		
		if(pSCIinfo->rxBlock.pcb == IFS_RESPONSE)
		{
			pSCIinfo->ifs = pSCIinfo->rxBlock.inf[0];
				ret = SCI_SUCCESS;
		}
	}
	else
		ret = SCI_FAIL;

	SCI_PRINT("ReqIFS[%d] Done\n", pSCIinfo->ifs);

	return ret;
}


static void SCI_CoreConfigInterface(SCI_INFO_T* pSCIinfo)
{
	LX_SCI_ATR_T* pATR = &pSCIinfo->atr;

	SCI_PRINT("enter SCI_CoreConfigInterface\n");

	/* Configure SCI by ATR */
    if(pATR->T == 0)
        SCI_RegSetModeByteORBlock(T_BYTE_MODE);
    else if(pATR->T == 1)
        SCI_RegSetModeByteORBlock(T_BLOCK_MODE);
    else
        SCI_ERROR("Specific mode Error\n");

    SCI_RegSetAddionalGuardTime(pATR->guard_time);

    SCI_RegSetTimeOut(pATR->bwi, pATR->cwi);

    SCI_RegDisableModeAutoRXChange();
    SCI_RegSetModeBigEndian();
    SCI_RegSetModeConversion(pATR->conversion);
    SCI_RegSetMaxRepetition(0x03);
    SCI_RegSetModeDataCheckLRC();


    SCI_RegSetDI(pATR->DI);

	SCI_CoreSetBaud(pATR->FI, pATR->DI, pSCIinfo->clock_supplied_khz);

    SCI_RegSetFlowControl(3, ENABLE, ENABLE);

    SCI_RegSetReceiveFIFODepth(MAX_RF_DEPTH);
	pSCIinfo->RxFIFODepth = MAX_RF_DEPTH;
	
    SCI_RegSetTransmitFIFODepth(MAX_TF_DEPTH);
	pSCIinfo->TxFIFODepth = MAX_TF_DEPTH;


	//SCI_RegEnableInterrupt(SCI_IRQ_EOT|SCI_IRQ_EOR);
	SCI_RegEnableInterrupt(SCI_IRQ_EOT|SCI_IRQ_EOR|SCI_IRQ_FTE|SCI_IRQ_FRNE|SCI_IRQ_FRF);
}


void SCI_Reset(SCI_INFO_T* pSCIinfo)
{
	SCI_PRINT("enter SCI_Reset\n");

	SCI_RegDisableInterrupt(SCI_IRQ_ALL);
	SCI_RegEnableInterrupt(SCI_IRQ_CDPWR | SCI_IRQ_ERR);

    SCI_RegSetCardResetActiveLow(ENABLE);
    SCI_RegPowerOff();
    SCI_RegReset();

    // Need delay for reset (if not, will fail)
    udelay(10);

    SCI_RegSetInit();
    SCI_RegSetPwrMonHigh(DISABLE);
    //SCI_RegSetCardResetActiveLow(DISABLE);


    // for GetATR
    SCI_RegSetExtSmartCardChipInterface(IF_MOD_TDA8004);

#if 0
    SCI_RegSetDelay(RST_DUR_40_000_PERIOS, CLK_DIV_32);
#else 
    /* 20131210 jinhwan.bae to support New BCAS card type A, ACK after RST is about 6.75ms 
       make HW IP wait time to 49600*2 ticks / 6.25M = 15.8ms
       before , 40000*2 ticks / 6.25M = 12.8 ms
       IP raise ATR-timeout after that, regardless of Driver Wait time */
    SCI_RegSetDelay(RST_DUR_49_600_PERIOS, CLK_DIV_32);
#endif

	// calculate baud parameter value for setting baud rate
    SCI_CoreSetBaud(SCI_DEFAULT_FS, SCI_DEFAULT_DI, pSCIinfo->clock_supplied_khz);

    //SCI_RegDisableModeAutoRXChange();
    //SCI_RegSetModeBigEndian();
    //SCI_RegSetModeConversion(0);
    SCI_RegSetMaxRepetition(0x03);
    //SCI_RegSetModeDataCheckLRC();
    SCI_RegSetModeByteORBlock(T_BYTE_MODE);


    SCI_RegSetFlowControl(3, DISABLE, ENABLE);

    //SCI_RegSetWorkWaitTime(0x3F);

    //SCI_RegSetAddionalGuardTime(0);

    //SCI_RegSetDI(0x0F);

	//memset(pSCIinfo, 0, sizeof(SCI_INFO_T));
}


SCI_RETURN_T SCI_Activate(SCI_INFO_T* info)
{
	SCI_RETURN_T res = SCI_SUCCESS;
    UINT8 i;
	UINT8 ifs = 254;	// MAX IFS

	SINT8 atrDataLength;
    UINT8 cardStatus = SCI_RegGetCardStatus();

	info->deviceNS = 0;
	info->cardNS = 0;

	SCI_PRINT("enter SCI_Activate : 0x%02X\n", cardStatus);

	switch(cardStatus)
	{
		case STATUS_READY:
			SCI_ERROR("Card isn't inserted!\n");
            res = SCI_NO_CARD;
			break;

		case STATUS_DETECTED:
		case STATUS_ACTIVE:
        {
        	memset(&info->atr, 0, sizeof(LX_SCI_ATR_T));

            SCI_Reset(info);

			/* 20131004 jinhwan.bae 
			    ARIB spec changed and specified card interface VCC to 5V Class A only
			    To Changed Activation Sequennce from Class B -> Class A  to Class A -> Class B.
			    Class B support remains because we should support various field condition */
        	SCI_SetVoltage(SCI_VOLTAGE_5V);

    		atrDataLength = SCI_CoreReadATR(info->atr.data);
        	if (atrDataLength < 0)    // fail to get ATR
        	{
                //SCI_Reset(info);
                /* 20131210 jinhwanbae
                                 Set Also 5V after ATR Fail Case.
                                 It means, always maintain 5V interface according to BCAS Spec */
        		//SCI_SetVoltage(SCI_VOLTAGE_3V);
        		SCI_SetVoltage(SCI_VOLTAGE_5V);

				cardStatus = SCI_RegGetCardStatus();
				if(cardStatus == STATUS_READY)
				{
        			SCI_ERROR("ReadATR Error - card isn't inserted\n");

        			return -15;
				}
				
    		    atrDataLength = SCI_CoreReadATR(info->atr.data);
        		if (atrDataLength < 0)    // fail to get ATR
        		{
        			SCI_ERROR("ReadATR Error");
					
        			return -10;
        		}
        	}
        	res = SCI_CoreDecodeATR(&info->atr, atrDataLength);

        	if (res != SCI_SUCCESS)
        	{
                SCI_ERROR("Failed to decode ATR\n");
        		return -12;//SCI_FAIL;
        	}

        	SCI_CoreConfigInterface(info);

    		info->SciAutoRecv = 1;

            info->ifs = ifs;	// IFS can be 1 ~ 254, 254 is max INF size

            for(i = 0; i < 3; i++)
            {
        	    res = SCI_CoreRequestIFS(info);

                if(res == SCI_SUCCESS)
                    break;
            }

        	if (res != SCI_SUCCESS)
        		return -14;//res;
        		
        	if(info->ifs != ifs)
				return -13;

        	//_requestWTX();
        	//res = pSCI->configWTX(&pSCI->atr);
        	info->isActivated = 1;
		}
		break;

		case STATUS_FAILED:
			SCI_ERROR("Status is failed. Removed card and insert again!\n");
            res = SCI_FAIL;
			break;

		default:
			SCI_ERROR("Card status[%d] is not supported!\n",cardStatus);
            res = SCI_FAIL;
			break;
	}

	return res;
}

void SCI_Deactivate(SCI_INFO_T* info)
{
	SCI_PRINT("enter SCI_Deactivate\n");

    SCI_RegSetCardResetActiveLow(ENABLE);
    SCI_RegPowerOff();
    SCI_RegSetPwrMonHigh(DISABLE);
	
    info->isActivated = 0;
}


SCI_RETURN_T SCI_GetATR(LX_SCI_ATR_T* pATR)
{
	SCI_RETURN_T ret;
    SINT8 atrDataLength;
	UINT8 cardStatus =  SCI_RegGetCardStatus();

	SCI_PRINT("enter SCI_GetATR : 0x%02X\n", cardStatus);

    switch(cardStatus)
    {
        case STATUS_READY:
    		/* Card is not inserted */
    		ret = SCI_NO_CARD;
            break;

        case STATUS_DETECTED:
        case STATUS_ACTIVE:
        	/* Clear ATR buffer */
        	memset(pATR, 0, sizeof(LX_SCI_ATR_T));

    		ret = SCI_CoreReadATR(pATR->data);
            atrDataLength = ret;

            if(atrDataLength >= 0)
                ret = SCI_SUCCESS;

            break;

        case STATUS_FAILED:
		    ret = SCI_FAIL;
            SCI_ERROR("GetATR Err : %d\n", ret);
            break;

        default:
		    ret = SCI_INVAL_PARAM;
            break;
    }

	return ret;
}


LX_SCI_STATUS SCI_GetStatus(void)
{
	UINT8 cardStatus;
	LX_SCI_STATUS ret = SCI_STATUS_READY;


    cardStatus = SCI_RegGetCardStatus();
	SCI_PRINT("enter SCI_GetStatus : 0x%02X\n", SCI_RegGetStatus());

	switch(cardStatus)
	{
		case STATUS_READY:
			ret = SCI_STATUS_READY;
			break;
		case STATUS_DETECTED:
			ret = SCI_STATUS_DETECTED;
			break;
		case STATUS_ACTIVE:
			ret = SCI_STATUS_ACTIVE;
			break;
		case STATUS_FAILED:
			ret = SCI_STATUS_FAIL;
			break;
		default:
			SCI_ERROR("Card status[%d] is not supported!\n", cardStatus);
			break;
	}
	return ret;
}


LX_SCI_VOLTAGE SCI_GetVoltage(void)
{
	UINT8 voltage = 0;
	LX_SCI_VOLTAGE ret;


    voltage = SCI_RegGetVoltage();

	SCI_PRINT("enter SCI_GetVoltage : 0x%02X\n", voltage);

    switch(voltage)
    {
        case 0:
		    ret = SCI_VOLTAGE_3V;
            break;
        case 1:
		    ret = SCI_VOLTAGE_5V;
            break;

        default:
		    ret = SCI_VOLTAGGE_UNKNOWN;
			SCI_ERROR("invalid voltage value[%d]!\n",voltage);
            break;
    }
	return ret;
}


SCI_RETURN_T SCI_SetVoltage(LX_SCI_VOLTAGE voltage)
{
	SCI_PRINT("enter SCI_SetVoltage : 0x%02X\n", voltage);

	if (voltage == SCI_VOLTAGE_5V)
        SCI_RegSelVoltage(VCC_SEL_5V);
	else if (voltage == SCI_VOLTAGE_3V)
        SCI_RegSelVoltage(VCC_SEL_3V);
	else
	{
        SCI_RegSelVoltage(VCC_SEL_3V);
		SCI_ERROR("This voltage[%d] is not supported!!!\n", voltage);
		return SCI_INVAL_PARAM;
	}

	return SCI_SUCCESS;
}


#if 0
/*
TRANSMISSION_NEXT_IBLOCK
RETRANSMISSION_IBLOCK
READY_NEXT_IBLOCK
REQUEST_RETRANSMISSION
TRANSMISSION_DONE

*/


static SCI_ANALYZE_STATUS SCI_CoreAnalyzeRxBlock(SCI_INFO_T* pSCIinfo)
{
	int ret;
	
	if(!(pSCIinfo->txBlock.pcb & 0x80))	// Tx is I Block
	{
		SCI_PRINT("Tx is I Block\n");
		
		if(pSCIinfo->txBlock.pcb & 0x20) // moreData
		{
			SCI_PRINT("more Data!\n");
			if(!(pSCIinfo->rxBlock.pcb & 0x80))// Rx is I Block
			{
				SCI_PRINT("Rx is I Block - Strange...Check!\n");

			}
			else if(!(pSCIinfo->rxBlock.pcb & 0x40)) // Rx is R Block
			{
				SCI_PRINT("Rx is R Block\n");
				
				if(GET_R_BLOCK_NS(pSCIinfo->rxBlock.pcb) == GET_I_BLOCK_NS(pSCIinfo->txBlock.pcb))	// retamsnission
					ret = RETRANSMISSION_IBLOCK;
				else		// Next Transmission
					ret = TRANSMISSION_NEXT_IBLOCK;
			}
			else // Rx is S BLOCK
			{


			}
		}
		else	// not more Data - Transmission is done
		{
			if(!(pSCIinfo->rxBlock.pcb & 0x80))// Rx is I Block
			{
				SCI_PRINT("Rx is I Block\n");
				
				if(GET_I_BLOCK_NS(pSCIinfo->rxBlock.pcb) == pSCIinfo->cardNS)	// received correct I Block
				{
					if(pSCIinfo->rxBlock.pcb & 0x20) // moreData
					{
						SCI_PRINT("more Data!\n");

						ret = READY_NEXT_IBLOCK;
					}
					else
					{
						SCI_PRINT("Transmission Done!\n");
						
						ret = TRANSMISSION_DONE;
					}
				}
				else
				{
					SCI_PRINT("Invalid Rx N(S)!\n");
					
					ret = REQUEST_RETRANSMISSION;
				}
			}
			else if(!(pSCIinfo->rxBlock.pcb & 0x40)) // Rx is R Block
			{
				SCI_PRINT("Rx is R Block\n");
				
				if(GET_R_BLOCK_NS(pSCIinfo->rxBlock.pcb) == GET_I_BLOCK_NS(pSCIinfo->txBlock.pcb))	// retamsnission
					ret = RETRANSMISSION_IBLOCK;
				else		// Strange !! why R(N)? // request previous block?
				{
					SCI_PRINT("No more Tx Data!! Strange...Check!\n");
					
					ret = READY_NEXT_IBLOCK;	// or ret = REQUEST_RETRANSMISSION;
				}
			}
			else // Rx is S BLOCK
			{


			}
		}
	}
	else if(!(pSCIinfo->txBlock.pcb & 0x40))	// Tx is R Block
	{
		SCI_PRINT("Tx is R Block\n");
		
		if(!(pSCIinfo->rxBlock.pcb & 0x80))// Rx is I Block
		{
			SCI_PRINT("Rx is I Block\n");
			
			if(GET_I_BLOCK_NS(pSCIinfo->rxBlock.pcb) == GET_R_BLOCK_NS(pSCIinfo->txBlock.pcb))	// received correct I Block
			{
				if(pSCIinfo->rxBlock.pcb & 0x20) // moreData
				{
					SCI_PRINT("more Data!\n");
					
					ret = READY_NEXT_IBLOCK;
				}
				else
					ret = TRANSMISSION_DONE;
			}
			else		// strange N(S)......need check
			{
				SCI_PRINT("Invalid Rx N(S)!\n");
				
				ret = REQUEST_RETRANSMISSION;
			}
		}
		else if(!(pSCIinfo->rxBlock.pcb & 0x40)) // Resend R block
		{
			SCI_PRINT("Rx is R Block\n");
			
			if(GET_R_BLOCK_NS(pSCIinfo->rxBlock.pcb) == pSCIinfo->deviceNS)	// retransmission
				ret = RETRANSMISSION_IBLOCK;
			else
				ret = TRANSMISSION_NEXT_IBLOCK;
		}
		else	// Rx is S Block
		{


		}
	}
	else // S BLOCK
	{
		if(!(pSCIinfo->rxBlock.pcb & 0x80))// Rx is I Block
		{
			SCI_PRINT("Rx is I Block\n");
			
			if(GET_I_BLOCK_NS(pSCIinfo->rxBlock.pcb) == pSCIinfo->cardNS)	// received correct I Block
			{
				if(pSCIinfo->rxBlock.pcb & 0x20) // moreData
				{
					SCI_PRINT("more Data!\n");

					ret = READY_NEXT_IBLOCK;
				}
				else
					ret = TRANSMISSION_DONE;
			}
			else
			{
				SCI_PRINT("Invalid Rx N(S)!\n");
				
				ret = REQUEST_RETRANSMISSION;
			}
		}
		else if(!(pSCIinfo->rxBlock.pcb & 0x40)) // Rx is R block
		{
			SCI_PRINT("Rx is R Block\n");
			
			if(GET_R_BLOCK_NS(pSCIinfo->rxBlock.pcb) == pSCIinfo->deviceNS)	// Next I Block
				ret = RETRANSMISSION_IBLOCK;
			else			// have to send Previous I BLock
				ret = TRANSMISSION_NEXT_IBLOCK;
		}
		else	// Rx is S Block
		{


		}
	}

	return ret;
}



int SCI_NewTransmit(SCI_INFO_T* pSCIinfo, LX_SCI_TRANSMIT* transmitArg)
{
	int ret = SCI_FAIL;
	
	unsigned char RxResult = 0;
	unsigned char  numRxTimeOut = 0;
	unsigned char transmitCount = 0;
	int txAPDUoffset = 0, rxAPDUoffset = 0;
	
	SCI_ANALYZE_STATUS analyzeStatus;

	// if I Block
	// make PCB of I Block for Tx
	pSCIinfo->txBlock.pcb = I_BLOCK;
    if(pSCIinfo->deviceNS)
        pSCIinfo->txBlock.pcb |= I_BLOCK_NS;
    else
        pSCIinfo->txBlock.pcb &= ~I_BLOCK_NS;

	// make first Tx Block
    if(transmitArg->txBuf.len - txAPDUoffset > pSCIinfo->ifs)
   	{
        pSCIinfo->txBlock.len = pSCIinfo->ifs;
        pSCIinfo->txBlock.pcb |= I_BLOCK_MORE_DATA;
		
   	}
    else
        pSCIinfo->txBlock.len = transmitArg->txBuf.len - txAPDUoffset;

	memcpy(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf + txAPDUoffset, pSCIinfo->txBlock.len);
	txAPDUoffset += pSCIinfo->txBlock.len;
	

	do{
		RxResult = SCI_CoreSendAndReceiveBlock(pSCIinfo);	
		transmitCount++;

		if(RxResult)	//Received Block
		{
	        SCI_TRACE("RX nad : %02x, RX pcb : %02x, RX len : %02x\n", pSCIinfo->rxBlock.nad, pSCIinfo->rxBlock.pcb, pSCIinfo->rxBlock.len);
			
			analyzeStatus = SCI_CoreAnalyzeRxBlock(pSCIinfo);

			if(analyzeStatus == TRANSMISSION_NEXT_IBLOCK)
			{
				// device NS increase
                if(pSCIinfo->deviceNS)
                    pSCIinfo->deviceNS = 0;
                else
                    pSCIinfo->deviceNS = 1;

				// make PCB of I Block for Tx
				pSCIinfo->txBlock.pcb = I_BLOCK;
				if(pSCIinfo->deviceNS)
					pSCIinfo->txBlock.pcb |= I_BLOCK_NS;
				else
					pSCIinfo->txBlock.pcb &= ~I_BLOCK_NS;

				// Make Next Tx Block (Chained Next I Block)
			    if(transmitArg->txBuf.len - txAPDUoffset > pSCIinfo->ifs)
		    	{
			        pSCIinfo->txBlock.len = pSCIinfo->ifs;
			        pSCIinfo->txBlock.pcb |= I_BLOCK_MORE_DATA;
				}
			    else
			        pSCIinfo->txBlock.len = transmitArg->txBuf.len - txAPDUoffset;
	
				memcpy(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf + txAPDUoffset, pSCIinfo->txBlock.len);
				txAPDUoffset += pSCIinfo->txBlock.len;


			}
			else if(analyzeStatus == RETRANSMISSION_IBLOCK)
			{
				if((pSCIinfo->txBlock.pcb & 0x80))	// if Tx was not  I Block
				{
					// make PCB of I Block for Tx
					pSCIinfo->txBlock.pcb = I_BLOCK;
					if(pSCIinfo->deviceNS)
						pSCIinfo->txBlock.pcb |= I_BLOCK_NS;
					else
						pSCIinfo->txBlock.pcb &= ~I_BLOCK_NS;
					
					// Make Next Tx Block (Chained Next I Block)
				    if(txAPDUoffset % pSCIinfo->ifs)	// Last chained Block
				        pSCIinfo->txBlock.len = txAPDUoffset % pSCIinfo->ifs;
				    else
			    	{
				        pSCIinfo->txBlock.len = pSCIinfo->ifs;
				        pSCIinfo->txBlock.pcb |= I_BLOCK_MORE_DATA;
					}
		
					memcpy(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf + txAPDUoffset - pSCIinfo->txBlock.len, pSCIinfo->txBlock.len);
					
				}
			}
			else if(analyzeStatus == READY_NEXT_IBLOCK)
			{
				if(!(pSCIinfo->rxBlock.pcb & 0x80))	// Rx is I Block
				{
					memcpy(transmitArg->rxBuf.buf + rxAPDUoffset, pSCIinfo->rxBlock.inf, pSCIinfo->rxBlock.len);
					rxAPDUoffset += pSCIinfo->rxBlock.len;
					
					// card NS increase
	                if(pSCIinfo->cardNS)
	                    pSCIinfo->cardNS = 0;
	                else
	                    pSCIinfo->cardNS = 1;
				}
				
				// next Tx Data is Request Next Block
				pSCIinfo->txBlock.nad = 0;
				pSCIinfo->txBlock.pcb = R_BLOCK ;
				if(pSCIinfo->cardNS)
					pSCIinfo->txBlock.pcb |= R_BLOCK_NR;
				else
					pSCIinfo->txBlock.pcb &= ~R_BLOCK_NR;
				pSCIinfo->txBlock.len = 0;
			}
			else if(analyzeStatus == REQUEST_RETRANSMISSION)
			{
				// next Tx Data is Request Retransmit
				pSCIinfo->txBlock.nad = 0;
				pSCIinfo->txBlock.pcb = R_BLOCK ;
				if(pSCIinfo->cardNS)
					pSCIinfo->txBlock.pcb |= R_BLOCK_NR;
				else
					pSCIinfo->txBlock.pcb &= ~R_BLOCK_NR;
				pSCIinfo->txBlock.len = 0;
			}
			else if(analyzeStatus == TRANSMISSION_DONE)
			{
				memcpy(transmitArg->rxBuf.buf + rxAPDUoffset, pSCIinfo->rxBlock.inf, pSCIinfo->rxBlock.len);
				rxAPDUoffset += pSCIinfo->rxBlock.len;				
				
				//card NS increase
                if(pSCIinfo->cardNS)
                    pSCIinfo->cardNS = 0;
                else
                    pSCIinfo->cardNS = 1;

				transmitArg->rxBuf.len = rxAPDUoffset;

				ret = SCI_SUCCESS;
				break;
			}
			else
			{
				SCI_ERROR("What the hell!?\n");

			}
		}
		else	// Rx Fail -> TX : Request Resynch
		{
			SCI_TRACE("RX Fail, status : 0x%02x, irqStatus : 0x%02x, intrMask : 0x%02x\n", SCI_RegGetStatus(), SCI_RegGetInterruptStatus(), SCI_RegGetInterruptMask());
			numRxTimeOut++;

			if(numRxTimeOut < 3)
			{
				SCI_PRINT("Request Resynch\n");
				// next Tx Data is Request Next Block
				pSCIinfo->txBlock.nad = 0;
				pSCIinfo->txBlock.pcb = R_BLOCK ;
				if(pSCIinfo->cardNS)
					pSCIinfo->txBlock.pcb |= R_BLOCK_NR;
				else
					pSCIinfo->txBlock.pcb &= ~R_BLOCK_NR;
				pSCIinfo->txBlock.len = 0;
			}
			else
			{
				SCI_PRINT("Request Resynch\n");
				numRxTimeOut = 0;
				
	            // next Tx Data is Request Resynch            
	            pSCIinfo->txBlock.nad = 0;
	            pSCIinfo->txBlock.pcb = RESYNCH_REQUEST;
	            pSCIinfo->txBlock.len = 0;
			}
		}
	}while(transmitCount < 30);

	SCI_PRINT("SCI_Transmit Done - TxNum[%d]\n", transmitCount);

	return ret;
}
#endif

int SCI_Transmit(SCI_INFO_T* pSCIinfo, LX_SCI_TRANSMIT* transmitArg)
{
	SCI_RETURN_T ret = SCI_FAIL;

	int RxResult = 0;
	unsigned char transmitCount = 0, sBlockType;
	unsigned int moreDataOffset = 0;
	unsigned int rxDataLimit = transmitArg->rxBuf.len; // jinhwan.bae 20140205 protect memcpy latch up at boot
	int numRxTimeOut = 0;
	//20141024 hyunjong.yim add for retry logic
#ifdef SCI_RETRY
	int retryCnt = 0;
	int intFlag = 0;
	int i;
	SCI_INFO_T txBackup;
#endif
	
	SCI_PRINT("enter SCI_Transmit : 0x%02X\n", SCI_RegGetStatus());

    if(!pSCIinfo->isActivated)
    {
	    SCI_ERROR("SCI not activate\n");
        return ret;
    }
	
    // initialyze first Transmit Data (I Block)
    pSCIinfo->txBlock.nad = 0;
    pSCIinfo->txBlock.len = transmitArg->txBuf.len;
	
	// jinhwan.bae 20140205 protect memcpy latch up at boot
#if 0	
    memcpy(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf, pSCIinfo->txBlock.len);
#else
	if(copy_from_user(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf, pSCIinfo->txBlock.len))
	{
		SCI_ERROR("copy_from_user failed\n");
        return ret;
	}
#endif

	pSCIinfo->txBlock.pcb = I_BLOCK;
    if(pSCIinfo->deviceNS)
        pSCIinfo->txBlock.pcb |= I_BLOCK_NS;
    else
        pSCIinfo->txBlock.pcb &= ~I_BLOCK_NS;

	// 20141024 hyunjong.yim add for retry logic
#ifdef SCI_RETRY
	if(pSCIinfo->txBlock.len == 5)
	{
		if(pSCIinfo->txBlock.inf[0] == 0x90 && pSCIinfo->txBlock.inf[1] == 0x30 && pSCIinfo->txBlock.inf[2] == 0x00 
				&& pSCIinfo->txBlock.inf[3] == 0x00 && pSCIinfo->txBlock.inf[4] == 0x00)
		{
			SCI_ERROR("[SCI] Get INT Command\n");
			intFlag = 1;
			memcpy(&txBackup, pSCIinfo, sizeof(SCI_INFO_T));
		}
	}
#endif

    // Loop to get correct Receive Data (I Block)
	do{
        pSCIinfo->txBlock.idx = 0;
		RxResult = SCI_CoreSendAndReceiveBlock(pSCIinfo);
		
		transmitCount++;
 
        if(RxResult)  // Received Success!!
        {
	        SCI_TRACE("RX nad : %02x, RX pcb : %02x, RX len : %02x\n", pSCIinfo->rxBlock.nad, pSCIinfo->rxBlock.pcb, pSCIinfo->rxBlock.len);

            // Error handle when received wrong NAD
            if(pSCIinfo->txBlock.nad != pSCIinfo->rxBlock.nad)
            {
	            // next Tx Data is Request Resynch            
	            pSCIinfo->txBlock.nad = 0;
	            pSCIinfo->txBlock.pcb = RESYNCH_REQUEST;
	            pSCIinfo->txBlock.len = 0;

                continue;
            }
            
            // analyze Receive Block
            if(!(pSCIinfo->rxBlock.pcb & 0x80))   // I Block
            {
                SCI_TRACE("Recv I Block\n");

				// 20141024 hyunjong.yim for retry logic
#ifdef SCI_RETRY
				if(intFlag == 1)
				{
					if((pSCIinfo->rxBlock.inf[pSCIinfo->rxBlock.len - 1] != 0x00 || pSCIinfo-> rxBlock.inf[pSCIinfo->rxBlock.len - 2] != 0x90) 
							&& pSCIinfo->rxBlock.len == 61)
					{
						SCI_ERROR("[SCI]Wrong SW1, 2 with INT command in upper layer. Try again\n");
						if(retryCnt < 3)
						{
							SCI_ERROR("[SCI]Retry %d\n", retryCnt);
							retryCnt++;
							udelay(100);
							continue;
						}
						SCI_ERROR("[SCI]Too many retry.\n");
					}
				}
#endif
				if(GET_I_BLOCK_NS(pSCIinfo->rxBlock.pcb) == pSCIinfo->cardNS)	// Success Transmission	
				{				
					if(pSCIinfo->rxBlock.pcb & 0x1F)	// strange I Block
					{
                    	SCI_TRACE("strange I Block!!\n");
						
						// next Tx Data is Request Retransmit
						pSCIinfo->txBlock.nad = 0;
						pSCIinfo->txBlock.pcb = R_BLOCK | (pSCIinfo->cardNS << 4);;
						pSCIinfo->txBlock.len = 0;

						continue;
					}
						
					/* I Block response */					
                    SCI_TRACE("Correct N(S). Transmit Success!\n");					

                    if(pSCIinfo->cardNS)
                        pSCIinfo->cardNS = 0;
                    else
                        pSCIinfo->cardNS = 1;

					// jinhwan.bae 20140205 protect memcpy latch up at boot
					if((moreDataOffset + pSCIinfo->rxBlock.len) > rxDataLimit)
					{
						SCI_ERROR("\n[MARINE] Could Not Copy RX Data [%d] is greater than [%d]\n", (moreDataOffset + pSCIinfo->rxBlock.len), rxDataLimit );
						return ret;
					}

                    // copy to Rx Buffer of upper layer
#if 0               // jinhwan.bae 20140205 protect memcpy latch up at boot
                    memcpy(transmitArg->rxBuf.buf + moreDataOffset, pSCIinfo->rxBlock.inf, pSCIinfo->rxBlock.len);
#else
					if(copy_to_user(transmitArg->rxBuf.buf + moreDataOffset, pSCIinfo->rxBlock.inf, pSCIinfo->rxBlock.len))
					{
						SCI_ERROR("copy_from_user failed\n");
						return ret;
					}
#endif
                    transmitArg->rxBuf.len = pSCIinfo->rxBlock.len + moreDataOffset;
					
                    if(pSCIinfo->rxBlock.pcb & I_BLOCK_MORE_DATA)	// More data denoted!!
                	{
	                    SCI_TRACE("More-data!!\n");
						
						//moreDataFlag = 1;
						moreDataOffset += pSCIinfo->rxBlock.len;
						
						// Request R(N)
						pSCIinfo->txBlock.pcb = R_BLOCK;
                        if(pSCIinfo->cardNS)
                            pSCIinfo->txBlock.pcb |= R_BLOCK_NR;
                        else
                            pSCIinfo->txBlock.pcb &= ~R_BLOCK_NR;

						pSCIinfo->txBlock.len = 0;

						continue;
                	}
					
	                if(pSCIinfo->deviceNS)
	                    pSCIinfo->deviceNS = 0;
	                else
	                    pSCIinfo->deviceNS = 1;
				
                    ret = SCI_SUCCESS;
					break;
				}
                else // NS is different -> request R Block
                {
                    SCI_TRACE("different N(S)! Check this!\n");
                    
                    // next Tx Data is Request Retransmit
                    pSCIinfo->txBlock.nad = 0;
                    pSCIinfo->txBlock.pcb = R_BLOCK | (pSCIinfo->cardNS << 4);;
                    pSCIinfo->txBlock.len = 0;
                }
            }
            else if(!(pSCIinfo->rxBlock.pcb & 0x40))  // R Block
            {
                SCI_TRACE("Recv R Block\n");
                
				if(GET_R_BLOCK_NS(pSCIinfo->rxBlock.pcb) == pSCIinfo->deviceNS)
				{
                    if(GET_R_BLOCK_ERROR(pSCIinfo->rxBlock.pcb) == 0)  // retransmission I Block
                    {
                        // configu Transmit Data (I Block)
                        pSCIinfo->txBlock.len = transmitArg->txBuf.len;
#if 0					 // jinhwan.bae 20140205 protect memcpy latch up at boot
                        memcpy(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf, pSCIinfo->txBlock.len);
#else
						if(copy_from_user(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf, pSCIinfo->txBlock.len))
						{
							SCI_ERROR("copy_from_user failed\n");
							return ret;
						}
#endif

						pSCIinfo->txBlock.pcb = I_BLOCK;
                        if(pSCIinfo->deviceNS)
                            pSCIinfo->txBlock.pcb |= I_BLOCK_NS;
                        else
                            pSCIinfo->txBlock.pcb &= ~I_BLOCK_NS;
                    }
                    else
                    {
                		SCI_TRACE("R Block Error[%x]\n", GET_R_BLOCK_ERROR(pSCIinfo->rxBlock.pcb));

						//pSCIinfo->txBlock.nad = 0;
						pSCIinfo->txBlock.pcb = R_BLOCK;
                        if(pSCIinfo->deviceNS)
                            pSCIinfo->txBlock.pcb |= R_BLOCK_NR;
                        else
                            pSCIinfo->txBlock.pcb &= ~R_BLOCK_NR;

						pSCIinfo->txBlock.len = 0;
                    }
				}
				else							// How?
				{
					SCI_ERROR("N(R) is different\n");
				}
            }
            else    // S Block
            {
                SCI_TRACE("Recv S Block\n");

				sBlockType = pSCIinfo->rxBlock.pcb & S_BLOCK_TYPE;

				switch(sBlockType)
				{
				case S_BLOCK_RESYNCH:
	                if(pSCIinfo->rxBlock.pcb & S_BLOCK_RESPONSE)   // Resynch
	                {                    
	                    if(pSCIinfo->txBlock.pcb == RESYNCH_REQUEST)    // Tx - Rx Req - Resp match correctly
	                    {
	            			pSCIinfo->deviceNS = 0;
	            			pSCIinfo->cardNS = 0;
							
	                        // configu Transmit Data (I Block)
	                        pSCIinfo->txBlock.pcb = I_BLOCK;
	                        if(pSCIinfo->deviceNS)
	                            pSCIinfo->txBlock.pcb |= I_BLOCK_NS;
	                        else
	                            pSCIinfo->txBlock.pcb &= ~I_BLOCK_NS;
	            
	                        pSCIinfo->txBlock.len = transmitArg->txBuf.len;
							
#if 0					 	// jinhwan.bae 20140205 protect memcpy latch up at boot
							memcpy(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf, pSCIinfo->txBlock.len);
#else
							if(copy_from_user(pSCIinfo->txBlock.inf, transmitArg->txBuf.buf, pSCIinfo->txBlock.len))
							{
								SCI_ERROR("copy_from_user failed\n");
								return ret;
							}
#endif							
				
							numRxTimeOut = 0;
	                    }
						else
							SCI_ERROR("strange RESYNCH process\n");							
	                }
					else if(pSCIinfo->rxBlock.pcb & RESYNCH_REQUEST)
					{
			            // next Tx Data is Request Resynch            
			            pSCIinfo->txBlock.nad = 0;
			            pSCIinfo->txBlock.pcb = RESYNCH_RESPONSE;
			            pSCIinfo->txBlock.len = 0;
						
	            		pSCIinfo->cardNS = 0;
					}
					else
						SCI_ERROR("Unknown RESYNCH Block[%02X]\n", pSCIinfo->rxBlock.pcb);						
					break;
					
				case S_BLOCK_IFS:

					break;

				case S_BLOCK_ABORT:

					break;

				case S_BLOCK_WTX:

					break;

				default:
                    SCI_ERROR("what S Block???\n");

					break;
				}// End switch of S Block Type 
            } // End S Block Handle
#ifdef SCI_RETRY
			// 20141024 hyunjong.yim for retry logic
			if(intFlag == 1)
			{	
				volatile UINT8 len = pSCIinfo->rxBlock.len;
				volatile UINT8 val1 = pSCIinfo->rxBlock.inf[len - 2];
				volatile UINT8 val2 = pSCIinfo->rxBlock.inf[len - 1];

				if(((val2 != 0x00) || (val1 != 0x90)) && (len == 61))
				{
					if((pSCIinfo->rxBlock.inf[pSCIinfo->rxBlock.len - 1] != 0x00) || (pSCIinfo->rxBlock.inf[pSCIinfo->rxBlock.len - 2] != 0x90)) 
					{
						if(retryCnt < 3)
						{
							SCI_ERROR("[SCI]Retry %d\n", retryCnt);
							retryCnt++;
							udelay(100);
							memcpy(pSCIinfo, &txBackup, sizeof(SCI_INFO_T));
							continue;
						}
						SCI_ERROR("[SCI]Too many retry.\n");
					}
				}
			}
#endif
        }// End Rx Success
        else    // Receive Fail!!
        {
			SCI_TRACE("RX Fail, status : 0x%02x, irqStatus : 0x%02x, intrMask : 0x%02x\n", SCI_RegGetStatus(), SCI_RegGetInterruptStatus(), SCI_RegGetInterruptMask());
			
			numRxTimeOut++;

			if(numRxTimeOut < 3)
			{				
				//pSCIinfo->txBlock.nad = 0;
				pSCIinfo->txBlock.pcb = R_BLOCK;
				if(pSCIinfo->cardNS)
					pSCIinfo->txBlock.pcb |= R_BLOCK_NR;
				else
					pSCIinfo->txBlock.pcb &= ~R_BLOCK_NR;
				
				pSCIinfo->txBlock.len = 0;
			}
			else
			{
	            // next Tx Data is Request Resynch            
	            pSCIinfo->txBlock.nad = 0;
	            pSCIinfo->txBlock.pcb = RESYNCH_REQUEST;
	            pSCIinfo->txBlock.len = 0;
			}
        }		
	}while(transmitCount < MAX_TRANSMISSION_TIMES);
	
	SCI_PRINT("SCI_Transmit Done - TxNum[%d]\n", transmitCount);

	return ret;
}

