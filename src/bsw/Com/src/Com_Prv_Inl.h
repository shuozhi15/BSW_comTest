

#ifndef COM_PRV_INL_H
#define COM_PRV_INL_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function sets TMS data in to be accessesd via CurrentTxModePtr
 * after evaluating TMS mode for the IPdu
 */
LOCAL_INLINE
FUNC(void, COM_CODE) Com_SetCurrentTxModePtr(
                                Com_TxIpduRamPtrType                TxIpduRamPtr,
                                Com_TxIpduConstPtrType              TxIPduConstPtr,
                                VAR(uint16,AUTOMATIC)               TMSStatus,
                                VAR(boolean,AUTOMATIC)              ModeRequested
                                            );

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to update Rx-GroupSignal application buffers
 */
LOCAL_INLINE
FUNC(void, COM_CODE) Com_UpdateRxSignalBuffer(
                                Com_RxSigConstPtrType               RxSigConstPtr,
                                VAR(Com_SigMaxType, AUTOMATIC)      RxNewVal_Sig,
                                VAR(Com_MainFuncType,AUTOMATIC)     RxMainFuncId
                                             );

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to update Rx-GroupSignal secondary buffers
 */
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE
FUNC(void, COM_CODE) Com_UpdateRxGroupSignalSecondBuffer(
                                Com_RxGrpSigConstPtrType            RxGrpSigConstPtr,
                                VAR(Com_SigMaxType, AUTOMATIC)      RxNewVal,
                                VAR(Com_MainFuncType,AUTOMATIC)     RxMainFuncId
                                                        );
#endif /* #ifdef COM_RX_SIGNALGROUP */

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to initialize signal intial value into the provided IPdu buffer
 */
LOCAL_INLINE
FUNC(void, COM_CODE) Com_InitializePduBuffWithSignalInitValue(
                                P2VAR(uint8,AUTOMATIC,COM_APPL_DATA)    BuffPtr,
                                VAR(Com_SigMaxType,AUTOMATIC)           SigInitVal,
                                VAR(Com_BitpositionType,AUTOMATIC)      SigBitPos,
                                VAR(Com_BitsizeType,AUTOMATIC)          SigBitSize,
                                VAR(uint8,AUTOMATIC)                    SigType,
                                VAR(uint8,AUTOMATIC)                    SigEndianess
                                                            );

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to initialize Rx-signal group with initial values
 */
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE
FUNC(void, COM_CODE) Com_Prv_SigGrpReplaceToInit(
                                Com_RxSigGrpConstPtrType            RxSigGrpConstPtr,
                                VAR(Com_MainFuncType,AUTOMATIC)     RxMainFuncId
                                                );
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallTxNotifyCallback(Com_IpduIdType IpduId);
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallTxErrNotifyCallback(Com_IpduIdType IpduId);
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallTxTONotifyCallback(Com_IpduIdType IpduId);
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallRxTONotifyCallback(Com_IpduIdType IpduId);
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

/* Inline function to check for the RxIPdu and TxIpdu status*/
LOCAL_INLINE boolean Com_CheckRxIPduStatus(PduIdType IpduId);
LOCAL_INLINE boolean Com_CheckTxIPduStatus(PduIdType IpduId);

#if(COM_CONFIGURATION_USE_DET == STD_ON)

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_DETCheck_Void(uint8 ApiId);
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_DETCheck_Param(boolean isInValidParam, uint8 ApiId);
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_DETCheck_Ptr(boolean isInValidPtr, uint8 ApiId);
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_DETCheck_Param_Ptr(boolean isInValidParam, boolean isInValidPtr,
                                                                uint8 ApiId);
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Ptr_Only(boolean isInValidPtr, uint8 ApiId);
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Param_Only(boolean isInValidParam, uint8 ApiId);

#endif /*  #if (COM_CONFIGURATION_USE_DET == STD_ON) */


#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE FUNC(void, COM_CODE) Com_CopyTxGroupSignalData( Com_SignalIdType    GrpSignalId,
                                                             Com_SigMaxType      TxGrpSig_NewVal,
                                                             Com_MainFuncType    TxMainFuncId );
#endif /* #ifdef COM_TX_SIGNALGROUP */

/**
 * @ingroup COM_PRV_INL_H
 *
 * Inline function to handle gateway and non-gateway use-case for tx-signal/signalGroup
 */
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProceedToSendIpdu(
                                                VAR(Com_IpduIdType, AUTOMATIC)          ComTxPduId,
                                                VAR(Com_SendIpduInfo, AUTOMATIC)        sendIpduFlag_u16
                                                           );

#ifdef COM_TxFilters
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_CheckModeChange(VAR(Com_IpduIdType, AUTOMATIC) TxIpduId);
#endif


/**********************************************************************************************************************
 * Inline Function Definition
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Function name    : Com_SetCurrentTxModePtr [Non-AUTOSAR]
 Syntax           : void Com_SetCurrentTxModePtr(
                                Com_TxIpduRamPtrType    TxIpduRamPtr,
                                Com_TxIpduConstPtrType  TxIPduConstPtr,
                                uint16                  TMSStatus,
                                boolean                 ModeRequested
                                            )
 Description      : Inline function sets TMS data in to be accessesd via CurrentTxModePtr
                    after evaluating TMS mode for the IPdu

                    ----------------------------------------------------------------------------------------|
                     * Mode                  |    Configuration details                              | Value|
                    ----------------------------------------------------------------------------------------|
                     * COM_MODE_VALID        |    Both the TRUE and FALSE modes are configured.      |    00|
                     * COM_TRUEMODE_VALID    |    Only TRUE mode is configured                       |    01|
                     * COM_FALSEMODE_VALID   |    Only FALSE mode is configured                      |    10|
                     * COM_MODE_INVALID      |    Both the TRUE and FALSE mode are not configured    |    11|
                    ----------------------------------------------------------------------------------------|
                    This INLINE function checks the TMS generated values against the requested mode and performs action
                    |-------------|--------------|----------|--------------------------------------------------------------------------------------------------------|
                    |ModeRequested|Configured Mode values   |         Description                                           :      Action to be taken                |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    0       |      0        |      0   |  FALSE mode is requested and IPdu TMS is COM_MODE_VALID         : Increment to the false mode data     |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    0       |      0        |      1   |  FALSE mode is requested and IPdu TMS is COM_TRUEMODE_VALID     : Point to NONE mode data              |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    0       |      1        |      0   |  FALSE mode is requested and IPdu TMS is COM_FALSEMODE_VALID     : Do nothing                          |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    0       |      1        |      1   |  FALSE mode is requested and IPdu TMS is COM_MODE_INVALID     : Do nothing                             |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    1       |      0        |      0   |  TRUE mode is requested and IPdu TMS is COM_MODE_VALID         : Do nothing                            |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    1       |      0        |      1   |  TRUE mode is requested and IPdu TMS is COM_TRUEMODE_VALID     : Do nothing                            |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    1       |      1        |      0   |  TRUE mode is requested and IPdu TMS is COM_FALSEMODE_VALID     : Point to NONE mode data              |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|
                    |    1       |      1        |      1   |  TRUE mode is requested and IPdu TMS is COM_MODE_INVALID         : Do nothing                          |
                    |------------|---------------|----------|--------------------------------------------------------------------------------------------------------|

                    where 0 in column 1 stands for FALSE mode
                    and   1 in column 1 stands for TRUE mode

 Parameter        : TxIpduRamPtr    - pointer to Txipdu ram data
                  : TxIPduConstPtr  - pointer to Txipdu Rom data
                  : TMSStatus       - pointer to transmission mode
                  : ModeRequested   - mode requested
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE
FUNC(void, COM_CODE) Com_SetCurrentTxModePtr(
                                Com_TxIpduRamPtrType                TxIpduRamPtr,
                                Com_TxIpduConstPtrType              TxIPduConstPtr,
                                VAR(uint16,AUTOMATIC)               TMSStatus,
                                VAR(boolean,AUTOMATIC)              ModeRequested
                                            )
{
    /* Get the latest TM pointer */
    TxIpduRamPtr->CurrentTxModePtr = TxIPduConstPtr->Com_TMConstPtr;

    if(ModeRequested == COM_FALSE)
    {
        if((TMSStatus) == COM_TRUEMODE_VALID)
        {
            /*If yes,load NONE mode data */
            TxIpduRamPtr->CurrentTxModePtr = &Com_NONE_TransModeInfo ;
        }
        if((TMSStatus) == COM_MODE_VALID)
        {
            /*Increment to FALSE mode data, if the FALSE mode is configured */
            TxIpduRamPtr->CurrentTxModePtr++;
        }
    }
    else
    {
        if((TMSStatus) == COM_FALSEMODE_VALID)
        {
            TxIpduRamPtr->CurrentTxModePtr = &Com_NONE_TransModeInfo ;
        }
    }
}

/**********************************************************************************************************************
 Function name    : Com_UpdateRxGroupSignalSecondBuffer [Non-AUTOSAR]
 Syntax           : void Com_UpdateRxGroupSignalSecondBuffer(
                                Com_RxGrpSigConstPtrType    RxGrpSigConstPtr,
                                Com_SigMaxType              RxNewVal,
                                Com_MainFuncType            RxMainFuncId
                                            )
 Description      : Inline function copy Group signal value into the Group signal Second buffer
 Parameter        : RxGrpSigConstPtr - pointer to RxGroupSignal configuration
                  : RxNewVal         - latest RxGroupSignal data
                  : RxMainFuncId     - Internally generated Rx-MainFunction-Id
 Return value     : None
 *********************************************************************************************************************/
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE
FUNC(void, COM_CODE) Com_UpdateRxGroupSignalSecondBuffer(
                                Com_RxGrpSigConstPtrType            RxGrpSigConstPtr,
                                VAR(Com_SigMaxType, AUTOMATIC)      RxNewVal,
                                VAR(Com_MainFuncType,AUTOMATIC)     RxMainFuncId
                                                        )
{
    VAR(uint8,AUTOMATIC)    Type;

    Type = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);

    switch(Type >> 1u)
    {
    case 0x00u:
        /* UINT08 signal */
        COM_GET_SECRXGRPSIGTYPEU8_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) = (uint8)RxNewVal;
        break;

    case 0x01u:
        /* UINT16 signal */
        COM_GET_SECRXGRPSIGTYPEU16_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) = (uint16)RxNewVal;
        break;

    case 0x02u:
        /* UINT32 signal */
        /* FC_VariationPoint_START */
    case (COM_FLOAT32 >> 1u):
        /* FLOAT32 signal */
        /* FC_VariationPoint_END */
        COM_GET_SECRXGRPSIGTYPEU32_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) = (uint32)RxNewVal;
        break;

#ifdef COM_RXGRPSIG_INT64
    case 0x08u:
        /* UINT64/SINT64 signal */
        COM_GET_SECRXGRPSIGTYPEU64_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) = RxNewVal;
        break;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

    case 0x03u:
        /*Boolean Signal*/
        COM_GET_SECRXGRPSIGTYPEU8_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) = (uint8)RxNewVal;
        break;

    case 0x04u:
        /* UINT8_N signal */
        Com_ByteCopyInit(&COM_GET_SECRXGRPSIGTYPEU8_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index),
                         (uint32)RxNewVal,RxGrpSigConstPtr->BitSize);
        break;
    default:
        /**
         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
         * to be configured, hence default case is intentionally left empty.
         */
        break;

    }
}
#endif

/**********************************************************************************************************************
 Function name    : Com_UpdateRxSignalBuffer [Non-AUTOSAR]
 Syntax           : void Com_UpdateRxSignalBuffer(
                                Com_RxSigConstPtrType   RxSigConstPtr,
                                Com_SigMaxType          RxNewVal_Sig,
                                Com_MainFuncType        RxMainFuncId
                                            )
 Description      : Inline function to copy signal value into the signal buffer and
                    is used when signal value as a variable is being passed
 Parameter        : RxSigConstPtr - pointer to RxSignal configuration
                  : RxNewVal_Sig  - latest RxSignal data
                  : RxMainFuncId  - Internally generated Rx-MainFunction-Id
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE
FUNC(void, COM_CODE) Com_UpdateRxSignalBuffer(
                                Com_RxSigConstPtrType               RxSigConstPtr,
                                VAR(Com_SigMaxType, AUTOMATIC)      RxNewVal_Sig,
                                VAR(Com_MainFuncType,AUTOMATIC)     RxMainFuncId
                                             )
{
    VAR(uint8,AUTOMATIC)    Type;

    Type = Com_GetValue(GEN,_TYPE,RxSigConstPtr->General);

    switch(Type >> 1u)
    {
    case 0x00u:     /* COM_UINT8 & COM_SINT8 */
    case (COM_BOOLEAN >> 1):
        COM_GET_SIGTYPEUINT8_BUFF(RxMainFuncId,RxSigConstPtr->SigBuff_Index) = (uint8)RxNewVal_Sig;
        break;

    case 0x01u:     /* COM_UINT16 & COM_SINT16 */
        COM_GET_SIGTYPEUINT16_BUFF(RxMainFuncId,RxSigConstPtr->SigBuff_Index) = (uint16)RxNewVal_Sig;
        break;

    case 0x02u:     /* COM_UINT32 & COM_SINT32 */
/* FC_VariationPoint_START */
        /* COM_FLOAT32 signal */
#ifdef COM_FLOAT32SUPP
    case (COM_FLOAT32 >> 1u):
#endif /* #ifdef COM_FLOAT32SUPP */
/* FC_VariationPoint_END */
        COM_GET_SIGTYPEUINT32_BUFF(RxMainFuncId,RxSigConstPtr->SigBuff_Index) = (uint32)RxNewVal_Sig;
        break;

#ifdef COM_RXSIG_INT64
    case 0x08u:     /* COM_UINT64 & COM_SINT64 */
        COM_GET_SIGTYPEUINT64_BUFF(RxMainFuncId,RxSigConstPtr->SigBuff_Index) = RxNewVal_Sig;
        break;
#endif /* #if defined(COM_RXSIG_INT64) && defined(COM_TXSIG_INT64) */

    case 0x04u:     /* COM_UINT8_N */
        Com_ByteCopyInit(&COM_GET_SIGTYPEUINT8_BUFF(RxMainFuncId,RxSigConstPtr->SigBuff_Index),
                         (uint32)RxNewVal_Sig,RxSigConstPtr->BitSize);
        break;
    default:
        /**
         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
         * to be configured, hence default case is intentionally left empty.
         */
        break;
    }
}

/**********************************************************************************************************************
 Function name    : Com_InitializePduBuffWithSignalInitValue [Non-AUTOSAR]
 Syntax           : void Com_InitializePduBuffWithSignalInitValue(
                                P2VAR(uint8,AUTOMATIC,COM_APPL_DATA)    BuffPtr,
                                VAR(Com_SigMaxType,AUTOMATIC)           SigInitVal,
                                VAR(Com_BitpositionType,AUTOMATIC)      SigBitPos,
                                VAR(Com_BitsizeType,AUTOMATIC)          SigBitSize,
                                VAR(uint8,AUTOMATIC)                    SigType,
                                VAR(uint8,AUTOMATIC)                    SigEndianess
                                                                 )
 Description      : Inline function to call required interface to pack signal data into Ipdu buffer
 Parameter        : BuffPtr     - pointer to (SignalGroup)IPdu Buffer
                  : SigInitVal  - (Group)Signal InitValue
                  : SigBitPos   - (Group)Signal Bit Position
                  : SigBitSize  - (Group)Signal Bit Size
                  : SigType     - (Group)Signal Type
                  : SigEndianess- (Group)Signal Endianness
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE
FUNC(void, COM_CODE) Com_InitializePduBuffWithSignalInitValue(
                                P2VAR(uint8,AUTOMATIC,COM_APPL_DATA)    BuffPtr,
                                VAR(Com_SigMaxType,AUTOMATIC)           SigInitVal,
                                VAR(Com_BitpositionType,AUTOMATIC)      SigBitPos,
                                VAR(Com_BitsizeType,AUTOMATIC)          SigBitSize,
                                VAR(uint8,AUTOMATIC)                    SigType,
                                VAR(uint8,AUTOMATIC)                    SigEndianess
                                                            )
{
    if ( SigType != (uint8)COM_UINT8_N )
    {
/* FC_VariationPoint_START */
#if (defined(COM_TXSIG_FLOAT64SUPP) || defined(COM_TXGRPSIG_FLOAT64SUPP) ||\
    (defined(COM_RX_SIGNALGROUP_ARRAY) && defined(COM_RXGRPSIG_FLOAT64SUPP)))
        if ( SigType == COM_FLOAT64 )
        {
            /* Convert the float value into 32 bit stream and then pack the value */
            /* Com_PackFloatSignal(TxSigConstPtr->General.Endianess,TxSigConstPtr->Bit_Pos,
                                   Com_getfloat64InitValue(SigNewVal), TxIPduConstPtr->BuffPtr); */
            Com_PackFloatSignal( SigEndianess, SigBitPos, Com_getfloat64InitValue(SigInitVal), BuffPtr );
        }
        else
#endif
/* FC_VariationPoint_END */
        {
            /* Pack the signal with Initial Value */
            /* Com_PackSignal(TxSigConstPtr->General.Endianess,TxSigConstPtr->Bit_Pos,TxSigConstPtr->BitSize,SigNewVal,
             *                TxIPduConstPtr->BuffPtr);*/
            Com_PackSignal( SigEndianess, SigBitPos, SigBitSize, SigInitVal, BuffPtr );
        }
    }
    else
    {
        PduLengthType ByteOffset_uo;
        /* Find the starting byte position(ByteNo_uo) of Signal in the IPDU buffer */
        ByteOffset_uo = ( PduLengthType )( SigBitPos >> 3 );

        /* Now copy the Signal into the IPDU buffer */
        Com_ByteCopyInit( (BuffPtr + ByteOffset_uo), (uint32)SigInitVal, SigBitSize );
    }
}

/**********************************************************************************************************************
 Function name    : Com_Prv_SigGrpReplaceToInit
 Syntax           : void Com_Prv_SigGrpReplaceToInit(
                                        Com_RxSigGrpConstPtrType  RxSigGrpConstPtr,
                                        Com_MainFuncType          RxMainFuncId
                                                    )
 Description      : If the SigGrp is enabled with ArrayAccess, RxSigGrpBuffer will be packed with
                    GroupSignal Init Values OR else
                    GroupSignals Shadow buffers will be replaced with its Init Values
 Parameter        : RxSigGrpConstPtr - pointer to RxSignalGroup configuration
                  : RxMainFuncId     - Internally generated RxMainFunction-ID
 Return value     : None
 *********************************************************************************************************************/
#ifdef COM_RX_SIGNALGROUP
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_SigGrpReplaceToInit(
                                                Com_RxSigGrpConstPtrType        RxSigGrpConstPtr,
                                                VAR(Com_MainFuncType,AUTOMATIC) RxMainFuncId
                                                             )
{
    Com_RxGrpSigConstPtrType      RxGrpSigConstPtr;      /* Local pointer to Rx GroupSignal static configuration */
# ifdef COM_RX_SIGNALGROUP_ARRAY
    Com_RxSigGrpArrayConstPtrType RxSigGrpArrayConstPtr; /* Local pointer to Rx SignalGroup Array configuration  */
    uint8                       * RxSigGrpArrayBufRamPtr;
    PduLengthType                 SigGrpBitOffset;
# endif
    VAR(uint16_least, AUTOMATIC)  index_ui;
    VAR(Com_SigMaxType, AUTOMATIC) SigValue;
# if (defined(COM_RX_SIGNALGROUP_ARRAY) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_INT64))
    VAR(uint8, AUTOMATIC)         SigType;  /* Signal Type */
# endif
# ifdef COM_RX_SIGNALGROUP_ARRAY
    boolean                       ConstIsSigGrpOfArrayType_b;

    ConstIsSigGrpOfArrayType_b = Com_GetValue(RXSIGGRP,_ARRAYACCESS,RxSigGrpConstPtr->rxSignalGrpFields);

    if ( ConstIsSigGrpOfArrayType_b )
    {
        RxSigGrpArrayConstPtr  = COM_GET_RXSIGGRP_ARRAY_CONSTDATA( RxSigGrpConstPtr->SigGrpArray_Index );

        RxSigGrpArrayBufRamPtr = &COM_GET_RXSIGGRP_ARRAY_BUFF(RxMainFuncId, RxSigGrpArrayConstPtr->RxSigGrpBuf_Index );

        SigGrpBitOffset        = ( RxSigGrpArrayConstPtr->FirstByteNo * 8u );

        SchM_Enter_Com_RxSigGrpArrayBuff();
    }
    else
# endif /* # ifdef COM_RX_SIGNALGROUP_ARRAY */
    {
        SchM_Enter_Com_RxGrpSigSecBuff();
    }

    RxGrpSigConstPtr = COM_GET_RXGRPSIG_CONSTDATA( RxSigGrpConstPtr->FirstGrpSig_Index );

    for ( index_ui = (uint16_least)RxSigGrpConstPtr->No_Of_GrpSig; index_ui != (uint16_least)0; index_ui-- )
    {
# if (defined(COM_RX_SIGNALGROUP_ARRAY) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_INT64))
        SigType = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);
# endif

        SigValue = (Com_SigMaxType)RxGrpSigConstPtr->Init_Val;

#ifdef COM_RXGRPSIG_INT64
        /* [SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
         * to the size of its ComSignalType.
         * Currently for COM_SINT64 signal type, ComSignalInitValue is supported for maximum 32bits.
         * If negative values are configured, by default these values will be sign extended only till 32bits.
         * so, for COM_SINT64, it is required to perform the sign extension till 64bits explicity. */
        SigValue = (SigType == COM_SINT64) ? (Com_SigMaxType)((sint64)((sint32)SigValue)) : SigValue;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

# ifdef COM_RX_SIGNALGROUP_ARRAY
        if ( ConstIsSigGrpOfArrayType_b )
        {
            Com_BitpositionType RelativeSigBitPos; /* Relative BitPosition in RxSignalGroup Array */

            /* The Buffer into which Signal Values needs to be packed will be available with starting address
             * of the Rx-SignalGroup Array Buffer.
             * But we have GroupSignal: Bit_Pos, i.e., Signal's actual LSB in the IPdu Buffer.
             * So, Rx-SignalGroup BitOffset(SigGrpBitOffset) is removed from each of the GroupSignal and
             * is used as BitPosition to pack its value into the Rx-SignalGroup Array Buffer */
            RelativeSigBitPos = RxGrpSigConstPtr->Bit_Pos - ( Com_BitpositionType )SigGrpBitOffset;

            Com_InitializePduBuffWithSignalInitValue(
                                              RxSigGrpArrayBufRamPtr,
                                              SigValue,
                                              RelativeSigBitPos,
                                              RxGrpSigConstPtr->BitSize,
                                              SigType,
                                              Com_GetValue(RXGRPSIG,_ENDIANESS,RxGrpSigConstPtr->rxGrpSigFields) );
        }
        else
# endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */
        {
/* FC_VariationPoint_START */
# ifdef COM_RXGRPSIG_FLOAT64SUPP
            if(SigType == COM_FLOAT64)
            {
                COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(RxMainFuncId,RxGrpSigConstPtr->RxGrpSigBuff_Index) =
                                                                      Com_getfloat64InitValue(SigValue);
            }
            else
# endif
/* FC_VariationPoint_END */
            {
                Com_UpdateRxGroupSignalSecondBuffer(RxGrpSigConstPtr, SigValue, RxMainFuncId);
            }
        }
# if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)
        /* [SWS_Com_00603] The AUTOSAR COM module shall set the old_value of the filtering mechanisms
         * for each signal to the ComSignalInitValue during start-up */
        if ( ( COM_INIT != Com_Uninit_Flag ) && ( COM_MASKED_NEW_DIFFERS_MASKED_OLD == RxGrpSigConstPtr->Filter_Algo ) )
        {
            COM_GET_F_OLDVALUE(RxGrpSigConstPtr->Filter_Index) = (uint32)SigValue;
        }
# endif /* # if defined(COM_RxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD) */

        RxGrpSigConstPtr++;
    }
# ifdef COM_RX_SIGNALGROUP_ARRAY

    if ( ConstIsSigGrpOfArrayType_b )
    {
        SchM_Exit_Com_RxSigGrpArrayBuff();
    }
    else
# endif /* # ifdef COM_RX_SIGNALGROUP_ARRAY */
    {
        SchM_Exit_Com_RxGrpSigSecBuff();
    }
}
#endif /* #ifdef COM_RX_SIGNALGROUP */

#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#ifdef COM_TxIPduNotification
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallTxNotifyCallback(Com_IpduIdType IpduId)
        {
    Com_TxIpduConstPtrType TxIPduConstPtr;    /* Local pointer to Tx ipdu static configuration */
    Com_TxSigConstPtrType TxSigConstPtr;    /* Local pointer to Tx signal static configuration */
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpConstPtrType TxSigGrpConstPtr; /* Local pointer to hold the address of Tx SignalGroup static configuration */
#endif /* #ifdef COM_TX_SIGNALGROUP */
    uint8_least index_u8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
    * to internal Id which is generated through configuration
    * If PC variant is selected, then no mapping table will be used. */

    IpduId = COM_GET_TX_IPDU_ID(IpduId);
    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);
    TxSigConstPtr = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);
#ifdef COM_TX_SIGNALGROUP
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(TxIPduConstPtr->FirstTxSigGrp_Index);
#endif /* #ifdef COM_TX_SIGNALGROUP */

    for(index_u8 = TxIPduConstPtr->TxSigId_Index; index_u8 < (TxIPduConstPtr->TxSigId_Index + TxIPduConstPtr->No_Of_Sig_Ref); index_u8++)
    {
        if(TxSigConstPtr->Notification_Cbk != NULL_PTR)
        {
            TxSigConstPtr->Notification_Cbk();
        }
        TxSigConstPtr++;
    }
#ifdef COM_TX_SIGNALGROUP
    for(index_u8 = TxIPduConstPtr->FirstTxSigGrp_Index; index_u8 < (TxIPduConstPtr->FirstTxSigGrp_Index + TxIPduConstPtr->No_Of_SigGrp_Ref); index_u8++)
    {
        if(TxSigGrpConstPtr->Notification_Cbk != NULL_PTR)
        {
            TxSigGrpConstPtr->Notification_Cbk();
        }
        TxSigGrpConstPtr++;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */

        }
#endif /* #ifdef COM_TxIPduNotification */

#ifdef COM_ERRORNOTIFICATION
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallTxErrNotifyCallback(Com_IpduIdType IpduId)
{

    Com_TxIpduConstPtrType TxIPduConstPtr;    /* Local pointer to Tx ipdu static configuration */
    Com_TxSigConstPtrType TxSigConstPtr;    /* Local pointer to Tx signal static configuration */
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpConstPtrType TxSigGrpConstPtr; /* Local pointer to hold the address of Tx SignalGroup static configuration */
#endif /* #ifdef COM_TX_SIGNALGROUP */
    uint8_least index_u8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
    * to internal Id which is generated through configuration
    * If PC variant is selected, then no mapping table will be used. */

    IpduId = COM_GET_TX_IPDU_ID(IpduId);
    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);
    TxSigConstPtr = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);
#ifdef COM_TX_SIGNALGROUP
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(TxIPduConstPtr->FirstTxSigGrp_Index);
#endif /* #ifdef COM_TX_SIGNALGROUP */

    for(index_u8 = TxIPduConstPtr->TxSigId_Index; index_u8 < (TxIPduConstPtr->TxSigId_Index + TxIPduConstPtr->No_Of_Sig_Ref); index_u8++)
    {
        if(TxSigConstPtr->NotificationError_Cbk != NULL_PTR)
        {
            TxSigConstPtr->NotificationError_Cbk();
        }
        TxSigConstPtr++;
    }
#ifdef COM_TX_SIGNALGROUP
    for(index_u8 = TxIPduConstPtr->FirstTxSigGrp_Index; index_u8 < (TxIPduConstPtr->FirstTxSigGrp_Index + TxIPduConstPtr->No_Of_SigGrp_Ref); index_u8++)
    {
        if(TxSigGrpConstPtr->NotificationError_Cbk != NULL_PTR)
        {
            TxSigGrpConstPtr->NotificationError_Cbk();
        }
        TxSigGrpConstPtr++;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */

}
#endif /* #ifdef COM_ERRORNOTIFICATION */

#ifdef COM_TxIPduTimeOutNotify
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallTxTONotifyCallback(Com_IpduIdType IpduId)
{

    Com_TxIpduConstPtrType TxIPduConstPtr;    /* Local pointer to Tx ipdu static configuration */
    Com_TxSigConstPtrType TxSigConstPtr;    /* Local pointer to Tx signal static configuration */
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpConstPtrType TxSigGrpConstPtr; /* Local pointer to hold the address of Tx SignalGroup static configuration */
#endif /* #ifdef COM_TX_SIGNALGROUP */
    uint8_least index_u8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
    * to internal Id which is generated through configuration
    * If PC variant is selected, then no mapping table will be used. */

    IpduId = COM_GET_TX_IPDU_ID(IpduId);
    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);
    TxSigConstPtr = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);
#ifdef COM_TX_SIGNALGROUP
    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(TxIPduConstPtr->FirstTxSigGrp_Index);
#endif /* #ifdef COM_TX_SIGNALGROUP */

    for(index_u8 = TxIPduConstPtr->TxSigId_Index; index_u8 < (TxIPduConstPtr->TxSigId_Index + TxIPduConstPtr->No_Of_Sig_Ref); index_u8++)
    {
        if(TxSigConstPtr->TimeOutNotification_Cbk != NULL_PTR)
        {
            TxSigConstPtr->TimeOutNotification_Cbk();
        }
        TxSigConstPtr++;
    }
#ifdef COM_TX_SIGNALGROUP
    for(index_u8 = TxIPduConstPtr->FirstTxSigGrp_Index; index_u8 < (TxIPduConstPtr->FirstTxSigGrp_Index + TxIPduConstPtr->No_Of_SigGrp_Ref); index_u8++)
    {
        if(TxSigGrpConstPtr->TimeOutNotification_Cbk != NULL_PTR)
        {
            TxSigGrpConstPtr->TimeOutNotification_Cbk();
        }
        TxSigGrpConstPtr++;
    }
#endif /* #ifdef COM_TX_SIGNALGROUP */

}
#endif /* #ifdef COM_TxIPduTimeOutNotify */
#ifdef COM_RxIPduTimeoutNotify
LOCAL_INLINE FUNC(void, COM_CODE) Com_CallRxTONotifyCallback(Com_IpduIdType IpduId)
{

    Com_RxIpduConstPtrType RxIPduConstPtr;    /* Local pointer to Rx ipdu static configuration */
    Com_RxSigConstPtrType RxSigConstPtr;    /* Local pointer to Rx signal static configuration */
#ifdef COM_RX_SIGNALGROUP
    Com_RxSigGrpConstPtrType RxSigGrpConstPtr; /* Local pointer to hold the address of Rx SignalGroup static configuration */
#endif /* #ifdef COM_RX_SIGNALGROUP */
    uint8_least index_u8;

    /* If PB variant is selected, then PduId which is passed to this function will be changed
    * to internal Id which is generated through configuration
    * If PC variant is selected, then no mapping table will be used. */

    IpduId = COM_GET_RX_IPDU_ID(IpduId);
    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(IpduId);
    RxSigConstPtr = COM_GET_RXSIG_CONSTDATA(RxIPduConstPtr->RxSigId_Index);
#ifdef COM_RX_SIGNALGROUP
    RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA(RxIPduConstPtr->FirstRxSigGrp_Index);
#endif /* #ifdef COM_RX_SIGNALGROUP */

    for(index_u8 = RxIPduConstPtr->RxSigId_Index; index_u8 < (RxIPduConstPtr->RxSigId_Index + RxIPduConstPtr->No_Of_Sig_Ref); index_u8++)
    {
        if(RxSigConstPtr->TimeOutNotification_Cbk != NULL_PTR)
        {
            RxSigConstPtr->TimeOutNotification_Cbk();
        }
        RxSigConstPtr++;
    }
#ifdef COM_RX_SIGNALGROUP
    for(index_u8 = RxIPduConstPtr->FirstRxSigGrp_Index; index_u8 < (RxIPduConstPtr->FirstRxSigGrp_Index + RxIPduConstPtr->No_Of_SigGrp_Ref); index_u8++)
    {
        if(RxSigGrpConstPtr->TimeOutNotification_Cbk != NULL_PTR)
        {
            RxSigGrpConstPtr->TimeOutNotification_Cbk();
        }
        RxSigGrpConstPtr++;
    }
#endif /* #ifdef COM_RX_SIGNALGROUP */

}
#endif /* #ifdef COM_RxIPduTimeoutNotify */
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE FUNC(void, COM_CODE) Com_CopyTxGroupSignalData( Com_SignalIdType    GrpSignalId,
                                                             Com_SigMaxType      TxGrpSig_NewVal,
                                                             Com_MainFuncType    TxMainFuncId )
{
    Com_TxGrpSigConstPtrType                   TxGrpSigConstPtr;
    VAR(Com_TxGrpSigBuffIndexType, AUTOMATIC)  GrpSigBuff_Index;
    VAR(uint8, AUTOMATIC)                      Type;

    TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(GrpSignalId);
    GrpSigBuff_Index = TxGrpSigConstPtr->TxGrpSigBuff_Index;
    Type = Com_GetValue(TXGRPSIG,_TYPE,TxGrpSigConstPtr->txGrpSigFields);

    SchM_Enter_Com_TxGrpSigBuffer();
    switch(Type >> 1u)
    {

        case 0x00u: /* UINT8 and SINT8 type */
        case 0x03u: /* Boolean type */
            COM_GET_TXGRPSIGTYPEU8_BUFF(TxMainFuncId, GrpSigBuff_Index) = (uint8)TxGrpSig_NewVal;
            break;

        /* UINT16 and SINT16 type */
        case 0x01u:
            COM_GET_TXGRPSIGTYPEU16_BUFF(TxMainFuncId, GrpSigBuff_Index) = (uint16)TxGrpSig_NewVal;
            break;

        /* UINT32,SINT32 and FLOAT32 type */
        case 0x02u:
        /* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case (COM_FLOAT32 >> 1u):
#endif /* #ifdef COM_FLOAT32SUPP */
        /* FC_VariationPoint_END */
            COM_GET_TXGRPSIGTYPEU32_BUFF(TxMainFuncId, GrpSigBuff_Index) = (uint32)TxGrpSig_NewVal;
            break;

        /* UINT64 and SINT64 type */
#ifdef COM_TXGRPSIG_INT64
        case 0x08u:
            COM_GET_TXGRPSIGTYPEU64_BUFF(TxMainFuncId, GrpSigBuff_Index) = (uint64)TxGrpSig_NewVal;
        break;
#endif /* #ifdef COM_TXGRPSIG_INT64 */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are
             * allowed to be configured, hence default case is intentionally left empty.
             */
        break;
   }
    SchM_Exit_Com_TxGrpSigBuffer();

}
#endif /* #ifdef COM_TX_SIGNALGROUP */


#if(COM_CONFIGURATION_USE_DET == STD_ON)

/**********************************************************************************************************************
 Function name    : Com_Prv_DETCheck_Void
 Syntax           : boolean Com_Prv_DETCheck_Void(uint8 ApiId)
 Description      : Inline function to check for module initialization
 Parameter        : ApiId - Com service id
 Return value     : COM_TRUE  - No development error
                    COM_FALSE - development error is detected, same is reported to DET
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Void(uint8 ApiId)
{
    /* Local variable*/
    boolean Status;

    /* Initialization of local variable */
    Status  = COM_TRUE;

    /* Check if Com is initialized or not */
    if (Com_Uninit_Flag == COM_UNINIT)
    {
        /* Report DET Error */
        COM_REPORT_ERROR_CHECK(ApiId, COM_E_UNINIT);
        /* Update the status */
        Status = COM_FALSE;
    }
    else
    {
        /* No Error */
    }

    return Status;
}

/**********************************************************************************************************************
 Function name    : Com_Prv_DETCheck_Param
 Syntax           : boolean Com_Prv_DETCheck_Param(boolean isInValidParam, uint8 ApiId)
 Description      : Inline function to check for module initialization and valid parameter value.
 Parameter        : isInValidParam - calling service is called with invalid parameter value
                    ApiId          - Com service id
 Return value     : COM_TRUE  - No development error
                    COM_FALSE - development error is detected, same is reported to DET
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Param(boolean isInValidParam, uint8 ApiId)
{
    /* Local variable*/
    uint8   ErrorId;
    boolean Status;

    /* Initialization of local variable */
    ErrorId = COM_DET_NO_ERROR;
    Status  = COM_TRUE;

    /* Check if Com is initialized or not */
    if (Com_Uninit_Flag == COM_UNINIT)
    {
        ErrorId = COM_E_UNINIT;
    }
    else if (isInValidParam)
    {
        ErrorId = COM_E_PARAM;
    }
    else
    {
        /* No Error */
    }

    /* Check if ErrorId has Error Code */
    if (ErrorId != COM_DET_NO_ERROR)
    {
        /* Report DET Error */
        COM_REPORT_ERROR_CHECK(ApiId, ErrorId);
        /* Update the status */
        Status = COM_FALSE;
    }
    return Status;
}

/**********************************************************************************************************************
 Function name    : Com_Prv_DETCheck_Ptr
 Syntax           : boolean Com_Prv_DETCheck_Ptr(boolean isInValidPtr, uint8 ApiId)
 Description      : Inline function to check for module initialization and valid pointer.
 Parameter        : isInValidPtr - calling service is called with invalid pointer
                    ApiId        - Com service id
 Return value     : COM_TRUE  - No development error
                    COM_FALSE - development error is detected, same is reported to DET
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Ptr(boolean isInValidPtr, uint8 ApiId)
{
    /* Local variable*/
    uint8   ErrorId;
    boolean Status;

    /* Initialization of local variable */
    ErrorId = COM_DET_NO_ERROR;
    Status  = COM_TRUE;

    /* Check if Com is initialized or not */
    if (Com_Uninit_Flag == COM_UNINIT)
    {
        ErrorId = COM_E_UNINIT;
    }
    else if (isInValidPtr)
    {
        ErrorId = COM_E_PARAM_POINTER;
    }
    else
    {
        /* No Error */
    }

    /* Check if ErrorId has Error Code */
    if (ErrorId != COM_DET_NO_ERROR)
    {
        /* Report DET Error */
        COM_REPORT_ERROR_CHECK(ApiId, ErrorId);
        /* Update the status */
        Status = COM_FALSE;
    }
    return Status;
}

/**********************************************************************************************************************
 Function name    : Com_Prv_DETCheck_Param_Ptr
 Syntax           : boolean Com_Prv_DETCheck_Param_Ptr(boolean isInValidParam, boolean isInValidPtr, uint8 ApiId)
 Description      : Inline function to check for module initialization, valid parameter value and valid pointer.
 Parameter        : isInValidParam - calling service is called with invalid parameter value
                    isInValidPtr   - calling service is called with invalid pointer
                    ApiId          - Com service id
 Return value     : COM_TRUE  - No development error
                    COM_FALSE - development error is detected, same is reported to DET
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Param_Ptr(boolean isInValidParam, boolean isInValidPtr, uint8 ApiId)
{
    /* Local variable*/
    uint8   ErrorId;
    boolean Status;

    /* Initialization of local variable */
    ErrorId = COM_DET_NO_ERROR;
    Status  = COM_TRUE;

    /* Check if Com is initialized or not */
    if (Com_Uninit_Flag == COM_UNINIT)
    {
        ErrorId = COM_E_UNINIT;
    }
    else if (isInValidParam)
    {
        ErrorId = COM_E_PARAM;
    }
    else if (isInValidPtr)
    {
        ErrorId = COM_E_PARAM_POINTER;
    }
    else
    {
        /* No Error */
    }

    /* Check if ErrorId has Error Code */
    if (ErrorId != COM_DET_NO_ERROR)
    {
        /* Report DET Error */
        COM_REPORT_ERROR_CHECK(ApiId, ErrorId);
        /* Update the status */
        Status = COM_FALSE;
    }
    return Status;
}


/**********************************************************************************************************************
 Function name    : Com_Prv_DETCheck_Ptr_Only
 Syntax           : boolean Com_Prv_DETCheck_Ptr_Only(boolean isInValidPtr, uint8 ApiId)
 Description      : Inline function to check for valid pointer.
 Parameter        : isInValidPtr - calling service is called with invalid pointer
                    ApiId        - Com service id
 Return value     : COM_TRUE  - No development error
                    COM_FALSE - development error is detected, same is reported to DET
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Ptr_Only(boolean isInValidPtr, uint8 ApiId)
{
    /* Local variable*/
    boolean Status;

    /* Initialization of local variable */
    Status = COM_TRUE;

    /* Check if Com is initialized or not */
    if (isInValidPtr)
    {
        /* Report DET Error */
        COM_REPORT_ERROR_CHECK(ApiId, COM_E_PARAM_POINTER);
        /* Update the status */
        Status = COM_FALSE;
    }
    else
    {
        /* No Error */
    }

    return Status;
}


/**********************************************************************************************************************
 Function name    : Com_Prv_DETCheck_Param_Only
 Syntax           : boolean Com_Prv_DETCheck_Param_Only(boolean isInValidParam, uint8 ApiId)
 Description      : Inline function to check for module initialization and valid parameter value.
 Parameter        : isInValidParam - calling service is called with invalid parameter value
                    ApiId          - Com service id
 Return value     : COM_TRUE  - No development error
                    COM_FALSE - development error is detected, same is reported to DET
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(boolean,COM_CODE) Com_Prv_DETCheck_Param_Only(boolean isInValidParam, uint8 ApiId)
{
    /* Local variable*/
    boolean Status;

    /* Initialization of local variable */
    Status = COM_TRUE;

    if (isInValidParam)
    {
        /* Report DET Error */
        COM_REPORT_ERROR_CHECK(ApiId, COM_E_PARAM);
        /* Update the status */
        Status = COM_FALSE;
    }
    else
    {
        /* No Error */
    }

    return Status;
}

#endif /* #if(COM_CONFIGURATION_USE_DET == STD_ON) */

/**********************************************************************************************************************
 Function name    : Com_CheckTxIPduStatus
 Syntax           : boolean Com_CheckTxIPduStatus(PduIdType IpduId)
 Description      : Inline function to check TxIPdu status
 Parameter        : IpduId - TxIPdu Id
 Return value     : COM_TRUE  - TxIPdu is started
                    COM_FALSE - TxIPdu is stopped
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_CheckTxIPduStatus(PduIdType IpduId)
{
    VAR(boolean, AUTOMATIC) TxIPduStatus;

    TxIPduStatus =  Com_GetRamValue(TXIPDU,_PDUSTATUS,COM_GET_TXPDURAM_S(IpduId).Com_TxFlags)
                /* FC_VariationPoint_START */
                /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
                 * of operands does not impact the system behaviour even though of volatile-qualified,
                 * Hence the warning is suppressed. */
#ifdef COM_TX_IPDUCONTROL_VIA_CALIBRATION
                && (COM_CheckTxIPduCalibrationVarStatus((PduIdType)(IpduId)))
#endif /* #ifdef COM_TX_IPDUCONTROL_VIA_CALIBRATION */
#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
                && (Com_CheckTxIPduControlViaRbaNdsEcuVariant((PduIdType)(IpduId)))
#endif /* #ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
                /* FC_VariationPoint_END */
                         ;

    return TxIPduStatus;
}

/**********************************************************************************************************************
 Function name    : Com_CheckRxIPduStatus
 Syntax           : boolean Com_CheckRxIPduStatus(PduIdType IpduId)
 Description      : Inline function to check RxIPdu status
 Parameter        : IpduId - RxIPdu Id
 Return value     : COM_TRUE  - RxIPdu is started
                    COM_FALSE - RxIPdu is stopped
 *********************************************************************************************************************/
LOCAL_INLINE boolean Com_CheckRxIPduStatus(PduIdType IpduId)
{
    VAR(boolean, AUTOMATIC) RxIPduStatus;

    RxIPduStatus = Com_GetRamValue(RXIPDU,_PDUSTATUS,COM_GET_RXPDURAM_S(IpduId).RxFlags)
                /* FC_VariationPoint_START */
                /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
                 * of operands does not impact the system behaviour even though of volatile-qualified,
                 * Hence the warning is suppressed. */
#ifdef COM_RX_IPDUCONTROL_VIA_CALIBRATION
                && (COM_CheckRxIPduCalibrationVarStatus((PduIdType)(IpduId)))
#endif /* #ifdef COM_RX_IPDUCONTROL_VIA_CALIBRATION */
#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
                && (Com_CheckRxIPduControlViaRbaNdsEcuVariant((PduIdType)(IpduId)))
#endif /* #ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
                /* FC_VariationPoint_END */
                        ;

    return RxIPduStatus;
}

/**********************************************************************************************************************
 Function name    : Com_Prv_ProceedToSendIpdu
 Syntax           : void Com_Prv_ProceedToSendIpdu(Com_IpduIdType ComTxPduId, Com_SendIpduInfo sendIpduFlag_u16)
 Description      : 1. For gateway txIpdu - set the flags here and defer the transmission till
                        Com_MainFunctionRouteSignals
                    2. For non-gateway txIPdu - trigger the IPdu only for event type or for mode change.
 Parameter        : IpduId_ui        -> Internal Id of the TxIPdu
                    sendIpduFlag_u16 -> flags to decide ipdu transmission
 Return value     : None
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProceedToSendIpdu(
                                                VAR(Com_IpduIdType, AUTOMATIC)   ComTxPduId,
                                                VAR(Com_SendIpduInfo, AUTOMATIC) sendIpduFlag_u16
                                                           )
{
    Com_TxIpduConstPtrType  TxIPduConstPtr;
    Com_TxIpduRamPtrType    TxIpduRamPtr;

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(ComTxPduId);
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(ComTxPduId);

#if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY)

    if (Com_GetValue(TXIPDU,_ISGWPDU,TxIPduConstPtr->txIPduFields) != COM_FALSE)
    {
        /* If Signal is a part of Gw-Ipdu, then defer till Com_MainFunctionRouteSignals */
        if (sendIpduFlag_u16.sigTransProp == COM_SET)
        {
            /* For Tx Ipdus with Gw signals the event transmissions are done from Com_MainfunctionRouteSignals.
             * The below step will raise a trigger in the next call of Com_MainfunctionRouteSignals, and
             * the Tx Ipdu is transmitted.
             * The Gw Ipdus which are cyclically triggered are sent from Com_mainfunctionTx */
            Com_SetRamValue(TXIPDU,_ISGWTRIGGRD,TxIpduRamPtr->Com_TxFlags,COM_TRUE);
        }

#ifdef COM_TxFilters
        /* If the TMS changes, set the global TMS flag to consolidate transmission mode change
         * for the Gw-Tx IPdu after its last transmission */
        if (sendIpduFlag_u16.isModeChangd == COM_SET)
        {
            /* Consolidate the TMS Changes */
            if (Com_GetRamValue(TXIPDU,_GWIPDUTMSCHANGED,TxIpduRamPtr->Com_TxFlags))
            {
                Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
            }
            else
            {
                /* Store the transmission mode change and shall be used while triggering the Tx-IPdu from
                 * Com_MainFunctionRouteSignals() */
                Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,TxIpduRamPtr->Com_TxFlags,COM_TRUE);
            }
        }
#endif /* #ifdef COM_TxFilters */

#ifdef COM_TxIPduTimeOut
        if (sendIpduFlag_u16.isTimeoutReq == COM_SET)
        {
            /* This flag is set to start Timeout monitoring in the next call of Com_MainfunctionRouteSignals */
            Com_SetRamValue(TXIPDU,_ISTOENABLED,TxIpduRamPtr->Com_TxFlags,COM_TRUE);
        }
#endif /* #ifdef COM_TxIPduTimeOut */
    }
    else
#endif /* #if defined(COM_SIGNALGATEWAY) || defined(COM_SIGNALGROUPGATEWAY) */
    {
        /* Process Tx-IPdu Transmission.
         * If this signal TX request causes mode change to MIXED/PERIODIC TM & signal transfer property is PENDING,
         * then do not transmit the IPDU. Because IPDU will be transmitted immediately,
         * due to periodic part of MIXED/PERIODIC mode.
         */
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status.
         * Evaluation(or no evaluation) of the 2nd operand does not impact the system behaviour.
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        if (((sendIpduFlag_u16.sigTransProp == COM_SET) &&
             (Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode) <= COM_TXMODE_MIXED))
#ifdef COM_TxFilters
            || (sendIpduFlag_u16.isModeChangd == COM_SET)
#endif
           )
        {
            /* this flag will be set, as call happens apart from Com_MainFunctionTx */
            sendIpduFlag_u16.isEventTrig = COM_SET;

            Com_Prv_SendIpdu((PduIdType)ComTxPduId, sendIpduFlag_u16);
        }
    }
}   /* End of Com_Prv_ProceedToSendIpdu */


#ifdef COM_TxFilters

/***************************************************************************************************
 Function name    : Com_Prv_CheckModeChange
 Syntax           : LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_CheckModeChange(Com_TxIpduRamPtrType TxIpduRamPtr,
                                                                 Com_TxIpduConstPtrType TxIPduConstPtr)
 Description      : This function evaluates the current TMS of the IPDU, based on the corresponding signal or signal group which is updated.
 Parameter        : TxIpduId -> Id of the Tx ipdu
                  : SignalId -> Id of the signal
                  : TxNewVal -> New signal value
                  : IsSignalGroup -> boolean to check whether it is an SignalGroup or not
 Return value     : COM_TRUE/COM_FALSE
 ***************************************************************************************************/
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Prv_CheckModeChange(VAR(Com_IpduIdType, AUTOMATIC) TxIpduId)
{

    /* Variable declaration */
    Com_TxIpduConstPtrType  TxIPduConstPtr;    /* Local pointer to Tx ipdu   static  configuration */
    Com_TxIpduRamPtrType    TxIpduRamPtr;      /* Local pointer to Tx ipdu   dynamic configuration */
    VAR(boolean, AUTOMATIC) ReturnValue;
    VAR(boolean, AUTOMATIC) Mode;

    /* Initialization of variables */
    TxIpduRamPtr    = &COM_GET_TXPDURAM_S(TxIpduId);
    TxIPduConstPtr  = COM_GET_TX_IPDU_CONSTDATA(TxIpduId);

    Mode            = COM_FALSE;
    ReturnValue     = COM_FALSE;

    if (TxIpduRamPtr ->Com_TMSTrueCounter_u8 != 0)
    {
        /* After evaluating filter for current signal/signal group, check if one of the signal/signal group filter
         * state is TRUE
         * Otherwise init value for Mode ie COM_FALSE is used
         */
        Mode = COM_TRUE;
    }

    /*If there is a Mode change detected for the IPdu */
    if(Com_GetRamValue(TXIPDU,_TXMODESTATE,TxIpduRamPtr->Com_TransMode) != Mode)
    {
        ReturnValue = COM_TRUE;
        /* Update the Com_TxModeState with the Mode */
        Com_SetRamValue(TXIPDU,_TXMODESTATE,TxIpduRamPtr->Com_TransMode,Mode);
        /* Update the CurrentTxModePtr */
        Com_SetCurrentTxModePtr(TxIpduRamPtr,TxIPduConstPtr,Com_GetValue(TXIPDU,_TMS,TxIPduConstPtr->txIPduFields),Mode);

        /* Set the mode to latest mode */
        Com_SetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode,TxIpduRamPtr->CurrentTxModePtr->Mode);

        /* Note: Timer update because of mode change happens in Com_sendIpdu */
        /* FC_VariationPoint_START */
#ifdef COM_MIXEDPHASESHIFT
        /* Initialise MixedPhaseShift state, when IPdu tx mode is calculated in Com_IPduGroupStart() */
        Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,TxIpduRamPtr->Com_TransMode,TxIpduRamPtr->CurrentTxModePtr->MixedPhaseShift);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
        /* FC_VariationPoint_END */
    }

    return(ReturnValue);
}

#endif /* #ifdef COM_TxFilters */


#endif /* COM_PRV_INL_H */

