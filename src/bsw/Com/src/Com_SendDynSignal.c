
/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
LOCAL_INLINE FUNC(uint8, COM_CODE) Com_Prv_SendDynSignal(   VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                            VAR(uint16, AUTOMATIC) Length);
LOCAL_INLINE FUNC(uint8, COM_CODE) Com_Prv_CopyDynSignal(   VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                            VAR(uint16, AUTOMATIC) Length);
#endif
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_SendDynSignal
 Syntax           : uint8 Com_SendDynSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                            VAR(uint16, AUTOMATIC) Length)
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : SignalId -> Id of the signal.
                  : SignalDataPtr -> The pointer to the address where the application data is available.
                  : Length -> Length of the dynamic length signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/E_NOT_OK/COM_BUSY
 ***************************************************************************************************/
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_SendDynSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                        P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                        VAR(uint16, AUTOMATIC) Length)
{
    /* Local pointer to hold the Tx ipdu status information */
    Com_TxIpduRamPtrType            TxIpduRamPtr;
    Com_TxSigConstPtrType           TxSigConstPtr;
    VAR(uint16_least, AUTOMATIC)    IpduId_ui;
    VAR(uint8, AUTOMATIC)           ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)           Status;  /* Return status */
    VAR(uint8, AUTOMATIC)           Type;

    Status = COM_SERVICE_NOT_AVAILABLE;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr((SignalId >= COM_GET_NUM_TX_SIGNALS), (SignalDataPtr == NULL_PTR),
                                                                         COMServiceId_SendDynSignal ))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */

        SignalId = COM_GET_TXSIGNAL_ID(SignalId);

        TxSigConstPtr       = COM_GET_TXSIG_CONSTDATA(SignalId);
        ConstByteValue_u8   = TxSigConstPtr->General;

        Type                = Com_GetValue(GEN,_TYPE,ConstByteValue_u8);
        IpduId_ui           = TxSigConstPtr->IpduRef;
        TxIpduRamPtr        = &COM_GET_TXPDURAM_S(IpduId_ui);

        /* Check whether the large data pdu flag has been reset */
#ifdef COM_TP_IPDUTYPE
        if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
             Status = COM_BUSY;
        }/* if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) == (uint8)COM_TRUE) */
        else
#endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            /* If the application requests with length greater than the maximum length of the signal */
            if((Length <= TxSigConstPtr->BitSize) && ((uint8)COM_UINT8_DYN == Type))
            {
                Status = Com_Prv_SendDynSignal(SignalId, SignalDataPtr, Length);
            }
            else
            {
                Status = E_NOT_OK;
            }
        }
    }
    return Status;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /*#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT*/


/***************************************************************************************************
 Function name    : Com_Prv_SendDynSignal
 Syntax           : uint8 Com_Prv_SendDynSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                VAR(uint16, AUTOMATIC) Length)
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : SignalId -> Id of the signal.
                  : SignalDataPtr -> The pointer to the address where the application data is available.
                  : Length -> Length of the dynamic length signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 ***************************************************************************************************/
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT

LOCAL_INLINE FUNC(uint8, COM_CODE) Com_Prv_SendDynSignal(   VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                            VAR(uint16, AUTOMATIC) Length)
{
    /* Local pointer to hold the Tx ipdu status information */
    Com_TxSigConstPtrType           TxSigConstPtr;
#if defined(COM_TxFilters) || defined(COM_TxIPduTimeOut) || defined(COM_SigTriggeredWithoutRepetition)
    /* Variable to hold Signal property */
    VAR(uint16, AUTOMAIC)           ConstByteValue_u16;
#endif
    VAR(uint16_least, AUTOMATIC)    IpduId_ui;
    VAR(uint8, AUTOMATIC)           Status;  /* Return status */
    VAR(uint8, AUTOMATIC)           isSigTriggered_u8;

    Status              = E_OK;

    TxSigConstPtr       = COM_GET_TXSIG_CONSTDATA(SignalId);
#if defined(COM_TxFilters) || defined(COM_TxIPduTimeOut) || defined(COM_SigTriggeredWithoutRepetition)
    /* Get signal property to variable */
    ConstByteValue_u16  = TxSigConstPtr->txSignalFields;
#endif
    IpduId_ui           = TxSigConstPtr->IpduRef;

    isSigTriggered_u8 = Com_Prv_CopyDynSignal(SignalId, SignalDataPtr, Length);

    /* As per SWS_Com_00334: The AUTOSAR COM module shall update the values of its internal buffers
     * even for stopped I-PDUs.
     * So the IPDU Group status is checked only after Signal packing is done. */
    if(Com_CheckTxIPduStatus((Com_IpduIdType)IpduId_ui))
    {
        /* All unused flags are reset here */
        VAR(Com_SendIpduInfo,AUTOMATIC) sendIpduFlag_u16 = COM_CLEAR_SENDIPDU_FLAGS;

        /* Store if signal is triggered */
        sendIpduFlag_u16.sigTransProp = isSigTriggered_u8;

#ifdef COM_TxFilters
        /* The Dynamic signal can have ALWAYS/NEVER as filter algorithm.
         * Evaluate TMS only if IPDU doesnt have a fixed Transmission Mode during configuration time and
         * the signal contributes to TMS calculation
         */
        if(Com_GetValue(TXSIG,_FILTRALG,ConstByteValue_u16) != (uint16)COM_NOTCONFIGURED)
        {
             /* Com_SigTxChangeMode() returns COM_TRUE, if TMS changes,
             * Here the value sent is '0' as filter algorithm can only be always/never/One_every_n */
            if(Com_Prv_SigTxChangeMode(SignalId,0))
            {
                sendIpduFlag_u16.isModeChangd = COM_SET;
            }
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        /* Check if signal-timeout is configured */
        sendIpduFlag_u16.isTimeoutReq = COM_BOOL_TO_UNSIGNED(Com_GetValue(TXSIG,_TOENABLED,ConstByteValue_u16));
#endif

#ifdef COM_SigTriggeredWithoutRepetition
        if (COM_TRIGGERED_WITHOUT_REPETITION == Com_GetValue(TXSIG,_TP,ConstByteValue_u16))
        {
            /* Set the flag to indicate Signal TP is TRIGGERED_WITHOUT_REPETITION
             * (This flag stops the loading of NumberOfRepetitions) */
            sendIpduFlag_u16.ignoreRepetitions = COM_SET;
        }
#endif /* COM_SigTriggeredWithoutRepetition */

        /* Proceed further to send ipdu */
        Com_Prv_ProceedToSendIpdu( (Com_IpduIdType)IpduId_ui, sendIpduFlag_u16 );

    }/*End of If IpduGroupStarted*/
    else
    {
        Status = COM_SERVICE_NOT_AVAILABLE;
    }
    return Status;
}

#endif /*#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT*/

/***************************************************************************************************
 Function name    : Com_Prv_CopyDynSignal
 Syntax           : uint8 Com_Prv_CopyDynSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                VAR(uint16, AUTOMATIC) Length)
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter and length of the dynamic length signal
 Parameter        : SignalId -> Id of the signal.
                  : SignalDataPtr -> The pointer to the address where the application data is available.
                  : Length -> Length of the dynamic length signal
 Return value     : COM_SET/COM_RESET
 ***************************************************************************************************/
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT

LOCAL_INLINE FUNC(uint8, COM_CODE) Com_Prv_CopyDynSignal(   VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                            VAR(uint16, AUTOMATIC) Length)
{

    /* Local pointer to hold the Tx ipdu status information */
    Com_TxIpduRamPtrType            TxIpduRamPtr;
    Com_TxIpduConstPtrType          TxIPduConstPtr;
    Com_TxSigConstPtrType           TxSigConstPtr;
    VAR(PduLengthType, AUTOMATIC)   ByteNo_u8;
    /* Variable to hold Signal property */
    VAR(uint16, AUTOMAIC)           ConstByteValue_u16;
    VAR(uint16_least, AUTOMATIC)    IpduId_ui;
#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition)
    /* Variable to hold the signal transfer property */
    VAR(uint8, AUTOMATIC)           TxSigTP_u8;
#endif
    VAR(uint8, AUTOMATIC)           isSigTriggered_u8;

    isSigTriggered_u8   = COM_RESET;

    TxSigConstPtr       = COM_GET_TXSIG_CONSTDATA(SignalId);
    /* Get signal property to variable */
    ConstByteValue_u16  = TxSigConstPtr->txSignalFields;

    IpduId_ui           = TxSigConstPtr->IpduRef;
    TxIPduConstPtr      = COM_GET_TX_IPDU_CONSTDATA(IpduId_ui);
    TxIpduRamPtr        = &COM_GET_TXPDURAM_S(IpduId_ui);

    /* Find the ByteNo_u8 of the IPDU buffer */
    /* The locks here is to protect 1. The Tx Ipdu buffer data update, 2. Dynamic signal length update */
    SchM_Enter_Com_TxIpduProtArea(SENDDYNSIGNAL);

#ifdef COM_TxSigUpdateBit
    /*As per COM334: the signal has to be updated in Internal buffer even if IPDU Group is STOPPED*/
    /* Update bit if this bit is configured */
    if(Com_GetValue(GEN,_UPDBITCONF,TxSigConstPtr->General))
    {
        /* Find the ByteNo_u8 of the IPDU buffer */
        ByteNo_u8 = (PduLengthType)(TxSigConstPtr->Update_Bit_Pos >> 3u);
        /* This macro sets the update bit value at the Update bit position */
        Com_SetUpdateBitValue(TxIPduConstPtr,TxSigConstPtr->Update_Bit_Pos,ByteNo_u8)
    }
#endif
    if(Length != 0)
    {
        /* Find the ByteNo_u8 of the IPDU buffer */
        ByteNo_u8 = (PduLengthType)(TxSigConstPtr->Bit_Pos >> 3u);
        /* MR12 RULE 11.5 VIOLATION: For SignalDataPtr is defined as void pointer in the AUTOSAR
           specification, so suppressing warning "Cast from a pointer to void to a pointer to object" */
        Com_ByteCopy((TxIPduConstPtr->BuffPtr + ByteNo_u8),(const uint8*)SignalDataPtr,(uint32)Length);
    }

    /* The dynamic Length of the Ipdu, is updated here */
    TxIpduRamPtr->Com_DynLength= Length;
    SchM_Exit_Com_TxIpduProtArea(SENDDYNSIGNAL);

#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition)
    /* Get the Signal transfer property */
    TxSigTP_u8 = (uint8)Com_GetValue(TXSIG,_TP,ConstByteValue_u16);

    /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
     * HIS-LEVEL parameter will be violated if more conditional statements are added.
     * Hence the warning is suppressed. */
    if (COM_SIG_TP_IS_TRIGGERED(TxSigTP_u8))
    {
        isSigTriggered_u8 = COM_SET;
    }
#endif
    return isSigTriggered_u8;
}

#endif /*#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT*/

