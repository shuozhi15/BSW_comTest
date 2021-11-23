
#ifndef NM_CBK_H
#define NM_CBK_H

/**
\defgroup NM_CBK_H    Nm - Autosar Generic Network Management Interface.
*/

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface header file for declaration of callback notifications
 *
 * \scope               This is included by the <Bus>Nm modules
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Types header for common types */
#include "Nm.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Network Mode, this is called by BusNm  \n
 *  when it enters Network mode. This in turn notifies ComM.                            \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                        \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_NetworkMode(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Bus Sleep Mode. This is called by BusNm when it    \n
 *  enters Bus Sleep mode. This in turn notifies ComM about Bus Sleep mode.                         \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_BusSleepMode(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Prepare Bus Sleep Mode, this is not called by  \n
 *  FrNm or J1939Nm, this is called by CanNm when it enters Prepare Bus Sleep mode. This in turn notifies  \n
 *  ComM about Prepare Bus Sleep mode.                                                          \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_PrepareBusSleepMode(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
* @ingroup NM_CBK_H
*
*  This is the NM call-back interface for notification of Network Start,   \n
*  this function in turn notifies ComM.                                    \n
*
*  @param  In: NetworkHandle- Identification of the NM-channel.            \n
*
*  @return None \n
*/
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_NetworkStartIndication(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to indicate detection of Remote Sleep status,\n
 *  this is called by either CanNm or FrNm, it in turn notifies user application.   \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_RemoteSleepIndication(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to cancel detection of Remote Sleep status   \n
 *  this is called by either CanNm or FrNm, it in turn notifies user application.   \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_RemoteSleepCancellation(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/* Start of declarations for OEM Specific Extensions */
/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Transmission timeout. This is called by FrNm   \n
 *  when it detects timeout in getting Tx Confirmation.                                         \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_TxTimeoutException(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_NetworkTimeoutException(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface notifying Car Wake Up Indication.
 *  This is called by either CanNm or FrNm when CarWakeUp request is received in the NM-PDU    \n


 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_CarWakeUpIndication(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to notify that the NM has received a message.            \n
 *  This is called by either CanNm or FrNm. This interface is available depending on the        \n
 *  configuration parameter NM_PDU_RX_INDICATION_ENABLED. It in turn notifies the application.  \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_PduRxIndication(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is the NM call-back interface to notify state changes. This is called by CanNm  \n
 *  or FrNm or J1939Nm. It in turn notifies the application                                                \n
 *
 *  @param  In: NetworkHandle   - Identification of the NM-channel.                             \n
 *  @param  In: nmPreviousState - previous state of NM.                                         \n
 *  @param  In: nmCurrentState  - current state of NM.                                          \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_StateChangeNotification
(
    VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle,
    VAR(Nm_StateType, AUTOMATIC) nmPreviousState,
    VAR(Nm_StateType, AUTOMATIC) nmCurrentState
);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

/**
 * @ingroup NM_CBK_H
 *
 *  This is an NM call-back interface to notify that an NM msg is received with Repeat Message      \n
 *  Bit set. This is called by either CanNm or FrNm. This interface is available depending on the   \n
 *  configuration parameter NM_REPEAT_MESSAGE_IND_ENABLED. It in turn notifies the application.     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_RepeatMessageIndication(VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"


/**
 * @ingroup NM_CBK_H
 *
 * This is an NM call-back interface to notify the NM Coordinator functionality that this is a suitable     \n
 * point in time to initiate the coordinated shutdown on.This is called by Cyclic channel eg.FrNm.          \n
 * This interface is available depending on the configuration parameter NM_COORDINATOR_SUPPORT_ENABLED.     \n
 *
 *  @param  In: NetworkHandle- Identification of the NM-channel.                                    \n
 *
 *  @return None \n
 */
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
extern FUNC(void, NM_CODE) Nm_SynchronizationPoint(VAR(NetworkHandleType, AUTOMATIC) nmNetworkHandle);
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
/* End of declarations for OEM Specific Extensions */

#endif /* NM_CBK_H */

