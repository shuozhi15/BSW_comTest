

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_GetUserData
 Description      : This is the AUTOSAR interface for getting the user data out of the most recently
                    received NM PDU. This API shall be called by Nm.
 Parameter        : nmChannelHandle - Identification of the NM-channel
                  : nmUserDataPtr - Pointer where user data out of the most recently received
                    NM PDU shall be copied to.
 Return value     : E_OK     - No error
                  : E_NOT_OK - Getting of user data has failed.
 ***************************************************************************************************
 */

/* TRACE[SWS_CanNm_00268] : CanNm_GetUserData is available only if CANNM_USER_DATA_ENABLED is set to TRUE */
#if(CANNM_USER_DATA_ENABLED != STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetUserData(
                                                     VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                     P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmUserDataPtr
                                                    )
{
    /* Pointer to RAM data */
    P2VAR(CanNm_RamType, AUTOMATIC, CANNM_APPL_DATA) RamPtr_ps;

    /* Pointer to configuration data */
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) ConfigPtr_pcs;

    /* Pointer to receive buffer */
    P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) BufferPtr;

    /* Return value of the API */
    VAR(Std_ReturnType, AUTOMATIC) RetVal_en;

    /* Variable to hold the Network Handle of CanNm */
    VAR(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle;

    /**** End Of Declarations ****/

    /* Receive CanNm channel index from the received ComM channelID */
    CanNm_NetworkHandle = CANNM_GET_HANDLE(nmChannelHandle);

    /* Initialize the return value*/
	RetVal_en = E_NOT_OK;

    /********************************* Start: DET *************************************/

    /* Report DET if network handle is invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_NetworkHandle >= CANNM_NUMBER_OF_CHANNELS),
                            nmChannelHandle, CANNM_SID_GETUSERDATA, CANNM_E_INVALID_CHANNEL)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
                                nmChannelHandle, CANNM_SID_GETUSERDATA, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK((nmUserDataPtr == NULL_PTR),
                            nmChannelHandle, CANNM_SID_GETUSERDATA, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* Initialise the pointer to RAM data structure */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Initialise the pointer to configuration data structure of channel NetworkHandle */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Get the user data only if NM message is received */
    if (RamPtr_ps->RxStatus_b != FALSE)
    {
        /* Get the address where user bytes are stored*/
        BufferPtr = &(RamPtr_ps->RxBuffer_au8[ConfigPtr_pcs->PduLength_u8 - ConfigPtr_pcs->UserDataLength_u8]);

        /* Suspend interrupts to provide Data consistency */
        SchM_Enter_CanNm_GetUserDataNoNest();

        /* TRACE[SWS_CanNm_00160] : Copy the user data of most received NMPDU into passed pointer */
        /* This is an internal function with predictable run-time; hence doesn't affect interrupt lock */
        CanNm_CopyBuffer(BufferPtr,nmUserDataPtr,ConfigPtr_pcs->UserDataLength_u8);

        /* Enable interrupts */
        SchM_Exit_CanNm_GetUserDataNoNest();

        /* Return successful operation */
        RetVal_en = E_OK;
    }

        /* API is rejected as there is no NM message received during this NM Sleep-Cycle */
        return RetVal_en;

}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


