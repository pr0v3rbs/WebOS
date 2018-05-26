/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 


/*****************************************************************************
**
**  Name: ademod_common_demod.c
**
**  Description:   Demodulator software interface.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_Demod_Open
**                  LX_ADEMOD_Result  ADEMOD_Demod_Close
**                  LX_ADEMOD_Result  ADEMOD_Demodulate
**                  LX_ADEMOD_Result  ADEMOD_Register_Set
**                  LX_ADEMOD_Result  ADEMOD_Register_Get
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Set
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Get
**                  LX_ADEMOD_Result  ADEMOD_Firmware_Load
**                  LX_ADEMOD_Result  ADEMOD_AGC_Set
**                  LX_ADEMOD_Result  ADEMOD_AGC_Get
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Set
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Get
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Set
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Get
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Write
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Read
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result  ADEMOD_Demod_Open
**                  LX_ADEMOD_Result  ADEMOD_Demod_Close
**                  LX_ADEMOD_Result  ADEMOD_Demodulate
**                  LX_ADEMOD_Result  ADEMOD_Register_Set
**                  LX_ADEMOD_Result  ADEMOD_Register_Get
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Set
**                  LX_ADEMOD_Result  ADEMOD_Parameter_Get
**                  LX_ADEMOD_Result  ADEMOD_Firmware_Load
**                  LX_ADEMOD_Result  ADEMOD_AGC_Set
**                  LX_ADEMOD_Result  ADEMOD_AGC_Get
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Set
**                  LX_ADEMOD_Result  ADEMOD_ChanScan_Get
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Set
**                  LX_ADEMOD_Result  ADEMOD_SleepMode_Get
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Write
**                  LX_ADEMOD_Result  ADEMOD_HostI2C_Read
**
**  Dependencies:   ademod_common_demod.h 
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "ademod_common.h"
#include "ademod_common_demod.h"
//#include "ademod_common_hostI2C.h"
/*
**  Global demodulator context.
*/

/*
**  Local Function Prototypes - not available for external access.
*/
LX_ADEMOD_Result ADEMOD_GetContext(Handle_t hDevice, LX_ADEMOD_Demod_Instance** ppLX_ADEMOD_instance);

/*
**  API implementation.
*/


/******************************************************************************
**
**  Name: ADEMOD_Demodulate
**
**  Description:    Starts demodulation in specified mode.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  InputMode    - The input mode. Can be one of the following:
**                                 LX_ADEMOD_INPUTMODE_LEGACY,
**                                 LX_ADEMOD_INPUTMODE_DVBT,
**                                 LX_ADEMOD_INPUTMODE_ATSC.
**                  VideoStandard - Legacy video standard (see LX_ADEMOD_VIDEO_STANDARD).
**                                 For digital modes must be set to LX_ADEMOD_VIDEO_STANDARD_NONE.
**                  AudioStandard - Legacy audio standard (see LX_ADEMOD_AUDIO_STANDARD).
**                                 For digital modes must be set to LX_ADEMOD_AUDIO_STANDARD_NONE.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Demodulate(Handle_t hDevice, UINT32 InputMode, UINT32 VideoStandard, UINT32 AudioStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_Demodulate)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_Demodulate(hDevice, InputMode, VideoStandard, AudioStandard );
	

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_Register_Set
**
**  Description:    Sets the value for specified register.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  RegAddr      - The register's address,
**                  RegSize      - The register's size (in bytes).
**                  RegData      - Register's data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Register_Set(Handle_t hDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 RegData)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if (RegSize <= 4) {
		status = ADEMOD_WriteSub(pInstance->hUserData, RegAddr, RegData, RegSize);
	}
	else if (RegSize <= 8) { // long registers
		INT64 LongRegData = (INT64)RegData << (8 * (RegSize - 4));
		status = ADEMOD_WriteLong(pInstance->hUserData, RegAddr, LongRegData, RegSize);
	}
	else { // too long
		status = retUserSizeTooBig;
	}

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_Register_SetX
**
**  Description:    Sets the value for specified register.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  RegAddr      - The register's address,
**                  RegSize      - The register's size (in bytes).
**                  RegData      - Register's data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**  Note:			When called with UpdateNow = FALSE will optimize direct register
**					writes using cache.
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Register_SetX(Handle_t hDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 RegData, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if (RegSize <= 4) {
		status = ADEMOD_WriteSubX(pInstance->hUserData, RegAddr, RegData, RegSize, UpdateNow);
	}
	else if (RegSize <= 8) { // long registers
		INT64 LongRegData = (INT64)RegData << (8 * (RegSize - 4));
		status = ADEMOD_WriteLongX(pInstance->hUserData, RegAddr, LongRegData, RegSize, UpdateNow);
	}
	else { // too long
		status = retUserSizeTooBig;
	}

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_Register_Get
**
**  Description:    Gets the value of specified register.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  RegAddr      - The register's address,
**                  RegSize      - The register's size (in bytes).
**                  pRegData     - Pointer to the register's data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Register_Get(Handle_t hDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if (RegSize <= 4) {
		status = ADEMOD_ReadSub(pInstance->hUserData, RegAddr, pRegData, RegSize);
	}
	else if (RegSize <= 8) { // long registers
		INT64 LongRegData = 0;
		status = ADEMOD_ReadLong(pInstance->hUserData, RegAddr, &LongRegData, RegSize);
		*pRegData = (UINT32)(LongRegData >> (8 * (RegSize - 4)));
	}
	else { // too long
		status = retUserSizeTooBig;
	}

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_Parameter_Set
**
**  Description:    Sets the value for specified parameter.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  ParamID      - Enumerated type for parameter ID (see LX_ADEMOD_PARAMETER),
**                  pParamValue  - Pointer to the parameter's value,
**                  UpdateNow    - When TRUE ?the value of the parameter will be
**                                 propagated into the registers during the function call;
**                                 When FALSE ?the value of the parameter will be
**                                 internally stored until any other function is called
**                                 with UpdateNow parameter equal TRUE or ADEMOD_Demodulate
**                                 function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Parameter_Set(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;
	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_Parameter_Set)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_Parameter_Set(hDevice, ParamID, pParamValue, UpdateNow );
	

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_Parameter_Get
**
**  Description:    Gets the value for specified parameter.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  ParamID      - Enumerated type for parameter ID (see LX_ADEMOD_PARAMETER),
**                  pParamValue  - Pointer to the parameter's value,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Parameter_Get(Handle_t hDevice, UINT32 ParamID, UINT32* pParamValue)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_Parameter_Get)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_Parameter_Get(hDevice, ParamID, pParamValue );


    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_Firmware_Load
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pTable       - Pointer to the table containing firmware microcode,
**                  Size         - The size of the firmware microcode in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_Firmware_Load(Handle_t hDevice, UINT8* pTable, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	status = retErrBadDevice;  
	// FW download is not supported by M14

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_AGC_Set
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the AGC descriptor (see ADEMOD_AGC_Descriptor).
**                  UpdateNow    - When TRUE ?all the affected registers will be updated
**                                 during the function call;
**                                 When FALSE ?only internal context variables will be affected
**                                 during the function call;
**                                 The hardware registers will not be updated until
**                                 any other function is called with UpdateNow parameter equal TRUE
**                                 or ADEMOD_Demodulate function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_AGC_Set(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_AGC_Set)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_AGC_Set(hDevice, pDescriptor, UpdateNow );

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_AGC_Get
**
**  Description:    Initiates the demodulator's firmware download.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the AGC descriptor (see ADEMOD_AGC_Descriptor).
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_AGC_Get(Handle_t hDevice, UINT32* pDescriptor)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_AGC_Get)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_AGC_Get(hDevice, pDescriptor );

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_ChanScan_Set
**
**  Description:    Sets the channel scan profile for Legacy input mode.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
**                  UpdateNow    - When TRUE ?all the affected registers will be updated
**                                 during the function call;
**                                 When FALSE ?only internal context variables will be affected
**                                 during the function call;
**                                 The hardware registers will not be updated until
**                                 any other function is called with UpdateNow parameter equal TRUE
**                                 or ADEMOD_Demodulate function is called.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_ChanScan_Set(Handle_t hDevice, UINT32* pDescriptor, BOOL UpdateNow)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_ChanScan_Set)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_ChanScan_Set(hDevice, pDescriptor, UpdateNow );

	return status;
}


/******************************************************************************
**
**  Name: ADEMOD_ChanScan_Get
**
**  Description:    Returns the channel scan status for Legacy input mode.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pDescriptor  - Pointer to the Legacy channel scan descriptor.
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_ChanScan_Get(Handle_t hDevice, UINT32* pDescriptor)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}
   // TBD
    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_SleepMode_Set
**
**  Description:    Toggles the sleep mode on and off.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  Mode         - The sleep mode.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_SleepMode_Set(Handle_t hDevice, UINT32 Mode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_SleepMode_Set)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_SleepMode_Set(hDevice, Mode );

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_SleepMode_Get
**
**  Description:    Returns the sleep mode status.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  pMode        - Pointer to the sleep mode.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_SleepMode_Get(Handle_t hDevice, UINT32* pMode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_SleepMode_Get)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_SleepMode_Get(hDevice, pMode );

    return status;
}


/******************************************************************************
**
**  Name: ADEMOD_HostI2C_Write
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  pData        - Pointer to data to be written,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_HostI2C_Write(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_HostI2C_Write)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_HostI2C_Write(hDevice, DevAddr, RegAddr, pData, Size );

	return status;
}


/******************************************************************************
**
**  Name: ADEMOD_HostI2C_Read
**
**  Description:    Writes data to host I2C.
**
**  Parameters:     hDevice      - Device Handle returned by ADEMOD_Demod_Open,
**                  DevAddr      - Target device address,
**                  RegAddr      - Target register address,
**                  pData        - Pointer to data to be read,
**                  Size         - Size in bytes.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
LX_ADEMOD_Result ADEMOD_HostI2C_Read(Handle_t hDevice, UINT8 DevAddr, UINT8 RegAddr, UINT8* pData, UINT32 Size)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if ((status = ADEMOD_GetContext(hDevice, &pInstance)) != retOK) {
		return status;
	}

	if(NULL == g_demod_hal.ADEMOD_HostI2C_Read)
	{
		DEMOD_PRINT("^y^[[DEMODDEV]] %s() Not supported (but OK)\n", __F__);
		return retOK;
	}
	
	status = g_demod_hal.ADEMOD_HostI2C_Read(hDevice, DevAddr, RegAddr, pData, Size );

	return status;
}


LX_ADEMOD_Result ADEMOD_GetContext(Handle_t hDevice, LX_ADEMOD_Demod_Instance** ppLX_ADEMOD_instance) //LGIT 091010
{
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	if (!hDevice) {
		return retErrBadHandle;
	}

	pInstance = (pLX_ADEMOD_Demod_Instance)hDevice;

	*ppLX_ADEMOD_instance = pInstance;

    return retOK;
}

