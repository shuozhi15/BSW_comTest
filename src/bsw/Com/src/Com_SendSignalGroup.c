

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/

#ifdef COM_TX_SIGNALGROUP
/***************************************************************************************************
 Function name    : Com_SendSignalGroup
 Syntax           : uint8 Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId)
 Description      : Service copies the content of the associated shadow buffer to the associated
                    I-PDU buffer.Prior to this call, all group signals should be updated in the
                    shadow buffer by the call of Com_UpdateShadowSignal.
 Parameter        : SignalGroupId -> Id of the SignalGroup
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/COM_BUSY
 ***************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_SendSignalGroup(VAR(Com_SignalGroupIdType, AUTOMATIC) SignalGroupId)
{
    Com_TxSigGrpConstPtrType TxSigGrpConstPtr; /* Local pointer to Tx SignalGroup static configuration */
    Com_TxIpduRamPtrType     TxIpduRamPtr;     /* Local pointer to Tx IPdu        static configuration */
    VAR(uint8, AUTOMATIC)    Status;           /* Return status                                        */

    Status = COM_SERVICE_NOT_AVAILABLE;

    /* Call DET Function to check for the DET Errors COM_E_UNINIT and COM_E_PARAM */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if(Com_Prv_DETCheck_Param((SignalGroupId >= COM_GET_NUM_TX_SIGNALGRP), COMServiceId_SendSignalGroup))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        SignalGroupId    = COM_GET_TXSIGNALGRP_ID(SignalGroupId);

        TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA( SignalGroupId );
        TxIpduRamPtr     = &COM_GET_TXPDURAM_S( TxSigGrpConstPtr->IpduRef );

        /* Proceed only if Large Data tranmission is not in Progress */
#ifdef COM_TP_IPDUTYPE
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
            Status = COM_BUSY;
        }
        else
#endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            Status = Com_InternalSendSignalGroup(SignalGroupId);
        }

    }
    return Status;
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

# define COM_START_SEC_CODE
# include "Com_MemMap.h"
/***************************************************************************************************
 Function name    : Com_InternalSendSignalGroup
 Syntax           : uint8 Com_InternalSendSignalGroup(Com_SignalGroupIdType SignalGroupId)
 Description      : Service copies the content of the associated shadow buffer to the associated
                    I-PDU buffer.Prior to this call, all group signals should be updated in the
                    shadow buffer by the call of Com_UpdateShadowSignal.
                    This is an internal function to update the signal group.
                    This function doesnt do DET checks, these checks are done in the API itself.
 Parameter        : SignalGroupId -> Internal Id of the SignalGroup (relevant for post-build)
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 ***************************************************************************************************/
/* HIS METRIC CALLS VIOLATION IN Com_InternalSendSignalGroup:
   Com_InternalSendSignalGroup function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(uint8, COM_CODE) Com_InternalSendSignalGroup(VAR(Com_SignalGroupIdType, AUTOMATIC) SignalGroupId)
{
#ifdef COM_EffectiveSigGrpTOC
    /* Local pointer to hold the address of SignalGroup status structure */
    Com_TxSigGrpRamPtrType              TxSigGrpRamPtr;
#endif
    /* Local pointer to hold the address of Tx SignalGroup static configuration */
    Com_TxSigGrpConstPtrType            TxSigGrpConstPtr;
#if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY)
    /* Local pointer to hold the address of Tx ipdu static configuration */
    Com_TxIpduConstPtrType              TxIPduConstPtr;
    #endif /* #if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY) */
    /* Local pointer to hold the address of Tx ipdu status structure */
    Com_TxIpduRamPtrType                TxIpduRamPtr;
    VAR(uint16_least, AUTOMATIC)        IpduId_ui;
    VAR(uint8, AUTOMATIC)               ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)               Status;              /* Return status */
    VAR(uint8, AUTOMATIC)               isSigTriggered_u8;
#if (defined(COM_SigGrpTriggered) || defined(COM_SigGrpTriggeredWithoutRepetition) || defined(COM_EffectiveSigGrpTOC))
    VAR(uint8, AUTOMATIC)               transferProperty_u8;
#endif
    VAR(boolean, AUTOMATIC)             TransProp_bool;
#ifdef COM_TX_SIGNALGROUP_ARRAY
    VAR(boolean, AUTOMATIC)             ConstIsSigGrpOfArrayType_b;
#endif

    Status              = E_OK;
    isSigTriggered_u8   = COM_RESET;
    TransProp_bool      = COM_FALSE;

    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(SignalGroupId);

    IpduId_ui = TxSigGrpConstPtr->IpduRef;
#if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY)

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId_ui);

#endif /* #if defined (COM_TxSigGrpUpdateBit) || defined (COM_SIGNALGROUPGATEWAY) */
    TxIpduRamPtr = &COM_GET_TXPDURAM_S(IpduId_ui);
    ConstByteValue_u8 = TxSigGrpConstPtr->txSignalGrpFields;

#ifdef COM_EffectiveSigGrpTOC
    TxSigGrpRamPtr = &COM_GET_TXSIGGRPFLAG(SignalGroupId);
#endif

#ifdef COM_TX_SIGNALGROUP_ARRAY
    ConstIsSigGrpOfArrayType_b = Com_GetValue(TXSIGGRP,_ARRAYACCESS,TxSigGrpConstPtr->txSignalGrpFields);
#endif

    /* Pack: Shadow buffer to Ipdu Buffer */
    SchM_Enter_Com_TxIpduProtArea(SENDSIGNALGRP);

#ifdef COM_TX_SIGNALGROUP_ARRAY
    if ( ConstIsSigGrpOfArrayType_b == COM_FALSE )
#endif
    {
        Com_ShadowBuffToIpduBuff(SignalGroupId);
    }
#ifdef COM_TX_SIGNALGROUP_ARRAY
    else
    {
        /* GroupSignal's SignalInitValues are already updated into the IPduBuffer
         * in Com_SendSignalGroupArray() */
    }
#endif

#ifdef COM_TxSigGrpUpdateBit
    /* Update bit if this bit is configured */
    if(Com_GetValue(TXSIGGRP,_UPDBITCONF,TxSigGrpConstPtr->txSignalGrpFields))
    {
        VAR(PduLengthType, AUTOMATIC) ByteNo_u8;
        /*Find the Byte No, where the Update bit lies */
        ByteNo_u8 = (PduLengthType)((TxSigGrpConstPtr->Update_Bit_Pos)>>3u);
        /* This macro sets the update bit value at the Update bit position */
        Com_SetUpdateBitValue(TxIPduConstPtr,TxSigGrpConstPtr->Update_Bit_Pos,ByteNo_u8)
    }
#endif
    SchM_Exit_Com_TxIpduProtArea(SENDSIGNALGRP);
#if (defined(COM_SigGrpTriggered) || defined(COM_SigGrpTriggeredWithoutRepetition) || defined(COM_EffectiveSigGrpTOC))
    transferProperty_u8 = Com_GetValue(TXSIGGRP,_TP,ConstByteValue_u8);
#endif
#if defined(COM_SigGrpTriggered) || defined(COM_SigGrpTriggeredWithoutRepetition)
    /* Set variable TransProp_bool, if SGs TP is Triggered or TriggeredWithoutRepetition */
    if (COM_SIG_TP_IS_TRIGGERED(transferProperty_u8))
    {
        isSigTriggered_u8 = COM_SET;
    }
    else
#endif
    {
#ifdef COM_EffectiveSigGrpTOC
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement access RAM, use of intermediate variables
         * to store and read here, may read obsolete value. Evaluation(or no evaluation) of 2nd operand
         * shall not impact the system behaviour. Hence suppressed. */
        /* If SGs TP is TriggeredOnChange or TriggeredWithoutRepetition set the variable TransProp_bool */
        if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(transferProperty_u8) &&
           (Com_GetRamValue(TXSIGGRP,_TRIGONCHNG,TxSigGrpRamPtr ->txSigGrpRAMFields) != COM_FALSE))
        {
            isSigTriggered_u8 = COM_SET;
            /* Reset the flag, so that next transmission is not triggered unless one of the group signal value
             * changes
             */
            /* TxSigGrpRamPtr ->txSigGrpRAMFields.TrigOnChange = COM_FALSE */
            Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,TxSigGrpRamPtr->txSigGrpRAMFields,COM_FALSE);
        }
#endif /* #ifdef COM_EffectiveSigGrpTOC */
    }


    /* As per SWS_Com_00334: The AUTOSAR COM module shall update the values of its internal buffers
     * even for stopped I-PDUs.
     * So the IPDU Group status is checked only after SignalGroup packing is done. */
    /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
     * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
     * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
     * Hence the warning is suppressed. */
    if(Com_CheckTxIPduStatus((PduIdType)IpduId_ui))
    {
        /* All unused flags are reset here */
        VAR(Com_SendIpduInfo,AUTOMATIC) sendIpduFlag_u16 = COM_CLEAR_SENDIPDU_FLAGS;

        /* Store if signal is triggered */
        sendIpduFlag_u16.sigTransProp = isSigTriggered_u8;

#ifdef COM_TxFilters
        /* Tx Filters are enabled and hence based on the TMC, TMS will be calculated.
         * Evaluate TMS only if IPDU doesnt have a fixed TM during configuration time and the signal contributes
         * to TMS calculation.
         */
        if(Com_GetValue(TXSIGGRP,_FILTEREVALREQ,TxSigGrpConstPtr->txSignalGrpFields) != COM_FALSE)
        {
            /* If TMS changes, Com_Prv_SigGrpTxChangeMode() returns COM_TRUE */
            sendIpduFlag_u16.isModeChangd = COM_BOOL_TO_UNSIGNED(Com_Prv_SigGrpTxChangeMode(SignalGroupId));
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        sendIpduFlag_u16.isTimeoutReq = COM_BOOL_TO_UNSIGNED(Com_GetValue(TXSIGGRP,_TOENABLED,ConstByteValue_u8));
#endif /* #ifdef COM_TxIPduTimeOut */

#if defined(COM_SigGrpTriggeredOnChangeWithoutRepetition) || defined(COM_SigGrpTriggeredWithoutRepetition)
        if (COM_SIG_TP_IS_WITHOUT_REPETITION(transferProperty_u8))
        {
            /* Set IpduFlag indicating SignalGroup's Tp is TriggeredOnChangeWithoutRepetition or
             * TriggeredWithoutRepetition (This flag stops to load NumberOfRepetitions) */
            sendIpduFlag_u16.ignoreRepetitions = COM_SET;
        }
#endif
        /* Proceed further to send ipdu */
        Com_Prv_ProceedToSendIpdu( (Com_IpduIdType)IpduId_ui, sendIpduFlag_u16 );

    }/* End of If IpduGroupStarted */
    else
    {
        Status = COM_SERVICE_NOT_AVAILABLE;
    }

    return (Status);
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

#endif /* #ifdef COM_TX_SIGNALGROUP */


