
#ifndef NM_H
#define NM_H

/**
\defgroup NM_H    Nm - Autosar Generic Network Management Interface.
*/

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface main header file
 *
 * \scope               This is included by all the applications controlling NM
 ***************************************************************************************************
 */


/* Development Error Codes for DET Support*/
/**
 * @ingroup NM_H
 *
 * Macro to indicate unitialization of Nm.
 */
#define NM_E_UNINIT                0x00

/**
 * @ingroup NM_H
 *
 * Macro to indicate Nm Network handle is not configured.
 */
#define NM_E_HANDLE_UNDEF           0x01

/**
 * @ingroup NM_H
 *
 * Macro to indicate NULL pointer is passed as an argument to a function.
 */
#define    NM_E_PARAM_POINTER       0x02

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Types header for common types */
#include "NmStack_Types.h"

#include "ComStack_Types.h"
/* NM Interface configuration header */
#include "Nm_Cfg.h"


#if (!defined(COMTYPE_AR_RELEASE_MAJOR_VERSION) || (COMTYPE_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMTYPE_AR_RELEASE_MINOR_VERSION) || (COMTYPE_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
/**
 * @ingroup NM_H
 *
 * This typedef is used to define datatype of Nm_TimerType.
 */

typedef uint32 Nm_TimerType;

/* type definition of the configuration structure containing pointers to configuration structures of
   CAN NM, FR NM , UDP NM and J1939 NM
 */
/**
 * @ingroup NM_H
 *
 * This is the configuration of Nm  \n
 * typedef struct                   \n
 * {                                \n
 *      Nm_TimerType        ShutdownDelay;      This variable consists the value of configured time, for which \n
 *                                              NmCoordinator can wait in Preparewaitsleep before entering into \n
 *                                              waitsleep. \n
 * #if (NM_STATE_REPORT_ENABLED != STD_OFF) \n
 *      Com_SignalIdType    NMSSignal;          This variable decides whether NMS report is enabled or not  \n
 * #endif \n
 *      Nm_BusType          BusNmType;          This variable stores the type of Bus. \n
 *      NetworkHandleType   ClusterIdx;         This variable Stores the index of the cluster. \n
 *      NetworkHandleType   comMHandle;         This variable stores the handle of comM channel. \n
 * } Nm_ConfigType;
 */
typedef struct
{
    Nm_TimerType        ShutdownDelay;
#if (NM_STATE_REPORT_ENABLED != STD_OFF)
    Com_SignalIdType    NMSSignal;
#endif
    Nm_BusType          BusNmType;
    Nm_ChannelType      NmChannelType;
    NetworkHandleType   ClusterIdx;
    NetworkHandleType   comMHandle;
    boolean             NmChannelSleepMaster;
    boolean             NmSynchronizingNetwork;
    boolean             NmNodeDetectionEnabled;
    boolean             NmNodeIdEnabled;
    boolean             NmRepeatMsgIndEnabled;

} Nm_ConfigType;


/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

/**
 * @ingroup NM_H
 *
 *  This is the main function of NM Coordinator \n
 *
 *  @Param None \n
 *  @return None \n
 */
#if NM_ECUC_RB_RTE_IN_USE != STD_ON
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_MainFunction(void);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif



/**
 * @ingroup NM_H
 *
 *  This function initializes the NM Interface. \n
 *
 * @Param ConfigPtr - Pointer to the selected configuration set. \n
 * The Configuration pointer ConfigPtr is currently not used and shall therefore be set NULL_PTR value. \n
 *
 * @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_Init(P2CONST(Nm_ConfigType , AUTOMATIC, NM_APPL_CONST) ConfigPtr);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for triggering passive start up of NM, this API calls  \n
 *  CanNm_PassiveStartUp or FrNm_PassiveStartUp or J1939Nm_PassiveStartUp based on the configuration.                     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error.                                                     \n
 *          E_NOT_OK         - Passive start of network management has failed.               \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_PassiveStartUp(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to request Network. This API calls either CanNm_NetworkRequest\n
 *  or FrNm_NetworkRequest based on the configuration.                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error.                                                     \n
 *          E_NOT_OK         - Request for bus communication has failed.                     \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_NetworkRequest(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to release Network, this API calls either CanNm_NetworkRelease\n
 *  or FrNm_NetworkRelease based on the configuration.                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return E_OK             - No error                                                      \n
 *          E_NOT_OK         - Release for bus communication has failed.                     \n
 *
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_NetworkRelease(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface to get the current state of NM,                           \n
 *  this API calls CanNm_GetState or FrNm_GetState or J1939Nm_GetState based on the configuration.       \n
 *
 *  @param  In:     NetworkHandle-  Identification of the NM-channel.                       \n
 *  @param  out:    nmStatePtr   -  Pointer to the location where the state of the network  \n
 *                                  management shall be copied.                             \n
 *  @param  out:    nmModePtr    -  Pointer to the location where the mode of the network   \n
 *                                  management shall be copied.                             \n
 *
 *  @return E_OK              -  No error.                                               \n
 *          E_NOT_OK          -  Getting of NM state has failed.                         \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetState
(
    VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
    P2VAR(Nm_StateType, AUTOMATIC, NM_APPL_DATA) nmStatePtr,
    P2VAR(Nm_ModeType, AUTOMATIC, NM_APPL_DATA) nmModePtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for setting the user data for NM messages transmitted next on \n
 *  the bus, this API calls either CanNm_SetUserData or FrNm_SetUserData based on the channel   \n
 *  handle passed.                                                                              \n
 *
 *  @param  In: NetworkHandle - Identification of the NM-channel.                               \n
 *  @param  In: nmUserDataPtr - Pointer where the user data for the next transmitted NM message \n
 *                              shall be copied from.                                           \n
 *
 *  @return E_OK           - No error.                                                       \n
 *          E_NOT_OK       - Setting of user data has failed.                                \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_SetUserData
(
    VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
    P2CONST(uint8, AUTOMATIC, NM_APPL_DATA) nmUserDataPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for getting the user data out of the most recently received   \n
 *  NM message, this API calls either CanNm_GetUserData or FrNm_GetUserData based on the        \n
 *  channel handle.                                                                             \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                           \n
 *  @param  out:    nmUserDataPtr - Pointer where user data out of the last successfully received   \n
 *                                  NM message shall be copied to.                              \n
 *
 *  @return E_OK               - No error.                                                   \n
 *          E_NOT_OK           - Getting of user data has failed                             \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetUserData
(
    VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
    P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmUserDataPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the AUTOSAR interface for getting the whole NM PDU out of the most recently received        \n
 *  NM message, this API calls either CanNm_GetPduData or FrNm_GetPduData based on the channel handle.  \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                                   \n
 *  @param  out:    nmPduData     - Pointer where whole Pdu data out of the most recently received NM   \n
 *                                  message shall be copied to.                                         \n
 *
 *  @return E_OK               - No error.                                                           \n
 *          E_NOT_OK           - Getting of PDU has failed.                                          \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetPduData
(
    VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
    P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmPduData
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This function is used to get the node identifier out of the last NM message received on the bus.    \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                                   \n
 *  @param  out:    nmNodeIdPtr   - Pointer where node identifier out of the last successfully          \n
 *                                  received NM message shall be copied to.                             \n
 *
 *  @return E_OK               - Getting of node identifier is successful.                           \n
 *          E_NOT_OK           - Getting of node identifier has failed.                              \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetNodeIdentifier
(
     VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
     P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmNodeIdPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This function is used to get the node identifier of the host ECU.                               \n
 *
 *  @param  In:     NetworkHandle - Identification of the NM-channel.                               \n
 *  @param  out:    nmNodeIdPtr   - Pointer where node identifier of host ECU shall be copied to.   \n
 *
 *  @return E_OK               - Getting of node identifier of the host ECU is successful.       \n
 *          E_NOT_OK           - Getting of node identifier of the host ECU has failed.          \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetLocalNodeIdentifier
(
    VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
    P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmNodeIdPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This AUTOSAR interface gets the status of remote sleep detection.                               \n
 *
 *  @param  In:     NetworkHandle       - Identification of the NM-channel.                         \n
 *  @param  out:    nmRemoteSleepIndPtr - Pointer to the location where the check result of remote sleep    \n
 *                                        indication shall be copied.                               \n
 *
 *  @return E_OK               - No error.                                                       \n
 *          E_NOT_OK           -Checking of remote sleep indication bits has failed.           \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_CheckRemoteSleepIndication
(
    VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
    P2VAR(boolean, AUTOMATIC, NM_APPL_DATA) nmRemoteSleepIndPtr
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This AUTOSAR interface causes Repeat Message Request to be transmitted next on the bus.         \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - Repeat Message Request has failed.                                \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_RepeatMessageRequest(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the interface to disable NM transmission.                                               \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - Disabling of NM PDU transmission ability has failed.              \n
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_DisableCommunication(VAR(NetworkHandleType, AUTOMATIC)  NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This is the interface to enable NM transmission.                                                \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                                 \n
 *
 *  @return E_OK             - No error.                                                         \n
 *          E_NOT_OK         - enabling of transmission has failed.
 *
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(Std_ReturnType, NM_CODE) Nm_EnableCommunication(VAR(NetworkHandleType, AUTOMATIC)  NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_H
 *
 *  This service returns the version information of this module.                                        \n
 *
 *  @param  out:    nmVerInfoPtr    - Pointer to where to store the version information of this module. \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, NM_APPL_CONST) nmVerInfoPtr);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"



#if(NM_COORDINATOR_SYNC_SUPPORT_ENABLED !=STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_CoordReadyToSleepIndication(VAR(NetworkHandleType, AUTOMATIC)   NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_CoordReadyToSleepCancellation(VAR(NetworkHandleType, AUTOMATIC)   NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

#endif /* NM_H */

