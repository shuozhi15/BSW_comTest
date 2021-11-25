

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**********************************************************************************************************************
 * Global functions (declared in header files )
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Function name    : Com_InvalidateShadowSignal
 Syntax           : void Com_InvalidateShadowSignal(Com_SignalIdType SignalId)
 Description      : Service for Invalidating the shadow signal (i.e Group signal)
 Parameter        : SignalId
 Return value     : None
**********************************************************************************************************************/
#ifdef COM_TX_SIGNALGROUP
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_InvalidateShadowSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId)
{
#ifdef COM_TxGrpSigInvalid

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param((SignalId >= COM_GET_NUM_TX_GRPSIGNALS), COMServiceId_InvalidateShadowSignal))
#endif
    {
        /* Convert external HandleId to internal HandleId for Tx Group Signal
         * IMP : before accessing the tables, it is required to converth the IDs
         * For pre-compile, conversion is not required. Access Macro will return the same ID, which is passed to it */
        SignalId = COM_GET_TXGRPSIGNAL_ID(SignalId);

        Com_InternalInvalidateShadowSignal(SignalId);
    }

#else
    (void)SignalId;
#endif /* #ifdef COM_TxGrpSigInvalid */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#ifdef COM_TxGrpSigInvalid
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/***************************************************************************************************
 Function name    : Com_InternalInvalidateShadowSignal
 Syntax           : void Com_InternalInvalidateShadowSignal(Com_SignalIdType   SignalId)
 Description      : Service for Invalidating the shadow signal (i.e Group signal)
                    This is an internal function to invalidate a group signal
                    This function doesnt do DET checks, these checks are done in the API itself.
 Parameter        : SignalId -> Internal Id of the group signal. (Relevant for post-build case)
 Return value     : None
***************************************************************************************************/
FUNC(void, COM_CODE) Com_InternalInvalidateShadowSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId)
{
    Com_TxGrpSigConstPtrType            TxGrpSigConstPtr;
    /* TxGrpSig_NewVal datatype is uint32, as currently maximum 32bits are supported for Invalid value */
    VAR(uint32, AUTOMATIC)              TxGrpSig_NewVal;
    VAR(uint8, AUTOMATIC)               ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)               Type;
    VAR(Com_MainFuncType,AUTOMATIC)     txMainFuncId;

    TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(SignalId);
    Type = Com_GetValue(TXGRPSIG,_TYPE,TxGrpSigConstPtr->txGrpSigFields);

    ConstByteValue_u8 = TxGrpSigConstPtr->txGrpSigFields;

    /* As per the SWS Page 42/43 (V2.1), we are supposed to call the Com_SendSignal( ) i.e Com_UpdateShadowSignal */
    /* As per COM286:By this call no send request shall be initiated */
    /*As per COM288, there is statement as "For processing, a Data invalid value must have been configured" */
    /* Pack the data invalid value into the shadow buffer for normal signals*/
    /*no call to UpdateShadow Signal is required for the same*/
    /* if(TxGrpSigConstPtr->txGrpSigFields.DataInvalidType)) */
    if(Com_GetValue(TXGRPSIG,_INVACTION,ConstByteValue_u8))
    {
        TxGrpSig_NewVal  = TxGrpSigConstPtr->DataInvalid_Val;

        /* Fetch the Tx-MainFunction internal Id */
        txMainFuncId = (COM_GET_TX_IPDU_CONSTDATA((COM_GET_TXSIGGRP_CONSTDATA(TxGrpSigConstPtr->SigGrpRef))->IpduRef))->MainFunctionRef;

        if(Type != (uint8)COM_UINT8_N)
        {
            Com_CopyTxGroupSignalData(SignalId, (Com_SigMaxType)TxGrpSig_NewVal, txMainFuncId);
        }
        else
        {
            Com_ByteCopyInit(&COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index),
                             TxGrpSig_NewVal, (uint32)TxGrpSigConstPtr->BitSize);
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_TxGrpSigInvalid */

#endif /* #ifdef COM_TX_SIGNALGROUP */

