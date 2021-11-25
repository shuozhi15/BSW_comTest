

#include "Nm_Priv.h"

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
LOCAL_INLINE FUNC(void, NM_CODE) Nm_Int_ComMRequestUpdate(P2VAR(boolean,AUTOMATIC,NM_APPL_DATA) ComMRequestPtr ,
                                                            P2VAR(uint8,AUTOMATIC,NM_APPL_DATA) ctComMRequestsPtr,
                                                            VAR(boolean,AUTOMATIC) active);


FUNC(Std_ReturnType, NM_CODE)  Nm_Int_NetworkRequest(boolean active, NetworkHandleType NetworkHandle , Nm_NetworkRequestSource source)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;
    VAR(NetworkHandleType, AUTOMATIC)                   Nm_NetworkHandle;
    VAR(Std_ReturnType,AUTOMATIC)                        RetVal;


    Nm_NetworkHandle =NM_GET_HANDLE(NetworkHandle);

    ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
    MainNetworkRamDataPtr = &Nm_ChannelData_s[Nm_NetworkHandle];
    GlobalRamDataPtr = &Nm_GlobalData_s[ConfDataPtr->ClusterIdx];
    FuncPtr=&Nm_BusNmApi[ConfDataPtr->BusNmType];

    RetVal = E_NOT_OK;

    switch(MainNetworkRamDataPtr->ChannelState)
    {
        case NM_CHANNEL_STATE_INIT:
        {
            if(ConfDataPtr->NmSynchronizingNetwork != FALSE)
            {
                GlobalRamDataPtr->SyncPointState = NM_SYNCPOINT_INIT;
            }
            //check the source for network request
            if(source == NM_EXTERNAL_REQUEST)
            {
                MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_NORMAL;
                RetVal=(*FuncPtr->BusNm_NetworkRequest)(NetworkHandle);

                if(ConfDataPtr->NmChannelSleepMaster == FALSE)
                {

                    MainNetworkRamDataPtr->RemoteSleepStatus = FALSE;

                    //active channel
                    if(ConfDataPtr->NmChannelType == NM_ACTIVE_CHANNEL)
                    {
                        SchM_Enter_Nm_ChangeStateNoNest();
                        GlobalRamDataPtr->ctActive++;
                        SchM_Exit_Nm_ChangeStateNoNest();
                    }
                    else //Passive channel
                    {
                        SchM_Enter_Nm_ChangeStateNoNest();
                        GlobalRamDataPtr->ctPassive++;
                        SchM_Exit_Nm_ChangeStateNoNest();
                    }
                }

                Nm_Int_ComMRequestUpdate(&(MainNetworkRamDataPtr->ComMRequest),&(GlobalRamDataPtr->ctComMRequests),active);
            }
            else
            {
                RetVal = E_OK;
                // ComM_Nm_NetworkRestartIndication();
            }
            break;

        }

        case NM_CHANNEL_STATE_NORMAL:
        {
            if(source == NM_EXTERNAL_REQUEST)
            {
                Nm_Int_ComMRequestUpdate(&(MainNetworkRamDataPtr->ComMRequest),&(GlobalRamDataPtr->ctComMRequests),active);
                RetVal = E_OK;
            }
            break;
        }

        case NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP:
        {
            /*comes here in Abort shutdown case which is not handled in current design*/
            if(source == NM_EXTERNAL_REQUEST)
            {
                Nm_Int_ComMRequestUpdate(&(MainNetworkRamDataPtr->ComMRequest),&(GlobalRamDataPtr->ctComMRequests),active);
                MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_NORMAL;
                RetVal = E_OK;
            }
            break;
        }
        case NM_CHANNEL_STATE_WAIT_SLEEP:
        {
            /*comes here in Abort shutdown case which is not handled in current design*/
            if(source == NM_EXTERNAL_REQUEST)
            {
                Nm_Int_ComMRequestUpdate(&(MainNetworkRamDataPtr->ComMRequest),&(GlobalRamDataPtr->ctComMRequests),active);
                MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_NORMAL;
                RetVal = E_OK;
            }
            else
            {
                if(ConfDataPtr->NmChannelType == NM_PASSIVE_CHANNEL)
                {
                    MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_NORMAL;
                    RetVal=(*FuncPtr->BusNm_NetworkRequest)(NetworkHandle);

                    if(ConfDataPtr->NmChannelSleepMaster == FALSE)
                    {
                        MainNetworkRamDataPtr->RemoteSleepStatus = FALSE;
                    }

                }
                else
                {
                    RetVal = E_OK;
                }
            }
            break;
        }
        case NM_CHANNEL_STATE_BUS_SLEEP:
        {
            if(source == NM_EXTERNAL_REQUEST)
            {
                MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_NORMAL;
                RetVal=(*FuncPtr->BusNm_NetworkRequest)(NetworkHandle);

                Nm_Int_ComMRequestUpdate(&(MainNetworkRamDataPtr->ComMRequest),&(GlobalRamDataPtr->ctComMRequests),active);

                if(GlobalRamDataPtr->ClusterState != NM_CLUSTER_PASSIVE)
                {
                    if(ConfDataPtr->NmChannelSleepMaster == FALSE)
                    {
                        MainNetworkRamDataPtr->RemoteSleepStatus = FALSE;
                        //active channel
                        if(ConfDataPtr->NmChannelType == NM_ACTIVE_CHANNEL)
                        {
                            /*increament active counter as active channel is request for the channel*/
                            SchM_Enter_Nm_ChangeStateNoNest();
                            GlobalRamDataPtr->ctActive++;
                            SchM_Exit_Nm_ChangeStateNoNest();
                        }
                        else //Passive channel
                        {
                            /*increament passive counter as passive channel is request for the channel*/
                            SchM_Enter_Nm_ChangeStateNoNest();
                            GlobalRamDataPtr->ctPassive++;
                            SchM_Exit_Nm_ChangeStateNoNest();
                        }
                    }//mastersleep
                }//not in passive
            }
            else
            {
                RetVal = E_OK;
                //ComM_Nm_RestartIndication();
            }
            break;
        }

        case NM_CHANNEL_STATE_UNINIT:
        default:
        {
            /* Intentionally Empty */
            break;
        }
    }
    return RetVal;
}

#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#endif

/***************************************************************************************************
 * Function name    : Nm_Int_ComMRequestUpdate
 * Syntax           : LOCAL_INLINE FUNC(void, NM_CODE) Nm_Int_ComMRequestUpdate(P2VAR(boolean,AUTOMATIC,NM_APPL_DATA) ComMRequestPtr ,
                                                            P2VAR(uint8,AUTOMATIC,NM_APPL_DATA) ctComMRequestsPtr,
                                                            VAR(boolean,AUTOMATIC) active)
 * Description      : Updating the ComM request to the counter.
 * Parameter        : ComMRequestPtr -> status flag for channel ipdates as per the request from network request or passive startup
                      ctComMRequestsPtr ->counter to keep track for number of ComM request per cluster
                      active -> variable to distinguish the request came from network request or passive startup
 * Return value     : void
 ***************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

LOCAL_INLINE FUNC(void, NM_CODE) Nm_Int_ComMRequestUpdate(P2VAR(boolean,AUTOMATIC,NM_APPL_DATA) ComMRequestPtr ,
                                                            P2VAR(uint8,AUTOMATIC,NM_APPL_DATA) ctComMRequestsPtr,
                                                            VAR(boolean,AUTOMATIC) active)
{
    /*Increament ComM counter to indicate there is an active request on a channel in the cluster*/
    SchM_Enter_Nm_ChangeStateNoNest();
    if((active == TRUE) && ((*ComMRequestPtr) == FALSE))
    {
        (*ComMRequestPtr) = TRUE;
        (*ctComMRequestsPtr)++;
    }
    SchM_Exit_Nm_ChangeStateNoNest();
}


#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#endif

