

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
 Function name    : Com_CopyTxData
 Syntax           : BufReq_ReturnType Com_CopyTxData( PduIdType PduId, PduInfoType* PduInfoPtr,
                                                      RetryInfoType* RetryInfoPtr, PduLengthType* TxDataCntPtr )
 Description      : At invocation of Com_CopyTxData the AUTOSAR COM module copies the re-quested transmit data of the
                    large IPDU with ID PduId from its internal transmit buffer to the location specified by the
                    PduInfoPtr.
                    The function Com_CopyTxData also calculates and sets the TxDataCntPtr to the amount of remaining
                    bytes for the transmission of this large I-PDU. When TpDataState is TP_DATARETRY,
                    Com_CopyTxData copies previously transmitted data again.
 Parameter        : PduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr(out)
 Return value     : BufReq_ReturnType
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(RetryInfoPtr) in a function prototype should be declared as pointer
 to const if the pointer is not used to modify the addressed object. But AUTOSAR SWS specification signature is as
 below, Hence suppressed */
FUNC(BufReq_ReturnType, COM_CODE) Com_CopyTxData(PduIdType PduId, const PduInfoType* PduInfoPtr,
                                                 RetryInfoType* RetryInfoPtr, PduLengthType* TxDataCntPtr)
{

    Com_TxIpduConstPtrType TxIPduConstPtr;      /* Local pointer to the Tx ipdu static configuration */
    Com_TxIpduRamPtrType   TxIpduRamPtr;        /* Local pointer to the Tx ipdu status structure */
    VAR(BufReq_ReturnType, AUTOMATIC) return_value;

    /* Initialize variable */

    /* Return value with BUFREQ_E_NOT_OK indicates, IPDU is not started or large data pdu flag has not set or
     * DET error has occured*/
    return_value = BUFREQ_E_NOT_OK;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if(Com_Prv_DETCheck_Param_Ptr((PduId >= COM_GET_NUM_TX_IPDU), (PduInfoPtr == NULL_PTR), COMServiceId_CopyTxData))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */

        PduId = COM_GET_TX_IPDU_ID(PduId);

        TxIpduRamPtr = &COM_GET_TXPDURAM_S(PduId);

        /* Currently Retransmission feature is not supported */
        (void)RetryInfoPtr;

        /* Check whether the large data pdu flag has been set */
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags))
        {
            /* Ensure that this is not query request */
            if(PduInfoPtr->SduLength != 0u)
            {
                /* Requested length should be less than the PduLength */
                if((PduInfoPtr->SduLength + TxIpduRamPtr->TxTPIPduLength) <= COM_GET_TPTXIPDULENGTH_AUO(PduId))
                {

                    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(PduId);

                    /* Now Copy the Data from the IPDU buffer to SduDataPointer */
                    Com_ByteCopy((uint8*)(PduInfoPtr ->SduDataPtr),
                                         (TxIPduConstPtr->BuffPtr + TxIpduRamPtr->TxTPIPduLength),
                                           PduInfoPtr->SduLength);

                    /* Keep track of how many bytes of data has been copied into the lower layer buffer,
                     * which is used as an offset for Ipdu buffer */
                    TxIpduRamPtr->TxTPIPduLength += PduInfoPtr->SduLength;

                    /* Update the out parameter TxDataCntPtr, with the number of bytes remaining */
                    *TxDataCntPtr = COM_GET_TPTXIPDULENGTH_AUO(PduId) - TxIpduRamPtr->TxTPIPduLength;
                    /* Positive condition hence status as BUFREQ_OK */
                    return_value = BUFREQ_OK;
                }
            }
            else
            {
                /* Update the out parameter TxDataCntPtr, with the number of bytes remaining */
                *TxDataCntPtr = COM_GET_TPTXIPDULENGTH_AUO(PduId) - TxIpduRamPtr->TxTPIPduLength;
                /* Positive condition hence status as BUFREQ_OK */
                return_value = BUFREQ_OK;
            }
        }
    }
    return return_value;
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

