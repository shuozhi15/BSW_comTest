

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"

/*
 ***************************************************************************************************
 * used functions
 ***************************************************************************************************
 */
/**************************************************************************************************************/
/* Function name    : CanIf_ReadTx_NotifStatus                                                                */
/* Syntax           : FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadTx_NotifStatus(
                                                                                VAR(PduIdType,AUTOMATIC) CanIfTxSduId
                                                                                      )                       */
/* Description      : This service provides the status of the CAN Tx L-PDU requested by CanTxPduId. This API
                      service notifies the upper layer about any transmit confirmation or receive indication
                      event to the corresponding requested CAN LPDU.                                          */
/* Parameter        : CanTxPduId                                                                              */
/* Return value     : CANIF_TX_RX_NOTIFICATION / CANIF_NO_NOTIFICATION                                        */
/**************************************************************************************************************/

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF && CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadTxNotifStatus(
                                                                  VAR(PduIdType,AUTOMATIC) CanIfTxSduId
                                                               )
{


    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;

    P2VAR(CanIf_NotifStatusType, AUTOMATIC,AUTOMATIC) lTxNotifPtr;

    /* Store the Notification Status of each CanTxPduId  */
    VAR(CanIf_NotifStatusType, AUTOMATIC) NotifStatus = CANIF_NO_NOTIFICATION;
    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;


    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((FALSE == CanIf_Prv_InitStatus_b), CANIF_RB_READ_TXPDU_CANID, CANIF_E_UNINIT)

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((CanIfTxSduId >= CANIF_TOTAL_TXPDUS), \
                                                        CANIF_READ_TX_NOTIF_STATUS_SID, CANIF_E_INVALID_TXPDUID)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanIfTxSduId];

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((ltxPduCustId_t == 0xFFFFu), CANIF_READ_TX_NOTIF_STATUS_SID, CANIF_E_INVALID_TXPDUID)

    /*Get the address of config structure of requested PDU*/
    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;



    lTxNotifPtr = (CanIf_Prv_TxNotification_aen + ltxPduCustId_t);

    /* Check if the Notification status can be read */
    if( FALSE != lTxPduConfig_pst->TxPduReadNotifyStatus )
    {
        /* Enter Critical Section */

        SchM_Enter_CanIf_TxRxNotifNoNest();

        /* Copy the status from the global variable into a local variable */
        NotifStatus = (CanIf_NotifStatusType)(*lTxNotifPtr);

        /* Reset the global variable */
        *(lTxNotifPtr) = CANIF_NO_NOTIFICATION;

        /* Leave Critical Section */
        SchM_Exit_CanIf_TxRxNotifNoNest();

    }

    return(NotifStatus);
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


/**********************************************************************************************************************
** used functions                                                                                                    **
**********************************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_ReadRxNotifStatus                                                                       **
** Syntax            : CanIf_NotifStatusType CanIf_ReadRxNotifStatus(PduIdType CanIfRxSduId)                         **
** Service ID[hex]   : 0x08                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : CanIfRxSduId                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : CanIf_NotifStatusType                                                                         **
** Description       : This service returns the indication status (indication occurred or not) of a specific CAN     **
**                     Rx L-PDU, requested by the CanIfRxSduId.                                                      **
**********************************************************************************************************************/
#if (CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API != STD_OFF && CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadRxNotifStatus(VAR(PduIdType, AUTOMATIC) CanIfRxSduId)
{
    /* Pointer to Rx Pdu configuration */
    P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduCfg_pcst;

    /* pointer to global variable CanIf_RxNotification_0 */
    P2VAR(CanIf_NotifStatusType, AUTOMATIC,AUTOMATIC) RxNotifStatus_pen;

    /* Store the Notification Status of each CanRxPduId  */
    VAR(CanIf_NotifStatusType, AUTOMATIC) NotifStatus_ten = CANIF_NO_NOTIFICATION;

    /* If CAN Interface is uninitialized, report to DET and return CANIF_NO_NOTIFICATION */
    CANIF_DET_REPORT_ERROR_NO_NOTIF((FALSE == CanIf_Prv_InitStatus_b), CANIF_READ_RX_NOTIF_STATUS_SID,CANIF_E_UNINIT)



    /* [SWS_CANIF_00336] If CanRxPduId is invalid, report to DET and return E_NOT_OK
     * first check of the pduid obtained is correct then check if the pdu can be buffered*/
    CANIF_DET_REPORT_ERROR_NO_NOTIF((CanIfRxSduId>= CANIF_CFG_NUM_CANRXPDUIDS) , \
                                                            CANIF_READ_RX_NOTIF_STATUS_SID, CANIF_E_INVALID_RXPDUID)

    CANIF_DET_REPORT_ERROR_NO_NOTIF((CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxSduId]== INVALID_ID) , \
                                                               CANIF_READ_RX_NOTIF_STATUS_SID, CANIF_E_INVALID_RXPDUID)

    /* Get the pointer to desired Rx pdu */
    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxSduId];

    CANIF_DET_REPORT_ERROR_NO_NOTIF(((RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_DATA) || (RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NONE)), CANIF_READ_RX_NOTIF_STATUS_SID, CANIF_E_INVALID_RXPDUID)

    /* Initialize pointer to HRH configuration structure */
   // HrhCfg_pcst = (CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst) + RxPduCfg_pcst->Hrhref_t;

    /*Initializing global variable for storing notification status */
    RxNotifStatus_pen = CanIf_Prv_RxNotification_taen;

    /* Check if the Notification status can be read
     * If det is not neede then this condition needs to be checked */
    if((RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NOTIFSTATUS_DATA) ||                          \
                                        (RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NOTIFSTATUS))
    {
        /* Enter Critical Section */
        SchM_Enter_CanIf_TxRxNotifNoNest();

        /* Copy the notification status */
        NotifStatus_ten = *(RxNotifStatus_pen + CanIfRxSduId);

        /* Reset the global variable holding the notification status */
        *(RxNotifStatus_pen + CanIfRxSduId) = CANIF_NO_NOTIFICATION;

        /* Leave Critical Section */
        SchM_Exit_CanIf_TxRxNotifNoNest();
    }

    return (NotifStatus_ten);
}/* End of CanIf_ReadRx_NotifStatus() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif
