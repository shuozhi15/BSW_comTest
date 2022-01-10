 

/*<VersionHead>
 ***************************************************************************************************
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Nm/ AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 ***************************************************************************************************
 </VersionHead>*/

/**
 ***************************************************************************************************
 * \moduledescription
 *                      NM Interface configuration header file.
 *                      This file is generated by CUBEC
 *
 * \scope               This is included by the Nm.h file. So, all the parameters are exported.
 ***************************************************************************************************
 */

#ifndef NM_CFG_H
#define NM_CFG_H
/*
 ***************************************************************************************************
 * Version Information Defines
 ***************************************************************************************************
 */
/**
 * @ingroup NM_H
 *
 * NM module id(according to AUTOSAR BSW module list)
 */
#define NM_MODULE_ID                                                   29u
/**
 * @ingroup NM_H
 *
 * NM module id(according to AUTOSAR BSW module list)
 */
#define NM_VENDOR_ID                                                   6u

/* Version of AUTOSAR NM specification */
/**
 * @ingroup NM_H
 *
 * Major Version of AUTOSAR NM specification
 */
#define NM_AR_RELEASE_MAJOR_VERSION                                    4u
/**
 * @ingroup NM_H
 *
 * Minor Version of AUTOSAR NM specification
 */
#define NM_AR_RELEASE_MINOR_VERSION                                    2u
/**
 * @ingroup NM_H
 *
 * Revision Version of AUTOSAR NM specification
 */
#define NM_AR_RELEASE_REVISION_VERSION                                 2u

/* Version of AUTOSAR NM implementation */
/**
 * @ingroup NM_H
 *
 * Major Version of AUTOSAR NM implementation
 */
#define NM_SW_MAJOR_VERSION                                            6u
/**
 * @ingroup NM_H
 *
 * Minor Version of AUTOSAR NM implementation
 */
#define NM_SW_MINOR_VERSION                                            0u
/**
 * @ingroup NM_H
 *
 * Patch Version of AUTOSAR NM implementation
 */
#define NM_SW_PATCH_VERSION                                            0u



/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm.h"
#if (!defined(CANNM_AR_RELEASE_MAJOR_VERSION) || (CANNM_AR_RELEASE_MAJOR_VERSION != NM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANNM_AR_RELEASE_MINOR_VERSION) || (CANNM_AR_RELEASE_MINOR_VERSION != NM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/* Pre-processor switches for selecting Optional Features */
#define NM_VERSION_INFO_API                 (STD_ON)          /* Enable support of Version Info API */
#define NM_DEV_ERROR_DETECT                 (STD_OFF)

#define NM_COORDINATOR_SUPPORT_ENABLED      (STD_OFF)
                                            
#define NM_COORDINATOR_SYNC_SUPPORT_ENABLED (STD_OFF) /*Enable coordinator sync support*/

#define NM_MAIN_FUNCTION_CYCLETIME          (0)
#define NM_BUS_SYNCHRONIZATION_ENABLED      (STD_OFF)          /* Enable support of Bus Synchronization */
                                                                                             /* applicable only in CAN NM */

#define NM_COM_CONTROL_ENABLED              (STD_ON)          /* Enable support of Communication Control service of i14229 */
#define NM_CAR_WAKEUP_RX_ENABLED            (STD_OFF)         /* Enable support of CarWakeUp Indication */
#define NM_CAR_WAKEUP_CALLOUT_ENABLED       (STD_OFF)    /* CarWakeUp Callout Function is configured */


#define NM_NODE_ID_ENABLED                  (STD_ON)          /* Enable support of source Node Identifier */
#define NM_NODE_DETECTION_ENABLED           (STD_ON)          /* Enable support of Node Detection */

#define NM_PASSIVE_MODE_ENABLED             (STD_OFF)          /* Enable support of Passive Nodes */

#define NM_PDU_RX_INDICATION_ENABLED        (STD_OFF)           /* Enable support of indication of received PDUs */

#define NM_REMOTE_SLEEP_IND_ENABLED         (STD_OFF)            /* Enable support of Remote Sleep Indication Feature            */

#define NM_STATE_CHANGE_IND_ENABLED         (STD_ON)           /* Enable support of indication of state changes             */

#define NM_STATE_REPORT_ENABLED                (STD_OFF)           /* Enable support of state report */

#define NM_USER_DATA_ENABLED                (STD_ON)          /* Enable support of User Data */

#define NM_COM_USER_DATA_SUPPORT            (STD_OFF)          /* Enable support of User Data via Com */


#define NM_REPEAT_MESSAGE_IND_ENABLED       (STD_ON)          /* Enable support of Repeat Message Indication Notification */

#define NM_BUSNM_CANNM_ENABLED              (STD_ON)
#define NM_BUSNM_J1939NM_ENABLED            (STD_OFF)
#define NM_BUSNM_FRNM_ENABLED               (STD_OFF)
#define NM_BUSNM_UDPNM_ENABLED              (STD_OFF)

#define NM_NUMBER_OF_CHANNELS               (1)
#define NM_MAX_CANNM_CHANNELS               (1)
#define NM_MAX_J1939NM_CHANNELS             (0)
#define NM_MAX_FRNM_CHANNELS                (0)
#define NM_MAX_UDPNM_CHANNELS               (0)


#define NM_LINNM_SUPPORT                   (STD_OFF)  /* Enable support of LinNm */

#define NM_NUMBER_OF_CLUSTERS               (0)
                                                                                   
/* Macro for information regarding RTE used in the project or not */

#define NM_ECUC_RB_RTE_IN_USE  STD_ON 



/* Size of the Mapping Table based on the maximum value of NmChannelId */
#define NM_MAPPING_TABLE_SIZE            (1)

//Defines for Nm channel handles
#define        NmConf_NmChannelConfig_NmChannelConfig_Can_Network_Channel    0

/* Macro which receives ComM HandleId and returns the matching Nm HandleId */
#define NM_GET_HANDLE(NetworkHandle)     (NetworkHandle) 

/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
 typedef enum
 {
    NM_BUS_TYPE_CAN = 0
 }Nm_BusType;

/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */
/* Declarations of upper layer callback functions that are called by NM */
/* [please note that these shall be located in the region defined by NM_CODE] */
extern FUNC(void, NM_CODE) TestNm_RemoteSleepIndication(
                                                                CONST(NetworkHandleType, AUTOMATIC)   NetworkHandle
                                                                    );

extern FUNC(void, NM_CODE) TestNm_RemoteSleepCancellation(
                                                                CONST(NetworkHandleType, AUTOMATIC)   NetworkHandle
                                                                    );


#if (NM_CAR_WAKEUP_RX_ENABLED != STD_OFF)
#if (NM_CAR_WAKEUP_CALLOUT_ENABLED != STD_OFF)
#else
#include "BswM_Nm.h"
#endif
#endif
#if (NM_CAR_WAKEUP_CALLOUT_ENABLED != STD_OFF )
#define NM_USER_CARWAKEUPINDICATION 
#endif






/*
 ******************************************************************************
 * Defines
 ******************************************************************************
 */

#endif  /*NM_CFG_H*/

