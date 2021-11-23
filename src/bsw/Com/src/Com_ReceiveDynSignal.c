

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_ReceiveDynSignal
 Syntax           : uint8 Com_ReceiveDynSignal(Com_SignalIdType SignalId,void* SignalDataPtr,uint16 *LengthPtr)
 Description      : Service for receiving the signal, This API copies the data to the application buffer
 Parameter        : SignalId         -> Id of the Signal
                  : SignalDataPtr    -> Pointer to the address where signal data needs to be copied.
                  : LengthPtr        -> Pointer to length of the recieved length.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 ***************************************************************************************************/
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_ReceiveDynSignal(        VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                              P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                            P2VAR(uint16, AUTOMATIC, COM_APPL_DATA) LengthPtr      )
{
    Com_RxSigConstPtrType RxSigConstPtr; /* Local pointer holds the address of Rx signal static configuration */
    PduLengthType         StartByteNo;   /* Start Byte Number of Dynamic Signal */
    VAR(uint16,AUTOMATIC) DynLength;     /* Received Dynamic Signal Length  */
    VAR(Com_SigBuffIndexType, AUTOMATIC) SigBuff_Index;
    VAR(uint8, AUTOMATIC) Type;          /* Local variable holds the signal type */
    VAR(uint8, AUTOMATIC) Status;        /* Return status */


    /* Initialize variable */
    /* Returns with COM_SERVICE_NOT_AVAILABLE when IPDU Group is Stopped or
     * DET error occurs or when Type of the signal is not configured with UINT8_DYN */
    Status = COM_SERVICE_NOT_AVAILABLE;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Errors COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
    if(Com_Prv_DETCheck_Param_Ptr((SignalId >= COM_GET_NUM_RX_SIGNALS),
                                  ((SignalDataPtr == NULL_PTR) || (LengthPtr == NULL_PTR)),
                                   COMServiceId_ReceiveDynSignal))
#endif
    {

        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        SignalId = COM_GET_RXSIGNAL_ID(SignalId);

        RxSigConstPtr = COM_GET_RXSIG_CONSTDATA(SignalId);
        SigBuff_Index = RxSigConstPtr->SigBuff_Index;
        Type = Com_GetValue(GEN,_TYPE,RxSigConstPtr->General);

        /*Check if Ipdu Group is started*/
        if(Com_CheckRxIPduStatus((PduIdType)RxSigConstPtr->IpduRef) && (COM_UINT8_DYN == Type))
        {
            /* Calculate Rx Dynamic Signal Start Position */
            StartByteNo = ( RxSigConstPtr->Bit_Pos >> 3u );

            /* Check if Dynamic Signal has been received with atleast one byte */
            if ( COM_GET_RXPDURAM_S(RxSigConstPtr->IpduRef).RxIPduLength > StartByteNo )
            {
                /* Calculate its received length */
                DynLength = (uint16)( COM_GET_RXPDURAM_S(RxSigConstPtr->IpduRef).RxIPduLength - StartByteNo );
            }
            else
            {
                /* Report it as 0, if it is not received yet */
                DynLength = 0u;
            }

            /* If the requested length is less than the received length, then return E_NOT_OK */
            if(*LengthPtr >= DynLength)
            {

                /*Copy each byte into the SignalDataPtr from Signal buffer  */
                /* The locks inside the function Com_ByteCopy() are removed.
                 * Hence locks are used here
                 */
                SchM_Enter_Com_RxSigDynBuff();
                * LengthPtr = DynLength;
                if(0u != DynLength)
                {
                    VAR(Com_MainFuncType,AUTOMATIC) rxMainFuncId;

                    /* Fetch the Rx-MainFunction internal Id */
                    rxMainFuncId = (COM_GET_RX_IPDU_CONSTDATA(RxSigConstPtr->IpduRef))->MainFunctionRef;

                    /* MR12 RULE 11.5 VIOLATION: For SignalDataPtr is defined as void pointer in the AUTOSAR
                     * specification, so suppressing warning "Cast from a pointer to void to a pointer to object" */
                    Com_ByteCopy((uint8*)SignalDataPtr,
                                  &COM_GET_SIGTYPEUINT8DYN_BUFF(rxMainFuncId,SigBuff_Index),DynLength);
                }
                SchM_Exit_Com_RxSigDynBuff();

                /* Positive condition hence status as E_OK */
                Status = E_OK;
            }
            else
            {
                Status = E_NOT_OK;
            }
        }
    }
    return(Status);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif

