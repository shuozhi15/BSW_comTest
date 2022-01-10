
 

/*<VersionHead>
 ***************************************************************************************************
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Nm/ AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 ***************************************************************************************************
 </VersionHead>*/


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
/* Nm interface main header file */
#include "Nm.h"
/* Nm interface private header file */
#include "Nm_Priv.h"


/*start of declarations*/








#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static FUNC(Std_ReturnType, NM_CODE) Nm_DummyRequestBusSynchronization(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle);


static FUNC(Std_ReturnType, NM_CODE) Nm_DummyRequestBusSynchronization(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle)
{
	(void)nmChannelHandle;
    /* [REQ] Return E_NOT_OK without executing any functionality */
    return(E_NOT_OK);

}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



 #define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static FUNC(Std_ReturnType, NM_CODE) Nm_DummyCheckRemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                CONSTP2VAR(boolean, AUTOMATIC, NM_APPL_DATA) nmRemoteSleepIndPtr);

static FUNC(Std_ReturnType, NM_CODE) Nm_DummyCheckRemoteSleepIndication(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                CONSTP2VAR(boolean, AUTOMATIC, NM_APPL_DATA) nmRemoteSleepIndPtr)
{
    (void)nmChannelHandle;
    *nmRemoteSleepIndPtr = *nmRemoteSleepIndPtr;

    /* [REQ] Return E_NOT_OK without executing any functionality */
    return(E_NOT_OK);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"























#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
static FUNC(Std_ReturnType, NM_CODE) Nm_DummySetSleepReadyBit(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle ,const boolean setBit);

static FUNC(Std_ReturnType, NM_CODE) Nm_DummySetSleepReadyBit(CONST(NetworkHandleType, AUTOMATIC) nmChannelHandle ,const boolean setBit )
{
    (void)nmChannelHandle;
    (void)setBit;
    

    /* [REQ] Return E_NOT_OK without executing any functionality */
    return(E_NOT_OK);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
CONST(Nm_ConfigType, NM_CONST) Nm_ConfData_cs[NM_NUMBER_OF_CHANNELS] =
{
    // Channel 0
    {
        0,             // Shutdown delay time
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
        NM_NMS_REPORT_DISABLED,                            // NMS report not used for the channel
#endif
        NM_BUS_TYPE_CAN,     // BusNm Type
        NM_ACTIVE_CHANNEL,     // Channel Type
        0xFF,      // Index of the cluster this channel belongs to
        0,               // Handle of the ComM channel
        FALSE,     //ChannelSleepMaster   
        FALSE,     //SynchronizingNetwork
        TRUE,     // Node detection enabled
        TRUE,     // Node Id enabled
        TRUE,     // Repeat message id enabled      
    }};
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

 #if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
VAR(Nm_NetworkRamType, NM_VAR) Nm_ChannelData_s[NM_NUMBER_OF_CHANNELS];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"

#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
VAR(Nm_GlobalRamType, NM_VAR) Nm_GlobalData_s[NM_NUMBER_OF_CLUSTERS];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"

#define NM_START_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"
VAR(Nm_TimerType ,NM_VAR)  SwFRTimer;
#define NM_STOP_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"

#endif


/* APIs of BusNm */

#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
CONST(Nm_BusNmApiType, NM_CONST) Nm_BusNmApi[] =
{
    {
        &CanNm_NetworkRelease,
        &CanNm_NetworkRequest,
        &CanNm_PassiveStartUp,
        &Nm_DummyRequestBusSynchronization,
        &Nm_DummyCheckRemoteSleepIndication,
        &CanNm_DisableCommunication,
        &CanNm_EnableCommunication,
        &CanNm_GetLocalNodeIdentifier,
        &CanNm_GetNodeIdentifier,
        &CanNm_GetPduData,
        &CanNm_GetState,
        &CanNm_GetUserData,
        &CanNm_RepeatMessageRequest,
        &CanNm_SetUserData,
        &Nm_DummySetSleepReadyBit
    }
};
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"


/* Call-back functions called by Nm, defined by User application */

#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
CONST(Nm_UserCallbackType, NM_CONST) Nm_UserCallbacks =
{
   /* PduRxIndication */
    NULL_PTR,

    //RemoteSleepCancel
    &TestNm_RemoteSleepCancellation,

    //RemoteSleepInd
    &TestNm_RemoteSleepIndication,

    //StateChangeInd
    NULL_PTR

};
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"








