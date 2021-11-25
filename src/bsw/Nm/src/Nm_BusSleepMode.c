
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


/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_BusSleepMode

 * Description:   This is the NM call-back interface notifying Bus Sleep Mode. This is called by BusNm when it
 *                enters Bus Sleep mode. This in turn notifies ComM about Bus Sleep mode
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_BusSleepMode(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{

    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
     // Declare local variable for channels,cluster and conf channnel
#endif
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);
    /* Process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        /* Notification that the network management has entered Bus-Sleep Mode */
        ComM_Nm_BusSleepMode(NetworkHandle);

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)

        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        if((ConfDataPtr->ClusterIdx != 0xFF))
        {
            MainNetworkRamDataPtr = &Nm_ChannelData_s[Nm_NetworkHandle];
            GlobalRamDataPtr = &Nm_GlobalData_s[ConfDataPtr->ClusterIdx];
            if(MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_WAIT_SLEEP)
            {

#if (NM_COORDINATOR_SYNC_SUPPORT_ENABLED != STD_OFF)
                Nm_CoordReadyToSleepIndication(NetworkHandle);
#endif
                SchM_Enter_Nm_ChangeStateNoNest();
                MainNetworkRamDataPtr->ChannelState = NM_CHANNEL_STATE_BUS_SLEEP;
                /*Decreament BusSleep counter as there is an indication of one channel going to sleep*/
                if(GlobalRamDataPtr->ctBusSleep > 0)
                {
                    GlobalRamDataPtr->ctBusSleep--;
                }
                SchM_Exit_Nm_ChangeStateNoNest();

            }
        }
        #endif
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

