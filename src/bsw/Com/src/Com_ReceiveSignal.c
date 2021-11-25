

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_UnsignedType (VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                                 P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr);
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_signedType   (VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                                 P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr);
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_ReceiveSignal
 Syntax           : uint8 Com_ReceiveSignal(Com_SignalIdType SignalId,Com_ApplicationDataRefType   SignalDataPtr)
 Description      : Service for receiving the signal, This API copies the data to the application buffer
 Parameter        : SignalId -> Id of the Signal
                  : SignalDataPtr -> Pointer to the address where signal data needs to be copied.
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_ReceiveSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                        P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
{
    /* Local pointer holds the address of Rx signal static configuration */
    Com_RxSigConstPtrType                       RxSigConstPtr;
    VAR(uint8, AUTOMATIC)                       Status;  /* Return status */
    /* Local variable holds the signal type */
    VAR(uint8, AUTOMATIC)                       SigType;

    Status = COM_SERVICE_NOT_AVAILABLE;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr((SignalId >= COM_GET_NUM_RX_SIGNALS), (SignalDataPtr == NULL_PTR),
                                                                                            COMServiceId_ReceiveSignal))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */

        SignalId = COM_GET_RXSIGNAL_ID(SignalId);

        RxSigConstPtr = COM_GET_RXSIG_CONSTDATA(SignalId);

        SigType = Com_GetValue(GEN,_TYPE,RxSigConstPtr->General);
        /*Check if Ipdu Group is started*/
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
         * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        if(Com_CheckRxIPduStatus((PduIdType)RxSigConstPtr->IpduRef))
        {
            Status = E_OK;
        }

        if((SigType % COM_TWO) == COM_ZERO)
        {
            /* Copy Data of Unsigned Signal Types */
            Com_Prv_CopyData_UnsignedType(SignalId, SignalDataPtr);
        }
        else
        {
            /* Copy Data of Signed Signal Types */
            Com_Prv_CopyData_signedType(SignalId, SignalDataPtr);
        }
    }
    return(Status);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/***************************************************************************************************
 Function name    : Com_Prv_CopyData_UnsignedType
 Syntax           : void Com_Prv_CopyData_UnsignedType(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                       P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
 Description      : Service to copy the data for UnSigned Signal type
 Parameter        : SignalId      -> Id of the signal
                  : SignalDataPtr -> Pointer to the the signal data
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_UnsignedType(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                                P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
{
    /* Local pointer holds the address of Rx signal static configuration */
    Com_RxSigConstPtrType                       RxSigConstPtr;
    VAR(Com_SigMaxType, AUTOMATIC)              RxNewVal;
    VAR(Com_SigBuffIndexType, AUTOMATIC)        SigBuff_Index;
    /* Local variable holds internal Rx-MainFunction Id */
    VAR(Com_MainFuncType, AUTOMATIC)            rxMainFuncId;
    /* Local variable holds the signal type */
    VAR(uint8, AUTOMATIC)                       Type;

    RxSigConstPtr       = COM_GET_RXSIG_CONSTDATA(SignalId);
    SigBuff_Index       = RxSigConstPtr->SigBuff_Index;
    /* Type = RxSigConstPtr->General.Type */
    Type                = Com_GetValue(GEN,_TYPE,RxSigConstPtr->General);
    rxMainFuncId        = (COM_GET_RX_IPDU_CONSTDATA(RxSigConstPtr->IpduRef))->MainFunctionRef;

    /* Copy the SigBuffer data into SignalDataPtr*/
    switch(Type)
    {
        /* MR12 RULE 11.5 VIOLATION: For SignalDataPtr is defined as void pointer in the
           AUTOSAR specification, so suppressing warning "Cast from a pointer to void to a pointer to object". */
        /* Locks are required here, as the signal buffer may be updated in Com_RxIndication(),
           which may be called in interrupt context. */
        case COM_UINT8:
            {
            RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId, SigBuff_Index);
             *(uint8*)SignalDataPtr =  (uint8)RxNewVal;
            }
            break;
        case COM_UINT16:
            {
                #if (CPU_TYPE == CPU_TYPE_8)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT16_BUFF(rxMainFuncId, SigBuff_Index);
                #if (CPU_TYPE == CPU_TYPE_8)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                *(uint16*)SignalDataPtr =  (uint16)RxNewVal;
            }
            break;
        case COM_UINT32:
            {
                #if (CPU_TYPE != CPU_TYPE_32)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT32_BUFF(rxMainFuncId, SigBuff_Index);
                #if (CPU_TYPE != CPU_TYPE_32)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                *(uint32*)SignalDataPtr =  (uint32)RxNewVal;
            }
            break;
#ifdef COM_RXSIG_INT64
        case COM_UINT64:
            {
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                RxNewVal = COM_GET_SIGTYPEUINT64_BUFF(rxMainFuncId, SigBuff_Index);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                *(uint64*)SignalDataPtr =  (uint64)RxNewVal;
            }
            break;
#endif /* #ifdef COM_RXSIG_INT64 */
        case COM_BOOLEAN:
            {
                RxNewVal = (uint32)COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId, SigBuff_Index);
                *(boolean*)SignalDataPtr =  (boolean)(RxNewVal != 0u);
            }
            break;
        case COM_UINT8_N:
            {
                /*Copy each byte into the SignalDataPtr from Signal buffer  */
                /* The locks inside the function Com_ByteCopy() are removed.
                 * Hence locks are used here
                 */
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                Com_ByteCopy((uint8*)SignalDataPtr,&COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId, SigBuff_Index),
                             (uint32)RxSigConstPtr->BitSize);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
            }
            break;

/* FC_VariationPoint_START */
#ifdef COM_FLOAT32SUPP
        case COM_FLOAT32:
            {
                #if (CPU_TYPE != CPU_TYPE_32)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT32_BUFF(rxMainFuncId, SigBuff_Index);
                #if (CPU_TYPE != CPU_TYPE_32)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                /* Convert the 32 bit stream  into float value */
                /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with rba_BswSrv_MemCopy() definition.
                   But the void pointer is always deferenced to a type, based on the pointers alignment.
                   Hence the alignment warning can safely be ignored. */
                (void)rba_BswSrv_MemCopy( SignalDataPtr, (const void *)&RxNewVal, 4 );

            }
            break;
#endif /* #ifdef COM_FLOAT32SUPP */

#ifdef COM_RXSIG_FLOAT64SUPP
        case COM_FLOAT64:
            /* FLOAT64 signal */
            {
                float64 RxNewVal_f64;
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                RxNewVal_f64 = COM_GET_SIGTYPEFLOAT64_BUFF(rxMainFuncId, SigBuff_Index);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);

                /* MR12 RULE 11.5 VIOLATION: For SignalDataPtr is defined as void pointer in the AUTOSAR specification,
                   so suppressing warning "Cast from a pointer to void to a pointer to object". */
                *(float64*)SignalDataPtr =  (float64)RxNewVal_f64;

            }
            break;
#endif
/* FC_VariationPoint_END */

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
 Syntax           : void Com_Prv_CopyData_SignedType(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                     P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
 Description      : Service to copy the data for Signed Signal type
 Parameter        : SignalId      -> Id of the signal
                  : SignalDataPtr -> Pointer to the the signal data
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_CopyData_signedType(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                              P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
{
    /* Local pointer holds the address of Rx signal static configuration */
    Com_RxSigConstPtrType                       RxSigConstPtr;
    VAR(Com_SigMaxType, AUTOMATIC)              RxNewVal;
    VAR(Com_SigBuffIndexType, AUTOMATIC)        SigBuff_Index;
    /* Local variable holds internal Rx-MainFunction Id */
    VAR(Com_MainFuncType, AUTOMATIC)            rxMainFuncId;
    /* Local variable holds the signal type */
    VAR(uint8, AUTOMATIC)                       Type;

    RxSigConstPtr       = COM_GET_RXSIG_CONSTDATA(SignalId);
    SigBuff_Index       = RxSigConstPtr->SigBuff_Index;
    /* Type = RxSigConstPtr->General.Type */
    Type                = Com_GetValue(GEN,_TYPE,RxSigConstPtr->General);
    rxMainFuncId        = (COM_GET_RX_IPDU_CONSTDATA(RxSigConstPtr->IpduRef))->MainFunctionRef;

    /* Copy the SigBuffer data into SignalDataPtr*/
    switch(Type)
    {
        /* MR12 RULE 11.5 VIOLATION: For SignalDataPtr is defined as void pointer in the
           AUTOSAR specification, so suppressing warning "Cast from a pointer to void to a pointer to object". */
        /* Locks are required here, as the signal buffer may be updated in Com_RxIndication(),
           which may be called in interrupt context. */
        case COM_SINT8:
            {
                RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT8_BUFF(rxMainFuncId, SigBuff_Index);
                *(sint8*)SignalDataPtr =  (sint8)RxNewVal;
            }
            break;
        case COM_SINT16:
            {
                #if (CPU_TYPE == CPU_TYPE_8)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT16_BUFF(rxMainFuncId, SigBuff_Index);
                #if (CPU_TYPE == CPU_TYPE_8)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                *(sint16*)SignalDataPtr =  (sint16)RxNewVal;
            }
            break;
        case COM_SINT32:
            {
                #if (CPU_TYPE != CPU_TYPE_32)
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                RxNewVal = (Com_SigMaxType)COM_GET_SIGTYPEUINT32_BUFF(rxMainFuncId, SigBuff_Index);
                #if (CPU_TYPE != CPU_TYPE_32)
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                #endif
                *(sint32*)SignalDataPtr =  (sint32)RxNewVal;
            }
            break;
#ifdef COM_RXSIG_INT64
        case COM_SINT64:
            {
                SchM_Enter_Com_RxSigBuff(RECEIVESIGNAL);
                RxNewVal = COM_GET_SIGTYPEUINT64_BUFF(rxMainFuncId, SigBuff_Index);
                SchM_Exit_Com_RxSigBuff(RECEIVESIGNAL);
                *(sint64*)SignalDataPtr =  (sint64)RxNewVal;
            }
            break;
#endif /* #ifdef COM_RXSIG_INT64 */

        default:
            /**
             * Default case is mandatory in switch syntax. Moreover only AUTOSAR Com defined signal types are allowed
             * to be configured, hence default case is intentionally left empty.
             */
            break;
    }
}
