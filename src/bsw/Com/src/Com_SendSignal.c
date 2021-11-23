

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(Com_SigMaxType, COM_CODE) Com_Prv_SigBufftoIpduBuff(
                                                            VAR(Com_SignalIdType, AUTOMATIC)        SignalId,
                                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
                                                                     );

/**********************************************************************************************************************
 * Global functions (declared in header files)
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Function name    : Com_SendSignal
 Syntax           : uint8 Com_SendSignal(Com_SignalIdType SignalId, const void*   SignalDataPtr)
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter.
 Parameter        : SignalId -> Id of the signal.
                  : SignalDataPtr -> The pointer to the address where the application data is available.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE/COM_BUSY
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_SendSignal( VAR(Com_SignalIdType, AUTOMATIC)        SignalId,
                                      P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr )
{
    Com_TxSigConstPtrType TxSigConstPtr; /* Local pointer to Tx Signal static configuration */
#ifdef COM_TP_IPDUTYPE
    Com_TxIpduRamPtrType  TxIpduRamPtr;  /* Local pointer to Tx IPdu   static configuration */
#endif
    VAR(uint8, AUTOMATIC) Status;        /* Return status                                   */

    Status = COM_SERVICE_NOT_AVAILABLE;

    /* Call DET Function to check for the DET Errors COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr((SignalId >= COM_GET_NUM_TX_SIGNALS), (SignalDataPtr == NULL_PTR),
                                                                            COMServiceId_SendSignal))
#endif
    {
        /* Convert external HandleId to internal HandleId for Tx Signal
         * IMP : before accessing the tables, it is required to convert the IDs
         * For pre-compile , conversion is not required. Access Macro will return the same ID, which is passed to it */
        SignalId      = COM_GET_TXSIGNAL_ID(SignalId);

        TxSigConstPtr = COM_GET_TXSIG_CONSTDATA( SignalId );
#ifdef COM_TP_IPDUTYPE
        TxIpduRamPtr  = &COM_GET_TXPDURAM_S( TxSigConstPtr->IpduRef );
#endif

        /* Proceed only if Large Data tranmission is not in Progress */
#ifdef COM_TP_IPDUTYPE
        if (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
        {
            /* The TxIPdu transmission of large Data IPdu is in progress, hence no signal update is allowed.
             * Until the transmission is completed. */
             Status = COM_BUSY;
        }
        else
#endif /*#ifdef COM_TP_IPDUTYPE*/
        {
            Status = Com_InternalSendSignal(SignalId,SignalDataPtr);
        }
    }
    return Status;

} /* End of Com_SendSignal() */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**********************************************************************************************************************
 Function name    : Com_InternalSendSignal
 Syntax           : uint8 Com_InternalSendSignal(Com_SignalIdType SignalId, const void* SignalDataPtr)
 Description      : Service updates the signal object identified by SignalId with the signal
                    referenced by the SignalDataPtr parameter.
                    This is an internal function to update the signal.
                    This function doesnt do DET checks, these checks are done in the API itself.
 Parameter        : SignalId -> Internal Id of the signal. (Relevant for post-build case)
                  : SignalDataPtr -> The pointer to the address where the application data is available.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS VIOLATION IN Com_InternalSendSignal:
   Com_InternalSendSignal function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(uint8, COM_CODE) Com_InternalSendSignal(
                                    VAR(Com_SignalIdType, AUTOMATIC)        SignalId,
                                    P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
                                            )
{
#ifdef COM_TxSigUpdateBit
    Com_TxIpduConstPtrType          TxIPduConstPtr;
#endif
    Com_TxSigConstPtrType           TxSigConstPtr;
    VAR(Com_SigMaxType, AUTOMATIC)  TxSigNewVal;
    VAR(uint16_least, AUTOMATIC)    IpduId_ui;
    VAR(uint16, AUTOMATIC)          ConstTxSignalFields_u16; /* Varible to hold the Signal Property */
    VAR(uint8, AUTOMATIC)           TxSigTP_u8;              /* Varible to hold the Signal Transfer Property */
    VAR(uint8, AUTOMATIC)           isSigTriggered_u8;
    VAR(uint8, AUTOMATIC)           Status;                  /* Return status */

    Status                  = E_OK;

    TxSigConstPtr           = COM_GET_TXSIG_CONSTDATA(SignalId);
    IpduId_ui               = TxSigConstPtr->IpduRef;
    ConstTxSignalFields_u16 = TxSigConstPtr->txSignalFields; /* Load the signal property */

    /* Load the Signal Transfer Property */
    TxSigTP_u8              = (uint8)Com_GetValue(TXSIG,_TP,ConstTxSignalFields_u16);

    SchM_Enter_Com_TxIpduProtArea(SENDSIGNAL);

#ifdef COM_TxSigUpdateBit

    TxIPduConstPtr          = COM_GET_TX_IPDU_CONSTDATA(IpduId_ui);

    /* Set update bit if it is configured */
    if (Com_GetValue(GEN,_UPDBITCONF,TxSigConstPtr->General))
    {
        VAR(PduLengthType, AUTOMATIC) ByteNo_uo;

        /* Calculate byte position of update bit in IPDU buffer */
        ByteNo_uo = (PduLengthType)(TxSigConstPtr->Update_Bit_Pos >> 3u);

        /* This macro sets the update bit value at the Update bit position */
        Com_SetUpdateBitValue(TxIPduConstPtr, TxSigConstPtr->Update_Bit_Pos, ByteNo_uo)
    }
#endif /* #ifdef COM_TxSigUpdateBit */

    /* Copy signal data from application buffer into IPdu buffer. */
    TxSigNewVal = Com_Prv_SigBufftoIpduBuff(SignalId, SignalDataPtr);

    SchM_Exit_Com_TxIpduProtArea(SENDSIGNAL);

#if defined(COM_SigTriggered) || defined(COM_SigTriggeredWithoutRepetition)

    if (COM_SIG_TP_IS_TRIGGERED(TxSigTP_u8))
    {
        isSigTriggered_u8 = COM_SET;
    }
    else
#endif
#ifdef COM_EffectiveSigTOC

    if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(TxSigTP_u8) &&
       (COM_GET_OLDVALTRIGONCHANGE(TxSigConstPtr->OldValue_Index) != (Com_OldValTrigOnChngType)TxSigNewVal))
    {
        isSigTriggered_u8 = COM_SET;
        COM_GET_OLDVALTRIGONCHANGE(TxSigConstPtr->OldValue_Index) = (Com_OldValTrigOnChngType)TxSigNewVal;
    }
    else
#endif
    {
        /* signal transfer property: PENDING OR
         * signal transfer property: TRIG_ON_CHANGE & signal value has not changed after the last update.
         */
        isSigTriggered_u8 = COM_RESET;
    }

    /* As per SWS_Com_00334: The AUTOSAR COM module shall update the values of its internal buffers
     * even for stopped I-PDUs. So the IPDU Group status is checked only after Signal packing is done. */
    if (Com_CheckTxIPduStatus((PduIdType)IpduId_ui))
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
        if (Com_GetValue(TXSIG,_FILTRALG,ConstTxSignalFields_u16) != (uint16)COM_NOTCONFIGURED)
        {
            /* If TMS changes, Com_Prv_SigTxChangeMode() returns COM_TRUE */
            if (Com_Prv_SigTxChangeMode(SignalId, (uint32)TxSigNewVal))
            {
                sendIpduFlag_u16.isModeChangd = COM_SET;
            }
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        /* Check if signal-timeout is configured */
        if (Com_GetValue(TXSIG,_TOENABLED,ConstTxSignalFields_u16))
        {
            sendIpduFlag_u16.isTimeoutReq = COM_SET;
        }
#endif

#if defined(COM_SigTriggeredWithoutRepetition) || defined(COM_SigTriggeredOnChangeWithoutRepetition)
        /* Check if signal transfer property is configured as WITHOUT_REPETITION */
        if (COM_SIG_TP_IS_WITHOUT_REPETITION(TxSigTP_u8))
        {
            sendIpduFlag_u16.ignoreRepetitions = COM_SET;
        }
#endif

        /* Proceed further to send ipdu */
        Com_Prv_ProceedToSendIpdu( (Com_IpduIdType)IpduId_ui, sendIpduFlag_u16 );

    } /* End of Com_CheckTxIPduStatus */
    else
    {
        /* corresponding I-PDU group is stopped, so donot process txIPdu */
        Status = COM_SERVICE_NOT_AVAILABLE;
    }

    return Status;

} /* End of Com_InternalSendSignal */
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**********************************************************************************************************************
 Function name    : Com_Prv_SigBufftoIpduBuff
 Syntax           : Com_SigMaxType Com_Prv_SigBufftoIpduBuff(Com_SignalIdType SignalId, const void * SignalDataPtr)
 Description      : Pack/Copy signal buffer into IPdu buffer
 Parameter        : SignalId      -> Internal Id of the signal. (Relevant for post-build case)
                  : SignalDataPtr -> The pointer to the address where the application data is available.
 Return value     : returns signal buffer
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(Com_SigMaxType, COM_CODE) Com_Prv_SigBufftoIpduBuff(
                                                            VAR(Com_SignalIdType, AUTOMATIC)        SignalId,
                                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
                                                                     )
{
    Com_TxIpduConstPtrType          TxIPduConstPtr;
    Com_TxSigConstPtrType           TxSigConstPtr;
    VAR(uint16_least, AUTOMATIC)    IpduId_ui;
    VAR(Com_SigMaxType, AUTOMATIC)  TxSigNewVal;
    VAR(PduLengthType, AUTOMATIC)   ByteNo_uo;
    VAR(uint8, AUTOMATIC)           Type;
    VAR(uint8, AUTOMATIC)           ConstGeneral_u8;

    TxSigConstPtr   = COM_GET_TXSIG_CONSTDATA(SignalId);
    IpduId_ui       = TxSigConstPtr->IpduRef;
    ConstGeneral_u8 = TxSigConstPtr->General;

    TxIPduConstPtr  = COM_GET_TX_IPDU_CONSTDATA(IpduId_ui);

    TxSigNewVal     = (Com_SigMaxType)0;
    Type            = Com_GetValue(GEN,_TYPE,ConstGeneral_u8);

    if (Type != (uint8)COM_UINT8_N)
    {
        VAR(uint8, AUTOMATIC) SigEndianess;

        SigEndianess = Com_GetValue(GEN,_ENDIANESS,ConstGeneral_u8);

        /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
           so suppressing warning "Cast from a pointer to void to a pointer to object". */
        switch(Type)
        {
        case COM_SINT8:
        case COM_UINT8:
            /* Update the Src_Buf with the data i.e typecast the data to uint8*/
            TxSigNewVal = (Com_SigMaxType)(*(P2CONST(uint8,AUTOMATIC,COM_APPL_DATA))SignalDataPtr);
            break;
        case COM_SINT16:
        case COM_UINT16:
            /* Update the Src_Buf with the data i.e typecast the data to uint16*/
            TxSigNewVal = (Com_SigMaxType)(*(P2CONST(uint16,AUTOMATIC,COM_APPL_DATA))SignalDataPtr);
            break;
        case COM_BOOLEAN:
            /* Update the Src_Buf with the data i.e typecast the data to the boolean variable*/
            TxSigNewVal = (Com_SigMaxType)COM_BOOL_TO_UNSIGNED(*(P2CONST(boolean,AUTOMATIC,COM_APPL_DATA))SignalDataPtr);
            break;
        case COM_SINT32:
        case COM_UINT32:
            /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
            TxSigNewVal = (Com_SigMaxType)(*(P2CONST(uint32, AUTOMATIC, COM_APPL_DATA))SignalDataPtr);
            break;
#ifdef COM_TXSIG_INT64
        case COM_UINT64:
        case COM_SINT64:
            /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
            TxSigNewVal = *(P2CONST(uint64, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;
            break;
#endif /* #ifdef COM_TXSIG_INT64 */

            /* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case COM_FLOAT32:
            /* Convert the float value into 32 bit stream and then pack the value */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
               But the void pointer is always deferenced to a type, based on the pointers address alignment.
               Hence the alignment warning can safely be ignored. */
            (void)rba_BswSrv_MemCopy( (void *)&TxSigNewVal, SignalDataPtr, 4 );
            break;
#endif /* #ifdef COM_FLOAT32SUPP */
            /* FC_VariationPoint_END */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
        }
        /* FC_VariationPoint_START */
#ifdef COM_TXSIG_FLOAT64SUPP
        if (Type == COM_FLOAT64)
        {
            VAR(float64, AUTOMATIC) TxSigNewVal_f64;

            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
               so suppressing warning "Cast from a pointer to void to a pointer to object". */
            TxSigNewVal_f64 = *(P2CONST(float64, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;

            /* Convert the float value into 32 bit stream and then pack the value */
            Com_PackFloatSignal(SigEndianess, TxSigConstPtr->Bit_Pos, TxSigNewVal_f64, TxIPduConstPtr->BuffPtr);
        }
        else
#endif /* #ifdef COM_TXSIG_FLOAT64SUPP */
        /* FC_VariationPoint_END */
        {
            Com_PackSignal(SigEndianess, TxSigConstPtr->Bit_Pos, TxSigConstPtr->BitSize, TxSigNewVal,
                           TxIPduConstPtr->BuffPtr);
        }
    }
    else
    {
        /* Find the ByteNo_uo of the IPDU buffer */
        ByteNo_uo = (PduLengthType)(TxSigConstPtr->Bit_Pos >> 3u);
        /* The locks inside the function Com_ByteCopy() are removed.
         * Hence locks are used here
         */
        /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
           so suppressing warning "Cast from a pointer to void to a pointer to object". */
        Com_ByteCopy((TxIPduConstPtr->BuffPtr + ByteNo_uo),(const uint8*)SignalDataPtr,(uint32)TxSigConstPtr->BitSize);

    }

    return TxSigNewVal;

} /* End of Com_Prv_SigBufftoIpduBuff */

