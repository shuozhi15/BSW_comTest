
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

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
 * Defines
 ***************************************************************************************************
 */

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_Init

 * Description:   This is the Initialization function, if EcuM exists, it'll call this interface for
 *                initialization of Nm Interface.
 * Parameter:     ConfigPtr - Pointer to the selected configuration set. \n
 * The Configuration pointer ConfigPtr is currently not used and shall therefore be set NULL_PTR value. \n
 * Return:        void
 *************************************************************************************************************/

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_Init( P2CONST(Nm_ConfigType , AUTOMATIC, NM_APPL_CONST) ConfigPtr)
{
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
    VAR(NetworkHandleType, AUTOMATIC)               ClusterLoopIdx;

    for (ClusterLoopIdx = 0; ClusterLoopIdx < NM_NUMBER_OF_CLUSTERS ;ClusterLoopIdx++)
    {
        Nm_Reset(ClusterLoopIdx);
    }

#endif
    (void)ConfigPtr;
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_Reset

 * Description:   This is the Initialization function, if EcuM exists, it'll call this interface for
 *                initialization of Nm Interface.
 * Parameter:     None
 * Return:        void
 *************************************************************************************************************/
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_Reset(const NetworkHandleType ClusterIdx)
{

    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
    VAR(NetworkHandleType, AUTOMATIC)                   ChannelLoopIdx;

    GlobalRamDataPtr = &Nm_GlobalData_s[ClusterIdx];

    //Initializze cluster elements
    GlobalRamDataPtr->ClusterState      =   NM_CLUSTER_INIT;
    GlobalRamDataPtr->ctComMRequests    =   NM_COUNTER_RESET;
    GlobalRamDataPtr->ctActive          =   NM_COUNTER_RESET;
    GlobalRamDataPtr->ctPassive         =   NM_COUNTER_RESET;
    GlobalRamDataPtr->ctBusSleep        =   NM_COUNTER_RESET;
    GlobalRamDataPtr->InitStatus        =   TRUE;
    GlobalRamDataPtr->SyncPointState    =   NM_SYNCPOINT_INVALID;

    for (ChannelLoopIdx = 0; ChannelLoopIdx < NM_NUMBER_OF_CHANNELS; ChannelLoopIdx++)
    {

        MainNetworkRamDataPtr   = &Nm_ChannelData_s[ChannelLoopIdx];
        ConfDataPtr             = &Nm_ConfData_cs[ChannelLoopIdx];

        if(ConfDataPtr->ClusterIdx == ClusterIdx)
        {
            // Initialise all the channel elements
            MainNetworkRamDataPtr->ChannelState  = NM_CHANNEL_STATE_INIT;
            MainNetworkRamDataPtr->ShutdownDelayTimeStamp =NM_COUNTER_RESET;
            MainNetworkRamDataPtr->ComMRequest =FALSE;

            if (ConfDataPtr->NmChannelSleepMaster== TRUE)
            {
                MainNetworkRamDataPtr->RemoteSleepStatus = TRUE;
            }
            else
            {
                MainNetworkRamDataPtr->RemoteSleepStatus = FALSE;
            }
        }

    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

