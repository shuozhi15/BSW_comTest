

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
#ifdef COM_TP_IPDUTYPE

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : Com_TpRxIndication
 Syntax           : void Com_TpRxIndication( PduIdType PduId, Std_ReturnType Result )
 Description      : By a call to Com_TpRxIndication the AUTOSAR COM module starts the processing
                    of the received large or dynamic length IPDU.
 Parameter        : PduId,Result
 Return value     : Void
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(void, COM_CODE) Com_TpRxIndication( PduIdType PduId, Std_ReturnType Result )
{
    /* Start: Variable declarations */
    Com_RxIpduConstPtrType RxIPduConstPtr;
    Com_RxIpduRamPtrType   RxIPduRamPtr;
    VAR(uint8, AUTOMATIC)  ConstByteValue_u8;
    /* End: Variable declarations */

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Errors COM_E_UNINIT and COM_E_PARAM */
    if(Com_Prv_DETCheck_Param((PduId >= COM_GET_NUM_RX_IPDU), COMServiceId_TpRxIndication))
#endif
    {
        /* If PB variant is selected, then PduId which is passed to this function will be changed
        * to internal Id which is generated through configuration
        * If PC variant is selected, then no mapping table will be used. */
        PduId = COM_GET_RX_IPDU_ID(PduId);

        RxIPduConstPtr    = COM_GET_RX_IPDU_CONSTDATA(PduId);
        RxIPduRamPtr      = &COM_GET_RXPDURAM_S(PduId);

        ConstByteValue_u8 = RxIPduConstPtr->rxIPduFields;

        /* Check whether RxPdu flag is set, which will be done in Com_StartOfReception and
         * Proceed further only if the reception result was successfull
         */
        if((Com_GetRamValue(RXIPDU,_LARGEDATAINPROG,RxIPduRamPtr ->RxFlags)) && (Result == E_OK))
        {
            /*Reset The large pdu Rx flag, to accept new reception*/
            Com_SetRamValue(RXIPDU,_LARGEDATAINPROG,RxIPduRamPtr ->RxFlags,COM_FALSE);

            /*Set the RxIndication flag*/
            Com_SetRamValue(RXIPDU,_INDICATION,RxIPduRamPtr ->RxFlags,COM_TRUE);

            RxIPduRamPtr->RxIPduLength = RxIPduRamPtr->RxTPIPduLength;

            /* END: Now Copy the Data into the Local IPDU buffer */
            if (Com_GetValue(RXIPDU,_SIGPROC,ConstByteValue_u8) == COM_IMMEDIATE)
            {
                VAR(PduInfoType, COM_VAR) TPRxIPduInfo;   /* Variable to Hold received TP-IPdu information */

                /* Assign the Address and the length to local PduInfoType variable */
                TPRxIPduInfo.SduDataPtr = RxIPduConstPtr->BuffPtr;
                TPRxIPduInfo.SduLength  = RxIPduRamPtr->RxIPduLength;

                Com_ProcessRxIPdu(PduId,&TPRxIPduInfo);
            }
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* COM_TP_IPDUTYPE */

