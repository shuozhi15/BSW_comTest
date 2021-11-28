

/*
**********************************************************************************************************************
* Includes
**********************************************************************************************************************
*/

#include "CanIf_Prv.h"

/**********************************************************************************************************************
**                                    Function Definition                                                            **
**********************************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_ReadRxPduData                                                                           **
** Syntax            : Std_ReturnType CanIf_ReadRxPduData(PduIdType CanIfRxSduId, PduInfoType* CanIfRxInfoPtr)       **
** Service ID[hex]   : 0x06                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Non Reentrant                                                                                 **
** Parameters (in)   : CanIfRxSduId                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : CanIfRxInfoPtr                                                                                **
** Return value      : Std_ReturnType                                                                                **
** Description       : This service provides the CAN DLC and the received data of the requested CanIfRxSduId to the  **
**                     calling upper layer.                                                                          **
**********************************************************************************************************************/

#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON && CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(Std_ReturnType, CANIF_CODE) CanIf_ReadRxPduData(VAR(PduIdType, AUTOMATIC) CanIfRxSduId,
        P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_CONST) CanIfRxInfoPtr)
{
    /* Pointer to HRH configuration */
    P2CONST(CanIf_Cfg_Hrhtype_tst, AUTOMATIC, CANIF_CFG_CONST) HrhCfg_pcst;

    /* Pointer to Rx Pdu configuration */
    P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduCfg_pcst;

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) ControllerState_ps;

    /* Return Value of the API */
    VAR(uint8_least, AUTOMATIC) Status;

    /*To store the controller state (Device mode)*/
    VAR (CanIf_ControllerModeType, AUTOMATIC) DeviceMode_Temp;

    /*To store the Pdu mode*/
    VAR (CanIf_PduModeType, AUTOMATIC) ChannelMode_Temp;

    /* [SWS_CANIF_00661] If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_READ_RX_PDU_DATA_SID, CANIF_E_UNINIT)

    /* Initialization */
    Status = E_NOT_OK;


    /* [SWS_CANIF_00325] If CanRxPduId is invalid, report to DET and return E_NOT_OK
     * first check of the pduid obtained is correct then check if the pdu can be buffered*/
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIfRxSduId>= CANIF_CFG_NUM_CANRXPDUIDS), \
                                                                CANIF_READ_RX_PDU_DATA_SID, CANIF_E_INVALID_RXPDUID)
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxSduId]== INVALID_ID), \
                                                                   CANIF_READ_RX_PDU_DATA_SID, CANIF_E_INVALID_RXPDUID)

    /* Get the pointer to desired Rx pdu */
    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxSduId];

    CANIF_DET_REPORT_ERROR_NOT_OK(((RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NOTIFSTATUS)  || \
                                (RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NONE)), \
                                            CANIF_READ_RX_PDU_DATA_SID, CANIF_E_INVALID_RXPDUID)

    /* [SWS_CANIF_00326] If the PduInfoPtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == CanIfRxInfoPtr), CANIF_READ_RX_PDU_DATA_SID, CANIF_E_PARAM_POINTER)

    /* Initialize pointer to HRH configuration structure */
    HrhCfg_pcst = (CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst) + RxPduCfg_pcst->Hrhref_t;

    /*Enter critical section*/
    SchM_Enter_CanIf_ControllerState();

    /* Initialize pointer to Controller State */
    ControllerState_ps = CanIf_Prv_ControllerState_ast + (HrhCfg_pcst->ControllerId_u8);

    /*Make a local copy of Device mode and Channel mode*/
    DeviceMode_Temp = ControllerState_ps->DeviceMode;
    ChannelMode_Temp = ControllerState_ps->ChannelMode;

    /*Leave critical section*/
    SchM_Exit_CanIf_ControllerState();

    /* [SWS_CANIF_00324] Check whether the data can be read, the controller mode and the channel mode */
    if((CANIF_CS_STARTED == DeviceMode_Temp) && ((CANIF_ONLINE == ChannelMode_Temp) ||
            (CANIF_TX_OFFLINE == ChannelMode_Temp)))
    {
        /* Read the data from the Rx buffer */
        CanIf_Prv_ReadRxBuffer(CanIfRxInfoPtr, RxPduCfg_pcst);

        /*Request for L-SDU data has been accepted */
        Status = E_OK;
    }

    return((Std_ReturnType)Status);
}  /* End of CanIf_ReadRxPduData() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif


