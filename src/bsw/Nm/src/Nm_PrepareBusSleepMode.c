
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
 * Function Name: Nm_PrepareBusSleepMode

 * Description:   This is the NM call-back interface notifying Prepare Bus Sleep Mode, this is not called by
 *                FrNm, this is called by CanNm when it enters Prepare Bus Sleep mode. This in turn notifies
 *                ComM about Prepare Bus Sleep mode.
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        void
 *************************************************************************************************************/

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(void, NM_CODE) Nm_PrepareBusSleepMode(VAR(NetworkHandleType, AUTOMATIC)   NetworkHandle)
{
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;  /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);
    /* Process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        /* Notify ComM */
        ComM_Nm_PrepareBusSleepMode(NetworkHandle);
    }
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

