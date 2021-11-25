

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_TriggerIPDUSend
 Syntax           : void Com_TriggerIPDUSend(PduIdType PduId)
 Description      : Service used within the Ipdu-Callout.
 The I-PDU with the given ID is triggered for transmission
 Parameter        : PduId-> Id of the Tx ipdu
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_TriggerIPDUSend(VAR(PduIdType, AUTOMATIC) PduId)
{
#ifdef COM_TxIPduTimeOut
    Com_TxIpduConstPtrType TxIPduConstPtr; /* Loca pointer to Tx ipdu static configuration */
#endif
    Com_TxIpduRamPtrType TxIpduRamPtr; /* Local pointer to the Tx ipdu status structure */

    /* Call DET Function to check for the DET Errors COM_E_UNINIT and COM_E_PARAM */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if ( Com_Prv_DETCheck_Param( (PduId >= COM_GET_NUM_TX_IPDU), COMServiceId_TriggerIpduSend) )
#endif

    /* If PB variant is selected, then PduId which is passed to this function will be changed
     * to internal Id which is generated through configuration
     * If PC variant is selected, then no mapping table will be used. */
    {
        PduId = COM_GET_TX_IPDU_ID(PduId);

        TxIpduRamPtr = &COM_GET_TXPDURAM_S(PduId);

#ifdef COM_TxIPduTimeOut

        TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(PduId);

#endif

    if(Com_CheckTxIPduStatus((PduIdType)PduId))
        {

            /* Check whether the large data pdu flag has been reset */
#ifdef COM_TP_IPDUTYPE
            if (Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) != COM_TRUE)
#endif /*#ifdef COM_TP_IPDUTYPE*/
            {
                /* All unused flags are reset here */
                Com_SendIpduInfo sendIpduFlag_u16 = COM_CLEAR_SENDIPDU_FLAGS;
                sendIpduFlag_u16.isEventTrig = COM_SET;
                /* Call Com_Prv_SendIpdu,for the Ipdu, indicating mode change */
                sendIpduFlag_u16.isModeChangd = COM_RESET;
                /* As the signal transfer property information is not known here.
                 * It is assumed, for transmission from this APi,the signal transfer property is Pending */
                /* The timeout handling should happen as usual*/
#ifdef COM_TxIPduTimeOut

                sendIpduFlag_u16.isTimeoutReq = (0u != TxIPduConstPtr->Timeout_Fact)? COM_SET : COM_RESET;

#endif
                /* [SWS_Com_00388] Ignore repetitions */
                sendIpduFlag_u16.ignoreRepetitions = COM_SET;

                Com_Prv_SendIpdu((PduIdType) PduId, sendIpduFlag_u16);
            }

        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

