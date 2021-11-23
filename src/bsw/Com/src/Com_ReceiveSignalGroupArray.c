

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

/* Com_ReceiveSignalGroupArray() API shall only be available when the configuration switch
 *  ComEnableSignalGroupArrayApi is enabled */
#if defined ( COM_RX_SIGNALGROUP_ARRAY )
/*
**********************************************************************************************************************
* Function name    : Com_ReceiveSignalGroupArray
* Syntax           : uint8
*                    Com_ReceiveSignalGroupArray( Com_SignalGroupIdType SignalGroupId, uint8* SignalGroupArrayPtr )
* Description      : The service Com_ReceiveSignalGroupArray copies the received signal group array representation
*                    from the PDU to the location designated by SignalGroupArrayPtr.
* Parameters   IN  : SignalGroupId - Id of signal group to be received.
               OUT : SignalGroupArrayPtr - reference to the location where the
                                           received signal group array shall be stored
* Sync/Async       : Synchronous
* Reentrancy       : Non Reentrant for the same signal group. Reentrant for different signal groups.
* Return value     : E_OK                      : service has been accepted
*                    COM_SERVICE_NOT_AVAILABLE : corresponding I-PDU group was stopped
*                                                (or service failed due to development error)
**********************************************************************************************************************
*/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_ReceiveSignalGroupArray( Com_SignalGroupIdType SignalGroupId, uint8 * SignalGroupArrayPtr )
{
    Com_RxSigGrpConstPtrType            RxSigGrpConstPtr; /* Local pointer to Rx SignalGroup static configuration */
    VAR(uint8,AUTOMATIC)                returnValue_u8;
    VAR(Com_MainFuncType,AUTOMATIC)     rxMainFuncId;
    VAR(Com_IpduIdType,AUTOMATIC)       IpduId_ui;

    /* Return COM_SERVICE_NOT_AVAILABLE code for the below conditions:
     * - DET Error is detected
     * - Corresponding IPduGroup is stopped
     * - SignalGroup is not configured with ArrayAccess */
    returnValue_u8 = COM_SERVICE_NOT_AVAILABLE;

    /* Call DET Function to check for the DET Errors COM_E_UNINIT, COM_E_PARAM and COM_E_PARAM_POINTER */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr((SignalGroupId >= COM_GET_NUM_RX_SIGNALGRP),(SignalGroupArrayPtr == NULL_PTR),
                                        COMServiceId_ReceiveSignalGroupArray))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */
        SignalGroupId    = COM_GET_RXSIGNALGRP_ID( SignalGroupId );

        RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA( SignalGroupId );

        IpduId_ui        = RxSigGrpConstPtr->IpduRef;

        /* Fetch the Rx-MainFunction internal Id */
        rxMainFuncId     = (COM_GET_RX_IPDU_CONSTDATA(IpduId_ui))->MainFunctionRef;

        /* Check if ArrayAccess is enabled for this Rx-Signal Group */
        if ( Com_GetValue(RXSIGGRP,_ARRAYACCESS,RxSigGrpConstPtr->rxSignalGrpFields) )
        {
            Com_RxSigGrpArrayConstPtrType        RxSigGrpArrayConstPtr; /* Ptr to Rx SignalGroupArray static config */
            P2VAR(uint8,AUTOMATIC,COM_APPL_DATA) RxSigGrpArrayBufRamPtr;

            RxSigGrpArrayConstPtr  = COM_GET_RXSIGGRP_ARRAY_CONSTDATA( RxSigGrpConstPtr->SigGrpArray_Index );

            RxSigGrpArrayBufRamPtr =
                                &COM_GET_RXSIGGRP_ARRAY_BUFF( rxMainFuncId,RxSigGrpArrayConstPtr->RxSigGrpBuf_Index );

            /**
             * A call to Com_ReceiveSignalGroupArray() on a Rx-SignalGroup that belongs to a stopped I-PDU
             * copy the last known data (or the init value) of the I-PDU to the SignalGroupArrayPtr.
             */
            if ( Com_CheckRxIPduStatus((PduIdType)IpduId_ui ) )
            {
                returnValue_u8 = E_OK;
            }

            SchM_Enter_Com_RxSigGrpArrayBuff();

            /**
             * Copy RxSignalGroup Data(uint8[n]) from IPdu Buffer into *SignalGroupArrayPtr
             * Design: Here in Rx-SignalGroup, LastByte is generated as (LastByteNo + 1),
             *         so the Length is derived as just ( LastByte - FirstByte ) instead of ( LastByte - FirstByte + 1 )
             */
            Com_ByteCopy(   ( P2VAR(uint8,AUTOMATIC,COM_APPL_DATA) )( SignalGroupArrayPtr    ),
                            ( P2CONST(uint8,AUTOMATIC,COM_APPL_DATA) )( RxSigGrpArrayBufRamPtr ),
                            ( uint32 )( RxSigGrpConstPtr->lastByte_u8 - RxSigGrpArrayConstPtr->FirstByteNo ) );

            SchM_Exit_Com_RxSigGrpArrayBuff();
        }
    }
    return returnValue_u8;
}
# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"

#endif /* #if defined ( COM_RX_SIGNALGROUP_ARRAY ) */
