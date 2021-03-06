

/*
**********************************************************************************************************************
* Includes
**********************************************************************************************************************
*/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/*
**********************************************************************************************************************
* Global function Definitions (declared in header files)
**********************************************************************************************************************
*/

/* Com_SendSignalGroupArray() API shall only be available when the configuration switch
 *  ComEnableSignalGroupArrayApi is enabled */
#if defined ( COM_TX_SIGNALGROUP_ARRAY )

# ifdef COM_EffectiveSigGrpTOC
/* Check if GroupSignal data has changed from the last update */
LOCAL_INLINE FUNC(void, COM_CODE) Com_InternalSigGrpArrayTrigOnChange( Com_SignalGroupIdType SignalGroupId );
# endif /* #ifdef COM_EffectiveSigGrpTOC */

/*
**********************************************************************************************************************
* Function name    : Com_SendSignalGroupArray
* Syntax           : uint8
*                    Com_SendSignalGroupArray( Com_SignalGroupIdType SignalGroupId, const uint8* SignalGroupArrayPtr )
* Description      : The service Com_SendSignalGroupArray copies the content of the provided SignalGroupArrayPtr to
*                    the associated I-PDU. The provided data shall correspond to the array representation of
*                    the signal group.
* Parameters    IN : SignalGroupId       - Id of signal group to be sent
*                    SignalGroupArrayPtr - Reference to the signal group array
* Sync/Async       : Asynchronous
* Reentrancy       : Non Reentrant for the same signal group. Reentrant for different signal groups.
* Return value     : E_OK                      : service has been accepted
*                    COM_SERVICE_NOT_AVAILABLE : corresponding I-PDU group was stopped
*                                                (or service failed due to development error)
*                    COM_BUSY                  : in case the TP-Buffer is locked for large data types handling
**********************************************************************************************************************
*/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"
/* HIS METRIC CALLS VIOLATION IN Com_SendSignalGroupArray:
   Com_SendSignalGroupArray function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(uint8, COM_CODE) Com_SendSignalGroupArray( Com_SignalGroupIdType SignalGroupId, const uint8 * SignalGroupArrayPtr )
{
    Com_TxIpduConstPtrType        TxIpduConstPtr;        /* Local pointer to Tx IPdu             static configuration */
    Com_TxIpduRamPtrType          TxIpduRamPtr;          /* Local pointer to Tx IPdu             RAM    configuration */
    Com_TxSigGrpConstPtrType      TxSigGrpConstPtr;      /* Local pointer to Tx SignalGroup      static configuration */
    Com_TxSigGrpArrayConstPtrType TxSigGrpArrayConstPtr; /* Local pointer to Tx SignalGroupArray static configuration */
    uint8                         ConstxSignalGrpFields;
    uint8                         returnValue_u8;
    #ifdef COM_EffectiveSigGrpTOC
    uint8                         TransferProperty_u8; /* Variable to hold the transfer Property */
    #endif

    returnValue_u8  =  COM_SERVICE_NOT_AVAILABLE;

    /*Call DET Function to check for the DET Error COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER*/
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr((SignalGroupId >= COM_GET_NUM_TX_SIGNALGRP),(SignalGroupArrayPtr == NULL_PTR),
                                                                                COMServiceId_SendSignalGroupArray))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        SignalGroupId         = COM_GET_TXSIGNALGRP_ID( SignalGroupId );
        TxSigGrpConstPtr      = COM_GET_TXSIGGRP_CONSTDATA( SignalGroupId );
        ConstxSignalGrpFields = TxSigGrpConstPtr->txSignalGrpFields;

        /* Proceed only if Tx-SignalGroup is enabled with Array Access */
        if ( Com_GetValue(TXSIGGRP,_ARRAYACCESS,ConstxSignalGrpFields) )
        {
            Com_IpduIdType IpduId_ui;
            IpduId_ui    = TxSigGrpConstPtr->IpduRef;
            TxIpduRamPtr = &COM_GET_TXPDURAM_S( IpduId_ui );
            /* Proceed only if Large Data tranmission is not in Progress */
#ifdef COM_TP_IPDUTYPE
            if ( Com_GetRamValue(TXIPDU,_LARGEDATAINPROG, TxIpduRamPtr->Com_TxFlags) == COM_TRUE )
            {
                returnValue_u8 = COM_BUSY;
            }
            else
#endif /*#ifdef COM_TP_IPDUTYPE*/
            {
                TxIpduConstPtr        = COM_GET_TX_IPDU_CONSTDATA( IpduId_ui );
                TxSigGrpArrayConstPtr = COM_GET_TXSIGGRP_ARRAY_CONSTDATA( TxSigGrpConstPtr->SigGrpArray_Index );
                SchM_Enter_Com_TxIpduProtArea(SENDSIGNALGRP);
                /* The service Com_SendSignalGroupArray() shall copy the content designated by the SignalGroupArrayPtr
                 * to the associated I-PDU Buffer */
                Com_ByteCopy(
                   ( P2VAR(uint8,AUTOMATIC,COM_APPL_DATA) )( TxIpduConstPtr->BuffPtr + TxSigGrpArrayConstPtr->FirstByteNo ),
                 ( P2CONST(uint8,AUTOMATIC,COM_APPL_DATA) )( SignalGroupArrayPtr ),
                                            ( uint32  )( TxSigGrpArrayConstPtr->Length ) );
                SchM_Exit_Com_TxIpduProtArea(SENDSIGNALGRP);
    #  ifdef COM_EffectiveSigGrpTOC
                TransferProperty_u8 = Com_GetValue(TXSIGGRP,_TP,ConstxSignalGrpFields);
                if(COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(TransferProperty_u8))
                {
                    /* Unpack & check, if any of the GroupSignal data has changed whose transfer property is
                     * configured with TRIGGERED_ON_CHANGE/TRIGGERED_ON_CHANGE_WITHOUT_REPETIION */
                    Com_InternalSigGrpArrayTrigOnChange( SignalGroupId );
                }
                else
                {
                    /* Do-nothing */
                }
    #  endif /* #  ifdef COM_EffectiveSigGrpTOC */
                returnValue_u8 = Com_InternalSendSignalGroup( SignalGroupId );

            }
        }
    }
   return returnValue_u8;
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

# ifdef COM_EffectiveSigGrpTOC

/**********************************************************************************************************************
 * Function name : Com_InternalSigGrpArrayTrigOnChange
 * Description   : Check if GroupSignal data has changed from the last update
 * Parameters    : (IN) SignalGroupId - Tx SignalGroup Id
 * Return Value  : None
 *********************************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_InternalSigGrpArrayTrigOnChange( Com_SignalGroupIdType SignalGroupId )
{
    Com_TxIpduConstPtrType    TxIPduConstPtr;   /* Local pointer to Tx IPdu        static configuration */
    Com_TxSigGrpConstPtrType  TxSigGrpConstPtr; /* Local pointer to Tx SignalGroup static configuration */
    Com_TxGrpSigConstPtrType  TxGrpSigConstPtr; /* Local pointer to Tx GroupSignal static configuration */
    Com_TxSigGrpRamPtrType    TxSigGrpRamPtr;   /* Local pointer to Tx SignalGroup status structure     */
    Com_SigMaxType            TxNewVal_GrpSig;
    uint16_least              GrpSig_Index_ui;
    uint8                     ConstByteValue_u8;
    uint8                     GrpSigType_u8;

    TxSigGrpConstPtr = COM_GET_TXSIGGRP_CONSTDATA(SignalGroupId);
    TxSigGrpRamPtr   = &COM_GET_TXSIGGRPFLAG(SignalGroupId);

    TxIPduConstPtr   = COM_GET_TX_IPDU_CONSTDATA(TxSigGrpConstPtr->IpduRef);

    TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(TxSigGrpConstPtr->FirstGrpSig_Index);

    for ( GrpSig_Index_ui = TxSigGrpConstPtr->No_Of_GrpSig; GrpSig_Index_ui != 0u; GrpSig_Index_ui-- )
    {
        ConstByteValue_u8 = TxGrpSigConstPtr->txGrpSigFields;

        GrpSigType_u8     = Com_GetValue(TXGRPSIG,_TYPE,ConstByteValue_u8);

        /* Check if the group signal contributes to evaluation of transfer property of signal group */
        if ( Com_GetValue(TXGRPSIG,_TP,ConstByteValue_u8) != COM_FALSE )
        {
            /* Unpack GroupSignal data from SignalGroup with ArrayAccess */
            TxNewVal_GrpSig = Com_UnpackSignal( Com_GetValue(TXGRPSIG,_ENDIANESS,ConstByteValue_u8),
                                                TxGrpSigConstPtr->Bit_Pos,
                                                TxGrpSigConstPtr->BitSize,
                                                TxIPduConstPtr->BuffPtr,
                                                ((GrpSigType_u8 & 0x01u) != 0x0u));

            /* Check if GroupSignal data has changed */
            if ( COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) !=
                                                                            (Com_OldValTrigOnChngType)TxNewVal_GrpSig )
            {
                /* Update the old value with the latest data */
                COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) =
                                                                             (Com_OldValTrigOnChngType)TxNewVal_GrpSig;
                /* Set the global TRIGGERED_ON_CHANGE flag if SignalGroup data has changed */
                Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,TxSigGrpRamPtr->txSigGrpRAMFields,COM_TRUE);
            }
            else
            {
                /* If GroupSignal data is unchanged, then it is not stored into Old_Value Buffer */
            }
        }
        else
        {
            /* GroupSignal configured with TRIGGERED_ON_CHANGE TP is not supported for Array(UINT8_N) and
               FLOAT64 Types */
        }
        TxGrpSigConstPtr++;
    }
}
# endif /* # ifdef COM_EffectiveSigGrpTOC */

#endif /* #if defined ( COM_TX_SIGNALGROUP_ARRAY ) */
