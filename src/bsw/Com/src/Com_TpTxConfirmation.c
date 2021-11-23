

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

#ifdef COM_TP_IPDUTYPE

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_TpTxConfirmation
 Syntax           : void Com_TpTxConfirmation( PduIdType PduId, Std_ReturnType Result )
 Description      : This function is called by the PduR after a large I-PDU has been transmitted via the transport
                    protocol on its network.
 Parameter        : PduId,Result
 Return value     : Void
 ***************************************************************************************************/

#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(void, COM_CODE) Com_TpTxConfirmation( PduIdType PduId, Std_ReturnType Result )
{

    /* Start: Variable declarations */
    Com_TxIpduRamPtrType TxIPduRamPtr;
    /* End: Variable declarations */

    /* Call DET Function to check for the DET Errors COM_E_UNINIT and COM_E_PARAM */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if(Com_Prv_DETCheck_Param((PduId >= COM_GET_NUM_TX_IPDU), COMServiceId_TpTxConfirmation))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */

        PduId = COM_GET_TX_IPDU_ID(PduId);

        TxIPduRamPtr = &COM_GET_TXPDURAM_S(PduId);

        if(Com_CheckTxIPduStatus((PduIdType)PduId))
        {
            /* Check whether the large data pdu flag has been set */
            if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIPduRamPtr->Com_TxFlags) && (E_OK == Result))
            {
                Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,TxIPduRamPtr->Com_TxFlags,COM_FALSE);

                /* Proceed further to process the TxConfirmation only if the transmission result was successfull  */
                /* Call the internal function Com_InternalProcessTxConfirmation, to process this TxConfirmation */
                Com_InternalProcessTxConfirmation(PduId);
            }
        }
    }
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

