

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Inl.h"
#include "CanIf.h"
#if (!defined(CANIF_AR_RELEASE_MAJOR_VERSION) || (CANIF_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANIF_AR_RELEASE_MINOR_VERSION) || (CANIF_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
static FUNC(void, CANNM_CODE) CanNm_InternalMainProcess(CONST(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle);
static FUNC(void, CANNM_CODE) CanNm_MainFunctionTx(CONST(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle);
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
LOCAL_INLINE FUNC(void, CANNM_CODE) CanNm_MainProcess_EIRA(void);
#endif

#if (CANNM_ERACALC_ENABLED != STD_OFF)
LOCAL_INLINE FUNC(void, CANNM_CODE) CanNm_MainProcess_ERA(VAR(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle);
#endif

/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/
/***************************************************************************************************
 Function name    : CanNm_MainFunction
 Syntax           : void CanNm_MainFunction( void )
 Description      : This is an autosar specific API describing the main function of CAN NM
                    This calls channel specific main Function for each channel with the proper
                    handle and raster period
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

FUNC(void, CANNM_CODE) CanNm_MainFunction( void )
{
    VAR(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle;

    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) ConfigPtr_pcs;

    for(CanNm_NetworkHandle = 0; CanNm_NetworkHandle < CANNM_NUMBER_OF_CHANNELS; CanNm_NetworkHandle++)
    {
        /* Set the pointer to configuration structure of channel CanNm_NetworkHandle */
        ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

        CanNm_InternalMainProcess(CanNm_NetworkHandle);

#if (CANNM_ERACALC_ENABLED != STD_OFF)
        if(ConfigPtr_pcs->EraCalcStatus_b != FALSE)
        {
            CanNm_MainProcess_ERA(CanNm_NetworkHandle);
        }
#else
     (void)ConfigPtr_pcs;
#endif
     }

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
    CanNm_MainProcess_EIRA();
#endif

}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


/***************************************************************************************************
 Function name    : CanNm_InternalMainProcess
 Syntax           : void CanNm_InternalMainProcess( const NetworkHandleType CanNm_NetworkHandle )
 Description      : This is an internal main function of CANNM which does state machine processing
                    for all channels.
 Parameter        : CanNm_NetworkHandle - Identification of the CANNM-channel
 Return value     : None
 ***************************************************************************************************/

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

static FUNC(void, CANNM_CODE) CanNm_InternalMainProcess (CONST(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle)

{
    /* Pointer to configuration data */
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) ConfigPtr_pcs;

    /* Pointer to RAM data */
    P2VAR(CanNm_RamType, AUTOMATIC, CANNM_APPL_DATA) RamPtr_ps;

    /* To store local copy of Rx Indication flag */
    VAR(boolean, AUTOMATIC) PduRxInd_b;

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* To store local copy of Tx confirmation flag */
    VAR(boolean, AUTOMATIC) PduTxConfirmation_b;
#endif

#if ((CANNM_NODE_DETECTION_ENABLED != STD_OFF)||(CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF))
    /* To store local copy of received Control Bit Vector */
    VAR(uint8, AUTOMATIC) CtrlBitVector_u8;
#endif

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* To store Network Request state */
    VAR(CanNm_NetworkStateType, AUTOMATIC) stNetworkState_e;
#endif

    /* To store local copy of CanNmRepeatMessageTime [to reduce run-time] */
    VAR(CanNm_TimerType, AUTOMATIC) RepeatMsgTime;

    /* To store local copy of CanNm state [to reduce run-time] */
    VAR(Nm_StateType, AUTOMATIC) StateCopy_e;

    /**** End Of Declarations ****/

    /* Set the pointer to configuration structure of channel CanNm_NetworkHandle */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /********************************* Start: DET *************************************/

    /* Report DET if CANNM is uninitialized */
    CANNM_DET_REPORT_ERROR((CanNm_RamData_s[CanNm_NetworkHandle].State_en == NM_STATE_UNINIT),
            ConfigPtr_pcs->NetworkHandle, CANNM_SID_MAINFUNCTION, CANNM_E_NO_INIT)

    /*********************************  End: DET  *************************************/

    /* Set the pointer to RAM structure of Channel CanNm_NetworkHandle */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Store a local copy of CanNm state */
    StateCopy_e = RamPtr_ps->State_en;

    /* Compute the free running timer */
    CanNm_ComputeSwFrTimer(RamPtr_ps);

    /*** protect from interrupt ***/
    SchM_Enter_CanNm_MainFunctionNoNest();

    /* Get the RxIndication status */
    PduRxInd_b = RamPtr_ps->RxIndication_b;
    RamPtr_ps->RxIndication_b = FALSE;

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Get transmit confirmation status */
    PduTxConfirmation_b = RamPtr_ps->TxConfirmation_b;
    RamPtr_ps->TxConfirmation_b = FALSE;
#endif

#if ((CANNM_NODE_DETECTION_ENABLED != STD_OFF)||(CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF))
    /* Store the received control bit vector locally */
    CtrlBitVector_u8 = RamPtr_ps->RxCtrlBitVector_u8;
#endif

#if (CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF)
    /* Get the NmCoordinatorSleepReady bit from the CBV of the received message */
    RamPtr_ps->CoordReadySleepBit_u8 = (CtrlBitVector_u8 & CANNM_COORD_READY_SLEEP_BIT_MASK );
#endif

    /* Reset the control bit vector after storage*/
    RamPtr_ps->RxCtrlBitVector_u8 = (RamPtr_ps->RxCtrlBitVector_u8 & CANNM_RESET_CONTROL_BIT_VECTOR_MASK );

    /*** unprotect ***/
    SchM_Exit_CanNm_MainFunctionNoNest();

    /* Check whether CanNm is in Network mode */
    if (RamPtr_ps->Mode_en == NM_MODE_NETWORK)
    {
        StateCopy_e = CanNm_NetworkModeProcessing(
                    #if (CANNM_PN_ENABLED != STD_OFF && CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                        CanNm_NetworkHandle,
                    #endif
                    #if (CANNM_REMOTE_SLEEP_IND_ENABLED != STD_OFF || CANNM_BUS_LOAD_REDUCTION_ENABLED != STD_OFF || \
                         CANNM_COORDINATOR_SYNC_SUPPORT != STD_OFF || CANNM_PASSIVE_MODE_ENABLED == STD_OFF )
                        ConfigPtr_pcs,
                    #endif
                        RamPtr_ps,PduRxInd_b
                    #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                        ,PduTxConfirmation_b
                    #endif
                        );
    }

#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    stNetworkState_e = RamPtr_ps->NetworkReqState_en;
#endif
    RepeatMsgTime = ConfigPtr_pcs->RepeatMessageTime;

    /* Start state machine handling */
    switch(StateCopy_e)
    {

        case NM_STATE_BUS_SLEEP:
        {
            CanNm_CaseBusSleep(
                                #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                    CanNm_NetworkHandle,
                                #endif
                                    ConfigPtr_pcs,
                                #if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                                    RamPtr_ps,
                                #endif
                                    PduRxInd_b );
            break;
        }

        case NM_STATE_PREPARE_BUS_SLEEP:
        {
            CanNm_CasePrepareBusSleep(CanNm_NetworkHandle,ConfigPtr_pcs, RamPtr_ps,PduRxInd_b);
            break;
        }

        case NM_STATE_READY_SLEEP:
        {
            CanNm_CaseReadySleep(
                          #if (CANNM_NODE_DETECTION_ENABLED != STD_OFF || CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
                           CanNm_NetworkHandle,
                          #endif
                           ConfigPtr_pcs, RamPtr_ps
                          #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                           ,CtrlBitVector_u8
                          #endif
            );
            break;
        }

/* These states are available only if the passive mode  is disabled*/
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
        case NM_STATE_NORMAL_OPERATION:
        {
            CanNm_CaseNormalOperation(
                            #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                                CanNm_NetworkHandle,
                            #endif
                                ConfigPtr_pcs, RamPtr_ps
                            #if(CANNM_NODE_DETECTION_ENABLED != STD_OFF)
                                , CtrlBitVector_u8
                            #endif
            );
            break;
        }

        case NM_STATE_REPEAT_MESSAGE:
        {
            CanNm_CaseRepeatMessage(ConfigPtr_pcs, RamPtr_ps);
            break;
        }
#endif /* End of the passive mode switch */

        default:
        {
            /* Do nothing*/
            break;
        }
    } /* End - SWITCH CASE -> State Machine handling */

    /* Get the updated CanNm state */
    StateCopy_e = RamPtr_ps->State_en;

    /* Processing for zero wait bus sleep time */
    if ((ConfigPtr_pcs->WaitBusSleepTime == 0U) && (StateCopy_e == NM_STATE_PREPARE_BUS_SLEEP))
    {
        /* Prepare Bus Sleep State is transient*/
        CanNm_GotoBusSleep(ConfigPtr_pcs, RamPtr_ps);
    }

/* Network can be requested if passive mode is disabled*/
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    /* Repeat Message state is transient */
    if((RepeatMsgTime == 0U) && (StateCopy_e == NM_STATE_REPEAT_MESSAGE))
    {
        if(stNetworkState_e == CANNM_NETWORK_RELEASED_E)
        {
            /* If network is released, goto Ready Sleep state */
            CanNm_RepeatMessageToRS(ConfigPtr_pcs, RamPtr_ps);
        }
        else
        {
            /* If network is requested, goto Normal Operation state */
            CanNm_RepeatMessageToNO(ConfigPtr_pcs, RamPtr_ps);
        }
    }
#endif


/* Transmit NM PDU if passive mode is disabled */
#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
    CanNm_MainFunctionTx(CanNm_NetworkHandle);
#endif

    return;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


/***************************************************************************************************
 Function name    : CanNm_MainFunctionTx
 Syntax           : void CanNm_MainFunctionTx( const NetworkHandleType CanNm_NetworkHandle)
 Description      : This is the internal function of CanNm for transmission of Nm messages on CAN.
 Parameter        : CanNm_NetworkHandle - Identification of the CANNM-channel
 Return value     : None
 ***************************************************************************************************/
/* NM PDUs can be transmitted if Passive Mode is disabled*/
#if(CANNM_PASSIVE_MODE_ENABLED == STD_OFF)

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

static FUNC(void, CANNM_CODE) CanNm_MainFunctionTx(CONST(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle)
{

    /* Pointer to configuration data */
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) ConfigPtr_pcs;

    /* Pointer to RAM data */
    P2VAR(CanNm_RamType, AUTOMATIC, CANNM_APPL_DATA) RamPtr_ps;

    /* Return value of the functions called by Main */
    VAR(uint8_least, AUTOMATIC) RetValOfFuncs_ui;

    /* To store local copy of ComM ChannelId */
    NetworkHandleType nmChannelHandle;

    /**** End Of Declarations ****/

    /* Set the pointer to RAM structure of Channel CanNm_NetworkHandle */
    RamPtr_ps = &CanNm_RamData_s[CanNm_NetworkHandle];

    /* Initialise pointer to configuration structure */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);

    /* Store ComM ChannelId from the configuration */
    nmChannelHandle = ConfigPtr_pcs->NetworkHandle;

    /* TRACE[SWS_CanNm_00108] : Check for start of transmission and message cycle time expiration */
    if((RamPtr_ps->stCanNmComm != FALSE) && (RamPtr_ps->MsgTxStatus_b != FALSE) &&
            (CanNm_TimerExpired(RamPtr_ps->PrevMsgCycleTimestamp, RamPtr_ps->MsgCyclePeriod) != FALSE))
    {

        /* Check for the number of immediate Nm Pdus to be transmitted */
        if(RamPtr_ps->ImmediateNmTx_u8 == 0)
        {
            /* Store the message cycle time */
            RamPtr_ps->MsgCyclePeriod = ConfigPtr_pcs->MsgCycleTime;
        }
        else
        {
            /* Store the immediate Nm cycle time */
            RamPtr_ps->MsgCyclePeriod = ConfigPtr_pcs->ImmediateNmCycleTime;

            /* Decrement the immediate Nm Tx counter    */
            RamPtr_ps->ImmediateNmTx_u8--;

            /* Check for the number of immediate Nm Pdus after decrementing the immediate Nm Tx counter */
            if(RamPtr_ps->ImmediateNmTx_u8 == 0)
            {
				/* Load with message cycle offset time, after the transmission of all immediate Nm Pdus */
                RamPtr_ps->MsgCyclePeriod = ConfigPtr_pcs->MsgCycleOffset;
            }
        }
        /* Copy the Pdu data into Tx Buffer */
        CanNm_UpdateTxPdu(ConfigPtr_pcs,RamPtr_ps);

        /* Transmit the PDU */
        RetValOfFuncs_ui = CanNm_MessageTransmit(ConfigPtr_pcs,RamPtr_ps);

        /* Start monitoring message cycle time */
        CanNm_StartTimer(RamPtr_ps->PrevMsgCycleTimestamp);

        /* Start Msg timeout timer if there is transmit confirmation */
        if((RetValOfFuncs_ui == (uint8_least)E_OK) && (RamPtr_ps->TxConfirmation_b != FALSE))
        {
            /* Start Timeout time */
            CanNm_StartTimer(RamPtr_ps->PrevMsgTimeoutTimestamp);

            /* Msg transmit confirmation required */
            RamPtr_ps->TxConfirmation_b = FALSE;
        }
    }

    /* Monitor Msg timeout timer only in Repeat Message state and Normal Operation state. Also only when there is a transmission*/
    if ((RamPtr_ps->TxTimeoutMonitoringActive_b != FALSE) && (RamPtr_ps->stCanNmComm != FALSE) &&
            (RamPtr_ps->MsgTxStatus_b != FALSE) && (CanNm_TimerExpired(RamPtr_ps->PrevMsgTimeoutTimestamp, ConfigPtr_pcs->MsgTimeoutTime) != FALSE))
    {
        /* Inform NM about Tx Timeout */
        Nm_TxTimeoutException(nmChannelHandle);
# if (CANNM_PN_ENABLED != STD_OFF)
        CanSM_TxTimeoutException(nmChannelHandle);
# endif
		/* TRACE[SWS_CanNm_00066]: Clearing TxTimeoutMonitoringActive_b flag as Nm_TxTimeoutException should be called only once */
    	RamPtr_ps->TxTimeoutMonitoringActive_b = FALSE;
    }
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif


/***************************************************************************************************
 Function name    : CanNm_MainProcess_EIRA
 Syntax           : void CanNm_MainProcess_EIRA( void )
 Description      : This is the internal function of CanNm for transmission of Nm messages on CAN.
 Parameter        : None
 Return value     : None
 ***************************************************************************************************/

#if (CANNM_EIRACALC_ENABLED != STD_OFF)
LOCAL_INLINE FUNC(void, CANNM_CODE) CanNm_MainProcess_EIRA(void)
{
    VAR(uint8_least,CANNM_VAR)  Index_ui;
    VAR(boolean,CANNM_VAR)      up_Indication_b;
    P2CONST(CanNm_PNConfigType, AUTOMATIC, CANNM_APPL_CONST)  PnInfoType_pcs;
    VAR(uint8,CANNM_VAR)        currentStatusEIRA_au8[CANNM_PN_INFOLENGTH];

    up_Indication_b = FALSE;

    /*** protect from interrupt ***/
    SchM_Enter_CanNm_MainFunctionNoNest();

    /* Copy the Current status into a local status variable */
    for (Index_ui = 0;Index_ui < CANNM_PN_INFOLENGTH;Index_ui++)
    {
        /* Check if any PN , which is relevant for ECU, is requested */
        currentStatusEIRA_au8[Index_ui] = CanNM_EIRACurrentStatus_au8[Index_ui];
        /* Reset the Current status as the data is retrieved */
        CanNM_EIRACurrentStatus_au8[Index_ui] = 0;

    }

    SchM_Exit_CanNm_MainFunctionNoNest();

    PnInfoType_pcs = CanNm_GlobalConfigData_pcs->PnInfo_pcs;
    for (Index_ui = 0; Index_ui < CANNM_GET_NUMBER_OF_PN_CONFIGURED(); Index_ui++)
    {

        /* Check if the latest state of the PN is SET */
        if((currentStatusEIRA_au8[PnInfoType_pcs->ByteNum] & (PnInfoType_pcs->BitMask)) != 0u)
        {
            /* Reload the timer */
            /* Timer value +1 is added as the counter is immediately decremented */
            CanNm_PNTimer_au32[Index_ui] = (uint32)(CanNm_GlobalConfigData_pcs->PnResetTime + 1u);
            /* Check if the Status of the EIRA Flag was '0'(to detect a change)*/
            if ((uint8)(CanNM_EIRAGlobalStatus_au8[PnInfoType_pcs->ByteNum] & (PnInfoType_pcs->BitMask)) == (uint8)0x0)
            {
                /* Set the status of the EIRA flag to '1' */
                CanNM_EIRAGlobalStatus_au8[PnInfoType_pcs->ByteNum] = (CanNM_EIRAGlobalStatus_au8[PnInfoType_pcs->ByteNum] | (PnInfoType_pcs->BitMask));

                /* Inform COMM about the change in EIRA Status */
                up_Indication_b = TRUE;
            }
        }

        /* Reduce the ticks for the PN-EIRA timer, if it is started  */
        if (CanNm_PNTimer_au32[Index_ui] != 0U)
        {
            CanNm_PNTimer_au32[Index_ui]--;
            /* If there is a Time-Out and if the Old state of the PN was Set */
            if (CanNm_PNTimer_au32[Index_ui] == 0U)
            {
                /* Set the status of the EIRA flag to '0' */
                CanNM_EIRAGlobalStatus_au8[PnInfoType_pcs->ByteNum] = (CanNM_EIRAGlobalStatus_au8[PnInfoType_pcs->ByteNum] & ((uint8)(~(PnInfoType_pcs->BitMask))));

                /* Inform COMM about the change in EIRA Status */
                up_Indication_b = TRUE;

            }

        }


        /* Check if the latest state of the PN is RESET */
        PnInfoType_pcs++;
    }

    if (up_Indication_b == TRUE)
    {
        PduInfoType Data;
        Data.SduDataPtr = CanNM_EIRAGlobalStatus_au8;
        Data.SduLength = CANNM_PN_INFOLENGTH;
        /*Notify about the change in EIRA*/
        PduR_CanNmRxIndication(CANNM_PNEIRA_RXPDU_ID,&Data);
    }
}

#endif

/***************************************************************************************************
 Function name    : CanNm_MainProcess_ERA
 Syntax           : void CanNm_MainProcess_ERA( NetworkHandleType CanNm_NetworkHandle )
 Description      : This is the internal function of CanNm for transmission of Nm messages on CAN.
 Parameter        : CanNm_NetworkHandle - Identification of the CANNM-channel
 Return value     : None
 ***************************************************************************************************/

#if (CANNM_ERACALC_ENABLED != STD_OFF)
LOCAL_INLINE FUNC(void, CANNM_CODE) CanNm_MainProcess_ERA(VAR(NetworkHandleType, AUTOMATIC) CanNm_NetworkHandle)
{
    /* Pointer to Configuration Data */
    P2CONST(CanNm_ChannelConfigType, AUTOMATIC, CANNM_APPL_CONST) ConfigPtr_pcs;
    P2CONST(CanNm_PNConfigType, AUTOMATIC, CANNM_APPL_CONST)  PnInfoType_pcs;
    P2VAR(uint32, AUTOMATIC, CANNM_APPL_CONST) StartIndex_timer_pu32;
    VAR(uint8,CANNM_VAR) currentStatusERA_au8[CANNM_PN_INFOLENGTH];
    VAR(uint8_least,CANNM_VAR) Index_ui;
    VAR(uint8,CANNM_VAR) start_index_status_u8;
    VAR(uint8,CANNM_VAR) start_index_timer_u8;
    VAR(uint8,CANNM_VAR) index_ERA_u8;
    boolean up_Indication_b;

    up_Indication_b = FALSE;
    /* Initialize the Config data Ptr */
    ConfigPtr_pcs = CANNM_GET_CHANNEL_CONFIG(CanNm_NetworkHandle);
    /* get the start index for the channel in ERA array */
    start_index_status_u8 = CANNM_GetStartIndex_ERAStatus(ConfigPtr_pcs->ChannelOffset_u8);
    /* get the start index for the channel in Timer array */
    start_index_timer_u8 = CANNM_GetStartIndex_ERATimer(ConfigPtr_pcs->ChannelOffset_u8);

    /*** protect from interrupt ***/
    SchM_Enter_CanNm_MainFunctionNoNest();

    /* Copy the Current status into a local status variable */
    {
        uint8 * StartIndex_Status_pu8;
        StartIndex_Status_pu8 = &CanNM_ERACurrentStatus_au8[start_index_status_u8];
        for (Index_ui = 0; Index_ui < CANNM_PN_INFOLENGTH; Index_ui++)
        {
            /* Check if any PN , which is relevant for ECU, is requested */
            currentStatusERA_au8[Index_ui] = *StartIndex_Status_pu8;
            /* Reset the Current status as the data is retrieved */
            *StartIndex_Status_pu8 = 0;
            StartIndex_Status_pu8++;

        }
    }

    SchM_Exit_CanNm_MainFunctionNoNest();

    {
        PnInfoType_pcs = CanNm_GlobalConfigData_pcs->PnInfo_pcs;

        StartIndex_timer_pu32 = &CanNm_PNTimer_ERA_au32[start_index_timer_u8];

        /* Loop through Partial networks for this channel */
        for (Index_ui =  0; Index_ui < CANNM_GET_NUMBER_OF_PN_CONFIGURED(); Index_ui++)
        {
            /* Get the index into the CanNM_ERAGlobalStatus array for this PNC */
            index_ERA_u8 = start_index_status_u8 + PnInfoType_pcs->ByteNum;
            /* Check if the latest state of the PN is SET */
            if((currentStatusERA_au8[PnInfoType_pcs->ByteNum] & (PnInfoType_pcs->BitMask)) != 0u )
            {
                /* Reload the timer */
                /* Timer value +1 is added as the counter is immediately decremented */
                *StartIndex_timer_pu32 = (uint32)(CanNm_GlobalConfigData_pcs->PnResetTime + 1u);
                /* Check if the Status of the ERA Flag was '0'(to detect a change)*/
                if ((uint8)(CanNM_ERAGlobalStatus_au8[index_ERA_u8] & (PnInfoType_pcs->BitMask)) == (uint8)0x0)
                {
                    /* Set the status of the ERA flag to '1' */
                    CanNM_ERAGlobalStatus_au8[index_ERA_u8] = (CanNM_ERAGlobalStatus_au8[index_ERA_u8]  | (PnInfoType_pcs->BitMask));

                    /* Inform COMM about the change in ERA Status */
                    up_Indication_b = TRUE;
                }
            }

            /* Reduce the ticks for the PN-ERA timer, if it is started  */
            if (*StartIndex_timer_pu32 != 0U)
            {
                *StartIndex_timer_pu32 = (uint32)(*StartIndex_timer_pu32) - 1u;
                /* If there is a Time-Out and if the Old state of the PN was Set */
                if (*StartIndex_timer_pu32 == 0U)
                {
                    /* Set the status of the ERA flag to '0' */
                    CanNM_ERAGlobalStatus_au8[index_ERA_u8] = (CanNM_ERAGlobalStatus_au8[index_ERA_u8] & ((uint8)(~(PnInfoType_pcs->BitMask))));

                    /* Inform COMM about the change in ERA Status */
                    up_Indication_b = TRUE;

                }

            }

            /* Increment to point to the next PN */
            PnInfoType_pcs++;
            StartIndex_timer_pu32++;
        }
    }

    if (up_Indication_b == TRUE)
    {
        PduInfoType Data;
        Data.SduDataPtr = &CanNM_ERAGlobalStatus_au8[start_index_status_u8];
        Data.SduLength = CANNM_PN_INFOLENGTH;
        /*Notify about the change in ERA*/
        PduR_CanNmRxIndication(ConfigPtr_pcs->ERA_ComPduId_u8,&Data);
    }

}

#endif



