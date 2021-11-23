

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_TxConfirmation
 Syntax           : void Com_TxConfirmation(PduIdType TxPduId)
 Description      : Service called by the lower layer after the PDU has been transmitted on the network.
 Parameter        : TxPduId -> Id of the Tx Ipdu
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(void, COM_CODE) Com_TxConfirmation(VAR(PduIdType, AUTOMATIC) TxPduId)
{
    /* Call DET Function to check for the DET Errors COM_E_UNINIT and COM_E_PARAM */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if(Com_Prv_DETCheck_Param((TxPduId >= COM_GET_NUM_TX_IPDU), COMServiceId_TxConfirmation))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        TxPduId = COM_GET_TX_IPDU_ID(TxPduId);

        /* Check if Ipdu Group is started / Stopped */
        if(Com_CheckTxIPduStatus((PduIdType)TxPduId))
        {
            /* Call the internal function Com_InternalProcessTxConfirmation, to process this TxConfirmation */
            Com_InternalProcessTxConfirmation(TxPduId);
        }
        /*End of Checking IpduGroup Started/Stopped */
    }
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"



/***************************************************************************************************
 Function name    : Com_InternalProcessTxConfirmation
 Syntax           : void Com_InternalProcessTxConfirmation(PduIdType TxPduId)
 Description      : Internal function is called to process the TxConfirmation.
 Parameter        : TxPduId -> Id of the Tx Ipdu
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_InternalProcessTxConfirmation(VAR(PduIdType, AUTOMATIC) TxPduId)
{

    Com_TxIpduRamPtrType TxIpduRamPtr;
    Com_TxIpduConstPtrType TxIPduConstPtr;
    VAR(uint16, AUTOMATIC) ConstByteValue_u16;
#ifdef COM_TxIPduNotification
    VAR(boolean, AUTOMATIC) notifyFlag_b;

    notifyFlag_b = COM_FALSE;    /* Initializing to COM_FALSE */
#endif /* #ifdef COM_TxIPduNotification */

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(TxPduId);

    TxIpduRamPtr = &COM_GET_TXPDURAM_S(TxPduId);
    ConstByteValue_u16 = TxIPduConstPtr->txIPduFields;

    /* Minimum Delay Time has to be updated in the reception of confirmation as mentioned in the section
     * Minimum Delay Timer (MDT)*/
    /* Load the MDT timer, if
     * 1)MDT is configured for the IPdu.
     * 2)MDT is not set to zero, because of Tx timeout.(If Tx timeout happens then MDT timer is set to zero in
     *  cyclic task. In this case,delayed confirmation should not set the MDT again.  )
     */
    SchM_Enter_Com_TxIpduProtArea(TXCONFIRMATION);

    if ((TxIPduConstPtr->Min_Delay_Time_Fact != 0) && (TxIpduRamPtr ->Com_MinDelayTick != 0))
    {
        /* If IPdu is transmitted due to event triggered request, then add an offset of 1.
         * This is required because TxConfirmation can come immediately , which loads MDT timer.
         * Now this would be reduced by 1 immediately in next cyclic task, leading to early timeout of MDT timer.
         * In case of deferred processing, _EVENTTRIG flag will be cleared. Hence normal MDT will be loaded.
         */
        TxIpduRamPtr ->Com_MinDelayTick = (TxIPduConstPtr->Min_Delay_Time_Fact) +
                                          (Com_GetRamValue(TXIPDU,_EVENTTRIG,TxIpduRamPtr->Com_TxFlags) ? 1u : 0u);
    }

    if(TxIpduRamPtr->Com_n != 0)
    {
        --TxIpduRamPtr->Com_n;
    }
    if(TxIpduRamPtr->Com_n == 0)
    {
        /* TxIpduRamPtr->Com_TxFlags.TxConfirFlag = COM_SET */
        Com_SetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags,COM_TRUE);  /*This is checked in Com_MainFunctionTx()*/

#ifdef COM_TxIPduTimeOut
        /* if(TxIPduConstPtr->txIPduFields.NoneModeTimeOut != (uint8)COM_FALSE) */
        if(Com_GetValue(TXIPDU,_NONETOSTATUS,ConstByteValue_u16) != COM_FALSE)
        {
            /* The timer is reset at the TX confirmation for the IPdu with only None mode configured */
            TxIpduRamPtr->Com_TickTxTimeout = TxIPduConstPtr->Timeout_Fact;
            /* TxIpduRamPtr->Com_TxFlags.TickTxTimeout_Flag = COM_START */
            Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_START);
        }
        else
#endif
        {
            /*This Flag has to be set to START only in SendIpdu() i,e just before Sending the IPDU*/
            /* TxIpduRamPtr->Com_TxFlags.TickTxTimeout_Flag = COM_STOP */
            Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_STOP);
        }
        /* If the parameter ComTxIPduClearUpdateBit of an I-PDU is configured to Confirma-tion, the AUTOSAR COM module
         * shall clear all update-bits of all contained signals and signal groups after this I-PDU was sent out
         *  via PduR_ComTransmit, PduR_ComTransmit returned E_OK and the I-PDU was successfully confirmed*/
#if (defined(COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit))
        if(Com_GetValue(TXIPDU,_CLRUPDBIT,ConstByteValue_u16) == (uint16)COM_CLRUPDATEBIT_CONFIRMATION)
        {
            Com_Prv_ClearUpdateBits(TxIPduConstPtr);
        }
#endif /* #if defined ((COM_TxSigUpdateBit)||defined(COM_TxSigGrpUpdateBit)) */
#ifdef COM_TxIPduNotification
        notifyFlag_b = (Com_GetValue(TXIPDU,_SIGPROC,ConstByteValue_u16) == COM_IMMEDIATE);
#endif /* #ifdef COM_TxIPduNotification */
    }
    SchM_Exit_Com_TxIpduProtArea(TXCONFIRMATION);

#ifdef COM_TxIPduNotification
    if((notifyFlag_b == COM_TRUE) && (TxIPduConstPtr->Notification_Cbk != NULL_PTR))
    {
        /* Notification callback function for IPDU.
         * This function is generated through code generator.
         * Individual signal and signal group notifications functions are called from within this function.
         * If IPDU has configured TX notification callback funcion, this function
         * is also called from the generated function
         */
        TxIPduConstPtr->Notification_Cbk();
    }
#endif /* #ifdef COM_TxIPduNotification */

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

