

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "Com_Prv.h"

/**********************************************************************************************************************
 * Global functions (declared in header files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Function name    : Com_TxIPduStop
 Syntax           : void Com_TxIPduStop(Com_IpduIdType  IpduId)
 Description      : Service to stop the ipdu
 Parameter        : IpduId - Id of the ipdu
 Return value     : None
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_TxIPduStop( VAR(Com_IpduIdType, AUTOMATIC) IpduId )
{
    Com_TxIpduRamPtrType        TxIpduRamPtr;
#ifdef COM_ERRORNOTIFICATION
    Com_TxIpduConstPtrType      TxIPduConstPtr;

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);
#endif

    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(IpduId);

#ifdef COM_ERRORNOTIFICATION
    /* [SWS_Com_00479] If an I-PDU is stopped as result of a call to Com_IpduGroupControl, the AUTOSAR COM module
     * shall immediately invoke the configured ComErrorNotification, for outstanding not confirmed
     * transmitted signals/ signal groups of the stopped I-PDU.
     * Outstanding not confirmed I-PDU's can be one of the below possibilities:
     * => Tx-Timeout timer is started, i.e., already transmitted to the lower layer, waiting for the confirmation.
     * => N-times repetitions is not completed yet, its confirmation is pending */
    if ((Com_GetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags) == COM_START) || (TxIpduRamPtr->Com_n != 0))
    {
        if ((Com_GetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags) == COM_FALSE) &&
            (TxIPduConstPtr->NotificationError_Cbk != NULL_PTR))
        {
            TxIPduConstPtr->NotificationError_Cbk();
        }
    }
#endif /* #ifdef COM_ERRORNOTIFICATION */

    /* [SWS_Com_00115] If an I-PDU is stopped as result of a call to Com_IpduGroupControl,
     * the AUTOSAR COM module shall cancel the deadline monitoring for all pending confirmations. */
    Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_STOP);

#ifdef COM_RETRY_FAILED_TX_REQUESTS
    /* [SWS_Com_00777] - If an I-PDU is stopped as result of a call to Com_IpduGroupControl,
     * cancel any potential retries with respect to ComRetryFailedTransmitRequests. */
    Com_SetRamValue(TXIPDU,_RETRYFAILEDTXREQ,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
#endif

    /* [SWS_Com_00800] The AUTOSAR COM module shall ignore any transmit confirmations for a stopped I-PDU. */
    /* Reset all other Tx Global variables including Min Delays */
    Com_SetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags,COM_FALSE);

#ifdef COM_TxIPduTimeOut
    TxIpduRamPtr->Com_TickTxTimeout = 0;
#endif

    /* [SWS_Com_00777] - If an I-PDU is stopped as result of a call to Com_IpduGroupControl,
     * cancel any outstanding transmission requests for this I-PDU. */
    TxIpduRamPtr->Com_n = 0;
    Com_SetRamValue(TXIPDU,_MDT,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
    TxIpduRamPtr->Com_MinDelayTick = 0;

    /* [SWS_Com_00714] If a large I-PDU is stopped while its transmission is already in progress,
     * the AUTOSAR COM module shall stop the transmission process immediately.
     * Reset the large data Tx Pdu flag, if the Ipdu group is stopped. */
    Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

