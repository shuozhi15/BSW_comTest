

/**********************************************************************************************************************
 * Preprocessor includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/
#ifdef COM_RxSigUpdateBit
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_ProcessSigUpdateBit(
                                        VAR(Com_SignalIdType,AUTOMATIC)                 SignalId,
                                        P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                        VAR(uint8, AUTOMATIC)                           UpdateBitStatus,
                                        P2VAR(uint16_least, AUTOMATIC, COM_APPL_DATA)   Idx_SigToutPtr
                                                                );
#endif

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_CopyRxSignal(
                                        VAR(Com_SignalIdType,AUTOMATIC)                 SignalId,
                                        P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                        VAR(Com_BitsizeType, AUTOMATIC)                 Size
                                                         );

#if defined(COM_RxSigInvalid) || defined (COM_RxFilters)
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_ValidateRxSignal(
                                        VAR(Com_SignalIdType, AUTOMATIC)                SignalId,
                                        P2VAR(Com_SigMaxType, AUTOMATIC, COM_APPL_DATA) RxNewValPtr
                                                            );
#endif

/**********************************************************************************************************************
 * Global functions (declared in header files )
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessSignal
 Syntax           : void Com_Prv_ProcessSignal( PduIdType RxPduId, const PduInfoType * PduInfoPtr )
 Description      : Process rx-signals of the received I-PDU.
 Parameter        : RxPduId    - ID of the received I-PDU.
                  : PduInfoPtr - Contains the length (SduLength) of the received I-PDU and
                                 a pointer to a buffer (SduDataPtr) containing the I-PDU.
 Return value     : None
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_ProcessSignal(
                                VAR(PduIdType, AUTOMATIC)                       RxPduId,
                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr
                                          )
{
    Com_RxIpduConstPtrType          RxIPduConstPtr;
    Com_RxSigConstPtrType           RxSigConstPtr;
    Com_RxSigRamPtrType             RxSigRamPtr;
    VAR(uint16_least, AUTOMATIC)    Idx_Sig_ui;
    VAR(uint16_least, AUTOMATIC)    limit_ui;
#ifdef COM_RxSigUpdateBit
    VAR(uint16_least, AUTOMATIC)    Idx_SigTout;
    VAR(uint8, AUTOMATIC)           UpdateBitStatus;
#endif
    /* This variable is used to decide whether to copy the new value of the signal into signal buffer or not */
    VAR(boolean, AUTOMATIC)         ProcessSignal_b;

    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(RxPduId);

#ifdef COM_RxSigUpdateBit
#ifdef COM_RxSigUpdateTimeout

    /* If this IPDU has signals with update-bits , initialise the pointers
     * Separate deadline monitoring should be performed for signals with update-bit
     * Deadline monitoring should not be done if it is disabled
     */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if ((RxIPduConstPtr->SignalTimeout_Ref != NULL_PTR) && (COM_CheckRxIPduDMStatus(RxPduId)))
    {

        /* Update the variable UpdateBitStatus with value UPDATEBIT_APPLICABLE
         * ie at least one of the signal within the IPDU is configured with both update-bit and deadline monitoring.
         */
        UpdateBitStatus = UPDATEBIT_APPLICABLE;
    }
    else
#endif /* #ifdef COM_RxSigUpdateTimeout */
    {
        UpdateBitStatus = UPDATEBIT_NOT_APPLICABLE;
    }

    /* Initialize the variable to be used to refer the first signal timeout configuration */
    Idx_SigTout = 0u;

#endif /* #ifdef COM_RxSigUpdateBit */

    limit_ui      = (uint16_least)RxIPduConstPtr->RxSigId_Index + (uint16_least)RxIPduConstPtr->No_Of_Sig_Ref;
    RxSigRamPtr   = &COM_GET_RXSIGNALFLAG(RxIPduConstPtr->RxSigId_Index);
    RxSigConstPtr = COM_GET_RXSIG_CONSTDATA(RxIPduConstPtr->RxSigId_Index);


    for (Idx_Sig_ui = (uint16_least)RxIPduConstPtr->RxSigId_Index; Idx_Sig_ui < limit_ui; Idx_Sig_ui++)
    {
        /* Initialise to TRUE i.e., proceed further to process the received signal */
        ProcessSignal_b = COM_TRUE;

#ifdef COM_RxSigUpdateBit
        /* Returns false if signal UpdateBit is configured and is received with reset state,
         * otherwise true is returned */
        if (Com_Prv_ProcessSigUpdateBit( (Com_SignalIdType)Idx_Sig_ui, PduInfoPtr, UpdateBitStatus, &Idx_SigTout ))
#endif /* #ifdef COM_RxSigUpdateBit */
        {
            VAR(Com_BitsizeType, AUTOMATIC)     Size;
            VAR(uint8, AUTOMATIC)               ConstByteValue_u8;
            VAR(uint8, AUTOMATIC)               SigType;
            VAR(uint8, AUTOMATIC)               SigEndianess;

            ConstByteValue_u8  = RxSigConstPtr->General;

            SigType            = Com_GetValue(GEN,_TYPE,ConstByteValue_u8);
            SigEndianess       = Com_GetValue(GEN,_ENDIANESS,ConstByteValue_u8);

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            if (SigType != COM_UINT8_DYN)
#endif
            {
                VAR(PduLengthType, AUTOMATIC) SigLastByteNo;

                Size = RxSigConstPtr->BitSize;

                /* Calculate the last byte where the signal span in the Rx-IPdu buffer
                 * COM_LITTLEENDIAN : MSB > LSB; i.e, signal last byte is same as the byte where MSB is configured.
                 * COM_BIGENDIAN    : LSB > MSB; i.e, signal last byte is same as the byte where LSB is configured.
                 */
                if (SigEndianess == COM_LITTLEENDIAN)
                {
                    VAR(Com_BitpositionType, AUTOMATIC) lastBitPosition;

                    /* Currently COM_OPAQUE is same as COM_LITTLEENDIAN and
                     * its SignalLength in bytes is stored in BitSize */
                    lastBitPosition = (RxSigConstPtr->Bit_Pos +
                                      ((SigType == COM_UINT8_N) ? ((Com_BitpositionType)Size * 8u) : Size));

                    SigLastByteNo   = (PduLengthType)((lastBitPosition != 0u) ? ((lastBitPosition - 1u) >> 3u) : (0u));
                }
                else
                {
                    /* COM_BIGENDIAN */
                    SigLastByteNo   = (PduLengthType)(RxSigConstPtr->Bit_Pos >> 3u);
                }

                /* [SWS_Com_00574] When unpacking an I-PDU, the AUTOSAR COM module shall check the received data length
                 * (PduInfoPtr->SduLength) and unpack and notify only completely received signals via ComNotification.
                 * Check if the Signal is part of the received rx pdu length */
                if ((SigLastByteNo + 1u) > PduInfoPtr->SduLength)
                {
                    /* Signal is not received or received partially, Ignore further processing */
                    ProcessSignal_b = COM_FALSE;
                }
            }
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            else
            {
                VAR(PduLengthType, AUTOMATIC) StartByteNo;

                /* Calculate Dynamic Signal Start Position */
                StartByteNo = (PduLengthType)(RxSigConstPtr->Bit_Pos >> 3u);

                /* Check if Dynamic Signal is received with atleast one byte */
                Size = ((COM_GET_RXPDURAM_S(RxPduId).RxIPduLength > StartByteNo) ?

                       /* Calculate its received length */
                       (Com_BitsizeType)( COM_GET_RXPDURAM_S(RxPduId).RxIPduLength - StartByteNo ) :

                       /* Design Decision: Even if 0 bytes are received for a dynamic signal in RxIPdu,
                        * configured Signal-ComNotification is always invoked.
                        * If it is not received, neither extract nor copy Dynamic Signal.
                        */
                       (0u));
            }
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */

            /* If complete signal is received, proceed to unpack and copy signal */
            if (ProcessSignal_b != COM_FALSE)
            {
                ProcessSignal_b = Com_Prv_CopyRxSignal((Com_SignalIdType)Idx_Sig_ui, PduInfoPtr, Size);
            }

        } /* End of Com_Prv_ProcessSigUpdateBit() check */

#ifdef COM_RxSignalNotify
        /* Set the _SIGNOTIF flag to invoke configured signal-ComNotification */
        Com_SetRamValue(RXSIG,_SIGNOTIF,RxSigRamPtr->rxSigRAMFields,ProcessSignal_b);
#else
        /* return value from CopyRxSignal() is not used, if Signal notifications are not configured */
        (void)(ProcessSignal_b);
#endif

        RxSigConstPtr++;
        RxSigRamPtr++;

    } /* End of for loop */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessSigUpdateBit
 Syntax           : boolean Com_Prv_ProcessSigUpdateBit( Com_SignalIdType SignalId, const PduInfoType * PduInfoPtr,
                                                         uint8 UpdateBitStatus,     uint16_least * Idx_SigToutPtr )
 Description      : Private function to validate signal UpdateBit
 Parameter        : SignalId        - ID of rx-signal.
                  : PduInfoPtr      - Contains the length (SduLength) of the received I-PDU and
                                      a pointer to a buffer (SduDataPtr) containing the I-PDU.
                    UpdateBitStatus - NOT_APPLICABLE: none of the rx-signals are configured with UB & TO
                                      APPLICABLE    : some of the rx-signals are configured with UB & TO
                    Idx_SigToutPtr  - Offset to be used to refer to the signal with UB & TO
 Return value     : false - UpdateBit is configured & received with reset state.
                    true  - other cases
 *********************************************************************************************************************/

#ifdef COM_RxSigUpdateBit
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(Idx_SigToutPtr) in a function prototype should be declared as pointer
 to variable, if COM_RxSigUpdateTimeout feature is enabled then the pointer will be modified.*/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_ProcessSigUpdateBit(
                                                    VAR(Com_SignalIdType,AUTOMATIC)                 SignalId,
                                                    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                                    VAR(uint8, AUTOMATIC)                           UpdateBitStatus,
                                                    P2VAR(uint16_least, AUTOMATIC, COM_APPL_DATA)   Idx_SigToutPtr
                                                                )
{
    Com_RxSigConstPtrType                                       RxSigConstPtr;
#ifdef COM_RxSigUpdateTimeout
    Com_RxIpduConstPtrType                                      RxIPduConstPtr;
    P2CONST(Com_SignalTimeoutInfo, AUTOMATIC, COM_APPL_CONST)   SigTimeOutConfigPtr;
    P2VAR(uint16, AUTOMATIC, COM_APPL_DATA)                     SigTimeOutTicksPtr;
#endif
    VAR(boolean, AUTOMATIC)                                     IsUpdateBitSet;

    /* Initialize update bit to true */
    IsUpdateBitSet = COM_TRUE;

    RxSigConstPtr  = COM_GET_RXSIG_CONSTDATA(SignalId);

    /* Check for the update bit if configured */
    if (Com_GetValue(GEN,_UPDBITCONF,RxSigConstPtr->General))
    {
        VAR(PduLengthType, AUTOMATIC) UB_ByteNo_u8;
        VAR(boolean, AUTOMATIC)       updateBitValue_b;

        UB_ByteNo_u8     = (PduLengthType)(RxSigConstPtr->Update_Bit_Pos >> 3u);

        updateBitValue_b = Com_GetUpdateBitValue(PduInfoPtr->SduDataPtr, RxSigConstPtr->Update_Bit_Pos, UB_ByteNo_u8);

        /* Start: Check if the Update bit is SET or NOT */
        if (((UB_ByteNo_u8 + 1u) <= PduInfoPtr->SduLength) && (updateBitValue_b == COM_FALSE))
        {
            /* [SWS_Com_00802] If a signal or signal group with an attached update bit is received and
             * the update-bit is not set, the AUTOSAR COM module shall discard this signal.
             * If the signal has not been updated, the signal will not be routed via the signal gateway.
             * It will only be discarded.
             * If the RTE reads a signal with an associated cleared update-bit, the init value or
             * the last received value is returned. */
            IsUpdateBitSet  = COM_FALSE;
        }
#ifdef COM_RxSigUpdateTimeout
        else
        {
            /* [SWS_Com_00794] In case of receiving of a smaller I-PDU than expected results into receiving a
             * signal without its configured update-bit, the AUTOSAR COM module shall treat this signal as if
             * its update bit was set and interpret such a signal as updated.
             * If UpdateBitStatus is UPDATEBIT_NOT_APPLICABLE(0x0), then it remains unchanged.
             * If UpdateBitStatus is UPDATEBIT_APPLICABLE(0x1), then it is changed to UPDATEBIT_SET(0x2) here.
             */
            UpdateBitStatus <<= 1u;
        }

        /* Check if deadline monitoring needs to be performed for signal with update-bit, within
         * this IPDU.
         * This variable would be set UBDATEBIT_APPLICABLE only if atleast one the signal group within
         * the IPDU has configured update-bit & timeout value and deadline monitoring for IPDU
         * is not disabled.
         * check if present signal with update-bit has configured time out.
         * Only the signals with update-bit and configured timeout will be present in structure
         * Com_SignalTimeoutInfo.
         */
        if (UpdateBitStatus != UPDATEBIT_NOT_APPLICABLE)
        {
            RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(RxSigConstPtr->IpduRef);

            /* Increment pointers to refer to current signal with update-bit & configured timeout
             * (within this IPDU) */
            SigTimeOutConfigPtr = (RxIPduConstPtr->SignalTimeout_Ref->TimeoutInfo_Ref) + (*Idx_SigToutPtr);

            if (SigTimeOutConfigPtr->SigId == (uint16)SignalId)
            {
                if (UpdateBitStatus == UPDATEBIT_SET)
                {
                    /* Increment pointer to refer to current signal timer
                     * (within this IPDU) */
                    SigTimeOutTicksPtr  = (RxIPduConstPtr->SignalTimeout_Ref->TimeoutTicks_p) + (*Idx_SigToutPtr);

                    /* Reload the timer for signal with TimeOut value
                     * Timer should be reloaded only if corresponding update-bit is set for the signal
                     * It is ensured by the code generator that if TimeOut value is zero, then the
                     * signal is not considered for deadline monitoring and hence an entry is the
                     * structure Com_SignalTimeoutInfo is not present.
                     * Hence there is no need to check for zero value of TimeOut.
                     */
                    SchM_Enter_Com_RxSigToTicks();

/* FC_VariationPoint_START */
#if (COM_ENABLE_JITTERFLITER == STD_ON)
                    /* CC requirements:
                     * An extra cycle is added to the timeout value after a PDU has been received incase filter is ON
                     * Provides a chance incase of jitter( extra cycle is added in normal case and hence
                     * if JITTERFILTER is enabled extra two cycles are added)
                     */
                    (*SigTimeOutTicksPtr) = SigTimeOutConfigPtr->Timeout + 2;
#else
/* FC_VariationPoint_END   */
                    /* Extra cycle is added to prevent early timeouts */
                    (*SigTimeOutTicksPtr) = SigTimeOutConfigPtr->Timeout + 1;
/* FC_VariationPoint_START */
#endif /* #ifdef COM_ENABLE_JITTERFLITER */
/* FC_VariationPoint_END   */

                    SchM_Exit_Com_RxSigToTicks();
                }

                /* Increment signal TO instance to refer to next signal with update-bit & configured timeout
                 * (within this IPDU) */
                (*Idx_SigToutPtr)++;

            } /* Current signal matches with the signal in the timeout configuration */

        } /* if (UpdateBitStatus != UPDATEBIT_NOT_APPLICABLE) */

#else
        /* unused signal timeout parameters */
        (void)(UpdateBitStatus);
        (void)(Idx_SigToutPtr);
#endif /* #ifdef COM_RxSigUpdateTimeout */

    } /* End of UpdateBit check */

    return IsUpdateBitSet;
}
#endif /* #ifdef COM_RxSigUpdateBit */

/**********************************************************************************************************************
 Function name    : Com_Prv_CopyRxSignal
 Syntax           : boolean Com_Prv_CopyRxSignal( Com_SignalIdType SignalId, const PduInfoType * PduInfoPtr,
                                              Com_BitsizeType  Size )
 Description      : Private function to unpack and copy rx-signal
 Parameter        : SignalId   - rx-signal to be processed
                    PduInfoPtr - Contains the length (SduLength) of the received I-PDU and
                                 a pointer to a buffer (SduDataPtr) containing the I-PDU.
                    Size       - received signal length
 Return value     : true  - signal is copied into buffer
                    false - signal is discarded
 *********************************************************************************************************************/
/* HIS METRIC CALLS VIOLATION IN Com_Prv_CopyRxSignal:
   Com_Prv_CopyRxSignal function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_CopyRxSignal(
                                              VAR(Com_SignalIdType,AUTOMATIC)                 SignalId,
                                              P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                              VAR(Com_BitsizeType, AUTOMATIC)                 Size
                                                         )
{
    Com_RxIpduConstPtrType                  RxIPduConstPtr;
    Com_RxSigConstPtrType                   RxSigConstPtr;
    Com_RxSigRamPtrType                     RxSigRamPtr;
    VAR(Com_SigMaxType, AUTOMATIC)          RxSigNewVal;
    VAR(PduIdType, AUTOMATIC)               RxPduId;
    VAR(Com_MainFuncType, AUTOMATIC)        rxMainFuncId;
    VAR(uint8, AUTOMATIC)                   ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)                   SigType;
    VAR(uint8, AUTOMATIC)                   SigEndianess;
    VAR(boolean, AUTOMATIC)                 copySignal_b;

    RxSigConstPtr      = COM_GET_RXSIG_CONSTDATA(SignalId);
    RxSigRamPtr        = &COM_GET_RXSIGNALFLAG(SignalId);

    RxPduId            = RxSigConstPtr->IpduRef;
    RxIPduConstPtr     = COM_GET_RX_IPDU_CONSTDATA(RxPduId);
    rxMainFuncId       = RxIPduConstPtr->MainFunctionRef; /* Fetch the Rx-MainFunction internal Id */

    ConstByteValue_u8  = RxSigConstPtr->General;

    SigType            = Com_GetValue(GEN,_TYPE,ConstByteValue_u8);
    SigEndianess       = Com_GetValue(GEN,_ENDIANESS,ConstByteValue_u8);

    RxSigNewVal        = 0u;
    copySignal_b       = COM_TRUE;

    /* 1. For signals with update bit;this part will be executed when the Update bit is SET
     * 2. For signals without update bit,process the signal further */
    /* Unpack the signal value */
/* FC_VariationPoint_START */
#ifdef COM_RXSIG_FLOAT64SUPP
    if (SigType == COM_FLOAT64)
    {
        VAR(float64, AUTOMATIC) RxSigNewVal_f64;

        RxSigNewVal_f64 = Com_UnPackFloatSignal( SigEndianess, RxSigConstPtr->Bit_Pos, PduInfoPtr->SduDataPtr );

        SchM_Enter_Com_RxSigBuff(RXINDICATION);
        /* Update the received data into the signal buffer */
        COM_GET_SIGTYPEFLOAT64_BUFF(rxMainFuncId,RxSigConstPtr->SigBuff_Index) = RxSigNewVal_f64;
        SchM_Exit_Com_RxSigBuff(RXINDICATION);
    }
    else
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */
/* FC_VariationPoint_END   */
    {
        /* Only UINT8_N and UINT8_DYN signals have type value greater than 8 */
        if ( !((SigType == COM_UINT8_N)
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
             ||(SigType == COM_UINT8_DYN)
#endif
            ))
        {
            RxSigNewVal = Com_UnpackSignal( SigEndianess, RxSigConstPtr->Bit_Pos, Size, PduInfoPtr->SduDataPtr,
                                                                                    ((SigType & 0x01u) != 0x0u));
        }
        else
        {
            RxSigNewVal = (Com_SigMaxType)Com_UnpackOpaqueSignal(RxSigConstPtr->Bit_Pos, Size, PduInfoPtr->SduDataPtr);
        }

#if defined(COM_RxSigInvalid) || defined(COM_RxFilters)

        /* In the following function
         * 1. Perform data Invalidation processing (REPLACE or NOTIFY)
         * 2. Signal filter processing
         */
        copySignal_b = Com_Prv_ValidateRxSignal( SignalId, &RxSigNewVal );

#endif /* #if defined(COM_RxSigInvalid) || defined(COM_RxFilters) */
    }

    /* Update the signal buffer,
     * For signals with    update bit : Signal update bit set + signal validation pass
     * For signals without update bit : Signal validation pass
     */
#if defined(COM_RxSigInvalid) || defined(COM_RxFilters)
    if (copySignal_b != COM_FALSE)
#endif /* #if defined(COM_RxSigInvalid) || defined(COM_RxFilters) */
    {
        /* Proceed to copy signal buffer only for non-zero size */
        if ((Size != 0u)
/* FC_VariationPoint_START */
#ifdef COM_RXSIG_FLOAT64SUPP
        /* Signal copy is done above for FLOAT64 type signals, so below it is done for other types */
        && (SigType != COM_FLOAT64)
#endif
/* FC_VariationPoint_END   */
           )
        {
            VAR(Com_SigBuffIndexType,AUTOMATIC) SigBuff_Index;

            SigBuff_Index = RxSigConstPtr->SigBuff_Index;

            /* NOTE on Exclusive area COM_RXINDICATION:
             * If there is a possibility that Com_RxIndication() can be interrupted by
             * Com_ReceiveSignal() or Com_Init() function for any IPdu,
             * then this exclusive area needs to be defined in Com_Cfg_SchM.h file.
             */
            /* Copy the new value of the signal into signal buffer */

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            if (SigType != COM_UINT8_DYN)
#endif
            {
                SchM_Enter_Com_RxSigBuff(RXINDICATION);

                if (SigType != COM_UINT8_N)
                {
                    /* Signal copy is done for rx signals */
                    Com_UpdateRxSignalBuffer(RxSigConstPtr, RxSigNewVal, rxMainFuncId);
                }
                else
                {
                    /* UINT8_N signal */
                    Com_ByteCopy( &COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId,SigBuff_Index),
                                  (PduInfoPtr->SduDataPtr + (RxSigConstPtr->Bit_Pos >> 3u)), Size );
                }

                SchM_Exit_Com_RxSigBuff(RXINDICATION);
            }
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
            else
            {
                SchM_Enter_Com_RxSigDynBuff();

                /* UINT8_DYN signal */
                Com_ByteCopy( &COM_GET_SIGTYPEUINT8DYN_BUFF(rxMainFuncId,SigBuff_Index),
                              (PduInfoPtr->SduDataPtr + (RxSigConstPtr->Bit_Pos >> 3u)), Size );

                SchM_Exit_Com_RxSigDynBuff();
            }
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */
        }

#ifdef COM_SIGNALGATEWAY
        /* This part of code updates the Rx Gateway queue If the signal is updated in the signal buffer by one
         * of the below means
         * 1. Reception of Ipdu and thereby signal update after update bit processing
         */
        if (Com_GetValue(RXSIG,_ISGWSIG,RxSigConstPtr->rxSignalFields))
        {
            /* If the signal is recieved first time after the last call of Com_MainfunctionRouteSignals then
             * 1. The fill bit per signal has to be set
             * 2. Queue entry has to be done for the signal
             * For the further receptions of the signal, Queue and signal fill bit are not set */
            Com_SetRamValue(RXSIG,_SIGNALFILLBIT,RxSigRamPtr->rxSigRAMFields,COM_TRUE);

            /* This flag will tell if any GW signals in the Ipdu are updated */
            COM_GET_IS_GWSIG_UPDATED( RxPduId ) = COM_TRUE;
        }
#endif /* #ifdef COM_SIGNALGATEWAY */

    } /* if (copySignal_b != COM_FALSE) */

    return copySignal_b;
}

/***************************************************************************************************
 Function name    : Com_Prv_ValidateRxSignal
 Syntax           : boolean Com_Prv_ValidateRxSignal(Com_SignalIdType SignalId, Com_SigMaxType * RxNewValPtr)
 Description      : Internal function used for validating Rx signal
 Parameter        : SignalId    -> Id of the Signal
                  : RxNewValPtr -> Pointer to New Signal value
 Return value     : true  - Copy new signal value into signal buffer
                    false - Do not copy new signal value into signal buffer
 ***************************************************************************************************/
#if defined(COM_RxSigInvalid) || defined (COM_RxFilters)

LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_ValidateRxSignal(
                                                VAR(Com_SignalIdType, AUTOMATIC)                SignalId,
                                                P2VAR(Com_SigMaxType, AUTOMATIC, COM_APPL_DATA) RxNewValPtr
                                                            )
{
    Com_RxSigConstPtrType   RxSigConstPtr;
    Com_RxSigRamPtrType     RxSigRamPtr;
    VAR(uint8, AUTOMATIC)   ConstByteValue_u8;
    VAR(boolean, AUTOMATIC) isValidRxSig_b;

    RxSigConstPtr     = COM_GET_RXSIG_CONSTDATA(SignalId);
    isValidRxSig_b    = COM_TRUE;

    RxSigRamPtr       = &COM_GET_RXSIGNALFLAG(SignalId);
    ConstByteValue_u8 = RxSigConstPtr->rxSignalFields;
    /*Check For the Data Invalid Action and the value*/
#ifdef COM_RxSigInvalid
    {
        VAR(uint8, AUTOMATIC) InvAction;

        InvAction = Com_GetValue(RXSIG,_INVACTION,ConstByteValue_u8);

        /* For Float signals the invalid value is type casted to UNIT32 before comaparing */
        /* Check if the signal has invalid value configured and the newly received value is invalid value */
        /*if (RxSigConstPtr->rxSignalFields.DataInvalidType != (uint8)COM_NONE) &&
         *   (RxSigConstPtr->DataInvalid_Val == *RxNewValPtr))*/
        if ((InvAction != (uint8)COM_NONE) && ((Com_SigMaxType)RxSigConstPtr->DataInvalid_Val == *RxNewValPtr))
        {
            if ( InvAction == (uint8)COM_REPLACE )
            {
                VAR(Com_SigMaxType,AUTOMATIC) RxSigVal;

                /* Sign extension is performed on Init value during the below copy */
                /* Read the initial value */
                RxSigVal = (Com_SigMaxType)RxSigConstPtr->Init_Val;

#ifdef COM_RXSIG_INT64
                /* [SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
                 * to the size of its ComSignalType.
                 * Init_Val by default will be sign extended only for 32bits. Currently for COM_SINT64 signal types,
                 * if the signal is of Negative, it is required to perform the sign extension till 64bits explicity. */
                if ( Com_GetValue(GEN,_TYPE,RxSigConstPtr->General) == COM_SINT64 )
                {
                    RxSigVal = (Com_SigMaxType)((sint64)((sint32)RxSigVal));
                }
                else
                {
                    /* do nothing for other signal types */
                }
#endif /* #ifdef COM_RXSIG_INT64 */

                /* [SWS_Com_00681] The AUTOSAR COM module shall replace the signals value by its configured
                 * ComSignalInitValue. After the replacement, the normal signal processing like filtering and
                 * notification shall take place as if the ComSignalInitValue would have been received instead of
                 * the ComSignalDataInvalidValue */
                (*RxNewValPtr) = RxSigVal;
            }
            /* If invalid value is not configured ie if flag rxSignalFields.DataInvalidType is COM_NONE, then this
             * check is performed at the beginning itself.
             * Hence the other value will be COM_NOTIFY.
             */
#ifdef COM_RxSigInvalidNotify
            else
            {
                /* [SWS_Com_00680] The AUTOSAR COM module shall notify the RTE via the configured
                 * ComInvalidNotification function. In this case, no other signal processing like filtering or the
                 * normal signal indication shall take place. */
                /* Signal is invalid,and the Invalid Action is configured
                 * hence do not copy new value of signal into signal buffer */
                isValidRxSig_b = COM_FALSE;
                /* If this flag is set, do not call regular signal notification function */
                /* Call Invalid Notification */
                if(RxSigConstPtr->Sig_DataInvalid_Indication_Cbk != NULL_PTR)
                {
                    RxSigConstPtr->Sig_DataInvalid_Indication_Cbk();
                }
                /* In this case, the signal buffer should contain the previous value of the signal,
                 * leave the signal buffer unchanged
                 */
            }
#endif /* #ifdef COM_RxSigInvalidNotify */
        }
    }
#endif /* #ifdef COM_RxSigInvalid */

#ifdef COM_RxFilters
    {
        VAR(uint8, AUTOMATIC) filterAlgo_u8;

        filterAlgo_u8 = Com_GetValue(RXSIG,_FILTRALG,ConstByteValue_u8);

        /* Proceed only if the signal filter is configured */
        if ( ( COM_FALSE != isValidRxSig_b ) && ( COM_NOTCONFIGURED != filterAlgo_u8 ) )
        {
            /* The signal is valid, hence perform filtering */
            /* Get The Return value of the Filter which need not be returned if signal is invalid */
            /* The last parameter is Information from where the call to Com_Filter happens */
            /* NOTE on last parameter:
            * 1.Currently the values for the parameter callContext_u8 are COM_TRUE/COM_FALSE
            * 2.It is used only for filter ONE_EVERYN */
#if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout))
            /* If the filter algo is COM_MASKED_NEW_DIFFERS_MASKED_OLD and the first valid reception of the signal after
             * the reception deadline monitoring timeout should not be filtered out */
            if(!((Com_GetRamValue(RXSIG,_DMFILTERING,RxSigRamPtr->rxSigRAMFields) == COM_TRUE) &&
                 (COM_MASKED_NEW_DIFFERS_MASKED_OLD == filterAlgo_u8)))
            {
#endif /* #if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout)) */
                isValidRxSig_b = Com_Filter((uint32)(*RxNewValPtr), filterAlgo_u8, RxSigConstPtr->Filter_Index,
                                                                                                     COM_TRUE);
#if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout))
            }
            else
            {
                /* Resetting this flag, for further reception of the signal, if Rx deadline monitoring timer elapses,
                 * again this flag shall be set in Com_MainFunctionRx() */
                Com_SetRamValue(RXSIG,_DMFILTERING,RxSigRamPtr->rxSigRAMFields,COM_FALSE);
            }
#endif /* #if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined(COM_RxSigUpdateTimeout)) */
        }
    }
#endif /* #ifdef COM_RxFilters */

    return (isValidRxSig_b);
}
#endif /* #if defined(COM_RxSigInvalid) || defined (COM_RxFilters) */

