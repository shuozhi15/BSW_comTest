
#ifndef NM_PRIV_H
#define NM_PRIV_H

/**
 * \defgroup NM_PRIV_H    Nm_Priv - Private Function of Nm.
 * \n
 */

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface private header file
 *
 * \scope               This is included only by Nm module
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "Nm.h"
#include "Nm_Cfg_SchM.h"


#ifdef NM_TESTCD_CALLBACK
#include "testcd_nm.h"
#endif
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/**
 * @ingroup NM_PRIV_H
 *
 * Macro to get Flexray Timer
 */
#define Nm_GetCurrFRTime()                          (SwFRTimer)
/**
 * @ingroup NM_PRIV_H
 *
 * Macro to check if Timer has expired or not
 */
#define Nm_TimerExpired(PrevTimeStamp, Timeout)     ((Nm_GetCurrFRTime() - (PrevTimeStamp)) >= (Timeout))


/**
 * @ingroup NM_PRIV_H
 *
 *  Used for invalid handle ids
 */
#define NM_INVALID_HANDLE                    0xFFu

/**
 * @ingroup NM_PRIV_H
 *
 *  This value indicates that for a given channel, NMS reporting is disabled
 * The Com_SignalIdType is defined as uint16 in COM spec, hence the value of 0xFFFFu is chosen for the same
 */
#define NM_NMS_REPORT_DISABLED                0xFFFFu

/* Service IDs of Nm APIs */

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_INIT                              (0)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_PassiveStartUp
 */
#define NM_SID_PASSIVESTARTUP                    (0x01)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_NetworkRequest
 */
#define NM_SID_NETWORKREQUEST                    (0x02)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_NetworkRelease
 */
#define NM_SID_NETWORKRELEASE                    (0x03)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see  Nm_DisableCommunication
 */
#define NM_SID_DISABLECOMMUNICATION              (0x04)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_EnableCommunication
 */
#define NM_SID_ENABLECOMMUNICATION               (0x05)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_SetUserData
 */
#define NM_SID_SETUSERDATA                       (0x06)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetUserData
 */
#define NM_SID_GETUSERDATA                       (0x07)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetPduData
 */
#define NM_SID_GETPDUDATA                        (0x08)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_RepeatMessageRequest
 */
#define NM_SID_REPEATMESSAGEREQUEST              (0x09)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_GETNODEIDENTIFIER                 (0x0A)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetLocalNodeIdentifier
 */
#define NM_SID_GETLOCALNODEIDENTIFIER            (0x0B)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_CHECKREMOTESLEEPINDICATION        (0x0D)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError corresponds to @see Nm_GetState
 */
#define NM_SID_GETSTATE                          (0x0E)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_GETVERSIONINFO                    (0x0F)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_MAINFUNCTION                         (0x10)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_NETWORKSTARTINDICATION            (0x11)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_NETWORKMODE                         (0x12)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_PREPAREBUSSLEEPMODE                 (0x13)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_BUSSLEEPMODE                         (0x14)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_PDURXINDCATION                       (0x15)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_STATECHANGENOTIFICATION              (0x16)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_REMOTESLEEPINDICATION                (0x17)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_REMOTESLEEPCANCELLATION              (0x18)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_SYNCHRONIZATIONPOINT                 (0x19)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError .
 */
#define NM_SID_REPEATMSGINDICATION                  (0x1A)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_TXTIMEOUTEXCEPTION                   (0x1B)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_CARWAKEUPINDICATION                   (0x1d)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_COORDREADYTOSLEEPINDICATION              (0x1e)

/**
 * @ingroup NM_PRIV_H
 *
 * Indicates that Api Id passed in the call of Det_ReportError.
 */
#define NM_SID_COORDREADYTOSLEEPCANCELLATION            (0x1f)


/* MACRO for DET Support */
#if (NM_DEV_ERROR_DETECT != STD_OFF)
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for functions with return type as void
 */
#define NM_DET_REPORT_ERROR(InstanceId, ApiId, ErrorId)       (void)Det_ReportError((NM_MODULE_ID),(InstanceId),(ApiId),(ErrorId))

#else
/**
 * @ingroup NM_PRIV_H
 *
 * Macro to handle DET error report if device error detection is disabled
 */
#define NM_DET_REPORT_ERROR(InstanceId, ApiId, ErrorId)       do{}while(0)
#endif

/* Defines for NM State Report Feature Implementation */
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from bus sleep mode to repeat message state
 */
#define NM_NMS_RM_BSM                0x01
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from prepare bus sleep mode to repeat message state
 */
#define NM_NMS_RM_PBSM                0x02
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from repeat message to normal operation state
 */
#define NM_NMS_NO_RM                0x04
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from ready sleep to normal operation state
 */
#define NM_NMS_NO_RS                0x08
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from ready sleep to repeat message state
 */
#define NM_NMS_RM_RS                0x10
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for NMS value due to transistion from normal operation to repeat message state
 */
#define NM_NMS_RM_NO                0x20
/**
 * @ingroup NM_PRIV_H
 *
 * Macro for resetting the counter
 */
#define NM_COUNTER_RESET            0x00

/**
 * @ingroup NM_PRIV_H
 *
 * Macro for resetting the Coord Ready to sleep bit in CBV
 */
#define RESETBIT                    FALSE

/**
 * @ingroup NM_PRIV_H
 *
 * Macro for setting the Coord Ready to sleep bit in CBV
 */
#define SETBIT                      TRUE


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */


/**
 * @ingroup NM_PRIV_H
 *
 * typedef enum
 *  {\n
 *   NM_INTERNAL_REQUEST = 0,           network request made by Nm internal APIs\n
 *   NM_EXTERNAL_REQUEST                network request made by external module esp ComM\n
 *  } Nm_NetworkRequestSource;\n
 *
 */
typedef enum
{
    NM_INTERNAL_REQUEST = 0,
    NM_EXTERNAL_REQUEST
} Nm_NetworkRequestSource;


/**
 * @ingroup NM_PRIV_H
 *
 * typedef enum
 *  {\n
 *   NM_SYNCPOINT_INVALID = 0,
 *   NM_SYNCPOINT_INIT,            SyncPoint state is at init \n
 *   NM_SYNCPOINT_READY,                SyncPoint is ready to receive Sync Call back  \n
 *   NM_SYNCPOINT_REACHED,              SyncPoint is reached and callback is called \n
 *  } Nm_SynchType;\n
 *
 */
typedef enum
{
    NM_SYNCPOINT_INVALID = 0,
    NM_SYNCPOINT_INIT,
    NM_SYNCPOINT_READY,
    NM_SYNCPOINT_REACHED
} Nm_SynchType;

/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   NM_CLUSTER_INIT = 0,            Init state ..Co ordination is over\n
 *   NM_CLUSTER_PASSIVE,             T0 started , aborted shutdown can happen\n
 *   NM_CLUSTER_ACTIVE_ACTIVE,       Active state , active channel kept active by remote nodes\n
 *   NM_CLUSTER_ACTIVE_PASSIVE       Active state , passive channel kept active by active coordinator\n
 *  } Nm_ClusterStateType;
 */
typedef enum
{
    NM_CLUSTER_INIT = 0,
    NM_CLUSTER_PASSIVE,
    NM_CLUSTER_ACTIVE_ACTIVE,
    NM_CLUSTER_ACTIVE_PASSIVE
} Nm_ClusterStateType;


/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   NM_CHANNEL_STATE_UNINIT = 0,
 *   NM_CHANNEL_STATE_INIT,
 *   NM_CHANNEL_STATE_NORMAL,                    Network is requested in this state\n
 *   NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP,        T0 started..channel timer started waiting to release the network\n
 *   NM_CHANNEL_STATE_WAIT_SLEEP,                network released .. waiting for bussleep indication from nm\n
 *   NM_CHANNEL_STATE_BUS_SLEEP                  Bussleep state possible only after cooridnation has started\n
 *  } Nm_ChannelStateType;
 */
typedef enum
{
    NM_CHANNEL_STATE_UNINIT = 0,
    NM_CHANNEL_STATE_INIT,
    NM_CHANNEL_STATE_NORMAL,
    NM_CHANNEL_STATE_PREPARE_WAIT_SLEEP,
    NM_CHANNEL_STATE_WAIT_SLEEP,
    NM_CHANNEL_STATE_BUS_SLEEP

} Nm_ChannelStateType;


/**
 * @ingroup NM_PRIV_H
 *
 *  typedef enum
 *  {\n
 *   RESETBIT = 0,                   For clearing SleepCoordbit in control bit vector\n
     SETBIT                          For seting SleepCoordbit in control bit vector\n
 *  }
 */
/*typedef enum
{
    RESETBIT = 0,
    SETBIT
}Nm_SetSleepReadyBitType;
*/


/**
 * @ingroup NM_PRIV_H
 *
 * typedef struct
 * {\n
 *   Nm_ChannelStateType   ChannelState;              holds Channel state \n
 *   Nm_TimerType        ShutdownDelayTimeStamp;  variable to monitor shutdown delay \n
 *   boolean             ComMRequest;               status of ComM Full Communication request \n
 *   boolean             RemoteSleepStatus;       status of remote sleep detection \n

 *  } Nm_NetworkRamType;
 */
typedef struct
{
    Nm_ChannelStateType ChannelState;
    Nm_TimerType        ShutdownDelayTimeStamp;
    boolean             ComMRequest;
    boolean             RemoteSleepStatus;
 } Nm_NetworkRamType ;



 /**
  * @ingroup NM_PRIV_H
  *
  * typedef struct
  * { \n
  *   Nm_ClusterStateType ClusterState;       Holds cluster status \n
  *   Nm_SynchType        SyncPointState;    Holds the synchronization state of the cluster\n
  *   uint8               ctComMRequests;    Number of Full Com Requests from ComM
  *   uint8               ctActive;          Number of active channels that are not ready to sleep\n
  *   uint8               ctPassive;         Number of passive channels that are not ready to sleep\n
  *   uint8               ctBusSleep;        Number of channels that are yet to report bussleep..Used to signal the end of coordination\n
  *   boolean             InitStatus;         initialisation status \n
  * } Nm_GlobalRamType;
  */
 typedef struct
 {
     Nm_ClusterStateType ClusterState;
     Nm_SynchType        SyncPointState;
     uint8               ctComMRequests;
     uint8               ctActive;
     uint8               ctPassive;
     uint8               ctBusSleep;
     boolean             InitStatus;
 } Nm_GlobalRamType;


/**
 * @ingroup NM_PRIV_H
 *
 *  typedef struct
 *   {\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_NetworkRelease)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_NetworkRequest)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_PassiveStartUp)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_RequestBusSynchronization)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_CheckRemoteSleepIndication)( NetworkHandleType,\n
 *                                                                       P2VAR(boolean, AUTOMATIC, NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_DisableCommunication)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_EnableCommunication)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetLocalNodeIdentifier)( NetworkHandleType,\n
 *                                                                     P2VAR(uint8,AUTOMATIC,NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetNodeIdentifier)( NetworkHandleType,\n
 *                                                                P2VAR(uint8, AUTOMATIC, NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetPduData)( NetworkHandleType,\n
 *                                                         P2VAR(uint8, AUTOMATIC, NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetState)( NetworkHandleType,\n
 *                                                       P2VAR(Nm_StateType, AUTOMATIC, NM_APPL_DATA),\n
 *                                                       P2VAR(Nm_ModeType, AUTOMATIC, NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetUserData)( NetworkHandleType,\n
 *                                                          P2VAR(uint8, AUTOMATIC, NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_RepeatMessageRequest)( NetworkHandleType);\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_SetUserData)( NetworkHandleType,\n
 *                                                          CONSTP2CONST(uint8, AUTOMATIC, NM_APPL_DATA));\n
 *   P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_SetSleepReadyBit)( NetworkHandleType,  boolean);
 *   } Nm_BusNmApiType;
 */
typedef struct
{
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_NetworkRelease)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_NetworkRequest)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_PassiveStartUp)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_RequestBusSynchronization)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_CheckRemoteSleepIndication)( NetworkHandleType,
                                                                        P2VAR(boolean, AUTOMATIC, NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_DisableCommunication)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_EnableCommunication)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetLocalNodeIdentifier)( NetworkHandleType,
                                                                      P2VAR(uint8,AUTOMATIC,NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetNodeIdentifier)( NetworkHandleType,
                                                                 P2VAR(uint8, AUTOMATIC, NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetPduData)( NetworkHandleType,
                                                          P2VAR(uint8, AUTOMATIC, NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetState)( NetworkHandleType,
                                                        P2VAR(Nm_StateType, AUTOMATIC, NM_APPL_DATA),
                                                        P2VAR(Nm_ModeType, AUTOMATIC, NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_GetUserData)( NetworkHandleType,
                                                           P2VAR(uint8, AUTOMATIC, NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_RepeatMessageRequest)( NetworkHandleType);
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_SetUserData)( NetworkHandleType,
                                                          P2CONST(uint8, AUTOMATIC, NM_APPL_DATA));
    P2FUNC(Std_ReturnType, NM_APPL_CODE, BusNm_SetSleepReadyBit)( NetworkHandleType,  boolean);

} Nm_BusNmApiType;

/**
 * @ingroup NM_PRIV_H
 *
 *  typedef struct
 * {\n
 *   P2FUNC(void, NM_APPL_CODE, PduRxIndication)(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle);\n
 *   P2FUNC(void, NM_APPL_CODE, RemoteSleepCancellation)(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle);\n
 *   P2FUNC(void, NM_APPL_CODE, RemoteSleepIndication)(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle);\n
 *   P2FUNC(void, NM_APPL_CODE, StateChangeInd)(\n
 *                                              CONST(NetworkHandleType, AUTOMATIC)  NetworkHandle,\n
 *                                              CONST(Nm_StateType, AUTOMATIC)   nmPreviousState,\n
 *                                              CONST(Nm_StateType, AUTOMATIC)   nmCurrentState\n
 *                                             );\n
 *  } Nm_UserCallbackType;
 *
 */
typedef struct
{
    P2FUNC(void, NM_APPL_CODE, PduRxIndication)(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle);
    P2FUNC(void, NM_APPL_CODE, RemoteSleepCancellation)(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle);
    P2FUNC(void, NM_APPL_CODE, RemoteSleepIndication)(CONST(NetworkHandleType, AUTOMATIC) NetworkHandle);
    P2FUNC(void, NM_APPL_CODE, StateChangeInd)(
                                               CONST(NetworkHandleType, AUTOMATIC)  NetworkHandle,
                                               CONST(Nm_StateType, AUTOMATIC)   nmPreviousState,
                                               CONST(Nm_StateType, AUTOMATIC)   nmCurrentState
                                              );

} Nm_UserCallbackType;

/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold configuration data structure
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern CONST(Nm_ConfigType, NM_CONST) Nm_ConfData_cs[];
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold the address of <Bus>Nm apis'
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern CONST(Nm_BusNmApiType, NM_CONST) Nm_BusNmApi[];
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold the address of user callback functions
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern CONST(Nm_UserCallbackType, NM_CONST) Nm_UserCallbacks;
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold ComM Id mapped to <Bus>Nm
 */
#define NM_START_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"
extern CONST(NetworkHandleType, NM_CONST) Nm_HandleTable[];
#define NM_STOP_SEC_CONST_UNSPECIFIED
#include "Nm_MemMap.h"

#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)
/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold global configuration data structure
 */
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
extern VAR(Nm_GlobalRamType, NM_VAR) Nm_GlobalData_s[];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"

/**
 * @ingroup NM_PRIV_H
 *
 * Array to hold global configuration data structure
 */
#define NM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
extern VAR(Nm_NetworkRamType, NM_VAR) Nm_ChannelData_s[];
#define NM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Nm_MemMap.h"
#endif

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */


#if (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF)

#define NM_START_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"
extern  VAR(Nm_TimerType ,NM_VAR)  SwFRTimer;
#define NM_STOP_SEC_VAR_CLEARED_32
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_Int_NetworkRequest(boolean active, NetworkHandleType NetworkHandle , Nm_NetworkRequestSource source );
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_Int_NetworkRelease(NetworkHandleType NetworkHandle , Nm_NetworkRequestSource source);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_Reset(const NetworkHandleType ClusterIdx);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#endif /* NM_PRIV_H */

