

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"

/* FC_VariationPoint_START */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_StatusType,COM_VAR)         Com_Uninit_Flag;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
/* FC_VariationPoint_END */

/* Note: Com_Prv_Config_pcst pointer variable should always be under Pre-Compile MemMap section, or else it can lead to
 * RAM corruption */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
P2CONST(Com_ConfigData_tst,AUTOMATIC,COM_APPL_CONST) Com_Prv_Config_pcst;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

#if(COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)

LOCAL_INLINE FUNC(boolean, COM_CODE) Com_CompareVersionInfo(
                                               P2CONST(Std_VersionInfoType, AUTOMATIC, COM_APPL_CONST) srcVersionInfo,
                                               P2CONST(Std_VersionInfoType, AUTOMATIC, COM_APPL_CONST) destVersionInfo
                                                            );
#endif /* #if(COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */


LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_Receive(void);

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_Send(void);


#ifdef COM_IPDU_WITHOUT_IPDUGROUPREF
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_IpduWoGroupRef(VAR(PduIdType, AUTOMATIC) IpduId);
#endif /* COM_IPDU_WITHOUT_IPDUGROUPREF */

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_TxSignal(VAR(PduIdType, AUTOMATIC) IpduId);

#ifdef COM_TX_SIGNALGROUP
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_TxSignalGroup(VAR(PduIdType, AUTOMATIC) IpduId);

LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_GroupSignal(Com_TxIpduConstPtrType TxIPduConstPtr,
                                                              Com_TxSigGrpConstPtrType TxSigGrpConstPtr);
#endif /* COM_TX_SIGNALGROUP */

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/

/***************************************************************************************************
 Function name    : Com_Init
 Syntax           : void Com_Init(const Com_ConfigType* config)
 Description      : Service for initializing COM module
 Parameter        : config -> Pointer to Com module's configuration data
 Return value     : None
 ***************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"

FUNC(void, COM_CODE) Com_Init(P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_DATA) config)
{

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    Com_RxGwQueuePtrType          l_RxGwQueuePtr_struc;
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */
#ifdef COM_F_ONEEVERYN
    VAR(uint16_least, AUTOMATIC)  Idx_ui;
#endif /* #ifdef COM_F_ONEEVERYN */
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
{
    /* variable to read version info from Com_GetVersionInfo */
    VAR(Std_VersionInfoType, AUTOMATIC) versionInfo_s;
    /* Read the pre-compile version info */
    Com_GetVersionInfo(&versionInfo_s);

# if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Call DET Function to check for the DET Error COM_E_PARAM_POINTER */
    if(Com_Prv_DETCheck_Ptr_Only((config == NULL_PTR), COMServiceId_Init))
#endif /* (COM_CONFIGURATION_USE_DET == STD_ON) */
    {
        /* Compare pre-compile & post-build version info */
        if(Com_CompareVersionInfo(&versionInfo_s, config->versionInfo))
        {
            /* versionInfo doesnt match .. Inform application */
            ComAppl_IncompatibleGenerator();

            /* Abort further initialisation ..Com will remain in COM_UNINIT state */
            return;
        }

        /* MR12 RULE 11.5 VIOLATION: On assignment, target pointer will reference to the memory address, and there are
         * no side effects. Hence suppressed the warning. */
        Com_Prv_Config_pcst = (P2CONST(Com_ConfigData_tst,AUTOMATIC,COM_APPL_CONST))config->Com_ConfigData;
    }
}
#else
    (void)config;
#endif /* (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */
#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    l_RxGwQueuePtr_struc                        = COM_GET_RXGWQUEUE_S;
    l_RxGwQueuePtr_struc->Com_RxGwQueueRead_ux  = COM_ZERO;
    l_RxGwQueuePtr_struc->Com_RxGwQueueWrite_ux = COM_ZERO;
    l_RxGwQueuePtr_struc->RxGwQueuePtr          = &COM_GET_RXGWQUEUE_AUX(0);
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

    rba_BswSrv_MemSet8(&Com_IpduGrpVector[COM_ZERO]   ,(uint32)COM_ZERO,(uint32)COM_NUM_ARR_IPDUGRPVECT);
    rba_BswSrv_MemSet8(&Com_IpduGrpVector_DM[COM_ZERO],(uint32)COM_ZERO,(uint32)COM_NUM_ARR_IPDUGRPVECT);

/* FC_VariationPoint_START */
#if (COM_COMMON_TX_PERIOD == STD_ON)
    /* Reset common IPDU Tx period feature, if active */
    Com_ReducedPeriod_u16 = COM_ZERO;
#endif /* COM_COMMON_TX_PERIOD */
/* FC_VariationPoint_END */

#ifdef COM_F_ONEEVERYN
    /* Initialize RAM structure Com_OneEveryN */
    for (Idx_ui = COM_ZERO; Idx_ui < COM_GET_NUM_SIG_GRPSIG_WITH_ONE_EVERY_N_FILTER; Idx_ui++)
    {
        COM_GET_ONEEVERYN( Idx_ui ).Period     = COM_GET_ONEEVERYN_CONST( Idx_ui ).Period;
        COM_GET_ONEEVERYN( Idx_ui ).Offset     = COM_GET_ONEEVERYN_CONST( Idx_ui ).Offset;
        COM_GET_ONEEVERYN( Idx_ui ).Occurrence = COM_GET_ONEEVERYN_CONST( Idx_ui ).Occurrence;
    }
#endif /* #ifdef COM_F_ONEEVERYN */

    /* Initialize configured Receive Signals, Signal Groups */
    Com_Prv_Init_Receive();
    /* Initialize configured transmission Signals, Signal Groups */
    Com_Prv_Init_Send();

    /* END: Reset all IPDU Buffer with the padding Bytes */
/* FC_VariationPoint_START */
#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
    /* Initial value for all the TxIpdus should be set for IpduControl Via Rba_Nds_EcuVariant feature */
    rba_BswSrv_MemSet8(&Com_TxIpduControlVector[COM_ZERO],(uint32)0xFFu,(uint32)COM_NUM_ARR_TXIPDUCONTROLVECTOR);
#endif /* #ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
    /* Initial value for all the RxIpdus should be set for IpduControl Via Rba_Nds_EcuVariant feature */
    rba_BswSrv_MemSet8(&Com_RxIpduControlVector[COM_ZERO],(uint32)0xFFu,(uint32)COM_NUM_ARR_RXIPDUCONTROLVECTOR);
#endif /* #ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */

/* FC_VariationPoint_END */

    /* Reset the UNINIT flag */
    Com_Uninit_Flag = COM_INIT;

}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/**
 ***************************************************************************************************
 * Com_CompareVersionInfo - compare version info structures ie of type Std_VersionInfoType
 *
 * compare version info structures ie of type Std_VersionInfoType.
 *
 * \param   srcVersionInfo      pointer to first Std_VersionInfoType structure
 * \param   destVersionInfo     pointer to second Std_VersionInfoType structure
 * \return  stEqual_u32         result of comparison
 * \retval  0                   memory area contents are equal
 * \retval  1                   memory area contents differ
  ***************************************************************************************************
 */
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
LOCAL_INLINE FUNC(boolean, COM_CODE) Com_CompareVersionInfo(
                  P2CONST(Std_VersionInfoType, AUTOMATIC, COM_APPL_CONST) srcVersionInfo,
                  P2CONST(Std_VersionInfoType, AUTOMATIC, COM_APPL_CONST) destVersionInfo
                                                           )
{
    VAR(boolean, AUTOMATIC)   stEqual_b;
    stEqual_b = COM_TRUE;

    if ((srcVersionInfo->vendorID == destVersionInfo->vendorID) &&
        (srcVersionInfo->moduleID == destVersionInfo->moduleID) &&
        (srcVersionInfo->sw_major_version == destVersionInfo->sw_major_version) &&
        (srcVersionInfo->sw_minor_version == destVersionInfo->sw_minor_version) &&
        (srcVersionInfo->sw_patch_version == destVersionInfo->sw_patch_version)
    )
    {
        stEqual_b = COM_FALSE;
    }
    return stEqual_b;
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

/***************************************************************************************************
 Function name    : Com_Prv_Init_Receive
 Syntax           : void Com_Prv_Init_Receive(void)
 Description      : Service to initialize all configured signals, signalGroup at the receiver side
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_Receive(void)
{

    Com_RxIpduRamPtrType          RxIPduRamPtr; /* Local pointer to hold the address of Rx ipdu status structure */
    Com_RxIpduConstPtrType        RxIPduConstPtr; /*Local pointer to hold the address of Rx ipdu static configuration*/
    Com_RxSigConstPtrType         RxSigConstPtr; /*Local pointer to hold the address of Rx signal static configuration*/
    VAR(uint16_least, AUTOMATIC)  Idx_ui;
    VAR(uint16_least, AUTOMATIC)  Idx_ByteNSig_ui;

#ifdef COM_RX_SIGNALGROUP
    Com_RxSigGrpConstPtrType      RxSigGrpConstPtr; /* Local pointer to Rx GroupSignal configuration */
    VAR(uint16_least, AUTOMATIC)  Idx_SigGrp_ui;    /* Signal Group Id */
#endif /* #ifdef COM_RX_SIGNALGROUP */

    Com_RxSigRamPtrType                 RxSigRamPtr;
    VAR(Com_SigMaxType,AUTOMATIC)       RxSigVal;
    VAR(Com_MainFuncType,AUTOMATIC)     rxMainFuncId;
#ifdef COM_RxFilters
# ifdef COM_F_MASKEDNEWDIFFERSOLD
    VAR(uint8, AUTOMATIC)               ConstByteValue_u8;
# endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */
#endif
#if defined( COM_RXSIG_INT64 ) || defined( COM_RXSIG_FLOAT64SUPP )
    VAR(uint8, AUTOMATIC)               Type;
#endif

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
    Com_RxGwQueuePtrType          l_RxGwQueuePtr_struc;

    l_RxGwQueuePtr_struc                        = COM_GET_RXGWQUEUE_S;
    l_RxGwQueuePtr_struc->Com_RxGwQueueRead_ux  = COM_ZERO;
    l_RxGwQueuePtr_struc->Com_RxGwQueueWrite_ux = COM_ZERO;
    l_RxGwQueuePtr_struc->RxGwQueuePtr          = &COM_GET_RXGWQUEUE_AUX(0);
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

    /* Start: Reset all Rx Flags */
    RxIPduRamPtr   = &COM_GET_RXPDURAM_S(0);

    RxIPduConstPtr = COM_GET_RX_IPDU_CONSTDATA(0);

    for(Idx_ui = COM_ZERO; Idx_ui < COM_GET_NUM_RX_IPDU; Idx_ui++)
    {

        COM_GET_IPDUCOUNTER_S(Idx_ui)  = (uint8)COM_ZERO;
        COM_GET_IPDUCOUNTER_DM(Idx_ui) = (uint8)COM_ZERO;

        /*Reset all the Rx Ipdu flags*/
        RxIPduRamPtr ->RxFlags      = (uint8)COM_ZERO;
        RxIPduRamPtr ->RxIPduLength = COM_ZERO;

        /* Here the Ipdu entry is popped out of the queue,The flag is reset to capture the changes in Com_RxIndication*/
        Com_SetRamValue(RXIPDU,_GWPDU_TOBEPROCESSED,RxIPduRamPtr->RxFlags,COM_FALSE);

        /* AUTOSAR COM shall initialize all signals on receiver side with the configured init values */
        RxSigRamPtr     = &COM_GET_RXSIGNALFLAG(RxIPduConstPtr->RxSigId_Index);
        RxSigConstPtr   = COM_GET_RXSIG_CONSTDATA(RxIPduConstPtr->RxSigId_Index);

        /* Fetch the Rx-MainFunction internal Id */
        rxMainFuncId    = RxIPduConstPtr->MainFunctionRef;

        for(Idx_ByteNSig_ui  = (uint16_least)(RxIPduConstPtr->No_Of_Sig_Ref);Idx_ByteNSig_ui != (uint16_least)COM_ZERO; Idx_ByteNSig_ui-- )
        {
            /* Reset all the flags in the rxSigRAMFields */
            RxSigRamPtr->rxSigRAMFields = (uint8)COM_ZERO;
            /* Get the init value of the signal */
            /* Sign extension to be performed on Init value
             * But when a signed value is assigned to a uint32 variable,it is sign extended by default */
            RxSigVal    = (Com_SigMaxType)RxSigConstPtr->Init_Val;

#if defined( COM_RXSIG_INT64 ) || defined( COM_RXSIG_FLOAT64SUPP )
            /* Get the signal type */
            Type        = Com_GetValue(GEN,_TYPE,RxSigConstPtr->General);
#endif /* #if defined( COM_RXSIG_INT64 ) || defined( COM_RXSIG_FLOAT64SUPP ) */

#ifdef COM_RXSIG_INT64
            /* [SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
             * to the size of its ComSignalType.
             * Init_Val by default will be sign extended only for 32bits. Currently for COM_SINT64 signal types,
             * if the signal is of Negative, it is required to perform the sign extension till 64bits explicitly. */
            if ( Type == COM_SINT64 )
            {
                RxSigVal = (Com_SigMaxType)((sint64)((sint32)RxSigVal));
            }
            else
            {
                /* do nothing for other signal types */
            }
#endif /* #ifdef COM_RXSIG_INT64 */

    /* FC_VariationPoint_START */
#ifdef COM_RXSIG_FLOAT64SUPP

            if(Type == COM_FLOAT64)
            {
                /* For float64 signals, RxSigConstPtr->Init_Val holds index to float array holding init values.*/
                COM_GET_SIGTYPEFLOAT64_BUFF(rxMainFuncId,RxSigConstPtr->SigBuff_Index) =
                                                         Com_getfloat64InitValue(RxSigVal);
            }
            else
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */
    /* FC_VariationPoint_END */
            {
                Com_UpdateRxSignalBuffer(RxSigConstPtr, RxSigVal, rxMainFuncId);
            }

#ifdef COM_RxFilters
# ifdef COM_F_MASKEDNEWDIFFERSOLD

            ConstByteValue_u8 = RxSigConstPtr->rxSignalFields;
            /* Check the filter algorithm */
            if(Com_GetValue(RXSIG,_FILTRALG,ConstByteValue_u8) == (uint8)COM_MASKED_NEW_DIFFERS_MASKED_OLD)
            {
                /* Load the new received value as old value */
                COM_GET_F_OLDVALUE(RxSigConstPtr->Filter_Index) = (uint32)RxSigVal;
            }
# endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */
#endif /* #ifdef COM_RxFilters */
            RxSigRamPtr++;
            RxSigConstPtr++;
        }

#ifdef COM_RX_SIGNALGROUP
        {

            Com_RxSigGrpRamPtrType RxSigGrpRamPtr;    /* Local RAM pointer to SignalGroup RAM structure */
            RxSigGrpRamPtr = &COM_GET_RXSIGGRPFLAG(RxIPduConstPtr->FirstRxSigGrp_Index);

            /* AUTOSAR COM shall initialize all Rx-SignalGroup Array(if any) and Secondary GroupSignal Buffers
             * on receiver side with the configured init values */

            RxSigGrpConstPtr = COM_GET_RXSIGGRP_CONSTDATA( RxIPduConstPtr->FirstRxSigGrp_Index );

            for (Idx_SigGrp_ui  = (uint16_least)RxIPduConstPtr->No_Of_SigGrp_Ref;Idx_SigGrp_ui != (uint16_least)COM_ZERO;Idx_SigGrp_ui-- )
            {
                Com_Prv_SigGrpReplaceToInit( RxSigGrpConstPtr, rxMainFuncId );
                /* Reset all the flags in the rxSigGrpRAMFields */
                RxSigGrpRamPtr->rxSigGrpRAMFields = (uint8)COM_ZERO;

                RxSigGrpRamPtr++;
                RxSigGrpConstPtr++;
            }
        }
#endif  /* #ifdef COM_RX_SIGNALGROUP */

        /* As per this requirement there is no need of updating the Rx Shadowbuffer */
        RxIPduRamPtr++;
        RxIPduConstPtr++;
    }
    /* End: Reset all Rx Falgs */
}


/***************************************************************************************************
 Function name    : Com_Prv_Init_Send
 Syntax           : void Com_Prv_Init_Send(void)
 Description      : Service to initialize all configured signals, signalGroup at the sender side
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_Send(void)
{

    Com_TxIpduRamPtrType           TxIpduRamPtr;   /* Local ptr to hold the address of Tx ipdu status structure */
    Com_TxIpduConstPtrType         TxIPduConstPtr; /* Local ptr to hold the address of Tx ipdu static configuration */
    VAR(uint16, AUTOMATIC)         Idx_ui;

    /* START: Reset all IPDU Buffer with the padding Bytes */
    /*Update the bytes with Initial Value*/
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(0);

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(0);

    for(Idx_ui = COM_ZERO; Idx_ui < COM_GET_NUM_TX_IPDU; Idx_ui++)
    {
        /*Reset all the Tx Flags*/
        TxIpduRamPtr ->Com_TxFlags      = (uint16)COM_ZERO;
#ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT
        TxIpduRamPtr->Com_DynLength     = COM_ZERO;
#endif /* #ifdef COM_TX_DYNAMIC_SIGNAL_SUPPORT */
        TxIpduRamPtr->CurrentTxModePtr  = TxIPduConstPtr->Com_TMConstPtr;

        /*Update the IPDU buffer with Padding Bytes/Unused Area default*/
        rba_BswSrv_MemSet8((TxIPduConstPtr->BuffPtr),(uint32)TxIPduConstPtr->PaddingByte, (uint32)TxIPduConstPtr->Size);

        /* Start: Reset all Tx Flags */
        if(Com_GetValue( TXIPDU,_PDUWITHPDUGRP,TxIPduConstPtr->txIPduFields ) != COM_FALSE)
        {
            COM_GET_IPDUCOUNTER_S(Idx_ui + COM_GET_NUM_RX_IPDU) = COM_ZERO;

            /* Set the TxIPdu-Mode to most probable case */
            Com_SetRamValue(TXIPDU,_TXMODESTATE,TxIpduRamPtr->Com_TransMode,COM_TRUE);

            Com_SetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode,(uint8)COM_TXMODE_NONE);
            /* FC_VariationPoint_START */
            #ifdef COM_MIXEDPHASESHIFT
            /* Disable MixedPhaseShift by default */
            Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,TxIpduRamPtr->Com_TransMode,COM_FALSE);
            #endif /* #ifdef COM_MIXEDPHASESHIFT */
            /* FC_VariationPoint_END */
        }
#ifdef COM_IPDU_WITHOUT_IPDUGROUPREF
        else
        {
            /* Call function to initialize Ipdu which does not belongs to any IpduGroup */
            Com_Prv_Init_IpduWoGroupRef(Idx_ui);
        }
#endif /* #ifdef COM_IPDU_WITHOUT_IPDUGROUPREF */


        /*Reset all other Tx Global variables including Min Delays*/
#ifdef COM_TxIPduTimeOut
        TxIpduRamPtr->Com_TickTxTimeout     = COM_ZERO;
#endif /* #ifdef COM_TxIPduTimeOut */
#ifdef COM_TxFilters
        TxIpduRamPtr->Com_TMSTrueCounter_u8 = COM_ZERO;
#endif /* #ifdef COM_TxFilters */
        TxIpduRamPtr->Com_n                 = COM_ZERO;

        TxIpduRamPtr->Com_MinDelayTick      = COM_ZERO; /*Minimum Delay Tick for the Tx IPDU's */

        /* Call function to initialize configured transmission signals */
        Com_Prv_Init_TxSignal(Idx_ui);

#ifdef COM_TX_SIGNALGROUP
        /* Call function to initialize configured transmission signalgroup */
        Com_Prv_Init_TxSignalGroup(Idx_ui);
#endif

        TxIPduConstPtr++;
        TxIpduRamPtr++;

    } /*end of for(Idx_ui=0;Idx_ui<COM_GET_NUM_TX_IPDU;Idx_ui++)*/
}


#ifdef COM_IPDU_WITHOUT_IPDUGROUPREF

/***************************************************************************************************
 Function name    : Com_Prv_Init_IpduWoGroupRef
 Syntax           : void Com_Prv_Init_IpduWoGroupRef(PduIdType IpduId)
 Description      : Service for initializing COM module for the transmission
 Parameter        : IpduId         -> Index of the Ipdu
 Return value     : None
 ***************************************************************************************************/
/* HIS METRIC CALLS VIOLATION IN Com_Prv_Init_IpduWoGroupRef:
   Com_Prv_Init_IpduWoGroupRef function contains necessary function calls statements.
   HIS metric compliance would decrease readability and maintainability */
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_IpduWoGroupRef(VAR(PduIdType, AUTOMATIC) IpduId)
{

    /* Variable declaration */
    Com_TxIpduRamPtrType        TxIpduRamPtr;
    Com_TxIpduConstPtrType      TxIPduConstPtr;
    VAR(boolean, AUTOMATIC)     DefaultTMSStatus;

    /*Update the bytes with Initial Value*/
    TxIpduRamPtr   = &COM_GET_TXPDURAM_S(IpduId);

    TxIPduConstPtr = COM_GET_TX_IPDU_CONSTDATA(IpduId);

    /* Fetch the default TMS status which is evaluated during the code generation time
     * for the ComIpdu's which are not part of any ComIpduGroup  */
    DefaultTMSStatus = Com_GetValue(TXIPDU,_DEFAULTTMSSTATUS,TxIPduConstPtr->txIPduFields);

    /* [SWS_Com_00840] If an I-PDU is not assigned to any I-PDU group, the I-PDU shall be initially started
     * (and never been stopped) */
    COM_GET_IPDUCOUNTER_S(IpduId + COM_GET_NUM_RX_IPDU) = COM_ONE;

    Com_SetRamValue(TXIPDU,_PDUSTATUS,TxIpduRamPtr->Com_TxFlags,COM_START);

    /* [SWS_Com_00877]: If an I-PDU is not part of any I-PDU Group, it is started during the initialization of COM.
     * Its starting transmission mode shall be evaluated according to the
     * ComSignalInitValue of the signals contributing to its TMS.
     * Set the Tx Mode state as DefaultTMSStatus. */
    Com_SetRamValue(TXIPDU,_TXMODESTATE,TxIpduRamPtr->Com_TransMode,DefaultTMSStatus);
    /* Set accordingly the TxModeMode as per DefaultTMSStatus */
    Com_SetCurrentTxModePtr(TxIpduRamPtr,TxIPduConstPtr,Com_GetValue(TXIPDU,_TMS,TxIPduConstPtr->txIPduFields),DefaultTMSStatus);
    /* Set the latest mode as per current mode */
    Com_SetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode,TxIpduRamPtr->CurrentTxModePtr->Mode);
    /* FC_VariationPoint_START */

    /* FC_VariationPoint_END */
    if(COM_RESET != (Com_GetRamValue(TXIPDU,_LATESTMODE,TxIpduRamPtr->Com_TransMode) & COM_TXMODE_MIXED))
    {
        /* I.e Load the TxTimeOffset for cyclic type Pdu's(PERIODIC or MIXED) */
        TxIpduRamPtr->Com_Tick_Tx = COM_GET_TXIPDU_TIMEOFFSET(IpduId, TxIpduRamPtr->CurrentTxModePtr);
#ifdef COM_MIXEDPHASESHIFT
        /* Set the MixedPhaseShift Mode */
        Com_SetRamValue(TXIPDU,_MIXEDPHASESHIFT,TxIpduRamPtr->Com_TransMode,TxIpduRamPtr->CurrentTxModePtr->MixedPhaseShift);
#endif /* #ifdef COM_MIXEDPHASESHIFT */
    }
}
#endif /* #ifdef COM_IPDU_WITHOUT_IPDUGROUPREF */


/***************************************************************************************************
 Function name    : Com_Prv_Init_TxSignal
 Syntax           : void Com_Prv_Init_TxSignal(VAR(PduIdType, AUTOMATIC) IpduId)
 Description      : Service to initialize all configured signals at the sender side
 Parameter        : IpduId -> Index of the Ipdu
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_TxSignal(VAR(PduIdType, AUTOMATIC) IpduId)
{

    Com_TxIpduConstPtrType         TxIPduConstPtr; /* Local ptr to hold the address of Tx ipdu static configuration */
    Com_TxSigConstPtrType          TxSigConstPtr;  /* Local ptr to hold the address of Tx signal static configuration*/
    VAR(Com_SigMaxType, AUTOMATIC) TxSigNewVal;
#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) || defined(COM_EffectiveSigTOC)
    /* Variable to hold the signal property */
    VAR(uint16, AUTOMATIC)         ConstByteValue_u16;
#endif /* #if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) || defined(COM_EffectiveSigTOC) */
#ifdef COM_EffectiveSigTOC
    /* Variable to hold the Transfer Property of Signal */
    VAR(uint16, AUTOMATIC)         TxSigTp_u16;
#endif /* #ifdef COM_EffectiveSigTOC */

    VAR(uint16_least, AUTOMATIC)   Idx_ByteNSig_ui;

#ifdef COM_TxSigUpdateBit
    VAR(PduLengthType, AUTOMATIC)  ByteNo_u8;
#endif /* #ifdef COM_TxSigUpdateBit */
    VAR(uint8, AUTOMATIC)          ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)          Type;

    TxIPduConstPtr      = COM_GET_TX_IPDU_CONSTDATA(IpduId);

    /* AUTOSAR COM shall initialize all signals on sender side with
    the configured init values */
    TxSigConstPtr       = COM_GET_TXSIG_CONSTDATA(TxIPduConstPtr->TxSigId_Index);

    for(Idx_ByteNSig_ui = TxIPduConstPtr->No_Of_Sig_Ref; Idx_ByteNSig_ui != COM_ZERO; Idx_ByteNSig_ui--)
    {

        ConstByteValue_u8   = TxSigConstPtr->General;
        /* Get the signal type */
        Type                = Com_GetValue(GEN,_TYPE,ConstByteValue_u8);

        /* Sign extension to be performed on Init value
        * But when a signed value is assigned to a uint32 variable,it is sign extended by default*/
        TxSigNewVal         = (Com_SigMaxType)TxSigConstPtr->Init_Val;

#ifdef COM_TXSIG_INT64
        /* [SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a signal
        * to the size of its ComSignalType.
        * Currently for COM_SINT64 signal type, ComSignalInitValue is supported for maximum 32bits.
        * If negative values are configured, by default these values will be sign extended only till 32bits.
        * so, for COM_SINT64, it is required to perform the sign extension till 64bits explicitly. */
        if ( Type == COM_SINT64 )
        {
            TxSigNewVal = (Com_SigMaxType)((sint64)((sint32)TxSigNewVal));
        }
        else
        {
            /* do nothing for other signal types */
        }
#endif /* #ifdef COM_TXSIG_INT64 */

        Com_InitializePduBuffWithSignalInitValue( TxIPduConstPtr->BuffPtr,
                                                  TxSigNewVal,
                                                  TxSigConstPtr->Bit_Pos,
                                                  TxSigConstPtr->BitSize, Type,
                                                  Com_GetValue(GEN,_ENDIANESS,ConstByteValue_u8)  );

#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) || defined(COM_EffectiveSigTOC)
        /* Load the signal property to variable */
        ConstByteValue_u16 = TxSigConstPtr->txSignalFields;
#endif /* #if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) || defined(COM_EffectiveSigTOC) */
#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD))
        /* The old value of the filter is set to Initial value */
        if(Com_GetValue(TXSIG,_FILTRALG,ConstByteValue_u16) == (uint16)COM_MASKED_NEW_DIFFERS_MASKED_OLD)
        {
            COM_GET_F_OLDVALUE(TxSigConstPtr->Filter_Index) = (uint32)TxSigNewVal;
        }
#endif /* #if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) */

#ifdef COM_EffectiveSigTOC
        /* Get Signal Transfer Property */
        TxSigTp_u16 = Com_GetValue(TXSIG,_TP,ConstByteValue_u16);
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
        * HIS-LEVEL parameter will be violated if more conditional statements are added.
        * Hence the warning is suppressed. */

        /* Assign Init value as Old Value, if Signal's Transfer property is TriggeredOnChange or
         * TriggeredOnChangeWithoutRepetition */
        if (COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(TxSigTp_u16))
        {
            COM_GET_OLDVALTRIGONCHANGE(TxSigConstPtr->OldValue_Index) = (Com_OldValTrigOnChngType)TxSigNewVal;
        }
#endif /* #ifdef COM_EffectiveSigTOC */

#ifdef COM_TxSigUpdateBit
        /* Only the first byte of txIPduFields is needed */
        ConstByteValue_u8 = (uint8)TxIPduConstPtr->txIPduFields;
        /* Check the clear update bit */
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
        * HIS-LEVEL parameter will be violated if more conditional statements are added.
        * Hence the warning is suppressed. */
        if((Com_GetValue(TXIPDU,_CLRUPDBIT,ConstByteValue_u8) != (uint16)COM_CLRUPDATEBIT_NONE) &&
                (Com_GetValue(GEN,_UPDBITCONF,TxSigConstPtr->General)))
        {
            /*Find the Byte No*/
            ByteNo_u8 = (PduLengthType)(TxSigConstPtr->Update_Bit_Pos>>3u);

            /*Update the Bit in IPDU Buffer*/
            Com_ClearUpdateBitValue(TxIPduConstPtr, TxSigConstPtr->Update_Bit_Pos, ByteNo_u8)
        }
#endif/*COM_TxSigUpdateBit*/
        TxSigConstPtr++;
    }
}

#ifdef COM_TX_SIGNALGROUP
/***************************************************************************************************
 Function name    : Com_Prv_Init_TxSignalGroup
 Syntax           : void Com_Prv_Init_TxSignalGroup(VAR(PduIdType, AUTOMATIC) IpduId)
 Description      : Service to initialize all configured signal Group at the sender side
 Parameter        : IpduId  -> Index of the Ipdu
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE)   Com_Prv_Init_TxSignalGroup(VAR(PduIdType, AUTOMATIC) IpduId)
{

    Com_TxIpduConstPtrType         TxIPduConstPtr; /* Local ptr to hold the address of Tx ipdu static configuration */
    Com_TxSigGrpConstPtrType       TxSigGrpConstPtr;
    VAR(uint16_least, AUTOMATIC)   Idx_ByteNSig_ui;
    VAR(uint16_least, AUTOMATIC)   Idx_SigGrp_ui;
    VAR(uint16_least, AUTOMATIC)    FirstTxSigGrp_Index;
#if defined (COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit)
    VAR(PduLengthType, AUTOMATIC)  ByteNo_u8;
#endif /* #if defined (COM_TxSigUpdateBit) || defined(COM_TxSigGrpUpdateBit) */
#ifdef COM_EffectiveSigGrpTOC
    /* Variable to hold the Transfer Property of Signal Group */
    VAR(uint8, AUTOMATIC)          TxSigGrpTp_u8;
#endif /* #ifdef COM_EffectiveSigGrpTOC */
    VAR(uint8, AUTOMATIC)          ConstByteValue_u8;
#ifdef COM_TX_SIGNALGROUP_ARRAY
    boolean                        ConstIsSigGrpOfArrayType_b;
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */

    TxIPduConstPtr      = COM_GET_TX_IPDU_CONSTDATA(IpduId);

    FirstTxSigGrp_Index = TxIPduConstPtr->FirstTxSigGrp_Index;

    Idx_ByteNSig_ui     = (uint16_least)FirstTxSigGrp_Index + (uint16_least)TxIPduConstPtr->No_Of_SigGrp_Ref;

    TxSigGrpConstPtr    = COM_GET_TXSIGGRP_CONSTDATA(FirstTxSigGrp_Index);

    for(Idx_SigGrp_ui   = (uint16_least)FirstTxSigGrp_Index; Idx_SigGrp_ui < Idx_ByteNSig_ui;
                             Idx_SigGrp_ui++ )
    {

#ifdef COM_TX_SIGNALGROUP_ARRAY
        ConstIsSigGrpOfArrayType_b = Com_GetValue(TXSIGGRP,_ARRAYACCESS,TxSigGrpConstPtr->txSignalGrpFields);
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */

#ifdef COM_SIGNALGROUPGATEWAY
        /* Reset the SignalGroupGateway flag */
        Com_SetRamValue(TXSIGGRP,_ISGWUPDTD,COM_GET_TXSIGGRPFLAG(Idx_SigGrp_ui).txSigGrpRAMFields, COM_FALSE);
#endif /* #ifdef COM_SIGNALGROUPGATEWAY */

        /* Call function to initialize configured transmission group signals */
        Com_Prv_Init_GroupSignal(TxIPduConstPtr, TxSigGrpConstPtr);

#ifdef COM_EffectiveSigGrpTOC
        /* Get the SignalGroup property */
        TxSigGrpTp_u8 = Com_GetValue(TXSIGGRP,_TP,TxSigGrpConstPtr->txSignalGrpFields);
        /* If SignalGroups TP is TriggeredOnChange or TriggeredOnChangeWithoutRepetition set the RAM value */
        if(COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(TxSigGrpTp_u8))
        {
            /* Reset TriggerredOnChange flag */
            Com_SetRamValue(TXSIGGRP,_TRIGONCHNG,COM_GET_TXSIGGRPFLAG(Idx_SigGrp_ui).txSigGrpRAMFields,COM_FALSE);
        }
#endif /* #ifdef COM_EffectiveSigGrpTOC */
#ifdef COM_TX_SIGNALGROUP_ARRAY
        /* Check if SignalGroupArray is enabled to copy the data to IPDU buffer else, do nothing
         * GroupSignal's SignalInitValues are already updated into the IPduBuffer in
         * Com_InitializePduBuffWithSignalInitValue() */
        if ( ConstIsSigGrpOfArrayType_b != COM_TRUE )
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */
        {
            /* Copy the data which is present in Shadow buffer to IPDU buffer */
            Com_ShadowBuffToIpduBuff((Com_SignalGroupIdType)Idx_SigGrp_ui);
        }

#ifdef COM_TxSigGrpUpdateBit
        /* Only the first byte of txIPduFields is needed */
        ConstByteValue_u8 = (uint8)TxIPduConstPtr->txIPduFields;
        /* Clear Update bits*/
        /* MR12 RULE 13.5 VIOLATION: Below conditional statement reads signal field status.
        * HIS-LEVEL parameter will be violated if more conditional statements are added.
        * Hence the warning is suppressed. */
        if((Com_GetValue(TXIPDU,_CLRUPDBIT,ConstByteValue_u8) != (uint16)COM_CLRUPDATEBIT_NONE) &&
                (Com_GetValue(TXSIGGRP,_UPDBITCONF,TxSigGrpConstPtr->txSignalGrpFields)))
        {
            /*Find the Byte No*/
            ByteNo_u8 = (PduLengthType)(TxSigGrpConstPtr ->Update_Bit_Pos>>3u);

            /*Update the Bit in IPDU Buffer*/
            Com_ClearUpdateBitValue(TxIPduConstPtr, TxSigGrpConstPtr->Update_Bit_Pos, ByteNo_u8)
        }
#endif  /*COM_TxSigGrpUpdateBit*/
        TxSigGrpConstPtr++;
    }
}


/***************************************************************************************************
 Function name    : Com_Prv_Init_GroupSignal
 Syntax           : void Com_Prv_Init_GroupSignal(Com_TxIpduConstPtrType TxIPduConstPtr,
                                                              Com_TxSigGrpConstPtrType TxSigGrpConstPtr)
 Description      : Service to initialize all configured Groupsignal at the sender side
 Parameter        : TxIPduConstPtr      -> Address to Tx IPdu
                    TxSigGrpConstPtr    -> Address to SignalGroup
 Return value     : None
 ***************************************************************************************************/
LOCAL_INLINE FUNC(void, COM_CODE) Com_Prv_Init_GroupSignal(Com_TxIpduConstPtrType TxIPduConstPtr,
                                                              Com_TxSigGrpConstPtrType TxSigGrpConstPtr)
{
    /* Variable declaration */
    Com_TxGrpSigConstPtrType        TxGrpSigConstPtr;
    VAR(Com_SigMaxType, AUTOMATIC)  TxGrpSig_NewVal;
    VAR(uint16_least, AUTOMATIC)    Idx_TxGrpSig_ui;
    VAR(uint8, AUTOMATIC)           ConstByteValue_u8;
    VAR(uint8, AUTOMATIC)           Type;
    VAR(Com_MainFuncType,AUTOMATIC) txMainFuncId;
#ifdef COM_TX_SIGNALGROUP_ARRAY
    boolean                         ConstIsSigGrpOfArrayType_b;
    ConstIsSigGrpOfArrayType_b = Com_GetValue(TXSIGGRP,_ARRAYACCESS,TxSigGrpConstPtr->txSignalGrpFields);
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */

    txMainFuncId        = TxIPduConstPtr->MainFunctionRef;

    TxGrpSigConstPtr    = COM_GET_TXGRPSIG_CONSTDATA(TxSigGrpConstPtr-> FirstGrpSig_Index);

    for( Idx_TxGrpSig_ui  = (uint16_least)COM_ZERO; Idx_TxGrpSig_ui < (uint16_least)TxSigGrpConstPtr->No_Of_GrpSig;
                                                                          Idx_TxGrpSig_ui++)
    {
        /* Shadowbuffer of a signal Group(group signal buffers) on sender-side shall be initialized*/
        /* Sign extension to be performed on Init valu, when a signed value is assigned to a uint32 variable,
         * it is sign extended by default*/
        TxGrpSig_NewVal     = (Com_SigMaxType)TxGrpSigConstPtr->Init_Val;

        /* Get the value of the ComSignaltype bit field */
        ConstByteValue_u8   = TxGrpSigConstPtr->txGrpSigFields;
        /* Get type of the signal */
        Type = Com_GetValue(TXGRPSIG,_TYPE,ConstByteValue_u8);

#ifdef COM_TXGRPSIG_INT64
        /* [SWS_Com_00723] The AUTOSAR COM module shall extend the init value (ComSignalInitValue) of a
         * signal to the size of its ComSignalType.
         * Currently for COM_SINT64 signal type, ComSignalInitValue is supported for maximum 32bits.
         * If negative values are configured, by default these values will be sign extended only till 32bits.
         * so, for COM_SINT64, it is required to perform the sign extension till 64bits explicitly. */
        if ( Type == COM_SINT64 )
        {
            TxGrpSig_NewVal = (Com_SigMaxType)((sint64)((sint32)TxGrpSig_NewVal));
        }
        else
        {
            /* do nothing for other signal types */
        }
#endif /* #ifdef COM_TXGRPSIG_INT64 */

#ifdef COM_TX_SIGNALGROUP_ARRAY
        if ( ConstIsSigGrpOfArrayType_b )
        {
            Com_InitializePduBuffWithSignalInitValue( TxIPduConstPtr->BuffPtr,
                                                      TxGrpSig_NewVal,
                                                      TxGrpSigConstPtr->Bit_Pos,
                                                      TxGrpSigConstPtr->BitSize, Type,
                                                      Com_GetValue(TXGRPSIG,_ENDIANESS,ConstByteValue_u8) );
        }
        else
#endif /* #ifdef COM_TX_SIGNALGROUP_ARRAY */
#ifdef COM_TXGRPSIG_FLOAT64SUPP
        if(Type == COM_FLOAT64)
        {
            /* FLOAT64 signal */
            /* For float64 signals, TxGrpSig_NewVal holds index to float array holding init values.*/
            COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index) =
                                                                             Com_getfloat64InitValue(TxGrpSig_NewVal);
        }
        else
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */
        if(Type == COM_UINT8_N) /* UINT8_N signal */
        {
            Com_ByteCopyInit(&COM_GET_TXGRPSIGTYPEU8_BUFF(txMainFuncId, TxGrpSigConstPtr->TxGrpSigBuff_Index),
                            (uint32)TxGrpSig_NewVal,(uint32)TxGrpSigConstPtr->BitSize);
        }
        else
        {
            Com_CopyTxGroupSignalData(
                ((Com_SignalIdType)(TxSigGrpConstPtr->FirstGrpSig_Index) + (Com_TxIntGrpSignalIdType)Idx_TxGrpSig_ui),
                TxGrpSig_NewVal, txMainFuncId
                                     );
        }

        /*COM_230: The old value of the filter is set to Initial value */
#if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD))
        if(TxGrpSigConstPtr->Filter_Algo == (uint8)COM_MASKED_NEW_DIFFERS_MASKED_OLD)
        {
            COM_GET_F_OLDVALUE(TxGrpSigConstPtr->Filter_Index) = (uint32)TxGrpSig_NewVal;
        }
#endif /* #if (defined(COM_TxFilters) && defined(COM_F_MASKEDNEWDIFFERSOLD)) */
#ifdef COM_EffectiveSigGrpTOC
        /* Get transfer property of the group signal */
        if(Com_GetValue(TXGRPSIG,_TP,ConstByteValue_u8))
        {
            COM_GET_OLDVALTRIGONCHANGE(TxGrpSigConstPtr->OldValue_Index) = (Com_OldValTrigOnChngType)TxGrpSig_NewVal;
        }
#endif /* #ifdef COM_EffectiveSigGrpTOC */
        TxGrpSigConstPtr++;
    }
}
#endif  /*COM_TX_SIGNALGROUP*/

