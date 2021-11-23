
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"


/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_MainFunction
 * Syntax :       void Nm_MainFunction( void )
 * Description:   This is the main function of NM Coordinator, this handles all channels that are being
 *                coordinated for shutdown.
 * Parameter:     void
 * Return:        void
 *************************************************************************************************************/
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static FUNC(void, NM_CODE) Nm_ComputeSwFRTimer(const uint32 MainFunctionCycleTime);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_MainFunction(void)
{

    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
#if (NM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF)
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;
#endif
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;


    VAR(Std_ReturnType,AUTOMATIC)                        RetVal;
    VAR(NetworkHandleType, AUTOMATIC)                   NetworkHandle;
    VAR(uint8,AUTOMATIC)                                ClusterLoopIdx;
    VAR(uint8,AUTOMATIC)                                ChannelLoopIdx;
    VAR(boolean,AUTOMATIC)                              stateChangePossible;
    static VAR(Nm_ClusterStateType,AUTOMATIC)           PrevClusterState;

    /*Computation of Nm timer*/
    Nm_ComputeSwFRTimer(NM_MAIN_FUNCTION_CYCLETIME);

    //loop through every cluster
    for(ClusterLoopIdx=0;ClusterLoopIdx < NM_NUMBER_OF_CLUSTERS;ClusterLoopIdx++)
    {
        GlobalRamDataPtr = &Nm_GlobalData_s[ClusterLoopIdx];

        if(GlobalRamDataPtr->InitStatus != TRUE)
        {
            /********************************* Start: DET *************************************/
            /* Report DET if NM is Uninitialized */
            NM_DET_REPORT_ERROR(ClusterLoopIdx, NM_SID_MAINFUNCTION,NM_E_UNINIT);

            /*********************************  End: DET  *************************************/
        }

        stateChangePossible = FALSE;

        do{
            switch(GlobalRamDataPtr->ClusterState)
            {
                case NM_CLUSTER_INIT:
                {
                    stateChangePossible = FALSE;

                    /*check if there is any network request or passive request*/
                    if((GlobalRamDataPtr->ctActive > 0) || (GlobalRamDataPtr->ctPassive > 0) || (GlobalRamDataPtr->ctComMRequests >0))
                    {
                        PrevClusterState = GlobalRamDataPtr->ClusterState;
                        GlobalRamDataPtr->ClusterState=NM_CLUSTER_ACTIVE_ACTIVE;
                        stateChangePossible = TRUE;
                    }
                    break;
                }//case NM_CLUSTER_INIT

                case NM_CLUSTER_ACTIVE_ACTIVE:
                {
                    if(stateChangePossible == TRUE)
                    {
                        stateChangePossible=FALSE;
                        if(PrevClusterState == NM_CLUSTER_INIT)
                        {
                            if((GlobalRamDataPtr->ctActive == 0) && (GlobalRamDataPtr->ctComMRequests == 0))
                            {
                                PrevClusterState = GlobalRamDataPtr->ClusterState;
                                GlobalRamDataPtr->ClusterState = NM_CLUSTER_ACTIVE_PASSIVE;
                                stateChangePossible = TRUE;
                            }
                        }
                        else if(PrevClusterState == NM_CLUSTER_ACTIVE_PASSIVE)
                        {
                            for(ChannelLoopIdx = 0 ;ChannelLoopIdx < NM_NUMBER_OF_CHANNELS; ChannelLoopIdx++)
                            {
                                ConfDataPtr = &Nm_ConfData_cs[ChannelLoopIdx];
                                NetworkHandle = ConfDataPtr->comMHandle;
                                if((ConfDataPtr->NmChannelType == NM_PASSIVE_CHANNEL) && (ConfDataPtr->ClusterIdx == ClusterLoopIdx))
                                {
                                    /*call internal network request to request bus specific network request and handle the states accordingly*/
                                    RetVal = Nm_Int_NetworkRequest(FALSE , NetworkHandle , NM_INTERNAL_REQUEST);
                                }
                            }
                        }
                        /*Previous state of cluster  is Passive*/
                        else
                        {
                            //To be handled in abort shutdown case.
                        }

                    }
                    else
                    {
                        /*Check if only passive channel is requested and active request for any channel form ComM within cluster */
                        if((GlobalRamDataPtr->ctActive == 0) && (GlobalRamDataPtr->ctComMRequests == 0))
                        {
                            PrevClusterState = GlobalRamDataPtr->ClusterState;
                            GlobalRamDataPtr->ClusterState = NM_CLUSTER_ACTIVE_PASSIVE;
                            stateChangePossible = TRUE;
                        }
                    }
                    break;
                }//case NM_CLUSTER_ACTIVE_ACTIVE

                case NM_CLUSTER_ACTIVE_PASSIVE:
                {
                    if(stateChangePossible == TRUE)
                    {
                        stateChangePossible = FALSE;
                        for(ChannelLoopIdx = 0;ChannelLoopIdx < NM_NUMBER_OF_CHANNELS;ChannelLoopIdx++)
                        {
                            ConfDataPtr = &Nm_ConfData_cs[ChannelLoopIdx];
                            NetworkHandle = ConfDataPtr->comMHandle;
                            MainNetworkRamDataPtr=&Nm_ChannelData_s[ChannelLoopIdx];
                            if((ConfDataPtr->NmChannelType == NM_PASSIVE_CHANNEL)&&(ConfDataPtr->ClusterIdx == ClusterLoopIdx) \
                                    &&(MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_NORMAL))
                            {
                                /*call bus specific release for passive channel and handle channel state accordingly*/
                                RetVal = Nm_Int_NetworkRelease(NetworkHandle,NM_INTERNAL_REQUEST);
                            }
                        }
                    }
                    else
                    {
                        if((GlobalRamDataPtr->ctActive > 0) || (GlobalRamDataPtr->ctComMRequests > 0))
                        {
                            PrevClusterState = GlobalRamDataPtr->ClusterState;
                            GlobalRamDataPtr->ClusterState = NM_CLUSTER_ACTIVE_ACTIVE;
                            stateChangePossible = TRUE;
                        }
                        else if(GlobalRamDataPtr->ctPassive == 0)
                        {
                            PrevClusterState = GlobalRamDataPtr->ClusterState;
                            GlobalRamDataPtr->ClusterState = NM_CLUSTER_PASSIVE;
                            stateChangePossible = TRUE;
                        }
                        else
                        {
                            //do nothing
                        }
                    }
                    break;
                }//case NM_CLUSTER_ACTIVE_PASSIVE

                case NM_CLUSTER_PASSIVE:
                {
                    if(stateChangePossible == TRUE)
                    {
                        stateChangePossible=FALSE;
                        GlobalRamDataPtr->ctBusSleep = 0;
                        /*Syncpoint would be initialized if Nm_SynchronizingNetwork is enabled */
                        /*Check whether Syncpoint is initialized*/
                        if(GlobalRamDataPtr->SyncPointState == NM_SYNCPOINT_INIT)
                        {
                            GlobalRamDataPtr->SyncPointState = NM_SYNCPOINT_READY;
                        }
                        else
                        {
                            GlobalRamDataPtr->SyncPointState = NM_SYNCPOINT_REACHED;
                        }


                    }//if(statechangepossible)
                    else
                    {
                        if(GlobalRamDataPtr->SyncPointState == NM_SYNCPOINT_REACHED)
                        {
                           GlobalRamDataPtr->SyncPointState = NM_SYNCPOINT_INVALID;
                           /*loop through every active channel to initiate shutdown procedure*/
                           for(ChannelLoopIdx = 0 ; ChannelLoopIdx < NM_NUMBER_OF_CHANNELS ; ChannelLoopIdx++)
                           {
                               ConfDataPtr=&Nm_ConfData_cs[ChannelLoopIdx];
                               NetworkHandle = ConfDataPtr->comMHandle;
                               MainNetworkRamDataPtr=&Nm_ChannelData_s[ChannelLoopIdx];

                               if((ConfDataPtr->ClusterIdx == ClusterLoopIdx))
                               {
                                   if((ConfDataPtr->NmChannelType == NM_ACTIVE_CHANNEL) && (MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_NORMAL))
                                   {
                                       RetVal = Nm_Int_NetworkRelease(NetworkHandle,NM_INTERNAL_REQUEST);
                                   }

                                   if((MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP) || \
                                           (MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_WAIT_SLEEP))
                                   {
                                       /*Increament BusSleep counter based on number of channel within a cluster are expected to go to bus sleep*/
                                       SchM_Enter_Nm_ChangeStateNoNest();
                                       GlobalRamDataPtr->ctBusSleep++;
                                       SchM_Exit_Nm_ChangeStateNoNest();
                                   }
                               }// if(part of cluster)
                           }//for loop
                        }//if(syncpointreached)

                        /*check all the bus have went to sleep*/
                        if((GlobalRamDataPtr->SyncPointState == NM_SYNCPOINT_INVALID) && (GlobalRamDataPtr->ctBusSleep == 0))
                        {
                            /*reset all the channels within the cluster*/
                            Nm_Reset(ClusterLoopIdx);
                            stateChangePossible = TRUE;
                        }
                    }//else
                    break;
                }//case NM_CLUSTER_PASSIVE
                default:
                    /* Intentionally Empty */
                    break;
            }//switch

        }while(stateChangePossible == TRUE);

    }// for loop through clusters


/* Checking each channel whether shutdown delay timer has expired or not*/
    for(ChannelLoopIdx = 0; ChannelLoopIdx < NM_NUMBER_OF_CHANNELS ; ChannelLoopIdx++)
    {
        ConfDataPtr = &Nm_ConfData_cs[ChannelLoopIdx];
        MainNetworkRamDataPtr = &Nm_ChannelData_s[ChannelLoopIdx];

        if(ConfDataPtr->ClusterIdx != 0xFF)
        {
            if(MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP)
            {
                /*Wait for Shutdown timer to expire*/
                if(Nm_TimerExpired(MainNetworkRamDataPtr->ShutdownDelayTimeStamp,ConfDataPtr->ShutdownDelay))
                {
                   NetworkHandle = ConfDataPtr->comMHandle;
#if (NM_BUS_SYNCHRONIZATION_ENABLED != STD_OFF)
                   /*Call busSynchronization to send the Nm message*/
                   FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];
                   RetVal = (*FuncPtr->BusNm_RequestBusSynchronization)(NetworkHandle);
#endif
                   /*call Bus Specific Network release and handle the channel state accordingly*/
                   RetVal = Nm_Int_NetworkRelease(NetworkHandle,NM_INTERNAL_REQUEST);
                }
            }
        }
    }//for loop channels
    (void)RetVal;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/*****************************************************************************************************
 * Function name: Nm_ComputeSwFRTimer
 * Description: This function calculate the Free running timer for NM Coordinator
 * Parameter: MainFunctionCycleTime - period of MainFunction calls
 * Return: void
 ****************************************************************************************************/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static FUNC(void, NM_CODE) Nm_ComputeSwFRTimer(const uint32 MainFunctionCycleTime)
{
    SwFRTimer += MainFunctionCycleTime;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif /* #if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF) */

