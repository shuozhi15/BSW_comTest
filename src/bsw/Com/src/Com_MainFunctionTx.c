

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
#if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE)
#include "PduR_Com.h"
/* [SWS_BSW_00036] Inter Module Checks */
#if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != COM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != COM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
#ifdef COM_TxIPduTimeOut
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_IPduTimeOut (VAR(uint16_least, AUTOMATIC) TxPduId);
#endif

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_Modes       (VAR(uint16_least, AUTOMATIC) TxPduId);

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_DirectMode  (VAR(uint16_least, AUTOMATIC) TxPduId);

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_MixedMode   (VAR(uint16_least, AUTOMATIC) TxPduId);

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_NoneMode    (VAR(uint16_least, AUTOMATIC) TxPduId);

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_PeriodicMode(VAR(uint16_least, AUTOMATIC) TxPduId);

LOCAL_INLINE FUNC(void, COM_CODE)    Com_Prv_MainFunctionTx_SendIPdu    (VAR(uint16_least, AUTOMATIC) TxPduId,
                                                                         VAR(boolean, AUTOMATIC) SendIPdu);

/***************************************************************************************************
 Function name    : Com_InternalMainFunctionTx
 Syntax           : void Com_InternalMainFunctionTx( Com_MainFuncType TxMainFuncId )
 Description      : Service to perform the processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : TxMainFuncId - Internal Tx-ComMainFunction Id
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS, LEVEL VIOLATION IN Com_InternalMainFunctionTx:
   Com_InternalMainFunctionTx function contains necessary function calls and simple "if" and "else if" statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_InternalMainFunctionTx( VAR(Com_MainFuncType,AUTOMATIC) TxMainFuncId )
{
#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
    /* Local pointer to hold tha address of the Tx ipdu static configuration */
    Com_TxIpduConstPtrType                  TxIPduConstPtr;
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
#endif
    VAR(uint16_least, AUTOMATIC)            Idx_Pdu_ui;
    VAR(Com_IpduIdType,AUTOMATIC)           StartIPduId;    /* Start TxIPduId to be processed */
    VAR(Com_IpduIdType,AUTOMATIC)           EndIPduId;      /* Last  TxIPduId to be processed */
    VAR(boolean, AUTOMATIC)                 SendIPdu;

#ifdef COM_TxIPduTimeOut
    VAR(boolean, AUTOMATIC)                 Timeout_Flag;
#endif /* #ifdef COM_TxIPduTimeOutNotify */

#ifdef COM_TxIPduNotification
    VAR(boolean, AUTOMATIC)                 Notify_Flag;
#endif /* #ifdef COM_TxIPduNotification */

#ifdef COM_TxIPduTimeOut
    /* Initializing Timeout_Flag */
    Timeout_Flag = COM_FALSE;
#endif /* #ifdef COM_TxIPduTimeOutNotify */

#ifdef COM_TxIPduNotification
    /* Initializing Notify_Flag */
    Notify_Flag = COM_FALSE;
#endif /* #ifdef COM_TxIPduNotification */

    if (
#if(COM_CONFIGURATION_USE_DET == STD_ON)
        Com_Prv_DETCheck_Void(COMServiceId_MainFunctionTx)
#else
        (Com_Uninit_Flag == COM_INIT)
#endif
        )
    {
        /* Get TxIPdu-Ids which shall be processed for given ComMainFunction */
        StartIPduId = COM_GET_MAINFUNCTION_CFG(COM_NUM_OF_RX_MAINFUNCTION + (uint16)TxMainFuncId).StartIPduId;
        EndIPduId   = StartIPduId + COM_GET_MAINFUNCTION_CFG(COM_NUM_OF_RX_MAINFUNCTION + (uint16)TxMainFuncId).NumOfIpdus;

#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
        TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(StartIPduId);
        TxIpduRamPtr   = &COM_GET_TXPDURAM_S(StartIPduId);
#endif

        for ( Idx_Pdu_ui = StartIPduId; Idx_Pdu_ui < EndIPduId; Idx_Pdu_ui++ )
        {
            /* Check if IPdu group is started */
            if(Com_CheckTxIPduStatus((PduIdType)Idx_Pdu_ui))
            {
                SchM_Enter_Com_TxIpduProtArea(MAINFUNCTIONTX);
                /* Handling if Tx timeout is enabled */
#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
                /*Start: Tx Timeout Check*/
                if (Com_GetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
                {
                    Com_SetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
#ifdef COM_TxIPduNotification
                    Notify_Flag = (Com_GetValue(TXIPDU,_SIGPROC,TxIPduConstPtr->txIPduFields) == COM_DEFERRED);
#endif /* #ifdef COM_TxIPduNotification */
                }
                /*Start: Tx Timeout Check*/
#ifdef COM_TxIPduTimeOut
                else
                {
                    Timeout_Flag = Com_Prv_MainFunctionTx_IPduTimeOut(Idx_Pdu_ui);
                }
#endif

#endif /* #if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification) */

                /*End: Tx Timeout Check*/

                SendIPdu = Com_Prv_MainFunctionTx_Modes(Idx_Pdu_ui);

                SchM_Exit_Com_TxIpduProtArea(MAINFUNCTIONTX);

#ifdef COM_TxIPduTimeOutNotify
                /*Check if Timeout Notification function need to be called */
                if(Timeout_Flag == COM_TRUE)
                {
                    /* Call Ipdu Error Notification if configured
                     * This generated function will call the timeout notification callback functions of all signals and
                     * signal groups within this IPdu.
                     */
                    if(TxIPduConstPtr->TimeOutNotification_Cbk != NULL_PTR)
                    {
                        TxIPduConstPtr->TimeOutNotification_Cbk();
                    }
                }
#else
# ifdef COM_TxIPduTimeOut
                (void)Timeout_Flag;
# endif
#endif /* #ifdef COM_TxIPduTimeOutNotify */

#ifdef COM_TxIPduNotification
                /*Check if Deferred Notification function need to be called */
                if(Notify_Flag == COM_TRUE)
                {
                    /* Notification callback function for IPDU.
                     * This function is generated through code generator.
                     * Individual signal and signal group notifications functions are called from within this function.
                     * If IPDU has configured TX notification callback funcion, this function
                     * is also called from the generated function
                     */
                    if(TxIPduConstPtr->Notification_Cbk != NULL_PTR)
                    {
                        TxIPduConstPtr->Notification_Cbk();
                    }
                }
#endif /* #ifdef COM_TxIPduNotification */

            Com_Prv_MainFunctionTx_SendIPdu(Idx_Pdu_ui, SendIPdu);
            } /* end of IPdu group started check */
#if defined(COM_TxIPduTimeOut) || defined(COM_TxIPduNotification)
            TxIPduConstPtr++ ;
            TxIpduRamPtr++;
#endif
        } /* end of for loop */
    }
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#ifdef COM_TxIPduTimeOut

/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_IPduTimeOut
 Syntax           : boolean Com_Prv_MainFunctionTx_IPduTimeOut(uint16_least TxPduId)
 Description      : Service to perform the IPdu Timeout processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_IPduTimeOut(VAR(uint16_least, AUTOMATIC) TxPduId)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
#ifdef COM_CANCELTRANSMITSUPPORT
    /* Local pointer to hold tha address of the Tx ipdu static configuration */
    Com_TxIpduConstPtrType                  TxIPduConstPtr;
    VAR( boolean, AUTOMATIC )               isCancelTransmitSupported;
#endif
    VAR(boolean, AUTOMATIC)                 Timeout_Flag;
    /* Initializing to COM_FALSE */
    Timeout_Flag = COM_FALSE;

#ifdef COM_CANCELTRANSMITSUPPORT
    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(TxPduId);
#endif
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);

    /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
    * HIS-LEVEL parameter will be violated if more conditional statements are added.
    * Hence the warning is suppressed. */
    if((Com_GetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags) != COM_TRUE)
       &&
       (Com_GetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags) == COM_START))
    {
        /*Reduce Tick Timeout*/
        if (TxIpduRamPtr->Com_TickTxTimeout != 0)
        {
            --TxIpduRamPtr->Com_TickTxTimeout;
        }
        if(TxIpduRamPtr->Com_TickTxTimeout == 0)
        {
            Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_STOP);
            /*This Flag has to be set to START only in SendIpdu()i.e just before Sending IPDU*/

#ifdef COM_RETRY_FAILED_TX_REQUESTS
            /* [SWS_Com_00775] If ComRetryFailedTransmitRequests is enabled and
             * a transmission deadline monitoring timeout occurs for an I-PDU,
             * the AUTOSAR COM module shall expire any pending transmission request for this I-PDU. */
            Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
#endif

            /* When there is Timeout we dont care the value of TxIpduRamPtr->Com_n, So RESETING it.
             * If a transmission deadline monitoring timeout occurs before the N-Times
             * transmission is complete, the N-Times transmission shall be abandoned. */
            TxIpduRamPtr->Com_n = 0;

            TxIpduRamPtr->Com_MinDelayTick = 0;

            Timeout_Flag = COM_TRUE; /*Timeout Notification need to be called for this Ipdu*/

#if defined(COM_CANCELTRANSMITSUPPORT) && defined(COM_TP_IPDUTYPE)
            {
                isCancelTransmitSupported = Com_GetValue(TXIPDU,_ISCANCELTRANSMITSUPPORTED,TxIPduConstPtr->txIPduFields);
                /* If the Pdu is of TP type and cancel transmit is supported for this pdu */
                /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
                * HIS-LEVEL parameter will be violated if more conditional statements are added.
                * Hence the warning is suppressed. */
                if((Com_GetValue(TXIPDU,_ISLARGEDATAPDU,TxIPduConstPtr->txIPduFields) != COM_FALSE)
                   &&
                   (isCancelTransmitSupported != COM_FALSE)
                   &&
                   /* Check is there ongoing transmission, then only call the cancel transmit */
                   (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) != COM_TRUE)
                   )
                {
                    /* Timeout occurred cancel the ongoing transmission.
                     * As per the specification, the AUTOSAR COM ignores the return code */
                    ( void )PduR_ComCancelTransmit(TxIPduConstPtr->PdurId);
                }
            }
#endif /* #ifdef COM_CANCELTRANSMITSUPPORT */
        }
    }
    return Timeout_Flag;
}

#endif /* #ifdef COM_TxIPduTimeOut */

/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_Modes
 Syntax           : boolean Com_Prv_MainFunctionTx_Modes(uint16_least TxPduId)
 Description      : Service to perform the tranmission mode processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE boolean Com_Prv_MainFunctionTx_Modes(VAR(uint16_least, AUTOMATIC) TxPduId)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
    VAR(boolean, AUTOMATIC)                 SendIPdu;

    SendIPdu = COM_FALSE;
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);

    /* Decrement the MDT timer, if timer is not already zero and also Transmission is not blocked because of
     * non receipt of Com_TxConfirmation().
     */
    if (TxIpduRamPtr->Com_MinDelayTick != 0)
    {
        /* Decrement the MDT timer, if timer is not already zero */
        --TxIpduRamPtr->Com_MinDelayTick;
    }

    switch(Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode))
    {
    case COM_TXMODE_DIRECT:
        SendIPdu = Com_Prv_MainFunctionTx_DirectMode(TxPduId);
        break;

    case COM_TXMODE_MIXED:
        SendIPdu = Com_Prv_MainFunctionTx_MixedMode(TxPduId);
        break;

    case COM_TXMODE_NONE:
        SendIPdu = Com_Prv_MainFunctionTx_NoneMode(TxPduId);
        break;

    case COM_TXMODE_PERIODIC:
        SendIPdu = Com_Prv_MainFunctionTx_PeriodicMode(TxPduId);
        break;

    default:
        /* Validations and code generation shall take care to generate the valid transmission mode.
         * In case, if the program control reach here, nothing shall be done
         * Hence it is left empty. */
        break;
    }
    return SendIPdu;
}


/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_DirectMode
 Syntax           : boolean Com_Prv_MainFunctionTx_DirectMode(uint16_least TxPduId)
 Description      : Service to perform the Direct Mode transmision processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_DirectMode(VAR(uint16_least, AUTOMATIC) TxPduId)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
    VAR(boolean, AUTOMATIC)                 SendIPdu;

    SendIPdu = COM_FALSE;
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);

    if (TxIpduRamPtr->Com_n_Tick_Tx != 0)
    {
        --TxIpduRamPtr->Com_n_Tick_Tx;
    }
    if(TxIpduRamPtr->Com_MinDelayTick == 0)
        /*This condition is for the DIRECT && N=0 && Triggered and Com_TriggerIPDUSend()*/
    {
        /* Check if MDT flag is TRUE*/
        if(Com_GetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
            /*Call SendIpdu*/
            SendIPdu = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
        }
        else
        {
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* the first check is for event transmissions triggered from Com_SendSignal/Com_SendSignalGroup
             * /Com_SendDynSignal/Com_TriggerIpduSend */
            /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of
             * intermediate variables to store and read here, may read obsolete value.
             * Evaluation(or no evaluation) shall not impact the system behaviour.
             * Hence suppressed. */
            if((TxIpduRamPtr->Com_n_Tick_Tx == 0) &&
                                    (Com_GetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE))
            {
                /*Call SendIpdu*/ /*This Send IPDU is for Direct frames of MIXED mode*/
                SendIPdu = COM_TRUE;
            }
            else
#endif/* #ifdef COM_TXPDU_DEFERREDPROCESSING */
            if((TxIpduRamPtr->Com_n_Tick_Tx == 0) && (TxIpduRamPtr->Com_n != 0))
            {
                SendIPdu = COM_TRUE;
                /*Call SendIpdu*/
            }
            else
            {

            }
        }
        if (TxIpduRamPtr->Com_n != 0)
        {
            if (TxIpduRamPtr->Com_n_Tick_Tx == 0)
            {
                TxIpduRamPtr->Com_n_Tick_Tx = TxIpduRamPtr->CurrentTxModePtr->RepetitionPeriodFact;
            }
        }
    }
    return SendIPdu;
}


/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_MixedMode
 Syntax           : boolean Com_Prv_MainFunctionTx_MixedMode(uint16_least TxPduId)
 Description      : Service to perform the Mixed Mode transmision processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
/* HIS METRIC PATH, V(G), LEVEL VIOLATION IN Com_Prv_MainFunctionTx_MixedMode:
   Com_Prv_MainFunctionTx_MixedMode function contains simple "if" and "else if" statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_MixedMode( VAR(uint16_least, AUTOMATIC) TxPduId)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
    VAR(boolean, AUTOMATIC)                 SendIPdu;

    SendIPdu = COM_FALSE;
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);

    if (TxIpduRamPtr->Com_Tick_Tx != 0)
    {
        --TxIpduRamPtr->Com_Tick_Tx;
    }
    if (TxIpduRamPtr->Com_n_Tick_Tx != 0)
    {
        --TxIpduRamPtr ->Com_n_Tick_Tx;
    }
    if(TxIpduRamPtr->Com_MinDelayTick == 0)
    {
        /* Check if MDT flag is TRUE */
        if(Com_GetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
            /*Call SendIpdu*/
            SendIPdu = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
        }
        else
        {
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* the first check if for event transmissions triggered from Com_SendSignal/Com_SendSignalGroup
             * /Com_SendDynSignal/Com_TriggerIpduSend */
            /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
             * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
             * system behaviour. Hence suppressed. */
            if((TxIpduRamPtr->Com_n_Tick_Tx == 0) &&
                            (Com_GetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE))
            {
                /*Call SendIpdu*/ /* This Send IPDU is for Direct frames of MIXED mode */
                SendIPdu = COM_TRUE;
            }
            else
#endif
            /* Regular N-times transmission */
            if((TxIpduRamPtr->Com_n_Tick_Tx == 0) && (TxIpduRamPtr->Com_n != 0))
            {
                SendIPdu = COM_TRUE;
                /*Call SendIpdu*/ /*This Send IPDU is for Direct frames of MIXED mode*/
            }
            else
            {
                if((TxIpduRamPtr->Com_Tick_Tx == 0)&&(TxIpduRamPtr->Com_n == 0))
                {
                    SendIPdu = COM_TRUE;
                    /* Call SendIpdu*/ /* This Send IPDU is for Periodic Frames of MIXED mode */
                }
            }
        }
        if ((TxIpduRamPtr->Com_n != 0) && (TxIpduRamPtr->Com_n_Tick_Tx == 0))
        {
            TxIpduRamPtr->Com_n_Tick_Tx = TxIpduRamPtr->CurrentTxModePtr->RepetitionPeriodFact;
        }
    }/*if(Com_MinDelayTick[TxPduId] == 0)*/
    else
    {
        if((TxIpduRamPtr->Com_Tick_Tx == 0) && (TxIpduRamPtr->Com_n == 0))
        {
            /* Usecase diagram 5b from Appendix A, Postponed periodic transmission to respect MDT */
            Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_TRUE);
        }
    }
    if(TxIpduRamPtr->Com_Tick_Tx == 0)
    {
        /* Load tick for the Tx IPDU */
        TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEPERIOD(TxPduId, (TxIpduRamPtr->CurrentTxModePtr));
    }
    return SendIPdu;
}


/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_NoneMode
 Syntax           : boolean Com_Prv_MainFunctionTx_NoneMode(uint16_least TxPduId)
 Description      : Service to perform the None Mode transmision processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_NoneMode(  VAR(uint16_least, AUTOMATIC) TxPduId)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
    VAR(boolean, AUTOMATIC)                 SendIPdu;

    SendIPdu = COM_FALSE;
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);

    if(TxIpduRamPtr->Com_MinDelayTick == 0)
    {
        /* Check if MDT is TRUE */
        if(Com_GetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
            /*Call SendIpdu*/
            SendIPdu = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
        }
        else
        {
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* This part of code is executed when Com_TriggerIpduSend triggers transmission from NONE Ipdus */
            if(Com_GetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
            {
                if(TxIpduRamPtr->Com_n_Tick_Tx != 0)
                {
                    TxIpduRamPtr->Com_n_Tick_Tx--;
                }
                if(TxIpduRamPtr->Com_n_Tick_Tx == 0)
                {
                    /* Trigger transmission for Ipdu */
                    SendIPdu = COM_TRUE;
                }
            }
#endif
        }
    }
    return SendIPdu;
}


/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_PeriodicMode
 Syntax           : boolean Com_Prv_MainFunctionTx_PeriodicMode(uint16_least TxPduId)
 Description      : Service to perform the Periodic Mode transmision processing of the AUTOSAR COM transmission
                    activities that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Com_Prv_MainFunctionTx_PeriodicMode:
   Com_Prv_MainFunctionTx_PeriodicMode function contains simple "else if" statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_MainFunctionTx_PeriodicMode(  VAR(uint16_least, AUTOMATIC) TxPduId)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
    VAR(boolean, AUTOMATIC)                 SendIPdu;

    SendIPdu = COM_FALSE;
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);

    if (TxIpduRamPtr->Com_Tick_Tx != 0)
    {
        --TxIpduRamPtr->Com_Tick_Tx;
    }
    if(TxIpduRamPtr->Com_MinDelayTick == 0 )
    {
        /* Check if MDT flag is TRUE*/
        if(Com_GetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
            /*This condition is for the DIRECT && N=0 && Triggered and Com_TriggerIPDUSend()*/
            /*Call SendIpdu*/
            SendIPdu = COM_TRUE;

            /*This has to be RESET as soon as it is entered into this condition*/
            Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
        }
        /* the first check if for event transmissions triggered from Com_SendSignal/Com_SendSignalGroup/
         * Com_SendDynSignal/Com_TriggerIpduSend */
        else
        {
            if(TxIpduRamPtr->Com_Tick_Tx == 0)
            {
                SendIPdu = COM_TRUE;
                /*Call SendIpdu*/ /* This Send IPDU is for Periodic Frames */
            }
#ifdef COM_TXPDU_DEFERREDPROCESSING
            /* This part of code is executed when Com_TriggerIpduSend triggers transmission from Periodic Ipdus */
            else if(Com_GetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
            {
                if (TxIpduRamPtr->Com_n_Tick_Tx != 0)
                {
                    TxIpduRamPtr->Com_n_Tick_Tx--;
                }
                if(TxIpduRamPtr->Com_n_Tick_Tx == 0)
                {
                    /* Trigger transmission for Ipdu */
                    SendIPdu = COM_TRUE;
                }
            }
            else
            {

            }
#endif /* #ifdef COM_TXPDU_DEFERREDPROCESSING */
        }
    }
    else
    {
        if(TxIpduRamPtr->Com_Tick_Tx == 0)
        {
            /* TxIpduRamPtr->Com_TxFlags.MDT_Flag = COM_RESET */
            Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_TRUE);  /*Usecase diagram 5b in */
        }
    }
    /*the Timer is started only if the timer is already not running */
    if(TxIpduRamPtr->Com_Tick_Tx == 0)
    {
        /* Load tick for the Tx IPDU */
        TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEPERIOD(TxPduId, (TxIpduRamPtr->CurrentTxModePtr));
    }
    return SendIPdu;
}


/***************************************************************************************************
 Function name    : Com_Prv_MainFunctionTx_SendIPdu
 Syntax           : void Com_Prv_MainFunctionTx_SendIPdu( uint16_least TxPduId, boolean SendIPdu)
 Description      : Service to perform the timout notification processing of the AUTOSAR COM transmission activities
                    that are not directly initiated by the calls from the RTE and PDU-R.
 Parameter        : void
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_MainFunctionTx_SendIPdu(  VAR(uint16_least, AUTOMATIC) TxPduId,
                                                                    VAR(boolean, AUTOMATIC) SendIPdu)
{
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                    TxIpduRamPtr;
#ifdef COM_TxIPduTimeOut
    /* Local pointer to hold tha address of the Tx ipdu static configuration */
    Com_TxIpduConstPtrType                  TxIPduConstPtr;
    VAR(uint8, AUTOMATIC)                   LatestTransMode_u8;

    TxIPduConstPtr  = COM_GET_TX_IPDU_CONSTDATA(TxPduId);
#endif
    TxIpduRamPtr    = &COM_GET_TXPDURAM_S(TxPduId);

    /* Send the IPdu , if there has been a condition for the same.
     * Trigger TxIPdu, if ComRetryFailedTransmitRequests is enabled and last call to PduR_ComTransmit()
     * returned E_NOT_OK */

    /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status.
     * Evaluation(or no evaluation) of the 2nd operand does not impact the system behaviour,
     * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
     * Hence the warning is suppressed. */
    if ( ( SendIPdu == COM_TRUE )
#ifdef COM_RETRY_FAILED_TX_REQUESTS
       || ( Com_GetRamValue(TXIPDU,_RETRYFAILEDTXREQ,TxIpduRamPtr->Com_TxFlags) != COM_FALSE )
#endif
     )
    {
        /* All unused flags are reset here */
        Com_SendIpduInfo sendIpduFlag_u16 = COM_CLEAR_SENDIPDU_FLAGS;
        /* RESET the flag, as this IPdu is triggered from the cyclic task */
        Com_SetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags,COM_FALSE);

        /* The bit field is loaded as below
         * 1. Call is from proc function,so COM_RESET
         * 2. No mode change happens from Com_mainfunctionTx ,so COM_RESET
         * 3. Signal transfer property information is not used, so COM_RESET
         * 4. Timeout monitoring is done for PERIODIC and MIXED frames.
         * */
        sendIpduFlag_u16.isEventTrig      =  COM_RESET;
#ifdef COM_TxIPduTimeOut
        LatestTransMode_u8 = Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode);
        /* MODE is PERIODIC or MIXED and timeout factor is configured */
        if(((LatestTransMode_u8 & COM_TXMODE_MIXED) > (uint8)0) && (TxIPduConstPtr->Timeout_Fact != 0u ))
        {
            sendIpduFlag_u16.isTimeoutReq = COM_SET;
        }
#endif
        /* In case of Tp large pdu: If already a transmission request has been place, then do not trigger new request
         * In case of Normal pdu: Then this flag will never be set, so always Com_SendIpdu() will be called */
#ifdef COM_TP_IPDUTYPE
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) != COM_TRUE)
#endif
        {
            Com_Prv_SendIpdu((PduIdType)TxPduId,sendIpduFlag_u16);
        }
    }
}

/* FC_VariationPoint_START */
/**********************************************************************************************************************
 Function name    : Com_IsTxScheduled
 Syntax           : boolean Com_IsTxScheduled(PduIdType ComTxPduId, uint16 ComCallerTaskCycle)
 Description      : this function checks whether the Tx frame is scheduled to be sent in the coming cycle
 Parameter        : ComTxPduId
                    ComCallerTaskCycle- this parameter is the value of caller's task cycle in ms.
                                        i.e : if the Caller is in 1ms, then the ComCallerTaskCycle = 1
 Return value     : boolean

 To Be considered before using this function
 --------------------------------------------
 1. this function shall be used for only Periodic type Ipdu and not for other Pdu types

When Com_IsTxScheduled is called in different task cycle as compared to Com_mainfucntionTx:
In this scenario, the API signature is changed and an additional parameter is to be passed by the caller.
Tb : Time raster of Com_MainFunctionTx
Tc : Time raster of caller of Com_IsTxScheduled

Time raster passed by the caller is in ms, hence the ticks are converted into time in ms,
Multiplying ticks by Tb will serve the purpose.
After the conversion, the value hence obtained,is remaining time for Ipdu transmission in ms

There are three cases observed in this scenario, presented below
Case 1: (Tb = Tc)
If Com_IsTxScheduled is called in the same raster as Com_MainFunctionTx,
then if Remaining time is greater than Caller time, the caller can wait for another raster to update IPdu data.
else Ipdu data update can happen in current raster.

Case 2: (Tb < Tc)
If Com_IsTxScheduled is called slower than Com_mainfunctionTx,
then if Remaining time is greater than Caller time, the caller can wait for another raster to update IPdu data.
else Ipdu data update can happen in current raster.

Case 3: (Tb > Tc)
If Com_IsTxScheduled is called faster than Com_mainfunctionTx,
Here the Com internal ticks would not have enough resolution to acknowledge the last raster before transmission.
Hence when the Com ticks reach value 1, the caller can do Ipdu data update in the current raster.

Scenarios analysed:
1. For 10ms frame, the Com_Tick_Tx will be always 1 as the com_mainfunction_tx (function above) loads the
value at the end of the function. hence the If condition will fail and function will always return TRUE
2. For >10ms frame, the Com_Tick_Tx will never have 0 value as the value is loaded at the end of the
Com_Mainfunction_Tx. When the Tick value becomes 1, this function will return true and SCL processing
will happen.
3. for >10ms Frame, if minimum delay is configured, then whever the value of Mindelay is 1, this function
will return TRUE as the frame can have request for transmission and the next transmission criteria of minimum delay
will be over in the subsequent call of mainfunction_Tx (min delay becomes 0 and can have any transmission based on signal
or transmission mode)
4. For any Cycletime Frame, if they have transmission mode change enabled (COM_TMS_VARIABLE) then
this function will return TRUE always to be on the safer side.

 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(boolean, COM_CODE) Com_IsTxScheduled( VAR(PduIdType, AUTOMATIC) ComTxPduId,
                                           VAR(uint16, AUTOMATIC)    ComCallerTaskCycle)
{
    Com_TxIpduRamPtrType                TxIpduRamPtr;
#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE
    VAR(uint16, AUTOMATIC)              ticksinMs_u16;
    VAR(Com_TimeBaseType, AUTOMATIC)    timeBaseInMs;
#endif
    VAR(boolean, AUTOMATIC)             IsTxScheduled;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */
    ComTxPduId   = COM_GET_TX_IPDU_ID(ComTxPduId);

    TxIpduRamPtr = &COM_GET_TXPDURAM_S(ComTxPduId);

    /*if COM_TXSCHEDULED_DIFF_TIMEBASE is disabled then ComCallerTaskCycle will be considered to be null  */
#ifndef COM_TXSCHEDULED_DIFF_TIMEBASE
    (void)ComCallerTaskCycle;
#endif

#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE

    timeBaseInMs  = COM_GET_MAINFUNCTION_CFG( COM_NUM_OF_RX_MAINFUNCTION + (COM_GET_TX_IPDU_CONSTDATA(ComTxPduId))->MainFunctionRef ).TimeBaseInMs;

    /* The ticks are multiplies by the time in ms to get the remaining time in ms
     * Time base is multiplied by 1000, to get the time value in ms.(i.e 1ms = 1) */
    ticksinMs_u16 = ((TxIpduRamPtr->Com_Tick_Tx) * timeBaseInMs);

#endif /* #ifdef COM_TXSCHEDULED_DIFF_TIMEBASE */

   /* If the Pdu has varied Transmission mode, then it is safe to return TRUE always as Transmission mode selecion
    * happens based on the signal filter algo and if the function returns true at wrong time there could be chances
    * that the signal update could be missed and transmission mode cannot happen (Autosar Com Use case 3)
    * This check has to the first check as this has the highest priority, this part checks whether
    * the configured Pdu is Periodic and it is scheduled to be Transmitted in the coming cycle */
#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE
    /* In normal scenario, it is not intended that caller of Com_IsTxScheduled is called in lesser task cycle than
     * Com_MainfunctionTx. But if this is the case, then Com_IsTxScheduled returns true when
     * TxIpduRamPtr->Com_Tick_Tx = 2, 1 or 0. Hence Caller has to update the data a few cycles earlier */
    if (ComCallerTaskCycle < timeBaseInMs)
#endif
    {
        if((Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode) == (uint8)COM_TXMODE_PERIODIC) &&
           (TxIpduRamPtr->Com_Tick_Tx > 1u) )
        {
            IsTxScheduled = COM_FALSE;
        }
        else
        {
            IsTxScheduled = COM_TRUE;
        }
    }
#ifdef COM_TXSCHEDULED_DIFF_TIMEBASE
    else
    {
        if((Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode) == (uint8)COM_TXMODE_PERIODIC) &&
           (ticksinMs_u16 > ComCallerTaskCycle) )

        {
            IsTxScheduled = COM_FALSE;
        }
        else
        {
            IsTxScheduled = COM_TRUE;
        }
    }
#endif /* #ifdef COM_TXSCHEDULED_DIFF_TIMEBASE */

    return IsTxScheduled;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* FC_VariationPoint_END */

#ifdef COM_PROVIDE_IPDU_STATUS
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/***************************************************************************************************
 Function name    : COM_ProvideTxIpduStatus
 Syntax           : boolean COM_ProvideTxIpduStatus(PduId)
 Description      : The service Com_ProvideTxIpduStatus returns the status of Tx I-PDU referenced by PduId.
 Parameter        : PduId
 Return value     : boolean
 ***************************************************************************************************/
FUNC(boolean,COM_CODE) Com_ProvideTxIpduStatus(VAR(PduIdType, AUTOMATIC) PduId)
{
    VAR(boolean, AUTOMATIC) pduStatus_b;

    /* If the Pdu Id passed is not an Tx Ipdu Id or Ipdu is not started, then status false is returned */
    if (PduId < COM_GET_NUM_TX_IPDU)
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        PduId = COM_GET_TX_IPDU_ID(PduId);

        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
         * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        pduStatus_b = (Com_CheckTxIPduStatus((PduIdType)PduId));
    }
    else
    {
        pduStatus_b = COM_FALSE;
    }

    return(pduStatus_b);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_PROVIDE_IPDU_STATUS */

