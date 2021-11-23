
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface header file for declaration of callback notifications,
 * this is included by the <Bus>Nm modules */
#include "Nm_Cbk.h"
/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"
/* Included to access ComM APIs related with NM */
#include "ComM.h"
#if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#if(NM_STATE_REPORT_ENABLED != STD_OFF)

#include "Com.h"
#if (!defined(COM_AR_RELEASE_MAJOR_VERSION) || (COM_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COM_AR_RELEASE_MINOR_VERSION) || (COM_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#endif
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_TxTimeoutException
 * Syntax :       FUNC(void, NM_CODE) Nm_TxTimeoutException(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle)
 * Description:   This is the NM call-back interface notifying Transmission timeout. This is called by FrNm
 *                when it detects timeout in getting Tx Confirmation.
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/
#if (NM_PASSIVE_MODE_ENABLED == STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_TxTimeoutException(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        ComM_Nm_TransmissionFailure(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/*****************************************************************************************************
 * Function name: Nm_NetworkTimeoutException
 * Syntax: FUNC(void, NM_CODE) Nm_NetworkTimeoutException(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle)
 * Description : This is the NM call-back interface notifying expiration of NM-Timeout time.
 *               This is called by either CNm or FrNm when a timeout of NM-Timeout timer is detected
 *               This in turn notifies ComM.
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_NetworkTimeoutException(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        ComM_Nm_NetworkTimeoutException(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/*****************************************************************************************************
 * Function name: Nm_CarWakeUpIndication
 * Syntax: FUNC(void, NM_CODE) Nm_CarWakeUpIndication (VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
 * Description : This is the NM call-back interface notifying Car Wake Up Indication.
 *               This is called by either CanNm or FrNm when CarWakeUp request is received in the NM-PDU
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#if (NM_CAR_WAKEUP_RX_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_CarWakeUpIndication(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);
    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
       {
			#if (NM_CAR_WAKEUP_CALLOUT_ENABLED != STD_OFF)
			   	NM_USER_CARWAKEUPINDICATION(NetworkHandle);
			#else
			   	BswM_NmIf_CarWakeUpIndication(NetworkHandle);
			#endif
       }
    else
    {
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CARWAKEUPINDICATION, NM_E_HANDLE_UNDEF);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
/*****************************************************************************************************
 * Function name: Nm_PduRxIndication
 * Syntax: FUNC(void, NM_CODE) Nm_PduRxIndication(
 *                                      CONST(NetworkHandleType, AUTOMATIC) NetworkHandle
 *                                     )
 * Description : This is the NM call-back interface to notify that the NM has received a message.
 *               This is called by either CanNm or FrNm. This interface is available depending on the
 *               configuration parameter NM_PDU_RX_INDICATION_ENABLED. It in turn notifies the application
 * Parameter: NetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/

#if (NM_PDU_RX_INDICATION_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_PduRxIndication(
                                       VAR(NetworkHandleType, AUTOMATIC) NetworkHandle
                                      )
{
    P2CONST(Nm_UserCallbackType,AUTOMATIC,NM_APPL_CONST)  UserCallback;
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);
    UserCallback = &Nm_UserCallbacks;

    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        /* call application call-back */
        if(UserCallback->PduRxIndication != NULL_PTR)
        {
            (*UserCallback->PduRxIndication)(NetworkHandle);
        }
    }
    return;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

/*****************************************************************************************************
 * Function name: Nm_StateChangeNotification
 * Syntax: FUNC(void, NM_CODE) Nm_StateChangeNotification(
 *                                              CONST(NetworkHandleType, AUTOMATIC)   nmNetworkHandle,
 *                                              CONST(Nm_StateType, AUTOMATIC)        nmPreviousState,
 *                                              CONST(Nm_StateType, AUTOMATIC)        nmCurrentState
 *                                             )
 * Description: This is the NM call-back interface to notify state changes. This is called by either CanNm
 *              or FrNm. It in turn notifies the application
 * Parameter1: nmNetworkHandle - Identification of the NM-channel
 * Parameter2: nmPreviousState - previous state of NM
 * Parameter3: nmCurrentState  - current state of NM
 * Return: void
 ****************************************************************************************************/

#if (NM_STATE_CHANGE_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_StateChangeNotification(
                                               VAR(NetworkHandleType, AUTOMATIC)   nmNetworkHandle,
                                               VAR(Nm_StateType, AUTOMATIC)        nmPreviousState,
                                               VAR(Nm_StateType, AUTOMATIC)        nmCurrentState
                                              )
{
    P2CONST(Nm_UserCallbackType,AUTOMATIC,NM_APPL_CONST)  UserCallback;
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
#endif
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(nmNetworkHandle);

    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        /* Since Repeat Message state has many entry points, check has to be done whether the
         * state change call is for entry into Network Mode
         */
        if ((nmCurrentState == NM_STATE_REPEAT_MESSAGE) && (nmPreviousState == NM_STATE_SYNCHRONIZE))
        {
            /* Notify ComM about Network mode */
            ComM_Nm_NetworkMode(nmNetworkHandle);
        }
        else
        {
            /* the transition is either within Network Mode OR it is for non-FrNm node;
             * hence no further notification is made
             */
        }

        UserCallback = &Nm_UserCallbacks;
        /* Update the application about NM state change */
        if (UserCallback->StateChangeInd != NULL_PTR)
        {
            (*UserCallback->StateChangeInd)(nmNetworkHandle, nmPreviousState, nmCurrentState);
        }


        /* Check if NMS report is enabled for the channel */
#if (NM_STATE_REPORT_ENABLED != STD_OFF)

        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];

        if (ConfDataPtr->NMSSignal != NM_NMS_REPORT_DISABLED)
        {
            uint8 NMSValue;
            NMSValue = 0xFF;
            if (nmCurrentState == NM_STATE_REPEAT_MESSAGE)
            {
                switch(nmPreviousState)
                {
                    case NM_STATE_BUS_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from BusSleepMode) */
                        NMSValue = NM_NMS_RM_BSM;
                        break;
                    }

                    case NM_STATE_PREPARE_BUS_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from PrepareBusSleepMode) */
                        NMSValue = NM_NMS_RM_PBSM;
                        break;
                    }

                    case NM_STATE_READY_SLEEP:
                    {
                        /* NM in state RepeatMessage (transition from ReadySleep) */
                        NMSValue = NM_NMS_RM_RS;
                        break;
                    }

                    case NM_STATE_NORMAL_OPERATION:
                    {
                        /* NM in state RepeatMessage (transition from NormalOperation) */
                        NMSValue = NM_NMS_RM_NO;
                        break;
                    }

                    default:
                        /* Intentionally empty */
                        break;

                }

            }
            else if (nmCurrentState == NM_STATE_NORMAL_OPERATION)
            {
                if (nmPreviousState == NM_STATE_REPEAT_MESSAGE)
                {
                    /* NM in state NormalOperation (transition from RepeatMessage) */
                    NMSValue = NM_NMS_NO_RM;
                }
                else if (nmPreviousState == NM_STATE_READY_SLEEP)
                {
                    /* NM in state NormalOperation (transition from ReadySleep) */
                    NMSValue = NM_NMS_NO_RS;

                }
                else
                {
                    /* Intentionally Empty */
                }

            }
            else
            {
                /* Intentionally Empty */
            }
            /* NMS value is within the range defined, hence report the state change */
            if (NMSValue != 0xFF)
            {
                /* MR12 DIR 1.1 VIOLATION: Com_SendSignal's SignalDataPtr return value should be void as per Autosar guidelines. */
                (void)Com_SendSignal(ConfDataPtr->NMSSignal, &NMSValue);
            }
        }
#endif /* #if (NM_STATE_REPORT_ENABLED != STD_OFF) */
    }

    return;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif


/*****************************************************************************************************
 * Function name: Nm_RepeatMessageIndication
 * Syntax: FUNC(void, NM_CODE) Nm_RepeatMessageIndication(VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle)
 * Description : This is an NM call-back interface to notify that an NM msg is received with Repeat Message
 *               Bit set. This is called by either CanNm or FrNm. This interface is available depending on the
 *               configuration parameter NM_REPEAT_MESSAGE_IND_ENABLED. It in turn notifies the application
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/

#if (NM_REPEAT_MESSAGE_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

/*This feature RepeatMessageIndication is now in channel and this should be taken care while using it in future.*/
FUNC(void, NM_CODE) Nm_RepeatMessageIndication(VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle)
{
	(void)nmNetworkHandle;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif



/*****************************************************************************************************
 * Function name: Nm_SychronizationPoint
 * Syntax: FUNC(void, NM_CODE) Nm_SynchronizationPoint(VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle)
 * Description : This is an NM call-back interface to notify the NM Coordinator functionality that this is a suitable
 *               point in time to initiate the coordinated shutdown on.This is called by Cyclic channel eg.FrNm.
 *               This interface is available depending on the configuration parameter NM_COORDINATOR_SUPPORT_ENABLED.
 * Parameter: nmNetworkHandle - Identification of the NM-channel
 * Return: void
 ****************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_SynchronizationPoint(VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle)
{
    // Declare local variable for channels,cluster and conf channnel
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
    VAR(NetworkHandleType, AUTOMATIC)                   Nm_NetworkHandle;

    Nm_NetworkHandle = NM_GET_HANDLE(nmNetworkHandle);

    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        /*Check if channel is part of any cluster*/
        if(ConfDataPtr->ClusterIdx != 0xFF)
        {
            GlobalRamDataPtr = &Nm_GlobalData_s[ConfDataPtr->ClusterIdx];
            MainNetworkRamDataPtr = &Nm_ChannelData_s[Nm_NetworkHandle];

            /*check if the channel is configured for synchronization and whether the channel is active*/
            if((ConfDataPtr->NmSynchronizingNetwork !=FALSE) && (ConfDataPtr->NmChannelType == NM_ACTIVE_CHANNEL))
            {
                /*checking the cluster has reached proper state before setting he Syncpoint to reached state*/
                SchM_Enter_Nm_ChangeStateNoNest();
                if((GlobalRamDataPtr->SyncPointState == NM_SYNCPOINT_READY) &&  \
                    (MainNetworkRamDataPtr->RemoteSleepStatus == TRUE) && \
                    (MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_NORMAL))
                {
                    GlobalRamDataPtr->SyncPointState = NM_SYNCPOINT_REACHED;
                }
                SchM_Exit_Nm_ChangeStateNoNest();

            }
        }
    }
    else
    {
        NM_DET_REPORT_ERROR(nmNetworkHandle, NM_SID_SYNCHRONIZATIONPOINT, NM_E_HANDLE_UNDEF);
    }

}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif



