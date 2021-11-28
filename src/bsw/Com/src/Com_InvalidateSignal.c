

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_InvalidateSignal
 Syntax           : uint8 Com_InvalidateSignal(Com_SignalIdType   SignalId)
 Description      : Service for Invalidating the signal
 Parameter        : SignalId -> Id of the signal
 Return value     : E_OK/COM_SERVICE_NOT_AVAILABLE
***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE) Com_InvalidateSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId)
{
#ifdef COM_TxInvalid
    Com_TxSigConstPtrType           TxSigConstPtr;  /* Pointer to Tx Signal static configuration       */
    Com_TxIpduConstPtrType          TxIPduConstPtr; /* Pointer to Tx Ipdu static configuration         */
    Com_TxIpduRamPtrType            TxIpduRamPtr;   /* Local pointer to Tx IPdu   static configuration */
    VAR(uint16_least, AUTOMATIC)    IpduId_ui;
    VAR(PduLengthType, AUTOMATIC)   ByteNo_u8;
    VAR(uint8, AUTOMATIC)           Status;

    Status = COM_SERVICE_NOT_AVAILABLE;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if(Com_Prv_DETCheck_Param((SignalId >= COM_GET_NUM_TX_SIGNALS), COMServiceId_InvalidateSignal))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
         * to internal Id which is generated through configuration
         * If PC variant is selected, then no mapping table will be used. */

        SignalId       = COM_GET_TXSIGNAL_ID(SignalId);

        TxSigConstPtr  = COM_GET_TXSIG_CONSTDATA(SignalId);

        IpduId_ui      = TxSigConstPtr->IpduRef;

        TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId_ui);
        TxIpduRamPtr   = &COM_GET_TXPDURAM_S( IpduId_ui );

        /* The AUTOSAR COM module shall only process Com_InvalidateSignal only if a ComSignalDataInvalidValue is
         * configured for the signal with the given SignalId.
         * Copy the Invalid value to all the Bytes of this signal in Ipdu buffer */
        if (Com_GetValue(TXSIG,_INVACTION,TxSigConstPtr->txSignalFields))
        {
#ifdef COM_TP_IPDUTYPE
            /* Proceed only if Large Data tranmission is not in Progress */
            if(Com_GetRamValue(TXIPDU,_LARGEDATAINPROG,TxIpduRamPtr->Com_TxFlags) == COM_TRUE)
            {
                /* The TxIPdu transmission of large Data IPdu is in progress, hence no signal update is allowed until
                 * the transmission is completed. */
                Status = COM_BUSY;
            }
            else
#endif /*#ifdef COM_TP_IPDUTYPE*/
            {
                uint8 type_u8;

                type_u8 = Com_GetValue(GEN,_TYPE,TxSigConstPtr->General);

                /* After invaliding the signal data, this function shall perform a call of Com_SendSignal internally.
                 * But as per design, Com_InternalSendSignal() is called with the signal Id and pass Address of Data.
                 * Checking if the IpduGroup is started is checked in Com_InternalSendSignal() so no need of checking
                 * it here. */
                if(type_u8 != (uint8)COM_UINT8_N)
                {
                    VAR(Com_SigMaxType,AUTOMATIC) txSigInvValue;

                    txSigInvValue = (Com_SigMaxType)TxSigConstPtr->DataInvalid_Val;

                    /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_InternalSendSignal()
                     * definition.
                     * But the void pointer is always deferenced to a same/lesser integer type & it is 32bit aligned.
                     * Hence the alignment warning can safely be ignored. */
                    Status = Com_InternalSendSignal( SignalId, (const void *)&txSigInvValue );
                 }
                 else
                 {
                     /*Find the ByteNo_u8 of the IPDU buffer*/
                     ByteNo_u8 = (PduLengthType)(TxSigConstPtr->Bit_Pos>>3u);
                     /* Enter exclusive area here */
                     SchM_Enter_Com_TxIpduProtArea(INVALIDATESIGNAL);

                     Com_ByteCopyInit((TxIPduConstPtr->BuffPtr + ByteNo_u8),
                     TxSigConstPtr->DataInvalid_Val,(uint32)TxSigConstPtr->BitSize);

                     SchM_Exit_Com_TxIpduProtArea(INVALIDATESIGNAL);
                     /* Exit exclusive area here */

                     /* MR12 DIR 1.1 VIOLATION: Explicit cast is provided in line with Com_InternalSendSignal()
                      * definition.
                      * But the void pointer is always deferenced to a same integer type. Hence the alignment
                      * warning can safely be ignored. */
                     Status = Com_InternalSendSignal( SignalId, (const void *)(TxIPduConstPtr->BuffPtr + ByteNo_u8) );
                 }
            }
        }
        else
        {
            /* Return COM_SERVICE_NOT_AVAILABLE if ComSignalDataInvalidValue is not configured with the given SignalId*/
        }
    }

    /* Note: if IPdu Group is stopped, then the Com_InternalSendSignal( ) will return COM_SERVICE_NOT_AVAILABLE */
    return (Status);

#else
    (void)SignalId;
    return( COM_SERVICE_NOT_AVAILABLE );
#endif /* #ifdef COM_TxInvalid */

}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

