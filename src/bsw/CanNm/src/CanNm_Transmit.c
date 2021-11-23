

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"
#include "CanIf.h"

#if (!defined(CANIF_AR_RELEASE_MAJOR_VERSION) || (CANIF_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANIF_AR_RELEASE_MINOR_VERSION) || (CANIF_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_Transmit
 Description      : Used by PduR to trigger a spontaneous transmission of an NM PDU with the provided
                    NM User Data
 Parameter        : CanNmTxPduId - Handle of the CanNm user data PDU
                  : PduInfoPtr   - Pointer to a structure containing the pointer to the NM user data buffer
                                   and the related DLC.
 Return value     : E_OK     - Transmit request has been accepted
                  : E_NOT_OK - Transmit request has not been accepted (CanNm is not in Repeat Message or
                               Normal Operation State)
 ***************************************************************************************************/

/* TRACE[SWS_CanNm_00330] : This API shall be provided if CanNmComUserDataSupport or CanNmPnEnabled is enabled */
#if((CANNM_COM_USER_DATA_SUPPORT != STD_OFF) || (CANNM_PN_ENABLED != STD_OFF))

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

FUNC(Std_ReturnType, CANNM_CODE) CanNm_Transmit(
                                              PduIdType CanNmTxPduId,
                                              P2CONST(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr
                                             )
{
    /* pointer to configuration data */
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST)  ConfigPtr_pcs;

    /* Pointer to RAM data */
    P2VAR(CanNm_RamType, AUTOMATIC, CANNM_APPL_DATA) RamPtr_ps;

    /* Return value of the API */
    VAR(Std_ReturnType, AUTOMATIC) RetVal_en;

    /* Initialize the return value*/
    RetVal_en = E_NOT_OK;

    /********************************* Start: DET *************************************/
    /* Report DET if TxPduId is Invalid */
    CANNM_DET_REPORT_ERROR_NOK((CanNmTxPduId >= CANNM_NUMBER_OF_CHANNELS),
                            (uint8)CanNmTxPduId, CANNM_SID_TRANSMIT, CANNM_E_INVALID_PDUID)

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR_NOK((CanNm_RamData_s[CanNmTxPduId].State_en == NM_STATE_UNINIT),
                            (uint8)CanNmTxPduId, CANNM_SID_TRANSMIT, CANNM_E_NO_INIT)

    /* Report DET if received pointer is a null pointer*/
    CANNM_DET_REPORT_ERROR_NOK(((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR)),
            (uint8)CanNmTxPduId, CANNM_SID_TRANSMIT, CANNM_E_PARAM_POINTER)

    /*********************************  End: DET  *************************************/

    /* Initialize pointer to configuration structure */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNmTxPduId);

    /* Initialize pointer to RAM structure */
    RamPtr_ps = &CanNm_RamData_s[CanNmTxPduId];

#if(CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
	/* TRACE[SWS_CanNm_00333] : Additional transmission of NM PDU with provided user data */
    if ((RamPtr_ps->State_en == NM_STATE_REPEAT_MESSAGE) || (RamPtr_ps->State_en == NM_STATE_NORMAL_OPERATION))
    {

        RetVal_en = CanIf_Transmit(ConfigPtr_pcs->TxPduId, PduInfoPtr);

        if(RetVal_en == E_OK)
        {
          /* Start Message timeout time only if message is accepted */
          CanNm_StartTimer(RamPtr_ps->PrevMsgTimeoutTimestamp);

          /* Set TxTimeoutMonitoringActive_b flag for monitoring message timeout time */
          RamPtr_ps->TxTimeoutMonitoringActive_b = TRUE;

        }

    }
#endif

    /* TRACE[SWS_CanNm_00333] : CanNm is in Ready Sleep State or Bus Sleep Mode or Prepare Bus Sleep Mode */
    return RetVal_en;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


