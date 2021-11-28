

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
 Function name    : Com_StartOfReception
 Syntax           : BufReq_ReturnType Com_StartOfReception( PduIdType ComRxPduId, PduInfoType* TpSduInfoPtr,
                    PduLengthType TpSduLength, PduLengthType* RxBufferSizePtr )
 Description      : Com_StartOfReception initiates reception of the TP I-PDU with ID ComRxPduId.
                    It returns the size of the internal receive buffer for the I-PDU via RxBufferSizePtr.
 Parameter        : ComRxPduId,TpSduInfoPtr,TpSduLength,RxBufferSizePtr(out)
 Return value     : BufReq_ReturnType
 ***************************************************************************************************/

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(BufReq_ReturnType, COM_CODE) Com_StartOfReception( PduIdType ComRxPduId, const PduInfoType* TpSduInfoPtr,
                                                        PduLengthType TpSduLength, PduLengthType* RxBufferSizePtr )
{
    /* Start: Variable declarations */
    Com_RxIpduConstPtrType RxIPduConstPtr;
    Com_RxIpduRamPtrType   RxIPduRamPtr;
    VAR(BufReq_ReturnType, AUTOMATIC) return_value;
    /* End: Variable declarations */

    /* Initialize variable */
    /* Return with BUFREQ_E_NOT_OK if DET occurs or Ipdu group containing this IPDU is not started */
    return_value = BUFREQ_E_NOT_OK;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Errors COM_E_UNINIT and COM_E_PARAM */
    if(Com_Prv_DETCheck_Param((ComRxPduId >= COM_GET_NUM_RX_IPDU), COMServiceId_StartOfReception))
#endif
    {

        /* If PB variant is selected, then PduId which is passed to this function will be changed
        *  to internal Id which is generated through configuration
        *  If PC variant is selected, then no mapping table will be used. */
        ComRxPduId     = COM_GET_RX_IPDU_ID(ComRxPduId);

        RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(ComRxPduId);

        RxIPduRamPtr   = &COM_GET_RXPDURAM_S(ComRxPduId);

        /*Currently this parameter is not used*/
        (void)TpSduInfoPtr;

        /* Check if the Ipdu group containing this IPDU is started */
        if(Com_CheckRxIPduStatus((PduIdType)ComRxPduId))
        {
            /* Check if the reception requests fits Ipdu size,
            * else return overflow as we cannot accomodate this */
            if(TpSduLength <= RxIPduConstPtr->Size)
            {
                /* As all the checks are passed, then we accept this reception and
                 * set flag which will be monitored in CopyRxData calls */
                Com_SetRamValue(RXIPDU,_LARGEDATAINPROG,RxIPduRamPtr ->RxFlags,COM_TRUE);
                /* Initialise the received length RAM variable to zero here,
                 * This length variable will be incremented in the Com_CopyRxData API */
                RxIPduRamPtr->RxTPIPduLength = 0;

                /* Update the TP with Ipdu size for buffer size out parameter */
                *RxBufferSizePtr = RxIPduConstPtr->Size;

                /* Positive condition hence status as BUFREQ_OK */
                return_value = BUFREQ_OK;
            }
            else
            {
                /* return with BUFREQ_E_OVFL */
                return_value = BUFREQ_E_OVFL;
            }
        }
    }

    return return_value;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

