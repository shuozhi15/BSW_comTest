

/**********************************************************************************************************************
 * Preprocessor includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
/**********************************************************************************************************************
 * Global functions (declared in header files )
 *********************************************************************************************************************/
#ifdef COM_SIGNALGATEWAY
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**********************************************************************************************************************
 * Function name  : Com_Prv_PackRxSignalGwBufferData
 * Syntax         : void Com_Prv_PackRxSignalGwBufferData(Com_RxSigConstPtrType RxSigConstPtr,
 *                                                        Com_SignalIdType l_TxGwDestId)
 * Description    : This function is called to
 * 1. Set Update bit if relevant
 * 2. Copy the data from the Rx signal buffer and pack into Tx Ipdu buffer
 * 3. Evaluate transfer property and transmission mode selection for each signal and update the Tx Ipdu flag accordingly
 * --------------------------------------------
 * Parameter      : Com_RxSigConstPtrType RxSigConstPtr
 *                : Com_SignalIdType      l_TxGwDestId
 * Return value   : void
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Prv_PackRxSignalGwBufferData(Com_RxSigConstPtrType RxSigConstPtr,
                                                     Com_SignalIdType l_TxGwDestId)
{
    Com_TxSigConstPtrType                TxSigConstPtr;  /* Local pointer to Tx SignalGroup static configuration */
#if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP)
    VAR(float64, AUTOMATIC)              TxNewVal_f64;
#endif
    VAR(Com_SigBuffIndexType, AUTOMATIC) RxSigBuff_Index;
    VAR(Com_SigMaxType, AUTOMATIC)       TxNewVal;
    VAR(uint8, AUTOMATIC)                Type;
    VAR(Com_MainFuncType, AUTOMATIC)     rxMainFuncId; /* Local variable that holds internal Rx-MainFunction Id */

    TxSigConstPtr   = COM_GET_TXSIG_CONSTDATA(l_TxGwDestId);
    Type            = Com_GetValue(GEN,_TYPE,TxSigConstPtr->General);

    /* Fetch the Rx-MainFunction internal Id */
    rxMainFuncId    = (COM_GET_RX_IPDU_CONSTDATA(RxSigConstPtr->IpduRef))->MainFunctionRef;

    RxSigBuff_Index = RxSigConstPtr->SigBuff_Index;
    TxNewVal        = (Com_SigMaxType)0;

    if(Type != (uint8)COM_UINT8_N)
    {
        /* Refer to Com_Cfg_SchM file; this is a multi-core lock */
        SchM_Enter_Com_RxSigGwMcLockProtArea();

        switch(Type >> 1u)
        {
        case 0x00u:     /* COM_UINT8 & COM_SINT8 */
        case (COM_BOOLEAN >> 1):
            /* Update the Src_Buf with the data i.e typecast the data to uint8*/
            TxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId, RxSigBuff_Index);
            break;

        case 0x01u:     /* COM_UINT16 & COM_SINT16 */
            /* Update the Src_Buf with the data i.e typecast the data to uint16*/
            TxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT16_BUFF(rxMainFuncId, RxSigBuff_Index);
            break;

        case 0x02u:     /* COM_UINT32 & COM_SINT32 */
/* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case (COM_FLOAT32 >> 1):
#endif /* #ifdef COM_FLOAT32SUPP */
/* FC_VariationPoint_END */
            /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
            TxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT32_BUFF(rxMainFuncId, RxSigBuff_Index);
            break;

#if defined(COM_RXSIG_INT64) && defined(COM_TXSIG_INT64)
        case 0x08u:     /* COM_UINT64 & COM_SINT64 */
            /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
            TxNewVal = COM_GET_SIGTYPEUINT64_BUFF(rxMainFuncId, RxSigBuff_Index);
            break;
#endif /* #if defined(COM_RXSIG_INT64) && defined(COM_TXSIG_INT64) */

/* FC_VariationPoint_START */
#if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP)
        case (COM_FLOAT64 >> 1):
            TxNewVal_f64 = COM_GET_SIGTYPEFLOAT64_BUFF(rxMainFuncId, RxSigBuff_Index);
            break;
#endif /* #if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */
        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
        }

        SchM_Exit_Com_RxSigGwMcLockProtArea();

/* FC_VariationPoint_START */
#if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP)
        if(Type == COM_FLOAT64)
        {
            /* 1. Set the UpdateBit, if configured
             * 2. Update TxIPdu buffer with the routed Signal data
             * Return value is based on IPduGroup status that can be ignored here, as it is checked later
             * in Com_MainFunctionRouteSignals()
             */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_InternalSendSignal() definition.
               But the void pointer is always deferenced to a same type. Hence the alignment warning
               can safely be ignored. */
            (void)Com_InternalSendSignal( l_TxGwDestId, (const void *)&TxNewVal_f64 );

        }
        else
#endif /* #if defined(COM_TXSIG_FLOAT64SUPP) && defined(COM_RXSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */
        {
            /* 1. Set the UpdateBit, if configured
             * 2. Update TxIPdu buffer with the routed Signal data
             * 3. Evaluate TMC of Signal
             * 4. Evaluate TMS of TxIPdu
             * Return value is based on IPduGroup status that can be ignored here, as it is checked later
             * in Com_MainFunctionRouteSignals()
             */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_InternalSendSignal() definition.
               But the void pointer is always deferenced to a same/lesser integer type & it is 32bit aligned.
               Hence the alignment warning can safely be ignored. */
            (void)Com_InternalSendSignal( l_TxGwDestId, (const void *)&TxNewVal );

        }
    }
    else
    {
        /* Refer to Com_Cfg_SchM file; this is a multi-core lock */
        SchM_Enter_Com_RxSigGwMcLockProtArea();

        /* SignalType: UINT8_N */
        /* 1. Set the UpdateBit, if configured
         * 2. Update TxIPdu buffer with the routed Signal data
         * 3. Evaluate TMC of Signal
         * 4. Evaluate TMS of TxIPdu
         * Return value is based on IPduGroup status that can be ignored here, as it is checked later
         * in Com_MainFunctionRouteSignals()
         */
        /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_InternalSendSignal() definition.
           But the void pointer is always deferenced to a same type. Hence the alignment warning
           can safely be ignored. */
        (void)Com_InternalSendSignal( l_TxGwDestId, (const void *)&COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId, RxSigBuff_Index) );

        SchM_Exit_Com_RxSigGwMcLockProtArea();
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_SIGNALGATEWAY */


#if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/**********************************************************************************************************************
 Function name    : Com_Prv_PackRxGrpSigGwBufferData
 Syntax           : void Com_Prv_PackRxGrpSigGwBufferData(Com_RxGrpSigConstPtrType RxGrpSigConstPtr,
                                                      Com_TxIntGrpSignalIdType l_TxGwDestId)
 Description      : This function is called to
 *                   Copy the data from the Rx GroupSignal buffer to its corresponding Tx GroupSignal
 Parameter        : Com_RxGrpSigConstPtrType RxGrpSigConstPtr
                  : Com_TxIntGrpSignalIdType l_TxGwDestId
 Return value     : void
 *********************************************************************************************************************/
/* HIS METRIC CALLS VIOLATION IN Com_Prv_PackRxGrpSigGwBufferData:
   Com_Prv_PackRxGrpSigGwBufferData function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_Prv_PackRxGrpSigGwBufferData( Com_RxGrpSigConstPtrType RxGrpSigConstPtr,
                                                      Com_TxIntGrpSignalIdType l_TxGwDestId)
{
    Com_TxGrpSigConstPtrType                TxGrpSigConstPtr; /* Local pointer to Tx GroupSignal static configuration */
    Com_TxSigGrpRamPtrType                  TxSigGrpRamPtr;   /* Local pointer to status of the Tx SignalGroup */
    VAR(Com_SigMaxType, AUTOMATIC)          TxNewVal;         /* Local variable to store the new value */
    VAR(Com_RxGrpSigBuffIndexType, AUTOMATIC) rxGrpSignalBuffIndex;
    VAR(uint8, AUTOMATIC)                   Type;             /* Type of the GroupSignal */
    VAR(Com_MainFuncType, AUTOMATIC)        rxMainFuncId;     /* Local variable holds MainFunction reference */
    VAR(Com_MainFuncType,AUTOMATIC)         txMainFuncId;

    TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(l_TxGwDestId);

    TxSigGrpRamPtr   = &COM_GET_TXSIGGRPFLAG(TxGrpSigConstPtr->SigGrpRef);

    Type = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);

    /* Fetch the Rx-MainFunction internal Id */
    rxMainFuncId = (COM_GET_RX_IPDU_CONSTDATA((COM_GET_RXSIGGRP_CONSTDATA(RxGrpSigConstPtr->SigGrpRef))->IpduRef))->MainFunctionRef;
    /* Fetch the Tx-MainFunction internal Id */
    txMainFuncId = (COM_GET_TX_IPDU_CONSTDATA((COM_GET_TXSIGGRP_CONSTDATA(TxGrpSigConstPtr->SigGrpRef))->IpduRef))->MainFunctionRef;

    TxNewVal = 0u;

    rxGrpSignalBuffIndex = RxGrpSigConstPtr->RxGrpSigBuff_Index;

    /* Even if the IPDU Group is stopped this API will update the data in the Internal Buffer */
    if(Type != (uint8)COM_UINT8_N)
    {
        SchM_Enter_Com_TxGrpSigBuffer();
        switch(Type >> 1u)
        {
        case 0x00u:     /* COM_UINT8 & COM_SINT8 */
        case (COM_BOOLEAN >> 1):
            /* Update the Src_Buf with the data i.e typecast the data to uint8*/
            TxNewVal = (Com_SigMaxType)COM_GET_SECRXGRPSIGTYPEU8_BUFF(rxMainFuncId, rxGrpSignalBuffIndex);
            COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index)  = (uint8)TxNewVal;
            break;

        case 0x01u:     /* COM_UINT16 & COM_SINT16 */
            #if (CPU_TYPE == CPU_TYPE_8)
            SchM_Enter_Com_RxGrpSigSecBuff();
            #endif
            /* Update the Src_Buf with the data i.e typecast the data to uint16*/
            TxNewVal = (Com_SigMaxType)COM_GET_SECRXGRPSIGTYPEU16_BUFF(rxMainFuncId, rxGrpSignalBuffIndex);
            #if (CPU_TYPE == CPU_TYPE_8)
            SchM_Exit_Com_RxGrpSigSecBuff();
            #endif
            COM_GET_TXGRPSIGTYPEU16_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index)  = (uint16)TxNewVal;
            break;

        case 0x02u:     /* COM_UINT32 & COM_SINT32 */
/* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
        case (COM_FLOAT32 >> 1u):
#endif /* #ifdef COM_GRPSIGFLOAT32SUPP */
/* FC_VariationPoint_END */
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigSecBuff();
            #endif
            /* Update the Src_Buf with the data i.e typecast the data to the uint32 variable*/
            TxNewVal = (Com_SigMaxType)COM_GET_SECRXGRPSIGTYPEU32_BUFF(rxMainFuncId, rxGrpSignalBuffIndex);
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigSecBuff();
            #endif
            COM_GET_TXGRPSIGTYPEU32_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index)  = (uint32)TxNewVal;
            break;

#if defined(COM_RXGRPSIG_INT64) && defined(COM_TXGRPSIG_INT64)
        case 0x08u:     /* COM_UINT64 & COM_SINT64 */
            SchM_Enter_Com_RxGrpSigSecBuff();
            /* Update the Src_Buf with the data i.e typecast the data to the uint64 variable*/
            TxNewVal = COM_GET_SECRXGRPSIGTYPEU64_BUFF(rxMainFuncId, rxGrpSignalBuffIndex);
            SchM_Exit_Com_RxGrpSigSecBuff();
            COM_GET_TXGRPSIGTYPEU64_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index)  = TxNewVal;
            break;
#endif /* #if defined(COM_RXGRPSIG_INT64) && defined(COM_TXGRPSIG_INT64) */

/* FC_VariationPoint_START */
#if defined(COM_TXGRPSIG_FLOAT64SUPP) && defined(COM_RXGRPSIG_FLOAT64SUPP)
        case (COM_FLOAT64 >> 1):
            SchM_Enter_Com_RxGrpSigSecBuff();
            /* Pack the float value into float64 shadow buffer */
            COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index) =
                         COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(rxMainFuncId, rxGrpSignalBuffIndex);
            SchM_Exit_Com_RxGrpSigSecBuff();
            break;
#endif /* #if defined(COM_TXGRPSIG_FLOAT64SUPP) && defined(COM_RXGRPSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */
        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;

        }
        SchM_Exit_Com_TxGrpSigBuffer();

        /* Before updating endianess conversion should be performed */
        /* Update the Signal in the Shadow buffer, here you have to find the bitposition in the shadow
         * buffer for this signal  */

#ifdef COM_EffectiveSigGrpTOC
        /* Check if the group signal contributes to evaluation of transfer property of signal group */
        /* if((TxGrpSigConstPtr->txGrpSigFields.TransProp != (uint8)COM_FALSE)  &&
         *    (COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) != TxNewVal)) */
        if((Com_GetValue(TXGRPSIG,_TP,TxGrpSigConstPtr->txGrpSigFields) != COM_FALSE)  &&
           (COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) != (Com_OldValTrigOnChngType)TxNewVal))
        {
            /* Update the old value */
            COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) = (Com_OldValTrigOnChngType)TxNewVal;
            /* TxSigGrpRamPtr ->txSigGrpRAMFields.TrigOnChange = COM_TRUE */
            Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,TxSigGrpRamPtr->txSigGrpRAMFields,COM_TRUE);
        }
#endif

    }
    else
    {
        /* The locks inside the function Com_ByteCopy() are removed.
         * Hence locks are used here
         * Copy the *SignalDataPtr to all the Bytes of this signal in Shadow buffer */
        SchM_Enter_Com_TxGrpSigBuffer();
        SchM_Enter_Com_RxGrpSigSecBuff();
        Com_ByteCopy(&COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index),
                     &COM_GET_SECRXGRPSIGTYPEU8_BUFF(rxMainFuncId, rxGrpSignalBuffIndex),(uint32)TxGrpSigConstPtr->BitSize);
        SchM_Exit_Com_RxGrpSigSecBuff();
        SchM_Exit_Com_TxGrpSigBuffer();

    }

    /* This is to flag the signal group, that a group signal within it was updated */
    /* TxSigGrpRamPtr ->txSigGrpRAMFields.IsGwUpdated = COM_TRUE */
    Com_SetRamValue(TXSIGGRP,_ISGWUPDTD,TxSigGrpRamPtr->txSigGrpRAMFields,COM_TRUE);

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP)) */

