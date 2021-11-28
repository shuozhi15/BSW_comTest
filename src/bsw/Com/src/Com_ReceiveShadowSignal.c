

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/

#ifdef COM_RX_SIGNALGROUP

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_UnsignedType( VAR(Com_SignalIdType, AUTOMATIC)      IntSignalId,
                                                                 P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                                 VAR(Com_MainFuncType,AUTOMATIC)       RxMainFuncId );

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_SignedType(   VAR(Com_SignalIdType, AUTOMATIC)      IntSignalId,
                                                                 P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                                 VAR(Com_MainFuncType,AUTOMATIC)       RxMainFuncId );
#endif /* #ifdef COM_RX_SIGNALGROUP */

/***************************************************************************************************
 Function name    : Com_ReceiveShadowSignal
 Syntax           : void Com_ReceiveShadowSignal(Com_SignalIdType  SignalId, void*  SignalDataPtr)
 Description      : Service for receiving the shadow Signal
 Parameter        : SignalId      -> Id of the signal
                  : SignalDataPtr -> Pointer to the the signal data
 Return value     : None
 ***************************************************************************************************/
#ifdef COM_RX_SIGNALGROUP
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(void, COM_CODE) Com_ReceiveShadowSignal( VAR(Com_SignalIdType, AUTOMATIC)      SignalId,
                                              P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
{
    /* Local Variables */
    Com_RxGrpSigConstPtrType                    RxGrpSigConstPtr;
    VAR(uint8, AUTOMATIC)                       SigType;
    VAR(Com_MainFuncType,AUTOMATIC)             rxMainFuncId;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Errors COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
    if(Com_Prv_DETCheck_Param_Ptr((SignalId >= COM_GET_NUM_RX_GRPSIGNALS), (SignalDataPtr == NULL_PTR),
                                   COMServiceId_ReceiveShadowSignal))
#endif
    {
        SignalId            = COM_GET_RXGRPSIGNAL_ID(SignalId);

        RxGrpSigConstPtr    = COM_GET_RXGRPSIG_CONSTDATA(SignalId);

        SigType             = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);

        /* Fetch the Rx-MainFunction internal Id */
        rxMainFuncId = (COM_GET_RX_IPDU_CONSTDATA((COM_GET_RXSIGGRP_CONSTDATA(RxGrpSigConstPtr->SigGrpRef))->IpduRef))->MainFunctionRef;

        /* The last known value has to be given back even if IPDU Group is stopped
         * So No need of checking Ipdu Group Status */
        if((SigType % COM_TWO) == COM_ZERO)
        {
            /* Copy Data of Unsigned Signal Types */
            Com_Prv_CopyData_UnsignedType(SignalId, SignalDataPtr, rxMainFuncId);
        }
        else
        {
            /* Copy Data of Signed Signal Types */
            Com_Prv_CopyData_SignedType(SignalId, SignalDataPtr, rxMainFuncId);
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/***************************************************************************************************
 Function name    : Com_Prv_CopyData_UnsignedType
 Syntax           : void Com_Prv_CopyData_UnsignedType( VAR(Com_SignalIdType, AUTOMATIC)      IntSignalId,
                                                        P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                        VAR(Com_MainFuncType,AUTOMATIC)       RxMainFuncId )
 Description      : Service to copy the data for UnSigned Signal type
 Parameter        : SignalId      -> Id of the signal
                    SignalDataPtr -> Pointer to the the signal data
                    RxMainFuncId  -> MainFunction Id to access GroupSignal buffer
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_UnsignedType( VAR(Com_SignalIdType, AUTOMATIC)      IntSignalId,
                                                                 P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                                 VAR(Com_MainFuncType,AUTOMATIC)       RxMainFuncId )
{
    /* Local Variables */
    Com_RxGrpSigConstPtrType                    RxGrpSigConstPtr;
    VAR(Com_RxGrpSigBuffIndexType, AUTOMATIC)   GrpSigBuff_Index;
    VAR(uint8, AUTOMATIC)                       SigType;
    VAR(Com_SigMaxType, AUTOMATIC)              RxNewVal_GrpSig;

    RxGrpSigConstPtr = COM_GET_RXGRPSIG_CONSTDATA(IntSignalId);

    SigType          = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);
    GrpSigBuff_Index = RxGrpSigConstPtr->RxGrpSigBuff_Index;

    /* Copy GroupSignal data from shadow buffer into SignalDataPtr */
    switch( SigType )
    {

        case COM_UINT8:
        {
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT8_BUFF(RxMainFuncId,GrpSigBuff_Index);
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(uint8*)SignalDataPtr  =  (uint8)RxNewVal_GrpSig;
        }
        break;
        case COM_UINT16:
        {
            #if (CPU_TYPE == CPU_TYPE_8)
            SchM_Enter_Com_RxGrpSigBuff();
            #endif
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT16_BUFF(RxMainFuncId,GrpSigBuff_Index);
            #if (CPU_TYPE == CPU_TYPE_8)
            SchM_Exit_Com_RxGrpSigBuff();
            #endif
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(uint16*)SignalDataPtr =  (uint16)RxNewVal_GrpSig;
        }
        break;
        case COM_UINT32:
        {
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigBuff();
            #endif
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT32_BUFF(RxMainFuncId,GrpSigBuff_Index);
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigBuff();
            #endif
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(uint32*)SignalDataPtr =  (uint32)RxNewVal_GrpSig;
        }
        break;

#ifdef COM_RXGRPSIG_INT64
        case COM_UINT64:
        {
            SchM_Enter_Com_RxGrpSigBuff();
            RxNewVal_GrpSig = COM_GET_RXGRPSIGTYPEUINT64_BUFF(RxMainFuncId,GrpSigBuff_Index);
            SchM_Exit_Com_RxGrpSigBuff();
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored.
             */
            *(uint64*)SignalDataPtr =  (uint64)RxNewVal_GrpSig;
        }
        break;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

        case COM_BOOLEAN:
        {
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT8_BUFF(RxMainFuncId,GrpSigBuff_Index);
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored.
             */
            *(boolean*)SignalDataPtr =  (boolean)(RxNewVal_GrpSig != 0u);
        }
        break;

/* FC_VariationPoint_START */
#ifdef COM_GRPSIGFLOAT32SUPP
        case COM_FLOAT32:
        {
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigBuff();
            #endif
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT32_BUFF(RxMainFuncId,GrpSigBuff_Index);
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigBuff();
            #endif
            /* Convert the 32 bit stream into float value */
            /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
             * But the void pointer is always deferenced to a type, based on the pointers alignment.
             * Hence the alignment warning can safely be ignored.
             */
           (void)rba_BswSrv_MemCopy(SignalDataPtr,(const void *)&RxNewVal_GrpSig,4);
        }
        break;
#endif /* COM_GRPSIGFLOAT32SUPP */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
        case COM_FLOAT64:
        {
            VAR(float64, AUTOMATIC) RxNewVal_GrpSig_f64;
            SchM_Enter_Com_RxGrpSigBuff();
            RxNewVal_GrpSig_f64 =  (float64)COM_GET_RXGRPSIGTYPEFLOAT64_BUFF(RxMainFuncId,GrpSigBuff_Index);
            SchM_Exit_Com_RxGrpSigBuff();
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored.
             */
            *(float64*)SignalDataPtr =  (float64)RxNewVal_GrpSig_f64;
        }
        break;
#endif
/* FC_VariationPoint_END */
       case COM_UINT8_N:
        {
            /* UINT8[n] Signal */
            SchM_Enter_Com_RxGrpSigBuff();

            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification,
             * to support different groupsignal types. On call to this api, application has to pass a valid datatype
             * pointer address. Hence this warning can safely be ignored. */
            Com_ByteCopy( (uint8*)SignalDataPtr, &(COM_GET_RXGRPSIGTYPEUINT8_BUFF(RxMainFuncId,GrpSigBuff_Index)),
                          (uint32)RxGrpSigConstPtr->BitSize);

            SchM_Exit_Com_RxGrpSigBuff();
        }
        break;
        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
        break;
    }
}

/***************************************************************************************************
 Function name    : Com_Prv_CopyData_SignedType
 Syntax           : void Com_Prv_CopyData_SignedType( VAR(Com_SignalIdType, AUTOMATIC)      IntSignalId,
                                                      P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                      VAR(Com_MainFuncType,AUTOMATIC)       RxMainFuncId )
 Description      : Service to copy the data for Signed Signal type
 Parameter        : SignalId      -> Id of the signal
                    SignalDataPtr -> Pointer to the the signal data
                    RxMainFuncId  -> MainFunction Id to access GroupSignal buffer
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE)  Com_Prv_CopyData_SignedType( VAR(Com_SignalIdType, AUTOMATIC)      IntSignalId,
                                                                P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
                                                                VAR(Com_MainFuncType,AUTOMATIC)       RxMainFuncId )
{
    /* Local Variables */
    Com_RxGrpSigConstPtrType                    RxGrpSigConstPtr;
    VAR(Com_RxGrpSigBuffIndexType, AUTOMATIC)   GrpSigBuff_Index;
    VAR(uint8, AUTOMATIC)                       SigType;
    VAR(Com_SigMaxType, AUTOMATIC)              RxNewVal_GrpSig;

    RxGrpSigConstPtr = COM_GET_RXGRPSIG_CONSTDATA(IntSignalId);

    SigType          = Com_GetValue(RXGRPSIG,_TYPE,RxGrpSigConstPtr->rxGrpSigFields);
    GrpSigBuff_Index = RxGrpSigConstPtr->RxGrpSigBuff_Index;

    /* Copy GroupSignal data from shadow buffer into SignalDataPtr */
    switch( SigType )
    {
        case COM_SINT8:
        {
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT8_BUFF(RxMainFuncId,GrpSigBuff_Index);
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored. */
            *(sint8*)SignalDataPtr  =  (sint8)RxNewVal_GrpSig;
        }
        break;
        case COM_SINT16:
        {
            #if (CPU_TYPE == CPU_TYPE_8)
            SchM_Enter_Com_RxGrpSigBuff();
            #endif
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT16_BUFF(RxMainFuncId,GrpSigBuff_Index);
            #if (CPU_TYPE == CPU_TYPE_8)
            SchM_Exit_Com_RxGrpSigBuff();
            #endif
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(sint16*)SignalDataPtr =  (sint16)RxNewVal_GrpSig;
        }
        break;
        case COM_SINT32:
        {
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Enter_Com_RxGrpSigBuff();
            #endif
            RxNewVal_GrpSig = (Com_SigMaxType)COM_GET_RXGRPSIGTYPEUINT32_BUFF(RxMainFuncId,GrpSigBuff_Index);
            #if (CPU_TYPE != CPU_TYPE_32)
            SchM_Exit_Com_RxGrpSigBuff();
            #endif
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(sint32*)SignalDataPtr =  (sint32)RxNewVal_GrpSig;
        }
        break;

#ifdef COM_RXGRPSIG_INT64
        case COM_SINT64:
        {
            SchM_Enter_Com_RxGrpSigBuff();
            RxNewVal_GrpSig = COM_GET_RXGRPSIGTYPEUINT64_BUFF(RxMainFuncId,GrpSigBuff_Index);
            SchM_Exit_Com_RxGrpSigBuff();
            /* MR12 RULE 11.5 VIOLATION: SignalDataPtr is defined as void pointer in the AUTOSAR specification, to support
             * different groupsignal types. On call to this api, application has to pass a valid datatype pointer address.
             * Hence this warning can safely be ignored.
             */
            *(sint64*)SignalDataPtr =  (sint64)RxNewVal_GrpSig;
        }
        break;
#endif /* #ifdef COM_RXGRPSIG_INT64 */

        default:
       /**
         * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
         * to be configured, hence default case is intentionally left empty.
         */
       break;
    }
}
#endif /* #ifdef COM_RX_SIGNALGROUP */

