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
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvCMDQ.c
// @brief  CMDQ Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "halCMDQ.h"
#include "drvCMDQ.h"
#include "regCMDQ.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvCMDQ_priv.h"

static MS_U8* testaddress;
int count_command_set = 0;
static MS_U32 new_start_pointer = 0;
static MS_U32 Memory_Start_Address = 0;
static MS_U32 Memory_End_Address = 0;


//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
static DrvCMDQ_DbgLevel _u32CMDQDBGLevel;
static MS_U32 CMDQ_MODE = RING_BUFFER_MODE;    // only agate will use DIRECT_MODE, but agate will no longer use
static MS_U32 TOTAL_BUFFER_SIZE = 0;

static MSIF_Version _drv_comq_version = {
    .DDI = { CMDQ_DRV_VERSION, },
};



//define mutex(FIX: mutex should remove if utopia2.0)
static MS_S32 g_s32CMDQMutex=-1;
#define CMDQ_MUTEX_CREATE()          g_s32CMDQMutex = MsOS_CreateMutex(E_MSOS_FIFO, "CMDQMUTEX" , MSOS_PROCESS_SHARED)
#define CMDQ_MUTEX_LOCK()            MsOS_ObtainMutex(g_s32CMDQMutex,MSOS_WAIT_FOREVER)
#define CMDQ_MUTEX_UNLOCK()          MsOS_ReleaseMutex(g_s32CMDQMutex)
#define CMDQ_MUTEX_DELETE()          MsOS_DeleteMutex(g_s32CMDQMutex)
//physical address to virtual address
#define MS_CMDQ2PA(_addr_)                (MS_U32)MsOS_VA2PA((_addr_))
#define MS_PA2KSEG0(_addr_)             (MS_U32)MsOS_PA2KSEG0((_addr_))
#define MS_PA2KSEG1(_addr_)             (MS_U32)MsOS_PA2KSEG1((_addr_))

#define VERIFY_DIRECT_MODE 0
//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define DEBUG_CMDQ(debug_level, x)     do { if (_u32CMDQDBGLevel >= (debug_level)) (x); } while(0)

//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------
///Get MMIO_Base and set struct _REG_CMDQCtrl * _CMDQCtrl
//---------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Init(void)
{
    MS_U32 u32Bank, u32BankSize = 0x0;
    MS_U32 Check_inited;

    if(FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_CMDQ))
    {
		printf("\033[35mFunction = %s, Line = %d, u32Bank is 0x%X and u32BankSize is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)u32Bank, (unsigned int)u32BankSize);
        return DRVCMDQ_FAIL;
    }

    CMDQ_MUTEX_CREATE();
    HAL_CMDQ_SetBank(u32Bank);

    // check if inited
    Check_inited = HAL_CMDQ_Read_Dummy_Register();
    if(Check_inited == 0x4567)
    {
        printf("\033[35mdummy is %lu, CMDQ already inited\033[m\n", Check_inited);
        return DRVCMDQ_OK;
    }

    HAL_CMDQ_Enable();
    HAL_CMDQ_Reset();

    if(FALSE == HAL_CMDQ_Set_Mode(CMDQ_MODE))
    {
        printf("\033[35mCMDQ Set Mode FAIL!!\033[m\n");
        return DRVCMDQ_FAIL;
    }

	// only 16 bit for dummy register, check if already init
    MDrv_CMDQ_Set_dummy_Address(0x4567);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVCMDQ_FAIL;
    }

    *ppVersion = &_drv_comq_version;
    return DRVCMDQ_OK;
}

//--------------------------------------------------------------------------------------------------
// In ring-buffer mode, this function will trig for update reg_sw_wr_mi_wadr
// The CMDQ will keep on executing cmd until reg_rd_mi_radr reach reg_sw_wr_mi_wadr
//--------------------------------------------------------------------------------------------------
DRVCMDQ_RESULT MDrv_CMDQ_Start(MS_BOOL bStart)
{
    if(bStart == 1)
    {
        HAL_CMDQ_Start();
        return DRVCMDQ_OK;
    }
    else
    {
        DEBUG_CMDQ(E_DRVCMDQ_DBG_L1, printf("bStart %d, \n",bStart));
        return DRVCMDQ_FAIL;
    }
}

//--------------------------------------------------------------------------------------------------
// Set the Start and End address of CMDQ DRAM(this function will call MDrv_CMDQ_Set_Buffer)
// @ \b in: SmallAddr => Start address
// @ \b in: BigAddr   => End address
//--------------------------------------------------------------------------------------------------
// this function should be called for setting CMDQ Buffer(System Side), need to add in ioctl 
// => MDrv_CMDQ_Get_Memory_Size and MDrv_CMDQ_Set_timer_ratio can be add to CMDQ_Init
DRVCMDQ_RESULT MDrv_CMDQ_Get_Memory_Size(MS_U32 SmallAddr, MS_U32 BigAddr)
{
    CMDQ_MUTEX_LOCK();
    Memory_Start_Address = SmallAddr;
    new_start_pointer = SmallAddr;
    Memory_End_Address = BigAddr;
    MDrv_CMDQ_Set_Buffer(SmallAddr,BigAddr);

    // Number of Command Line
    TOTAL_BUFFER_SIZE = ((BigAddr - SmallAddr) + 1);
    //printf("\033[35mTotal Buffer Size is 0x%X\033[m\n", (unsigned int)TOTAL_BUFFER_SIZE); // joe.liu

    CMDQ_MUTEX_UNLOCK();
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_Set_Buffer(MS_U32 StartAddr, MS_U32 EndAddr)
{
    HAL_CMDQ_Set_Start_Pointer(StartAddr);
    HAL_CMDQ_Set_End_Pointer(EndAddr);
    return DRVCMDQ_OK;
}

// currently not used
DRVCMDQ_RESULT MDrv_CMDQ_Set_Start_Pointer(MS_U32 StartAddr)
{
    CMDQ_MUTEX_LOCK();
    HAL_CMDQ_Set_Start_Pointer(StartAddr);
    CMDQ_MUTEX_UNLOCK();
    return DRVCMDQ_OK;
}

// currently not used
DRVCMDQ_RESULT MDrv_CMDQ_Set_End_Pointer(MS_U32 EndAddr)
{
    CMDQ_MUTEX_LOCK();
    HAL_CMDQ_Set_End_Pointer(EndAddr);
    CMDQ_MUTEX_UNLOCK();
    return DRVCMDQ_OK;
}

// currently not used
DRVCMDQ_RESULT MDrv_CMDQ_Reset(void)
{
    CMDQ_MUTEX_LOCK();
    HAL_CMDQ_Reset();
    CMDQ_MUTEX_UNLOCK();
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_Exit(void)
{
    CMDQ_MUTEX_LOCK();
    HAL_CMDQ_Stop();
    CMDQ_MUTEX_UNLOCK();
    CMDQ_MUTEX_DELETE();
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_Stop(void)
{
    //modify the later dates.....
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_SetDbgLevel(DrvCMDQ_DbgLevel DbgLevel)
{
    _u32CMDQDBGLevel = DbgLevel;
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_Receive(CH_Struct_Pointer New_IP_Head)
{
	//printf("\033[35m================ Use Ring Buffer to do CMDQ... ================\033[m\n");
    CMDQ_MUTEX_LOCK();
    int New_CommandArray_Number = 0;	// Command_Line Number in DRAM
	int Transfer_Number = 0;			// Command Number

	MS_U32 Current_Read_Addr = 0; // Current_Read_Addr, Last_Write_Addr is Memory_Start_Address
	MS_U32 Usable_Size = 0;

	CAF_Struct_Pointer New_CommandArray_Pointer = New_IP_Head->Pointer_To_CAFArray;
	Transfer_Number = New_IP_Head->Command_Number;
#if (VERIFY_DIRECT_MODE)
	// Use Direct Mode to Test Ring_Buffer
	//we will write one NULL_cmd and one wait_cmd at the end the cmd -> this waitng is for auto-running next CAFs
	printf("\033[35mUse Direct Mode to Test Ring_Buffer\033[m\n"); // joe.liu
	New_CommandArray_Number = New_IP_Head->Command_Number + 2;
#else
	New_CommandArray_Number = New_IP_Head->Command_Number;
#endif

	if(New_CommandArray_Number & 0x0000001)  // new receive struct odd or even
    {
		New_CommandArray_Number = (New_CommandArray_Number+1) / 2;
    }
    else
    {
		New_CommandArray_Number = New_CommandArray_Number/2;
    }

    if(Memory_Start_Address == new_start_pointer)
    {
        // **** Check if command_size over total_buffer_size
        if(New_CommandArray_Number >= TOTAL_BUFFER_SIZE)
    	{
            printf("\033[35mError, New_CommandArray_Number >= TOTAL_BUFFER_SIZE\033[m\n");
            CMDQ_MUTEX_UNLOCK();
            return DRVCMDQ_FAIL;
    	}
        printf("Memory_Start_Address 0x%X\n", (unsigned int)Memory_Start_Address);
        printf("New_CommandArray_Number %d\n", New_CommandArray_Number);

        //to get a VA by using PA, Remember to use MsOS_Mapping to get CMDQ_Buffer Mapping
        testaddress = (MS_U8*)MS_PA2KSEG1((MS_U32)Memory_Start_Address << 4);
        printf("\033[35mtestaddress start from 0x%X\033[m\n", (unsigned int)testaddress);

    	// step_1: Write CMD to CMDQ Buffer
        MDrv_CMDQ_Transfer(New_CommandArray_Pointer, Transfer_Number);
        MsOS_Dcache_Flush((MS_U32)MS_PA2KSEG1((MS_U32)Memory_Start_Address << 4), (MS_U32)New_CommandArray_Number * 16);  //flush Dcache data
        MsOS_FlushMemory();  //flush OCP data

    	// step_2: Update software write position of command in miu
        // (Need to take care of if using ring buffer, which will occurs when New_CommandArray_Number = TOTAL_BUFFER_SIZE)
        Memory_Start_Address = Memory_Start_Address + New_CommandArray_Number;
        if(Memory_Start_Address > Memory_End_Address)
        {
            // actually, in this case, New_CommandArray_Number will large/equal to TOTAL_BUFFER_SIZE, 
            // this case should return DRVCMDQ_FAIL at upper check
            printf("Adjust Memory_Start_Address form 0x%X to ", (unsigned int)Memory_Start_Address);
            Memory_Start_Address = Memory_Start_Address - Memory_End_Address - 1 + new_start_pointer;
            printf("0x%X\n", (unsigned int)Memory_Start_Address);
        }
        HAL_CMDQ_Write_Pointer(Memory_Start_Address);

        // step_3: trigger to start
        MDrv_CMDQ_Start(1);
    }
    else
    {
    	// **** check usable_size and command_size to determinr if cmdq full ****
        Current_Read_Addr = HAL_CMDQ_Read_Pointer();

        if(Current_Read_Addr > Memory_Start_Address)
        {
            // This means already using ring-buffer, the Memory_Start_Address is looped
            Usable_Size = Current_Read_Addr - Memory_Start_Address;
        }
        else
        {
            // this means previous job is done(equal case) or non ring-buffered(small)
            Usable_Size = TOTAL_BUFFER_SIZE - (Memory_Start_Address - Current_Read_Addr);
        }

        // if command_size over usable_size
        if(New_CommandArray_Number >= Usable_Size)
        {
            printf("\033[35mError, New_CommandArray_Number > Usable_Size\033[m\n");
            CMDQ_MUTEX_UNLOCK();
            return DRVCMDQ_FAIL;
        }
        printf("Memory_Start_Address 0x%X\n", (unsigned int)Memory_Start_Address);
        printf("New_CommandArray_Number %d\n",New_CommandArray_Number);

        testaddress = (MS_U8*)MS_PA2KSEG1((MS_U32)Memory_Start_Address << 4); // Use Current Write Address

        // step_1: Write CMD to CMDQ Buffer
        MDrv_CMDQ_Transfer(New_CommandArray_Pointer, Transfer_Number);
        MsOS_Dcache_Flush((MS_U32)MS_PA2KSEG1((MS_U32)Memory_Start_Address << 4), (MS_U32)New_CommandArray_Number*16);  //flush Dcache data
        MsOS_FlushMemory();  //flush OCP data

        // step_2: Update software write position of command in miu(Need to take care of if using ring buffer)
        Memory_Start_Address = Memory_Start_Address + New_CommandArray_Number;
        if(Memory_Start_Address > Memory_End_Address)
        {
            printf("Adjust Memory_Start_Address form 0x%X to ", (unsigned int)Memory_Start_Address);
            Memory_Start_Address = Memory_Start_Address - Memory_End_Address - 1 + new_start_pointer;
            printf("0x%X\n", (unsigned int)Memory_Start_Address);
        }
        HAL_CMDQ_Write_Pointer(Memory_Start_Address);

        // step_3: trigger to start
        MDrv_CMDQ_Start(1);
    }

    count_command_set = count_command_set + 1;
    CMDQ_MUTEX_UNLOCK();
    return DRVCMDQ_OK;
}

void Insert_Redundant_Null(int Need_Null_Number)
{
    int j=0;
	Need_Null_Number *= 2;

	for(j=0;j<Need_Null_Number;j++)
    {
		printf("Insert %d Null_Commands\n", j);
        testaddress[0] = 0;
        testaddress[1] = 0;
        testaddress[2] = 0;
        testaddress[3] = 0;
        testaddress[4] = 0;
        testaddress[5] = 0;
        testaddress[6] = 0;
        testaddress[7] = 0;   // write 36 null command
        testaddress = testaddress + 8;
    }
}

//--------------------------------------------------------------------------------------------------
// Write the CAF content to CMDQ Buffer
// @ \b in: Array_Command => Pointer to CAF
// @ \b in: Number_Array_Command => Number of Commands
//--------------------------------------------------------------------------------------------------
void MDrv_CMDQ_Transfer(CAF_Struct_Pointer Array_Command, int Number_Array_Command)
{
    int i=0;
    int mask_of_BF=0x00ffffff;
    int BInt1=0,BInt2=0,BInt3=0;
    int WInt1=0,WInt2=0,WInt3=0;
    int bit_4=0x00ff;
    int bit_6=0x0000ff;
    int DDR_1=0x10,DDR_2=0,DDR_3=0,DDR_4=0,DDR_5=0,DDR_6=0,DDR_7=0,DDR_8=0;
    int odd_or_even=0;

    if(Number_Array_Command&0x0000001)  // new receive struct odd or even
    {
        odd_or_even=1;
    }
    else
    {
        odd_or_even=0;
    }
    
	for(i = 0; i < Number_Array_Command; i++)
    {
		// This Command is over Memory_End_Address, change to Buffer Head
		// However, the Memory_End_Address can insert CMD
	    if((MS_CMDQ2PA((MS_U32)testaddress) >> 4) > Memory_End_Address)
		{
    	    printf("Restart Ring buffer: Memory_End_Address is  0x%X\n", (unsigned int)Memory_End_Address);
    		printf("                     Current Write Address is 0x%X\n", (unsigned int)MS_CMDQ2PA((MS_U32)testaddress));
		    printf("Change testaddress to Buffer Head: 0x%X\n", (unsigned int)MS_PA2KSEG1((MS_U32)new_start_pointer << 4));

			testaddress = (MS_U8*)MS_PA2KSEG1((MS_U32)new_start_pointer << 4);
		}

		DDR_1 = 0x10, DDR_2 = 0, DDR_3 = 0, DDR_4 = 0, DDR_5 = 0, DDR_6 = 0, DDR_7 = 0, DDR_8 = 0;
        
		if(Array_Command[i].operation=='b')
        {
            BInt1=(Array_Command[i].destionation_address&mask_of_BF)>>2;
            BInt2=Array_Command[i].destionation_value;
            BInt3=Array_Command[i].mask;
            BInt2=(65535-BInt2);
            DDR_1=0x10,
            DDR_2=BInt1>>16,
            DDR_3=(BInt1>>8&(bit_4)),
            DDR_4=(BInt1)&(bit_6),
            DDR_5=BInt3>>8,
            DDR_6=BInt3&(bit_4),
            DDR_7=BInt2>>8,
            DDR_8=BInt2&(bit_4);

            testaddress[0] =DDR_8;
            testaddress[1] =DDR_7;
            testaddress[2] =DDR_6;
            testaddress[3] =DDR_5;
            testaddress[4] =DDR_4;
            testaddress[5] =DDR_3;
            testaddress[6] =DDR_2;
            testaddress[7] =DDR_1;
        }
        else if(Array_Command[i].operation == 'd')
        {

            testaddress[0] =0xff;
            testaddress[1] =0x7f;
            testaddress[2] =0x00;
            testaddress[3] =0x80;
            testaddress[4] =0xCB;
            testaddress[5] =0x1A;
            testaddress[6] =0x09;
            testaddress[7] =0x30;
        }
        else if(Array_Command[i].operation == 't')
        {
            BInt2=Array_Command[i].destionation_value;
            DDR_5=BInt2>>8,
            DDR_6=BInt2&(bit_4);

            testaddress[0] =0x00;
            testaddress[1] =0x00;
            testaddress[2] =DDR_6;
            testaddress[3] =DDR_5;
            testaddress[4] =0xCA;
            testaddress[5] =0x1A;
            testaddress[6] =0x09;
            testaddress[7] =0x10;
        }
        else if(Array_Command[i].operation == 'r')
        {
            BInt2=Array_Command[i].destionation_value;
            DDR_5=BInt2>>8,
            DDR_6=BInt2&(bit_4);

            testaddress[0] =0x00;
            testaddress[1] =0xff;
            testaddress[2] =DDR_6;
            testaddress[3] =DDR_5;
            testaddress[4] =0xCB;
            testaddress[5] =0x1A;
            testaddress[6] =0x09;
            testaddress[7] =0x10;
        }
        else if(Array_Command[i].operation == 'w')
        {
            WInt1 = (Array_Command[i].destionation_address & mask_of_BF) >> 2; // RIU_addr << 1 | 0xBF000000 => destionation_address
            WInt2=Array_Command[i].destionation_value;
            WInt3=Array_Command[i].mask;
            DDR_1=0x10,
            DDR_2=WInt1>>16,
            DDR_3=(WInt1>>8&(bit_4)),
            DDR_4=(WInt1)&(bit_6),
            DDR_5=WInt2>>8,
            DDR_6=WInt2&(bit_4),
            DDR_7=WInt3>>8,
            DDR_8=WInt3&(bit_4);

            testaddress[0] =DDR_8;
            testaddress[1] =DDR_7;
            testaddress[2] =DDR_6;
            testaddress[3] =DDR_5;
            testaddress[4] =DDR_4;
            testaddress[5] =DDR_3;
            testaddress[6] =DDR_2;
            testaddress[7] =DDR_1;

			// write command
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_1); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_2); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_3); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_4); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_5); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_6); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_7); // joe.liu
			//printf("\033[33mFunction = %s, Line = %d, write 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)DDR_8); // joe.liu
		}
        else if(Array_Command[i].operation == 'p')  // polling_eq
        {
			printf("\033[35mFunction = %s, Line = %d, this is polling equal command\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
            WInt1 = (Array_Command[i].destionation_address & mask_of_BF) >> 2; // RIU_addr << 1 | 0xBF000000 => destionation_address
            WInt2 = Array_Command[i].destionation_value;
            WInt3 = Array_Command[i].mask;
            DDR_1 = 0x30,
            DDR_2 = WInt1>>16,
            DDR_3 = (WInt1 >> 8 & (bit_4)),
            DDR_4 = (WInt1) & (bit_6),
            DDR_5 = WInt2 >> 8,
            DDR_6 = WInt2 & (bit_4),
            DDR_7 = WInt3 >> 8,
            DDR_8 = WInt3 & (bit_4);

            testaddress[0] = DDR_8;
            testaddress[1] = DDR_7;
            testaddress[2] = DDR_6;
            testaddress[3] = DDR_5;
            testaddress[4] = DDR_4;
            testaddress[5] = DDR_3;
            testaddress[6] = DDR_2;
            testaddress[7] = DDR_1;
        }
        else if(Array_Command[i].operation == 'C')  // polling_neq
        {
            printf("\033[35mFunction = %s, Line = %d, this is polling non-equal command\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
            WInt1 = (Array_Command[i].destionation_address & mask_of_BF) >> 2; // RIU_addr << 1 | 0xBF000000 => destionation_address
            WInt2 = Array_Command[i].destionation_value;
            WInt3 = Array_Command[i].mask;
            DDR_1 = 0xb1,
            DDR_2 = WInt1>>16,
            DDR_3 = (WInt1 >> 8 & (bit_4)),
            DDR_4 = (WInt1) & (bit_6),
            DDR_5 = WInt2 >> 8,
            DDR_6 = WInt2 & (bit_4),
            DDR_7 = WInt3 >> 8,
            DDR_8 = WInt3 & (bit_4);

            testaddress[0] = DDR_8;
            testaddress[1] = DDR_7;
            testaddress[2] = DDR_6;
            testaddress[3] = DDR_5;
            testaddress[4] = DDR_4;
            testaddress[5] = DDR_3;
            testaddress[6] = DDR_2;
            testaddress[7] = DDR_1;
        }
		else
		{
			printf("\033[35mError, Unknown opertion!! Array_Command[%d].operation is %u\033[m\n", i, Array_Command[i].operation);
		}

        testaddress = testaddress + 8;
    }

	if(odd_or_even==1)
	{
#if 1
	    testaddress[0] = 0xFF;
        testaddress[1] = 0xFF;
        testaddress[2] = 0;
        testaddress[3] = 0;
        testaddress[4] = 0;
        testaddress[5] = 0;
        testaddress[6] = 0;
        testaddress[7] = 0;
        testaddress=testaddress+8;
#else
    	// test if CMDQ works
		testaddress[0] = 0x00;
    	testaddress[1] = 0x00;
    	testaddress[2] = 0x34;
    	testaddress[3] = 0x12;
    	testaddress[4] = 0xF8;
    	testaddress[5] = 0x1A;
    	testaddress[6] = 0x09;
    	testaddress[7] = 0x10;
    	testaddress = testaddress + 8;
#endif
    	printf("\033[35mFunction = %s, Line = %d, Odd instructions\033[m\n", __PRETTY_FUNCTION__, __LINE__);
	}

#if (VERIFY_DIRECT_MODE)
    testaddress[0] = 0;
    testaddress[1] = 0;
    testaddress[2] = 0;
    testaddress[3] = 0;
    testaddress[4] = 0;
    testaddress[5] = 0;
    testaddress[6] = 0;
    testaddress[7] = 0;
    testaddress = testaddress + 8;

	// polling_eq, only for Direct Mode
	testaddress[0] = 0xff;
    testaddress[1] = 0x7f;
    testaddress[2] = 0x00;
    testaddress[3] = 0x80;
    testaddress[4] = 0xCB;
    testaddress[5] = 0x1A;
    testaddress[6] = 0x09;
    testaddress[7] = 0x30;
    testaddress = testaddress + 8;

	printf("\033[35mFunction = %s, Line = %d, Insert Test Code Done\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
#endif
}

void MDrv_CMDQ_Set_dummy_Address(MS_U32 dummyAddr)
{
    HAL_CMDQ_Write_Dummy_Register(dummyAddr);
}

DRVCMDQ_RESULT MDrv_CMDQ_Set_timer_ratio(MS_U32 time,MS_U32 ratio)
{
    HAL_CMDQ_Set_Timer(time);
    HAL_CMDQ_Set_Ratio(ratio);
    return DRVCMDQ_OK;
}

DRVCMDQ_RESULT MDrv_CMDQ_Printf_Crash_Command(void)
{
    MS_U32 Command_15_0_bit,Command_31_16_bit,Command_55_32_bit,Command_63_56_bit;
    Command_15_0_bit=HAL_CMDQ_Error_Command(CMDQ_CRASH_15_0_BIT);
    Command_31_16_bit=HAL_CMDQ_Error_Command(CMDQ_CRASH_31_16_BIT);
    Command_55_32_bit=HAL_CMDQ_Error_Command(CMDQ_CRASH_55_32_BIT);
    Command_63_56_bit=HAL_CMDQ_Error_Command(CMDQ_CRASH_63_56_BIT);

    printf("\033[35mCommand _bits: %lx, %lx, %lx, %lx\033[m\n", Command_63_56_bit, Command_55_32_bit, Command_31_16_bit, Command_15_0_bit);
    return DRVCMDQ_OK;
}

/////////////////////////////////////////////////////////////////
//////// UTOPIA2 CODE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////
enum
{
    CMDQ_POOL_ID_CMDQ0=0
} eCMDQ_PoolID;

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register module
//--------------------------------------------------------------------------------------------------
void CMDQRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_CMDQ, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)CMDQOpen, (FUtopiaClose)CMDQClose, (FUtopiaIOctl)CMDQIoctl);

    // 2. Resource register
    void* psResource = NULL;

    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, CMDQ_POOL_ID_CMDQ0);

    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("cmdq0", sizeof(CMDQ_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, CMDQ_POOL_ID_CMDQ0);

    //UtopiaResourceCreate("cmdq1", sizeof(CMDQ_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, CMDQ_POOL_ID_CMDQ0);

    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, CMDQ_POOL_ID_CMDQ0);

    // You can get the number of init times, this can prevent multiple INIT
    MDrv_CMDQ_Init();
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use CMDQ
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 CMDQOpen(void** ppInstance, void* pAttribute)
{
    MS_U32 u32Ret = 0;
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // for multi-process safe, check if already other Instance exist
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_CMDQ, &pModule);
    
    if(UtopiaResourceObtain(pModule, CMDQ_POOL_ID_CMDQ0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pCMDQResPri = NULL;    
    UtopiaResourceGetPrivate(pResource, &pCMDQResPri); 
     
    if(*(MS_U32 *)pCMDQResPri == 0)
    {
        *(MS_U32 *)pCMDQResPri = 1;
        UtopiaResourceRelease(pResource);
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, [CMDQ INFO] can not open an INSTANCE\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
        UtopiaResourceRelease(pResource);
        return 1;
    }
#endif
    
    printf("\033[35mFunction = %s, Line = %d, [CMDQ INFO] OPEN INSTANCE...\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
    CMDQ_INSTANT_PRIVATE *pCMDQPri = NULL;

    // instance is allocated here, also can allocate private for internal use, ex, CMDQ_INSTANT_PRIVATE, ppInstance will point to a pointer of the created UTOPIA_INSTANCE
    u32Ret = UtopiaInstanceCreate(sizeof(CMDQ_INSTANT_PRIVATE), ppInstance);
    if(u32Ret)
    {
        printf("\033[35mFunction = %s, Line = %d, [CMDQ OPEN] Create Instance Failed!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        return 1;
    }
    // set the pCMDQPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pCMDQPri);

    // setup func in private and assign the calling func in func ptr in instance private
    pCMDQPri->fpCMDQGetLibVer = (IOCTL_CMDQ_GETLIBVER)MDrv_CMDQ_GetLibVer;
    pCMDQPri->fpCMDQPrintf_Crash_Command = (IOCTL_CMDQ_PRINTFCRASHCOMMAND)MDrv_CMDQ_Printf_Crash_Command;
    pCMDQPri->fpCMDQReceive = (IOCTL_CMDQ_RECEIVE)MDrv_CMDQ_Receive;
    pCMDQPri->fpCMDQSet_Timer_Ratio = (IOCTL_CMDQ_SETTIMERRATIO)MDrv_CMDQ_Set_timer_ratio;
    pCMDQPri->fpCMDQSetDbgLevel = (IOCTL_CMDQ_SETDBGLEVEL)MDrv_CMDQ_SetDbgLevel;

    // 0 is pass for UtopiaOpen
    return 0;
}

MS_U32 CMDQIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    CMDQ_INSTANT_PRIVATE* psCMDQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psCMDQInstPri);

    PCMDQ_GETLIBVER_PARAM pGetLibVerParam = NULL;
    PCMDQ_RECEIVE_PARAM pReceiveParam = NULL;
    PCMDQ_SETTIMERRATIO_PARAM pSetTimerRatioParam = NULL;
    PCMDQ_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;

    MS_U32 u32Ret;

    // no need to get resource, only one instance at one time
    switch(u32Cmd)
    {
        case MDrv_CMD_CMDQ_GetLibVer:
            printf("CMDQIoctl - MDrv_CMD_CMDQ_GetLibVer\n");
            pGetLibVerParam = (PCMDQ_GETLIBVER_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQGetLibVer(pGetLibVerParam->ppVersion);
            return u32Ret;
        case MDrv_CMD_CMDQ_Printf_Crash_Command:
            printf("CMDQIoctl - MDrv_CMD_CMDQ_Printf_Crash_Command\n");
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQPrintf_Crash_Command();
            return u32Ret;
        case MDrv_CMD_CMDQ_Receive:
            printf("CMDQIoctl - MDrv_CMD_CMDQ_Receive\n");
            pReceiveParam = (PCMDQ_RECEIVE_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQReceive(pReceiveParam->New_IP_Head);
            return u32Ret;
        case MDrv_CMD_CMDQ_Set_timer_ratio:
            printf("CMDQIoctl - MDrv_CMD_CMDQ_Set_timer_ratio\n");
            pSetTimerRatioParam = (PCMDQ_SETTIMERRATIO_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQSet_Timer_Ratio(pSetTimerRatioParam->time, pSetTimerRatioParam->ratio);
            return u32Ret;
        case MDrv_CMD_CMDQ_SetDbgLevel:
            printf("CMDQIoctl - MDrv_CMD_CMDQ_SetDbgLevel\n");
            pSetDbgLevelParam = (PCMDQ_SETDBGLEVEL_PARAM)pArgs;
            u32Ret = (MS_U32)psCMDQInstPri->fpCMDQSetDbgLevel(pSetDbgLevelParam->DbgLevel);
            return u32Ret;
        default:
            break;
    };

	return 1; // FIXME: error code(if no this cmd, return fail)
}

MS_U32 CMDQClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // Restore resource pri_shm content
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_CMDQ, &pModule);
    
    if(UtopiaResourceObtain(pModule, CMDQ_POOL_ID_CMDQ0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pCMDQResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pCMDQResPri);
    
    if(*(MS_U32 *)pCMDQResPri == 0)
    {
        printf("\033[35mFunction = %s, Line = %d, [CMDQClose] Strange resource pri_shm content!!\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu
        UtopiaResourceRelease(pResource);
        return 1;
    }
    else
    {
        printf("\033[35mFunction = %s, Line = %d, [Multi-process Safe] Release an Instance!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        *(MS_U32 *)pCMDQResPri = 0;
        UtopiaResourceRelease(pResource);
        return 0;
    }
#endif

    return 0;
}

MS_U32 MDrv_CMDQ_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U32 u32Return = UTOPIA_STATUS_FAIL;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u32Return = UTOPIA_STATUS_SUCCESS; //SUSPEND_OK
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_CMDQ_Init();

			_prev_u16PowerState = u16PowerState;
			u32Return = UTOPIA_STATUS_SUCCESS; //RESUME_OK
		}
		else
		{
			printf("[%s,%5d]It is not suspended yet. We should not resume\n",__FUNCTION__,__LINE__);
			u32Return = UTOPIA_STATUS_FAIL; //SUSPEND_FAILED
		}
	}
	else
	{
		printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u32Return = UTOPIA_STATUS_FAIL;
	}

	return u32Return;
}
