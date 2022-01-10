

#include "Nm_Priv.h"


#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"

FUNC(Std_ReturnType, NM_CODE)  Nm_Int_NetworkRelease(NetworkHandleType NetworkHandle , Nm_NetworkRequestSource source)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;
    VAR(NetworkHandleType, AUTOMATIC)                   Nm_NetworkHandle;
    VAR(Std_ReturnType,AUTOMATIC)                        RetVal;

    Nm_NetworkHandle =NM_GET_HANDLE(NetworkHandle);

    RetVal = E_NOT_OK;
    ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
    MainNetworkRamDataPtr = &Nm_ChannelData_s[Nm_NetworkHandle];
    GlobalRamDataPtr = &Nm_GlobalData_s[ConfDataPtr->ClusterIdx];
    FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

    if (source == NM_EXTERNAL_REQUEST)
    {
        /*decreament ComM counter as channel is been release from ComM*/
        SchM_Enter_Nm_ChangeStateNoNest();
        if(MainNetworkRamDataPtr->ComMRequest == TRUE)
        {
            GlobalRamDataPtr->ctComMRequests--;
            MainNetworkRamDataPtr->ComMRequest = FALSE;
        }
        SchM_Exit_Nm_ChangeStateNoNest();
        RetVal = E_OK;
    }
    else
    {
        switch(MainNetworkRamDataPtr->ChannelState)
        {
            case NM_CHANNEL_STATE_NORMAL:
            {
                if (ConfDataPtr->NmChannelType == NM_ACTIVE_CHANNEL)
                {
                    MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP;
                    /*Start the shutdown delay timer for the specific channel*/
                    MainNetworkRamDataPtr->ShutdownDelayTimeStamp = SwFRTimer;
                    /*set sleep ready bit to 1 to indicate other ecu in the channel to indicate that the active node is ready to sleep*/
                    RetVal = (*FuncPtr->BusNm_SetSleepReadyBit)(NetworkHandle,SETBIT);
                }
                else
                {
                    MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_WAIT_SLEEP;
                    /*Call Bus Specific Network Release*/
                    RetVal = (*FuncPtr->BusNm_NetworkRelease)(NetworkHandle);

                }
                break;
            }

            case NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP:
            {
                //always active channnel would be called
                MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_WAIT_SLEEP;
                /*Call Bus Specific Network Release*/
                RetVal = (*FuncPtr->BusNm_NetworkRelease)(NetworkHandle);

                break;
            }

            case NM_CHANNEL_STATE_INIT:
            case NM_CHANNEL_STATE_WAIT_SLEEP:
            case NM_CHANNEL_STATE_BUS_SLEEP:
            case NM_CHANNEL_STATE_UNINIT:
            default:
            {
                /* Intentionally Empty */
                break;
            }
        }
    }
    return RetVal;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif
