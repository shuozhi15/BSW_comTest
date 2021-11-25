

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_TriggerTransmit
 Syntax           : Std_ReturnType Com_TriggerTransmit(PduIdType TxPduId,PduInfoType* PduInfoPtr)
 Description      : Service called by the lower layer when an AUTOSAR COM I-PDU shall be transmitted.
                    Within this function, AUTOSAR COM shall copy the contents of its I-PDU transmit
                    buffer to the L-PDU buffer given by PduInfoPtr.
 Parameter        : TxPduId    -> id of the Tx ipdu
                  : PduInfoPtr -> Pointer to the address where the COM module shall copy the ipdu data
                                  and length information.
 Return value     : E_OK/E_NOT_OK
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS VIOLATION IN Com_TriggerTransmit:
   Com_TriggerTransmit function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
FUNC(Std_ReturnType, COM_CODE) Com_TriggerTransmit(VAR(PduIdType, AUTOMATIC) TxPduId,
                                                   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{

    Com_TxIpduConstPtrType          TxIPduConstPtr;
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
    Com_TxIpduRamPtrType            TxIpduRamPtr;
#endif
    VAR(PduLengthType, AUTOMATIC)   Size;
    VAR(Std_ReturnType, AUTOMATIC)  Status;
#if ((defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) || defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TxIPduCallOuts))
    VAR(uint16, AUTOMATIC)          ConstByteValue_u16;
#endif /* #if ((defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) || defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TxIPduCallOuts)) */

    /* Return value with E_NOT_OK indicates, IPDU is not started or DET error has occured */
    Status = E_NOT_OK;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Errors COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
    if(Com_Prv_DETCheck_Param_Ptr((TxPduId >= COM_GET_NUM_TX_IPDU), (PduInfoPtr == NULL_PTR),
                                                                    COMServiceId_TriggerTransmit))
#endif
    {

        /* If PB variant is selected, then PduId which is passed to this function will be changed to internal Id
         * which is generated through configuration. If PC variant is selected, then no mapping table will be used.
         */

        TxPduId        = COM_GET_TX_IPDU_ID(TxPduId);

        TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(TxPduId);

    #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
        TxIpduRamPtr   = &COM_GET_TXPDURAM_S(TxPduId);
    #endif
        Status         = E_OK;

    #if ((defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)) || defined(COM_TX_DYNAMIC_SIGNAL_SUPPORT) || defined(COM_TxIPduCallOuts))
        ConstByteValue_u16 = TxIPduConstPtr->txIPduFields;
    #endif

        Size = TxIPduConstPtr->Size;

        /*Check if Ipdu Group is started */
        if(!(Com_CheckTxIPduStatus((PduIdType)TxPduId)))
        {
            /* This part will be executed when IPDU Group is Stopped (old value will be returned) */
            Status = E_NOT_OK;
        }

        /* No MDT monitoring is required*/
        /* Dont check for Ipdu Group Stopped/Started i.e Normal reaction on Com_TriggerTransmit( ) */
    #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
        /* The total Ipdu size will be sum of Ipdu static length and dynamic length for a dynamic IPdu.
         * The Dynamic length of the Ipdu is updated in the call to Com_SendDynSignal.*/
        /* Check if Dynamic Signal is supported */
        if(COM_TRUE == Com_GetRamValue(TXIPDU,_ISDYNIPDU,ConstByteValue_u16))
        {
            Size = (PduLengthType)(TxIPduConstPtr->Size + TxIpduRamPtr->Com_DynLength);
        }
    #endif/* #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT */

        SchM_Enter_Com_TxIpduProtArea(TRIGGERTRANSMIT);
        Com_ByteCopy(PduInfoPtr->SduDataPtr,TxIPduConstPtr->BuffPtr,(uint32)Size);
        PduInfoPtr ->SduLength = Size;
        /* Trigger Transmit is processed even if the IPDU Group is STOPPED */
        /* If the parameter ComTxIPduClearUpdateBit of an I-PDU is configured to TriggerTransmit,
         * the AUTOSAR COM module shall clear all update-bits of all contained signals and signal groups
         * after the contents of this I-PDU was requested by a call to Com_TriggerTransmit.*/
    #if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
        if(Com_GetValue(TXIPDU,_CLRUPDBIT,ConstByteValue_u16) == (uint16)COM_CLRUPDATEBIT_TRIGGERTRANSMIT)
        {
            Com_Prv_ClearUpdateBits(TxIPduConstPtr);
        }
    #endif /* #if defined ((COM_TxSigUpdateBit)||defined(COM_TxSigGrpUpdateBit)) */
        SchM_Exit_Com_TxIpduProtArea(TRIGGERTRANSMIT);

        /* The callout is processed over the data given to the IF buffer
         * Note: As the data pointer given to application is a Pointer to variable, There are chances that application
         * layer can update the data. Any data update from application which happens over this target buffer are passed
         * to the lower layer. How ever this updated data is not stored in the local buffers of Com.
         * Impact: Application cannot assume that the data updates done in one Com Callout function will be preserved
         * across different calls of Com callout. */
    #ifdef COM_TxIPduCallOuts
        /*Call Ipdu_Callout*/
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
         * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        if ((Status != E_NOT_OK) && (Com_GetValue(TXIPDU,_ISCALLOUTFRMTRIGTRANS,ConstByteValue_u16) != COM_FALSE))
        {
            /* The Return Value from IPDU Callout shall be ignored */
            (void)TxIPduConstPtr->CallOut(TxPduId, PduInfoPtr);
        }
    #endif
    }
    return(Status);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

