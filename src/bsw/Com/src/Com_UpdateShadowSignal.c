

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/

/***************************************************************************************************
 Function name    : Com_UpdateShadowSignal
 Syntax           : void Com_UpdateShadowSignal(Com_SignalIdType  SignalId,const void*   SignalDataPtr)
 Description      : Service updates a group signal with the data, referenced by SignalDataPtr
                    The update of the group signal data is done in the shadow buffer
 Parameter        : SignalId -> Id of the GroupSignal
                  : SignalDataPtr -> Pointer to the address where application data is available
 Return value     : None
 ***************************************************************************************************/

#ifdef COM_TX_SIGNALGROUP
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC V(G), CALLS VIOLATION IN Com_UpdateShadowSignal:
   Com_UpdateShadowSignal function contains simple "if" and "else if" statements and necesary function calls.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_UpdateShadowSignal(VAR(Com_SignalIdType, AUTOMATIC)  SignalId,
                                            P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
{
#ifdef COM_EffectiveSigGrpTOC
    Com_TxSigGrpRamPtrType TxSigGrpRamPtr;      /* Local pointer to hold the status of the SignalGroup */
#endif
    /* Local pointer to hold the address of Tx GroupSignal static configuration */
    Com_TxGrpSigConstPtrType  TxGrpSigConstPtr;
    VAR(Com_SigMaxType, AUTOMATIC) TxNewVal;    /* Local variable to store the new value */
    VAR(uint8, AUTOMATIC) ConstByteValue_u8;
    VAR(uint8, AUTOMATIC) Type;                 /* Type of the GroupSignal */
    VAR(Com_MainFuncType,AUTOMATIC) txMainFuncId;

    /*Call DET Function to check for the DET Error COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER*/
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr((SignalId >= COM_GET_NUM_TX_GRPSIGNALS),(SignalDataPtr == NULL_PTR),
                                    COMServiceId_UpdateShadowSignal))
#endif
    {
        SignalId = COM_GET_TXGRPSIGNAL_ID(SignalId);

        TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(SignalId);

        /* Fetch the Tx-MainFunction internal Id */
        txMainFuncId = (COM_GET_TX_IPDU_CONSTDATA((COM_GET_TXSIGGRP_CONSTDATA(TxGrpSigConstPtr->SigGrpRef))->IpduRef))->MainFunctionRef;

        /*Get the Bit field Byte value*/
        ConstByteValue_u8 = TxGrpSigConstPtr->txGrpSigFields;
        /*     Type = TxGrpSigConstPtr->txGrpSigFields.Type; */
        Type = Com_GetValue(TXGRPSIG,_TYPE,ConstByteValue_u8);
        TxNewVal = 0u;

        /* Even if the IPDU Group is stopped this API will update the data in the Internal Buffer */
        if(Type != (uint8)COM_UINT8_N)
        {
            switch(Type)
            {
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in AUTOSAR specification,
               so suppressing warning "Cast from a pointer to void to a pointer to object". */
            case COM_UINT8:
            case COM_SINT8:
                /* Update the Src_Buf with the data i.e typecast the data to uint8*/
                TxNewVal = *(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;
                break;
            case COM_UINT16:
            case COM_SINT16:
                /* Update the Src_Buf with the data i.e typecast the data to uint16*/
                TxNewVal = *(P2CONST(uint16, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;
                break;
            case COM_BOOLEAN:
                /* Update the Src_Buf with the data i.e typecast the data to the boolean variable*/
                TxNewVal = COM_BOOL_TO_UNSIGNED(*(P2CONST(boolean,AUTOMATIC,COM_APPL_DATA))SignalDataPtr);
                break;
            case COM_UINT32:
            case COM_SINT32:
                /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
                TxNewVal = *(P2CONST(uint32,AUTOMATIC,COM_APPL_DATA))SignalDataPtr;
                break;
#ifdef COM_TXGRPSIG_INT64
            case COM_UINT64:
            case COM_SINT64:
                /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
                TxNewVal = *(P2CONST(uint64, AUTOMATIC, COM_APPL_DATA))SignalDataPtr;
                break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

/* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
            case COM_FLOAT32:
                /* Convert the float value into 32 bit stream and then pack the value into shadow buffer */
                /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
                   But the void pointer is always deferenced to a type, based on the pointers alignment.
                   Hence the alignment warning can safely be ignored. */
                (void)rba_BswSrv_MemCopy( (void *)(&TxNewVal),(const void *)SignalDataPtr, 4 );
                break;
#endif /* #ifdef COM_GRPSIGFLOAT32SUPP */

#ifdef COM_TXGRPSIG_FLOAT64SUPP
            case COM_FLOAT64:
                /* Pack the float value into float64 shadow buffer */
                /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in AUTOSAR specification,
                   so suppressing warning "Cast from a pointer to void to a pointer to object". */
                SchM_Enter_Com_TxGrpSigBuffer();
                COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index) =
                        *(P2CONST(float64,AUTOMATIC,COM_APPL_DATA))SignalDataPtr;
                SchM_Exit_Com_TxGrpSigBuffer();
                break;
#endif
/* FC_VariationPoint_END */

            default:
                /**
                 * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are
                 * allowed to be configured, hence default case is intentionally left empty.
                 */
                break;
            }

            /* Call a generic internal api which copies the TxNewVal data into corresponding GroupSignal buffer */
            Com_CopyTxGroupSignalData(SignalId, TxNewVal, txMainFuncId);
            /* Before updating endianess conversion should be performed */
            /* Update the Signal in the Shadow buffer, here you have to find the bitposition in the shadow
            buffer for this signal  */
#ifdef COM_EffectiveSigGrpTOC
            TxSigGrpRamPtr = &COM_GET_TXSIGGRPFLAG(TxGrpSigConstPtr->SigGrpRef);
            /* Check if the group signal contributes to evaluation of transfer property of signal group */
            if((Com_GetValue(TXGRPSIG,_TP,ConstByteValue_u8) != COM_FALSE)  &&
               (COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) != (Com_OldValTrigOnChngType)TxNewVal))
            {
                /* Update the old value */
                COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) = (Com_OldValTrigOnChngType)TxNewVal;
                /* TxSigGrpRamPtr ->txSigGrpRAMFields.TrigOnChange = COM_TRUE */
                Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,TxSigGrpRamPtr ->txSigGrpRAMFields,COM_TRUE);
            }
#endif
        }
        else
        {
            /* The locks inside the function Com_ByteCopy() are removed.
             * Hence locks are used here
             * Copy the *SignalDataPtr to all the Bytes of this signal in Shadow buffer */
            SchM_Enter_Com_TxGrpSigBuffer();

            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in AUTOSAR specification,
               so suppressing warning "Cast from a pointer to void to a pointer to object". */
            Com_ByteCopy(&COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index),
                    (P2CONST(uint8, AUTOMATIC, COM_APPL_DATA))SignalDataPtr,(uint32)TxGrpSigConstPtr->BitSize);

            SchM_Exit_Com_TxGrpSigBuffer();
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif

