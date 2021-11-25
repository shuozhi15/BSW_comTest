

/**********************************************************************************************************************
 * Preprocessor includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/* This file contains signal groups processing from the received Pdu.
 * Hence complete file contents have to be placed inside this macro */
#ifdef COM_RX_SIGNALGROUP
/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/
#ifdef COM_RxSigGrpUpdateBit
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_ProcessSigGrpUpdateBit(
                                                VAR(Com_SignalGroupIdType,AUTOMATIC)            SigGrpId,
                                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                                VAR(uint8, AUTOMATIC)                           UpdateBitStatus,
                                                P2VAR(uint16_least, AUTOMATIC, COM_APPL_DATA)   Idx_SigToutPtr
                                                                   );
#endif

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_IpduBuffToSecondBuff(
                                                VAR(Com_SignalGroupIdType,AUTOMATIC)            SigGrpId,
                                                P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)        BufferPtr,
                                                VAR(uint8,AUTOMATIC)                            ValidateSigGrp_u8
                                                              );

#if defined(COM_RxSigGrpInvalid) || defined (COM_RxFilters)
LOCAL_INLINE FUNC(uint8,COM_CODE) Com_Prv_ValidateRxSignalGroup(
                                                VAR(Com_SignalGroupIdType,AUTOMATIC)            SigGrpId,
                                                P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)        BufferPtr
                                                               );

LOCAL_INLINE FUNC(uint8,COM_CODE) Com_Prv_ValidateRxGroupSignal(VAR(uint16_least, AUTOMATIC)    Idx_GrpSig_ui,
                                                                VAR(Com_SigMaxType, AUTOMATIC)  RxNewVal_GrpSig);
#endif


LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopySecondaryBuffer(Com_RxSigGrpConstPtrType RxSigGrpConstPtr,
                                                              P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) Buffer);

/**********************************************************************************************************************
 * Global functions (declared in header files )
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessSignalGroup
 Syntax           : void Com_Prv_ProcessSignalGroup(
                                          VAR(PduIdType, AUTOMATIC)                     RxPduId,
                                          P2CONST(PduInfoType, AUTOMATIC,COM_APPL_DATA) PduInfoPtr
                                                   )
 Description      : Process rx-signal groups of the received I-PDU.
 Parameter        : ComRxPduId - ID of the received I-PDU.
                  : PduInfoPtr - Contains the length (SduLength) of the received I-PDU and
                                 a pointer to a buffer (SduDataPtr) containing the I-PDU.
 Return value     : None
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_ProcessSignalGroup(
                                VAR(PduIdType, AUTOMATIC)                      RxPduId,
                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr
                                               )
{
    Com_RxIpduConstPtrType          RxIPduConstPtr;
    Com_RxSigGrpConstPtrType        RxSigGrpConstPtr;
    Com_RxSigGrpRamPtrType          RxSigGrpRamPtr;
    VAR(uint16_least, AUTOMATIC)    Idx_SigGrp_ui;
    VAR(uint16_least, AUTOMATIC)    limit_ui;
    VAR(uint8, AUTOMATIC)           ValidateSigGrp_u8;
#ifdef COM_RxSigGrpUpdateBit
    VAR(uint16_least, AUTOMATIC)    Idx_SigTout;
    VAR(uint8, AUTOMATIC)           UpdateBitStatus;
#endif

    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(RxPduId);

#ifdef COM_RxSigGrpUpdateBit
#ifdef COM_RxSigGrpUpdateTimeout

    /* If this IPDU has signal groups with update-bits , initialise the pointers
     * Separate deadline monitoring should be performed for signal groups with update-bit
     * Deadline monitoring should not be done if it is disabled
     */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
     * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
     * system behaviour. Hence suppressed. */
    if ((RxIPduConstPtr->SignalGrpTimeout_Ref != NULL_PTR) && (COM_CheckRxIPduDMStatus(RxPduId)))
    {

        /* Update the variable UpdateBitStatus with value UPDATEBIT_APPLICABLE. ie at least one of the signal group
         * within the IPDU is configured with both update-bit and deadline monitoring.
         */
        UpdateBitStatus = UPDATEBIT_APPLICABLE;
    }
    else
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */
    {
        /* Reset the variable to default value ie UPDATEBIT_NOT_APPLICABLE */
        UpdateBitStatus = UPDATEBIT_NOT_APPLICABLE;
    }

    /* Initialize the variable to be used to refer the first signal timeout configuration */
    Idx_SigTout = 0u;

#endif /* #ifdef COM_RxSigGrpUpdateBit */

    limit_ui          = (uint16_least)RxIPduConstPtr->FirstRxSigGrp_Index +
                        (uint16_least)RxIPduConstPtr->No_Of_SigGrp_Ref;
    RxSigGrpConstPtr  = COM_GET_RXSIGGRP_CONSTDATA(RxIPduConstPtr->FirstRxSigGrp_Index);
    RxSigGrpRamPtr    = &COM_GET_RXSIGGRPFLAG(RxIPduConstPtr->FirstRxSigGrp_Index);

    /* Start processing for signal groups. */
    for (Idx_SigGrp_ui = RxIPduConstPtr->FirstRxSigGrp_Index; Idx_SigGrp_ui < limit_ui; Idx_SigGrp_ui++)
    {
        ValidateSigGrp_u8  = COM_SET;

        /* SignalGroup is processed only if below conditions are satisfied:
         * 1. Update bit validation for signal groups with Update bit configured.
         * 2. Completely received SignalGroup [SWS_Com_00575].
         */
        if (
#ifdef COM_RxSigGrpUpdateBit
            (Com_Prv_ProcessSigGrpUpdateBit( (Com_SignalGroupIdType)Idx_SigGrp_ui, PduInfoPtr,
                                                               UpdateBitStatus, &Idx_SigTout )) &&
#endif
            (RxSigGrpConstPtr->lastByte_u8 <= PduInfoPtr->SduLength)
           )
        {
#if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters)

            /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
             * to store and read here, may read obsolete value. Evaluation(or no evaluation) shall not impact the
             * system behaviour. Hence suppressed. */
            if ((Com_GetValue(RXSIGGRP,_INVACTION,RxSigGrpConstPtr->rxSignalGrpFields) != (uint8)COM_NONE)
#ifdef COM_RxFilters
               || (Com_GetValue(RXSIGGRP,_FILTEREVALREQ,RxSigGrpConstPtr->rxSignalGrpFields) != COM_FALSE)
#endif
               )
            {
                ValidateSigGrp_u8 = Com_Prv_ValidateRxSignalGroup((Com_SignalGroupIdType)Idx_SigGrp_ui,
                                                                               PduInfoPtr->SduDataPtr);
            }

            if (ValidateSigGrp_u8 != (uint8)COM_RESET)
#endif /* #if defined(COM_RxSigGrpInvalid) || defined(COM_RxFilters) */
            {
                /* The processed data is copied into second buffer, because, if the API Com_Recivesignalgroup() is
                 * not interrupt safe there can be data corruption on the group signal values. */
                Com_Prv_IpduBuffToSecondBuff((Com_SignalGroupIdType)Idx_SigGrp_ui, PduInfoPtr->SduDataPtr,
                                                                                       ValidateSigGrp_u8);

#ifdef COM_SIGNALGROUPGATEWAY

                if (Com_GetValue(RXSIGGRP,_ISGWSIGGRP,RxSigGrpConstPtr->rxSignalGrpFields))
                {
                    /* This part of code updates the Rx Gateway queue If the signal group is to be updated into
                     * signalgroup buffer by one of the below means
                     * 1. Reception of Ipdu and thereby signal update after update bit processing
                     */
                    /* If the signal is recieved first time after the last call of Com_MainfunctionRouteSignals then
                     * 1. The fill bit per signal has to be set
                     * 2. Queue entry has to be done for the signal
                     * For the further receptions of the signal, Queue and signal fill bit are not set */
                    Com_SetRamValue(RXSIGGRP,_SIGNALFILLBIT,RxSigGrpRamPtr->rxSigGrpRAMFields,COM_TRUE);

                    /* This flag will tell if any GW signal groups in the Ipdu are updated */
                    COM_GET_IS_GWSIG_UPDATED( RxPduId ) = COM_TRUE;
                }
#endif /* #ifdef COM_SIGNALGROUPGATEWAY */

            }
        }
        else
        {
            /* Either Update bit is received with reset state or signalgroup is partially received */
            ValidateSigGrp_u8 = COM_RESET;
        }

#ifdef COM_RxSignalGrpNotify
        /* Set _SIGNOTIF flag to invoke configured SignalGroup-ComNotification */
        Com_SetRamValue(RXSIGGRP,_SIGNOTIF,RxSigGrpRamPtr->rxSigGrpRAMFields,(ValidateSigGrp_u8 != (uint8)COM_RESET));
#endif

        RxSigGrpConstPtr++;
        RxSigGrpRamPtr++;
    }/* End of for loop */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessSigGrpUpdateBit
 Syntax           : boolean Com_Prv_ProcessSigGrpUpdateBit(
                                        VAR(Com_SignalIdType,AUTOMATIC)                 SignalId,
                                        P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                        VAR(uint8, AUTOMATIC)                           UpdateBitStatus,
                                        P2VAR(uint16_least, AUTOMATIC, COM_APPL_DATA)   Idx_SigToutPtr
                                                          )
 Description      : Private function to validate signalgroup UpdateBit
 Parameter        : SigGrpId        - ID of rx-signal group.
                  : PduInfoPtr      - Contains the length (SduLength) of the received I-PDU and
                                      a pointer to a buffer (SduDataPtr) containing the I-PDU.
                    UpdateBitStatus - NOT_APPLICABLE: none of the rx-signals are configured with UB & TO
                                      APPLICABLE    : some of the rx-signals are configured with UB & TO
                    Idx_SigToutPtr -  Offset to be used to refer to the signal with UB & TO
 Return value     : false - UpdateBit is configured & received with reset state.
                    true  - other cases
 *********************************************************************************************************************/
#ifdef COM_RxSigGrpUpdateBit
/* MR12 RULE 8.13 VIOLATION: A pointer parameter(Idx_SigToutPtr) in a function prototype should be declared as pointer
 to variable, if COM_RxSigGrpUpdateTimeout feature is enabled then the pointer will be modified.*/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_ProcessSigGrpUpdateBit(
                                                    VAR(Com_SignalGroupIdType,AUTOMATIC)            SigGrpId,
                                                    P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr,
                                                    VAR(uint8, AUTOMATIC)                           UpdateBitStatus,
                                                    P2VAR(uint16_least, AUTOMATIC, COM_APPL_DATA)   Idx_SigToutPtr
                                                                   )
{
    Com_RxSigGrpConstPtrType                                    RxSigGrpConstPtr;
#ifdef COM_RxSigGrpUpdateTimeout
    Com_RxIpduConstPtrType                                      RxIPduConstPtr;
    P2CONST(Com_SignalTimeoutInfo, AUTOMATIC, COM_APPL_CONST)   SigTimeOutConfigPtr;
    P2VAR(uint16, AUTOMATIC, COM_APPL_DATA)                     SigTimeOutTicksPtr;
#endif
    VAR(boolean, AUTOMATIC)                                     IsUpdateBitSet;

    /* Initialize update bit to true */
    IsUpdateBitSet = COM_TRUE;

    RxSigGrpConstPtr  = COM_GET_RXSIGGRP_CONSTDATA(SigGrpId);

    /* Check if the signal group contained within this IPDU has update-bit configured */
    if (Com_GetValue(RXSIGGRP,_UPDBITCONF,RxSigGrpConstPtr->rxSignalGrpFields))
    {
        VAR(PduLengthType, AUTOMATIC)  UB_ByteNo_u8;
        VAR(boolean, AUTOMATIC)        updateBitValue_b;

        /* Find the byte No of this Update bit */
        UB_ByteNo_u8     = (PduLengthType)(RxSigGrpConstPtr->Update_Bit_Pos >> 3u);

        updateBitValue_b = Com_GetUpdateBitValue( PduInfoPtr->SduDataPtr,
                                                  RxSigGrpConstPtr->Update_Bit_Pos,
                                                  UB_ByteNo_u8 );

        /* Start: Check if the Update bit is SET or NOT */
        if (((UB_ByteNo_u8 + 1u) <= PduInfoPtr->SduLength) && (updateBitValue_b == COM_FALSE))
        {
            /* [SWS_Com_00802] If a signal or signal group with an attached update bit is received and
             * the update-bit is not set, the AUTOSAR COM module shall discard this signal.
             * If the signal has not been updated, the signal will not be routed via the signal gateway.
             * It will only be discarded.
             */
            IsUpdateBitSet = COM_FALSE;
        }
#ifdef COM_RxSigGrpUpdateTimeout
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

        /* Check if deadline monitoring needs to be performed for signal groups with update-bit, within
         * this IPDU.
         * This variable would be set UBDATEBIT_APPLICABLE only if atleast one the signal group within
         * the IPDU has configured update-bit & timeout value and deadline monitoring for IPDU
         * is not disabled.
         * check if present signal group with update-bit has configured time out.
         * Only the signals with update-bit and configured timeout will be present in structure
         * Com_SignalTimeoutInfo.
         */
        if (UpdateBitStatus != UPDATEBIT_NOT_APPLICABLE)
        {
            RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(RxSigGrpConstPtr->IpduRef);

            /* Increment pointers to refer to current signal with update-bit & configured timeout
             * (within this IPDU) */
            SigTimeOutConfigPtr = (RxIPduConstPtr->SignalGrpTimeout_Ref->TimeoutInfo_Ref) + (*Idx_SigToutPtr);

            if (SigTimeOutConfigPtr->SigId == (uint16)SigGrpId)
            {
                if (UpdateBitStatus == UPDATEBIT_SET)
                {
                    /* Increment pointer to refer to current signal timer
                     * (within this IPDU) */
                    SigTimeOutTicksPtr  = (RxIPduConstPtr->SignalGrpTimeout_Ref->TimeoutTicks_p) + (*Idx_SigToutPtr);

                    /* Reload the timer for signal with Timeout value
                     * Timer should be reloaded only if corresponding update-bit is set for the signal group
                     * It is ensured by the code generator that if TimeOut value is zero, then the
                     * signal group is not considered for deadline monitoring and hence an entry is the
                     * structure Com_SignalTimeoutInfo is not present.
                     * Hence there is no need to check for zero value of Timeout.
                     */
                    SchM_Enter_Com_RxSigGrpToTicks();

/* FC_VariationPoint_START */
#if (COM_ENABLE_JITTERFLITER == STD_ON)
                    /* CC requirements:
                     * An extra cycle is added to the timeout value after a PDU has been received incase filter is ON
                     * Provides a chance incase of jitter(extra cycle is added in normal case and hence
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

                    SchM_Exit_Com_RxSigGrpToTicks();
                }

                /* Increment signal TO instance to refer to next signal with update-bit & configured timeout
                 * (within this IPDU) */
                (*Idx_SigToutPtr)++;

            } /* Current signalGroup matches with the signal in the timeout configuration */

        } /* if (UpdateBitStatus != UPDATEBIT_NOT_APPLICABLE) */

#else
        /* unused signal timeout parameters */
        (void)(UpdateBitStatus);
        (void)(Idx_SigToutPtr);
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */

    } /* End of UpdateBit check */

    return IsUpdateBitSet;
}
#endif /* #ifdef COM_RxSigGrpUpdateBit */

/***************************************************************************************************
 Function name    : Com_Prv_IpduBuffToSecondBuff
 Syntax           : void Com_Prv_IpduBuffToSecondBuff (VAR(Com_SignalGroupIdType,AUTOMATIC) SigGrpId,
                        P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)  BufferPtr, VAR(uint8,AUTOMATIC) ValidateSigGrp_u8)
 Description      : Internal function used for copying the signal group contents from IPdu buffer to second buffer
 Parameter        : SigGrpId          -> Signal Group Id
                  : BufferPtr         -> Pointer to Rx SignalGroup static configuration
                  : ValidateSigGrp_u8 -> Flag to tell if the group signal value should be copied
                                         with Initial values/received value
 Return value     : None
 ***************************************************************************************************/

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_IpduBuffToSecondBuff(VAR(Com_SignalGroupIdType,AUTOMATIC) SigGrpId,
                                                               P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)  BufferPtr,
                                                               VAR(uint8,AUTOMATIC)  ValidateSigGrp_u8 )
{
    /* Local pointer to hold the address of Rx SignalGroup static configuration */
    Com_RxSigGrpConstPtrType            RxSigGrpConstPtr;

    VAR(Com_MainFuncType, AUTOMATIC)    rxMainFuncId;


    RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA(SigGrpId);

    /* Fetch the Rx-MainFunction internal Id */
    rxMainFuncId     = (COM_GET_RX_IPDU_CONSTDATA(RxSigGrpConstPtr->IpduRef))->MainFunctionRef;

    if (ValidateSigGrp_u8 != COM_REPLACE_TO_INIT)
    {
# ifdef COM_RX_SIGNALGROUP_ARRAY
        if (Com_GetValue(RXSIGGRP,_ARRAYACCESS,RxSigGrpConstPtr->rxSignalGrpFields))
        {
            Com_RxSigGrpArrayConstPtrType RxSigGrpArrayConstPtr; /* Pointer to Rx SignalGroup Array configuration*/
            uint8                       * RxSigGrpArrayBufRamPtr;

            RxSigGrpArrayConstPtr  = COM_GET_RXSIGGRP_ARRAY_CONSTDATA( RxSigGrpConstPtr->SigGrpArray_Index );

            RxSigGrpArrayBufRamPtr =
                               &COM_GET_RXSIGGRP_ARRAY_BUFF( rxMainFuncId, RxSigGrpArrayConstPtr->RxSigGrpBuf_Index );

            SchM_Enter_Com_RxSigGrpArrayBuff();

            /* Copy GroupSignal's received data into RxSignalGroupArray Buffer from IPdu Buffer.
             * As it is UINT8 array copy, so RxSignalGroupArray Buffer can also hold other
             * interleaved Signal's received data of the same IPdu */
            Com_ByteCopy((uint8 *)(RxSigGrpArrayBufRamPtr),
                          (const uint8 *)(BufferPtr + RxSigGrpArrayConstPtr->FirstByteNo),
                                           ( RxSigGrpConstPtr->lastByte_u8 - RxSigGrpArrayConstPtr->FirstByteNo));

            SchM_Exit_Com_RxSigGrpArrayBuff();
        }
        else
# endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */
        {
            Com_Prv_CopySecondaryBuffer(RxSigGrpConstPtr, BufferPtr);
        }
    }
    else
    {
        /* Discard GroupSignal's received data and place its SignalInitValues in RxSignalGroupArray Buffer */
        Com_Prv_SigGrpReplaceToInit(RxSigGrpConstPtr, rxMainFuncId);
    }
}

/***************************************************************************************************
 Function name    : Com_Prv_CopySecondaryBuffer
 Syntax           : void Com_Prv_CopySecondaryBuffer(Com_RxSigGrpConstPtrType  RxSigGrpConstPtr,
                                                   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)  Buffer)
 Description      : Internal function used for copying the signal group contents from IPdu buffer to second buffer
 Parameter        : RxSigGrpConstPtr  -> Pointer to Rx SignalGroup static configuration
                  : Buffer            -> Address where the data is available
 Return value     : None
 ***************************************************************************************************/
/* HIS METRIC CALLS VIOLATION IN Com_Prv_CopySecondaryBuffer:
   Com_Prv_CopySecondaryBuffer function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopySecondaryBuffer(Com_RxSigGrpConstPtrType RxSigGrpConstPtr,
                                                               P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) Buffer)
{

    /* Local pointer to hold the address of Rx GroupSignal static configuration */
    Com_RxGrpSigConstPtrType            RxGrpSigConstPtr;
    VAR(uint16_least, AUTOMATIC)        index_ui;
    VAR(Com_MainFuncType, AUTOMATIC)    rxMainFuncId;
    VAR(uint8, AUTOMATIC)               SigType;

    RxGrpSigConstPtr = COM_GET_RXGRPSIG_CONSTDATA(RxSigGrpConstPtr->FirstGrpSig_Index);

    /* Fetch the Rx-MainFunction internal Id */
    rxMainFuncId     = (COM_GET_RX_IPDU_CONSTDATA(RxSigGrpConstPtr->IpduRef))->MainFunctionRef;

    SchM_Enter_Com_RxGrpSigSecBuff();
    for (index_ui = (uint16_least)RxSigGrpConstPtr->No_Of_GrpSig; index_ui != (uint16_least)0; index_ui--)
    {
        /* SigType = RxGrpSigConstPtr->rxGrpSigFields.SigType */
        SigType = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);

        if(SigType != (uint8)COM_UINT8_N)
        {
            VAR(uint8, AUTOMATIC) SigEndianess;

            SigEndianess = Com_GetValue(RXGRPSIG,_ENDIANESS,RxGrpSigConstPtr->rxGrpSigFields);

/* FC_VariationPoint_START */
# ifdef COM_RXGRPSIG_FLOAT64SUPP
            if(SigType == COM_FLOAT64)
            {
                float64 RxNewVal_f64;
                /* RxNewVal_f64 = Com_UnPackFloatSignal(RxGrpSigConstPtr->rxGrpSigFields.Endianess,
                 *                                               RxGrpSigConstPtr->Bit_Pos,Buffer); */
                RxNewVal_f64 = Com_UnPackFloatSignal( SigEndianess, RxGrpSigConstPtr->Bit_Pos, Buffer );

                COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(rxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) =
                                                                                                  RxNewVal_f64;
            }
            else
# endif
/* FC_VariationPoint_END */
            {
                VAR(Com_SigMaxType,AUTOMATIC) RxGrpSig_NewVal;

                RxGrpSig_NewVal = Com_UnpackSignal( SigEndianess,
                                                    RxGrpSigConstPtr->Bit_Pos,
                                                    RxGrpSigConstPtr->BitSize, Buffer,
                                                    ((SigType & (uint8)0x1)) != 0x0u);

                /* Now copy corresponding Group signal buffer with the received value */
                Com_UpdateRxGroupSignalSecondBuffer(RxGrpSigConstPtr, RxGrpSig_NewVal, rxMainFuncId);
            }
        }
        else
        {
            Com_ByteCopy( &COM_GET_SECRXGRPSIGTYPEU8_BUFF(rxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index),
                            (Buffer +(RxGrpSigConstPtr->Bit_Pos >> 3u)),
                             RxGrpSigConstPtr->BitSize );
        }
        RxGrpSigConstPtr++;
    }
    SchM_Exit_Com_RxGrpSigSecBuff();
}

#if defined(COM_RxSigGrpInvalid) || defined (COM_RxFilters)

/**********************************************************************************************************************
 Function name    : Com_Prv_ValidateRxSignalGroup
 Syntax           : void Com_Prv_ValidateRxSignalGroup(Com_SignalGroupIdType SigGrpId, const uint8 * BufferPtr)
 Description      : Private function to validate and process the configured filters for the given signal group
 Parameter        : SigGrpId  - Signal Group Id to be validated
                    BufferPtr - IPdu buffer pointer
 Return value     : COM_RESET            - discard signalgroup
                    COM_SET              - valid signalgroup is received, copy into local buffer
                    COM_REPLACE_TO_INIT  - replace groupsignals with SignalInit values
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(uint8,COM_CODE) Com_Prv_ValidateRxSignalGroup(
                                                VAR(Com_SignalGroupIdType, AUTOMATIC)       SigGrpId,
                                                P2CONST(uint8, AUTOMATIC, COM_APPL_DATA)    BufferPtr
                                                               )
{
    /* Local pointer to Rx GroupSignal static configuration */
    Com_RxGrpSigConstPtrType            RxGrpSigConstPtr;
    /* Local pointer to Rx SignalGroup static configuration */
    Com_RxSigGrpConstPtrType            RxSigGrpConstPtr;
    /* Local RAM pointer to SignalGroup RAM structure */
    Com_RxSigGrpRamPtrType              RxSigGrpRamPtr;
    VAR(Com_SigMaxType, AUTOMATIC)      RxNewVal_GrpSig;
    VAR(uint16_least, AUTOMATIC)        Idx_GrpSig_ui;
    VAR(uint16_least, AUTOMATIC)        LastGrpSigId;
    VAR(uint8, AUTOMATIC)               ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)               ValidateSigGrp_u8;
    VAR(uint8, AUTOMATIC)               SigType;
#ifdef COM_RxFilters
    VAR(uint8, AUTOMATIC)               filterAlgo_u8;
#endif
    VAR(boolean, AUTOMATIC)             InvAction_b;

    ValidateSigGrp_u8 = COM_SET;
    RxNewVal_GrpSig   = 0;

    RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA(SigGrpId);
    RxGrpSigConstPtr = COM_GET_RXGRPSIG_CONSTDATA(RxSigGrpConstPtr->FirstGrpSig_Index);
    RxSigGrpRamPtr   = &COM_GET_RXSIGGRPFLAG(SigGrpId);

    LastGrpSigId = RxSigGrpConstPtr->FirstGrpSig_Index + RxSigGrpConstPtr->No_Of_GrpSig;
    for( Idx_GrpSig_ui = RxSigGrpConstPtr->FirstGrpSig_Index;
         ((ValidateSigGrp_u8 == COM_SET) && (Idx_GrpSig_ui < LastGrpSigId)); Idx_GrpSig_ui++)
    {
        ConstByteValue_u8 = RxGrpSigConstPtr->rxGrpSigFields;
#ifdef COM_RxFilters
        filterAlgo_u8 = RxGrpSigConstPtr->Filter_Algo;
#endif
        InvAction_b   = Com_GetValue(RXGRPSIG,_INVACTION,ConstByteValue_u8);

        /* if(RxGrpSigConstPtr->rxGrpSigFields.GrpSigInvalid != COM_NONE) */
        if((InvAction_b != COM_FALSE)
#ifdef COM_RxFilters
                || (filterAlgo_u8 != COM_NOTCONFIGURED)
#endif
                )
        {
            /* SigType = RxGrpSigConstPtr->rxGrpSigFields.Type */
            SigType = Com_GetValue(RXGRPSIG,_TYPE,ConstByteValue_u8);

            /*Unpack the group signal value */
            if(SigType == (uint8)COM_UINT8_N)
            {
                RxNewVal_GrpSig = (Com_SigMaxType)Com_UnpackOpaqueSignal(RxGrpSigConstPtr->Bit_Pos,
                                                                         RxGrpSigConstPtr->BitSize, BufferPtr);
            }
            /* FC_VariationPoint_START */
#ifdef COM_RXGRPSIG_FLOAT64SUPP
            else if(SigType == COM_FLOAT64)
            {
                /* Do nothing */
            }
#endif
            /* FC_VariationPoint_END */
            else
            {
                RxNewVal_GrpSig = Com_UnpackSignal(Com_GetValue(RXGRPSIG,_ENDIANESS,ConstByteValue_u8),
                                                    RxGrpSigConstPtr->Bit_Pos ,RxGrpSigConstPtr->BitSize, BufferPtr,
                                                    ((SigType & 0x01u) != 0x0u));
            }

            ValidateSigGrp_u8 = Com_Prv_ValidateRxGroupSignal(Idx_GrpSig_ui, RxNewVal_GrpSig);
        }

        RxGrpSigConstPtr++;
    }

#if (defined (COM_RxFilters) && defined (COM_F_MASKEDNEWDIFFERSOLD)) && \
                                                    (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout))
    /* Reset the DM filtering flag only if it was set during the previous timeout session */
    if(ValidateSigGrp_u8 != COM_RESET)
    {
        /* Resetting this flag which is maintained at SignalGroup level,
        * for further reception of the signal, if Rx deadline monitoring timer elapses,
        * again this flag shall be set in Com_MainFunctionRx()
        * This flag is resetted after processing all the ComGroupSignal inside this SignalGroup */
        Com_SetRamValue(RXSIGGRP,_DMFILTERING,RxSigGrpRamPtr->rxSigGrpRAMFields,COM_FALSE);
    }
#endif /* #if (defined (COM_RxFilters) && defined (COM_F_MASKEDNEWDIFFERSOLD)) &&
                                                (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout)) */

    return(ValidateSigGrp_u8);
}

/**********************************************************************************************************************
 Function name    : Com_Prv_ValidateRxGroupSignal
 Syntax           : uint8 Com_Prv_ValidateRxGroupSignal(VAR(uint16_least, AUTOMATIC)        Idx_GrpSig_ui,
                                                    VAR(Com_SigMaxType, AUTOMATIC) RxNewVal_GrpSig,
                                                    VAR(uint8, AUTOMATIC)    ValidateSigGrp_u8)
 Description      : Internal function for checking invalid values for given signal group
 Parameter        : SignalGrpId - Signal Group Id to be validated
                    Src         - IPdu buffer pointer
 Return value     : None
 **********************************************************************************************************************/
LOCAL_INLINE FUNC(uint8,COM_CODE) Com_Prv_ValidateRxGroupSignal(VAR(uint16_least, AUTOMATIC)        Idx_GrpSig_ui,
                                                                VAR(Com_SigMaxType, AUTOMATIC)      RxNewVal_GrpSig)
{
    /* Local pointer to Rx GroupSignal static configuration */
    Com_RxGrpSigConstPtrType                    RxGrpSigConstPtr;
    /* Local pointer to Rx SignalGroup static configuration */
    Com_RxSigGrpConstPtrType                    RxSigGrpConstPtr;
    /* Local RAM pointer to SignalGroup RAM structure */
    Com_RxSigGrpRamPtrType                      RxSigGrpRamPtr;
    VAR(Com_RxIntSignalGroupIdType, AUTOMATIC)  SignalGrpId;
    VAR(uint8, AUTOMATIC)                       ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)                       ConstByteValue_SignalGroup_u8;
    VAR(uint8, AUTOMATIC)                       SigType;
    VAR(uint8, AUTOMATIC)                       ValidateSigGrp_u8;
#ifdef COM_RxFilters
    VAR(uint8, AUTOMATIC)                       filterAlgo_u8;
    VAR(boolean, AUTOMATIC)                     filterVal_b;
#endif
    VAR(boolean, AUTOMATIC)                     InvAction_b;

    RxGrpSigConstPtr    = COM_GET_RXGRPSIG_CONSTDATA(Idx_GrpSig_ui);
    SignalGrpId         = RxGrpSigConstPtr->SigGrpRef;
    RxSigGrpConstPtr    = COM_GET_RXSIGGRP_CONSTDATA(SignalGrpId);
    RxSigGrpRamPtr      = &COM_GET_RXSIGGRPFLAG(SignalGrpId);

    ValidateSigGrp_u8 = COM_SET;

#ifdef COM_RxFilters
    filterAlgo_u8 = RxGrpSigConstPtr->Filter_Algo;
#endif
    ConstByteValue_u8 = RxGrpSigConstPtr->rxGrpSigFields;
    InvAction_b   = Com_GetValue(RXGRPSIG,_INVACTION,ConstByteValue_u8);
    SigType = Com_GetValue(RXGRPSIG,_TYPE,ConstByteValue_u8);

#if defined(COM_RxSigGrpInvalid)
    /* For Float signals the invalid value is type casted to UNIT32 before comparing */
    if ((InvAction_b != COM_FALSE)
        && ((Com_SigMaxType)RxGrpSigConstPtr->DataInvalid_Val == RxNewVal_GrpSig)
        /* FC_VariationPoint_START */
#ifdef COM_RXGRPSIG_FLOAT64SUPP
        && (SigType != COM_FLOAT64)
#endif
        /* FC_VariationPoint_END */
        )
    {
        ConstByteValue_SignalGroup_u8 = RxSigGrpConstPtr->rxSignalGrpFields;
        /* if ( RxSigGrpConstPtr->rxSignalGrpFields.GrpSigInvalid == (uint8)COM_REPLACE) */
        if (Com_GetValue(RXSIGGRP,_INVACTION,ConstByteValue_SignalGroup_u8) == (uint8)COM_REPLACE)
        {
            /*  set the flag to Replace all the Group Signals(Com_RxNewVal_GrpSig) which is under
             * the same Signal Group with Initial value */
            ValidateSigGrp_u8 = COM_REPLACE_TO_INIT;
        }
#ifdef COM_RxSigGrpInvalidNotify
        else
        {
            /* If invalid action is NOTIFY */
            /* Check for invalid notification, only if SigGrpNotify_Flag is TRUE ie invalid notifcation
             * function not called yet..
             * The invalid notification function should be called only once for signal group
             */
            ValidateSigGrp_u8 = COM_RESET;

            /* Call Invalid notification for the whole Signal Group as soon as the
             * first group signal with invalid value is encountered
             */
            if(RxSigGrpConstPtr->DataInvalid_Indication_Cbk != NULL_PTR)
            {
                RxSigGrpConstPtr->DataInvalid_Indication_Cbk();
            }
        }
#endif /* #ifdef COM_RxSigGrpInvalidNotify */
        /* [SWS_Com_00680] SignalGroup is invalid, so no filtering process has to be done for this signal group
         * Break out of the for loop, as no more processing of group signals is required  */
    }
    else
#endif /* #if defined(COM_RxSigGrpInvalid) */
    {
#ifdef COM_RxFilters
        /* Proceed only if the GroupSignal is configured with a filter */
        if (filterAlgo_u8 != COM_NOTCONFIGURED)
        {
            /* The Groupsignal is valid, hence perform filtering */
            /* Get The Return value of the Filter which need not be returned if signal is invalid */
            /* The last parameter is Information from where the call to Com_Filter happens */
            /* NOTE on last parameter:
            * 1.Currently the values for the parameter callContext_u8 are COM_TRUE/COM_FALSE
            * 2.It is used only for filter ONE_EVERYN */
#if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout))
            if(!((Com_GetRamValue(RXSIGGRP,_DMFILTERING,RxSigGrpRamPtr->rxSigGrpRAMFields) == COM_TRUE) &&
                 (COM_MASKED_NEW_DIFFERS_MASKED_OLD == filterAlgo_u8)))
#endif /* #if defined (COM_F_MASKEDNEWDIFFERSOLD) && (defined (COM_RxIPduTimeout) || defined (COM_RxSigGrpUpdateTimeout)) */
            {
                /* Call the Com_Filter API only if the conditions meet:
                 * 1. If the GroupSignal is not invalidated
                 * 2. This SignalGroup was not timed out */
                filterVal_b = Com_Filter((uint32)RxNewVal_GrpSig,filterAlgo_u8,RxGrpSigConstPtr->Filter_Index,COM_TRUE);

                if(COM_TRUE != filterVal_b)
                {
                    /* [SWS_Com_00836]: Break out of the for loop, as no more processing of group signals is
                     * required And discard the whole signal group */
                    ValidateSigGrp_u8 = COM_RESET;
                }
            }
        }
#endif /* #ifdef COM_RxFilters */
    }
    return ValidateSigGrp_u8;
}
#endif /* #if defined(COM_RxSigGrpInvalid) || defined (COM_RxFilters) */

#endif /* #ifdef COM_RX_SIGNALGROUP */

