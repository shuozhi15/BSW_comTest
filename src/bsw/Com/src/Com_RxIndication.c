

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/

#if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)
LOCAL_INLINE FUNC(void, COM_CODE) Com_ProcessRxIPduNotification( VAR(PduIdType, AUTOMATIC) RxPduId );
#endif


/**********************************************************************************************************************
 * Global functions (declared in header files )
 *********************************************************************************************************************/

/***************************************************************************************************
 Function name    : Com_RxIndication
 Syntax           : void Com_RxIndication(PduIdType RxPduId,const PduInfoType *PduInfoPtr)
 Description      : Service called by the lower layer after an I-PDU has been received.
 Parameter        : RxPduId,PduInfoPtr
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS VIOLATION IN Com_RxIndication:
   Com_RxIndication function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_RxIndication(VAR(PduIdType, AUTOMATIC) RxPduId,
                                          P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
    /* To Remove Compiler Warning */
    Com_RxIpduConstPtrType RxIPduConstPtr;
    Com_RxIpduRamPtrType   RxIPduRamPtr;
    VAR(uint8, AUTOMATIC)  ConstByteValue_u8;


#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Error COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
    if(Com_Prv_DETCheck_Param_Ptr((RxPduId >= COM_GET_NUM_RX_IPDU), (PduInfoPtr == NULL_PTR),
                                                                    COMServiceId_RxIndication))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        RxPduId             = COM_GET_RX_IPDU_ID(RxPduId);

        RxIPduConstPtr      = COM_GET_RX_IPDU_CONSTDATA(RxPduId);

        RxIPduRamPtr        = &COM_GET_RXPDURAM_S(RxPduId);
        ConstByteValue_u8   = RxIPduConstPtr->rxIPduFields;

        /* Check if the Ipdu group containing this IPDU is started */
        if(Com_CheckRxIPduStatus((PduIdType)RxPduId))
        {
            VAR(boolean, AUTOMATIC) IPduSigProcessing_b;

            IPduSigProcessing_b = Com_GetValue(RXIPDU,_SIGPROC,ConstByteValue_u8);

            SchM_Enter_Com_RxPduBuffer();

            /* The AUTOSAR COM module does not copy or handle additional received data for not configured signals in
            * case the received data length is greater than expected.
            */
            RxIPduRamPtr->RxIPduLength = ((PduInfoPtr->SduLength <= RxIPduConstPtr->Size) ?
                                           (PduInfoPtr->SduLength) : (RxIPduConstPtr->Size));

            /* Set RxIndication flag: RxIndicationFlag */
            Com_SetRamValue(RXIPDU,_INDICATION,RxIPduRamPtr ->RxFlags,COM_TRUE);
            SchM_Exit_Com_RxPduBuffer();

            /* if(RxIPduConstPtr->rxIPduFields.Signal_Processing == COM_IMMEDIATE) */
            if (COM_IMMEDIATE == IPduSigProcessing_b)
            {
                Com_ProcessRxIPdu(RxPduId,PduInfoPtr);
            }
#ifdef COM_RxIPduDeferredProcessing
            else
            {
               SchM_Enter_Com_RxPduBuffer();
               /* Now Copy the received Data into the RxIPDU buffer */
               Com_ByteCopy(RxIPduConstPtr->BuffPtr,(uint8*)(PduInfoPtr ->SduDataPtr),RxIPduRamPtr->RxIPduLength);
               SchM_Exit_Com_RxPduBuffer();
           }
#endif /* #ifdef COM_RxIPduDeferredProcessing */
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**********************************************************************************************************************
 Function name    : Com_ProcessRxIPdu
 Syntax           : void Com_ProcessRxIPdu(PduIdType RxPduId, const uint8 *PduInfoPtr)
 Description      : Service called by the lower layer after an I-PDU has been received.
 Parameter        : RxPduId    - Id of the Rx ipdu
                  : PduInfoPtr - Pointer to the buffer + sdu length where the received data is stored
 Return value     : None
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS VIOLATION IN Com_ProcessRxIPdu:
   Com_ProcessRxIPdu function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_ProcessRxIPdu(
                                VAR(PduIdType, AUTOMATIC)                       RxPduId,
                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr
                                      )
{
    Com_RxIpduConstPtrType   RxIPduConstPtr;
#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)
    VAR(boolean, AUTOMATIC)  isGwRxIpdu;
#endif
#ifdef COM_RxIPduCallOuts
    VAR(boolean, AUTOMATIC)  rxIpduCalloutStatus;
#endif

    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(RxPduId);

#ifdef COM_RxIPduCallOuts

    /* If IPdu callout is configured for the Pdu, Callout is called,
     * if it returns false, no further processing is done */
    if (RxIPduConstPtr->CallOut != NULL_PTR)
    {
        rxIpduCalloutStatus = RxIPduConstPtr->CallOut(RxPduId, PduInfoPtr);
    }
    else
    {
        /* If callout is not configured, proceed further to process Ipdu */
        rxIpduCalloutStatus = COM_TRUE;
    }

    if (COM_TRUE == rxIpduCalloutStatus)
#endif /* #ifdef COM_RxIPduCallOuts */
    {
#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)

        isGwRxIpdu = Com_GetValue(RXIPDU,_ISGWPDU,RxIPduConstPtr->rxIPduFields);

        /* Initialize the flag only if the RxIPdu has gateway signals or signal groups */
        if (isGwRxIpdu)
        {
            COM_GET_IS_GWSIG_UPDATED(RxPduId) = COM_FALSE;
        }
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)*/

        /* Process all the Signals in the IPdu
         * 1. Update bit validation for signals with Update bit configured
         * 2. Timeout monitoring for Signals with Update bit
         * 3. Data Invalid value validation
         * 4. Validation of filters for Signals with filter configured */
        if (RxIPduConstPtr->No_Of_Sig_Ref > 0u)
        {
            Com_Prv_ProcessSignal(RxPduId, PduInfoPtr);
        }

#ifdef COM_RX_SIGNALGROUP

        /* Process all the Signal groups in the IPdu
         * 1. Update bit validation for signal groups with Update bit configured
         * 2. Timeout monitoring for signal groups with Update bit
         * 3. Data Invalid value validation for group signals with Invalid value configured */
        if (RxIPduConstPtr->No_Of_SigGrp_Ref > 0u)
        {
            Com_Prv_ProcessSignalGroup(RxPduId, PduInfoPtr);
        }
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

        /* This part of code updates the Rx Gateway queue If
         * 1. If the RxIPdu has gateway signals or signal groups
         * 2. Any Gw signal in the Ipdu is updated in the signal buffer
         * 3. Already an entry into the queue does not exist
         */
        if ((isGwRxIpdu) && (COM_GET_IS_GWSIG_UPDATED(RxPduId)))
        {
            if (Com_GetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,COM_GET_RXPDURAM_S(RxPduId).RxFlags) != COM_TRUE)
            {
                SchM_Enter_Com_RxSigGwQueueProtArea();
                /* There is a new reception of Ipdu caused a new update on the Signal, Hence set the flag to process in
                 * next Com_MainfunctionRouteSignals
                 * Once the Ipdu is processed, this flag will hold COM_TRUE in Com_MainfunctionRouteSignals
                 * If any new receptions are invoked on the same Ipdu before the next processing is done,
                 * 1. Signal buffers are updated
                 * 2. Queue is not updated */
                (void)Com_WriteSigGwReceiveQueue(RxPduId);
                /* There is a new reception of Ipdu/ Ipdu timeout caused a new update on the Signal, Hence set the flag
                 * to process in next Com_MainfunctionRouteSignals
                 * Once the Ipdu is processed, this flag will hold COM_TRUE in Com_MainfunctionRouteSignals
                 * If any new receptions are invoked on the same Ipdu before the next processing is done,
                 * 1. Signal buffers are updated
                 * 2. Queue is not updated */
                Com_SetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,COM_GET_RXPDURAM_S(RxPduId).RxFlags,COM_TRUE);
                SchM_Exit_Com_RxSigGwQueueProtArea();
            }
        }
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)

        Com_ProcessRxIPduNotification( RxPduId );
#endif
    } /* if (COM_TRUE == rxIpduCalloutStatus) */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)
/**********************************************************************************************************************
 Function name    : Com_ProcessRxIPduNotification
 Syntax           : void Com_ProcessRxIPduNotification( PduIdType RxPduId )
 Description      : Process Notification configured at RxIPdu
 Parameter        : RxIPduConstPtr - Pointer to current RxIPdu configuration
 Return value     : None
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_ProcessRxIPduNotification( VAR(PduIdType, AUTOMATIC) RxPduId )
{
    Com_RxIpduConstPtrType          RxIPduConstPtr;
#ifdef COM_RxSignalNotify
    Com_RxSigConstPtrType           RxSigConstPtr;
    Com_RxSigRamPtrType             RxSigRamPtr;
#endif
#ifdef COM_RxSignalGrpNotify
    Com_RxSigGrpConstPtrType        RxSigGrpConstPtr;
    Com_RxSigGrpRamPtrType          RxSigGrpRamPtr;
#endif
#if defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)
    VAR(uint16_least, AUTOMATIC)    SigOrSigGrpId;
#endif

    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(RxPduId);

#ifdef COM_RxIPduNotification

    /* Ipdu notification is called for the Ipdu */
    if (RxIPduConstPtr->RxNotification_Cbk != NULL_PTR)
    {
        RxIPduConstPtr->RxNotification_Cbk();
    }
#endif

#if defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify)

    /* Check if any signal notification callbacks are configured for the IPdu */
    if (Com_GetValue(RXIPDU,_NOTIFCBK,RxIPduConstPtr->rxIPduFields))
    {
#ifdef COM_RxSignalNotify

        RxSigConstPtr = COM_GET_RXSIG_CONSTDATA(RxIPduConstPtr->RxSigId_Index);
        RxSigRamPtr   = &COM_GET_RXSIGNALFLAG(RxIPduConstPtr->RxSigId_Index);

        /* Signal notifications are called for all the signals with notifications configured */
        for (SigOrSigGrpId = (uint16_least)RxIPduConstPtr->No_Of_Sig_Ref; SigOrSigGrpId != 0u; SigOrSigGrpId--)
        {
            if (RxSigConstPtr->Notification_Cbk != NULL_PTR)
            {
                if (Com_GetRamValue(RXSIG,_SIGNOTIF,RxSigRamPtr->rxSigRAMFields))
                {
                    RxSigConstPtr->Notification_Cbk();
                    Com_SetRamValue(RXSIG,_SIGNOTIF,RxSigRamPtr->rxSigRAMFields,COM_FALSE);
                }
            }
            RxSigConstPtr++;
            RxSigRamPtr++;
        }
#endif /* COM_RxSignalNotify */

#ifdef COM_RxSignalGrpNotify

        RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA(RxIPduConstPtr->FirstRxSigGrp_Index);
        RxSigGrpRamPtr   = &COM_GET_RXSIGGRPFLAG(RxIPduConstPtr->FirstRxSigGrp_Index);

        /* Signal group notifications are called for all the signal groups with notifications configured */
        for (SigOrSigGrpId = (uint16_least)RxIPduConstPtr->No_Of_SigGrp_Ref; SigOrSigGrpId != 0u; SigOrSigGrpId--)
        {
            if (RxSigGrpConstPtr->Notification_Cbk != NULL_PTR)
            {
                if (Com_GetRamValue(RXSIGGRP,_SIGNOTIF,RxSigGrpRamPtr->rxSigGrpRAMFields))
                {
                    RxSigGrpConstPtr->Notification_Cbk();
                    Com_SetRamValue(RXSIGGRP,_SIGNOTIF,RxSigGrpRamPtr->rxSigGrpRAMFields,COM_FALSE);
                }
            }
            RxSigGrpConstPtr++;
            RxSigGrpRamPtr++;
        }
#endif /* #ifdef COM_RxSignalGrpNotify */

    } /* if (Com_GetValue(RXIPDU,_NOTIFCBK,RxIPduConstPtr->rxIPduFields)) */

#endif /* #if defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify) */

}
#endif /* #if defined(COM_RxIPduNotification) || defined(COM_RxSignalNotify) || defined(COM_RxSignalGrpNotify) */


/* FC_VariationPoint_START */
/**********************************************************************************************************************
 Function name    : Com_ReadRxIPduLength
 Syntax           : uint8 Com_ReadRxIPduLength(
                                VAR(PduIdType,AUTOMATIC)                     RxPduId,
                                P2VAR(PduLengthType,AUTOMATIC,COM_APPL_DATA) RxIPduLengthPtr
                                              )
 Description      : Service for Reading the length of the received IPdu
                    The API returns COM_SERVICE_NOT_AVAILABLE in case the RxPduId is not in range or
                    the corresponding IPdu Group is not started and updates the RxIPdu length with Zero.
                    In normal case it returns E_OK by updating the RxIPdu length.
 Parameter        : RxPduId         - ID of the reception IPDU
                    RxIPduLengthPtr - memory location to update the PduLength for the requested RxIPDU
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 *********************************************************************************************************************/
#ifdef COM_ENABLE_READRXIPDULENGTH
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_ReadRxIPduLength(
                                VAR(PduIdType,AUTOMATIC)                     RxPduId,
                                P2VAR(PduLengthType,AUTOMATIC,COM_APPL_DATA) RxIPduLengthPtr
                                           )
{
    VAR(uint8,AUTOMATIC)  Status;

    Status = COM_SERVICE_NOT_AVAILABLE;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr( (RxPduId >= COM_GET_NUM_RX_IPDU), (RxIPduLengthPtr == NULL_PTR),
                                     COMServiceId_ReadRxIPduLength ))
#endif
    {
        VAR(PduLengthType,AUTOMATIC)  RxIPduLength;

        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */

        RxPduId       = COM_GET_RX_IPDU_ID(RxPduId);
        RxIPduLength  = 0u;

        /* Check if Ipdu Group is started */
        if(Com_CheckRxIPduStatus((PduIdType)RxPduId))
        {
            RxIPduLength = COM_GET_RXPDURAM_S(RxPduId).RxIPduLength;
            Status       = E_OK;
        }
        else
        {
            /* This part will be executed when IPDU Group is Stopped (Zero is returned as IPdu length) */
        }

        *RxIPduLengthPtr = RxIPduLength;
    }

    return(Status);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_ENABLE_READRXIPDULENGTH */
/* FC_VariationPoint_END */

