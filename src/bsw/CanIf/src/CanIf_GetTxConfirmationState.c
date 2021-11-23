


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"


/***********************************************************************************************************************
 * Function name    : CanIf_GetTxConfirmationState
 * Syntax           : FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_GetTxConfirmationState(
                                                                        VAR(uint8,AUTOMATIC) ControllerId
                                                                                    )
 * Description      : This service reports, if any TX confirmation has been done for the whole CAN controller
                      since the last CAN controller start.
 * Parameter        : ControllerId
 * Return value     : CANIF_NO_NOTIFICATION / CANIF_TX_RX_NOTIFICATION
 **********************************************************************************************************************/

#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_GetTxConfirmationState(
                                       VAR(uint8,AUTOMATIC) ControllerId
                                               )
{
    VAR(CanIf_NotifStatusType, AUTOMATIC) lTxConfmStatus;
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;
#endif
    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_TX_CONFIRMATION_STATE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_GET_TX_CONFIRMATION_STATE_SID, \
                                                                                        CANIF_E_PARAM_CONTROLLERID)
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((lCtrlCustId_u8 == 0xFFu), CANIF_GET_TX_CONFIRMATION_STATE_SID, \
                                                                                        CANIF_E_PARAM_CONTROLLERID)
    /*Read TxConfirmationState of the requested controller*/
    lTxConfmStatus = (CanIf_Prv_ControllerState_ast + ControllerId)->CanIf_TxCnfmStatus;


    return(lTxConfmStatus);

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


