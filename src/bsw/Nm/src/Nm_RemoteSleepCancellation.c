
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
 * Function Name: Nm_RemoteSleepCancellation
 * Syntax :       void Nm_RemoteSleepCancellation( const NetworkHandleType nmNetworkHandle )
 * Description:   This is the NM call-back interface to cancel detection of Remote Sleep status
 *                this is called by either CanNm or FrNm, it in turn notifies user application
 * Parameter:     NetworkHandle - Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/

#if (NM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_RemoteSleepCancellation(
                                               VAR(NetworkHandleType, AUTOMATIC)   NetworkHandle
                                              )
{
    P2CONST(Nm_UserCallbackType,AUTOMATIC,NM_APPL_CONST)    UserCallback; /* Holds user callback pointers */
/*#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
 * declare varibale for channle ,cluster
 * #endif
 * */
#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2VAR(Nm_NetworkRamType,AUTOMATIC,NM_APPL_DATA)     MainNetworkRamDataPtr;
    P2VAR(Nm_GlobalRamType,AUTOMATIC,NM_APPL_DATA)      GlobalRamDataPtr;
#endif

    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;                       /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);
    /* Process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        /* Gets User callback data and holds in UserCallback */
        UserCallback = &Nm_UserCallbacks;
        /* Indicate Remote Sleep cancellation to the application */
        if (UserCallback->RemoteSleepCancellation != NULL_PTR)
        {
            /* Call-back functions called by Nm, defined by User application */
            (*UserCallback->RemoteSleepCancellation)(NetworkHandle);
        }

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        if((ConfDataPtr->ClusterIdx != 0xFF))
        {

            MainNetworkRamDataPtr = &Nm_ChannelData_s[Nm_NetworkHandle];
            GlobalRamDataPtr = &Nm_GlobalData_s[ConfDataPtr->ClusterIdx];

            /*Increament the active counter and set rremotesleep status to false as remotessleep Cancellation is received on active channel*/
            if((ConfDataPtr->NmChannelType == NM_ACTIVE_CHANNEL) && (ConfDataPtr->NmChannelSleepMaster == FALSE))
            {
                SchM_Enter_Nm_ChangeStateNoNest();
                if((MainNetworkRamDataPtr->ChannelState == NM_CHANNEL_STATE_NORMAL) && (MainNetworkRamDataPtr->RemoteSleepStatus == TRUE))
                {
                    GlobalRamDataPtr->ctActive++;
                    MainNetworkRamDataPtr->RemoteSleepStatus = FALSE;
                }
                SchM_Exit_Nm_ChangeStateNoNest();
            }
        }
        #endif
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

