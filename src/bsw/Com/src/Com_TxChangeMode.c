

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
#ifdef COM_TxFilters
LOCAL_INLINE FUNC(void,COM_CODE) Com_Prv_TxChangeMode_Signal            (VAR(Com_IpduIdType, AUTOMATIC) TxIpduId);

# ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE FUNC(void,COM_CODE) Com_Prv_TxChangeMode_SignalGroup       (VAR(Com_IpduIdType, AUTOMATIC) TxIpduId);
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_TxChangeMode_GroupSignal    (VAR(Com_IpduIdType, AUTOMATIC) TxIpduId,
                                                                         Com_TxGrpSigConstPtrType TxGrpSigConstPtr);
# endif
#endif

/***************************************************************************************************
 Function name    : Com_TxChangeMode
 Syntax           : void Com_TxChangeMode(Com_IpduIdType TxIpduId)
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : TxIpduId
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

/* HIS METRIC CALLS VIOLATION IN Com_TxChangeMode:
   Com_TxChangeMode function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void,COM_CODE) Com_TxChangeMode(VAR(Com_IpduIdType, AUTOMATIC) TxIpduId)
{
    /* Local pointer to hold the address of Tx ipdu static configuration */
    Com_TxIpduConstPtrType                          TxIPduConstPtr;
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                            TxIpduRamPtr;
    VAR(uint8, AUTOMATIC)                           LatestTransMode_u8;
    VAR(boolean, AUTOMATIC)                         Mode;

    /* Initialise Mode as TRUE, it could be the most probable case */
    Mode = COM_TRUE;
    TxIPduConstPtr   = COM_GET_TX_IPDU_CONSTDATA(TxIpduId);
    TxIpduRamPtr     = &COM_GET_TXPDURAM_S(TxIpduId);

    /* Call Filter for all signals and Group Signals in the IPDU */
#ifdef COM_TxFilters
    TxIpduRamPtr->Com_TMSTrueCounter_u8 = 0;

    /* Check if Transmission Mode switch is possible by signal filter mechanism */
    if (Com_GetValue(TXIPDU,_FILTEREVALREQ,TxIPduConstPtr->txIPduFields) != COM_FALSE )
    {
        Com_Prv_TxChangeMode_Signal(TxIpduId);

#ifdef COM_TX_SIGNALGROUP
        Com_Prv_TxChangeMode_SignalGroup(TxIpduId);
#endif

        if (TxIpduRamPtr->Com_TMSTrueCounter_u8 != 0)
        {
            /* If any signal/signal group evaluates filter as TRUE, then TM TRUE is selected for IPDU */
            Mode = COM_TRUE;
        }
        else
        {
            /* If all signal/signal group evaluates filter as FALSE, then TM FALSE is selected for IPDU */
            Mode = COM_FALSE;
        }
    }
#endif /*COM_TxFilters*/

    /* If a change of the TMS causes a change of the transmission mode for one I-PDU, the timer for the cycle
    time of the periodic and the Mixed Tx Mode shall be retstarted */

    /* Update the TxModeState with the Mode */
    Com_SetRamValue(TXIPDU,_TXMODESTATE,TxIpduRamPtr->Com_TransMode,Mode);
    /* Update the CurrentTxModePtr */
    Com_SetCurrentTxModePtr(TxIpduRamPtr,TxIPduConstPtr,Com_GetValue(TXIPDU,_TMS,TxIPduConstPtr->txIPduFields),Mode);

    /* Update the Com_LatestTransMode[TxIpduId] */
    LatestTransMode_u8 = TxIpduRamPtr->CurrentTxModePtr->Mode;
    Com_SetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode,LatestTransMode_u8);

    /* Reset the timers */
    /* This check is equivalent to
     * if(((LatestTransMode_u8 == COM_TXMODE_PERIODIC) ||
     *    (LatestTransMode_u8 == COM_TXMODE_MIXED)) )
     */
    if( 0u != ( LatestTransMode_u8 & COM_TXMODE_MIXED ) )
    {
        /* The function may be interrupted by Com_MainFunctionTx().This will lead to inconsistant data in
         * Com_MainFunctionTx(), So Interrupt is locked here. */
        TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEPERIOD(TxIpduId, (TxIpduRamPtr->CurrentTxModePtr));

        /*Below Initialize is done if Com_TxChangeMode() is called by Ipdu_GroupStart()*/
        TxIpduRamPtr ->Com_n_Tick_Tx = 0;
        /*This means there is no n Transmission, This will be updated in Com_SendSignal()and Com_SendSignalGroup()*/
        TxIpduRamPtr ->Com_n = 0;
        /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
        Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,TxIpduRamPtr->Com_TransMode,
                                                TxIpduRamPtr->CurrentTxModePtr->MixedPhaseShift);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
        /* FC_VariationPoint_END */
    }
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/***************************************************************************************************
 Function name    : Com_Prv_TxChangeMode_Signal
 Syntax           : void Com_Prv_TxChangeMode_Signal(Com_IpduIdType TxIpduId)
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : TxIpduId
 Return value     : None
 ***************************************************************************************************/
#ifdef COM_TxFilters

LOCAL_INLINE FUNC(void,COM_CODE) Com_Prv_TxChangeMode_Signal(VAR(Com_IpduIdType, AUTOMATIC) TxIpduId)
{
    /* Local pointer to hold the address of Tx ipdu static configuration */
    Com_TxIpduConstPtrType                          TxIPduConstPtr;
    /* Local pointer to hold the address of the Tx ipdu status structure */
    Com_TxIpduRamPtrType                            TxIpduRamPtr;
    /* Local pointer to Tx signal static configuration */
    Com_TxSigConstPtrType                           TxSigConstPtr;
    /* Local pointer to Tx signal status structure */
    Com_TxSigRamPtrType                             TxSigRamPtr;
    VAR(uint16_least, AUTOMATIC)                    Index_ui;
    VAR(Com_SigMaxType, AUTOMATIC)                  TxNewVal;
    VAR(uint8, AUTOMATIC)                           FilterAlg_u8;  /* Filter type */
    VAR(uint8, AUTOMATIC)                           ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)                           Type;   /* Signal Type */
    VAR(boolean, AUTOMATIC)                         Filter_Val;

    TxIPduConstPtr  = COM_GET_TX_IPDU_CONSTDATA(TxIpduId);
    TxIpduRamPtr    = &COM_GET_TXPDURAM_S(TxIpduId);
    TxSigConstPtr   = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);
    TxSigRamPtr     = &COM_GET_TXSIGNALFLAG(TxIPduConstPtr->TxSigId_Index);

    for(Index_ui = TxIPduConstPtr->No_Of_Sig_Ref; Index_ui != 0u; Index_ui--)
    {
        FilterAlg_u8 = (uint8)Com_GetValue(TXSIG,_FILTRALG,TxSigConstPtr->txSignalFields);

        /* Check if the filter is configured, NOTE: If filter is not configured, then the Value will be COM_ALWAYS
            in the configuration and this means that the signal will not contribute to TMS calculation */
        if(FilterAlg_u8 != (uint8)COM_NOTCONFIGURED)
        {
            ConstByteValue_u8 = TxSigConstPtr->General;
            Type = Com_GetValue(GEN,_TYPE,ConstByteValue_u8);

            if(Type != (uint8)COM_UINT8_N)
            {
                TxNewVal = Com_UnpackSignal( Com_GetValue(GEN,_ENDIANESS,ConstByteValue_u8),
                                             TxSigConstPtr->Bit_Pos,
                                             TxSigConstPtr->BitSize,
                                             TxIPduConstPtr ->BuffPtr,
                                             ((Type & 0x01u) != 0x0u));
            }
            else
            {
                TxNewVal = 0;
                /* This variable will not be checked in Com_Filter, as the Algorithm for Uint8[n] will be only
                 * ALWAYS or NEVER */
            }
            /* The last parameter is Information from where the call to Com_Filter happens */
            /* NOTE on last parameter:
             * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
             * 2.It is used only for filter ONE_EVERYN */
            Filter_Val = Com_Filter((uint32)TxNewVal,FilterAlg_u8, TxSigConstPtr->Filter_Index, COM_FALSE);

            if (Filter_Val != COM_FALSE)
            {
                /* Increment TRUE counter , based on filter condition */
                TxIpduRamPtr ->Com_TMSTrueCounter_u8++;
            }
            /* Store the latest filter state of the signal */
            Com_SetRamValue(TXSIG,_SIGCHNGMODE,TxSigRamPtr->txSigRAMFields,Filter_Val);
        }
        TxSigConstPtr++;
        TxSigRamPtr++;
    }
}
#endif /* #ifdef COM_TxFilters */


/***************************************************************************************************
 Function name    : Com_Prv_TxChangeMode_SignalGroup
 Syntax           : void Com_Prv_TxChangeMode_SignalGroup(Com_IpduIdType TxIpduId)
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : TxIpduId
 Return value     : None
 ***************************************************************************************************/
#if defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP)

LOCAL_INLINE FUNC(void,COM_CODE) Com_Prv_TxChangeMode_SignalGroup(VAR(Com_IpduIdType, AUTOMATIC) TxIpduId)
{
    /* Local pointer to hold the address of Tx ipdu static configuration */
    Com_TxIpduConstPtrType                          TxIPduConstPtr;
    /* Local pointer to Tx SignalGroup static configuration */
    Com_TxSigGrpConstPtrType                        TxSigGrpConstPtr;
    /* Local pointer to Tx GroupSignal static configuration */
    Com_TxGrpSigConstPtrType                        TxGrpSigConstPtr;
    /* Local pointer to Tx GroupSignal status structure */
    Com_TxGrpSigRamPtrType                          TxGrpSigRamPtr;
    VAR(uint16_least, AUTOMATIC)                    GrpSig_Index_ui;
    VAR(uint16_least, AUTOMATIC)                    Index_ui;
    VAR(boolean, AUTOMATIC)                         Filter_Val;

    TxIPduConstPtr   = COM_GET_TX_IPDU_CONSTDATA(TxIpduId);

    /* Secondly check for Group Signals */
    /* For Selection of the transmission mode the Group signals shall be treated as normal signals */
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(TxIPduConstPtr->FirstTxSigGrp_Index);

    for(Index_ui = TxIPduConstPtr->No_Of_SigGrp_Ref; Index_ui!= 0u; Index_ui--)
    {
        if(Com_GetValue(TXSIGGRP,_FILTEREVALREQ,TxSigGrpConstPtr->txSignalGrpFields) != COM_FALSE)
        {
            /* Now check for each Group Signal of this signal group */
            TxGrpSigRamPtr = &COM_GET_TXGRPSIGFLAG(TxSigGrpConstPtr->FirstGrpSig_Index);

            TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(TxSigGrpConstPtr->FirstGrpSig_Index);

            for(GrpSig_Index_ui = TxSigGrpConstPtr->No_Of_GrpSig; GrpSig_Index_ui != 0u; GrpSig_Index_ui--)
            {
                /* Check if the filter is configured, NOTE: If filter is not configured,
                then the Value will be COM_ALWAYS in the configuration */
                if(TxGrpSigConstPtr->Filter_Algo != (uint8)COM_NOTCONFIGURED)
                {
                    Filter_Val = Com_Prv_TxChangeMode_GroupSignal(TxIpduId, TxGrpSigConstPtr);

                    /* Store the latest filter state of the signal */
                    Com_SetRamValue(TXGRPSIG,_SIGCHNGMODE,TxGrpSigRamPtr->txGrpSigRAMFields,Filter_Val);
                }
                TxGrpSigConstPtr++;
                TxGrpSigRamPtr++;
            }
        }
        TxSigGrpConstPtr++;
    }
}


/***************************************************************************************************
 Function name    : Com_Prv_TxChangeMode_GroupSignal
 Syntax           : boolean Com_Prv_TxChangeMode_GroupSignal    (VAR(Com_IpduIdType, AUTOMATIC) TxIpduId,
                                                                 Com_TxGrpSigConstPtrType TxGrpSigConstPtr)
 Description      : Internal function used for Transmission Mode Selector
 Parameter        : TxIpduId         - TX Pdu Id
                    TxGrpSigConstPtr - Group signal constant pointer
 Return value     : Filter_Val - COM_TRUE/COM_FALSE
 ***************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_TxChangeMode_GroupSignal   (VAR(Com_IpduIdType, AUTOMATIC) TxIpduId,
                                                                        Com_TxGrpSigConstPtrType TxGrpSigConstPtr)
{
    /* Local pointer to hold the address of Tx ipdu static configuration */
    Com_TxIpduConstPtrType                          TxIPduConstPtr;
    Com_TxIpduRamPtrType                            TxIpduRamPtr;
# ifdef COM_TX_SIGNALGROUP_ARRAY
    Com_TxSigGrpConstPtrType                        TxSigGrpConstPtr;
#endif
    VAR(Com_SigMaxType, AUTOMATIC)                  TxNewVal;
    VAR(Com_TxGrpSigBuffIndexType, AUTOMATIC)       GrpSigBuff_Index;
    VAR(Com_MainFuncType,AUTOMATIC)                 txMainFuncId;
    VAR(uint8, AUTOMATIC)                           ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)                           Type;   /* Signal Type */
    VAR(boolean, AUTOMATIC)                         Filter_Val;
# ifdef COM_TX_SIGNALGROUP_ARRAY
    VAR(boolean, AUTOMATIC)                         ConstIsSigGrpOfArrayType_b;
#endif

    TxIPduConstPtr  = COM_GET_TX_IPDU_CONSTDATA(TxIpduId);
    TxIpduRamPtr    = &COM_GET_TXPDURAM_S(TxIpduId);
    txMainFuncId    = TxIPduConstPtr->MainFunctionRef;
    TxNewVal        = 0u;

    ConstByteValue_u8 = TxGrpSigConstPtr->txGrpSigFields;
    Type = Com_GetValue(TXGRPSIG,_TYPE,ConstByteValue_u8);
# ifdef COM_TX_SIGNALGROUP_ARRAY
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(TxGrpSigConstPtr->SigGrpRef);
    ConstIsSigGrpOfArrayType_b = Com_GetValue(TXSIGGRP,_ARRAYACCESS,TxSigGrpConstPtr->txSignalGrpFields);
# endif
    if(Type != (uint8)COM_UINT8_N)
    {
# ifdef COM_TX_SIGNALGROUP_ARRAY
        if ( ConstIsSigGrpOfArrayType_b )
        {
            /* Unpack GroupSignal data from SignalGroup with ArrayAccess */
            TxNewVal = Com_UnpackSignal( Com_GetValue(TXGRPSIG,_ENDIANESS,ConstByteValue_u8),
                                         TxGrpSigConstPtr->Bit_Pos,
                                         TxGrpSigConstPtr->BitSize,
                                         TxIPduConstPtr->BuffPtr,
                                         ((Type & 0x01u) != 0x0u));

        }
        else
# endif /* # ifdef COM_TX_SIGNALGROUP_ARRAY */
        {
            GrpSigBuff_Index = TxGrpSigConstPtr->TxGrpSigBuff_Index;

            switch(Type >> 1u)
            {
            case 0x00u: /* SINT8 signal, UINT8 signal */
            case 0x03u: /*Boolean Signal*/
                TxNewVal = COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, GrpSigBuff_Index);
                break;

            case 0x01u:
                /* SINT16 signal, UINT16 signal */
                TxNewVal = COM_GET_TXGRPSIGTYPEU16_BUFF(txMainFuncId, GrpSigBuff_Index);
                break;

            case 0x02u:
                /* SINT32 signal, UINT32 signal */
                TxNewVal = COM_GET_TXGRPSIGTYPEU32_BUFF(txMainFuncId, GrpSigBuff_Index);
                break;

# ifdef COM_TXGRPSIG_INT64
            case 0x08u:
                /* SINT64 signal, UINT64 signal */
                TxNewVal = COM_GET_TXGRPSIGTYPEU64_BUFF(txMainFuncId, GrpSigBuff_Index);
                break;
# endif /* #ifdef COM_TXGRPSIG_INT64 */

            default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal
             * types are allowed to be configured, hence default case is intentionally left empty.
             */
                break;
            }
        }
    }
    else
    {
        /* For GroupSignal of array types(UINT8[n]), signal values are not used for
         * filter(ALWAYS, NEVER, ONE_EVERY_N) evaluation */
        TxNewVal = 0u;
    }
    /* The last parameter is Information from where the call to Com_Filter happens */
    /* NOTE on last parameter:
    * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
    * 2.It is used only for filter ONE_EVERYN */
    Filter_Val = Com_Filter((uint32)TxNewVal, TxGrpSigConstPtr->Filter_Algo,
                        TxGrpSigConstPtr->Filter_Index,COM_FALSE);

    if (Filter_Val != COM_FALSE)
    {
        /* Increment TRUE counter , based on filter condition */
        TxIpduRamPtr ->Com_TMSTrueCounter_u8++;
    }
    return Filter_Val;
}
#endif /* #if defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP) */


/***************************************************************************************************
 Function name    : Com_Prv_SigTxChangeMode
 Syntax           : boolean Com_Prv_SigTxChangeMode(uint16 SignalId, uint32 TxNewVal)
 Description      : This function evaluates the current TMS of the IPDU, based on the corresponding signal
                    which is updated.
 Parameter        : SignalId -> Id of the signal
                  : TxNewVal -> New signal value
 Return value     : COM_TRUE/COM_FALSE
 ***************************************************************************************************/
#ifdef COM_TxFilters

#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(boolean,COM_CODE) Com_Prv_SigTxChangeMode(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                               VAR(uint32, AUTOMATIC) TxNewVal)
{

    Com_TxIpduRamPtrType    TxIpduRamPtr;      /* Local pointer to Tx ipdu   dynamic configuration */
    Com_TxSigConstPtrType   TxSigConstPtr;     /* Local pointer to Tx signal static  configuration */
    Com_TxSigRamPtrType     TxSigRamPtr;       /* Local pointer to Tx signal dynamic configuration */
    VAR(Com_IpduIdType, AUTOMATIC)  TxIpduId;
    VAR(boolean, AUTOMATIC)         sig_TMSState;
    VAR(boolean, AUTOMATIC)         ReturnValue;

    TxSigConstPtr   = COM_GET_TXSIG_CONSTDATA(SignalId);
    TxIpduId        = TxSigConstPtr->IpduRef;

    TxIpduRamPtr    = &COM_GET_TXPDURAM_S(TxIpduId);

    /* Call Filter for all signals in the IPDU */
    SchM_Enter_Com_TxIpduProtArea(SIGTXCHANGEMODE);
    {
        TxSigRamPtr     = &COM_GET_TXSIGNALFLAG(SignalId);

        /* Check the configured filter algorithm */
        if(Com_GetValue(TXSIG,_FILTRALG,TxSigConstPtr->txSignalFields) > (uint16)COM_NEVER)
        {
            /* Evaluate current filter state of the signal */
            /* The last parameter is Information from where the call to Com_Filter happens */
            /* NOTE on last parameter:
             * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
             * 2.It is used only for filter ONE_EVERYN */
            sig_TMSState = Com_Filter(TxNewVal,
                               (uint8)Com_GetValue(TXSIG,_FILTRALG,TxSigConstPtr->txSignalFields),
                                   TxSigConstPtr->Filter_Index,COM_TRUE);
            /* Update TMS TRUE counter , only if filter state of the signal changes.
             * Otherwise, TMS TRUE counter value remains unchanged.
             */
            if (Com_GetRamValue(TXSIG,_SIGCHNGMODE,TxSigRamPtr->txSigRAMFields)!= sig_TMSState)
            {
                /* Signal has changed from FALSE to TRUE, hence increment TMS TRUE counter, else
                 * if Signal has changed from TRUE to FALSE, hence decrement TMS TRUE counter
                 */
                TxIpduRamPtr ->Com_TMSTrueCounter_u8 =
                       ((sig_TMSState != COM_FALSE) ? (TxIpduRamPtr ->Com_TMSTrueCounter_u8 + 1):
                                                    (TxIpduRamPtr ->Com_TMSTrueCounter_u8-1));

                /* Update the latest filter state of the signal, if the signal filter state changes */
                Com_SetRamValue(TXSIG,_SIGCHNGMODE,TxSigRamPtr->txSigRAMFields,sig_TMSState);
            }
        }
    }
    /* Call function to check for the mode changes */
    ReturnValue = Com_Prv_CheckModeChange(TxIpduId);
    SchM_Exit_Com_TxIpduProtArea(SIGTXCHANGEMODE);

    return(ReturnValue);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TxFilters */


#if (defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP))
/***************************************************************************************************
 Function name    : Com_Prv_SigGrpTxChangeMode
 Syntax           : FUNC(boolean,COM_CODE) Com_Prv_SigGrpTxChangeMode(VAR(Com_IpduIdType, AUTOMATIC) SignalGroupId
 Description      : This function evaluates the current TMS of the IPDU, based on the corresponding signal or
                    signal group which is updated.
 Parameter        : SignalGroupId -> Id of the SignalGroup
 Return value     : COM_TRUE/COM_FALSE
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS, LEVEL VIOLATION IN Com_Prv_SigGrpTxChangeMode:
   Com_Prv_SigGrpTxChangeMode function contains necessary function calls and simple "if", "switch" case statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(boolean,COM_CODE) Com_Prv_SigGrpTxChangeMode(VAR(Com_SignalGroupIdType, AUTOMATIC) SignalGroupId)
{

    Com_TxIpduConstPtrType  TxIPduConstPtr;    /* Local pointer to Tx ipdu   static  configuration */
    Com_TxIpduRamPtrType    TxIpduRamPtr;      /* Local pointer to Tx ipdu   dynamic configuration */
    /* Local pointer to Tx GroupSignal dynamic configuration */
    Com_TxGrpSigRamPtrType                      TxGrpSigRamPtr;
    /* Local pointer to Tx SignalGroup static configuration */
    Com_TxSigGrpConstPtrType                    TxSigGrpConstPtr;
    /* Local pointer to Tx GroupSignal static configuration */
    Com_TxGrpSigConstPtrType                    TxGrpSigConstPtr;

    VAR(Com_SigMaxType, AUTOMATIC)              TxNewVal_GrpSig;
    VAR(Com_TxGrpSigBuffIndexType, AUTOMATIC)   GrpSigBuff_Index;
    VAR(Com_IpduIdType, AUTOMATIC)              TxIpduId;
    VAR(uint16_least, AUTOMATIC)                GrpSig_Index_ui;
    VAR(uint8, AUTOMATIC)                       ConstByteValue_u8;
    VAR(boolean, AUTOMATIC)                     sig_TMSState;
    VAR(boolean, AUTOMATIC)                     ReturnValue;
    VAR(uint8, AUTOMATIC)                       Type;
    VAR(Com_MainFuncType,AUTOMATIC)             txMainFuncId;
#ifdef COM_TX_SIGNALGROUP_ARRAY
    boolean                                     ConstIsSigGrpOfArrayType_b;
#endif

    TxNewVal_GrpSig  = (Com_SigMaxType)COM_ZERO;
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(SignalGroupId);
    TxIpduId         = TxSigGrpConstPtr->IpduRef;

    TxIpduRamPtr    = &COM_GET_TXPDURAM_S(TxIpduId);
    TxIPduConstPtr  = COM_GET_TX_IPDU_CONSTDATA(TxIpduId);

    /* Call Filter for all Group Signals in the IPDU */
    SchM_Enter_Com_TxIpduProtArea(SIGGRPTXCHANGEMODE);

    /* Get the Bit field Byte value */
    TxGrpSigRamPtr              = &COM_GET_TXGRPSIGFLAG(TxSigGrpConstPtr->FirstGrpSig_Index);

    TxGrpSigConstPtr            = COM_GET_TXGRPSIG_CONSTDATA(TxSigGrpConstPtr->FirstGrpSig_Index);

    txMainFuncId = TxIPduConstPtr->MainFunctionRef;

#ifdef COM_TX_SIGNALGROUP_ARRAY
    ConstIsSigGrpOfArrayType_b  = Com_GetValue(TXSIGGRP,_ARRAYACCESS,TxSigGrpConstPtr->txSignalGrpFields);
#endif

    for(GrpSig_Index_ui = TxSigGrpConstPtr->No_Of_GrpSig; GrpSig_Index_ui != 0u; GrpSig_Index_ui--)
    {
        /* Check if the filter is configured, NOTE: If filter is not configured,
           then the Value will be COM_ALWAYS in the configuration */
        if((TxGrpSigConstPtr->Filter_Algo != (uint8)COM_NOTCONFIGURED) && (TxGrpSigConstPtr->Filter_Algo > COM_NEVER))
        {

            ConstByteValue_u8   = TxGrpSigConstPtr->txGrpSigFields;
            Type                = Com_GetValue(TXGRPSIG,_TYPE,ConstByteValue_u8);

            if(Type != (uint8)COM_UINT8_N)
            {
#ifdef COM_TX_SIGNALGROUP_ARRAY
                if (ConstIsSigGrpOfArrayType_b)
                {
                    /* Unpack GroupSignal data from SignalGroup with ArrayAccess */
                    TxNewVal_GrpSig = Com_UnpackSignal( Com_GetValue(TXGRPSIG,_ENDIANESS,ConstByteValue_u8),
                                                        TxGrpSigConstPtr->Bit_Pos,
                                                        TxGrpSigConstPtr->BitSize,
                                                        TxIPduConstPtr->BuffPtr,
                                                        ((Type & 0x01u) != 0x0u));
                }
                else
#endif /* # ifdef COM_TX_SIGNALGROUP_ARRAY */
                {
                    GrpSigBuff_Index = TxGrpSigConstPtr->TxGrpSigBuff_Index;

                    switch(Type >> COM_ONE)
                    {
                    case 0x00u:  /* UINT8   Signal */
                    case 0x03u:  /* Boolean Signal */
                        TxNewVal_GrpSig = COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, GrpSigBuff_Index);
                        break;

                    case 0x01u:
                        /* UINT16 signal */
                        TxNewVal_GrpSig = COM_GET_TXGRPSIGTYPEU16_BUFF(txMainFuncId, GrpSigBuff_Index);
                        break;

                    case 0x02u:
                        /* UINT32 signal */
                        TxNewVal_GrpSig = COM_GET_TXGRPSIGTYPEU32_BUFF(txMainFuncId, GrpSigBuff_Index);
                        break;

#ifdef COM_TXGRPSIG_INT64
                    case 0x08u:
                        /* UINT64 signal */
                        TxNewVal_GrpSig = COM_GET_TXGRPSIGTYPEU64_BUFF(txMainFuncId, GrpSigBuff_Index);
                        break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

                    default:
                        /**
                         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal
                         * types are allowed to be configured, hence default case is intentionally left empty.
                         */
                        break;
                    }
                }
            }
            else
            {
                /* For GroupSignal of array types(UINT8[n]), signal values are not used for
                 * filter(ALWAYS, NEVER, ONE_EVERY_N) evaluation */
                TxNewVal_GrpSig = COM_ZERO;
            }

            /* The last parameter is Information from where the call to Com_Filter happens */
            /* NOTE on last parameter:
             * 1.Currently the values for the parameter CallContext_b are COM_TRUE/COM_FALSE
             * 2.It is used only for filter ONE_EVERYN */
            sig_TMSState = Com_Filter((uint32)TxNewVal_GrpSig, TxGrpSigConstPtr->Filter_Algo,
                                                               TxGrpSigConstPtr->Filter_Index,COM_TRUE);

            /* Update TMS TRUE counter , only if filter state of the signal changes.
             * Otherwise, TMS TRUE counter value remains unchanged.
             */
            if (Com_GetRamValue(TXGRPSIG,_SIGCHNGMODE,TxGrpSigRamPtr->txGrpSigRAMFields)!= sig_TMSState)
            {

                /* Signal has changed from FALSE to TRUE, hence increment TMS TRUE counter,
                 * else if Signal has changed from TRUE to FALSE, hence decrement TMS TRUE counter
                 */
                TxIpduRamPtr->Com_TMSTrueCounter_u8 =
                    ((sig_TMSState != COM_FALSE) ? (TxIpduRamPtr->Com_TMSTrueCounter_u8+1):
                                                        (TxIpduRamPtr->Com_TMSTrueCounter_u8-1));

                Com_SetRamValue(TXGRPSIG,_SIGCHNGMODE,TxGrpSigRamPtr->txGrpSigRAMFields,sig_TMSState);
            }
        }
        TxGrpSigConstPtr++;
        TxGrpSigRamPtr++;
    }
    /* Call function to check whether mode is changed or not */
    ReturnValue = Com_Prv_CheckModeChange(TxIpduId);

    SchM_Exit_Com_TxIpduProtArea(SIGGRPTXCHANGEMODE);

    return(ReturnValue);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if (defined(COM_TxFilters) && defined(COM_TX_SIGNALGROUP)) */

