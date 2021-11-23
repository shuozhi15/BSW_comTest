

/**********************************************************************************************************************
 * Preprocessor includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "PduR_Com.h"
/* [SWS_BSW_00036] Inter Module Checks */
#if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != COM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != COM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_LoadPeriodicModeInfo(
                                                VAR(PduIdType, AUTOMATIC)           ComTxPduId,
                                                VAR(uint8, AUTOMATIC)               isSigTriggered
                                                                 );

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_LoadEventModeInfo(
                                                VAR(PduIdType, AUTOMATIC)           ComTxPduId,
                                                VAR(Com_SendIpduInfo, AUTOMATIC)    sendIpduFlag_u16
                                                              );

LOCAL_INLINE FUNC(PduLengthType, COM_CODE) Com_Prv_CalculateTxPduLength(
                                                VAR(PduIdType, AUTOMATIC)           ComTxPduId,
                                                VAR(uint8, AUTOMATIC)               isTrigIpduSendWithMetadata
                                                                       );

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProcessTxReqStatus(
                                                VAR(PduIdType, AUTOMATIC)           ComTxPduId,
                                                VAR(Std_ReturnType, AUTOMATIC)      ipduTransStatus_ui
                                                            );

/**********************************************************************************************************************
 * Global functions (declared in header files)
 *********************************************************************************************************************/

/* Below table details the transmission behavior in combination with Mode change
-----------------------------------------------------------------------------------------------------------------------
    TxMode        ModeChange    Other Cases            Flag value  N - Number of repetitions
-----------------------------------------------------------------------------------------------------------------------
    None            No              NA                    : 00 -- No transmission
    None            Yes             NA                    : 00 -- No transmission
    Periodic        No              NA                    : 00 -- No transmission
    Periodic        Yes             NA                    : 01 -- COM_TRANS_IN_PROC
    Direct          No              N>0                   : 01 -- N-times transmission+COM_TRANS_IN_PROC
    Direct          No              N=0                   : 00 -- COM_RESET
    Direct          Yes             N>0                   : 01 -- N-times transmission+COM_TRANS_IN_PROC
    Direct          Yes             N=0                   : 00 -- COM_RESET
    Mixed           No         Pending signal             : 00 -- No transmission
    Mixed           No         Trig signal,N=0            : 00 -- COM_RESET
    Mixed           No         Trig signal,N>0            : 01 -- N-times transmission+COM_TRANS_IN_PROC
    Mixed           Yes        Pending signal             : 02 -- COM_MIXED_PENDING
    Mixed           Yes        Trig signal,N=0            : 03 -- COM_MIXED_TRIG
    Mixed           Yes        Trig signal,N>0            : 03 -- COM_MIXED_TRIG
    COM_RESET         : Transmission will happen in Com_Prv_SendIpdu immediately
    COM_TRANS_IN_PROC : Transmission will be initiated in Com_MainFunctionTx
    COM_MIXED_PENDING : N-times transmission should be cancelled + periodic Transmission should be started
    COM_MIXED_TRIG    : Periodic transmission should be cancelled + N-times transmission should be initiated.
*/

/**********************************************************************************************************************
 Function name    : Com_Prv_SendIpdu
 Syntax           : void Com_Prv_SendIpdu(PduIdType ComTxPduId, Com_SendIpduInfo sendIpduFlag_u16)
 Description      : Internal function used for Transmitting Tx Ipdu's
 Parameter        : ComTxPduId       - Id of the Tx ipdu
                    sendIpduFlag_u16 - flags to decide ipdu transmission
 Return value     : Void
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS VIOLATION IN Com_Prv_SendIpdu:
   Com_Prv_SendIpdu function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_Prv_SendIpdu(
                            VAR(PduIdType, AUTOMATIC)           ComTxPduId,
                            VAR(Com_SendIpduInfo, AUTOMATIC)    sendIpduFlag_u16
                                     )
{
    Com_TxIpduConstPtrType  TxIPduConstPtr;
    Com_TxIpduRamPtrType    TxIpduRamPtr;
    VAR(boolean,AUTOMATIC)  SendIpdu_b;

    SendIpdu_b     = COM_FALSE;

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(ComTxPduId);
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(ComTxPduId);

    SchM_Enter_Com_TxIpduProtArea(SENDIPDU_TXFLAGS);

    /* This flag is to check if the Call to Com_Prv_SendIpdu happened from
     * 1. Com_SendSignal()
     * 2. Com_SendSignalGroup()
     * 3. Com_SwitchIpduTxMode()
     * 4. Com_TriggerIPduSend()
     * 5. Com_SendDynSignal()
     * 6. Com_TriggerIPDUSendWithMetaData()
     * 7. Com_MainFunctionRouteSignals()
     * 8. Com_SendSignalGroupArray()
     */
    if (sendIpduFlag_u16.isEventTrig != COM_RESET)
    {
        VAR(boolean,AUTOMATIC) isEventMode;

        /* Stop the current RepetitionTicks. As Pdu needs to be triggered immediately either for event transmission or
         * for mode change. It is loaded accordingly in the later checks.
         */
        TxIpduRamPtr->Com_n_Tick_Tx = 0;

        /* Check if transmission mode has changed */
        if (sendIpduFlag_u16.isModeChangd == COM_SET)
        {
            /* Periodic transmisions are always triggered from Com_MainFunctionTx to maintain the periodicity */
            isEventMode = Com_Prv_LoadPeriodicModeInfo(ComTxPduId, sendIpduFlag_u16.sigTransProp);
        }
        else
        {
            /* If there is no mode change, then Com_Prv_SendIpdu is called for transmission of event message.
             * N-Repetitions are to be loaded.
             */
            isEventMode = COM_TRUE;
        }

        /* This part will be executed, Only for the event transmissions for Ipdus with:
         * 1. Direct Mode
         * 2. Mixed mode with the signal transfer property Triggered
         * In case of mode change for Ipdus with tx mode:
         * 1. Direct mode
         * 2. Mixed mode with the signal triggered transfer property
         */
        if (isEventMode == COM_TRUE)
        {
            SendIpdu_b = Com_Prv_LoadEventModeInfo(ComTxPduId, sendIpduFlag_u16);
        }
    }
    /* If call is from Com_MainFunctionTx */
    else
    {
        /* MDT checks happen in Com_MainFunctionTx, hence no MDT related checks here. */
        SendIpdu_b = COM_TRUE;

#ifdef COM_TxIPduTimeOut
        /* In case of periodic transmission from PERIODIC or MIXED mode, reload timeout only if it is not started yet.
         * In case of N-repetitions transmission, it is already reloaded in the call to "Send" api as per below req:
         * “The monitoring timer is started upon completion of the call to the SendMessage,
         * SendDynamicMessage or SendZeroMessage API service.”
         */
        if (sendIpduFlag_u16.isTimeoutReq == COM_SET)
        {
            if (Com_GetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags) != COM_START)
            {
                /* Start the TxConfirmation Timer now i.e set the Timer value to the one configured */
                Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_START);

                TxIpduRamPtr->Com_TickTxTimeout = TxIPduConstPtr->Timeout_Fact;
            }
        }
#endif /* #ifdef COM_TxIPduTimeOut */

    } /* if (sendIpduFlag_u16.isEventTrig != COM_RESET) */

    SchM_Exit_Com_TxIpduProtArea(SENDIPDU_TXFLAGS);

    if (SendIpdu_b == COM_TRUE)
    {
        VAR(PduInfoType, COM_VAR)       ComTxIPduInfo;
        VAR(Std_ReturnType, AUTOMATIC)  ipduTransStatus_ui;
#ifdef COM_TxIPduCallOuts
        VAR(boolean, AUTOMATIC)         isTransmitCallout_b;
        VAR(boolean, AUTOMATIC)         ipduCalloutStatus;
#endif

        /* Assign IpduBuffer address and its length to the local variable */
        ComTxIPduInfo.SduDataPtr = TxIPduConstPtr->BuffPtr;
        ComTxIPduInfo.SduLength  = Com_Prv_CalculateTxPduLength( ComTxPduId,
                                                                 sendIpduFlag_u16.isTriggerIpduSendWithMetadata );

#ifdef COM_TxIPduCallOuts
        /* _ISCALLOUTFRMTRIGTRANS flag indicates if the callout has to be invoked from here(false) or
         * from TriggerTransmit(true) */
        isTransmitCallout_b = !(Com_GetValue(TXIPDU,_ISCALLOUTFRMTRIGTRANS,TxIPduConstPtr->txIPduFields));

        /* Continue to call PduR_ComTransmit, if
         * + callout is not configured OR
         * + callout is configured and returns TRUE OR
         * + callout is configured, but it is to be called from Com_TriggerTransmit.
         * If callout is configured and returns FALSE, then dont call PduR_ComTransmit.
         */
        if ((TxIPduConstPtr->CallOut != NULL_PTR) && (isTransmitCallout_b))
        {
            SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA);
            ipduCalloutStatus = TxIPduConstPtr->CallOut(ComTxPduId, &ComTxIPduInfo);
            SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA);
        }
        else
        {
            ipduCalloutStatus = COM_TRUE;
        }

        if (ipduCalloutStatus == COM_TRUE)
#endif /* #ifdef COM_TxIPduCallOuts */
        {
#ifdef COM_MULTICORE_SUPPORT
            SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA);
#endif

            /* Call PduR_ComTransmit() */
            ipduTransStatus_ui = PduR_ComTransmit(TxIPduConstPtr->PdurId, &ComTxIPduInfo);

#ifdef COM_MULTICORE_SUPPORT
            SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA);
#endif
        }
#ifdef COM_TxIPduCallOuts
        else
        {
            /* As specified in OSEK COM, if the I-PDU callout returns false
             * the I-PDU will not be processed any further */
            ipduTransStatus_ui = E_NOT_OK;
        }
#endif /* #ifdef COM_TxIPduCallOuts */

        /* Process transmission request status */
        Com_Prv_ProcessTxReqStatus(ComTxPduId, ipduTransStatus_ui);
    }

} /* End of Com_Prv_SendIpdu */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**********************************************************************************************************************
 Function name    : Com_Prv_LoadPeriodicModeInfo
 Syntax           : boolean Com_Prv_LoadPeriodicModeInfo(PduIdType ComTxPduId, uint8 isSigTriggered)
 Description      : Private function to load periodic ticks in case of PERIODIC or MIXED mode
 Parameter        : ComTxPduId     - Id of the Tx ipdu
                    isSigTriggered - Call to SendIPdu is caused from triggered signal
 Return value     : true -> is event(Direct/Mixed) transmision mode
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_LoadPeriodicModeInfo(
                                                VAR(PduIdType, AUTOMATIC)   ComTxPduId,
                                                VAR(uint8, AUTOMATIC)       isSigTriggered
                                                                 )
{
    Com_TxIpduRamPtrType    TxIpduRamPtr;
    VAR(boolean,AUTOMATIC)  isEventMode;

    isEventMode  = COM_FALSE;

    TxIpduRamPtr = &COM_GET_TXPDURAM_S(ComTxPduId);

    /* If mode has changed, cancel the existing n-repetition transmissions.
     * As repetitions from the new transmission mode has to be used.
     */
    TxIpduRamPtr->Com_n = 0;

    /* Fetch latest transmission mode */
    switch (Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode))
    {
        case COM_TXMODE_PERIODIC:
        {
            /* [SWS_Com_00495] The below counter will be decremented in the next call of Com_MainFunctionTx() and
             * IPDU will be transmitted if IPDU tx mode is PERIODIC */
            TxIpduRamPtr->Com_Tick_Tx = 1;
        }
        break;

        case COM_TXMODE_DIRECT:
        {
            /* [SWS_Com_00582] If a change of the TMS causes a change to the transmission mode DIRECT,
             * an immediate (within the next main function at the latest unless shifted due to the MDT) direct/
             * n-times transmission to the underlying layer shall be initiated.
             * The above requirement clarifies the behavior in case the TMS-switch to ComTxModeMode DIRECT
             * was triggered by a signal with ComTransferProperty PENDING.
             */
            /* N-Repetitions are to be loaded. */
            isEventMode = COM_TRUE;
        }
        break;

        case COM_TXMODE_MIXED:
        {
            /* [SWS_Com_00495] If TMS changes to MIXED mode because of a PENDING signal, then IPDU will be sent in
             * periodic mode immediately, respecting MDT. */
            if (isSigTriggered != COM_SET)
            {
                /* The below counter will be decremented in the next call of Com_MainFunctionTx() and
                 * IPDU will be transmitted if IPDU tx mode is PERIODIC or MIXED */
                TxIpduRamPtr->Com_Tick_Tx = 1;
            }
            else
            {
                /* The CurrentTxModePtr points to the structure information of the current TxModeState. */
                TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEPERIOD(((PduIdType)ComTxPduId),
                                                                                      (TxIpduRamPtr->CurrentTxModePtr));

                /* Periodic transmission is always done from Com_MainFunctionTx,
                 * hence to maintain the periodicity between the first & second transmission,
                 * extra tick is added here.
                 * For subsequent periodic transmissions, they are reloaded in Com_MainFunctionTx.
                 */
                TxIpduRamPtr->Com_Tick_Tx++;

                /* N-Repetitions are to be loaded. */
                isEventMode = COM_TRUE;
            }
        }
        break;

        case COM_TXMODE_NONE:
        {
            /* do nothing */
        }
        break;

        default:
        {
            /* do nothing */
        }
        break;
    }

    return isEventMode;

} /* End of Com_Prv_LoadPeriodicModeInfo */


/**********************************************************************************************************************
 Function name    : Com_Prv_LoadEventModeInfo
 Syntax           : void Com_Prv_LoadEventModeInfo(PduIdType ComTxPduId, Com_SendIpduInfo sendIpduFlag_u16)
 Description      : Private function to load event(Direct/Mixed) mode configuration
 Parameter        : ComTxPduId       - Id of the Tx ipdu
                    sendIpduFlag_u16 - flags to decide ipdu transmission
 Return value     : true -> Proceed to SendIpdu
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_LoadEventModeInfo(
                                                VAR(PduIdType, AUTOMATIC)           ComTxPduId,
                                                VAR(Com_SendIpduInfo, AUTOMATIC)    sendIpduFlag_u16
                                                              )
{
#ifdef COM_TxIPduTimeOut
    Com_TxIpduConstPtrType  TxIPduConstPtr;
#endif
    Com_TxIpduRamPtrType    TxIpduRamPtr;
    VAR(boolean,AUTOMATIC)  SendIpdu_b;

    SendIpdu_b     = COM_FALSE;

#ifdef COM_TxIPduTimeOut
    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(ComTxPduId);
#endif
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(ComTxPduId);

    /* N-Repetitions are reloaded only here. */
    if (sendIpduFlag_u16.ignoreRepetitions != COM_SET)
    {
        /* The CurrentTxModePtr points to the structure information of the current TxModeState.
         * Update the counter for number of repetitions,
         * - If TMS changes to DIRECT mode/MIXED mode with triggered signal transfer property.
         * - [SWS_Com_00279] New event transmission.
         * then send the IPDU synchronous with Com_MainFunctionTx, respecting MDT */
        /* FC_VariationPoint_START */
#ifdef COM_TXPDU_NUMOFREPETITION_VIA_CALIBRATION
        TxIpduRamPtr->Com_n = Com_GetTxIPduNumberOfRepetitionCalibration((PduIdType)ComTxPduId);
#else
        /* FC_VariationPoint_END   */
        TxIpduRamPtr->Com_n = TxIpduRamPtr->CurrentTxModePtr->NumOfRepetitions;
        /* FC_VariationPoint_START */
#endif /* #define COM_TXPDU_NUMOFREPETITION_VIA_CALIBRATION */
        /* FC_VariationPoint_END   */
    }
    else
    {
        /* Repetitions are not loaded if
         * 1. The updated Signal/SigGrp is configured with TransferProperty
         *    TriggeredOnChangeWithoutRepetition or TriggeredWithoutRepetition.
         * 2. Call is from Com_TriggerIpduSend or Com_TriggerIPDUSendWithMetaData then
         *    the outstanding transmissions for the direct/Mixed mode with N > 0 should be cancelled */
        TxIpduRamPtr->Com_n = 0;
    }

#ifdef COM_TxIPduTimeOut
    /* The Transmission mode is Mixed/Direct */
    if (sendIpduFlag_u16.isTimeoutReq == COM_SET)
    {
        /* The timer is started only in case the signal/signal group has configured timeout.
         * Start the TxConfirmation Timer now i.e Set the Timer value to the one configured */
        Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_START);

        /* Note 1: If the transmission deadline monitoring timer runs out,there shall be an
         * ipdu-error-notification/signal-TimeoutNotification, regardless of the reason.
         * Even if it was postponed because of the MDT or it was filtered out via an I-PDU callout
         * Note 2: Timeout_Fact + 1,because in the immediate call to Com_MainfunctionTx(),
         * this Tick will be decremented */
        TxIpduRamPtr->Com_TickTxTimeout = (TxIPduConstPtr->Timeout_Fact + 1);
    }
#endif /* #ifdef COM_TxIPduTimeOut */

    /* 1. Check for Min Delay Timer of this IPDU,If MDT is not ZERO, SET a Flag, so that as soon as MDT becomes
     *    Zero in Com_MainfunctionTx(), this IPDU Can be transmitted.
     * 2. The Mentioned Sentence can be supported if MDT required for DIRECT Frames */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if ((TxIpduRamPtr->Com_MinDelayTick == 0u) &&
        (Com_GetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags) == COM_FALSE))
    {

        /* Check if it is a single transmission. */
        if (TxIpduRamPtr->Com_n == 0)
        {
            /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
            /* In case of a last transmission, reload Mixed-PhaseShift peridiocity(Com_Tick_Tx).
             * Check if MixedPhaseShift is enabled.
             * NOTE : PhaseShift is applicable for COM_MIXED mode only.If mode is COM_PERIODIC then this
             * bitfield is initialised to COM_FALSE.
             * If enabled, then periodic timer has to be reloaded here. */
            if (Com_GetRamValue(TXIPDU,_MIXEDPHASESHIFT,TxIpduRamPtr->Com_TransMode))
            {
                TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEPERIOD(((PduIdType)ComTxPduId),
                                                                                      (TxIpduRamPtr->CurrentTxModePtr));

                TxIpduRamPtr->Com_Tick_Tx++;
            }
#endif /* #ifdef COM_MIXEDPHASESHIFT */
            /* FC_VariationPoint_END */

            /* This flag does multiple operations:
             * - In case of gateway transmission from Com_MainFunctionRouteSignals, flag will add the extra offset to
             * configured MDT value to prevent MDT-timer from early expiry.
             * - In case of deferred processing, flag enables the TxIPdu to be transmitted through Com_MainFunctionTx.
             * - In case of non-deferred processing, flag will add the extra offset to configured MDT value
             * to prevent MDT-timer from early expiry.
             */
            Com_SetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags,COM_TRUE);

#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)
            if (sendIpduFlag_u16.isGwIpduSend != COM_RESET)
            {
                /* Trigger immediate transmission only for Gw Ipdus */
                SendIpdu_b = COM_TRUE;
            }
            else
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */
            {
                /* FC_VariationPoint_START */
#ifdef COM_TXPDU_DEFERREDPROCESSING
                /* Com Design and optimisation requirement: The transmissions are never done event based;
                 * Whenever there is an event transmission, Com flags it and initiates the transmission in
                 * the Next Mainfunction
                 * Please note this holds good for Ipdus transmitted from API Com_TriggerIpduSend()
                 * Effectively there are two transmission paths in Com
                 * 1. Com_MainfucntionTx
                 * 2. Event transmission for Com Gw Pdus */
                TxIpduRamPtr->Com_n_Tick_Tx = 1;

#else
                /* FC_VariationPoint_END   */

                /* In case of non-deferred processing, immediately trigger for transmission */
                SendIpdu_b = COM_TRUE;

                /* FC_VariationPoint_START */
#endif /*COM_TXPDU_DEFERREDPROCESSING*/
                /* FC_VariationPoint_END   */
            }
        }
        else
        {
            /* Com Design and optimisation requirement: The transmissions are never done event based;
             * Whenever there is an event transmission, Com flags it and initiates the transmission in
             * the Next Mainfunction
             * Please note this holds good for Ipdus transmitted from API Com_TriggerIpduSend()
             * Effectively there are two transmission paths in Com
             * 1. Com_MainfucntionTx
             * 2. Event transmission for Com Gw Pdus */
            TxIpduRamPtr->Com_n_Tick_Tx = 1;

        } /* END of if (TxIpduRamPtr->Com_n == 0) */
    }
    else
    {
        /* This Flag has to be RESET in Com_MainFunctionTx just before calling Com_Prv_SendIpdu */
        Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_TRUE);

        /* Periodic timer update for mixed phase shift when msg triggered within ComMinimumDelayTime */
#ifdef COM_MIXEDPHASESHIFT
        if (Com_GetRamValue(TXIPDU,_MIXEDPHASESHIFT,TxIpduRamPtr->Com_TransMode))
        {
            TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEPERIOD(((PduIdType)ComTxPduId),
                                                                                      (TxIpduRamPtr->CurrentTxModePtr));
        }
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }

    return SendIpdu_b;

} /* End of Com_Prv_LoadEventModeInfo */


/**********************************************************************************************************************
 Function name    : Com_Prv_CalculateTxPduLength
 Syntax           : PduLengthType Com_Prv_CalculateTxPduLength(PduIdType ComTxPduId, uint8 isTrigIpduSendWithMetadata)
 Description      : Private function to calculate PduLength
 Parameter        : ComTxPduId                 - Id of the Tx ipdu
                    isTrigIpduSendWithMetadata - flag that indicates that call to trigger txIPdu originated from
                                                 Com_TriggerIpduSendWithMetadata
 Return value     : PduLength
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(PduLengthType, COM_CODE) Com_Prv_CalculateTxPduLength(
                                                        VAR(PduIdType, AUTOMATIC)   ComTxPduId,
                                                        VAR(uint8, AUTOMATIC)       isTrigIpduSendWithMetadata
                                                                       )
{
    Com_TxIpduConstPtrType          TxIPduConstPtr;
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
    Com_TxIpduRamPtrType            TxIpduRamPtr;
#endif
    VAR(PduLengthType,AUTOMATIC)    comTxPduLength;

#ifndef COM_METADATA_SUPPORT
    (void)isTrigIpduSendWithMetadata;
#endif

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(ComTxPduId);
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(ComTxPduId);
#endif
    comTxPduLength = TxIPduConstPtr->Size;

#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
        /* The total Ipdu size will be sum of Ipdu static length and dynamic length for a dynamic IPdu.
         * The Dynamic length of the Ipdu is updated in the call to Com_SendDynSignal. */
        if (Com_GetRamValue(TXIPDU,_ISDYNIPDU,TxIPduConstPtr->txIPduFields))
        {
            comTxPduLength += (PduLengthType)TxIpduRamPtr->Com_DynLength;
        }
        else
#endif /* #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT */
        {
#ifdef COM_METADATA_SUPPORT
            /* If IPdu supports MetaData then the total IPdu size will be sum of IPdu static length and
             * MetaDataLength */
            if (Com_GetValue(TXIPDU,_ISMETADATASUPPORTED,TxIPduConstPtr->txIPduFields))
            {
                Com_MetaDataInfoPtr MetaDataInfoPtr;

                MetaDataInfoPtr = TxIPduConstPtr->MetaDataPtr;

                comTxPduLength += MetaDataInfoPtr->MetaDataLength;

                /* Check if the request for a transmission is invoked apart from Com_TriggerIPDUWithMetaData API */
                if (isTrigIpduSendWithMetadata != COM_SET)
                {
                    SchM_Enter_Com_TxIpduProtArea(SENDIPDU_DATA);

                    /* Restore default metadata in Tx Buffer */
                    Com_ByteCopy( (TxIPduConstPtr->BuffPtr + TxIPduConstPtr->Size),
                                  (&MetaDataInfoPtr->MetaDataDefaultValue[0]),
                                  ( MetaDataInfoPtr->MetaDataLength) );

                    SchM_Exit_Com_TxIpduProtArea(SENDIPDU_DATA);
                }
            }
            else
#endif /* #ifdef COM_METADATA_SUPPORT */
            {
                /* Pdu is neither of dynamic nor metadata,
                 * Hence, SduLength has to be provided only with the already updated static length */
            }
        }

#ifdef COM_TP_IPDUTYPE
        /* Finally store the calculated Tx-PduLength into global variable
         * that shall be used in Com_CopyTxData() */
        COM_GET_TPTXIPDULENGTH_AUO(ComTxPduId) = comTxPduLength;
#endif

    return comTxPduLength;

} /* End of Com_Prv_CalculateTxPduLength */


/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessTxReqStatus
 Syntax           : PduLengthType Com_Prv_ProcessTxReqStatus(PduIdType ComTxPduId, Std_ReturnType ipduTransStatus_ui)
 Description      : Private function to process transmission request status
 Parameter        : ComTxPduId         - Id of the Tx ipdu
                    ipduTransStatus_ui - E_OK/E_NOT_OK
 Return value     : PduLength
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProcessTxReqStatus(
                                                VAR(PduIdType, AUTOMATIC)       ComTxPduId,
                                                VAR(Std_ReturnType, AUTOMATIC)  ipduTransStatus_ui
                                                            )
{
    Com_TxIpduConstPtrType  TxIPduConstPtr;
    Com_TxIpduRamPtrType    TxIpduRamPtr;

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(ComTxPduId);
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(ComTxPduId);

    SchM_Enter_Com_TxIpduProtArea(SENDIPDU_TXFLAGS);

    if (ipduTransStatus_ui == E_OK)
    {
#ifdef COM_RETRY_FAILED_TX_REQUESTS
        /* If ComRetryFailedTransmitRequests is enabled and call to PduR_ComTransmit() returns E_OK,
         * then clear the failed Tx-IPdu requests */
        Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
#endif

        /* If ComMinimumDelayTime of an I-PDU is configured greater than 0, the AUTOSAR COM module shall load and
         * start the MDT counter upon transmission of that I PDU to the PDU Router via PduR_ComTransmit.
         * NOTE : Check (TxIpduRamPtr->Com_MinDelayTick == 0) is done because of the below reasons.
         * If interrupts are enabled, then interrupt could be called between the call of PduR_ComTransmit() above
         * and Com_TxConfirmation(). In this case Com_MinDelayTick will be overwritten here, leading to deadlock.
         */
        if ((TxIPduConstPtr->Min_Delay_Time_Fact != 0u) && (TxIpduRamPtr->Com_MinDelayTick == 0u))
        {
            TxIpduRamPtr->Com_MinDelayTick = TxIPduConstPtr->Min_Delay_Time_Fact +
                                             (Com_GetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags) ? 1u : 0u);
        }

        /* If the parameter ComTxIPduClearUpdateBit of an I-PDU is configured to Transmit, the AUTOSAR COM module
         * shall clear all update-bits of all contained signals and signal groups
         * after this I-PDU was sent out via PduR_ComTransmit and PduR_ComTransmit returned E_OK. */
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
        if (Com_GetValue(TXIPDU,_CLRUPDBIT,TxIPduConstPtr->txIPduFields) == (uint16)COM_CLRUPDATEBIT_TRANSMIT)
        {
            Com_Prv_ClearUpdateBits(TxIPduConstPtr);
        }
#endif /* #if defined ((COM_TxSigUpdateBit)||defined(COM_TxSigGrpUpdateBit)) */

#ifdef COM_TP_IPDUTYPE
        if (Com_GetValue(TXIPDU,_ISLARGEDATAPDU,TxIPduConstPtr->txIPduFields))
        {
            /* For TP-type Pdus, lock the buffer for further updates, until TP transmission is completed. */
            Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags,COM_TRUE);
            TxIpduRamPtr->TxTPIPduLength = 0;
        }
#endif /* #ifdef COM_TP_IPDUTYPE */

    }
#ifdef COM_RETRY_FAILED_TX_REQUESTS
    else
    {
        /* If ComRetryFailedTransmitRequests is enabled and call to PduR_ComTransmit() returns E_NOT_OK,
         * then store the failed Tx-IPdu requests */
        Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,TxIpduRamPtr->Com_TxFlags,COM_TRUE);
    } /* End of ipduTransStatus_ui check */
#endif
    SchM_Exit_Com_TxIpduProtArea(SENDIPDU_TXFLAGS);

} /* End of Com_Prv_ProcessTxReqStatus */


/**********************************************************************************************************************
 Function name    : Com_Prv_ClearUpdateBits
 Syntax           : void Com_Prv_ClearUpdateBits(Com_TxIpduConstPtrType TxIPduConstPtr)
 Description      : Internal function used for clearing the update bits of signals and signal groups in the IPdu
 Parameter        : TxIPduConstPtr -> Pointer to Tx ipdu static configuration
 Return value     : None
 *********************************************************************************************************************/
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void,COM_CODE) Com_Prv_ClearUpdateBits( Com_TxIpduConstPtrType TxIPduConstPtr )
{
#ifdef COM_TxSigUpdateBit
    Com_TxSigConstPtrType           TxSigConstPtr;
#endif
#ifdef COM_TxSigGrpUpdateBit
    Com_TxSigGrpConstPtrType        TxSigGrpConstPtr;
#endif
    VAR(uint16_least, AUTOMATIC)    index_ui;
    VAR(PduLengthType, AUTOMATIC)   ByteNo_uo;

    /* After an I-PDU is sent to lower layers and no synchronous error is returned
     * by the lower layer the update-bits of all signals and signal groups belonging
     * to the I-PDU sent shall be cleared.*/

#ifdef COM_TxSigUpdateBit
    /* Clear all Update Bits if configured in signals */
    TxSigConstPtr = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);

    for(index_ui = TxIPduConstPtr->No_Of_Sig_Ref; index_ui != 0u; index_ui--)
    {
        /* Check if UpdateBit is configure for the signal */
        if(Com_GetValue(GEN,_UPDBITCONF,TxSigConstPtr->General))
        {
            /* Find the Byte No, where the Update bit lies */
            ByteNo_uo = (PduLengthType)((TxSigConstPtr->Update_Bit_Pos) >> 3u);

            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(TxIPduConstPtr, TxSigConstPtr->Update_Bit_Pos, ByteNo_uo)
        }
        TxSigConstPtr++;
    }
#endif /* #ifdef COM_TxSigUpdateBit*/

#ifdef COM_TxSigGrpUpdateBit
    /* Clear all Update Bits if configured in signal groups */
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(TxIPduConstPtr->FirstTxSigGrp_Index);

    for(index_ui = TxIPduConstPtr->No_Of_SigGrp_Ref; index_ui != 0u; index_ui--)
    {
        /* Check if UpdateBit is configure for the signal group */
        if(Com_GetValue(TXSIGGRP,_UPDBITCONF,TxSigGrpConstPtr->txSignalGrpFields))
        {
            /* Find the Byte No, where the Update bit lies */
            ByteNo_uo = (PduLengthType)((TxSigGrpConstPtr->Update_Bit_Pos) >> 3u);

            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(TxIPduConstPtr, TxSigGrpConstPtr->Update_Bit_Pos, ByteNo_uo)
        }
        TxSigGrpConstPtr++;
    }
#endif /* #ifdef COM_TxSigGrpUpdateBit*/
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif/* #if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) */

