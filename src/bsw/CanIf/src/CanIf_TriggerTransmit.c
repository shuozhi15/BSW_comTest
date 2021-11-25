


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


#if (CANIF_TRIGGERTRANSMIT_SUPPORT == STD_ON)

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
Std_ReturnType CanIf_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    VAR (Std_ReturnType, AUTOMATIC   ) lRetVal_en = E_NOT_OK;
#if (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;
    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_TRIGGER_TRANSMIT_SID, CANIF_E_UNINIT)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[TxPduId];

    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;     /*TxPduId passed is always valid as it is sent by Canif to Can*/


    if(lTxPduConfig_pst->TxPduTriggerTransmit == TRUE)
    {

         if(NULL_PTR != lTxPduConfig_pst->UserTriggerTransmit)
         {
             lRetVal_en = lTxPduConfig_pst->UserTriggerTransmit(TxPduId, PduInfoPtr);
         }


    }
#endif
    return lRetVal_en;
}

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
