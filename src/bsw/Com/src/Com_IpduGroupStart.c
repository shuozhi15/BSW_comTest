

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/**********************************************************************************************************************
 * Local Prototypes
 *********************************************************************************************************************/

static INLINE FUNC(void, COM_CODE) Com_RxIPduStart( VAR(Com_IpduIdType, AUTOMATIC)  IpduId,
                                                    VAR(boolean, AUTOMATIC)         Initialize );

static INLINE FUNC(void, COM_CODE) Com_TxIPduStart( VAR(Com_IpduIdType, AUTOMATIC)  IpduId,
                                                    VAR(boolean, AUTOMATIC)         Initialize );

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProcessIPduGroupVector(
                                                    P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) ipduGroupVector
                                                            );

LOCAL_INLINE FUNC(void, COM_CODE)  Com_Prv_UpdateBitAndFilter(VAR(Com_IpduIdType, AUTOMATIC) IpduId);


/**********************************************************************************************************************
 * Global functions (declared in header files)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Function name    : Com_IpduGroupControl
 Syntax           : void Com_IpduGroupControl( Com_IpduGroupVector ipduGroupVector, boolean initialize )
 Description      : Service for controlling the Ipdu groups
 Parameter        : Com_IpduGroupVector - I-PDU group vector containing the activation state (stopped = 0/started = 1)
                                          for all I-PDU groups.
                  : Initialize          - flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
/* HIS METRIC CALLS, VIOLATION IN Com_IpduGroupControl:
   Com_IpduGroupControl function contains necessary function calls, "for" loops and simple "if" and "else if" statement.
   HIS metric compliance would decrease readability and maintainability */
FUNC(void, COM_CODE) Com_IpduGroupControl(
                                VAR(Com_IpduGroupVector, AUTOMATIC)  ipduGroupVector,
                                VAR(boolean, AUTOMATIC)              initialize
                                         )
{
    Com_RxIpduRamPtrType            RxIPduRamPtr;
    Com_TxIpduRamPtrType            TxIPduRamPtr;
    VAR(uint16_least,AUTOMATIC)     Index_u16;

#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Ptr((ipduGroupVector == NULL_PTR), COMServiceId_IpduGroupControl))
#endif
    {
        /* A call to the API Com_ReceptionDMControl is made to set the DM vector with the same value
         * as that of the IPduGroupVector */
        Com_ReceptionDMControl(ipduGroupVector);

        /* Extract ipduGroupVector and flag changed IpduGroups */
        Com_Prv_ProcessIPduGroupVector((P2CONST(uint8, AUTOMATIC, COM_APPL_DATA))ipduGroupVector);

        RxIPduRamPtr   = &COM_GET_RXPDURAM_S(0);
        for (Index_u16 = 0; Index_u16 < COM_GET_NUM_RX_IPDU; Index_u16++)
        {
            /* If any of the Ipdu Group containing the IPdu is active,
             * i.e., counter will have non-zero value */
            if (COM_GET_IPDUCOUNTER_S(Index_u16) > 0)
            {
                /* If the PDU state is changed from STOP to START */
                if (Com_GetRamValue(RXIPDU,_PDUSTATUS,RxIPduRamPtr->RxFlags) == COM_STOP)
                {
                    Com_RxIPduStart((Com_IpduIdType)Index_u16,initialize);

                    Com_SetRamValue(RXIPDU,_PDUSTATUS,RxIPduRamPtr->RxFlags,COM_START);
                }
            }
            else
            {
                /* If the PDU state is changed from START to STOP */
                if (Com_GetRamValue(RXIPDU,_PDUSTATUS,RxIPduRamPtr->RxFlags))
                {
                    Com_SetRamValue(RXIPDU,_INDICATION,RxIPduRamPtr->RxFlags,COM_FALSE);

                    Com_SetRamValue(RXIPDU,_PDUSTATUS,RxIPduRamPtr->RxFlags,COM_STOP);
                    /* Large Pdu Rx status is reset, No further calls for this reception are processed */
                    Com_SetRamValue(RXIPDU,_LARGEDATAINPROG,RxIPduRamPtr->RxFlags,COM_FALSE);
                }
            }

            RxIPduRamPtr++;
        } /*End of RxIpdu for loop */

        TxIPduRamPtr   = &COM_GET_TXPDURAM_S(0);
        for (Index_u16 = 0; Index_u16 < COM_GET_NUM_TX_IPDU; Index_u16++)
        {
            /* If any of the Ipdu Group containing the IPdu is active,
             * i.e., counter will have non-zero value */
            if (COM_GET_IPDUCOUNTER_S(Index_u16 + COM_GET_NUM_RX_IPDU) > 0)
            {
                /* If the PDU state is changed from STOP to START */
                if (Com_GetRamValue(TXIPDU,_PDUSTATUS,TxIPduRamPtr->Com_TxFlags) == COM_STOP)
                {
                    /* NOTE: This below order is to be maintained, to avoid any interrupt related race conditions.
                     * REASON: If the below function call is interrupted, by any other API,
                     * as the _PDUSTATUS is set after the function returns,
                     * the interrupting API returns without any effect */
                    Com_TxIPduStart((Com_IpduIdType)Index_u16,initialize);

                    Com_SetRamValue(TXIPDU,_PDUSTATUS,TxIPduRamPtr->Com_TxFlags,COM_START);
                    /* By default release the large Ipdu buffer lock, which is acquired in Com_Prv_SendIpdu */
                    Com_SetRamValue(TXIPDU,_LARGEDATAINPROG,TxIPduRamPtr->Com_TxFlags,COM_FALSE);
                }
            }
            else
            {
                /* If the PDU state is changed from START to STOP */
                if (Com_GetRamValue(TXIPDU,_PDUSTATUS,TxIPduRamPtr->Com_TxFlags))
                {
                    /* NOTE: This below order is to be maintained, to avoid any interrupt related race conditions.
                     * REASON: If the below function call is interrupted, by any other API,
                     * as the _PDUSTATUS is set before the function call,
                     * the interrupting API returns without any effect */
                    Com_SetRamValue(TXIPDU,_PDUSTATUS,TxIPduRamPtr->Com_TxFlags,COM_STOP);
                    Com_TxIPduStop((Com_IpduIdType)Index_u16);
                }
            }

            TxIPduRamPtr++;
        } /*  End of TxIpdu for loop  */
    }/* End of DET Check */
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**********************************************************************************************************************
 Function name    : Com_Prv_ProcessIPduGroupVector
 Syntax           : void Com_Prv_ProcessIPduGroupVector( const uint8 * ipduGroupVector )
 Description      : Service to process IPduGroupVector and to modify the local IPduGroups states as requested
                    by the application.
 Parameter        : ipduGroupVector - I-PDU group vector containing the latest activation state
                                      (stopped = 0/started = 1) for all I-PDU groups.
                  : Initialize      - flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 *********************************************************************************************************************/
/* HIS METRIC LEVEL VIOLATION IN Com_Prv_ProcessIPduGroupVector:
   Com_Prv_ProcessIPduGroupVector function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_ProcessIPduGroupVector(
                                            P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) ipduGroupVector
                                                            )
{
    P2CONST(Com_IpduIdType,AUTOMATIC,COM_APPL_CONST)    IPduRefPtr;
    Com_IPduGrpConstPtrType                             IPduGrpConstPtr;
    VAR(uint16,AUTOMATIC)                               Index_u16;
    VAR(uint16,AUTOMATIC)                               NoOfPdus_u16;
    VAR(Com_IpduGroupIdType,AUTOMATIC)                  IpduGrpId_u16;
    VAR(uint8,AUTOMATIC)                                ByteVal_u8;
    VAR(uint8,AUTOMATIC)                                BitOffset_u8;
    VAR(uint8,AUTOMATIC)                                PduCounterVal;

    for (Index_u16 = 0; Index_u16 < COM_NUM_ARR_IPDUGRPVECT; Index_u16++)
    {
        if (Com_IpduGrpVector[Index_u16] != ipduGroupVector[Index_u16])
        {
            /* Perform an XOR to find the Ipdu Groups which have changed the states */
            ByteVal_u8   = (uint8)(Com_IpduGrpVector[Index_u16] ^ ipduGroupVector[Index_u16]);
            BitOffset_u8 = 0u;

            /* If there are some Ipdu groups which have changed the status */
            do
            {
                /* Check for the status of the changed Ipdu groups and perform actions accordingly.
                 * Check if any of the IPdu Group is changed by performing right bit shift */
                if (COM_RESET != (ByteVal_u8 & 1u))
                {
                    /* Get the Ipdu group Id if the status is changed */
                    IpduGrpId_u16   = (8u * Index_u16) + BitOffset_u8;

                    /* Convert external HandleId to internal HandleId.
                     * IMP : ipduGroupVector will be set/reset based on the external ID.
                     * It is required to convert the ID only when accessing the tables. For pre-compile,
                     * conversion is not required. Access Macro will return the same ID, which is passed to it */
                    IpduGrpId_u16   = COM_GET_IPDUGRP_ID(IpduGrpId_u16);

                    /* Get the latest status of the IPdu group and store the any of the below value
                     * 0x01u eq +1, to increment the IPDU_COUNTER
                     * 0xFFu eq -1, to decrement the IPDU_COUNTER */
                    PduCounterVal   = ((((ipduGroupVector[Index_u16] >> BitOffset_u8) & 1u) != 0u) ? (0x1u) : (0xFFu));

                    IPduGrpConstPtr = COM_GET_IPDUGRP_CONSTDATA(IpduGrpId_u16);
                    IPduRefPtr      = COM_GET_IPDUGRP_IPDUREF_CONSTDATA(IPduGrpConstPtr->FirstIpdu_Index);

                    /* Difference between the current IPduGroup Index to the next Index provides
                     * the total number of Pdus referred to the IPduGroup.
                     * In case, current IPduGroup is the last member in the generated table, then the total number of
                     * Pdus are stored in the separate pre-processor directive[COM_GET_NUM_IPDUS_IN_LAST_IPDUGRP]
                     */
                    NoOfPdus_u16 =  (IpduGrpId_u16 != (COM_GET_NUM_TOTAL_IPDU_GRP - 1u)) ?
                                    ((IPduGrpConstPtr + 1)->FirstIpdu_Index - IPduGrpConstPtr->FirstIpdu_Index) :
                                    (COM_GET_NUM_IPDUS_IN_LAST_IPDUGRP);

                    while (NoOfPdus_u16 > 0u)
                    {
                        /* Below counter shall increment if the latest state is started otherwise decrement it. */
                         COM_GET_IPDUCOUNTER_S(*IPduRefPtr) = (uint8)
                        (COM_GET_IPDUCOUNTER_S(*IPduRefPtr) + PduCounterVal);

                        IPduRefPtr++;
                        NoOfPdus_u16--;

                    } /* while (NoOfPdus_u16 > 0u) */

                } /* if (COM_RESET != (ByteVal_u8 & 1u)) */

                BitOffset_u8++;
                ByteVal_u8 >>= 1u;

            } while (ByteVal_u8 > 0u);

            /* update the status of the IPdu group */
            Com_IpduGrpVector[Index_u16] = ipduGroupVector[Index_u16];

        } /* if (Com_IpduGrpVector[Index_u16] != ipduGroupVector[Index_u16] ) */

    } /* End of For loop */
}


/**********************************************************************************************************************
 Function name    : Com_ClearIpduGroupVector
 Syntax           : void Com_ClearIpduGroupVector( Com_IpduGroupVector ipduGroupVector )
 Description      : Service to clear Ipdu group vector
 Parameter        : Com_IpduGroupVector - I-PDU group vector containing
                    the activation state (stopped = 0/ started = 1) for all I-PDU groups.
 Return value     : None
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_ClearIpduGroupVector( VAR(Com_IpduGroupVector, AUTOMATIC) ipduGroupVector )
{
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Ptr( (ipduGroupVector == NULL_PTR), COMServiceId_ClearIpduGroupVector ))
#endif
    {
        VAR(uint16, AUTOMATIC) Index_u16;

        for (Index_u16 = 0u; Index_u16 < COM_NUM_ARR_IPDUGRPVECT; Index_u16++)
        {
            ipduGroupVector[Index_u16] = (uint8)0;
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/**********************************************************************************************************************
 Function name    : Com_SetIpduGroup
 Syntax           : void Com_SetIpduGroup(
                                Com_IpduGroupVector ipduGroupVector,
                                Com_IpduGroupIdType IpduGroupId,
                                boolean             bitval
                                         )
 Description      : Service to set bit of the given Ipdu group vector that corresponds to the given I-PDU group.
 Parameter        : ipduGroupVector - I-PDU group vector to be modified
                  : IpduGroupId     - ipduGroup used to identify the corresponding bit in the I-PDU group vector
                  : bitval          - New value of the corresponding bit
 Return value     : None
 *********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_SetIpduGroup(
                            VAR(Com_IpduGroupVector, AUTOMATIC)  ipduGroupVector,
                            VAR(Com_IpduGroupIdType, AUTOMATIC)  IpduGroupId,
                            VAR(boolean, AUTOMATIC)              bitval
                                     )
{
#if(COM_CONFIGURATION_USE_DET == STD_ON)
    if (Com_Prv_DETCheck_Param_Ptr( (IpduGroupId >= COM_GET_NUM_TOTAL_IPDU_GRP),(ipduGroupVector == NULL_PTR),
                                     COMServiceId_SetIpduGroup))
#endif
    {
        VAR(uint16, AUTOMATIC)   Index_u16;
        VAR(uint8, AUTOMATIC)    BitOffset_u8;

        Index_u16     = (uint16)(IpduGroupId >> 3u);
        BitOffset_u8  = (uint8) (IpduGroupId % 8u);

        if (bitval)
        {
            ipduGroupVector[Index_u16] |= ((uint8)(1u << BitOffset_u8));
        }
        else
        {
            ipduGroupVector[Index_u16] &= ((uint8)(~(1u << BitOffset_u8)));
        }
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/***************************************************************************************************
 Function name    : Com_TxIPduStart
 Syntax           : void Com_TxIPduStart(Com_IpduIdType IpduId, boolean Initialize)
 Description      : Service to start the Tx ipdu
 Parameter        : IpduId      -> Id of the ipdu
                  : Initialize  -> flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 ***************************************************************************************************/

static INLINE FUNC(void, COM_CODE) Com_TxIPduStart(VAR(Com_IpduIdType, AUTOMATIC) IpduId,
                                                   VAR(boolean, AUTOMATIC) Initialize)
{

    Com_TxIpduConstPtrType  TxIPduConstPtr; /* Local pointer to Tx ipdu static  configuration */
    Com_TxIpduRamPtrType    TxIpduRamPtr;   /* Local pointer to Tx ipdu dynamic configuration */

    VAR(uint8, AUTOMATIC)   LatestTransMode_u8;

    /* The below Initialize check should be only after Com_LatestTransMode is calculated */
    if(Initialize != COM_FALSE)
    {
        TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);

        TxIpduRamPtr   = &COM_GET_TXPDURAM_S(IpduId);

        /* Call function to update bit   */
        Com_Prv_UpdateBitAndFilter(IpduId);

        /* Call Com_TxChangeMode( ) to calculate the Com_LatestTransMode[i] */
        Com_TxChangeMode(IpduId);
        /* Note: Even though if the Com_Tick_Tx is updated in Com_TxChangeMode(),
                it is once again updated in Ipdu Group Start i.e below */
        LatestTransMode_u8 = Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode);

        /* Reset the global TMS status flag on the start of IPduGroup */
        Com_SetRamValue(TXIPDU,_GWIPDUTMSCHANGED,TxIpduRamPtr->Com_TxFlags,COM_FALSE);

        /* time period and offset attributes of IPDUs in Periodic or Mixed Transmission Mode */
        /* The CurrentTxModePtr points to the structure information of the current TxModeState */
        if( COM_RESET != ( LatestTransMode_u8 & COM_TXMODE_MIXED ) )
        {
            /* I.e Load the TxTimeOffset for cyclic type Pdu's(PERIODIC or MIXED) */
            /* FC_VariationPoint_START */
#ifdef COM_TXPDU_TIMEOFFSET_VIA_CALIBRATION
            TxIpduRamPtr->Com_Tick_Tx = Com_GetTxIPduTimeOffsetCalibration((PduIdType)IpduId);
#else
            /* FC_VariationPoint_END */
            TxIpduRamPtr->Com_Tick_Tx = TxIpduRamPtr->CurrentTxModePtr->TimeOffsetFact;
            /* FC_VariationPoint_START */
#endif /* COM_TXPDU_TIMEOFFSET_VIA_CALIBRATION */
            /* FC_VariationPoint_END */
        }
        /* the minimum delay time attribute of IPDUs in
                       Direct/N-Times or Mixed Transmission Mode */
        /*Reset it to Zero i.e No Min delay check as soon as IpduGroup is started */
        TxIpduRamPtr->Com_MinDelayTick = 0;

        /*RESET all the Tx Confirmation and Tx timeout variables*/
        /*Reset it to Stop, Note: This Flag has to be set
                           to START only in SendIpdu() i,e just before Sending the IPDU*/
        Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_STOP);
        /*Reset it to RESET*/

        /* The AUTOSAR COM module shall start the transmission deadline monitoring timer
         ** for this I-PDU upon the start of the I-PDU group to which the I-PDU belongs to*/
#ifdef COM_TxIPduTimeOut
        if(Com_GetValue(TXIPDU,_NONETOSTATUS,TxIPduConstPtr->txIPduFields))
        {
            Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_START);
            TxIpduRamPtr->Com_TickTxTimeout = TxIPduConstPtr->Timeout_Fact;
        }
        else
#endif/* #ifdef COM_TxIPduTimeOut */
        {
            /*Reset the timer to Zero */
#ifdef COM_TxIPduTimeOut
            TxIpduRamPtr->Com_TickTxTimeout = 0;
#endif/* #ifdef COM_TxIPduTimeOut */
            Com_SetRamValue(TXIPDU,_CONFIR,TxIpduRamPtr->Com_TxFlags,COM_FALSE);
        }
    }/*End of If(INITIALIZE)*/
    else
    {

        /* Check if IPDU group is TX group
         * Cyclic IPDU's will be sent out cyclically after the call of this API
         */
        TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);

#ifdef COM_TxIPduTimeOut
        TxIpduRamPtr = &COM_GET_TXPDURAM_S(IpduId);
#endif
        /* Call Com_TxChangeMode() to calculate the Com_LatestTransMode[i] */
        Com_TxChangeMode((Com_IpduIdType)IpduId);
        /* Note: Eventhough if the Com_Tick_Tx is updated in Com_TxChangeMode(),
                it is once again updated in Ipdu Group Start i.e below */

        /* The AUTOSAR COM module shall start the transmission deadline monitoring timer
         * for this I-PDU upon the start of the I-PDU group to which the I-PDU belongs to*/
#ifdef COM_TxIPduTimeOut
        if(Com_GetValue(TXIPDU,_NONETOSTATUS,TxIPduConstPtr->txIPduFields))
        {
            Com_SetRamValue(TXIPDU,_TICKTXTO,TxIpduRamPtr->Com_TxFlags,COM_START);
            TxIpduRamPtr->Com_TickTxTimeout = TxIPduConstPtr->Timeout_Fact;
        }
#endif
    }
}

/**********************************************************************************************************************
 Function name    : Com_RxIPduStart
 Syntax           : void Com_RxIPduStart(Com_IpduIdType IpduId, boolean Initialize)
 Description      : Service to start the Rx-IPDU
 Parameter        : IpduId     - Id of the Rx-IPDU
                  : Initialize - flag to request initialization of the I-PDUs which are newly started
 Return value     : None
 *********************************************************************************************************************/
static INLINE FUNC(void, COM_CODE) Com_RxIPduStart(
                                            VAR(Com_IpduIdType, AUTOMATIC)  IpduId,
                                            VAR(boolean, AUTOMATIC)         Initialize
                                                  )
{
#if (defined(COM_RxFilters) && defined(COM_F_ONEEVERYN))
    Com_RxIpduConstPtrType           RxIPduConstPtr;
    Com_RxSigConstPtrType            RxSigConstPtr;
    VAR(uint16_least, AUTOMATIC)     Idx_Sig_Grp_ui; /* Index for both Rx-SignalGroup and Rx-Signal */
# ifdef COM_RX_SIGNALGROUP
    Com_RxSigGrpConstPtrType         RxSigGrpConstPtr;
    Com_RxGrpSigConstPtrType         RxGrpSigConstPtr;
    VAR(uint16_least, AUTOMATIC)     Idx_GrpSig_ui; /* Index for Rx-GroupSignal */
# endif

    RxIPduConstPtr   = COM_GET_RX_IPDU_CONSTDATA(IpduId);

    /* Rx-SignalGroup */
# ifdef COM_RX_SIGNALGROUP
    RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA(RxIPduConstPtr->FirstRxSigGrp_Index);

    for (Idx_Sig_Grp_ui = (uint16_least)RxIPduConstPtr->No_Of_SigGrp_Ref; Idx_Sig_Grp_ui != 0u; Idx_Sig_Grp_ui--)
    {
        if (Com_GetValue(RXSIGGRP,_FILTEREVALREQ,RxSigGrpConstPtr->rxSignalGrpFields))
        {
            RxGrpSigConstPtr = COM_GET_RXGRPSIG_CONSTDATA(RxSigGrpConstPtr->FirstGrpSig_Index);

            for (Idx_GrpSig_ui = (uint16_least)RxSigGrpConstPtr->No_Of_GrpSig; Idx_GrpSig_ui != 0u; Idx_GrpSig_ui--)
            {
                if (RxGrpSigConstPtr->Filter_Algo == (uint8)COM_ONE_EVERY_N)
                {
                    /* [SWS_Com_00787] reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N */
                    COM_GET_ONEEVERYN(RxGrpSigConstPtr->Filter_Index).Occurrence = 0;
                }
                RxGrpSigConstPtr++;
            }
        }
        RxSigGrpConstPtr++;
    }
# endif /* #ifdef COM_RX_SIGNALGROUP */

    /* Rx-Signal */
    RxSigConstPtr    = COM_GET_RXSIG_CONSTDATA(RxIPduConstPtr->RxSigId_Index);

    for (Idx_Sig_Grp_ui = (uint16_least)RxIPduConstPtr->No_Of_Sig_Ref; Idx_Sig_Grp_ui != 0u; Idx_Sig_Grp_ui--)
    {
        if (Com_GetValue(RXSIG,_FILTRALG,RxSigConstPtr->rxSignalFields) == (uint8)COM_ONE_EVERY_N)
        {
            /* [SWS_Com_00787] reset OCCURRENCE of filter with ComFilterAlgorithm ONE_EVERY_N */
            COM_GET_ONEEVERYN(RxSigConstPtr->Filter_Index).Occurrence = 0;
        }
        RxSigConstPtr++;
    }

#else
    /*  Misra warning removal for unused parameter */
    (void)IpduId;
#endif/* #if (defined(COM_RxFilters) && defined(COM_F_ONEEVERYN)) */

    (void)Initialize; /* This variable is retained for future use */
}

/**********************************************************************************************************************
 Function name    : Com_Prv_UpdateBitAndFilter
 Syntax           : Com_Prv_UpdateBitAndFilter(Com_IpduIdType IpduId)
 Description      : Service to clear update bit if configured for all the configured signal and signalgroup
 Parameter        : IpduId      -> Id of the ipdu
 Return value     : None
 *********************************************************************************************************************/

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_UpdateBitAndFilter(VAR(Com_IpduIdType, AUTOMATIC) IpduId)
{

    Com_TxSigConstPtrType   TxSigConstPtr;  /* Local pointer to Tx Signal static configuration */
    Com_TxIpduConstPtrType  TxIPduConstPtr; /* Local pointer to Tx ipdu static configuration */

    VAR(uint16_least, AUTOMATIC) Index_Sig_ui;
#ifdef COM_TX_SIGNALGROUP
    Com_TxSigGrpConstPtrType     TxSigGrpConstPtr;    /* Local pointer to Tx SignalGroup static configuration */
    VAR(uint16_least, AUTOMATIC) Index_SigGrp_ui;
#endif

#if defined (COM_TX_SIGNALGROUP) && defined(COM_TxFilters) && defined (COM_F_ONEEVERYN)
    Com_TxGrpSigConstPtrType      TxGrpSigConstPtr;  /* Local pointer to Tx GroupSignal static configuration */
#endif

#if defined (COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)
    VAR(PduLengthType, AUTOMATIC) ByteNo_u8;
    VAR(uint8, AUTOMATIC)         ConstByteValue_u8;
#endif

    TxIPduConstPtr      = COM_GET_TX_IPDU_CONSTDATA(IpduId);

    TxSigConstPtr       = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);

    for(Index_Sig_ui = TxIPduConstPtr->No_Of_Sig_Ref;Index_Sig_ui != 0u;Index_Sig_ui--)
    {

        #ifdef COM_TxSigUpdateBit
        /* Only the first byte of txIPduFields is needed */
        ConstByteValue_u8 = (uint8)TxIPduConstPtr->txIPduFields;

        /* Get ClearBit and Update Bit */
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
         * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        if((Com_GetValue(TXIPDU,_CLRUPDBIT,ConstByteValue_u8) != (uint16)COM_CLRUPDATEBIT_NONE) &&
           (Com_GetValue(GEN,_UPDBITCONF,TxSigConstPtr->General)))
        {

            /* Find the Byte No, where the Update bit lies */
            ByteNo_u8 = (PduLengthType)((TxSigConstPtr->Update_Bit_Pos)>>3u);
            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(TxIPduConstPtr,TxSigConstPtr->Update_Bit_Pos,ByteNo_u8)
        }
        #endif/*COM_TxSigUpdateBit*/

            /* OCCURRENCE shall be set to zero by Com_IpduGroupStart*/
        #ifdef COM_TxFilters
        # ifdef COM_F_ONEEVERYN
        if(Com_GetValue(TXSIG,_FILTRALG,TxSigConstPtr->txSignalFields) == (uint16)COM_ONE_EVERY_N)
        {
            COM_GET_ONEEVERYN(TxSigConstPtr->Filter_Index).Occurrence = COM_ZERO;
        }
        # endif
        #endif

        TxSigConstPtr++;
    }

    #ifdef COM_TX_SIGNALGROUP

    TxSigGrpConstPtr   = COM_GET_TXSIGGRP_CONSTDATA(TxIPduConstPtr->FirstTxSigGrp_Index);

    for(Index_SigGrp_ui=(uint16_least)TxIPduConstPtr ->No_Of_SigGrp_Ref;Index_SigGrp_ui != 0u;Index_SigGrp_ui--)
    {

        # if defined(COM_TxFilters) && defined (COM_F_ONEEVERYN)

        TxGrpSigConstPtr = COM_GET_TXGRPSIG_CONSTDATA(TxSigGrpConstPtr ->FirstGrpSig_Index);

        for(Index_Sig_ui = TxSigGrpConstPtr ->No_Of_GrpSig;Index_Sig_ui !=0u;Index_Sig_ui--)
        {

            /* OCCURRENCE shall be set to zero by Com_IpduGroupStart*/
            if( TxGrpSigConstPtr->Filter_Algo == (uint8)COM_ONE_EVERY_N)
            {
                COM_GET_ONEEVERYN(TxGrpSigConstPtr->Filter_Index).Occurrence = COM_ZERO;
            }
            TxGrpSigConstPtr++;
        }
        # endif /* #if defined(COM_TxFilters) && defined (COM_F_ONEEVERYN) */

            /* Clear Update bits*/
        # ifdef COM_TxSigGrpUpdateBit
        /* Only the first byte of txIPduFields is needed */
        ConstByteValue_u8 = (uint8)TxIPduConstPtr->txIPduFields;

        /* Get ClearBit and Update Bit */
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads flag status. Evaluation(or no evaluation)
         * of the 2nd or 3rd operands does not impact the system behaviour even though of volatile-qualified,
         * moreover HIS-LEVEL parameter will be violated if more conditional statements are added.
         * Hence the warning is suppressed. */
        if((Com_GetValue(TXIPDU,_CLRUPDBIT,ConstByteValue_u8) != (uint16)COM_CLRUPDATEBIT_NONE) &&
           (Com_GetValue(TXSIGGRP,_UPDBITCONF,TxSigGrpConstPtr->txSignalGrpFields)))
        {

            /*Find the Byte No, where the Update bit lies */
            ByteNo_u8 = (PduLengthType)((TxSigGrpConstPtr ->Update_Bit_Pos)>>3u);
            /* This macro clears the update bit value at the Update bit position */
            Com_ClearUpdateBitValue(TxIPduConstPtr,TxSigGrpConstPtr ->Update_Bit_Pos,ByteNo_u8)
        }
        # endif  /*COM_TxSigGrpUpdateBit*/

        TxSigGrpConstPtr++;
    }
    #endif
}


