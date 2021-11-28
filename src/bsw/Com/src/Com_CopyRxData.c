

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
 Function name    : Com_CopyRxData
 Syntax           : BufReq_ReturnType Com_CopyRxData( PduIdType PduId, PduInfoType* PduInfoPointer,
                                                      PduLengthType* RxBufferSizePtr )
 Description      : Called once upon reception of each segment. Within this call, the received data is copied to the
                    receive TP buffer. The API should only be called with an SduLength greater 0 if the RxBufferSizePtr
                    returned by the previous API call indicates sufficient receive buffer (SduLength = RxBufferSizePtr).
                    The function must only be called if the connection has been accepted by an initial call to
                    Com_StartOfReception.
 Parameter        : PduId,PduInfoPointer,RxBufferSizePtr(out)
 Return value     : BufReq_ReturnType
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(BufReq_ReturnType, COM_CODE) Com_CopyRxData( PduIdType PduId, const PduInfoType* PduInfoPointer,
                                                  PduLengthType* RxBufferSizePtr )
{

    /* Start: Variable declarations */
    Com_RxIpduConstPtrType  RxIPduConstPtr;
    Com_RxIpduRamPtrType    RxIPduRamPtr;
    VAR(BufReq_ReturnType, AUTOMATIC) return_value;
    /* End: Variable declarations */

    /* Initialize varibale */

    /* Return value with BUFREQ_E_NOT_OK indicates, IPDU is not started or large data pdu flag has not set or
     * DET error has occured */
    return_value = BUFREQ_E_NOT_OK;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Errors COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
    if(Com_Prv_DETCheck_Param_Ptr((PduId >= COM_GET_NUM_RX_IPDU), (PduInfoPointer == NULL_PTR),
                                                                   COMServiceId_CopyRxData))
#endif
    {

        /* If PB variant is selected, then PduId which is passed to this function will be changed to internal Id
         * which is generated through configuration. If PC variant is selected, then no mapping table will be used.
         */

        PduId           = COM_GET_RX_IPDU_ID(PduId);

        RxIPduConstPtr  = COM_GET_RX_IPDU_CONSTDATA(PduId);

        RxIPduRamPtr    = &COM_GET_RXPDURAM_S(PduId);

        /* Check whether RxPdu flag is set, which will be done in Com_StartOfReception */
        if(Com_GetRamValue(RXIPDU,_LARGEDATAINPROG,RxIPduRamPtr->RxFlags))
        {
            /* Check whether the requested length fits the Ipdu */
            /* This check is to protect from RAM corruption when TP recieves more than what was expected */
            if((RxIPduRamPtr->RxTPIPduLength + PduInfoPointer->SduLength) <= RxIPduConstPtr->Size)
            {
                /*Check if the requested length is a non-zero value*/
                if(COM_ZERO != PduInfoPointer->SduLength)
                {
                    /* Now Copy the Data into the IPDU buffer */
                     Com_ByteCopy((RxIPduConstPtr->BuffPtr + RxIPduRamPtr->RxTPIPduLength),
                                   (uint8*)(PduInfoPointer ->SduDataPtr),PduInfoPointer->SduLength);
                     RxIPduRamPtr->RxTPIPduLength += PduInfoPointer->SduLength ;
                }
                /* Update the out parameter with the remaining buffer length */
                *RxBufferSizePtr = RxIPduConstPtr->Size - RxIPduRamPtr->RxTPIPduLength;
                /* Return with BUFREQ_OK indicates Data copied successfully */
                return_value = BUFREQ_OK;

            } /* if((RxIPduRamPtr->RxTPIPduLength + PduInfoPointer->SduLength) <= RxIPduConstPtr->Size) */
            else
            {
                /* If the TP request more than the available RxBuffer size return with BUFREQ_E_OVFL */
                return_value = BUFREQ_E_OVFL;
            }
        }/* if(Com_GetRamValue(RXIPDU,_LARGEDATAINPROG,RxIPduRamPtr->RxFlags)) */
    }

    return return_value;
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

