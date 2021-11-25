

/**********************************************************************************************************************
 * Preprocessor includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/
#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProcessRxDMIPduGroupVector(
                                                            P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) ipduGroupVector
                                                                    );

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_EnableRxDeadlineMonitoring(VAR(Com_IpduIdType, AUTOMATIC) IpduId);

#endif

/**********************************************************************************************************************
 * Global functions (declared in header files)
 *********************************************************************************************************************/
/**********************************************************************************************************************
 Function name    : Com_ReceptionDMControl
 Syntax           : void Com_ReceptionDMControl(Com_IpduGroupVector ipduGroupVector)
 Description      : Service Enables deadline monitoring for the I-PDU groups within the given I-PDU group.
 Parameter        : ipduGroupVector -> contains the requested deadline monitoring state (disabled = 0/ enabled = 1)
                    for all I-PDU groups.
 Return value     : None
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(void, COM_CODE) Com_ReceptionDMControl(VAR(Com_IpduGroupVector, AUTOMATIC)  ipduGroupVector)
{
#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
    /* Local pointer to hold the address of the Rx Ipdu status structure           */
    Com_RxIpduRamPtrType            RxIPduRamPtr;
    VAR(uint16_least, AUTOMATIC)    Index_u16;
#endif
    VAR(uint16,AUTOMATIC)           ByteNo_u16;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Ptr( (ipduGroupVector == NULL_PTR), COMServiceId_ReceptionDMControl ))
#endif
    {
        /* START: Clear Unused BitFields
        * If the Group vector is filled with non-zero in unconfigured bitfields,
        * it might cause an exception in the software. */
        ByteNo_u16 = ( uint16 )( COM_GET_NUM_TOTAL_IPDU_GRP / 8u );
        if ( ByteNo_u16 < COM_NUM_ARR_IPDUGRPVECT )
        {
            ipduGroupVector[ ByteNo_u16 ] &= ( ( uint8 )( 0xFF ) >> ( 8u - ( COM_GET_NUM_TOTAL_IPDU_GRP % 8u ) ) );
            ByteNo_u16++;

            while ( ByteNo_u16 < COM_NUM_ARR_IPDUGRPVECT )
            {
                ipduGroupVector[ ByteNo_u16 ] = ( uint8 )0;
                ByteNo_u16++;
            }
        }
        /* END: Clear Unused BitFields */

#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)

        /* Extract ipduGroupVector and identify the IpduGroups with different states */
        Com_Prv_ProcessRxDMIPduGroupVector((P2CONST(uint8, AUTOMATIC, COM_APPL_DATA))(ipduGroupVector));

        RxIPduRamPtr   = &COM_GET_RXPDURAM_S(0);

        for (Index_u16 = 0; Index_u16 < COM_GET_NUM_RX_IPDU; Index_u16++)
        {
            if (COM_GET_IPDUCOUNTER_DM(Index_u16) >0 )
            {
                /*If the state is changed from RESET to SET*/
                /*As the counters are already updated, no necessary actions are required the other way around */
                /* if(RxIPduRamPtr->RxFlags.Pdu_DMStatus == 0)  */
                if ((Com_GetRamValue(RXIPDU,_DMSTATUS,RxIPduRamPtr->RxFlags)) == COM_STOP)
                {
                    Com_Prv_EnableRxDeadlineMonitoring((Com_IpduIdType)Index_u16);
                    /* (RxIPduRamPtr->RxFlags.Pdu_DMStatus = COM_SET)  */
                    Com_SetRamValue(RXIPDU,_DMSTATUS,RxIPduRamPtr->RxFlags, COM_START);
                }
            }
            else
            {
                /* if(RxIPduRamPtr->RxFlags.Pdu_DMStatus)  */
                if (Com_GetRamValue(RXIPDU,_DMSTATUS,RxIPduRamPtr->RxFlags))
                {
                    /*Interrupt is locked due to the updation of IPdu Group DM Status. */
                    /* (RxIPduRamPtr->RxFlags.Pdu_DMStatus = COM_RESET)  */
                    Com_SetRamValue(RXIPDU,_DMSTATUS,RxIPduRamPtr->RxFlags, COM_STOP);
                }
            }
            RxIPduRamPtr++;
        } /* End of Rx IPdu for loop */
#endif /* #if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout) */
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout)
/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessRxDMIPduGroupVector
 Syntax           : void Com_Prv_ProcessRxDMIPduGroupVector( const uint8 * ipduGroupVector )
 Description      : Service to process IPduGroupVector and to modify the local IPduGroups states as requested
                    by the application.
 Parameter        : ipduGroupVector - I-PDU group vector containing the latest Rx-DM activation state
                                      (stopped = 0/started = 1) for all I-PDU groups.
 Return value     : None
 *********************************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Com_Prv_ProcessRxDMIPduGroupVector:
   Com_Prv_ProcessRxDMIPduGroupVector function contains "for" loops and simple "if" statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProcessRxDMIPduGroupVector(
                                            P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) ipduGroupVector
                                                                    )
{
    /* Local pointer which holds the address of the array which stores the ipdu id */
    P2CONST(Com_IpduIdType, AUTOMATIC, COM_APPL_CONST)  IPduRefPtr;
    /* Local pointer to hold the address of Ipdu group structure                   */
    Com_IPduGrpConstPtrType                             IPduGrpConstPtr;
    VAR(uint16_least,AUTOMATIC)                         Index_u16;
    VAR(uint16_least,AUTOMATIC)                         NoOfPdus_u16;
    VAR(Com_IpduGroupIdType, AUTOMATIC)                 IpduGrpId_u16; /* To store the Ipdu Group Id */
    VAR(uint8, AUTOMATIC)                               ByteVal_u8;
    VAR(uint8,AUTOMATIC)                                BitOffset_u8;
    VAR(uint8,AUTOMATIC)                                PduCounterVal;

    for (Index_u16 = 0; Index_u16 < COM_NUM_ARR_IPDUGRPVECT; Index_u16++)
    {
        if (Com_IpduGrpVector_DM[Index_u16] != ipduGroupVector[Index_u16] )
        {
            /* Perform an XOR to find the Ipdu Groups which have changed the states */
            ByteVal_u8 = (uint8)((Com_IpduGrpVector_DM[Index_u16]) ^ (ipduGroupVector[Index_u16]));
            BitOffset_u8 = 0u;

            /*If there are some Ipdu groups which have changed the status*/
            do
            {
                /*Check for the status of the changed Ipdu groups and perform actions accordingly*/
                /* Check if any of the IPdu Group is changed by performing right bit shift*/
                if (COM_RESET != (ByteVal_u8 & 1u))
                {
                    /*Get the Ipdu group Id if the status is changed*/
                    IpduGrpId_u16   = (Com_IpduGroupIdType)(8u * Index_u16)+ BitOffset_u8 ;

                    /* Convert external HandleId to internal HandleId
                       IMP : ipduGroupVector will be set/reset based on the external ID.
                       It is required to convert the ID only when accessing the tables
                       For pre-compile , conversion is not required. Access Macro will return the same ID,
                       which is passed to it */

                    IpduGrpId_u16   = COM_GET_IPDUGRP_ID(IpduGrpId_u16);

                    /* Get the latest status of the IPdu group and store the any of the below value into PduCounterVal
                     * 0x01u eq +1, to increment the IPDU_COUNTER
                     * 0xFFu eq -1, to decrement the IPDU_COUNTER */
                    PduCounterVal   = ((((ipduGroupVector[Index_u16] >> BitOffset_u8) & 1u) != 0u) ? (0x1u) : (0xFFu));

                    IPduGrpConstPtr = COM_GET_IPDUGRP_CONSTDATA(IpduGrpId_u16);

                    IPduRefPtr      = COM_GET_IPDUGRP_IPDUREF_CONSTDATA(IPduGrpConstPtr->FirstIpdu_Index);

                    NoOfPdus_u16    = IPduGrpConstPtr->NoOfRxPdus;

                    while (NoOfPdus_u16 > 0u)
                    {
                        /*Check if the DM state of the Ipdu Group is changed and if it is an RX IPdu*/
                         COM_GET_IPDUCOUNTER_DM(*IPduRefPtr) = (uint8)
                        (COM_GET_IPDUCOUNTER_DM(*IPduRefPtr) + PduCounterVal);

                        IPduRefPtr++;
                        NoOfPdus_u16--;
                    }/* while (NoofPdus_u16 > 0 ) */
                } /* if (COM_RESET != (ByteVal_u8 & 1u)) */

                BitOffset_u8 ++;
                ByteVal_u8 >>= 1u;

            } while (ByteVal_u8 > 0u);

            /* update the status of the IPdu group */
            Com_IpduGrpVector_DM[Index_u16] = ipduGroupVector[Index_u16] ;
        } /* if (Com_IpduGrpVector_DM[Index_u16] != ipduGroupVector[Index_u16] ) */

    } /* End of For loop */
}


/**********************************************************************************************************************
 Function name    : Com_Prv_EnableRxDeadlineMonitoring
 Syntax           : void Com_Prv_EnableRxDeadlineMonitoring(Com_IpduIdType IpduId)
 Description      : Inline function to enable the Rx DeadlineMonitoring.
 Parameter        : IpduId -> Id of the Rx ipdu
 Return value     : None
**********************************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_EnableRxDeadlineMonitoring(VAR(Com_IpduIdType, AUTOMATIC) IpduId)
{
    Com_RxIpduConstPtrType                                      RxIPduConstPtr;
#ifdef COM_RxIPduTimeout
    Com_RxIpduRamPtrType                                        RxIPduRamPtr;
#endif
#if (defined(COM_RxSigUpdateTimeout) || defined(COM_RxSigGrpUpdateTimeout))
    P2CONST(Com_SignalTimeoutInfo, AUTOMATIC, COM_APPL_CONST)   SigTimeOutConfigPtr;
    P2VAR(uint16, AUTOMATIC, COM_APPL_DATA)                     SigTimeOutTicksPtr;
    VAR(uint16_least, AUTOMATIC)                                Idx_Sig_ui;
    VAR(uint16_least, AUTOMATIC)                                NoOfSigOrSigGrp_ui;
#endif

    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(IpduId);

#ifdef COM_RxIPduTimeout
    RxIPduRamPtr   = &COM_GET_RXPDURAM_S(IpduId);

    /* If First timeout is not configured , load the timer with specific value ie COM_RXTIMER_MAX(0xFFFF).
     * If First timeout is configured then load the actual value
     */
    RxIPduRamPtr->RxTicks_u16 = (RxIPduConstPtr->FirstTimeout_Factor == 0) ? COM_RXTIMER_MAX :
                                                                             RxIPduConstPtr->FirstTimeout_Factor;
#endif /* #ifdef COM_RxIPduTimeout */

#ifdef COM_RxSigUpdateTimeout
    /* If this IPDU has signals with update-bits , initialise the pointers.
     * Separate deadline monitoring should be performed for signals with update-bit
     */
    if (RxIPduConstPtr->SignalTimeout_Ref != NULL_PTR)
    {
        /* Get the number of signals with update-bit & configured timeout, within this IPDU */
        NoOfSigOrSigGrp_ui  = RxIPduConstPtr->SignalTimeout_Ref->numWithUpdateBit;
        /* Initialise pointer to Com_SignalTimeoutInfo */
        SigTimeOutConfigPtr = RxIPduConstPtr->SignalTimeout_Ref->TimeoutInfo_Ref;
        /* Initialise pointer to signal RAM timer */
        SigTimeOutTicksPtr  = RxIPduConstPtr->SignalTimeout_Ref->TimeoutTicks_p;

        SchM_Enter_Com_RxSigToTicks();

        for (Idx_Sig_ui = NoOfSigOrSigGrp_ui; Idx_Sig_ui != 0u; Idx_Sig_ui--)
        {
            /* If FirstTimeOutFactor is not configured then load timer with COM_RXTIMER_MAX(0xFFFF)
             * If FirstTimeOutFactor is configured then load the ComFirstTimeout value
             */
            (*SigTimeOutTicksPtr) = (SigTimeOutConfigPtr->First_Timeout == 0) ? COM_RXTIMER_MAX :
                                                                                SigTimeOutConfigPtr->First_Timeout;

            /* Increment pointers to refer next signal with update-bit(within this IPDU) */
            SigTimeOutTicksPtr++;
            SigTimeOutConfigPtr++;

        }/*End of for loop*/

        SchM_Exit_Com_RxSigToTicks();
    }
#endif /* #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSigGrpUpdateTimeout
    /* If this IPDU has signal groups with update-bits , initialise the pointers.
     * Separate deadline monitoring should be performed for signal groups with update-bit
     */
    if (RxIPduConstPtr->SignalGrpTimeout_Ref != NULL_PTR)
    {
        /* Get the number of signals with update-bit & configured timeout, within this IPDU */
        NoOfSigOrSigGrp_ui  = RxIPduConstPtr->SignalGrpTimeout_Ref->numWithUpdateBit;
        /* Initialise pointer to Com_SignalGrpTimeoutInfo */
        SigTimeOutConfigPtr = RxIPduConstPtr->SignalGrpTimeout_Ref->TimeoutInfo_Ref;
        /* Initialise pointer to signalGroup timer */
        SigTimeOutTicksPtr = RxIPduConstPtr->SignalGrpTimeout_Ref->TimeoutTicks_p;

        SchM_Enter_Com_RxSigGrpToTicks();

        for (Idx_Sig_ui = NoOfSigOrSigGrp_ui; Idx_Sig_ui != 0u; Idx_Sig_ui--)
        {
            /* If FirstTimeOutFactor is not configured then load timer with COM_RXTIMER_MAX(0xFFFF)
             * If FirstTimeOutFactor is configured then load the ComFirstTimeout value
             */
            (*SigTimeOutTicksPtr) = (SigTimeOutConfigPtr->First_Timeout == 0) ? COM_RXTIMER_MAX :
                                                                                SigTimeOutConfigPtr->First_Timeout;

            /* Increment pointers to refer next signal group with update-bit(within this IPDU)
             * If no next signal group with update-bit is present in the IPDU, then the if() check fails and
             * the code inside if() block is not executed.
             */
            SigTimeOutConfigPtr++;
            SigTimeOutTicksPtr++;
        }/*End of for loop*/

        SchM_Exit_Com_RxSigGrpToTicks();
    }
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */
}
#endif /* #if defined (COM_RxIPduTimeout) || defined (COM_RxSigUpdateTimeout) || defined (COM_RxSigGrpUpdateTimeout) */

