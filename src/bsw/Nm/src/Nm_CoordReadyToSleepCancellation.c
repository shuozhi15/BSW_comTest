

 /* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"



#if (NM_COORDINATOR_SYNC_SUPPORT_ENABLED != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

FUNC(void, NM_CODE) Nm_CoordReadyToSleepCancellation(VAR(NetworkHandleType, AUTOMATIC)   NetworkHandle)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    VAR(NetworkHandleType, AUTOMATIC)                   Nm_NetworkHandle;

    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        /*Check if channel is part of any cluster*/
        if((ConfDataPtr->ClusterIdx != 0xFF))
        {

            GlobalRamDataPtr = &Nm_GlobalData_s[ConfDataPtr->ClusterIdx];
            MainNetworkRamDataPtr = &Nm_ChannelData_s[Nm_NetworkHandle];
            /*check if the channel is passive configured*/
            if((ConfDataPtr->NmChannelType == NM_PASSIVE_CHANNEL) && (ConfDataPtr->NmChannelSleepMaster == FALSE))
            {
                /*Increament the passive counter as there is ready to sleep cancellation*/
                SchM_Enter_Nm_ChangeStateNoNest();
                if ((MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_WAIT_SLEEP) && (MainNetworkRamDataPtr->RemoteSleepStatus ==TRUE))
                {
                    GlobalRamDataPtr->ctPassive++;
                    MainNetworkRamDataPtr->RemoteSleepStatus = FALSE;
                }
                SchM_Exit_Nm_ChangeStateNoNest();
            }
        }
    }
    else
    {
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_COORDREADYTOSLEEPCANCELLATION, NM_E_HANDLE_UNDEF);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
